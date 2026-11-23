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

/// What a shortcut key's assignment requests, bits 15..14
/// (Field_RequestShortcut), as The Broken Seal's.
enum FieldShortcutKind {
  FIELD_SHORTCUT_PSYNERGY = 0, ///< FieldWork.psynergy, an ability
  FIELD_SHORTCUT_ITEM = 1      ///< FieldWork.item
};

/// A shortcut assignment's request, bits 13..0, and its kind above them.
#define FIELD_SHORTCUT_ID_MASK 0x3fff
#define FIELD_SHORTCUT_KIND_SHIFT 14
/// MapWork.groundEffects' footprints, its bits 1..0: whether the leader leaves
/// them.
#define GROUND_EFFECTS_FOOTPRINTS_MASK 0x03

/// How far ahead of the leader the world map tests the ground, 16.16 pixels.
#define WORLD_MAP_PROBE_RADIUS toFIXED(8)

/// The leader's speeds and accelerations, in pixels a frame as 16.16 fixed
/// point, as The Broken Seal's; on the world map this game runs with twice the
/// walk's acceleration.
#define FIELD_WALK_SPEED toFIXED(1)      ///< 1 pixel a frame
#define FIELD_RUN_SPEED (toFIXED(3) / 2) ///< 1.5, with a dash key held
#define FIELD_ACCELERATION                                                     \
  (toFIXED(1) / 4)                       ///< a quarter pixel faster each frame
#define FIELD_FAST_WALK_SPEED toFIXED(4) ///< 4, B held with FLAG_FAST_WALK
#define FIELD_FAST_WALK_ACCELERATION toFIXED(1)
#define WORLD_MAP_WALK_SPEED (toFIXED(1) / 2) ///< half a pixel a frame
#define WORLD_MAP_RUN_SPEED toFIXED(1)        ///< 1, with a dash key held
#define WORLD_MAP_RUN_ACCELERATION (toFIXED(1) / 2)
#define WORLD_MAP_ACCELERATION (toFIXED(1) / 4)
/// The world map camera turns toward the pad's yaw by at most about 2.8
/// degrees a frame.
#define WORLD_MAP_YAW_STEP_MAX (TRIG_TURN / 128)
/// The footprints the leader leaves in snow and sand, and the leader's
/// userValue that asks for the pair next.
enum FootprintAnimation {
  FOOTPRINT_ONE_FOOT = 1, ///< one print, fading
  /// Two prints side by side, fading: laid first after a stop.
  FOOTPRINT_BOTH_FEET = 2
};

