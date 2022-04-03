
#pragma once
#include "StateManager.h"
#include "MadexPlayer.h"
#include <JuceHeader.h>
using namespace std;
#include <iostream>
#ifndef STATES_H
#define STATES_H
class MadexPlayer;

// A base class to provide an interface for implementing specific MadexPlayer states 
class State
{
          public:
                    State(){};
                    ~State(){};
                    State(State const&)=delete;
                    State(State &&)=delete;
                    State& operator=(State const&)=delete;
                    State& operator=(State &&)=delete;

                    virtual void set_context(StateManager *context); //A pure virtual function to set the context everytime a state transitions

          protected:
                    StateManager *context_; // holds the current context in which the states have meaning
};

// The PlayerState class is an implementation of the State class
// It holds a PlayerStateManager object as one of its members which allows it to use polymoprhism and downcast or upcast and call its actions as required (The MadexPlayer inherits and implements the PlayerStateManager)
// Each state below inherits from the PlayerState and so implements its methods
class PlayerState: public State
{
          public:

                    PlayerState();
                    void setContextToPlayer(PlayerStateManager *_playerContext); 

                    // MadexPlayer state actions to be implemented by inheritting states
                    virtual void cuePressed()=0;
                    virtual void stopPressed()=0;
                    virtual void playPressed() = 0 ;
                    virtual void loadPressed(juce::URL& url) = 0;
                    virtual void samplePressed() = 0;

          protected:
                    PlayerStateManager* playerContext; // This is basically how the state can perform actions on the MadexPlayer
                    MadexPlayer *player;
                    
                    
};

class StoppedState:public PlayerState
{
          public:
                    using PlayerState::PlayerState;
                    void cuePressed();
                    void stopPressed();
                    void playPressed();
                    void loadPressed(juce::URL& url);
                    void samplePressed(){};
};

class PlayingState : public PlayerState
{
          public:
                    using PlayerState::PlayerState;
                    void cuePressed();
                    void stopPressed();
                    void playPressed();
                    void loadPressed(juce::URL &url);
                    void samplePressed();
};

class PausedState: public PlayerState
{
          public:
                    using PlayerState::PlayerState;
                    void cuePressed();
                    void stopPressed();
                    void playPressed();
                    void loadPressed(juce::URL &url);
                    void samplePressed(){};
};


#endif
