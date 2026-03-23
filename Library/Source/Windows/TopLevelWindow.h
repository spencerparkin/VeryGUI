#pragma once

#include "Window.h"
#include <string>

namespace VeryGUI
{
	/**
	 * These are windows meant to be immediate children of the desktop window
	 * that float around and that can have their size adjusted, etc.
	 */
	class TopLevelWindow : public Window
	{
	public:
		TopLevelWindow();
		virtual ~TopLevelWindow();

		virtual void LayoutChildren() override;
		virtual void Draw(GAL2D::GraphicsInterface* graphics) override;

		void SetTitle(const std::string& title);
		const std::string& GetTitle() const;

	protected:

		struct BorderRects
		{
			GAL2D::Rectangle tRect;
			GAL2D::Rectangle bRect;
			GAL2D::Rectangle lRect;
			GAL2D::Rectangle rRect;
			GAL2D::Rectangle tlRect;
			GAL2D::Rectangle trRect;
			GAL2D::Rectangle blRect;
			GAL2D::Rectangle brRect;
		};

		void CalcBorderRects(BorderRects& borderRects);

		std::string title;
		GAL2D::Rectangle titleBarRect;
		GAL2D::Rectangle innerRect;
		std::shared_ptr<GAL2D::Font> titleFont;

		static double borderMargin;
		static double titleBarThickness;
	};
}