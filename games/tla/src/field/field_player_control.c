/// @file
/// Player control: the configured keys, and SC_PLAYER, the leader moved by the
/// pad (Actor_UpdatePlayerControl).
// Part of field.c.

#include "field_internal.h"
#include "flags.h"
#include "fixed_math.h"
#include "sounds.h"
#include "field/flag_call.h"
#include "system/frame.h"
#include "field/field.h"
#include "field/object.h"
#include "field/resource_slot.h"
#include "macros.h"
#include "version.h"

// Two 16.16 coordinates fall in the same 16-pixel cell, rounding to nearest.
#define SAME_CELL(a, b) ((((a) - (b) + 0x80000) >> 20) == 0)

/// What a shortcut key's assignment requests, bits 15..14
/// (Field_RequestShortcut), as The Broken Seal's.
enum FieldShortcutKind {
  FIELD_SHORTCUT_PSYNERGY = 0, ///< FieldWork.psynergy, an ability
  FIELD_SHORTCUT_ITEM = 1      ///< FieldWork.item
};

/// A shortcut assignment's request, bits 13..0.
#define FIELD_SHORTCUT_ID_MASK 0x3fff

#define FIELD_SHORTCUT_KEYS gKeysHeld
#define WORLD_MAP_PROBE_RADIUS px(8)

// The leader's speeds and accelerations, in pixels a frame as 16.16 fixed
// point, as The Broken Seal's; on the world map this game runs with twice the
// walk's acceleration.
#define FIELD_WALK_SPEED px(1)         ///< 1 pixel a frame
#define FIELD_RUN_SPEED (px(3) / 2)    ///< 1.5, with a dash key held
#define FIELD_ACCELERATION (px(1) / 4) ///< a quarter pixel faster each frame
#define FIELD_FAST_WALK_SPEED px(4)    ///< 4, B held with FLAG_FAST_WALK
#define FIELD_FAST_WALK_ACCELERATION px(1)
#define WORLD_MAP_WALK_SPEED (px(1) / 2) ///< half a pixel a frame
#define WORLD_MAP_RUN_SPEED px(1)        ///< 1, with a dash key held
#define WORLD_MAP_RUN_ACCELERATION (px(1) / 2)
#define WORLD_MAP_ACCELERATION (px(1) / 4)
/// The world map camera turns toward the pad's yaw by at most about 2.8
/// degrees a frame.
#define WORLD_MAP_YAW_STEP_MAX (TRIG_TURN / 128)
/// What stopped the leader's step this frame (Actor_UpdatePlayerControl,
/// Actor_MoveOnWorldMap).
enum PlayerStepBlock {
  PLAYER_STEP_BLOCKED_BY_MAP = 1, ///< every heading tried runs into the map
  /// A solid actor ahead, or one with no room to be pushed.
  PLAYER_STEP_BLOCKED_BY_ACTOR = 2,
  PLAYER_STEP_BLOCKED = 3, ///< either of the two
  /// No step, and none counted as blocked: the pad is not held, or a climb
  /// meets a wall.
  PLAYER_STEP_STOPPED = 4
};

/// The footprints the leader leaves in snow and sand, and the leader's
/// userValue that asks for the pair next.
enum FootprintAnimation {
  FOOTPRINT_ONE_FOOT = 1, ///< one print, fading
  /// Two prints side by side, fading: laid first after a stop.
  FOOTPRINT_BOTH_FEET = 2
};

