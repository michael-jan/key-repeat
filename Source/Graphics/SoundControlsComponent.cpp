/*
  ==============================================================================

	MiddleKnobsComponent.cpp
	Created: 4 Jun 2019 3:14:12am
	Author:  Michael Jan

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "SoundControlsComponent.h"
#include "Utils.h"

//==============================================================================
SoundControlsComponent::SoundControlsComponent(KeyRepeatAudioProcessor& p) :
	processor(p),
	humanizeKnob(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox),
	humanizeLabel("humanizeLabel", "Humanize"),
	swingKnob(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox),
	swingLabel("swingLabel", "Swing")
{
	addAndMakeVisible(humanizeLabel);
	addAndMakeVisible(swingLabel);

	addAndMakeVisible(humanizeKnob);
	humanizeAttachment.reset(new SliderAttachment(p.getVTS(), "humanize", humanizeKnob));

	addAndMakeVisible(swingKnob);
	swingAttachment.reset(new SliderAttachment(p.getVTS(), "swing", swingKnob));
}

SoundControlsComponent::~SoundControlsComponent() {
}

void SoundControlsComponent::paint(Graphics& g) {
}

void SoundControlsComponent::resized() {
	Rectangle<int> rect = getLocalBounds();

	rect.removeFromTop(Utils::scale(24));
	humanizeKnob.setBounds(rect.removeFromTop(Utils::scale(40)));
	Utils::attachToComponent(humanizeLabel, humanizeKnob, Utils::scale(2));

	rect.removeFromTop(Utils::scale(27));
	swingKnob.setBounds(rect.removeFromTop(Utils::scale(40)));
	Utils::attachToComponent(swingLabel, swingKnob, Utils::scale(2));
}