/*
  ==============================================================================

    KeyswitchComponent.h
    Created: 17 Jun 2019 12:03:07am
    Author:  Michael Jan

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Processing/PluginProcessor.h"
#include "../Processing/KeyswitchManager.h"

//==============================================================================
/*
*/

class SingleKeyswitchComponent : public Component {
public:
    SingleKeyswitchComponent(KeyRepeatAudioProcessor& p);
    ~SingleKeyswitchComponent();

    void paint(Graphics&) override;
    void resized() override;

    void setDisplayInfoElement(KeyswitchDisplayInfoElement *displayInfoElement);

private:
    KeyRepeatAudioProcessor& processor;
    KeyswitchDisplayInfoElement *whatToDisplay;
    Label noteLabel;
    Label typeLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SingleKeyswitchComponent)
};

class KeyswitchComponent : public Component, Timer {
public:
    KeyswitchComponent(KeyRepeatAudioProcessor& p);
    ~KeyswitchComponent();

    void paint(Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:

    KeyRepeatAudioProcessor& processor;
    KeyswitchManager& keyswitchManager;
    OwnedArray<SingleKeyswitchComponent> keyswitches;

    void updateDisplayElements();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyswitchComponent)
};