/// Script command 0x24 (control mode 11): the player control at one pace,
/// a pixel a frame whatever the keys. Unused: no code sets control mode 11.
s32 Actor_UpdateWalkingControl(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  struct FieldPosition contact;
  s16 angleOffsets[22];
  s32 collision;
  s32 pushed;
  s32 movementAnimation;
  u16 angle;
  s16 heading;
  s32 angleIndex;
  s32 onObject;
  struct Actor *other;
  struct FieldWork *work;

  movementAnimation = ANIM_WALK;
  collision = 0;
  pushed = 0;
  if (gInput.held & gMainRoutineWork.dashKeys) {
    object->speedLimit = px(1);
    object->acceleration = px(1) / 20;
    movementAnimation = ANIM_LEADER_RUN;
  } else {
    object->speedLimit = px(1);
    object->acceleration = px(1) / 20;
  }

  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
  if (angle == WORLD_MAP_KEY_HEADING_NONE) {
    collision |= PLAYER_STEP_STOPPED;
    goto counted;
  }
  // DEAD: collision is already 0 here; the ROM keeps its clearing.
  collision = 0;
  SET_POSITION(&position, object->x.fixed, object->y.fixed, object->z.fixed);
  Vector_AddPolarOffset(px(8), angle, &position);
  if (object->priorityFlags & ACTOR_PRIORITY_WALKS_ON_PLATFORMS) {
    s32 withinHalfCell;

    onObject = 0;
    other = (struct Actor *)Ram_HeapSlots->actors;
    withinHalfCell = 0x7ffff;
    for (angleIndex = 0; angleIndex < 64; angleIndex++, other++) {
      s32 delta;
      s32 dy;

      if (other->script == NULL)
        continue;
      if (!(other->priorityFlags & ACTOR_PRIORITY_PLATFORM))
        continue;
      if (other == object)
        continue;
      delta = object->y.fixed - other->y.fixed;
      dy = delta - px(16);
      if (dy < 0)
        dy = px(16) - delta;
      if (dy > withinHalfCell)
        continue;
      if (SAME_CELL(object->x.fixed, other->x.fixed) &&
          SAME_CELL(object->z.fixed, other->z.fixed))
        onObject = 1;
      if (SAME_CELL(position.x, other->x.fixed) &&
          SAME_CELL(position.z, other->z.fixed))
        goto clear;
    }
    if (onObject && !MapGrid_CheckCollision(object, &position)) {
      // DEAD: collision is already 0 here; the ROM keeps its clearing.
      collision = 0;
      goto counted;
    }
  }
  /* FAKEMATCH(dead): a test that sets onObject to 1 when it is nonzero. */
  if (onObject)
    onObject = 1;
  if (gDebugMode != 0 && (gInput.held & KEY_L)) {
    /* FAKEMATCH(dead): a store of the 0 collision already holds. */
    collision = 0;
    goto counted;
  }
  if (!MapGrid_CheckCollision(object, &position)) {
    SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                 object->z.fixed);
    Vector_AddPolarOffset(px(8), angle + FIELD_PROBE_ANGLE, &testPosition);
    if (!MapGrid_CheckCollision(object, &testPosition)) {
      SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                   object->z.fixed);
      Vector_AddPolarOffset(px(8), angle - FIELD_PROBE_ANGLE, &testPosition);
      if (!MapGrid_CheckCollision(object, &testPosition)) {
        SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                     object->z.fixed);
        Vector_AddPolarOffset(px(8), angle + TRIG_TURN / 8, &testPosition);
        if (!MapGrid_CheckCollision(object, &testPosition)) {
          SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                       object->z.fixed);
          Vector_AddPolarOffset(px(8), angle - TRIG_TURN / 8, &testPosition);
          if (!MapGrid_CheckCollision(object, &testPosition)) {
            heading = angle;
            goto push_objects;
          }
        }
      }
    }
  }
  {
    s32 headingCount = 0;

    angleOffsets[headingCount++] = angle + FIELD_PROBE_ANGLE;
    angleOffsets[headingCount++] = angle - FIELD_PROBE_ANGLE;
    angleOffsets[headingCount++] = angle + TRIG_TURN / 8;
    angleOffsets[headingCount++] = angle - TRIG_TURN / 8;
    angleOffsets[headingCount++] = angle + FIELD_PROBE_ANGLE * 3;
    angleOffsets[headingCount++] = angle - FIELD_PROBE_ANGLE * 3;
    /* FAKEMATCH(dead): a test of headingCount that is never true. */
    if (headingCount == 0)
      goto clear;
    for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
      heading = angleOffsets[angleIndex];
      SET_POSITION(&position, object->x.fixed, object->y.fixed,
                   object->z.fixed);
      Vector_AddPolarOffset(px(8), (u16)heading, &position);
      if (MapGrid_CheckCollision(object, &position))
        continue;
      SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                   object->z.fixed);
      Vector_AddPolarOffset(px(8), (u16)heading + FIELD_PROBE_ANGLE,
                            &testPosition);
      if (MapGrid_CheckCollision(object, &testPosition))
        continue;
      SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                   object->z.fixed);
      Vector_AddPolarOffset(px(8), (u16)heading - FIELD_PROBE_ANGLE,
                            &testPosition);
      if (MapGrid_CheckCollision(object, &testPosition))
        continue;
      SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                   object->z.fixed);
      Vector_AddPolarOffset(px(8), (u16)heading + TRIG_TURN / 8, &testPosition);
      if (MapGrid_CheckCollision(object, &testPosition))
        continue;
      SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                   object->z.fixed);
      Vector_AddPolarOffset(px(8), (u16)heading - TRIG_TURN / 8, &testPosition);
      if (!MapGrid_CheckCollision(object, &testPosition))
        goto push_objects;
    }
  }
  goto blocked;

