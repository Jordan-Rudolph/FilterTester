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
#define J_2PI juce::MathConstants<float>::twoPi

class JLPFilter {
    public:
    JLPFilter() = default;
    ~JLPFilter() = default;
    void prepare(float s) {
        sampleRate = s;
        recalculateCoeffecients();
    }
    void processBuffer(juce::AudioBuffer<float>& buffer);
	float processSampleDFI(float sample, int channel);
    float processSampleDFII(float sample, int channel);

	//Setters with recalculation of coefficients
    void setCutoffFrequency(float f) {
        if (f != cutoffFrequency) {
            cutoffFrequency = f;
            recalculateCoeffecients();
        }
    }
    void setResonance(float r) {
        if (r != resonance) {
            resonance = r;
			recalculateCoeffecients();
        }
	}
    void setFilterType(int type) {
        formType = type;
    }

    enum FormType {
        DirectFormI = 0,
        DirectFormII = 1
	};

private:

    void recalculateCoeffecients() {
        const float omega = J_2PI * cutoffFrequency / sampleRate;
        const float sin_omega = sinf(omega);
        const float cos_omega = cosf(omega);
        const float alpha = sin_omega / (2.f * resonance);

        b_0 = (1.f - cos_omega) / 2.f;
        b_1 = 1.f - cos_omega;
        b_2 = (1.f - cos_omega) / 2.f;

        a_0 = 1.f + alpha;
        a_1 = -2.f * cos_omega;
        a_2 = 1.f - alpha;
	}

    //Filter coefficients
	float a_0 = 1.0f, a_1 = 0.0f, a_2 = 0.0f; 
	float b_0 = 0.0f, b_1 = 0.0f, b_2 = 0.0f;

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