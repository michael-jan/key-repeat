#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

class KeyRepeatAudioProcessorEditor :
	public AudioProcessorEditor,
	public ChangeListener
{
public:
    KeyRepeatAudioProcessorEditor (KeyRepeatAudioProcessor&);
    ~KeyRepeatAudioProcessorEditor();

    void paint(Graphics&) override;
    void resized() override;

	void loadNewFile(const String& absoluteFilePath);

private:
    KeyRepeatAudioProcessor& processor;
	AudioFormatManager formatManager;

	void changeListenerCallback(ChangeBroadcaster *source) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyRepeatAudioProcessorEditor)
};