clear:
  // DEAD: collision is already 0 here; the ROM keeps its clearing.
  collision = 0;
  goto counted;

blocked:
  SET_POSITION(&position, object->x.fixed, object->y.fixed, object->z.fixed);
  collision |= PLAYER_STEP_BLOCKED_BY_MAP;

push_objects:
  SET_POSITION(&contact, object->x.fixed, object->y.fixed, object->z.fixed);
  Vector_AddPolarOffset(px(4), (u16)heading, &contact);
  other = (struct Actor *)Ram_HeapSlots->actors;
  for (angleIndex = 63; angleIndex >= 0; angleIndex--, other++) {
    s32 radius;

    radius = object->radius - 2;
    if (other->script == NULL)
      continue;
    if (!(other->collisionFlags & 1))
      continue;
    if (other == object)
      continue;
    if (Field_CheckSpheresOverlap(&other->x.fixed, radius, &contact.x,
                                  other->radius - 2) < 0)
      continue;
    if ((other->collisionFlags & 2) && other->paused == 0) {
      u16 direction;

      direction = ArcTan2(other->z.fixed - object->z.fixed,
                          other->x.fixed - object->x.fixed);
      SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                   other->z.fixed);
      Vector_AddPolarOffset(px(1) / 4, direction, &testPosition);
      if (!ActorScript_CheckOverlap(other, &testPosition.x)) {
        SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                     other->z.fixed);
        Vector_AddPolarOffset(px(10), direction, &testPosition);
        if (!MapGrid_CheckCollision(other, &testPosition)) {
          SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                       other->z.fixed);
          Vector_AddPolarOffset(px(10), direction + FIELD_PROBE_ANGLE,
                                &testPosition);
          // The same probe is tested twice.
          if (!MapGrid_CheckCollision(other, &testPosition) &&
              !MapGrid_CheckCollision(other, &testPosition)) {
            SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                         other->z.fixed);
            Vector_AddPolarOffset(px(10), direction - FIELD_PROBE_ANGLE,
                                  &testPosition);
            if (!MapGrid_CheckCollision(other, &testPosition)) {
              Vector_AddPolarOffset(px(1) / 4, direction,
                                    (struct FieldPosition *)&other->x.fixed);
              other->targetX = TARGET_UNSET;
              other->targetY = TARGET_UNSET;
              other->targetZ = TARGET_UNSET;
              pushed |= 1;
              continue;
            }
          }
        }
      }
    }
    collision |= PLAYER_STEP_BLOCKED_BY_ACTOR;
  }
  if (collision == 0 && pushed != 0) {
    object->speedLimit = px(1) / 4;
    object->acceleration = px(1) / 8;
  }

