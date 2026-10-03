#ifndef ALCHEMY_KEYSTATE_H
#define ALCHEMY_KEYSTATE_H

#include "TYPES.H"

/* TBS keeps its input state in separate IWRAM cells. */
extern u32 gKeysHeld;
extern u32 gKeyState;
extern u32 gKeysRepeat;
extern u32 gKeysPressedLatch;
extern u32 Data_03001afc; /* Repeated keys with one direction selected. */
extern u32 Data_03001d04; /* Previous direction mask. */
extern u32 Data_03001cf4; /* Held keys at the previous input update. */
extern u32 Data_03001d0c; /* Held keys at the previous VBlank. */
extern s32 Data_03001b00; /* Key repeat timer. */

#endif
