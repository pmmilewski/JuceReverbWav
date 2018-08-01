/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayBlock.h"
#include "CombReverbParallel.h"
#include "AllpassReverbSeries.h"
#include "ProcessingPipeline.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent,
                        public ChangeListener,
                        public Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int, double) override;
    void getNextAudioBlock (const AudioSourceChannelInfo&) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

    void changeListenerCallback (ChangeBroadcaster*) override;

    void timerCallback() override;
    void updateLoopState (bool);

private:
     enum class TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };

    enum class ImpulseTestState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };

    enum Mode
    {
        WAV,
        ImpulseTest,
        Other
    };


    void changeState (TransportState);
    void changeState (ImpulseTestState);
    void openButtonClicked();
    void impulseButtonClicked();
    void playImpulseButtonClicked();

    void playButtonClicked();
    void stopButtonClicked();
    void loopButtonChanged();

    void enableReverbControls();
    void disableReverbControls();

    float getSampleRate();

    void addCombButtonClicked();
    void addAllpassButtonClicked();
    void addDelayButtonClicked();

    void addAPSerieToPipelineButtonClicked();
    void addCombSerieToPipelineButtonClicked();
    //==============================================================================
    // Your private member variables go here...

    TextButton openButton;
    TextButton playButton;
    TextButton playImpulseButton;
    TextButton stopButton;


    TextButton addAllpassButton;
    TextButton addCombButton;
    TextButton addDelayButton;

    TextButton addAPSerieToPipelineButton;
    TextButton addCombSerieToPipelineButton;

    ToggleButton impulseButton;

    Slider delaySlider;
    Slider gainSlider;
    Slider dryWetSlider;

    Label rtLabel;
    Label allpassCountLabel;
    Label combCountLabel;
    Label delayCountLabel;

    ToggleButton loopingToggle;
    Label currentPositionLabel;

    AudioFormatManager formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioDeviceManager::AudioDeviceSetup deviceSetup;

    ProcessingPipeline left_channel_processing;
    ProcessingPipeline right_channel_processing;

    AllpassReverbSeries* leftAllpassReverbSeriesToAdd;
    AllpassReverbSeries* rightAllpassReverbSeriesToAdd;

    CombReverbParallel* leftCombReverbParallelToAdd;
    CombReverbParallel* rightCombReverbParallelToAdd;

    AudioTransportSource transportSource;
    bool fileLoaded;
    bool impulsePlayed;

    TransportState tState;
    ImpulseTestState iState;
    Mode currentMode;

    int allpassCount, combCount,delayCount;
    float reverbTime;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
