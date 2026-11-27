/* Raises the metatile under each listed pillar 32 pixels, to its top, and marks
   it MAP_METATILE_JUMP and MAP_METATILE_CLIMBABLE, and the metatile south of
   it, its face, climbable; then puts the leader back on the ground if it rose
   over him. */
void scPillarRaise(Uint16 *list)
{
	MAP_STATE *map = RamHeapSlots->mapGrid;
	ACTOR	  *leader;
	Sint32	   height;

	for(; *list != ACTOR_LIST_END; list++) {
		ACTOR *actor = fdActorGetById(*list);
		Uint8 *layer = &actor->layer;
		Sint32 shape =
			fdMapGridGetTerrainKind(*layer, actor->x.fixed, actor->z.fixed);
		Sint32 level =
			fdMapGetTerrainHeight(*layer, actor->x.fixed, actor->z.fixed);
		Sint32 index;
		Uint8 *metatiles;
		Uint8 *shapes;
		Uint8  newShape;
		Uint8 *grid;

		level = METATILE_HEIGHT_STEPS(level);
		index = fdActorGetMetatileIndex(actor);
		metatiles = (Uint8 *)map->layers[*layer].metatiles;
		grid = RamMapShapeGrid;
		shapes = grid +
			(((MAP_METATILE *)metatiles) -
				(MAP_METATILE *)RamMapMetatileBuffer);
		actor->motionFlags = 0;
		actor->collisionFlags = 0;
		fdActorSetSpriteFlags(actor, 0);
		level += METATILE_HEIGHT_STEPS(toFIXED(32));
		newShape = fdMapShapeFindOrAdd(*layer, shape, level);
		shapes += index;
		index <<= 2;
		((SHAPE_METATILE *)shapes)->shape = newShape;
		metatiles += index;
		((MAP_METATILE *)metatiles)->attributeB |=
			MAP_METATILE_JUMP | MAP_METATILE_CLIMBABLE;
		metatiles += MAPGRID_SIZE * sizeof(MAP_METATILE);
		((MAP_METATILE *)metatiles)->attributeB |= MAP_METATILE_CLIMBABLE;
	}
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	height =
		fdMapGetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
	if(leader->y.fixed < height) {
		leader->groundHeight = height;
		leader->y.fixed = height;
	}
}
