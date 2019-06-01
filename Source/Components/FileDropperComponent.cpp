/*
  ==============================================================================

	FileDropperComponent.cpp
	Created: 26 May 2019 3:08:33am
	Author:  Michael Jan

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "FileDropperComponent.h"

//==============================================================================
FileDropperComponent::FileDropperComponent(KeyRepeatAudioProcessor& p) :
	processor(p), absoluteFilePath(""), filledState(Unfilled), hoverState(NoHover)
{
	setSize(300, 30);
}

FileDropperComponent::~FileDropperComponent() {
}

void FileDropperComponent::paint(Graphics& g) {

	if (filledState == Filled) {
		switch (hoverState) {
			case FileDropperComponent::ValidHover:
				g.setColour(Colours::green);
				break;
			case FileDropperComponent::InvalidHover:
				g.setColour(Colours::lightgreen);
				break;
			case FileDropperComponent::NoHover:
				g.setColour(Colours::lightgreen);
				break;
		}
	} else {
		switch (hoverState) {
			case FileDropperComponent::ValidHover:
				g.setColour(Colours::darkgrey);
				break;
			case FileDropperComponent::InvalidHover:
				g.setColour(Colours::grey);
				break;
			case FileDropperComponent::NoHover:
				g.setColour(Colours::grey);
				break;
		}

	}
	g.fillAll();

	g.setColour(Colours::black);
	g.drawRoundedRectangle(getLocalBounds().toFloat(), 5, 3.0f);
}

void FileDropperComponent::resized() {

}

String FileDropperComponent::getAbsoluteFilePath() const {
	return absoluteFilePath;
}


void FileDropperComponent::changeState(FilledState newFilledState, HoverState newHoverState) {
	if (filledState != newFilledState || hoverState != newHoverState) {
		filledState = newFilledState;
		hoverState = newHoverState;
		repaint();
	}
}

/* Begin FileDragAndDropTarget callbacks */
bool FileDropperComponent::isInterestedInFileDrag(const StringArray& files) {
	return files.size() == 1 && (
			files[0].endsWithIgnoreCase(".wav") ||
			files[0].endsWithIgnoreCase(".mp3") ||
			files[0].endsWithIgnoreCase(".aiff")
		);
}

void FileDropperComponent::fileDragEnter(const StringArray& files, int x, int y) {
	if (isInterestedInFileDrag(files)) {
		changeState(filledState, ValidHover);
	} else {
		changeState(filledState, InvalidHover);
	}
}

void FileDropperComponent::fileDragMove(const StringArray& files, int x, int y) {
	// do nothing
}

void FileDropperComponent::fileDragExit(const StringArray& files) {
	changeState(filledState, NoHover);
}

void FileDropperComponent::filesDropped(const StringArray& files, int x, int y) {
	changeState(Filled, NoHover);
	processor.loadNewFile(files[0]);
}
/* End FileDragAndDropTarget callbacks */

