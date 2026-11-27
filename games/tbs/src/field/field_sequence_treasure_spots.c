/* @file */
/* The map's treasure spots: chests and Psynergy Stones. */
/* Part of field_sequence.c. */

#include "field_sequence_internal.h"

/* Sets an actor's y and groundHeight to the map's height at its x and z. */
void fdActorSnapToTerrain(void *object) {
	ACTOR *actor = object;
	s32 groundHeight;

	groundHeight =
		fdMapGridGetTerrainHeightStub(0, actor->x.fixed, actor->z.fixed);
	actor->y.fixed = groundHeight;
	actor->groundHeight = groundHeight;
}

/* The sprites of the actors a map's treasure spots place. */
enum {
	TREASURE_SPOT_CHEST_SPRITE = SPRITE_CHEST,
	TREASURE_SPOT_PSYNERGY_STONE_SPRITE = SPRITE_PSYNERGY_STONE
};

/* The chest sprite's animations. */
enum {
	TREASURE_SPOT_CHEST_OPEN = 2, /* standing open */
	TREASURE_SPOT_CHEST_OPENING = 4,
	TREASURE_SPOT_CHEST_CLOSING =
		5,						  /* closing again on what nobody could carry */
	TREASURE_SPOT_CHEST_MIMIC = 7 /* jumping up as a mimic */
};

/* Places an actor at each of the map's treasure spots (ids 100 to 239) that */
/* the scene has an event for, up to ten: a chest for a chest event, shown open
 */
/* once its flag is set and gone once its mimic was beaten, and a Psynergy */
/* Stone for an examine trigger giving one, until it is taken. */
void fdPlaceTreasureSpots(void) {
	u32 id;
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	FIELD_STATE *state = FieldStateWork;
	s32 markerCount = 0;
	u8 *list = grid->markers;
	TREASURE_SPOT *marker = state->treasureSpots;
	const TRIGGER *trigger;
	ACTOR *markerActor;
	u32 column;
	u32 row;

	btDmaFill32(0, marker, sizeof(TREASURE_SPOT) * TREASURE_SPOT_MAX);
	/* A map without a treasure spot list places nothing. */
	if(list == NULL)
		return;

	column = *list++;
	row = *list++;
	while(column != TREASURE_SPOT_LIST_END || row != TREASURE_SPOT_LIST_END) {
		id = *list++;
		if(id >= TREASURE_SPOT_FIRST_ID && id <= TREASURE_SPOT_LAST_ID) {
			/* The first event with this trigger that places something wins; the
			 */
			/* event's value says what the search gives (its FIND_ type, as */
			/* fdExamineCell reads it). */
			trigger = OverlayArea.triggers();
			for(; trigger->kind != -1; trigger++) {
				if(trigger->subject != id)
					continue;
				if((trigger->kind & EXAMINE_TRIGGER_MASK) ==
				   EXAMINE_TRIGGER(EXAMINE_CHEST)) {
					/* A chest, at the centre of its cell. */
					markerActor = fdActorCreateStub(TREASURE_SPOT_CHEST_SPRITE,
													FIELD_CELL_CENTER(column),
													0, FIELD_CELL_CENTER(row));
					if(markerActor == NULL)
						continue;
					fdActorSnapToTerrain(markerActor);
					fdActorSetSprFlagsStub(markerActor, 0);
					/* An opened chest shows open; an opened mimic's chest is
					 * not placed */
					/* at all. */
					if(gmFlagGetStub(trigger->condition)) {
						if((trigger->value & FIND_TYPE_MASK) == FIND_MIMIC) {
							fdActorReleaseStub(markerActor);
							continue;
						}
						fdActorSelectAnimationStub(markerActor,
												   TREASURE_SPOT_CHEST_OPEN);
					}
					fdActorStartIdleStub(markerActor);
					markerActor->homeX = markerActor->x.fixed / FIXED_ONE;
					markerActor->homeZ = markerActor->z.fixed / FIXED_ONE;
					markerActor->priFlg = ACTOR_PRIORITY_AUTOMATIC;
					markerActor->collisionFlg = ACTOR_SOLID;
					marker->id = trigger->subject;
					marker->actor = markerActor;
					marker->column =
						markerActor->x.fixed / toFIXED(MAPGRID_CELL_PIXELS);
					marker->row =
						markerActor->z.fixed / toFIXED(MAPGRID_CELL_PIXELS);
					marker++;
					markerCount++;
					if(markerCount >= TREASURE_SPOT_MAX)
						return;
				}
				else if((trigger->kind & EXAMINE_TRIGGER_MASK) ==
						TRIGGER_EXAMINE) {
					/* A Psynergy Stone not yet taken. */
					if((trigger->value & FIND_TYPE_MASK) != FIND_PSYNERGY_STONE)
						continue;
					if(gmFlagGetStub(trigger->condition))
						continue;
					markerActor = fdActorCreateStub(
						TREASURE_SPOT_PSYNERGY_STONE_SPRITE,
						FIELD_CELL_CENTER(column), 0, FIELD_CELL_CENTER(row));
					if(markerActor == NULL)
						continue;
					fdActorSnapToTerrain(markerActor);
					fdActorSetSprFlagsStub(markerActor, 0);
					fdActorStartIdleStub(markerActor);
					fdActorSelectAnimationStub(markerActor, 1);
					markerActor->homeX = markerActor->x.fixed / FIXED_ONE;
					markerActor->homeZ = markerActor->z.fixed / FIXED_ONE;
					markerActor->collisionFlg = ACTOR_SOLID;
					markerActor->priFlg = ACTOR_PRIORITY_AUTOMATIC;
					marker->actor = markerActor;
					marker->id = trigger->subject;
					marker->column =
						markerActor->x.fixed / toFIXED(MAPGRID_CELL_PIXELS);
					marker->row =
						markerActor->z.fixed / toFIXED(MAPGRID_CELL_PIXELS);
					marker++;
					markerCount++;
					if(markerCount >= TREASURE_SPOT_MAX)
						return;
				}
				else {
					continue;
				}
				break;
			}
		}
		column = *list++;
		row = *list++;
	}
}

