#ifndef ALCHEMY_KEYSTATE_H
#define ALCHEMY_KEYSTATE_H

#include "types.h"

// キー入力の状態。VBlank 割り込みが書き換えるので、ハードウェア
// レジスタと同じく volatile で宣言する。
extern volatile u32 gKeysHeld;	 // 押しているキー
extern volatile u32 gKeyState;	 // 押したキー
extern volatile u32 gKeysRepeat; // リピート付きの押したキー

extern u32 gKeysPressedLatch;
extern u32 gKeysRepeatDirected;
extern volatile u32 gKeysDirection;
extern u32 gKeysHeldLast;
extern u32 gKeysHeldAtVBlank;
extern s32 gKeyRepeatTimer;

#endif
