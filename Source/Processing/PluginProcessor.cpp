#include "PluginProcessor.h"
#include "PluginEditor.h"

KeyRepeatAudioProcessor::KeyRepeatAudioProcessor() :
    parameters(*this, nullptr, Identifier("PARAMETERS"), createParameterLayout()),
    samplerSound(nullptr),
    audioThumbnailCache(2),
    audioThumbnail(1, audioFormatManager, audioThumbnailCache),
    prevLevel((float) levelParameter.getValue()),
    prevPanLeftLevel(std::sin(PI)),
    prevPanRightLevel(std::sin(PI))
#ifndef JucePlugin_PreferredChannelConfigurations
     , AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    audioFormatManager.registerBasicFormats();
    linkParameterValues();
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

    keyswitchManager.setKeyboardStatePointer(&physicalKeyboardState);

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
        if (!keyswitchManager.isKeyswitch(m.getNoteNumber())) {
            newMidiMessages.addEvent(m, pos);
        }
    }
}

void KeyRepeatAudioProcessor::transformMidiMessages(MidiBuffer& newMidiMessages, const ProcessBlockInfo& info) {
    std::vector<double> triggers = keyswitchManager.getCurrentTriggers(swingParameter.getValue());
    for (double& triggerInBeats : triggers) {
        if (info.beatsIntoMeasure - EPSILON <= triggerInBeats && triggerInBeats < info.nextBeatsIntoMeasure - EPSILON) {
            
            // hack to avoid double-tapping on beat 0/beat 4
            if (wasLastHitOnFour && std::fabs(triggerInBeats) < EPSILON) continue;
            if (std::fabs(triggerInBeats - 4.0) < EPSILON) wasLastHitOnFour = true;
            else wasLastHitOnFour = false;
            
            // we want our note repeats to be sample-accurate
            int internalSample = (int) ((info.samplesPerMeasure * triggerInBeats / 4) - info.samplesIntoMeasure);

            for (int note = 0; note < NUM_MIDI_KEYS; note++) {
                for (int midiChannel = 1; midiChannel <= NUM_MIDI_CHANNELS; midiChannel++) {
                    if (physicalKeyboardState.isNoteOn(midiChannel, note) && !keyswitchManager.isKeyswitch(note)) {
                        
                        // humanize parameter
                        // 0.0, 0.5, 1.0  linearly scaled in halves to  0.0, 0.25, 1.0
                        float velOffsetParam = (float)humanizeParameter.getValue();
                        float velOffset = velOffsetParam < 0.5f ? velOffsetParam * 0.5f : (velOffsetParam - 0.5f) * 1.5f + 0.25f;
                        velOffset *= 1.5f * (random.nextFloat() - 0.5);

                        float vel = jmax(0.0f, jmin(1.0f, midiVelocities[midiChannel][note] + velOffset));
                        newMidiMessages.addEvent(MidiMessage::noteOn(midiChannel, note, vel), internalSample);
                    }
                }
            }
        }
    }
}

void KeyRepeatAudioProcessor::updateADSR() {
    if (samplerSound != nullptr) {
        // Divide by 1000 to convert milliseconds -> seconds.
        // Use jmax() to safeguard against 0.0f for a, d, and r.
        float a = jmax(0.01f, (float) attackParameter.getValue()) / 1000.0f;
        float d = jmax(0.01f, (float) decayParameter.getValue()) / 1000.0f;
        float s = sustainParameter.getValue();
        float r = jmax(0.01f, (float) releaseParameter.getValue()) / 1000.0f;
        samplerSound->setEnvelopeParameters({ a, d, s, r });
    }
}

void KeyRepeatAudioProcessor::updatePitch(MidiBuffer& midiMessages) {
    MidiBuffer newMidiMessages;
    MidiBuffer::Iterator midiIterator(midiMessages);
    MidiMessage m;
    int pos;
    while (midiIterator.getNextEvent(m, pos)) {
        if (m.isNoteOn()) {
            int newNoteNumber = m.getNoteNumber() + (int) pitchParameter.getValue();
            if (0 <= newNoteNumber && newNoteNumber < NUM_MIDI_KEYS) {
                m.setNoteNumber(newNoteNumber);
                newMidiMessages.addEvent(m, pos);
            }
        }
    }
    newMidiMessages.swapWith(midiMessages);
}

