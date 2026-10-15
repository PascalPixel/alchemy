#ifndef ALCHEMY_KEYSTATE_H
#define ALCHEMY_KEYSTATE_H

#include "types.h"

// The keypad state. The VBlank interrupt rewrites these cells, so like the
// hardware registers they are volatile: every read goes back to memory.
/* FAKEMATCH: volatile makes every access to this cell a load or store of its
 * own. Measured 2026-10-12 with alchemy diff on tbs-en: without it 18 functions
 * differ, most UiWork_StepChannelScript (249), Clear_CheckButtonCodes (134) and
 * BattleEv_RunWait (58). */
extern volatile u32 gKeysHeld; // keys down now
/* FAKEMATCH: volatile, as gKeysHeld above: without it 41 functions differ in
 * tbs-en, most PsynergyMenu_RunList (299), PsynergyMenu_SelectAction (261) and
 * BattleMenu_RunActionSelection (135). */
extern volatile u32 gKeysPressed; // keys that went down this frame
/* FAKEMATCH: volatile, as gKeysHeld above: without it 54 functions differ in
 * tbs-en, most CharacterSelector_RunRearrange (288), PsynergyMenu_SelectAction
 * (236) and Debug_RunPaletteEditor (233). */
extern volatile u32 gKeysRepeat; // pressed keys, repeating while held
/* FAKEMATCH: volatile, as gKeysHeld above: without it
 * Input_UpdateKeyRepeatAndDirection: differ 70 in tbs-en, 117 instructions for
 * 121. */
extern volatile u32 gKeysRepeatDirected; // gKeysRepeat with one D-pad direction
/* FAKEMATCH: volatile, as gKeysHeld above: without it
 * Input_UpdateKeyRepeatAndDirection: differ 52 in tbs-en, 117 instructions for
 * 121. */
extern volatile u32
    gKeysDirection; // the D-pad direction gKeysRepeatDirected keeps

// Cells whose rereads the bytes do not show
extern u32 gKeysPressedLatch; // presses since a reader last cleared it
extern u32 gKeysHeldLast;     // gKeysHeld when the frame loop last read it
extern u32 gKeysHeldAtVBlank; // gKeysHeld at the previous VBlank
extern s32 gKeyRepeatTimer;   // frames until the held keys repeat

#endif
