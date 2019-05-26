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
FileDropperComponent::FileDropperComponent() :
	absoluteFilePath(""), filledState(Unfilled), hoverState(NoHover)
{
	// In your constructor, you should add any child components, and
	// initialise any special settings that your component needs.
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
	// This method is where you should set the bounds of any child
	// components that your component contains..

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
	absoluteFilePath = files[0];
	sendChangeMessage();
}
/* End FileDragAndDropTarget callbacks */

