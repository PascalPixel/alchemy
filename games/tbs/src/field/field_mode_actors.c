/* The party's sprites, placing a scene's actors, and the actor slots. Part of
   field_mode.c. */

#include "field_mode_internal.h"

enum {
	/* 20 flags per element: the Djinn's caught flags are 0x30 to 0x7f. */
	ACTOR_DJINNI_FLAG_COUNT = ELEMENT_COUNT * 20
};

/* A placement row's sprite: 0 to 7 is a party member, who keeps that actor id;
   one up to this takes the next free slot, and one above it, which no table
   names, takes the slot of the row before. */
#define PLACEMENT_SPRITE_MAX 9989

/* The ids fdActorLookup answers: more than the ACTOR_ID_COUNT slots, so one
   past them reads the field state's members after the slots. */
#define LOOKUP_ID_COUNT 192

/* The last sprite the tests below take as a party member's: one past PIERS,
   though only Isaac's and Garet's sprites are ever swapped. */
#define PARTY_SPRITE_LAST (PIERS + 1)

/* Returns the sprite of character, which is the character itself except that
   FLAG_CHILDHOOD gives Isaac and Garet SPRITE_ISAAC_CHILD and
   SPRITE_GARET_CHILD, and FLAG_ISAAC_WEARS_TUNIC gives grown Isaac sprite
   SPRITE_ISAAC_TUNIC. */
Sint32 fdPartyGetCharacterSpr(Sint32 character)
{
	Sint32 sprite;

	sprite = character;
	if(sprite <= PARTY_SPRITE_LAST) {
		if(gmFlagGetStub(FLAG_CHILDHOOD) != 0) {
			if(sprite == ISAAC) {
				sprite = SPRITE_ISAAC_CHILD;
			}
			if(sprite == GARET) {
				sprite = SPRITE_GARET_CHILD;
			}
		} else if((gmFlagGetStub(FLAG_ISAAC_WEARS_TUNIC) != 0) &&
			(sprite == ISAAC)) {
			sprite = SPRITE_ISAAC_TUNIC;
		}
	}
	return sprite;
}

/* Returns the sprite of character as fdPartyGetCharacterSpr would with
   FLAG_CHILDHOOD set to childhood (as recorded in a save file's summary). */
Sint32 fdPartyGetCharacterSprForChildhood(Sint32 character, Sint32 childhood)
{
	Sint32 sprite;

	sprite = character;
	if((sprite <= PARTY_SPRITE_LAST) && (childhood != 0)) {
		if(sprite == ISAAC) {
			sprite = SPRITE_ISAAC_CHILD;
		}
		if(sprite == GARET) {
			sprite = SPRITE_GARET_CHILD;
		}
	}
	return sprite;
}

/* Creates the actors of a scene's actor placement table (ended by sprite -1),
   and records the table among the scene's tables. */
