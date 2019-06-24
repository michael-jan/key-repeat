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
BottomComponent::BottomComponent(KeyRepeatAudioProcessor& p) : 
	processor(p),
	keyswitchControlsComponent(p)
{
	addAndMakeVisible(keyswitchControlsComponent);
}

BottomComponent::~BottomComponent() {
}

void BottomComponent::paint(Graphics& g) {
}

void BottomComponent::resized() {
	Rectangle<int> rect = getLocalBounds();
	keyswitchControlsComponent.setBounds(rect.removeFromLeft(getWidth() / 3));
}
