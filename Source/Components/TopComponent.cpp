/*
  ==============================================================================

	TopComponent.cpp
	Created: 1 Jun 2019 8:32:14pm
	Author:  Michael Jan

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "TopComponent.h"

//==============================================================================
TopComponent::TopComponent() {

}

TopComponent::~TopComponent() {
}

void TopComponent::paint(Graphics& g) {
	Colour lightBlack(40, 42, 47);
	Colour darkBlack(28, 29, 35);

	Rectangle<int> solidRect = getLocalBounds().removeFromTop(getHeight() * 9/10);

	Point<int> zerozero(0, 0);
	DropShadow ds(Colours::black, getHeight() / 10, zerozero);
	ds.drawForRectangle(g, solidRect);

	g.setColour(lightBlack);
	g.drawHorizontalLine(solidRect.getBottom(), 0, getWidth());

	ColourGradient gradient(lightBlack, 0, 0, darkBlack, 0, getHeight(), false);
	g.setGradientFill(gradient);
	g.fillRect(solidRect);
}

void TopComponent::resized() {
}
