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
	processor(p), absoluteFilePath(""), filledState(Unfilled), hoverState(NoHover),
	thumbnailCache(2), thumbnail(1<<16, formatManager, thumbnailCache), label("fileDropperLabel", "< Drop In Sample >")
{
	formatManager.registerBasicFormats();

	addAndMakeVisible(label);
	label.setJustificationType(Justification::centred);
	label.setColour(Label::textColourId, Colours::grey.withAlpha(0.25f));
}

FileDropperComponent::~FileDropperComponent() {
}

void FileDropperComponent::paint(Graphics& g) {

	Colour lightGrey(43, 45, 49);
	Colour darkGrey(35, 36, 41);
	Colour orange(255, 150, 0);

	Path roundedBoundsPath;
	roundedBoundsPath.addRoundedRectangle(getLocalBounds().toFloat(), 10.0f);

	g.setColour(darkGrey);
	g.fillPath(roundedBoundsPath);

	if (thumbnail.getNumChannels() > 0) {
		g.setColour(orange);
		thumbnail.drawChannel(g,
			getLocalBounds().reduced(10),
			0.0,                                    // start time
			thumbnail.getTotalLength(),				// end time
			0,										// channel num
			1.0f);                                  // vertical zoom
	}

	if (hoverState == FileDropperComponent::ValidHover) {
		g.setColour(Colours::black.withAlpha(0.12f));
		g.fillPath(roundedBoundsPath);
	}

	drawInnerShadow(g, roundedBoundsPath);

	g.setColour(Colours::whitesmoke.withAlpha(0.14f));
	PathStrokeType pathStrokeType(1, PathStrokeType::JointStyle::curved, PathStrokeType::EndCapStyle::rounded);
	g.strokePath(roundedBoundsPath, pathStrokeType);

	if (!label.isEnabled()) {
		label.setAlpha(0.0f);
	}
}

void FileDropperComponent::resized() {
	label.setFont(Font(getHeight() / 5));
	label.setBounds(getLocalBounds());
}

String FileDropperComponent::getAbsoluteFilePath() const {
	return absoluteFilePath;
}


void FileDropperComponent::changeState(FilledState newFilledState, HoverState newHoverState) {
	if (filledState != newFilledState || hoverState != newHoverState) {
		filledState = newFilledState;
		hoverState = newHoverState;
	}
	repaint();
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
		label.setAlpha(0.4f);
		changeState(filledState, ValidHover);
	} else {
		changeState(filledState, InvalidHover);
	}
}

void FileDropperComponent::fileDragMove(const StringArray& files, int x, int y) {
	// do nothing
}

void FileDropperComponent::fileDragExit(const StringArray& files) {
	label.setAlpha(1.0f);
	changeState(filledState, NoHover);
}

void FileDropperComponent::filesDropped(const StringArray& files, int x, int y) {
	label.setEnabled(false);
	changeState(Filled, NoHover);
	File file(files[0]);
	AudioFormatReader *reader = formatManager.createReaderFor(file);
	if (reader != nullptr) {
		thumbnail.setSource(new FileInputSource(file));
		processor.loadNewFile(reader);
		delete reader;
	}
}
/* End FileDragAndDropTarget callbacks */

void FileDropperComponent::changeListenerCallback(ChangeBroadcaster* source) {
	if (source == &thumbnail) {
		repaint();
	}
}

// Credit goes to CrushedPixel for coming up with this hack.
// Code originally found here: https://forum.juce.com/t/inner-shadow-or-workarounds/19704/3
void FileDropperComponent::drawInnerShadow(Graphics &g, Path target) {
	// resets the Clip Region when the function returns
	Graphics::ScopedSaveState saveState(g);

	// invert the path's fill shape and enlarge it,
	// so it casts a shadow
	Path shadowPath(target);
	shadowPath.addRectangle(target.getBounds().expanded(10));
	shadowPath.setUsingNonZeroWinding(false);

	// reduce clip region to avoid the shadow
	// being drawn outside of the shape to cast the shadow on
	g.reduceClipRegion(target);

	DropShadow ds(Colour::fromRGB(27,28,31), 5, { 0, 0 });
	ds.drawForPath(g, shadowPath);
}
