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
KeyswitchComponent::KeyswitchComponent(KeyRepeatAudioProcessor& p) : 
	processor(p),
	keyswitchManager(processor.getKeyswitchManager())
{
}

KeyswitchComponent::~KeyswitchComponent() {
}

void KeyswitchComponent::paint(Graphics& g) {
}

void KeyswitchComponent::resized() {
}
