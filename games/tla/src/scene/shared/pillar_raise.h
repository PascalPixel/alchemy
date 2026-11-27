/* Raises the cell under each listed pillar 32 pixels, to its top, and */
/* marks it MAP_CELL_JUMP and MAP_CELL_CLIMBABLE, and the cell south of it, its
 */
/* face, climbable; then puts the leader back on the ground if it rose over */
/* him. */
void scPillarRaise(u16 *list) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	ACTOR *leader;
	s32 height;

	for(; *list != ACTOR_LIST_END; list++) {
		ACTOR *actor = fdActorGetById(*list);
		u8 *layer = &actor->layer;
		s32 shape =
			fdMapGridGetTerrainKind(*layer, actor->x.fixed, actor->z.fixed);
		s32 level =
			fdMapGetTerrainHeight(*layer, actor->x.fixed, actor->z.fixed);
		s32 index;
		u8 *cells;
		u8 *shapes;
		u8 newShape;
		u8 *grid;

		level = CELL_HEIGHT_STEPS(level);
		index = fdActorGetCellIndex(actor);
		cells = (u8 *)map->layers[*layer].cells;
		grid = Ram_MapShapeGrid;
		shapes = grid + (((MAP_CELL *)cells) - (MAP_CELL *)Ram_MapCellBuffer);
		actor->motionFlg = 0;
		actor->collisionFlg = 0;
		fdActorSetSprFlags(actor, 0);
		level += CELL_HEIGHT_STEPS(toFIXED(32));
		newShape = fdMapShapeFindOrAdd(*layer, shape, level);
		shapes += index;
		index <<= 2;
		((SHAPE_CELL *)shapes)->shape = newShape;
		cells += index;
		((MAP_CELL *)cells)->attributeB |= MAP_CELL_JUMP | MAP_CELL_CLIMBABLE;
		cells += MAPGRID_SIZE * sizeof(MAP_CELL);
		((MAP_CELL *)cells)->attributeB |= MAP_CELL_CLIMBABLE;
	}
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	height =
		fdMapGetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
	if(leader->y.fixed < height) {
		leader->groundHeight = height;
		leader->y.fixed = height;
	}
}
