#include "Windows/BlankWindow.h"
#include <assert.h>

using namespace VeryGUI;

BlankWindow::BlankWindow(const std::filesystem::path& backgroundImagePath)
{
	this->backgroundImagePath = backgroundImagePath;
}

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

/*virtual*/ void BlankWindow::Draw(GAL2D::GraphicsInterface* graphics)
{
	if (!this->backgroundImagePath.empty() && !this->backgroundTexture.get())
	{
		this->backgroundTexture = graphics->MakeTexture(this->backgroundImagePath);
		assert(this->backgroundTexture.get() != nullptr);
	}

	// STPTODO: Adjust rect here to preserve aspect ratio of texture.  But you'll also need to enable clipping.
	graphics->RenderRectangle(this->boundingRect, this->color, this->backgroundTexture);
}