counted:
  work = Ram_HeapSlots->fieldWork;
  if (work != NULL) {
    if (collision & PLAYER_STEP_BLOCKED)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
  }
  if (pushed != 0) {
    Actor_SelectAnimation(object, ANIM_PUSH);
  } else if (collision != 0) {
    s32 action;

    action = ANIM_LEADER_LOOK_AROUND;
    if (Unit_Get(gMainRoutineWork.leaderActorId)->hp == 0)
      action = ANIM_LEADER_LEAN_BACK;
    Actor_SelectAnimation(object, action);
  } else {
    Actor_SelectAnimation(object, movementAnimation);
  }

  if (collision != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    if (collision & PLAYER_STEP_BLOCKED) {
      s32 difference;

      difference = (s16)(angle - object->facing);
      if (difference > FIELD_BLOCKED_TURN_MAX)
        difference = FIELD_BLOCKED_TURN_MAX;
      if (difference < -FIELD_BLOCKED_TURN_MAX)
        difference = -FIELD_BLOCKED_TURN_MAX;
      object->facing += difference;
    }
    object->timer = 0;
    object->userValue = 2;
  } else {
    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    if (object->timer != 0)
      object->timer--;
  }
  Field_CheckConfiguredKeys();
  object->scriptCursor++;
  return 1;
}

/// Passes on a shortcut key's assignment, as The Broken Seal's: bits 15..14 say
/// whether the request is a field Psynergy or an item, bits 13..0 which. On the
/// world map R and L raise the world picture and the zoom instead, and while
/// FLAG_MENUS_RAISE_TRIGGER is set the scene's raised trigger. Returns the
/// request.
s32 Field_RequestShortcut(u32 assignment) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u32 kind = assignment >> 14;
  s32 request = assignment & FIELD_SHORTCUT_ID_MASK;

  if (GameFlag_Get(FLAG_MENUS_RAISE_TRIGGER) != 0) {
    work->raisedTrigger = FIELD_RAISED_TRIGGER_MENUS;
  } else if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    if (gInput.held & KEY_R)
      work->raisedTrigger = FIELD_RAISED_TRIGGER_WORLD_PICTURE;
    else if (gInput.held & KEY_L)
      work->raisedTrigger = FIELD_RAISED_TRIGGER_WORLD_MAP_ZOOM;
  } else {
    switch (kind) {
    case FIELD_SHORTCUT_PSYNERGY:
      work->psynergy = request;
      break;
    case FIELD_SHORTCUT_ITEM:
      work->item = request;
      break;
    }
  }
  return request;
}

/// Checks the keys the player has configured: the menu, examine and start
/// keys set their request and return 1, the two shortcut keys pass on their
/// assignment (Field_RequestShortcut). While a second event is pending, any of
/// the five requests command 0x209b instead. Returns 0 when none is pressed.
s32 Field_CheckConfiguredKeys(void) {
  struct FieldWork *work = gFieldWork;
  s32 result = 0;

  if (work == NULL)
    return 0;
  if (work->ringActive != 0) {
    if ((gInput.pressed & gMainRoutineWork.selectKeys) ||
        (gInput.pressed & gMainRoutineWork.aKeys) ||
        (gInput.pressed & gMainRoutineWork.startKeys) ||
        (gInput.pressed & gMainRoutineWork.firstShortcutKeys) ||
        (gInput.pressed & gMainRoutineWork.secondShortcutKeys)) {
      work->psynergy = 0x209b;
      return 1;
    }
    return 0;
  } else {
    if (gInput.pressed & gMainRoutineWork.selectKeys) {
      work->fieldMenu = 1;
      result = 1;
    } else if (gInput.pressed & gMainRoutineWork.aKeys) {
      work->aButton = 1;
      result = 1;
    } else if (gInput.pressed & gMainRoutineWork.startKeys) {
      work->pauseMenu = 1;
      result = 1;
    } else if (gInput.pressed & gMainRoutineWork.firstShortcutKeys) {
      result = Field_RequestShortcut(gMainRoutineWork.firstShortcut);
    } else if (gInput.pressed & gMainRoutineWork.secondShortcutKeys) {
      result = Field_RequestShortcut(gMainRoutineWork.secondShortcut);
    }
  }
  return result;
}

