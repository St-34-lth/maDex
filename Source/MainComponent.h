#pragma once

#include <JuceHeader.h>
#include <random>
#include "MadexPlayer.h"
#include "MadexGUI.h"
#include "Playlist.h"
#include "Sampler.h"

using namespace juce;
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent: 
                    public AudioAppComponent,
                    public ActionListener,
                    public Slider::Listener
{
          public:
                    //==============================================================================
                    MainComponent();
                    ~MainComponent() override;

                    //==============================================================================
                    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
                    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
                    void releaseResources() override;

                    //==============================================================================
                    void paint (juce::Graphics& g) override;
                    void resized() override;
                    void actionListenerCallback (const String& message);
                    void sliderValueChanged(Slider *slider);
                    
          private:
                    // Init of basic JUCE audio functionality 
                    MixerAudioSource mixerSource;
                    AudioFormatManager formatManager;
                    AudioFormatManager samplerFormatManager;
                    // Init of a thumbnail cache for the waveform display
                    AudioThumbnailCache thumbCache{100};
                    
                    // Init of player decks  
                    MadexPlayer playerL{formatManager, new StoppedState{}};
                    MadexPlayer playerR{formatManager,new StoppedState{}};
                    
                    // Init of sampler decks
                    Sampler samplerL{samplerFormatManager, new StoppedState{}};
                    Sampler samplerR{samplerFormatManager, new StoppedState{}};

                    // Init of deck gui components
                    MadexGUI guiL{&playerL, formatManager, thumbCache, &samplerL};
                    MadexGUI guiR{&playerR, formatManager, thumbCache, &samplerR};

                    // Init of main juce TooltipWindow to display tooltips around the app 
                    std::shared_ptr<TooltipWindow> tooltip;

                    // Vars, components etc
                    Slider mixer{};
                    double playerRgain {0};
                    double playerLgain {0};
                    
                    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent);
};
