#include <stdlib.h>
#include <graph.h>
#include <conio.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <i86.h>
#include "protocol.h"
#include "math.h"
#include "scale.h"
#include "font.h"
#include "screen.h"

unsigned char CharWide=8;
unsigned char CharHigh=16;
padPt TTYLoc;
uint8_t FONT_SIZE_X;
uint8_t FONT_SIZE_Y;
uint16_t* scalex;
uint16_t* scaley;
uint8_t* font;
uint16_t* fontptr;
uint16_t width;
uint16_t height;
unsigned char screen_mode=8; // Try to detect by default
bool is_mono=false;
short default_background=0;
short default_foreground=1;
long default_foreground_color=0x003f3f3f;
long default_background_color=0;
long palette[256];
struct videoconfig vc;

extern padBool FastText; /* protocol.c */
extern uint16_t touch_x_offset; /* touch.c */
extern uint16_t touch_y_offset;
extern uint16_t touch_x_scale;
extern uint16_t touch_y_scale;
extern bool touch_soft_cursor;

/**
 * screen_init() - Set up the screen
 */
void screen_init(void)
{
    switch(screen_mode)
      {
      case 0: // CGA 320x200x4
	_setvideomode(_MRES4COLOR);
	width=320;
	height=200;
	FONT_SIZE_X=5;
	FONT_SIZE_Y=6;
	font=&font_320x200;
	scalex=&scalex_320;
	scaley=&scaley_200;
	fontptr=&fontptr_6;
	is_mono=true;
	default_foreground=3; // White.
	break;
      case 1: // CGA 640x200x2
	_setvideomode(_HRESBW);
	width=640;
	height=200;
	FONT_SIZE_X=8;
	FONT_SIZE_Y=6;
	font=&font_640x200;
	scalex=&scalex_640;
	scaley=&scaley_200;
	fontptr=&fontptr_6;
	is_mono=true;
	break;
      case 2: // Hercules 720x350x2
	_setvideomode(_HERCMONO);
	width=720;
	height=350;
	FONT_SIZE_X=8;
	FONT_SIZE_Y=10;
	font=&font_640x350;
	scalex=&scalex_720;
	scaley=&scaley_350;
	fontptr=&fontptr_10;
	touch_soft_cursor=true;
	is_mono=true;
	break;
      case 3: // EGA 640x350x16
	_setvideomode(_ERESCOLOR);
	width=640;
	height=350;
	FONT_SIZE_X=8;
	FONT_SIZE_Y=10;
	font=&font_640x350;
	scalex=&scalex_640;
	scaley=&scaley_350;
	fontptr=&fontptr_10;
	break;
      case 4: // VGA 640x480x2
	_setvideomode(_VRES2COLOR);
	width=640;
	height=480;
	FONT_SIZE_X=8;
	FONT_SIZE_Y=15;
	font=&font_640x480;
	scalex=&scalex_640;
	scaley=&scaley_480;
	fontptr=&fontptr_16;
	is_mono=true;
	_remappalette(1,0x003F3F3F); // quickly get a white in palette.
	break;
      case 5: // VGA 640x480x16
	_setvideomode(_VRES16COLOR);
	width=640;
	height=480;
	FONT_SIZE_X=8;
	FONT_SIZE_Y=15;
	font=&font_640x480;
	scalex=&scalex_640;
	scaley=&scaley_480;
	fontptr=&fontptr_16;
	_remappalette(1,0x003F3F3F); // quickly get a white in palette.
	break;
      case 6: // MCGA 320x200x256
	_setvideomode(_MRES256COLOR);
	width=320;
	height=200;
	FONT_SIZE_X=5;
	FONT_SIZE_Y=6;
	font=&font_320x200;
	scalex=&scalex_320;
	scaley=&scaley_200;
	fontptr=&fontptr_6;
	_remappalette(1,0x003F3F3F); // quickly get a white in palette.
	break;
      case 7: // SVGA 640x480x256
	_setvideomode(_VRES256COLOR);
	width=640;
	height=480;
	FONT_SIZE_X=8;
	FONT_SIZE_Y=15;
	font=&font_640x480;
	scalex=&scalex_640;
	scaley=&scaley_480;
	fontptr=&fontptr_16;
	_remappalette(1,0x003F3F3F); // quickly get a white in palette.
	touch_soft_cursor=true;
	break;
      case 9: // GRiD VGA Mono 640x400
	_setvideomode(_VRES2COLOR);
	width=640;
	height=400;
	FONT_SIZE_X=8;
	FONT_SIZE_Y=12;
	font=&font_640x400;
	scalex=&scalex_640;
	scaley=&scaley_400;
	fontptr=&fontptr_12;
	is_mono=true;
	_remappalette(1,0x003F3F3F); // quickly get a white in palette.
	break;
      case 8: // Detect
	_getvideoconfig(&vc);
	switch(vc.adapter)
	  {
	  case _SVGA:
	    _setvideomode(_VRES16COLOR);
	    width=640;
	    height=480;
	    screen_mode=5;
	    FONT_SIZE_X=8;
	    FONT_SIZE_Y=15;
	    font=&font_640x480;
	    scalex=&scalex_640;
	    scaley=&scaley_480;
	    fontptr=&fontptr_16;
	    _remappalette(1,0x003F3F3F); // quickly get a white in palette.
	    // touch_soft_cursor=true;
	    break;
	  case _VGA:
	    _setvideomode(_VRES16COLOR);
	    width=640;
	    height=480;
	    screen_mode=5;
	    FONT_SIZE_X=8;
	    FONT_SIZE_Y=15;
	    font=&font_640x480;
	    scalex=&scalex_640;
	    scaley=&scaley_480;
	    fontptr=&fontptr_16;
	    _remappalette(1,0x003F3F3F); // quickly get a white in palette.
	    break;
	  case _MCGA:
	    _setvideomode(_MRES256COLOR);
	    width=320;
	    height=200;
	    screen_mode=6;
	    FONT_SIZE_X=5;
	    FONT_SIZE_Y=6;
	    font=&font_320x200;
	    scalex=&scalex_320;
	    scaley=&scaley_200;
	    fontptr=&fontptr_6;
	    _remappalette(1,0x003F3F3F); // quickly get a white in palette.
	    break;
	  case _EGA:
	    _setvideomode(_ERESCOLOR);
	    width=640;
	    height=350;
	    screen_mode=3;
	    FONT_SIZE_X=8;
	    FONT_SIZE_Y=10;
	    font=&font_640x350;
	    scalex=&scalex_640;
	    scaley=&scaley_350;
	    fontptr=&fontptr_10;
	    _remappalette(1,0x003F3F3F); // quickly get a white in palette.
	    break;
	  case _CGA:
	    _setvideomode(_HRESBW);
	    is_mono=true;
	    width=640;
	    height=200;
	    screen_mode=1;
	    FONT_SIZE_X=8;
	    FONT_SIZE_Y=6;
	    font=&font_640x200;
	    scalex=&scalex_640;
	    scaley=&scaley_200;
	    fontptr=&fontptr_6;
	    default_foreground=3; // white
	    break;
	  case _HERCULES:
	    _setvideomode(_HERCMONO);
	    is_mono=true;
	    width=720;
	    height=350;
	    screen_mode=2;
	    FONT_SIZE_X=8;
	    FONT_SIZE_Y=10;
	    font=&font_640x350;
	    scalex=&scalex_720;
	    scaley=&scaley_350;
	    fontptr=&fontptr_10;
	    touch_soft_cursor=true;
	    break;
	  default: // No supported video card.
	    puts("PLATOTerm requires a graphics card.");
	    exit(1);
	    break;
	  }
      }

    // Set touch offsets based on first scale table entries.
}

