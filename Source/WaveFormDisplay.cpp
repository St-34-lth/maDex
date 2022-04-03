/*
  ==============================================================================

    waveformdisplay.cpp
    Created: 30 Jan 2022 11:58:13am
    Author:  solo

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveFormDisplay.h"

//==============================================================================
WaveFormDisplay::WaveFormDisplay(juce::AudioFormatManager &_formatManagerToUse,
                                 juce::AudioThumbnailCache &_thumbCacheToUse) : audioThumb(1000, _formatManagerToUse, _thumbCacheToUse),
                                                                                fileLoaded(false), position(0)
{
          //Init and listener registration
          audioThumb.addChangeListener(this);
          addMouseListener(this,false);
          thumbWidth=getWidth();
}

WaveFormDisplay::~WaveFormDisplay()
{

}
// JUCE Functionality
void WaveFormDisplay::paint (juce::Graphics& g)
{
 

          g.fillAll(Colours::antiquewhite);
         
          g.setFont(20.0f);
          g.setColour (juce::Colours::blue);
          auto area = getLocalBounds();
          // decide whether to show blue or purple according the ID of the component the display is used by 
          if(getParentComponent()->getComponentID() == "0")
          {
                    g.setColour(juce::Colours::blue);
          };
          if (getParentComponent()->getComponentID() == "1")
          {
                    g.setColour(juce::Colours::purple);
          };
          // Draws the thumbnail on the component
          if (fileLoaded)
          {

                    audioThumb.drawChannel(g, area, 0, audioThumb.getTotalLength(), 0, .8f);

                    if (getParentComponent()->getComponentID() == "0") // blue 
                    {
                              if (cueMarkerSet)
                              {
                                        g.setColour(Colours::green);
                                        g.drawVerticalLine(cueMarkerPos * getWidth(), 0, getHeight());
                                      
                              };
                              g.setColour(juce::Colours::red);
                    };
                    if (getParentComponent()->getComponentID() == "1") //purple
                    {
                              if(cueMarkerSet)
                              {
                                        g.setColour(Colours::yellow);
                                        g.drawVerticalLine(cueMarkerPos * getWidth(), 0, getHeight());
                                       
                              };
                              g.setColour(juce::Colours::black);
                    };
                    g.drawVerticalLine(position * getWidth(), 0, getHeight());
          }
            
          else g.drawText("File not loaded", area,Justification::centredTop, true);
          
}

void WaveFormDisplay::resized()
{
          thumbWidth=getWidth();
}

// As implemented in the starter code - no changes 
void WaveFormDisplay::loadURL(juce::URL audioURL)
{
          audioThumb.clear();
          fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
          
          if(fileLoaded)
          {
                    std::cout << "form loaded" << '\n';
          }
          else  std::cout << "not loaded" << '\n';
                    
          resetCueMarker();

}

// As implemented in the starter code - no changes
void WaveFormDisplay::changeListenerCallback(juce::ChangeBroadcaster *source)
{
          if(source == &audioThumb)
          {
                    repaint();
          }
          
};

// Resets the CUE marker flag and position
void WaveFormDisplay::resetCueMarker() 
{
          cueMarkerSet = false;
          cueMarkerPos = -1;
};

// As implemented in the starter code - no changes
void WaveFormDisplay::setPositionRelative(double pos)
{
         
          if (position != pos)
          {
                    position = pos; 
                    repaint();
          };
}

// Takes in a double representing a position in seconds on the track 
// The cue marker is then set at that position and the flag is set to true
// If called while the flag is set to true, the marker variables are returned to their original values
void WaveFormDisplay::setCueMarker(double pos)
{
          if (pos > 0.01)
          {
                    cueMarkerPos = pos;
                    cueMarkerSet = !cueMarkerSet;
                    return;
          }
          resetCueMarker();
          return ;
};

// Takes in an int that represents a location on the X axis of the component
// Translates the location relative to the thumbnail width and returns the result
// To be used with a mouse listener for clicking on the thumbnail  
double WaveFormDisplay::getCoordinatesOnDisplay(int xLocation) { return (double) (xLocation / thumbWidth) ; };


