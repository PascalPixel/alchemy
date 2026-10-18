// Part of menu.c, the menu object, compiled only through it.
// FourObjectMotion: the row of four figures the Status and Djinn menus show,
// placed by a scheduler callback and eased in and out.

#include "../menu_internal.h"

// Replaces the four displayed objects and starts their projected-position task.
void FourObjectMotion_InitializeTopRow(void *unused0, s32 unused1) {
  struct FourObjectMotionState *state = gMenuWork;
  s32 index;

  for (index = 0; index < 4; index++) {
    struct AnimationObject *object = state->objects[index];

    if (object != NULL) {
      ResourceObject_ReleaseFar(object);
      state->objects[index] = NULL;
    }
  }

  for (index = 0; index < 4; index++) {
    struct AnimationObject *object =
        ResourceObject_CreateFar(gFourObjectMotion_ResourceIds[index]);

    if (object != NULL) {
      AnimationObjects_SelectAnimationFar(object, 2);
    }
    state->objects[index] = object;
    state->positionsX[index] = 0x10;
    state->positionsY[index] = 0x20;
  }
  Scheduler_AddOrUpdateCallback(FourObjectMotion_UpdateAllPositions, 0xc80);
}

// Releases the four objects and removes their scheduler callback.
#define FOUR_OBJECT_MOTION_DEFINE_RELEASE(name, update)                        \
  void name(void) {                                                            \
    struct FourObjectMotionState *state = gMenuWork;                           \
    s32 index = 0;                                                             \
                                                                               \
    do {                                                                       \
      struct AnimationObject *object = state->objects[index];                  \
                                                                               \
      if (object != NULL) {                                                    \
        ResourceObject_ReleaseFar(object);                                     \
        state->objects[index] = NULL;                                          \
      }                                                                        \
      index++;                                                                 \
    } while (index < 4);                                                       \
    Scheduler_RemoveCallback(update);                                          \
  }

FOUR_OBJECT_MOTION_DEFINE_RELEASE(FourObjectMotion_ClearSlotsAndSchedule,
                                  FourObjectMotion_UpdateAllPositions)

// Full size, 1.0 in 16.16, and the height the figures are placed from: 482
// pixels, in 16.16.
#define FOUR_OBJECT_MOTION_FULL_SCALE 0x10000u
#define FOUR_OBJECT_MOTION_ORIGIN_Y 0x01e20000u

// Projects the four objects at full scale from their saved row positions.
void FourObjectMotion_UpdateAllPositions(void) {
  struct FourObjectMotionState *state = gMenuWork;
  s32 index;
  struct SpriteScale scale;
  struct SpritePlacement placement;

  index = 0;
  while (1) {
    struct AnimationObject *object = state->objects[index];

    if (object != 0) {
      u32 vertical = FOUR_OBJECT_MOTION_ORIGIN_Y -
                     ((s32)state->verticalOrigins[index] << 16);
      object->image.priority = 0;
      scale.x = FOUR_OBJECT_MOTION_FULL_SCALE;
      scale.y = FOUR_OBJECT_MOTION_FULL_SCALE;
      placement.x = (s32)state->positionsX[index] << 16;
      placement.height = vertical;
      placement.y = ((s32)state->positionsY[index] << 16) + vertical;
      placement.groundHeight = 0;
      Object_ApplyProjectedPlacementFar(object, &placement, &scale, 0x4000);
    }
    index += 1;
    if (index > 3)
      break;
  }
}

// Projects the bottom row using its saved scale or easing toward full size.
void FourObjectMotion_UpdateBottomRow(void) {
  struct FourObjectMotionState *menu = gMenuWork;
  s32 objectIndex;
  struct SpriteScale scale;
  struct SpritePlacement placement;

  Link_DrawShiftedTilePairFar((void *)0x06002500);
  objectIndex = 0;
  do {
    struct AnimationObject *object = menu->objects[objectIndex];

    if (object != 0) {
      u32 projectedY =
          (241u << 17) - ((u32)(s32)menu->verticalOrigins[objectIndex] << 16);
      s32 phase;
      s32 limit;
      /* FAKEMATCH(pin): rowY fixed in r2, where the row's second read goes;
       * as a plain s32, which takes r3 and costs limit its register, 39 of
       * 120 instructions differ in tbs-en. */
      register s32 rowY asm("r2");

      object->image.priority = 0;
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
      Object_ApplyProjectedPlacementFar(object, &placement, &scale, limit);
    }
    objectIndex++;
  } while (objectIndex <= 3);
}

// Replaces the bottom row's four objects and starts its easing task.
void FourObjectMotion_InitializeBottomRow(s32 unused0, s32 unused1) {
  struct FourObjectMotionState *state = gMenuWork;
  s32 index;

  for (index = 0; index < 4; index++) {
    struct AnimationObject *object = state->objects[index];

    if (object != NULL) {
      ResourceObject_ReleaseFar(object);
      state->objects[index] = NULL;
    }
  }
  for (index = 0; index < 4; index++) {
    struct AnimationObject *object =
        ResourceObject_CreateFar(gFourObjectMotion_ResourceIds[index]);

    if (object != NULL)
      AnimationObjects_SelectAnimationFar(object, 2);
    state->objects[index] = object;
    state->phases[index] = 0x10000;
    state->positionsX[index] = 0x10;
    state->positionsY[index] = 0xc8;
  }
  Scheduler_AddOrUpdateCallback(FourObjectMotion_UpdateBottomRow, 0xc80);
}

// Updates one object's saved position and its signed y marker.
s32 FourObjectMotion_SetSlotPosition(s32 index, s32 x, s32 y, s32 negative) {
  struct FourObjectMotionState *state = gMenuWork;

  if (state->objects[index] != NULL) {
    state->positionsX[index] = x;
    state->positionsY[index] = negative != 0 ? y | (s32)0xffff8000 : y;
    return 0x23c + index * 2;
  }
  return index;
}

// Sets one displayed object's easing phase.
s32 FourObjectMotion_SetSlotPhase(s32 index, s32 phase) {
  ((struct FourObjectMotionState *)gMenuWork)->phases[index] = phase;
}

// Replaces one displayed object and selects its animation.
s32 FourObjectMotion_ReplaceSlot(s32 index, s32 resourceKind, s32 value) {
  struct FourObjectMotionState *state = gMenuWork;

  if (state->objects[index] != NULL) {
    ResourceObject_ReleaseFar(state->objects[index]);
    state->objects[index] = NULL;
  }
  {
    struct AnimationObject *object =
        ResourceObject_CreateFar(gFourObjectMotion_ResourceIds[resourceKind]);

    if (object != NULL)
      AnimationObjects_SelectAnimationFar(object, value);
    state->objects[index] = object;
  }
  return 1;
}

FOUR_OBJECT_MOTION_DEFINE_RELEASE(FourObjectMotion_ClearSlotsAndScheduleAlt,
                                  FourObjectMotion_UpdateBottomRow)

// Selects animation 1 for each party figure that exists.
void Menu_EnableAllItemObjects(void) {
  struct InventoryMenuState *state = gMenuWork;
  s32 index;

  for (index = 0; index < state->partyCount; ++index) {
    AnimationObjects_SelectAnimationFar(state->ownerObjects[index], 1);
  }
}

#undef FOUR_OBJECT_MOTION_DEFINE_RELEASE
#undef FOUR_OBJECT_MOTION_FULL_SCALE
#undef FOUR_OBJECT_MOTION_ORIGIN_Y
