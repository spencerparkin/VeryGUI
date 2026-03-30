#pragma once

#include "Windows/BlankWindow.h"
#include "Windows/MenuWindow.h"

class CustomBlankWindow : public VeryGUI::BlankWindow
{
public:
	CustomBlankWindow(const std::filesystem::path& backgroundImagePath);
	virtual ~CustomBlankWindow();

	virtual void HandleEvent(VeryGUI::EventType eventType, const void* eventData) override;

private:

	class ContextMenuDriver : public VeryGUI::MenuDriver
	{
	public:
		ContextMenuDriver();
		virtual ~ContextMenuDriver();

		virtual int GetNumMenuItems(const std::string& menuName) override;
		virtual bool GetMenuItemLabel(const std::string& menuName, int i, std::string& label) override;
		virtual bool IsMenuItemEnabled(const std::string& menuName, int i) override;
		virtual bool GetMenuItemIconPath(const std::string& menuName, int i, std::filesystem::path& iconPath) override;
		virtual bool GetMenuItemSubMenuName(const std::string& menuName, int i, std::string& subMenuName) override;
		virtual void HandleMenuItemClick(const std::string& menuName, int i) override;
	};

	std::shared_ptr<ContextMenuDriver> contextMenuDriver;
	std::weak_ptr<VeryGUI::MenuWindow> contextMenuWindowWeakPtr;
};