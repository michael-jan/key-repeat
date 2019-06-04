/*
  ==============================================================================

    KeySwitchManager.h
    Created: 29 May 2019 3:49:40am
    Author:  Michael Jan

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class KeySwitchManager : public MidiKeyboardStateListener {

public:

	KeySwitchManager();
	~KeySwitchManager();

	bool isKeyswitch(int midiNode) const;

	std::vector<double>& getCurrentTriggers(float swing);
	void update();

	void setKeyboardStatePointer(MidiKeyboardState *newKeyboardStatePointer);

	int getFirstKeyswitchNoteNumber() const { return DEFAULT_KEYSWITCH_NUM + 12 * keyswitchOctave; }
	void setKeyswitchOctave(int newKeyswitchOctave) { keyswitchOctave = newKeyswitchOctave; }

	bool isSeparateTripletButton() const { return separateTripletButton; }
	void setSeparateTripletButton(bool newSeparateTripletButton) { separateTripletButton = newSeparateTripletButton; }

	bool isLatch() const { return latch; }
	void setLatch(bool newLatch) { latch = newLatch; }

	bool isRepeatOff() const { return currentRepeatState == Off; }

private:
		
	const int NUM_REPEAT_NOTES = 10;
	const int NUM_KEYSWITCH_KEYS = 7;
	const int DEFAULT_KEYSWITCH_NUM = 24;
	const int ALL_CHANNELS = 0xFFFFFFFF;

	enum RepeatState {
		Quarter,
		QuarterTriplet,
		Eighth,
		EighthTriplet,
		Sixteenth,
		SixteenthTriplet,
		ThirtySecond,
		ThirtySecondTriplet,
		SixtyFourth,
		SixtyFourthTriplet,
		Off
	};

	RepeatState currentRepeatState;

	// KeySwitchManager is not responsible for the lifetime of the MidiKeyboardState
	MidiKeyboardState *keyboardStatePointer;

	int keyswitchOctave;
	bool separateTripletButton;
	bool latch;

	// Stores the times (in quarter notes) of when to trigger samples for
	// each repeat state. Should be filled upon construction.
	std::vector< std::vector<double> > whenToPlayInfo;
	std::vector<double> tempWhenToPlay;

	bool isNoteOn(int noteNumber) const;
	int getOffNoteNumber() const;
	int getTripletNoteNumber() const;
	void fillWhenToPlayInfo();

	bool isCurrentRepeatStateTriplet() { return currentRepeatState % 2 == 1; }

	// toggleable triplet flag
	// valid only when both latch and separateTripletButton are on
	bool isTripletLatched;

	void handleNoteOn(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;
	void handleNoteOff(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;

};