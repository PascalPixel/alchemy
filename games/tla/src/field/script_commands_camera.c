// The script commands that move the camera and wait for it. Part of script_commands.c.

// The script command "camera move to": sends the camera to the requested axes, 16.16 pixels (-1
// keeps that axis), kept far enough inside the map that the screen never shows past its edge. With
// pan 0 the camera cuts there at once; otherwise it pans there at its own speed.
void fdCameraMoveWithinMapBounds(FIXED requestedX, FIXED requestedY, FIXED requestedZ, Sint32 pan)
{
	Sint32       panning;
	FIELD_STATE *state;
	Sint32       minimumX;
	FIXED        minimumZ;
	Sint32       maximumX;
	FIXED        height;
	FIXED        maximumZ;
	FIXED        posX;
	FIXED        posZ;
	FIXED        posY;
	MAP_STATE   *map;
	ACTOR       *actor;

	posX        = requestedX;
	posY        = requestedY;
	panning     = pan;
	posZ        = requestedZ;
	state       = syHeapSlotAllocateEWRAM(HEAP_SLOT_FIELD, sizeof(FIELD_STATE));
	actor       = state->camera;
	map         = RamHeapSlots->mapGrid;
	// Keep the screen inside the map: the view centre stays 120 pixels from either side, 96 from
	// the top and 64 from the bottom, the z limits moved down the map by its height.
	minimumX    = map->minX + toFIXED(120);
	height      = actor->y.fixed;
	minimumZ    = map->minZ + height + toFIXED(96);
	maximumX    = map->maxX - toFIXED(120);
	maximumZ    = map->maxZ + height - toFIXED(64);
	map->origin = &actor->x.fixed;
	fdActorStartIdleStub(actor);
	if (posX == -1)
	{
		posX = actor->x.fixed;
	}
	if (posY == -1)
	{
		posY = actor->y.fixed;
	}
	if (posZ == -1)
	{
		posZ = actor->z.fixed;
	}
	if (posX < minimumX)
	{
		posX = minimumX;
	}
	if (posZ < minimumZ)
	{
		posZ = minimumZ;
	}
	if (posX > maximumX)
	{
		posX = maximumX;
	}
	if (posZ > maximumZ)
	{
		posZ = maximumZ;
	}
	if (panning == 0)
	{
		SET_ACTOR_POSITION(actor, posX, posY, posZ);
		sySynch(1U);
		if (state->mode != ACTOR_SYSTEM_WORLD_MAP)
		{
			fdMapGridCenterOnFollowedPointStub();
		}
	}
	else
	{
		fdActorSetMoveTargetStub(actor, posX, posY, posZ);
	}
}

// The script command "camera move to actor": fdCameraMoveWithinMapBounds to the actor's x and z.
void fdCameraMoveToActorById(Sint32 actorId, Sint32 pan)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
		fdCameraMoveWithinMapBounds(actor->x.fixed, -1, actor->z.fixed, pan);
}

// As fdCameraMoveToActorById, to the actor's height too.
void fdCameraMoveToActor(Sint32 actorId, Sint32 pan)
{
	ACTOR *actor = fdActorLookup(actorId);

	if (actor != NULL)
		fdCameraMoveWithinMapBounds(actor->x.fixed, actor->y.fixed, actor->z.fixed, pan);
}

// The script command "camera wait for move": waits (at most 600 ticks) for the camera to arrive,
// then 2 ticks more.
void fdCameraWaitForArrival(void)
{
	fdActorWaitForMoveStub(
		((FIELD_STATE *)syHeapSlotAllocateEWRAM(HEAP_SLOT_FIELD, sizeof(FIELD_STATE)))->camera);
	fdScriptWait(2);
}

// The script command "get camera": returns the camera's own actor, which the view centres on.
ACTOR *fdCameraGetActor(void)
{
	return ((FIELD_STATE *)syHeapSlotAllocateEWRAM(HEAP_SLOT_FIELD, sizeof(FIELD_STATE)))->camera;
}

// Unused: makes the camera follow target; unless keepCurrentPos is set it jumps there at once.
UNUSED void fdCameraFollow(ACTOR *target, Sint32 keepCurrentPos)
{
	ACTOR *camera;

	camera = ((FIELD_STATE *)syHeapSlotAllocateEWRAM(HEAP_SLOT_FIELD, sizeof(FIELD_STATE)))->camera;
	if (target != NULL)
	{
		fdActorStartCameraStub(camera, 0);
		camera->watchedActor = (union FIELD_OBJECT *)target;
		if (keepCurrentPos == 0)
		{
			SET_ACTOR_POSITION(camera, target->x.fixed, target->y.fixed, target->z.fixed);
		}
	}
}
