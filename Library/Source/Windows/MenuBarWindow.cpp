#include "MenuBarWindow.h"

using namespace VeryGUI;

MenuBarWindow::MenuBarWindow(const std::string& menuName, std::shared_ptr<MenuDriver> menuDriver)
{
	this->menuName = menuName;
	this->menuDriver = menuDriver;
}

/*virtual*/ MenuBarWindow::~MenuBarWindow()
{
}

/*virtual*/ void MenuBarWindow::LayoutChildren(GAL2D::GraphicsInterface* graphics)
{
	Window::LayoutChildren(graphics);
}

/*virtual*/ void MenuBarWindow::Draw(GAL2D::GraphicsInterface* graphics)
{
	graphics->RenderRectangle(this->boundingRect, GAL2D::Color(0.5, 0.5, 0.5, 1.0));

	Window::Draw(graphics);
}

/*virtual*/ void MenuBarWindow::HandleEvent(EventType eventType, const void* eventData)
{
}

/*virtual*/ bool MenuBarWindow::CanExceedParentBounds() const
{
	return true;
}

/*virtual*/ double MenuBarWindow::GetDesiredHeight()
{
	return 20.0;
}