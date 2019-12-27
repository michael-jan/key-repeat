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
	if (whatToDisplay->isEnabled && whatToDisplay->isActive) {
		int sideRemoveAmount = jmax(getLocalBounds().getWidth() / 20, 1);
		Rectangle<int> fillBounds = getLocalBounds();
		fillBounds.removeFromLeft(sideRemoveAmount);
		fillBounds.removeFromRight(sideRemoveAmount);
		g.setColour(MyLookAndFeel::BLACK.withAlpha(0.2f));
		g.fillRect(fillBounds);
	}
}

void SingleKeyswitchComponent::resized() {
	Rectangle<int> noteBounds = getLocalBounds().withTrimmedTop(Utils::scale(20));
	noteLabel.setFont(MyLookAndFeel::getFontLight().withHeight(Utils::scale(MyLookAndFeel::SMALL_FONT_SIZE)));
	noteLabel.setJustificationType(Justification::centredTop);
	noteLabel.setBounds(noteBounds);

	Rectangle<int> typeBounds = getLocalBounds().withTrimmedTop(Utils::scale(31));
	typeLabel.setFont(MyLookAndFeel::getFontRegular().withHeight(Utils::scale(MyLookAndFeel::LABEL_FONT_SIZE)));
	typeLabel.setJustificationType(Justification::centredTop);
	typeLabel.setBounds(typeBounds);
}

void SingleKeyswitchComponent::setDisplayInfoElement(KeyswitchDisplayInfoElement *displayInfoElement) {
	whatToDisplay = displayInfoElement;
	noteLabel.setText(whatToDisplay->noteName, NotificationType::dontSendNotification);
	typeLabel.setText(whatToDisplay->keyswitchName, NotificationType::dontSendNotification);
	noteLabel.setAlpha(whatToDisplay->isEnabled ? 1.0f : 0.3f);
	typeLabel.setAlpha(whatToDisplay->isEnabled ? 1.0f : 0.3f);
}

KeyswitchComponent::KeyswitchComponent(KeyRepeatAudioProcessor& p) : 
	processor(p),
	keyswitchManager(processor.getKeyswitchManager())
{
 	for (int i = 0; i < KeyswitchManager::NUM_KEYSWITCH_KEYS; i++) {
		keyswitches.add(new SingleKeyswitchComponent(p));
 	}
	updateDisplayElements();
	for (int i = 0; i < KeyswitchManager::NUM_KEYSWITCH_KEYS; i++) {
 		addAndMakeVisible(keyswitches[i]);
	}
	startTimerHz(30);
}

KeyswitchComponent::~KeyswitchComponent() {
}

void KeyswitchComponent::paint(Graphics& g) {
}

void KeyswitchComponent::resized() {
	Rectangle<float> rect = getLocalBounds().toFloat();
	for (int i = 0; i < keyswitches.size(); i++) {
		keyswitches[i]->setBounds(rect.removeFromLeft(getWidth() / 7.0f).toNearestInt());
	}
}

void KeyswitchComponent::updateDisplayElements() {
	KeyswitchDisplayInfoElement *displayElements = keyswitchManager.getDisplayElements();
	for (int i = 0; i < keyswitches.size(); i++) {
		keyswitches[i]->setDisplayInfoElement(&displayElements[i]);
	}
}

void KeyswitchComponent::timerCallback() {
	updateDisplayElements();
	repaint();
}
