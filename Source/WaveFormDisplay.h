/*
  ==============================================================================

    waveformdisplay.h
    Created: 30 Jan 2022 11:58:13am
    Author:  solo

  ==============================================================================
*/

#pragma once
#include "Tokenizer.h"
#include "../JuceLibraryCode/JuceHeader.h"

using namespace juce;
//==============================================================================
/*
*/
// A component that holds a WaveForm like display
// Takes in references to formatManager and AudioThumbnailCache
class WaveFormDisplay :       public Component, 
                              public ChangeListener                     
{
public:
          WaveFormDisplay(AudioFormatManager &_formatManagerToUse,
                          AudioThumbnailCache &_thumbCacheToUse);
          ~WaveFormDisplay() override;
          
          // JUCE functionality
          void paint(Graphics &) override;
          void resized() override;
          
          // Setters
          void setPositionRelative(double pos);
          void setCueMarker(double pos);
          void resetCueMarker();
          void loadURL(URL audioURL);
          
          // Getter
          double getCoordinatesOnDisplay(int xLocation);
          
          // change listener functionality
          void changeListenerCallback(ChangeBroadcaster *source) override;

private:
          AudioThumbnail audioThumb; //holds the audiothumbnail reference 
          double position;  //holds the current position of the track 
          bool fileLoaded ; //holds the currently loaded file 
          bool cueMarkerSet{false}; //a flag to indicate whether a CUE marker has been set or not by the player 
          double cueMarkerPos {-1};  // a variable to hold the CUE marker's position on the display 
          double thumbWidth {0}; // a variable to hold the thumbnail's width
          
          JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveFormDisplay);
};
