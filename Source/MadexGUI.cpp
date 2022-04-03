/*
  ==============================================================================

    wmpgui.cpp
    Created: 28 Jan 2022 6:45:33pm
    Author:  solo

  ==============================================================================
*/


#include "MadexGUI.h"


//==============================================================================
MadexGUI::MadexGUI( MadexPlayer* _player, 
                    AudioFormatManager &_formatManagerToUse, 
                    AudioThumbnailCache &_thumbCacheToUse,
                    Sampler* _sampler):         
                    player(move(_player)),
                    sampler(move(_sampler)),
                    display(_formatManagerToUse,_thumbCacheToUse),
                    showNavBar("Navbar")
                      
{          
          // Init
          id = MadexGUI::guiID;
          MadexGUI::guiID = +1;
          deck = std::make_shared<Dex>();
          
          playlist.reset(new Playlist(id));
          // positions the sidepanel to the left or right according to the GUI id
          if (id == 1)
          {
                    sidePanel.reset(new SidePanel("",250,false,nullptr,false));
          }
          else
          {
                    sidePanel.reset(new SidePanel("", 250, true, nullptr, false));
          }
          auto navBarPanel = new NavBar("Side Panel");
          auto navBarList = new NavList;

          // visibility
                    // components
          addAndMakeVisible(trackDisplay);
         
          addAndMakeVisible(searchBox);
          addAndMakeVisible(*playlist);
          addAndMakeVisible(display);
          addAndMakeVisible(deck.get());
                    // buttons
          addAndMakeVisible(playButton);
          addAndMakeVisible(stopButton);
          addAndMakeVisible(sampleButton);
          addAndMakeVisible(cueButton);
                    // sliders
          addAndMakeVisible(gainSlider);
          addAndMakeVisible(tempo);
                    // sidepanel
          addAndMakeVisible(showNavBar);
          addAndMakeVisible(navBarStatus);
          addAndMakeVisible(*sidePanel);

          // settings
          startTimer(50);
          setComponentID(String{id});
          setSize(600, 400);
                    // components
          player->setGain(0);
          deck->setSize(100, 100);
          navBarList->addEntry("New", newPlaylist);
          navBarList->addEntry("Load",loadPlaylist);
          navBarList->addEntry("Save",savePlaylist);
          
          sidePanel->setContent(navBarList);
          sidePanel->setTitleBarComponent(navBarPanel, true);
          sidePanel->setColour(SidePanel::ColourIds::dismissButtonNormalColour, Colours::black);
          

          
          navBarStatus.setText("status", NotificationType::dontSendNotification);          
          trackDisplay.setBounds(getLocalBounds());
          trackDisplay.setColour(Label::backgroundColourId,Colours::antiquewhite);
          trackDisplay.setColour(Label::textColourId,Colours::black);
          trackDisplay.setJustificationType(Justification::centred);
          
          searchBox.setBounds(getLocalBounds());
          searchBox.setColour(TextEditor::ColourIds::backgroundColourId,Colours::antiquewhite);
          searchBox.setColour(TextEditor::ColourIds::textColourId,Colours::black);
          searchBox.setTextToShowWhenEmpty("Type to find a song in the playlist",Colours::green);
          searchBox.setMultiLine(false,true);
          
                    // buttons
          playButton.setToggleable(true);
          playButton.setTooltip("Plays a loaded song");
          stopButton.setTooltip("Stops a playing song");
          cueButton.setTooltip("Places a cue mark on a playing song.\nPress again to move at the mark");
          sampleButton.setTooltip("Samples a playing song. Press once to start, twice to stop.\nDefaults to a limit of 10 seconds");
          newPlaylist.setTooltip("Clears the current playlist.\nCreates a new persistent list");
          savePlaylist.setTooltip("Saves the current playlist to a file of choice");
          loadPlaylist.setTooltip("Loads a playlist from a file of choice");
          showNavBar.setTooltip("Displays playlist options");
          
          
          deck->setTooltip("Acts as a playhead\nRotate anticlockwise to move backwards\n clockwise to move forwards");
          showNavBar.setColour(TextButton::ColourIds::buttonColourId,Colours::antiquewhite);
          
                    // sliders
          gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
          gainSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
          gainSlider.setRange(0, 1);
          gainSlider.setTooltip("Sets the volume of the deck");

          tempo.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
          tempo.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 40, 15);
          tempo.setColour(Slider::ColourIds::textBoxTextColourId, Colours::black);
          tempo.setDoubleClickReturnValue(true, 1.0);
          tempo.setRange(0, 2);
          tempo.setValue(1, NotificationType::dontSendNotification);
          tempo.setTooltip("Sets the playing speed of the song");
          
         
                    // Images
          auto pausebtnImg = Drawable::createFromImageFile(File{"/home/m/Desktop/OOP/madex/assets/icons8-pause-24.png"});
          auto playbtnImg = Drawable::createFromImageFile(File{"/home/m/Desktop/OOP/madex/assets/icons8-play-96.png"});
          auto stopbtnImg = Drawable::createFromImageFile(File{"/home/m/Desktop/OOP/madex/assets/icons8-stop-96.png"});
          auto cuebtnImg = Drawable::createFromImageFile(File{"/home/m/Desktop/OOP/madex/assets/icons8-synchronize-96.png"});
          auto samplebtnIMg = Drawable::createFromImageFile(File{"/home/m/Desktop/OOP/madex/assets/icons8-color-dropper-96.png"});
          auto deckImg = String{"/home/m/Desktop/OOP/madex/assets/icons8-music-record-100.png"};
          
          playButton.setImages(playbtnImg.get(), nullptr, nullptr, nullptr, pausebtnImg.get());
          playButton.setColour(DrawableButton::ColourIds::backgroundOnColourId, Colours::antiquewhite);
          savePlaylist.setImages(Drawable::createFromImageFile(File{"/home/m/Desktop/OOP/madex/assets/icons8-save-96.png"}).get());
          newPlaylist.setImages(Drawable::createFromImageFile(File{"/home/m/Desktop/OOP/madex/assets/icons8-plus-math-96.png"}).get());
          loadPlaylist.setImages(Drawable::createFromImageFile(File{"/home/m/Desktop/OOP/madex/assets/icons8-edit-96.png"}).get());
          stopButton.setImages(stopbtnImg.get());
          cueButton.setImages(cuebtnImg.get());
          sampleButton.setImages(samplebtnIMg.get());
          deck->setAnimationImage(deckImg);

          // Listeners
                    // components
          searchBox.onTextChange = [this] { findTrack(); };
          deck->addMouseListener(this, true);
          display.addMouseListener(this, true);
          player->addActionListener(this);
          playlist->addActionListener(this);
          playlist->addMouseListener(this,true);
          
                    // sliders
          tempo.addListener(this);
          gainSlider.addListener(this);
                    // buttons
          playButton.addListener(this);
          stopButton.addListener(this);
          cueButton.addListener(this);
          sampleButton.addListener(this);
          newPlaylist.addListener(this);
          loadPlaylist.addListener(this);
          savePlaylist.addListener(this);
          showNavBar.addListener(this);

          // sampling & delete buttons
          for (int i = 0; i < 4; i++)
          {


                    // visibility
                    addAndMakeVisible(sampleButtons[i]);
                    addAndMakeVisible(deleteSampleButtons[i]);
                    // listeners
                    sampleButtons[i]->addListener(this);
                    deleteSampleButtons[i]->addListener(this);

                    //tooltips
                    sampleButtons[i]->setTooltip("Plays the registered sample.\nUse the 'colordrop' button to create one.");
                    deleteSampleButtons[i]->setTooltip("Deletes the corresponding registered sample.");
                    
                    // settings
                    sampleButtons[i]->setComponentID(String{std::to_string(i)});
                    sampleButtons[i]->setToggleable(true);
                    sampleButtons[i]->setToggleState(false, NotificationType::dontSendNotification);
                    sampleButtons[i]->setColour(TextButton::ColourIds::buttonColourId, Colours::antiquewhite);
                    sampleButtons[i]->setColour(TextButton::ColourIds::textColourOffId, Colours::black);
                    sampleButtons[i]->setColour(TextButton::ColourIds::textColourOnId, Colours::black);
                    deleteSampleButtons[i]->setComponentID(String{std::to_string(i)});
                    deleteSampleButtons[i]->setColour(TextButton::ColourIds::buttonColourId, Colours::antiquewhite);
                    deleteSampleButtons[i]->setColour(TextButton::ColourIds::textColourOffId, Colours::black);
                    deleteSampleButtons[i]->setColour(TextButton::ColourIds::textColourOnId, Colours::black);

                    if (id == 0)
                    {
                              sampleButtons[i]->setColour(TextButton::ColourIds::buttonOnColourId, Colours::blue);
                    }
                    else
                    {
                              sampleButtons[i]->setColour(TextButton::ColourIds::buttonOnColourId, Colours::purple);
                    };
          };
};

