#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SamplerSynth.h"
#include "FileDropperComponent.h"

#define EPSILON 0.0001f

class KeyRepeatAudioProcessor :
	public AudioProcessor,
	public ChangeListener,
	public ChangeBroadcaster
{
public:

	SamplerSynth synth;
	FileDropperComponent fileDropperComponent;

	KeyRepeatAudioProcessor();
    ~KeyRepeatAudioProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

	void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	void changeListenerCallback(ChangeBroadcaster *source) override;

private:

	MidiKeyboardState physicalKeyboardState;

	enum RepeatState {
		Off,
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
	};
	RepeatState repeatState;
	std::vector< std::vector<double> > whenToPlayInfo;
	void fillWhenToPlay();

	// used when not playing/recording in timeline
	double fakeSamplesIntoMeasure;

	// because MidiKeyboardState does not store velocities
	float midiVelocities[128];

	// used in hack to avoid double-tapping on beat 0 aka beat 4
	double lastNextBeatsIntoMeasure;
	bool wasLastHitOnFour;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyRepeatAudioProcessor)
};