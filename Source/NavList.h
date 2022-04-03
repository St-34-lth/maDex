/*
  ==============================================================================

    NavList.h
    Created: 18 Feb 2022 1:42:47pm
    Author:  m

  ==============================================================================
*/

#pragma once

// #include "/home/m/Desktop/OOP/webmusicplayer/JuceLibraryCode/JuceHeader.h"
#include <JuceHeader.h>
using namespace juce;
//==============================================================================
/*
*/
// * A class to hold the contents of the sidepanel. it basically implements a JUCE ListBoxModel object to hold buttons and functionality in a table like manner
class NavList  : 
                    public Component,
                    public ListBoxModel
{
          public:   
                    NavList();
                    ~NavList() override;

                    // JUCE component functionality
                    void paint (juce::Graphics&) override;
                    void resized() override;
                    void paintListBoxItem(int rowNumber, Graphics &g, int width, int height, bool rowIsSelected) override;
                    int getNumRows() override;
                    void selectedRowsChanged(int row) override;
                    
                    // Custom functionality
                    void addEntry(const String &entry, Button& entryBtn); 
                    void setEntrySelectionCallback(std::function<void(int)> callback);
                    void addComponent(TextButton* &btn);

          private:
                    ListBox navList; // a listbox object 
                    StringArray navEntries; // an array of strings to hold entries in the listboxmodel
                    std::function<void(int)> entryCallback; // declares the callback expected and its arguments.
                    std::vector <Button*> navButtons {}; //a vector that holds the buttons used in the listbox
                    
                    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NavList)
};
