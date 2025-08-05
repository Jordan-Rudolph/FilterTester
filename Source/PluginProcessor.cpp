/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterTesterAudioProcessor::FilterTesterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    apvts(*this, nullptr, juce::Identifier("FilterTester"), juce::AudioProcessorValueTreeState::ParameterLayout{
			std::make_unique<juce::AudioParameterFloat>("cutoff", "Cutoff Frequency", 
                juce::NormalisableRange<float>{20.f, 20000.f, 0.1f, 0.3f, false}, 1000.f),
            std::make_unique<juce::AudioParameterFloat>("resonance", "Resonance (Q)", 
                juce::NormalisableRange<float>{0.1f, 18.f, 0.001f, 1.f, false}, 0.707f),

            std::make_unique<juce::AudioParameterChoice>("filterForm", "Filter Form",
				juce::StringArray{"Direct Form I", "Direct Form II"}, 0)
        })
{
    cutoffParam = apvts.getRawParameterValue("cutoff");
	resonanceParam = apvts.getRawParameterValue("resonance");
	filterFormParam = apvts.getRawParameterValue("filterForm");
}

FilterTesterAudioProcessor::~FilterTesterAudioProcessor()
{
}

//==============================================================================
const juce::String FilterTesterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FilterTesterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FilterTesterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FilterTesterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FilterTesterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FilterTesterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FilterTesterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FilterTesterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FilterTesterAudioProcessor::getProgramName (int index)
{
    return {};
}

void FilterTesterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FilterTesterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    jlpFilter.prepare(static_cast<float>(sampleRate));
}

void FilterTesterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FilterTesterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void FilterTesterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    
    //Load parameters from APVTS
    const float cutoff = cutoffParam->load();
	const float resonance = resonanceParam->load();
    const int filterForm = static_cast<int>(filterFormParam->load());

    //Set parameters
	jlpFilter.setCutoffFrequency(cutoff);
	jlpFilter.setResonance(resonance);
	jlpFilter.setFilterType(filterForm);

    //Main processing
	jlpFilter.processBuffer(buffer);
}

//==============================================================================
bool FilterTesterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FilterTesterAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void FilterTesterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FilterTesterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FilterTesterAudioProcessor();
}