/// Returns 0 when two spheres overlap: at positions a and b (16.16 pixels) with
/// radii radius_a and radius_b (pixels); -1 otherwise.
s32 Field_CheckSpheresOverlap(s32 *firstPosition, s32 radiusA,
                              s32 *secondPosition, s32 radiusB) {
  const struct FieldPosition *firstPoint =
      (const struct FieldPosition *)firstPosition;
  const struct FieldPosition *secondPoint =
      (const struct FieldPosition *)secondPosition;
  const s32 *firstCoordinate = &firstPoint->x;
  const s32 *secondCoordinate = &secondPoint->x;
  s32 dx = FIELD_PIXEL(*firstCoordinate++ - *secondCoordinate++);
  s32 dy = FIELD_PIXEL(*firstCoordinate++ - *secondCoordinate++);
  s32 dz = FIELD_PIXEL(*firstCoordinate - *secondCoordinate);
  s32 radius = radiusA + radiusB;

  // The bound is 64 pixels in 16.16, the units before the shift, but dx and dz
  // are already whole pixels (at most 32768), so these tests always pass and
  // only the distance decides.
  if (!(dx > px(64)) && !(dz > px(64)) &&
      dx * dx + dy * dy + dz * dz < radius * radius)
    return 0;
  return -1;
}

/// A splash at the feet of an actor wading through water (cell attribute 16):
/// sprite 14 started on its splash script, its sound every other step; mode is
/// the map's ground effect kind, and kind 2 takes another picture.
void Actor_SpawnWaterSplash(struct Actor *object, s32 mode) {
  struct Actor *created;
  struct Child *sprite;

  created = Actor_Create(14, object->x.fixed, object->y.fixed + px(4),
                         object->z.fixed);
  sprite = object->sprite;
  if (!(object->userValue & 1))
    Sound_PlayFar(SE_WADE);
  if (created != NULL) {
    struct Child *child = created->sprite;

    created->groundHeight = object->groundHeight;
    Actor_StartScript(created, gActor_SplashScript);
    created->motionFlags = 0;
    if (child != NULL) {
      Sprite_StartAnimation((struct Sprite *)child, 1);
      child->flags = 0;
      child->layer = sprite->layer;
      if (mode == 2) {
        Animation_ApplyChildValuesToRecord((struct Sprite *)child, 13);
        Sprite_SetAnimationSpeed((struct Sprite *)child, 8);
      }
    }
  }
}
#undef SAME_CELL

extern struct MapCell gMapGridCellBuffer[];

// Two 16.16 coordinates fall in the same 16-pixel cell, rounding to nearest.
#define SAME_CELL(a, b) ((((a) - (b) + 0x80000) >> 20) == 0)

