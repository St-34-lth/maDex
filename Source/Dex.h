/*
  ==============================================================================

    Dex.h
    Created: 3 Mar 2022 1:56:22pm
    Author:  m

  ==============================================================================
*/

#pragma once

// #include "../JuceLibraryCode/JuceHeader.h"
#include <JuceHeader.h>
#include <regex>
using namespace juce;

//==============================================================================
/*
*/
#ifndef DEX_H
#define DEX_H
#include "MadexGUI.h"
#include "Tokenizer.h"
class MadexGUI;

// A simple cusstom made animation component that holds any image and rotates with a constant speed 
// It also holds a loaded track to allow itself to set and rotate in a controlled manner if necessary 
class Dex: 
                    public AnimatedAppComponent,
                    public SettableTooltipClient
{
          public:
                    Dex();
                    ~Dex() override;
                    // JUCE functionality
                    void paint (juce::Graphics&) override;
                    void resized() override;
                    void update() override;

                    // Control functionality & setters
                    void start();
                    void stop();
                    void setAnimationImage(String& animationImage);
                    void setTrackLength(const double& lengthInSeconds);
                    void setRotation(Point<float> rotationPoint);
                    void setPositionAbs(double& pos);
                    void addAngle(int& direction);
                    void setPosInRads(double& pos);

                    // Getters
                    Point<float> getRotationCenter();
                    double getCurrentAngle();
                    double getCurrentPosInSeconds();
                    
          private:
                    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Dex);
                    std::unique_ptr<MadexGUI> guiInterface; //A variable to be used in case the deck needs to perform some action on the interface that holds it
                    DrawableImage deckControl; //Holds the image for the component
                    bool playing {false}; //a flag to control whether the deck is rotating or not 
                    Point<float> initPos; // A JUCE point object to hold the center of rotation
                    double angle {0.0}; // a variable to hold the current angle 

                    // 120 updates to complete a circle / 60 updates per second half a circle / 6 seconds per circle * 60 updates per second = 360 updates to complete a circle =  6.2832 rads(360 deg)/360(updates) = 0.017453 rads per update (or 0.017453 * seconds) = total updates in time specified 

                    // Variables to calculate rotation etc
                    double trackLengthSeconds{0.0};
                    double trackLengthRads{0.0};
                    double currentPosInSeconds {0.0};
                    double currentPosInRads{0.0};
                    double angularVelocity{0.017453333};
                    double radsPerSecond {6.2832 / 6};
};
#endif