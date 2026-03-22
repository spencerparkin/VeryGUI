#include "Main.h"
#include "DesktopWindow.h"
#include "TopLevelWindow.h"
#include "BlankWindow.h"

int WINAPI WinMain(HINSTANCE instanceHandle, HINSTANCE prevInstanceHandle, LPSTR cmdLine, int cmdShow)
{
	std::shared_ptr<VeryGUI::DesktopWindow> desktop = std::make_shared<VeryGUI::DesktopWindow>(
		GAL2D::Vector(1600.0, 1200.0),
		"D:\\git_repos\\VeryGUI\\Test\\Images\\desktop_background.png",
		"D:\\git_repos\\VeryGUI\\Test\\Fonts\\Roboto_Regular.ttf");

	auto windowA = std::make_shared<VeryGUI::TopLevelWindow>();
	windowA->SetBoundingRectangle(GAL2D::Rectangle(100.0, 500.0, 100.0, 500.0));
	windowA->SetTitle("Window A");
	desktop->AddChildWindow(windowA);

	auto blankWindow = std::make_shared<VeryGUI::BlankWindow>(GAL2D::Color(0.5, 0.5, 0.5, 1.0));
	windowA->AddChildWindow(blankWindow);

	return desktop->Run() ? 0 : -1;
}