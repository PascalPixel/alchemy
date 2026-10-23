/// @file
/// FourObjectMotion: the row of four figures the Status and Djinn menus show,
/// placed by a scheduler callback and eased in and out.
// Part of menu.c.

#include "../menu_internal.h"

/// Replaces the four displayed objects and starts their projected-position
/// task: one Djinni figure of each element (gFourObjectMotion_ResourceIds) in
/// animation 2, at (16, 32) until the screen moves them, placed each frame by
/// FourObjectMotion_UpdateAllPositions. The Status menu calls it when the
/// party has Djinn; FourObjectMotion_ClearSlotsAndSchedule releases them.
void FourObjectMotion_InitializeTopRow(void *unused0, s32 unused1) {
  struct FourObjectMotionState *state = gMenuWork;
  s32 index;

  for (index = 0; index < 4; index++) {
    struct AnimationObject *animation = state->animations[index];

    if (animation != NULL) {
      ResourceObject_ReleaseFar(animation);
      state->animations[index] = NULL;
    }
  }

  for (index = 0; index < 4; index++) {
    struct AnimationObject *animation =
        ResourceObject_CreateFar(gFourObjectMotion_ResourceIds[index]);

    if (animation != NULL) {
      AnimationObject_SelectAnimationFar(animation, 2);
    }
    state->animations[index] = animation;
    state->positionsX[index] = 0x10;
    state->positionsY[index] = 0x20;
  }
  Scheduler_AddOrUpdateCallback(FourObjectMotion_UpdateAllPositions, 0xc80);
}

/// Releases the four objects and removes their scheduler callback. It defines
/// FourObjectMotion_ClearSlotsAndSchedule (the Status menu's row) and
/// FourObjectMotion_ClearSlotsAndScheduleAlt (the Djinn menu's), which
/// schedule nothing despite their names.
#define FOUR_OBJECT_MOTION_DEFINE_RELEASE(name, update)                        \
  void name(void) {                                                            \
    struct FourObjectMotionState *state = gMenuWork;                           \
    s32 index = 0;                                                             \
                                                                               \
    do {                                                                       \
      struct AnimationObject *object = state->animations[index];               \
                                                                               \
      if (object != NULL) {                                                    \
        ResourceObject_ReleaseFar(object);                                     \
        state->animations[index] = NULL;                                       \
      }                                                                        \
      index++;                                                                 \
    } while (index < 4);                                                       \
    Scheduler_RemoveCallback(update);                                          \
  }

FOUR_OBJECT_MOTION_DEFINE_RELEASE(FourObjectMotion_ClearSlotsAndSchedule,
                                  FourObjectMotion_UpdateAllPositions)

/// Full size, 1.0 in 16.16, and the height the figures are placed from: 482
/// pixels, in 16.16. As for the party figures
/// (Menu_UpdateEntryObjectTransforms), the height is 482 less the origin and
/// the ground lies that far below the row, so the image stands at its row.
#define FOUR_OBJECT_MOTION_FULL_SCALE 0x10000u
#define FOUR_OBJECT_MOTION_ORIGIN_Y 0x01e20000u

/// Projects the four objects at full scale from their saved row positions.
/// Scheduler callback, each frame from FourObjectMotion_InitializeTopRow. The
/// image stands at (positionsX, positionsY); verticalOrigins, which is the
/// party figures' ownerY, only moves the depth slot.
void FourObjectMotion_UpdateAllPositions(void) {
  struct FourObjectMotionState *state = gMenuWork;
  s32 index;
  struct SpriteScale scale;
  struct SpritePlacement placement;

  index = 0;
  while (1) {
    struct AnimationObject *animation = state->animations[index];

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
      Object_ApplyProjectedPlacementFar(animation, &placement, &scale, 0x4000);
    }
    index += 1;
    if (index > 3)
      break;
  }
}

