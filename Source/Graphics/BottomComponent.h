/*
  ==============================================================================

	BottomComponent.h
	Created: 1 Jun 2019 8:32:26pm
	Author:  Michael Jan

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Processing/PluginProcessor.h"
#include "KeyswitchControlsComponent.h"

//==============================================================================
/*
*/
class BottomComponent : public Component {
public:
	BottomComponent(KeyRepeatAudioProcessor& p);
	~BottomComponent();

	void paint(Graphics&) override;
	void resized() override;

private:

	KeyRepeatAudioProcessor& processor;
	KeyswitchControlsComponent keyswitchControlsComponent;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BottomComponent)
};
