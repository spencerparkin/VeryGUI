#include "Windows/TopLevelWindow.h"
#include "Windows/DesktopWindow.h"
#include <assert.h>

using namespace VeryGUI;

double TopLevelWindow::borderMargin = 5.0;
double TopLevelWindow::titleBarThickness = 20.0;

TopLevelWindow::TopLevelWindow()
{
	this->mouseAction = MouseAction::NONE;
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

/*virtual*/ void TopLevelWindow::Draw(GAL2D::GraphicsInterface* graphics)
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

	if (!this->titleFont.get())
		this->titleFont = graphics->MakeFont("Fonts\\Roboto_Regular.ttf");

	GAL2D::Rectangle textRect = this->titleBarRect;
	textRect.ApplyMarginDelta(-2.0);
	graphics->RenderText(this->title, this->titleFont, textRect, GAL2D::Color(1.0, 1.0, 1.0, 1.0), GAL2D::GraphicsInterface::ALIGN_LEFT);

	Window::Draw(graphics);
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

/*virtual*/ void TopLevelWindow::HandleEvent(EventType eventType, const void* eventData)
{
	switch (eventType)
	{
		case EventType::MOUSE_CLICK:
		{
			auto* event = static_cast<const MouseClickEvent*>(eventData);

			if (event->mouseButton == GAL2D::MouseButton::Left)
			{
				std::shared_ptr<DesktopWindow> desktopWindow = std::dynamic_pointer_cast<DesktopWindow>(this->GetRootWindow());
				if (desktopWindow.get())
				{
					switch (event->buttonState)
					{
						case GAL2D::ButtonState::Down:
						{
							BorderRects borderRects;
							this->CalcBorderRects(borderRects);

							if (this->titleBarRect.ContainsPoint(event->mousePosition))
							{
								this->mouseAction = MouseAction::CHANGING_LOCATION;
								this->dragDeltaMin = event->mousePosition - this->boundingRect.minCorner;
								this->dragDeltaMax = event->mousePosition - this->boundingRect.maxCorner;
							}
							else if (borderRects.lRect.ContainsPoint(event->mousePosition))
							{
								this->mouseAction = MouseAction::CHANGING_MIN_X;
								this->dragDeltaMin.x = event->mousePosition.x - this->boundingRect.minCorner.x;
							}
							else if (borderRects.rRect.ContainsPoint(event->mousePosition))
							{
								this->mouseAction = MouseAction::CHANGING_MAX_X;
								this->dragDeltaMax.x = event->mousePosition.x - this->boundingRect.maxCorner.x;
							}
							else if (borderRects.bRect.ContainsPoint(event->mousePosition))
							{
								this->mouseAction = MouseAction::CHANGING_MIN_Y;
								this->dragDeltaMin.y = event->mousePosition.y - this->boundingRect.minCorner.y;
							}
							else if (borderRects.tRect.ContainsPoint(event->mousePosition))
							{
								this->mouseAction = MouseAction::CHANGING_MAX_Y;
								this->dragDeltaMax.y = event->mousePosition.y - this->boundingRect.maxCorner.y;
							}
							else if (borderRects.blRect.ContainsPoint(event->mousePosition))
							{
								this->mouseAction = MouseAction::CHANGING_MIN_X_MIN_Y;
								this->dragDeltaMin.x = event->mousePosition.x - this->boundingRect.minCorner.x;
								this->dragDeltaMin.y = event->mousePosition.y - this->boundingRect.minCorner.y;
							}
							else if (borderRects.brRect.ContainsPoint(event->mousePosition))
							{
								this->mouseAction = MouseAction::CHANGING_MAX_X_MIN_Y;
								this->dragDeltaMax.x = event->mousePosition.x - this->boundingRect.maxCorner.x;
								this->dragDeltaMin.y = event->mousePosition.y - this->boundingRect.minCorner.y;
							}
							else if (borderRects.tlRect.ContainsPoint(event->mousePosition))
							{
								this->mouseAction = MouseAction::CHANGING_MIN_X_MAX_Y;
								this->dragDeltaMin.x = event->mousePosition.x - this->boundingRect.minCorner.x;
								this->dragDeltaMax.y = event->mousePosition.y - this->boundingRect.maxCorner.y;
							}
							else if (borderRects.trRect.ContainsPoint(event->mousePosition))
							{
								this->mouseAction = MouseAction::CHANGING_MAX_X_MAX_Y;
								this->dragDeltaMax.x = event->mousePosition.x - this->boundingRect.maxCorner.x;
								this->dragDeltaMax.y = event->mousePosition.y - this->boundingRect.maxCorner.y;
							}

							if (this->mouseAction != MouseAction::NONE)
								desktopWindow->SetMouseCaptureWindow(this);
							
							desktopWindow->BringWindowToFront(this);
							break;
						}
						case GAL2D::ButtonState::Up:
						{
							if (this->mouseAction != MouseAction::NONE)
							{
								desktopWindow->SetMouseCaptureWindow(nullptr);
								this->mouseAction = MouseAction::NONE;
							}
							break;
						}
					}
				}
			}

			break;
		}
		case EventType::MOUSE_MOTION:
		{
			auto* event = static_cast<const MouseMotionEvent*>(eventData);

			GAL2D::Rectangle originalRect = this->boundingRect;

			switch (this->mouseAction)
			{
				case MouseAction::CHANGING_LOCATION:
				{
					this->boundingRect.minCorner = event->mousePosition - this->dragDeltaMin;
					this->boundingRect.maxCorner = event->mousePosition - this->dragDeltaMax;
					break;
				}
				case MouseAction::CHANGING_MIN_X:
				{
					this->boundingRect.minCorner.x = event->mousePosition.x - this->dragDeltaMin.x;
					break;
				}
				case MouseAction::CHANGING_MAX_X:
				{
					this->boundingRect.maxCorner.x = event->mousePosition.x - this->dragDeltaMax.x;
					break;
				}
				case MouseAction::CHANGING_MIN_Y:
				{
					this->boundingRect.minCorner.y = event->mousePosition.y - this->dragDeltaMin.y;
					break;
				}
				case MouseAction::CHANGING_MAX_Y:
				{
					this->boundingRect.maxCorner.y = event->mousePosition.y - this->dragDeltaMax.y;
					break;
				}
				case MouseAction::CHANGING_MIN_X_MIN_Y:
				{
					this->boundingRect.minCorner.x = event->mousePosition.x - this->dragDeltaMin.x;
					this->boundingRect.minCorner.y = event->mousePosition.y - this->dragDeltaMin.y;
					break;
				}
				case MouseAction::CHANGING_MAX_X_MIN_Y:
				{
					this->boundingRect.maxCorner.x = event->mousePosition.x - this->dragDeltaMax.x;
					this->boundingRect.minCorner.y = event->mousePosition.y - this->dragDeltaMin.y;
					break;
				}
				case MouseAction::CHANGING_MIN_X_MAX_Y:
				{
					this->boundingRect.minCorner.x = event->mousePosition.x - this->dragDeltaMin.x;
					this->boundingRect.maxCorner.y = event->mousePosition.y - this->dragDeltaMax.y;
					break;
				}
				case MouseAction::CHANGING_MAX_X_MAX_Y:
				{
					this->boundingRect.maxCorner.x = event->mousePosition.x - this->dragDeltaMax.x;
					this->boundingRect.maxCorner.y = event->mousePosition.y - this->dragDeltaMax.y;
					break;
				}
				case MouseAction::NONE:
				{
					BorderRects borderRects;
					this->CalcBorderRects(borderRects);

					// STPTODO: Change cursor icon here if mouse is hovering over a border rect.

					break;
				}
			}

			if (!this->boundingRect.IsValid() ||
				this->boundingRect.Width() < this->titleBarThickness * 2.0 ||
				this->boundingRect.Height() < this->titleBarThickness * 2.0)
			{
				this->boundingRect = originalRect;
			}

			break;
		}
	}
}