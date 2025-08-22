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
    // This class implements a simple low-pass filter using Direct Form I and II structures.
	// It uses circular buffers to store input and output samples for efficient processing.
public:
	// Constructor and destructor
    JLPFilter() = default;
    ~JLPFilter() = default;

	//Prepare the filter with a sample rate, should be called during the prepareToPlay phase if using JUCE framework
    void prepare(float s) {
        sampleRate = s;
        recalculateCoeffecients();
    }

	//Main processing functions, implemented for both Direct Form I and II
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

	//Filter type setter, doesn't require recalculation of coefficients
    void setFilterType(int type) {
        formType = type;
    }

	// Enumeration for filter form types
    enum FormType {
        DirectFormI = 0,
        DirectFormII = 1
	};

private:

	// Recalculate filter coefficients based on the current cutoff frequency and resonance
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

	// Hard clipping function to limit output samples to the range [-1.0, 1.0]
	// This is used to prevent clipping artifacts in the output signal during testing, but may not be necessary in production code.
    float hardClip(float sample) {
        return juce::jlimit(-1.0f, 1.0f, sample);
	}

    // Circular buffers for input and output samples, used in filter calculations
	std::array<SimpleCircularBuffer<3>, 2> inputBuffer; 
    std::array<SimpleCircularBuffer<2>, 2> outputBuffer; 

    // Buffer for Direct Form II processing
	std::array<SimpleCircularBuffer<2>, 2> dfiiBuffer; 

    // Default filter type
    int formType = FormType::DirectFormI; 

    // Default cutoff frequency
	float cutoffFrequency = 1000.0f; 

    // Default resonance (Q)
	float resonance = 0.71f; 

    // Default sample rate
	float sampleRate = 44100.0f; 


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JLPFilter)
};