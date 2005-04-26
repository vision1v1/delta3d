#ifndef DELTA_GUI
#define DELTA_GUI

#include <dtCore/export.h>
#include <CEGUI/renderers/OpenGLGUIRenderer/openglrenderer.h>

#ifdef APIENTRY
#undef APIENTRY
#endif //APIENTRY

#ifdef WINGDIAPI
#undef WINGDIAPI
#endif //WINGDIAPI

namespace dtGUI
{
   ///Simple CEGui rendering class based on the CEGUI::OpenGLRender class
   class DT_EXPORT Renderer : public CEGUI::OpenGLRenderer
   {
   public:
      Renderer(unsigned int max_quads, int width, int height);
      ~Renderer(void) {};

      ///Create the ResourceProvider (a dtGUI::ResourceProvider)
      virtual CEGUI::ResourceProvider* createResourceProvider(void);
   };
}

#endif //DELTA_GUI
