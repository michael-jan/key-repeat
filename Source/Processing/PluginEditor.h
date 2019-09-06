#pragma once

#include <chrono>
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "../Graphics/MainComponent.h"
#include "../Graphics/Utils.h"

class KeyRepeatAudioProcessorEditor :
	public AudioProcessorEditor
{
public:
    KeyRepeatAudioProcessorEditor(KeyRepeatAudioProcessor&, LookAndFeel_V4 *lookAndFeel);
    ~KeyRepeatAudioProcessorEditor();

    void paint(Graphics&) override;
    void resized() override;

private:
    KeyRepeatAudioProcessor& processor;
	MainComponent mainComponent;
	ComponentBoundsConstrainer constrainer;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyRepeatAudioProcessorEditor)
};
