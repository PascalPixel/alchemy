/// @file
/// Tinting a battler, and the routine before it, not yet understood.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "gamepak/far_call/field.h"

/// Shows the battle background behind a cleared canvas.
void BattleEffect_SetupBlendedDisplay(void) {
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  void *buffer = Ram_HeapSlots->battleCanvas;

  REG_DISPCNT = DISPCNT_MODE_1;
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background, 24);
  IWRAM_FILL_WORDS(buffer, CANVAS_BYTES, 0);
  IWRAM_FILL_WORDS((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES, 0);
  wait(1);
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(14);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_DISPCNT = EFFECT_DISPCNT;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
}

/// How many sprite animations a sprite holds.
static __inline__ u8 GroupCount(struct Sprite *group) { return group->count; }

/// Updates every effect group of one unit's battle object: its palette, its
/// pose (reset only when it changes) and the timer slot's frames.
void BattleEffect_TintBattler(s32 setId, s32 objectValue, s32 groupValue,
                              s32 stateSlot, s32 stateValue) {
  struct BattleObject *set;
  struct Sprite *group;
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
          struct SpriteAnimation **objects;

          objects = group->entries;
          do {
            struct SpriteAnimation *object;

            object = *objects++;
            if (object != NULL && object != set->shadowEntry &&
                object != set->animationEntry) {
              u8 old = object->colorMode;

              if (objectValue == 0)
                object->colorMode = BattleMotion_GetSlotPaletteFar(setId);
              else
                object->colorMode = objectValue;
              if (object->colorMode != old)
                object->picture = SPRITE_PICTURE_NONE;
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
