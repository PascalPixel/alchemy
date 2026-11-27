/* The party's sprites, placing a scene's actors, and the actor table. Part of
   field_mode.c. */

enum {
	/* DJINN_PER_ELEMENT flags per element: the Djinn flags are 48 to 127, their
	   hiding flags the next 80. */
	DJINN_FLAG_COUNT = ELEMENT_COUNT * DJINN_PER_ELEMENT
};

/* A placement row's sprite: below PLACEMENT_PARTY_COUNT a party member, who
   keeps that actor id; one up to PLACEMENT_SPRITE_MAX takes the next free slot,
   and one above it, which no table names, takes the slot of the row before. */
#define PLACEMENT_PARTY_COUNT 8
#define PLACEMENT_SPRITE_MAX  9989
/* A placement's character after the party: the ship. */
#define PLACEMENT_SHIP		  8

/* The ids fdActorLookup answers: more than the FIELD_ACTOR_COUNT slots. */
#define LOOKUP_ID_COUNT 192

/* Picks the sprite a scene placement's character id shows: the ship
   (PLACEMENT_SHIP) is SPRITE_WINGED_SHIP_SMALL on the world map and
   SPRITE_WINGED_SHIP elsewhere, each one's flying picture once FLAG_SHIP_FLIES
   is set. */
Sint32 fdPartyGetCharacterSprite(Sint32 character)
{
	Sint32 sprite;

	sprite = character;
	if(sprite <= PLACEMENT_SHIP) {
		if(sprite == PLACEMENT_SHIP) {
			sprite = SPRITE_WINGED_SHIP_SMALL;
			if(MainRoutineStateWork.mapId != SCENE_CLEAR)
				sprite = SPRITE_WINGED_SHIP;
			if(syGameFlagGet(FLAG_SHIP_FLIES))
				sprite++;
		}
	}
	return sprite;
}

/* Does nothing; nothing calls fdActorReservedNoOperationStub. */
UNUSED void fdActorReservedNoOperation(void)
{
}

/* Creates the actors of a scene's actor placement table (ended by sprite -1),
   and records the table among the scene's tables. */
