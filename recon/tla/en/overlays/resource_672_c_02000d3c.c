// Differ 6 of 116 (tla-en, 2026-11-08, -mtune=gba).
// 7 November (permuter): alpha before blend, and fade reused for cue 298 after the loop,
// which makes fade a global pseudo in r0 as in the native (16 to 6). Left: the division's
// copy of next ties to fade in r0 where the native gives it r3.
// Before: differ 16 of 116 (tla-en, 5 November): the leader steps back and the map's two scrolling layers sink while the blend fades.
// The fade's form: next = step + 1; fade = next / 4; blend = fade << 8 before alpha = 16 - fade; blend |= alpha.
// Left, one cause: next (4 refs over 12) must rank above fade (3 over 4), for next r1, fade r0; here fade r1, next r0.
// A constant local for the 16 set before the division leaves 8 but moves the 16. Func_02000be8 and Func_02001ba8 share it.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "system/io_reg.h"

void Func_02001da0(void);
void Func_02001da8(void);
void Func_02001ee8(s32 skip);
void Func_02001d60(void);
void Actor_SetSpeedParameters(u32 id, s32 speed, s32 acceleration);
void Actor_OffsetPositionAndResetMotion(u32 id, s32 dx, s32 dz);
void Actor_WaitForMoveById(u32 id);
void Func_02001e70(u32 id, s32 angle);
void Func_02001f10(s32 cue);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
struct Actor *Actor_GetById(u32 id);
void GameFlag_SetBit(s32 flag);

void Func_02000d3c(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *lower = map + 0x108;
  u8 *upper = map + 0x178;
  s32 i, step, fade, alpha;

  Func_02001da0();
  Func_02001ee8(0);
  Func_02001d60();
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_OffsetPositionAndResetMotion(gMainRoutineWork.currentOwner, 0, -8);
  Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Func_02001e70(gMainRoutineWork.currentOwner, 0x4000);
  Func_02001f10(308);
  step = 1;
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  for (i = 0; i <= 351; i++) {
  if ((u32)(i - 17) <= 62) {
    s32 next = step + 1;
    s32 blend;

    fade = next / 4;
    alpha = 16 - fade;
    blend = fade << 8;
    blend |= alpha;
    REG_BLDALPHA = blend;
    step = next;
  }
  *(s32 *)(lower + 12) += 0x10000;
  *(s32 *)(upper + 12) += 0x10000;
  wait(1);
  }
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  // fade is reused for the cue; with its own constant the blend loses r0 (18).
  fade = 298;
  Func_02001f10(fade);
  wait(1);
  Func_02001f10(125);
  FieldEvent_Wait(20);
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 1;
  GameFlag_SetBit(0x206);
  GameFlag_SetBit(0x207);
  Func_02001da8();
}
