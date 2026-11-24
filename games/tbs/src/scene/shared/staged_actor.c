/// @file
/// Seventeen scenes with things the leader pushes (Altmiller Cave, Babi
/// Lighthouse, Crossbone Isle and its rooms, Goma Cave's pillar room, Gondowan
/// Cave, Kolima Forest (SceneId_KolimaForestEntrance to
/// SceneId_KolimaForestCliff), Lunpa's fortress, Mercury Lighthouse and its
/// entrance, Mogall Forest, the Tunnel Ruins, Vale and its cave): things
/// pushed a cell at a time, such as stone pillars, and logs and other large
/// things that roll until they meet something, redrawn into the map grid where
/// they stop.
///
/// A large thing is a log, a pipe or a fallen column, lying west to east or
/// north to south: one of six rectangles, by its sprite
/// (gPushedActor_RectangleKinds), with its edges in
/// gPushedActor_Rectangles (pushed_actor_tables.c).

#include "field/map_grid_scroll.h"
#include "scene/actor_step.h"
#include "system/point.h"
#include "system/iwram_call.h"
#include "system/system_call.h"

/// Pixel step by direction: X in the upper halfword, Z in the lower.
extern s32 gPushedActor_DirectionSteps[];
extern s32 gPushedActor_Rectangles[];

#include "actor_distance.c"

/// Returns the placed actor on the cell of the 16.16 position and at its
/// height (in whole pixels), or 0. origin is not read.
static struct Actor *
PushedActor_FindAtTile(const struct FieldPosition *position,
                       const struct Actor *origin) {
  struct FieldState *state = gFieldState;
  struct Actor **slots = state->actors;
  u32 i;

  for (i = ACTOR_FIRST_PLACED;
       i < sizeof(state->actors) / sizeof(state->actors[0]); i++) {
    struct Actor *actor = slots[i];

    // UB: Actor_ClearAll leaves unfilled slots null.
#ifdef UBFIX
    if (actor == NULL)
      continue;
#endif
    if (FIELD_CELL(position->x) == FIELD_CELL(actor->x.fixed) &&
        (position->y / toFIXED(1)) == (actor->y.fixed / toFIXED(1)) &&
        FIELD_CELL(position->z) == FIELD_CELL(actor->z.fixed)) {
      return actor;
    }
  }
  return 0;
}

/// The leader pushes what stands in front of them a cell on, unless
/// something solid is beyond it or on top of it, the ground beyond refuses it
/// or it is still moving: after a quarter second's push it and the leader
/// slide one cell together, with SE_PUSH.
UNUSED static void PushedActor_AdvancePair(void) {
  struct FieldPosition dst;
  struct Actor *lead;
  struct Actor *next;
  struct Actor *blocker;
  s32 dir;
  FIXED rate;

  lead = Actor_GetById(ACTOR_PARTY_LEADER);
  dir = FACING_INDEX(lead->facing);
  Actor_StepFrom(&dst, lead, gPushedActor_DirectionSteps[dir]);
  next = PushedActor_FindAtTile(&dst, lead);
  if (next == NULL)
    return;

  Actor_StepFrom(&dst, next, gPushedActor_DirectionSteps[dir]);
  blocker = PushedActor_FindAtTile(&dst, next);
  if (blocker != NULL && (blocker->collisionFlags & ACTOR_SOLID) != 0)
    return;

  dst.x = next->x.fixed;
  dst.y = next->y.fixed + toFIXED(16);
  dst.z = next->z.fixed;
  blocker = PushedActor_FindAtTile(&dst, next);
  if (blocker != NULL && (blocker->collisionFlags & ACTOR_SOLID) != 0)
    return;

  next->layer = 2;
  Actor_StepFrom(&dst, next, gPushedActor_DirectionSteps[dir]);
  if (FieldActor_CheckMovementCollision(next, &dst) > 0)
    return;

  if (next->riseCounter != 0)
    return;

  Actor_SelectAnimation(lead, ANIM_PUSH);
  rate = toFIXED(1) / 5;
  wait(15);
  Sound_Play(SE_PUSH);
  next->speedLimit = rate;
  next->acceleration = rate;
  FieldActor_SetMoveTargetFX(next, dst.x, dst.y, dst.z);
  lead->speedLimit = rate;
  lead->acceleration = rate;
  FieldActor_SetMoveTargetFX(lead, dst.x, dst.y, dst.z);
  Actor_WaitForMove(next);
  Scene_PlayQueuedMusic();
  next->x.fixed = dst.x;
  next->z.fixed = dst.z;
  next->velocityX = 0;
  next->velocityZ = 0;
  lead->targetX = ACTOR_NO_TARGET;
  lead->targetZ = ACTOR_NO_TARGET;
  lead->velocityX = 0;
  lead->velocityZ = 0;
  lead->x.fixed = INTtoFIXED(lead->x.part[0].integer);
  lead->z.fixed = INTtoFIXED(lead->z.part[0].integer);
  Actor_SelectAnimation(lead, ANIM_STAND);
}

