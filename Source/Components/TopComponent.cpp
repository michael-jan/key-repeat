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
	Colour darkGrey(35, 36, 41);

	Rectangle<int> solidRect = getLocalBounds();

	ColourGradient gradient(darkGrey.brighter(0.03f), 0, 0, darkGrey, 0, getHeight() / 2, false);
	g.setGradientFill(gradient);
	g.fillRect(solidRect);
}

void TopComponent::resized() {
}
