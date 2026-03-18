#pragma once

#include "Math/Rectangle.h"
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

	protected:
		
		GAL2D::Rectangle boundingRect;
		
		std::weak_ptr<Window> parentWindowWeakPtr;
		std::vector<std::shared_ptr<Window>> childWindowArray;
	};
}