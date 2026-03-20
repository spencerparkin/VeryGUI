#include "Window.h"

using namespace VeryGUI;

Window::Window()
{
}

/*virtual*/ Window::~Window()
{
}

/*virtual*/ void Window::Layout()
{
}

/*virtual*/ void Window::Draw(GAL2D::GraphicsInterface* graphics)
{
	for (const std::shared_ptr<Window> childWindow : this->childWindowArray)
		childWindow->Draw(graphics);
}