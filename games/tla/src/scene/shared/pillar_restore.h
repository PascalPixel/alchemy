#include "pillar_stand_in.h"

/* Unused: as scMagmaRockInsideRestoreCrumblingPillars, in a room whose pillars
   share one sprite, as Dehkan Plateau's scCrumblingPillarPlaceWithStandIn does:
   first loads the stand-in's and the two pictures' sprites beside the leader,
   the cracked one showing its cracks, then puts them out of sight; a cracked
   pillar takes the cracked picture. */
UNUSED void scMagmaRockInsideRestorePillars(Uint16 *list)
{
	MAP_STATE	 *map = RamHeapSlots->mapGrid;
	ACTOR		 *actor;
	Sint32		  id;
	Sint32		  flg;
	Sint32		  nextFlg;
	MAP_METATILE *metatiles;
	Uint8		 *shapes;
	MAP_METATILE *metatile;
	Sint32		  idx;
	Uint8		 *grid = MapShapeGrid;

	fdActorPlaceOnActor(
		ACTOR_PILLAR_STAND_IN, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(
		ACTOR_CRACKED_PILLAR_SPRITE, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(
		ACTOR_WHOLE_PILLAR_SPRITE, MainRoutineStateWork.leaderActorId);
	syWait(1);
	scPillarTakeOutOfPlay(ACTOR_PILLAR_STAND_IN);
	scPillarTakeOutOfPlay(ACTOR_CRACKED_PILLAR_SPRITE);
	scPillarTakeOutOfPlay(ACTOR_WHOLE_PILLAR_SPRITE);
	fdActorSetAnimation(ACTOR_CRACKED_PILLAR_SPRITE, 0);
	syWait(1);
	fdActorSetPos(ACTOR_PILLAR_STAND_IN, 0, 0);
	fdActorSetPos(ACTOR_CRACKED_PILLAR_SPRITE, 0, 0);
	fdActorSetPos(ACTOR_WHOLE_PILLAR_SPRITE, 0, 0);
	syWait(1);
	while(*list != ACTOR_LIST_END) {
		id = list[0];
		actor = fdActorGet(id);
		flg = list[1];
		nextFlg = flg + 1;
		metatiles = map->layers[actor->layer].metatiles;
		shapes = grid + (metatiles - (MAP_METATILE *)RamMapMetatileBuffer);
		actor->retainSprCache =
			ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
		if(fdScriptGetFlag(nextFlg)) {
			fdActorSetPos(id, 0, 0);
		} else {
			Sint32 shape;
			Uint8 *slot;
			Sint32 height;

			idx = fdActorGetMetatileIndex(actor);
			metatile = &metatiles[idx];
			slot = shapes + idx;
			shape = fdMapGridGetTerrainKind(
				actor->layer, actor->x.fixed, actor->z.fixed);
			height = fdMapGetTerrainHeight(
				actor->layer, actor->x.fixed, actor->z.fixed);
			fdActorDisableById(id);
			metatile->attributeB = MAP_METATILE_JUMP;
			*slot = fdMapShapeFindOrAdd(
				actor->layer, shape, METATILE_HEIGHT_STEPS(height) + 4);
			if(fdScriptGetFlag(flg))
				fdActorCopyGraphics(id, ACTOR_CRACKED_PILLAR_SPRITE);
		}
		list += 2;
	}
	syWait(10);
}
