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
	panKnob(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox),
	levelKnob(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox)
{
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

	ColourGradient gradient(darkGrey.brighter(0.03f), 0, 0, darkGrey, 0, getHeight() / 2, false);
	g.setGradientFill(gradient);
	g.fillRect(solidRect);
}

void TopComponent::resized() {
	Rectangle<int> rect = getLocalBounds();
	rect.removeFromRight(getWidth() / 50);
	levelKnob.setBounds(rect.removeFromRight(getWidth() / 10).reduced(getWidth() / 200));
	panKnob.setBounds(rect.removeFromRight(getWidth() / 10).reduced(getWidth() / 200));
	pitchKnob.setBounds(rect.removeFromRight(getWidth() / 10).reduced(getWidth() / 200));
}
