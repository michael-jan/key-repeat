#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "FileDropperComponent.h"

KeyRepeatAudioProcessorEditor::KeyRepeatAudioProcessorEditor(KeyRepeatAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p)
{
	processor.fileDropperComponent.addChangeListener(this);
	addAndMakeVisible(processor.fileDropperComponent);

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
	processor.fileDropperComponent.setCentrePosition(getWidth() / 2, 25);
}

void KeyRepeatAudioProcessorEditor::loadNewFile(const String& absoluteFilePath) {

	DBG("loadNewFile() called");

	File myFile(absoluteFilePath);
	AudioFormatReader *reader = formatManager.createReaderFor(myFile);

	BigInteger allNotes;
	allNotes.setRange(0, 128, true);

	// if file successfully read
	if (reader != nullptr) {
		std::unique_ptr<AudioFormatReaderSource> tempSource(new AudioFormatReaderSource(reader, true));
		processor.synth.addSound(new SamplerSound("sampleSound", *reader, allNotes, 60, 0, 10, 10.0));
	}
}

void KeyRepeatAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster *source) {
	if (source == &processor.fileDropperComponent) {
		loadNewFile(processor.fileDropperComponent.getAbsoluteFilePath());
	}
}