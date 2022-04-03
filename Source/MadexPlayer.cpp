#include "MadexPlayer.h"
#include <memory>

MadexPlayer::MadexPlayer(AudioFormatManager &_formatManager, PlayerState* playerstate): 
          formatManager(_formatManager), PlayerStateManager(playerstate)
{

};

MadexPlayer::~MadexPlayer()
{

};

// Prepares the audio sources and adds them to the player's mixer
void MadexPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) 
{
          transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
          resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
          
          mixerSource.prepareToPlay(samplesPerBlockExpected,sampleRate);
          mixerSource.addInputSource(&resampleSource,false);
};

void MadexPlayer::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) { mixerSource.getNextAudioBlock(bufferToFill);};

void MadexPlayer::releaseResources()
{
          transportSource.releaseResources();
          resampleSource.releaseResources();
          mixerSource.releaseResources();
};

// Takes in a URL and tries to load it onto the player
// Displays an alert window if something goes wrong 
void MadexPlayer::loadURL(URL audioURL)
{
          
          if (transportSource.isPlaying())
          { 
                    stopPressed();
          };
           
          try{
                    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(
                    URL::InputStreamOptions(URL::ParameterHandling::inAddress)));

                  
                        // if the sampleReader was not pointed to a nullptr
                    if (reader != nullptr)
                    {
                              // make a smart pointer readerSource for the specific format
                              std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
                              // set the new readerSource as the transported source
                              transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
                              
                              // the new readerSource is then released and assigned to the class scoped reader source pointer, which is itself a unique ptr so both eventually get deleted at the end of this function
                              readerSource.reset(newSource.release());

                              // keep a copy of the filepath playing
                              loadedTrack = audioURL.getLocalFile().getFullPathName();

                    }
                    else
                    {
                              AlertWindow::showMessageBox(MessageBoxIconType::WarningIcon,"Warning","Something went wrong loading the file");
                              
                    };
          }
          catch (exception&e)
          {
                    std::cerr<<e.what() << '\n';
          }
          
};

// A setter to set the player's gain value
// Checks for proper values before setting
void MadexPlayer::setGain(double gain)
{
        
          if (gain>1)
          {
                    gain = 1;
          }
          if(gain <=0.01)
          {
                    gain = 0;
          }
          transportSource.setGain(gain);
          
};

//Similar to the setGain function, but sets the gain relative to the ratio passed in instead of an absolute value
// Checks for proper values before setting
void MadexPlayer::setGainRelative(double gainRatio)
{
          
          if (gainRatio > 1)
          {
                    gainRatio = 1;
          }
          if (gainRatio <= 0.01)
          {
                    gainRatio = 0;
          }

          double gain = getGain()* gainRatio;
          setGain(gain);
          
          
          
}

// Sets player playback speed
void MadexPlayer::setSpeed(double ratio) { resampleSource.setResamplingRatio(ratio); };

// Sets player playback direction
void MadexPlayer::setDirection(int direction)
{
          playDirection=direction;
}

// Sets player playback position relative to the playing track
// Checks for proper values
void MadexPlayer::setPositionRelative(double pos)
{
          
          if(pos < 0.0 || pos > 1.0)
          {
                  return;  
          }
          else 
          {
                    double posInSecs = transportSource.getLengthInSeconds() * pos;
                    setPosition(posInSecs);
          }
};

// Sets player playback position of the playing track
void MadexPlayer::setPosition(double posInSecs) 
{
          if (posInSecs > 0.001 && posInSecs < transportSource.getTotalLength())
          {
                   
                    transportSource.setPosition(posInSecs);
          }
          else 
          {
                    return;
          }
};

// Cue functionality
//  If no cue marker exists, sets a cue marker at the current track position
// If a cue marker exists, sets the player playback position to that position and resets the marker
void MadexPlayer::setCue()
{
          try
          {
                    if (currentPos != -1)
                    {

                              setPosition(currentPos);
                              resetCue();
                              return;
                    };
                    currentPos = transportSource.getCurrentPosition();
          }
          catch(std::exception& e)
          {
                    std::cerr<<e.what() << '\n';
          };
          
};

// Simple reseter for cue functionality 
void MadexPlayer::resetCue() { currentPos = -1; };

// Starts the player playback
void MadexPlayer::start() { transportSource.start(); };

// Pauses the player playback
void MadexPlayer::pause() { transportSource.stop(); }

// Stops the player playback - Resets the position back to start
void MadexPlayer::stop()
{
          transportSource.stop();
          setPosition(0.1);
};