/* Returns the treasure spot slot (below TREASURE_SPOT_MAX) whose event trigger
 */
/* is id, or -1. */
s32 fdTreasureSpotFindSlot(s32 id) {
	s32 slotIndex;
	s32 matchingSlot;
	TREASURE_SPOT *marker;

	marker = FieldStateWork->treasureSpots;
	matchingSlot = -1;
	for(slotIndex = 0; slotIndex <= TREASURE_SPOT_MAX - 1;
		slotIndex++, marker++) {
		if(marker->id == id) {
			matchingSlot = slotIndex;
			break;
		}
	}
	return matchingSlot;
}

/* Unused: empty. */
UNUSED void fdTreasureSpotReservedNoOp(void) {}

/* Gives the actor of treasure spot id animation, plays sound and waits 12 */
/* frames. */
static __inline__ void SetMarkerAnimationAndPlaySound(s32 id, s32 animation,
													  s32 sound) {
	s32 slotIndex;
	TREASURE_SPOT *marker;
	ACTOR *actor;

	slotIndex = fdTreasureSpotFindSlot(id);
	if(slotIndex != -1) {
		marker = &FieldStateWork->treasureSpots[slotIndex];
		actor = marker->actor;
		if(actor != NULL)
			fdActorSelectAnimationStub(actor, animation);
		sySndPlayStub(sound);
		sySynch(12);
	}
}

/* Closes the chest of treasure spot id again (its item could not be carried),
 */
/* with SE_CHEST_CLOSE. */
void fdTreasureSpotCloseChest(s32 id) {
	SetMarkerAnimationAndPlaySound(id, TREASURE_SPOT_CHEST_CLOSING,
								   SE_CHEST_CLOSE);
}

