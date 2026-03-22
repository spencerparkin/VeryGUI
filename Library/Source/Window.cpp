#include "Window.h"

using namespace VeryGUI;

Window::Window()
{
}

/*virtual*/ Window::~Window()
{
}

/*virtual*/ void Window::LayoutChildren()
{
	for (const std::shared_ptr<Window> childWindow : this->childWindowArray)
		childWindow->LayoutChildren();
}

/*virtual*/ void Window::Draw(GAL2D::GraphicsInterface* graphics, GAL2D::Font* commonFont)
{
	for (const std::shared_ptr<Window> childWindow : this->childWindowArray)
		childWindow->Draw(graphics, commonFont);
}

bool Window::AddChildWindow(std::shared_ptr<Window> childWindow)
{
	if (this->HasChildWindow(childWindow))
		return false;

	this->childWindowArray.push_back(childWindow);
	childWindow->parentWindowWeakPtr = this->shared_from_this();
	return true;
}

bool Window::RemoveChildWindow(std::shared_ptr<Window> childWindow)
{
	int i = -1;
	if (!this->HasChildWindow(childWindow, &i))
		return false;

	this->childWindowArray.erase(this->childWindowArray.begin() + i);
	childWindow->parentWindowWeakPtr.reset();
	return true;
}

bool Window::HasChildWindow(std::shared_ptr<Window> childWindow, int* offset /*= nullptr*/) const
{
	if (offset)
		*offset = -1;

	for (int i = 0; i < (int)this->childWindowArray.size(); i++)
	{
		if (this->childWindowArray[i] == childWindow)
		{
			if (offset)
				*offset = i;

			return true;
		}
	}

	return false;
}

void Window::SetBoundingRectangle(const GAL2D::Rectangle& boundingRect)
{
	this->boundingRect = boundingRect;
}

const GAL2D::Rectangle& Window::GetBoundingRect() const
{
	return this->boundingRect;
}