MadexGUI::~MadexGUI()
{
          sampleButtons.clear();
          sampleButtons.shrink_to_fit();
          stopTimer();
          deleteAllChildren();
}

// Draws graphics on component and screen
void MadexGUI::paint (juce::Graphics& g)
{
   
          g.setFont (14.0f);
          g.setColour(Colours::purple);          
          g.drawText(trackDisplay.getText(), trackDisplay.getLocalBounds(),Justification::centred, true); //Simply draws the current track name 

};

// sets bounds for component parts etc
// Each 'area' variable uses JUCE rectangles to assign boundaries for the components in it( buttons,sliders,etc)

// what about magic numbers ?
void MadexGUI::resized()
{

          double topBorder = getHeight()/15;
          double bottomBorder = getHeight()-topBorder;
          double leftBorder = getWidth() / 12;
          double rightBorder = getWidth() - leftBorder;
          double buttonHeight = bottomBorder / 10;
          double buttonWidth = rightBorder / 5;
          double sliderHeight = buttonHeight;


          // Bounds
          auto area = getLocalBounds();
          auto playlistAreaSize = area.getHeight() / 3;


          // Sizes
          auto headerFooterheight = 20;
          auto trackDisplayHeight = buttonHeight * 0.8;

          auto playlistBounds = area.removeFromTop(jmax((double)100, (double)playlistAreaSize));
          if (id == 0)
          {
                    showNavBar.setBounds(playlistBounds.removeFromLeft(buttonWidth / 4).removeFromTop(buttonHeight * 3 + (buttonHeight / 2)));
          };
          if(id==1)
          {
                    showNavBar.setBounds(playlistBounds.removeFromRight(buttonWidth / 4).removeFromTop(buttonHeight * 3 + (buttonHeight / 2)));
          };
         
          playlist->setBounds(playlistBounds.removeFromTop(buttonHeight * 3));
          searchBox.setBounds(playlistBounds.removeFromTop(buttonHeight / 2));

          // Display bounds 

          auto displayBounds = area.removeFromTop(area.getHeight() / 5);

          trackDisplay.setBounds(displayBounds.removeFromTop(trackDisplayHeight * 0.5));
          display.setBounds(displayBounds.removeFromTop(buttonHeight));

          
          // Button bounds
          
          auto buttonArea1 = area.removeFromTop(jmax((double)100, (double)area.getHeight() / 4));
          auto btnArea1Width = buttonArea1.getWidth() / 3;
          
          playButton.setBounds(buttonArea1.removeFromLeft(btnArea1Width).removeFromTop(buttonHeight).reduced(5));
          stopButton.setBounds(buttonArea1.removeFromLeft(btnArea1Width).removeFromTop(buttonHeight).reduced(5));
          cueButton.setBounds(buttonArea1.removeFromLeft(btnArea1Width).removeFromTop(buttonHeight).reduced(5));
          
          // Deck area
          
          auto deckArea = area.removeFromTop(jmax((double)100, (double)area.getHeight() / 2));
          auto deckAreaButtonWidth = (deckArea.getWidth() / 3 ) ;
          deck->setBounds(deckArea.removeFromLeft(deckAreaButtonWidth).reduced(15));
          
          
          // Slider bounds
          
          gainSlider.setBounds(deckArea.removeFromLeft(deckAreaButtonWidth).reduced(15));
          tempo.setBounds(deckArea.removeFromLeft(deckAreaButtonWidth).reduced(15));
          auto buttonArea2 = area.removeFromTop(jmax((double)100, (double)area.getHeight() / 4));
          auto btnArea2Width = buttonWidth / 1.5;

          sampleButton.setBounds(buttonArea2.removeFromLeft(btnArea1Width).removeFromTop(buttonHeight).reduced(5));

          for (TextButton *sampleBtn : sampleButtons)
          {
                    sampleBtn->setBounds(buttonArea2.removeFromLeft(btnArea2Width).removeFromTop(buttonHeight).reduced(5));
                    
          };

          auto buttonArea3 = area.removeFromTop(jmax((double)100, (double)area.getHeight() / 4));
          buttonArea3.removeFromLeft(btnArea1Width);
        
          for (auto delSampleBtn : deleteSampleButtons)
          {
                    delSampleBtn->setBounds(buttonArea3.removeFromLeft(btnArea2Width).removeFromTop(buttonHeight / 2).reduced(5));
          };
}
                              
                              // *  Utilities *//

