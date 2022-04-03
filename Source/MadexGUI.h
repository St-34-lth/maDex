#pragma once

// #include "/home/m/Desktop/OOP/webmusicplayer/JuceLibraryCode/JuceHeader.h"
#include <JuceHeader.h>
#include "MadexPlayer.h"
#include "Playlist.h"
#include "WaveFormDisplay.h"
#include "Tokenizer.h"
#include "NavBar.h"
#include "NavList.h"
#include "Dex.h"
#include "Sampler.h"
#include <memory>

using namespace juce;

#ifndef MADEXGUI_H
#define MADEXGUI_H

class Dex;
class MadexGUI : 
                    public Component,
                    public Slider::Listener,
                    public Button::Listener,
                    public FileDragAndDropTarget,
                    public Timer,
                    public ActionListener,
                    public ActionBroadcaster       
{                            
          // Holds a static variable to hold the gui ID seperately
          inline static int guiID;
          
          public:
                    // Constructor function - takes in a player, sampler object pointers and formatManager and thumbnailcache references
                    MadexGUI(MadexPlayer *_player, AudioFormatManager &_formatManagerToUse, AudioThumbnailCache &_thumbCacheToUse,Sampler *_sampler);
                    // destructor
                    ~MadexGUI() override;
                    
                    // JUCE functionality methods

                              // component utilities
                    void paint (juce::Graphics&) override;
                    void resized() override;
                              // button listeners
                    void buttonClicked(juce::Button *button) override;
                    void buttonStateChanged(juce::Button *) override;
                              // slider listeners
                    void sliderValueChanged(juce::Slider *slider) override;
                              // file listeners
                    bool isInterestedInFileDrag(const juce::StringArray &files) override;
                    void filesDropped(const juce::StringArray &files, int x, int y) override;
                              // action listener 
                    void actionListenerCallback(const String &message) override;
                              // mouse listeners
                    void mouseDown(const MouseEvent &e) override;
                    void mouseDrag(const MouseEvent &e) override;
                              // label listener
                    void labelTextChanged(Label *labelThatHasChanged);
                    
                              // utilities
                    void timerCallback() override;

                    // Own methods
                    double getPlayerPositionAbsolute();
                    double getGainSliderValue();
                    void findTrack();
                    int getGuiId();
                    
          private :
                    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MadexGUI);
                    
                    // A juce file object instance to hold the primary location in which to store/load playlists, you can change this to anything
                    File initDirectory{"/home/m/Desktop/OOP/madex"};

                    // side bar component & callback methods
                    std::shared_ptr <SidePanel> sidePanel;
                    void homeButtonClicked();
                    void settingsButtonClicked();
                    void listEntryClicked(int index);
                    
                    // labels and text editors
                    Label trackDisplay{"trackDisplayBox", "No song loaded"};
                    Label navBarStatus;
                    TextEditor searchBox{"searchBox", 0};
                    
                    // Buttons
                    DrawableButton playButton{"Play", DrawableButton::ButtonStyle::ImageFitted};
                    DrawableButton stopButton{"Stop",DrawableButton::ButtonStyle::ImageFitted};
                    DrawableButton cueButton{"Cue",DrawableButton::ButtonStyle::ImageFitted};
                    DrawableButton sampleButton{"Load sample",DrawableButton::ButtonStyle::ImageFitted};
                    DrawableButton savePlaylist{"Save tracks",DrawableButton::ButtonStyle::ImageFitted};
                    DrawableButton loadPlaylist{"Load tracks",DrawableButton::ButtonStyle::ImageFitted};
                    DrawableButton newPlaylist{"New list",DrawableButton::ButtonStyle::ImageFitted};
                    TextButton showNavBar;

                    // declare the sampling buttons here
                    TextButton s1{"S1"}, s2{"S2"}, s3{"S3"}, s4{"S4"};
                    TextButton d1{""}, d2{""}, d3{""}, d4{""};

                    // Sliders
                    Slider gainSlider{};
                    Slider tempo{};

                    // *data members
                    std::shared_ptr<MadexPlayer> player; // smart pointer to hold the instance of the deck object
                    std::shared_ptr<Sampler> sampler;   // smart pointer to hold the instance of the deck object
                    std::unique_ptr<Playlist> playlist; // smart pointer to hold the instance of the deck object
                    std::shared_ptr<Dex> deck; //smart pointer to hold the instance of the deck object
                    WaveFormDisplay display;  // member to hold the waveform display
                    bool settingDeck{false}; // flag to track when the deck control is activated
                    int id; // var that holds the GUI's static ID 
                    
                    // declare two vectors of pointers that holds references to the sampling  and delete buttons
                    std::vector<TextButton*> sampleButtons{&s1, &s2, &s3, &s4}; 
                    std::vector<TextButton*> deleteSampleButtons{&d1, &d2, &d3, &d4};
};          
#endif