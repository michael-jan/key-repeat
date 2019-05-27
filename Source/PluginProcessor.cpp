#include "PluginProcessor.h"
#include "PluginEditor.h"

KeyRepeatAudioProcessor::KeyRepeatAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	//transportSource.addChangeListener(this);
}

KeyRepeatAudioProcessor::~KeyRepeatAudioProcessor() {

}

const String KeyRepeatAudioProcessor::getName() const {
    return JucePlugin_Name;
}

bool KeyRepeatAudioProcessor::acceptsMidi() const {
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KeyRepeatAudioProcessor::producesMidi() const {
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KeyRepeatAudioProcessor::isMidiEffect() const {
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double KeyRepeatAudioProcessor::getTailLengthSeconds() const {
    return 0.0;
}

int KeyRepeatAudioProcessor::getNumPrograms() {
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KeyRepeatAudioProcessor::getCurrentProgram() {
    return 0;
}

void KeyRepeatAudioProcessor::setCurrentProgram(int index) {
}

const String KeyRepeatAudioProcessor::getProgramName(int index) {
    return {};
}

void KeyRepeatAudioProcessor::changeProgramName(int index, const String& newName) {
}

//==============================================================================
void KeyRepeatAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	//transportSource.prepareToPlay(samplesPerBlock, sampleRate);
	synth.setup();
	synth.setCurrentPlaybackSampleRate(sampleRate);
}

void KeyRepeatAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
	//transportSource.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KeyrepeatAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void KeyRepeatAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {
	/*
    ScopedNoDenormals noDenormals;
    int totalNumInputChannels  = getTotalNumInputChannels();
    int totalNumOutputChannels = getTotalNumOutputChannels();

	for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
		buffer.clear(i, 0, buffer.getNumSamples());
	}
	
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        float* channelData = buffer.getWritePointer (channel);
    }
	*/

	AudioPlayHead *const playHead = getPlayHead();
	double bpm = 120;
	double ppqPosition = 0;
	if (playHead != nullptr) {
		AudioPlayHead::CurrentPositionInfo currPosInfo;
		playHead->getCurrentPosition(currPosInfo);
		bpm = currPosInfo.bpm;
		ppqPosition = currPosInfo.ppqPosition;
		DBG("bpm=" + std::to_string(bpm));
		DBG("ppqPosition=" + std::to_string(ppqPosition));
	}
	double sRate = getSampleRate();
	double samplesPerMeasure = sRate * 60 * 4 / bpm; // assumes 4/4 time sig

	//AudioSourceChannelInfo channelInfo(buffer);
	//transportSource.getNextAudioBlock(channelInfo);
	synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool KeyRepeatAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* KeyRepeatAudioProcessor::createEditor() {
    return new KeyRepeatAudioProcessorEditor (*this);
}

//==============================================================================
void KeyRepeatAudioProcessor::getStateInformation (MemoryBlock& destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void KeyRepeatAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void KeyRepeatAudioProcessor::changeListenerCallback(ChangeBroadcaster *source) {
	//if (source == &transportSource) {
	//	sendChangeMessage();
	//}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new KeyRepeatAudioProcessor();
}
