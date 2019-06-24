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
	levelLabel("levelLabel", "Level")
{
	addAndMakeVisible(pitchLabel);
	addAndMakeVisible(panLabel);
	addAndMakeVisible(levelLabel);

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
	Colour darkGrey(35, 36, 41);

	Rectangle<int> solidRect = getLocalBounds();

	//ColourGradient gradient(darkGrey.brighter(0.05f), 0, 0, darkGrey, 0, getHeight() * 2/3, false);
	//g.setGradientFill(gradient);
	g.setColour(darkGrey);
	g.fillRect(solidRect);

	g.setColour(darkGrey.brighter(0.2f));
	g.drawLine(0, getHeight(), getWidth(), getHeight());

}

void TopComponent::resized() {
	Rectangle<int> rect = getLocalBounds();
	//rect.removeFromTop(getHeight() / 9);
	rect.removeFromRight(getWidth() / 35);
	rect.removeFromBottom(getHeight() / 4);

	int knobWidth = getWidth() / 10;
	int knobReduced = getWidth() / 200;
	int labelBottom = rect.getHeight() * 49/40;

	levelKnob.setBounds(rect.removeFromRight(knobWidth));
	levelLabel.setBounds(levelKnob.getBounds().withBottom(labelBottom));

	panKnob.setBounds(rect.removeFromRight(knobWidth));
	panLabel.setBounds(panKnob.getBounds().withBottom(labelBottom));

	pitchKnob.setBounds(rect.removeFromRight(knobWidth));
	pitchLabel.setBounds(pitchKnob.getBounds().withBottom(labelBottom));
}
