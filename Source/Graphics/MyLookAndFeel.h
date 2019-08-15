/*
  ==============================================================================

    MyLookAndFeel.h
    Created: 16 Jun 2019 6:45:14pm
    Author:  Michael Jan

  ==============================================================================
*/

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"

class MyLookAndFeel : public LookAndFeel_V4 {

public:
	static const Colour WHITE, LIGHT_GREY, GREY, DARK_GREY, VERY_DARK_GREY, LIGHT_PINK, PINK, DARK_PINK;
	MyLookAndFeel();
};