#pragma once

#include "Window.h"

namespace VeryGUI
{
	/**
	 * The logic and data in this class is shared between the @ref VBoxWindow and @ref HBoxWindow classes.
	 */
	class HVBoxWindow : public Window
	{
	protected:
		HVBoxWindow();
		virtual ~HVBoxWindow();

	public:

		enum SizeConstraint
		{
			FIXED,				/// The width (or height) must be a given length, if possible.
			PROPORTIONAL,		/// The width (or height) is given a portion of all remaining non-allocated length.
			FIT_TO_CONTENT		/// The width (or height) should be what is desired by the child, if possible.
		};

		struct SizingConfig
		{
			SizeConstraint constraint;
			union
			{
				double proportion;
				double length;
			};
		};

		bool AddChildWindowWithSizingConfig(std::shared_ptr<Window> window, SizingConfig sizingConfig);

	protected:

		virtual double GetChildWindowDesiredLength(int i) = 0;
		virtual double GetTotalLengthAvailable() = 0;

		std::vector<SizingConfig> sizingConfigArray;
	};

	/**
	 * With an HBoxWindow, all children have the same height (the height of this window)
	 * and are layed out horizontally.  What amount of width each child gets is
	 * determined by their sizing constraint.  Compare to @ref VBoxWindow.
	 * This class is designed purely for layout purposes and does not render anything.
	 */
	class HBoxWindow : public HVBoxWindow
	{
	public:
		HBoxWindow();
		virtual ~HBoxWindow();

		virtual void LayoutChildren(GAL2D::GraphicsInterface* graphics) override;

	protected:
		virtual double GetChildWindowDesiredLength(int i) override;
		virtual double GetTotalLengthAvailable() override;
	};

	/**
	 * With a VBoxWindow, all children have the same width (the width of this window)
	 * and are layed out vertically.  What amount of height each child gets is
	 * determined by their sizing constraint.  Compare to @ref HBoxWindow.
	 * This class is designed purely for layout purposes and does not render anything.
	 */
	class VBoxWindow : public HVBoxWindow
	{
	public:
		VBoxWindow();
		virtual ~VBoxWindow();

		virtual void LayoutChildren(GAL2D::GraphicsInterface* graphics) override;

	protected:
		virtual double GetChildWindowDesiredLength(int i) override;
		virtual double GetTotalLengthAvailable() override;
	};
}