void fdScriptSpawnActorPlacements(ACTOR_PLACE *entry, Sint32 slot)
{
	FIELD_STATE *state;
	ACTOR		*actor;
	ACTOR		*previous;
	SPR			*sprite;
	Sint32		 tableIndex;
	Sint32		 actorIndex;
	Sint32		 spriteId;
	Sint32		 condition;
	Uint8		 spriteSlot;
	Uint16		 placementSprite;

	state = FieldStateWork;
	/* Remember the table once, before consuming slots for its actor
	   placements. */
	for(tableIndex = 0; tableIndex < (Sint32)ARRAY_COUNT(state->tbls);
		tableIndex++) {
		if(state->tbls[tableIndex] == entry)
			break;
		if(state->tbls[tableIndex] == 0) {
			state->tbls[tableIndex] = entry;
			break;
		}
	}
	for(placementSprite = entry->spr;
		(Sint16)placementSprite != -1 && slot <= ACTOR_ID_COUNT - 1;
		entry++, placementSprite = entry->spr) {
		/* Current actor placement tables use sprites 0 to 504, so every live
		   row sets actorIndex. */
		if(entry->spr <= 7)
			actorIndex = entry->spr;
		else if(entry->spr <= PLACEMENT_SPRITE_MAX)
			actorIndex = slot++;
		condition = entry->condition;
		if(!fdGameFlagIsConditionActive(condition))
			continue;
		/* fdDjinnResolvePending hides a Djinni after the party loses or flees.
		   gmArrive clears these temporary flags on a normal entry. */
		if(condition >= FLAG_DJINN_FIRST &&
			condition < FLAG_DJINN_FIRST + ACTOR_DJINNI_FLAG_COUNT &&
			state->mode != ACTOR_SYSTEM_WORLD_MAP &&
			!fdGameFlagIsConditionActive(condition + ACTOR_DJINNI_FLAG_COUNT))
			continue;
		spriteId = fdPartyGetCharacterSpr(entry->spr);
		actor = fdActorLookup(actorIndex);
		if(actor == 0) {
			actor = fdActorCreateStub(spriteId, entry->x, entry->y, entry->z);
			/* ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE: share the previous actor's
			   sprite resource. */
			if(entry->flg & ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE) {
				previous = fdActorLookup(actorIndex - 1);
				if(previous->sprStorageKind == ACTOR_SPRITE_ONE &&
					actor->sprStorageKind == ACTOR_SPRITE_ONE) {
					sprite = previous->spr;
					sprite->retainCacheEntry = 1;
					spriteSlot = sprite->slot;
					sprite = actor->spr;
					sprite->retainCacheEntry = 1;
					syVRAMCacheFreeEntry(sprite->slot);
					sprite->slot = spriteSlot;
				}
			}
			/* Dora's tunic overlays the young sprites with its child part. */
			if(gmFlagGetStub(FLAG_ISAAC_WEARS_TUNIC) &&
				(Uint32)(spriteId - SPRITE_ISAAC_CHILD) <=
					SPRITE_GARET_CHILD - SPRITE_ISAAC_CHILD)
				fdActorRegisterSprMetadataStub(actor, SPRITE_TUNIC);
		} else if(!gmFlagGetStub(FLAG_KEEP_PARTY_POSITION)) {
			fdActorSetPosAndResetMotionStub(
				actor, entry->x, entry->y, entry->z);
		}

		if(actor) {
			fdActorSelectAnimationStub(actor, ANIM_STAND);
			/* lastNo is the animation last selected, which selecting again
			   skips: a random 0 to 29 makes the second selection below restart
			   ANIM_STAND from its first frame, unless it happens to be
			   ANIM_STAND. */
			if(actor->sprStorageKind == ACTOR_SPRITE_ONE &&
				(sprite = actor->spr) != 0)
				sprite->lastNo = ((Uint32)(syRandom()) % (Uint32)30);
			actor->facing = entry->facing;
			actor->collisionFlg = ACTOR_SOLID;
			fdActorSetMovementType(actor, entry->movement);
			fdActorSelectAnimationStub(actor, ANIM_STAND);
			actor->homeX = actor->x.fixed / toFIXED(1);
			actor->homeZ = actor->z.fixed / toFIXED(1);
			/* An actor placement with a height is lifted half a pixel more and
			   floats (ACTOR_FLOATS): it neither follows the ground nor
			   falls. */
			if(actor->y.fixed != 0) {
				actor->motionFlg = ACTOR_FLOATS;
				actor->y.fixed += toFIXED(1) / 2;
			}
			if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
				/* World Map actor placements use single sprites and do not
				   follow the ground. */
				actor->motionFlg &= ~ACTOR_FOLLOWS_TERRAIN;
				if(!gmFlagGetStub(FLAG_ISAAC_WEARS_TUNIC)) {
					/* UB: sprite creation can fail while the actor slot stays
					   live. */
#if FIX_NULL_DEREFERENCE
					if(actor->sprStorageKind == ACTOR_SPRITE_ONE &&
						(sprite = actor->spr) != NULL)
#endif
						sprite->scale = syMulFX(sprite->scale, toFIXED(3) / 4);
				}
			} else {
				actor->groundHeight = fdMapGridGetTerrainHeightStub(
					0, actor->x.fixed, actor->z.fixed);
				actor->y.fixed += actor->groundHeight;
			}
			actor->priFlg = ACTOR_PRIORITY_AUTOMATIC;
		}
		state->actors[actorIndex] = actor;
	}
}

