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

private:
    KeyRepeatAudioProcessor& processor;

	enum TransportState {
		Stopped,
		Starting,
		Playing,
		Stopping,
	};

	TransportState state;

	void openButtonClicked();
	void playButtonClicked();
	void stopButtonClicked();
	void changeState(TransportState newState);

	AudioFormatManager formatManager;

	TextButton openButton;
	TextButton playButton;
	TextButton stopButton;

	void changeListenerCallback(ChangeBroadcaster *source) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyRepeatAudioProcessorEditor)
};
