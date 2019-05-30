/*
  ==============================================================================

    KeySwitchManager.cpp
    Created: 29 May 2019 3:49:40am
    Author:  Michael Jan

  ==============================================================================
*/

#include "KeySwitchManager.h"

KeySwitchManager::KeySwitchManager() :
	currentRepeatState(Off),
	firstKeyswitchNoteNumber(21),
	separateTripletButton(false),
	latch(false),
	isTripletLatched(false)
{
	fillWhenToPlayInfo();
}

KeySwitchManager::~KeySwitchManager() {
}

bool KeySwitchManager::isKeyswitch(int midiNode) const {
	if (separateTripletButton) {
		// NUM_REPEAT_NOTES + 2 (one for Off button and one for the separate triplet button)
		return firstKeyswitchNoteNumber <= midiNode && midiNode < firstKeyswitchNoteNumber + NUM_REPEAT_NOTES / 2 + 2;
	} else {
		// NUM_REPEAT_NOTES + 1 (for Off button)
		return firstKeyswitchNoteNumber <= midiNode && midiNode < firstKeyswitchNoteNumber + NUM_REPEAT_NOTES + 1;
	}
}

void KeySwitchManager::update() {

	// With latch off, repeat state is reset to Off unless otherwise specified.
	// With latch on, repeat state simply holds it's previous value until other specified.
	if (!latch) {
		currentRepeatState = Off;
	}

	int lo = firstKeyswitchNoteNumber;

	if (separateTripletButton) {
		// [2] [4] [8] [16] [32] [64] [T] [Off]
		int hi = lo + NUM_REPEAT_NOTES / 2;
		for (int note = lo; note < hi; note++) {
			if (isNoteOn(note)) {
				currentRepeatState = (RepeatState)((note - lo) * 2);

				// if latch is off and triplet note is held, make triplet
				if (!latch && isNoteOn(getTripletNoteNumber())) {
					currentRepeatState = (RepeatState)(currentRepeatState + 1);
				}

				// if latch is on and triplet is latched, make triplet
				if (latch && isTripletLatched) {
					currentRepeatState = (RepeatState)(currentRepeatState + 1);
				}
			}
		}
	} else {
		// [2] [2T] [4] [4T] [8] [8T] [16] [16T] [32] [32T] [64] [64T] [Off]
		int hi = lo + NUM_REPEAT_NOTES;
		for (int note = lo; note < hi; note++) {
			if (isNoteOn(note)) {
				currentRepeatState = (RepeatState)(note - lo);
			}
		}
	}

	if (isNoteOn(getOffNoteNumber())) {
		currentRepeatState = Off;
	} 
}

void KeySwitchManager::setKeyboardStatePointer(MidiKeyboardState *newKeyboardStatePointer) {
	keyboardStatePointer = newKeyboardStatePointer;
	keyboardStatePointer->addListener(this);
}

bool KeySwitchManager::isNoteOn(int noteNumber) const {
	return keyboardStatePointer->isNoteOnForChannels(ALL_CHANNELS, noteNumber);
}

int KeySwitchManager::getOffNoteNumber() const {
	if (separateTripletButton) {
		return firstKeyswitchNoteNumber + NUM_REPEAT_NOTES / 2 + 1;
	} else {
		return firstKeyswitchNoteNumber + NUM_REPEAT_NOTES;
	}
}

int KeySwitchManager::getTripletNoteNumber() const {
	if (separateTripletButton) {
		return firstKeyswitchNoteNumber + NUM_REPEAT_NOTES / 2;
	}
	return -1;
}

void KeySwitchManager::fillWhenToPlayInfo() {
	int playsPerMeasure[] =
	{   2,  // Half
		3,  // HalfTriplet
		4,  // Quarter
		6,  // QuarterTriplet
		8,  // Eighth
		12, // EighthTriplet
		16, // Sixteenth
		24, // SixteenthTriplet
		32, // ThirtySecond
		48, // ThirtySecondTriplet
		64, // SixtyFourth
		96, // SixtyFourthTriplet 
		-1  // Off
	};

	// fill in the trigger times for the rest of the RepeatStates
	for (int i = 0; i < NUM_REPEAT_NOTES; i++) {
		std::vector<double> temp;
		temp.push_back(0.0);
		for (int j = 1; j < playsPerMeasure[i]; j++) {
			temp.push_back(4.0 / playsPerMeasure[i] * j);
		}
		temp.push_back(4.0);
		whenToPlayInfo.push_back(temp);
	}

	// corresponds to "Off"
	std::vector<double> dummy;
}

void KeySwitchManager::handleNoteOn(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) {
	// if the triplet button is pressed, toggle triplet on current repeat state
	if (source == keyboardStatePointer && latch && separateTripletButton && midiNoteNumber == getTripletNoteNumber()) {
		isTripletLatched = !isTripletLatched;
		if (isTripletLatched) {
			currentRepeatState = (RepeatState)(currentRepeatState + 1);
		} else {
			currentRepeatState = (RepeatState)(currentRepeatState - 1);
		}
	}
}

void KeySwitchManager::handleNoteOff(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) {
	// do nothing
}