/// SC_PLAYER: moves the player on the field by the direction pad: walking or
/// running, sliding around walls, pushing movable actors, turning, playing the
/// matching animation and leaving footprints.
s32 Actor_UpdatePlayerControl(struct Actor *object) {
  struct FieldPosition position;
  struct FieldPosition testPosition;
  struct FieldPosition contact;
  s16 angleOffsets[22];
  s32 collision;
  s32 pushed;
  struct PlayerSprite *sprite;
  s16 heading;
  s32 angleIndex;
  s32 movementAnimation;
  s32 headingCount;
  s32 footprintAngle;
  s32 onObject;
  struct FieldWork *work;
  struct MapFlagsWork *map;
  u16 angle;
  u16 direction;
  struct Actor *other;
  s32 oldFlags;
  u32 attribute;

  sprite = object->sprite;
  movementAnimation = ANIM_WALK;
  onObject = 0;
  work = Ram_HeapSlots->fieldWork;
  map = (struct MapFlagsWork *)Ram_HeapSlots->mapGrid;
  collision = 0;
  pushed = 0;
  if (gInput.held & gMainRoutineWork.dashKeys) {
    object->speedLimit = FIELD_RUN_SPEED;
    object->acceleration = FIELD_ACCELERATION;
    movementAnimation = ANIM_LEADER_RUN;
  } else {
    object->speedLimit = FIELD_WALK_SPEED;
    object->acceleration = FIELD_ACCELERATION;
  }

  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
  if ((u16)angle == WORLD_MAP_KEY_HEADING_NONE) {
    collision |= PLAYER_STEP_STOPPED;
    goto counted;
  }
  collision = 0;
  SET_POSITION(&position, object->x.fixed, object->y.fixed, object->z.fixed);
  Vector_AddPolarOffset(px(8), (u16)angle, &position);
  if (object->priorityFlags & ACTOR_PRIORITY_WALKS_ON_PLATFORMS) {
    // Standing on another object a cell below: the step is clear when the
    // object under the step (and, after Japanese, both its sides) is a still
    // one.
#if TLA_PATCH_1
    SET_POSITION(&testPosition, position.x, position.y, position.z);
    Vector_AddPolarOffset(px(2), (u16)angle + TRIG_TURN / 4, &testPosition);
    SET_POSITION(&contact, position.x, position.y, position.z);
    Vector_AddPolarOffset(px(2), (u16)angle - TRIG_TURN / 4, &contact);
#endif
    angleIndex = 0;
    other = (struct Actor *)Ram_HeapSlots->actors;
    for (; angleIndex < 64; angleIndex++, other++) {
      s32 dy;

      if (other->script == NULL)
        continue;
      if (!(other->priorityFlags & ACTOR_PRIORITY_PLATFORM))
        continue;
      if (other == object)
        continue;
      dy = object->y.fixed - other->y.fixed;
      if ((dy - px(16) < 0 ? -(dy - px(16)) : dy - px(16)) > 0x7ffff)
        continue;
      if (SAME_CELL(object->x.fixed, other->x.fixed) &&
          SAME_CELL(object->z.fixed, other->z.fixed))
        onObject = 1;
      if (SAME_CELL(position.x, other->x.fixed) &&
          SAME_CELL(position.z, other->z.fixed) &&
#if TLA_PATCH_1
          SAME_CELL(testPosition.x, other->x.fixed) &&
          SAME_CELL(testPosition.z, other->z.fixed) &&
          SAME_CELL(contact.x, other->x.fixed) &&
          SAME_CELL(contact.z, other->z.fixed) &&
#endif
          other->targetX == TARGET_UNSET && other->targetZ == other->targetX)
        goto clear;
    }
    if (onObject && !MapGrid_CheckCollision(object, &position)) {
      collision = 0;
      heading = angle;
      goto push_objects;
    }
  }
  if (gDebugMode != 0 && (gInput.held & KEY_L)) {
    collision = 0;
    heading = angle;
    goto counted;
  }
  if (!MapGrid_CheckCollision(object, &position)) {
    SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                 object->z.fixed);
    Vector_AddPolarOffset(px(8), (u16)angle + FIELD_PROBE_ANGLE, &testPosition);
    if (!MapGrid_CheckCollision(object, &testPosition)) {
      SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                   object->z.fixed);
      Vector_AddPolarOffset(px(8), (u16)angle - FIELD_PROBE_ANGLE,
                            &testPosition);
      if (!MapGrid_CheckCollision(object, &testPosition)) {
        SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                     object->z.fixed);
        Vector_AddPolarOffset(px(8), (u16)angle + TRIG_TURN / 8, &testPosition);
        if (!MapGrid_CheckCollision(object, &testPosition)) {
          SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                       object->z.fixed);
          Vector_AddPolarOffset(px(8), (u16)angle - TRIG_TURN / 8,
                                &testPosition);
          if (!MapGrid_CheckCollision(object, &testPosition)) {
            heading = angle;
            goto push_objects;
          }
        }
      }
    }
  }
  direction = angle;
  headingCount = 0;
  angleOffsets[headingCount++] = direction + FIELD_PROBE_ANGLE;
  angleOffsets[headingCount++] = direction - FIELD_PROBE_ANGLE;
  angleOffsets[headingCount++] = direction + TRIG_TURN / 8;
  angleOffsets[headingCount++] = direction - TRIG_TURN / 8;
  angleOffsets[headingCount++] = direction + FIELD_PROBE_ANGLE * 3;
  angleOffsets[headingCount++] = direction - FIELD_PROBE_ANGLE * 3;
  for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
    heading = angleOffsets[angleIndex];
    SET_POSITION(&position, object->x.fixed, object->y.fixed, object->z.fixed);
    Vector_AddPolarOffset(px(8), (u16)heading, &position);
    if (MapGrid_CheckCollision(object, &position))
      continue;
    SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                 object->z.fixed);
    Vector_AddPolarOffset(px(8), (u16)heading + FIELD_PROBE_ANGLE,
                          &testPosition);
    if (MapGrid_CheckCollision(object, &testPosition))
      continue;
    SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                 object->z.fixed);
    Vector_AddPolarOffset(px(8), (u16)heading - FIELD_PROBE_ANGLE,
                          &testPosition);
    if (MapGrid_CheckCollision(object, &testPosition))
      continue;
    SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                 object->z.fixed);
    Vector_AddPolarOffset(px(8), (u16)heading + TRIG_TURN / 8, &testPosition);
    if (MapGrid_CheckCollision(object, &testPosition))
      continue;
    SET_POSITION(&testPosition, object->x.fixed, object->y.fixed,
                 object->z.fixed);
    Vector_AddPolarOffset(px(8), (u16)heading - TRIG_TURN / 8, &testPosition);
    if (!MapGrid_CheckCollision(object, &testPosition))
      goto push_objects;
  }
  SET_POSITION(&position, object->x.fixed, object->y.fixed, object->z.fixed);
  collision |= PLAYER_STEP_BLOCKED_BY_MAP;
  goto push_objects;

