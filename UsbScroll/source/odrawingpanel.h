#pragma once

#include "ocameradatamanager.h"
#include "wx/scrolwin.h"
#include <mutex>

class ODrawingPanel : public wxScrolledCanvas, public INewFrameObserver
{
public:
   ODrawingPanel(wxWindow* poParent_p, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxCLIP_CHILDREN|wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
   virtual ~ODrawingPanel();

   void ProcessNewFrame() override;
   void PrepareForStreamStart(eseusbcam_long_t FrameSizeX, eseusbcam_long_t FrameSizeY) override;
   void OnPaint( wxPaintEvent& event );
   void OnSize( wxSizeEvent& event );

   void SetDrawFrames(bool bEnabled_p);
private:
   void UpdateScrollingParams();
private:
   wxSize m_oPrevBitmapSize;
   wxPoint m_oBitmapShift;
   OFPSCalculator m_oPaintFPSCalculator;
   bool m_bDrawProcessed = true;
   std::mutex m_oDrawProcessedMutex;
   bool m_bDrawFramesEnabled = true;
};