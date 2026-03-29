#include "CustomBlankWindow.h"

//------------------------------------- CustomBlankWindow -------------------------------------

CustomBlankWindow::CustomBlankWindow(const std::filesystem::path& backgroundImagePath) : BlankWindow(backgroundImagePath)
{
	this->contextMenuDriver = std::make_shared<ContextMenuDriver>();
}

/*virtual*/ CustomBlankWindow::~CustomBlankWindow()
{
}

/*virtual*/ void CustomBlankWindow::HandleEvent(VeryGUI::EventType eventType, const void* eventData)
{
	switch (eventType)
	{
		case VeryGUI::EventType::MOUSE_CLICK:
		{
			auto event = static_cast<const VeryGUI::MouseClickEvent*>(eventData);

			if (event->buttonState == GAL2D::ButtonState::Down)
			{
				if (event->mouseButton == GAL2D::MouseButton::Right)
				{
					if (this->contextMenuWindowWeakPtr.expired())
					{
						std::shared_ptr<VeryGUI::MenuWindow> contextMenuWindow = std::make_shared<VeryGUI::MenuWindow>("menuA", this->contextMenuDriver);
						contextMenuWindow->SetAnchor(event->mousePosition, VeryGUI::MenuWindow::AnchorPlacement::UPPER_LEFT);
						this->AddChildWindow(contextMenuWindow);
						this->contextMenuWindowWeakPtr = contextMenuWindow;
					}
				}
				else if (event->mouseButton == GAL2D::MouseButton::Left)
				{
					std::shared_ptr<VeryGUI::MenuWindow> contextMenuWindow = this->contextMenuWindowWeakPtr.lock();
					if (contextMenuWindow.get())
						this->RemoveChildWindow(contextMenuWindow);
				}
			}

			break;
		}
	}
}

//------------------------------------- CustomBlankWindow::ContextMenuDriver -------------------------------------

CustomBlankWindow::ContextMenuDriver::ContextMenuDriver()
{
}

/*virtual*/ CustomBlankWindow::ContextMenuDriver::~ContextMenuDriver()
{
}

/*virtual*/ int CustomBlankWindow::ContextMenuDriver::GetNumMenuItems(const std::string& menuName)
{
	if (menuName == "menuA")
		return 6;
	else if (menuName == "menuB")
		return 4;
	else if (menuName == "menuC")
		return 3;

	return 0;
}

/*virtual*/ bool CustomBlankWindow::ContextMenuDriver::GetMenuItemLabel(const std::string& menuName, int i, std::string& label)
{
	if (menuName == "menuA")
	{
		switch (i)
		{
		case 0:
			label = "Option 1";
			return true;
		case 1:
			label = "Option 2";
			return true;
		case 2:
			label = "Option 3";
			return true;
		case 3:
			label = "Amazing option 1";
			return true;
		case 4:
			label = "Amazing option 2";
			return true;
		case 5:
			label = "Amazing option 3";
			return true;
		}
	}
	else if (menuName == "menuB")
	{
		switch (i)
		{
		case 0:
			label = "Launch Nukes";
			return true;
		case 1:
			label = "Launch Missiles";
			return true;
		case 2:
			label = "Launch Torpedoes";
			return true;
		case 3:
			label = "Launch Merchandise";
			return true;
		}
	}
	else if (menuName == "menuC")
	{
		switch (i)
		{
		case 0:
			label = "T-shirt";
			return true;
		case 1:
			label = "Sun Cap";
			return true;
		case 2:
			label = "Mug";
			return true;
		}
	}

	return false;
}

/*virtual*/ bool CustomBlankWindow::ContextMenuDriver::IsMenuItemEnabled(const std::string& menuName, int i)
{
	if (menuName == "menuA")
		return true;

	return false;
}

/*virtual*/ bool CustomBlankWindow::ContextMenuDriver::GetMenuItemIconPath(const std::string& menuName, std::filesystem::path& iconPath)
{
	return false;
}

/*virtual*/ bool CustomBlankWindow::ContextMenuDriver::GetMenuItemSubMenuName(const std::string& menuName, int i, std::string& subMenuName)
{
	if (menuName == "menuA")
	{
		switch (i)
		{
		case 4:
			subMenuName = "menuB";
			return true;
		}
	}
	else if (menuName == "menuB")
	{
		switch (i)
		{
		case 3:
			subMenuName = "menuC";
			return true;
		}
	}

	return false;
}

/*virtual*/ void CustomBlankWindow::ContextMenuDriver::HandleMenuItemClick(const std::string& menuName, int i)
{
}