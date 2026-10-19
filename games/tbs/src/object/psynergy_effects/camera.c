/// @file
/// The event camera.
/// Part of psynergy_effects.c, compiled only through it.

#include "../psynergy_effects_internal.h"

/// The event command "camera follow actor": the camera (the event's view
/// centre) follows actor. Unless keepPosition is set it jumps there at once
/// and, off the world map, the map is redrawn around it.
void Camera_FollowActor(s32 actor, s32 keepPosition) {
  struct ObjectRuntime *subject;
  struct ObjectRuntime *camera;
  struct FieldWork *work;
  struct MapScrollWork *map;

  subject = ObjectTable_Get(actor);
  work = Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(struct FieldWork));
  camera = work->camera;
  map = gMapWork[0];
  if (subject != 0) {
    map->origin = &camera->x;
    ObjectDispatch_StartCameraFar(camera, subject);
    if (keepPosition == 0) {
      SET_POSITION(camera, subject->x, subject->y, subject->z);
      WaitFrames(1);
      if (work->mode != MODE_WORLD_MAP) {
        Map_CenterOnFollowedPointFar();
      }
    }
  }
}

/// The event command "camera set speed": the camera's top speed and
/// acceleration, 16.16 pixels per frame.
void Camera_SetSpeedParameters(s32 speedLimit, s32 acceleration) {
  struct FieldWork *owner =
      Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(struct FieldWork));
  owner->camera->speedLimit = speedLimit;
  owner->camera->acceleration = acceleration;
}

/// The event command "camera move to": sends the camera to the requested axes,
/// 16.16 pixels (-1 keeps that axis), kept far enough inside the map that the
/// screen never shows past its edge. With glide 0 the camera jumps there at
/// once; otherwise it travels at its own speed.
void Camera_MoveWithinMapBounds(s32 requestedX, s32 requestedY, s32 requestedZ,
                                s32 glide) {
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
  struct MapScrollWork *map;
  struct ObjectRuntime *camera;

  positionX = requestedX;
  positionY = requestedY;
  shouldGlide = glide;
  positionZ = requestedZ;
  runtimeBlock =
      Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(struct FieldWork));
  camera = runtimeBlock->camera;
  map = gMapWork[0];

  // Half a screen (120 by 64 to 96 pixels) inside the map's bounds.
  minimumX = map->minX + 0x780000;
  cameraY = camera->y;
  minimumZ = map->minY + cameraY + 0x600000;
  maximumX = map->maxX + 0xFF880000;
  maximumZ = map->maxY + cameraY + 0xFFC00000;
  map->origin = &camera->x;
  ObjectDispatch_StartIdleFar(camera);
  if (positionX == -1) {
    positionX = camera->x;
  }
  if (positionY == -1) {
    positionY = camera->y;
  }
  if (positionZ == -1) {
    positionZ = camera->z;
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
    SET_POSITION(camera, positionX, positionY, positionZ);
    WaitFrames(1U);
    if (runtimeBlock->mode != MODE_WORLD_MAP) {
      Map_CenterOnFollowedPointFar();
    }
  } else {
    Object_SetMoveTargetFar(camera, positionX, positionY, positionZ);
  }
}

/// The event command "camera move to actor": Camera_MoveWithinMapBounds to the
/// actor's x and z.
void Object_PlaceCurrentWithinCameraBounds(s32 actor, s32 glide) {
  struct ObjectRuntime *subject;

  subject = ObjectTable_Get(actor);
  Runtime_AllocateBlock(0x1B, 0xCCC);
  if (subject != NULL) {
    Camera_MoveWithinMapBounds(subject->x, -1, subject->z, glide);
  }
}

/// The event command "camera wait for move": waits (at most 600 frames) for
/// the camera to arrive, then 2 frames more.
void Camera_WaitForArrival(void) {
  FieldObject_WaitForMoveFar(((struct FieldWork *)Runtime_AllocateBlock(
                                  HEAP_SLOT_EVENT, sizeof(struct FieldWork)))
                                 ->camera);
  FieldEvent_WaitFrames(2);
}

/// The event command "get view centre": returns the camera object.
struct FieldActor *Camera_GetEventObject(void) {
  return (struct FieldActor *)((struct FieldWork *)Runtime_AllocateBlock(
                                   HEAP_SLOT_EVENT, sizeof(struct FieldWork)))
      ->camera;
}

/// Makes the camera follow target; unless keepCurrentPosition is set it jumps
/// there at once.
UNUSED void Camera_FollowObject(struct ObjectRuntime *target,
                                s32 keepCurrentPosition) {
  struct ObjectRuntime *camera;

  camera = ((struct FieldWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT,
                                                      sizeof(struct FieldWork)))
               ->camera;
  if (target != NULL) {
    ObjectDispatch_StartCameraFar(camera, NULL);
    camera->linkedObject = target;
    if (keepCurrentPosition == 0) {
      SET_POSITION(camera, target->x, target->y, target->z);
    }
  }
}