clear:
  collision = 0;
  heading = angle;

push_objects:
  // Push the movable objects just ahead a quarter pixel further, when they
  // have room; any other solid object there blocks the step.
  SET_POSITION(&contact, object->x.fixed, object->y.fixed, object->z.fixed);
  Vector_AddPolarOffset(px(4), (u16)heading, &contact);
  other = (struct Actor *)Ram_HeapSlots->actors;
  for (angleIndex = 63; angleIndex >= 0; angleIndex--, other++) {
    s32 radius;

    radius = object->radius - 2;
    if (other->script == NULL)
      continue;
    if (!(other->collisionFlags & 1))
      continue;
    if (other == object)
      continue;
    if (Field_CheckSpheresOverlap(&other->x.fixed, radius, &contact.x,
                                  other->radius - 2) < 0)
      continue;
    if ((other->collisionFlags & 2) && other->paused == 0) {
      direction = ArcTan2(other->z.fixed - object->z.fixed,
                          other->x.fixed - object->x.fixed);
      SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                   other->z.fixed);
      heading = direction;
      Vector_AddPolarOffset(px(1) / 4, direction, &testPosition);
      if (!ActorScript_CheckOverlap(other, &testPosition.x)) {
        SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                     other->z.fixed);
        Vector_AddPolarOffset(px(10), direction, &testPosition);
        if (!MapGrid_CheckCollision(other, &testPosition)) {
          SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                       other->z.fixed);
          Vector_AddPolarOffset(px(10), direction + FIELD_PROBE_ANGLE,
                                &testPosition);
          // The same probe is tested twice.
          if (!MapGrid_CheckCollision(other, &testPosition) &&
              !MapGrid_CheckCollision(other, &testPosition)) {
            SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                         other->z.fixed);
            Vector_AddPolarOffset(px(10), direction - FIELD_PROBE_ANGLE,
                                  &testPosition);
            if (!MapGrid_CheckCollision(other, &testPosition)) {
              Vector_AddPolarOffset(px(1) / 4, direction,
                                    (struct FieldPosition *)&other->x.fixed);
              other->targetX = TARGET_UNSET;
              other->targetY = TARGET_UNSET;
              other->targetZ = TARGET_UNSET;
              pushed |= 1;
              continue;
            }
          }
        }
      }
    }
    collision |= PLAYER_STEP_BLOCKED_BY_ACTOR;
  }
  if (collision == 0 && pushed != 0) {
    object->speedLimit = px(1) / 4;
    object->acceleration = px(1) / 8;
  }

