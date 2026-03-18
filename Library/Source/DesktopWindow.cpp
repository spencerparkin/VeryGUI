#include "DesktopWindow.h"

using namespace VeryGUI;

DesktopWindow::DesktopWindow(const GAL2D::Vector& size)
{
	this->size = size;
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

	this->graphics = GAL2D::CreateGraphicsInterface(&driverInitData);

	if (!this->graphics->Setup())
	{
		MessageBoxA(NULL, "Failed to setup graphcis!", "Error!", MB_OK | MB_ICONERROR);
		return false;
	}

	this->graphics->renderState.worldRegion.minCorner.x = 0.0;
	this->graphics->renderState.worldRegion.minCorner.y = 0.0;
	this->graphics->renderState.worldRegion.maxCorner = this->size;

	while (this->graphics->HandleEvents())
	{
		graphics->BeginRendering();

		graphics->EndRendering();
	}

	graphics->Shutdown();

	return true;
}