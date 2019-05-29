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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

	wasLastHitOnFour = false;
	lastNextBeatsIntoMeasure = 0.0;
	fakeSamplesIntoMeasure = 0.0;
	std::fill_n(midiVelocities, 128, 0.0);
	repeatState = Off;

	fillWhenToPlayInfo();

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

void KeyRepeatAudioProcessor::fillWhenToPlayInfo() {
	int playsPerMeasure[] =
	{	2,  // Half
		3,  // HalfTriplet
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
	int numKeySwitches = 12;
	for (int i = 0; i < numKeySwitches; i++) {
		std::vector<double> temp;
		temp.push_back(0.0);
		for (int j = 1; j < playsPerMeasure[i]; j++) {
			temp.push_back(4.0 / playsPerMeasure[i] * j);
		}
		temp.push_back(4.0);
		whenToPlayInfo.push_back(temp);
	}

	// corresponds to "Off"
	std::vector<double> dummy;
	whenToPlayInfo.push_back(dummy);


}

void KeyRepeatAudioProcessor::fillProcessBlockInfo(ProcessBlockInfo& info, AudioBuffer<float>& buffer) {

	int bpm = 120; // default, will be later set by host
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

void KeyRepeatAudioProcessor::updateKeyboardState(MidiBuffer& midiMessages) {
	MidiBuffer::Iterator midiIterator(midiMessages);
	MidiMessage m;
	int pos;

	while (midiIterator.getNextEvent(m, pos)) {
		physicalKeyboardState.processNextMidiEvent(m);
		if (m.isNoteOn()) {
			midiVelocities[m.getNoteNumber()] = m.getFloatVelocity();
		}
	}
}

void KeyRepeatAudioProcessor::updateKeyswitchRepeatState() {
	int lo = 21; // note number for A0
	int hi = lo + 12;
	repeatState = Off;
	for (int note = lo; note <= hi; note++) {
		if (physicalKeyboardState.isNoteOnForChannels(ALL_CHANNELS, note)) {
			repeatState = (RepeatState) (note - lo);
		}
	}
}

void KeyRepeatAudioProcessor::transformMidiMessages(MidiBuffer& midiMessages, MidiBuffer& newMidiMessages, ProcessBlockInfo& info) {

	MidiBuffer::Iterator midiIterator(midiMessages);
	MidiMessage m;
	int pos;

	std::vector<double> *triggers = &whenToPlayInfo[repeatState];

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

			for (int note = 0; note < 128; note++) {
				for (int midiChannel = 1; midiChannel <= 16; midiChannel++) {
					if (physicalKeyboardState.isNoteOn(midiChannel, note) && note > 21 + 12) {
						newMidiMessages.addEvent(MidiMessage::noteOn(midiChannel, note, midiVelocities[note]), internalSample);
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

	MidiBuffer newMidiMessages;
	updateKeyboardState(midiMessages);
	updateKeyswitchRepeatState();

	if (repeatState == Off) {
		// if repeat is off, just act like a normal midi sampler
		newMidiMessages.swapWith(midiMessages);
	} else {
		// if repeat is on, the we must use our given midi messages
		// to generate the actual midi messages that will be played
		transformMidiMessages(midiMessages, newMidiMessages, info);
	}
	
	// fill the audio buffer with sounds given the new midi messages
	synth.renderNextBlock(buffer, newMidiMessages, 0, info.bufferNumSamples);

	// increment fake samples with modular arithmetic
	fakeSamplesIntoMeasure = std::fmod(fakeSamplesIntoMeasure + info.bufferNumSamples, info.samplesPerMeasure);

}

//==============================================================================
bool KeyRepeatAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* KeyRepeatAudioProcessor::createEditor() {
	DBG("new editor");
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