#include "pushed_actor_fill_rectangle.c"

/// The leader's update routine while pushing a large thing: stop the leader
/// when another actor that is not a large thing, or a wall, is in the
/// way.
static UNREAD_RESULT PushedActor_StopBlockedMotion(struct Actor *actor) {
  struct FieldPosition probe;
  u32 dir;
  struct Actor *target;

  dir = FACING_INDEX(actor->facing);
  Actor_StepFrom(&probe, actor, gPushedActor_DirectionSteps[dir]);
  target = PushedActor_FindAtTile(&probe, actor);
  if (target != NULL) {
    u32 i = 0;
    s32 kind = target->sprite->entries[0]->animationId;
    s32 *kinds = gPushedActor_RectangleKinds;

    do {
      if (kind == *kinds++) {
        RETURN_UNREAD(0);
      }
      i++;
    } while (i <= 5);
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
  }
  Actor_StepFrom(&probe, actor, gPushedActor_DirectionSteps[dir]);
  if (FieldActor_CheckMovementCollision(actor, &probe) > 0) {
    actor->velocityX = 0;
    actor->velocityZ = 0;
    actor->targetX = ACTOR_NO_TARGET;
    actor->targetZ = ACTOR_NO_TARGET;
  }
#ifndef UBFIX
  return 0;
#endif
}

/// Returns the large thing whose rectangle covers the cell ahead of the leader
/// (the leader not standing in line with its first column or row), with the
/// leader's direction, its actor id and its rectangle; or NULL.
static struct Actor *Scene_FindActorRegion(s32 *direction, s32 *slot,
                                           s32 *rectangle) {
  struct FieldState *state = gFieldState;
  struct Actor *player = Actor_GetById(ACTOR_PARTY_LEADER);
  struct Actor **list;
  u32 i, j;

  *direction = FACING_INDEX(player->facing);
  list = state->actors + ACTOR_FIRST_PLACED;
  for (i = ACTOR_FIRST_PLACED;
       i < sizeof(state->actors) / sizeof(state->actors[0]); list++, i++) {
    struct Actor *actor = *list;
    s32 kind;
    // UB: the same empty slots are scanned here.
#ifdef UBFIX
    if (actor == NULL)
      continue;
#endif
    for (j = 0, kind = actor->sprite->entries[0]->animationId; j < 6; j++) {
      if (kind == gPushedActor_RectangleKinds[j]) {
        s32 x, z, left, top, right, bottom;
        *rectangle = j;
        x = ((FIXEDtoINT(player->x.fixed)) +
             (FIXEDtoINT(gPushedActor_DirectionSteps[*direction]))) >>
            4;
        z = ((FIXEDtoINT(player->z.fixed)) +
             (s16)gPushedActor_DirectionSteps[*direction]) >>
            4;
        left =
            (actor->x.part[0].integer + gPushedActor_Rectangles[j * 4 + 0]) >>
            4;
        top = (actor->z.part[0].integer + gPushedActor_Rectangles[j * 4 + 1]) >>
              4;
        right =
            (actor->x.part[0].integer + gPushedActor_Rectangles[j * 4 + 2]) >>
            4;
        bottom =
            (actor->z.part[0].integer + gPushedActor_Rectangles[j * 4 + 3]) >>
            4;
        if (left <= x && x < right && top <= z && z < bottom) {
          if (j & 1) {
            if (left != FIELD_CELL(player->x.fixed)) {
              *slot = i;
              return actor;
            }
          } else if (top != FIELD_CELL(player->z.fixed)) {
            *slot = i;
            return actor;
          }
        }
      }
    }
  }
  return NULL;
}

