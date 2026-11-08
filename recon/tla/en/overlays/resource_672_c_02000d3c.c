// Matches: differ 0 (tla-en, 2026-11-08)
// Also differ 0 in tla-ja, tla-de, tla-es, tla-fr and tla-it. The leader steps
// back and the map's two scrolling layers sink while the blend fades.
// 8 November: the last 6 were global allocation's preferences. fade is reused
// for cue 298, so its copy into the argument gives it r0; expand_preferences
// then hands r0 to the division's rounding temp through fade = temp >> 2 (the
// greg dump lists "76 preferences: 0 2"), so the temp took r0 where the listing
// has r3. Kept: next / 4 spelled out as a bias and a shift so the temp can be
// named, and an empty asm reading it after the shift, which keeps it live past
// the shift so the preference is not shared (FAKEMATCH barrier). The temp
// pinned to r3 also gives 0 (a worse kind, not kept). fade as s16 is the one
// plain form that splits the temp from fade (temp r3, fade r0), but its sign
// extension costs 2 instructions (9).
// No milder kind found, all still 6: the spelled-out division alone, a copy of
// the temp before the shift, fade = temp; fade >>= 2, inline helpers for the
// shift or the whole division, (void)& of the temp, fade or blend, an asm tying
// the quotient to fade or rewriting the temp in place. Also no gain: fade,
// alpha or blend as u32 (6), the division repeated in alpha and blend (6).
// Worse: fade as u16 (25) or u8 (29), blend as u16 (26), next as u32 (12), the
// store as one expression (31 to 33), blend before alpha with or without a dead
// fade = 0 (16 to 22), next reused for SetSpritePriority's 1 (46 to 62).
// Earlier (6 to 8 November, no gain): step++ or ++step in place of next,
// (step + 1) / 4, 1 + step, the division through (u32), >> 2 or fade = next;
// fade /= 4, the range as two tests, declaration orders and scopes, 16 minutes
// of permute. The 7 November permuter found the reuse of fade for cue 298 (16
// to 6). Func_02000be8 and Func_02001ba8 have the same rise loop and the same
// r0/r3 gap; this device is untested there.
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
      s32 biased = next;
      s32 blend;

      /* FAKEMATCH(barrier): next / 4 spelled out as a bias toward zero and a
       * shift, with an empty asm reading the biased value after the shift; as
       * fade = next / 4, 6 of 116 instructions differ in tla-en. */
      if (next < 0)
        biased = next + 3;
      fade = biased >> 2;
      asm("" : : "r"(biased));
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
  // fade is reused for the cue; with its own constant the blend loses r0 (22).
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
