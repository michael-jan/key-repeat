/*
  ==============================================================================

    MyLookAndFeel.cpp
    Created: 16 Jun 2019 6:45:14pm
    Author:  Michael Jan

  ==============================================================================
*/

#include "MyLookAndFeel.h"

MyLookAndFeel::MyLookAndFeel() {
	setColour(Slider::trackColourId, Colours::lightblue);
	setColour(Label::textColourId, Colours::lightgrey);
	setDefaultSansSerifTypefaceName("Laconic");
}
