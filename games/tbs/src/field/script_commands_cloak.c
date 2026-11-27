/* Cloak: the leader blinks into a dark outline and stays hidden while it walks
   on metatiles that allow it (METATILE_BEHAVIOR_CLOAK). Part of
   script_commands.c. */

#include "script_commands_internal.h"

/* Event of Cloak: when the metatile under the leader is not
   METATILE_BEHAVIOR_CLOAK, requests CLOAK_END. The behavior is byte 2 of a
   metatile (MAP_GRID_METATILE.behavior), or of a 4-byte MetatileTiles entry on
   the World Map. */
void fdPsynergyCheckCloakTile(void)
{
	FIELD_STATE *state;
	ACTOR		*leader;
	Uint8		*metatile;
	FIXED		 x;
	FIXED		 y;

	state = FieldStateWork;
	leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
	metatile = MapGridState[0];

	/* Whole blocks or metatiles, rounded toward zero as a division would be. */
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		Uint32 tx;
		Uint32 ty;

		tx = leader->x.fixed / toFIXED(WORLD_MAP_BLOCK_PIXELS) &
			(WORLD_MAP_BLOCKS_ACROSS - 1);
		ty = leader->z.fixed / toFIXED(WORLD_MAP_BLOCK_PIXELS) &
			(WORLD_MAP_BLOCKS_ACROSS - 1);

		metatile = (Uint8 *)&MetatileTiles[tx + ty * WORLD_MAP_BLOCKS_ACROSS];
	} else {
		if(leader->layer < MAPGRID_LAYER_COUNT) {
			metatile = (Uint8 *)((MAP_GRID_SCROLL_STATE *)metatile)
						   ->layers[leader->layer]
						   .metatiles;
		} else
			metatile = (Uint8 *)MapGridMetatileBuffer;

		x = leader->x.fixed;
		{
			Uint32 tx = x / toFIXED(MAPGRID_METATILE_PIXELS);

			y = leader->z.fixed;

			metatile = (Uint8 *)(metatile +
				((tx + y / toFIXED(MAPGRID_METATILE_PIXELS) * MAPGRID_SIZE)
					<< 2));
		}
	}

	if(metatile[2] != METATILE_BEHAVIOR_CLOAK)
		state->psynergy = CLOAK_END;
}

/* Ends Cloak: stops the metatile check and makes the leader blink back into
   sight, clearing cloaked. */
void fdPsynergyEndCloak(void)
{
	ACTOR		  *actor;
	SPR			  *sprite;
	SPR_ANIMATION *entry;
	Uint32		   i;

	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	sprite = actor->sprite;
	entry = sprite->entries[0];

	sySoundPlayStub(SE_CAST);
	syCloseEvent(fdPsynergyCheckCloakTile);
	fdActorSelectAnimationStub(actor, ANIM_STILL);
	actor->update = 0;

	for(i = 0; i < 5; ++i) {
		entry->colorMode = SPRITE_COLOR_WHITE;
		sprite->dirty = 1;
		sprite->flags = SPRITE_FLAG_OUTLINED;
		sySynch(2);
		sprite->dirty = 1;
		sprite->flags = 0;
		sySynch(2);
	}

	for(i = 0; i < 5; ++i) {
		entry->colorMode = SPRITE_COLOR_WHITE;
		sprite->dirty = 1;
		sprite->flags = 0;
		sySynch(2);
		entry->colorMode = SPRITE_COLOR_NORMAL;
		sprite->dirty = 1;
		sySynch(2);
	}

	sprite->flags = SPRITE_FLAG_SHADOW;
	MainRoutineStateWork.cloaked = 0;
}

/* Starts the Cloak metatile check if the leader is hidden. fdModeRun calls it
   as a scene loads, since MainRoutineStateWork.cloaked outlives the scene. */
void fdPsynergyResumeCloakCheck(void)
{
	if(MainRoutineStateWork.cloaked != 0) {
		sySetEvent(fdPsynergyCheckCloakTile, EVENT_PASS_TICK);
	}
}

/* The leader blinks 10 times and hides (cloaked set), and the metatile check
   starts; where the leader cannot hide it shows again at once. */
void fdPsynergyStartCloak(void)
{
	FIELD_STATE	  *state = FieldStateWork;
	ACTOR		  *leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
	SPR			  *sprite = leader->sprite;
	SPR_ANIMATION *entry = sprite->entries[0];
	Uint32		   n;
	void (*check)(void);

	sySoundPlayStub(SE_PSYNERGY_START);
	fdActorSelectAnimationStub(leader, ANIM_STILL);
	leader->update = 0;
	for(n = 0; n <= 9; n++) {
		entry->colorMode = SPRITE_COLOR_WHITE;
		sprite->dirty = 1;
		sprite->flags = SPRITE_FLAG_OUTLINED;
		sySynch(2);
		sprite->dirty = 1;
		sprite->flags = 0;
		sySynch(2);
	}
	n = 0;
	entry->colorMode = n;
	sprite->flags = SPRITE_FLAG_OUTLINED;
	sprite->dirty = 1;
	/* Check the metatile at once: off a Cloak metatile, unhide and drop the
	   request the check just raised (n is 0 here). */
	check = fdPsynergyCheckCloakTile;
	sySetEvent(check, EVENT_PASS_TICK);
	MainRoutineStateWork.cloaked = 1;
	check();
	if(state->psynergy == CLOAK_END) {
		fdPsynergyEndCloak();
		state->psynergy = n;
	}
}

#undef WORLD_MAP_BLOCK
