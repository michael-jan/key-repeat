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

	Slider panKnob;
	std::unique_ptr<SliderAttachment> panAttachment;

	Slider levelKnob;
	std::unique_ptr<SliderAttachment> levelAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopComponent)
};
