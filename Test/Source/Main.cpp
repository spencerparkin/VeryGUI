#include "Main.h"
#include "DesktopWindow.h"

int WINAPI WinMain(HINSTANCE instanceHandle, HINSTANCE prevInstanceHandle, LPSTR cmdLine, int cmdShow)
{
	std::shared_ptr<VeryGUI::DesktopWindow> desktop = std::make_shared<VeryGUI::DesktopWindow>(GAL2D::Vector(800.0, 600.0), "D:\\git_repos\\VeryGUI\\Test\\Images\\desktop_background.png");

	return desktop->Run() ? 0 : -1;
}