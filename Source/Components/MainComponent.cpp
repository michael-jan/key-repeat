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
	fileDropperComponent(p) 
{
	addAndMakeVisible(fileDropperComponent);
}

MainComponent::~MainComponent() {
}

void MainComponent::paint(Graphics& g) {

	Colour light(40, 40, 40);
	Colour dark(16, 16, 16);

	Path roundedRectangle;
	roundedRectangle.addRoundedRectangle(getLocalBounds().toFloat().reduced(5), 10.0f);

	Point<int> zerozero(0, 0);
	DropShadow ds(light, 8, zerozero);
	ds.drawForPath(g, roundedRectangle);

	ColourGradient gradient(light, getWidth() / 2, - getHeight() / 2, dark, getWidth() / 2, getHeight(), true);
	g.setGradientFill(gradient);
	g.fillPath(roundedRectangle);

}

void MainComponent::resized() {
	fileDropperComponent.setSize(getWidth() / 3, 100);
	fileDropperComponent.setCentrePosition(getWidth() / 2, 70);
}


