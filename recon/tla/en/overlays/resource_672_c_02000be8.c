// Differ 14 of 145 (tla-en, 2026-11-08, -mtune=gba).
// 8 November: 8 minutes of permute reached 12 only by reusing the loop counter i for fade (wrong behaviour, not kept); fade in an inner block 32, step + 1 in its own local 14, without the dead device 24.
// 8 November: cue 298 as a plain constant (fade no longer reused for it) gives 16 to 14;
// the sink loop now matches. Left: the rise loop's step + 1 in r0 and its quotient in r1, the
// native r1 and r0. No gain: the sink loop's forms in the rise loop (14, 23), ++step or step++
// first (23), step++ after the blend (14), the locals' order (14).
// 7 November (permuter): a dead first division in the sink loop (FAKEMATCH dead)
// puts the sink loop's step + 1 in r3 as the native (20 to 16). Left: in both loops
// the division's copy ties to fade in r0 (native r3 and r2; sink fade r1).
// 7 November, under the older tune (undone 8 November): fade reused for cue 298 after the
// loops made it a global pseudo (24 to 20), as in Func_02000d3c. Left: in both loops the division's copy of
// step + 1 ties to fade (r0) where the native gives it r3 (rise) and r2 (sink).
// Before: differ 24 of 145 (tla-en, 2026-11-06). The sink loop now builds step + 1 in
// blend before dividing, which gives the listing its copy (29 to 24); doing the
// same in the rise loop costs 9. Left: allocation only, r0/r1 swapped in the
// rise loop, r2/r3 in the sink loop. The permuter reaches 20 only by reusing
// fade for currentOwner (a forced temporary), not kept. blend holding step + 1
// and then the register value is itself a reused local: tag it FAKEMATCH
// (temporary) at adoption unless a plain form is found.
// Before: differ 29 (2026-11-06). The 24 of 5 November came from wrong
// C: blend was built from the previous frame's fade, and the sink loop dropped
// 16 - fade. With both written as the listing computes them, left: step + 1 and
// fade swap r0 and r1 in the rise loop, and the sink loop has no copy of step +
// 1 (the listing keeps one, as if step + 1 were a variable there). No gain:
// ++step forms, step-- inside the division, step -= 1, >> 2, BLEND_ALPHA or one
// expression for the register (49 to 78: the 16 is then pooled).
// Under -Os, 5 November: differ 24 of 145 (tla-en); older figures below predate
// it. Draft, differ 75 of 145, skeleton gap 13/16 (tla-en, 2026-11-01)
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
  s32 i, step, fade, blend;

  Func_02001da0();
  Func_02001ee8(0);
  Func_02001d60();
  step = 1;
  i = 0;
rise:
  if (i >= 17 && i < 80) {
    fade = (step + 1) / 4;
    blend = fade << 8;
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
    if (i >= 33 && i < 96) {
      /* FAKEMATCH(dead): fade is first set from step + 1 directly, and then again
       * through blend; as the second alone, 24 of 145 instructions differ in tla-en. */
      fade = (step + 1) / 4;
      blend = step + 1;
      fade = blend / 4;
      blend = fade << 8;
      blend |= 16 - fade;
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
