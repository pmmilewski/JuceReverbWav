/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "CombReverbParallel.h"
#include "AllpassReverbSeries.h"

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
    //==============================================================================
    // Your private member variables go here...

    TextButton openButton;
    TextButton playButton;
    TextButton stopButton;
    ToggleButton loopingToggle;
    Label currentPositionLabel;

    AudioFormatManager formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;

    std::unique_ptr<CombReverbParallel> left_combReverbs;
    std::unique_ptr<AllpassReverbSeries> left_allpassReverbs;

    std::unique_ptr<CombReverbParallel> right_combReverbs;
    std::unique_ptr<AllpassReverbSeries> right_allpassReverbs;

    AudioTransportSource transportSource;
    TransportState state;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
