#pragma once

#include "Windows/MenuWindow.h"

class MainMenuDriver : public VeryGUI::MenuDriver
{
public:
	MainMenuDriver();
	virtual ~MainMenuDriver();

	virtual int GetNumMenuItems(const std::string& menuName) override;
	virtual bool GetMenuItemLabel(const std::string& menuName, int i, std::string& label) override;
	virtual bool IsMenuItemEnabled(const std::string& menuName, int i) override;
	virtual bool GetMenuItemIconPath(const std::string& menuName, int i, std::filesystem::path& iconPath) override;
	virtual bool GetMenuItemSubMenuName(const std::string& menuName, int i, std::string& subMenuName) override;
	virtual void HandleMenuItemClick(const std::string& menuName, int i) override;
};