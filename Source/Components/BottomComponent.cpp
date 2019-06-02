/*
  ==============================================================================

	BottomComponent.cpp
	Created: 1 Jun 2019 8:32:26pm
	Author:  Michael Jan

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "BottomComponent.h"

//==============================================================================
BottomComponent::BottomComponent() {
}

BottomComponent::~BottomComponent() {
}

void BottomComponent::paint(Graphics& g) {
	g.setColour(Colours::orange);
	//g.drawRect(getLocalBounds(), 2.0f);
}

void BottomComponent::resized() {
}
