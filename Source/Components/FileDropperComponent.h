/*
  ==============================================================================

	FileDropperComponent.h
	Created: 26 May 2019 3:08:33am
	Author:  Michael Jan

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Processing/PluginProcessor.h"

//==============================================================================

class FileDropperComponent :
	public Component,
	public FileDragAndDropTarget,
	public ChangeListener,
	public ChangeBroadcaster
{
public:

	FileDropperComponent(KeyRepeatAudioProcessor& p);
	~FileDropperComponent();

	void paint(Graphics&) override;
	void resized() override;

	String getAbsoluteFilePath() const;

private:

	KeyRepeatAudioProcessor& processor;
	AudioFormatManager formatManager;
	AudioThumbnailCache thumbnailCache;
	AudioThumbnail thumbnail;

	Label label;

	enum FilledState {
		Filled,
		Unfilled
	};

	enum HoverState {
		ValidHover,
		InvalidHover,
		NoHover
	};

	FilledState filledState;
	HoverState hoverState;
	void changeState(FilledState filledState, HoverState hoverState);

	String absoluteFilePath;

	bool isInterestedInFileDrag(const StringArray& files) override;
	void fileDragEnter(const StringArray& files, int x, int y) override;
	void fileDragMove(const StringArray& files, int x, int y) override;
	void fileDragExit(const StringArray& files) override;
	void filesDropped(const StringArray& files, int x, int y) override;

	void changeListenerCallback(ChangeBroadcaster* source) override;

	void drawInnerShadow(Graphics& g, Path target);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileDropperComponent)
};
