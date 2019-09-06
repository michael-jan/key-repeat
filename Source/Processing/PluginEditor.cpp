#include "PluginEditor.h"

KeyRepeatAudioProcessorEditor::KeyRepeatAudioProcessorEditor(KeyRepeatAudioProcessor& p, LookAndFeel_V4 *lookAndFeel)
	: AudioProcessorEditor(&p), processor(p), mainComponent(p)
{
	LookAndFeel::setDefaultLookAndFeel(lookAndFeel);
	addAndMakeVisible(mainComponent);

	constrainer.setFixedAspectRatio((double) Utils::ORIGINAL_WIDTH / Utils::ORIGINAL_HEIGHT);
	constrainer.setMinimumWidth(Utils::ORIGINAL_WIDTH);
	constrainer.setMaximumWidth(Utils::ORIGINAL_WIDTH * 4);
	setConstrainer(&constrainer);
	setResizable(false, false);
	setSize(
		Utils::scale(Utils::ORIGINAL_WIDTH),
		Utils::scale(Utils::ORIGINAL_HEIGHT)
	);
}

KeyRepeatAudioProcessorEditor::~KeyRepeatAudioProcessorEditor() {
}

void KeyRepeatAudioProcessorEditor::paint(Graphics& g) {
	g.fillAll(Colours::black);
}

void KeyRepeatAudioProcessorEditor::resized() {
	Utils::SCALE_FACTOR = (float)getHeight() / Utils::ORIGINAL_HEIGHT;
	mainComponent.setBounds(getLocalBounds());
}