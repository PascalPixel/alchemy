#ifndef ALCHEMY_KEYSTATE_H
#define ALCHEMY_KEYSTATE_H

/// @file
/// The keypad state the VBlank interrupt keeps.
///
/// The VBlank handler reads the keypad into these cells, and
/// Input_UpdateKeyRepeatAndDirection turns them into repeats after each frame
/// (system/system/frame.c). The first five are volatile, a device: each tag
/// names what differs without it.

#include "types.h"

/// Keys down now.
/* FAKEMATCH(volatile): as plain u32, 18 functions differ in tbs-en, most
 * Message_Type (249), FileScreen_CheckButtonCodes (134). */
extern volatile u32 gKeysHeld;
/// Keys that went down this frame.
/* FAKEMATCH(volatile): as plain u32, 41 functions differ in tbs-en, most
 * PsynergyMenu_RunList (299), PsynergyMenu_SelectAction (261). */
extern volatile u32 gKeysPressed;
/// Pressed keys, repeating while held: after 19 frames, then every 6.
/* FAKEMATCH(volatile): as plain u32, 54 functions differ in tbs-en, most
 * PartyMemberChooser_RunRearrange (288), PsynergyMenu_SelectAction (236). */
extern volatile u32 gKeysRepeat;
/// gKeysRepeat with at most one D-pad direction.
/* FAKEMATCH(volatile): as plain u32, 70 of 121 instructions differ in tbs-en
 * (Input_UpdateKeyRepeatAndDirection). */
extern volatile u32 gKeysRepeatDirected;
/// The D-pad direction gKeysRepeatDirected keeps: of several held, the one
/// added last.
/* FAKEMATCH(volatile): as plain u32, 52 of 121 instructions differ in tbs-en
 * (Input_UpdateKeyRepeatAndDirection). */
extern volatile u32 gKeysDirection;

// Cells whose rereads the bytes do not show
extern u32 gKeysPressedLatch; ///< Presses since a reader last cleared it.
extern u32 gKeysHeldLast;     ///< gKeysHeld when the frame loop last read it.
extern u32 gKeysHeldAtVBlank; ///< gKeysHeld at the previous VBlank.
/// Frames until the held keys repeat; -1 makes them repeat at once.
extern s32 gKeyRepeatTimer;

#endif
