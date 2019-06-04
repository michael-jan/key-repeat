/*
  ==============================================================================

    MiddleComponent.h
    Created: 1 Jun 2019 8:32:20pm
    Author:  Michael Jan

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "../Processing/PluginProcessor.h"
#include "SoundKnobsComponent.h"
#include "FileDropperComponent.h"

//==============================================================================
/*
*/
class MiddleComponent : public Component
{
public:
    MiddleComponent(KeyRepeatAudioProcessor& p);
    ~MiddleComponent();

    void paint (Graphics&) override;
    void resized() override;

private:

	KeyRepeatAudioProcessor& processor;
	SoundKnobsComponent soundKnobsComponent;
	FileDropperComponent fileDropperComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiddleComponent)
};
