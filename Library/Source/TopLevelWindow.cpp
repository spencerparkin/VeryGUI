#include "TopLevelWindow.h"
#include "Graphics/Font.h"
#include <assert.h>

using namespace VeryGUI;

double TopLevelWindow::borderMargin = 5.0;
double TopLevelWindow::titleBarThickness = 20.0;

TopLevelWindow::TopLevelWindow()
{
}

/*virtual*/ TopLevelWindow::~TopLevelWindow()
{
}

/*virtual*/ void TopLevelWindow::LayoutChildren()
{
	this->innerRect = this->boundingRect;
	this->innerRect.ApplyMarginDelta(-borderMargin);

	if (this->childWindowArray.size() > 0)
	{
		if (this->childWindowArray.size() > 1)
			assert(false && "Top-level windows should have at most one child window.");

		this->titleBarRect = this->innerRect;
		this->titleBarRect.minCorner.y = this->titleBarRect.maxCorner.y - titleBarThickness;
		
		GAL2D::Rectangle clientRect = this->innerRect;
		clientRect.maxCorner.y -= titleBarThickness;

		assert(clientRect.IsValid() && this->titleBarRect.IsValid() && "Top-level window is too small!");

		std::shared_ptr<Window> childWindow = this->childWindowArray[0];
		childWindow->SetBoundingRectangle(clientRect);
	}
}

/*virtual*/ void TopLevelWindow::Draw(GAL2D::GraphicsInterface* graphics, GAL2D::Font* commonFont)
{
	BorderRects borderRects;
	this->CalcBorderRects(borderRects);
	
	graphics->RenderRectangle(borderRects.lRect, GAL2D::Color(0.5, 0.5, 1.0, 1.0));
	graphics->RenderRectangle(borderRects.rRect, GAL2D::Color(0.5, 0.5, 1.0, 1.0));
	graphics->RenderRectangle(borderRects.tRect, GAL2D::Color(0.5, 0.5, 1.0, 1.0));
	graphics->RenderRectangle(borderRects.bRect, GAL2D::Color(0.5, 0.5, 1.0, 1.0));

	graphics->RenderRectangle(borderRects.tlRect, GAL2D::Color(0.5, 1.0, 1.0, 1.0));
	graphics->RenderRectangle(borderRects.trRect, GAL2D::Color(0.5, 1.0, 1.0, 1.0));
	graphics->RenderRectangle(borderRects.blRect, GAL2D::Color(0.5, 1.0, 1.0, 1.0));
	graphics->RenderRectangle(borderRects.brRect, GAL2D::Color(0.5, 1.0, 1.0, 1.0));

	graphics->RenderRectangle(this->titleBarRect, GAL2D::Color(0.0, 0.0, 0.0, 1.0));

	GAL2D::Rectangle textRect = this->titleBarRect;
	textRect.ApplyMarginDelta(-2.0);
	graphics->RenderText(this->title, commonFont->shared_from_this(), textRect, GAL2D::Color(1.0, 1.0, 1.0, 1.0), GAL2D::GraphicsInterface::ALIGN_LEFT);

	Window::Draw(graphics, commonFont);
}

void TopLevelWindow::CalcBorderRects(BorderRects& borderRects)
{
	borderRects.lRect.minCorner.x = this->boundingRect.minCorner.x;
	borderRects.lRect.maxCorner.x = this->innerRect.minCorner.x;
	borderRects.lRect.minCorner.y = this->innerRect.minCorner.y;
	borderRects.lRect.maxCorner.y = this->innerRect.maxCorner.y;

	borderRects.rRect.minCorner.x = this->innerRect.maxCorner.x;
	borderRects.rRect.maxCorner.x = this->boundingRect.maxCorner.x;
	borderRects.rRect.minCorner.y = this->innerRect.minCorner.y;
	borderRects.rRect.maxCorner.y = this->innerRect.maxCorner.y;

	borderRects.tRect.minCorner.x = this->innerRect.minCorner.x;
	borderRects.tRect.maxCorner.x = this->innerRect.maxCorner.x;
	borderRects.tRect.minCorner.y = this->innerRect.maxCorner.y;
	borderRects.tRect.maxCorner.y = this->boundingRect.maxCorner.y;

	borderRects.bRect.minCorner.x = this->innerRect.minCorner.x;
	borderRects.bRect.maxCorner.x = this->innerRect.maxCorner.x;
	borderRects.bRect.minCorner.y = this->boundingRect.minCorner.y;
	borderRects.bRect.maxCorner.y = this->innerRect.minCorner.y;

	borderRects.tlRect.minCorner.x = this->boundingRect.minCorner.x;
	borderRects.tlRect.maxCorner.x = this->innerRect.minCorner.x;
	borderRects.tlRect.minCorner.y = this->innerRect.maxCorner.y;
	borderRects.tlRect.maxCorner.y = this->boundingRect.maxCorner.y;

	borderRects.trRect.minCorner.x = this->innerRect.maxCorner.x;
	borderRects.trRect.maxCorner.x = this->boundingRect.maxCorner.x;
	borderRects.trRect.minCorner.y = this->innerRect.maxCorner.y;
	borderRects.trRect.maxCorner.y = this->boundingRect.maxCorner.y;

	borderRects.blRect.minCorner.x = this->boundingRect.minCorner.x;
	borderRects.blRect.maxCorner.x = this->innerRect.minCorner.x;
	borderRects.blRect.minCorner.y = this->boundingRect.minCorner.y;
	borderRects.blRect.maxCorner.y = this->innerRect.minCorner.y;

	borderRects.brRect.minCorner.x = this->innerRect.maxCorner.x;
	borderRects.brRect.maxCorner.x = this->boundingRect.maxCorner.x;
	borderRects.brRect.minCorner.y = this->boundingRect.minCorner.y;
	borderRects.brRect.maxCorner.y = this->innerRect.minCorner.y;
}

void TopLevelWindow::SetTitle(const std::string& title)
{
	this->title = title;
}

const std::string& TopLevelWindow::GetTitle() const
{
	return this->title;
}