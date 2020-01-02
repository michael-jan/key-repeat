/*
  ==============================================================================

    KeySwitchManager.h
    Created: 29 May 2019 3:49:40am
    Author:  Michael Jan

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

struct KeyswitchDisplayInfoElement {
	String noteName;
	String keyswitchName;
	bool isActive;
	bool isEnabled;
};

class KeyswitchManager : public MidiKeyboardStateListener {

public:

	static const int NUM_KEYSWITCH_KEYS = 7;

	KeyswitchManager();
	~KeyswitchManager();

	bool isKeyswitch(int midiNode) const;

	const std::vector<double>& getCurrentTriggers(float swing);
	void update();

	void setKeyboardStatePointer(MidiKeyboardState *newKeyboardStatePointer);

	int getFirstKeyswitchNoteNumber() const { return DEFAULT_KEYSWITCH_NUM + 12 * keyswitchOctave; }
	void setKeyswitchOctave(int newKeyswitchOctave) { keyswitchOctave = newKeyswitchOctave; }

	void setSeparateTripletButton(bool newSeparateTripletButton) { separateTripletButton = newSeparateTripletButton; }
	void setLatch(bool newLatch) { latch = newLatch; }

	bool isRepeatOff() const { return currentRepeatState == Off; }

	KeyswitchDisplayInfoElement *getDisplayElements();

private:
		
	static const int NUM_REPEAT_NOTES = 10;
	static const int DEFAULT_KEYSWITCH_NUM = 24;
	static const int ALL_CHANNELS = 0xFFFFFFFF;

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

	KeyswitchDisplayInfoElement displayElements[7];

	RepeatState currentRepeatState;

	// KeySwitchManager is not responsible for the lifetime of the MidiKeyboardState
	MidiKeyboardState *keyboardStatePointer;

	int keyswitchOctave;
	bool separateTripletButton;
	bool latch;

	// Stores the times (in quarter notes) of when to trigger samples for
	// each repeat state. Should be filled upon construction.
	// Also, the order of these two apparently matter, and it appears that the reason is more than just destruction order.
	// Maybe some weird memory bug..
	std::vector<double> tempWhenToPlay;
	std::vector< std::vector<double> > whenToPlayInfo;

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
