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
		BlankWindow(const std::filesystem::path& backgroundImagePath);
		BlankWindow(const GAL2D::Color& color);
		virtual ~BlankWindow();

		virtual DrawOrder Draw(GAL2D::GraphicsInterface* graphics) override;

	private:
		GAL2D::Color color;
		std::shared_ptr<GAL2D::Texture> backgroundTexture;
		std::filesystem::path backgroundImagePath;
	};
}