#pragma once
#include <JuceHeader.h>
// #include "../JuceLibraryCode/JuceHeader.h"
#include <memory>
#include <vector>

#include "MadexPlayer.h"
#include "States.h"
#include "StateManager.h"

using namespace juce;
#ifndef SAMPLER_H
#define SAMPLER_H

// * A leaner copy of the MadexPlayer class to provide sampling functionalities for the application.
// Takes in a formatManager reference and playerstate pointer
// Only provides playback control functionalities, no samples are stored here
class Sampler :     public AudioSource,
                    public PlayerStateManager       
{
          public:
                    Sampler(AudioFormatManager &formatManager, PlayerState *playerstate);
                    ~Sampler();
                    //*JUCE AudioSource implementation
                    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
                    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
                    void releaseResources() override;
                    // Sampler functionality
                    void start();
                    void stop();
                    void pause();
                    void loadSample(std::shared_ptr<MadexPlayer::playerSample> sample);
                    void setGain(double gain);
                    void playPressed() {};
                    void loadPressed(juce::URL &url){};
                    void cuePressed() {};
                    void stopPressed() {};
                   
          private:
                    
                    MadexPlayer::playerSample currentSample; // a variable that holds details on the currentSample, if one exists
                    URL loadedSampleURL; // a URL variable to hold the currently loadedSample track 
                    AudioFormatManager &formatManager;
                    std::shared_ptr<AudioFormatReaderSource> readerSource;
                    AudioTransportSource transportSource;
                    ResamplingAudioSource resampleSource{&transportSource, false, 2};
                    MixerAudioSource mixerSource;
};
#endif