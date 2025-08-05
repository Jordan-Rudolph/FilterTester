/*
  ==============================================================================

    FilterTester.cpp
    Created: 15 Jul 2025 1:27:20pm
    Author:  jorda

  ==============================================================================
*/

#include "FilterTester.h"
#define J_2PI juce::MathConstants<float>::twoPi

void JLPFilter::processBuffer(juce::AudioBuffer<float>& buffer)
{
    switch (formType) {
    case DirectFormI:
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                channelData[sample] = processSampleDFI(channelData[sample], channel);
            }
        }
        break;
    case DirectFormII:
		//TODO: Implement Direct Form II processing
        break;
    }
}

float JLPFilter::processSampleDFI(float sample, int channel) {
	//Calculate filter coefficients
    const float omega = J_2PI * cutoffFrequency / sampleRate;
	const float sin_omega = sinf(omega);
	const float cos_omega = cosf(omega);
    const float alpha = sin_omega / (2.f * resonance);

    const float b_0 = (1.f - cos_omega)/2.f;
    const float b_1 = 1.f - cos_omega;
	const float b_2 = (1.f - cos_omega) / 2.f;

    const float a_0 = 1.f + alpha;
    const float a_1 = -2.f * cos_omega;
	const float a_2 = 1.f - alpha;

	//Add the new sample to the input buffer
	inputBuffer[channel].push(sample);

	//Calculate the output using Direct Form I 
    float output = 
        (inputBuffer[channel][0] * b_0)
        + (inputBuffer[channel][1] * b_1)
        + (inputBuffer[channel][2] * b_2)
        - (outputBuffer[channel][0] * a_1)
        - (outputBuffer[channel][1] * a_2);

	output /= a_0;

	//Push output to the output buffer and return
	outputBuffer[channel].push(output);
    return hardClip(output);
}

float JLPFilter::processSampleDFII(float sample, int channel) {
    //Calculate filter coefficients
    const float omega = J_2PI * cutoffFrequency / sampleRate;
    const float sin_omega = sinf(omega);
    const float cos_omega = cosf(omega);
    const float alpha = sin_omega / (2.f * resonance);

    const float b_0 = (1.f - cos_omega) / 2.f;
    const float b_1 = 1.f - cos_omega;
    const float b_2 = (1.f - cos_omega) / 2.f;

    const float a_0 = 1.f + alpha;
    const float a_1 = -2.f * cos_omega;
    const float a_2 = 1.f - alpha;

    //Add the new sample to the input buffer
    dfiiBuffer[channel].push(sample);

    //Calculate the output using Direct Form I 
    float output =
        (inputBuffer[channel][0] * b_0)
        + (inputBuffer[channel][1] * b_1)
        + (inputBuffer[channel][2] * b_2)
        - (outputBuffer[channel][0] * a_1)
        - (outputBuffer[channel][1] * a_2);

    output /= a_0;

    //Push output to the output buffer and return
    return hardClip(output);
}