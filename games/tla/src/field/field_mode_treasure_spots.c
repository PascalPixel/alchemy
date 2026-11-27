/* The map's treasure spots: chests and Psynergy Stones. Part of
   field_mode.c. */

#include "field/script.h"

/* Sets an actor's y and groundHeight to the map's height at its x and z. */
void fdActorSnapToTerrain(void *actorPtr)
{
	ACTOR *actor = actorPtr;
	Sint32 groundHeight;

	groundHeight =
		fdMapGridGetTerrainHeightStub(0, actor->x.fixed, actor->z.fixed);
	actor->y.fixed = groundHeight;
	actor->groundHeight = groundHeight;
}

/* The current scene's rows of the scene item table (SceneItems): those after
   the first header for this scene whose flag is -1 or not yet set, or NULL. */
Sint16 *fdSceneItemGetRows(void)
{
	Sint32	scene = MainRoutineStateWork.next;
	Sint16 *p = SceneItems;
	Sint16 *found = NULL;
	Sint32	header;
	Sint32	flg;
	Sint16	value;

	for(;;) {
		header = *p++;
		flg = *p++;
		if(header == 0)
			break;
		if((header & (SCENE_ITEMS_HEADER - 1)) == scene &&
			(flg == -1 || !syGameFlagGet(flg))) {
			found = p;
			break;
		}
		/* Skip to the next header or the end. */
		while((value = p[0]) != 0 && !(value & SCENE_ITEMS_HEADER))
			p += 2;
	}
	return found;
}

/* Clears out, then fills it with the position of scene point value; returns 0
   when the point was found, else -1. */
Sint32 fdSceneItemFindPos(Sint32 value, Sint32 *out)
{
	SET_VECTOR(out, 0, 0, 0);
	return fdScenePointGetPos(value, out);
}

/* Places an actor at each of the map's treasure spots (ids 100 to 239) that the
   scene has a script for, up to sixteen: a chest for a chest script, shown open
   once its flag is set and gone once its mimic was beaten, and a Psynergy Stone
   for an examine cue giving one, until it is taken. */
void fdPlaceTreasureSpots(void)
{
	Sint16		*rows = fdSceneItemGetRows();
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	Sint32		 i;
	Sint16		 type;
	Sint32		 id;
	Sint32		 flg;
	Sint32		 item;
	Sint32		 pos[3];
	Sint32		 kind;
	Sint32		 buried;
	ACTOR		*actor;

	if(rows == NULL)
		return;
	i = 0;
	type = *rows++;
	if(type == 0 || (type & SCENE_ITEMS_HEADER))
		return;
	do {
		id = rows[0];
		flg = rows[1];
		item = rows[2];
		rows += 3;
		if((type == CONTAINER_CHEST || flg == -1 || !syGameFlagGet(flg)) &&
			!fdSceneItemFindPos(id, pos)) {
			buried = 0;
			if((Uint16)(type - CONTAINER_CHEST) <=
				CONTAINER_MIMIC - CONTAINER_CHEST)
				kind = SPRITE_CHEST;
			else if(type == CONTAINER_PSYNERGY_STONE)
				kind = SPRITE_PSYNERGY_STONE;
			else if(type == CONTAINER_STONE_TABLET)
				kind = SPRITE_STONE_TABLET;
			else if(type == CONTAINER_GROUND)
				kind = SPRITE_UNSEEN_MARKER;
			else {
				buried = 1;
				kind = SPRITE_UNSEEN_MARKER;
			}
			actor = fdActorCreateStub(kind, pos[0], pos[1], pos[2]);
			if(actor != NULL) {
				if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
					actor->motionFlg = ACTOR_FALLS;
					actor->groundHeight = 0;
					actor->y.fixed = 0;
				} else {
					actor->groundHeight = fdMapGridGetTerrainHeightStub(
						0, actor->x.fixed, actor->z.fixed);
					actor->y.fixed += actor->groundHeight;
					if(buried) {
						Uint8		 *motionFlg = &actor->motionFlg;
						MAP_STATE	 *map;
						MAP_METATILE *metatiles;
						Sint32		  x;
						Sint32		  z;

						*motionFlg = ACTOR_FALLS;
						actor->groundHeight -= toFIXED(16);
						actor->y.fixed = actor->groundHeight;
						map = Ram_HeapSlots->mapGrid;
						x = FIELD_METATILE(actor->x.fixed);
						z = FIELD_METATILE(actor->z.fixed);
						metatiles = (MAP_METATILE *)MapGridMetatileBuffer;
						if(map != NULL)
							metatiles = map->layers[0].metatiles;
						metatiles += x + z * MAPGRID_SIZE;
						if((Uint32)(metatiles->collisionCode -
							   METATILE_BEHAVIOR_JAR) <=
							METATILE_BEHAVIOR_WELL - METATILE_BEHAVIOR_JAR)
							metatiles->collisionCode = METATILE_BEHAVIOR_NORMAL;
					}
				}
				fdActorSelectAnimationStub(actor, ANIM_STAND);
				fdActorSetSprFlagsStub(actor, 0);
				if(syGameFlagGet(flg))
					fdActorSelectAnimationStub(actor, TREASURE_SPOT_CHEST_OPEN);
				fdActorStartIdleStub(actor);
				actor->homeX = actor->x.fixed / toFIXED(1);
				actor->homeZ = actor->z.fixed / toFIXED(1);
				actor->priFlg = ACTOR_PRIORITY_AUTOMATIC;
				actor->collisionFlg = ACTOR_SOLID;
				state->actors[FIELD_TREASURE_ACTOR + i] = actor;
				if(type == CONTAINER_GROUND)
					fdActorShowItemIconById(FIELD_TREASURE_ACTOR + i, item);
				if(type == CONTAINER_POND)
					actor->collisionFlg = 0;
			}
		}
		i++;
		if(i >= TREASURE_SPOT_MAX)
			break;
		type = *rows++;
		if(type == 0)
			break;
	} while(!(type & SCENE_ITEMS_HEADER));
}