/* Releases the actor in slot actorIndex, if there is one, and empties the
   slot. */
void fdActorReleaseAtIndex(Sint32 actorIndex)
{
	FIELD_STATE *state = FieldStateWork;
	void		*object = fdActorLookup(actorIndex);

	if(object != NULL) {
		fdActorReleaseStub(object);
#if FIX_POINTER_OUTSIDE_OBJECT
		{
			void *empty = NULL;

			/* fdActorLookup also accepts ids past the actor slots, within the
			   field state. */
			memcpy((Uint8 *)state + offsetof(FIELD_STATE, actors) +
					(Uint32)actorIndex * sizeof(empty),
				&empty, sizeof(empty));
		}
#else
		state->actors[actorIndex] = NULL;
#endif
	}
}

/* Creates a scene's actors on entering it: the party leader where the main
   routine's state puts it, then the scene's actor placement table from actor
   slot 8, and the camera that follows the leader. */
void fdActorResetForLeader(ACTOR_PLACE *table)
{
	FIELD_STATE		  *state;
	ACTOR_PLACE		  *entry;
	ACTOR			  *actor;
	ACTOR			  *camera;
	MAP_GRID_METATILE *metatile;
	MAP_GRID_METATILE *above;
	Sint32			   leader;
	Sint32			   pos;
	FIXED			   height;

	state = FieldStateWork;
	leader = MainRoutineStateWork.leaderActorId;
	/* Build the leader's temporary actor placement before the scene's placed
	   actors. */
	entry = (ACTOR_PLACE *)((Uint8 *)state +
		offsetof(FIELD_STATE, leaderPlacements));
	entry[0] = Field_DefaultActorPlacements[0];
	entry[1] = Field_DefaultActorPlacements[1];
	for(pos = 0; pos < (Sint32)ARRAY_COUNT(state->tbls); pos++)
		state->tbls[pos] = 0;
	fdActorClearAll();
	entry->condition = -1;
	entry->spr = leader;
	SET_POSITION(entry, MainRoutineStateWork.x, 0, MainRoutineStateWork.z);
	entry->facing = MainRoutineStateWork.heading;
	fdScriptSpawnActorPlacements(entry, leader);
	fdScriptSpawnActorPlacements(table, ACTOR_FIRST_PLACED);

	actor = state->actors[leader];
	/* MainRoutineStateWork.layer keeps the leader's map grid layer (fdModeRun
	   saves its layer there). */
	actor->layer = MainRoutineStateWork.layer;
	pos = actor->x.fixed / toFIXED(MAPGRID_METATILE_PIXELS) +
		(actor->z.fixed / toFIXED(MAPGRID_METATILE_PIXELS)) * MAPGRID_SIZE;
	metatile = &MAPGRID_METATILES[pos];
	above = &MAPGRID_METATILES[pos - MAPGRID_SIZE];
	if(MainRoutineStateWork.y != 0 &&
		metatile->behavior == METATILE_BEHAVIOR_CLIMB &&
		above->behavior == METATILE_BEHAVIOR_CLIMB) {
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_CLIMB;
		height = fdMapGridGetTerrainHeightStub(
					 0, actor->x.fixed, actor->z.fixed - toFIXED(16)) -
			toFIXED(32);
		actor->y.fixed += height;
		actor->groundHeight = actor->y.fixed;
		actor->motionFlg = 0;
		fdActorSetSprFlagsStub(actor, 0);
		fdActorSelectAnimationStub(actor, ANIM_LEADER_CLING);
	} else {
		MainRoutineStateWork.movementMode = LEADER_MOVEMENT_WALK;
	}

	/* The camera follows this leader and starts at its resolved ground height;
	   the map scrolls by the camera's position. It takes no sprite. */
	camera = fdActorCreateStub(ACTOR_CREATE_KIND(ACTOR_CREATE_NO_SPRITE),
		actor->x.fixed, actor->y.fixed, actor->z.fixed);
	camera->groundHeight = actor->groundHeight;
	fdActorStartCameraStub(camera, (Sint32)actor);
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		SPR_ANIMATION *meta = (SPR_ANIMATION *)fdSprAddAnimationStub(
			actor->spr, SPRITE_WORLD_MAP_FOREST_COVER);
		meta->colMode = SPRITE_COLOR_HIDDEN;
		meta->pri = 9;
	}
	((MAP_GRID_SCROLL_STATE *)MapGridState[0])->origin = &camera->x.fixed;
	state->camera = camera;
}

