/*
  ==============================================================================

	TopComponent.cpp
	Created: 1 Jun 2019 8:32:14pm
	Author:  Michael Jan

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "TopComponent.h"

TopComponent::TopComponent(KeyRepeatAudioProcessor& p) :
	processor(p),
	pitchKnob(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox),
	pitchLabel("pitchLabel", "Pitch"),
	panKnob(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox),
	panLabel("panLabel", "Pan"),
	levelKnob(Slider::SliderStyle::RotaryVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox),
	levelLabel("levelLabel", "Level")
{
    addAndMakeVisible(topLogoComponent);
    topLogoComponent.setBufferedToImage(true);

	addAndMakeVisible(pitchLabel);
	addAndMakeVisible(panLabel);
	addAndMakeVisible(levelLabel);

	addAndMakeVisible(pitchKnob);
	pitchKnob.getProperties().set("startFromMiddle", true);
	pitchAttachment.reset(new SliderAttachment(p.getVTS(), "pitch", pitchKnob));

	addAndMakeVisible(panKnob);
	panKnob.getProperties().set("startFromMiddle", true);
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
		.withTrimmedRight(Utils::scale(15))
		.withTrimmedTop(Utils::scale(7))
		.withTrimmedBottom(Utils::scale(22));

	int knobWidth = getWidth() / 10;
	int labelOffset = Utils::scale(2);

	levelKnob.setBounds(controlsRect.removeFromRight(knobWidth));
	Utils::attachToComponent(levelLabel, levelKnob, labelOffset);

	panKnob.setBounds(controlsRect.removeFromRight(knobWidth));
	Utils::attachToComponent(panLabel, panKnob, labelOffset);

	pitchKnob.setBounds(controlsRect.removeFromRight(knobWidth));
	Utils::attachToComponent(pitchLabel, pitchKnob, labelOffset);

    topLogoComponent.setBounds(getLocalBounds());
}

/* TopLogoComponent */

TopLogoComponent::TopLogoComponent() :
    titleLabel("titleLabel", "key repeat"),
    authorLabel("authorLabel", "by michael jan")
{
    addAndMakeVisible(titleLabel);
    addAndMakeVisible(authorLabel);
}

TopLogoComponent::~TopLogoComponent() {
}

void TopLogoComponent::paint(Graphics& g) {
    g.setColour(MyLookAndFeel::DARK_GREY);
    g.fillAll();

    Path arrow;
    Rectangle<float> arrowRectangularPart = getLocalBounds().removeFromLeft(Utils::scale(206)).toFloat();
    arrow.addRectangle(arrowRectangularPart);
    arrow.startNewSubPath(arrowRectangularPart.getTopRight().toFloat());
    arrow.lineTo(arrowRectangularPart.getRight() + Utils::scale(56), arrowRectangularPart.getCentreY());
    arrow.lineTo(arrowRectangularPart.getBottomRight());
    arrow.closeSubPath();

    arrow.applyTransform(AffineTransform::translation(Utils::scale(38), 0));
    Utils::drawPathShadow(g, arrow, Utils::scale(10), 0.08f);
    g.setColour(MyLookAndFeel::DARK_PINK);
    g.fillPath(arrow);

    arrow.applyTransform(AffineTransform::translation(Utils::scale(-19), 0));
    Utils::drawPathShadow(g, arrow, Utils::scale(11), 0.08f);
    g.setColour(MyLookAndFeel::PINK);
    g.fillPath(arrow);

    arrow.applyTransform(AffineTransform::translation(Utils::scale(-19) - 1, 0));
    Utils::drawPathShadow(g, arrow, Utils::scale(12), 0.08f);
    g.setColour(MyLookAndFeel::LIGHT_PINK);
    g.fillPath(arrow);
}

void TopLogoComponent::resized() {
    Rectangle<int> titleRect = getLocalBounds()
        .withTrimmedRight(getLocalBounds().getWidth() * 0.73)
        .withTrimmedTop(Utils::scale(11.5f));
    titleLabel.setFont(MyLookAndFeel::getFontBold().withHeight(Utils::scale(32.5)));
    titleLabel.setJustificationType(Justification::centredTop);
    titleLabel.setBounds(titleRect);
    
    static DropShadowEffect dse;
    dse.setShadowProperties({ MyLookAndFeel::BLACK.withAlpha(0.2f), Utils::scale(4), { Utils::scale(3), Utils::scale(1.5f) } });
    titleLabel.setComponentEffect(&dse);

    Rectangle<int> authorRect = getLocalBounds()
        .withTrimmedRight(getLocalBounds().getWidth() * 0.5)
        .withTrimmedTop(Utils::scale(39.5f));
    authorLabel.setFont(MyLookAndFeel::getFontLight().withHeight(Utils::scale(MyLookAndFeel::LABEL_FONT_SIZE)));
    authorLabel.setJustificationType(Justification::centredTop);
    authorLabel.setAlpha(0.60f);
    authorLabel.setBounds(authorRect);
}
