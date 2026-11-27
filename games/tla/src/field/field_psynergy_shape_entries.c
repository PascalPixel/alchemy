/* @file */
/* The map's shape entries and the flag blocks: the puddles Frost freezes */
/* into pillars of ice, each with the flag that says it is frozen. */
/* Part of field_psynergy.c. */

/* Sets up Alhafra's and the Alhafran Cave's puddles as fdFlagBlockPlaceAll */
/* does, and marks the open cells on each puddle's four sides a gap to jump */
/* across (fdMapGridMarkJumpIfOpen), so the leader jumps onto a frozen one. */
void fdFlagBlockPlaceAllMarkingEdges(ACTOR_FLAG *list) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	ACTOR_FLAG *first = list;
	ACTOR *leader;

	for(; list->actor != ACTOR_LIST_END; list++) {
		ACTOR *actor = fdActorGetById(list->actor);
		u16 flag = list->flg;
		s32 slot = fdActorGetCellIndex(actor);
		MAP_CELL *cell = &map->layers[actor->layer].cells[slot];

		actor->motionFlg = 0;
		actor->collisionFlg &= ~ACTOR_SOLID;
		fdActorSetSprFlagsStub(actor, 0);
		cell->collisionCode = CELL_BEHAVIOR_WALL;
		fdMapGridMarkJumpIfOpen(actor->x.fixed + toFIXED(MAPGRID_CELL_PIXELS),
								actor->z.fixed, actor->layer,
								actor->groundHeight);
		fdMapGridMarkJumpIfOpen(actor->x.fixed - toFIXED(MAPGRID_CELL_PIXELS),
								actor->z.fixed, actor->layer,
								actor->groundHeight);
		fdMapGridMarkJumpIfOpen(actor->x.fixed,
								actor->z.fixed + toFIXED(MAPGRID_CELL_PIXELS),
								actor->layer, actor->groundHeight);
		fdMapGridMarkJumpIfOpen(actor->x.fixed,
								actor->z.fixed - toFIXED(MAPGRID_CELL_PIXELS),
								actor->layer, actor->groundHeight);
		if(syGameFlagGet(flag)) {
			fdActorSelectAnimationStub(actor, PUDDLE_ANIMATION_FROZEN);
			syGameFlagClear(flag);
		}
		else {
			fdActorSelectAnimationStub(actor, PUDDLE_ANIMATION_PUDDLE);
		}
	}
	fdFlagBlockApplyFlags(first);
	leader = fdActorGetById(fdPartyGetLeaderActor());
	if(fdMapGridGetTerrainHeightAboveStub(leader->x.fixed, leader->z.fixed,
										  leader->layer, leader->groundHeight))
		leader->groundHeight = leader->y.fixed = fdMapGridGetTerrainHeightStub(
			leader->layer, leader->x.fixed, leader->z.fixed);
}

/* For each flag block whose animation disagrees with its flag, turns its cell
 */
/* and flag over: with the flag set and the ice pillar not showing, the cell */
/* is lowered 32 pixels to a wall and the flag cleared; with the flag clear */
/* and the puddle not showing (Frost freezing it), the cell is raised 32 */
/* pixels, open and a gap to jump across, and the flag set. The field state */
/* remembers the block. */
void fdFlagBlockApplyFlags(ACTOR_FLAG *list) {
	FIELD_STATE *field = Ram_HeapSlots->fieldState;
	u8 *grid = MapShapeGrid;

	field->movedBlock = NULL;
	for(; list->actor != ACTOR_LIST_END; list++) {
		s32 change = 0;
		ACTOR *actor = fdActorGetById(list->actor);
		u8 animation = ((SPR *)actor->spr)->lastNo;
		u16 flag = list->flg;

		if(syGameFlagGet(flag)) {
			if(animation != PUDDLE_ANIMATION_FROZEN)
				change = 1;
		}
		else {
			if(animation != PUDDLE_ANIMATION_PUDDLE)
				change = 1;
		}
		if(change) {
			MAP_CELL *cells = ((MAP_STATE *)Ram_HeapSlots->mapGrid)
								  ->layers[actor->layer]
								  .cells;
			u8 *shapes;
			s32 slot;
			u32 kind;
			s32 height;

			shapes = &grid[((s32)cells - (s32)Ram_MapCellBuffer) >> 2];
			slot = fdActorGetCellIndex(actor);
			kind = fdMapGridGetTerrainKindStub(actor->layer, actor->x.fixed,
											   actor->z.fixed);
			height = CELL_HEIGHT_STEPS(fdMapGridGetTerrainHeightStub(
				actor->layer, actor->x.fixed, actor->z.fixed));

			field->movedBlock = actor;
			if(syGameFlagGet(flag)) {
				height -= CELL_HEIGHT_STEPS(toFIXED(32));
				syGameFlagClear(flag);
			}
			else {
				height += CELL_HEIGHT_STEPS(toFIXED(32));
				syGameFlagSet(flag);
			}
			if((actor->x.fixed | actor->z.fixed) != 0) {

				cells += slot;
				shapes += slot;
				*shapes = fdMapShapeFindOrAdd(actor->layer, kind, height);
				if(syGameFlagGet(flag)) {
					s32 bits;

					cells->collisionCode = CELL_BEHAVIOR_NORMAL;
					bits = cells->attributeB | MAP_CELL_JUMP;
					cells->attributeB = bits;
				}
				else {
					cells->collisionCode = CELL_BEHAVIOR_WALL;
					cells->attributeB &= ~MAP_CELL_JUMP;
				}
			}
		}
	}
}

/* Returns the index of the actor's cell in its map layer. */
s32 fdActorGetCellIndex(ACTOR *actor) {
	return actor->x.fixed / toFIXED(MAPGRID_CELL_PIXELS) +
		   (actor->z.fixed / toFIXED(MAPGRID_CELL_PIXELS)) * MAPGRID_SIZE;
}

/* Returns the index of the map's shape entry (MapGridShapes, MAP_SHAPE) of */
/* shape whose first height is height, in 8-pixel steps, writing it into the */
/* first free entry (MAP_SHAPE_FREE) when there is none: the entry a raised */
/* or lowered cell's shape grid byte takes. The layer goes unread. */
s32 fdMapShapeFindOrAdd(s32 layer, s32 shape, s32 height) {
	u8 *entry = MapGridShapes;
	s8 entryHeight;
	s32 i = 0;

	entryHeight = height;
	while(1) {
		if(entry[0] == shape) {
			if(*(s8 *)++entry == entryHeight)
				return i;
			entry--;
		}
		else if(entry[0] == MAP_SHAPE_FREE) {
			entry[0] = shape;
			entry[1] = entryHeight;
			return i;
		}
		entry += 4;
		i++;
	}
}