/* Returns the slot after the last placed actor (8 when there is none), or -1
   when the last slot is occupied. */
Sint32 fdActorFindLastActiveId(void)
{
	FIELD_STATE *state = FieldStateWork;
	Sint32		 lastActorIndex = ACTOR_FIRST_PLACED - 1;
	Sint32		 actorIndex = ACTOR_FIRST_PLACED;
	ACTOR	   **slot = &state->actors[ACTOR_FIRST_PLACED];

	for(; actorIndex < ACTOR_ID_COUNT; actorIndex++) {
		ACTOR *actor = *slot++;

		if(actor != 0) {
			lastActorIndex = actorIndex;
		}
	}
	lastActorIndex++;
	if(lastActorIndex == ACTOR_ID_COUNT) {
		lastActorIndex = -1;
	}
	return lastActorIndex;
}

/* Unused: returns the address of actor slot index in the field state. */
UNUSED void *fdActorGetSlotAddress(Uint32 actorIndex)
{
	return (Uint8 *)FieldStateWork + offsetof(FIELD_STATE, actors) +
		actorIndex * sizeof(void *);
}

/* Actors and the view: actor placement conditions from the map's view bounds,
   and clearing placed actors. */

#define FLAG_IN_VIEW	 0x164 /* kept clear, so a placement using it appears */
/* kept set, so a placement using it does not */
#define FLAG_OUT_OF_VIEW 0x165

#define PLACED_MAX \
	(ACTOR_ID_COUNT - ACTOR_FIRST_PLACED) /* placed actors' slots */

/* Gives each actor placement with condition 0 a condition saying whether it
   lies within the map's view bounds, so that only those inside appear. */
void fdAssignViewFlags(ACTOR_PLACE *entry)
{
	MAP_GRID_SCROLL_STATE *view = MapGridState[0];

	gmFlagClearStub(FLAG_IN_VIEW);
	gmFlagSetStub(FLAG_OUT_OF_VIEW);
	while(entry->spr != -1) {
		Sint32 x;
		Sint32 z;

		/* A placement with a condition of its own keeps it. */
		if(entry->condition != 0) {
			entry++;
			continue;
		}
		x = entry->x;
		z = entry->z;
		if(view->minX <= x && x <= view->maxX && view->minY <= z &&
			z <= view->maxY)
			entry->condition = FLAG_IN_VIEW;
		else
			entry->condition = FLAG_OUT_OF_VIEW;
		entry++;
	}
}

/* The byte of SPR after its slot, whose bit 0 is retainCacheEntry. */
#define SPRITE_CACHE_FLAGS (offsetof(SPR, slot) + 1)

/* Releases the placed actors far from the view's centre: more than 160 pixels
   to either side, 200 pixels north or 100 pixels south. Actors at (0, 0)
   stay. */
void fdActorClearOutOfBounds(void)
{
	FIELD_STATE *state = FieldStateWork;
	ACTOR		*camera;
	FIXED		 centerX;
	FIXED		 centerZ;
	FIXED		 left;
	FIXED		 right;
	FIXED		 top;
	FIXED		 bottom;
	ACTOR	   **actorSlot;
	Sint32		 releaseCacheMask;
	Sint32		 i;

	camera = state->camera;
	centerX = camera->x.fixed;
	left = centerX - toFIXED(160);
	right = centerX + toFIXED(160);
	centerZ = camera->z.fixed;
	top = centerZ - toFIXED(200);
	bottom = centerZ + toFIXED(100);
	actorSlot = &state->actors[ACTOR_FIRST_PLACED];
	/* Every bit of SPRITE_CACHE_FLAGS but retainCacheEntry. */
	releaseCacheMask = ~1;

	for(i = 0; i < PLACED_MAX; i++) {
		ACTOR *actor = *actorSlot;

		if(actor != NULL) {
			FIXED actorX = actor->x.fixed;
			FIXED actorZ = actor->z.fixed;

			if(actorX != 0 || actorZ != 0) {
				if(actorX < left || actorX > right || actorZ < top ||
					actorZ > bottom) {
					Uint8 *sprite;
					Uint8 *removed = &actor->sprStorageKind;
					Uint32 flags;

					/* ACTOR_SPRITE_ONE makes the release free the actor's one
					   sprite, and clearing retainCacheEntry frees its VRAM
					   cache entry with it, even one shared with a neighbour
					   (ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE). */
					*removed = ACTOR_SPRITE_ONE;
					sprite = (Uint8 *)actor->spr;
					flags = sprite[SPRITE_CACHE_FLAGS];
					sprite[SPRITE_CACHE_FLAGS] = flags & releaseCacheMask;
					fdActorReleaseStub(actor);
					sprite = NULL;
					*actorSlot = (ACTOR *)sprite;
				}
			}
		}

		actorSlot++;
	}
}

