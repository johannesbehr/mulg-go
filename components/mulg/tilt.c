#include "esp_system.h"
#include "tilt.h"


Err TiltLibOpen(UInt16 refNum){return 0;}
Err TiltLibClose(UInt16 refNum, UInt16 *numappsP){return 0;}
Err TiltLibZero(UInt16 refNum){return 0;}
Err TiltLibGet(UInt16 refNum, UInt16 *x, UInt16 *y){return 0;}
Err TiltLibGetAbs(UInt16 refNum, UInt16 *x, UInt16 *y){return 0;}
