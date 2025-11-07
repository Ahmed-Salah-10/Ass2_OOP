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
int is_looping = false;
void PlayerGUI::timerCallback()
{
    if (playerAudio.has_finish() && is_looping)
    {
        playerAudio.setPosition(0.0);
        playerAudio.start();
    }
    timeSlider.setValue(playerAudio.getPosition());
}

void PlayerGUI::openFileChooser()
{

    fileChooser = std::make_unique<juce::FileChooser>(
        "Select an audio file...",
        juce::File{},
        "*.wav;*.mp3");

    fileChooser->launchAsync(
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectMultipleItems,
        [this](const juce::FileChooser& fc)
        {
            auto results = fc.getResults();

            for (auto& file : results)
            {
                if (file.existsAsFile())
                {
                    files.add(file);
                    currentIndex++;
                    DBG("Added: " << file.getFileName());
                }
            }
            playlistBox.updateContent();
            playlistBox.repaint();

        });
}

int PlayerGUI::getNumRows()
{
    return files.size();
}

void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::lightblue);
    else
        g.fillAll(juce::Colours::black);

    if (juce::isPositiveAndBelow(rowNumber, files.size()))
    {
        g.setColour(juce::Colours::white);
        g.drawText(files[rowNumber].getFileName(),
            5, 0, width - 10, height,
            juce::Justification::centredLeft);
    }
}

void PlayerGUI::selectedRowsChanged(int lastRowSelected)
{
    if (juce::isPositiveAndBelow(lastRowSelected, files.size()))
    {
        currentIndex = lastRowSelected;
        DBG("Selected: " << files[currentIndex].getFileName());
        load_and_play(files[currentIndex]);
    }
}

void PlayerGUI::load_and_play(const juce::File& file)
{
    playerAudio.loadFile(file);
    timeSlider.setRange(0.0, playerAudio.getLength());
    startTimer(1000);
    file_name.setText("file name: " + file.getFileNameWithoutExtension(), juce::dontSendNotification);
    double lengthInSeconds = playerAudio.getLength();
    juce::String durationText;
    if (lengthInSeconds >= 60.0)
    {
        int minutes = static_cast<int>(lengthInSeconds / 60.0);
        int seconds = static_cast<int>(std::fmod(lengthInSeconds, 60.0));
        durationText = juce::String(minutes) + " min " + juce::String(seconds).paddedLeft('0', 2) + " sec";
    }
    else
    {
        durationText = juce::String(lengthInSeconds, 2) + " sec";
    }
    file_lenght.setText("  Lenght: " + durationText, juce::dontSendNotification);
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

PlayerGUI::PlayerGUI()
{
    // Add buttons
    for (auto* btn : { &loadButton, &playButton , &startButton, &endButton ,&pauseButton, &muteButton, &loopButton, &addButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.2);
    volumeSlider.addListener(this);
    volumeSlider.setHelpText("volume");
    addAndMakeVisible(volumeSlider);

    timeSlider.setRange(0.0, 100.0);
    timeSlider.setValue(0.0);
    timeSlider.addListener(this);
    addAndMakeVisible(timeSlider);
    timeSlider.setNumDecimalPlacesToDisplay(0);

    speedSlider.setRange(0.25, 2.00, 0.25);
    speedSlider.setValue(1.00);
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);


    addAndMakeVisible(file_name);
    //file_name.setJustificationType(juce::Justification::right);
    //file_name.setColour(juce::Label::textColourId, juce::Colours::blue);

    addAndMakeVisible(playlistBox);
    playlistBox.setRowHeight(50);

    addAndMakeVisible(file_lenght);
    file_name.setFont(20);
    file_lenght.setFont(16);

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
    loopButton.setBounds(640, y, 80, 40);
    addButton.setBounds(850, 30, 150, 60);

    /*prevButton.setBounds(340, y, 80, 40);
    nextButton.setBounds(440, y, 80, 40);*/

    volumeSlider.setBounds(20, 100, 700, 30);
    timeSlider.setBounds(20, 200, 700, 30);
    speedSlider.setBounds(20, 300, 700, 30);

    file_name.setBounds(20, getHeight() - 150, 400, 100);
    file_lenght.setBounds(20, getHeight() - 120, 400, 100);


    playlistBox.setBounds(750, 100, 340, 350);

}
PlayerGUI::~PlayerGUI()
{
}
double temp = 0.0;
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
                    load_and_play(file);
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

    if (button == &loopButton)
    {
        if (!is_looping)
        {
            is_looping = true;
            loopButton.setButtonText("islooping");
        }
        else
        {
            is_looping = false;
            loopButton.setButtonText("unlooping");
        }
    }
    if (button == &addButton)
    {
        openFileChooser();
    }

}
void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
    if (slider == &timeSlider)
        playerAudio.setPosition((float)slider->getValue());
    if (slider == &speedSlider)
    {
        double ratio = speedSlider.getValue();
        playerAudio.setSpeed(ratio);
    }
}