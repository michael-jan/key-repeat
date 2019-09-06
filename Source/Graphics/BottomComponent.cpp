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
	processor(p), keyswitchComponent(p)
{
	addAndMakeVisible(keyswitchComponent);
}

BottomComponent::~BottomComponent() {
}

void BottomComponent::paint(Graphics& g) {
	g.setColour(MyLookAndFeel::DARK_GREY);
	g.fillAll();
}

void BottomComponent::resized() {
	Rectangle<int> rect = getLocalBounds();
	keyswitchComponent.setBounds(
		rect.withSizeKeepingCentre(Utils::scale(350), rect.getHeight())
	);
}
