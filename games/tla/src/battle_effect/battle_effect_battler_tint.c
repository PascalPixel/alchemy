/// @file
/// Bringing the battle back behind a cleared canvas, and tinting a battler.
// Part of battle_effect.c.

#include "battle_effect_internal.h"
#include "sounds.h"
#include "gamepak/far_call/battle.h"
#include "gamepak/far_call/field.h"

/// Shows the battle background, as dark as the canvas's fade left it, behind a
/// cleared canvas, and fades out the action's sound effects.
void BattleEffect_SetupBlendedDisplay(void) {
  struct BattleState *battle = Ram_HeapSlots->battleState;
  u8 *canvas = Ram_HeapSlots->battleCanvas;

  REG_DISPCNT = DISPCNT_MODE_1;
  gBgScroll[1].y = BATTLE_BACKGROUND_SCROLL_Y;
  BattleBackground_LoadFar(BATTLE_BACKGROUND_FIXED, battle->background,
                           BACKGROUND_DARKENED_LEVEL);
  IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
  IWRAM_FILL_WORDS((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES, 0);
  wait(1);
  REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
  REG_BLDCNT =
      BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_DISPCNT = EFFECT_DISPCNT;
  gProjection.centerY = BATTLE_PROJECTION_CENTER_Y;
  Sound_PlayFar(SOUND_FADE_OUT_ACTION_SE);
}

/// Tints one unit's battle object and sets its pose, for every sprite it is
/// made of, and starts that unit's flash timer.
///
/// palette: a SpriteColorMode, SPRITE_COLOR_NORMAL for the unit's own palette,
///          or TINT_KEEP.
/// animation: the pose to start, or TINT_KEEP.
/// timerSlot: the state->actorTimers entry to set to timerFrames, or TINT_KEEP.
void BattleEffect_TintBattler(s32 unitId, s32 palette, s32 animation,
                              s32 timerSlot, s32 timerFrames) {
  struct BattleObject *slot;
  struct Sprite *group;
  struct BattleEffectState *state;
  s32 groupIndex;

  slot = BattleObject_GetFar(unitId);
  state = Ram_HeapSlots->battleEffect;

  for (groupIndex = 0;
       (group = Actor_GetSpriteFar(slot->object, groupIndex)) != NULL;
       groupIndex++) {
    if (timerSlot != TINT_KEEP)
      state->actorTimers[timerSlot] = timerFrames;
    if (slot->fading == 0) {
      if (palette != TINT_KEEP) {
        s32 partIndex;

        for (partIndex = 0; partIndex != group->count; partIndex++) {
          struct SpriteAnimation *part = group->entries[partIndex];

          if (part != NULL && part != slot->shadowEntry &&
              part != slot->animationEntry) {
            u8 oldPalette = part->colorMode;

            if (palette == SPRITE_COLOR_NORMAL)
              part->colorMode = BattleMotion_GetSlotPaletteFar(unitId);
            else
              part->colorMode = palette;
            if (part->colorMode != oldPalette)
              part->picture = SPRITE_PICTURE_NONE;
          }
        }
      }
      if (animation != TINT_KEEP)
        Sprite_StartAnimationFar(group, animation);
    }
  }
}
