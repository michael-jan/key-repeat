/*
  ==============================================================================

    SamplerSynth.cpp
    Created: 27 May 2019 8:33:00am
    Author:  Michael Jan

  ==============================================================================
*/

#include "SamplerSynth.h"
#define MAX_VOICES 16

void SamplerSynth::setup() {

	for (int i = 0; i < MAX_VOICES; i++) {
		addVoice(new SamplerVoice());
	}

}