#pragma once						// PlayerGUI.h
#include <JuceHeader.h>
#include "PlayerAudio.h"


class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer,
    public juce::ListBoxModel

{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    void timerCallback() override;

    void load_and_play(const juce::File& file);

private:
    PlayerAudio playerAudio;


    // GUI elements
    juce::ListBox playlistBox{ "Playlist", this };
    juce::TextButton loadButton{ "Load File" };
    juce::TextButton playButton{ "|>" };
    juce::TextButton startButton{ "|< Start" };
    juce::TextButton endButton{ "End >|" };
    juce::TextButton pauseButton{ "||" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton loopButton{ "loop" };
    juce::Slider volumeSlider;
    juce::Slider timeSlider;

    

    std::unique_ptr<juce::FileChooser> fileChooser;

    // Event handlers
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};