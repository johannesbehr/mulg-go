#pragma once

#include "esp_system.h"
#include "typedefs.h"

 typedef struct fontCharInfo{
        UInt16 fontType;         // font type
        UInt16 firstChar;        // ASCII code of first character
        UInt16 lastChar;         // ASCII code of last character
        UInt16 maxWidth;         // maximum character width
        UInt16 kernMax;          // negative of maximum character kern
        UInt16 nDescent;         // negative of descent
        UInt16 fRectWidth;       // width of font rectangle
        UInt16 fRectHeight;      // height of font rectangle
        UInt16 owTLoc;           // offset to offset/width table
        UInt16 ascent;           // ascent
        UInt16 descent;          // descent
        UInt16 leading;          // leading
        UInt16 rowWords;         // row width of bit image / 2
		UInt16 width;
};


// Font structures for newer Adafruit_GFX (1.1 and later).
// Example fonts are included in 'Fonts' directory.
// To use a font in your Arduino sketch, #include the corresponding .h
// file and pass address of GFXfont struct to setFont().  Pass NULL to
// revert to 'classic' fixed-space bitmap font.

#ifndef _GFXFONT_H_
#define _GFXFONT_H_

/// Font data stored PER GLYPH
typedef struct {
	uint16_t bitmapOffset;     ///< Pointer into GFXfont->bitmap
	uint8_t  width;            ///< Bitmap dimensions in pixels
        uint8_t  height;           ///< Bitmap dimensions in pixels
	uint8_t  xAdvance;         ///< Distance to advance cursor (x axis)
	int8_t   xOffset;          ///< X dist from cursor pos to UL corner
        int8_t   yOffset;          ///< Y dist from cursor pos to UL corner
} GFXglyph;

/// Data stored for FONT AS A WHOLE
typedef struct { 
	uint8_t  *bitmap;      ///< Glyph bitmaps, concatenated
	GFXglyph *glyph;       ///< Glyph array
	uint8_t   first;       ///< ASCII extents (first char)
        uint8_t   last;        ///< ASCII extents (last char)
	uint8_t   yAdvance;    ///< Newline distance (y axis)
} GFXfont;

#endif // _GFXFONT_H_



typedef struct fontCharInfo FontCharInfoType;
typedef FontCharInfoType* FontPtr;

UInt16 FntCharHeight();
FontPtr FntGetFontPtr();

void draw_text(uint16_t x, uint16_t y, const char* message, uint16_t count);
void drawChar(int16_t x, int16_t y, unsigned char c, uint8_t color);
void drawGlyph(int16_t x, int16_t y, GFXglyph *glyph, uint8_t  *bitmap, uint8_t color);
void writeFastHLine(int16_t x, int16_t y,int16_t w, uint8_t color);
uint16_t FntWordWrap(const char* message, uint16_t width);
uint16_t FntLineHeight();
uint16_t FntLineWidth(const char* message, uint16_t count);

