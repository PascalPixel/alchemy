/// @file
/// The script camera.
// Part of psynergy_effects.c.

#include "../psynergy_effects_internal.h"

/// The script command "camera follow object event": the camera (the script's
/// view centre) follows localId. Unless keepPosition is set it jumps there
/// at once and, off the world map, the map is redrawn around it.
/// FieldScript_End calls it with the leader and keepPosition 1, so after a
/// script the view follows the leader again from where it is, without a jump.
void Camera_FollowObjectEvent(s32 localId, s32 keepPosition) {
  struct ObjectRuntime *subject;
  struct ObjectRuntime *camera;
  struct FieldWork *work;
  struct MapGridScrollWork *grid;

  subject = ObjectTable_Get(localId);
  // The slot already holds the field work, which this returns; the camera
  // commands here all find it this way.
  work = Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(struct FieldWork));
  camera = work->camera;
  grid = gMapGridWork[0];
  if (subject != 0) {
    grid->origin = &camera->x;
    ObjectDispatch_StartCameraFar((struct DispatchObject *)camera,
                                  (s32)subject);
    if (keepPosition == 0) {
      SET_POSITION(camera, subject->x, subject->y, subject->z);
      WaitFrames(1);
      if (work->mode != MODE_WORLD_MAP) {
        MapGrid_CenterOnFollowedPointFar();
      }
    }
  }
}

/// The script command "camera set speed": the camera's top speed and
/// acceleration, 16.16 pixels per frame.
void Camera_SetSpeedParameters(s32 speedLimit, s32 acceleration) {
  struct FieldWork *owner =
      Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(struct FieldWork));
  owner->camera->speedLimit = speedLimit;
  owner->camera->acceleration = acceleration;
}

/// The script command "camera move to": sends the camera to the requested axes,
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
  struct MapGridScrollWork *grid;
  struct ObjectRuntime *camera;

  positionX = requestedX;
  positionY = requestedY;
  shouldGlide = glide;
  positionZ = requestedZ;
  runtimeBlock =
      Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(struct FieldWork));
  camera = runtimeBlock->camera;
  grid = gMapGridWork[0];

  // Keep the screen inside the map: the view centre stays 120 pixels from
  // either side, 96 from the top and 64 from the bottom (it sits 96 pixels
  // down the 160-pixel screen), the z limits moved down the map by its
  // height, since it shows at z less its height.
  minimumX = grid->minX + 0x780000;
  cameraY = camera->y;
  minimumZ = grid->minY + cameraY + 0x600000;
  maximumX = grid->maxX + 0xFF880000;
  maximumZ = grid->maxY + cameraY + 0xFFC00000;
  grid->origin = &camera->x;
  ObjectDispatch_StartIdleFar((struct DispatchObject *)camera);
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
      MapGrid_CenterOnFollowedPointFar();
    }
  } else {
    Object_SetMoveTargetFar(camera, positionX, positionY, positionZ);
  }
}

/// The script command "camera move to object event": Camera_MoveWithinMapBounds
/// to the object event's x and z.
void ObjectEvent_PlaceCurrentWithinCameraBounds(s32 localId, s32 glide) {
  struct ObjectRuntime *subject;

  subject = ObjectTable_Get(localId);
  // The field work, looked up and not used.
  Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xCCC);
  if (subject != NULL) {
    Camera_MoveWithinMapBounds(subject->x, -1, subject->z, glide);
  }
}

/// The script command "camera wait for move": waits (at most 600 frames) for
/// the camera to arrive, then 2 frames more.
void Camera_WaitForArrival(void) {
  FieldObject_WaitForMoveFar(((struct FieldWork *)Runtime_AllocateBlock(
                                  HEAP_SLOT_EVENT, sizeof(struct FieldWork)))
                                 ->camera);
  FieldScript_WaitFrames(2);
}

/// The script command "get view centre": returns the camera object.
struct ObjectEvent *Camera_GetScriptObject(void) {
  return (struct ObjectEvent *)((struct FieldWork *)Runtime_AllocateBlock(
                                    HEAP_SLOT_EVENT, sizeof(struct FieldWork)))
      ->camera;
}

/// Unused: makes the camera follow target; unless keepCurrentPosition is set it
/// jumps there at once.
/// Nothing calls it.
UNUSED void Camera_FollowObject(struct ObjectRuntime *target,
                                s32 keepCurrentPosition) {
  struct ObjectRuntime *camera;

  camera = ((struct FieldWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT,
                                                      sizeof(struct FieldWork)))
               ->camera;
  if (target != NULL) {
    ObjectDispatch_StartCameraFar((struct DispatchObject *)camera, 0);
    camera->linkedObject = target;
    if (keepCurrentPosition == 0) {
      SET_POSITION(camera, target->x, target->y, target->z);
    }
  }
}
