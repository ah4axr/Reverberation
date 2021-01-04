/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AndrewHfinalreverbAudioProcessorEditor  : public juce::AudioProcessorEditor,
    public juce::Slider::Listener, public juce::Timer
{
public:
    AndrewHfinalreverbAudioProcessorEditor (AndrewHfinalreverbAudioProcessor&);
    ~AndrewHfinalreverbAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AndrewHfinalreverbAudioProcessor& audioProcessor;
    //Sliders
    juce::Slider mFeedbackGainSlider;
    juce::Slider mLateReflectionsSlider;

    juce::Slider mMixSlider;

    juce::Label mFeedbackGainLabel;
    juce::Label mLateReflectionsLabel;

    juce::Label mMixLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AndrewHfinalreverbAudioProcessorEditor)
};
