#include "DesktopWindow.h"
#include "Graphics/Texture.h"

using namespace VeryGUI;

DesktopWindow::DesktopWindow(const GAL2D::Vector& size, const std::string& backgroundImagePath)
{
	this->size = size;
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
	{
		MessageBoxA(NULL, "Failed to setup graphcis!", "Error!", MB_OK | MB_ICONERROR);
		return false;
	}

	if (this->backgroundImagePath.length() > 0)
	{
		this->backgroundTexture = this->graphicsInterface->MakeTexture(this->backgroundImagePath);
		if (!this->backgroundTexture.get())
		{
			MessageBoxA(NULL, "Failed to load background image!", "Error!", MB_OK | MB_ICONERROR);
			return false;
		}
	}

	while (this->graphicsInterface->HandleEvents())
	{
		GAL2D::Vector screenSize;
		this->graphicsInterface->GetScreenResolution(screenSize);

		this->graphicsInterface->renderState.worldRegion.minCorner.x = 0.0;
		this->graphicsInterface->renderState.worldRegion.minCorner.y = 0.0;
		this->graphicsInterface->renderState.worldRegion.maxCorner.x = screenSize.x;
		this->graphicsInterface->renderState.worldRegion.maxCorner.y = screenSize.y;

		this->Layout();

		graphicsInterface->BeginRendering();
		this->Draw(this->graphicsInterface.get());
		graphicsInterface->EndRendering();
	}

	graphicsInterface->Shutdown();

	return true;
}

/*virtual*/ void DesktopWindow::Layout()
{
	this->boundingRect = graphicsInterface->renderState.worldRegion;

	for (std::shared_ptr<Window> window : this->childWindowArray)
		window->Layout();
}

/*virtual*/ void DesktopWindow::Draw(GAL2D::GraphicsInterface* graphics)
{
	GAL2D::Vector textureSize = this->backgroundTexture->GetSize();
	GAL2D::Rectangle backgroundRect = this->boundingRect;
	backgroundRect.ExpandToMatchAspectRatio(textureSize.x / textureSize.y);

	graphics->RenderRectangle(backgroundRect, GAL2D::Color(0.5, 0.5, 0.5, 1.0), this->backgroundTexture);

	Window::Draw(graphics);
}