/// @file
/// The camera an event moves: following an actor, panning to a point kept
/// inside the map, and waiting for it to arrive.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// Makes the camera, the event's view centre, follow actorId.
void FieldCamera_FollowActor(s32 actorId, s32 keepPosition) {
  struct Actor *subject;
  struct Actor *camera;
  struct FieldState *state;
  struct MapGridScrollState *grid;

  subject = Actor_Lookup(actorId);
  // The slot already holds the field state, which this returns; the camera
  // commands here all find it this way.
  state = HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(struct FieldState));
  camera = state->camera;
  grid = gMapGridState[0];
  if (subject != 0) {
    grid->origin = &camera->x.fixed;
    Actor_StartCameraFar(camera, (s32)subject);
    if (keepPosition == 0) {
      SET_ACTOR_POSITION(camera, subject->x.fixed, subject->y.fixed,
                         subject->z.fixed);
      wait(1);
      if (state->mode != ACTOR_SYSTEM_WORLD_MAP) {
        MapGrid_CenterOnFollowedPointFar();
      }
    }
  }
}

/// Sets the camera's top speed and acceleration, in 16.16 pixels per frame.
void FieldCamera_SetSpeedParameters(FIXED speedLimit, FIXED acceleration) {
  struct FieldState *fieldState =
      HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(struct FieldState));
  fieldState->camera->speedLimit = speedLimit;
  fieldState->camera->acceleration = acceleration;
}

/// Pans the camera to a point in 16.16 pixels (CAMERA_KEEP keeps that axis),
/// or cuts there when glide is 0, kept far enough inside the map that the
/// screen never shows past its edge.
void FieldCamera_MoveWithinMapBounds(FIXED requestedX, FIXED requestedY,
                                     FIXED requestedZ, s32 glide) {
  s32 shouldGlide;
  struct FieldState *fieldState;
  FIXED minimumX;
  FIXED minimumZ;
  FIXED maximumX;
  FIXED cameraY;
  FIXED maximumZ;
  FIXED positionX;
  FIXED positionZ;
  FIXED positionY;
  struct MapGridScrollState *grid;
  struct Actor *camera;

  positionX = requestedX;
  positionY = requestedY;
  shouldGlide = glide;
  positionZ = requestedZ;
  fieldState = HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(struct FieldState));
  camera = fieldState->camera;
  grid = gMapGridState[0];

  // Keep the screen inside the map: the view centre stays 120 pixels from
  // either side, 96 from the top and 64 from the bottom (it sits 96 pixels
  // down the 160-pixel screen), the z limits moved down the map by its
  // height, since it shows at z less its height.
  minimumX = grid->minX + toFIXED(120);
  cameraY = camera->y.fixed;
  minimumZ = grid->minY + cameraY + toFIXED(96);
  maximumX = grid->maxX - toFIXED(120);
  maximumZ = grid->maxY + cameraY - toFIXED(64);
  grid->origin = &camera->x.fixed;
  Actor_StartIdleFar(camera);
  if (positionX == CAMERA_KEEP) {
    positionX = camera->x.fixed;
  }
  if (positionY == CAMERA_KEEP) {
    positionY = camera->y.fixed;
  }
  if (positionZ == CAMERA_KEEP) {
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
    wait(1);
    if (fieldState->mode != ACTOR_SYSTEM_WORLD_MAP) {
      MapGrid_CenterOnFollowedPointFar();
    }
  } else {
    Actor_SetMoveTargetFar(camera, positionX, positionY, positionZ);
  }
}

/// Moves the camera to an actor's x and z, as FieldCamera_MoveWithinMapBounds
/// does to a point.
void FieldCamera_MoveToActorById(s32 actorId, s32 glide) {
  struct Actor *subject;

  subject = Actor_Lookup(actorId);
  // The field state, looked up and not used.
  HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(struct FieldState));
  if (subject != NULL) {
    FieldCamera_MoveWithinMapBounds(subject->x.fixed, CAMERA_KEEP,
                                    subject->z.fixed, glide);
  }
}

/// Waits (at most 600 frames) for the camera to arrive, then 2 frames more.
void FieldCamera_WaitForArrival(void) {
  Actor_WaitForMoveFar(((struct FieldState *)HeapSlot_AllocEwram(
                            HEAP_SLOT_FIELD, sizeof(struct FieldState)))
                           ->camera);
  Event_Wait(2);
}

/// Returns the camera actor.
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
    camera->watchedActor = (union ActorOrEffect *)target;
    if (keepCurrentPosition == 0) {
      SET_ACTOR_POSITION(camera, target->x.fixed, target->y.fixed,
                         target->z.fixed);
    }
  }
}
