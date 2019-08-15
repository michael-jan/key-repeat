#include "PluginEditor.h"

KeyRepeatAudioProcessorEditor::KeyRepeatAudioProcessorEditor(KeyRepeatAudioProcessor& p, LookAndFeel_V4 *lookAndFeel)
	: AudioProcessorEditor(&p), processor(p), mainComponent(p)
{
	LookAndFeel::setDefaultLookAndFeel(lookAndFeel);
	addAndMakeVisible(mainComponent);
	setSize(
		Utils::scale(Utils::ORIGINAL_WIDTH),
		Utils::scale(Utils::ORIGINAL_HEIGHT)
	);
}

KeyRepeatAudioProcessorEditor::~KeyRepeatAudioProcessorEditor() {
}

void KeyRepeatAudioProcessorEditor::paint (Graphics& g) {
	g.fillAll(Colours::black);
}

void KeyRepeatAudioProcessorEditor::resized() {
	mainComponent.setBounds(getLocalBounds());
}