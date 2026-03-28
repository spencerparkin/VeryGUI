#pragma once

#include "Window.h"

namespace VeryGUI
{
	/**
	 * This type of window is designed to have exactly two children, arranged
	 * vertically or horizontally.  The divider can be dragged with the mouse
	 * to size the children.
	 */
	class SplitterWindow : public Window
	{
	public:
		enum Orientation
		{
			SPLIT_VERTICAL,
			SPLIT_HORIZONTAL
		};

		SplitterWindow(Orientation orientation);
		virtual ~SplitterWindow();

		virtual void LayoutChildren(GAL2D::GraphicsInterface* graphics) override;
		virtual void Draw(GAL2D::GraphicsInterface* graphics) override;

	private:
		Orientation orientation;
		static double dividerThickness;
	};
}