/// Script command 0x24 (control mode 11): the player control at one pace,
/// a pixel a frame whatever the keys. Unused: no code sets control mode 11.
s32 Actor_UpdateWalkingControl(struct Actor *actor) {
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
  s32 headingCount;
  s32 onObject;
  struct Actor *other;
  struct FieldWork *work;

  movementAnimation = ANIM_WALK;
  collision = 0;
  pushed = 0;
  headingCount = 0;
  if (gInput.held & gMainRoutineWork.dashKeys) {
    actor->speedLimit = toFIXED(1);
    actor->acceleration = toFIXED(1) / 20;
    movementAnimation = ANIM_LEADER_RUN;
  } else {
    actor->speedLimit = toFIXED(1);
    actor->acceleration = toFIXED(1) / 20;
  }

  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
  if (angle == WORLD_MAP_KEY_HEADING_NONE) {
    collision |= PLAYER_STEP_STOPPED;
    goto counted;
  }
  // DEAD: collision is already 0 here; the ROM keeps its clearing.
  collision = 0;
  SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  Vector_AddPolarOffset(toFIXED(8), angle, &position);
  if (actor->priorityFlags & ACTOR_PRIORITY_WALKS_ON_PLATFORMS) {
    onObject = 0;
    other = (struct Actor *)Ram_HeapSlots->actors;
    for (angleIndex = 0; angleIndex < ACTOR_COUNT; angleIndex++, other++) {
      FIXED dy;

      if (other->script == NULL)
        continue;
      if (!(other->priorityFlags & ACTOR_PRIORITY_PLATFORM))
        continue;
      if (other == actor)
        continue;
      dy = actor->y.fixed - other->y.fixed;
      if (ABS(dy - toFIXED(16)) > toFIXED(8) - 1)
        continue;
      if (SAME_CELL(actor->x.fixed, other->x.fixed) &&
          SAME_CELL(actor->z.fixed, other->z.fixed))
        onObject = 1;
      if (SAME_CELL(position.x, other->x.fixed) &&
          SAME_CELL(position.z, other->z.fixed))
        goto clear;
    }
    if (onObject && !MapGrid_CheckCollision(actor, &position)) {
      // DEAD: collision is already 0 here; the ROM keeps its clearing.
      collision = 0;
      goto counted;
    }
  }
  if (gDebugMode != 0 && (gInput.held & KEY_L)) {
    // DEAD: collision is already 0 here; the ROM keeps its clearing, which
    // cross-jumping shares with clear's.
    collision = 0;
    goto counted;
  }
  if (!MapGrid_CheckCollision(actor, &position)) {
    SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(toFIXED(8), angle + FIELD_PROBE_ANGLE, &testPosition);
    if (!MapGrid_CheckCollision(actor, &testPosition)) {
      SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      Vector_AddPolarOffset(toFIXED(8), angle - FIELD_PROBE_ANGLE,
                            &testPosition);
      if (!MapGrid_CheckCollision(actor, &testPosition)) {
        SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed,
                     actor->z.fixed);
        Vector_AddPolarOffset(toFIXED(8), angle + DEGtoANG(45), &testPosition);
        if (!MapGrid_CheckCollision(actor, &testPosition)) {
          SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed,
                       actor->z.fixed);
          Vector_AddPolarOffset(toFIXED(8), angle - DEGtoANG(45),
                                &testPosition);
          if (!MapGrid_CheckCollision(actor, &testPosition)) {
            heading = angle;
            goto push_objects;
          }
        }
      }
    }
  }
  angleOffsets[headingCount++] = angle + FIELD_PROBE_ANGLE;
  angleOffsets[headingCount++] = angle - FIELD_PROBE_ANGLE;
  angleOffsets[headingCount++] = angle + DEGtoANG(45);
  angleOffsets[headingCount++] = angle - DEGtoANG(45);
  angleOffsets[headingCount++] = angle + FIELD_PROBE_ANGLE * 3;
  angleOffsets[headingCount++] = angle - FIELD_PROBE_ANGLE * 3;
  for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
    heading = angleOffsets[angleIndex];
    SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(toFIXED(8), (u16)heading, &position);
    if (MapGrid_CheckCollision(actor, &position))
      continue;
    SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(toFIXED(8), (u16)heading + FIELD_PROBE_ANGLE,
                          &testPosition);
    if (MapGrid_CheckCollision(actor, &testPosition))
      continue;
    SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(toFIXED(8), (u16)heading - FIELD_PROBE_ANGLE,
                          &testPosition);
    if (MapGrid_CheckCollision(actor, &testPosition))
      continue;
    SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(toFIXED(8), (u16)heading + DEGtoANG(45),
                          &testPosition);
    if (MapGrid_CheckCollision(actor, &testPosition))
      continue;
    SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(toFIXED(8), (u16)heading - DEGtoANG(45),
                          &testPosition);
    if (!MapGrid_CheckCollision(actor, &testPosition))
      goto push_objects;
  }

  goto blocked;

clear:
  // DEAD: collision is already 0 here; the ROM keeps its clearing.
  collision = 0;
  goto counted;

blocked:
  SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  collision |= PLAYER_STEP_BLOCKED_BY_MAP;

