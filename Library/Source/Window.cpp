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

/*virtual*/ void Window::Draw(GAL2D::GraphicsInterface* graphics)
{
	for (const std::shared_ptr<Window> childWindow : this->childWindowArray)
		childWindow->Draw(graphics);
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

std::shared_ptr<Window> Window::FindDeepestWindowContainingPoint(const GAL2D::Vector& point)
{
	if (!this->boundingRect.ContainsPoint(point))
		return nullptr;
	
	Window* window = this;
	while (window)
	{
		Window* subWindow = nullptr;

		for (int i = (int)window->childWindowArray.size() - 1; i >= 0; i--)
		{
			std::shared_ptr<Window> childWindow = window->childWindowArray[i];

			if (childWindow->boundingRect.ContainsPoint(point))
			{
				subWindow = childWindow.get();
				break;
			}
		}

		if (!subWindow)
			break;

		window = subWindow;
	}

	return window->shared_from_this();
}

std::shared_ptr<Window> Window::GetParentWindow()
{
	return this->parentWindowWeakPtr.lock();
}

std::shared_ptr<Window> Window::GetRootWindow()
{
	std::shared_ptr<Window> window = this->shared_from_this();
	while (true)
	{
		std::shared_ptr<Window> parentWindow = window->GetParentWindow();
		if (!parentWindow.get())
			break;

		window = parentWindow;
	}

	return window;
}

/*virtual*/ void Window::HandleEvent(EventType eventType, const void* eventData)
{
	std::shared_ptr<Window> parentWindow = this->GetParentWindow();
	if (parentWindow.get())
		parentWindow->HandleEvent(eventType, eventData);
}