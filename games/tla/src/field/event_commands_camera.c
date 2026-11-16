/// @file
/// The event camera.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "macros.h"

/// The event command "camera move to", as The Broken Seal's
/// FieldCamera_MoveWithinMapBounds: sends the camera to the requested axes,
/// 16.16 pixels (-1 keeps that axis), kept far enough inside the map that the
/// screen never shows past its edge. With useSetter 0 the camera jumps there
/// at once; otherwise it travels at its own speed.
void FieldCamera_MoveWithinMapBounds(FIXED requestedX, FIXED requestedY,
                                     FIXED requestedZ, s32 useSetter) {
  s32 shouldUseSetter;
  struct FieldWork *runtimeBlock;
  s32 minimumX;
  FIXED minimumZ;
  s32 maximumX;
  FIXED objectZOffset;
  FIXED maximumZ;
  FIXED positionX;
  FIXED positionZ;
  FIXED positionY;
  struct MapWork *map;
  struct Actor *actor;

  positionX = requestedX;
  positionY = requestedY;
  shouldUseSetter = useSetter;
  positionZ = requestedZ;
  runtimeBlock = HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(struct FieldWork));
  actor = runtimeBlock->camera;
  map = Ram_HeapSlots->mapGrid;
  // Keep the screen inside the map: the view centre stays 120 pixels from
  // either side, 96 from the top and 64 from the bottom, the z limits moved
  // down the map by its height.
  minimumX = map->minX + toFIXED(120);
  objectZOffset = actor->y.fixed;
  minimumZ = map->minZ + objectZOffset + toFIXED(96);
  maximumX = map->maxX - toFIXED(120);
  maximumZ = map->maxZ + objectZOffset - toFIXED(64);
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
  if (shouldUseSetter == 0) {
    SET_ACTOR_POSITION(actor, positionX, positionY, positionZ);
    wait(1U);
    if (runtimeBlock->mode != ACTOR_SYSTEM_WORLD_MAP) {
      MapGrid_CenterOnFollowedPointFar();
    }
  } else {
    Actor_SetMoveTargetFar(actor, positionX, positionY, positionZ);
  }
}

/// The event command "camera move to actor": FieldCamera_MoveWithinMapBounds to
/// the actor's x and z.
void FieldCamera_MoveToActorById(s32 id, s32 use_setter) {
  struct Actor *obj = Actor_Lookup(id);

  if (obj != NULL)
    FieldCamera_MoveWithinMapBounds(obj->x.fixed, -1, obj->z.fixed, use_setter);
}

/// As FieldCamera_MoveToActorById, to the actor's height too.
void FieldCamera_MoveToActor(s32 id, s32 use_setter) {
  struct Actor *obj = Actor_Lookup(id);

  if (obj != NULL)
    FieldCamera_MoveWithinMapBounds(obj->x.fixed, obj->y.fixed, obj->z.fixed,
                                    use_setter);
}

/// The event command "camera wait for move": waits (at most 600 frames) for
/// the camera to arrive, then 2 frames more.
void FieldCamera_WaitForArrival(void) {
  Actor_WaitForMoveFar(((struct FieldWork *)HeapSlot_AllocEwram(
                            HEAP_SLOT_FIELD, sizeof(struct FieldWork)))
                           ->camera);
  Event_Wait(2);
}

/// The event command "get view centre": returns the camera actor.
struct Actor *FieldCamera_GetActor(void) {
  return ((struct FieldWork *)HeapSlot_AllocEwram(HEAP_SLOT_FIELD,
                                                  sizeof(struct FieldWork)))
      ->camera;
}

/// Unused: makes the camera follow target; unless keepCurrentPosition is set it
/// jumps there at once.
UNUSED void FieldCamera_Follow(struct Actor *target, s32 keepCurrentPosition) {
  struct Actor *camera;

  camera = ((struct FieldWork *)HeapSlot_AllocEwram(HEAP_SLOT_FIELD,
                                                    sizeof(struct FieldWork)))
               ->camera;
  if (target != NULL) {
    Actor_StartCameraFar(camera, 0);
    camera->watchedActor = target;
    if (keepCurrentPosition == 0) {
      SET_ACTOR_POSITION(camera, target->x.fixed, target->y.fixed,
                         target->z.fixed);
    }
  }
}
