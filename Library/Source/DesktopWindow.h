#pragma once

#include "Window.h"
#include "GAL2DMakeInterface.h"

namespace VeryGUI
{
	/**
	 * This is always the root window.
	 */
	class DesktopWindow : public Window
	{
	public:
		DesktopWindow(const GAL2D::Vector& size);
		virtual ~DesktopWindow();

		bool Run();

	protected:
		GAL2D::Vector size;
		std::shared_ptr<GAL2D::GraphicsInterface> graphics;
	};
}