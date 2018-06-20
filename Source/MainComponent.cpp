/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : state(Stopped)
{
    left_allpassReverbs = AllpassReverbSeries();
    left_combReverbs = CombReverbParallel();

    right_allpassReverbs = AllpassReverbSeries();
    right_combReverbs = CombReverbParallel();

    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);


    addAndMakeVisible(&openButton);
    openButton.setButtonText("Open...");
    openButton.onClick = [this] { openButtonClicked(); };

    addAndMakeVisible(&playButton);
    playButton.setButtonText("Play");
    playButton.onClick = [this] { playButtonClicked(); };
    playButton.setColour(TextButton::buttonColourId, Colours::green);
    playButton.setEnabled(false);

    addAndMakeVisible(&stopButton);
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setColour(TextButton::buttonColourId, Colours::red);
    stopButton.setEnabled(false);

    addAndMakeVisible(&addAllpassButton);
    addAllpassButton.setButtonText("Add Allpass block");
    addAllpassButton.onClick = [this] { addAllpassButtonClicked(); };
    addAllpassButton.setColour(TextButton::buttonOnColourId, Colours::blue);
    addAllpassButton.setEnabled(false);

    addAndMakeVisible(&addCombButton);
    addCombButton.setButtonText("Add Comb block");
    addCombButton.onClick = [this] { addCombButtonClicked(); };
    addCombButton.setColour(TextButton::buttonOnColourId, Colours::blue);
    addCombButton.setEnabled(false);

    addAndMakeVisible(&delaySlider);
    delaySlider.setRange(1.0, 1000.0, 1.0);
    delaySlider.setValue(100.0);
    delaySlider.setEnabled(false);

    addAndMakeVisible(&gainSlider);
    gainSlider.setRange(0.01, 0.99, 0.01);
    gainSlider.setValue(0.7);
    gainSlider.setEnabled(false);

    addAndMakeVisible(&allpassCountLabel);
    allpassCountLabel.setText(std::to_string(0), dontSendNotification);
    
    addAndMakeVisible(&combCountLabel);
    combCountLabel.setText(std::to_string(0), dontSendNotification);

    addAndMakeVisible(&loopingToggle);
    loopingToggle.setButtonText("Loop");
    loopingToggle.onClick = [this] { loopButtonChanged(); };

    addAndMakeVisible(&currentPositionLabel);
    currentPositionLabel.setText("Stopped", dontSendNotification);

    setSize(600, 400);

    setAudioChannels(2, 2);
    startTimer(20);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    openButton.setBounds(10, 10, getWidth() - 20, 20);
    playButton.setBounds(10, 40, getWidth() - 20, 20);
    stopButton.setBounds(10, 70, getWidth() - 20, 20);

    delaySlider.setBounds(10, 100, getWidth() - 20, 20);
    gainSlider.setBounds(10, 130, getWidth() - 20, 20);

    addAllpassButton.setBounds(10, 190, getWidth() - 20, 20);
    addCombButton.setBounds(10, 160, getWidth() - 20, 20);

    allpassCountLabel.setBounds(10, 310, getWidth() - 20, 20);
    combCountLabel.setBounds(10, 280, getWidth() - 20, 20);

    loopingToggle.setBounds(10, 220, getWidth() - 20, 20);
    currentPositionLabel.setBounds(10, 250, getWidth() - 20, 20);
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    transportSource.getNextAudioBlock(bufferToFill);
    
    if(state == TransportState::Playing)
    {
        float* leftChannelPtr = bufferToFill.buffer->getWritePointer(0);
        float* rightChannelPtr = bufferToFill.buffer->getWritePointer(1);
        int numberOfSamples = bufferToFill.numSamples;
        
        if(left_combReverbs.getCount() > 0)
        {
            for(size_t i = 0; i < numberOfSamples; i++)
            {
                *(leftChannelPtr+i) += left_combReverbs.process(*(leftChannelPtr+i));
                *(rightChannelPtr+i) += right_combReverbs.process(*(rightChannelPtr+i));
            }   
        }
        
        if(left_allpassReverbs.getCount() > 0)
        {
            for(size_t i = 0; i < numberOfSamples; i++)
            {
                *(leftChannelPtr+i) += left_allpassReverbs.process(*(leftChannelPtr+i));
                *(rightChannelPtr+i) += right_allpassReverbs.process(*(rightChannelPtr+i));
            }
        }
        
    }
    
}

