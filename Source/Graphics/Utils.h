/*
  ==============================================================================

    Utils.h
    Created: 19 Jun 2019 12:55:57pm
    Author:  Michael Jan

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"
#include "MyLookAndFeel.h"

class Utils {
	
public:
	static const int ORIGINAL_HEIGHT, ORIGINAL_WIDTH;
	static const double DEFAULT_SCALE_FACTOR;
	static double SCALE_FACTOR;

	template <typename T>
	static T winMac(T a, T b) {
		return (SystemStats::getOperatingSystemType() & SystemStats::Windows) != 0 ? a : b;
	}

	static int scale(float dimension);
	static int getTextWidth(Label& label);
	static void attachToComponent(Label& label, Component& component, int offset = 0);
	static void drawLineShadow(Graphics& g, Line<float> line, int spread, float alpha = 1.0f);
	static void drawPathShadow(Graphics& g, Path path, int spread, float alpha = 1.0f); 

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Utils)
};