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
	wasLastHitOnFour = false;
	lastNextBeatsIntoMeasure = 0.0;
	fakeSamplesIntoMeasure = 0.0;

	keySwitchManager.setLatch(true);
	keySwitchManager.setSeparateTripletButton(true);
	keySwitchManager.setKeyboardStatePointer(&physicalKeyboardState);

	synth.setup();
	synth.setCurrentPlaybackSampleRate(sampleRate);
}

void KeyRepeatAudioProcessor::releaseResources() {
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
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

void KeyRepeatAudioProcessor::fillProcessBlockInfo(ProcessBlockInfo& info, const AudioBuffer<float>& buffer) {

	double bpm = 120; // default, will be later set by host
	info.samplesPerSecond = getSampleRate();
	info.bufferNumSamples = buffer.getNumSamples();

	AudioPlayHead *const playHead = getPlayHead();

	AudioPlayHead::CurrentPositionInfo currPosInfo;
	if (playHead != nullptr) {
		playHead->getCurrentPosition(currPosInfo);
		bpm = currPosInfo.bpm;
	}

	if (playHead != nullptr && currPosInfo.isPlaying) {
		// playing in timeline
		double ppqPosition = currPosInfo.ppqPosition;

		info.samplesPerBeat = info.samplesPerSecond * 60 / bpm;
		info.samplesPerMeasure = info.samplesPerBeat * 4;
		info.beatsIntoMeasure = std::fmod(ppqPosition, 4.0);
		info.samplesIntoMeasure = info.samplesPerBeat * info.beatsIntoMeasure;
	} else {
		// not playing in timeline
		info.samplesPerBeat = info.samplesPerSecond * 60 / bpm;
		info.samplesPerMeasure = info.samplesPerBeat * 4;
		info.samplesIntoMeasure = fakeSamplesIntoMeasure;
		info.beatsIntoMeasure = info.samplesIntoMeasure / info.samplesPerBeat;
	}

	info.nextBeatsIntoMeasure = info.beatsIntoMeasure + (info.bufferNumSamples / info.samplesPerBeat);
}

void KeyRepeatAudioProcessor::updateKeyboardState(const MidiBuffer& midiMessages) {
	MidiBuffer::Iterator midiIterator(midiMessages);
	MidiMessage m;
	int pos;
	while (midiIterator.getNextEvent(m, pos)) {
		physicalKeyboardState.processNextMidiEvent(m);
		if (m.isNoteOn()) {
			midiVelocities[m.getChannel()][m.getNoteNumber()] = m.getFloatVelocity();
		}
	}
}

void KeyRepeatAudioProcessor::addAllNonKeyswitchMidiMessages(MidiBuffer& newMidiMessages, const MidiBuffer& midiMessages) {
	MidiBuffer::Iterator midiIterator(midiMessages);
	MidiMessage m;
	int pos;
	while (midiIterator.getNextEvent(m, pos)) {
		if (!keySwitchManager.isKeyswitch(m.getNoteNumber())) {
			newMidiMessages.addEvent(m, pos);
		}
	}
}

void KeyRepeatAudioProcessor::transformMidiMessages(MidiBuffer& newMidiMessages, const ProcessBlockInfo& info) {

	std::vector<double> *triggers = &keySwitchManager.getCurrentTriggers();

	DBG(" ");
	DBG(info.beatsIntoMeasure);
	DBG(info.nextBeatsIntoMeasure);

	for (double triggerInBeats : *triggers) {
		if (info.beatsIntoMeasure - EPSILON <= triggerInBeats && triggerInBeats < info.nextBeatsIntoMeasure - EPSILON) {

			// hack to avoid double-tapping on beat 0 aka beat 4
			if (wasLastHitOnFour && std::fabs(triggerInBeats) < EPSILON) continue;
			if (std::fabs(triggerInBeats - 4.0) < EPSILON) wasLastHitOnFour = true;
			else wasLastHitOnFour = false;

			// we want our note repeats to be sample-accurate
			int internalSample = (int)((info.samplesPerMeasure * triggerInBeats / 4) - info.samplesIntoMeasure);
			DBG("triggered, internalSample=" + std::to_string(internalSample));

			for (int note = 0; note < NUM_MIDI_KEYS; note++) {
				for (int midiChannel = 1; midiChannel <= NUM_MIDI_CHANNELS; midiChannel++) {
					if (physicalKeyboardState.isNoteOn(midiChannel, note) && !keySwitchManager.isKeyswitch(note)) {
						newMidiMessages.addEvent(MidiMessage::noteOn(midiChannel, note, midiVelocities[midiChannel][note]), internalSample);
					}
				}
			}

		}
	}

}


void KeyRepeatAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {

	ScopedNoDenormals noDenormals;
	int totalNumInputChannels = getTotalNumInputChannels();
	int totalNumOutputChannels = getTotalNumOutputChannels();

	for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
		buffer.clear(i, 0, buffer.getNumSamples());
	}

	ProcessBlockInfo info;
	fillProcessBlockInfo(info, buffer);

	updateKeyboardState(midiMessages);
	keySwitchManager.update();

	MidiBuffer newMidiMessages;
	if (keySwitchManager.isRepeatOff()) {
		// if repeat is off, just act like a normal midi sampler
		addAllNonKeyswitchMidiMessages(newMidiMessages, midiMessages);
	} else {
		// if repeat is on, the we must use our given midi messages
		// to generate the actual midi messages that will be played
		transformMidiMessages(newMidiMessages, info);
	}
	
	// fill the audio buffer with sounds using the new midi messages
	synth.renderNextBlock(buffer, newMidiMessages, 0, info.bufferNumSamples);

	// increment the fake samples counter with modular arithmetic
	fakeSamplesIntoMeasure = std::fmod(fakeSamplesIntoMeasure + info.bufferNumSamples, info.samplesPerMeasure);

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
