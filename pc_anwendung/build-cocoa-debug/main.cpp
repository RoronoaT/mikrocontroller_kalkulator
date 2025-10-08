#include "main.h"
#include "frame.h"

IMPLEMENT_APP(GUI)

bool GUI::OnInit()
{
 ViewPort *guiViewPort = new ViewPort(wxT("Mikrocontroller-Rechner"));
 guiViewPort->Show(true);

 return true;
}

// run using this: g++ frame.cpp frame.h main.cpp main.h  `./wx-config --cxxflags --libs`