// Calls the playlist component's find track function - 
//used as the searchbox's callback to filter the visible tracks when the user types
void MadexGUI::findTrack() { playlist->findTrack(searchBox.getText()); };

//Getter that simply returns the gain slider's value when called 
double MadexGUI::getGainSliderValue() { return gainSlider.getValue(); };

//Getter that simply returns the current track's absolute position
double MadexGUI::getPlayerPositionAbsolute() { return player->getPositionAbsolute(); };
                             
                              //* Listeners *//
// Listens for all buttons in the GUI
// Calls corresponding actions for each button
void MadexGUI::buttonClicked(Button* button)
{
         
          if(button == & newPlaylist)
          {
                    playlist->makeNewPlaylist();
          }
          if (button == &loadPlaylist)
          {
                    
                    FileChooser chooser{"Load playlist", initDirectory, "*.json,*.txt", true, false};
                    if (chooser.browseForFileToOpen())
                    {
                              playlist->loadPlaylist(chooser.getResult());
                    };
          }
          if (button == &savePlaylist)
          {
                    FileChooser chooser{"Save playlist", initDirectory, "*.json", true, false};
                    if (chooser.browseForFileToSave(true))
                    {
                              playlist->savePlaylist(chooser.getResult());
                    };
          }

          if (button == &playButton)
          { 
                    
                    player->playPressed();
                    if(playButton.getToggleState() == false)
                    {
                              // playButton.setToggleState(true, NotificationType::dontSendNotification);
                    }
                    playButton.setToggleState(!playButton.getToggleState(), NotificationType::dontSendNotification);
          };

          if (button == &stopButton)
          {
                    player->stopPressed();
                    playButton.setToggleState(false, NotificationType::dontSendNotification);
                    
          };

          if(button==&cueButton)
          {
                    
                    player->cuePressed();
                    
                    display.setCueMarker(player->getPositionRelative());
          }
          
          // Sample button functionality
          // initiates sampling 'state' in the player
          // Connects the delete functionality to the sample made when complete
          // Also changes the color of the delete button to give feedback to the user that a sample exists
          if (button == &sampleButton)
          {
                    player->samplePressed();
                    for(int i = 0; i < 4; i++)
                    {
                              if (player->verifySample(player->getSample(i)))
                              {
                                        deleteSampleButtons[i]->setColour(TextButton::ColourIds::buttonColourId, Colours::black);
                                        deleteSampleButtons[i]->setColour(TextButton::ColourIds::textColourOffId, Colours::yellow);
                                        String btnTxt{"del"};
                                        deleteSampleButtons[i]->setButtonText(btnTxt);
                              }
                    }
                    
          }
          // Listens for one of the sample buttons being pressed 
          // Plays/Stops the stored sample
          for (size_t i = 0; i < sampleButtons.size(); i++)
          {
                    if (button == sampleButtons[i])
                    {

                              if (sampleButtons[i]->getToggleState() == false)
                              {
                                        
                                        sampleButtons[i]->setToggleState(!sampleButtons[i]->getToggleState(), NotificationType::dontSendNotification);
                                        try
                                        {
                                                  sampler->loadSample(player->getSample(i));
                                                  sampler->start();
                                        }
                                        catch(std::exception&e)
                                        {
                                                  std::cerr<<e.what() << '\n';
                                        }
                                        break;

                              }
                              sampleButtons[i]->setToggleState(!sampleButtons[i]->getToggleState(), NotificationType::dontSendNotification);
                              sampler->stop();
                    };
          };
          
          // Listens for one of the delete buttons being pressed
          // Plays the stored sample
          for (int i = 0; i < deleteSampleButtons.size(); i++)
          {
                    if(button == deleteSampleButtons[i])
                    {          
                              player->delSample(i);
                              deleteSampleButtons[i]->setColour(TextButton::ColourIds::buttonColourId, Colours::antiquewhite);
                              deleteSampleButtons[i]->setColour(TextButton::ColourIds::textColourOffId, Colours::antiquewhite);
                              String btnTxt{""};
                              deleteSampleButtons[i]->setButtonText(btnTxt);
                    }
          };

          if(button==&showNavBar)
          {
                    sidePanel->showOrHide(true);
          }
};

