#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
          //Init 
          setSize (960, 800);
          tooltip = make_shared<TooltipWindow>(this, 2500);
          formatManager.registerBasicFormats();
          samplerFormatManager.registerBasicFormats();

          if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
          && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
          {
                    juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                                  [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
          }
          else
          {
                    setAudioChannels (0, 2);
          }

          // Settings
          mixer.setTooltip("A slider to control the deck audio ratio\nSlide left to decrease right playback and vice versa");
          mixer.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
          mixer.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox,true,0,0);
          mixer.setRange(-1, 1);

          // Visibility
          addAndMakeVisible(guiL);
          addAndMakeVisible(guiR);
          addAndMakeVisible(mixer);
          
          // Listeners
          guiL.addActionListener(this);
          guiR.addActionListener(this);
          mixer.addListener(this);

         
};

// shut down audio resources 
MainComponent::~MainComponent()
{
    
          shutdownAudio();
};

//==============================================================================
// Add all deck/sampler inputs to the mixer, and prepare it for playing
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
          
          mixerSource.addInputSource(&playerL,false);
          mixerSource.addInputSource(&playerR,false);
          mixerSource.addInputSource(&samplerL, false);
          mixerSource.addInputSource(&samplerR, false);
          mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
         
          // Simply setter for deck players' gains, sets the same value to the sampler components
          if (playerRgain != playerR.getGain())
          {
                    playerRgain = playerR.getGain();
                    samplerR.setGain(playerRgain);
          };
                    

          if (playerLgain != playerL.getGain())
          {
                    playerLgain = playerL.getGain();
                    samplerL.setGain(playerLgain);
          };

          mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
          
          playerL.releaseResources();
          playerR.releaseResources();
          samplerL.releaseResources();
          samplerR.releaseResources();
          mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    
          auto backColor = juce::Colours::antiquewhite;
          backColor.withAlpha(0.0f);
          g.fillAll(backColor);
    
}

void MainComponent::resized()
{
          auto area = getLocalBounds();
          auto mixerWidth= 100;
          guiL.setBounds(area.removeFromLeft(getWidth() / 2-mixerWidth/2));
          
          auto displayArea = area.removeFromLeft(mixerWidth);
          mixer.setBounds(displayArea.removeFromTop(20));
          guiR.setBounds(area.removeFromLeft(getWidth() / 2 - mixerWidth / 2));
          
};

void MainComponent::actionListenerCallback(const String &message)
{
          
          
}

// sliderValue listener 
void MainComponent::sliderValueChanged(Slider* slider)
{
          // Listens for the deck ratio playback mixer and adjusts each deck's gain accordingly          
          if(slider==&mixer)
          {
                    auto mixValue = mixer.getValue();
                    auto threshold = 0.01;
                    if (mixValue > threshold) //Moving to the right 
                    {
                              auto newGainVal = 1- mixValue;
                              if (newGainVal > playerLgain)
                              {
                                        auto prevGainL = guiL.getGainSliderValue();
                                       

                                        if (prevGainL > newGainVal)
                                        {
                                                  playerL.setGain(newGainVal);
                                        };
                                        return;
                              };


                              if (newGainVal < playerLgain)
                              {

                                        playerL.setGain(newGainVal);
                                        return ;
                              };
                              
                                                         
                    }
                    if (mixValue < threshold) //Moving to the left
                    {
                              auto newGainVal = 1 + mixValue;
                              if (newGainVal > playerRgain)
                              {
                                        auto prevGainR = guiR.getGainSliderValue();

                                        if (prevGainR > newGainVal)
                                        {
                                                  playerR.setGain(newGainVal);
                                        };
                                        return;

                              }
                              if (newGainVal < playerRgain)
                              {
                                        playerR.setGain(newGainVal);
                                        return;
                              };
                    }
                  
          }
}