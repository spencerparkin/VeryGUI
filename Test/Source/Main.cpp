#include "Main.h"
#include "GAL2DMakeInterface.h"

int WINAPI WinMain(HINSTANCE instanceHandle, HINSTANCE prevInstanceHandle, LPSTR cmdLine, int cmdShow)
{
	GAL2D::DriverInitData driverInitData;
	driverInitData.instanceHandle = instanceHandle;
	driverInitData.cmdShow = cmdShow;
	driverInitData.windowTitle = "VeryGUI Test App";

	std::shared_ptr<GAL2D::GraphicsInterface> graphics = GAL2D::CreateGraphicsInterface(&driverInitData);

	if (!graphics->Setup())
	{
		MessageBoxA(NULL, "Failed to setup graphics!", "Error!", MB_OK | MB_ICONERROR);
		return -1;
	}

	graphics->renderState.worldRegion.minCorner.x = -10.0;
	graphics->renderState.worldRegion.maxCorner.x = 10.0;
	graphics->renderState.worldRegion.minCorner.y = -10.0;
	graphics->renderState.worldRegion.maxCorner.y = 10.0;

	while (graphics->HandleEvents())
	{
		graphics->BeginRendering();
		//...
		graphics->EndRendering();
	}

	graphics->Shutdown();

	return 0;
}