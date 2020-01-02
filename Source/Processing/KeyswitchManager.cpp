/*
  ==============================================================================

    KeySwitchManager.cpp
    Created: 29 May 2019 3:49:40am
    Author:  Michael Jan

  ==============================================================================
*/

#include "KeyswitchManager.h"

KeyswitchManager::KeyswitchManager() :
	currentRepeatState(Off),
	keyswitchOctave(0),
	separateTripletButton(true),
	latch(false),
	isTripletLatched(false)
{
	fillWhenToPlayInfo();
}

KeyswitchManager::~KeyswitchManager() {
}

bool KeyswitchManager::isKeyswitch(int midiNode) const {
	return getFirstKeyswitchNoteNumber() <= midiNode && midiNode < getFirstKeyswitchNoteNumber() + NUM_KEYSWITCH_KEYS;
}

const std::vector<double>& KeyswitchManager::getCurrentTriggers(float swing) {
	tempWhenToPlay.clear();
	for (double num : whenToPlayInfo[currentRepeatState]) {
		tempWhenToPlay.push_back(num);
	}

	if (!isRepeatOff() && !isCurrentRepeatStateTriplet()) {
		double diff = tempWhenToPlay[1] - tempWhenToPlay[0];
		double swingHangTime = swing * diff;
		for (int i = 1; i < tempWhenToPlay.size(); i += 2) {
			tempWhenToPlay[i] += swingHangTime;
		}
	}

	return tempWhenToPlay;
}

void KeyswitchManager::update() {
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

void KeyswitchManager::setKeyboardStatePointer(MidiKeyboardState *newKeyboardStatePointer) {
	keyboardStatePointer = newKeyboardStatePointer;
	keyboardStatePointer->addListener(this);
}

bool KeyswitchManager::isNoteOn(int noteNumber) const {
	return keyboardStatePointer->isNoteOnForChannels(ALL_CHANNELS, noteNumber);
}

int KeyswitchManager::getOffNoteNumber() const {
	return getFirstKeyswitchNoteNumber() + NUM_KEYSWITCH_KEYS - 1;
}

int KeyswitchManager::getTripletNoteNumber() const {
	if (separateTripletButton) {
		return getFirstKeyswitchNoteNumber() + NUM_KEYSWITCH_KEYS - 2;
	}
	return -1;
}

void KeyswitchManager::fillWhenToPlayInfo() {
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

KeyswitchDisplayInfoElement* KeyswitchManager::getDisplayElements() {
	static String noteNames[4][7] = { 
		{ "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0" },
		{ "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1" },
		{ "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2" }
	};
	static String keyswitchNamesSeparateTripletButton[] = { "1/4", "1/8", "1/16", "1/32", "1/64", "Trip", "Off" };
	static String keyswitchNamesNoSeparateTripletButton[] = { "1/8", "1/8T", "1/16", "1/16T", "1/32", "1/32T", "Off" };
	String *keyswitchNames = separateTripletButton ? keyswitchNamesSeparateTripletButton : keyswitchNamesNoSeparateTripletButton;

	for (int i = 0; i < NUM_KEYSWITCH_KEYS; i++) {
		displayElements[i].noteName = noteNames[keyswitchOctave][i];
		displayElements[i].keyswitchName = keyswitchNames[i];
		displayElements[i].isActive = false;
		displayElements[i].isEnabled = true;
	}

    int offIndex = getOffNoteNumber() - getFirstKeyswitchNoteNumber();

	if (separateTripletButton) {
		int tripletIndex = getTripletNoteNumber() - getFirstKeyswitchNoteNumber();
		displayElements[currentRepeatState / 2].isActive = true;
		displayElements[tripletIndex].isActive = currentRepeatState % 2 == 1;
	} else {
        // Ideally this should be just currentRepeatState - 2, but the jmin catches an
        // out-of-bounds that probably occurs in the edge case where the separateTripletButton flag
        // is stale for a moment before updating.
		displayElements[jmin(currentRepeatState - 2, NUM_KEYSWITCH_KEYS - 1)].isActive = true;
	}

	displayElements[offIndex].isActive = isNoteOn(getOffNoteNumber());
	displayElements[offIndex].isEnabled = latch;
	return displayElements;
}

void KeyswitchManager::handleNoteOn(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) {
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

void KeyswitchManager::handleNoteOff(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) {
	// no-op
}