/**
 * screen_load_driver()
 * Load the TGI driver
 */
void screen_load_driver(void)
{
}

/**
 * screen_init_hook()
 * Called after tgi_init to set any special features, e.g. nmi trampolines.
 */
void screen_init_hook(void)
{
}

/**
 * screen_cycle_foreground()
 * Go to the next foreground color in palette
 */
void screen_cycle_foreground(void)
{
}

/**
 * screen_cycle_background()
 * Go to the next background color in palette
 */
void screen_cycle_background(void)
{
}

/**
 * screen_cycle_border()
 * Go to the next border color in palette
 *
 */
void screen_cycle_border(void)
{
}

/**
 * screen_update_colors() - Set the terminal colors
 */
void screen_update_colors(void)
{
}

/**
 * screen_wait(void) - Sleep for approx 16.67ms
 */
void screen_wait(void)
{
}

/**
 * screen_beep(void) - Beep the terminal
 */
void screen_beep(void)
{
  sound(440);
  delay(100);
  nosound();
}

/**
 * screen_clear - Clear the screen
 */
void screen_clear(void)
{
    _clearscreen(_GCLEARSCREEN);
    memset(&palette,-1,sizeof(palette));

    palette[0]=default_background_color;

    // Avoid duplicating palette entry if foreground/background colors are the same.
    if (default_background_color!=default_foreground_color)
      palette[1]=default_foreground_color;
   
    _remapallpalette(palette);
    screen_palette_dump();
}