/* OAM attribute 1's sizes (GBATEK "OBJ Attributes"): an OBJ_SHAPE_HORIZONTAL
   sprite of size 0 is 16 by 8 pixels, an OBJ_SHAPE_SQUARE one of size 1 16 by
   16. */
enum {
	OAM_SIZE_16X8 = 0, /* with OBJ_SHAPE_HORIZONTAL */
	OAM_SIZE_16X16 = 1 /* with OBJ_SHAPE_SQUARE */
};

/* TREASURE_SPOT_PUT_AWAY moves the actor to x and z 0 and marks it with
   sceneWord TREASURE_SPOT_ON_METATILE; any other mode draws its sprite wide, 16
   by 8 pixels, 4 pixels lower. */
void fdActorSetWideSpr(Uint32 actorId, Sint32 mode)
{
	ACTOR *actor;
	SPR	  *spr;

	actor = fdActorLookup(actorId);
	if(actor == 0)
		return;
	if(mode == TREASURE_SPOT_PUT_AWAY) {
		actor->x.fixed = 0;
		actor->z.fixed = 0;
		actor->sceneWord = TREASURE_SPOT_ON_METATILE;
		return;
	}
	spr = actor->spr;
	if(spr == 0)
		return;
	spr->ofsY += 4;
	spr->image.shape = OBJ_SHAPE_HORIZONTAL;
	spr->image.size = OAM_SIZE_16X8;
}

/* Draws the actor's sprite square, 16 by 16 pixels, 8 pixels down. */
void fdActorSetSquareSpr(Uint32 actorId)
{
	ACTOR *actor;
	SPR	  *spr;

	actor = fdActorLookup(actorId);
	if(actor == 0)
		return;
	spr = actor->spr;
	if(spr == 0)
		return;
	spr->ofsY = 8;
	spr->image.shape = OBJ_SHAPE_SQUARE;
	spr->image.size = OAM_SIZE_16X16;
}

/* Returns the OAM shape of the actor's sprite (0 square, 1 wide, 2 tall), or 0
   without one. */
Sint32 fdActorGetSprShape(Uint32 actorId)
{
	ACTOR *actor;
	SPR	  *spr;
	Sint32 shape = OBJ_SHAPE_SQUARE;

	actor = fdActorLookup(actorId);
	if(actor != 0) {
		spr = actor->spr;
		if(spr != 0)
			shape = spr->image.shape;
	}
	return shape;
}

/* RETURN_HOME_CLEAR_Z moves the actor to z 0; RETURN_HOME_DUG_UP sets flag,
   draws a moved actor square again and drops it back home (homeX, homeZ) at
   Scoop's anchor. */
