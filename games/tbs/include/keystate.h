#ifndef ALCHEMY_KEYSTATE_H
#define ALCHEMY_KEYSTATE_H

#include "types.h"

#include "menu_list.h"
#define gKeyState (*(volatile u32 *)&gKeyState)
#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)

extern u32 gKeysHeld;
extern u32 gKeysPressedLatch;
extern u32 gKeysRepeatDirected;
extern u32 gKeysDirection;
extern u32 gKeysHeldLast;
extern u32 gKeysHeldAtVBlank;
extern s32 gKeyRepeatTimer;

#endif