void fdScriptSpawnActorTable(ACTOR_PLACE *entry, Sint32 slot)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Sint32		 actorIndex = 0;
	Sint32		 i;
	Uint16		 placementSprite;

	/* Remember the table once, before consuming slots for its actor
	   placements. */
	for(i = 0; i < 4; i++) {
		if(state->tables[i] == entry)
			break;
		if(state->tables[i] == NULL) {
			state->tables[i] = entry;
			break;
		}
	}
	for(placementSprite = entry->sprite;
		(Sint16)placementSprite != -1 && slot <= FIELD_TREASURE_ACTOR - 1;
		entry++, placementSprite = entry->sprite) {
		ACTOR *actor;
		SPR	  *sprite;
		Sint32 condition;
		Sint32 spriteId;

		if(entry->sprite <= PLACEMENT_PARTY_COUNT - 1)
			actorIndex = entry->sprite;
		else if(entry->sprite <= PLACEMENT_SPRITE_MAX)
			actorIndex = slot++;
		condition = entry->condition;
		if(!fdGameFlagIsConditionActive(condition))
			continue;
		if((Uint32)(condition - FLAG_DJINN_FIRST) <= DJINN_FLAG_COUNT - 1) {
			Sint32 element = (condition - FLAG_DJINN_FIRST) / DJINN_PER_ELEMENT;

			/* The last flag of an element stands for the Djinni the scene chose
			   for it, numbered from 1; with none chosen the actor is left
			   out. */
			if((condition - FLAG_DJINN_FIRST) % DJINN_PER_ELEMENT ==
				DJINN_PER_ELEMENT - 1) {
				Sint32 member =
					SceneLocalState[element + SCENE_STATE_CHOSEN_DJINN];

				if(member == 0)
					continue;
				condition =
					element * DJINN_PER_ELEMENT + member + FLAG_DJINN_FIRST - 1;
				if(!fdGameFlagIsConditionActive(condition))
					continue;
			}
			/* fdDjinnResolvePending hides a Djinni after the party loses or
			   flees. */
			if(state->mode != ACTOR_SYSTEM_WORLD_MAP &&
				!fdGameFlagIsConditionActive(
					condition + FLAG_DJINN_HIDING_FIRST - FLAG_DJINN_FIRST))
				continue;
		}
		spriteId = fdPartyGetCharacterSprite(entry->sprite);
		actor = fdActorLookup(actorIndex);
		if(actor == NULL) {
			/* ACTOR_PLACEMENT_AT_SCENE_POINT: x is the number of a scene point
			   to stand on. */
			if(entry->flags & ACTOR_PLACEMENT_AT_SCENE_POINT) {
				Sint32 pos[3];

				pos[0] = (Sint32)actor;
				pos[1] = (Sint32)actor;
				pos[2] = (Sint32)actor;
				fdScenePointGetPos(entry->x, pos);
				actor = fdActorCreateStub(spriteId, pos[0], 0, pos[2]);
			} else {
				actor =
					fdActorCreateStub(spriteId, entry->x, entry->y, entry->z);
			}
			/* ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE: share the previous actor's
			   sprite resource. */
			if(entry->flags & ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE) {
				ACTOR *prev = fdActorLookup(actorIndex - 1);

				if(prev->spriteStorageKind == ACTOR_SPRITE_ONE &&
					actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
					Uint8 spriteSlot;

					sprite = prev->sprite;
					sprite->retainCacheEntry = 1;
					spriteSlot = sprite->slot;
					sprite = actor->sprite;
					sprite->retainCacheEntry = 1;
					syVRAMCacheFreeEntry(sprite->slot);
					sprite->slot = spriteSlot;
				}
			}
		} else {
			if(entry->sprite == MainRoutineStateWork.leaderActorId)
				continue;
			fdActorSetPosAndResetMotionStub(
				actor, entry->x, entry->y, entry->z);
		}
		if(actor != NULL) {
			sprite = actor->sprite;
			fdActorSelectAnimationStub(actor, ANIM_STAND);
			/* Each layer starts its first picture 0 to 4 sixteenths of a tick
			   apart, so actors placed together do not move in step. */
			if(actor->spriteStorageKind == ACTOR_SPRITE_ONE && sprite != NULL) {
				for(i = 0; i < sprite->count; i++) {
					SPR_ANIMATION *layer = sprite->entries[i];

					if(layer != NULL)
						layer->timer = (syRandom() * 5) >> 15;
				}
				sprite->dirty = 1;
			}
			actor->facing = entry->facing;
			actor->collisionFlags = ACTOR_SOLID;
			fdActorSetMovementType(actor, entry->behavior);
			actor->homeX = actor->x.fixed / toFIXED(1);
			actor->homeZ = actor->z.fixed / toFIXED(1);
			/* An actor placement with a height is lifted half a pixel more and
			   floats: it neither follows the ground nor falls. */
			if(actor->y.fixed != 0) {
				actor->motionFlags = ACTOR_FLOATS;
				actor->y.fixed += toFIXED(1) / 2;
			}
			if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
				actor->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
				if(!syGameFlagGet(FLAG_ISAAC_WEARS_TUNIC))
					sprite->scale = syMulFX(sprite->scale, toFIXED(3) / 4);
			} else {
				actor->groundHeight = fdMapGridGetTerrainHeightStub(
					0, actor->x.fixed, actor->z.fixed);
				actor->y.fixed += actor->groundHeight;
			}
			actor->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
		}
		state->actors[actorIndex] = actor;
	}
}

/* Looks point id up in the map's scene points (its scene_points.tsv, ended by
   id 0xff) and writes its x and z, in 16.16 pixels, to position; returns 0, or
   -1 when the map has no such point. */