// Required implementation for button listener inheritance
void MadexGUI::buttonStateChanged(Button *button)
{
          
};


// Listens for gain and tempo sliders
// Acts accordingly
void MadexGUI::sliderValueChanged(Slider* slider)
{

          if (slider == &gainSlider)
          {
                    player->setGain(slider->getValue());
                    sampler->setGain(player->getGain());
          }
         
          if (slider == &tempo)
          {
                   player->setSpeed(slider->getValue());

          }
};
// File drag and drop listener implementation
bool MadexGUI::isInterestedInFileDrag(const StringArray &files)
{
          return true ;
};
// Listens for drag and drops over the GUI
// Adds the track to the playlist if a single one is dragged & dropped anywhere outside the playlist component 
void MadexGUI::filesDropped(const StringArray &files, int x, int y)
{
          
          if (files.size() ==1)
          {
                    String trackFilepath{files[0]};
                    playlist->addTrackByURL(trackFilepath);
                    
          }
          
}

                    //* Callbacks *//


void MadexGUI::timerCallback()
{
         
          display.setPositionRelative(player->getPositionRelative());
          if (!settingDeck)
          {
                    double pos = player->getPositionAbsolute();
                    deck->setPosInRads(pos);
          };
          
}

// action callback listener expects messages from the playlist instance of the GUI
// a play message is received everytime a song is right clicked and the load/play option is selected
// the received message format is a string, seperated by commas
// a tokenizer class is used to parse the message. Each token is then checked and corresponding actions are taken on the player
void MadexGUI::actionListenerCallback(const String &message)
{
          const regex re(R"([,]+)"); // regular expression to seperate commas, required by the tokenizer 

          list<std::string> msgTokens = Tokenizer::tokenizeMessage(message.toStdString(), re); // a list of tokens returned by the tokenizer class 
          
          if (msgTokens.front() == "play") //in case the user selected play - corresponding actions are taken to load the song and play it 
          {
                    msgTokens.pop_front();
                    File pathToLoadTrack{String{msgTokens.front()}};
                    URL trackToLoad = URL{pathToLoadTrack};
                    
                    trackDisplay.setText(trackToLoad.getLocalFile().getFileNameWithoutExtension(), NotificationType::dontSendNotification);
                    player->loadURL(URL{pathToLoadTrack});
                    display.loadURL(URL{pathToLoadTrack});
                    deck->setTrackLength(player->getLoadedTrackLength());
                    player->playPressed();
          };

          if (msgTokens.front() == "load")
          {
                    msgTokens.pop_front();
                    File pathToLoadTrack{String{msgTokens.front()}};
                    URL trackToLoad = URL{pathToLoadTrack};

                    trackDisplay.setText(trackToLoad.getLocalFile().getFileNameWithoutExtension(), NotificationType::dontSendNotification);
                    player->loadURL(URL{pathToLoadTrack});
                    display.loadURL(URL{pathToLoadTrack});
                    deck->setTrackLength(player->getLoadedTrackLength());
          };
};