void MainComponent::releaseResources()
{
    transportSource.releaseResources();
}

void MainComponent::changeListenerCallback(ChangeBroadcaster *source)
{
    if (source == &transportSource)
    {
        if (transportSource.isPlaying())
            changeState(Playing);
        else
            changeState(Stopped);
    }
}

void MainComponent::timerCallback()
{
    if (transportSource.isPlaying())
    {
        RelativeTime position(transportSource.getCurrentPosition());

        auto minutes = ((int)position.inMinutes()) % 60;
        auto seconds = ((int)position.inSeconds()) % 60;
        auto millis = ((int)position.inMilliseconds()) % 1000;

        auto positionString = String::formatted("%02d:%02d:%03d", minutes, seconds, millis);

        currentPositionLabel.setText(positionString, dontSendNotification);
    }
    else
    {
        currentPositionLabel.setText("Stopped", dontSendNotification);
    }
}

void MainComponent::updateLoopState(bool shouldLoop)
{
    if (readerSource.get() != nullptr)
        readerSource->setLooping(shouldLoop);
}

void MainComponent::changeState(TransportState newState)
{
    if (state != newState)
    {
        state = newState;

        switch (state)
        {
        case Stopped:
            stopButton.setEnabled(false);
            playButton.setEnabled(true);
            transportSource.setPosition(0.0);
            break;

        case Starting:
            playButton.setEnabled(false);
            transportSource.start();
            break;

        case Playing:
            stopButton.setEnabled(true);
            break;

        case Stopping:
            transportSource.stop();
            break;
        }
    }
}

void MainComponent::openButtonClicked()
{
    FileChooser chooser("Select a Wave file to play...",
                        File::nonexistent,
                        "*.wav");

    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        auto *reader = formatManager.createReaderFor(file);

        if (reader != nullptr)
        {
            std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
            transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
            playButton.setEnabled(true);
            readerSource.reset(newSource.release());
            addAllpassButton.setEnabled(true);
            addCombButton.setEnabled(true);
            delaySlider.setEnabled(true);
            gainSlider.setEnabled(true);
        }
    }
}

void MainComponent::playButtonClicked()
{
    if(left_allpassReverbs.getCount() != 0)
    {
        left_allpassReverbs.clear();
        right_allpassReverbs.clear();
    }
    
    if(left_combReverbs.getCount() != 0)
    {
        left_combReverbs.clear();
        right_combReverbs.clear();
    }

    updateLoopState(loopingToggle.getToggleState());
    changeState(Starting);
}

void MainComponent::stopButtonClicked()
{
    changeState(Stopping);
}

void MainComponent::loopButtonChanged()
{
    updateLoopState(loopingToggle.getToggleState());
}

void MainComponent::addAllpassButtonClicked()
{
    int delay_samples = readerSource->getAudioFormatReader()->sampleRate*delaySlider.getValue()/1000;

    left_allpassReverbs.addBlock(delay_samples, gainSlider.getValue());
    right_allpassReverbs.addBlock(delay_samples, gainSlider.getValue());

    allpassCountLabel.setText(std::to_string(left_allpassReverbs.getCount()), dontSendNotification);
}


void MainComponent::addCombButtonClicked()
{
    int delay_samples = readerSource->getAudioFormatReader()->sampleRate*delaySlider.getValue()/1000;

    left_combReverbs.addBlock(delay_samples, gainSlider.getValue());
    right_combReverbs.addBlock(delay_samples, gainSlider.getValue());
    
    combCountLabel.setText(std::to_string(left_combReverbs.getCount()), dontSendNotification);
}
