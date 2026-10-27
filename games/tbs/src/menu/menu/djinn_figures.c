/// @file
/// DjinnFigures: the row of four figures the Status and Djinn menus show,
/// placed by a task and eased in and out.
// Part of menu.c.

#include "../menu_internal.h"

/// Replaces the four displayed sprites and starts their projected-position
/// task: one Djinni figure of each element (gDjinnFigures_SpriteIds) in
/// animation 2, at (16, 32) until the screen moves them, placed each frame by
/// DjinnFigures_UpdateAllPositions. The Status menu calls it when the
/// party has Djinn; DjinnFigures_ReleaseTopRow releases them.
void DjinnFigures_InitializeTopRow(void *unused0, s32 unused1) {
  struct ItemMenuFiguresView *state = gMenuWork;
  s32 index;

  for (index = 0; index < 4; index++) {
    struct Sprite *animation = state->animations[index];

    if (animation != NULL) {
      Sprite_DestroyFar(animation);
      state->animations[index] = NULL;
    }
  }

  for (index = 0; index < 4; index++) {
    struct Sprite *animation = Sprite_CreateFar(gDjinnFigures_SpriteIds[index]);

    if (animation != NULL) {
      Sprite_StartAnimFar(animation, 2);
    }
    state->animations[index] = animation;
    state->positionsX[index] = 0x10;
    state->positionsY[index] = 0x20;
  }
  Task_Add(DjinnFigures_UpdateAllPositions, TASK_PASS_FRAME);
}

/// Releases the four sprites and removes their task. It defines
/// DjinnFigures_ReleaseTopRow (the Status menu's row) and
/// DjinnFigures_ReleaseBottomRow (the Djinn menu's).
#define FOUR_OBJECT_MOTION_DEFINE_RELEASE(name, update)                        \
  void name(void) {                                                            \
    struct ItemMenuFiguresView *state = gMenuWork;                             \
    s32 index = 0;                                                             \
                                                                               \
    do {                                                                       \
      struct Sprite *object = state->animations[index];                        \
                                                                               \
      if (object != NULL) {                                                    \
        Sprite_DestroyFar(object);                                             \
        state->animations[index] = NULL;                                       \
      }                                                                        \
      index++;                                                                 \
    } while (index < 4);                                                       \
    Task_Remove(update);                                                       \
  }

FOUR_OBJECT_MOTION_DEFINE_RELEASE(DjinnFigures_ReleaseTopRow,
                                  DjinnFigures_UpdateAllPositions)

/// Full size, 1.0 in 16.16, and the height the figures are placed from: 482
/// pixels, in 16.16. As for the party figures
/// (Menu_UpdateEntryActorTransforms), the height is 482 less the origin and
/// the ground lies that far below the row, so the image stands at its row.
#define FOUR_OBJECT_MOTION_FULL_SCALE 0x10000u
#define FOUR_OBJECT_MOTION_ORIGIN_Y 0x01e20000u

