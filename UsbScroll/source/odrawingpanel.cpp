
#include "odrawingpanel.h"
#include "wx/dcbuffer.h"

namespace
{
   class DrawProcessScopeExit
   {
   public:
      DrawProcessScopeExit(bool& rbFlag_p, std::mutex& roMutex_p)
      : m_rbFlag(rbFlag_p)
      , m_roMutex(roMutex_p)
      {}
      ~DrawProcessScopeExit()
      {
         std::lock_guard oLock(m_roMutex);
         m_rbFlag = true;
      }

   protected:
      bool& m_rbFlag;
      std::mutex& m_roMutex;
   };
}

ODrawingPanel::ODrawingPanel(wxWindow* poParent_p, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
   : wxScrolledCanvas(poParent_p, id, pos, size, wxALWAYS_SHOW_SB|style)
{
   Connect( wxEVT_PAINT, wxPaintEventHandler( ODrawingPanel::OnPaint ) );
   Connect( wxEVT_SIZE, wxSizeEventHandler( ODrawingPanel::OnSize ), nullptr, this );

   SetBackgroundStyle(wxBG_STYLE_PAINT);
   SetScrollRate(5, 5);
   UpdateScrollingParams();
}

ODrawingPanel::~ODrawingPanel()
{
   Disconnect( wxEVT_PAINT, wxPaintEventHandler( ODrawingPanel::OnPaint ) );
   Disconnect( wxEVT_SIZE, wxSizeEventHandler( ODrawingPanel::OnSize ), nullptr, this );
}

void ODrawingPanel::ProcessNewFrame()
{
   std::lock_guard oLock(m_oDrawProcessedMutex);
   if (m_bDrawProcessed && m_bDrawFramesEnabled)
   {
      m_bDrawProcessed = false;
      CallAfter(&ODrawingPanel::Refresh, true, nullptr);
   }
}

void ODrawingPanel::SetDrawFrames(bool bEnabled_p)
{
   std::lock_guard oLock(m_oDrawProcessedMutex);
   m_bDrawFramesEnabled = bEnabled_p;
}

void ODrawingPanel::PrepareForStreamStart(eseusbcam_long_t FrameSizeX, eseusbcam_long_t FrameSizeY)
{
   if (m_oPrevBitmapSize.GetWidth() == FrameSizeX &&
       m_oPrevBitmapSize.GetHeight() == FrameSizeY)
   {
      return;
   }
   m_oPrevBitmapSize.SetWidth(FrameSizeX);
   m_oPrevBitmapSize.SetHeight(FrameSizeY);
   UpdateScrollingParams();
}

void ODrawingPanel::UpdateScrollingParams()
{
   m_oBitmapShift.x = 0;
   m_oBitmapShift.y = 0;
   wxSize oCltSize = GetClientSize();
   wxSize oWndSize = GetVirtualSize();
   wxSize oNewSize(oWndSize);
   if (oCltSize.GetWidth() >= m_oPrevBitmapSize.GetWidth())
   {
      m_oBitmapShift.x = (oCltSize.GetWidth() - m_oPrevBitmapSize.GetWidth())/2;
      oNewSize.SetWidth(oCltSize.GetWidth());
   }
   else if (oWndSize.GetWidth() < m_oPrevBitmapSize.GetWidth())
   {
      oNewSize.SetWidth(m_oPrevBitmapSize.GetWidth());
   }
   if (oCltSize.GetHeight() >= m_oPrevBitmapSize.GetHeight())
   {
      m_oBitmapShift.y = (oCltSize.GetHeight() - m_oPrevBitmapSize.GetHeight())/2;
      oNewSize.SetHeight(oCltSize.GetHeight());
   }
   else if (oWndSize.GetHeight() < m_oPrevBitmapSize.GetHeight())
   {
      oNewSize.SetHeight(m_oPrevBitmapSize.GetHeight());
   }

   SetVirtualSize(oNewSize);
   Layout();
}

void ODrawingPanel::OnSize( wxSizeEvent& event )
{
   UpdateScrollingParams();
}

void ODrawingPanel::OnPaint( wxPaintEvent& event )
{
   DrawProcessScopeExit oDrawProcessExitProc(m_bDrawProcessed, m_oDrawProcessedMutex);
   
   m_oPaintFPSCalculator.Next();

   wxBufferedPaintDC oDC(this);
   if (!oDC.IsOk())
   {
      return;
   }

   DoPrepareDC(oDC);

   auto& roCamM = CamM;
   auto oLocker = roCamM.SharedLockFrameData();
   const auto& roFrameData = roCamM.GetFrameData();
   if (roFrameData.m_eState != ETransferState::eNormal || !roFrameData.GetFrameBuffer().IsOk())
   {
      oDC.Clear();
      return;
   }

   if (!oDC.IsOk())
   {
      return;
   }
 
   wxSize oCltSize = GetClientSize();
   oDC.SetBrush(*wxBLACK_BRUSH);
   oDC.SetPen(*wxBLACK_PEN);
   wxPoint oViewStart = GetViewStart();
   int xUnit = 0, yUnit = 0;
   GetScrollPixelsPerUnit (&xUnit, &yUnit) ;
   oViewStart.x *= xUnit;
   oViewStart.y *= yUnit;

   if (m_oBitmapShift.x > 0)
   {
      oDC.DrawRectangle(oViewStart.x, oViewStart.y, m_oBitmapShift.x+1, oCltSize.GetHeight());
      oDC.DrawRectangle(m_oBitmapShift.x + roFrameData.GetFrameBuffer().GetWidth(), oViewStart.y, m_oBitmapShift.x+1, oCltSize.GetHeight());
   }

   if (m_oBitmapShift.y > 0)
   {
      oDC.DrawRectangle(oViewStart.x, oViewStart.y, oCltSize.GetWidth(), m_oBitmapShift.y+1);
      oDC.DrawRectangle(oViewStart.x, m_oBitmapShift.y + roFrameData.GetFrameBuffer().GetHeight(), oCltSize.GetWidth(), m_oBitmapShift.y+1);
   }

   oDC.DrawBitmap(roFrameData.GetFrameBuffer(), m_oBitmapShift.x, m_oBitmapShift.y);
}
