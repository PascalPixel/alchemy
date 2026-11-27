/* @file */
/* The crumbling and pounded pillars of the Izumo Ruins and Dehkan Plateau: */
/* lists of (actor, flag) or (actor, kind, flag) entries ended by */
/* ACTOR_LIST_END, placed from their flags and stepped on, struck or pounded */
/* until they fall. */
#include "pillar_place.h"

#include "pound_sink_target.h"

#include "actor_set_cell_open.h"

#include "pillar_stand_in.h"

/* Places each crumbling pillar of an (actor, flag) list: one that has given */
/* way is moved out of sight; the others stand as their cell's ground, 32 */
/* pixels above the floor, which the leader jumps onto (MAP_CELL_JUMP), and a */
/* cracked one shows its cracks. A leader below the ground where he stands is */
/* raised onto it. */
void syCrumblingPillarPlace(u16 *list) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	ACTOR *leader;
	s32 height;

	while(list[0] != ACTOR_LIST_END) {
		u32 id = list[0];
		ACTOR *actor = fdActorGet(id);
		s32 flag = list[1];
		s32 next = flag + 1;
		MAP_CELL *cells = map->layers[actor->layer].cells;
		u8 *shapes = MapShapeGrid + (cells - (MAP_CELL *)Ram_MapCellBuffer);

		if(fdEventGetFlag(next)) {
			fdActorSetPos(id, 0, 0);
		}
		else {
			s32 index = fdActorGetCellIndex(actor);
			MAP_CELL *cell;
			u8 *slot;
			s32 shape, ground;

			cell = cells + index;
			slot = shapes + index;
			shape = fdMapGridGetTerrainKind(actor->layer, actor->x.fixed,
											actor->z.fixed);
			ground = fdMapGetTerrainHeight(actor->layer, actor->x.fixed,
										   actor->z.fixed);

			fdActorDisableById(id);
			cell->attributeB = MAP_CELL_JUMP;
			*slot = fdMapShapeFindOrAdd(actor->layer, shape,
										CELL_HEIGHT_STEPS(ground) + 4);
			if(fdEventGetFlag(flag))
				fdActorSelectAnimation(actor, 0);
		}
		list += 2;
	}
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	height =
		fdMapGetTerrainHeight(leader->layer, leader->x.fixed, leader->z.fixed);
	if(leader->y.fixed < height) {
		leader->groundHeight = height;
		leader->y.fixed = height;
	}
}

/* Stepping onto a crumbling pillar: the first time it cracks, the second time
 */
/* it gives way with the leader. */
void syCrumblingPillarStep(u16 *list) {
	ACTOR *leader;
	ACTOR *pillar;
	FIXED position[3];

	if(fdTryJumpForward() != 0)
		return;
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	position[0] = leader->x.fixed;
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed;
	pillar = fdActorFindAtCell(position, 0);
	if(pillar == 0)
		return;
	while(list[0] != ACTOR_LIST_END) {
		u32 id = list[0];

		if(fdActorGetById(id) != pillar) {
			list += 2;
			continue;
		}
		{
			s32 flag = list[1];

			fdEventStart();
			if(!syGameFlagGet(flag)) {
				sySndPlay(SE_CHEST_CLOSE);
				fdActorSetSprCol(fdActorGetById(id), SPRITE_COLOR_WHITE);
				sySynch(2);
				fdActorSelectAnimation(pillar, 0);
				fdActorSetSprCol(fdActorGetById(id), SPRITE_COLOR_NORMAL);
				sySynch(2);
				fdActorSetSprCol(fdActorGetById(id), SPRITE_COLOR_WHITE);
				sySynch(4);
				fdActorSetSprCol(fdActorGetById(id), SPRITE_COLOR_NORMAL);
				scDustScatter(0);
				syGameFlagSet(flag);
			}
			else if(!syGameFlagGet(flag + 1)) {
				u8 *flags = &leader->motionFlg;

				*flags = 0;
				sySndPlay(SE_PUSH);
				fdMapGridSetShake(1, 4, 1);
				scDustScatter(0);
				sySynch(8);
				fdActorSelectAnimation(pillar, 2);
				fdLeaderSetCellShapeAt(-2, pillar->layer);
				scDustScatter(1);
				sySynch(16);
				fdLeaderSetCellShapeAt(-2, pillar->layer);
				sySynch(4);
				fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
				sySynch(8);
				*flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
				sySynch(5);
				fdActorSetPosAndResetMotion(pillar, 0, 0, 0);
				sySynch(2);
				sySndPlay(SE_THUMP);
				scDustBurst();
				sySynch(20);
				syGameFlagSet(flag + 1);
			}
			fdEventEnd();
			return;
		}
	}
}

