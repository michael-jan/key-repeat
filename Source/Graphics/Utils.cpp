/*
  ==============================================================================

    Utils.cpp
    Created: 19 Jun 2019 1:03:55pm
    Author:  Michael Jan

  ==============================================================================
*/

#include "Utils.h"

const int Utils::ORIGINAL_WIDTH = 660;
const int Utils::ORIGINAL_HEIGHT = 315;
const double Utils::SCALE_FACTOR = 1.2;

int Utils::scale(int dimension) {
	return roundToInt(dimension * SCALE_FACTOR);
}

int Utils::getTextWidth(Label& label) {
	return label.getFont().getStringWidth(label.getText());
}

void Utils::attachToSlider(Label& label, Slider& slider, int offset) {
	label.setJustificationType(Justification::centred);
	label.setFont(Font(17));

	Rectangle<int> labelBounds = slider.getBounds();

	int originalCentreX = labelBounds.getCentreX();
	labelBounds.setLeft(originalCentreX - getTextWidth(label));
	labelBounds.setRight(originalCentreX + getTextWidth(label));

	labelBounds.setTop(slider.getBounds().getBottom() + offset);
	labelBounds.setBottom(slider.getBounds().getBottom() + label.getFont().getHeight() + offset);

	label.setBounds(labelBounds);
}

void Utils::drawLineShadow(Graphics& g, Line<float> line, int spread) {
	Path linePath;
	linePath.addLineSegment(line, 1);
	DropShadow ds(Colours::black.withAlpha(0.5f), spread / 2, { 0, 0 });
	ds.drawForPath(g, linePath);
	DropShadow ds2(Colours::black, spread, { 0, 0 });
	ds2.drawForPath(g, linePath);
}