/*
  ==============================================================================

    MyLookAndFeel.cpp
    Created: 16 Jun 2019 6:45:14pm
    Author:  Michael Jan

  ==============================================================================
*/

#include "MyLookAndFeel.h"

const Colour MyLookAndFeel::WHITE(243, 245, 246);
const Colour MyLookAndFeel::LIGHT_GREY(178, 182, 203);
const Colour MyLookAndFeel::GREY(145, 146, 163);
const Colour MyLookAndFeel::DARK_GREY(106, 106, 120);
const Colour MyLookAndFeel::VERY_DARK_GREY(69, 71, 82);

const Colour MyLookAndFeel::LIGHT_PINK(239, 138, 255);
const Colour MyLookAndFeel::PINK(235, 115, 255);
const Colour MyLookAndFeel::DARK_PINK(232, 91, 255);

MyLookAndFeel::MyLookAndFeel() {
	setDefaultSansSerifTypefaceName("Laconic");
}
