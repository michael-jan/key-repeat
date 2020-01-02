/*
  ==============================================================================

	KeyswitchControlsComponent.cpp
	Created: 15 Jun 2019 2:52:58pm
	Author:  Michael Jan

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "KeyswitchControlsComponent.h"

//==============================================================================
KeyswitchControlsComponent::KeyswitchControlsComponent(KeyRepeatAudioProcessor& p) :
	processor(p),
	latchLabel("latchLabel", "Latch"),
	easyLabel("easyLabel", "EZ Triplet")
{
	addAndMakeVisible(easyLabel);
	addAndMakeVisible(latchLabel);

	latchSwitch.setName("latch");
	addAndMakeVisible(latchSwitch);
	latchAttachment.reset(new ButtonAttachment(p.getVTS(), "latch", latchSwitch));

	easySwitch.setName("easy");
	addAndMakeVisible(easySwitch);
	easyAttachment.reset(new ButtonAttachment(p.getVTS(), "easy", easySwitch));
}

KeyswitchControlsComponent::~KeyswitchControlsComponent() {
}

void KeyswitchControlsComponent::paint(Graphics& g) {
}

void KeyswitchControlsComponent::resized() {
	Rectangle<int> rect = getLocalBounds();
	int switchHeight = Utils::scale(20);
	int labelOffset = Utils::scale(10);

	rect.removeFromTop(Utils::scale(37.5));
	latchSwitch.setBounds(rect.removeFromTop(switchHeight));
	Utils::attachToComponent(latchLabel, latchSwitch, labelOffset - Utils::scale(1));

	rect.removeFromTop(Utils::scale(46));
	easySwitch.setBounds(rect.removeFromTop(switchHeight));
	Utils::attachToComponent(easyLabel, easySwitch, labelOffset + Utils::scale(1.2));
}
