#ifndef ALCHEMY_KEYSTATE_H
#define ALCHEMY_KEYSTATE_H

#include "types.h"

// キー入力の状態。VBlank 割り込みが書き換えるので、ハードウェア
// レジスタと同じく volatile で宣言する。読むたびにメモリから読み直す。
/* FAKEMATCH: volatile makes every access to this cell a load or store of its own. Measured 2026-10-12 with alchemy diff on tbs-en: without it 18 functions differ, most UiWork_StepChannelScript (249), Clear_CheckButtonCodes (134) and BattleEv_RunWait (58). */
extern volatile u32 gKeysHeld; // 押しているキー
/* FAKEMATCH: volatile, as gKeysHeld above: without it 41 functions differ in tbs-en, most PsynergyMenu_RunList (299), PsynergyMenu_SelectAction (261) and BattleMenu_RunActionSelection (135). */
extern volatile u32 gKeyState; // 押したキー
/* FAKEMATCH: volatile, as gKeysHeld above: without it 54 functions differ in tbs-en, most CharacterSelector_RunRearrange (288), PsynergyMenu_SelectAction (236) and Debug_RunPaletteEditor (233). */
extern volatile u32 gKeysRepeat; // リピート付きの押したキー
/* FAKEMATCH: volatile, as gKeysHeld above: without it Input_UpdateKeyRepeatAndDirection: differ 70 in tbs-en, 117 instructions for 121. */
extern volatile u32 gKeysRepeatDirected; // リピート（方向を絞ったもの）
/* FAKEMATCH: volatile, as gKeysHeld above: without it Input_UpdateKeyRepeatAndDirection: differ 52 in tbs-en, 117 instructions for 121. */
extern volatile u32 gKeysDirection; // 絞った方向

// 読み直しがバイトに現れない値
extern u32 gKeysPressedLatch;
extern u32 gKeysHeldLast;
extern u32 gKeysHeldAtVBlank;
extern s32 gKeyRepeatTimer;

#endif
