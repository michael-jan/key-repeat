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

class FileDropperShadowComponent : public Component {
public:
    void paint(Graphics&) override;
    void resized() override;
    void drawInnerShadow(Graphics& g, Path target);
};

class FileDropperComponent :
    public Component,
    public FileDragAndDropTarget
{
public:

    FileDropperComponent(KeyRepeatAudioProcessor& p);
    ~FileDropperComponent();

    void paint(Graphics&) override;
    void resized() override;

private:

    KeyRepeatAudioProcessor& processor;
    Rectangle<int> displayBounds;

    FileDropperShadowComponent fileDropperShadowComponent;
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

    bool isInterestedInFileDrag(const StringArray& files) override;
    void fileDragEnter(const StringArray& files, int x, int y) override;
    void fileDragMove(const StringArray& files, int x, int y) override;
    void fileDragExit(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileDropperComponent)
};