// Creates samples up to 10 seconds on the fly using player playback as source
// Samples are created sequentially 1-4. If a sample exists in a container, the sample will be placed onto the next container
// If the samples container is full 
void MadexPlayer::makeSample()
{
          try
          {
                    if (sampling)
                    {

                              for (size_t i = 0; i < samplesContainer.size(); i++)
                              {

                                        if (samplesContainer[i]->start != -1 && samplesContainer[i]->end == -1)
                                        {
                                                  samplesContainer[i]->end = transportSource.getCurrentPosition();
                                                  if (!verifySample(samplesContainer[i]))
                                                  {
                                                            samplesContainer[i]->end = samplesContainer[i]->start + 10;
                                                            
                                                  };

                                                  sampling = false;
                                                  
                                                  break;
                                        }
                              };
                              return;
                    };

                    for (size_t i = 0; i < samplesContainer.size(); i++)
                    {
                              if (samplesContainer[i].use_count() == 0)
                              {
                                        samplesContainer[i] = make_shared<playerSample>();
                              };

                              if (samplesContainer[i]->start == -1 && samplesContainer[i]->end == -1)
                              {

                                        samplesContainer[i]->track = URL{File{loadedTrack}};
                                        samplesContainer[i]->start = transportSource.getCurrentPosition();
                                        samplesContainer[i]->speed = resampleSource.getResamplingRatio();
                                        sampling = true;
                                        
                                        break;
                              };
                    };
          }
          catch(std::exception& e)
          {
                    std::cerr << e.what() << '\n';
          };

          return;
};

// Deletes an existing sample from the samples container
// Checks if the sample is a valid one
// If so, it deletes it
// If not, displays a window to inform the user about its non-existence
void MadexPlayer::delSample(int& id)
{
          try 
          {         
                    if(samplesContainer[id].use_count() ==0 || (samplesContainer[id]->start == -1 || samplesContainer[id]->end == -1 ))
                    {
                              AlertWindow::showOkCancelBox(MessageBoxIconType::InfoIcon,"Warning","No samples to delete","","");
                    }
                    samplesContainer[id] = std::make_shared<playerSample>();
                    
          }
          catch(std::exception& e)
          {
                    std::cerr << e.what() << '\n';
          }
};

// Helper function that verifies samples before creating
// Takes in a pointer to a sample, checks its length and returns true if the sample is valid, false otherwise
bool MadexPlayer::verifySample(std::shared_ptr<playerSample> sample)
{
          
          if (sample.use_count() == 0)
          {
                    return false;
          }
          
          if ((sample->end -  sample->start) < 10.001)
          {
                    
                    return true;
          };

          return false;
};

// Getter to return player gain 
double MadexPlayer::getGain() { return transportSource.getGain(); };

//*PlayerStateManager implementation

// Getter to return player's relative playback  position 
double MadexPlayer::getPositionRelative() 
{
          return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds(); 
};

// Getter to return player's absolute playback position
double MadexPlayer::getPositionAbsolute()
{
          return transportSource.getCurrentPosition();
};

// Getter to return the player's loaded track length
double MadexPlayer::getLoadedTrackLength()
{
          return transportSource.getLengthInSeconds();
};

// Getter to return the player's available formats
String MadexPlayer::getAvailableFormats() { return formatManager.getWildcardForAllFormats(); };

// Getter that returns a smart pointer to a sample
// Takes in a sampleID (range 1 to 4)
std::shared_ptr<MadexPlayer::playerSample> MadexPlayer::getSample(int sampleID)
{
          
          std::shared_ptr<playerSample> sample;
          try
          {
                    if (sampleID > samplesContainer.size())
                    {
                              cerr << "Unable to access sample" << '\n';

                    };
                    if (samplesContainer[sampleID].use_count() == 0)
                    {
                              DBG("empty sample returned");
                              return sample;
                    }
                    else
                    {
                              sample = samplesContainer[sampleID];
                    }
                   
          }
          catch(std::exception& e)
          {
                    std::cerr<<e.what() << '\n';
          }
          
          return sample;
          
          
}

// Helper function that returns a bool indicating whether the playback is playing or not
bool MadexPlayer::isPlaying()
{
          return transportSource.isPlaying();
}

// action listener implementation
// Used to communicate between components where necessary
void MadexPlayer::actionListenerCallback(const String &message) 
{
       
}

//* player states
// Actions are taken according to the state the player is in -> see the state class implementation and header files for more details
void MadexPlayer::loadPressed(URL& url) { this->playerState->loadPressed(url); };

// PLayerState implementation for stopPressed function
void MadexPlayer::stopPressed() { this->playerState->stopPressed(); };

// PLayerState implementation for playPressed function
void MadexPlayer::playPressed() { this->playerState->playPressed(); };

// PLayerState implementation for cuePressed  function
void MadexPlayer::cuePressed() { this->playerState->cuePressed();  };

// PLayerState implementation for samplePressed function
void MadexPlayer::samplePressed() { this->playerState->samplePressed(); };