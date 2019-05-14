/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Glasgowkeene_a2AudioProcessor::Glasgowkeene_a2AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), parameters(*this, nullptr, Identifier("RingMod"),  //change "RingMod" to name of your DAW
                                    {
                                        //add list of parameters that you think your plugin will have
                                        std::make_unique<AudioParameterFloat>("mix", //Parameter ID
                                                                              "Mix", //Parameter Name
                                                                              0.0f, //minimum value
                                                                              1.0f, //max value
                                                                              0.5f //default value
                                                                              )
                                       //std::make_unique<AudioParameterFloat>("next parameter (?)))", //Parameter ID
                                                                         
                                    }
                        )
#endif
{
    // store pointer to our parameter
    mixParameter = parameters.getRawParameterValue("mix");
    
    currentSampleRate = 0.0f;
    currentAngle = 0.0f;
    angleDelta = 0.0f;
    sinFreq = 0.0f;
}

Glasgowkeene_a2AudioProcessor::~Glasgowkeene_a2AudioProcessor()
{
}

//==============================================================================
const String Glasgowkeene_a2AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Glasgowkeene_a2AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Glasgowkeene_a2AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Glasgowkeene_a2AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Glasgowkeene_a2AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Glasgowkeene_a2AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Glasgowkeene_a2AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Glasgowkeene_a2AudioProcessor::setCurrentProgram (int index)
{
}

const String Glasgowkeene_a2AudioProcessor::getProgramName (int index)
{
    return {};
}

void Glasgowkeene_a2AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Glasgowkeene_a2AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    currentSampleRate = sampleRate;
    sinFreq = 100.0f;
    updateAngleDelta();
    mixLevel = 0.15f;
    
    gain.setGainDecibels(12.0f);
    
    String message;
    message << "Preparing to play..." << newLine;
    message << "My sample rate is " << currentSampleRate << newLine;
    message << "Buffer size is " << samplesPerBlock << newLine;
    Logger::getCurrentLogger()->writeToLog(message);
}

void Glasgowkeene_a2AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Glasgowkeene_a2AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Glasgowkeene_a2AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    //set up another "basket" (copy of input buffer) for the wet signal , so we can keep our original
    AudioBuffer<float> wetBuffer(totalNumInputChannels, buffer.getNumSamples());
    wetBuffer.makeCopyOf(buffer);
    
    //for loop for going through each channel of audio that is available (e.g. left and right)
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        //we are getting an audio block of size 512
        auto* channelData = buffer.getWritePointer (channel);
        auto* wetData = wetBuffer.getWritePointer (channel);
        
        // ..do something to each audio sample
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
           //generating a set of random values to modulate input amplitude
            float modulator = random.nextFloat() * 0.25f - 0.125f;
            wetData[sample] = wetData[sample] * modulator;
            
            //calculate value to put into buffer (FOR SINE WAVE GENERATOR G:)
            auto currentSinSample = (float) std::sin(currentAngle);
            currentAngle += angleDelta; //IS THE SAME AS: currentAngle = currentAngle + angleDelta
            
            //let's make our sine wave wiggle~~~
            modulator = random.nextFloat() * 0.1f - 0.05f;
            wetData[sample] = wetData[sample] * currentSinSample + modulator;
            
            //soft clip with hypbolic tangent
            auto shapedSample = (float) std::tanh(wetData[sample]);
            wetData[sample] = shapedSample;
            
            auto automatableMixLevel = *mixParameter;
            
            //add original dry signal with processed wet signal into our output buffer (aka input buffer, cause they're the same "basket")
            //*0.5 or whatever variable --> the ratio for wet/dry
            //4TH APRIL EDIT: added mixLevel. the 0.25f in (0.25f - mixLevel) (or whatever value) is the maximum level you can go
            channelData[sample] = channelData[sample] * (1.0f - automatableMixLevel) +
                wetData[sample] * automatableMixLevel;
            
            //channelData[sample] = random.nextFloat() * 0.25f - 0.125f; //scale first then offset (dunno what this means tho tbh)
        }
    }
    dsp::AudioBlock<float> output(buffer);
    gain.process(dsp::ProcessContextReplacing<float> (output));
}

//==============================================================================
bool Glasgowkeene_a2AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Glasgowkeene_a2AudioProcessor::createEditor()
{
    return new Glasgowkeene_a2AudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Glasgowkeene_a2AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = parameters.copyState(); //saves valueTree in to "state"
    std::unique_ptr<XmlElement> xml(state.createXml()); //creates an XML version of "state"
    copyXmlToBinary(*xml, destData);
}

void Glasgowkeene_a2AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
    
    if(xmlState.get() != nullptr)
    {
        if (xmlState->hasTagName(parameters.state.getType()))
        {
            parameters.replaceState(ValueTree::fromXml(*xmlState));
        }
    }
}

//==============================================================================
//User functions

void Glasgowkeene_a2AudioProcessor::updateAngleDelta()
{
    ////calculate no. of cycles that we will need to complete for each output sample.
    auto cyclesPerSample = sinFreq / currentSampleRate;
    //mutiply by the length of a whole sine wave cycle
    angleDelta = cyclesPerSample; //* MathConstants<float>::twoPi;
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Glasgowkeene_a2AudioProcessor();
}
