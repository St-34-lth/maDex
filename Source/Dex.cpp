/*
  ==============================================================================

    Dex.cpp
    Created: 3 Mar 2022 1:56:22pm
    Author:  m

  ==============================================================================
*/


#include "Dex.h"

//==============================================================================
Dex::Dex()
{
          // Init
          setFramesPerSecond(60);
}

Dex::~Dex()
{

}
// JUCE component functionality
// Draws thebackground of the component, the image as well as the text that shows the current position
void Dex::paint (juce::Graphics& g)
{
          
          g.fillAll(Colours::antiquewhite);
          
              // animations
          g.setColour(Colours::whitesmoke);
          auto img = deckControl.getImage();
          g.drawImageTransformed(img,AffineTransform::rotation(angle,img.getBounds().getCentreX(),img.getBounds().getCentreY()));
          g.setColour(Colours::black);
          g.setFont(14);
          g.drawFittedText("Pos: " + std::to_string(+(int)(currentPosInSeconds)) + " sec", img.getBounds().expanded(0, g.getCurrentFont().getHeight()), Justification::centredBottom, 2);
}
// Starts/pauses the rotation of the component
void Dex::start() 
{ 
          if(playing == true)
          {
                    playing=false;
          }
          else playing = true;
          
};

// Stops the rotation of the component and sets the angle to 0
void Dex::stop() 
{ 
          playing = false ;
          angle = 0.0d;
};

// updates the component - keeps the centre of rotation stable 
void Dex::update()
{
          initPos.setXY(deckControl.getBounds().getCentreX(), deckControl.getBounds().getCentreY());
};

void Dex::resized()
{  
          deckControl.setBounds(deckControl.getImage().getBounds());
}


// Takes in a string representing the file path of an image
// sets the animation image to the component 
void Dex::setAnimationImage(String &animationImage)
{
          deckControl.setImage(ImageFileFormat::loadFrom(File{animationImage}));
};

// Takes in a pos in seconds and updates the component's currentPosInseconds 
void Dex::setPositionAbs(double &pos)
{
          if (currentPosInSeconds != pos)
          {
                    currentPosInSeconds = pos;
                    update();
                    repaint();
          };
};

// Takes in a position in seconds and translates it to rads, updates the component's posinrads variable
void Dex::setPosInRads(double& pos)
{
          if (currentPosInSeconds != pos)
          {
                    auto posInRads = pos * radsPerSecond;
                    currentPosInRads = posInRads;
                    currentPosInSeconds = pos;
                    angle = currentPosInRads;
                    update();
                    repaint();
          };
};

// takes in a double representing the total length in seconds of loaded track, updates the required variables of the component to animate it correctly
void Dex::setTrackLength(const double &lengthInSeconds)
{
          angle = 0.0;
          trackLengthSeconds = lengthInSeconds;
          trackLengthRads = lengthInSeconds * radsPerSecond;
          currentPosInRads =0.0;
          currentPosInSeconds = 0.0;
         
          update();
          repaint();
};

// Takes in an int representing the direction of rotation
// Adds a constant angle to the rotation of the component and updates it 
void Dex::addAngle(int& direction)
{

        
          if (direction == 0) //forwards
          {
                    if (currentPosInSeconds < trackLengthSeconds)
                    {
                              currentPosInSeconds += angularVelocity / radsPerSecond;
                              currentPosInRads += angularVelocity;
                              update();
                              repaint();
                    };
          };

          if (direction == 1)  //backwards
          {
                    
                    if (currentPosInSeconds > 0)
                    {
                              currentPosInSeconds -= angularVelocity / radsPerSecond;
                              currentPosInRads -= angularVelocity;
                              update();
                              repaint();
                    };
                    
          };
          
          
};

// A simple getter that returns the component's currentPosInSeconds variable
double Dex::getCurrentPosInSeconds()
{
          return currentPosInSeconds;
}

// A simple getter that returns the component's current angle variable
double Dex::getCurrentAngle()
{
          return angle;
}
// A simple getter that returns the component's centre of rotation variable
Point<float> Dex::getRotationCenter()
{
          return initPos;
};

