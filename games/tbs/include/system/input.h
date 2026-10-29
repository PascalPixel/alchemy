#ifndef ALCHEMY_INPUT_H
#define ALCHEMY_INPUT_H

/// @file
/// The keypad state the VBlank interrupt keeps.
///
/// The VBlank handler reads the keypad into these cells, and
/// Input_UpdateKeyRepeatAndDirection turns them into repeats after each frame
/// (system/system/frame.c). The first three are volatile because the VBlank
/// handler sets them while the code it interrupts reads them, so they are no
/// device; the next two are volatile only to match, and their tags name what
/// differs without it.

#include "types.h"

/// Keys down now.
extern volatile u32 gKeysHeld;
/// Keys that went down this frame.
extern volatile u32 gKeysPressed;
/// Pressed keys, repeating while held: after 19 frames, then every 6.
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
