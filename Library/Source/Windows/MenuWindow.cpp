#include "MenuWindow.h"
#include "MenuBarWindow.h"
#include "Graphics/Font.h"
#include <assert.h>
#include <unordered_set>

using namespace VeryGUI;

//----------------------------------- MenuDriver -----------------------------------

MenuDriver::MenuDriver()
{
}

/*virtual*/ MenuDriver::~MenuDriver()
{
}

/*virtual*/ bool MenuDriver::IsMenuItemEnabled(const std::string& menuName, int i)
{
	return true;
}

/*virtual*/ bool MenuDriver::GetMenuItemIconPath(const std::string& menuName, int i, std::filesystem::path& iconPath)
{
	return false;
}

/*virtual*/ bool MenuDriver::GetMenuItemSubMenuName(const std::string& menuName, int i, std::string& subMenuName)
{
	return false;
}

/*virtual*/ std::shared_ptr<MenuDriver> MenuDriver::GetSubMenuDriver(const std::string& subMenuName)
{
	return this->shared_from_this();
}

/*virtual*/ void MenuDriver::HandleMenuItemClick(const std::string& menuName, int i)
{
}

//----------------------------------- MenuWindow -----------------------------------

double MenuWindow::labelHeight = 20.0;
double MenuWindow::margin = 4.0;

MenuWindow::MenuWindow(const std::string& menuName, std::shared_ptr<MenuDriver> menuDriver)
{
	this->menuName = menuName;
	this->menuDriver = menuDriver;
	this->anchorPlacement = AnchorPlacement::UPPER_LEFT;
}

/*virtual*/ MenuWindow::~MenuWindow()
{
}

void MenuWindow::SetAnchor(const GAL2D::Vector& anchorPoint, AnchorPlacement anchorPlacement)
{
	this->anchorPoint = anchorPoint;
	this->anchorPlacement = anchorPlacement;
}

/*virtual*/ void MenuWindow::LayoutChildren(GAL2D::GraphicsInterface* graphics)
{
	if (!this->labelFont.get())
		this->labelFont = graphics->MakeFont("Fonts\\Roboto_Regular.ttf");

	int numMenuItems = this->menuDriver->GetNumMenuItems(this->menuName);
	assert(numMenuItems > 0);

	double maxLabelWidth = 0.0;
	std::vector<std::string> labelArray;

	for (int i = 0; i < numMenuItems; i++)
	{
		std::string label;
		this->menuDriver->GetMenuItemLabel(this->menuName, i, label);

		labelArray.push_back(label);

		std::shared_ptr<MenuItemCache> menuItemCache;
		auto iter = this->menuItemCacheMap.find(label);
		if (iter != this->menuItemCacheMap.end())
			menuItemCache = iter->second;
		else
		{
			menuItemCache = std::make_shared<MenuItemCache>();
			menuItemCache->i = i;
			menuItemCache->isHighlighted = false;
			this->labelFont->CalcTextWidth(label, this->labelHeight, menuItemCache->labelWidth);
			this->menuItemCacheMap.insert(std::pair(label, menuItemCache));
		}

		if (maxLabelWidth < menuItemCache->labelWidth)
			maxLabelWidth = menuItemCache->labelWidth;

		std::filesystem::path iconPath;
		if (!this->menuDriver->GetMenuItemIconPath(this->menuName, i, iconPath))
			menuItemCache->iconTexture.reset();
		else
		{
			if (iconPath != menuItemCache->iconPath)
			{
				menuItemCache->iconPath = iconPath;
				menuItemCache->iconTexture = graphics->MakeTexture(iconPath);
			}
		}
	}

	GAL2D::Rectangle localIconRect;
	localIconRect.minCorner = GAL2D::Vector(0.0, 0.0);
	localIconRect.maxCorner = GAL2D::Vector(this->labelHeight, this->labelHeight);

	GAL2D::Rectangle localLabelRect;
	localLabelRect.minCorner = GAL2D::Vector(this->labelHeight + this->margin, 0.0);
	localLabelRect.maxCorner = GAL2D::Vector(this->labelHeight + this->margin + maxLabelWidth, this->labelHeight);

	GAL2D::Rectangle localItemRect;
	localItemRect.minCorner = GAL2D::Vector(0.0, 0.0);
	localItemRect.maxCorner = GAL2D::Vector(this->labelHeight + this->margin + maxLabelWidth, this->labelHeight);
	localItemRect.ApplyMarginDelta(this->margin);

	GAL2D::Vector location = this->anchorPoint;

	if (this->anchorPlacement == AnchorPlacement::UPPER_RIGHT || this->anchorPlacement == AnchorPlacement::LOWER_RIGHT)
		location.x -= localItemRect.Width();

	if (this->anchorPlacement == AnchorPlacement::LOWER_LEFT || this->anchorPlacement == AnchorPlacement::LOWER_RIGHT)
		location.y += localItemRect.Height() * double(numMenuItems);

	for (int i = 0; i < (int)labelArray.size(); i++)
	{
		const std::string& label = labelArray[i];
		auto pair = this->menuItemCacheMap.find(label);
		std::shared_ptr<MenuItemCache> menuItemCache = pair->second;

		GAL2D::Vector delta = localItemRect.GetTranslation(location, GAL2D::Rectangle::Corner::UPPER_LEFT);

		menuItemCache->itemRect = localItemRect + delta;
		menuItemCache->labelRect = localLabelRect + delta;
		menuItemCache->iconRect = localIconRect + delta;

		location.y -= localItemRect.Height();
	}

	this->boundingRect.PrepareForExpansion();
	for (auto pair : this->menuItemCacheMap)
	{
		std::shared_ptr<MenuItemCache> menuItemCache = pair.second;
		this->boundingRect.MinimallyExpandToIncludeRect(menuItemCache->itemRect);
	}

	Window::LayoutChildren(graphics);
}

