#include "Windows/DesktopWindow.h"
#include "Graphics/Texture.h"

using namespace VeryGUI;

DesktopWindow::DesktopWindow(const GAL2D::Vector& size, const std::filesystem::path& resourceBasePath, const std::filesystem::path& backgroundImagePath)
{
	this->size = size;
	this->resourceBasePath = resourceBasePath;
	this->backgroundImagePath = backgroundImagePath;
}

/*virtual*/ DesktopWindow::~DesktopWindow()
{
}

bool DesktopWindow::Run()
{
	GAL2D::DriverInitData driverInitData{};
	driverInitData.windowTitle = "Very GUI";
	driverInitData.windowWidth = (int)this->size.x;
	driverInitData.windowHeight = (int)this->size.y;

	this->graphicsInterface = GAL2D::CreateGraphicsInterface(&driverInitData);

	if (!this->graphicsInterface->Setup())
		return false;

	this->graphicsInterface->SetResourceBasePath(this->resourceBasePath);

	if (!this->backgroundImagePath.empty())
	{
		this->backgroundTexture = this->graphicsInterface->MakeTexture(this->backgroundImagePath);
		if (!this->backgroundTexture.get())
			return false;
	}

	this->graphicsInterface->mouseClickEventHandler = [this](const GAL2D::Vector& mousePosition, GAL2D::MouseButton mouseButton, GAL2D::ButtonState buttonState) -> void
		{
			MouseClickEvent event;
			event.mousePosition = mousePosition;
			event.mouseButton = mouseButton;
			event.buttonState = buttonState;

			std::shared_ptr<Window> mouseCaptureWindow = this->mouseCaptureWindowWeakPtr.lock();
			if (mouseCaptureWindow.get())
				mouseCaptureWindow->HandleEvent(EventType::MOUSE_CLICK, &event);
			else
			{
				std::shared_ptr<Window> window = this->FindDeepestWindowContainingPoint(mousePosition);
				if (window.get())
					window->HandleEvent(EventType::MOUSE_CLICK, &event);
			}
		};

	this->graphicsInterface->mouseMotionEventHandler = [this](const GAL2D::Vector& mousePosition) -> void
		{
			MouseMotionEvent event;
			event.mousePosition = mousePosition;

			std::shared_ptr<Window> mouseCaptureWindow = this->mouseCaptureWindowWeakPtr.lock();
			if (mouseCaptureWindow.get())
				mouseCaptureWindow->HandleEvent(EventType::MOUSE_MOTION, &event);
			else
			{
				std::shared_ptr<Window> window = this->FindDeepestWindowContainingPoint(mousePosition);
				if (window.get())
					window->HandleEvent(EventType::MOUSE_MOTION, &event);
			}
		};

	while (this->graphicsInterface->HandleEvents())
	{
		this->SetBoundingRectangle(graphicsInterface->renderState.worldRegion);

		// Should probably not be calculating layout constantly, but only when needed.
		this->LayoutChildren(this->graphicsInterface.get());

		// A better system wouldn't be constantly rendering, but only when and where needed.
		graphicsInterface->BeginRendering();
		this->Draw(this->graphicsInterface.get());
		graphicsInterface->EndRendering();
	}

	this->childWindowArray.clear();

	graphicsInterface->Shutdown();

	return true;
}

/*virtual*/ void DesktopWindow::Draw(GAL2D::GraphicsInterface* graphics)
{
	GAL2D::Rectangle backgroundRect = this->boundingRect;

	if (this->backgroundTexture.get())
	{
		GAL2D::Vector textureSize = this->backgroundTexture->GetSize();
		backgroundRect.ExpandToMatchAspectRatio(textureSize.x / textureSize.y);
	}

	graphics->RenderRectangle(backgroundRect, GAL2D::Color(0.5, 0.5, 0.5, 1.0), this->backgroundTexture);

	Window::Draw(graphics);
}

void DesktopWindow::SetMouseCaptureWindow(Window* window)
{
	if (window)
		this->mouseCaptureWindowWeakPtr = window->shared_from_this();
	else
		this->mouseCaptureWindowWeakPtr.reset();
}

bool DesktopWindow::BringWindowToFront(Window* window)
{
	for (int i = 0; i < (int)this->childWindowArray.size(); i++)
	{
		if (this->childWindowArray[i].get() == window)
		{
			int j = (int)this->childWindowArray.size() - 1;
			if (i != j)
			{
				std::shared_ptr<Window> windowPtr = this->childWindowArray[j];
				this->childWindowArray[j] = this->childWindowArray[i];
				this->childWindowArray[i] = windowPtr;
			}

			return true;
		}
	}

	return false;
}