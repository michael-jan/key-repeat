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
#include "KeyswitchControlsComponent.h"
#include "ADSRComponent.h"
#include "SoundControlsComponent.h"
#include "FileDropperComponent.h"

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
    KeyswitchControlsComponent keyswitchControlsComponent;
    ADSRComponent adsrComponent;
    SoundControlsComponent soundControlsComponent;
    FileDropperComponent fileDropperComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiddleComponent)
};