void fdActorReturnHome(Sint32 mode, Uint32 actorId, Sint32 flg)
{
	ACTOR *actor;

	actor = fdActorLookup(actorId);
	if(actor == 0)
		return;
	if(mode == RETURN_HOME_CLEAR_Z)
		actor->z.fixed = 0;
	if(mode == RETURN_HOME_DUG_UP) {
		syGameFlagSet(flg);
		if(actor->x.fixed != 0)
			fdActorSetSquareSpr(actorId);
		actor->x.fixed = toFIXED(actor->homeX);
		actor->z.fixed = toFIXED(actor->homeZ);
		fdScoopDropFromHand(actorId);
	}
}

/* Shakes the actor's facing at random for 16 ticks, then turns it back. */
UNUSED void fdActorShakeAng(Uint32 actorId)
{
	ACTOR *actor;
	Sint32 ang;
	Sint32 i;

	actor = fdActorLookup(actorId);
	if(actor == 0)
		return;
	ang = actor->facing;
	for(i = 0; i < 16; i++) {
		Uint32 a = syRandom();
		Uint32 b = syRandom();
		Uint32 cur;

		cur = actor->facing;
		b >>= 4;
		a >>= 4;
		actor->facing = cur + (a - b);
		sySynch(1);
	}
	actor->facing = ang;
}

/* Closes the chest of the actor again (its item could not be carried), with
   SE_CHEST_CLOSE, and waits 12 ticks. */
void fdTreasureSpotCloseChest(Uint32 actorId)
{
	fdActorSelectAnimationStub(
		fdActorLookup(actorId), TREASURE_SPOT_CHEST_CLOSING);
	sySndPlayStub(SE_CHEST_CLOSE);
	sySynch(12);
}

/* Makes the chest of the actor jump up as a mimic: after 18 ticks it takes
   TREASURE_SPOT_CHEST_MIMIC, plays SE_ENEMY_CRY and gets an upward velocity of
   8 pixels a tick, its shadow shown. */
void fdTreasureSpotLaunchMimic(Uint32 actorId)
{
	ACTOR *actor;

	actor = fdActorLookup(actorId);
	sySynch(18);
	fdActorSelectAnimationStub(actor, TREASURE_SPOT_CHEST_MIMIC);
	sySndPlayStub(SE_ENEMY_CRY);
	if(actor != 0) {
		actor->velY = toFIXED(8);
		fdActorSetSprFlagsStub(actor, SPRITE_FLAG_SHADOW);
	}
}

/* Opens the chest of the actor with SE_CHEST_OPEN, and waits 12 ticks. */
void fdTreasureSpotOpenChest(Uint32 actorId)
{
	fdActorSelectAnimationStub(
		fdActorLookup(actorId), TREASURE_SPOT_CHEST_OPENING);
	sySndPlayStub(SE_CHEST_OPEN);
	sySynch(12);
}

/* Shows the chest of the actor open, with no sound. */
void fdTreasureSpotShowOpened(Uint32 actorId)
{
	fdActorSelectAnimationStub(
		fdActorLookup(actorId), TREASURE_SPOT_CHEST_OPEN);
}

/* Opens the empty chest of the actor with SE_CHEST_OPEN, and waits 12
   ticks. */
void fdTreasureSpotOpenEmptyChest(Uint32 actorId)
{
	fdActorSelectAnimationStub(
		fdActorLookup(actorId), TREASURE_SPOT_CHEST_OPENING_EMPTY);
	sySndPlayStub(SE_CHEST_OPEN);
	sySynch(12);
}

/* Returns the actor, or NULL. */
ACTOR *fdTreasureSpotGetActor(Uint32 actorId)
{
	return fdActorLookup(actorId);
}

/* Releases the actor and empties its slot. */
void fdTreasureSpotRemove(Uint32 actorId)
{
	fdActorReleaseAtIndex(actorId);
}

/* Moves the actor to (x, z) on the ground, 16.16 pixels; -1 keeps its home
   (homeX, homeZ) on that axis. */
void fdTreasureSpotSetActorPos(Uint32 actorId, FIXED x, FIXED z)
{
	ACTOR *actor;

	actor = fdActorLookup(actorId);
	if(actor == 0)
		return;
	if(x == -1)
		x = toFIXED(actor->homeX);
	if(z == -1)
		z = toFIXED(actor->homeZ);
	actor->x.fixed = x;
	actor->z.fixed = z;
	actor->y.fixed = actor->groundHeight =
		fdMapGridGetTerrainHeightStub(0, x, z);
}
