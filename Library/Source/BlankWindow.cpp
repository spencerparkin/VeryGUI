#include "BlankWindow.h"
#include <assert.h>

using namespace VeryGUI;

BlankWindow::BlankWindow(const GAL2D::Color& color)
{
	this->color = color;
}

/*virtual*/ BlankWindow::~BlankWindow()
{
}

/*virtual*/ void BlankWindow::LayoutChildren()
{
	assert(this->childWindowArray.size() == 0 && "Blank windows should not have any children.");
}

/*virtual*/ void BlankWindow::Draw(GAL2D::GraphicsInterface* graphics, GAL2D::Font* commonFont)
{
	graphics->RenderRectangle(this->boundingRect, this->color);
}