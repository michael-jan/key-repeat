#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../Components/FileDropperComponent.h"
#include "../Components/MyLookAndFeel.h"

KeyRepeatAudioProcessorEditor::KeyRepeatAudioProcessorEditor(KeyRepeatAudioProcessor& p, LookAndFeel *lookAndFeel)
	: AudioProcessorEditor(&p), processor(p), mainComponent(p)
{
	addAndMakeVisible(mainComponent);
	setLookAndFeel(lookAndFeel);
	setSize(700, 400);
}

KeyRepeatAudioProcessorEditor::~KeyRepeatAudioProcessorEditor() {
}

void KeyRepeatAudioProcessorEditor::paint (Graphics& g) {
	g.fillAll(Colours::black);
}

void KeyRepeatAudioProcessorEditor::resized() {
	mainComponent.setBounds(getLocalBounds());
}