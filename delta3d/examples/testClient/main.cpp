/*
* Delta3D Open Source Game and Simulation Engine
* Copyright (C) 2007, MOVES Institute
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
*  Pjotr van Amerongen
*/
#include <dtCore/globals.h>
#include <dtUtil/log.h>
#include <dtUtil/exception.h>

#include "testClient.h"

//////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{ 
   dtCore::RefPtr<EchoClient> app; 

   dtUtil::Log::GetInstance().SetLogLevel(dtUtil::Log::LOG_INFO);

   try 
   {
      app = new EchoClient();
      app->Config();
      app->Run();
   }
   catch (dtUtil::Exception &ex)
   {
      std::cout << "Caught Exception: " << ex.ToString() << std::endl;
   }
   
   return 0;
}