push_objects:
  SET_POSITION(&contact, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  Vector_AddPolarOffset(toFIXED(4), (u16)heading, &contact);
  other = (struct Actor *)Ram_HeapSlots->actors;
  for (angleIndex = ACTOR_COUNT - 1; angleIndex >= 0; angleIndex--, other++) {
    s32 radius;

    radius = actor->radius - 2;
    if (other->script == NULL)
      continue;
    if (!(other->collisionFlags & ACTOR_SOLID))
      continue;
    if (other == actor)
      continue;
    if (Field_CheckSpheresOverlap(&other->x.fixed, radius, &contact.x,
                                  other->radius - 2) < 0)
      continue;
    if ((other->collisionFlags & 2) && other->paused == 0) {
      u16 direction;

      direction = Trig_Atan(other->z.fixed - actor->z.fixed,
                            other->x.fixed - actor->x.fixed);
      SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                   other->z.fixed);
      Vector_AddPolarOffset(toFIXED(1) / 4, direction, &testPosition);
      if (!ActorScript_CheckOverlap(other, &testPosition.x)) {
        SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                     other->z.fixed);
        Vector_AddPolarOffset(toFIXED(10), direction, &testPosition);
        if (!MapGrid_CheckCollision(other, &testPosition)) {
          SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                       other->z.fixed);
          Vector_AddPolarOffset(toFIXED(10), direction + FIELD_PROBE_ANGLE,
                                &testPosition);
          // The same probe is tested twice.
          if (!MapGrid_CheckCollision(other, &testPosition) &&
              !MapGrid_CheckCollision(other, &testPosition)) {
            SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                         other->z.fixed);
            Vector_AddPolarOffset(toFIXED(10), direction - FIELD_PROBE_ANGLE,
                                  &testPosition);
            if (!MapGrid_CheckCollision(other, &testPosition)) {
              Vector_AddPolarOffset(toFIXED(1) / 4, direction,
                                    (struct FieldPosition *)&other->x.fixed);
              other->targetX = ACTOR_NO_TARGET;
              other->targetY = ACTOR_NO_TARGET;
              other->targetZ = ACTOR_NO_TARGET;
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
    actor->speedLimit = toFIXED(1) / 4;
    actor->acceleration = toFIXED(1) / 8;
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
    Actor_SelectAnimation(actor, ANIM_PUSH);
  } else if (collision != 0) {
    s32 action;

    action = ANIM_LEADER_LOOK_AROUND;
    if (Unit_Get(gMainRoutineWork.leaderActorId)->hp == 0)
      action = ANIM_LEADER_LEAN_BACK;
    Actor_SelectAnimation(actor, action);
  } else {
    Actor_SelectAnimation(actor, movementAnimation);
  }

  if (collision != 0) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    if (collision & PLAYER_STEP_BLOCKED) {
      s32 difference;

      difference = (s16)(angle - actor->facing);
      if (difference > FIELD_BLOCKED_TURN_MAX)
        difference = FIELD_BLOCKED_TURN_MAX;
      if (difference < -FIELD_BLOCKED_TURN_MAX)
        difference = -FIELD_BLOCKED_TURN_MAX;
      actor->facing += difference;
    }
    actor->timer = 0;
    actor->userValue = 2;
  } else {
    Actor_SetMoveTarget(actor, position.x, position.y, position.z);
    if (actor->timer != 0)
      actor->timer--;
  }
  Field_CheckConfiguredKeys();
  actor->scriptCursor++;
  return 1;
}

