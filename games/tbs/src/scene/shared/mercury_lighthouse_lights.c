/* Mercury Lighthouse and its entrance: the lights that carry the leader over
   the pool (mercury_lighthouse_pillar.c). */

#include "system.h"

/* Sets up the lights for a floor whose pool is the metatile behavior region,
   keeping their state for the pool's scripts. On an ordinary arrival the leader
   has no lights. When the party returns to where it was, as after a battle, the
   column of light over the leader and the light under them in the pool are made
   again. */
void scMercuryLighthouseRestoreLights(
	Sint32 region, MERCURY_LIGHTHOUSE_LIGHTS *lights)
{
	ACTOR			  *actor;
	ACTOR			  *light;
	SPR				  *spr;
	MAP_GRID_METATILE *metatile;
	FIXED			   z;
	Sint32			   flg;

	*(MERCURY_LIGHTHOUSE_LIGHTS **)syHeapSlotAllocEWRAM(
		HEAP_SLOT_MERCURY_LIGHTHOUSE_LIGHTS, sizeof(lights)) = lights;
	flg = gmFlagGet(FLAG_KEEP_PARTY_POSITION);
	if(flg == 0) {
		DMA_FILL32(flg, lights, sizeof(*lights));
		lights->poolBehavior = region;
		return;
	}
	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	z = actor->z.fixed;
	metatile = (MAP_GRID_METATILE *)Ram_MapGridMetatileBuffer +
		(z / toFIXED(16)) * MAPGRID_SIZE + actor->x.fixed / toFIXED(16);
	if(lights->levels != 0 && lights->pillar != 0) {
		light = syFieldActorCreate(SPRITE_GOLDEN_RINGS, actor->x.fixed,
			actor->y.fixed + toFIXED(24), z);
		if(light != NULL) {
			light->groundHeight = actor->groundHeight;
			spr = light->spr;
			fdActorStartScript(light, MercuryLighthousePillarScript);
			light->watchedActor = (ACTOR_OR_EFFECT *)actor;
			light->motionFlg = ACTOR_FLOATS;
			light->y.fixed -= toFIXED(1) / 2;
			if(spr != NULL) {
				fdSprStartAnimation(
					spr, GOLDEN_RINGS_ANIMATION_COLUMN(lights->levels));
				spr->flg = 0;
				spr->image.pri = 1;
			}
			lights->pillar = light;
		}
	} else {
		lights->pillar = 0;
	}
	if(metatile->behavior == region && lights->stepEff != 0) {
		light = syFieldActorCreate(SPRITE_GOLDEN_RINGS, actor->x.fixed,
			actor->y.fixed, actor->z.fixed);
		if(light == NULL)
			return;
		light->groundHeight = actor->groundHeight;
		spr = light->spr;
		fdActorStartScript(light, MercuryLighthouseGrowLightScript);
		light->motionFlg = 0;
		light->tmr = 0;
		light->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		light->speedLimit = toFIXED(4);
		if(spr != NULL) {
			fdSprStartAnimation(spr, GOLDEN_RINGS_ANIMATION_POOL_LIGHT);
			spr->flg = 0;
		}
		lights->stepEff = light;
	} else {
		lights->stepEff = 0;
	}
}
