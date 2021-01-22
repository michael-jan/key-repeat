/*
  ==============================================================================

    ProcessBufferInfo.h
    Created: 28 May 2019 2:26:44am
    Author:  Michael Jan

  ==============================================================================
*/

#pragma once

/*
    Meant to store info that is updated every time processBlock() is called.
*/
struct ProcessBlockInfo {
    double samplesPerSecond;
    double samplesPerBeat;
    double samplesPerMeasure;
    double beatsIntoMeasure;
    double samplesIntoMeasure;
    double nextBeatsIntoMeasure;
    int bufferNumSamples;
};