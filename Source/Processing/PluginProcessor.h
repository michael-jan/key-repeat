#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SamplerSynth.h"
#include "ProcessBlockInfo.h"
#include "KeySwitchManager.h"


const bool DEBUG_TRIGGER = false;

const int ALL_CHANNELS = 0xFFFFFFFF;
const int NUM_MIDI_KEYS = 128;
const int NUM_MIDI_CHANNELS = 16;
const double MAX_SAMPLE_LENGTH_SEC = 20.0;

const float EPSILON = 0.0001f;
const double PI = 3.141592653589793238463;


class KeyRepeatAudioProcessor :
	public AudioProcessor
{
public:

	SamplerSynth synth;

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

	void loadNewFile(AudioFormatReader *reader);

	AudioProcessorValueTreeState& getVTS() { return parameters; }

private:

	AudioProcessorValueTreeState parameters;
	MidiKeyboardState physicalKeyboardState;
	KeySwitchManager keySwitchManager;
	SamplerSound *samplerSound;

	// used when not playing/recording in timeline
	double fakeSamplesIntoMeasure;

	// because MidiKeyboardState does not store velocity info
	float midiVelocities[NUM_MIDI_CHANNELS][NUM_MIDI_KEYS];

	// used in hack to avoid double-tapping on beat 0 aka beat 4
	double lastNextBeatsIntoMeasure;
	bool wasLastHitOnFour;


	void fillProcessBlockInfo(ProcessBlockInfo& info, const AudioBuffer<float>& buffer);
	void updateKeyboardState(const MidiBuffer& midiMessages);
	void addAllNonKeyswitchMidiMessages(MidiBuffer& newMidiMessages, const MidiBuffer& midiMessages);
	void transformMidiMessages(MidiBuffer& newMidiMessages, const ProcessBlockInfo& info);

	void updateADSR();
	void updatePitch(MidiBuffer& midiMessages);
	void updateLevel(AudioBuffer<float>& buffer);
	void updatePan(AudioBuffer<float>& buffer);


	AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

	float *pitchParameter;
	float *panParameter;
	float *levelParameter;

	float prevLevel;
	float prevPanLeftLevel;
	float prevPanRightLevel;

	float *attackParameter;
	float *decayParameter;
	float *sustainParameter;
	float *releaseParameter;

	float *swingParameter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyRepeatAudioProcessor)
};