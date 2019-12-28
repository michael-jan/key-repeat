/*
  ==============================================================================

	ADSRComponent.h
	Created: 19 Jun 2019 12:23:07pm
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
class ADSRComponent : public Component {
public:
	ADSRComponent(KeyRepeatAudioProcessor& p);
	~ADSRComponent();

	void paint(Graphics&) override;
	void resized() override;

private:
	
	KeyRepeatAudioProcessor& processor;

	Slider attackSlider;
	std::unique_ptr<SliderAttachment> attackAttachment;
	Label attackLabel;

	Slider decaySlider;
	std::unique_ptr<SliderAttachment> decayAttachment;
	Label decayLabel;

    Slider sustainSlider;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    Label sustainLabel;

	Slider releaseSlider;
	std::unique_ptr<SliderAttachment> releaseAttachment;
	Label releaseLabel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRComponent)
};
