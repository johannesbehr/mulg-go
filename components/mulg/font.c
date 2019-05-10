 #include "font.h"
 #include "fonts/FreeMono12pt7b.h"
 #include "fonts/FreeMono9pt7b.h"
 #include "fonts/FreeMonoBold9pt7b.h"
 #include "fonts/FreeSans9pt7b.h"

 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
 #define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
 #define pgm_read_word(addr) (*(const unsigned short *)(addr))
 #define writePixel(x, y, c)  framebuffer[x+((y)*240)]=c
 
 extern uint8_t* framebuffer; 
 
UInt16 FntCharHeight(){
	return 1;
}

FontPtr FntGetFontPtr()
{
	return NULL;
}

void writeFastHLine(int16_t x, int16_t y,int16_t w, uint8_t color) {

	uint16_t WIDTH = 240;
	uint16_t HEIGHT = 240;

	
	if(color==0){
		printf("writeFastHLine (%d,%d,%d,%d)\n", x, y,w,color);
	}

/*
    if((x >= _width) || (y < 0) || (y >= _height)) return;
    int16_t x2 = x + w - 1;
    if(x2 < 0) return;

    // Clip left/right
    if(x < 0) {
        x = 0;
        w = x2 + 1;
    }
    if(x2 >= _width) w = _width - x;
*/
/*    int16_t t;
    switch(rotation) {
        case 1:
            t = x;
            x = WIDTH  - 1 - y;
            y = t;
            break;
        case 2: 
            x = WIDTH  - 1 - x;
            y = HEIGHT - 1 - y;
            break;
        case 3:
            t = x;
            x = y;
            y = HEIGHT - 1 - t;
            break;
    }*/

	
    memset(framebuffer + (y * WIDTH) + x, color, w);
}

uint16_t FntLineHeight(){
	GFXfont* gfxFont = &FreeSans9pt7b;
	return(gfxFont->yAdvance);
}

uint16_t FntLineWidth(const char* message, uint16_t count){
	uint16_t width = 0;
	uint16_t i = 0;
	GFXfont* gfxFont = &FreeSans9pt7b;
	uint8_t first = gfxFont->first;
	
	while(message[i]!=0 && i < count){
		uint8_t c = message[i];
		GFXglyph *glyph = &(gfxFont->glyph[c - first]);
		width += glyph->xAdvance;
		i++;
	}
	
	return(width);
	
}


uint16_t FntWordWrap(const char* message, uint16_t maxWidth){
	
	
	
	uint16_t lastBreak = 0;
	uint16_t width = 0;
	
	uint16_t i = 0;
	GFXfont* gfxFont = &FreeSans9pt7b;
	uint8_t first = gfxFont->first;
	
	printf("FntWordWrap(%s,%d)\n",message,maxWidth);
	
	while(message[i]!=0 && width < maxWidth){
		uint8_t c = message[i];
		
		if(c==' '){
			lastBreak = i;
		}
		
		GFXglyph *glyph = &(gfxFont->glyph[c - first]);
		width += glyph->xAdvance;
		i++;
		
		if(width>maxWidth){
			printf("(%d>%d)=> %d\n",width,maxWidth,lastBreak);
			return lastBreak;
		}
	}

	printf("=> %d\n",i);
	return(i);
}



void draw_text(uint16_t x, uint16_t y, const char* message, uint16_t count){

	uint16_t i = 0;
	GFXfont* gfxFont = &FreeSans9pt7b;
	uint8_t first = gfxFont->first;
	
	while(message[i]!=0 && i<count){
		uint8_t c = message[i];
		GFXglyph *glyph = &(gfxFont->glyph[c - first]);
		drawGlyph(x, y, glyph,gfxFont->bitmap, 1);
		x += glyph->xAdvance;
		//y += gfxFont->yAdvance;
		i++;
	}
}

