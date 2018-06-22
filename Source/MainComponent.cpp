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
    delayCount = 0;
    allpassCount = 0;
    combCount = 0;

    leftAllpassReverbSeriesToAdd = nullptr;
    rightAllpassReverbSeriesToAdd = nullptr;

    leftCombReverbParallelToAdd = nullptr;
    rightCombReverbParallelToAdd = nullptr;

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
    addAllpassButton.setButtonText("Add Allpass block to serie");
    addAllpassButton.onClick = [this] { addAllpassButtonClicked(); };
    addAllpassButton.setColour(TextButton::buttonOnColourId, Colours::blue);
    addAllpassButton.setEnabled(false);

    addAndMakeVisible(&addCombButton);
    addCombButton.setButtonText("Add Comb block to serie");
    addCombButton.onClick = [this] { addCombButtonClicked(); };
    addCombButton.setColour(TextButton::buttonOnColourId, Colours::blue);
    addCombButton.setEnabled(false);

    addAndMakeVisible(&addDelayButton);
    addDelayButton.setButtonText("Add Delay block to pipeline");
    addDelayButton.onClick = [this] { addDelayButtonClicked(); };
    addDelayButton.setColour(TextButton::buttonOnColourId, Colours::blue);
    addDelayButton.setEnabled(false);

    addAndMakeVisible(&addAPSerieToPipelineButton);
    addAPSerieToPipelineButton.setButtonText("Add allpass serie to pipeline");
    addAPSerieToPipelineButton.onClick = [this] { addAPSerieToPipelineButtonClicked(); };
    addAPSerieToPipelineButton.setColour(TextButton::buttonOnColourId, Colours::blue);
    addAPSerieToPipelineButton.setEnabled(false);

    addAndMakeVisible(&addCombSerieToPipelineButton);
    addCombSerieToPipelineButton.setButtonText("Add parallel combs to pipeline");
    addCombSerieToPipelineButton.onClick = [this] { addCombSerieToPipelineButtonClicked(); };
    addCombSerieToPipelineButton.setColour(TextButton::buttonOnColourId, Colours::blue);
    addCombSerieToPipelineButton.setEnabled(false);

    addAndMakeVisible(&delaySlider);
    delaySlider.setRange(1.0, 1000.0, 1.0);
    delaySlider.setValue(100.0);
    delaySlider.setEnabled(false);

    addAndMakeVisible(&gainSlider);
    gainSlider.setRange(0.01, 0.99, 0.01);
    gainSlider.setValue(0.7);
    gainSlider.setEnabled(false);

    addAndMakeVisible(&dryWetSlider);
    dryWetSlider.setRange(0.00, 1.00, 0.01);
    dryWetSlider.setValue(0.5);
    dryWetSlider.setEnabled(false);

    addAndMakeVisible(&allpassCountLabel);
    allpassCountLabel.setText(std::to_string(allpassCount), dontSendNotification);
    
    addAndMakeVisible(&combCountLabel);
    combCountLabel.setText(std::to_string(combCount), dontSendNotification);

    addAndMakeVisible(&delayCountLabel);
    delayCountLabel.setText(std::to_string(delayCount), dontSendNotification);

    addAndMakeVisible(&loopingToggle);
    loopingToggle.setButtonText("Loop");
    loopingToggle.onClick = [this] { loopButtonChanged(); };

    addAndMakeVisible(&currentPositionLabel);
    currentPositionLabel.setText("Stopped", dontSendNotification);

    setSize(600, 600);

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
    dryWetSlider.setBounds(10, 400, getWidth() - 20, 20);
    

    addDelayButton.setBounds(10, 160, getWidth() - 20, 20);

    addAllpassButton.setBounds(10, 220, getWidth()/2 - 15, 20);
    addAPSerieToPipelineButton.setBounds(10 + getWidth()/2 - 15 + 10, 220, getWidth()/2 - 15, 20);

    addCombButton.setBounds(10, 190, getWidth()/2 - 15, 20);
    addCombSerieToPipelineButton.setBounds(10 + getWidth()/2 - 15 + 10, 190, getWidth()/2 - 15, 20);

    delayCountLabel.setBounds(10, 310, getWidth() - 20, 20);
    allpassCountLabel.setBounds(10, 370, getWidth() - 20, 20);
    combCountLabel.setBounds(10, 340, getWidth() - 20, 20);

    loopingToggle.setBounds(10, 250, getWidth() - 20, 20);
    currentPositionLabel.setBounds(10, 280, getWidth() - 20, 20);
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

        for(size_t i = 0; i < numberOfSamples; i++)
        {
            *(leftChannelPtr+i) += dryWetSlider.getValue()*left_channel_processing.process(*(leftChannelPtr+i));
            *(rightChannelPtr+i) += dryWetSlider.getValue()*right_channel_processing.process(*(rightChannelPtr+i));
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
            addDelayButton.setEnabled(true);
            delaySlider.setEnabled(true);
            gainSlider.setEnabled(true);
            dryWetSlider.setEnabled(true);
        }
    }
}