/* Makes the chest of treasure spot id jump up as a mimic: after 18 frames it */
/* turns into the mimic with a cry (SE_ENEMY_CRY) and leaps up at 8 pixels a */
/* frame, its shadow showing. */
void fdTreasureSpotLaunchMimic(s32 id) {
	s32 slotIndex;
	TREASURE_SPOT *marker;
	ACTOR *actor;

	slotIndex = fdTreasureSpotFindSlot(id);
	if(slotIndex != -1) {
		marker = &FieldStateWork->treasureSpots[slotIndex];
		actor = marker->actor;
		sySynch(18);
		if(actor != NULL)
			fdActorSelectAnimationStub(actor, TREASURE_SPOT_CHEST_MIMIC);
		sySndPlayStub(SE_ENEMY_CRY);
		if(actor != NULL) {
			actor->velY = toFIXED(8);
			fdActorSetSprFlagsStub(actor, SPRITE_FLAG_SHADOW);
		}
	}
}

/* Opens the chest of treasure spot id with SE_CHEST_OPEN, and waits 12 frames.
 */
void fdTreasureSpotOpenChest(s32 id) {
	SetMarkerAnimationAndPlaySound(id, TREASURE_SPOT_CHEST_OPENING,
								   SE_CHEST_OPEN);
}

/* Shows the chest of treasure spot id open, with no sound. */
void fdTreasureSpotShowOpened(s32 id) {
	s32 slotIndex;
	TREASURE_SPOT *marker;
	ACTOR *actor;

	slotIndex = fdTreasureSpotFindSlot(id);
	if(slotIndex != -1) {
		marker = &FieldStateWork->treasureSpots[slotIndex];
		actor = marker->actor;
		if(actor != NULL)
			fdActorSelectAnimationStub(actor, TREASURE_SPOT_CHEST_OPEN);
	}
}

/* Returns the actor of treasure spot id, or NULL when no slot has it. */
ACTOR *fdTreasureSpotGetActor(s32 id) {
	s32 slotIndex = fdTreasureSpotFindSlot(id);
	TREASURE_SPOT *marker;

	if(slotIndex == -1)
		return NULL;
	marker = &FieldStateWork->treasureSpots[slotIndex];
	return marker->actor;
}

/* Hides the actor of treasure spot id; the actor stays in its slot. */
void fdTreasureSpotHide(s32 id) {
	s32 slotIndex;
	TREASURE_SPOT *marker;
	ACTOR *actor;

	slotIndex = fdTreasureSpotFindSlot(id);
	if(slotIndex != -1) {
		marker = &FieldStateWork->treasureSpots[slotIndex];
		actor = marker->actor;
		if(actor != NULL)
			actor->sprStorageKind = ACTOR_SPRITE_NONE;
	}
}

/* Moves the actor of treasure spot id to (x, z) on the ground; -1 keeps the */
/* centre of its cell on that axis. Positions are 16.16. */
void fdTreasureSpotSetActorPos(s32 id, FIXED x, FIXED z) {
	s32 slotIndex;
	s32 groundHeight;
	s32 spotX;
	s32 spotZ;
	TREASURE_SPOT *marker;
	ACTOR *actor;

	spotZ = z;
	spotX = x;
	slotIndex = fdTreasureSpotFindSlot(id);
	if(slotIndex != -1) {
		marker = &FieldStateWork->treasureSpots[slotIndex];
		actor = marker->actor;
		if(actor != NULL) {
			if(spotX == -1)
				spotX = FIELD_CELL_CENTER(marker->column);
			if(spotZ == -1)
				spotZ = FIELD_CELL_CENTER(marker->row);
			actor->x.fixed = spotX;
			actor->z.fixed = spotZ;
			groundHeight = fdMapGridGetTerrainHeightStub(0, spotX, spotZ);
			actor->groundHeight = groundHeight;
			actor->y.fixed = groundHeight;
		}
	}
}
