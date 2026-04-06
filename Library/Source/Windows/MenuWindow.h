#pragma once

#include "Window.h"
#include <functional>
#include <unordered_map>

namespace VeryGUI
{
	/**
	 * Individual drivers can be created by the application per menu, or one driver
	 * can be created for all menus of the entire application, or somewhere inbetween.
	 * This class is designed to embed and reveal knowledge of menu structure.
	 */
	class MenuDriver : public std::enable_shared_from_this<MenuDriver>
	{
	public:
		MenuDriver();
		virtual ~MenuDriver();

		virtual int GetNumMenuItems(const std::string& menuName) = 0;
		virtual bool GetMenuItemLabel(const std::string& menuName, int i, std::string& label) = 0;
		virtual bool IsMenuItemEnabled(const std::string& menuName, int i);
		virtual bool GetMenuItemIconPath(const std::string& menuName, int i, std::filesystem::path& iconPath);
		virtual bool GetMenuItemSubMenuName(const std::string& menuName, int i, std::string& subMenuName);
		virtual std::shared_ptr<MenuDriver> GetSubMenuDriver(const std::string& subMenuName);
		virtual void HandleMenuItemClick(const std::string& menuName, int i);
	};

	/**
	 * This class represents a single popup menu.  This could come from a menu bar,
	 * or be a context menu, or whatever else.
	 */
	class MenuWindow : public Window
	{
	public:
		enum AnchorPlacement
		{
			LOWER_LEFT,
			LOWER_RIGHT,
			UPPER_LEFT,
			UPPER_RIGHT
		};

		MenuWindow(const std::string& menuName, std::shared_ptr<MenuDriver> menuDriver);
		virtual ~MenuWindow();

		void SetAnchor(const GAL2D::Vector& anchorPoint, AnchorPlacement anchorPlacement);

		virtual void LayoutChildren(GAL2D::GraphicsInterface* graphics) override;
		virtual DrawOrder Draw(GAL2D::GraphicsInterface* graphics) override;
		virtual void HandleEvent(EventType eventType, const void* eventData) override;
		virtual bool CanExceedParentBounds() const override;

	protected:

		struct MenuItemCache
		{
			int i;
			bool isHighlighted;
			double labelWidth;
			std::filesystem::path iconPath;
			std::shared_ptr<GAL2D::Texture> iconTexture;
			std::weak_ptr<MenuWindow> subMenuWindow;
			GAL2D::Rectangle labelRect;
			GAL2D::Rectangle iconRect;
			GAL2D::Rectangle itemRect;
		};

		std::shared_ptr<GAL2D::Font> labelFont;
		std::shared_ptr<MenuDriver> menuDriver;
		std::unordered_map<std::string, std::shared_ptr<MenuItemCache>> menuItemCacheMap;
		std::string menuName;
		GAL2D::Vector anchorPoint;
		AnchorPlacement anchorPlacement;
		static double labelHeight;
		static double margin;
	};
}