#ifndef TLA_INPUT_H
#define TLA_INPUT_H

// The Lost Age's key input state: held, pressed and repeating keys.
// The Broken Seal's input state is declared separately in keystate.h.

#include "types.h"
#include "static_assert.h"

struct InputState {
  u32 held;
  u32 pressed;
  u32 previousVblankHeld;
  /* FAKEMATCH: volatile steers The Lost Age only: without it, in tla-en,
   * WaitFrames: differ 57, Menu_WaitForSelectionInput: 24, Func_02000e4c: 17
   * and Menu_SelectionLoop: 15. The six tbs editions build the same bytes
   * without it: no tbs code reads gInput. */
  volatile u32 repeat;
  u32 direction;
  u32 previousDirection;
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
