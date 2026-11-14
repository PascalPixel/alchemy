/// @file
/// DjinnFigures: the row of four figures the Status and Djinn menus show,
/// placed by a task and eased in and out.
// Part of menu.c.

#include "menu_internal.h"

/// Replaces the four displayed sprites and starts their projected-position
/// task: one Djinni figure of each element (gDjinnFigures_SpriteIds) in
/// ANIM_WALK, at (16, 32) until the screen moves them, placed each frame by
/// DjinnFigures_UpdateAllPositions.
void DjinnFigures_InitializeTopRow(void *unused0, s32 unused1) {
  struct ItemMenuWork *state = gMenuWork;
  s32 index;

  for (index = 0; index < 4; index++) {
    struct Sprite *animation = state->figures[index];

    if (animation != NULL) {
      Sprite_DestroyFar(animation);
      state->figures[index] = NULL;
    }
  }

  for (index = 0; index < 4; index++) {
    struct Sprite *animation = Sprite_CreateFar(gDjinnFigures_SpriteIds[index]);

    if (animation != NULL) {
      Sprite_StartAnimationFar(animation, ANIM_WALK);
    }
    state->figures[index] = animation;
    state->figureX[index] = 16;
    state->figureY[index] = 32;
  }
  Task_Add(DjinnFigures_UpdateAllPositions, TASK_PASS_FRAME);
}

/// Releases the four sprites and removes their task.
#define DJINN_FIGURES_DEFINE_RELEASE(name, update)                             \
  void name(void) {                                                            \
    struct ItemMenuWork *state = gMenuWork;                                    \
    s32 index;                                                                 \
                                                                               \
    for (index = 0; index < 4; index++) {                                      \
      struct Sprite *figure = state->figures[index];                           \
                                                                               \
      if (figure != NULL) {                                                    \
        Sprite_DestroyFar(figure);                                             \
        state->figures[index] = NULL;                                          \
      }                                                                        \
    }                                                                          \
    Task_Remove(update);                                                       \
  }

DJINN_FIGURES_DEFINE_RELEASE(DjinnFigures_ReleaseTopRow,
                             DjinnFigures_UpdateAllPositions)

/// Full size, 1.0 in 16.16, and the height the figures are placed from: 482
/// pixels, in 16.16.
#define FOUR_OBJECT_MOTION_FULL_SCALE ((u32)Q16_ONE)
#define FOUR_OBJECT_MOTION_ORIGIN_Y (482u << 16)

/// Projects the four sprites at full scale from their saved row positions.
/// Task, each frame from DjinnFigures_InitializeTopRow.
void DjinnFigures_UpdateAllPositions(void) {
  struct ItemMenuWork *state = gMenuWork;
  s32 index;
  struct SpriteScale scale;
  struct SpritePlacement placement;

  index = 0;
  while (1) {
    struct Sprite *animation = state->figures[index];

    if (animation != 0) {
      u32 vertical =
          FOUR_OBJECT_MOTION_ORIGIN_Y - px((s32)state->memberY[index]);
      animation->image.priority = 0;
      scale.x = FOUR_OBJECT_MOTION_FULL_SCALE;
      scale.y = FOUR_OBJECT_MOTION_FULL_SCALE;
      placement.x = px((s32)state->figureX[index]);
      placement.height = vertical;
      placement.y = px((s32)state->figureY[index]) + vertical;
      placement.groundHeight = 0;
      Actor_ApplyProjectedPlacementFar(animation, &placement, &scale,
                                       FACING_SOUTH);
    }
    index += 1;
    if (index > 3)
      break;
  }
}

