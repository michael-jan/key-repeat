#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../Components/FileDropperComponent.h"

KeyRepeatAudioProcessorEditor::KeyRepeatAudioProcessorEditor(KeyRepeatAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p), mainComponent(p)
{
	addAndMakeVisible(mainComponent);
	setSize(950, 450);
}

KeyRepeatAudioProcessorEditor::~KeyRepeatAudioProcessorEditor() {
}

void KeyRepeatAudioProcessorEditor::paint (Graphics& g) {
	g.fillAll(Colours::black);
}

void KeyRepeatAudioProcessorEditor::resized() {
	mainComponent.setBounds(getLocalBounds());
}