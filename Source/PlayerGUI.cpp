
#include "PlayerGUI.h"

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}
void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}
void PlayerGUI::timerCallback()
{
    // double currentValue = playerAudio.getPosition();
    timeSlider.setValue(playerAudio.getPosition());
}
void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

PlayerGUI::PlayerGUI()
{
    // Add buttons
    for (auto* btn : { &loadButton, &playButton , &startButton, &endButton ,&pauseButton, &muteButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.2);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(timeSlider);
    timeSlider.setRange(0.0, 100.0);
    timeSlider.setValue(0.0);
    //timeSlider.addListener(this);
}
void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    playButton.setBounds(140, y, 80, 40);
    startButton.setBounds(240, y, 80, 40);
    endButton.setBounds(340, y, 80, 40);
    pauseButton.setBounds(440, y, 80, 40);
    muteButton.setBounds(540, y, 80, 40);


    /*prevButton.setBounds(340, y, 80, 40);
    nextButton.setBounds(440, y, 80, 40);*/

    volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
    timeSlider.setBounds(20, 200, getWidth() - 40, 30);

}
PlayerGUI::~PlayerGUI()
{
}
double temp = 0.0, height;
bool is_mute = false;
void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        juce::FileChooser chooser("Select audio files...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    playerAudio.loadFile(file);
                    timeSlider.setRange(0.0, playerAudio.getLength());
                    startTimer(1000);
                }

            });
    }

    if (button == &playButton)
    {
        playerAudio.start();
    }

    if (button == &startButton)
    {
        playerAudio.setPosition(0.0);
    }
    if (button == &endButton)
    {
        playerAudio.setPosition(playerAudio.getLength());
    }

    if (button == &pauseButton)
    {
        float now = playerAudio.getPosition();
        playerAudio.stop();
        playerAudio.setPosition(now);
    }

    if (button == &muteButton)
    {
        if (!is_mute)
        {
            temp = volumeSlider.getValue();
            volumeSlider.setValue(0.0);
            muteButton.setButtonText("Unmute");
            is_mute = true;
        }
        else
        {
            volumeSlider.setValue(temp);
            muteButton.setButtonText("Mute");
            is_mute = false;
        }
    }


}
void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
    if (slider == &timeSlider)
        playerAudio.setPosition((float)slider->getValue());
}