// mouse down listener - events bubble up from the waveform display and deck components
// WaveFormDisplay - checks whether a click occurred and sets the track to a position relative to the track if within range 
// checks if the mousedown event occurred on the deck component for less than some defined time - if so starts the playback 
void MadexGUI::mouseDown(const MouseEvent &e)
{
          if(e.eventComponent==&display) 
          {
                    if (e.mouseWasClicked())
                    {
                              
                              player->setPositionRelative(display.getCoordinatesOnDisplay(e.getMouseDownX()));
                              double pos = player->getPositionAbsolute();
                              deck->setPositionAbs(pos);
                              
                    };
          };

          if (e.eventComponent == deck.get()) 
          {
                    if (e.mods.isLeftButtonDown() && e.getLengthOfMousePress() < 250)
                    {
                              
                              if (!player->isPlaying())
                              {
                                        player->playPressed(); 
                              };

                              
                    };
          }
};

// mouse drag listener - events bubble up from the deck component
// If there's a mouse dragging on the deck component, gets mouse position and calculates angle relative to the component 
// Sets the track pos accordingly 
void MadexGUI::mouseDrag(const MouseEvent &e)
{
          int dir {};
          if (e.eventComponent == deck.get())
          {
                    settingDeck = true;

                    auto nextAngle = deck->getRotationCenter().getAngleToPoint(e.getMouseDownPosition().toFloat());
                    if (nextAngle > 0) 
                    {
                              dir =0;
                              deck->addAngle(dir);
                    };
                    if (nextAngle < 0 )
                    {
                              dir=1;
                              deck->addAngle(dir);   
                    };

                    player->setPosition(deck->getCurrentPosInSeconds());
                    settingDeck = false;
                    return;
          };
          
}
// simple getter that returns the GUI's id member
int MadexGUI::getGuiId()
{
          return id;
};