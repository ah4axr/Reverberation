/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace juce;
//==============================================================================
AndrewHfinalreverbAudioProcessorEditor::AndrewHfinalreverbAudioProcessorEditor (AndrewHfinalreverbAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 250);

    auto& params = processor.getParameters();

    ///////////////////////////////////////////////////////////////////////////////////////
    AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(0);
    mFeedbackGainSlider.setBounds(20, 100, 100, 100);
    mFeedbackGainSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mFeedbackGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    mFeedbackGainSlider.setRange(audioParam->range.start, audioParam->range.end);
    mFeedbackGainSlider.setValue(*audioParam);
    //mFeedbackGainSlider.setTextValueSuffix(" dB");
    mFeedbackGainSlider.addListener(this);
    addAndMakeVisible(mFeedbackGainSlider);
    addAndMakeVisible(mFeedbackGainLabel);
    mFeedbackGainLabel.setText("Decay Time", juce::dontSendNotification);
    mFeedbackGainLabel.setJustificationType(juce::Justification::centred);
    mFeedbackGainLabel.attachToComponent(&mFeedbackGainSlider, false);

    audioParam = (AudioParameterFloat*)params.getUnchecked(1);
    mMixSlider.setBounds(260, 100, 100, 100);
    mMixSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mMixSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    mMixSlider.setRange(audioParam->range.start, audioParam->range.end);
    mMixSlider.setValue(*audioParam);
    //mFeedbackGainSlider.setTextValueSuffix(" dB");
    mMixSlider.addListener(this);
    addAndMakeVisible(mMixSlider);
    addAndMakeVisible(mMixLabel);
    mMixLabel.setText("Mix", juce::dontSendNotification);
    mMixLabel.setJustificationType(juce::Justification::centred);
    mMixLabel.attachToComponent(&mMixSlider, false);

    audioParam = (AudioParameterFloat*)params.getUnchecked(2);
    mLateReflectionsSlider.setBounds(140, 100, 100, 100);
    mLateReflectionsSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    mLateReflectionsSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 70, 20);
    mLateReflectionsSlider.setRange(audioParam->range.start, audioParam->range.end);
    mLateReflectionsSlider.setValue(*audioParam);
    //mLateReflectionsSlider.setTextValueSuffix(" dB");
    mLateReflectionsSlider.addListener(this);
    addAndMakeVisible(mLateReflectionsSlider);
    addAndMakeVisible(mLateReflectionsLabel);
    mLateReflectionsLabel.setText("Late Reflections", juce::dontSendNotification);
    mLateReflectionsLabel.setJustificationType(juce::Justification::centred);
    mLateReflectionsLabel.attachToComponent(&mLateReflectionsSlider, false);
}

AndrewHfinalreverbAudioProcessorEditor::~AndrewHfinalreverbAudioProcessorEditor()
{
}

//==============================================================================
void AndrewHfinalreverbAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Reverb", getLocalBounds(), juce::Justification::centredTop, 1);
}

void AndrewHfinalreverbAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void AndrewHfinalreverbAudioProcessorEditor::sliderValueChanged(Slider* slider)
{

    auto& params = processor.getParameters();
    if (slider == &mFeedbackGainSlider)
    {
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(0);
        *audioParam = mFeedbackGainSlider.getValue();
    }
    else if (slider == &mMixSlider)
    {
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(1);
        *audioParam = mMixSlider.getValue();
    }
    else if (slider == &mLateReflectionsSlider)
    {
        AudioParameterFloat* audioParam = (AudioParameterFloat*)params.getUnchecked(2);
        *audioParam = mLateReflectionsSlider.getValue();
    }
}

void AndrewHfinalreverbAudioProcessorEditor::timerCallback()
{
    auto& params = processor.getParameters();
    //Update the value of each slider to match the value in the Processor    
    AudioParameterFloat* feedback = (AudioParameterFloat*)params.getUnchecked(0);
    mFeedbackGainSlider.setValue(feedback->get(), dontSendNotification);
    AudioParameterFloat* mix = (AudioParameterFloat*)params.getUnchecked(1);
    mMixSlider.setValue(mix->get(), dontSendNotification);
    AudioParameterFloat* lateReflections = (AudioParameterFloat*)params.getUnchecked(2);
    mLateReflectionsSlider.setValue(lateReflections->get(), dontSendNotification);
}