/**
 * screen_block_draw(Coord1, Coord2) - Perform a block fill from Coord1 to Coord2
 */
void screen_block_draw(padPt* Coord1, padPt* Coord2)
{

  // Special case for mono.
  if (is_mono==1)
    {
      if (CurMode==ModeErase || CurMode==ModeInverse)
	{
	  _setcolor(default_background);
	}
      else
	{
	  _setcolor(default_foreground);
	}
      _rectangle(_GFILLINTERIOR,scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y]);
      return;
    }
  
  if (CurMode==ModeErase || CurMode==ModeInverse)
    {
      if (palette[default_background]==-1)
	_setcolor(0);
      else
	_setcolor(default_background);
    }
  else
    {
      if (palette[default_foreground]==-1)
	_setcolor(1);
      else
	_setcolor(default_foreground);
    }
  _rectangle(_GFILLINTERIOR,scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y]);
}

/**
 * screen_dot_draw(Coord) - Plot a mode 0 pixel
 */
void screen_dot_draw(padPt* Coord)
{

  // Special case for mono.
  if (is_mono==1)
    {
      if (CurMode==ModeErase || CurMode==ModeInverse)
	  _setcolor(default_background);
      else
	  _setcolor(default_foreground);
      _setpixel(scalex[Coord->x],scaley[Coord->y]);
      return;
    }
  
  if (CurMode==ModeErase || CurMode==ModeInverse)
    {
      if (palette[default_background]==-1)
	_setcolor(0);
      else
	_setcolor(default_background);
    }
  else
    {
      if (palette[default_foreground]==-1)
	_setcolor(1);
      else
	_setcolor(default_foreground);
    }

  _setpixel(scalex[Coord->x],scaley[Coord->y]);

}

/**
 * screen_line_draw(Coord1, Coord2) - Draw a mode 1 line
 */
void screen_line_draw(padPt* Coord1, padPt* Coord2)
{
  unsigned short x1=scalex[Coord1->x];
  unsigned short x2=scalex[Coord2->x];
  unsigned short y1=scaley[Coord1->y];
  unsigned short y2=scaley[Coord2->y];

  // Special case for mono.
  if (is_mono==1)
    {
      if (CurMode==ModeErase || CurMode==ModeInverse)
	_setcolor(default_background);
      else
	_setcolor(default_foreground);
      _moveto(x1,y1);
      _lineto(x2,y2);
      return;
    }
  
  if (CurMode==ModeErase || CurMode==ModeInverse)
    {
      if (palette[default_background]==-1)
	_setcolor(0);
      else
	_setcolor(default_background);
    }
  else
    {
      if (palette[default_foreground]==-1)
	_setcolor(1);
      else
	_setcolor(default_foreground);
    }
  
    _moveto(x1,y1);
    _lineto(x2,y2);
}

/**
 * screen_char_draw(Coord, ch, count) - Output buffer from ch* of length count as PLATO characters
 */
