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
class SoundControlsComponent : public Component {
public:
	SoundControlsComponent(KeyRepeatAudioProcessor& p);
	~SoundControlsComponent();

	void paint(Graphics&) override;
	void resized() override;

private:

	KeyRepeatAudioProcessor& processor;

	Slider swingKnob;
	std::unique_ptr<SliderAttachment> swingAttachment;
	Label swingLabel;

	Slider humanizeKnob;
	std::unique_ptr<SliderAttachment> humanizeAttachment;
	Label humanizeLabel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SoundControlsComponent)
};
