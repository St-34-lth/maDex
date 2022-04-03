/*
  ==============================================================================

    PlaylistData.cpp
    Created: 3 Feb 2022 3:02:54pm
    Author:  solo

  ==============================================================================
*/

#include "PlaylistData.h"

PlaylistData::PlaylistData(int id)
{
          // Init
          cachedPath += "./" ;
          cachedPath+= id;
          cachedPath+= "cachedPlaylist.json"; //the path to be used for the cached playlist
          playlistId= id ;
          cachedPlaylist.reset(new File{cachedPath});
          loadedPlaylist.reset(new File{});

          // checks if the cachedPlaylist exists and loads it if so
          if(cachedPlaylist->existsAsFile())
          {
                   
                    loadPlaylist(*cachedPlaylist);
                   
          }
          else // otherwise it creates a new cachedPlaylist at the local path of the app
          {
                    cachedPlaylist->create();
          };
};

PlaylistData::~PlaylistData()
{
          
};

// takes in a trackID
// returns a URL holding the track from the playlist 
URL PlaylistData::getTrack(int trackId)
{
          URL track; 
          try
          {
                    track = visibleTracks[trackId];
                    return track;
          }
          catch(std::exception& e)
          {
                    std::cerr << e.what();
          }
          return track ;
          
}

// A getter that simply returns the size of the visible tracks vector
int PlaylistData::getSizeOf()
{

          return visibleTracks.size();
}

// Takes in a JUCE url instance holding a track 
// adds the track to both visibletrack and loadedtrack vectors
// saves the playlist at the cached path provided
void PlaylistData::addTrackByURL(URL track)
{
          visibleTracks.push_back(track);
          loadedTracks.push_back(track);
          savePlaylist(*cachedPlaylist,loadedTracks);         
};

// Takes in the rowNumber of the track to be deleted
// Removes the track from both visible and loadedtracks vectors
// Saves the changes to the cachedPath
void PlaylistData::deleteTrack(int rowNum)
{
         
          try
          {
                    visibleTracks.erase(visibleTracks.begin() + rowNum);
                    loadedTracks.erase(loadedTracks.begin() + rowNum);
                    savePlaylist(*cachedPlaylist,loadedTracks);
          }
          catch(const std::exception& e)
          {
                    DBG(e.what());
          };
         
};
// Provides the search functionality
// Takes in a string with the search data
// Searchs the loadedtracks and changes the visibletracks vector to display results 
// Fills up the searchresults vector with the tracks that contain the search input
// Swaps the searchresults with the visibletracks vector to display results
// A blank searchInput fills the visible tracks with all loaded tracks
void PlaylistData::searchTracks(String& searchInput)
{
       
          std::string searchFor = searchInput.toStdString();

          if (searchInput != "")
          {

                    for (size_t i = 0; i < loadedTracks.size(); i++)
                    {

                              auto searchIn = loadedTracks[i].getLocalFile().getFileNameWithoutExtension();

                              if (searchIn.contains(searchFor))
                              {

                                        std::cerr << loadedTracks[i].getFileName() << '\n';
                                        searchResults.push_back(loadedTracks[i]);
                              };
                    };

                    
                    visibleTracks.swap(searchResults);
                    searchResults.clear();
          }

          else
          {
                    visibleTracks.clear();
                    searchResults.clear();
                    std::vector<URL> tempContainer {loadedTracks};
                    visibleTracks.swap(loadedTracks);
                    loadedTracks.swap(tempContainer);
          }
};

// Clears the loaded and visible tracks vectors
// saves(overwrites) the new blank playlist at the cached path 
// saves to a JSON file
void PlaylistData::makeNewPlaylist()
{
          visibleTracks.clear();
          loadedTracks.clear();
          loadedPlaylist.reset(new File {""});
          savePlaylist(*cachedPlaylist,loadedTracks);
          
}

//Takes in a JUCE file instance representing a filepath 
//Loads the playlist from the filepath
// The filepath must be a JSON file
// Displays an alert window if loading fails
// Loading a playlist will not remove the previously loaded tracks, it will add them on top instead
// Explicit saving is required by the user to save the new modified playlist 
void PlaylistData::loadPlaylist(const File& filepath)
{
          
          
          FileInputStream stream{filepath};

          // if the stream opens
          if (stream.openedOk())
          {
                    try
                    {
                              // if the stream is not empty.
                              if (stream.isExhausted())
                              {
                                        return;
                              };


                              //  load the playlist into a var object
                              auto jsonVAR = JSON::parse(stream);

                                                           //  iterate over the results and populate
                              for (int i = 0; i < jsonVAR.getArray()->size(); i++)
                              {

                                        auto track = jsonVAR.getArray()->getReference(i);
                                        auto trackLoc = File{track.toString()};
                                       
                                        if (trackLoc.existsAsFile())
                                        {
                                                  URL trackURL = URL{File{track.toString()}};
                                                  loadedTracks.push_back(trackURL);
                                                  visibleTracks.push_back(trackURL);
                                        }
                                        else
                                        {
                                                  continue;
                                        };
                              };
                    }
                    catch (const std::exception &e)
                    {
                              std::cerr<< e.what() << '\n';
                    };
          }
          else
          {
                    AlertWindow::showOkCancelBox(MessageBoxIconType::WarningIcon,"Error","Playlist loading failed.");
          };

        
}

// Takes in a file instance representing a playlistpath and a vector of juce URLs representing the tracks to be saved in the playlist
// saves the playlist at the given path
void PlaylistData::savePlaylist(const File& playlistpath,std::vector<URL>& urls)
{
          
          var playlistTracks{};
          FileOutputStream stream{playlistpath};

          // the loadedTracks vector must be populated with something in order to save its contents to a playlist
          if (!urls.empty())
          {
                    for (auto const &track : urls) // iterate over its contents andappend string paths to a juce::var object
                    {
                              if (!track.isEmpty())
                              {
                                        playlistTracks.append(track.getLocalFile().getFullPathName());
                              }
                              else
                              {
                                        continue;
                              }
                    };
                    try // write the results to the playlist json file
                    {
                             
                              if (stream.openedOk())
                              {
                                        stream.setPosition(0);
                                        stream.truncate();

                                        JSON::writeToStream(stream, playlistTracks, false, 0);
                              };
                    }
                    catch(std::exception & e)
                    {
                              std::cerr<< e.what() << '\n';
                    }

                    
          }

          else // if the contents of the passed track vector was empty, we write an empty string to the playlist json file
          {
                    
                    playlistTracks.append("");
                    stream.setPosition(0);
                    stream.truncate();
                    JSON::writeToStream(stream, playlistTracks,false,0);
                   
                    return;
          }
          
          return;
};

// A simple wrapper for calling saveplaylist on the current loadedtracks;
void PlaylistData::savePlaylist(const File &playlistpath) 
{
          savePlaylist(playlistpath, loadedTracks);
          
};
