/*
 * Delta3D Open Source Game and Simulation Engine
 * Copyright (C) 2008 MOVES Institute
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Author: Jeff P. Houde
 */

#include <dtDirectorQt/valueitem.h>
#include <dtDirectorQt/directoreditor.h>
#include <dtDirectorQt/editorscene.h>
#include <dtDirectorQt/linkitem.h>
#include <dtDirectorQt/undomanager.h>
#include <dtDirectorQt/undocreateevent.h>

#include <dtDirector/valuenode.h>

#include <QtGui/QGraphicsScene>
#include <QtGui/QMenu>

#include <dtDAL/datatype.h>

#include <osg/Vec2>


namespace dtDirector
{
   //////////////////////////////////////////////////////////////////////////
   ValueItem::ValueItem(Node* node, QGraphicsItem* parent, EditorScene* scene)
       : NodeItem(node, parent, scene)
       , mValueText(NULL)
       , mValueLink(NULL)
   {
   }

   //////////////////////////////////////////////////////////////////////////
   void ValueItem::Draw()
   {
      NodeItem::Draw();

      mLoading = true;

      ValueNode *valueNode = dynamic_cast<ValueNode *>(mNode.get());
      if (valueNode)
      {
         SetTitle(mNode->GetName());
         SetValueText(valueNode->GetValueLabel());
         DrawTitle();

         DrawPolygonTop();
         DrawPolygonRightFlat();
         DrawPolygonBottomRound();
         DrawPolygonLeftFlat();

         int size = mNodeWidth;
         if (size < mNodeHeight) size = mNodeHeight;

         QRadialGradient radialGradient(mNodeWidth/2, mNodeHeight, size, mNodeWidth/2, mNodeHeight);
         if (mNode->GetEnabled())
         {
            if (mNode->GetNodeLogging())
            {
               setPen(QPen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            }
            else
            {
               setPen(QPen(GetDarkColorForType(valueNode->GetPropertyType().GetTypeId()), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            }

            QColor color = GetColorForType(valueNode->GetPropertyType().GetTypeId());
            color.setAlphaF(0.80f);
            radialGradient.setColorAt(0.0, color);

            color = GetDarkColorForType(valueNode->GetPropertyType().GetTypeId());
            color.setAlphaF(0.80f);
            radialGradient.setColorAt(1.0, color);
         }
         else
         {
            setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

            QColor color = GetColorForType(valueNode->GetPropertyType().GetTypeId());
            color.setAlphaF(0.25f);
            radialGradient.setColorAt(0.0, color);

            color = GetDarkColorForType(valueNode->GetPropertyType().GetTypeId());
            color.setAlphaF(0.25f);
            radialGradient.setColorAt(1.0, color);
         }

         setBrush(radialGradient);

         if (!mValueLink)
         {
            mValueLink = new ValueNodeLinkItem(this, this, mScene);
         }

         if (mValueLink)
         {
            QPolygonF poly;
            poly << QPointF(-LINK_SIZE/2, 0.0f) << QPointF(LINK_SIZE/2, 0.0f) <<
               QPointF(LINK_SIZE/2, -LINK_LENGTH + LINK_SIZE/2) <<
               QPointF(0, -LINK_LENGTH) <<
               QPointF(-LINK_SIZE/2, -LINK_LENGTH + LINK_SIZE/2);

            mValueLink->setPolygon(poly);
            mValueLink->setPos(mNodeWidth / 2, -1.0f);

            mValueLink->SetPropertyType(valueNode->GetPropertyType().GetTypeId());
            mValueLink->setPen(QPen(GetDarkColorForType(mValueLink->GetPropertyType()), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            mValueLink->setBrush(GetColorForType(mValueLink->GetPropertyType()));
         }

         setPolygon(mPolygon);

         osg::Vec2 pos = mNode->GetPosition();
         setPos(pos.x(), pos.y());

         SetComment(mNode->GetComment());
      }

      mLoading = false;
   }

   //////////////////////////////////////////////////////////////////////////
   void ValueItem::SetTitle(std::string text)
   {
      if (!mTitle)
      {
         mTitle = new GraphicsTextItem(this, scene());
         mTitle->setTextWidth(MIN_NODE_WIDTH);
         mTitle->setFlag(QGraphicsItem::ItemIgnoresParentOpacity, true);
      }

      mTitle->setHtml((std::string("<center>") + text + "</center>").c_str());

      // Create the title background.
      QRectF bounds = mTitle->boundingRect();
      mTextHeight = bounds.height();

      if (mNodeHeight < mTextHeight) mNodeHeight = mTextHeight;

      // Clamp the bounds to our min and max.
      if (bounds.width() > MAX_NODE_WIDTH - 2) bounds.setWidth(MAX_NODE_WIDTH - 2);
      if (bounds.width() < MIN_NODE_WIDTH - 2) bounds.setWidth(MIN_NODE_WIDTH - 2);

      // Resize the width of the node if it is not wide enough already.
      if (bounds.width() + 2 > mNodeWidth)
      {
         mNodeWidth = bounds.width();
      }
   }

   //////////////////////////////////////////////////////////////////////////
   void ValueItem::SetValueText(const std::string& text)
   {
      if (!mValueText)
      {
         mValueText = new GraphicsTextItem(this, scene());
         mValueText->setTextWidth(MIN_NODE_WIDTH);
         mValueText->setFlag(QGraphicsItem::ItemIgnoresParentOpacity, true);
      }

      mValueText->setHtml((std::string("<center>") + text + "</center>").c_str());

      if (text.empty()) return;

      // Create the title background.
      QRectF bounds = mValueText->boundingRect();

      float y = mNodeHeight - bounds.height();
      if (y < mTextHeight)
      {
         mNodeHeight += mTextHeight - y;
         y = mTextHeight;
      }

      mValueText->setPos(0, y + 11);
   }

   //////////////////////////////////////////////////////////////////////////
   void ValueItem::SetComment(const std::string& text)
   {
      if (!mComment)
      {
         mComment = new GraphicsTextItem(this, scene());

         mComment->setDefaultTextColor(Qt::darkGreen);
      }

      mComment->setPlainText(text.c_str());

      // Create the title background.
      QRectF nodeBounds = boundingRect();

      mComment->setTextWidth(nodeBounds.width() * 2.0f);
      mComment->setPos(nodeBounds.x(), nodeBounds.height());
   }

   //////////////////////////////////////////////////////////////////////////
   void ValueItem::ConnectLinks(bool fullConnect)
   {
      NodeItem::ConnectLinks(fullConnect);

      if (fullConnect && mNode.valid())
      {
         ValueNode* valueNode = dynamic_cast<ValueNode*>(mNode.get());
         if (valueNode)
         {
            int count = (int)valueNode->GetLinks().size();
            for (int index = 0; index < count; index++)
            {
               ValueLink* link = valueNode->GetLinks()[index];
               if (!link) continue;

               NodeItem* item = mScene->GetNodeItem(link->GetOwner()->GetID());
               if (!item) continue;

               int valueCount = (int)item->GetValues().size();
               for (int valueIndex = 0; valueIndex < valueCount; valueIndex++)
               {
                  ValueData& value = item->GetValues()[valueIndex];
                  if (value.link)
                  {
                     int linkCount = (int)value.link->GetLinks().size();
                     value.ResizeLinks(linkCount, mScene);
                     for (int linkIndex = 0; linkIndex < linkCount; linkIndex++)
                     {
                        if (value.link->GetLinks()[linkIndex] == mNode.get())
                        {
                           NodeItem::ConnectLinks(value, this, linkIndex);
                           break;
                        }
                     }
                  }
               }
            }
         }
      }
   }

   //////////////////////////////////////////////////////////////////////////
   void ValueItem::OnGotoReference()
   {
      dtDAL::ActorProperty* prop = mNode->GetProperty("Reference");
      if (prop)
      {
         // Find the referenced value node.
         std::string name = prop->ToString();

         // Search for the referenced node anywhere in the current graph
         // or any of its parents.
         Node* refNode = NULL;
         DirectorGraph* graph = mNode->GetGraph();
         while (graph)
         {
            refNode = graph->GetValueNode(name, false);
            if (refNode) break;

            graph = graph->mParent;
         }

         if (refNode)
         {
            // Center the view on the referenced node.
            EditorScene* scene = mScene;
            scene->SetGraph(refNode->GetGraph());
            NodeItem* item = scene->GetNodeItem(refNode->GetID(), true);
            if (item)
            {
               scene->clearSelection();
               item->setSelected(true);
               scene->CenterSelection();
            }
         }
      }
   }

   //////////////////////////////////////////////////////////////////////////
   void ValueItem::OnCreateReference()
   {
      EditorScene* scene = mScene;
      std::string name = mNode->GetName();

      // If the value is already a reference, make sure we reference
      // the actual value instead of the reference.
      const NodeType& type = mNode->GetType();
      if (type.GetFullName() == "Core.Reference")
      {
         Node* node = mNode.get();

         // Keep looping until we find an actual value, or no value.
         while (node)
         {
            dtDAL::ActorProperty* prop = node->GetProperty("Reference");
            if (prop) name = prop->ToString();

            node = scene->GetEditor()->GetDirector()->GetValueNode(name);
            if (!node || node->GetType() != type)
            {
               break;
            }
         }
      }

      osg::Vec2 position = mNode->GetPosition();

      Node* node = scene->CreateNode("Reference", "Core", position.x() + 20.0f, position.y() + 20.0f);
      if (node)
      {
         dtDAL::ActorProperty* prop = node->GetProperty("Reference");
         if (prop) prop->FromString(name);

         scene->Refresh();

         // Create an undo event for this creation event.
         UndoManager* undoManager = scene->GetEditor()->GetUndoManager();
         if (undoManager)
         {
            dtCore::RefPtr<UndoCreateEvent> event = new UndoCreateEvent(undoManager->GetEditor(), node->GetID(), scene->GetGraph()->GetID());
            undoManager->AddEvent(event);
         }
      }
   }

   //////////////////////////////////////////////////////////////////////////
   void ValueItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
   {
      setSelected(true);
      QMenu menu;
      bool hasDefault = false;
      if (mNode->GetType().GetFullName() == "Core.Reference")
      {
         QAction* gotoRefAction = menu.addAction("Go to Referenced Value");
         menu.setDefaultAction(gotoRefAction);
         hasDefault = true;
         connect(gotoRefAction, SIGNAL(triggered()), this, SLOT(OnGotoReference()));
      }
      else if (mNode->GetType().GetFullName() == "Core.Value Link")
      {
         menu.addAction(mScene->GetEditor()->GetParentAction());
         menu.setDefaultAction(mScene->GetEditor()->GetParentAction());
         hasDefault = true;
      }

      QAction* refAction = menu.addAction("Create Reference");
      connect(refAction, SIGNAL(triggered()), this, SLOT(OnCreateReference()));
      if (!hasDefault) menu.setDefaultAction(refAction);

      menu.addSeparator();
      menu.addAction(mScene->GetMacroSelectionAction());
      menu.addSeparator();
      menu.addAction(mScene->GetEditor()->GetCutAction());
      menu.addAction(mScene->GetEditor()->GetCopyAction());
      menu.addSeparator();
      menu.addAction(mScene->GetEditor()->GetDeleteAction());
      menu.exec(event->screenPos());
   }

   //////////////////////////////////////////////////////////////////////////
   void ValueItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
   {
      NodeItem::mouseDoubleClickEvent(event);

      // This only does anything if the value item is a reference value.
      if (mNode->GetType().GetFullName() == "Core.Reference")
      {
         OnGotoReference();
      }
      else if (mNode->GetType().GetFullName() == "Core.Value Link")
      {
         mScene->GetEditor()->OnParentButton();
      }
      else
      {
         OnCreateReference();
      }
   }
}

//////////////////////////////////////////////////////////////////////////