/// Finds where the large thing ahead of the leader would roll to: step its
/// rectangle on, a cell at a time the way the leader faces, until a cell of it
/// meets something (a collision result of 2). Fills probe with the thing, its
/// rectangle and that position; returns 0 when there is no thing or it cannot
/// move.
UNUSED static s32
PushedActor_FindClearPosition(struct PushedActorProbe *probe) {
  s32 direction;
  struct FieldPosition position;
  struct Actor *actor;
  s32 steps;
  s32 width, height;
  u8 *mode;
  s32 column, row;
  s32 nearEdge, farEdge;
  s32 offset;

  probe->callback = 0;
  actor = Scene_FindActorRegion(&direction, &probe->actorSlot,
                                &probe->rectangleIndex);
  if (actor == NULL) {
    return 0;
  }
  mode = &actor->layer;
  *mode = 2;
  {
    s32 rectangle = probe->rectangleIndex;

    steps = 0;
    offset = rectangle * 4;
    nearEdge = gPushedActor_Rectangles[offset + 1];
    if (nearEdge < 0) {
      nearEdge = -nearEdge;
    }
    farEdge = gPushedActor_Rectangles[offset + 3];
    if (farEdge < 0) {
      farEdge = -farEdge;
    }
    height = (nearEdge + farEdge) >> 4;
    nearEdge = gPushedActor_Rectangles[offset];
    if (nearEdge < 0) {
      nearEdge = -nearEdge;
    }
    farEdge = gPushedActor_Rectangles[offset + 2];
    if (farEdge < 0) {
      farEdge = -farEdge;
    }
    width = (nearEdge + farEdge) >> 4;
  }
  position.x = actor->x.fixed +
               DIRECTION_STEP_X_FIXED(gPushedActor_DirectionSteps[direction]);
  {
    FIXED y = actor->y.fixed;

    position.y = y;
    position.z = actor->z.fixed +
                 DIRECTION_STEP_Z_FIXED(gPushedActor_DirectionSteps[direction]);
    probe->position.y = y;
  }
  for (;;) {
    probe->position.z =
        position.z +
        INTtoFIXED(gPushedActor_Rectangles[probe->rectangleIndex * 4 + 1]);
    for (row = 0; row < height; row++) {
      probe->position.x =
          position.x +
          INTtoFIXED(gPushedActor_Rectangles[probe->rectangleIndex * 4]);
      for (column = 0; column < width; column++) {
        if (FieldActor_CheckMovementCollision(actor, &probe->position) == 2) {
          goto found;
        }
        probe->position.x += toFIXED(16);
      }
      probe->position.z += toFIXED(16);
    }
    steps++;
    position.x +=
        DIRECTION_STEP_X_FIXED(gPushedActor_DirectionSteps[direction]);
    position.z +=
        DIRECTION_STEP_Z_FIXED(gPushedActor_DirectionSteps[direction]);
  }
found:
  *mode = 0;
  if (steps == 0) {
    return 0;
  }
  probe->position.x =
      actor->x.fixed +
      DIRECTION_STEP_X_FIXED(gPushedActor_DirectionSteps[direction]) * steps;
  probe->position.y = actor->y.fixed;
  probe->position.z =
      actor->z.fixed +
      DIRECTION_STEP_Z_FIXED(gPushedActor_DirectionSteps[direction]) * steps;
  return 1;
}

