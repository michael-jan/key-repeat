/*
  ==============================================================================

	KeyswitchControlsComponent.h
	Created: 15 Jun 2019 2:52:58pm
	Author:  Michael Jan

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Processing/PluginProcessor.h"
#include "ParameterLabel.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

//==============================================================================
/*
*/
class KeyswitchControlsComponent : public Component {
public:
	KeyswitchControlsComponent(KeyRepeatAudioProcessor& p);
	~KeyswitchControlsComponent();

	void paint(Graphics&) override;
	void resized() override;

private:

	KeyRepeatAudioProcessor& processor;

	ToggleButton easySwitch;
	std::unique_ptr<ButtonAttachment> easyAttachment;
	ParameterLabel easyLabel;

	ToggleButton latchSwitch;
	std::unique_ptr<ButtonAttachment> latchAttachment;
	ParameterLabel latchLabel;

	Slider keyswitchOctaveKnob;
	std::unique_ptr<SliderAttachment> keyswitchOctaveAttachment;
	ParameterLabel keyswitchOctaveLabel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyswitchControlsComponent)
};
