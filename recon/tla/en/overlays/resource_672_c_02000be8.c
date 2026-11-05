// Draft, differ 75 of 145, skeleton gap 13/16 (tla-en, 2026-11-01)
// Func_02000d3c's companion: the map's two scrolling layers (0x108 and 0x178)
// climb 352 pixels at once while a layer fades in (frames 17 to 79), then
// after a screen transition the leader is placed (cue 308) and the layers sink
// back a pixel a frame while the layer fades out (frames 33 to 95 counted
// down); then cues 298 and 125 and flag 0x206.
// Left: one cause. The listing has i r6, step r7, upper r8, lower r10; here
// step loses to both pointers and lands in r10, and every skeleton difference
// is that high register (mov copies, step-- as negs/add, one extra reload).
// The lreg dump shows why: step scores 3 * 15 / 146 = .308, upper 3 * 9 / 84 =
// .321. update_equiv_regs doubles step's live length (73 to 146) because
// step = 1 is its first set and carries a constant REG_EQUAL note; later sets
// do not undo that. It takes one more ref (16 gives floor_log2 4, .438), an
// undoubled length, or upper at 88 insns or more to flip the order. Counting
// r7 in the listing gives exactly 15 refs at depth 1.
// Tried, no gain: real for or while (for hoists the constants, differ 136;
// while on the second loop only, differ 82), i = 0 first, step = i + 1, step
// initialised at its declaration, declaration orders, fade and blend in inner
// scopes per loop. A separate copy of step after the first loop puts the
// pointers right (upper r8, lower r10) but swaps i and step and leaves a copy,
// and it would be a forced temporary, so it is not kept.
// Closed: the blend value is built before the register's address, which puts
// the address in r3 after the orrs as the listing does. blend replaces the
// earlier alpha local, so the local count is unchanged. A reviewer may want to
// judge blend against S2 at adoption.
// Also open: fade is r0 in the first loop and r1 in the second, so the listing
// may hold two variables; split scopes cost 2 here while step is high.
#include "types.h"
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
  s32 i, step, fade, blend;

  Func_02001da0();
  Func_02001ee8(0);
  Func_02001d60();
  step = 1;
  i = 0;
rise:
  if ((u32)(i - 17) <= 62) {
    blend = fade << 8;
    fade = (step + 1) / 4;
    blend |= 16 - fade;
    REG_BLDALPHA = blend;
    step++;
  }
  *(s32 *)(lower + 12) += 0x10000;
  *(s32 *)(upper + 12) += 0x10000;
  i++;
  if (i < 352)
    goto rise;
  Func_02001d40();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 328, 184);
  FieldEvent_Wait(20);
  Func_02001f10(308);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  if (i != 0) {
  sink:
    if ((u32)(i - 33) <= 62) {
      fade = (step + 1) / 4;
      blend |= 16 - fade;
      blend = fade << 8;
      REG_BLDALPHA = blend;
      step--;
    }
    *(s32 *)(lower + 12) += -0x10000;
    *(s32 *)(upper + 12) += -0x10000;
    wait(1);
    i--;
    if (i != 0)
      goto sink;
  }
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
  Func_02001f10(298);
  wait(1);
  Func_02001f10(125);
  FieldEvent_Wait(20);
  GameFlag_SetBit(0x206);
  Func_02001da8();
}
