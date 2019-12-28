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
const double Utils::DEFAULT_SCALE_FACTOR = winMac(1.4, 1.2*2);
double Utils::SCALE_FACTOR = DEFAULT_SCALE_FACTOR;

int Utils::scale(float dimension) {
	return roundToInt(dimension * SCALE_FACTOR);
}

int Utils::getTextWidth(Label& label) {
	return label.getFont().getStringWidth(label.getText());
}

void Utils::attachToComponent(Label& label, Component& component, int offset) {
	label.setJustificationType(Justification::centred);
	label.setFont(Font(scale(MyLookAndFeel::LABEL_FONT_SIZE)));

	Rectangle<int> labelBounds = component.getBounds();

	int originalCentreX = labelBounds.getCentreX();
	labelBounds.setLeft(originalCentreX - getTextWidth(label));
	labelBounds.setRight(originalCentreX + getTextWidth(label));

	labelBounds.setTop(component.getBounds().getBottom() + offset);
	labelBounds.setBottom(component.getBounds().getBottom() + label.getFont().getHeight() + offset);

	label.setBounds(labelBounds.translated(0, winMac(0, Utils::scale(2))));
}

void Utils::drawLineShadow(Graphics& g, Line<float> line, int spread, float alpha) {
	Path linePath;
	linePath.addLineSegment(line, 1);
	drawPathShadow(g, linePath, spread, alpha);
}

void Utils::drawPathShadow(Graphics& g, Path path, int spread, float alpha) {
	DropShadow ds(Colours::black.withAlpha(alpha), spread, { 0, 0 });
	ds.drawForPath(g, path);
}