/// Passes on a shortcut key's assignment, as The Broken Seal's: bits 15..14 say
/// whether the request is a field Psynergy or an item, bits 13..0 which. On the
/// world map R and L raise the world picture and the zoom instead, and while
/// FLAG_MENUS_RAISE_TRIGGER is set the scene's raised trigger. Returns the
/// request.
s32 Field_RequestShortcut(u32 assignment) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u32 kind = assignment >> FIELD_SHORTCUT_KIND_SHIFT;
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
/// assignment (Field_RequestShortcut). While the Ring is out, any of the five
/// asks for the Ring again (ABILITY_MAGNET, packed), which ends it. Returns 0
/// when none is pressed.
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
      work->psynergy = ABILITY_MAGNET | ACT_PACKED;
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
s32 Field_CheckSpheresOverlap(FIXED *firstPosition, s32 radiusA,
                              FIXED *secondPosition, s32 radiusB) {
  const struct FieldPosition *firstPoint =
      (const struct FieldPosition *)firstPosition;
  const struct FieldPosition *secondPoint =
      (const struct FieldPosition *)secondPosition;
  const s32 *firstCoordinate = &firstPoint->x;
  const s32 *secondCoordinate = &secondPoint->x;
  s32 dx = FIXEDtoINT(*firstCoordinate++ - *secondCoordinate++);
  s32 dy = FIXEDtoINT(*firstCoordinate++ - *secondCoordinate++);
  s32 dz = FIXEDtoINT(*firstCoordinate - *secondCoordinate);
  s32 radius = radiusA + radiusB;

  // The bound is 64 pixels in 16.16, the units before the shift, but dx and dz
  // are already whole pixels (at most 32768), so these tests always pass and
  // only the distance decides.
  if (!(dx > toFIXED(64)) && !(dz > toFIXED(64)) &&
      dx * dx + dy * dy + dz * dz < radius * radius)
    return 0;
  return -1;
}

/// A splash at the feet of an actor wading through water (MAP_CELL_WATER):
/// SPRITE_WADING_SPLASH started on its splash script, its sound every other
/// step; mode is the map's kind of wading, and kind 2 takes another picture.
void Actor_SpawnWaterSplash(struct Actor *actor, s32 mode) {
  struct Actor *created;
  struct Child *sprite;

  created = Actor_Create(SPRITE_WADING_SPLASH, actor->x.fixed,
                         actor->y.fixed + toFIXED(4), actor->z.fixed);
  sprite = actor->sprite;
  if (!(actor->userValue & 1))
    Sound_PlayFar(SE_WADE);
  if (created != NULL) {
    struct Child *child = created->sprite;

    created->groundHeight = actor->groundHeight;
    Actor_StartScript(created, gActor_SplashScript);
    created->motionFlags = 0;
    if (child != NULL) {
      Sprite_StartAnimation((struct Sprite *)child, 1);
      child->flags = 0;
      child->layer = sprite->layer;
      if (mode == 2) {
        Sprite_SetAnimationColorModes((struct Sprite *)child,
                                      SPRITE_COLOR_FILL);
        Sprite_SetAnimationSpeed((struct Sprite *)child, 8);
      }
    }
  }
}

extern struct MapCell gMapGridCellBuffer[];

