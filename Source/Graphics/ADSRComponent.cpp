/*
  ==============================================================================

	ADSRComponent.cpp
	Created: 19 Jun 2019 12:23:07pm
	Author:  Michael Jan

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "ADSRComponent.h"
#include "Utils.h"

//==============================================================================
ADSRComponent::ADSRComponent(KeyRepeatAudioProcessor& p) : 
	processor(p),
	attackSlider(Slider::SliderStyle::LinearBarVertical, Slider::TextEntryBoxPosition::NoTextBox),
	attackLabel("attackLabel", "A"),
	decaySlider(Slider::SliderStyle::LinearBarVertical, Slider::TextEntryBoxPosition::NoTextBox),
	decayLabel("decayLabel", "D"),
	sustainSlider(Slider::SliderStyle::LinearBarVertical, Slider::TextEntryBoxPosition::NoTextBox),
	sustainLabel("sustainLabel", "S"),
	releaseSlider(Slider::SliderStyle::LinearBarVertical, Slider::TextEntryBoxPosition::NoTextBox),
	releaseLabel("releaseLabel", "R")
{
	addAndMakeVisible(attackLabel);
	addAndMakeVisible(decayLabel);
	addAndMakeVisible(sustainLabel);
	addAndMakeVisible(releaseLabel);

	addAndMakeVisible(attackSlider);
	attackAttachment.reset(new SliderAttachment(p.getVTS(), "attack", attackSlider));

	addAndMakeVisible(decaySlider);
	decayAttachment.reset(new SliderAttachment(p.getVTS(), "decay", decaySlider));

	addAndMakeVisible(sustainSlider);
	sustainAttachment.reset(new SliderAttachment(p.getVTS(), "sustain", sustainSlider));

	addAndMakeVisible(releaseSlider);
	releaseAttachment.reset(new SliderAttachment(p.getVTS(), "release", releaseSlider));
}

ADSRComponent::~ADSRComponent() {
}

void ADSRComponent::paint(Graphics& g) {
}

void ADSRComponent::resized() {
	Rectangle<int> rect = getLocalBounds()
		.withTrimmedTop(Utils::scale(25))
		.withTrimmedBottom(Utils::scale(44.5f));

	int sliderWidth = Utils::scale(34);
	int labelOffset = Utils::scale(3);

	attackSlider.setBounds(rect.removeFromLeft(sliderWidth));
	Utils::attachToComponent(attackLabel, attackSlider, labelOffset);

	decaySlider.setBounds(rect.removeFromLeft(sliderWidth));
	Utils::attachToComponent(decayLabel, decaySlider, labelOffset);

	sustainSlider.setBounds(rect.removeFromLeft(sliderWidth));
	Utils::attachToComponent(sustainLabel, sustainSlider, labelOffset);

	releaseSlider.setBounds(rect.removeFromLeft(sliderWidth));
	Utils::attachToComponent(releaseLabel, releaseSlider, labelOffset);
}