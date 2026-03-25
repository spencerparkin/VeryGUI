#pragma once

#include "Window.h"
#include "GAL2DMakeInterface.h"
#include <filesystem>
#include <unordered_set>

namespace VeryGUI
{
	/**
	 * This is always the root window.
	 */
	class DesktopWindow : public Window
	{
	public:
		DesktopWindow(const GAL2D::Vector& size, const std::filesystem::path& resourceBasePath, const std::filesystem::path& backgroundImagePath);
		virtual ~DesktopWindow();

		bool Run();

		virtual void Draw(GAL2D::GraphicsInterface* graphics) override;

		void SetMotionCaptureWindow(Window* window);

	protected:

		GAL2D::Vector size;
		std::filesystem::path resourceBasePath;
		std::filesystem::path backgroundImagePath;
		std::shared_ptr<GAL2D::GraphicsInterface> graphicsInterface;
		std::shared_ptr<GAL2D::Texture> backgroundTexture;
		std::weak_ptr<Window> motionCaptureWindowWeakPtr;
	};
}