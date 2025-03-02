
#pragma once

#include "wx/wx.h"
#include "wx/app.h"
#include "wx/intl.h"
#include <list>

class UsbScrollMainFrame;

class UsbScrollApp : public wxApp
{
public:
   UsbScrollApp();
   ~UsbScrollApp();

   bool OnInit();
   int OnExit();
private:
   UsbScrollMainFrame* m_poMainFrame = nullptr;
   
private:
   DECLARE_EVENT_TABLE()

};

DECLARE_APP(UsbScrollApp);
