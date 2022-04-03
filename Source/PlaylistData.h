/*
  ==============================================================================

    PlaylistData.h
    Created: 3 Feb 2022 3:02:54pm
    Author:  solo

  ==============================================================================
*/
#include <JuceHeader.h>
using namespace juce;
#pragma once
#include <algorithm>     
#include <functional>
#include <vector>
#include <memory>
#include <string>

// A data class to encapsulate the playlist's data functionality. Takes in an int hold as an ID if necessary to distinguish itself from other instances. 
class PlaylistData
{
          public: 
                    
                    PlaylistData();
                    PlaylistData(int id);
                    ~PlaylistData();
                    PlaylistData(PlaylistData const&) = delete; 
                    PlaylistData(PlaylistData &&) = delete;
                    PlaylistData& operator=(PlaylistData const&)= delete;
                    PlaylistData& operator=(PlaylistData &&)=delete;
                    
                    // Track functionality
                    void deleteTrack(int rowNum);
                    void searchTracks(String& searchInput, std::vector<URL> urlContainer);
                    void searchTracks(String& searchInput);
                    URL getTrack(int songId);
                    void addTrackByURL(URL track);

                    // List functionality
                    void makeNewPlaylist();
                    void savePlaylist(const File &playlistpath);
                    void savePlaylist(const File &playlistpath,std::vector<URL>& urls);
                    void loadPlaylist(const File& filepath);
                    
                    // Helpers
                    int getSizeOf();

          private:
                    int playlistId;

                    String cachedPath {""}; //A string to hold the cache location path
                    std::unique_ptr<File> cachedPlaylist; //A smart pointer to a juce File instance that holds a cachedPlaylist location
                    std::unique_ptr<File> loadedPlaylist; // A smart pointer to a juce File instance that holds the loadedPlaylist location
                    
                    // Vectors to hold tracks accordingly 
                    std::vector<URL> searchResults {}; //holds tracks that appear in search results 
                    std::vector<URL> visibleTracks {}; //holds visible tracks
                    std::vector<URL> loadedTracks{}; //holds loadedTracks
                    std::vector<URL> temp{}; // a temporary vector used in the class
                    
};