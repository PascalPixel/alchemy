/* @file */
/* Cloak: the leader blinks into a dark outline and stays hidden while it */
/* walks on cells that allow it (CELL_BEHAVIOR_CLOAK). */
/* Part of event_commands.c. */

#include "event_commands_internal.h"

/* Task of Cloak: when the cell under the leader is not CELL_BEHAVIOR_CLOAK, */
/* requests CLOAK_END. The behavior is byte 2 of a cell
 * (MAP_GRID_CELL.behavior),
 */
/* or of a 4-byte CellTiles entry on the World Map. */
void fdPsynergyCheckCloakTile(void) {
	FIELD_STATE *state;
	ACTOR *leader;
	u8 *cell;
	FIXED x;
	FIXED y;

	state = FieldStateWork;
	leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
	cell = MapGridState[0];

	/* Whole blocks or cells, rounded toward zero as a division would be. */
	if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		u32 tx;
		u32 ty;

		tx = leader->x.fixed / toFIXED(WORLD_MAP_BLOCK_PIXELS) &
			 (WORLD_MAP_BLOCKS_ACROSS - 1);
		ty = leader->z.fixed / toFIXED(WORLD_MAP_BLOCK_PIXELS) &
			 (WORLD_MAP_BLOCKS_ACROSS - 1);

		cell = (u8 *)&CellTiles[tx + ty * WORLD_MAP_BLOCKS_ACROSS];
	}
	else {
		if(leader->layer < MAPGRID_LAYER_COUNT) {
			cell = (u8 *)((MAP_GRID_SCROLL_STATE *)cell)
					   ->layers[leader->layer]
					   .cells;
		}
		else
			cell = (u8 *)MapGridCellBuffer;

		x = leader->x.fixed;
		{
			u32 tx = x / toFIXED(MAPGRID_CELL_PIXELS);

			y = leader->z.fixed;

			cell =
				(u8 *)(cell +
					   ((tx + y / toFIXED(MAPGRID_CELL_PIXELS) * MAPGRID_SIZE)
						<< 2));
		}
	}

	if(cell[2] != CELL_BEHAVIOR_CLOAK)
		state->psynergy = CLOAK_END;
}

/* Ends Cloak: stops the cell check and makes the leader blink back into sight,
 */
/* clearing cloaked. */
void fdPsynergyEndCloak(void) {
	ACTOR *actor;
	SPR *sprite;
	SPR_ANIMATION *entry;
	u32 i;

	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	sprite = actor->spr;
	entry = sprite->entries[0];

	sySndPlayStub(SE_CAST);
	syCloseEvent(fdPsynergyCheckCloakTile);
	fdActorSelectAnimationStub(actor, ANIM_STILL);
	actor->update = 0;

	for(i = 0; i < 5; ++i) {
		entry->colMode = SPRITE_COLOR_WHITE;
		sprite->dirty = 1;
		sprite->flg = SPRITE_FLAG_OUTLINED;
		sySynch(2);
		sprite->dirty = 1;
		sprite->flg = 0;
		sySynch(2);
	}

	for(i = 0; i < 5; ++i) {
		entry->colMode = SPRITE_COLOR_WHITE;
		sprite->dirty = 1;
		sprite->flg = 0;
		sySynch(2);
		entry->colMode = SPRITE_COLOR_NORMAL;
		sprite->dirty = 1;
		sySynch(2);
	}

	sprite->flg = SPRITE_FLAG_SHADOW;
	MainRoutineStateWork.cloaked = 0;
}

/* Starts the Cloak cell check if the leader is hidden. fdSequenceRun calls */
/* it as a scene loads, since MainRoutineStateWork.cloaked outlives the scene.
 */
void fdPsynergyResumeCloakCheck(void) {
	if(MainRoutineStateWork.cloaked != 0) {
		sySetEvent(fdPsynergyCheckCloakTile, TASK_PASS_FRAME);
	}
}

/* The leader blinks 10 times and hides (cloaked set), and the cell check */
/* starts; where the leader cannot hide it shows again at once. */
void fdPsynergyStartCloak(void) {
	FIELD_STATE *state = FieldStateWork;
	ACTOR *leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
	SPR *sprite = leader->spr;
	SPR_ANIMATION *entry = sprite->entries[0];
	u32 n;
	void (*check)(void);

	sySndPlayStub(SE_PSYNERGY_START);
	fdActorSelectAnimationStub(leader, ANIM_STILL);
	leader->update = 0;
	for(n = 0; n <= 9; n++) {
		entry->colMode = SPRITE_COLOR_WHITE;
		sprite->dirty = 1;
		sprite->flg = SPRITE_FLAG_OUTLINED;
		sySynch(2);
		sprite->dirty = 1;
		sprite->flg = 0;
		sySynch(2);
	}
	n = 0;
	entry->colMode = n;
	sprite->flg = SPRITE_FLAG_OUTLINED;
	sprite->dirty = 1;
	/* Check the cell at once: off a Cloak cell, unhide and drop the request */
	/* the check just raised (n is 0 here). */
	check = fdPsynergyCheckCloakTile;
	sySetEvent(check, TASK_PASS_FRAME);
	MainRoutineStateWork.cloaked = 1;
	check();
	if(state->psynergy == CLOAK_END) {
		fdPsynergyEndCloak();
		state->psynergy = n;
	}
}

#undef WORLD_MAP_BLOCK
