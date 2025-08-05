/*
  ==============================================================================

    FilterTester.h
    Created: 15 Jul 2025 1:27:20pm
    Author:  jorda

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SimpleCircularBuffer.h"


class JLPFilter {
    public:
    JLPFilter() = default;
    ~JLPFilter() = default;
    void prepare(float s) {
        sampleRate = s;
    }
    void processBuffer(juce::AudioBuffer<float>& buffer);
	float processSampleDFI(float sample, int channel);
    float processSampleDFII(float sample, int channel);
    void setCutoffFrequency(float f) {
		cutoffFrequency = f;
    }
    void setResonance(float r) {
		resonance = r;
	}
    void setFilterType(int type) {
        formType = type;
    }

    enum FormType {
        DirectFormI = 0,
        DirectFormII = 1
	};

private:

    float hardClip(float sample) {
        return juce::jlimit(-1.0f, 1.0f, sample);
	}

	std::array<SimpleCircularBuffer<3>, 2> inputBuffer; // Buffer for input samples
    std::array<SimpleCircularBuffer<2>, 2> outputBuffer; // Buffer for output samples

	std::array<SimpleCircularBuffer<2>, 2> dfiiBuffer; // Buffer for Direct Form II processing

    int formType = FormType::DirectFormI; // Default filter type
	float cutoffFrequency = 1000.0f; // Default cutoff frequency
	float resonance = 0.71f; // Default resonance (Q)
	float sampleRate = 44100.0f; // Default sample rate
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JLPFilter)
};