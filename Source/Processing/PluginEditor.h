#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "../Components/MainComponent.h"

class KeyRepeatAudioProcessorEditor :
	public AudioProcessorEditor
{
public:
    KeyRepeatAudioProcessorEditor(KeyRepeatAudioProcessor&, LookAndFeel *lookAndFeel);
    ~KeyRepeatAudioProcessorEditor();

    void paint(Graphics&) override;
    void resized() override;

private:
    KeyRepeatAudioProcessor& processor;
	MainComponent mainComponent;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyRepeatAudioProcessorEditor)
};
