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
    processor(p),
    label("fileDropperLabel", "Drag In Sample"),
    filledState(Unfilled),
    hoverState(NoHover)
{
	addAndMakeVisible(label);
	label.setJustificationType(Justification::centred);
	label.setColour(Label::textColourId, MyLookAndFeel::WHITE);

	fileDropperShadowComponent.setBufferedToImage(true);
	addAndMakeVisible(fileDropperShadowComponent);
}

FileDropperComponent::~FileDropperComponent() {
}

void FileDropperComponent::paint(Graphics& g) {
	Path roundedBoundsPath;
	roundedBoundsPath.addRectangle(displayBounds.toFloat());

	g.setColour(MyLookAndFeel::VERY_DARK_GREY);
	g.fillPath(roundedBoundsPath);
    
    g.setColour(Colours::whitesmoke.withAlpha(0.14f));
    PathStrokeType pathStrokeType(Utils::scale(1), PathStrokeType::JointStyle::curved, PathStrokeType::EndCapStyle::rounded);
    g.strokePath(roundedBoundsPath, pathStrokeType);
    
    AudioThumbnail& thumbnail = processor.getAudioThumbnail();
	if (thumbnail.getNumChannels() > 0) {
        // Draw audio thumbnail (waveform)
        label.setAlpha(0.0f);
		g.setColour(MyLookAndFeel::LIGHT_PINK);
		thumbnail.drawChannel(
			g,
			displayBounds.reduced(0),
			0.0,                                    // start time
			thumbnail.getTotalLength(),				// end time
			0,										// channel num
			1.0f                                    // vertical zoom
		);
    } else {
        // Draw placeholder for waveform
        label.setAlpha(0.15f);
        Path roundedRectanglePath;
        roundedRectanglePath.addRoundedRectangle(getLocalBounds().reduced(Utils::scale(15)), Utils::scale(14.5));
        float dashedLength[] = { (float) Utils::scale(12), (float) Utils::scale(14) };
        PathStrokeType dottedPathStrokeType(Utils::scale(1.5f));
        dottedPathStrokeType.createDashedStroke(roundedRectanglePath, roundedRectanglePath, dashedLength, 2, AffineTransform::translation(0, 0), 4.0);
        g.strokePath(roundedRectanglePath, dottedPathStrokeType);
    }
    
    if (hoverState == FileDropperComponent::ValidHover) {
        g.setColour(Colours::black.withAlpha(0.12f));
        g.fillPath(roundedBoundsPath);
    }
}

void FileDropperComponent::resized() {
	displayBounds = getLocalBounds();
	label.setFont(MyLookAndFeel::getFontLight().withHeight(Utils::scale(25)));
	label.setBounds(displayBounds);
	fileDropperShadowComponent.setBounds(displayBounds);
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
    processor.loadNewFile(files[0]);
    changeState(Filled, NoHover);
}

/* End FileDragAndDropTarget callbacks */

void FileDropperShadowComponent::paint(Graphics& g) {
	Path boundsPath;
	boundsPath.addRectangle(getLocalBounds());
	drawInnerShadow(g, boundsPath);
}

void FileDropperShadowComponent::resized() {
}

// Credit goes to CrushedPixel for coming up with this hack.
// Code originally found here: https://forum.juce.com/t/inner-shadow-or-workarounds/19704/3
void FileDropperShadowComponent::drawInnerShadow(Graphics &g, Path target) {
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

	DropShadow ds(MyLookAndFeel::BLACK.withAlpha(0.4f), Utils::scale(55), { 0, 0 });
	ds.drawForPath(g, shadowPath);
}
