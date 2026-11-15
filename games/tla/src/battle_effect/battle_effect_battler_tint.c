/// @file
/// Tinting a battler, and the routine before it, not yet understood.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "gamepak/far_call/field.h"

/* VIEW(f14cca8): static FillWords_ as FillWords___f14cca8; f13eb70 has static
 * __inline__ void FillWords_(void *destination, s32 size, u32 value) {
 * Iwram_FillWords(destination, size, value); }; not tried: FillWords_'s
 * definition holds a FAKEMATCH tag. */
/// Fills words through the IWRAM fill, in a once-only block.
static __inline__ void FillWords___f14cca8(void *destination, s32 size,
                                           u32 value) {
  /* FAKEMATCH(dead): the call in a once-only block. */
  do {
    Iwram_FillWordsEntry(destination, size, value);
  } while (0);
}

/// Shows the battle background behind a cleared canvas.
void BattleEffect_SetupBlendedDisplay(void) {
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  void *buffer = Ram_HeapSlots->battleCanvas;

  REG_DISPCNT = DISPCNT_MODE_1;
  gBgScroll[1].y = 32;
  BattleBackground_LoadFar(1, battle->background, 24);
  /* FAKEMATCH(wrapper): both fills through FillWords_. */
  FillWords___f14cca8(buffer, 0x4000, 0);
  FillWords___f14cca8((void *)BG_CHAR_BLOCK(1), 0x4000, 0);
  wait(1);
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_DISPCNT = EFFECT_DISPCNT;
  gProjection.centerY = 120;
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
}

/// The sprite group behind a battler, as the tint reads its entries.
struct FxGroup {
  u8 unk00[27];
  u8 count;
  u8 unk1c[12];
  struct FxGroupEntry *entries[4];
};

/// How many sprites an effect group holds.
static __inline__ u8 GroupCount(struct FxGroup *group) { return group->count; }

/// Updates every effect group of one unit's battle object: its palette, its
/// pose (reset only when it changes) and the timer slot's frames.
void BattleEffect_TintBattler(s32 setId, s32 objectValue, s32 groupValue,
                              s32 stateSlot, s32 stateValue) {
  struct FxSlot *set;
  struct FxGroup *group;
  struct BattleEffectWork *state;
  s32 groupIndex;

  set = BattleObject_GetFar(setId);
  state = Ram_HeapSlots->battleEffect;
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
        Sprite_StartAnimationFar(group, groupValue);
    }
    groupIndex++;
  }
}