/* A crumbling pillar named in an (actor, flag) list gives way with a shake, */
/* dust and a thump. */
void syCrumblingPillarCollapse(const void *data, s32 id) {
	const TRAP_ENTRY *entry = data;
	ACTOR *actor = fdActorGet(id);
	s32 flag;

	while(entry->actor != ACTOR_LIST_END) {
		if(entry->actor != id) {
			entry++;
			continue;
		}
		{
			flag = entry->flg;
			fdEventStart();
			if(!fdEventGetFlag(flag + 1)) {
				fdEventPlaySnd(SE_PUSH);
				fdMapGridSetShake(1, 4, 1);
				scDustScatter(0);
				syWait(8);
				fdActorSelectAnimation(actor, 2);
				fdActorSetCellShape(-4, actor->layer, id);
				scDustScatter(1);
				syWait(16);
				fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
				syWait(8);
				scFieldActorSetPosAndResetMotion(actor, 0, 0, 0);
				syWait(2);
				fdEventPlaySnd(SE_THUMP);
				scDustBurst();
				syWait(20);
				fdEventSetFlag(flag);
				fdEventSetFlag(flag + 1);
			}
			fdEventEnd();
			break;
		}
	}
}

/* Keeps an actor's sprite picture loaded even out of sight, and stops it */
/* moving, colliding and showing (fdActorDisableById). */
void syCrumblingPillarPrepare(s32 id) {
	fdActorGet(id)->retainSprCache =
		ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
	fdActorDisableById(id);
}

/* As syCrumblingPillarPlace, in a room whose pillars share one sprite: first */
/* loads the stand-in's and the two pictures' sprites beside the leader, the */
/* cracked one showing its cracks, then puts them out of sight; a cracked */
/* pillar takes the cracked picture. */
void syCrumblingPillarPlaceWithStandIn(u16 *list) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	u8 *grid;

	fdActorPlaceOnActor(ACTOR_PILLAR_STAND_IN,
						MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_CRACKED_PILLAR_SPRITE,
						MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_WHOLE_PILLAR_SPRITE,
						MainRoutineStateWork.leaderActorId);
	syWait(1);
	syCrumblingPillarPrepare(ACTOR_PILLAR_STAND_IN);
	syCrumblingPillarPrepare(ACTOR_CRACKED_PILLAR_SPRITE);
	syCrumblingPillarPrepare(ACTOR_WHOLE_PILLAR_SPRITE);
	fdActorSetAnimation(ACTOR_CRACKED_PILLAR_SPRITE, 0);
	syWait(1);
	fdActorSetPos(ACTOR_PILLAR_STAND_IN, 0, 0);
	fdActorSetPos(ACTOR_CRACKED_PILLAR_SPRITE, 0, 0);
	fdActorSetPos(ACTOR_WHOLE_PILLAR_SPRITE, 0, 0);
	syWait(1);
	grid = MapShapeGrid;
	while(list[0] != ACTOR_LIST_END) {
		u32 id = list[0];
		ACTOR *actor = fdActorGet(id);
		s32 flag = list[1];
		s32 next = flag + 1;
		MAP_CELL *cells = map->layers[actor->layer].cells;
		u8 *shapes = grid + (cells - (MAP_CELL *)Ram_MapCellBuffer);

		actor->retainSprCache =
			ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
		if(fdEventGetFlag(next)) {
			fdActorSetPos(id, 0, 0);
		}
		else {
			s32 index = fdActorGetCellIndex(actor);
			MAP_CELL *cell = cells + index;
			u8 *shape = shapes + index;
			s32 kind, ground;

			kind = fdMapGridGetTerrainKind(actor->layer, actor->x.fixed,
										   actor->z.fixed);
			ground = fdMapGetTerrainHeight(actor->layer, actor->x.fixed,
										   actor->z.fixed);

			fdActorDisableById(id);
			cell->attributeB = MAP_CELL_JUMP;
			*shape = fdMapShapeFindOrAdd(actor->layer, kind,
										 CELL_HEIGHT_STEPS(ground) + 4);
			if(fdEventGetFlag(flag))
				fdActorCopyGraphics(id, ACTOR_CRACKED_PILLAR_SPRITE);
		}
		list += 2;
	}
	syWait(10);
}

