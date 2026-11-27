/* The map's treasure spots: chests and Psynergy Stones. Part of
   field_mode.c. */

#include "field_mode_internal.h"

/* Sets an actor's y and groundHeight to the map's height at its x and z. */
void fdActorSnapToTerrain(void *object)
{
	ACTOR *actor = object;
	Sint32 groundHeight;

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
	/* closing again on what nobody could carry */
	TREASURE_SPOT_CHEST_CLOSING = 5,
	TREASURE_SPOT_CHEST_MIMIC = 7 /* jumping up as a mimic */
};

/* Places an actor at each of the map's treasure spots (ids 100 to 239) that the
   scene has a script for, up to ten: a chest for a chest script, shown open
   once its flag is set and gone once its mimic was beaten, and a Psynergy Stone
   for an examine cue giving one, until it is taken. */
void fdPlaceTreasureSpots(void)
{
	Uint32				   id;
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	FIELD_STATE			  *state = FieldStateWork;
	Sint32				   markerCnt = 0;
	Uint8				  *list = grid->markers;
	TREASURE_SPOT		  *marker = state->treasureSpots;
	const CUE			  *cue;
	ACTOR				  *markerActor;
	Uint32				   column;
	Uint32				   row;

	DMA_FILL32(0, marker, sizeof(TREASURE_SPOT) * TREASURE_SPOT_MAX);
	/* A map without a treasure spot list places nothing. */
	if(list == NULL)
		return;

	column = *list++;
	row = *list++;
	while(column != TREASURE_SPOT_LIST_END || row != TREASURE_SPOT_LIST_END) {
		id = *list++;
		if(id >= TREASURE_SPOT_FIRST_ID && id <= TREASURE_SPOT_LAST_ID) {
			/* The first script with this cue that places something wins; the
			   script's value says what the search gives (its FIND_ type, as
			   fdExamineMetatile reads it). */
			cue = OverlayArea.cues();
			for(; cue->kind != -1; cue++) {
				if(cue->subject != id)
					continue;
				if((cue->kind & EXAMINE_CUE_MASK) ==
					EXAMINE_CUE(EXAMINE_CHEST)) {
					/* A chest, at the centre of its metatile. */
					markerActor = fdActorCreateStub(TREASURE_SPOT_CHEST_SPRITE,
						FIELD_METATILE_CENTER(column), 0,
						FIELD_METATILE_CENTER(row));
					if(markerActor == NULL)
						continue;
					fdActorSnapToTerrain(markerActor);
					fdActorSetSprFlagsStub(markerActor, 0);
					/* An opened chest shows open; an opened mimic's chest is
					   not placed at all. */
					if(gmFlagGetStub(cue->condition)) {
						if((cue->value & FIND_TYPE_MASK) == FIND_MIMIC) {
							fdActorReleaseStub(markerActor);
							continue;
						}
						fdActorSelectAnimationStub(
							markerActor, TREASURE_SPOT_CHEST_OPEN);
					}
					fdActorStartIdleStub(markerActor);
					markerActor->homeX = markerActor->x.fixed / toFIXED(1);
					markerActor->homeZ = markerActor->z.fixed / toFIXED(1);
					markerActor->priFlg = ACTOR_PRIORITY_AUTOMATIC;
					markerActor->collisionFlg = ACTOR_SOLID;
					marker->id = cue->subject;
					marker->actor = markerActor;
					marker->column =
						markerActor->x.fixed / toFIXED(MAPGRID_METATILE_PIXELS);
					marker->row =
						markerActor->z.fixed / toFIXED(MAPGRID_METATILE_PIXELS);
					marker++;
					markerCnt++;
					if(markerCnt >= TREASURE_SPOT_MAX)
						return;
				} else if((cue->kind & EXAMINE_CUE_MASK) == CUE_EXAMINE) {
					/* A Psynergy Stone not yet taken. */
					if((cue->value & FIND_TYPE_MASK) != FIND_PSYNERGY_STONE)
						continue;
					if(gmFlagGetStub(cue->condition))
						continue;
					markerActor =
						fdActorCreateStub(TREASURE_SPOT_PSYNERGY_STONE_SPRITE,
							FIELD_METATILE_CENTER(column), 0,
							FIELD_METATILE_CENTER(row));
					if(markerActor == NULL)
						continue;
					fdActorSnapToTerrain(markerActor);
					fdActorSetSprFlagsStub(markerActor, 0);
					fdActorStartIdleStub(markerActor);
					fdActorSelectAnimationStub(markerActor, 1);
					markerActor->homeX = markerActor->x.fixed / toFIXED(1);
					markerActor->homeZ = markerActor->z.fixed / toFIXED(1);
					markerActor->collisionFlg = ACTOR_SOLID;
					markerActor->priFlg = ACTOR_PRIORITY_AUTOMATIC;
					marker->actor = markerActor;
					marker->id = cue->subject;
					marker->column =
						markerActor->x.fixed / toFIXED(MAPGRID_METATILE_PIXELS);
					marker->row =
						markerActor->z.fixed / toFIXED(MAPGRID_METATILE_PIXELS);
					marker++;
					markerCnt++;
					if(markerCnt >= TREASURE_SPOT_MAX)
						return;
				} else {
					continue;
				}
				break;
			}
		}
		column = *list++;
		row = *list++;
	}
}

