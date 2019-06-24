/*
  ==============================================================================

	ParameterLabel.h
	Created: 8 Jun 2019 9:31:57pm
	Author:  Michael Jan

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ParameterLabel : public Label {
public:
	ParameterLabel(const String& componentName, const String& labelText)
		: Label(componentName, labelText)
	{
		setJustificationType(Justification::centredBottom);
		setFont(Font(17));
	}
private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterLabel)
};