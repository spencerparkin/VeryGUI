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
			std::shared_ptr<Window> window = this->FindDeepestWindowContainingPoint(mousePosition);
			while (window.get())
			{
				if (!window->HandleMouseClickEvent(mousePosition, mouseButton, buttonState))
					window = window->GetParentWindow();
				else
					break;
			}
		};

	while (this->graphicsInterface->HandleEvents())
	{
		GAL2D::Vector screenSize;
		this->graphicsInterface->GetScreenResolution(screenSize);

		this->graphicsInterface->renderState.worldRegion.minCorner.x = 0.0;
		this->graphicsInterface->renderState.worldRegion.minCorner.y = 0.0;
		this->graphicsInterface->renderState.worldRegion.maxCorner.x = screenSize.x;
		this->graphicsInterface->renderState.worldRegion.maxCorner.y = screenSize.y;

		this->SetBoundingRectangle(graphicsInterface->renderState.worldRegion);

		this->LayoutChildren();

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