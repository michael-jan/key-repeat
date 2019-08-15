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
	attackSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox),
	attackLabel("attackLabel", "Atk"),
	decaySlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox),
	decayLabel("decayLabel", "Dec"),
	sustainSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox),
	sustainLabel("sustainLabel", "Sus"),
	releaseSlider(Slider::SliderStyle::LinearVertical, Slider::TextEntryBoxPosition::NoTextBox),
	releaseLabel("releaseLabel", "Rel")
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
		.reduced(getHeight() / 8)
		.withTrimmedBottom(getHeight() / 8)
		.withTrimmedRight(getWidth() / 70);

	int sliderWidth = rect.getWidth() / 4;
	int sliderReduce = rect.getWidth() / 60;

	attackSlider.setBounds(rect.removeFromLeft(sliderWidth).reduced(sliderReduce));
	Utils::attachToSlider(attackLabel, attackSlider);

	decaySlider.setBounds(rect.removeFromLeft(sliderWidth).reduced(sliderReduce));
	Utils::attachToSlider(decayLabel, decaySlider);

	sustainSlider.setBounds(rect.removeFromLeft(sliderWidth).reduced(sliderReduce));
	Utils::attachToSlider(sustainLabel, sustainSlider);

	releaseSlider.setBounds(rect.removeFromLeft(sliderWidth).reduced(sliderReduce));
	Utils::attachToSlider(releaseLabel, releaseSlider);
}