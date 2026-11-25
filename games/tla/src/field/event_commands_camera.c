/// @file
/// The event commands that move the camera and wait for it.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "macros.h"

/// The event command "camera move to": sends the camera to the requested axes,
/// 16.16 pixels (-1 keeps that axis), kept far enough inside the map that the
/// screen never shows past its edge. With pan 0 the camera cuts there at once;
/// otherwise it pans there at its own speed.
void FieldCamera_MoveWithinMapBounds(FIXED requestedX, FIXED requestedY,
                                     FIXED requestedZ, s32 pan) {
  s32 panning;
  struct FieldState *state;
  s32 minimumX;
  FIXED minimumZ;
  s32 maximumX;
  FIXED height;
  FIXED maximumZ;
  FIXED positionX;
  FIXED positionZ;
  FIXED positionY;
  struct MapState *map;
  struct Actor *actor;

  positionX = requestedX;
  positionY = requestedY;
  panning = pan;
  positionZ = requestedZ;
  state = HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(struct FieldState));
  actor = state->camera;
  map = Ram_HeapSlots->mapGrid;
  // Keep the screen inside the map: the view centre stays 120 pixels from
  // either side, 96 from the top and 64 from the bottom, the z limits moved
  // down the map by its height.
  minimumX = map->minX + toFIXED(120);
  height = actor->y.fixed;
  minimumZ = map->minZ + height + toFIXED(96);
  maximumX = map->maxX - toFIXED(120);
  maximumZ = map->maxZ + height - toFIXED(64);
  map->origin = &actor->x.fixed;
  Actor_StartIdleFar(actor);
  if (positionX == -1) {
    positionX = actor->x.fixed;
  }
  if (positionY == -1) {
    positionY = actor->y.fixed;
  }
  if (positionZ == -1) {
    positionZ = actor->z.fixed;
  }
  if (positionX < minimumX) {
    positionX = minimumX;
  }
  if (positionZ < minimumZ) {
    positionZ = minimumZ;
  }
  if (positionX > maximumX) {
    positionX = maximumX;
  }
  if (positionZ > maximumZ) {
    positionZ = maximumZ;
  }
  if (panning == 0) {
    SET_ACTOR_POSITION(actor, positionX, positionY, positionZ);
    wait(1U);
    if (state->mode != ACTOR_SYSTEM_WORLD_MAP) {
      MapGrid_CenterOnFollowedPointFar();
    }
  } else {
    Actor_SetMoveTargetFar(actor, positionX, positionY, positionZ);
  }
}

/// The event command "camera move to actor": FieldCamera_MoveWithinMapBounds to
/// the actor's x and z.
void FieldCamera_MoveToActorById(s32 actorId, s32 pan) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL)
    FieldCamera_MoveWithinMapBounds(actor->x.fixed, -1, actor->z.fixed, pan);
}

/// As FieldCamera_MoveToActorById, to the actor's height too.
void FieldCamera_MoveToActor(s32 actorId, s32 pan) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor != NULL)
    FieldCamera_MoveWithinMapBounds(actor->x.fixed, actor->y.fixed,
                                    actor->z.fixed, pan);
}

/// The event command "camera wait for move": waits (at most 600 frames) for
/// the camera to arrive, then 2 frames more.
void FieldCamera_WaitForArrival(void) {
  Actor_WaitForMoveFar(((struct FieldState *)HeapSlot_AllocEwram(
                            HEAP_SLOT_FIELD, sizeof(struct FieldState)))
                           ->camera);
  Event_Wait(2);
}

/// The event command "get camera": returns the camera's own actor, which the
/// view centres on.
struct Actor *FieldCamera_GetActor(void) {
  return ((struct FieldState *)HeapSlot_AllocEwram(HEAP_SLOT_FIELD,
                                                   sizeof(struct FieldState)))
      ->camera;
}

/// Unused: makes the camera follow target; unless keepCurrentPosition is set it
/// jumps there at once.
UNUSED void FieldCamera_Follow(struct Actor *target, s32 keepCurrentPosition) {
  struct Actor *camera;

  camera = ((struct FieldState *)HeapSlot_AllocEwram(HEAP_SLOT_FIELD,
                                                     sizeof(struct FieldState)))
               ->camera;
  if (target != NULL) {
    Actor_StartCameraFar(camera, 0);
    camera->watchedActor = (union FieldObject *)target;
    if (keepCurrentPosition == 0) {
      SET_ACTOR_POSITION(camera, target->x.fixed, target->y.fixed,
                         target->z.fixed);
    }
  }
}
