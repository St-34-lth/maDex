
#include "Playlist.h"
using namespace juce;

//==============================================================================

Playlist::Playlist(int guiID) : playlistData(std::move(new PlaylistData{guiID}))
{
           
          //  Init  
                    // setup the playlist popup menu 
          cellMenu = std::make_shared<PopupMenu>();
          cellMenu->addItem(1, "Play");
          cellMenu->addItem(2, "Load");
          cellMenu->addItem(3, "Delete");
          
          // Visibility
          addAndMakeVisible(tableComponent);
          
          // Settings
                    // Set up the columns for the playlist and their settings
          tableComponent.getHeader().addColumn("ID", 1, 70, 30);
          tableComponent.getHeader().addColumn("Title",2,70,30); 
          tableComponent.getHeader().addColumn("Length", 3, 70, 30);  
          tableComponent.getHeader().setStretchToFitActive(true);
          tableComponent.setColour(TableListBox::backgroundColourId,Colours::antiquewhite);
          tableComponent.setTooltip("Drag and drop to add songs\nDouble click to play a song\nRight click on a song for options");
          
          // Listeners
          addMouseListener(this, true);
          
}

Playlist::~Playlist()
{
          
}

// JUCE component functionality
void Playlist::paint (juce::Graphics& g)
{
 
}

void Playlist::resized()
{
          tableComponent.setBounds(0,0,getWidth(),getHeight());
          auto trackColumnWidth = tableComponent.getHeader().getWidth() / 2;
          auto otherColumnsWidth = (tableComponent.getHeader().getWidth() - tableComponent.getHeader().getColumnWidth(2) ) / 2 ;
          // size up the columns accordingly
          tableComponent.getHeader().setColumnWidth(1, otherColumnsWidth);
          tableComponent.getHeader().setColumnWidth(2, trackColumnWidth);
          tableComponent.getHeader().setColumnWidth(3, otherColumnsWidth);
         
}
// JUCE paint functionality - comments inside
void Playlist::paintRowBackground(Graphics & g,
                                int rowNumber,
                                int width, int height,
                                bool rowIsSelected)
{         
          auto trackFile = File { playlistData->getTrack(rowNumber).getLocalFile() }; //a File instance to get track details on the fly
          auto trackName = trackFile.getFileNameWithoutExtension(); 
          // Checks whether a string has been typed in the searchbox and filters the visible row/tracks accordingly
          if (trackName.contains(filter) || filter == "" || filter == "Enter your search")
          {
                    if(getParentComponent()->getComponentID() == "1") //different backgrounds for each deck/playlist
                    {
                              if(rowIsSelected)
                              {
                                        g.fillAll(Colours::pink);
                              }
                              else 
                              {
                                        g.fillAll(Colours::purple);
                              }
                    }
                    if (getParentComponent()->getComponentID() == "0")
                    {
                              if (rowIsSelected)
                              {
                                        g.fillAll(Colours::teal);
                              }
                              else
                              {
                                        g.fillAll(Colours::blue);
                              }
                    }
          }
}
// JUCE paint functionality - comments inside
void Playlist::paintCell(Graphics & g,
                         int rowNumber,
                         int columnId,
                         int width, int height,
                         bool rowIsSelected)
{
          // Local variables to get info on the tracks inside the playlist
          File trackFile;
          String length;
          std::unique_ptr<AudioFormatReader> reader; 
          formatManager = std::make_unique<AudioFormatManager>();
          formatManager->registerBasicFormats();
          
          try      
          {
                    trackFile =  File{ playlistData->getTrack(rowNumber).getLocalFile() };
                    reader.reset(formatManager->createReaderFor(trackFile));
          }
          catch(const std::exception& e)
          {
                    DBG(e.what());
          }
          
          auto trackName = trackFile.getFileNameWithoutExtension();
          if (trackName.contains(filter) || filter == "" || filter=="Enter your search") // Similarly to what's happening inside the JUCE paintrow function, this will avoid overkill drawings if not the tracks should not be actually displayed
          {
                    switch(columnId)
                    {
                              // ID
                              case 1:
                                        g.drawText(String{rowNumber}, 2, 0, width - 4, height, Justification::centredLeft);

                                        break;
                              //Title
                              case 2:
                                        g.setColour(Colours::yellow);
                                        g.drawText(trackFile.getFileNameWithoutExtension(), 2, 0, width - 4, height, Justification::centredLeft, true);

                                        break;
                                        
                              //Length   
                              case 3:
                                        if (reader)
                                        {
                                                  double minutes = ((reader->lengthInSamples / reader->sampleRate) / 60);
                                                  double seconds = (minutes - floor(minutes)) * 60;

                                                  String min{floor(minutes)};
                                                  String secs{round(seconds)};
                                                  length += min;
                                                  length += ":";
                                                  length += secs;
                                        }
                                        else
                                        {
                                                  length = "";
                                        }
                                        g.drawText(length, 2, 0, width - 4, height, Justification::centredLeft, true);

                                        break;
                    };
                   
          };
}