void KeyRepeatAudioProcessor::updateLevel(AudioBuffer<float>& buffer) {
    for (int channel = 0; channel < buffer.getNumChannels(); channel++) {
        buffer.applyGainRamp(channel, 0, buffer.getNumSamples(), prevLevel, levelParameter.getValue());
    }
    prevLevel = levelParameter.getValue();
}

void KeyRepeatAudioProcessor::updatePan(AudioBuffer<float>& buffer) {
    if (buffer.getNumChannels() < 2) {
        return;
    }
    float arg = PI / 2 * ((float) panParameter.getValue() / 2 + 0.5f);
    float left = std::cos(arg);
    float right = std::sin(arg);
    buffer.applyGainRamp(0, 0, buffer.getNumSamples(), prevPanLeftLevel, left);
    buffer.applyGainRamp(1, 0, buffer.getNumSamples(), prevPanRightLevel, right);
    prevPanLeftLevel = left;
    prevPanRightLevel = right;
}

void KeyRepeatAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {
    ScopedNoDenormals noDenormals;

    for (int i = 0; i < buffer.getNumChannels(); i++) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }

    ProcessBlockInfo info;
    fillProcessBlockInfo(info, buffer);

    updateKeyboardState(midiMessages);

    keyswitchManager.setLatch(latchParameter.getValue());
    keyswitchManager.setSeparateTripletButton(!easyParameter.getValue());
    keyswitchManager.setKeyswitchOctave(keyswitchOctaveParameter.getValue());
    keyswitchManager.update();

    MidiBuffer newMidiMessages;
    if (keyswitchManager.isRepeatOff()) {
        // if repeat is off, just act like a normal midi sampler
        addAllNonKeyswitchMidiMessages(newMidiMessages, midiMessages);
    } else {
        // if repeat is on, the we must use our given midi messages
        // to generate the actual midi messages that will be played
        transformMidiMessages(newMidiMessages, info);
    }
    
    updateADSR();
    updatePitch(newMidiMessages);

    // fill the audio buffer with sounds using the new midi messages
    synth.renderNextBlock(buffer, newMidiMessages, 0, info.bufferNumSamples);
    
    updateLevel(buffer);
    updatePan(buffer);

    // increment the fake samples counter with modular arithmetic
    fakeSamplesIntoMeasure = std::fmod(fakeSamplesIntoMeasure + info.bufferNumSamples, info.samplesPerMeasure);
}

bool KeyRepeatAudioProcessor::hasEditor() const {
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* KeyRepeatAudioProcessor::createEditor() {
    return new KeyRepeatAudioProcessorEditor(*this, &myLookAndFeel);
}

void KeyRepeatAudioProcessor::getStateInformation (MemoryBlock& destData) {
    MemoryBlock audioBlock;
    MemoryBlock parametersBlock;
        
    // write audio data to temp block
    if (audioFormatReader) {
        WavAudioFormat wavAudioFormat;
        std::unique_ptr<AudioFormatWriter> audioFormatWriter(
            wavAudioFormat.createWriterFor(
                new MemoryOutputStream(audioBlock, false),
                audioFormatReader->sampleRate,
                audioFormatReader->numChannels,
                audioFormatReader->bitsPerSample,
                audioFormatReader->metadataValues,
                0
            )
        );
        // Apparently writing from the the AudioReader means errors but from the SampleBuffer is fine...
        // audioFormatWriter->writeFromAudioReader(*audioFormatReader, 0, -1);
        audioFormatWriter->writeFromAudioSampleBuffer(audioBuffer, 0, audioBuffer.getNumSamples());
        audioFormatWriter->flush();
    }
    size_t audioBlockSize = audioBlock.getSize();

    // write parameters data temp block
    auto state = parameters.copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, parametersBlock);
    size_t parametersBlockSize = parametersBlock.getSize();
    
    // append temp blocks to actual data block
    destData.append(&audioBlockSize, sizeof(size_t));
    destData.append(&parametersBlockSize, sizeof(size_t));
    destData.append(audioBlock.getData(), audioBlockSize);
    destData.append(parametersBlock.getData(), parametersBlockSize);
}

void KeyRepeatAudioProcessor::setStateInformation (const void *data, int sizeInBytes) {
    if (!JUCEApplication::isStandaloneApp()) {
        size_t audioBlockSize      = ((size_t *) data)[0];
        size_t parametersBlockSize = ((size_t *) data)[1];
        data = (size_t *) data + 2;

        // read audio data
        if (audioBlockSize > 0) {
            audioFormatReader.reset(audioFormatManager.createReaderFor(new MemoryInputStream(data, audioBlockSize, false)));
            changeSound();
        }
        data = (char *) data + audioBlockSize;
        
        // reader parameter data
        std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, (int) parametersBlockSize));
        if (xmlState.get() && xmlState->hasTagName(parameters.state.getType())) {
            parameters.replaceState(ValueTree::fromXml(*xmlState));
            linkParameterValues();
        }
    }
}

