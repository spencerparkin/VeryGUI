#include "MenuBarWindow.h"
#include "Graphics/Font.h"
#include <assert.h>

using namespace VeryGUI;

double MenuBarWindow::labelHeight = 20.0;
double MenuBarWindow::margin = 4.0;

MenuBarWindow::MenuBarWindow(const std::string& menuName, std::shared_ptr<MenuDriver> menuDriver)
{
	this->menuName = menuName;
	this->menuDriver = menuDriver;
	this->popupMenus = false;
}

/*virtual*/ MenuBarWindow::~MenuBarWindow()
{
}

/*virtual*/ void MenuBarWindow::LayoutChildren(GAL2D::GraphicsInterface* graphics)
{
	if (!this->labelFont.get())
		this->labelFont = graphics->MakeFont("Fonts\\Roboto_Regular.ttf");

	int numMenuItems = this->menuDriver->GetNumMenuItems(this->menuName);
	assert(numMenuItems > 0);

	GAL2D::Vector position = this->boundingRect.GetCorner(GAL2D::Rectangle::Corner::UPPER_LEFT);

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
			this->labelFont->CalcTextWidth(label, this->labelHeight, menuItemCache->labelWidth);
			this->menuItemCacheMap.insert(std::pair(label, menuItemCache));
		}

		menuItemCache->labelRect = GAL2D::Rectangle(menuItemCache->labelWidth, this->labelHeight);
		menuItemCache->itemRect = GAL2D::Rectangle(menuItemCache->labelWidth + 2.0 * this->margin, this->boundingRect.Height());
		menuItemCache->labelRect.PlaceInRectangle(menuItemCache->itemRect, GAL2D::Rectangle::Alignment::CENTER, GAL2D::Rectangle::Alignment::CENTER);

		GAL2D::Vector delta = menuItemCache->itemRect.GetTranslation(position, GAL2D::Rectangle::Corner::UPPER_LEFT);
		menuItemCache->itemRect += delta;
		menuItemCache->labelRect += delta;

		position.x += menuItemCache->itemRect.Width();
	}

	Window::LayoutChildren(graphics);
}

/*virtual*/ Window::DrawOrder MenuBarWindow::Draw(GAL2D::GraphicsInterface* graphics)
{
	graphics->RenderRectangle(this->boundingRect, GAL2D::Color(0.5, 0.5, 0.5, 1.0));

	for (auto pair : this->menuItemCacheMap)
	{
		const std::string& label = pair.first;
		std::shared_ptr<MenuItemCache> menuItemCache = pair.second;

		if (menuItemCache->isHighlighted)
			graphics->RenderRectangle(menuItemCache->itemRect, GAL2D::Color(0.8, 0.8, 0.8, 1.0));

		graphics->RenderText(label, this->labelFont, menuItemCache->labelRect, GAL2D::Color(0.0, 0.0, 0.0, 1.0), GAL2D::GraphicsInterface::ALIGN_LEFT);
	}

	return DrawOrder::DEPTH_LAST;
}

/*virtual*/ void MenuBarWindow::HandleEvent(EventType eventType, const void* eventData)
{
	switch (eventType)
	{
		case EventType::MOUSE_MOTION:
		{
			auto event = static_cast<const MouseMotionEvent*>(eventData);

			this->ManagePopupMenus(event->mousePosition);

			break;
		}
		case EventType::MOUSE_CLICK:
		{
			auto event = static_cast<const MouseClickEvent*>(eventData);

			if (event->mouseButton == GAL2D::MouseButton::Left && event->buttonState == GAL2D::ButtonState::Down)
			{
				this->popupMenus = !this->popupMenus;

				this->ManagePopupMenus(event->mousePosition);
			}

			break;
		}
	}
}

void MenuBarWindow::ManagePopupMenus(const GAL2D::Vector& mousePosition)
{
	for (auto pair : this->menuItemCacheMap)
	{
		std::shared_ptr<MenuItemCache> menuItemCache = pair.second;
		menuItemCache->isHighlighted = menuItemCache->itemRect.ContainsPoint(mousePosition);

		if (this->popupMenus && menuItemCache->isHighlighted)
		{
			if (menuItemCache->subMenuWindow.expired())
			{
				std::string subMenuName;
				if (this->menuDriver->GetMenuItemSubMenuName(this->menuName, menuItemCache->i, subMenuName))
				{
					std::shared_ptr<MenuDriver> subMenuDriver = this->menuDriver->GetSubMenuDriver(subMenuName);
					assert(subMenuDriver.get());
					std::shared_ptr<MenuWindow> subMenuWindow = std::make_shared<MenuWindow>(subMenuName, subMenuDriver);
					GAL2D::Vector anchorPoint = menuItemCache->itemRect.GetCorner(GAL2D::Rectangle::Corner::LOWER_LEFT);
					subMenuWindow->SetAnchor(anchorPoint, MenuWindow::AnchorPlacement::UPPER_LEFT);
					this->AddChildWindow(subMenuWindow);
					menuItemCache->subMenuWindow = subMenuWindow;
				}
			}
		}
		else if (!this->popupMenus || !menuItemCache->isHighlighted)
		{
			std::shared_ptr<Window> subMenuWindow = menuItemCache->subMenuWindow.lock();
			if (subMenuWindow.get())
				this->RemoveChildWindow(subMenuWindow);
		}
	}
}

/*virtual*/ bool MenuBarWindow::CanExceedParentBounds() const
{
	return true;
}

/*virtual*/ double MenuBarWindow::GetDesiredHeight()
{
	return 30.0;
}

void MenuBarWindow::DisableAutoPopup()
{
	this->popupMenus = false;
}