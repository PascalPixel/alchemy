/* The crumbling and pounded pillars of the Izumo Ruins and Dehkan Plateau:
   lists of (actor, flag) or (actor, kind, flag) entries ended by
   ACTOR_LIST_END, placed from their flags and stepped on, struck or pounded
   until they fall. */
#include "pillar_place.h"

#include "pound_sink_target.h"

#include "actor_set_metatile_open.h"

#include "pillar_stand_in.h"

/* Places each crumbling pillar of an (actor, flag) list: one that has given way
   is moved out of sight; the others stand as their metatile's ground, 32 pixels
   above the floor, which the leader jumps onto (MAP_METATILE_JUMP), and a
   cracked one shows its cracks. A leader below the ground where he stands is
   raised onto it. */
void syCrumblingPillarPlace(Uint16 *list)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	ACTOR	  *leader;
	Sint32	   height;

	while(list[0] != ACTOR_LIST_END) {
		Uint32		  id = list[0];
		ACTOR		 *actor = fdActorGet(id);
		Sint32		  flg = list[1];
		Sint32		  next = flg + 1;
		MAP_METATILE *metatiles = map->layers[actor->layer].metatiles;
		Uint8		 *shapes =
			MapShapeGrid + (metatiles - (MAP_METATILE *)Ram_MapMetatileBuffer);

		if(fdScriptGetFlag(next)) {
			fdActorSetPos(id, 0, 0);
		} else {
			Sint32		  idx = fdActorGetMetatileIndex(actor);
			MAP_METATILE *metatile;
			Uint8		 *slot;
			Sint32		  shape, ground;

			metatile = metatiles + idx;
			slot = shapes + idx;
			shape = fdMapGridGetTerrainKind(
				actor->layer, actor->x.fixed, actor->z.fixed);
			ground = fdMapGetTerrainHeight(
				actor->layer, actor->x.fixed, actor->z.fixed);

			fdActorDisableById(id);
			metatile->attributeB = MAP_METATILE_JUMP;
			*slot = fdMapShapeFindOrAdd(
				actor->layer, shape, METATILE_HEIGHT_STEPS(ground) + 4);
			if(fdScriptGetFlag(flg))
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
   it gives way with the leader. */
void scCrumblingPillarStep(Uint16 *list)
{
	ACTOR *leader;
	ACTOR *pillar;
	FIXED  pos[3];

	if(fdTryJumpForward() != 0)
		return;
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	pos[0] = leader->x.fixed;
	pos[1] = leader->y.fixed;
	pos[2] = leader->z.fixed;
	pillar = fdActorFindAtMetatile(pos, 0);
	if(pillar == 0)
		return;
	while(list[0] != ACTOR_LIST_END) {
		Uint32 id = list[0];

		if(fdActorGetById(id) != pillar) {
			list += 2;
			continue;
		}
		{
			Sint32 flg = list[1];

			fdScriptStart();
			if(!syGameFlagGet(flg)) {
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
				syGameFlagSet(flg);
			} else if(!syGameFlagGet(flg + 1)) {
				Uint8 *motionFlg = &leader->motionFlg;

				*motionFlg = 0;
				sySndPlay(SE_PUSH);
				fdMapGridSetShake(1, 4, 1);
				scDustScatter(0);
				sySynch(8);
				fdActorSelectAnimation(pillar, 2);
				fdLeaderSetMetatileShapeAt(-2, pillar->layer);
				scDustScatter(1);
				sySynch(16);
				fdLeaderSetMetatileShapeAt(-2, pillar->layer);
				sySynch(4);
				fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
				sySynch(8);
				*motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
				sySynch(5);
				fdActorSetPosAndResetMotion(pillar, 0, 0, 0);
				sySynch(2);
				sySndPlay(SE_THUMP);
				scDustBurst();
				sySynch(20);
				syGameFlagSet(flg + 1);
			}
			fdScriptEnd();
			return;
		}
	}
}

/* A crumbling pillar named in an (actor, flag) list gives way with a shake,
   dust and a thump. */
void scCrumblingPillarCollapse(const void *data, Sint32 id)
{
	const TRAP_ENTRY *entry = data;
	ACTOR			 *actor = fdActorGet(id);
	Sint32			  flg;

	while(entry->actor != ACTOR_LIST_END) {
		if(entry->actor != id) {
			entry++;
			continue;
		}
		{
			flg = entry->flg;
			fdScriptStart();
			if(!fdScriptGetFlag(flg + 1)) {
				fdScriptPlaySnd(SE_PUSH);
				fdMapGridSetShake(1, 4, 1);
				scDustScatter(0);
				syWait(8);
				fdActorSelectAnimation(actor, 2);
				fdActorSetMetatileShape(-4, actor->layer, id);
				scDustScatter(1);
				syWait(16);
				fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
				syWait(8);
				scFieldActorSetPosAndResetMotion(actor, 0, 0, 0);
				syWait(2);
				fdScriptPlaySnd(SE_THUMP);
				scDustBurst();
				syWait(20);
				fdScriptSetFlag(flg);
				fdScriptSetFlag(flg + 1);
			}
			fdScriptEnd();
			break;
		}
	}
}

/* Keeps an actor's sprite picture loaded even out of sight, and stops it
   moving, colliding and showing (fdActorDisableById). */
void syCrumblingPillarPrepare(Sint32 id)
{
	fdActorGet(id)->retainSprCache =
		ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
	fdActorDisableById(id);
}

/* As syCrumblingPillarPlace, in a room whose pillars share one sprite: first
   loads the stand-in's and the two pictures' sprites beside the leader, the
   cracked one showing its cracks, then puts them out of sight; a cracked pillar
   takes the cracked picture. */
void scCrumblingPillarPlaceWithStandIn(Uint16 *list)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	Uint8	  *grid;

	fdActorPlaceOnActor(
		ACTOR_PILLAR_STAND_IN, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(
		ACTOR_CRACKED_PILLAR_SPRITE, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(
		ACTOR_WHOLE_PILLAR_SPRITE, MainRoutineStateWork.leaderActorId);
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
		Uint32		  id = list[0];
		ACTOR		 *actor = fdActorGet(id);
		Sint32		  flg = list[1];
		Sint32		  next = flg + 1;
		MAP_METATILE *metatiles = map->layers[actor->layer].metatiles;
		Uint8		 *shapes =
			grid + (metatiles - (MAP_METATILE *)Ram_MapMetatileBuffer);

		actor->retainSprCache =
			ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
		if(fdScriptGetFlag(next)) {
			fdActorSetPos(id, 0, 0);
		} else {
			Sint32		  idx = fdActorGetMetatileIndex(actor);
			MAP_METATILE *metatile = metatiles + idx;
			Uint8		 *shape = shapes + idx;
			Sint32		  kind, ground;

			kind = fdMapGridGetTerrainKind(
				actor->layer, actor->x.fixed, actor->z.fixed);
			ground = fdMapGetTerrainHeight(
				actor->layer, actor->x.fixed, actor->z.fixed);

			fdActorDisableById(id);
			metatile->attributeB = MAP_METATILE_JUMP;
			*shape = fdMapShapeFindOrAdd(
				actor->layer, kind, METATILE_HEIGHT_STEPS(ground) + 4);
			if(fdScriptGetFlag(flg))
				fdActorCopyGraphics(id, ACTOR_CRACKED_PILLAR_SPRITE);
		}
		list += 2;
	}
	syWait(10);
}

#include "actor_take_place_of.h"

/* As scCrumblingPillarStep, in a room whose pillars share one sprite: the
   stand-in takes the pillar's place to crack or give way, and a cracked pillar
   takes the cracked picture. */
void scCrumblingPillarStepWithStandIn(Uint16 *list)
{
	ACTOR *leader;
	ACTOR *standIn;
	ACTOR *pillar;
	FIXED  pos[3];

	if(fdTryJumpForward() != 0)
		return;
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	standIn = fdActorGetById(ACTOR_PILLAR_STAND_IN);
	pos[0] = leader->x.fixed;
	pos[1] = leader->y.fixed;
	pos[2] = leader->z.fixed;
	pillar = fdActorFindAtMetatile(pos, 0);
	if(pillar == 0)
		return;
	while(list[0] != ACTOR_LIST_END) {
		Uint32 id = list[0];

		if(fdActorGetById(id) != pillar) {
			list += 2;
			continue;
		}
		{
			Sint32 flg = list[1];

			fdScriptStart();
			if(!syGameFlagGet(flg)) {
				fdActorSelectAnimation(standIn, 1);
				scActorTakePlaceOf(standIn, pillar);
				sySynch(1);
				sySndPlay(SE_CHEST_CLOSE);
				fdActorSetSprCol(
					fdActorGetById(ACTOR_PILLAR_STAND_IN), SPRITE_COLOR_WHITE);
				sySynch(2);
				fdActorSelectAnimation(standIn, 0);
				fdActorCopyGraphics(id, ACTOR_CRACKED_PILLAR_SPRITE);
				fdActorSetSprCol(
					fdActorGetById(ACTOR_PILLAR_STAND_IN), SPRITE_COLOR_NORMAL);
				sySynch(2);
				fdActorSetSprCol(
					fdActorGetById(ACTOR_PILLAR_STAND_IN), SPRITE_COLOR_WHITE);
				sySynch(4);
				fdActorSetSprCol(
					fdActorGetById(ACTOR_PILLAR_STAND_IN), SPRITE_COLOR_NORMAL);
				scDustScatter(0);
				scActorTakePlaceOf(pillar, standIn);
				sySynch(1);
				syGameFlagSet(flg);
			} else if(!syGameFlagGet(flg + 1)) {
				Uint8 *motionFlg;

				fdActorSelectAnimation(standIn, 0);
				scActorTakePlaceOf(standIn, pillar);
				sySynch(1);
				motionFlg = &leader->motionFlg;
				*motionFlg = 0;
				sySndPlay(SE_PUSH);
				fdMapGridSetShake(1, 4, 1);
				scDustScatter(0);
				sySynch(8);
				fdActorSelectAnimation(standIn, 2);
				fdLeaderSetMetatileShapeAt(-2, pillar->layer);
				scDustScatter(1);
				sySynch(16);
				fdLeaderSetMetatileShapeAt(-2, pillar->layer);
				sySynch(4);
				fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
				sySynch(8);
				*motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
				sySynch(5);
				fdActorSetPosAndResetMotion(standIn, 0, 0, 0);
				sySynch(2);
				sySndPlay(SE_THUMP);
				scDustBurst();
				sySynch(20);
				syGameFlagSet(flg + 1);
			}
			fdScriptEnd();
			return;
		}
	}
}
