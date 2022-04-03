
#pragma once

// #include "/home/m/Desktop/OOP/webmusicplayer/JuceLibraryCode/JuceHeader.h"
#include <JuceHeader.h>
using namespace juce;
//==============================================================================
/*
*/

//* A class to hold the navbar component as a sidepanel, represents the top section of the sidepanel
class NavBar  : public juce::Component
{
          public:
                    NavBar(const String& navTitle);
                    ~NavBar();
                    //*JUCE component functionality
                    void paint (juce::Graphics&) override;
                    void resized() override;
                    
                    // NavBar Button optional callbacks
                    void setHomeButtonClicked(std::function<void()> callback);
                    void setSettingButtonClicked(std::function<void()> callback);

          private:
                    // *data members to hold a label as a title and two buttons for further navigation options
                    Label navBarLabel;
                    ShapeButton navHomeBtn{"Home", Colours::transparentBlack, Colours::transparentBlack, Colours::transparentBlack};
                    ShapeButton navSettingsBtn{"Settings", Colours::transparentBlack, Colours::transparentBlack, Colours::transparentBlack};
                    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NavBar)
};
