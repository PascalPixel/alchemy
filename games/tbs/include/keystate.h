#ifndef ALCHEMY_KEYSTATE_H
#define ALCHEMY_KEYSTATE_H

#include "types.h"

// キー入力の状態。VBlank 割り込みが書き換えるので、ハードウェア
// レジスタと同じく volatile で宣言する。読むたびにメモリから読み直す。
extern volatile u32 gKeysHeld;			 // 押しているキー
extern volatile u32 gKeyState;			 // 押したキー
extern volatile u32 gKeysRepeat;		 // リピート付きの押したキー
extern volatile u32 gKeysRepeatDirected; // リピート（方向を絞ったもの）
extern volatile u32 gKeysDirection;		 // 絞った方向

// 読み直しがバイトに現れない値
extern u32 gKeysPressedLatch;
extern u32 gKeysHeldLast;
extern u32 gKeysHeldAtVBlank;
extern s32 gKeyRepeatTimer;

#endif
