/*
  ==============================================================================

	TopComponent.h
	Created: 1 Jun 2019 8:32:14pm
	Author:  Michael Jan

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Processing/PluginProcessor.h"
#include "ParameterLabel.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
/*
*/
class TopComponent : public Component {
public:
	TopComponent(KeyRepeatAudioProcessor& p);
	~TopComponent();

	void paint(Graphics&) override;
	void resized() override;

private:

	KeyRepeatAudioProcessor& processor;

	Slider pitchKnob;
	std::unique_ptr<SliderAttachment> pitchAttachment;
	ParameterLabel pitchLabel;

	Slider panKnob;
	std::unique_ptr<SliderAttachment> panAttachment;
	ParameterLabel panLabel;

	Slider levelKnob;
	std::unique_ptr<SliderAttachment> levelAttachment;
	ParameterLabel levelLabel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopComponent)
};
