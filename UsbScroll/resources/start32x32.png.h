#ifndef START32X32_PNG_H
#define START32X32_PNG_H

#include <wx/mstream.h>
#include <wx/image.h>
#include <wx/bitmap.h>

static const unsigned char start32x32_png[] = 
{
	0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 
	0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x20, 
	0x00, 0x00, 0x00, 0x20, 0x08, 0x06, 0x00, 0x00, 0x00, 0x73, 
	0x7A, 0x7A, 0xF4, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 
	0x73, 0x00, 0x00, 0x0B, 0x13, 0x00, 0x00, 0x0B, 0x13, 0x01, 
	0x00, 0x9A, 0x9C, 0x18, 0x00, 0x00, 0x00, 0x57, 0x49, 0x44, 
	0x41, 0x54, 0x58, 0xC3, 0xED, 0x96, 0xB1, 0x0A, 0x00, 0x20, 
	0x08, 0x44, 0x35, 0xFC, 0xFF, 0x5F, 0xB6, 0x25, 0x5A, 0xA2, 
	0x86, 0x48, 0x4E, 0xF2, 0x6E, 0x71, 0xF4, 0xF1, 0x38, 0x41, 
	0x11, 0x70, 0x74, 0x4C, 0x47, 0xED, 0x6F, 0x68, 0x03, 0xB6, 
	0x31, 0x12, 0x9D, 0x69, 0x3C, 0x9D, 0x01, 0x09, 0xEE, 0xC4, 
	0x62, 0x38, 0xAD, 0x81, 0xB7, 0x1E, 0x0E, 0xCD, 0x82, 0x1B, 
	0x20, 0x00, 0x01, 0x08, 0x40, 0x00, 0x02, 0x10, 0x80, 0x00, 
	0x04, 0xB0, 0xDB, 0x4F, 0xE6, 0x7B, 0x03, 0x5A, 0xB6, 0x03, 
	0x5E, 0xEE, 0x0A, 0xE0, 0xE9, 0x88, 0x47, 0x05, 0x49, 0x52, 
	0x1A, 0xCC, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 
	0x44, 0xAE, 0x42, 0x60, 0x82, 
};

wxBitmap& start32x32_png_to_wx_bitmap()
{
	static wxMemoryInputStream memIStream( start32x32_png, sizeof( start32x32_png ) );
	static wxImage image( memIStream, wxBITMAP_TYPE_PNG );
	static wxBitmap bmp( image );
	return bmp;
};


#endif //START32X32_PNG_H