counted:
  if (work != NULL) {
    if (collision & PLAYER_STEP_BLOCKED)
      work->blockedSteps++;
    else
      work->blockedSteps = 0;
    work->heldKeys = gInput.held;
  }
  if (pushed != 0) {
    Actor_SelectAnimation(object, ANIM_PUSH);
  } else if (collision != 0) {
    s32 action;

    action = ANIM_LEADER_LOOK_AROUND;
    if (Unit_Get(gMainRoutineWork.leaderActorId)->hp == 0)
      action = ANIM_LEADER_LEAN_BACK;
    Actor_SelectAnimation(object, action);
  } else {
    Actor_SelectAnimation(object, movementAnimation);
  }

  if (collision != 0) {
    object->targetX = TARGET_UNSET;
    object->targetY = TARGET_UNSET;
    object->targetZ = TARGET_UNSET;
    object->velocityX = 0;
    object->velocityZ = 0;
    if (collision & PLAYER_STEP_BLOCKED) {
      s32 difference;

      difference = (s16)((u16)angle - object->facing);
      if (difference > FIELD_BLOCKED_TURN_MAX)
        difference = FIELD_BLOCKED_TURN_MAX;
      if (difference < -FIELD_BLOCKED_TURN_MAX)
        difference = -FIELD_BLOCKED_TURN_MAX;
      object->facing += difference;
    }
    object->timer = 0;
    object->userValue = 2;
  } else {
    s32 speed;

    Actor_SetMoveTarget(object, position.x, position.y, position.z);
    speed = FixedSqrt(Iwram_MulQ16(object->velocityX, object->velocityX) +
                      Iwram_MulQ16(object->velocityZ, object->velocityZ));
    object->velocityX = collision;
    object->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)heading,
                          (struct FieldPosition *)&object->velocityX);
    if (object->timer != 0)
      object->timer--;
  }

  // Ground that wades: hide the lower half of the sprite while on it, and
  // leave the wading effect as the leader moves.
  if (map->groundEffects & 12) {
    struct MapCell *cells;

    if (object->layer <= 2)
      cells = ((struct MapWork *)map)->layers[object->layer].cells;
    else
      cells = gMapGridCellBuffer;
    cells += object->x.fixed / px(16) + (object->z.fixed / px(16)) * 128;
    attribute = cells->attributeB;
    oldFlags = sprite->flags;
    if (attribute & 16) {
      if (!(oldFlags & 8)) {
        sprite->sunkRows = 4;
        sprite->flags = ((u8)oldFlags | 4) & ~1;
      }
      if (object->timer == 0 && collision == 0 && !onObject) {
        Actor_SpawnWaterSplash(object, (u32)(map->groundEffects << 28) >> 30);
        if (movementAnimation == 5)
          object->timer = 8;
        else
          object->timer = 12;
        object->userValue ^= 1;
      }
    } else if (!(oldFlags & 8)) {
      sprite->flags = ((u8)oldFlags | 1) & ~4;
    }
    if (oldFlags != sprite->flags)
      sprite->dirty = 1;
  }

  // Footprints on maps that show them, alternating left and right.
  footprintAngle = (u16)angle;
  if ((map->groundEffects & 3) && object->timer == 0 && collision == 0) {
    struct Actor *effect;

    effect =
        Actor_Create(0x10e, object->x.fixed, object->y.fixed, object->z.fixed);
    if (effect != 0) {
      struct PlayerSprite *effectSprite;

      effect->groundHeight = object->groundHeight;
      effectSprite = effect->sprite;
      Actor_StartScript(effect, gActor_FootprintScript);
      effect->priorityFlags = 2;
      effect->motionFlags = collision;
      if (effectSprite != NULL) {
        Sprite_StartAnimation(effectSprite, 1);
        effectSprite->flags = collision;
        effectSprite->rotation = footprintAngle + TRIG_TURN / 4;
        effectSprite->priority = 3;
      }
      if (*(s16 *)&object->userValue == 2) {
        Sprite_StartAnimation(effectSprite, 2);
        object->userValue = collision;
      }
      if (*(s16 *)&object->userValue != 0)
        effect->facing = TRIG_TURN / 2;
      if (movementAnimation == 5)
        object->timer = 12;
      else
        object->timer = 18;
      object->userValue ^= 1;
    }
  }
  Field_CheckConfiguredKeys();
  object->scriptCursor++;
  return 1;
}
#undef SAME_CELL
