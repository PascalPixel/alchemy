#ifndef ALCHEMY_KEYSTATE_H
#define ALCHEMY_KEYSTATE_H

#include "TYPES.H"

#include "MENU_LIST.H"
#define gKeyState (*(u32 *)&gKeyState)
#define gKeysRepeat (*(u32 *)&gKeysRepeat)

extern u32 gKeysHeld;
extern u32 gKeysPressedLatch;
extern u32 gKeysRepeatDirected;
extern u32 gKeysDirection;
extern u32 gKeysHeldLast;
extern u32 gKeysHeldAtVBlank;
extern s32 gKeyRepeatTimer;

#endif
