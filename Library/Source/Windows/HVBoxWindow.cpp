#include "HVBoxWindow.h"

using namespace VeryGUI;

//----------------------------------------- HVBoxWindow -----------------------------------------

HVBoxWindow::HVBoxWindow()
{
}

/*virtual*/ HVBoxWindow::~HVBoxWindow()
{
}

bool HVBoxWindow::AddChildWindowWithSizingConfig(std::shared_ptr<Window> window, SizingConfig sizingConfig)
{
	if (this->childWindowArray.size() != this->sizingConfigArray.size())
		return false;

	if (!this->AddChildWindow(window))
		return false;

	this->sizingConfigArray.push_back(sizingConfig);
	return true;
}

//----------------------------------------- HBoxWindow -----------------------------------------

HBoxWindow::HBoxWindow()
{
}

/*virtual*/ HBoxWindow::~HBoxWindow()
{
}

/*virtual*/ void HBoxWindow::LayoutChildren(GAL2D::GraphicsInterface* graphics)
{
	// STPTODO: Write this.

	Window::LayoutChildren(graphics);
}

/*virtual*/ double HBoxWindow::GetChildWindowDesiredLength(int i)
{
	return this->childWindowArray[i]->GetDesiredWidth();
}

/*virtual*/ double HBoxWindow::GetTotalLengthAvailable()
{
	return this->boundingRect.Width();
}

//----------------------------------------- VBoxWindow -----------------------------------------

VBoxWindow::VBoxWindow()
{
}

/*virtual*/ VBoxWindow::~VBoxWindow()
{
}

/*virtual*/ void VBoxWindow::LayoutChildren(GAL2D::GraphicsInterface* graphics)
{
	// STPTODO: Write this.

	Window::LayoutChildren(graphics);
}

/*virtual*/ double VBoxWindow::GetChildWindowDesiredLength(int i)
{
	return this->childWindowArray[i]->GetDesiredHeight();
}

/*virtual*/ double VBoxWindow::GetTotalLengthAvailable()
{
	return this->boundingRect.Height();
}