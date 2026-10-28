// Draft, differ 32 of 116: inside an event, the leader steps back 8 pixels and
// turns (cue 308) while the map's two scrolling layers (the map work's 0x108
// and 0x178) climb a pixel a frame for 352 frames and, from frame 17 to 79,
// the blend fades a layer in; then cues 298 and 125, the leader is put on
// layer 1 and flags 0x206 and 0x207 are set. Left: the native writes the blend after building both halves, with the count in r1, and tests the 352 frames against 351 where this draft compares with 352.
#include "types.h"
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
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  step = 1;
  i = 0;
climb:
  if ((u32)(i - 17) <= 62) {
    fade = (step + 1) / 4;
    alpha = 16 - fade;
    REG_BLDALPHA = (fade << 8) | alpha;
    step++;
  }
  *(s32 *)(lower + 12) += 0x10000;
  *(s32 *)(upper + 12) += 0x10000;
  wait(1);
  i++;
  if (i <= 351)
    goto climb;
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Func_02001f10(298);
  wait(1);
  Func_02001f10(125);
  FieldEvent_Wait(20);
  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 1;
  GameFlag_SetBit(0x206);
  GameFlag_SetBit(0x207);
  Func_02001da8();
}