/* Releases every placed actor and places the scene's actors again from its
   first actor placement table after the leader's. */
void fdActorRespawn(void)
{
	FIELD_STATE *state = FieldStateWork;
	ACTOR_PLACE *placements;
	Sint32		 slotIndex;

	for(slotIndex = 0; slotIndex < PLACED_MAX; slotIndex++) {
		ACTOR *actor = state->actors[ACTOR_FIRST_PLACED + slotIndex];

		if(actor != 0) {
			/* As in fdActorClearOutOfBounds: free the sprite and its VRAM cache
			   entry with the actor. */
			actor->sprStorageKind = ACTOR_SPRITE_ONE;
			actor->spr->retainCacheEntry = 0;
			fdActorReleaseStub(actor);
			state->actors[ACTOR_FIRST_PLACED + slotIndex] = 0;
		}
	}
	placements = state->tbls[1];
	state->tbls[1] = 0;
	state->tbls[2] = 0;
	state->tbls[3] = 0;
	if(placements != 0)
		fdScriptSpawnActorPlacements(placements, fdActorFindLastActiveId());
}

/* Empties all the actor slots, without releasing the actors. */
void fdActorClearAll(void)
{
	FIELD_STATE *state;
	ACTOR		*emptyActor;
	Sint32		 actorIndex;

	emptyActor = NULL;
	state = FieldStateWork;
	for(actorIndex = 0; actorIndex < ACTOR_ID_COUNT; actorIndex++) {
		state->actors[actorIndex] = emptyActor;
	}
}

/* Returns the actor in slot actorIndex (below LOOKUP_ID_COUNT), or NULL. */
void *fdActorLookup(Uint32 actorIndex)
{
	Uint8 *stateBytes = (Uint8 *)FieldStateWork;
	Uint32 slotOffset;
#if FIX_POINTER_OUTSIDE_OBJECT
	void *object;
#endif
	if(actorIndex > LOOKUP_ID_COUNT - 1)
		return 0;
	slotOffset = (actorIndex * 4) + offsetof(FIELD_STATE, actors);
#if FIX_POINTER_OUTSIDE_OBJECT
	/* The legacy range includes non-pointer fields. Copy their representation
	   without reading them through an incompatible pointer lvalue. */
	memcpy(&object, stateBytes + slotOffset, sizeof(object));
	return object;
#else
	return *(void **)(stateBytes + slotOffset);
#endif
}

/* Saves up to 32 actors (only the party's 8 on the World Map) into
   ActorSnapshotWork, with each one's slot, animation, sprite flags and
   priority, so that fdActorRestore can put them back. */