Sint32 fdScenePointGetPos(Sint32 id, FIXED *pos)
{
	Uint8 *cursor = RamMapScenePoints;
	Sint8  key;

	key = *cursor++;
	while(key != -1) {
		if(key == id) {
			pos[0] = toFIXED(fdStreamReadU16(&cursor));
			pos[2] = toFIXED(fdStreamReadU16(&cursor));
			return 0;
		}
		cursor += 4;
		key = *cursor++;
	}
	return -1;
}

/* Releases the actor in slot actorIndex of the field state's table, if there is
   one, and empties the slot. */
void fdActorReleaseAtIndex(Uint32 actorIndex)
{
	FIELD_STATE *state = FieldStateWork;
	ACTOR		*actor = fdActorLookup(actorIndex);

	if(actor != NULL) {
		fdActorReleaseStub(actor);
		state->actors[actorIndex] = NULL;
	}
}

Uint8 *fdSpriteAddAnimationStub(void *resource, Sint32 id);

/* Creates a scene's actors on entering it, as The Broken Seal's
   fdActorResetForLeader does: the party leader where the main routine's state
   puts it, the scene's placement table from actor slot 8, and the camera that
   follows the leader; it also chooses the leader's control mode. */
void fdActorResetForLeader(Sint32 table)
{
	FIELD_STATE	 *state;
	ACTOR_PLACE	 *entry;
	ACTOR		 *actor;
	ACTOR		 *camera;
	MAP_METATILE *metatile;
	MAP_METATILE *above;
	Sint32		  leader;
	Sint32		  i;
	Sint32		  height;

	state = FieldStateWork;
	leader = MainRoutineStateWork.leaderActorId;
	entry = state->leaderPlacements;
	entry[0] = FieldDefaultPlacements[0];
	entry[1] = FieldDefaultPlacements[1];
	for(i = 0; i < 4; i++)
		state->tables[i] = 0;
	fdActorTableClear();
	entry->condition = -1;
	entry->sprite = leader;
	SET_POSITION(entry, MainRoutineStateWork.x, 0, MainRoutineStateWork.z);
	entry->facing = MainRoutineStateWork.heading;
	fdScriptSpawnActorTable(entry, leader);
	{
		FIXED y = MainRoutineStateWork.y;

		if(y != 0) {
			actor = state->actors[leader];
			actor->y.fixed = y;
			actor->groundHeight = y;
		}
	}
	fdScriptSpawnActorTable((ACTOR_PLACE *)table, ACTOR_FIRST_PLACED);

	actor = state->actors[leader];
	actor->layer = MainRoutineStateWork.layer;
	actor->timer = 0;
	actor->userValue = 0;
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		if(MainRoutineStateWork.ownerOverride != 0)
			MainRoutineStateWork.movementMode = LEADER_MOVEMENT_SHIP;
		else
			MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WORLD_MAP;
	} else if(MainRoutineStateWork.ownerOverride != 0) {
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_SHIP_ON_MAP;
	} else {
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
		metatile = ((MAP_STATE *)RamHeapSlots->mapGrid)
					   ->layers[actor->layer]
					   .metatiles +
			((actor->x.fixed / toFIXED(MAPGRID_METATILE_PIXELS)) +
				((actor->z.fixed / toFIXED(MAPGRID_METATILE_PIXELS)) *
					MAPGRID_SIZE));
		above = metatile - MAPGRID_SIZE;
		if(MainRoutineStateWork.y != 0 &&
			(metatile->attributeB & MAP_METATILE_CLIMBABLE) &&
			(above->attributeB & MAP_METATILE_CLIMBABLE)) {
			MainRoutineStateWork.movementMode = LEADER_MOVEMENT_KEYS;
			height = fdMapGridGetTerrainHeightStub(
						 0, actor->x.fixed, actor->z.fixed - toFIXED(16)) -
				toFIXED(32);
			actor->y.fixed += height;
			actor->groundHeight = actor->y.fixed;
			actor->motionFlags = 0;
			fdActorSetSpriteFlagsStub(actor, 0);
			fdActorSelectAnimationStub(actor, ANIM_LEADER_CLING);
		}
	}

	if(MainRoutineStateWork.ownerOverride != 0) {
		ACTOR *stand = state->actors[ACTOR_FIRST_PLACED];

		SET_ACTOR_POSITION(
			stand, actor->x.fixed, actor->y.fixed, actor->z.fixed);
		stand->groundHeight = actor->groundHeight;
		stand->facing = actor->facing;
		actor->x.fixed = 0;
		actor->z.fixed = 0;
		actor = stand;
	}

	camera = fdActorCreateStub(ACTOR_CREATE_KIND(ACTOR_CREATE_NO_SPRITE),
		actor->x.fixed, actor->y.fixed, actor->z.fixed);
	camera->groundHeight = actor->groundHeight;
	fdActorStartCameraStub(camera, actor);
	/* On the World Map the leader takes a second layer, kept undrawn until
	   fdActorMoveOnWorldMap draws it in a forest. */
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		SPR_ANIMATION *secondLayer = (SPR_ANIMATION *)fdSpriteAddAnimationStub(
			actor->sprite, SPRITE_WORLD_MAP_FOREST_COVER);

		secondLayer->colorMode = SPRITE_COLOR_HIDDEN;
		secondLayer->priority = SPRITE_LAYER_NOT_DRAWN;
	}
	((MAP_STATE *)RamHeapSlots->mapGrid)->origin = &camera->x.fixed;
	state->camera = camera;
}