/// Projects the bottom row using its saved scale or easing toward full size.
/// Scheduler callback, each frame from FourObjectMotion_InitializeBottomRow
/// (the Djinn menu); it also animates the page arrows. A figure whose y has
/// FourObjectMotion_SetSlotPosition's negative mark (a recovering Djinni)
/// faces 0x8000 instead of 0x4000.
void FourObjectMotion_UpdateBottomRow(void) {
  struct FourObjectMotionState *menu = gMenuWork;
  s32 objectIndex;
  struct SpriteScale scale;
  struct SpritePlacement placement;

  Link_DrawShiftedTilePairFar(UI_PAGE_ARROW_VRAM);
  objectIndex = 0;
  do {
    struct AnimationObject *animation = menu->animations[objectIndex];

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
        scale.x = phase + (s32)(0x10000 - phase) / 3;
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
      Object_ApplyProjectedPlacementFar(animation, &placement, &scale, limit);
    }
    objectIndex++;
  } while (objectIndex <= 3);
}

/// Replaces the bottom row's four objects and starts its easing task: one
/// Djinni figure of each element in animation 2, at full size, at (16, 200)
/// below the screen until the Djinn menu moves one. DjinnMenu_Run calls it
/// and FourObjectMotion_ClearSlotsAndScheduleAlt releases them.
void FourObjectMotion_InitializeBottomRow(s32 unused0, s32 unused1) {
  struct FourObjectMotionState *state = gMenuWork;
  s32 index;

  for (index = 0; index < 4; index++) {
    struct AnimationObject *animation = state->animations[index];

    if (animation != NULL) {
      ResourceObject_ReleaseFar(animation);
      state->animations[index] = NULL;
    }
  }
  for (index = 0; index < 4; index++) {
    struct AnimationObject *animation =
        ResourceObject_CreateFar(gFourObjectMotion_ResourceIds[index]);

    if (animation != NULL)
      AnimationObject_SelectAnimationFar(animation, 2);
    state->animations[index] = animation;
    state->phases[index] = 0x10000;
    state->positionsX[index] = 0x10;
    state->positionsY[index] = 0xc8;
  }
  Scheduler_AddOrUpdateCallback(FourObjectMotion_UpdateBottomRow, 0xc80);
}

/// Updates one object's saved position and its signed y marker: with negative
/// set, y keeps its low bits but gets bit 15, which makes it negative. Does
/// nothing to an empty slot. Returns the work offset of positionsY[index]
/// (0x23c + 2 * index), or index for an empty slot; callers ignore it.
s32 FourObjectMotion_SetSlotPosition(s32 index, s32 x, s32 y, s32 negative) {
  struct FourObjectMotionState *state = gMenuWork;

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
s32 FourObjectMotion_SetSlotPhase(s32 index, s32 phase) {
  ((struct FourObjectMotionState *)gMenuWork)->phases[index] = phase;
#ifdef UBFIX
  return 0;
#endif
}

/// Replaces one displayed object and selects its animation: figure index
/// becomes the Djinni figure of element resourceKind
/// (gFourObjectMotion_ResourceIds) in animation value. Returns 1.
s32 FourObjectMotion_ReplaceSlot(s32 index, s32 resourceKind, s32 value) {
  struct FourObjectMotionState *state = gMenuWork;

  if (state->animations[index] != NULL) {
    ResourceObject_ReleaseFar(state->animations[index]);
    state->animations[index] = NULL;
  }
  {
    struct AnimationObject *animation =
        ResourceObject_CreateFar(gFourObjectMotion_ResourceIds[resourceKind]);

    if (animation != NULL)
      AnimationObject_SelectAnimationFar(animation, value);
    state->animations[index] = animation;
  }
  return 1;
}

FOUR_OBJECT_MOTION_DEFINE_RELEASE(FourObjectMotion_ClearSlotsAndScheduleAlt,
                                  FourObjectMotion_UpdateBottomRow)

/// Starts the walking animation (1) of each party figure. Nothing calls it.
UNUSED void Menu_StartPartyFiguresWalking(void) {
  struct InventoryMenuState *menu = gMenuWork;
  s32 index;

  for (index = 0; index < menu->partyCount; ++index) {
    AnimationObject_SelectAnimationFar(menu->ownerObjects[index], 1);
  }
}

#undef FOUR_OBJECT_MOTION_DEFINE_RELEASE
#undef FOUR_OBJECT_MOTION_FULL_SCALE
#undef FOUR_OBJECT_MOTION_ORIGIN_Y
