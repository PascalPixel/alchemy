#include "pillar_stand_in.h"

/* Unused: as scMagmaRockInsideRestoreCrumblingPillars, in a room whose pillars
 */
/* share one sprite, as Dehkan Plateau's syCrumblingPillarPlaceWithStandIn */
/* does: first loads the stand-in's and the two pictures' sprites beside the */
/* leader, the cracked one showing its cracks, then puts them out of sight; a */
/* cracked pillar takes the cracked picture. */
UNUSED void scMagmaRockInsideRestorePillars(u16 *list) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	ACTOR *actor;
	s32 id;
	s32 flag;
	s32 nextFlag;
	MAP_CELL *cells;
	u8 *shapes;
	MAP_CELL *cell;
	s32 index;
	u8 *grid = MapShapeGrid;

	fdActorPlaceOnActor(ACTOR_PILLAR_STAND_IN,
						MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_CRACKED_PILLAR_SPRITE,
						MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_WHOLE_PILLAR_SPRITE,
						MainRoutineStateWork.leaderActorId);
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
		flag = list[1];
		nextFlag = flag + 1;
		cells = map->layers[actor->layer].cells;
		shapes = grid + (cells - (MAP_CELL *)Ram_MapCellBuffer);
		actor->retainSprCache =
			ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
		if(fdEventGetFlag(nextFlag)) {
			fdActorSetPos(id, 0, 0);
		}
		else {
			s32 shape;
			u8 *slot;
			s32 height;

			index = fdActorGetCellIndex(actor);
			cell = &cells[index];
			slot = shapes + index;
			shape = fdMapGridGetTerrainKind(actor->layer, actor->x.fixed,
											actor->z.fixed);
			height = fdMapGetTerrainHeight(actor->layer, actor->x.fixed,
										   actor->z.fixed);
			fdActorDisableById(id);
			cell->attributeB = MAP_CELL_JUMP;
			*slot = fdMapShapeFindOrAdd(actor->layer, shape,
										CELL_HEIGHT_STEPS(height) + 4);
			if(fdEventGetFlag(flag))
				fdActorCopyGraphics(id, ACTOR_CRACKED_PILLAR_SPRITE);
		}
		list += 2;
	}
	syWait(10);
}
