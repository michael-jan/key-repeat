#include "PluginProcessor.h"
#include "PluginEditor.h"

KeyRepeatAudioProcessorEditor::KeyRepeatAudioProcessorEditor(KeyRepeatAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p),
	state(Stopped), openButton("Open"), playButton("Play"), stopButton("Stop")
{
	openButton.onClick = [this] { openButtonClicked(); };
	addAndMakeVisible(openButton);

	playButton.onClick = [this] { playButtonClicked(); };
	playButton.setColour(TextButton::buttonColourId, Colours::green);
	playButton.setEnabled(true);
	addAndMakeVisible(playButton);

	stopButton.onClick = [this] { stopButtonClicked(); };
	stopButton.setColour(TextButton::buttonColourId, Colours::red);
	stopButton.setEnabled(false);
	addAndMakeVisible(stopButton);

	formatManager.registerBasicFormats(); // enable .wav and .aiff files
	
	processor.addChangeListener(this);

	setSize(200, 150);
}

KeyRepeatAudioProcessorEditor::~KeyRepeatAudioProcessorEditor() {
}

void KeyRepeatAudioProcessorEditor::paint (Graphics& g) {
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void KeyRepeatAudioProcessorEditor::resized() {
	openButton.setBounds(10, 10, getWidth() - 20, 30);
	playButton.setBounds(10, 50, getWidth() - 20, 30);
	stopButton.setBounds(10, 90, getWidth() - 20, 30);
}

void KeyRepeatAudioProcessorEditor::openButtonClicked() {

	DBG("open button clicked");

	// open file chooser
	FileChooser chooser("Choose a WAV or AIFF file",
		File::getSpecialLocation(File::userDesktopDirectory), "*.wav; *.aiff; *.mp3");

	// if user chose a file
	if (chooser.browseForFileToOpen()) {

		File myFile = chooser.getResult();
		AudioFormatReader *reader = formatManager.createReaderFor(myFile);

		// if file successfully read
		if (reader != nullptr) {
			std::unique_ptr<AudioFormatReaderSource> tempSource(new AudioFormatReaderSource(reader, true));
			processor.transportSource.setSource(tempSource.get(), 0, nullptr, reader->sampleRate);
			changeState(Stopped);
			processor.playSource.reset(tempSource.release());
		}

	}

}

void KeyRepeatAudioProcessorEditor::playButtonClicked() {
	DBG("play button clicked");
	changeState(Starting);
}

void KeyRepeatAudioProcessorEditor::stopButtonClicked() {
	DBG("stop button clicked");
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
	DBG("RECEIVED 1");
	if (source == &processor) {
		DBG("RECEIVED 2");

		if (processor.transportSource.isPlaying()) {
			DBG("RECEIVED 3");

			changeState(Playing);
		} else {
			DBG("RECEIVED 4");

			changeState(Stopped);
		}
	}
}