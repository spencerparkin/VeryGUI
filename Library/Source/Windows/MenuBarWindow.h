#pragma once

#include "MenuWindow.h"

namespace VeryGUI
{
	/**
	 * 
	 */
	class MenuBarWindow : public Window
	{
	public:
		MenuBarWindow(const std::string& menuName, std::shared_ptr<MenuDriver> menuDriver);
		virtual ~MenuBarWindow();

		virtual void LayoutChildren(GAL2D::GraphicsInterface* graphics) override;
		virtual void Draw(GAL2D::GraphicsInterface* graphics) override;
		virtual void HandleEvent(EventType eventType, const void* eventData) override;
		virtual bool CanExceedParentBounds() const override;
		virtual double GetDesiredHeight() override;

	private:

		struct MenuItemCache
		{
			int i;
			bool isHighlighted;
			double labelWidth;
			std::weak_ptr<MenuWindow> subMenuWindow;
			GAL2D::Rectangle labelRect;
			GAL2D::Rectangle itemRect;
		};

		std::unordered_map<std::string, std::shared_ptr<MenuItemCache>> menuItemCacheMap;
		std::shared_ptr<GAL2D::Font> labelFont;
		std::shared_ptr<MenuDriver> menuDriver;
		std::string menuName;
		static double labelHeight;
		static double margin;
	};
}