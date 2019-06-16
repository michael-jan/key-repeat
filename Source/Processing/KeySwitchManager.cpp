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
	keyswitchOctave(0),
	separateTripletButton(true),
	latch(false),
	isTripletLatched(false)
{
	fillWhenToPlayInfo();
}

KeySwitchManager::~KeySwitchManager() {
}

bool KeySwitchManager::isKeyswitch(int midiNode) const {
	return getFirstKeyswitchNoteNumber() <= midiNode && midiNode < getFirstKeyswitchNoteNumber() + NUM_KEYSWITCH_KEYS;
}

std::vector<double>& KeySwitchManager::getCurrentTriggers(float swing) {
	tempWhenToPlay = whenToPlayInfo[currentRepeatState];
	if (!isRepeatOff() && !isCurrentRepeatStateTriplet()) {
		double diff = tempWhenToPlay[1] - tempWhenToPlay[0];
		double swingHangTime = swing * diff;
		bool isIndexOdd = false;
		for (double& num : tempWhenToPlay) {
			if (isIndexOdd) {
				num += swingHangTime;
			}
			isIndexOdd = !isIndexOdd;
		}
	}
	return tempWhenToPlay;
}


void KeySwitchManager::update() {

	// With latch off, repeat state is reset to Off unless otherwise specified.
	// With latch on, repeat state simply holds it's previous value until other specified.
	if (!latch) {
		currentRepeatState = Off;
	}

	int lo = getFirstKeyswitchNoteNumber();

	if (separateTripletButton) {
		// [4] [8] [16] [32] [64]   |  [T]  |  [Off]
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
		// [8] [8T] [16] [16T] [32] [32T]  |  [Off]
		int hi = lo + NUM_REPEAT_NOTES - 4;
		for (int note = lo; note < hi; note++) {
			if (isNoteOn(note)) {
				currentRepeatState = (RepeatState)(note - lo + 2);
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
	return getFirstKeyswitchNoteNumber() + NUM_KEYSWITCH_KEYS - 1;
}

int KeySwitchManager::getTripletNoteNumber() const {
	if (separateTripletButton) {
		return getFirstKeyswitchNoteNumber() + NUM_KEYSWITCH_KEYS - 2;
	}
	return -1;
}

void KeySwitchManager::fillWhenToPlayInfo() {
	int playsPerMeasure[] =
	{ 
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

}

void KeySwitchManager::handleNoteOn(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) {
	// if the triplet button is pressed, toggle triplet on current repeat state
	if (latch && separateTripletButton) {
		if (source == keyboardStatePointer && currentRepeatState != Off && midiNoteNumber == getTripletNoteNumber()) {
			isTripletLatched = !isTripletLatched;
			if (isTripletLatched) {
				currentRepeatState = (RepeatState)(currentRepeatState + 1);
			} else {
				currentRepeatState = (RepeatState)(currentRepeatState - 1);
			}
		}
	}
}

void KeySwitchManager::handleNoteOff(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) {
	// do nothing
}
