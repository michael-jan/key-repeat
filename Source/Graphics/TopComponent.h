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
#include "Utils.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class TopLogoComponent : public Component {
public:
    TopLogoComponent();
    ~TopLogoComponent();
    void paint(Graphics&) override;
    void resized() override;
private:
    Label titleLabel;
    Label authorLabel;
};

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
	Label pitchLabel;

	Slider panKnob;
	std::unique_ptr<SliderAttachment> panAttachment;
	Label panLabel;

	Slider levelKnob;
	std::unique_ptr<SliderAttachment> levelAttachment;
	Label levelLabel;
    
    TopLogoComponent topLogoComponent;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopComponent)
};