void screen_char_draw(padPt* Coord, unsigned char* ch, unsigned char count)
{
  short offset; /* due to negative offsets */
  unsigned short x;      /* Current X and Y coordinates */
  unsigned short y;
  unsigned short* px;   /* Pointers to X and Y coordinates used for actual plotting */
  unsigned short* py;
  unsigned char i; /* current character counter */
  unsigned char a; /* current character byte */
  unsigned char j,k; /* loop counters */
  unsigned char b; /* current character row bit signed */
  unsigned char width=FONT_SIZE_X;
  unsigned char height=FONT_SIZE_Y;
  unsigned short deltaX=1;
  unsigned short deltaY=1;
  unsigned char mainColor=1;
  unsigned char altColor=0;
  unsigned char *p;
  unsigned char* curfont;
  char tmp[32];

  switch(CurMem)
    {
    case M0:
      curfont=font;
      offset=-32;
      break;
    case M1:
      curfont=font;
      offset=64;
      break;
    case M2:
      curfont=fontm23;
      offset=-32;
      break;
    case M3:
      curfont=fontm23;
      offset=32;
      break;
    }

  if (CurMode==ModeRewrite)
    {
      altColor=default_background;
    }
  else if (CurMode==ModeInverse)
    {
      altColor=default_foreground;
    }

  if (CurMode==ModeErase || CurMode==ModeInverse)
    mainColor=default_background;
  else
    mainColor=default_foreground;

  if (is_mono==0)
    {
      if (palette[mainColor]==-1)
	mainColor=0;
      
      if (palette[altColor]==-1)
	altColor=0;
    }
  
  _setcolor(mainColor);

  x=scalex[(Coord->x&0x1FF)];
  if (ModeBold)
    y=scaley[(Coord->y)+30&0x1FF];
  else
    y=scaley[(Coord->y)+15&0x1FF];

  if (FastText==padF)
    {
      goto chardraw_with_fries;
    }

  y=scaley[(Coord->y)+14&0x1FF];

  /* the diet chardraw routine - fast text output. */

  for (i=0;i<count;++i)
    {
      a=*ch;
      ++ch;
      a+=offset;
      p=&curfont[fontptr[a]];

      for (j=0;j<FONT_SIZE_Y;++j)
        {
          b=*p;

          for (k=0;k<FONT_SIZE_X;++k)
            {
              if (b&0x80) /* check sign bit. */
                {
                  _setcolor(mainColor);
                  _setpixel(x,y);
                }

              ++x;
              b<<=1;
            }

          ++y;
          x-=width;
          ++p;
        }

      x+=width;
      y-=height;
    }

  return;

 chardraw_with_fries:  
  if (Rotate)
    {
      deltaX=-abs(deltaX);
      width=-abs(width);
      px=&y;
      py=&x;
    }
    else
    {
      px=&x;
      py=&y;
    }

  if (ModeBold)
    {
      deltaX = deltaY = 2;
      width<<=1;
      height<<=1;
    }

  for (i=0;i<count;++i)
    {
      a=*ch;
      ++ch;
      a+=offset;
      p=&curfont[fontptr[a]];
      for (j=0;j<FONT_SIZE_Y;++j)
        {
          b=*p;

          if (Rotate)
            {
              px=&y;
              py=&x;
            }
          else
            {
              px=&x;
              py=&y;
            }

          for (k=0;k<FONT_SIZE_X;++k)
            {
              if (b&0x80) /* check sign bit. */
                {
                  _setcolor(mainColor);
                  if (ModeBold)
                    {
                      _setpixel(*px+1,*py);
                      _setpixel(*px,*py+1);
                      _setpixel(*px+1,*py+1);
                    }
                  _setpixel(*px,*py);
                }
              else
                {
                  if (CurMode==ModeInverse || CurMode==ModeRewrite)
                    {
                      _setcolor(altColor);
                      if (ModeBold)
                        {
                          _setpixel(*px+1,*py);
                          _setpixel(*px,*py+1);
                          _setpixel(*px+1,*py+1);
                        }
                      _setpixel(*px,*py);
                    }
                }

              x += deltaX;
              b<<=1;
            }

          y+=deltaY;
          x-=width;
          ++p;
        }

      Coord->x+=width;
      x+=width;
      y-=height;
    }

  return;

}

/**
 * screen_tty_char - Called to plot chars when in tty mode
 */
