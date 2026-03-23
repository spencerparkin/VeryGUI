#include "Main.h"
#include "Windows/DesktopWindow.h"
#include "Windows/TopLevelWindow.h"
#include "Windows/BlankWindow.h"

int WINAPI WinMain(HINSTANCE instanceHandle, HINSTANCE prevInstanceHandle, LPSTR cmdLine, int cmdShow)
{
	std::shared_ptr<VeryGUI::DesktopWindow> desktop = std::make_shared<VeryGUI::DesktopWindow>(
		GAL2D::Vector(1600.0, 1200.0),
		"D:\\git_repos\\VeryGUI\\Test\\Resources\\",
		"Images\\desktop_background.png");

	// Enter scope so that shared pointers go out of scope before we run the desktop.
	{
		auto windowA = std::make_shared<VeryGUI::TopLevelWindow>();
		windowA->SetBoundingRectangle(GAL2D::Rectangle(100.0, 500.0, 100.0, 500.0));
		windowA->SetTitle("Window A");
		desktop->AddChildWindow(windowA);

		auto blankWindow = std::make_shared<VeryGUI::BlankWindow>(GAL2D::Color(0.5, 0.5, 0.5, 1.0));
		windowA->AddChildWindow(blankWindow);
	}

	return desktop->Run() ? 0 : -1;
}