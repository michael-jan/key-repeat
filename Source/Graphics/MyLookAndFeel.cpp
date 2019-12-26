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
const Colour MyLookAndFeel::VERY_DARK_GREY(83, 85, 97);
const Colour MyLookAndFeel::BLACK(0, 0, 0);

const Colour MyLookAndFeel::LIGHT_PINK(239, 138, 255);
const Colour MyLookAndFeel::PINK(235, 115, 255);
const Colour MyLookAndFeel::DARK_PINK(232, 91, 255);

const float MyLookAndFeel::LABEL_FONT_SIZE = 16.5;

MyLookAndFeel::MyLookAndFeel() {
	setDefaultSansSerifTypeface(getFontRegular().getTypeface());
	setColour(Label::textColourId, WHITE);
	setColour(Slider::textBoxOutlineColourId, WHITE.withAlpha(0.0f));
}

const Font& MyLookAndFeel::getFontLight() {
	static Font fontLight(Font(Typeface::createSystemTypefaceFor(BinaryData::Laconic_Light_ttf, BinaryData::Laconic_Light_ttfSize)));
	return fontLight;
}

const Font& MyLookAndFeel::getFontRegular() {
	static Font fontLight(Font(Typeface::createSystemTypefaceFor(BinaryData::Laconic_Regular_ttf, BinaryData::Laconic_Regular_ttfSize)));
	return fontLight;
}

const Font& MyLookAndFeel::getFontBold() {
	static Font fontLight(Font(Typeface::createSystemTypefaceFor(BinaryData::Laconic_Bold_ttf, BinaryData::Laconic_Bold_ttfSize)));
	return fontLight;
}

void MyLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
									 const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
	auto backgroundColor = LIGHT_GREY;
	auto sauceColor = PINK;
	auto thumbColor = PINK;

	bool startFromMiddle = slider.getProperties().getWithDefault("startFromMiddle", false);

	Rectangle<float> bounds(x, y, width, height);
	auto sideLength = Utils::scale(40.0f);
	auto radius = sideLength * 0.5f;
	auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
	auto angleDelta = (startFromMiddle ? sliderPos - 0.5f : sliderPos) * (rotaryEndAngle - rotaryStartAngle);

	Path background;
	background.startNewSubPath(bounds.getCentre());
	background.addCentredArc(
		bounds.getCentreX(),
		bounds.getCentreY(),
		radius,
		radius,
		startFromMiddle ? jmax(0.0f, angleDelta) : rotaryStartAngle + angleDelta,
		0.0f,
		MathConstants<float>::twoPi - std::abs(angleDelta)
	);
	background.closeSubPath();
	g.setColour(backgroundColor);
	g.fillPath(background);

	Path sauce;
	sauce.startNewSubPath(bounds.getCentre());
	sauce.addCentredArc(
		bounds.getCentreX(),
		bounds.getCentreY(),
		radius,
		radius,
		startFromMiddle ? 0.0f : rotaryStartAngle,
		0.0f,
		angleDelta
	);
	sauce.closeSubPath();
	g.setColour(sauceColor);
	g.fillPath(sauce);

	g.setColour(WHITE);
	g.fillEllipse(bounds.withSizeKeepingCentre(sideLength * 0.65f, sideLength * 0.65f));

	auto thumbWidth = sideLength * 0.1f;
	auto thumbRadius = sideLength * 0.175f;
	Point<float> thumbPoint(
		bounds.getCentreX() + thumbRadius * std::cos(toAngle - MathConstants<float>::halfPi),
		bounds.getCentreY() + thumbRadius * std::sin(toAngle - MathConstants<float>::halfPi)
	);
	g.setColour(thumbColor);
	g.fillEllipse(Rectangle<float>(thumbWidth, thumbWidth).withCentre(thumbPoint));
}

void MyLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos,
									 float maxSliderPos, const Slider::SliderStyle style, Slider& slider)
{
	auto borderColor = WHITE;
	auto backgroundColor = LIGHT_GREY;
	auto sauceColor = PINK;

	Rectangle<float> bounds(x, y, width, height);
	Rectangle<float> border = bounds.withSizeKeepingCentre(Utils::scale(17), height);
	g.setColour(borderColor);
	g.fillRect(border);

	auto sauceFraction = 1 - (sliderPos - 1) / height;
	Rectangle<float> inside = border.reduced(Utils::scale(4));
	Rectangle<float> sauce = inside.removeFromBottom(sauceFraction * inside.getHeight());
	g.setColour(sauceColor);
	g.fillRect(sauce);

	g.setColour(backgroundColor);
	g.fillRect(inside);
}

void MyLookAndFeel::drawLinearSliderBackground(Graphics& g, int x, int y, int width, int height,
	float /*sliderPos*/,
	float /*minSliderPos*/,
	float /*maxSliderPos*/,
	const Slider::SliderStyle /*style*/, Slider& slider) 
{
	// no-op
}

void MyLookAndFeel::drawToggleButton(Graphics& g, ToggleButton& button,
									  bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
	auto borderColor = WHITE;
	auto backgroundColor = LIGHT_GREY;
	auto onSauceColor = PINK;
	auto offSauceColor = GREY;
	auto textColor = WHITE.withAlpha(0.9f);

	bool isOn = button.getToggleState();

	if (!button.isEnabled())
		g.setOpacity(0.5f);

	Rectangle<float> border = button.getLocalBounds().toFloat().withSizeKeepingCentre(Utils::scale(53), Utils::scale(20));
	g.setColour(borderColor);
	g.fillRect(border);

	Rectangle<float> inside = border.reduced(Utils::scale(4));
	Rectangle<float> sauce = isOn ? inside.removeFromRight(inside.getWidth() * 2/3) : inside.removeFromLeft(inside.getWidth() * 2/3);

	g.setColour(backgroundColor);
	g.fillRect(inside);

	g.setColour(isOn ? onSauceColor : offSauceColor);
	g.fillRect(sauce);

	g.setColour(textColor);
	g.drawFittedText(isOn ? "On" : "Off", sauce.toNearestInt(), Justification::centred, 1);
}

void NumboxLAF::drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos,
								 float maxSliderPos, const Slider::SliderStyle style, Slider& slider)
{
	auto borderColor = WHITE;
	auto backgroundColor = LIGHT_GREY;

	Rectangle<float> bounds(x, y, width, height);
	Rectangle<float> border = bounds.withSizeKeepingCentre(Utils::scale(22), Utils::scale(22));
	g.setColour(borderColor);
	g.fillRect(border);

	Rectangle<float> inside = border.reduced(Utils::scale(4));
	g.setColour(backgroundColor);
	g.fillRect(inside);
}