/*virtual*/ Window::DrawOrder MenuWindow::Draw(GAL2D::GraphicsInterface* graphics)
{
	GAL2D::Rectangle shadowRect = this->boundingRect + GAL2D::Vector(4.0, -4.0);
	graphics->RenderRectangle(shadowRect, GAL2D::Color(0.0, 0.0, 0.0, 0.3));

	graphics->RenderRectangle(this->boundingRect, GAL2D::Color(0.5, 0.5, 0.5, 1.0));

	for (auto pair : this->menuItemCacheMap)
	{
		const std::string& label = pair.first;
		std::shared_ptr<MenuItemCache> menuItemCache = pair.second;

		if (menuItemCache->isHighlighted)
			graphics->RenderRectangle(menuItemCache->itemRect, GAL2D::Color(0.8, 0.8, 0.8, 1.0));

		if (menuItemCache->iconTexture.get())
			graphics->RenderRectangle(menuItemCache->iconRect, GAL2D::Color(1.0, 1.0, 1.0, 1.0), menuItemCache->iconTexture);

		graphics->RenderText(label, this->labelFont, menuItemCache->labelRect, GAL2D::Color(0.0, 0.0, 0.0, 1.0), GAL2D::GraphicsInterface::ALIGN_LEFT);
	}

	return DrawOrder::DEPTH_LAST;
}

/*virtual*/ bool MenuWindow::CanExceedParentBounds() const
{
	return true;
}

/*virtual*/ void MenuWindow::HandleEvent(EventType eventType, const void* eventData)
{
	switch (eventType)
	{
		case EventType::MOUSE_MOTION:
		{
			auto event = static_cast<const MouseMotionEvent*>(eventData);
			
			for (auto pair : this->menuItemCacheMap)
			{
				std::shared_ptr<MenuItemCache> menuItemCache = pair.second;
				menuItemCache->isHighlighted = menuItemCache->itemRect.ContainsPoint(event->mousePosition);
				if (menuItemCache->isHighlighted)
				{
					std::string subMenuName;
					if (this->menuDriver->GetMenuItemSubMenuName(this->menuName, menuItemCache->i, subMenuName))
					{
						if (menuItemCache->subMenuWindow.expired())
						{
							std::shared_ptr<MenuDriver> subMenuDriver = this->menuDriver->GetSubMenuDriver(subMenuName);
							assert(subMenuDriver.get());
							std::shared_ptr<MenuWindow> subMenuWindow = std::make_shared<MenuWindow>(subMenuName, subMenuDriver);
							GAL2D::Vector anchorPoint = menuItemCache->labelRect.maxCorner;
							anchorPoint.x += 4.0;
							subMenuWindow->SetAnchor(anchorPoint, AnchorPlacement::UPPER_LEFT);
							this->AddChildWindow(subMenuWindow);
							menuItemCache->subMenuWindow = subMenuWindow;
						}
					}
				}
				else
				{
					std::shared_ptr<MenuWindow> subMenuWindow = menuItemCache->subMenuWindow.lock();
					if (subMenuWindow)
						this->RemoveChildWindow(subMenuWindow);
				}
			}

			break;
		}
		case EventType::MOUSE_CLICK:
		{
			auto event = static_cast<const MouseClickEvent*>(eventData);

			if (event->mouseButton == GAL2D::MouseButton::Left && event->buttonState == GAL2D::ButtonState::Down)
			{
				for (auto pair : this->menuItemCacheMap)
				{
					std::shared_ptr<MenuItemCache> menuItemCache = pair.second;
					if (menuItemCache->itemRect.ContainsPoint(event->mousePosition))
					{
						const std::string& label = pair.first;
						
						this->menuDriver->HandleMenuItemClick(this->menuName, menuItemCache->i);

						std::shared_ptr<Window> parentWindow = this->GetParentWindow();
						std::shared_ptr<Window> childWindow = this->shared_from_this();
						while (dynamic_cast<MenuWindow*>(parentWindow.get()) != nullptr)
						{
							childWindow = parentWindow;
							parentWindow = parentWindow->GetParentWindow();
						}
						parentWindow->RemoveChildWindow(childWindow);

						auto menuBarWindow = dynamic_cast<MenuBarWindow*>(parentWindow.get());
						if (menuBarWindow)
							menuBarWindow->DisableAutoPopup();

						break;
					}
				}
			}

			break;
		}
	}
}