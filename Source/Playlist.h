/*
  ==============================================================================

    Playlist.h
    Created: 31 Jan 2022 4:27:08pm
    Author:  solo

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <string>
#include "PlaylistData.h"
#include <memory>

using namespace juce; 
//==============================================================================
/*
*/
// A component to hold playlist functionality. Receives an ID to distinguish itself from other playlists where necessary
class Playlist  :   
                    public Component,
                    public TableListBoxModel,
                    public FileDragAndDropTarget,
                    public ActionBroadcaster
{
          public:
                    
                    Playlist(int guiID);
                    Playlist();
                    ~Playlist() override;

                    //*JUCE component implementations
                    void paint (juce::Graphics&) override;
                    void resized() override;
                    void paintRowBackground(Graphics &,
                                                  int rowNumber,
                                                  int width, int height,
                                                  bool rowIsSelected) override;
                    void paintCell(Graphics &,
                                        int rowNumber,
                                        int columnId,
                                        int width, int height,
                                        bool rowIsSelected) override; 
                    int getNumRows() override;
                    void cellClicked(int rowNumber,int columnId,const MouseEvent&)override;
                    void cellDoubleClicked(int rowNumber, int columnId, const MouseEvent &e)override;
                    //* File drag and drop listener implementation 
                    bool isInterestedInFileDrag(const StringArray& files) override;
                    void filesDropped (const StringArray& files, int x, int y) override; 
                    
                    
                    //*Playlist functionality
                    void savePlaylist(const File &playlistpath);
                    void loadPlaylist(const File &playlistpath);
                    void makeNewPlaylist();
                    void addTrackByURL(const String &trackFilepath);
                    void findTrack(String searchInput);
                    
          private:
                    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Playlist)
                    
                    //* Data members
                    std::unique_ptr<AudioFormatManager> formatManager; //a format manager to use when information about tracks is required etc
                    std::unique_ptr<PlaylistData> playlistData; //an instance of the custom made PlaylistData class, used as a data holder instead of a vector

                    String filter = ""; // a variable to hold typed text by the user from the playlist searchbox
                    TableListBox tableComponent {{}, this}; //The tablelistbox component required by the tablelistboxmodel implementation
                    
                    std::shared_ptr<PopupMenu> cellMenu; // a smart pointer to hold the popup menu options and functionality callbacks of the playlist component

};        