// JUCE tablelistbox implementation
int Playlist::getNumRows()
{
          return playlistData->getSizeOf();
          tableComponent.updateContent();
}

//  Takes in a string with a trackFilepath and loads it into the playlistData instance
// This can be used also in cases where a track is not dragged&dropped on the playlist component itself but anywhere else on the GUI
void Playlist::addTrackByURL(const String &trackFilepath)
{

          try
          {
                    URL track{File{trackFilepath}};
                    playlistData->addTrackByURL(track);
                    tableComponent.updateContent();
          }
          catch (std::exception &e)
          {
                    DBG("Something went wrong with adding the track");
                    DBG(e.what());
          };
};

// Listeners

// file drag/drop listeners
bool Playlist::isInterestedInFileDrag(const StringArray &files)
{
          std::cout << "file on table" << '\n';
          return true;
}

// Adds the dropped tracks in the playlistData instance 
// Called if track(s) are dropped on the playlist component itself rather than anywhere else on the GUI  
// Simply uses the addTrackByURL function to add each track dropped
void Playlist::filesDropped(const StringArray &files, int x, int y)
{
          
          for (auto file : files)
          {
                    URL track = URL{File{file}} ;
                    playlistData->addTrackByURL(track);          
          };
          tableComponent.updateContent();
          tableComponent.repaintRow(getNumRows()-1);
}

// Listens for mouse clicks on playlist cells
// Right clicks display a menu
void Playlist::cellClicked(int rowNumber,int columnId,const MouseEvent& e)
{
         
          if(e.mods.isRightButtonDown())
          {
                    
                    cellMenu->showMenuAsync(PopupMenu::Options(),[=](int result)
                    {
                              
                              if (result==1)
                              {
                                        URL trackURL = playlistData->getTrack(rowNumber);
                                        String message{"play,"};
                                        message+= trackURL.getLocalFile().getFullPathName();
                                        sendActionMessage(message);
                                        
                              }
                              if (result == 2)
                              {
                                        URL trackURL = playlistData->getTrack(rowNumber);
                                        String message{"load,"};
                                        message += trackURL.getLocalFile().getFullPathName();
                                        sendActionMessage(message);
                              };

                              if (result == 3)
                              {
                                        playlistData->deleteTrack(rowNumber);
                                        tableComponent.updateContent();
                                        
                              };                              
       
                    });
          };
          
};

// Listens for double clicks on playlist cells
// Double clicks on a track play it
// Selected track data is sent through a JUCE action message to the player. The player loads the track and plays it
void Playlist::cellDoubleClicked(int rowNumber,int columnId,const MouseEvent& e)
{
          URL trackURL = playlistData->getTrack(rowNumber);
          String message{"play,"};
          message += trackURL.getLocalFile().getFullPathName();
          sendActionMessage(message);
}

// playlist utilities

// Takes in a String as search input
// calls the playlistdata  instance's searchTracks
// Updates the tableComponent to display results if any
void Playlist::findTrack(String searchInput)
{
          playlistData->searchTracks(searchInput);
          tableComponent.updateContent();
};

// Calls the playlistData instance's makeNewPlaylist functionality
// Updates the tableComponent to display the new empty playlist
void Playlist::makeNewPlaylist()
{
          playlistData->makeNewPlaylist();
          tableComponent.updateContent();
};

// Calls the playlistData instance's savePlaylist functionality
void Playlist::savePlaylist(const File& playlistpath)
{
          playlistData->savePlaylist(playlistpath);
};

// Calls the playlistData instance's loadPlaylist functionality
// Updates the tableComponent to display the new empty playlist
void Playlist::loadPlaylist(const File &playlistpath)
{
          playlistData->loadPlaylist(playlistpath);
          tableComponent.updateContent();
};
