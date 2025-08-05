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
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            auto* channelData = buffer.getWritePointer(channel);
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                channelData[sample] = processSampleDFII(channelData[sample], channel);
            }
        }
        break;
    }
}

float JLPFilter::processSampleDFI(float sample, int channel) {

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
    

    //Add the new sample to the input buffer

    //Calculate the output using Direct Form II 
    float output = //AKA v(n)
        sample
        - (dfiiBuffer[channel][0] * a_1) //Here, index 0 refers to the previous v(n)
        - (dfiiBuffer[channel][1] * a_2);

    output /= a_0;

    float temp = (dfiiBuffer[channel][0] * b_1)
        + (dfiiBuffer[channel][1] * b_2); //This is the second half of the filter graph

    dfiiBuffer[channel].push(output);

    output *= b_0;

    output += temp;

    //Return
    return hardClip(output);
}

