/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AndrewHfinalreverbAudioProcessor::AndrewHfinalreverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(mFeedbackGainParam = new juce::AudioParameterFloat(
        "FeedbackGain",
        "Decay Time",
        0.01f,
        99.99f,
        80.0f
    ));
    addParameter(mMixParam = new juce::AudioParameterFloat(
        "Mix",
        "Wet/Dry",
        0.0f,
        100.0f,
        50.0f
    ));
    addParameter(mLateReflectionsParam = new juce::AudioParameterFloat("LPF Cutoff for late reflections",
        "Late Reflections",
        2000.0f,
        20000.0f,
        20000.0f
    ));
}

AndrewHfinalreverbAudioProcessor::~AndrewHfinalreverbAudioProcessor()
{
}

//==============================================================================
const juce::String AndrewHfinalreverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AndrewHfinalreverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AndrewHfinalreverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AndrewHfinalreverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AndrewHfinalreverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AndrewHfinalreverbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AndrewHfinalreverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AndrewHfinalreverbAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AndrewHfinalreverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void AndrewHfinalreverbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

int AndrewHfinalreverbAudioProcessor::calcMsecToSamps(float mSec)
{
    return mSec * 0.001 * mFs;
}

//==============================================================================
void AndrewHfinalreverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    mFs = sampleRate;
}

void AndrewHfinalreverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AndrewHfinalreverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
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

void AndrewHfinalreverbAudioProcessor::calcAlgorithmParams()
{
    mFeedbackGain = mFeedbackGainParam->get()/100;
    mLateReflections = mLateReflectionsParam->get();

    mWet = mMixParam->get()/100;
    mDry = 1 - mWet;

    float coeffsLPF[2];
    Mu45FilterCalc::calcCoeffs1PoleLPF(coeffsLPF, mLateReflections, mFs);
    //set coefficients for each filter
    mLpfL1.setCoefficients(coeffsLPF[0], coeffsLPF[1]);
    mLpfR1.setCoefficients(coeffsLPF[0], coeffsLPF[1]);
    mLpfL2.setCoefficients(coeffsLPF[0], coeffsLPF[1]);
    mLpfR2.setCoefficients(coeffsLPF[0], coeffsLPF[1]);
    mLpfL3.setCoefficients(coeffsLPF[0], coeffsLPF[1]);
    mLpfR3.setCoefficients(coeffsLPF[0], coeffsLPF[1]);
    mLpfL4.setCoefficients(coeffsLPF[0], coeffsLPF[1]);
    mLpfR4.setCoefficients(coeffsLPF[0], coeffsLPF[1]);

    mLpfL5.setCoefficients(coeffsLPF[0], coeffsLPF[1]);
    mLpfR5.setCoefficients(coeffsLPF[0], coeffsLPF[1]);
    mLpfL6.setCoefficients(coeffsLPF[0], coeffsLPF[1]);
    mLpfR6.setCoefficients(coeffsLPF[0], coeffsLPF[1]);

    mDelay1L.setDelay(mFs * .023);
    mDelay1R.setDelay(mFs * .023);
    mDelay2L.setDelay(mFs * .029);
    mDelay2R.setDelay(mFs * .029);
    mDelay3L.setDelay(mFs * .031);
    mDelay3R.setDelay(mFs * .031);
    mDelay4L.setDelay(mFs * .037);
    mDelay4R.setDelay(mFs * .037);
    mDelay5L.setDelay(mFs * .041);
    mDelay5R.setDelay(mFs * .041);
    mDelay6L.setDelay(mFs * .043);
    mDelay6R.setDelay(mFs * .043);

    mAllDel1L.setDelay(mFs * .1);
    mAllDel1R.setDelay(mFs * .1);
    mAllDel2L.setDelay(mFs * .033);
    mAllDel2R.setDelay(mFs * .033);
    mAllDel3L.setDelay(mFs * .011);
    mAllDel3R.setDelay(mFs * .011);
    mAllDel4L.setDelay(mFs * .0037);
    mAllDel4R.setDelay(mFs * .0037);
    mAllDel5L.setDelay(mFs * .001234567);
    mAllDel5R.setDelay(mFs * .001234567);
}

void AndrewHfinalreverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    float outL, outR, temp1L, temp1R, temp2L, temp2R, temp3L, temp3R, temp4L, temp4R, temp5L, temp5R, temp6L, temp6R;
    float outAll1L, outAll1R, outAll2L, outAll2R, outAll3L, outAll3R, outAll4L, outAll4R;
    float outFullL, outFullR;
    float tempAll1L, tempAll1R, tempAll2L, tempAll2R, tempAll3L, tempAll3R, tempAll4L, tempAll4R, tempAll5L, tempAll5R;

    calcAlgorithmParams();

    auto* channelDataLeft = buffer.getWritePointer(0);
    auto* channelDataRight = buffer.getWritePointer(1);
    for (int samp = 0; samp < buffer.getNumSamples(); samp++)
    {
        //fix back to mono in
        //outL = channelDataLeft[samp];
        //outR = channelDataRight[samp];

        temp1L = mDelay1L.nextOut();
        temp1R = mDelay1R.nextOut();
        temp2L = mDelay2L.nextOut();
        temp2R = mDelay2R.nextOut();
        temp3L = mDelay3L.nextOut();
        temp3R = mDelay3R.nextOut();
        temp4L = mDelay4L.nextOut();
        temp4R = mDelay4R.nextOut();

        temp5L = mDelay5L.nextOut();
        temp5R = mDelay5R.nextOut();
        temp6L = mDelay6L.nextOut();
        temp6R = mDelay6R.nextOut();

        temp1L = mLpfL1.tick(temp1L);
        temp1R = mLpfR1.tick(temp1R);
        temp2L = mLpfL2.tick(temp2L);
        temp2R = mLpfR2.tick(temp2R);
        temp3L = mLpfL3.tick(temp3L);
        temp3R = mLpfR3.tick(temp3R);
        temp4L = mLpfL4.tick(temp4L);
        temp4R = mLpfR4.tick(temp4R);

        temp5L = mLpfL5.tick(temp5L);
        temp5R = mLpfR5.tick(temp5R);
        temp6L = mLpfL6.tick(temp6L);
        temp6R = mLpfR6.tick(temp6R);

        temp1L *= mFeedbackGain;
        temp1R *= mFeedbackGain;
        temp2L *= mFeedbackGain;
        temp2R *= mFeedbackGain;
        temp3L *= mFeedbackGain;
        temp3R *= mFeedbackGain;
        temp4L *= mFeedbackGain;
        temp4R *= mFeedbackGain;

        temp5L *= mFeedbackGain;
        temp5R *= mFeedbackGain;
        temp6L *= mFeedbackGain;
        temp6R *= mFeedbackGain;

        mDelay1L.tick(channelDataLeft[samp] - temp1L);
        mDelay1R.tick(channelDataRight[samp] - temp1R);
        mDelay2L.tick(channelDataLeft[samp] - temp2L);
        mDelay2R.tick(channelDataRight[samp] - temp2R);
        mDelay3L.tick(channelDataLeft[samp] - temp3L);
        mDelay3R.tick(channelDataRight[samp] - temp3R);
        mDelay4L.tick(channelDataLeft[samp] - temp4L);
        mDelay4R.tick(channelDataRight[samp] - temp4R);

        mDelay5L.tick(channelDataLeft[samp] - temp5L);
        mDelay5R.tick(channelDataRight[samp] - temp5R);
        mDelay6L.tick(channelDataLeft[samp] - temp6L);
        mDelay6R.tick(channelDataRight[samp] - temp6R);

        outL = (temp1L + temp2L + temp3L + temp4L + temp5L + temp6L);
        outR = (temp1R + temp2R + temp3R + temp4R + temp5R + temp6R);

        ////////////////////////////////////////////////////////////////////////////////////////////

        //feedforward
        outAll1L = outL * -0.7;
        outAll1R = outR * -0.7;

        tempAll1L = mAllDel1L.nextOut();
        tempAll1R = mAllDel1R.nextOut();

        tempAll1L *= 0.7;
        tempAll1R *= 0.7;

        mAllDel1L.tick(outL + tempAll1L);
        mAllDel1R.tick(outR + tempAll1R);

        //1-g^2
        tempAll1L *= 0.51;
        tempAll1R *= 0.51;

        outAll1L += tempAll1L;
        outAll1R += tempAll1R;

        ////////////////////////////////////////////////////////////////////////////////////////////

        //feedforward
        outAll2L = outAll1L * -0.7;
        outAll2R = outAll1R * -0.7;

        tempAll2L = mAllDel2L.nextOut();
        tempAll2R = mAllDel2R.nextOut();

        tempAll2L *= 0.7;
        tempAll2R *= 0.7;

        mAllDel2L.tick(outAll1L + tempAll2L);
        mAllDel2R.tick(outAll1R + tempAll2R);

        //1-g^2
        tempAll2L *= 0.51;
        tempAll2R *= 0.51;

        outAll2L += tempAll2L;
        outAll2R += tempAll2R;

        ////////////////////////////////////////////////////////////////////////////////////////////

        //feedforward
        outAll3L = outAll2L * -0.7;
        outAll3R = outAll2R * -0.7;

        tempAll3L = mAllDel3L.nextOut();
        tempAll3R = mAllDel3R.nextOut();

        tempAll3L *= 0.7;
        tempAll3R *= 0.7;

        mAllDel3L.tick(outAll2L + tempAll3L);
        mAllDel3R.tick(outAll2R + tempAll3R);

        //1-g^2
        tempAll3L *= 0.51;
        tempAll3R *= 0.51;

        outAll3L += tempAll3L;
        outAll3R += tempAll3R;

        ////////////////////////////////////////////////////////////////////////////////////////////

        //feedforward
        outAll4L = outAll3L * -0.7;
        outAll4R = outAll3R * -0.7;

        tempAll4L = mAllDel4L.nextOut();
        tempAll4R = mAllDel4R.nextOut();

        tempAll4L *= 0.7;
        tempAll4R *= 0.7;

        mAllDel4L.tick(outAll3L + tempAll4L);
        mAllDel4R.tick(outAll3R + tempAll4R);

        //1-g^2
        tempAll4L *= 0.51;
        tempAll4R *= 0.51;

        outAll4L += tempAll4L;
        outAll4R += tempAll4R;


        //////////////////////////////////////////////////////////////////////////////

        //feedforward
        outFullL = outAll4L * -0.7;
        outFullR = outAll4R * -0.7;

        tempAll5L = mAllDel5L.nextOut();
        tempAll5R = mAllDel5R.nextOut();

        tempAll5L *= 0.7;
        tempAll5R *= 0.7;

        mAllDel5L.tick(outAll4L + tempAll5L);
        mAllDel5R.tick(outAll4L + tempAll5R);

        //1-g^2
        tempAll5L *= 0.51;
        tempAll5R *= 0.51;

        outFullL += tempAll5L;
        outFullL += tempAll5L;

        //////////////////////////////////////////////////////////////

        channelDataLeft[samp] = (channelDataLeft[samp] * mDry) + (outFullL * mWet);
        channelDataRight[samp] = (channelDataRight[samp] * mDry) + (outFullR * mWet);
    }
}

//==============================================================================
bool AndrewHfinalreverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AndrewHfinalreverbAudioProcessor::createEditor()
{
    return new AndrewHfinalreverbAudioProcessorEditor (*this);
}

//==============================================================================
void AndrewHfinalreverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AndrewHfinalreverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AndrewHfinalreverbAudioProcessor();
}
