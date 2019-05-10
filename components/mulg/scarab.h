/* 
  Scarab bots code.

   (c) 1999    Pat Kane
*/

#pragma once

#include "esp_system.h"

extern unsigned int marble_x[],  marble_y[];
extern unsigned int marble_xp[],  marble_yp[];
extern          int marble_sx[],  marble_sy[];
extern unsigned char  level_buffer[MAX_HEIGHT][MAX_WIDTH];  
extern unsigned char attribute_buffer[MAX_HEIGHT][MAX_WIDTH];
extern unsigned short level_width, level_height;
extern void change_tile(unsigned short tile, int attribute, int x, int y);
extern void snd_clic(void);
extern char debug_msg[];
extern void swap_tile(unsigned short tile1, unsigned short tile2, int x, int y);
extern void switch_it(int id);

#define DebugPr(format, args...) {StrPrintF(debug_msg, format, ## args);}

#define MAX_SCARABS 4
#define N_JITTERBUG 3                     /* how soon bug gets bored    */
#define MAX_JITTER  ((N_JITTERBUG*2)-1)   /* do let bug get too wired   */
#define BUGACC      300                   /* how hard bug pushes marble */

struct Scarab
{
  int  x,  y;
  int  vx, vy;
  int  jitter;     /* boredom count */
  char alive;
  int  hold;       /* can hold one thing */
} scarabs[MAX_SCARABS];

void init_scarabs( );
void add_scarab( int x, int y );
void do_scarabs();
int sign(int v);