void MainComponent::playButtonClicked()
{

    left_channel_processing.clear();
    right_channel_processing.clear();

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

    if(leftAllpassReverbSeriesToAdd == nullptr)
    {
        leftAllpassReverbSeriesToAdd = new AllpassReverbSeries();
    }

    if(rightAllpassReverbSeriesToAdd == nullptr)
    {
        rightAllpassReverbSeriesToAdd = new AllpassReverbSeries();
    }

    leftAllpassReverbSeriesToAdd->addBlock(delay_samples, gainSlider.getValue());
    rightAllpassReverbSeriesToAdd->addBlock(delay_samples, gainSlider.getValue());  

    addAPSerieToPipelineButton.setEnabled(true);
    allpassCountLabel.setText(std::to_string(++allpassCount), dontSendNotification);
}


void MainComponent::addCombButtonClicked()
{
    int delay_samples = readerSource->getAudioFormatReader()->sampleRate*delaySlider.getValue()/1000;

    if(leftCombReverbParallelToAdd == nullptr)
    {
        leftCombReverbParallelToAdd = new CombReverbParallel();
    }

    if(rightCombReverbParallelToAdd == nullptr)
    {
        rightCombReverbParallelToAdd = new CombReverbParallel();
    }

    leftCombReverbParallelToAdd->addBlock(delay_samples, gainSlider.getValue());
    rightCombReverbParallelToAdd->addBlock(delay_samples, gainSlider.getValue());  
    
    addCombSerieToPipelineButton.setEnabled(true);
    combCountLabel.setText(std::to_string(++combCount), dontSendNotification);
}

void MainComponent::addDelayButtonClicked()
{
    int delay_samples = readerSource->getAudioFormatReader()->sampleRate*delaySlider.getValue()/1000;

    DelayBlock* ptr = new DelayBlock(delay_samples);
    left_channel_processing.addBlockToPipeline(dynamic_cast<IReverbBlock*>(ptr));

    ptr = new DelayBlock(delay_samples);
    right_channel_processing.addBlockToPipeline(dynamic_cast<IReverbBlock*>(ptr));

    delayCountLabel.setText(std::to_string(++delayCount), dontSendNotification);
}

void MainComponent::addCombSerieToPipelineButtonClicked()
{
    left_channel_processing.addBlockToPipeline(dynamic_cast<IReverbBlock*>(leftCombReverbParallelToAdd));
    right_channel_processing.addBlockToPipeline(dynamic_cast<IReverbBlock*>(rightCombReverbParallelToAdd));

    leftCombReverbParallelToAdd = nullptr;
    rightCombReverbParallelToAdd = nullptr;
    combCount = 0;
    
    addCombSerieToPipelineButton.setEnabled(false);

    combCountLabel.setText(std::to_string(combCount), dontSendNotification);

}

void MainComponent::addAPSerieToPipelineButtonClicked()
{   
    left_channel_processing.addBlockToPipeline(dynamic_cast<IReverbBlock*>(leftAllpassReverbSeriesToAdd));
    right_channel_processing.addBlockToPipeline(dynamic_cast<IReverbBlock*>(rightAllpassReverbSeriesToAdd));

    leftAllpassReverbSeriesToAdd = nullptr;
    rightAllpassReverbSeriesToAdd = nullptr;
    allpassCount = 0;
    addAPSerieToPipelineButton.setEnabled(false);

    allpassCountLabel.setText(std::to_string(allpassCount), dontSendNotification);
}
