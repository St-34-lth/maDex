#include "States.h"
#include "StateManager.h"

void State::set_context(StateManager *context)
{
          cout << "set CONTEXT: " << typeid(*context_).name() << endl;
          this->context_ = context;
};

// *Player states implementation

PlayerState::PlayerState(): State() {}

// Takes in a PlayerStateManager instance and casts it to a MadexPlayer type so it can perform actions on it as required
void PlayerState::setContextToPlayer(PlayerStateManager *_playerContext)
{
          this->playerContext = _playerContext;
          this->player = static_cast<MadexPlayer *>(this->playerContext);
};

// *Playing state functionality 

// Actions to do when the play button is pressed
void PlayingState::playPressed()
{
          this->player->pause();
          this->player->transitionTo(new PausedState{});
}

// Actions to do when the stop button is pressed
void PlayingState::stopPressed()
{
          this->player->stop();
          this->player->transitionTo(new StoppedState{});
}
// Actions to do when the load button is pressed
void PlayingState::loadPressed(URL& url)
{
          this->player->stop();
          this->player->loadURL(url);
          this->player->resetCue();
          this->player->start();
}

void PlayingState::cuePressed() { this->player->setCue(); }

void PlayingState::samplePressed() { this->player->makeSample(); };

// * PausedState functionality
void PausedState::stopPressed()
{
          
          this->player->stop();
          this->player->transitionTo(new StoppedState{});
};

void PausedState::playPressed()
{
          
          this->player->start();
          this->player->transitionTo(new PlayingState{});
}

void PausedState::loadPressed(URL &url)
{
          this->player->stop();
          this->player->loadURL(url);
          this->player->transitionTo(new StoppedState{});
}

void PausedState::cuePressed()
{
          this->player->setCue();
}

    // * Stopped state functionality

void StoppedState::playPressed()
{
          this->player->start();
          this->player->transitionTo(new PlayingState{});
}

void StoppedState::stopPressed() { DBG("no action for stop at stopped state "); }

void StoppedState::loadPressed(URL& url) { this->player->loadURL(url);DBG("success"); }

void StoppedState::cuePressed()
{
          this->player->resetCue();
};