/* Returns the treasure spot slot (below TREASURE_SPOT_MAX) whose script cue is
   id, or -1. */
Sint32 fdTreasureSpotFindSlot(Sint32 id)
{
	Sint32		   slotIdx;
	Sint32		   matchingSlot;
	TREASURE_SPOT *marker;

	marker = FieldStateWork->treasureSpots;
	matchingSlot = -1;
	for(slotIdx = 0; slotIdx <= TREASURE_SPOT_MAX - 1; slotIdx++, marker++) {
		if(marker->id == id) {
			matchingSlot = slotIdx;
			break;
		}
	}
	return matchingSlot;
}

/* Unused: empty. */
UNUSED void fdTreasureSpotReservedNoOperation(void)
{
}

/* Gives the actor of treasure spot id animation, plays sound and waits 12
   ticks. */
static __inline__ void fdSetMarkerAnimAndPlaySnd(
	Sint32 id, Sint32 anm, Sint32 snd)
{
	Sint32		   slotIdx;
	TREASURE_SPOT *marker;
	ACTOR		  *actor;

	slotIdx = fdTreasureSpotFindSlot(id);
	if(slotIdx != -1) {
		marker = &FieldStateWork->treasureSpots[slotIdx];
		actor = marker->actor;
		if(actor != NULL)
			fdActorSelectAnimationStub(actor, anm);
		sySndPlayStub(snd);
		sySynch(12);
	}
}

/* Closes the chest of treasure spot id again (its item could not be carried),
   with SE_CHEST_CLOSE. */
void fdTreasureSpotCloseChest(Sint32 id)
{
	fdSetMarkerAnimAndPlaySnd(id, TREASURE_SPOT_CHEST_CLOSING, SE_CHEST_CLOSE);
}

/* Makes the chest of treasure spot id jump up as a mimic: after 18 ticks it
   turns into the mimic with a cry (SE_ENEMY_CRY) and leaps up at 8 pixels a
   tick, its shadow showing. */
void fdTreasureSpotLaunchMimic(Sint32 id)
{
	Sint32		   slotIdx;
	TREASURE_SPOT *marker;
	ACTOR		  *actor;

	slotIdx = fdTreasureSpotFindSlot(id);
	if(slotIdx != -1) {
		marker = &FieldStateWork->treasureSpots[slotIdx];
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

/* Opens the chest of treasure spot id with SE_CHEST_OPEN, and waits 12
   ticks. */
void fdTreasureSpotOpenChest(Sint32 id)
{
	fdSetMarkerAnimAndPlaySnd(id, TREASURE_SPOT_CHEST_OPENING, SE_CHEST_OPEN);
}

/* Shows the chest of treasure spot id open, with no sound. */
void fdTreasureSpotShowOpened(Sint32 id)
{
	Sint32		   slotIdx;
	TREASURE_SPOT *marker;
	ACTOR		  *actor;

	slotIdx = fdTreasureSpotFindSlot(id);
	if(slotIdx != -1) {
		marker = &FieldStateWork->treasureSpots[slotIdx];
		actor = marker->actor;
		if(actor != NULL)
			fdActorSelectAnimationStub(actor, TREASURE_SPOT_CHEST_OPEN);
	}
}

/* Returns the actor of treasure spot id, or NULL when no slot has it. */
ACTOR *fdTreasureSpotGetActor(Sint32 id)
{
	Sint32		   slotIdx = fdTreasureSpotFindSlot(id);
	TREASURE_SPOT *marker;

	if(slotIdx == -1)
		return NULL;
	marker = &FieldStateWork->treasureSpots[slotIdx];
	return marker->actor;
}

/* Hides the actor of treasure spot id; the actor stays in its slot. */
void fdTreasureSpotHide(Sint32 id)
{
	Sint32		   slotIdx;
	TREASURE_SPOT *marker;
	ACTOR		  *actor;

	slotIdx = fdTreasureSpotFindSlot(id);
	if(slotIdx != -1) {
		marker = &FieldStateWork->treasureSpots[slotIdx];
		actor = marker->actor;
		if(actor != NULL)
			actor->sprStorageKind = ACTOR_SPRITE_NONE;
	}
}

/* Moves the actor of treasure spot id to (x, z) on the ground; -1 keeps the
   centre of its metatile on that axis. Positions are 16.16. */
void fdTreasureSpotSetActorPos(Sint32 id, FIXED x, FIXED z)
{
	Sint32		   slotIdx;
	Sint32		   groundHeight;
	Sint32		   spotX;
	Sint32		   spotZ;
	TREASURE_SPOT *marker;
	ACTOR		  *actor;

	spotZ = z;
	spotX = x;
	slotIdx = fdTreasureSpotFindSlot(id);
	if(slotIdx != -1) {
		marker = &FieldStateWork->treasureSpots[slotIdx];
		actor = marker->actor;
		if(actor != NULL) {
			if(spotX == -1)
				spotX = FIELD_METATILE_CENTER(marker->column);
			if(spotZ == -1)
				spotZ = FIELD_METATILE_CENTER(marker->row);
			actor->x.fixed = spotX;
			actor->z.fixed = spotZ;
			groundHeight = fdMapGridGetTerrainHeightStub(0, spotX, spotZ);
			actor->groundHeight = groundHeight;
			actor->y.fixed = groundHeight;
		}
	}
}
