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
		std::shared_ptr<GAL2D::Font> labelFont;
		std::shared_ptr<MenuDriver> menuDriver;
		std::string menuName;
	};
}