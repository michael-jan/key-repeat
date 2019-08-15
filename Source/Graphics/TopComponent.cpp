/*
  ==============================================================================

	TopComponent.cpp
	Created: 1 Jun 2019 8:32:14pm
	Author:  Michael Jan

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "TopComponent.h"

//==============================================================================
TopComponent::TopComponent(KeyRepeatAudioProcessor& p) :
	processor(p),
	pitchKnob(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox),
	pitchLabel("pitchLabel", "Pitch"),
	panKnob(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox),
	panLabel("panLabel", "Pan"),
	levelKnob(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox),
	levelLabel("levelLabel", "Level"),
	titleLabel("titleLabel", "Key Repeat"),
	authorLabel("authorLabel", "by Michael Jan")
{
	addAndMakeVisible(pitchLabel);
	addAndMakeVisible(panLabel);
	addAndMakeVisible(levelLabel);
	addAndMakeVisible(titleLabel);
	addAndMakeVisible(authorLabel);

	addAndMakeVisible(pitchKnob);
	pitchAttachment.reset(new SliderAttachment(p.getVTS(), "pitch", pitchKnob));

	addAndMakeVisible(panKnob);
	panAttachment.reset(new SliderAttachment(p.getVTS(), "pan", panKnob));

	addAndMakeVisible(levelKnob);
	levelAttachment.reset(new SliderAttachment(p.getVTS(), "level", levelKnob));
}

TopComponent::~TopComponent() {
}

void TopComponent::paint(Graphics& g) {
	g.setColour(MyLookAndFeel::DARK_GREY);
	g.fillAll();
}

void TopComponent::resized() {
	Rectangle<int> controlsRect = getLocalBounds()
		.withTrimmedRight(getWidth() / 35)
		.withTrimmedTop(getHeight() / 32)
		.withTrimmedBottom(getHeight() * 7/32);

	int knobWidth = getWidth() / 10;
	int labelOffset = -(levelLabel.getFont().getHeight() / 2);

	levelKnob.setBounds(controlsRect.removeFromRight(knobWidth));
	Utils::attachToSlider(levelLabel, levelKnob, labelOffset);

	panKnob.setBounds(controlsRect.removeFromRight(knobWidth));
	Utils::attachToSlider(panLabel, panKnob, labelOffset);

	pitchKnob.setBounds(controlsRect.removeFromRight(knobWidth));
	Utils::attachToSlider(pitchLabel, pitchKnob, labelOffset);

	int bigFontSize = getHeight() * 4/9;

	Rectangle<int> titleRect = getLocalBounds()
		.withTrimmedLeft(getWidth() / 22);
	titleLabel.setFont(Font(bigFontSize, Font::bold));
	titleLabel.setJustificationType(Justification::centredLeft);
	titleLabel.setBounds(titleRect);

	authorLabel.setFont(Font(bigFontSize));
	authorLabel.setJustificationType(Justification::centredLeft);
	authorLabel.setAlpha(0.2f);
	authorLabel.setBounds(titleRect.withTrimmedLeft(Utils::getTextWidth(titleLabel) + bigFontSize / 3));
}
