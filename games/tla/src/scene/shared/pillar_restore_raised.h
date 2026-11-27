/* Puts the crumbling pillars of a list back where their flags left them: one */
/* whose second flag is set has fallen and is put away; the others stand on */
/* their cells raised 32 pixels, gaps to jump onto, in animation 0 once their */
/* first flag is set, as the first step on one leaves it. Then a leader left */
/* below the ground is lifted onto it. */
void scMagmaRockInsideRestoreCrumblingPillars(const u16 *table) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	ACTOR *actor;
	s32 id;
	s32 flag;
	s32 nextFlag;
	MAP_CELL *cells;
	u8 *shapes;
	MAP_CELL *cell;
	s32 index;
	ACTOR *leader;
	s32 ground;

	for(; *table != ACTOR_LIST_END; table += 2) {
		id = table[0];
		actor = fdActorGet(id);
		flag = table[1];
		nextFlag = flag + 1;
		cells = map->layers[actor->layer].cells;
		shapes = MapShapeGrid + (cells - (MAP_CELL *)Ram_MapCellBuffer);
		if(fdEventGetFlag(nextFlag)) {
			fdActorSetPos(id, 0, 0);
		}
		else {
			s32 shape;
			u8 *slot;
			s32 height;

			index = fdActorGetCellIndex(actor);
			cell = &cells[index];
			slot = shapes + index;
			shape = fdMapGridGetTerrainKind(actor->layer, actor->x.fixed,
											actor->z.fixed);
			height = fdMapGetTerrainHeight(actor->layer, actor->x.fixed,
										   actor->z.fixed);
			fdActorDisableById(id);
			cell->attributeB = MAP_CELL_JUMP;
			*slot = fdMapShapeFindOrAdd(actor->layer, shape,
										CELL_HEIGHT_STEPS(height) +
											CELL_HEIGHT_STEPS(toFIXED(32)));
			if(fdEventGetFlag(flag))
				fdActorSelectAnimation(actor, 0);
		}
	}
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ground =
		fdMapGetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
	if(leader->y.fixed < ground) {
		leader->groundHeight = ground;
		leader->y.fixed = ground;
	}
}