void fdActorSnapshot(void)
{
	FIELD_STATE *state;
	ACTOR		*snapshot = ActorSnapshotWork.actors;
	Uint8		*animations = ActorSnapshotWork.anms;
	Uint8		*spriteFlags = ActorSnapshotWork.sprFlg;
	Uint8		*priorities = ActorSnapshotWork.priorities;
	Uint8		*indices = ActorSnapshotWork.slots;
	Uint32		 savedCount = 0;
	Sint32		 actorLimit = sizeof(state->actors) / sizeof(state->actors[0]);
	ACTOR		*actor;
	Sint32		 actorIndex;
	Uint32		 animation;
	Uint32		 spriteFlagBits;
	Uint32		 priority;

	state = FieldStateWork;
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP)
		actorLimit = ACTOR_FIRST_PLACED;
	/* Keep each actor and its three sprite-state bytes at matching save
	   indices. */
	for(actorIndex = 0; actorIndex < actorLimit; actorIndex++) {
		actor = fdActorLookup(actorIndex);
		if(actor == NULL)
			continue;
		*indices++ = actorIndex;
		syDMAXCopy(
			actor, snapshot, DMA_COPY_WORDS(sizeof(ACTOR) / 4), REG_DMA3);
		if(actor->sprStorageKind == ACTOR_SPRITE_ONE) {
			SPR *sprite = actor->spr;
			animation = sprite->lastNo;
			spriteFlagBits = sprite->flg;
			priority = sprite->image.pri;
		} else {
			animation = 0;
			spriteFlagBits = 0;
			priority = 0;
		}
		*animations++ = animation;
		*spriteFlags++ = spriteFlagBits;
		*priorities++ = priority;
		savedCount++;
		snapshot++;
		if(savedCount > ACTOR_SNAPSHOT_CAPACITY - 1)
			break;
	}
	for(actorIndex = savedCount; actorIndex < ACTOR_SNAPSHOT_CAPACITY;
		actorIndex++)
		*indices++ = ACTOR_SNAPSHOT_EMPTY;
}

/* Puts back the actors fdActorSnapshot saved, and moves the camera to the party
   leader's height. */
void fdActorRestore(void)
{
	ACTOR				   *snapshot = ActorSnapshotWork.actors;
	Uint8				   *indices = ActorSnapshotWork.slots;
	Uint8				   *animations = ActorSnapshotWork.anms;
	Uint8				   *spriteFlags = ActorSnapshotWork.sprFlg;
	Uint8				   *priorities = ActorSnapshotWork.priorities;
	Sint32					restoredCount;
	ACTOR				   *actor;
	SPR					   *sprite;
	ACTOR				   *camera;
	Sint32				   *cameraPosition;
	Sint32					actorIndex;
	Sint32					animation;
	Sint32					priority;
	FIXED					y;
	FIELD_STATE			  **state = &FieldStateWork;
	MAP_GRID_SCROLL_STATE **grid = (MAP_GRID_SCROLL_STATE **)MapGridState;

	restoredCount = 0;
	actorIndex = *indices++;
	while(actorIndex != ACTOR_SNAPSHOT_EMPTY) {
		actor = fdActorLookup(actorIndex);
		if(actor != NULL) {
			/* The actor keeps the sprite it has now, not the one saved with
			   it. */
			sprite = actor->spr;
			syDMAXCopy(
				snapshot, actor, DMA_COPY_WORDS(sizeof(ACTOR) / 4), REG_DMA3);
			animation = *animations;
			if(animation != 0)
				fdActorSelectAnimationStub(actor, animation);
			fdActorSetSprFlagsStub(actor, *spriteFlags);
			priority = *priorities;
			sprite->image.pri = priority;
			sprite->shadow.pri = priority;
			*(SPR **)&actor->spr = sprite;
			/* Re-anchor the camera only when restoring the selected leader. */
			if(actorIndex == MainRoutineStateWork.leaderActorId) {
				camera = (*state)->camera;
				cameraPosition = (*grid)->origin;
				y = actor->y.fixed;
				camera->groundHeight = y;
				camera->y.fixed = y;
				cameraPosition[1] = y;
				fdActorStartIdleStub(actor);
			}
		}
		snapshot++;
		animations++;
		spriteFlags++;
		priorities++;
		restoredCount++;
		if(restoredCount > ACTOR_SNAPSHOT_CAPACITY - 1)
			break;
		actorIndex = *indices++;
	}
}

#undef FLAG_IN_VIEW
#undef FLAG_OUT_OF_VIEW
#undef PLACED_MAX
#undef PLACEMENT_SPRITE_MAX
#undef LOOKUP_ID_COUNT
#undef PARTY_SPRITE_LAST
#undef SPRITE_CACHE_FLAGS
