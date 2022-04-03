// #include "../JuceLibraryCode/JuceHeader.h"
#pragma once
#include <JuceHeader.h>
#include "StateManager.h"
#include "States.h"
#include "Tokenizer.h"
#include <regex>
#include <list>

using namespace juce;

#ifndef MADEXPLAYER_H
#define MADEXPLAYER_H


class MadexPlayer: 
                    public AudioSource,
                    public ActionListener,
                    public ActionBroadcaster,
                    public PlayerStateManager
{
          public:
                    MadexPlayer(AudioFormatManager &formatManager, PlayerState *playerstate);
                   
                    ~MadexPlayer();
                    // A struct to hold created samples across the player and sampler components
                    struct playerSample
                    {
                              URL track;
                              double start{-1};
                              double end{-1};
                              double speed{1};
                    };
                    // AudioSource JUCE functionality
                    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
                    void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
                    void releaseResources() override;
                    
                    // Player functionality  
                    void start();
                    void stop();
                    void pause();
                    void loadURL(URL audioURL);
                    void resetCue();

                    // *setters
                    void setGain(double gain);
                    void setSpeed(double ratio);
                    void setPosition(double posInSecs);
                    void setPositionRelative(double pos);
                    void setDirection(int dir);
                    void setGainRelative(double gain);
                    void setCue();
                    

                    // *getters
                    
                    std::shared_ptr<playerSample> getSample(int sampleID); 
                    double getPositionRelative();
                    double getPositionAbsolute();
                    double getGain();
                    double getLoadedTrackLength();
                    String getAvailableFormats();
                    

                    //* Listeners
                    void actionListenerCallback(const String &message) override;
                    
                    
                    //* Helpers
                    bool verifySample(std::shared_ptr<playerSample> sample);
                    bool isPlaying();

                    // StateManager functionality
                    void loadPressed(URL& url);
                    void playPressed();
                    void stopPressed();
                    void cuePressed();
                    void samplePressed();
                    void makeSample();
                    void delSample(int &id);
                    
          
          private:
                    // JUCE AudioSource required members
                    AudioFormatManager& formatManager;
                    std::unique_ptr<AudioFormatReaderSource> readerSource;
                    AudioTransportSource transportSource;
                    MixerAudioSource mixerSource;
                    ResamplingAudioSource resampleSource{&transportSource, false, 2};
                   
                    // a simple string to maintain the loaded track filepath
                    String loadedTrack;
                    int playDirection = 0; //A var to hold information about the direction of the player (backwards/forwards)

                    // Cue & sampling functionality
                    double currentPos{-1};
                    bool sampling{false};
                    double samplingStart {-1};
                    double samplingEnd{-1};

                    //a vector to safely store pointers sample pointers
                    std::shared_ptr<playerSample> smpl1,smpl2,smpl3,smpl4;
                    std::vector<std::shared_ptr<playerSample>> samplesContainer{smpl1,smpl2,smpl3,smpl4};
                    
                    // A timekeeper to be used for updates in the player
                    Random rnd{Time::currentTimeMillis()};
};

#endif