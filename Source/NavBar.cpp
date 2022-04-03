#include "NavBar.h"

//==============================================================================
NavBar::NavBar(const String& navTitle)
{
          // Optimizes drawing the component  
          setOpaque(true);
          
          // settings
          navBarLabel.setText(navTitle, NotificationType::dontSendNotification);
          navBarLabel.setColour(Label::ColourIds::backgroundColourId,Colours::black);
          
          // visibility
          addAndMakeVisible(navBarLabel);
}

NavBar::~NavBar()
{
}
//*  JUCE component functionalities
void NavBar::paint (juce::Graphics& g)
{
          
          g.fillAll(Colours::black);
          auto normal = getLookAndFeel().findColour(SidePanel::dismissButtonNormalColour);
          auto over = getLookAndFeel().findColour(SidePanel::dismissButtonOverColour);
          auto down = getLookAndFeel().findColour(SidePanel::dismissButtonDownColour);
          
          navHomeBtn.setColours(normal, over, down);
          navSettingsBtn.setColours(normal, over, down);

}

void NavBar::resized()
{
   
          auto bounds = getLocalBounds();
          auto cid1 = navHomeBtn.getComponentID();

          bounds.removeFromLeft(10);
          navHomeBtn.setBounds(bounds.removeFromLeft(navHomeBtn.getWidth() + 20).reduced(7));
          navSettingsBtn.setBounds(bounds.removeFromLeft(navSettingsBtn.getWidth() + 20).reduced(7));
          bounds.removeFromLeft(10);
          bounds.removeFromRight(10);
          navBarLabel.setBounds(bounds);
}
// Callback for a home button implementation 
// Takes in a std::function and sets it as a member of the navbar
// The member can the be called from outside of the component - in other words this is a custom callback
void NavBar::setHomeButtonClicked(std::function<void()> callback)
{
          navHomeBtn.onClick = callback;
}

// Callback for a settings button implementation
// Takes in a std::function and sets it as a member of the navbar
// The member can the be called from outside of the component - in other words this is a custom callback
void NavBar::setSettingButtonClicked(std::function<void()> callback)
{
          navSettingsBtn.onClick = callback;
}