void KeyRepeatAudioProcessor::loadNewFile(String filePath) {
    audioFormatReader.reset(audioFormatManager.createReaderFor(filePath)); // reset audioFormatReader
    changeSound();
}

void KeyRepeatAudioProcessor::changeSound() {
    // reset audioBuffer
    AudioBuffer<float> tempBuf(audioFormatReader->numChannels, (int) audioFormatReader->lengthInSamples);
    audioFormatReader->read(&tempBuf, 0, (int) audioFormatReader->lengthInSamples, 0, true, true); // read from reader into tempBuf
    audioBuffer = tempBuf;
    
    // reset audioThumbnail
    audioThumbnail.reset(audioBuffer.getNumChannels(), getSampleRate(), audioBuffer.getNumSamples());
    audioThumbnail.addBlock(0, audioBuffer, 0, audioBuffer.getNumSamples());

    // add sound to synth
    if (audioFormatReader) {
        BigInteger allNotes;
        allNotes.setRange(0, 128, true);
        samplerSound = new SamplerSound(
            "samplerSound",     // name
            *audioFormatReader, // source
            allNotes,           // supported midi notes
            60,                 // midi note for normal pitch (middle C)
            0.00001,            // attack time in seconds
            0.0,                // release time in seconds
            MAX_SAMPLE_LENGTH_SEC
        );
        synth.addSound(samplerSound);
    }
}

AudioProcessorValueTreeState::ParameterLayout KeyRepeatAudioProcessor::createParameterLayout() {

    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    // Top right knobs
    params.push_back(std::make_unique<AudioParameterInt>("pitch", "Pitch", -12, 12, 0));
    params.push_back(std::make_unique<AudioParameterFloat>("pan", "Pan", NormalisableRange<float>(-1.0f, 1.0f), 0.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("level", "Level", NormalisableRange<float>(0.0f, 1.0f), 0.8f));

    // Middle ADSR envelope sliders (ADR logarithmically skewed)
    NormalisableRange<float> adrRange(0.0f, MAX_SAMPLE_LENGTH_SEC * 1000.0f);
    adrRange.setSkewForCentre(1000.0f);
    params.push_back(std::make_unique<AudioParameterFloat>("attack", "Attack", adrRange, 0.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("decay", "Decay", adrRange, 0.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("sustain", "Sustain", NormalisableRange<float>(0.0f, 1.0f), 1.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("release", "Release", adrRange, 200.0f));
    
    // Middle knobs
    params.push_back(std::make_unique<AudioParameterFloat>("swing", "Swing", NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    params.push_back(std::make_unique<AudioParameterFloat>("humanize", "Humanize", NormalisableRange<float>(0.0f, 1.0f), 0.0f));

    // Bottom controls
    params.push_back(std::make_unique<AudioParameterBool>("easy", "Easy", false));
    params.push_back(std::make_unique<AudioParameterBool>("latch", "Latch", false));
    params.push_back(std::make_unique<AudioParameterInt>("keyswitchOctave", "Keyswitch Octave", 0, 2, 0));
    
    return { params.begin(), params.end() };
}

void KeyRepeatAudioProcessor::linkParameterValues() {
    pitchParameter.referTo(parameters.getParameterAsValue("pitch"));
    panParameter.referTo(parameters.getParameterAsValue("pan"));
    levelParameter.referTo(parameters.getParameterAsValue("level"));
    attackParameter.referTo(parameters.getParameterAsValue("attack"));
    decayParameter.referTo(parameters.getParameterAsValue("decay"));
    sustainParameter.referTo(parameters.getParameterAsValue("sustain"));
    releaseParameter.referTo(parameters.getParameterAsValue("release"));
    swingParameter.referTo(parameters.getParameterAsValue("swing"));
    humanizeParameter.referTo(parameters.getParameterAsValue("humanize"));
    easyParameter.referTo(parameters.getParameterAsValue("easy"));
    latchParameter.referTo(parameters.getParameterAsValue("latch"));
    keyswitchOctaveParameter.referTo(parameters.getParameterAsValue("keyswitchOctave"));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new KeyRepeatAudioProcessor();
}
