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

	std::vector<double>& getCurrentTriggers() { DBG(currentRepeatState); return whenToPlayInfo[currentRepeatState]; }
	void update();

	void setKeyboardStatePointer(MidiKeyboardState *newKeyboardStatePointer);

	int getFirstKeyswitchNoteNumber() const { return firstKeyswitchNoteNumber; }
	void setFirstKeyswitchNoteNumber(int newFirstKeyswitchNoteNumber) { firstKeyswitchNoteNumber = newFirstKeyswitchNoteNumber; }

	bool isSeparateTripletButton() const { return separateTripletButton; }
	void setSeparateTripletButton(bool newSeparateTripletButton) { separateTripletButton = newSeparateTripletButton; }

	bool isLatch() const { return latch; }
	void setLatch(bool newLatch) { latch = newLatch; }

	bool isRepeatOff() const { return currentRepeatState == Off; }

private:
		
	const int NUM_REPEAT_NOTES = 12;
	const int ALL_CHANNELS = 0xFFFFFFFF;

	enum RepeatState {
		Half,
		HalfTriplet,
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

	int firstKeyswitchNoteNumber;
	bool separateTripletButton;
	bool latch;

	// Stores the times (in quarter notes) of when to trigger samples for
	// each repeat state. Should be filled upon construction.
	std::vector< std::vector<double> > whenToPlayInfo;

	bool isNoteOn(int noteNumber) const;
	int getOffNoteNumber() const;
	int getTripletNoteNumber() const;
	void fillWhenToPlayInfo();

	// toggleable triplet flag
	// valid only when both latch and separateTripletButton are on
	bool isTripletLatched;

	// 
	void handleNoteOn(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;
	void handleNoteOff(MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;

};