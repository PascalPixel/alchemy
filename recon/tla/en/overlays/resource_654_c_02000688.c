// 26 October 2026, under -mthumb-slow-loads: differ 2 of 206: the native issues
// a load one slot later after a store, where alchemy-gcc issues it at once;
// alchemy permute (5 to 6 minutes) and an empty asm or one-pass block after any
// one statement do not move it.
// Near miss, differ 2 of 206, from the listing (the twin, ☀️'s
// Scene_RunTransitionCue, is a false match): the blend fades through
// Io_Write16, the rectangle copies through shrine_of_the_sea_god.h. Left: after
// the leader's motion flags are cleared, the native builds
// ObjectMotion_SetActionVariant's 1 before it loads the leader id; here the
// load goes first (equal priority, and the load comes first in insn order).
// Measured: Call2 and Value2 (2), r1 pinned (2), the 1 held by an empty asm
// (fixes it, but the rectangle copy's arguments before it reorder: 4), a second
// barrier anywhere (4). Tried 2026-10-19: the 1 pinned to r1 and the id to r0
// in that order (2), the 1 behind an asm volatile with or without a memory
// clobber, or with the owner as a "+m" operand (4, 4 and 40), the first
// rectangle copy called directly (11).
#include "../../../../games/tla/src/scene/shrine_of_the_sea_god/shrine_of_the_sea_god.h"
#include "io_reg.h"
#include "iowrite.h"
#include "call.h"

void Func_020039a0(void);
void Func_020039b8(s32 a, s32 b, s32 c);
void Func_02003ad8(s32 value);
void Func_02000568(void);
void Func_02000618(void);
void Func_0200063c(s32 x, s32 y);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02000688(void) {
  u32 i;

  Func_020039d8();
  Func_02003b08(0);
  Func_020039a0();
  ObjectMotion_ArmCallback(gPartyState.currentOwner, 0x4000, 0);
  ShrineOfTheSeaGod_CamMove(248 << 16, -1, 168 << 16, 1);
  Func_02003ac8();
  Func_02003b48(244);
  Io_Write16(0x1001, &REG_BLDALPHA);
  WaitFrames(10);
  Io_Write16(0x1002, &REG_BLDALPHA);
  WaitFrames(10);
  Io_Write16(0x1003, &REG_BLDALPHA);
  WaitFrames(10);
  Io_Write16(0x1004, &REG_BLDALPHA);
  WaitFrames(10);
  Call3((void (*)())Func_020039b8, 0, 0x10000, 0x10000);
  ShrineOfTheSeaGod_CopyRect(59, 124, 9, 12, 12, 3);
  Object_GetById(gPartyState.currentOwner)->motionFlags = 0;
  ObjectMotion_SetActionVariant(gPartyState.currentOwner, 1);
  ObjectMotion_SetActionVariant(8, 1);
  ObjectMotion_SetActionVariant(9, 1);
  ObjectMotion_SetActionVariant(10, 1);
  ShrineOfTheSeaGod_CopyRect(46, 51, 6, 56, 18, 25);
  WaitFrames(10);
  Call3((void (*)())Func_020039b8, 0x10000, 0x10000, 0x10000);
  Func_0200063c(65, 51);
  Func_0200063c(65, 58);
  Io_Write16(0xf05, &REG_BLDALPHA);
  Func_0200063c(65, 65);
  Func_0200063c(65, 72);
  Io_Write16(0xe06, &REG_BLDALPHA);
  Func_0200063c(65, 79);
  Func_0200063c(65, 86);
  Io_Write16(0xd07, &REG_BLDALPHA);
  Func_0200063c(65, 93);
  Func_0200063c(65, 100);
  Io_Write16(0xc08, &REG_BLDALPHA);
  Func_0200063c(84, 51);
  Func_0200063c(84, 58);
  Io_Write16(0xb09, &REG_BLDALPHA);
  Func_0200063c(84, 65);
  Func_0200063c(84, 72);
  Io_Write16(0xa0a, &REG_BLDALPHA);
  Func_0200063c(84, 79);
  Func_0200063c(84, 86);
  Io_Write16(0x90b, &REG_BLDALPHA);
  Func_0200063c(84, 93);
  Func_0200063c(84, 100);
  Io_Write16(0x80c, &REG_BLDALPHA);
  for (i = 0; i < 24; i++)
    Func_02000618();
  for (i = 0; i < 24; i++)
    Func_02000568();
  ((struct ShrineOfTheSeaGodEventWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 521;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02003ad8(3);
}
