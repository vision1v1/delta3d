/* 
 * Delta3D Open Source Game and Simulation Engine 
 * Copyright (C) 2004-2005 MOVES Institute 
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
*/

#ifndef DELTA_CHARACTER
#define DELTA_CHARACTER

// character.h: Declaration of the Character class.
//
//////////////////////////////////////////////////////////////////////

// Undef some common macros so rbody can function.
#ifdef Status
#undef Status
#endif //Status

#ifdef DEBUG
#undef DEBUG
#endif //DEBUG

#include <rbody/osg/ReplicantBodyMgr.h>

#include <dtCore/transformable.h>
#include <dtCore/loadable.h>
#include <osgUtil/UpdateVisitor>
#include <dtChar/export.h>

/// @cond DOXYGEN_SHOULD_SKIP_THIS
namespace osgUtil
{
   class UpdateVisitor;
}
/// @endcond

namespace dtChar
{
   /**
    * An animated character.
    */
   class DT_CHAR_EXPORT Character : public dtCore::Transformable, public dtCore::Loadable
   {
      DECLARE_MANAGEMENT_LAYER(Character)

      public:

         /**
          * Constructor.
          *
          * @param name the instance name
          */
         Character(const std::string& name = "character");
      
      protected:

         /**
          * Destructor.
          */
         virtual ~Character();

      public:

         virtual osg::Node* GetOSGNode()
         { 
            return mBodyNode.get();
         }

         virtual const osg::Node* GetOSGNode() const
         {
            return mBodyNode.get();
         }

         virtual osg::MatrixTransform* GetMatrixNode()
         {
            return mBodyNode.get();
         }

         virtual const osg::MatrixTransform* GetMatrixNode() const
         {
            return mBodyNode.get() ;
         }

         /**
          * Notifies this drawable object that it has been added to
          * a scene.
          *
          * @param scene the scene to which this drawable object has
          * been added
          */
         virtual void AddedToScene(dtCore::Scene* scene);
         
         /**
          * Loads a ReplicantBody data file for this character.
          *
          * @param filename the name of the file to load
          * @return true if successful, false if not
          */
         virtual osg::Node* LoadFile( const std::string& filename, bool useCache = true );

         virtual void OnMessage( dtCore::Base::MessageData* data );

         /**
          * Sets the rotation of this character.
          *
          * @param rotation the new rotation, in degrees about the Z
          * axis
          */
         void SetRotation(float rotation);
         
         /**
          * Returns the current rotation of this character.
          *
          * @return the current rotation, in degrees about the Z
          * axis
          */
         float GetRotation() const;
         
         /**
          * Sets the walk/run velocity of this character.
          *
          * @param velocity the new walk/run velocity
          */
         void SetVelocity(float velocity);
         
         /**
          * Returns the current walk/run velocity of this character.
          *
          * @return the current walk/run velocity
          */
         float GetVelocity() const;
         
         /**
          * Executes a character action.
          *
          * @param name the name of the action to execute
          * @param priority whether or not the action is high-priority
          * @param force whether or not to force the action
          */
         void ExecuteAction(const std::string& name,
                            bool priority = true, 
                            bool force = false);
         
         /**
          * Executes a character action with a speed parameter.
          *
          * @param name the name of the action to execute
          * @param speed the speed at which to execute the action
          * @param priority whether or not the action is high-priority
          * @param force whether or not to force the action
          */
         void ExecuteActionWithSpeed(const std::string& name, 
                                     float speed,
                                     bool priority = true,
                                     bool force = false);
         
         /**
          * Executes a character action with an angle parameter.
          *
          * @param name the name of the action to execute
          * @param angle the angle at which to execute the action
          * @param priority whether or not the action is high-priority
          * @param force whether or not to force the action
          */
         void ExecuteActionWithAngle(const std::string& name, 
                                     float angle,
                                     bool priority = true,
                                     bool force = false);
         
         /**
          * Executes a character action with speed and angle parameters.
          *
          * @param name the name of the action to execute
          * @param speed the speed at which to execute the action
          * @param angle the angle at which to execute the action
          * @param priority whether or not the action is high-priority
          * @param force whether or not to force the action
          */
         void ExecuteActionWithSpeedAndAngle(const std::string& name, 
                                             float speed, 
                                             float angle,
                                             bool priority = true,
                                             bool force = false);
         
         /**
          * Stops a character action.
          *
          * @param name the name of the action to stop
          */
         void StopAction(const std::string& name);
         

         /**
          * 
          * Returns a pointer to the rbody osg node
          *
          */
         rbody::OsgBodyNode* GetBodyNode() { return mBodyNode.get(); }
         const rbody::OsgBodyNode* GetBodyNode() const { return mBodyNode.get(); }

      private:
         
         /**
          * The ReplicantBody OSG node.
          */
         dtCore::RefPtr<rbody::OsgBodyNode> mBodyNode;
         
         /**
          * The collision root node.
          */
         dtCore::RefPtr<osg::Node> mCollisionRootNode;
         
         /**
          * The name of the last file loaded.
          */
         std::string mFilename;
         
         /**
          * The character's current rotation.
          */
         float mRotation;
         
         /**
          * The character's current walk/run velocity.
          */
         float mVelocity;

         /**
          * The rbody node's update mode previous to pause.
          */
         rbody::OsgBodyNode::UpdateMode mPreviousUpdateMode;
         rbody::OsgBodyNode::UpdateMode mPreviousInternalUpdateMode;
         dtCore::RefPtr< osgUtil::UpdateVisitor > mUpdateVisitor;
         int mPauseFrameNumber;
   };
};

#endif // DELTA_CHARACTER
