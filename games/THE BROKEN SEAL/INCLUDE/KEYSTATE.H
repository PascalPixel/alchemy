#ifndef ALCHEMY_KEYSTATE_H
#define ALCHEMY_KEYSTATE_H

#include "TYPES.H"

/* TBS keeps its input state in separate IWRAM cells. */
extern u32 gKeysHeld;
extern u32 gKeyState;
extern u32 gKeysRepeat;
extern u32 gKeysPressedLatch;
/* The repeated keys with one direction kept. */
extern u32 gKeysRepeatDirected;
/* The direction kept from the last update, as a mask of the D-pad keys. */
extern u32 gKeysDirection;
/* The keys held at the last input update. */
extern u32 gKeysHeldLast;
/* The keys held at the previous VBlank. */
extern u32 gKeysHeldAtVBlank;
/* The frames until held keys repeat. */
extern s32 gKeyRepeatTimer;

#endif
