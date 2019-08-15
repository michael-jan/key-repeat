/*
  ==============================================================================

	KeyswitchComponent.h
	Created: 17 Jun 2019 12:03:07am
	Author:  Michael Jan

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Processing/PluginProcessor.h"

//==============================================================================
/*
*/
class KeyswitchComponent : public Component {
public:
	KeyswitchComponent(KeyRepeatAudioProcessor& p);
	~KeyswitchComponent();

	void paint(Graphics&) override;
	void resized() override;

private:

	KeyRepeatAudioProcessor& processor;
	KeyswitchManager& keyswitchManager;
	OwnedArray<ShapeButton> buttons;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyswitchComponent)
};
