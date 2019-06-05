/*
  ==============================================================================

	MiddleKnobsComponent.cpp
	Created: 4 Jun 2019 3:14:12am
	Author:  Michael Jan

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "SoundKnobsComponent.h"

//==============================================================================
SoundKnobsComponent::SoundKnobsComponent(KeyRepeatAudioProcessor& p) :
	processor(p),
	attackSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::TextBoxBelow),
	decaySlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::TextBoxBelow),
	sustainSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::TextBoxBelow),
	releaseSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::TextBoxBelow),
	swingKnob(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextEntryBoxPosition::TextBoxBelow)
{
	addAndMakeVisible(attackSlider);
	attackAttachment.reset(new SliderAttachment(p.getVTS(), "attack", attackSlider));

	addAndMakeVisible(decaySlider);
	decayAttachment.reset(new SliderAttachment(p.getVTS(), "decay", decaySlider));

	addAndMakeVisible(sustainSlider);
	sustainAttachment.reset(new SliderAttachment(p.getVTS(), "sustain", sustainSlider));

	addAndMakeVisible(releaseSlider);
	releaseAttachment.reset(new SliderAttachment(p.getVTS(), "release", releaseSlider));

	addAndMakeVisible(swingKnob);
	swingAttachment.reset(new SliderAttachment(p.getVTS(), "swing", swingKnob));
}

SoundKnobsComponent::~SoundKnobsComponent() {
}

void SoundKnobsComponent::paint(Graphics& g) {
}

void SoundKnobsComponent::resized() {
	Rectangle<int> rect = getLocalBounds();
	attackSlider.setBounds(rect.removeFromLeft(getWidth() / 8).reduced(getWidth() / 60));
	decaySlider.setBounds(rect.removeFromLeft(getWidth() / 8).reduced(getWidth() / 60));
	sustainSlider.setBounds(rect.removeFromLeft(getWidth() / 8).reduced(getWidth() / 60));
	releaseSlider.setBounds(rect.removeFromLeft(getWidth() / 8).reduced(getWidth() / 60));
	swingKnob.setBounds(rect.removeFromLeft(getWidth() / 4).reduced(getWidth() / 50));
}
