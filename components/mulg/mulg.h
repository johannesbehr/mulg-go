#include "level.h"

/*

*/

#include "typedefs.h"

#define MAX_SPRITES 2  /* 1 to 2 players marble */
#define MARBLE 0
#define MARBLE2 1


#define CREATOR     'Mulg' // 'Mulg'
#define DB_TYPE     'Levl' // 'Levl'
#define DB_TYPE_F   'LevF' // 'LevF' level databases for version IIf and up
#define DB_TYPE_N   'LevN' // 'LevN' level databases for version IIn and up
#define DB_TYPE_P   'LevP' // 'LevP' level databases for version IIp and up
#define DB_SCORE    'Scor' // 'Scor'

#define MAX_WIDTH  37
#define MAX_HEIGHT 33

typedef struct {
  unsigned char gray2[64];
  unsigned char gray4[128];
  unsigned char color[256];
} CUSTOM_TILE;

#ifndef IN_MAKELEVEL
extern CUSTOM_TILE *custom_tiles;
extern Boolean has_newscr;

#define ERROR(a) 

#define ABS(a)  ((a>0)?a:-a)
#define SGN(a)  ((a>0)?1:-1)

typedef struct {
  int   width;
  int   height;
  short *data;
} LEVEL;

#define DEBUG

#ifdef DEBUG
#define DEBUG_PORT 0x30000000
#define DEBUG_MSG(format, args...) { char xyz_tmp[128]; int xyz; StrPrintF(xyz_tmp, format, ## args); for(xyz=0;xyz<StrLen(xyz_tmp);xyz++) *((char*)DEBUG_PORT)=xyz_tmp[xyz]; }
#else
#define DEBUG_MSG(format, args...)
#endif

#endif



uint8_t get_marble_screen_position(uint8_t no, int *screen_x, int *screen_y, int xp, int yp);
void mulg_load();
uint8_t mulg_run(uint8_t js, uint8_t js2);
uint8_t mulg_draw_slave();
void mulg_prev_level();
void mulg_next_level();
void mulg_prev_file();
void mulg_next_file();
void scan_levelfiles();
void draw_startscreen();
void draw_messagebox(char* message);
int sqrt(int a);

