#include "types.h"
#include "ram_buffer.h"
#include "actor.h"

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
void BattleEffect_PrepareBufferInterpolation(void);
void wait(s32 frames);
s32 Party_GetLeaderActor(void);

/// Runs Func_080e74d8, fades the palettes back to the saved ones and waits
/// 10 frames.
void Func_080e7804(void) {
  Func_080e74d8();
  BattleEffect_PrepareBufferInterpolation();
  wait(10);
}

/// Field Psynergy kind 30's targeted handler.
void Func_080e7818(void) {}

/// Field Psynergy kind 30.
void BattleEffect_RunEffect30(void) {
  ObjectTable_Get(Party_GetLeaderActor());
  ((struct ControlEvent *)Ram_HeapSlots->eventWork)->cue = 0xfc89;
}

/// Field Psynergy kind 22 on a chosen target: runs kind 22's animation.
void BattleEffect_CallEffect22(void) { Func_080e7844(); }
