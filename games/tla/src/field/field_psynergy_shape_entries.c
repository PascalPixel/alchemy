/* The map's shape entries and the flag blocks: the puddles Frost freezes into
   pillars of ice, each with the flag that says it is frozen. Part of
   field_psynergy.c. */

/* Sets up Alhafra's and the Alhafran Cave's puddles as fdFlagBlockPlaceAll
   does, and marks the open metatiles on each puddle's four sides a gap to jump
   across (fdMapGridMarkJumpIfOpen), so the leader jumps onto a frozen one. */
void fdFlagBlockPlaceAllMarkingEdges(ACTOR_FLAG *list)
{
	MAP_STATE  *map = RamHeapSlots->mapGrid;
	ACTOR_FLAG *first = list;
	ACTOR	   *leader;

	for(; list->actor != ACTOR_LIST_END; list++) {
		ACTOR		 *actor = fdActorGetById(list->actor);
		Uint16		  flg = list->flg;
		Sint32		  slot = fdActorGetMetatileIndex(actor);
		MAP_METATILE *metatile = &map->layers[actor->layer].metatiles[slot];

		actor->motionFlg = 0;
		actor->collisionFlg &= ~ACTOR_SOLID;
		fdActorSetSprFlagsStub(actor, 0);
		metatile->collisionCode = METATILE_BEHAVIOR_WALL;
		fdMapGridMarkJumpIfOpen(
			actor->x.fixed + toFIXED(MAPGRID_METATILE_PIXELS), actor->z.fixed,
			actor->layer, actor->groundHeight);
		fdMapGridMarkJumpIfOpen(
			actor->x.fixed - toFIXED(MAPGRID_METATILE_PIXELS), actor->z.fixed,
			actor->layer, actor->groundHeight);
		fdMapGridMarkJumpIfOpen(actor->x.fixed,
			actor->z.fixed + toFIXED(MAPGRID_METATILE_PIXELS), actor->layer,
			actor->groundHeight);
		fdMapGridMarkJumpIfOpen(actor->x.fixed,
			actor->z.fixed - toFIXED(MAPGRID_METATILE_PIXELS), actor->layer,
			actor->groundHeight);
		if(syGameFlagGet(flg)) {
			fdActorSelectAnimationStub(actor, PUDDLE_ANIMATION_FROZEN);
			syGameFlagClear(flg);
		} else {
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

/* For each flag block whose animation disagrees with its flag, turns its
   metatile and flag over: with the flag set and the ice pillar not showing, the
   metatile is lowered 32 pixels to a wall and the flag cleared; with the flag
   clear and the puddle not showing (Frost freezing it), the metatile is raised
   32 pixels, open and a gap to jump across, and the flag set. The field state
   remembers the block. */
void fdFlagBlockApplyFlags(ACTOR_FLAG *list)
{
	FIELD_STATE *field = RamHeapSlots->fieldState;
	Uint8		*grid = MapShapeGrid;

	field->movedBlock = NULL;
	for(; list->actor != ACTOR_LIST_END; list++) {
		Sint32 change = 0;
		ACTOR *actor = fdActorGetById(list->actor);
		Uint8  anm = ((SPR *)actor->spr)->lastNo;
		Uint16 flg = list->flg;

		if(syGameFlagGet(flg)) {
			if(anm != PUDDLE_ANIMATION_FROZEN)
				change = 1;
		} else {
			if(anm != PUDDLE_ANIMATION_PUDDLE)
				change = 1;
		}
		if(change) {
			MAP_METATILE *metatiles = ((MAP_STATE *)RamHeapSlots->mapGrid)
										  ->layers[actor->layer]
										  .metatiles;
			Uint8 *shapes;
			Sint32 slot;
			Uint32 kind;
			Sint32 height;

			shapes =
				&grid[((Sint32)metatiles - (Sint32)RamMapMetatileBuffer) >> 2];
			slot = fdActorGetMetatileIndex(actor);
			kind = fdMapGridGetTerrainKindStub(
				actor->layer, actor->x.fixed, actor->z.fixed);
			height = METATILE_HEIGHT_STEPS(fdMapGridGetTerrainHeightStub(
				actor->layer, actor->x.fixed, actor->z.fixed));

			field->movedBlock = actor;
			if(syGameFlagGet(flg)) {
				height -= METATILE_HEIGHT_STEPS(toFIXED(32));
				syGameFlagClear(flg);
			} else {
				height += METATILE_HEIGHT_STEPS(toFIXED(32));
				syGameFlagSet(flg);
			}
			if((actor->x.fixed | actor->z.fixed) != 0) {

				metatiles += slot;
				shapes += slot;
				*shapes = fdMapShapeFindOrAdd(actor->layer, kind, height);
				if(syGameFlagGet(flg)) {
					Sint32 bits;

					metatiles->collisionCode = METATILE_BEHAVIOR_NORMAL;
					bits = metatiles->attributeB | MAP_METATILE_JUMP;
					metatiles->attributeB = bits;
				} else {
					metatiles->collisionCode = METATILE_BEHAVIOR_WALL;
					metatiles->attributeB &= ~MAP_METATILE_JUMP;
				}
			}
		}
	}
}

/* Returns the index of the actor's metatile in its map layer. */
Sint32 fdActorGetMetatileIndex(ACTOR *actor)
{
	return actor->x.fixed / toFIXED(MAPGRID_METATILE_PIXELS) +
		(actor->z.fixed / toFIXED(MAPGRID_METATILE_PIXELS)) * MAPGRID_SIZE;
}

/* Returns the index of the map's shape entry (MapGridShapes, MAP_SHAPE) of
   shape whose first height is height, in 8-pixel steps, writing it into the
   first free entry (MAP_SHAPE_FREE) when there is none: the entry a raised or
   lowered metatile's shape grid byte takes. The layer goes unread. */
Sint32 fdMapShapeFindOrAdd(Sint32 layer, Sint32 shape, Sint32 height)
{
	Uint8 *entry = MapGridShapes;
	Sint8  entryHeight;
	Sint32 i = 0;

	entryHeight = height;
	while(1) {
		if(entry[0] == shape) {
			if(*(Sint8 *)++entry == entryHeight)
				return i;
			entry--;
		} else if(entry[0] == MAP_SHAPE_FREE) {
			entry[0] = shape;
			entry[1] = entryHeight;
			return i;
		}
		entry += 4;
		i++;
	}
}
