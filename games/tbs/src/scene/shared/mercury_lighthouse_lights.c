/* @file */
/* Mercury Lighthouse and its entrance: the lights that carry the leader over */
/* the pool (mercury_lighthouse_pillar.c). */

#include "system.h"

/* Sets up the lights for a floor whose pool is the cell behavior region, */
/* keeping their state for the pool's events. On an ordinary arrival the */
/* leader has no lights. When the party returns to where it was, as after a */
/* battle, the column of light over the leader and the light under them in */
/* the pool are made again. */
void scMercuryLighthouseRestoreLights(s32 region,
									  MERCURY_LIGHTHOUSE_LIGHTS *lights) {
	ACTOR *actor;
	ACTOR *light;
	SPR *sprite;
	MAP_GRID_CELL *cell;
	FIXED z;
	s32 flag;

	*(MERCURY_LIGHTHOUSE_LIGHTS **)syHeapSlotAllocEwram(
		HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS, sizeof(lights)) = lights;
	flag = gmFlagGet(FLAG_KEEP_PARTY_POSITION);
	if(flag == 0) {
		btDmaFill32(flag, lights, sizeof(*lights));
		lights->poolBehavior = region;
		return;
	}
	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	z = actor->z.fixed;
	cell = (MAP_GRID_CELL *)Ram_MapGridCellBuffer +
		   (z / toFIXED(16)) * MAPGRID_SIZE + actor->x.fixed / toFIXED(16);
	if(lights->levels != 0 && lights->pillar != 0) {
		light = syFieldActorCreate(SPRITE_GOLDEN_RINGS, actor->x.fixed,
								   actor->y.fixed + toFIXED(24), z);
		if(light != NULL) {
			light->groundHeight = actor->groundHeight;
			sprite = light->spr;
			fdActorStartScript(light, MercuryLighthouse_PillarScript);
			light->watchedActor = (ACTOR_OR_EFFECT *)actor;
			light->motionFlg = ACTOR_FLOATS;
			light->y.fixed -= toFIXED(1) / 2;
			if(sprite != NULL) {
				fdSprStartAnimation(
					sprite, GOLDEN_RINGS_ANIMATION_COLUMN(lights->levels));
				sprite->flg = 0;
				sprite->image.pri = 1;
			}
			lights->pillar = light;
		}
	}
	else {
		lights->pillar = 0;
	}
	if(cell->behavior == region && lights->stepEff != 0) {
		light = syFieldActorCreate(SPRITE_GOLDEN_RINGS, actor->x.fixed,
								   actor->y.fixed, actor->z.fixed);
		if(light == NULL)
			return;
		light->groundHeight = actor->groundHeight;
		sprite = light->spr;
		fdActorStartScript(light, MercuryLighthouse_GrowLightScript);
		light->motionFlg = 0;
		light->tmr = 0;
		light->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		light->speedLimit = toFIXED(4);
		if(sprite != NULL) {
			fdSprStartAnimation(sprite, GOLDEN_RINGS_ANIMATION_POOL_LIGHT);
			sprite->flg = 0;
		}
		lights->stepEff = light;
	}
	else {
		lights->stepEff = 0;
	}
}
