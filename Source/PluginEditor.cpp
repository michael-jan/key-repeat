#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "FileDropperComponent.h"

KeyRepeatAudioProcessorEditor::KeyRepeatAudioProcessorEditor(KeyRepeatAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p),
	state(Stopped), openButton("Open"), playButton("Play"), stopButton("Stop")
{

	playButton.onClick = [this] { playButtonClicked(); };
	playButton.setColour(TextButton::buttonColourId, Colours::green);
	playButton.setEnabled(true);
	addAndMakeVisible(playButton);

	stopButton.onClick = [this] { stopButtonClicked(); };
	stopButton.setColour(TextButton::buttonColourId, Colours::red);
	stopButton.setEnabled(false);
	addAndMakeVisible(stopButton);

	fileDropperComponent.addChangeListener(this);
	addAndMakeVisible(fileDropperComponent);

	formatManager.registerBasicFormats(); // enable .wav and .aiff files
	processor.addChangeListener(this);

	setSize(500, 300);
}

KeyRepeatAudioProcessorEditor::~KeyRepeatAudioProcessorEditor() {
}

void KeyRepeatAudioProcessorEditor::paint (Graphics& g) {
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void KeyRepeatAudioProcessorEditor::resized() {
	fileDropperComponent.setCentrePosition(getWidth() / 2, 25);
	playButton.setBounds(10, 50, getWidth() - 20, 30);
	stopButton.setBounds(10, 90, getWidth() - 20, 30);
}

void KeyRepeatAudioProcessorEditor::loadNewFile(const String& absoluteFilePath) {

	DBG("loadNewFile() called");

	File myFile(absoluteFilePath);
	AudioFormatReader *reader = formatManager.createReaderFor(myFile);

	// if file successfully read
	if (reader != nullptr) {
		std::unique_ptr<AudioFormatReaderSource> tempSource(new AudioFormatReaderSource(reader, true));
		processor.transportSource.setSource(tempSource.get(), 0, nullptr, reader->sampleRate);
		changeState(Stopped);
		processor.playSource.reset(tempSource.release());
	}

}

void KeyRepeatAudioProcessorEditor::playButtonClicked() {
	DBG("playButtonClicked()");
	changeState(Starting);
}

void KeyRepeatAudioProcessorEditor::stopButtonClicked() {
	DBG("stopButtonClicked()");
	changeState(Stopping);
}

void KeyRepeatAudioProcessorEditor::changeState(TransportState newState) {

	if (state != newState) {
		state = newState;
		switch (state) {

			case Stopped:
				playButton.setEnabled(true);
				stopButton.setEnabled(false);
				processor.transportSource.setPosition(0.0);
				break;

			case Starting:
				playButton.setEnabled(false);
				processor.transportSource.start();
				break;

			case Playing:
				stopButton.setEnabled(true);
				break;

			case Stopping:
				processor.transportSource.stop();
				break;

		}
	}

}

void KeyRepeatAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster *source) {
	if (source == &processor) {
		if (processor.transportSource.isPlaying()) {
			changeState(Playing);
		} else {
			changeState(Stopped);
		}
	} else if (source == &fileDropperComponent) {
		loadNewFile(fileDropperComponent.getAbsoluteFilePath());
	}
}