/* -*-c++-*-
 * testAPP - Using 'The MIT License'
 * Copyright (C) 2014, Caper Holdings LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This software was developed by Alion Science and Technology Corporation under
 * circumstances in which the U. S. Government may have rights in the software.
 */

////////////////////////////////////////////////////////////////////////////////
// INCLUDE DIRECTIVES
////////////////////////////////////////////////////////////////////////////////
#include "guiscreen.h"
#include <dtUtil/log.h>

#ifdef None
#undef None
#endif
#include <CEGUI/CEGUI.h>


namespace dtExample
{
   //////////////////////////////////////////////////////////////////////////
   // CLASS CODE
   //////////////////////////////////////////////////////////////////////////
   const std::string GuiScreen::DEFAULT_NAME("dtExample::GuiScreen");

   //////////////////////////////////////////////////////////////////////////
   GuiScreen::GuiScreen(dtGUI::GUI& guiScene, const std::string& name, const std::string& layoutFile )
      : BaseClass()
      , mName(name)
      , mLayoutFile(layoutFile)
      , mGUIScene(&guiScene)
   {
   }

   //////////////////////////////////////////////////////////////////////////
   GuiScreen::~GuiScreen()
   {
   }

   //////////////////////////////////////////////////////////////////////////
   GuiNode* GuiScreen::GetRoot()
   {
      return mRoot;
   }

   //////////////////////////////////////////////////////////////////////////
   const GuiNode* GuiScreen::GetRoot() const
   {
      return mRoot;
   }

   //////////////////////////////////////////////////////////////////////////
   void GuiScreen::SetName(const std::string& name)
   {
      mName = name;
   }

   //////////////////////////////////////////////////////////////////////////
   const std::string& GuiScreen::GetName() const
   {
      return mName;
   }

   //////////////////////////////////////////////////////////////////////////
   const std::string& GuiScreen::GetLayoutFileName() const
   {
      return mLayoutFile;
   }

   //////////////////////////////////////////////////////////////////////////
   void GuiScreen::SetVisible( bool visible )
   {
      if( mRoot != NULL )
      {
         mRoot->setVisible( visible );
      }
   }

   //////////////////////////////////////////////////////////////////////////
   bool GuiScreen::IsVisible() const
   {
      return mRoot != NULL && mRoot->isVisible();
   }

   //////////////////////////////////////////////////////////////////////////
   void GuiScreen::Setup(GuiNode* parent)
   {
      // Load the layout for this screen, which contains all its widgets.
      // Note that this call will automatically add the screen to the
      // GUI's root sheet.
      mRoot = mGUIScene->LoadLayout(mLayoutFile);

      if (mRoot == NULL)
      {
         LOG_ERROR("Could not load layout \"" + mLayoutFile
            + "\" for GuiScreen \"" + GetName() + "\"");
      }

      // If another parent was specified, re-attach this screen's root
      // to the specified parent.
      if (parent != NULL && mRoot != NULL)
      {
         SetParent(parent);
      }
   }

   //////////////////////////////////////////////////////////////////////////
   void GuiScreen::SetParent(GuiNode* parent)
   {
      GuiNode* currentParent = mRoot->getParent();

      // Only change attachment if the specified parent is
      // different from the current parent.
      if (currentParent != parent)
      {
         if (currentParent != NULL)
         {
            currentParent->removeChildWindow(mRoot);
         }

         if (parent != NULL)
         {
            parent->addChildWindow(mRoot);
         }
      }
   }

   //////////////////////////////////////////////////////////////////////////
   void GuiScreen::OnEnter()
   {
      // DEBUG:
      LOG_ALWAYS("\tScreen entering: " + GetName());

      SetVisible(true);
   }

   //////////////////////////////////////////////////////////////////////////
   void GuiScreen::OnExit()
   {
      // DEBUG:
      LOG_ALWAYS("\tScreen exiting: " + GetName());

      SetVisible(false);
   }

   //////////////////////////////////////////////////////////////////////////
   void GuiScreen::Update(float timeDelta)
   {
      // OVERRIDE for custom behavior.
   }

   //////////////////////////////////////////////////////////////////////////
   GuiNode* GuiScreen::CreateText(const std::string& name, GuiNode* parent, const std::string& text,
                                    float x, float y, float width, float height)
   {
      // create base window and set our default attribs
      CEGUI::Window* result = mGUIScene->CreateWidget(parent, "WindowsLook/StaticText", name);
      result->setText(text);
      result->setPosition(CEGUI::UVector2(cegui_absdim(x), cegui_absdim(y)));
      result->setSize(CEGUI::UVector2(cegui_absdim(width), cegui_absdim(height)));
      result->setProperty("FrameEnabled", "false");
      result->setProperty("BackgroundEnabled", "false");
      result->setHorizontalAlignment(CEGUI::HA_LEFT);
      result->setVerticalAlignment(CEGUI::VA_TOP);
      // set default color to white
      result->setProperty("TextColours", 
         CEGUI::PropertyHelper::colourToString(CEGUI::colour(1.0f, 1.0f, 1.0f)));
      result->show();

      return result;
   }

   //////////////////////////////////////////////////////////////////////////
   void GuiScreen::UpdateText(GuiNode* textControl, const std::string& newText,
      osg::Vec3 color, float x, float y)
   {
      float red = color.x();
      float green = color.y();
      float blue = color.z();

      if (textControl != NULL)
      {
         // text and color
         if (textControl->getText() != newText)
         {
            textControl->setText(newText);
            if (red >= 0.00 && blue >= 0.0 && green >= 0.0)
            {
               textControl->setProperty("TextColours", 
                  CEGUI::PropertyHelper::colourToString(CEGUI::colour(red, green, blue)));
               // how to do it with a string.  Use "FF00FF00" or "FFFFFFFF" for examples
               //String col = PropertyHelper::colourRectToString(ColourRect(PropertyHelper::stringToColour(String(color))));
               //textControl->setProperty("TextColours", col);         }
            }
         }
         // position
         if (x > 0.0 && y > 0.0)
         {
            CEGUI::UVector2 position = textControl->getPosition();
            CEGUI::UVector2 newPos(cegui_absdim(x), cegui_absdim(y));
            if (position != newPos)
               textControl->setPosition(newPos);
         }
      }
   }

   //////////////////////////////////////////////////////////////////////////
   void GuiScreen::UpdateText(GuiNode* textControl, const std::string& newText,
      float x, float y)
   {
      osg::Vec3 color(1.0f, 1.0f, 1.0f);
      UpdateText(textControl, newText, color, x, y);
   }

}