void drawGlyph(int16_t x, int16_t y, GFXglyph *glyph, uint8_t  *bitmap, uint8_t color){
	
	uint16_t bo = glyph->bitmapOffset;
	uint8_t  w  = glyph->width,
	h  = glyph->height;
	int8_t   xo = glyph->xOffset,
	yo = glyph->yOffset;

//	printf("(10) w,h,xo, yo: %d,%d,%d,%d\n", w,h,xo,yo);

	uint8_t  xx, yy, bits = 0, bit = 0;

	// Todo: Add character clipping here

	// NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
	// THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
	// has typically been used with the 'classic' font to overwrite old
	// screen contents with new data.  This ONLY works because the
	// characters are a uniform size; it's not a sensible thing to do with
	// proportionally-spaced fonts with glyphs of varying sizes (and that
	// may overlap).  To replace previously-drawn text when using a custom
	// font, use the getTextBounds() function to determine the smallest
	// rectangle encompassing a string, erase the area with fillRect(),
	// then draw new text.  This WILL infortunately 'blink' the text, but
	// is unavoidable.  Drawing 'background' pixels will NOT fix this,
	// only creates a new set of problems.  Have an idea to work around
	// this (a canvas object type for MCUs that can afford the RAM and
	// displays supporting setAddrWindow() and pushColors()), but haven't
	// implemented this yet.

//	printf("Before Loop\n");

	//startWrite();
	for(yy=0; yy<h; yy++) {
		for(xx=0; xx<w; xx++) {
			if(!(bit++ & 7)) {
				bits = bitmap[bo++];
			}
			if(bits & 0x80) {
				writePixel(x+xo+xx, y+yo+yy, color);
			}
			bits <<= 1;
		}
	}
//	printf("Loop End.\n");

	
}


void drawChar(int16_t x, int16_t y, unsigned char c, uint8_t color){
	// Character is assumed previously filtered by write() to eliminate
	// newlines, returns, non-printable characters, etc.  Calling
	// drawChar() directly with 'bad' characters of font may cause mayhem!

	//printf("DrawChar (%d,%d,%c,%d)\n",x,y,c,color);

	GFXfont* gfxFont = &FreeMono12pt7b;
	c -= gfxFont->first;
	GFXglyph *glyph = &(gfxFont->glyph[c]);
	uint8_t  *bitmap = gfxFont->bitmap;
	uint16_t bo = glyph->bitmapOffset;
	uint8_t  w  = glyph->width,
	h  = glyph->height;
	int8_t   xo = glyph->xOffset,
	yo = glyph->yOffset;

	//printf("(10) w,h,xo, yo: %d,%d,%d,%d\n", w,h,xo,yo);

	uint8_t  xx, yy, bits = 0, bit = 0;

	// Todo: Add character clipping here

	// NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
	// THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
	// has typically been used with the 'classic' font to overwrite old
	// screen contents with new data.  This ONLY works because the
	// characters are a uniform size; it's not a sensible thing to do with
	// proportionally-spaced fonts with glyphs of varying sizes (and that
	// may overlap).  To replace previously-drawn text when using a custom
	// font, use the getTextBounds() function to determine the smallest
	// rectangle encompassing a string, erase the area with fillRect(),
	// then draw new text.  This WILL infortunately 'blink' the text, but
	// is unavoidable.  Drawing 'background' pixels will NOT fix this,
	// only creates a new set of problems.  Have an idea to work around
	// this (a canvas object type for MCUs that can afford the RAM and
	// displays supporting setAddrWindow() and pushColors()), but haven't
	// implemented this yet.

	//printf("Before Loop\n");

	//startWrite();
	for(yy=0; yy<h; yy++) {
		for(xx=0; xx<w; xx++) {
			if(!(bit++ & 7)) {
				bits = bitmap[bo++];
			}
			if(bits & 0x80) {
				writePixel(x+xo+xx, y+yo+yy, color);
			}
			bits <<= 1;
		}
	}
	//printf("Loop End.\n");
	//endWrite();

} // End classic vs custom font

