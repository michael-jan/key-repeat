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
	swingKnob(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox),
	swingLabel("swingLabel", "Swing"),
	humanizeKnob(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox),
	humanizeLabel("humanizeLabel", "Humanize")
{
	addAndMakeVisible(swingLabel);
	addAndMakeVisible(humanizeLabel);

	addAndMakeVisible(swingKnob);
	swingAttachment.reset(new SliderAttachment(p.getVTS(), "swing", swingKnob));

	addAndMakeVisible(humanizeKnob);
	humanizeAttachment.reset(new SliderAttachment(p.getVTS(), "humanize", humanizeKnob));
}

SoundControlsComponent::~SoundControlsComponent() {
}

void SoundControlsComponent::paint(Graphics& g) {
}

void SoundControlsComponent::resized() {
	Rectangle<int> rect = getLocalBounds()
		.reduced(getHeight() / 8)
		.withTrimmedBottom(getHeight() / 8)
		.withTrimmedLeft(getWidth() / 70);

	swingKnob.setBounds(rect.removeFromLeft(rect.getWidth() / 2).reduced(rect.getWidth() / 100));
	Utils::attachToSlider(swingLabel, swingKnob);

	humanizeKnob.setBounds(rect.reduced(rect.getWidth() / 100));
	Utils::attachToSlider(humanizeLabel, humanizeKnob);
}