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
     enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };

    void changeState (TransportState);
    void openButtonClicked();
    void playButtonClicked();
    void stopButtonClicked();
    void loopButtonChanged();

    void addCombButtonClicked();
    void addAllpassButtonClicked();
    void addDelayButtonClicked();
    //==============================================================================
    // Your private member variables go here...

    TextButton openButton;
    TextButton playButton;
    TextButton stopButton;


    TextButton addAllpassButton;
    TextButton addCombButton;
    TextButton addDelayButton;


    Slider delaySlider;
    Slider gainSlider;

    Label allpassCountLabel;
    Label combCountLabel;
    Label delayCountLabel;

    ToggleButton loopingToggle;
    Label currentPositionLabel;

    AudioFormatManager formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;

    ProcessingPipeline left_channel_processing;
    ProcessingPipeline right_channel_processing;

    CombReverbParallel left_combReverbs;
    AllpassReverbSeries left_allpassReverbs;

    CombReverbParallel right_combReverbs;
    AllpassReverbSeries right_allpassReverbs;

    AudioTransportSource transportSource;
    TransportState state;

    int allpassCount, combCount,delayCount;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
