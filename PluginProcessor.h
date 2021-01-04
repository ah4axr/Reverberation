/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StkLite-4.6.1/Delay.h"
#include "StkLite-4.6.1/DelayA.h"
#include "Mu45FilterCalc/Mu45FilterCalc.h"
#include "StkLite-4.6.1/BiQuad.h"
#include "StkLite-4.6.1/OnePole.h"
#include "StkLite-4.6.1/PoleZero.h"

#define MAXDELAYMSEC 1000.0
//==============================================================================
/**
*/
class AndrewHfinalreverbAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    AndrewHfinalreverbAudioProcessor();
    ~AndrewHfinalreverbAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    void calcAlgorithmParams();
    // milliseconds to seconds
    int calcMsecToSamps(float mSec);
    juce::AudioParameterFloat* mFeedbackGainParam;
    juce::AudioParameterFloat* mMixParam;
    juce::AudioParameterFloat* mEarlyParam;
    juce::AudioParameterFloat* mLateReflectionsParam;

    stk::Delay mDelay1L;
    stk::Delay mDelay1R;
    stk::Delay mDelay2L;
    stk::Delay mDelay2R;
    stk::Delay mDelay3L;
    stk::Delay mDelay3R;
    stk::Delay mDelay4L;
    stk::Delay mDelay4R;

    stk::Delay mDelay5L;
    stk::Delay mDelay5R;
    stk::Delay mDelay6L;
    stk::Delay mDelay6R;

    stk::Delay mAllDel1L;
    stk::Delay mAllDel1R;
    stk::Delay mAllDel2L;
    stk::Delay mAllDel2R;
    stk::Delay mAllDel3L;
    stk::Delay mAllDel3R;
    stk::Delay mAllDel4L;
    stk::Delay mAllDel4R;
    stk::Delay mAllDel5L;
    stk::Delay mAllDel5R;

    stk::OnePole mLpfL1;
    stk::OnePole mLpfR1;
    stk::OnePole mLpfL2;
    stk::OnePole mLpfR2;
    stk::OnePole mLpfL3;
    stk::OnePole mLpfR3;
    stk::OnePole mLpfL4;
    stk::OnePole mLpfR4;

    stk::OnePole mLpfL5;
    stk::OnePole mLpfR5;
    stk::OnePole mLpfL6;
    stk::OnePole mLpfR6;

    stk::BiQuad mApfL1;
    stk::BiQuad mApfR1;

    float mFeedbackGain;

    float mEarly;
    float mLateReflections;

    float mWet;
    float mDry;

    float mFs;       // Sampling rate
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AndrewHfinalreverbAudioProcessor)
};
