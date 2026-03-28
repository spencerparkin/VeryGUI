#include "Windows/SplitterWindow.h"
#include <assert.h>

using namespace VeryGUI;

double SplitterWindow::dividerThickness = 5.0;

SplitterWindow::SplitterWindow(Orientation orientation)
{
	this->orientation = orientation;
}

/*virtual*/ SplitterWindow::~SplitterWindow()
{
}

/*virtual*/ void SplitterWindow::LayoutChildren(GAL2D::GraphicsInterface* graphics)
{
	assert(this->childWindowArray.size() == 2 && "Splitter windows should have exactly two children.");

	// STPTODO: Write this.
}

/*virtual*/ void SplitterWindow::Draw(GAL2D::GraphicsInterface* graphics)
{
	// STPTODO: Write this.
}