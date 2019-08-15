/*
  ==============================================================================

	MiddleComponent.cpp
	Created: 1 Jun 2019 8:32:20pm
	Author:  Michael Jan

  ==============================================================================
*/

#include "../../JuceLibraryCode/JuceHeader.h"
#include "MiddleComponent.h"
#include "Utils.h"

//==============================================================================
MiddleComponent::MiddleComponent(KeyRepeatAudioProcessor& p) :
	processor(p),
	adsrComponent(p),
	fileDropperComponent(p),
	soundControlsComponent(p)
{
	shadowComponent.setBufferedToImage(true);
	addAndMakeVisible(shadowComponent);
	addAndMakeVisible(adsrComponent);
	addAndMakeVisible(fileDropperComponent);
	addAndMakeVisible(soundControlsComponent);
}

MiddleComponent::~MiddleComponent() {
}

void MiddleComponent::paint(Graphics& g) {
}

void MiddleComponent::resized() {
	Rectangle<int> rect = getLocalBounds();
	int originalRectWidth = rect.getWidth();

	shadowComponent.setBounds(rect);
	adsrComponent.setBounds(rect.removeFromLeft(originalRectWidth / 3));
	fileDropperComponent.setBounds(rect.removeFromLeft(originalRectWidth / 3));
	soundControlsComponent.setBounds(rect);
}

void MiddleShadowComponent::paint(Graphics& g) {
	Line<float> topLine(0, 0, getWidth(), 0);
	Line<float> bottomLine(0, getHeight(), getWidth(), getHeight());
	Utils::drawLineShadow(g, topLine, getHeight() / 2);
	Utils::drawLineShadow(g, bottomLine, getHeight() / 2);
}

void MiddleShadowComponent::resized() {
}