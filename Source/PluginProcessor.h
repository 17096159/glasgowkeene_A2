/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class Glasgowkeene_a2AudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Glasgowkeene_a2AudioProcessor();
    ~Glasgowkeene_a2AudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //AudioBuffer<float> returnAudioBuffer;
    //^^^in jon's code, can ignore for now apparently
    
    dsp::Gain<float> gain;
    float mixLevel;
    
    //FOR SMOOTHING THE VALUE SO YOU DON'T GET POPS WHEN YOU RAPIDLY SWING THE SLIDER
    //DUNNO HOW THO
    //SmoothedValue<float> mixLevel;


private:
    float currentSampleRate;
    float currentAngle;
    float angleDelta; //how much we need to increment
    float sinFreq;
    
    //dsp::Gain<float> gain;
    
    
    void updateAngleDelta();
    Random random;
    
    //double systemSampleRate;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Glasgowkeene_a2AudioProcessor)
};
