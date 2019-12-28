/*
  ==============================================================================

	MainComponent.cpp
	Created: 31 May 2019 9:12:52am
	Author:  Michael Jan

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent(KeyRepeatAudioProcessor& p) :
	processor(p),
	topComponent(p),
	middleComponent(p),
	bottomComponent(p)
{
	addAndMakeVisible(topComponent);
	addAndMakeVisible(middleComponent);
	addAndMakeVisible(bottomComponent);
}

MainComponent::~MainComponent() {
}

void MainComponent::paint(Graphics& g) {
	g.setColour(MyLookAndFeel::GREY);
	g.fillAll();
}

void MainComponent::resized() {
	Rectangle<int> rect = getLocalBounds();
	int totalHeight = rect.getHeight();
	topComponent.setBounds(rect.removeFromTop(totalHeight * 2/9));
	middleComponent.setBounds(rect.removeFromTop(totalHeight * 5/9));
	bottomComponent.setBounds(rect);
}


