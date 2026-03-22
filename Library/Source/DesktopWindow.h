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
		DesktopWindow(const GAL2D::Vector& size, const std::string& backgroundImagePath, const std::string& commonFontPath);
		virtual ~DesktopWindow();

		bool Run();

		virtual void Draw(GAL2D::GraphicsInterface* graphics, GAL2D::Font* commonFont) override;

	protected:

		GAL2D::Vector size;
		std::string backgroundImagePath;
		std::string commonFontPath;
		std::shared_ptr<GAL2D::GraphicsInterface> graphicsInterface;
		std::shared_ptr<GAL2D::Texture> backgroundTexture;
		std::shared_ptr<GAL2D::Font> commonFont;
	};
}