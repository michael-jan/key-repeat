/*
  ==============================================================================

	BottomComponent.cpp
	Created: 1 Jun 2019 8:32:26pm
	Author:  Michael Jan

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "BottomComponent.h"

//==============================================================================
BottomComponent::BottomComponent(KeyRepeatAudioProcessor& p) :
	processor(p),
	keyswitchComponent(p),
	keyswitchOctaveLabelTop("keyswitchOctaveTop"),
	keyswitchOctaveLabelBottom("keyswitchOctaveBottom")
{
	versionLabel.setText(std::string("Version ") + ProjectInfo::versionString, NotificationType::dontSendNotification);
	addAndMakeVisible(versionLabel);
	addAndMakeVisible(keyswitchComponent);

	keyswitchOctaveLabelTop.setText("Keyswitch", NotificationType::dontSendNotification);
	addAndMakeVisible(keyswitchOctaveLabelTop);

	keyswitchOctaveLabelBottom.setText("Octave", NotificationType::dontSendNotification);
	addAndMakeVisible(keyswitchOctaveLabelBottom);

	keyswitchOctaveNumbox.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
	keyswitchOctaveNumbox.setSliderSnapsToMousePosition(false);
	keyswitchOctaveNumbox.setTextBoxIsEditable(false);
	keyswitchOctaveNumbox.setLookAndFeel(&p.getNumboxLAF());
	addAndMakeVisible(keyswitchOctaveNumbox);
	keyswitchOctaveAttachment.reset(new SliderAttachment(p.getVTS(), "keyswitchOctave", keyswitchOctaveNumbox));
}

BottomComponent::~BottomComponent() {
}

void BottomComponent::paint(Graphics& g) {
	g.setColour(MyLookAndFeel::DARK_GREY);
	g.fillAll();
}

void BottomComponent::resized() {
	Rectangle<int> rect = getLocalBounds();
	keyswitchComponent.setBounds(
		rect.withSizeKeepingCentre(Utils::scale(350), rect.getHeight())
	);

	versionLabel.setFont(Font(Utils::scale(MyLookAndFeel::LABEL_FONT_SIZE)));
	versionLabel.setAlpha(0.15f);
	versionLabel.setJustificationType(Justification::centred);
	versionLabel.setBounds(
		getLocalBounds()
		.removeFromBottom(MyLookAndFeel::LABEL_FONT_SIZE + Utils::scale(8))
		.removeFromRight(Utils::scale(90))
	);

	Rectangle<int> keyswitchOctaveSide = getLocalBounds().removeFromLeft(Utils::scale(64.5));
	keyswitchOctaveNumbox.setBounds(keyswitchOctaveSide.removeFromTop(rect.getHeight() * 11 / 16));
	keyswitchOctaveNumbox.setMouseDragSensitivity(Utils::scale(65));

	keyswitchOctaveLabelTop.setFont(MyLookAndFeel::getFontLight().withHeight(Utils::scale(11)));
	keyswitchOctaveLabelTop.setJustificationType(Justification::centredTop);
	keyswitchOctaveLabelTop.setBounds(keyswitchOctaveSide.withBottomY(keyswitchOctaveNumbox.getBottom() + Utils::scale(12)));

	keyswitchOctaveLabelBottom.setFont(MyLookAndFeel::getFontLight().withHeight(Utils::scale(11)));
	keyswitchOctaveLabelBottom.setJustificationType(Justification::centredTop);
	keyswitchOctaveLabelBottom.setBounds(keyswitchOctaveSide.withBottomY(keyswitchOctaveLabelTop.getBottom() + Utils::scale(10)));

}
