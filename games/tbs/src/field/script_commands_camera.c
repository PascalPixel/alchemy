/* The camera a script moves: following an actor, panning to a point kept inside
   the map, and waiting for it to arrive. Part of script_commands.c. */

#include "script_commands_internal.h"

/* Makes the camera, the script's view centre, follow actorId. */
void fdCameraFollowActor(Sint32 actorId, Sint32 keepPos)
{
	ACTOR				  *subject;
	ACTOR				  *camera;
	FIELD_STATE			  *state;
	MAP_GRID_SCROLL_STATE *grid;

	subject = fdActorLookup(actorId);
	/* The slot already holds the field state, which this returns; the camera
	   commands here all find it this way. */
	state = syHeapSlotAllocateEWRAM(HEAP_SLOT_FIELD, sizeof(FIELD_STATE));
	camera = state->camera;
	grid = MapGridState[0];
	if(subject != 0) {
		grid->origin = &camera->x.fixed;
		fdActorStartCameraStub(camera, (Sint32)subject);
		if(keepPos == 0) {
			SET_ACTOR_POSITION(
				camera, subject->x.fixed, subject->y.fixed, subject->z.fixed);
			sySynch(1);
			if(state->mode != ACTOR_SYSTEM_WORLD_MAP) {
				fdMapGridCenterOnFollowedPointStub();
			}
		}
	}
}

/* Sets the camera's top speed and acceleration, in 16.16 pixels per tick. */
void fdCameraSetSpeedParams(FIXED speedLimit, FIXED acceleration)
{
	FIELD_STATE *fieldState =
		syHeapSlotAllocateEWRAM(HEAP_SLOT_FIELD, sizeof(FIELD_STATE));
	fieldState->camera->speedLimit = speedLimit;
	fieldState->camera->acceleration = acceleration;
}

/* Pans the camera to a point in 16.16 pixels (CAMERA_KEEP keeps that axis), or
   cuts there when glide is 0, kept far enough inside the map that the screen
   never shows past its edge. */
void fdCameraMoveWithinMapBounds(
	FIXED requestedX, FIXED requestedY, FIXED requestedZ, Sint32 glide)
{
	Sint32				   shouldGlide;
	FIELD_STATE			  *fieldState;
	FIXED				   minimumX;
	FIXED				   minimumZ;
	FIXED				   maximumX;
	FIXED				   cameraY;
	FIXED				   maximumZ;
	FIXED				   posX;
	FIXED				   posZ;
	FIXED				   posY;
	MAP_GRID_SCROLL_STATE *grid;
	ACTOR				  *camera;

	posX = requestedX;
	posY = requestedY;
	shouldGlide = glide;
	posZ = requestedZ;
	fieldState = syHeapSlotAllocateEWRAM(HEAP_SLOT_FIELD, sizeof(FIELD_STATE));
	camera = fieldState->camera;
	grid = MapGridState[0];

	/* Keep the screen inside the map: the view centre stays 120 pixels from
	   either side, 96 from the top and 64 from the bottom (it sits 96 pixels
	   down the 160-pixel screen), the z limits moved down the map by its
	   height, since it shows at z less its height. */
	minimumX = grid->minX + toFIXED(120);
	cameraY = camera->y.fixed;
	minimumZ = grid->minY + cameraY + toFIXED(96);
	maximumX = grid->maxX - toFIXED(120);
	maximumZ = grid->maxY + cameraY - toFIXED(64);
	grid->origin = &camera->x.fixed;
	fdActorStartIdleStub(camera);
	if(posX == CAMERA_KEEP) {
		posX = camera->x.fixed;
	}
	if(posY == CAMERA_KEEP) {
		posY = camera->y.fixed;
	}
	if(posZ == CAMERA_KEEP) {
		posZ = camera->z.fixed;
	}
	if(posX < minimumX) {
		posX = minimumX;
	}
	if(posZ < minimumZ) {
		posZ = minimumZ;
	}
	if(posX > maximumX) {
		posX = maximumX;
	}
	if(posZ > maximumZ) {
		posZ = maximumZ;
	}
	if(shouldGlide == 0) {
		SET_ACTOR_POSITION(camera, posX, posY, posZ);
		sySynch(1);
		if(fieldState->mode != ACTOR_SYSTEM_WORLD_MAP) {
			fdMapGridCenterOnFollowedPointStub();
		}
	} else {
		fdActorSetMoveTargetStub(camera, posX, posY, posZ);
	}
}

/* Moves the camera to an actor's x and z, as fdCameraMoveWithinMapBounds does
   to a point. */
void fdCameraMoveToActorById(Sint32 actorId, Sint32 glide)
{
	ACTOR *subject;

	subject = fdActorLookup(actorId);
	/* The field state, looked up and not used. */
	syHeapSlotAllocateEWRAM(HEAP_SLOT_FIELD, sizeof(FIELD_STATE));
	if(subject != NULL) {
		fdCameraMoveWithinMapBounds(
			subject->x.fixed, CAMERA_KEEP, subject->z.fixed, glide);
	}
}

/* Waits (at most 600 ticks) for the camera to arrive, then 2 ticks more. */
void fdCameraWaitForArrival(void)
{
	fdActorWaitForMoveStub(((FIELD_STATE *)syHeapSlotAllocateEWRAM(
								HEAP_SLOT_FIELD, sizeof(FIELD_STATE)))
			->camera);
	fdScriptWait(2);
}

/* Returns the camera actor. */
ACTOR *fdCameraGetActor(void)
{
	return ((FIELD_STATE *)syHeapSlotAllocateEWRAM(
				HEAP_SLOT_FIELD, sizeof(FIELD_STATE)))
		->camera;
}

/* Unused: makes the camera follow target; unless keepCurrentPos is set it
   jumps there at once. */
UNUSED void fdCameraFollow(ACTOR *target, Sint32 keepCurrentPos)
{
	ACTOR *camera;

	camera = ((FIELD_STATE *)syHeapSlotAllocateEWRAM(
				  HEAP_SLOT_FIELD, sizeof(FIELD_STATE)))
				 ->camera;
	if(target != NULL) {
		fdActorStartCameraStub(camera, 0);
		camera->watchedActor = (union ACTOR_OR_EFFECT *)target;
		if(keepCurrentPos == 0) {
			SET_ACTOR_POSITION(
				camera, target->x.fixed, target->y.fixed, target->z.fixed);
		}
	}
}
