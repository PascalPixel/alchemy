/* What fdMapGridCheckCollision says of a step to a position (COLLISION), under
   The Broken Seal's name for it. */
static inline Sint32 scFieldActorCheckMovementCollision(
	ACTOR *actor, Sint32 *pos)
{
	return fdMapGridCheckCollision(actor, pos);
}

/* Finds where the actor ahead stops when pushed: a metatile at a time, the
   whole of its rectangle (PushRectangles) until a wall blocks it; 1 when it
   moves at all. */
Bool scPushTargetFindStop(PUSH_TARGET *tgt)
{
	ACTOR *actor;
	Sint32 direction;
	FIXED  pos[3];
	Sint32 rectangle;
	Sint32 width;
	Sint32 height;
	Sint32 dx;
	FIXED  dz;
	Sint32 cnt;
	Sint32 row;
	Sint32 col;
	Uint8 *layer;

	actor = scPushTargetFindAhead(&direction, &tgt->id, &tgt->rectangle);
	if(actor != 0) {
		rectangle = tgt->rectangle;
		width = (ABS(PushRectangles[rectangle][0]) +
					ABS(PushRectangles[rectangle][2])) >>
			4;
		height = (ABS(PushRectangles[rectangle][1]) +
					 ABS(PushRectangles[rectangle][3])) >>
			4;
		/* A push step holds x pixels in its high half and z in its low. */
		dx = PushLeaderSteps[direction] & FIELD_PIXEL_MASK;
		dz = toFIXED(PushLeaderSteps[direction]);
		tgt->func = 0;
		cnt = 0;
		layer = &actor->layer;
		*layer = MAPGRID_LAYER_OVERHEAD;
		tgt->x = actor->x.fixed + dx;
		tgt->z = actor->z.fixed + dz;
		pos[1] = actor->y.fixed;
		for(;;) {
			pos[2] = tgt->z + toFIXED(PushRectangles[rectangle][1]);
			for(row = 0; row < height; row++) {
				pos[0] = tgt->x + toFIXED(PushRectangles[rectangle][0]);
				for(col = 0; col < width; col++) {
					if(scFieldActorCheckMovementCollision(actor, pos) ==
						COLLISION_IMPASSABLE)
						goto blocked;
					pos[0] += toFIXED(MAPGRID_METATILE_PIXELS);
				}
				pos[2] += toFIXED(MAPGRID_METATILE_PIXELS);
			}
			tgt->x += dx;
			cnt++;
			tgt->z += dz;
		}
	blocked:
		*layer = MAPGRID_LAYER_GROUND;
		if(cnt == 0)
			return FALSE;
		tgt->x = actor->x.fixed + dx * cnt;
		tgt->y = actor->y.fixed;
		tgt->z = actor->z.fixed + dz * cnt;
		return TRUE;
	}
	return FALSE;
}
