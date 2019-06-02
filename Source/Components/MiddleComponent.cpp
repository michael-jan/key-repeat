/*
  ==============================================================================

	MiddleComponent.cpp
	Created: 1 Jun 2019 8:32:20pm
	Author:  Michael Jan

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "MiddleComponent.h"

//==============================================================================
MiddleComponent::MiddleComponent(KeyRepeatAudioProcessor& p) :
	processor(p),
	fileDropperComponent(p) {
	addAndMakeVisible(fileDropperComponent);
}

MiddleComponent::~MiddleComponent() {
}

void MiddleComponent::paint(Graphics& g) {
	g.setColour(Colours::purple);
	//g.drawRect(getLocalBounds(), 2.0f);
}

void MiddleComponent::resized() {
	Rectangle<int> rect = getLocalBounds();
	fileDropperComponent.setBounds( rect.removeFromRight(rect.getWidth() * 2/5).reduced(12) );
}
