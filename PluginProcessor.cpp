/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MSUtilityAudioProcessor::MSUtilityAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
    //Adds Parameters to the AudioProcessor
    stereoWidth = new juce::AudioParameterFloat("StereoWidth", "Stereo Width", 0.0f, 2.0f, 1.0f);
    addParameter(stereoWidth);
    inputSelection = new juce::AudioParameterChoice("inputSelection", "Input Selection", { "Stereo", "MidSide" }, 1);
    addParameter(inputSelection);
    outputSelection = new juce::AudioParameterChoice("outputSelection", "Output Selection", { "Stereo", "MidSide" }, 1);
    addParameter(outputSelection);
}

MSUtilityAudioProcessor::~MSUtilityAudioProcessor()
{
}

//==============================================================================
const juce::String MSUtilityAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MSUtilityAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool MSUtilityAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool MSUtilityAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double MSUtilityAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MSUtilityAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MSUtilityAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MSUtilityAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String MSUtilityAudioProcessor::getProgramName(int index)
{
    return {};
}

void MSUtilityAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void MSUtilityAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MSUtilityAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MSUtilityAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void MSUtilityAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

    //Storing the number of samples for each channel
    auto* channelDataL = buffer.getWritePointer(0);
    auto* channelDataR = buffer.getWritePointer(1);

    //Loop runs from 0 to number of samples in the block 
    for (int i = 0;i < buffer.getNumSamples(); ++i)

    {
        float StereoWidth = stereoWidth->get();
        int InputSelection = inputSelection->getIndex();
        int OutputSelection = outputSelection->getIndex();

        float mid;
        float side;
        {
            if (inputSelection == 0 && outputSelection == 0) // ***stereo in and stereo out 
            {
                //Stereo To Midside Encoding
                side = 0.5f * (channelDataL[i] - channelDataR[i]);
                mid = 0.5f * (channelDataL[i] + channelDataR[i]);

                //Stereo Width Control
                auto side = StereoWidth * (channelDataL[i] - channelDataR[i]);
                auto mid = (2.0 - StereoWidth) * (channelDataL[i] + channelDataR[i]);

                //Midside to Stereo-Decoding 
                auto channelDataL = (mid + side);
                auto channelDataR = (mid - side);

            }
            else
                side = 0.5f * (channelDataL[i] - channelDataR[i]);
            mid = 0.5f * (channelDataL[i] + channelDataR[i]);

            //Stereo Width Control
            auto side = StereoWidth * (channelDataL[i] - channelDataR[i]);
            auto mid = (2.0 - StereoWidth) * (channelDataL[i] + channelDataR[i]);
            {

            }



        }

    }








}




















//==============================================================================
bool MSUtilityAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MSUtilityAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void MSUtilityAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    float SWidth;
    int InputSelection;
    int OutputSelection;
    float mid;
    float side;





    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MSUtilityAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    float SWidth;
    int InputSelection;
    int OutputSelection;
    float mid;
    float side;
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MSUtilityAudioProcessor();
}
