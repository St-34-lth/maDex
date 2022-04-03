#include "Sampler.h"

Sampler::Sampler(AudioFormatManager &_formatManager, PlayerState *_playerstate):
                              formatManager(_formatManager),
                              PlayerStateManager(_playerstate)
{
          
};

Sampler::~Sampler(){};

// JUCE AudioSource implementation
void Sampler::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{ 
          transportSource.prepareToPlay(samplesPerBlockExpected,sampleRate); 
          resampleSource.prepareToPlay(samplesPerBlockExpected,sampleRate);
                  
          mixerSource.prepareToPlay(samplesPerBlockExpected,sampleRate);
          mixerSource.addInputSource(&resampleSource, false);
          
};

//This function is modified to loop over specific sections of the loaded track
// The sections are provided by the currentSample variable 
void Sampler::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) 
{
          if ( transportSource.getCurrentPosition() >= currentSample.start 
                    && transportSource.getCurrentPosition()  <= currentSample.end)
          {
                   
                    // transportSource.getNextAudioBlock(bufferToFill);
                    mixerSource.getNextAudioBlock(bufferToFill);
          }
          else 
          {
                   
                    transportSource.setPosition(currentSample.start);
                    
          };
}

void Sampler::releaseResources() 
{ 
          transportSource.releaseResources(); 
          resampleSource.releaseResources();
          mixerSource.releaseResources();          
};

// Simply setter to control gain
void Sampler::setGain(double gain) { transportSource.setGain(gain); };

// Similar to the main MadexPlayer functionalities
void Sampler::start() 
{ 
          if(!currentSample.track.isEmpty())                     
          {
                    transportSource.start(); 
          }
          else 
          {
                    return ;
          }
};

void Sampler::stop() { transportSource.stop();  };

void Sampler::pause() {};

// Takes in a playerSample struct that holds details of the sample to be loaded
//  checks if the sample is valid and assigns it to the currentSample variable
//  otherwise displays an alert message 
void Sampler::loadSample(std::shared_ptr<MadexPlayer::playerSample> sample)
{
          currentSample = *sample.get();
          loadedSampleURL = currentSample.track;
          if (!loadedSampleURL.isEmpty())
          {
                    try
                    {
                              auto *reader = formatManager.createReaderFor(loadedSampleURL.createInputStream(
                                  URL::InputStreamOptions(URL::ParameterHandling::inAddress)));
                              // if the sampleReader was not pointed to a nullptr
                              if (reader != nullptr)
                              {
                                        // make a smart pointer readerSource for the specific format
                                        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, false));
                                        // set the new readerSource as the transported source
                                        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
                                        readerSource.reset(newSource.release());
                                        // keep a copy of the filepath playing
                                        loadedSampleURL = currentSample.track.getLocalFile().getFullPathName();
                                        resampleSource.setResamplingRatio(sample->speed);
                                        DBG("reader ok");
                              }
                              else
                              {
                                        AlertWindow::showMessageBox(MessageBoxIconType::WarningIcon, "Warning", "Something went wrong loading the file");
                              };
                              return; 
                    }
                    catch (exception &e)
                    {
                              std::cerr << e.what() << '\n';
                    };
          }
          else 
          {          
                    AlertWindow::showMessageBox(MessageBoxIconType::WarningIcon,"Warning","No sample available");
                    
          }
          
};

