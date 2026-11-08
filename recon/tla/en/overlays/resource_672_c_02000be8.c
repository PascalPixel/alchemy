// Matches: differ 0 (tla-en, 2026-11-08)
// Func_02000d3c's companion: the map's two scrolling layers (0x108 and 0x178)
// climb 352 pixels at once while a layer fades in (frames 17 to 79), then
// after a screen transition the leader is placed (cue 308) and the layers sink
// back a pixel a frame while the layer fades out (frames 33 to 95 counted
// down); then cues 298 and 125 and flag 0x206.
// What closed it: each loop writes the level out twice, ((step + 1) / 4) << 8
// and then 16 - (step + 1) / 4, with no local for the quotient. With a local
// (fade) the quotient was local to its block and local-alloc gave it r1 before
// global allocation reached step + 1, so the rise loop swapped r0 and r1 (14);
// the sink loop needed a FAKEMATCH(dead) first division to keep step + 1 apart
// from the division's copy (24 without it). That device is gone; nothing is
// tagged. The loops are plain for loops (the label-and-goto form also matches),
// and blend is block-scoped in each loop (function scope also matches).
// Func_02000d3c and Func_02001ba8 share this blend and the same r0/r1 swap; the
// same form is worth trying there.
#include "types.h"
#include "call.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "system/io_reg.h"

void Func_02001da0(void);
void Func_02001da8(void);
void Func_02001ee8(s32 skip);
void Func_02001d60(void);
void Func_02001d40(void);
void wait(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Actor_SetSpeedParameters(u32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(u32 id, s32 x, s32 z);
void FieldEvent_Wait(s32 frames);
void Func_02001f10(s32 cue);
void Actor_SetSpritePriority(u32 id, s32 variant);
void GameFlag_SetBit(s32 flag);

void Func_02000be8(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *lower = map + 0x108;
  u8 *upper = map + 0x178;
  s32 i, step;

  Func_02001da0();
  Func_02001ee8(0);
  Func_02001d60();
  step = 1;
  for (i = 0; i < 352; i++) {
    if (i >= 17 && i < 80) {
      s32 blend = ((step + 1) / 4) << 8;

      blend |= 16 - (step + 1) / 4;
      REG_BLDALPHA = blend;
      step++;
    }
    *(s32 *)(lower + 12) += 0x10000;
    *(s32 *)(upper + 12) += 0x10000;
  }
  Func_02001d40();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 328, 184);
  FieldEvent_Wait(20);
  Func_02001f10(308);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  for (; i != 0; i--) {
    if (i >= 33 && i < 96) {
      s32 blend = ((step + 1) / 4) << 8;

      blend |= 16 - (step + 1) / 4;
      REG_BLDALPHA = blend;
      step--;
    }
    *(s32 *)(lower + 12) += -0x10000;
    *(s32 *)(upper + 12) += -0x10000;
    wait(1);
  }
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Func_02001f10(298);
  wait(1);
  Func_02001f10(125);
  FieldEvent_Wait(20);
  GameFlag_SetBit(0x206);
  Func_02001da8();
}
