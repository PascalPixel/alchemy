#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct ControlEvent {
  u8 unk000[362];
  u16 cue;
};

void Func_080e3d04(void);
void Func_080e4a34(void);
void Func_080e572c(void);
void Func_080e5d5c(void);
void Func_080e68d0(void);
void Func_080e7844(void);
void Func_080e9af4(void);
void Func_080e74d8(void);
void BattleFx_PrepareBufferInterpolation(void);
void WaitFrames(s32 frames);
s32 EventRuntime_GetControlledOwner(void);

void Func_080e7804(void) {
  Func_080e74d8();
  BattleFx_PrepareBufferInterpolation();
  WaitFrames(10);
}

void Func_080e7818(void) {}

// Field Psynergy kind 30: looks up the controlled owner's object and sets
// the event work's cue to 0xfc89.
void BattleFx_RunEffect30(void) {
  ObjectTable_Get(EventRuntime_GetControlledOwner());
  ((struct ControlEvent *)Ram_HeapSlots->eventWork)->cue = 0xfc89;
}

// Field Psynergy kind 22 on a chosen target: runs kind 22's animation.
void BattleFx_CallEffect22(void) { Func_080e7844(); }
