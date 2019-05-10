#include "esp_system.h"
#include <string.h>

#define Boolean uint8_t
#define EventType int
#define Int16 int16_t
#define UInt8 uint8_t
#define UInt16 uint16_t
#define UInt32 uint32_t
#define DWord uint32_t
#define Word uint16_t
#define Ptr int
#define BitmapPtr int
#define MemPtr int
#define EventPtr int
#define FormType int
#define FormPtr int
#define SystemPreferencesType int

#define keyDownEvent 0
#define appStopEvent 1
#define ctlRepeatEvent 2
#define ctlSelectEvent 3
#define penDownEvent 4
#define penMoveEvent 5
#define menuChr 1
#define slOff 0


#define sysFtrNumROMVersion 1
#define sysFtrCreator 1
#define sysAppLaunchCmdNormalLaunch 1
#define scrDisplayModeGet 1
#define scrDisplayModeGetSupportedDepths 1
#define scrDisplayModeSet 1
#define noPreferenceFound 1
#define frmLoadEvent 1
#define frmOpenEvent 2
//#define CREATOR 1

#define Err int
#define LocalID int
#define DmOpenRef int
#define MemHandle int
#define DmSearchStateType int

#define MemPtrNew(size) calloc(size, 1)
#define MemPtrFree(pointer) free(pointer)
#define MemSet(a,b,c) memset(a,c,b)
#define MemMove(a,b,c) memmove(a,b,c)
#define MemHandleLock(a) 1
#define MemPtrUnlock(a)

#define StrCopy(a,b) strcpy(a,b)
#define StrCat(a,b) strcat(a,b)
#define StrLen(a) strlen(a)
#define StrCompare(a,b) strcmp(a,b)
#define StrPrintF(a,b,c) sprintf(a,b,c)

#define FrmCustomAlert(a,b,c,d) printf(b)
#define FrmAlert(a) printf(a)
#define ErrFatalDisplayIf(a,b) printf(b)

#define WinScreenMode(a,b,c,d,e) 0
#define WinEraseRectangle(a,b)
#define WinDrawChars(a,b,c,d)
#define WinDrawBitmap(a,b,c)

#define FrmDispatchEvent(a)
#define EvtResetAutoOffTimer()

//#define FntLineHeight() 1
//#define FntLineWidth(a,b) 1
//#define FntWordWrap(a,b) 1
#define FntSetFont(a)

#define CtlSetValue(a,b)
#define CtlGetValue(a) 1

#define DmOpenDatabase(a,b,c) 1
#define DmFindDatabase(a,b) 1
#define DmDeleteDatabase(a,b)
#define DmCreateDatabase(a,b,c,e,f)
#define DmCloseDatabase(a)
#define DmNumRecords(a) 1
#define DmNewRecord(a,b,c) 1
#define DmGetRecord(a,b) 1
#define DmWrite(a,b,c,d)
#define DmReleaseRecord(a,b,c)
#define DmDatabaseInfo(a,b,c,d,e,f,g,h,i,j,k,l,m)
#define DmGetNextDatabaseByTypeCreator(a,b,c,d,e,f,g) 1
#define DmGetResource(a,b) 1
#define DmReleaseResource(a)

#define SysTaskDelay(a)
#define SysTicksPerSecond() 1
#define SysRandom(a) esp_random()
#define SysLibRemove(a)

//#define do_scarabs()
#define db_beam(a)


#define sysGetROMVerMajor(dwVer) 1

#define FtrGet(a,b,c)
#define ScrDisplayMode(a,b,c,d,e)
#define PrefGetAppPreferences(a,b,c,d,e) 1
#define SysLibFind(a,b) 1
#define SysLibLoad(a,b,c) 1

#define SysHandleEvent(a) 1
#define MenuHandleEvent(a,b,c) 1

#define FrmGetObjectPtr(a,b) 1
#define FrmGetObjectIndex(a,b) 1
#define FrmDoDialog(a) 1
#define FrmDeleteForm(a)
#define FrmGotoForm(a)
#define PrefGetPreferences(a)
#define EvtGetEvent(a,b)
#define FrmInitForm(a) 1
#define FrmSetActiveForm(a)
#define FrmSetEventHandler(a,b)
#define FrmGetActiveForm() 1
#define FrmDrawForm(a)
#define PrefSetAppPreferences(a,b,c,d,e,f)
#define TimGetTicks() 1

typedef struct point PointType;
typedef struct rectangle RectangleType;

