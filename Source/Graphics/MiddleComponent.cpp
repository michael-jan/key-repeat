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
    keyswitchControlsComponent(p),
    adsrComponent(p),
    soundControlsComponent(p),
    fileDropperComponent(p)
{
    shadowComponent.setBufferedToImage(true);
    addAndMakeVisible(shadowComponent);
    addAndMakeVisible(keyswitchControlsComponent);
    addAndMakeVisible(adsrComponent);
    addAndMakeVisible(soundControlsComponent);
    addAndMakeVisible(fileDropperComponent);
}
MiddleComponent::~MiddleComponent() {
}

void MiddleComponent::paint(Graphics& g) {
}

void MiddleComponent::resized() {
    Rectangle<int> rect = getLocalBounds();
    shadowComponent.setBounds(rect);
    fileDropperComponent.setBounds(rect.removeFromRight(rect.getWidth() / 2));
    keyswitchControlsComponent.setBounds(rect.removeFromLeft(Utils::scale(96)));
    adsrComponent.setBounds(rect.removeFromLeft(Utils::scale(133)));
    soundControlsComponent.setBounds(rect.removeFromLeft(Utils::scale(95)));
}

void MiddleShadowComponent::paint(Graphics& g) {
    Line<float> topLine(0, 0, getWidth(), 0);
    Line<float> topLineSmallerForArrow1(0, 0, Utils::scale(218), 0);
    Line<float> topLineSmallerForArrow2(0, 0, Utils::scale(218 + 18), 0);
    Line<float> topLineSmallerForArrow3(0, 0, Utils::scale(218 + 36), 0);
    Line<float> bottomLine(0, getHeight(), getWidth(), getHeight());
    Line<float> rightLine(getWidth() / 2, 0, getWidth() / 2, getHeight());
    Utils::drawLineShadow(g, topLine, Utils::scale(12), 0.7f);
    Utils::drawLineShadow(g, topLineSmallerForArrow1, Utils::scale(18), 0.1f);
    Utils::drawLineShadow(g, topLineSmallerForArrow2, Utils::scale(16), 0.1f);
    Utils::drawLineShadow(g, topLineSmallerForArrow3, Utils::scale(14), 0.3f);
    Utils::drawLineShadow(g, bottomLine, Utils::scale(12), 0.7f);
    Utils::drawLineShadow(g, rightLine, Utils::scale(12), 0.7f);
}

void MiddleShadowComponent::resized() {
}