/// SC_PLAYER: moves the player on the field by the direction pad: walking or
/// running, sliding around walls, pushing movable actors, turning, playing the
/// matching animation and leaving footprints.
s32 Actor_UpdatePlayerControl(struct Actor *actor) {
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
  struct MapWork *map;
  u16 angle;
  u16 direction;
  struct Actor *other;
  s32 oldFlags;
  u32 attribute;

  sprite = actor->sprite;
  movementAnimation = ANIM_WALK;
  onObject = 0;
  work = Ram_HeapSlots->fieldWork;
  map = Ram_HeapSlots->mapGrid;
  collision = 0;
  pushed = 0;
  if (gInput.held & gMainRoutineWork.dashKeys) {
    actor->speedLimit = FIELD_RUN_SPEED;
    actor->acceleration = FIELD_ACCELERATION;
    movementAnimation = ANIM_LEADER_RUN;
  } else {
    actor->speedLimit = FIELD_WALK_SPEED;
    actor->acceleration = FIELD_ACCELERATION;
  }

  angle = gWorldMap_KeyHeadings[DPAD_INDEX(gInput.held)];
  if ((u16)angle == WORLD_MAP_KEY_HEADING_NONE) {
    collision |= PLAYER_STEP_STOPPED;
    goto counted;
  }
  collision = 0;
  SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  Vector_AddPolarOffset(toFIXED(8), (u16)angle, &position);
  if (actor->priorityFlags & ACTOR_PRIORITY_WALKS_ON_PLATFORMS) {
    // Standing on another actor a cell below: the step is clear when the
    // actor under the step (and, after Japanese, both its sides) is a still
    // one.
#if TLA_PATCH_1
    SET_POSITION(&testPosition, position.x, position.y, position.z);
    Vector_AddPolarOffset(toFIXED(2), (u16)angle + DEGtoANG(90), &testPosition);
    SET_POSITION(&contact, position.x, position.y, position.z);
    Vector_AddPolarOffset(toFIXED(2), (u16)angle - DEGtoANG(90), &contact);
#endif
    angleIndex = 0;
    other = (struct Actor *)Ram_HeapSlots->actors;
    for (; angleIndex < ACTOR_COUNT; angleIndex++, other++) {
      FIXED dy;

      if (other->script == NULL)
        continue;
      if (!(other->priorityFlags & ACTOR_PRIORITY_PLATFORM))
        continue;
      if (other == actor)
        continue;
      dy = actor->y.fixed - other->y.fixed;
      if ((dy - toFIXED(16) < 0 ? -(dy - toFIXED(16)) : dy - toFIXED(16)) >
          toFIXED(8) - 1)
        continue;
      if (SAME_CELL(actor->x.fixed, other->x.fixed) &&
          SAME_CELL(actor->z.fixed, other->z.fixed))
        onObject = 1;
      if (SAME_CELL(position.x, other->x.fixed) &&
          SAME_CELL(position.z, other->z.fixed) &&
#if TLA_PATCH_1
          SAME_CELL(testPosition.x, other->x.fixed) &&
          SAME_CELL(testPosition.z, other->z.fixed) &&
          SAME_CELL(contact.x, other->x.fixed) &&
          SAME_CELL(contact.z, other->z.fixed) &&
#endif
          other->targetX == ACTOR_NO_TARGET && other->targetZ == other->targetX)
        goto clear;
    }
    if (onObject && !MapGrid_CheckCollision(actor, &position)) {
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
  if (!MapGrid_CheckCollision(actor, &position)) {
    SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(toFIXED(8), (u16)angle + FIELD_PROBE_ANGLE,
                          &testPosition);
    if (!MapGrid_CheckCollision(actor, &testPosition)) {
      SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed,
                   actor->z.fixed);
      Vector_AddPolarOffset(toFIXED(8), (u16)angle - FIELD_PROBE_ANGLE,
                            &testPosition);
      if (!MapGrid_CheckCollision(actor, &testPosition)) {
        SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed,
                     actor->z.fixed);
        Vector_AddPolarOffset(toFIXED(8), (u16)angle + DEGtoANG(45),
                              &testPosition);
        if (!MapGrid_CheckCollision(actor, &testPosition)) {
          SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed,
                       actor->z.fixed);
          Vector_AddPolarOffset(toFIXED(8), (u16)angle - DEGtoANG(45),
                                &testPosition);
          if (!MapGrid_CheckCollision(actor, &testPosition)) {
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
  angleOffsets[headingCount++] = direction + DEGtoANG(45);
  angleOffsets[headingCount++] = direction - DEGtoANG(45);
  angleOffsets[headingCount++] = direction + FIELD_PROBE_ANGLE * 3;
  angleOffsets[headingCount++] = direction - FIELD_PROBE_ANGLE * 3;
  for (angleIndex = 0; angleIndex < headingCount; angleIndex++) {
    heading = angleOffsets[angleIndex];
    SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(toFIXED(8), (u16)heading, &position);
    if (MapGrid_CheckCollision(actor, &position))
      continue;
    SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(toFIXED(8), (u16)heading + FIELD_PROBE_ANGLE,
                          &testPosition);
    if (MapGrid_CheckCollision(actor, &testPosition))
      continue;
    SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(toFIXED(8), (u16)heading - FIELD_PROBE_ANGLE,
                          &testPosition);
    if (MapGrid_CheckCollision(actor, &testPosition))
      continue;
    SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(toFIXED(8), (u16)heading + DEGtoANG(45),
                          &testPosition);
    if (MapGrid_CheckCollision(actor, &testPosition))
      continue;
    SET_POSITION(&testPosition, actor->x.fixed, actor->y.fixed, actor->z.fixed);
    Vector_AddPolarOffset(toFIXED(8), (u16)heading - DEGtoANG(45),
                          &testPosition);
    if (!MapGrid_CheckCollision(actor, &testPosition))
      goto push_objects;
  }
  SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  collision |= PLAYER_STEP_BLOCKED_BY_MAP;
  goto push_objects;

clear:
  collision = 0;
  heading = angle;

push_objects:
  // Push the movable objects just ahead a quarter pixel further, when they
  // have room; any other solid actor there blocks the step.
  SET_POSITION(&contact, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  Vector_AddPolarOffset(toFIXED(4), (u16)heading, &contact);
  other = (struct Actor *)Ram_HeapSlots->actors;
  for (angleIndex = 0; angleIndex < ACTOR_COUNT; angleIndex++, other++) {
    s32 radius;

    radius = actor->radius - 2;
    if (other->script == NULL)
      continue;
    if (!(other->collisionFlags & ACTOR_SOLID))
      continue;
    if (other == actor)
      continue;
    if (Field_CheckSpheresOverlap(&other->x.fixed, radius, &contact.x,
                                  other->radius - 2) < 0)
      continue;
    if ((other->collisionFlags & 2) && other->paused == 0) {
      direction = Trig_Atan(other->z.fixed - actor->z.fixed,
                            other->x.fixed - actor->x.fixed);
      SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                   other->z.fixed);
      heading = direction;
      Vector_AddPolarOffset(toFIXED(1) / 4, direction, &testPosition);
      if (!ActorScript_CheckOverlap(other, &testPosition.x)) {
        SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                     other->z.fixed);
        Vector_AddPolarOffset(toFIXED(10), direction, &testPosition);
        if (!MapGrid_CheckCollision(other, &testPosition)) {
          SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                       other->z.fixed);
          Vector_AddPolarOffset(toFIXED(10), direction + FIELD_PROBE_ANGLE,
                                &testPosition);
          // The same probe is tested twice.
          if (!MapGrid_CheckCollision(other, &testPosition) &&
              !MapGrid_CheckCollision(other, &testPosition)) {
            SET_POSITION(&testPosition, other->x.fixed, other->y.fixed,
                         other->z.fixed);
            Vector_AddPolarOffset(toFIXED(10), direction - FIELD_PROBE_ANGLE,
                                  &testPosition);
            if (!MapGrid_CheckCollision(other, &testPosition)) {
              Vector_AddPolarOffset(toFIXED(1) / 4, direction,
                                    (struct FieldPosition *)&other->x.fixed);
              other->targetX = ACTOR_NO_TARGET;
              other->targetY = ACTOR_NO_TARGET;
              other->targetZ = ACTOR_NO_TARGET;
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
    actor->speedLimit = toFIXED(1) / 4;
    actor->acceleration = toFIXED(1) / 8;
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
    Actor_SelectAnimation(actor, ANIM_PUSH);
  } else if (collision != 0) {
    s32 action;

    action = ANIM_LEADER_LOOK_AROUND;
    if (Unit_Get(gMainRoutineWork.leaderActorId)->hp == 0)
      action = ANIM_LEADER_LEAN_BACK;
    Actor_SelectAnimation(actor, action);
  } else {
    Actor_SelectAnimation(actor, movementAnimation);
  }

  if (collision != 0) {
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetY = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
    actor->velocityX = 0;
    actor->velocityZ = 0;
    if (collision & PLAYER_STEP_BLOCKED) {
      s32 difference;

      difference = (s16)((u16)angle - actor->facing);
      if (difference > FIELD_BLOCKED_TURN_MAX)
        difference = FIELD_BLOCKED_TURN_MAX;
      if (difference < -FIELD_BLOCKED_TURN_MAX)
        difference = -FIELD_BLOCKED_TURN_MAX;
      actor->facing += difference;
    }
    actor->timer = 0;
    actor->userValue = 2;
  } else {
    FIXED speed;

    Actor_SetMoveTarget(actor, position.x, position.y, position.z);
    speed = Math_SqrtFX(Iwram_MulFX(actor->velocityX, actor->velocityX) +
                        Iwram_MulFX(actor->velocityZ, actor->velocityZ));
    actor->velocityX = collision;
    actor->velocityZ = collision;
    Vector_AddPolarOffset(speed, (u16)heading,
                          (struct FieldPosition *)&actor->velocityX);
    if (actor->timer != 0)
      actor->timer--;
  }

  // Ground that wades: hide the lower half of the sprite while on it, and
  // leave the wading effect as the leader moves.
  if (map->groundEffects & GROUND_EFFECTS_WADING_MASK) {
    struct MapCell *cells;

    if (actor->layer <= 2)
      cells = ((struct MapWork *)map)->layers[actor->layer].cells;
    else
      cells = gMapGridCellBuffer;
    cells +=
        actor->x.fixed / toFIXED(16) + (actor->z.fixed / toFIXED(16)) * 128;
    attribute = cells->attributeB;
    oldFlags = sprite->flags;
    if (attribute & MAP_CELL_WATER) {
      if (!(oldFlags & SPRITE_FLAG_SUNK)) {
        sprite->sunkRows = 4;
        sprite->flags =
            ((u8)oldFlags | SPRITE_FLAG_SHADED) & ~SPRITE_FLAG_SHADOW;
      }
      if (actor->timer == 0 && collision == 0 && !onObject) {
        Actor_SpawnWaterSplash(actor,
                               GROUND_EFFECTS_WADING(map->groundEffects));
        if (movementAnimation == ANIM_LEADER_RUN)
          actor->timer = 8;
        else
          actor->timer = 12;
        actor->userValue ^= 1;
      }
    } else if (!(oldFlags & SPRITE_FLAG_SUNK)) {
      sprite->flags = ((u8)oldFlags | SPRITE_FLAG_SHADOW) & ~SPRITE_FLAG_SHADED;
    }
    if (oldFlags != sprite->flags)
      sprite->dirty = 1;
  }

  // Footprints on maps that show them, alternating left and right.
  footprintAngle = (u16)angle;
  if ((map->groundEffects & GROUND_EFFECTS_FOOTPRINTS_MASK) &&
      actor->timer == 0 && collision == 0) {
    struct Actor *effect;

    effect = Actor_Create(SPRITE_FOOTPRINTS, actor->x.fixed, actor->y.fixed,
                          actor->z.fixed);
    if (effect != 0) {
      struct PlayerSprite *effectSprite;

      effect->groundHeight = actor->groundHeight;
      effectSprite = effect->sprite;
      Actor_StartScript(effect, gActor_FootprintScript);
      effect->priorityFlags = 2;
      effect->motionFlags = collision;
      if (effectSprite != NULL) {
        Sprite_StartAnimation(effectSprite, 1);
        effectSprite->flags = collision;
        effectSprite->rotation = footprintAngle + DEGtoANG(90);
        effectSprite->priority = 3;
      }
      if (*(s16 *)&actor->userValue == 2) {
        Sprite_StartAnimation(effectSprite, 2);
        actor->userValue = collision;
      }
      if (*(s16 *)&actor->userValue != 0)
        effect->facing = DEGtoANG(180);
      if (movementAnimation == 5)
        actor->timer = 12;
      else
        actor->timer = 18;
      actor->userValue ^= 1;
    }
  }
  Field_CheckConfiguredKeys();
  actor->scriptCursor++;
  return 1;
}
