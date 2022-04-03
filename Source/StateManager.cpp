#include "StateManager.h"
#include "States.h"


// *Base StateManager implementation

StateManager::StateManager(State *state):state_(nullptr) {  };

StateManager::~StateManager() { delete this->state_; };

void StateManager::transitionTo(State *state)
{
          std::cout << "StateManager: Transition to " << typeid(*state).name() << ".\n";
          
          if(this->state_!=nullptr)
          {
                    delete this->state_;
          }
          this->state_ = state;
          this->state_->set_context(this);
};


//* PlayerStateManager class implementation

PlayerStateManager::PlayerStateManager(PlayerState *_playerState) : playerState(nullptr),StateManager() { PlayerStateManager::transitionTo(_playerState); };

// Takes in a new PlayerState 
// Transitions from the current state to the passed state
void PlayerStateManager::transitionTo(PlayerState *_playerState)
{
          std::cout << "player: Transition to " << typeid(*_playerState).name() << ".\n";

          if (this->playerState != nullptr)
          {
                    delete this->playerState;
          }
          this->playerState = _playerState;
          this->playerState->setContextToPlayer(this); 
}


