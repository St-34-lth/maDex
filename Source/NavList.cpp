#include "NavList.h"

//==============================================================================
NavList::NavList()
{
          setOpaque(true);
          addAndMakeVisible(navList);
          navList.setModel(this);
          navList.setColour(ListBox::backgroundColourId,Colours::antiquewhite);
          navList.setRowHeight(50);
}

NavList::~NavList()
{
}

void NavList::paint(Graphics &g)
{
          
          g.fillAll(Colours::antiquewhite);
}

void NavList::resized()
{
          navList.setBounds(getLocalBounds());
          auto bounds = getLocalBounds();
          for (auto btn : navButtons)
          {
                    btn->setBounds(bounds.removeFromTop(50).reduced(5));
          };
}

void NavList::paintListBoxItem(int rowNumber, Graphics &g, int width, int height, bool rowIsSelected)
{
          if (rowNumber < 0 || rowNumber >= getNumRows())
                    return;

          Rectangle<int> bounds(0, 0, width, height);
          auto textColour = findColour(Label::textColourId);
          g.setColour(textColour.withAlpha(0.4f));

          if (rowNumber == 0)
                    g.fillRect(bounds.removeFromTop(5).reduced(10, 0));

          g.fillRect(bounds.removeFromBottom(50).reduced(5, 0));

}

void NavList::addEntry(const String &entry,Button& entryBtn)
{
          navList.deselectAllRows();
          navEntries.add(entry);
          navButtons.push_back(&entryBtn);

          addAndMakeVisible(&entryBtn);
          
          navList.updateContent();
}
void NavList::addComponent(TextButton* &btn)
{
          
          
}
int NavList::getNumRows()
{
         
          return navButtons.size();
}

void NavList::selectedRowsChanged(int row)
{
          if (row < 0)
                    return;
          entryCallback(row);
          navList.deselectAllRows();
}

// Links an entry in the listboxmodel to a custom made callback function                    
void NavList::setEntrySelectionCallback(std::function<void(int)> callback)
{
          entryCallback = callback;
}
