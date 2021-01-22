#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SamplerSynth.h"
#include "ProcessBlockInfo.h"
#include "KeyswitchManager.h"
#include "../Graphics/MyLookAndFeel.h"

const int ALL_CHANNELS = 0xFFFFFFFF;
const int NUM_MIDI_KEYS = 128;
const int NUM_MIDI_CHANNELS = 16;
const double MAX_SAMPLE_LENGTH_SEC = 20.0;

const float EPSILON = 0.00001f;
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

    void loadNewFile(String filePath);

    AudioProcessorValueTreeState& getVTS() { return parameters; }
    KeyswitchManager& getKeyswitchManager() { return keyswitchManager; }
    NumboxLAF& getNumboxLAF() { return numboxLAF; }
    AudioThumbnail& getAudioThumbnail() { return audioThumbnail; }

private:

    AudioProcessorValueTreeState parameters;
    MidiKeyboardState physicalKeyboardState;
    KeyswitchManager keyswitchManager;
    SamplerSound *samplerSound;
    Random random;

    MyLookAndFeel myLookAndFeel;
    NumboxLAF numboxLAF;
    
    // for the user-selected sample
    AudioFormatManager audioFormatManager;
    AudioThumbnailCache audioThumbnailCache;
    AudioThumbnail audioThumbnail;
    std::unique_ptr<AudioFormatReader> audioFormatReader;
    AudioBuffer<float> audioBuffer;

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

    // called once per audio callback
    void updateADSR();
    void updatePitch(MidiBuffer& midiMessages);
    void updateLevel(AudioBuffer<float>& buffer);
    void updatePan(AudioBuffer<float>& buffer);
    
    void changeSound();
    
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void linkParameterValues();

    Value pitchParameter;
    Value panParameter;
    Value levelParameter;

    float prevLevel;
    float prevPanLeftLevel;
    float prevPanRightLevel;

    Value attackParameter;
    Value decayParameter;
    Value sustainParameter;
    Value releaseParameter;

    Value swingParameter;
    Value humanizeParameter;

    Value easyParameter;
    Value latchParameter;
    Value keyswitchOctaveParameter;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyRepeatAudioProcessor)
};
