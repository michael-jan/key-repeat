/*
  ==============================================================================

    MiddleComponent.h
    Created: 1 Jun 2019 8:32:20pm
    Author:  Michael Jan

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Processing/PluginProcessor.h"
#include "ADSRComponent.h"
#include "FileDropperComponent.h"
#include "SoundControlsComponent.h"

class MiddleShadowComponent : public Component {
public:
	void paint(Graphics&) override;
	void resized() override;
};

class MiddleComponent : public Component {
public:
    MiddleComponent(KeyRepeatAudioProcessor& p);
    ~MiddleComponent();

    void paint (Graphics&) override;
    void resized() override;

private:

	KeyRepeatAudioProcessor& processor;
	MiddleShadowComponent shadowComponent;
	ADSRComponent adsrComponent;
	FileDropperComponent fileDropperComponent;
	SoundControlsComponent soundControlsComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiddleComponent)
};

