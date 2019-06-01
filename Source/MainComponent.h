/*
  ==============================================================================

	MainComponent.h
	Created: 31 May 2019 9:12:52am
	Author:  Michael Jan

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FileDropperComponent.h"
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class MainComponent :
	public Component
{
public:
	MainComponent(KeyRepeatAudioProcessor& p);
	~MainComponent();

	void paint(Graphics&) override;
	void resized() override;

private:
	KeyRepeatAudioProcessor& processor;
	FileDropperComponent fileDropperComponent;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
