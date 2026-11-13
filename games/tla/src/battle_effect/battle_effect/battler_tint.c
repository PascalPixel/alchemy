/// @file
/// Tinting a battler, and the routine before it, not yet understood.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "gamepak/far_call/battle.h"
#include "module/battle_effect/battle_effect.h"
#include "module/battle_effect/battle_effect.h"
#include "module/tla.h"

/* VIEW(f14cca8): variable gBgScroll as extern struct FxBgScroll gBgScroll[];
 * bg_scroll.h has extern struct BgScroll gBgScroll[]; tried as the rest of the
 * object declares it, BattleEffect_SetupBlendedDisplay moves: 1 of 57
 * instructions in tla-ja. */
extern struct FxBgScroll gBgScroll__f14cca8[] __asm__("gBgScroll");

/* VIEW(f14cca8): static FillWords_ as FillWords___f14cca8; f13eb70 has static
 * __inline__ void FillWords_(void *destination, s32 size, u32 value) {
 * Iwram_FillWords(destination, size, value); }; tried with the definition the
 * rest of the object has, BattleEffect_SetupBlendedDisplay moves: 3 of 57
 * instructions in tla-ja. */
static __inline__ void FillWords___f14cca8(void *destination, s32 size,
                                           u32 value) {
  // The call in a once-only block, so the entry loads before the size; as
  // the plain call, 6 of 59 instructions of BattleEffect_SetupBlendedDisplay
  // differ in tla-en.
  do {
    Iwram_FillWordsEntry(destination, size, value);
  } while (0);
}

void BattleEffect_SetupBlendedDisplay(void) {
  u8 *battle = Ram_HeapSlots->battleWork;
  void *buffer = Ram_HeapSlots->battleFxWork[1];

  REG_DISPCNT = 1;
  gBgScroll__f14cca8[0].y = 32;
  BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), 24);
  /* FAKEMATCH(wrapper): both fills through FillWords_, which sets each call's
   * own size anew; as direct Iwram_FillWords calls, 43 of 59 instructions
   * differ in tla-en. */
  FillWords___f14cca8(buffer, 0x4000, 0);
  FillWords___f14cca8((void *)0x06004000, 0x4000, 0);
  wait(1);
  REG_BLDALPHA = 0x100e;
  REG_BLDCNT = 0x3f46;
  REG_DISPCNT = 0x7741;
  gCameraSceneParameters[4] = 120;
  Sound_PlayFar(390);
}

struct FxGroup {
  u8 unk00[27];
  u8 count;
  u8 unk1c[12];
  struct FxGroupEntry *entries[4];
};

static __inline__ u8 GroupCount(struct FxGroup *group) { return group->count; }

/// The Broken Seal's BattleEffect_TintBattler as The Lost Age has it: the
/// member count is a byte at 0x1b, the timers sit at 0x78dc of the effect
/// work, and a member's frame is reset only when its parameter changes.
void Func_0814cd48(s32 setId, s32 objectValue, s32 groupValue, s32 stateSlot,
                   s32 stateValue) {
  struct FxSlot *set;
  struct FxGroup *group;
  struct FxTimerWork *state;
  s32 groupIndex;

  set = GetBattleObjectSlotFar(setId);
  state = (struct FxTimerWork *)Ram_HeapSlots->battleFxWork[0];
  groupIndex = 0;

  while ((group = GetMotionRecordFar(set->object, groupIndex)) != NULL) {
    if (stateSlot != -1)
      state->actorTimers[stateSlot] = ((u8 *)&stateValue)[0];
    if (set->fading == 0) {
      if (objectValue != -1) {
        s32 objectIndex;

        objectIndex = 0;
        if (GroupCount(group) != 0) {
          struct FxGroupEntry **objects;

          objects = group->entries;
          do {
            struct FxGroupEntry *object;

            object = *objects++;
            if (object != NULL && object != set->effectEntry &&
                object != set->animationEntry) {
              u8 old = object->param;

              if (objectValue == 0)
                object->param = BattleMotion_GetSlotPaletteFar(setId);
              else
                object->param = objectValue;
              if (object->param != old)
                object->frame = 0xff;
            }
            objectIndex++;
          } while (objectIndex != GroupCount(group));
        }
      }
      if (groupValue != -1)
        Animation_ApplyChildArgumentFar(group, groupValue);
    }
    groupIndex++;
  }
}