/// Rolls the large thing to the position probe found: the leader pushes, the
/// thing rolls (its sheet's animation 3 or 2, by the direction) with
/// SE_HEAVY_MOVE while the leader follows half a cell, and it stops with
/// SE_STOP_ACTION_SE and SE_HEAVY_STOP. Its cells are blocked
/// (CELL_BEHAVIOR_WALL) at its new place, through the map grid's layer 1
/// offset, and its old place is given back what was under it.
UNUSED static void Actor_MoveAndRedraw(struct PushedActorProbe probe) {
  struct MapGridScrollState *state;
  struct Actor *actor;
  struct FieldPosition originalPosition;
  struct FieldPosition tilePosition;
  s32 direction;
  s32 horizontalExtent;
  s32 verticalExtent;

  // UB: the heap slot is read as a word rather than as the pointer it holds.
#ifdef UBFIX
  state = gMapGridState[0];
#else
  state = (void *)((u32 *)gMapGridState)[0];
#endif
  direction = FACING_INDEX(Actor_GetById(ACTOR_PARTY_LEADER)->facing);
  actor = Actor_GetById(probe.actorSlot);
  {
    s32 extentA = gPushedActor_Rectangles[probe.rectangleIndex * 4 + 1];
    s32 extentB;

    if (extentA < 0)
      extentA = -extentA;
    extentB = gPushedActor_Rectangles[probe.rectangleIndex * 4 + 3];
    if (extentB < 0)
      extentB = -extentB;
    verticalExtent = (extentA + extentB) >> 4;
    extentA = gPushedActor_Rectangles[probe.rectangleIndex * 4 + 0];
    if (extentA < 0)
      extentA = -extentA;
    extentB = gPushedActor_Rectangles[probe.rectangleIndex * 4 + 2];
    if (extentB < 0)
      extentB = -extentB;
    horizontalExtent = (extentA + extentB) >> 4;
  }

  actor->speedLimit = toFIXED(1) / 2;
  actor->acceleration = toFIXED(1) / 10;
  originalPosition.x = actor->x.fixed;
  originalPosition.z = actor->z.fixed;
  tilePosition.x =
      actor->x.fixed +
      INTtoFIXED(gPushedActor_Rectangles[probe.rectangleIndex * 4]);
  tilePosition.z =
      actor->z.fixed +
      INTtoFIXED(gPushedActor_Rectangles[probe.rectangleIndex * 4 + 1]);
  tilePosition.x = FIELD_CELL(tilePosition.x);
  tilePosition.z = FIELD_CELL(tilePosition.z);

  PushedActor_FillGridAttributeRectangle(0, tilePosition.x, tilePosition.z,
                                         horizontalExtent, verticalExtent,
                                         CELL_BEHAVIOR_NORMAL);
  Actor_SetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 0.5, 0.1);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Event_Wait(15);

  {
    // Half the thing's roll, in whole pixels.
    s32 horizontalDelta = (probe.position.x - originalPosition.x) / toFIXED(2);
    {
      s32 verticalDelta = (probe.position.z - originalPosition.z) / toFIXED(2);

      Actor_SetDestinationOffsetById(ACTOR_PARTY_LEADER, horizontalDelta,
                                     verticalDelta);
    }
  }

  Actor_GetById(ACTOR_PARTY_LEADER)->update =
      (void (*)(struct Actor *))PushedActor_StopBlockedMotion;
  Event_Wait(4);
  if (direction >= FACING_INDEX(FACING_SOUTHWEST) &&
      direction <= FACING_INDEX(FACING_NORTH_NORTHEAST))
    Actor_SelectAnimation(actor, 3);
  else
    Actor_SelectAnimation(actor, 2);

  Sound_Play(SE_HEAVY_MOVE);
  FieldActor_SetMoveTargetFX(actor, probe.position.x, probe.position.y,
                             probe.position.z);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 0.3, 0.1);

  {
    s32 packedOrientation = gPushedActor_DirectionSteps[direction];
    Actor_SetDestinationOffsetById(ACTOR_PARTY_LEADER,
                                   (s16)DIRECTION_STEP_X(packedOrientation) / 2,
                                   DIRECTION_STEP_Z(packedOrientation) / 2);
  }
  if (probe.callback != 0)
    probe.callback();

  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_GetById(ACTOR_PARTY_LEADER)->update = NULL;
  Actor_WaitForMove(actor);
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_HEAVY_STOP);

  actor->x.fixed = probe.position.x;
  actor->z.fixed = probe.position.z;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  Actor_SelectAnimation(actor, 1);

  {
    s32 horizontalOrigin;
    s32 verticalOrigin;

    {
      probe.position.x +=
          INTtoFIXED(gPushedActor_Rectangles[probe.rectangleIndex * 4]);
      probe.position.z +=
          INTtoFIXED(gPushedActor_Rectangles[probe.rectangleIndex * 4 + 1]);
      probe.position.x = FIELD_CELL(probe.position.x);
      probe.position.z = FIELD_CELL(probe.position.z);
    }
    horizontalOrigin = FIELD_CELL(state->layers[1].offsetX);
    verticalOrigin = FIELD_CELL(state->layers[1].offsetY);

    MapGrid_CopyCellAttributes(
        probe.position.x, probe.position.z, horizontalExtent, verticalExtent,
        horizontalOrigin + probe.position.x, verticalOrigin + probe.position.z);
    PushedActor_FillGridAttributeRectangle(0, probe.position.x,
                                           probe.position.z, horizontalExtent,
                                           verticalExtent, CELL_BEHAVIOR_WALL);
    PushedActor_FillGridAttributeRectangle(2, probe.position.x,
                                           probe.position.z, horizontalExtent,
                                           verticalExtent, CELL_BEHAVIOR_WALL);

    {
      originalPosition.x +=
          INTtoFIXED(gPushedActor_Rectangles[probe.rectangleIndex * 4]);
      originalPosition.z +=
          INTtoFIXED(gPushedActor_Rectangles[probe.rectangleIndex * 4 + 1]);
      originalPosition.x = FIELD_CELL(originalPosition.x);
      originalPosition.z = FIELD_CELL(originalPosition.z);
    }
    horizontalOrigin += originalPosition.x;
    verticalOrigin += originalPosition.z;

    MapGrid_CopyCellAttributeRect(horizontalOrigin, verticalOrigin,
                                  horizontalExtent, verticalExtent,
                                  originalPosition.x, originalPosition.z);
    PushedActor_FillGridAttributeRectangle(
        2, originalPosition.x, originalPosition.z, horizontalExtent,
        verticalExtent, CELL_BEHAVIOR_NORMAL);
  }
  Scene_PlayQueuedMusic();
}

