/* What fdMapGridCheckCollision says of a step to a position (COLLISION), under
   The Broken Seal's name for it. */
static inline Sint32 scFieldActorCheckMovementCollision(
	ACTOR *actor, Sint32 *position)
{
	return fdMapGridCheckCollision(actor, position);
}

/* Finds where the actor ahead stops when pushed: a metatile at a time, the
   whole of its rectangle (PushRectangles) until a wall blocks it; 1 when it
   moves at all. */
Bool scPushTargetFindStop(PUSH_TARGET *target)
{
	ACTOR *actor;
	Sint32 direction;
	FIXED  position[3];
	Sint32 rectangle;
	Sint32 width;
	Sint32 height;
	Sint32 dx;
	FIXED  dz;
	Sint32 count;
	Sint32 row;
	Sint32 col;
	Uint8 *layer;

	actor = scPushTargetFindAhead(&direction, &target->id, &target->rectangle);
	if(actor != 0) {
		rectangle = target->rectangle;
		width = (ABS(PushRectangles[rectangle][0]) +
					ABS(PushRectangles[rectangle][2])) >>
			4;
		height = (ABS(PushRectangles[rectangle][1]) +
					 ABS(PushRectangles[rectangle][3])) >>
			4;
		/* A push step holds x pixels in its high half and z in its low. */
		dx = PushLeaderSteps[direction] & FIELD_PIXEL_MASK;
		dz = toFIXED(PushLeaderSteps[direction]);
		target->func = 0;
		count = 0;
		layer = &actor->layer;
		*layer = MAPGRID_LAYER_OVERHEAD;
		target->x = actor->x.fixed + dx;
		target->z = actor->z.fixed + dz;
		position[1] = actor->y.fixed;
		for(;;) {
			position[2] = target->z + toFIXED(PushRectangles[rectangle][1]);
			for(row = 0; row < height; row++) {
				position[0] = target->x + toFIXED(PushRectangles[rectangle][0]);
				for(col = 0; col < width; col++) {
					if(scFieldActorCheckMovementCollision(actor, position) ==
						COLLISION_IMPASSABLE)
						goto blocked;
					position[0] += toFIXED(MAPGRID_METATILE_PIXELS);
				}
				position[2] += toFIXED(MAPGRID_METATILE_PIXELS);
			}
			target->x += dx;
			count++;
			target->z += dz;
		}
	blocked:
		*layer = MAPGRID_LAYER_GROUND;
		if(count == 0)
			return FALSE;
		target->x = actor->x.fixed + dx * count;
		target->y = actor->y.fixed;
		target->z = actor->z.fixed + dz * count;
		return TRUE;
	}
	return FALSE;
}
