#include "SplitterWindow.h"
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

/*virtual*/ void SplitterWindow::LayoutChildren()
{
	assert(this->childWindowArray.size() == 2 && "Splitter windows should have exactly two children.");

	// STPTODO: Write this.
}

/*virtual*/ void SplitterWindow::Draw(GAL2D::GraphicsInterface* graphics, GAL2D::Font* commonFont)
{
	// STPTODO: Write this.
}