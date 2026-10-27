/// @file
/// The script camera.
// Part of field_services.c.

#include "../field_services_internal.h"

/// The script command "camera follow actor": the camera (the script's
/// view centre) follows actorId. Unless keepPosition is set it jumps there
/// at once and, off the world map, the map is redrawn around it.
/// FieldScript_End calls it with the leader and keepPosition 1, so after a
/// script the view follows the leader again from where it is, without a jump.
void FieldCamera_FollowActor(s32 actorId, s32 keepPosition) {
  struct Actor *subject;
  struct Actor *camera;
  struct FieldWork *work;
  struct MapGridScrollWork *grid;

  subject = Actor_Lookup(actorId);
  // The slot already holds the field work, which this returns; the camera
  // commands here all find it this way.
  work = AllocHeapSlotEwram(HEAP_SLOT_FIELD, sizeof(struct FieldWork));
  camera = work->camera;
  grid = gMapGridWork[0];
  if (subject != 0) {
    grid->origin = &camera->x.fixed;
    Actor_StartCameraFar(camera, (s32)subject);
    if (keepPosition == 0) {
      SET_ACTOR_POSITION(camera, subject->x.fixed, subject->y.fixed,
                         subject->z.fixed);
      WaitFrames(1);
      if (work->mode != ACTOR_SYSTEM_WORLD_MAP) {
        MapGrid_CenterOnFollowedPointFar();
      }
    }
  }
}

/// The script command "camera set speed": the camera's top speed and
/// acceleration, 16.16 pixels per frame.
void FieldCamera_SetSpeedParameters(s32 speedLimit, s32 acceleration) {
  struct FieldWork *owner =
      AllocHeapSlotEwram(HEAP_SLOT_FIELD, sizeof(struct FieldWork));
  owner->camera->speedLimit = speedLimit;
  owner->camera->acceleration = acceleration;
}

/// The script command "camera move to": sends the camera to the requested axes,
/// 16.16 pixels (-1 keeps that axis), kept far enough inside the map that the
/// screen never shows past its edge. With glide 0 the camera jumps there at
/// once; otherwise it travels at its own speed.
void FieldCamera_MoveWithinMapBounds(s32 requestedX, s32 requestedY,
                                     s32 requestedZ, s32 glide) {
  s32 shouldGlide;
  struct FieldWork *runtimeBlock;
  s32 minimumX;
  s32 minimumZ;
  s32 maximumX;
  s32 cameraY;
  s32 maximumZ;
  s32 positionX;
  s32 positionZ;
  s32 positionY;
  struct MapGridScrollWork *grid;
  struct Actor *camera;

  positionX = requestedX;
  positionY = requestedY;
  shouldGlide = glide;
  positionZ = requestedZ;
  runtimeBlock = AllocHeapSlotEwram(HEAP_SLOT_FIELD, sizeof(struct FieldWork));
  camera = runtimeBlock->camera;
  grid = gMapGridWork[0];

  // Keep the screen inside the map: the view centre stays 120 pixels from
  // either side, 96 from the top and 64 from the bottom (it sits 96 pixels
  // down the 160-pixel screen), the z limits moved down the map by its
  // height, since it shows at z less its height.
  minimumX = grid->minX + 0x780000;
  cameraY = camera->y.fixed;
  minimumZ = grid->minY + cameraY + 0x600000;
  maximumX = grid->maxX + 0xFF880000;
  maximumZ = grid->maxY + cameraY + 0xFFC00000;
  grid->origin = &camera->x.fixed;
  Actor_StartIdleFar(camera);
  if (positionX == -1) {
    positionX = camera->x.fixed;
  }
  if (positionY == -1) {
    positionY = camera->y.fixed;
  }
  if (positionZ == -1) {
    positionZ = camera->z.fixed;
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
  if (shouldGlide == 0) {
    SET_ACTOR_POSITION(camera, positionX, positionY, positionZ);
    WaitFrames(1U);
    if (runtimeBlock->mode != ACTOR_SYSTEM_WORLD_MAP) {
      MapGrid_CenterOnFollowedPointFar();
    }
  } else {
    Actor_SetMoveTargetFar(camera, positionX, positionY, positionZ);
  }
}

/// The script command "camera move to actor":
/// FieldCamera_MoveWithinMapBounds to the actor's x and z.
void Actor_PlaceCurrentWithinCameraBounds(s32 actorId, s32 glide) {
  struct Actor *subject;

  subject = Actor_Lookup(actorId);
  // The field work, looked up and not used.
  AllocHeapSlotEwram(HEAP_SLOT_FIELD, 0xCCC);
  if (subject != NULL) {
    FieldCamera_MoveWithinMapBounds(subject->x.fixed, -1, subject->z.fixed,
                                    glide);
  }
}

/// The script command "camera wait for move": waits (at most 600 frames) for
/// the camera to arrive, then 2 frames more.
void FieldCamera_WaitForArrival(void) {
  Actor_WaitForMoveFar(((struct FieldWork *)AllocHeapSlotEwram(
                            HEAP_SLOT_FIELD, sizeof(struct FieldWork)))
                           ->camera);
  FieldScript_WaitFrames(2);
}

/// The script command "get view centre": returns the camera object.
struct Actor *FieldCamera_GetActor(void) {
  return ((struct FieldWork *)AllocHeapSlotEwram(HEAP_SLOT_FIELD,
                                                 sizeof(struct FieldWork)))
      ->camera;
}

/// Unused: makes the camera follow target; unless keepCurrentPosition is set it
/// jumps there at once.
UNUSED void FieldCamera_Follow(struct Actor *target, s32 keepCurrentPosition) {
  struct Actor *camera;

  camera = ((struct FieldWork *)AllocHeapSlotEwram(HEAP_SLOT_FIELD,
                                                   sizeof(struct FieldWork)))
               ->camera;
  if (target != NULL) {
    Actor_StartCameraFar(camera, 0);
    camera->linkedActor = target;
    if (keepCurrentPosition == 0) {
      SET_ACTOR_POSITION(camera, target->x.fixed, target->y.fixed,
                         target->z.fixed);
    }
  }
}