void screen_tty_char(padByte theChar)
{
  if ((theChar >= 0x20) && (theChar < 0x7F)) {
    screen_char_draw(&TTYLoc, &theChar, 1);
    TTYLoc.x += CharWide;
  }
  else if ((theChar == 0x0b)) /* Vertical Tab */
    {
      TTYLoc.y += CharHigh;
    }
  else if ((theChar == 0x08) && (TTYLoc.x > 7)) /* backspace */
    {
      _setcolor(0);
      TTYLoc.x -= CharWide;
      _rectangle(_GFILLINTERIOR,scalex[TTYLoc.x],scaley[TTYLoc.y],scalex[TTYLoc.x+CharWide],scaley[TTYLoc.y+CharHigh]);
    }
  else if (theChar == 0x0A)                     /* line feed */
    TTYLoc.y -= CharHigh;
  else if (theChar == 0x0D)                     /* carriage return */
    TTYLoc.x = 0;

  if (TTYLoc.x + CharWide > 511) {      /* wrap at right side */
    TTYLoc.x = 0;
    TTYLoc.y -= CharHigh;
  }

  if (TTYLoc.y < 0) {
    screen_clear();
    TTYLoc.y=495;
  }

}

/**
 * get screen color index matching RGB color for all other palette based modes
 */
short screen_color(padRGB* theColor)
{
  short index=screen_color_matching(theColor);
  palette[index]=screen_color_transform(theColor);
  _remapallpalette(palette);
#ifdef __PALETTE_DEBUG__
  screen_palette_dump();
#endif
  return index;
}

/**
 * get screen color for mono displays
 */
short screen_color_mono(padRGB* theColor)
{
  if ((theColor->red == 0x00) &&
      (theColor->green == 0x00) &&
      (theColor->blue == 0x00))
    {
      return default_background_color;
    }
  return default_foreground_color;
}

/**
 * screen_foreground - Called to set foreground color.
 */
void screen_foreground(padRGB* theColor)
{
  short index;
  if (is_mono==1)
    {
      screen_color_mono(theColor);
      return;
    }
  // otherwise, handle via palette based color setting.
  index=screen_color(theColor);
  default_foreground=index;
  default_foreground_color=screen_color_transform(theColor);
  screen_palette_dump();
}

/**
 * screen_background - Called to set foreground color.
 */
void screen_background(padRGB* theColor)
{
  short index;
  if (is_mono==1)
    {
      screen_color_mono(theColor);
      return;
    }
  // otherwise, handle via palette based color setting.
  index=screen_color(theColor);
  default_background=index;
  default_background_color=screen_color_transform(theColor);
  screen_palette_dump();
}

/**
 * screen_paint - Called to paint at location.
 */
void screen_paint(padPt* Coord)
{
  _setcolor(default_foreground);
  _floodfill(scalex[Coord->x],scaley[Coord->y],-1);
}

/**
 * screen_color_transform - Transform 24-bit to 18-bit palette value
 */
long screen_color_transform(padRGB* theColor)
{
  unsigned long newRed;
  unsigned long newGreen;
  unsigned long newBlue;
  unsigned long transformedColor;
  
  newRed=theColor->red*0.25;
  newGreen=theColor->green*0.25;
  newBlue=theColor->blue*0.25;
  
  transformedColor=(newRed)|(newGreen<<8)|(newBlue<<16);
  return transformedColor;
}

/**
 * screen_color_matching - Find color index matching value.
 */
short screen_color_matching(padRGB* theColor)
{
  int i=0;
  long transformedColor=screen_color_transform(theColor);
  for (i=0;i<256;++i)
    {
      if (palette[i]==transformedColor)
	return i;
      else if (palette[i]==-1)
	return i;
    }
  return -1;
}

/**
 * screen_palette_dump
 */
void screen_palette_dump(void)
{
  /* unsigned char i; */
  /* short x=0; */
  /* char tmp[32]; */
  /* for (i=0;i<16;++i) */
  /*   { */
  /*     _setcolor(i); */
  /*     _rectangle(_GFILLINTERIOR,x,20,x+16,20+16); */
  /*     x+=8; */
  /*   } */

  /* _settextposition(10,1); */
  /* sprintf(tmp,"b: %02d f: %02d",default_background,default_foreground); */
  /* _outtext(tmp); */
  /* _settextposition(11,1); */
  /* sprintf(tmp,"[1] = 0x%08x",palette[1]); */
  /* _outtext(tmp); */
  /* _settextposition(12,1); */
  /* sprintf(tmp,"[2] = 0x%08x",palette[2]); */
  /* _outtext(tmp); */
}

/**
 * screen_done()
 * Close down TGI
 */
void screen_done(void)
{
  _setvideomode(_DEFAULTMODE);
}
