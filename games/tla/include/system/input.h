#ifndef ALCHEMY_INPUT_H
#define ALCHEMY_INPUT_H

/// @file
/// The keypad state the VBlank interrupt keeps.
///
/// The VBlank handler reads the keypad into gInput, and
/// Input_UpdateKeyRepeatAndDirection turns it into repeats after each frame
/// (system/system_frame.c). held, pressed, repeat and repeatTimer are volatile
/// because the VBlank handler sets them while the code it interrupts reads
/// them; direction and previousDirection the key repeat rebuilds and reads each
/// frame.

#include "types.h"
#include "static_assert.h"

/// The keypad state (gInput): the keys held, newly pressed and repeating, the
/// D-pad direction kept, and the repeat timer.
struct InputState {
  volatile u32 held;      ///< keys down now
  volatile u32 pressed;   ///< keys that went down this frame
  u32 previousVblankHeld; ///< held at the previous VBlank
  /// Pressed keys, repeating while held: after 19 frames, then every 6.
  volatile u32 repeat;
  /// repeat with at most one D-pad direction.
  volatile u32 direction;
  /// The D-pad direction kept: of several held, the one added last.
  volatile u32 previousDirection;
  u32 previousHeld;   ///< held when the frame loop last read it
  u32 pendingPressed; ///< presses since a reader last cleared it
  /// Frames until the held keys repeat; -1 makes them repeat at once.
  volatile s32 repeatTimer;
};

STATIC_ASSERT(sizeof(struct InputState) == 36);
STATIC_ASSERT(offsetof(struct InputState, held) == 0);
STATIC_ASSERT(offsetof(struct InputState, pressed) == 4);
STATIC_ASSERT(offsetof(struct InputState, repeat) == 12);
STATIC_ASSERT(offsetof(struct InputState, repeatTimer) == 32);

extern struct InputState gInput;
void Input_UpdateKeyRepeatAndDirection(void);
/// The keypad interrupt's handler.
void Input_HandleKeyInterrupt(void);

#endif