/* Returns the slot after the last placed actor (8 when there is none), or -1
   when the last slot is occupied. */
Sint32 fdActorFindLastActiveId(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Sint32		 last = ACTOR_FIRST_PLACED - 1;
	Sint32		 i;

	for(i = ACTOR_FIRST_PLACED; i <= FIELD_TREASURE_ACTOR - 1; i++) {
		if(state->actors[i] != NULL)
			last = i;
	}
	last++;
	if(last == FIELD_TREASURE_ACTOR)
		last = -1;
	return last;
}

#define FLAG_IN_VIEW	 0x164 /* kept clear, so a placement using it appears */
/* kept set, so a placement using it does not */
#define FLAG_OUT_OF_VIEW 0x165

/* Gives each actor placement with condition 0 a condition saying whether it
   lies within the map's view bounds, so that only those inside appear. */
void fdAssignViewFlags(VIEWED_PLACEMENT *entry)
{
	MAP_STATE *view = RamHeapSlots->mapGrid;

	syGameFlagClear(FLAG_IN_VIEW);
	syGameFlagSet(FLAG_OUT_OF_VIEW);
	while(entry->sprite != -1) {
		Sint32 x;
		Sint32 z;

		/* A placement with a flag of its own keeps it. */
		if(entry->condition != 0) {
			entry++;
			continue;
		}
		x = entry->x;
		z = entry->z;
		if(view->minX <= x && x <= view->maxX && view->minZ <= z &&
			z <= view->maxZ)
			entry->condition = FLAG_IN_VIEW;
		else
			entry->condition = FLAG_OUT_OF_VIEW;
		entry++;
	}
}

/* Empties all the actor slots, without releasing the actors. */
void fdActorTableClear(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Sint32		 i;

	for(i = 0; i < FIELD_ACTOR_COUNT; i++)
		state->actors[i] = NULL;
}

/* Returns the actor in slot actorId (below LOOKUP_ID_COUNT), or NULL. */
ACTOR *fdActorLookup(Uint32 actorId)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;

	if(actorId > LOOKUP_ID_COUNT - 1)
		return NULL;
	/* UB: ids from FIELD_ACTOR_COUNT to LOOKUP_ID_COUNT - 1 read past actors[],
	   into the field state's requests; UBFIX returns NULL for them. */
#if FIX_ACTOR_LOOKUP_PAST_TABLE
	if(actorId > FIELD_ACTOR_COUNT - 1)
		return NULL;
#endif
	return state->actors[actorId];
}

/* Saves up to 32 actors into SnapshotObjects, with each one's slot, animation,
   sprite flags and priority, so that they can be put back. */