#include "actor_take_place_of.h"

/* As syCrumblingPillarStep, in a room whose pillars share one sprite: the */
/* stand-in takes the pillar's place to crack or give way, and a cracked pillar
 */
/* takes the cracked picture. */
void syCrumblingPillarStepWithStandIn(u16 *list) {
	ACTOR *leader;
	ACTOR *standIn;
	ACTOR *pillar;
	FIXED position[3];

	if(fdTryJumpForward() != 0)
		return;
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	standIn = fdActorGetById(ACTOR_PILLAR_STAND_IN);
	position[0] = leader->x.fixed;
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed;
	pillar = fdActorFindAtCell(position, 0);
	if(pillar == 0)
		return;
	while(list[0] != ACTOR_LIST_END) {
		u32 id = list[0];

		if(fdActorGetById(id) != pillar) {
			list += 2;
			continue;
		}
		{
			s32 flag = list[1];

			fdEventStart();
			if(!syGameFlagGet(flag)) {
				fdActorSelectAnimation(standIn, 1);
				fdActorTakePlaceOf(standIn, pillar);
				sySynch(1);
				sySndPlay(SE_CHEST_CLOSE);
				fdActorSetSprCol(fdActorGetById(ACTOR_PILLAR_STAND_IN),
								 SPRITE_COLOR_WHITE);
				sySynch(2);
				fdActorSelectAnimation(standIn, 0);
				fdActorCopyGraphics(id, ACTOR_CRACKED_PILLAR_SPRITE);
				fdActorSetSprCol(fdActorGetById(ACTOR_PILLAR_STAND_IN),
								 SPRITE_COLOR_NORMAL);
				sySynch(2);
				fdActorSetSprCol(fdActorGetById(ACTOR_PILLAR_STAND_IN),
								 SPRITE_COLOR_WHITE);
				sySynch(4);
				fdActorSetSprCol(fdActorGetById(ACTOR_PILLAR_STAND_IN),
								 SPRITE_COLOR_NORMAL);
				scDustScatter(0);
				fdActorTakePlaceOf(pillar, standIn);
				sySynch(1);
				syGameFlagSet(flag);
			}
			else if(!syGameFlagGet(flag + 1)) {
				u8 *flags;

				fdActorSelectAnimation(standIn, 0);
				fdActorTakePlaceOf(standIn, pillar);
				sySynch(1);
				flags = &leader->motionFlg;
				*flags = 0;
				sySndPlay(SE_PUSH);
				fdMapGridSetShake(1, 4, 1);
				scDustScatter(0);
				sySynch(8);
				fdActorSelectAnimation(standIn, 2);
				fdLeaderSetCellShapeAt(-2, pillar->layer);
				scDustScatter(1);
				sySynch(16);
				fdLeaderSetCellShapeAt(-2, pillar->layer);
				sySynch(4);
				fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
				sySynch(8);
				*flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
				sySynch(5);
				fdActorSetPosAndResetMotion(standIn, 0, 0, 0);
				sySynch(2);
				sySndPlay(SE_THUMP);
				scDustBurst();
				sySynch(20);
				syGameFlagSet(flag + 1);
			}
			fdEventEnd();
			return;
		}
	}
}
