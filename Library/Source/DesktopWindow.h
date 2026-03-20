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
		DesktopWindow(const GAL2D::Vector& size, const std::string& backgroundImagePath);
		virtual ~DesktopWindow();

		bool Run();

		virtual void Layout() override;
		virtual void Draw(GAL2D::GraphicsInterface* graphics) override;

	protected:

		GAL2D::Vector size;
		std::string backgroundImagePath;
		std::shared_ptr<GAL2D::GraphicsInterface> graphicsInterface;
		std::shared_ptr<GAL2D::Texture> backgroundTexture;
	};
}