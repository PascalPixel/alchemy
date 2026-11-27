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
Sint32 fdPartyGetCharacterSpr(Sint32 character)
{
	Sint32 spr;

	spr = character;
	if(spr <= PLACEMENT_SHIP) {
		if(spr == PLACEMENT_SHIP) {
			spr = SPRITE_WINGED_SHIP_SMALL;
			if(MainRoutineStateWork.mapId != SCENE_CLEAR)
				spr = SPRITE_WINGED_SHIP;
			if(syGameFlagGet(FLAG_SHIP_FLIES))
				spr++;
		}
	}
	return spr;
}

#include "field/actor_reserved_no_operation.c"

/* Creates the actors of a scene's actor placement table (ended by sprite -1),
   and records the table among the scene's tables. */
void fdScriptSpawnActorTable(ACTOR_PLACE *entry, Sint32 slot)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Sint32		 actorIdx = 0;
	Sint32		 i;
	Uint16		 placementSpr;

	/* Remember the table once, before consuming slots for its actor
	   placements. */
	for(i = 0; i < 4; i++) {
		if(state->tbls[i] == entry)
			break;
		if(state->tbls[i] == NULL) {
			state->tbls[i] = entry;
			break;
		}
	}
	for(placementSpr = entry->spr;
		(Sint16)placementSpr != -1 && slot <= FIELD_TREASURE_ACTOR - 1;
		entry++, placementSpr = entry->spr) {
		ACTOR *actor;
		SPR	  *spr;
		Sint32 condition;
		Sint32 sprId;

		if(entry->spr <= PLACEMENT_PARTY_COUNT - 1)
			actorIdx = entry->spr;
		else if(entry->spr <= PLACEMENT_SPRITE_MAX)
			actorIdx = slot++;
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
		sprId = fdPartyGetCharacterSpr(entry->spr);
		actor = fdActorLookup(actorIdx);
		if(actor == NULL) {
			/* ACTOR_PLACEMENT_AT_SCENE_POINT: x is the number of a scene point
			   to stand on. */
			if(entry->flg & ACTOR_PLACEMENT_AT_SCENE_POINT) {
				Sint32 pos[3];

				pos[0] = (Sint32)actor;
				pos[1] = (Sint32)actor;
				pos[2] = (Sint32)actor;
				fdScenePointGetPos(entry->x, pos);
				actor = fdActorCreateStub(sprId, pos[0], 0, pos[2]);
			} else {
				actor = fdActorCreateStub(sprId, entry->x, entry->y, entry->z);
			}
			/* ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE: share the previous actor's
			   sprite resource. */
			if(entry->flg & ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE) {
				ACTOR *prev = fdActorLookup(actorIdx - 1);

				if(prev->sprStorageKind == ACTOR_SPRITE_ONE &&
					actor->sprStorageKind == ACTOR_SPRITE_ONE) {
					Uint8 sprSlot;

					spr = prev->spr;
					spr->retainCacheEntry = 1;
					sprSlot = spr->slot;
					spr = actor->spr;
					spr->retainCacheEntry = 1;
					syVRAMCacheFreeEntry(spr->slot);
					spr->slot = sprSlot;
				}
			}
		} else {
			if(entry->spr == MainRoutineStateWork.leaderActorId)
				continue;
			fdActorSetPosAndResetMotionStub(
				actor, entry->x, entry->y, entry->z);
		}
		if(actor != NULL) {
			spr = actor->spr;
			fdActorSelectAnimationStub(actor, ANIM_STAND);
			/* Each layer starts its first picture 0 to 4 sixteenths of a tick
			   apart, so actors placed together do not move in step. */
			if(actor->sprStorageKind == ACTOR_SPRITE_ONE && spr != NULL) {
				for(i = 0; i < spr->cnt; i++) {
					SPR_ANIMATION *layer = spr->entries[i];

					if(layer != NULL)
						layer->tmr = (syRandom() * 5) >> 15;
				}
				spr->dirty = 1;
			}
			actor->facing = entry->facing;
			actor->collisionFlg = ACTOR_SOLID;
			fdActorSetMovementType(actor, entry->behavior);
			actor->homeX = actor->x.fixed / toFIXED(1);
			actor->homeZ = actor->z.fixed / toFIXED(1);
			/* An actor placement with a height is lifted half a pixel more and
			   floats: it neither follows the ground nor falls. */
			if(actor->y.fixed != 0) {
				actor->motionFlg = ACTOR_FLOATS;
				actor->y.fixed += toFIXED(1) / 2;
			}
			if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
				actor->motionFlg &= ~ACTOR_FOLLOWS_TERRAIN;
				if(!syGameFlagGet(FLAG_ISAAC_WEARS_TUNIC))
					spr->scale = syMulFX(spr->scale, toFIXED(3) / 4);
			} else {
				actor->groundHeight = fdMapGridGetTerrainHeightStub(
					0, actor->x.fixed, actor->z.fixed);
				actor->y.fixed += actor->groundHeight;
			}
			actor->priFlg = ACTOR_PRIORITY_AUTOMATIC;
		}
		state->actors[actorIdx] = actor;
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

