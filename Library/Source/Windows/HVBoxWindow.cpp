#include "HVBoxWindow.h"
#include <assert.h>

using namespace VeryGUI;

//----------------------------------------- HVBoxWindow -----------------------------------------

HVBoxWindow::HVBoxWindow()
{
}

/*virtual*/ HVBoxWindow::~HVBoxWindow()
{
}

bool HVBoxWindow::AddChildWindowWithSizingConfig(std::shared_ptr<Window> window, SizingConfig sizingConfig)
{
	if (this->childWindowArray.size() != this->sizingConfigArray.size())
		return false;

	if (!this->AddChildWindow(window))
		return false;

	this->sizingConfigArray.push_back(sizingConfig);
	return true;
}

void HVBoxWindow::CalcLayout(std::vector<double>& lengthArray)
{
	assert(this->childWindowArray.size() == this->sizingConfigArray.size());

	double actualTotalLengthAvailable = this->GetTotalLengthAvailable();
	double minimumLengthNeeded = 0.0;
	double totalProportion = 0.0;

	for (int i = 0; i < (int)this->childWindowArray.size(); i++)
	{
		const SizingConfig& sizingConfig = this->sizingConfigArray[i];

		switch (sizingConfig.constraint)
		{
			case SizeConstraint::FIXED:
			{
				minimumLengthNeeded += sizingConfig.length;
				break;
			}
			case SizeConstraint::FIT_TO_CONTENT:
			{
				double desiredLength = this->GetChildWindowDesiredLength(i);
				assert(desiredLength > 0.0);
				minimumLengthNeeded += desiredLength;
				break;
			}
			case SizeConstraint::PROPORTIONAL:
			{
				minimumLengthNeeded += 5.0;
				totalProportion += sizingConfig.proportion;
				break;
			}
		}
	}

	double totalLengthAvailable = actualTotalLengthAvailable;
	if (minimumLengthNeeded > totalLengthAvailable)
		totalLengthAvailable = minimumLengthNeeded;

	double totalLengthRemaining = totalLengthAvailable;
	lengthArray.resize((int)this->childWindowArray.size());
	for (int i = 0; i < (int)this->childWindowArray.size(); i++)
	{
		const SizingConfig& sizingConfig = this->sizingConfigArray[i];

		double length = 0.0;
		switch (sizingConfig.constraint)
		{
			case SizeConstraint::FIXED:
			{
				length = sizingConfig.length;
				break;
			}
			case SizeConstraint::FIT_TO_CONTENT:
			{
				length = this->GetChildWindowDesiredLength(i);
				break;
			}
		}

		if (length != 0.0)
		{
			lengthArray[i] = length;
			totalLengthRemaining -= length;
		}
	}

	for (int i = 0; i < (int)this->childWindowArray.size(); i++)
	{
		const SizingConfig& sizingConfig = this->sizingConfigArray[i];

		if (sizingConfig.constraint != SizeConstraint::PROPORTIONAL)
			continue;

		lengthArray[i] = totalLengthRemaining * (sizingConfig.proportion / totalProportion);
	}

	if (actualTotalLengthAvailable < totalLengthAvailable)
	{
		double scale = actualTotalLengthAvailable / totalLengthAvailable;
		for (double& length : lengthArray)
			length *= scale;
	}
}

//----------------------------------------- HBoxWindow -----------------------------------------

HBoxWindow::HBoxWindow()
{
}

/*virtual*/ HBoxWindow::~HBoxWindow()
{
}

/*virtual*/ void HBoxWindow::LayoutChildren(GAL2D::GraphicsInterface* graphics)
{
	std::vector<double> lengthArray;
	this->CalcLayout(lengthArray);
	
	GAL2D::Vector location = this->boundingRect.GetCorner(GAL2D::Rectangle::Corner::UPPER_LEFT);
	for (int i = 0; i < (int)this->childWindowArray.size(); i++)
	{
		std::shared_ptr<Window> childWindow = this->childWindowArray[i];
		GAL2D::Rectangle childRect(lengthArray[i], this->boundingRect.Height());
		childRect += childRect.GetTranslation(location, GAL2D::Rectangle::Corner::UPPER_LEFT);
		childWindow->SetBoundingRectangle(childRect);
		location.x += lengthArray[i];
	}

	Window::LayoutChildren(graphics);
}

/*virtual*/ double HBoxWindow::GetChildWindowDesiredLength(int i)
{
	return this->childWindowArray[i]->GetDesiredWidth();
}

/*virtual*/ double HBoxWindow::GetTotalLengthAvailable()
{
	return this->boundingRect.Width();
}

//----------------------------------------- VBoxWindow -----------------------------------------

VBoxWindow::VBoxWindow()
{
}

/*virtual*/ VBoxWindow::~VBoxWindow()
{
}

/*virtual*/ void VBoxWindow::LayoutChildren(GAL2D::GraphicsInterface* graphics)
{
	std::vector<double> lengthArray;
	this->CalcLayout(lengthArray);

	GAL2D::Vector location = this->boundingRect.GetCorner(GAL2D::Rectangle::Corner::UPPER_LEFT);
	for (int i = 0; i < (int)this->childWindowArray.size(); i++)
	{
		std::shared_ptr<Window> childWindow = this->childWindowArray[i];
		GAL2D::Rectangle childRect(this->boundingRect.Width(), lengthArray[i]);
		childRect += childRect.GetTranslation(location, GAL2D::Rectangle::Corner::UPPER_LEFT);
		childWindow->SetBoundingRectangle(childRect);
		location.y -= lengthArray[i];
	}

	Window::LayoutChildren(graphics);
}

/*virtual*/ double VBoxWindow::GetChildWindowDesiredLength(int i)
{
	return this->childWindowArray[i]->GetDesiredHeight();
}

/*virtual*/ double VBoxWindow::GetTotalLengthAvailable()
{
	return this->boundingRect.Height();
}