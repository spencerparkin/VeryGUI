#pragma once

#include "Math/Rectangle.h"
#include "Graphics/GraphicsInterface.h"
#include <vector>
#include <memory>

namespace VeryGUI
{
	/**
	 * This is the base class for all members of the window hierarchy.
	 */
	class Window
	{
	public:
		Window();
		virtual ~Window();

		/**
		 * Each type of window is responsible for how it lays out its children.
		 * Some windows exist only to layout their children and don't draw anything.
		 */
		virtual void Layout();

		/**
		 * This should be overridden if the window renders anything.  It should draw
		 * only within its bounding rectangle.  Anything drawn outside that rectangle
		 * will get clipped by the stencil buffer.  The window derivative should draw
		 * first before calling the base class' draw method.  This default implementation
		 * just calls draw on the children.  This way we get the painter's algorithm,
		 * drawing everything back to front.
		 */
		virtual void Draw(GAL2D::GraphicsInterface* graphics);

	protected:
		
		/**
		 * This is the world boundary of the window.  Calculating this rectangle for
		 * all windows in the hierarchy is the result of the @ref Layout method.
		 * The @ref Draw methid is then responsible for rendering with this region.
		 */
		GAL2D::Rectangle boundingRect;
		
		std::weak_ptr<Window> parentWindowWeakPtr;
		std::vector<std::shared_ptr<Window>> childWindowArray;
	};
}