/// Blocks large thing id's cells (CELL_BEHAVIOR_WALL) where it stands,
/// through the map grid's layer 1 offset; returns 0 when its sprite is no large
/// thing.
UNUSED static s32 Scene_RedrawActorRectangle(s32 id) {
  struct MapGridScrollState *grid = gMapGridState[0];
  struct Actor *actor = Actor_GetById(id);
  struct PushedActorProbe probe;
  u32 idx = 0;
  s32 offset;
  s32 width;
  s32 height;
  s32 x0;
  s32 x1;
  s32 z0;
  s32 z1;
  s32 dstZ;

  for (idx = 0; idx <= 5; idx++) {
    if (actor->sprite->entries[0]->animationId ==
        gPushedActor_RectangleKinds[idx]) {
      probe.rectangleIndex = idx;
      break;
    }
    probe.rectangleIndex = 7;
  }

  if (probe.rectangleIndex < 0 || probe.rectangleIndex > 6)
    return 0;
  probe.position.x = actor->x.fixed;
  probe.position.y = actor->y.fixed;
  probe.position.z = actor->z.fixed;
  offset = probe.rectangleIndex * 4;
  z0 = gPushedActor_Rectangles[offset + 1];
  if (z0 < 0)
    z0 = -z0;
  z1 = gPushedActor_Rectangles[offset + 3];
  if (z1 < 0)
    z1 = -z1;
  height = (z0 + z1) >> 4;
  x0 = gPushedActor_Rectangles[offset];
  if (x0 < 0)
    x0 = -x0;
  x1 = gPushedActor_Rectangles[offset + 2];
  if (x1 < 0)
    x1 = -x1;
  width = (x0 + x1) >> 4;
  probe.position.x += INTtoFIXED(gPushedActor_Rectangles[offset]);
  probe.position.z += INTtoFIXED(gPushedActor_Rectangles[offset + 1]);
  probe.position.x = FIELD_CELL(probe.position.x);
  probe.position.z = FIELD_CELL(probe.position.z);
  idx = FIELD_CELL(grid->layers[1].offsetX);
  dstZ = FIELD_CELL(grid->layers[1].offsetY);
  MapGrid_CopyCellAttributeRect(probe.position.x, probe.position.z, width,
                                height, idx + probe.position.x,
                                dstZ + probe.position.z);
  PushedActor_FillGridAttributeRectangle(0, probe.position.x, probe.position.z,
                                         width, height, CELL_BEHAVIOR_WALL);
  PushedActor_FillGridAttributeRectangle(2, probe.position.x, probe.position.z,
                                         width, height, CELL_BEHAVIOR_WALL);
  return 1;
}
