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
	bottomComponent(p),
	middleComponent(p),
	topComponent(p)
{
	addAndMakeVisible(bottomComponent);
	addAndMakeVisible(middleComponent);
	addAndMakeVisible(topComponent);
}

MainComponent::~MainComponent() {
}

void MainComponent::paint(Graphics& g) {

	Colour lightWhite(255, 255, 255);
	Colour darkWhite(215, 215, 215);
	Colour grey(60, 62, 68);

	//ColourGradient gradient(grey, getWidth() / 2, 0, grey.darker(0.2f), getWidth() / 2, getHeight(), false);
	//g.setGradientFill(gradient);
	g.setColour(grey);
	g.fillAll();

	/*
	Path roundedRectangle;
	roundedRectangle.addRoundedRectangle(getLocalBounds().toFloat().reduced(5), 12.0f);

	Point<int> zerozero(0, 0);
	DropShadow ds(darkWhite, 12, zerozero);
	ds.drawForPath(g, roundedRectangle);

	ColourGradient gradient(lightWhite, getWidth() / 2, 0, darkWhite, getWidth() / 2, getHeight(), true);
	g.setGradientFill(gradient);
	g.fillPath(roundedRectangle);
	*/

}

void MainComponent::resized() {
	Rectangle<int> rect = getLocalBounds();
	topComponent.setBounds(rect.removeFromTop(rect.getHeight() * 2/11));
	middleComponent.setBounds(rect.removeFromTop(rect.getHeight() / 2));
	bottomComponent.setBounds(rect);
}