/* Releases the actor in slot actorIdx of the field state's table, if there is
   one, and empties the slot. */
void fdActorReleaseAtIndex(Uint32 actorIdx)
{
	FIELD_STATE *state = FieldStateWork;
	ACTOR		*actor = fdActorLookup(actorIdx);

	if(actor != NULL) {
		fdActorReleaseStub(actor);
		state->actors[actorIdx] = NULL;
	}
}

Uint8 *fdSprAddAnimationStub(void *resource, Sint32 id);

/* Creates a scene's actors on entering it, as The Broken Seal's
   fdActorResetForLeader does: the party leader where the main routine's state
   puts it, the scene's placement table from actor slot 8, and the camera that
   follows the leader; it also chooses the leader's control mode. */
void fdActorResetForLeader(Sint32 tbl)
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
		state->tbls[i] = 0;
	fdActorTableClear();
	entry->condition = -1;
	entry->spr = leader;
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
	fdScriptSpawnActorTable((ACTOR_PLACE *)tbl, ACTOR_FIRST_PLACED);

	actor = state->actors[leader];
	actor->layer = MainRoutineStateWork.layer;
	actor->tmr = 0;
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
			actor->motionFlg = 0;
			fdActorSetSprFlagsStub(actor, 0);
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
		SPR_ANIMATION *secondLayer = (SPR_ANIMATION *)fdSprAddAnimationStub(
			actor->spr, SPRITE_WORLD_MAP_FOREST_COVER);

		secondLayer->colMode = SPRITE_COLOR_HIDDEN;
		secondLayer->pri = SPRITE_LAYER_NOT_DRAWN;
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
	while(entry->spr != -1) {
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
	Uint8 *anms = (Uint8 *)&SnapshotObjects[ACTOR_SNAPSHOT_CAPACITY];
	Uint8 *sprFlg = anms + ACTOR_SNAPSHOT_CAPACITY;
	Uint8 *priorities = anms + 2 * ACTOR_SNAPSHOT_CAPACITY;
	Uint32 cnt = 0;
	ACTOR *actor;
	SPR	  *spr;
	Sint32 i;
	Uint32 anm;
	Uint32 flg;
	Uint32 pri;
	Sint32 limit = FIELD_ACTOR_COUNT;

	for(i = 0; i < limit; i++) {
		actor = fdActorLookup(i);
		if(actor == NULL)
			continue;
		*indices++ = i;
		syDMAXCopy(actor, copy, DMA_COPY_WORDS(sizeof(ACTOR) / 4), REG_DMA3);
		if(actor->sprStorageKind == ACTOR_SPRITE_ONE) {
			spr = actor->spr;
			anm = spr->lastNo;
			flg = spr->flg;
			pri = spr->image.pri;
		} else {
			anm = 0;
			flg = 0;
			pri = 0;
		}
		*anms++ = anm;
		*sprFlg++ = flg;
		*priorities++ = pri;
		cnt++;
		copy++;
		if(cnt > ACTOR_SNAPSHOT_CAPACITY - 1)
			break;
	}
	for(i = cnt; i < ACTOR_SNAPSHOT_CAPACITY; i++)
		*indices++ = ACTOR_SNAPSHOT_EMPTY;
}

/* Puts back the actors the snapshot saved, and moves the camera to the party
   leader's height. */
void fdActorRestore(void)
{
	ACTOR  *copy = SnapshotObjects;
	Uint8  *indices = (Uint8 *)SnapshotObjects - ACTOR_SNAPSHOT_CAPACITY;
	Uint8  *anms = (Uint8 *)&SnapshotObjects[ACTOR_SNAPSHOT_CAPACITY];
	Uint8  *sprFlg = anms + ACTOR_SNAPSHOT_CAPACITY;
	Uint8  *priorities = anms + 2 * ACTOR_SNAPSHOT_CAPACITY;
	Sint32	cnt;
	ACTOR  *actor;
	SPR	   *spr;
	Sint32 *view;
	ACTOR  *camera;
	Sint32	idx;
	Sint32	pri;
	FIXED	y;

	cnt = 0;
	idx = *indices++;
	while(idx != ACTOR_SNAPSHOT_EMPTY) {
		actor = fdActorLookup(idx);
		if(actor != NULL) {
			spr = actor->spr;
			syDMAXCopy(
				copy, actor, DMA_COPY_WORDS(sizeof(ACTOR) / 4), REG_DMA3);
			actor->spr = spr;
			fdActorSelectAnimationStub(actor, *anms);
			fdActorSetSprFlagsStub(actor, *sprFlg);
			pri = *priorities;
			spr->image.pri = pri;
			spr->shadow.pri = pri;
			if(idx == fdPartyGetLeaderActor()) {
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
		anms++;
		sprFlg++;
		cnt++;
		priorities++;
		if(cnt > ACTOR_SNAPSHOT_CAPACITY - 1)
			break;
		idx = *indices++;
	}
}

#undef PLACEMENT_PARTY_COUNT
#undef PLACEMENT_SPRITE_MAX
#undef PLACEMENT_SHIP
#undef LOOKUP_ID_COUNT
