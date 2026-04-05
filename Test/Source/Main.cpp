#include "Main.h"
#include "Windows/DesktopWindow.h"
#include "Windows/TopLevelWindow.h"
#include "Windows/BlankWindow.h"
#include "Windows/MenuBarWindow.h"
#include "Windows/HVBoxWindow.h"
#include "CustomBlankWindow.h"

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

		auto vBox = std::make_shared<VeryGUI::VBoxWindow>();
		windowA->AddChildWindow(vBox);

		auto menuBar = std::make_shared<VeryGUI::MenuBarWindow>("menuBarMenu", std::make_shared<MainMenuDriver>());
		auto blankWindow = std::make_shared<VeryGUI::BlankWindow>("Images\\ImageA.png");
		vBox->AddChildWindowWithSizingConfig(menuBar, { VeryGUI::HVBoxWindow::SizeConstraint::FIT_TO_CONTENT, 0.0 });
		vBox->AddChildWindowWithSizingConfig(blankWindow, { VeryGUI::HVBoxWindow::SizeConstraint::PROPORTIONAL, 1.0 });

		auto windowB = std::make_shared<VeryGUI::TopLevelWindow>();
		windowB->SetBoundingRectangle(GAL2D::Rectangle(600.0, 1100.0, 600.0, 1100.0));
		windowB->SetTitle("Window B");
		desktop->AddChildWindow(windowB);

		blankWindow = std::make_shared<CustomBlankWindow>("Images\\ImageB.png");
		windowB->AddChildWindow(blankWindow);
	}

	return desktop->Run() ? 0 : -1;
}

MainMenuDriver::MainMenuDriver()
{
}

/*virtual*/ MainMenuDriver::~MainMenuDriver()
{
}

/*virtual*/ int MainMenuDriver::GetNumMenuItems(const std::string& menuName)
{
	if (menuName == "menuBarMenu")
		return 4;
	else if (menuName == "fileMenu")
		return 4;
	else if (menuName == "editMenu")
		return 3;
	else if (menuName == "optionsMenu")
		return 2;
	else if (menuName == "helpMenu")
		return 2;

	return 0;
}

/*virtual*/ bool MainMenuDriver::GetMenuItemLabel(const std::string& menuName, int i, std::string& label)
{
	if (menuName == "menuBarMenu")
	{
		switch (i)
		{
		case 0:
			label = "File";
			return true;
		case 1:
			label = "Edit";
			return true;
		case 2:
			label = "Options";
			return true;
		case 3:
			label = "Help";
			return true;
		}
	}
	else if (menuName == "fileMenu")
	{
		switch (i)
		{
		case 0:
			label = "New";
			return true;
		case 1:
			label = "Open";
			return true;
		case 2:
			label = "Close";
			return true;
		case 3:
			label = "Exit";
			return true;
		}
	}
	else if (menuName == "editMenu")
	{
		switch (i)
		{
		case 0:
			label = "Cut";
			return true;
		case 1:
			label = "Copy";
			return true;
		case 2:
			label = "Paste";
			return true;
		}
	}
	else if (menuName == "optionsMenu")
	{
		switch (i)
		{
		case 0:
			label = "Preferences";
			return true;
		case 1:
			label = "Settings";
			return true;
		}
	}
	else if (menuName == "helpMenu")
	{
		switch (i)
		{
		case 0:
			label = "Documentation";
			return true;
		case 1:
			label = "About";
			return true;
		}
	}

	return false;
}

/*virtual*/ bool MainMenuDriver::IsMenuItemEnabled(const std::string& menuName, int i)
{
	return true;
}

/*virtual*/ bool MainMenuDriver::GetMenuItemIconPath(const std::string& menuName, int i, std::filesystem::path& iconPath)
{
	return false;
}

/*virtual*/ bool MainMenuDriver::GetMenuItemSubMenuName(const std::string& menuName, int i, std::string& subMenuName)
{
	if (menuName == "menuBarMenu")
	{
		switch (i)
		{
		case 0:
			subMenuName = "fileMenu";
			return true;
		case 1:
			subMenuName = "editMenu";
			return true;
		case 2:
			subMenuName = "optionsMenu";
			return true;
		case 3:
			subMenuName = "helpMenu";
			return true;
		}
	}

	return false;
}

/*virtual*/ void MainMenuDriver::HandleMenuItemClick(const std::string& menuName, int i)
{
}