/// Projects the bottom row using its saved scale or easing toward full size.
void DjinnFigures_UpdateBottomRow(void) {
  struct ItemMenuWork *menu = gMenuWork;
  s32 figure;
  struct SpriteScale scale;
  struct SpritePlacement placement;

  Link_DrawPageArrowsFar(PAGE_ARROW_VRAM);
  for (figure = 0; figure <= 3; figure++) {
    struct Sprite *animation = menu->figures[figure];

    if (animation != 0) {
      u32 projectedY = (241u << 17) - px((u32)(s32)menu->memberY[figure]);
      s32 phase;
      s32 limit;
      s32 rowY;

      animation->image.priority = 0;
      // A negative phase is a fixed scale of -phase; otherwise the scale
      // closes a third of its gap to full size (Q16_ONE) each frame.
      phase = menu->figureScales[figure];
      if (phase < 0) {
        scale.x = -phase;
        scale.y = -phase;
      } else {
        scale.x = phase + (s32)(Q16_ONE - phase) / 3;
        scale.y = scale.x;
        menu->figureScales[figure] = scale.x;
      }
      placement.x = px((u32)(s32)menu->figureX[figure]);
      placement.height = projectedY;
      placement.y = px((u32)(s32)menu->figureY[figure]) + projectedY;
      placement.groundHeight = 0;
      rowY = menu->figureY[figure];
      /* FAKEMATCH(dead): the scale's x assigned to itself while rowY is live;
       * as no statement, 81 of 119 instructions differ in tbs-en. */
      scale.x = scale.x;
      limit = FACING_WEST;
      if (rowY >= 0)
        limit = FACING_SOUTH;
      Actor_ApplyProjectedPlacementFar(animation, &placement, &scale, limit);
    }
  }
}

/// Replaces the bottom row's four sprites and starts its easing task: one
/// Djinni figure of each element in ANIM_WALK, at full size, at (16, 200) below
/// the screen until the Djinn menu moves one.
void DjinnFigures_InitializeBottomRow(s32 unused0, s32 unused1) {
  struct ItemMenuWork *state = gMenuWork;
  s32 index;

  for (index = 0; index < 4; index++) {
    struct Sprite *animation = state->figures[index];

    if (animation != NULL) {
      Sprite_DestroyFar(animation);
      state->figures[index] = NULL;
    }
  }
  for (index = 0; index < 4; index++) {
    struct Sprite *animation = Sprite_CreateFar(gDjinnFigures_SpriteIds[index]);

    if (animation != NULL)
      Sprite_StartAnimationFar(animation, ANIM_WALK);
    state->figures[index] = animation;
    state->figureScales[index] = Q16_ONE;
    state->figureX[index] = 16;
    state->figureY[index] = 200;
  }
  Task_Add(DjinnFigures_UpdateBottomRow, TASK_PASS_FRAME);
}

/// Updates one object's saved position and its signed y marker: with negative
/// set, y keeps its low bits but gets bit 15, which makes it negative.
s32 DjinnFigures_SetSlotPosition(s32 index, s32 x, s32 y, s32 negative) {
  struct ItemMenuWork *state = gMenuWork;

  if (state->figures[index] != NULL) {
    state->figureX[index] = x;
    state->figureY[index] = negative != 0 ? y | (s32)0xffff8000 : y;
    return 0x23c + index * 2;
  }
  return index;
}

/// Sets figure index's scale phase (16.16): negative for a fixed scale of
/// -phase, otherwise the scale it grows from toward full size.
s32 DjinnFigures_SetSlotPhase(s32 index, s32 phase) {
  ((struct ItemMenuWork *)gMenuWork)->figureScales[index] = phase;
#ifdef UBFIX
  return 0;
#endif
}

/// Replaces one displayed sprite and selects its animation: figure index
/// becomes the Djinni figure of element resourceKind
/// (gDjinnFigures_SpriteIds) playing `animation`. Returns 1.
s32 DjinnFigures_ReplaceSlot(s32 index, s32 resourceKind, s32 animation) {
  struct ItemMenuWork *state = gMenuWork;

  if (state->figures[index] != NULL) {
    Sprite_DestroyFar(state->figures[index]);
    state->figures[index] = NULL;
  }
  {
    struct Sprite *figure =
        Sprite_CreateFar(gDjinnFigures_SpriteIds[resourceKind]);

    if (figure != NULL)
      Sprite_StartAnimationFar(figure, animation);
    state->figures[index] = figure;
  }
  return 1;
}

DJINN_FIGURES_DEFINE_RELEASE(DjinnFigures_ReleaseBottomRow,
                             DjinnFigures_UpdateBottomRow)

/// Has each party figure stand (ANIM_STAND). Nothing calls it.
UNUSED void Menu_StartPartyFiguresStanding(void) {
  struct ItemMenuWork *menu = gMenuWork;
  s32 index;

  for (index = 0; index < menu->partyCount; ++index) {
    Sprite_StartAnimationFar(menu->memberSprites[index], ANIM_STAND);
  }
}

#undef DJINN_FIGURES_DEFINE_RELEASE
#undef FOUR_OBJECT_MOTION_FULL_SCALE
#undef FOUR_OBJECT_MOTION_ORIGIN_Y
