#pragma once

#include "Window.h"

namespace VeryGUI
{
	/**
	 * These windows just fill up space with a given solid color.
	 */
	class BlankWindow : public Window
	{
	public:
		BlankWindow(const GAL2D::Color& color);
		virtual ~BlankWindow();

		virtual void LayoutChildren() override;
		virtual void Draw(GAL2D::GraphicsInterface* graphics) override;

	private:
		GAL2D::Color color;
	};
}