void fdActorTableSnapshot(void)
{
	ACTOR *copy = SnapshotObjects;
	Uint8 *indices = (Uint8 *)SnapshotObjects - ACTOR_SNAPSHOT_CAPACITY;
	Uint8 *animations = (Uint8 *)&SnapshotObjects[ACTOR_SNAPSHOT_CAPACITY];
	Uint8 *spriteFlags = animations + ACTOR_SNAPSHOT_CAPACITY;
	Uint8 *priorities = animations + 2 * ACTOR_SNAPSHOT_CAPACITY;
	Uint32 count = 0;
	ACTOR *actor;
	SPR	  *sprite;
	Sint32 i;
	Uint32 animation;
	Uint32 flags;
	Uint32 priority;
	Sint32 limit = FIELD_ACTOR_COUNT;

	for(i = 0; i < limit; i++) {
		actor = fdActorLookup(i);
		if(actor == NULL)
			continue;
		*indices++ = i;
		syDMAXCopy(actor, copy, DMA_COPY_WORDS(sizeof(ACTOR) / 4), REG_DMA3);
		if(actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
			sprite = actor->sprite;
			animation = sprite->lastNo;
			flags = sprite->flags;
			priority = sprite->image.priority;
		} else {
			animation = 0;
			flags = 0;
			priority = 0;
		}
		*animations++ = animation;
		*spriteFlags++ = flags;
		*priorities++ = priority;
		count++;
		copy++;
		if(count > ACTOR_SNAPSHOT_CAPACITY - 1)
			break;
	}
	for(i = count; i < ACTOR_SNAPSHOT_CAPACITY; i++)
		*indices++ = ACTOR_SNAPSHOT_EMPTY;
}

/* Puts back the actors the snapshot saved, and moves the camera to the party
   leader's height. */
void fdActorRestore(void)
{
	ACTOR  *copy = SnapshotObjects;
	Uint8  *indices = (Uint8 *)SnapshotObjects - ACTOR_SNAPSHOT_CAPACITY;
	Uint8  *animations = (Uint8 *)&SnapshotObjects[ACTOR_SNAPSHOT_CAPACITY];
	Uint8  *spriteFlags = animations + ACTOR_SNAPSHOT_CAPACITY;
	Uint8  *priorities = animations + 2 * ACTOR_SNAPSHOT_CAPACITY;
	Sint32	count;
	ACTOR  *actor;
	SPR	   *sprite;
	Sint32 *view;
	ACTOR  *camera;
	Sint32	index;
	Sint32	priority;
	FIXED	y;

	count = 0;
	index = *indices++;
	while(index != ACTOR_SNAPSHOT_EMPTY) {
		actor = fdActorLookup(index);
		if(actor != NULL) {
			sprite = actor->sprite;
			syDMAXCopy(
				copy, actor, DMA_COPY_WORDS(sizeof(ACTOR) / 4), REG_DMA3);
			actor->sprite = sprite;
			fdActorSelectAnimationStub(actor, *animations);
			fdActorSetSpriteFlagsStub(actor, *spriteFlags);
			priority = *priorities;
			sprite->image.priority = priority;
			sprite->shadow.priority = priority;
			if(index == fdPartyGetLeaderActor()) {
				camera = FieldStateWork->camera;
				view = ((MAP_STATE *)RamHeapSlots->mapGrid)->origin;
				y = actor->y.fixed;
				camera->x.fixed = actor->x.fixed;
				camera->groundHeight = y;
				camera->y.fixed = y;
				camera->z.fixed = actor->z.fixed;
				view[1] = y;
				fdActorStartIdleStub(actor);
			}
		}
		copy++;
		animations++;
		spriteFlags++;
		count++;
		priorities++;
		if(count > ACTOR_SNAPSHOT_CAPACITY - 1)
			break;
		index = *indices++;
	}
}

#undef PLACEMENT_PARTY_COUNT
#undef PLACEMENT_SPRITE_MAX
#undef PLACEMENT_SHIP
#undef LOOKUP_ID_COUNT
