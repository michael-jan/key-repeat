/*
  ==============================================================================

	MiddleKnobsComponent.cpp
	Created: 4 Jun 2019 3:14:12am
	Author:  Michael Jan

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "SoundControlsComponent.h"

//==============================================================================
SoundControlsComponent::SoundControlsComponent(KeyRepeatAudioProcessor& p) :
	processor(p),
	attackSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox),
	attackLabel("attackLabel", "Atk"),
	decaySlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox),
	decayLabel("decayLabel", "Dec"),
	sustainSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox),
	sustainLabel("sustainLabel", "Sus"),
	releaseSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox),
	releaseLabel("releaseLabel", "Rel"),
	swingKnob(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox),
	swingLabel("swingLabel", "Swing"),
	humanizeKnob(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox),
	humanizeLabel("humanizeLabel", "Humanize")
{
	addAndMakeVisible(attackLabel);
	addAndMakeVisible(decayLabel);
	addAndMakeVisible(sustainLabel);
	addAndMakeVisible(releaseLabel);
	addAndMakeVisible(swingLabel);
	addAndMakeVisible(humanizeLabel);

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

	addAndMakeVisible(humanizeKnob);
	humanizeAttachment.reset(new SliderAttachment(p.getVTS(), "humanize", humanizeKnob));
}

SoundControlsComponent::~SoundControlsComponent() {
}

void SoundControlsComponent::paint(Graphics& g) {
}

void SoundControlsComponent::resized() {
	Rectangle<int> rect = getLocalBounds().removeFromTop(getHeight() * 5/6);
	rect.removeFromTop(getHeight() / 20);
	
	int sliderWidth = getWidth() / 9;
	int sliderReduce = getWidth() / 60;

	attackSlider.setBounds(rect.removeFromLeft(sliderWidth).reduced(sliderReduce));
	attachToSlider(attackLabel, attackSlider);

	decaySlider.setBounds(rect.removeFromLeft(sliderWidth).reduced(sliderReduce));
	attachToSlider(decayLabel, decaySlider);

	sustainSlider.setBounds(rect.removeFromLeft(sliderWidth).reduced(sliderReduce));
	attachToSlider(sustainLabel, sustainSlider);

	releaseSlider.setBounds(rect.removeFromLeft(sliderWidth).reduced(sliderReduce));
	attachToSlider(releaseLabel, releaseSlider);

	rect.removeFromLeft(getWidth() / 20);

	swingKnob.setBounds(rect.removeFromLeft(getWidth() / 4).reduced(getWidth() / 50));
	attachToSlider(swingLabel, swingKnob);

	humanizeKnob.setBounds(rect.removeFromLeft(getWidth() / 4).reduced(getWidth() / 50));
	attachToSlider(humanizeLabel, humanizeKnob);

}

void SoundControlsComponent::attachToSlider(ParameterLabel& label, Slider& slider) {
	label.setBounds(slider.getBounds().withSizeKeepingCentre(getWidth() / 4, slider.getHeight() * 4 / 3));
}
