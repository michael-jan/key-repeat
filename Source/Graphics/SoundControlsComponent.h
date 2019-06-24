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
#include "ParameterLabel.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
/*
*/
class SoundControlsComponent : public Component {
public:
	SoundControlsComponent(KeyRepeatAudioProcessor& p);
	~SoundControlsComponent();

	void paint(Graphics&) override;
	void resized() override;

private:

	KeyRepeatAudioProcessor& processor;

	Slider attackSlider;
	std::unique_ptr<SliderAttachment> attackAttachment;
	ParameterLabel attackLabel;

	Slider decaySlider;
	std::unique_ptr<SliderAttachment> decayAttachment;
	ParameterLabel decayLabel;

	Slider releaseSlider;
	std::unique_ptr<SliderAttachment> releaseAttachment;
	ParameterLabel releaseLabel;

	Slider sustainSlider;
	std::unique_ptr<SliderAttachment> sustainAttachment;
	ParameterLabel sustainLabel;

	Slider swingKnob;
	std::unique_ptr<SliderAttachment> swingAttachment;
	ParameterLabel swingLabel;

	Slider humanizeKnob;
	std::unique_ptr<SliderAttachment> humanizeAttachment;
	ParameterLabel humanizeLabel;


	void attachToSlider(ParameterLabel& label, Slider& slider);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SoundControlsComponent)
};
