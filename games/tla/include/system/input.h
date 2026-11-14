#ifndef ALCHEMY_INPUT_H
#define ALCHEMY_INPUT_H

/// @file
/// The Lost Age's key input state: held, pressed and repeating keys.
// The Broken Seal's input state is declared separately in system/input.h.

#include "types.h"
#include "static_assert.h"

struct InputState {
  // volatile: the VBlank handler sets it while the code it interrupts reads
  // it, so it is no device.
  volatile u32 held;
  // volatile: the VBlank handler sets it while the code it interrupts reads
  // it, so it is no device.
  volatile u32 pressed;
  u32 previousVblankHeld;
  // volatile: the VBlank handler sets it while the code it interrupts reads
  // it, so it is no device.
  volatile u32 repeat;
  // volatile: the key repeat rebuilds and reads it each frame, so it is no
  // device.
  volatile u32 direction;
  // volatile: the key repeat updates and reads it each frame, so it is no
  // device.
  volatile u32 previousDirection;
  u32 previousHeld;
  u32 pendingPressed;
  s32 repeatTimer;
};

STATIC_ASSERT(sizeof(struct InputState) == 0x24);
STATIC_ASSERT(offsetof(struct InputState, held) == 0);
STATIC_ASSERT(offsetof(struct InputState, pressed) == 4);
STATIC_ASSERT(offsetof(struct InputState, repeat) == 12);
STATIC_ASSERT(offsetof(struct InputState, repeatTimer) == 32);

extern struct InputState gInput;
void Input_UpdateKeyRepeatAndDirection(void);

#endif