/// Projects the four sprites at full scale from their saved row positions.
/// Task, each frame from DjinnFigures_InitializeTopRow. The
/// image stands at (positionsX, positionsY); verticalOrigins, which is the
/// party figures' memberY, only moves the depth slot.
void DjinnFigures_UpdateAllPositions(void) {
  struct ItemMenuFiguresView *state = gMenuWork;
  s32 index;
  struct SpriteScale scale;
  struct SpritePlacement placement;

  index = 0;
  while (1) {
    struct Sprite *animation = state->animations[index];

    if (animation != 0) {
      u32 vertical = FOUR_OBJECT_MOTION_ORIGIN_Y -
                     ((s32)state->verticalOrigins[index] << 16);
      animation->image.priority = 0;
      scale.x = FOUR_OBJECT_MOTION_FULL_SCALE;
      scale.y = FOUR_OBJECT_MOTION_FULL_SCALE;
      placement.x = (s32)state->positionsX[index] << 16;
      placement.height = vertical;
      placement.y = ((s32)state->positionsY[index] << 16) + vertical;
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
/// Task, each frame from DjinnFigures_InitializeBottomRow
/// (the Djinn menu); it also animates the page arrows. A figure whose y has
/// DjinnFigures_SetSlotPosition's negative mark (a recovering Djinni)
/// faces 0x8000 instead of 0x4000.
void DjinnFigures_UpdateBottomRow(void) {
  struct ItemMenuFiguresView *menu = gMenuWork;
  s32 objectIndex;
  struct SpriteScale scale;
  struct SpritePlacement placement;

  Link_DrawPageArrowsFar(PAGE_ARROW_VRAM);
  objectIndex = 0;
  do {
    struct Sprite *animation = menu->animations[objectIndex];

    if (animation != 0) {
      u32 projectedY =
          (241u << 17) - ((u32)(s32)menu->verticalOrigins[objectIndex] << 16);
      s32 phase;
      s32 limit;
      /* FAKEMATCH(pin): rowY fixed in r2, where the row's second read goes;
       * as a plain s32, which takes r3 and costs limit its register, 39 of
       * 120 instructions differ in tbs-en. */
      register s32 rowY asm("r2");

      animation->image.priority = 0;
      // A negative phase is a fixed scale of -phase; otherwise the scale
      // closes a third of its gap to full size (0x10000) each frame.
      phase = menu->phases[objectIndex];
      if (phase < 0) {
        scale.x = -phase;
        scale.y = -phase;
      } else {
        scale.x = phase + (s32)(Q16_ONE - phase) / 3;
        scale.y = scale.x;
        menu->phases[objectIndex] = scale.x;
      }
      placement.x = (u32)(s32)menu->positionsX[objectIndex] << 16;
      placement.height = projectedY;
      placement.y =
          ((u32)(s32)menu->positionsY[objectIndex] << 16) + projectedY;
      placement.groundHeight = 0;
      rowY = menu->positionsY[objectIndex];
      limit = 0x8000;
      if (rowY >= 0)
        limit = 0x4000;
      Actor_ApplyProjectedPlacementFar(animation, &placement, &scale, limit);
    }
    objectIndex++;
  } while (objectIndex <= 3);
}

/// Replaces the bottom row's four sprites and starts its easing task: one
/// Djinni figure of each element in animation 2, at full size, at (16, 200)
/// below the screen until the Djinn menu moves one. DjinnMenu_Run calls it
/// and DjinnFigures_ReleaseBottomRow releases them.
void DjinnFigures_InitializeBottomRow(s32 unused0, s32 unused1) {
  struct ItemMenuFiguresView *state = gMenuWork;
  s32 index;

  for (index = 0; index < 4; index++) {
    struct Sprite *animation = state->animations[index];

    if (animation != NULL) {
      Sprite_DestroyFar(animation);
      state->animations[index] = NULL;
    }
  }
  for (index = 0; index < 4; index++) {
    struct Sprite *animation = Sprite_CreateFar(gDjinnFigures_SpriteIds[index]);

    if (animation != NULL)
      Sprite_StartAnimFar(animation, 2);
    state->animations[index] = animation;
    state->phases[index] = Q16_ONE;
    state->positionsX[index] = 0x10;
    state->positionsY[index] = 0xc8;
  }
  Task_Add(DjinnFigures_UpdateBottomRow, TASK_PASS_FRAME);
}

/// Updates one object's saved position and its signed y marker: with negative
/// set, y keeps its low bits but gets bit 15, which makes it negative. Does
/// nothing to an empty slot. Returns the work offset of positionsY[index]
/// (0x23c + 2 * index), or index for an empty slot; callers ignore it.
s32 DjinnFigures_SetSlotPosition(s32 index, s32 x, s32 y, s32 negative) {
  struct ItemMenuFiguresView *state = gMenuWork;

  if (state->animations[index] != NULL) {
    state->positionsX[index] = x;
    state->positionsY[index] = negative != 0 ? y | (s32)0xffff8000 : y;
    return 0x23c + index * 2;
  }
  return index;
}

/// Sets figure index's scale phase (16.16): negative for a fixed scale of
/// -phase, otherwise the scale it grows from toward full size. The Djinn
/// chooser sets 0 after each cursor move, so the figure grows in again.
s32 DjinnFigures_SetSlotPhase(s32 index, s32 phase) {
  ((struct ItemMenuFiguresView *)gMenuWork)->phases[index] = phase;
#ifdef UBFIX
  return 0;
#endif
}

/// Replaces one displayed sprite and selects its animation: figure index
/// becomes the Djinni figure of element resourceKind
/// (gDjinnFigures_SpriteIds) in animation value. Returns 1.
s32 DjinnFigures_ReplaceSlot(s32 index, s32 resourceKind, s32 value) {
  struct ItemMenuFiguresView *state = gMenuWork;

  if (state->animations[index] != NULL) {
    Sprite_DestroyFar(state->animations[index]);
    state->animations[index] = NULL;
  }
  {
    struct Sprite *animation =
        Sprite_CreateFar(gDjinnFigures_SpriteIds[resourceKind]);

    if (animation != NULL)
      Sprite_StartAnimFar(animation, value);
    state->animations[index] = animation;
  }
  return 1;
}

FOUR_OBJECT_MOTION_DEFINE_RELEASE(DjinnFigures_ReleaseBottomRow,
                                  DjinnFigures_UpdateBottomRow)

/// Starts the walking animation (1) of each party figure. Nothing calls it.
UNUSED void Menu_StartPartyFiguresWalking(void) {
  struct ItemMenuWork *menu = gMenuWork;
  s32 index;

  for (index = 0; index < menu->partyCount; ++index) {
    Sprite_StartAnimFar(menu->memberSprites[index], 1);
  }
}

#undef FOUR_OBJECT_MOTION_DEFINE_RELEASE
#undef FOUR_OBJECT_MOTION_FULL_SCALE
#undef FOUR_OBJECT_MOTION_ORIGIN_Y
