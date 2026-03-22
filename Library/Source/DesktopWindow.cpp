#include "DesktopWindow.h"
#include "Graphics/Texture.h"

using namespace VeryGUI;

DesktopWindow::DesktopWindow(const GAL2D::Vector& size, const std::string& backgroundImagePath, const std::string& commonFontPath)
{
	this->size = size;
	this->backgroundImagePath = backgroundImagePath;
	this->commonFontPath = commonFontPath;
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

	if (this->backgroundImagePath.length() > 0)
	{
		this->backgroundTexture = this->graphicsInterface->MakeTexture(this->backgroundImagePath);
		if (!this->backgroundTexture.get())
			return false;
	}

	this->commonFont = this->graphicsInterface->MakeFont(this->commonFontPath);
	if (!this->commonFont.get())
		return false;

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
		this->Draw(this->graphicsInterface.get(), this->commonFont.get());
		graphicsInterface->EndRendering();
	}

	graphicsInterface->Shutdown();

	return true;
}

/*virtual*/ void DesktopWindow::Draw(GAL2D::GraphicsInterface* graphics, GAL2D::Font* commonFont)
{
	GAL2D::Vector textureSize = this->backgroundTexture->GetSize();
	GAL2D::Rectangle backgroundRect = this->boundingRect;
	backgroundRect.ExpandToMatchAspectRatio(textureSize.x / textureSize.y);

	graphics->RenderRectangle(backgroundRect, GAL2D::Color(0.5, 0.5, 0.5, 1.0), this->backgroundTexture);

	Window::Draw(graphics, commonFont);
}