/*
  ==============================================================================

	MiddleKnobsComponent.h
	Created: 4 Jun 2019 3:14:12am
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
class SoundKnobsComponent : public Component {
public:
	SoundKnobsComponent(KeyRepeatAudioProcessor& p);
	~SoundKnobsComponent();

	void paint(Graphics&) override;
	void resized() override;

private:

	KeyRepeatAudioProcessor& processor;

	Slider attackSlider;
	std::unique_ptr<SliderAttachment> attackAttachment;

	Slider decaySlider;
	std::unique_ptr<SliderAttachment> decayAttachment;

	Slider releaseSlider;
	std::unique_ptr<SliderAttachment> releaseAttachment;

	Slider sustainSlider;
	std::unique_ptr<SliderAttachment> sustainAttachment;

	Slider swingKnob;
	std::unique_ptr<SliderAttachment> swingAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SoundKnobsComponent)
};
