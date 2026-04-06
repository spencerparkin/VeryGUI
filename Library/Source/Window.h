#pragma once

#include "Math/Rectangle.h"
#include "Graphics/GraphicsInterface.h"
#include <vector>
#include <memory>

namespace VeryGUI
{
	enum EventType
	{
		MOUSE_CLICK,
		MOUSE_MOTION,
		KEYBOARD
	};

	struct MouseClickEvent
	{
		GAL2D::Vector mousePosition;
		GAL2D::MouseButton mouseButton;
		GAL2D::ButtonState buttonState;
	};

	struct MouseMotionEvent
	{
		GAL2D::Vector mousePosition;
	};

	/**
	 * This is the base class for all members of the window hierarchy.
	 */
	class Window : public std::enable_shared_from_this<Window>
	{
	public:
		Window();
		virtual ~Window();

		/**
		 * Each type of window is responsible for how it lays out its children.
		 * (Some windows exist only to layout their children and don't draw anything.)
		 * In most cases, prior to this call, the bounding rectangle of this window should
		 * have already been decided by the parent, and our only purpose here is to calculate
		 * the bounding rectangles of our child windows.  In other cases, this is where the
		 * bounding rectangle of this window is calculated.
		 * 
		 * @param[in] graphics This is provided, not for drawing, but for resource acquisition and/or metrics queries.
		 */
		virtual void LayoutChildren(GAL2D::GraphicsInterface* graphics);

		enum DrawOrder
		{
			DEPTH_FIRST,
			DEPTH_LAST
		};

		/**
		 * This should be overridden if the window renders anything.  It should draw
		 * only within its bounding rectangle.  Anything drawn outside that rectangle
		 * will get clipped by the stencil buffer.  The window derivative should draw
		 * first before calling the base class' draw method.  This default implementation
		 * just calls draw on the children.  This way we get the painter's algorithm,
		 * drawing everything back to front.
		 * 
		 * @param[in] graphics This is provided for drawing the window and its contents.
		 * @return Indicate whether this window's children should draw before or after its siblings.
		 */
		virtual DrawOrder Draw(GAL2D::GraphicsInterface* graphics);

		/**
		 * Override this method to handle events that occur in the window.
		 * Call this base-class method if you would like the parent to also handle the event.
		 * 
		 * @param[in] eventType This can be used to know how to cast the given event.
		 * @param[in] eventData This is an opaque pointer to the event structure.
		 */
		virtual void HandleEvent(EventType eventType, const void* eventData);

		/**
		 * Return true here if the child window's bounding rectangle can legally be
		 * fully or partially outside of the parent's bounding rectangle.  The default
		 * here returns false.
		 */
		virtual bool CanExceedParentBounds() const;

		virtual double GetDesiredWidth();
		virtual double GetDesiredHeight();

		bool AddChildWindow(std::shared_ptr<Window> childWindow);
		bool RemoveChildWindow(std::shared_ptr<Window> childWindow);
		bool HasChildWindow(std::shared_ptr<Window> childWindow, int* offset = nullptr) const;

		void SetBoundingRectangle(const GAL2D::Rectangle& boundingRect);
		const GAL2D::Rectangle& GetBoundingRect() const;

		std::shared_ptr<Window> FindDeepestWindowContainingPoint(const GAL2D::Vector& point);
		std::shared_ptr<Window> GetParentWindow();
		std::shared_ptr<Window> GetRootWindow();

		void RenderWindowTree(GAL2D::GraphicsInterface* graphics);

		/**
		 * Perform a breadth-first traversal of the window hierarchy starting at this window.
		 * 
		 * @param[in,out] visitationFunc This is called per window in the hierarchy.  Return false to terminate the traversal prematurely.
		 * @return True is returned if the traversal ran to completion.
		 */
		bool BreadthFirstTraversal(std::function<bool(Window*)> visitationFunc);

		enum DFTMode
		{
			CallBeforeRecursing,
			CallAfterRecursing
		};

		/**
		 * Perform a depth-first traversal of the window hierarchy starting at this window.
		 * 
		 * @param[in,out] visitationFunc This is called per window in the hierarchy.  Return false to terminate the traversal permaturely.
		 * @param True is returned if the traversal ran to completion.
		 */
		bool DepthFirstTraversal(std::function<bool(Window*)> visitationFunc, DFTMode mode);

	protected:
		
		/**
		 * This is the world boundary of the window.  Calculating this rectangle for
		 * all windows in the hierarchy is the result of the @ref LayoutChildren method.
		 * The @ref Draw method is then responsible for rendering with this region.
		 */
		GAL2D::Rectangle boundingRect;
		
		std::weak_ptr<Window> parentWindowWeakPtr;
		std::vector<std::shared_ptr<Window>> childWindowArray;
	};
}