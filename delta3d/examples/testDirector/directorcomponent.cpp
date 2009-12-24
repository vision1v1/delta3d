/*
 * Delta3D Open Source Game and Simulation Engine
 * Copyright (C) 2009 MOVES Institute
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
 * MG
 */

#include "directorcomponent.h"

#include <dtABC/application.h>
#include <dtGame/basemessages.h>
#include <dtCore/deltawin.h>
#include <dtCore/transform.h>
#include <dtCore/collisionmotionmodel.h>
#include <dtCore/collisioncategorydefaults.h>
#include <dtActors/engineactorregistry.h>
#include <dtDirector/director.h>

const std::string& DirectorComponent::NAME = "DirectorComponent";

////////////////////////////////////////////////////////////////////////////////

DirectorComponent::DirectorComponent(const std::string& name)
   : dtGame::BaseInputComponent(name)
   , mMotionModel(NULL)
{
}

////////////////////////////////////////////////////////////////////////////////
DirectorComponent::~DirectorComponent()
{
   // Save our recorded script for debugging later.
   mDoorDirector->SaveRecording("DoorRecording");
   mFireDirector->SaveRecording("FireRecording");
}

////////////////////////////////////////////////////////////////////////////////
void DirectorComponent::ProcessMessage(const dtGame::Message& message)
{
   if (message.GetMessageType() == dtGame::MessageType::TICK_LOCAL)
   {
      const dtGame::TickMessage& tickMessage = static_cast<const dtGame::TickMessage&>(message);

      // Update Director with both sim and real time
      mDoorDirector->Update(tickMessage.GetDeltaSimTime(), tickMessage.GetDeltaRealTime());
      mFireDirector->Update(tickMessage.GetDeltaSimTime(), tickMessage.GetDeltaRealTime());
   }
   else if (message.GetMessageType() == dtGame::MessageType::INFO_MAP_LOADED)
   {
      OnMapLoaded();
   }
}

////////////////////////////////////////////////////////////////////////////////
bool DirectorComponent::HandleKeyPressed(const dtCore::Keyboard* keyBoard, int key)
{
   bool handled = true;

   switch (key)
   {
   case osgGA::GUIEventAdapter::KEY_Escape:
      {
         GetGameManager()->GetApplication().Quit();
         return true;
      }
   case '~':
      {
         GetGameManager()->GetApplication().SetNextStatisticsType();
         return false;
      }
   default:
      break;
   };

   if (!handled)
   {
      return GetGameManager()->GetApplication().KeyPressed(keyBoard, key);
   }

   return handled;
}

////////////////////////////////////////////////////////////////////////////////
void DirectorComponent::OnAddedToGM()
{
   // Must call this to actually receive input
   BaseInputComponent::OnAddedToGM();

   // Creating and initializing our Director scripts.
   // In this case, we have two running in parallel.
   // One for the doors and the other for the fire places.
   mDoorDirector = new dtDirector::Director();
   mDoorDirector->Init();

   mFireDirector = new dtDirector::Director();
   mFireDirector->Init();
}

////////////////////////////////////////////////////////////////////////////////
void DirectorComponent::OnMapLoaded()
{
   dtGame::GameManager* gm = GetGameManager();
   dtABC::Application& app = gm->GetApplication();
   dtCore::Camera* camera  = gm->GetApplication().GetCamera();

   // Make sure collision detection is performed on the camera and with the trigger
   camera->SetCollisionSphere(1.0f);
   camera->SetCollisionCollideBits(COLLISION_CATEGORY_MASK_PROXIMITYTRIGGER);
   app.GetScene()->AddDrawable(camera);

   // Allow the player to walk around the level and collide with objects
   dtCore::CollisionMotionModel* motionModel = 
      new dtCore::CollisionMotionModel(1.5f, 0.2f, 0.1f, 0.05f, app.GetScene(), app.GetKeyboard(), app.GetMouse());

   // Prevent the motion model from colliding with the camera
   motionModel->GetFPSCollider().SetCollisionBitsForFeet(COLLISION_CATEGORY_MASK_OBJECT);
   motionModel->GetFPSCollider().SetCollisionBitsForTorso(COLLISION_CATEGORY_MASK_OBJECT);  

   motionModel->SetScene(&gm->GetScene());
   motionModel->SetTarget(camera);

   mMotionModel = motionModel;

   app.GetWindow()->ShowCursor(false);

   std::vector<dtDAL::ActorProxy*> proxyList;
   gm->FindActorsByType(*dtActors::EngineActorRegistry::STATIC_MESH_ACTOR_TYPE, proxyList);

   // Try to eliminate interpenetration overhead (ODE is the bottleneck here)
   for (size_t proxyIndex = 0; proxyIndex < proxyList.size(); ++proxyIndex)
   {
      dtCore::Transformable* actor = 
         dynamic_cast<dtCore::Transformable*>(proxyList[proxyIndex]->GetActor());

      actor->SetCollisionCategoryBits(COLLISION_CATEGORY_MASK_OBJECT);
      actor->SetCollisionCollideBits(COLLISION_CATEGORY_MASK_CAMERA);
      //actor->RenderCollisionGeometry(true);
   }

   // Get the player start position
   dtDAL::ActorProxy* playerStartProxy = NULL;
   gm->FindActorByType(*dtActors::EngineActorRegistry::PLAYER_START_ACTOR_TYPE, playerStartProxy);

   dtCore::Transformable* playerStart = 
      dynamic_cast<dtCore::Transformable*>(playerStartProxy->GetActor());

   dtCore::Transform startTransform;
   playerStart->GetTransform(startTransform);
   camera->SetTransform(startTransform);

   LoadDirectorScript();
}

////////////////////////////////////////////////////////////////////////////////
void DirectorComponent::LoadDirectorScript()
{
   dtGame::GameManager* gm = GetGameManager();
   dtCore::Camera* camera  = gm->GetApplication().GetCamera();

   // Here we set the player so any Player Value script nodes
   // used in any scripts will know who the player is.  In this
   // case, the camera represents our player so we will use that.
   mDoorDirector->SetPlayer(camera->GetUniqueId());
   mFireDirector->SetPlayer(camera->GetUniqueId());

   // Load our doors.dtDir and fires.dtDir script files.
   mDoorDirector->LoadScript("doors");
   mFireDirector->LoadScript("fires");

   // For debug purposes, set Node Logging to true if you want
   // all nodes that are flagged to log their output, to print
   // a log message in the console when they are executed.
   mDoorDirector->SetNodeLogging(true);
   mFireDirector->SetNodeLogging(true);

   // For further debugging, we can start recording the entire execution
   // of the script.  When we close this application, we will then save
   // the recorded data to a file.  Later, we can load the recorded data
   // file in the Director Graph Editor and step through all of the nodes
   // that were executed during this recording.
   mDoorDirector->StartRecording();
   mFireDirector->StartRecording();
}

////////////////////////////////////////////////////////////////////////////////
