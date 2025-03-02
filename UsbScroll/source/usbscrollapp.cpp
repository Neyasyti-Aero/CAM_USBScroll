
#include "usbscrollapp.h"
#include "usbscrollmainframe.h"

// Макрос для запуска wxWindows-приложения
IMPLEMENT_APP(UsbScrollApp)

BEGIN_EVENT_TABLE( UsbScrollApp, wxApp )
//	EVT_MENU(XRCID("ID_MENU_EDIT_PREFERENCES"),FProApp::OnPreferences)
END_EVENT_TABLE()

UsbScrollApp::UsbScrollApp()
{
}

UsbScrollApp::~UsbScrollApp()
{
}

//Запуск приложения
bool UsbScrollApp::OnInit()
{
   wxInitAllImageHandlers();

   m_poMainFrame = new UsbScrollMainFrame(nullptr);
   m_poMainFrame->Show(true);
   return true;
}

int UsbScrollApp::OnExit()
{
   return wxApp::OnExit();
}
