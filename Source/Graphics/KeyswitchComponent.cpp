/*
  ==============================================================================

	KeyswitchComponent.cpp
	Created: 17 Jun 2019 12:03:07am
	Author:  Michael Jan

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "KeyswitchComponent.h"

//==============================================================================

SingleKeyswitchComponent::SingleKeyswitchComponent(KeyRepeatAudioProcessor& p) :
	processor(p),
	whatToDisplay(),
	noteLabel("note"),
	typeLabel("type") 
{
	addAndMakeVisible(noteLabel);
	addAndMakeVisible(typeLabel);
}

SingleKeyswitchComponent::~SingleKeyswitchComponent() {
}

void SingleKeyswitchComponent::paint(Graphics& g) {
}

void SingleKeyswitchComponent::resized() {
	Rectangle<int> noteBounds = getLocalBounds().withTrimmedTop(Utils::scale(20));
	noteLabel.setText(whatToDisplay->noteName, NotificationType::dontSendNotification);
	noteLabel.setFont(MyLookAndFeel::getFontLight().withHeight(Utils::scale(11)));
	noteLabel.setJustificationType(Justification::centredTop);
	noteLabel.setBounds(noteBounds);

	Rectangle<int> typeBounds = getLocalBounds().withTrimmedTop(Utils::scale(31));
	typeLabel.setText(whatToDisplay->keyswitchName, NotificationType::dontSendNotification);
	typeLabel.setFont(MyLookAndFeel::getFontRegular().withHeight(Utils::scale(MyLookAndFeel::LABEL_FONT_SIZE)));
	typeLabel.setJustificationType(Justification::centredTop);
	typeLabel.setBounds(typeBounds);
}

void SingleKeyswitchComponent::setDisplayInfoElement(KeyswitchDisplayInfoElement *displayInfoElement) {
	whatToDisplay = displayInfoElement;
}

KeyswitchComponent::KeyswitchComponent(KeyRepeatAudioProcessor& p) : 
	processor(p),
	keyswitchManager(processor.getKeyswitchManager())
{
	for (int i = 0; i < KeyswitchManager::NUM_KEYSWITCH_KEYS; i++) {
		keyswitches.add(new SingleKeyswitchComponent(p));
		addAndMakeVisible(keyswitches[i]);
	}
}

KeyswitchComponent::~KeyswitchComponent() {
}

void KeyswitchComponent::paint(Graphics& g) {
}

void KeyswitchComponent::resized() {
	Rectangle<float> rect = getLocalBounds().toFloat();
	KeyswitchDisplayInfoElement *displayElements = keyswitchManager.getDisplayElements();
	for (int i = 0; i < keyswitches.size(); i++) {
		keyswitches[i]->setDisplayInfoElement(&displayElements[i]);
		keyswitches[i]->setBounds(rect.removeFromLeft(getWidth() / 7.0f).toNearestInt());
	}
}
