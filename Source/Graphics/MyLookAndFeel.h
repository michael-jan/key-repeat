/*
  ==============================================================================

    MyLookAndFeel.h
    Created: 16 Jun 2019 6:45:14pm
    Author:  Michael Jan

  ==============================================================================
*/

#pragma once
#include "../../JuceLibraryCode/JuceHeader.h"
#include "Utils.h"

class MyLookAndFeel : public LookAndFeel_V4 {

public:
	static const Colour WHITE, LIGHT_GREY, GREY, DARK_GREY, VERY_DARK_GREY, BLACK, LIGHT_PINK, PINK, DARK_PINK;
	static const float LABEL_FONT_SIZE, SMALL_FONT_SIZE;

	MyLookAndFeel();

	static const Font& getFontLight();
	static const Font& getFontRegular();
	static const Font& getFontBold();

	void drawRotarySlider(Graphics&, int x, int y, int width, int height,
		float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
		Slider&) override;

	virtual void drawLinearSlider(Graphics&, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const Slider::SliderStyle, Slider&) override;

	void drawLinearSliderBackground(Graphics&, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const Slider::SliderStyle, Slider&) override;

	void drawToggleButton(Graphics&, ToggleButton&,
		bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;
};

class NumboxLAF : public MyLookAndFeel {
public:

	NumboxLAF();

	void drawLinearSlider(Graphics&, int x, int y, int width, int height,
		float sliderPos, float minSliderPos, float maxSliderPos,
		const Slider::SliderStyle, Slider&) override;
    
    Font getLabelFont(Label &label) override;
    
    Slider::SliderLayout getSliderLayout (Slider&) override;
};
