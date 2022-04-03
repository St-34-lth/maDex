#include <JuceHeader.h>
#include <iostream>
#include <memory>

using namespace std;
#pragma once

#ifndef STATEMANAGER_H
#define STATEMANAGER_H

class State;
class PlayerState;


class PlayerStateManager;
//*A StateManager interface to implement various actions depending on its current state and context
// Takes in a new state pointer as its initial state transition to it
class StateManager
{
          public:
                    StateManager(State *state);
                    StateManager(){};
                    ~StateManager();
                    StateManager(StateManager const &) = delete;
                    StateManager(StateManager &&) = delete;
                    StateManager &operator=(StateManager const &) = delete;
                    StateManager &operator=(StateManager &&) = delete;
                    virtual void transitionTo(State *state) = 0; //a pure virtual function to be implemented by inheritting classes - transitions the player to whatever state it is passed 
          
          protected:
                    State *state_; //holds the current state the manager is located in
                   
};

// Implements the StateManager interface and provides specific actions for the MadexPlayer states 
class PlayerStateManager: public StateManager
{
          public:
                    PlayerStateManager(PlayerState *_playerState);
                    ~PlayerStateManager(){};
                    void transitionTo(State *state){};
                    void transitionTo(PlayerState *_playerState);
                    
          protected:
                    PlayerState *playerState;
                    virtual void playPressed() = 0;
                    virtual void loadPressed(juce::URL &url) = 0;
                    virtual void cuePressed() = 0;
                    virtual void stopPressed() = 0;                  
};

#endif
