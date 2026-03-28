#include "MenuWindow.h"
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

/*virtual*/ bool MenuDriver::GetMenuItemIconPath(const std::string& menuName, std::filesystem::path& iconPath)
{
	return false;
}

/*virtual*/ bool MenuDriver::GetMenuItemSubMenuName(const std::string& menuName, int i, std::string& subMenuName)
{
	return false;
}

/*virtual*/ std::shared_ptr<MenuDriver> MenuDriver::GetSubMenuDriver(const std::string& subMenuName)
{
	return nullptr;
}

/*virtual*/ void MenuDriver::HandleMenuItemClick(const std::string& menuName, int i)
{
}

//----------------------------------- MenuWindow -----------------------------------

double MenuWindow::menuItemHeight = 20.0;

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

	double menuWidth = 0.0;

	for (int i = 0; i < numMenuItems; i++)
	{
		std::string label;
		this->menuDriver->GetMenuItemLabel(this->menuName, i, label);

		std::shared_ptr<MenuItemCache> menuItemCache;
		auto iter = this->menuItemCacheMap.find(label);
		if (iter != this->menuItemCacheMap.end())
			menuItemCache = iter->second;
		else
		{
			menuItemCache = std::make_shared<MenuItemCache>();
			menuItemCache->i = i;
			menuItemCache->isHighlighted = false;
			this->labelFont->CalcTextWidth(label, this->menuItemHeight, menuItemCache->labelWidth);
			this->menuItemCacheMap.insert(std::pair(label, menuItemCache));
		}

		if (menuWidth < menuItemCache->labelWidth)
			menuWidth = menuItemCache->labelWidth;
	}

	double menuHeight = double(numMenuItems) * this->menuItemHeight;

	switch (this->anchorPlacement)
	{
		case AnchorPlacement::LOWER_LEFT:
		{
			this->boundingRect.minCorner = this->anchorPoint;
			this->boundingRect.maxCorner = this->boundingRect.minCorner + GAL2D::Vector(menuWidth, menuHeight);
			break;
		}
		case AnchorPlacement::LOWER_RIGHT:
		{
			this->boundingRect.minCorner.x = this->anchorPoint.x - menuWidth;
			this->boundingRect.minCorner.y = this->anchorPoint.y;
			this->boundingRect.maxCorner.x = this->anchorPoint.x;
			this->boundingRect.maxCorner.y = this->anchorPoint.y + menuHeight;
			break;
		}
		case AnchorPlacement::UPPER_LEFT:
		{
			this->boundingRect.minCorner.x = this->anchorPoint.x;
			this->boundingRect.minCorner.y = this->anchorPoint.y - menuHeight;
			this->boundingRect.maxCorner.x = this->anchorPoint.x + menuWidth;
			this->boundingRect.maxCorner.y = this->anchorPoint.y;
			break;
		}
		case AnchorPlacement::UPPER_RIGHT:
		{
			this->boundingRect.minCorner = this->anchorPoint - GAL2D::Vector(menuWidth, menuHeight);
			this->boundingRect.maxCorner = this->anchorPoint;
			break;
		}
	}

	for (auto pair : this->menuItemCacheMap)
	{
		const std::string& label = pair.first;
		std::shared_ptr<MenuItemCache> menuItemCache = pair.second;

		int j = (int)numMenuItems - 1 - menuItemCache->i;

		menuItemCache->labelRect.minCorner.x = this->boundingRect.minCorner.x;
		menuItemCache->labelRect.maxCorner.x = this->boundingRect.maxCorner.x;
		menuItemCache->labelRect.minCorner.y = this->boundingRect.minCorner.y + double(j) * menuItemHeight;
		menuItemCache->labelRect.maxCorner.y = menuItemCache->labelRect.minCorner.y + menuItemHeight;
	}

	this->boundingRect.ApplyMarginDelta(4.0);
}

/*virtual*/ void MenuWindow::Draw(GAL2D::GraphicsInterface* graphics)
{
	graphics->RenderRectangle(this->boundingRect, GAL2D::Color(0.5, 0.5, 0.5, 1.0));

	for (auto pair : this->menuItemCacheMap)
	{
		const std::string& label = pair.first;
		std::shared_ptr<MenuItemCache> menuItemCache = pair.second;

		if (menuItemCache->isHighlighted)
			graphics->RenderRectangle(menuItemCache->labelRect, GAL2D::Color(0.8, 0.8, 0.8, 1.0));

		graphics->RenderText(label, this->labelFont, menuItemCache->labelRect, GAL2D::Color(0.0, 0.0, 0.0, 1.0), GAL2D::GraphicsInterface::ALIGN_LEFT);
	}
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
				menuItemCache->isHighlighted = menuItemCache->labelRect.ContainsPoint(event->mousePosition);
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
					if (menuItemCache->labelRect.ContainsPoint(event->mousePosition))
					{
						const std::string& label = pair.first;
						this->menuDriver->HandleMenuItemClick(this->menuName, menuItemCache->i);
						std::shared_ptr<Window> parentWindow = this->GetParentWindow();
						parentWindow->RemoveChildWindow(this->shared_from_this());
						break;
					}
				}
			}

			break;
		}
	}
}