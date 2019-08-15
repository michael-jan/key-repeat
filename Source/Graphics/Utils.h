/*
  ==============================================================================

    Utils.h
    Created: 19 Jun 2019 12:55:57pm
    Author:  Michael Jan

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

class Utils {
	
public:
	static const int ORIGINAL_HEIGHT, ORIGINAL_WIDTH;
	static const double SCALE_FACTOR;

	static int scale(int dimension);
	static int getTextWidth(Label& label);
	static void attachToSlider(Label& label, Slider& slider, int offset = 0);
	static void drawLineShadow(Graphics& g, Line<float> line, int spread);

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Utils)
};