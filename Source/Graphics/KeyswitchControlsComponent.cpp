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
	easyLabel("easyLabel", "Mode"),
	latchLabel("latchLabel", "Latch"),
	keyswitchOctaveLabel("keyswitchOctaveLabel", "Keyswitch Octave"),
	keyswitchOctaveKnob(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox)
{

	addAndMakeVisible(easyLabel);
	addAndMakeVisible(latchLabel);
	addAndMakeVisible(keyswitchOctaveLabel);

	easySwitch.setName("easy");
	addAndMakeVisible(easySwitch);
	easyAttachment.reset(new ButtonAttachment(p.getVTS(), "easy", easySwitch));

	latchSwitch.setName("latch");
	addAndMakeVisible(latchSwitch);
	latchAttachment.reset(new ButtonAttachment(p.getVTS(), "latch", latchSwitch));

	addAndMakeVisible(keyswitchOctaveKnob);
	keyswitchOctaveAttachment.reset(new SliderAttachment(p.getVTS(), "keyswitchOctave", keyswitchOctaveKnob));
}

KeyswitchControlsComponent::~KeyswitchControlsComponent() {
}

void KeyswitchControlsComponent::paint(Graphics& g) {
	g.setColour(Colours::orange);
	g.drawRect(getLocalBounds(), 2.0f);
}

void KeyswitchControlsComponent::resized() {
	Rectangle<int> rect = getLocalBounds();

	Rectangle<int> left = rect.removeFromLeft(getWidth() * 2 / 7);
	left.removeFromTop(getHeight() / 8);
	left.removeFromBottom(getHeight() / 8);

	Rectangle<int> latchSwitchBounds = left.removeFromTop(left.getHeight() / 3);
	latchSwitch.setBounds(latchSwitchBounds);
	latchSwitch.changeWidthToFitText();
	latchSwitch.setCentrePosition(latchSwitchBounds.getCentre());

	Rectangle<int> easySwitchBounds = left.removeFromTop(left.getHeight() / 3);
	easySwitch.setBounds(easySwitchBounds);
	easySwitch.changeWidthToFitText();
	easySwitch.setCentrePosition(easySwitchBounds.getCentre());

	keyswitchOctaveKnob.setBounds(left);

}
