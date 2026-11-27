/* Douse: a rain cloud floats out over the target and rains on it, then shrinks
   back into the caster. Part of script_commands.c. */

#include "script_commands_internal.h"

/* The drops Douse rains: cast at nothing, cast on a target, and cast on a
   target in Mercury Lighthouse's entrance or middle floors
   (FIELD_EFFECT_STATE.lit), where larger blue drops sway down in their place
   and none splashes; elsewhere, as in Imil, the drops are small and white. XXX
   (no evidence): why those two floors take other drops; the code only tests the
   map, and a recording shows the difference, not its reason. */
#define DOUSE_DROPS					  10
#define DOUSE_DROPS_ON_TARGET		  24
#define DOUSE_SWAYING_DROPS_ON_TARGET 30

/* Per-tick update of a falling drop: on reaching its ground height it is
   released 2 ticks later and scatters three half-size splashes, which hop
   toward points 32 pixels away and are released after 6 ticks. */
void fdEffectUpdateDouseDrop(ACTOR *drop)
{
	Sint32 i;
	ACTOR *splash;

	if(drop->y.fixed <= drop->groundHeight) {
		drop->wait = 2;
		fdActorStartScriptStub(drop, FieldEffectSharedParticleScript);
		splash = NULL;
		drop->update = NULL;

		/* Half size, hopping up at a pixel a tick, toward a random point 32
		   pixels away. */
		for(i = 0; i <= 2; i++) {
			splash = fdActorSpawn(
				SPRITE_DROPLET, drop->x.fixed, drop->y.fixed, drop->z.fixed);
			if(splash == NULL) {
				break;
			}
			splash->scaleY = toFIXED(1) / 2;
			splash->scaleX = toFIXED(1) / 2;
			splash->motionFlg = ACTOR_FALLS;
			splash->velY = toFIXED(1);
			splash->speedLimit = syRandom() + toFIXED(6) / 5;
			fdActorSetTargetPosFromMagnitudeAng(
				splash, toFIXED(32), syRandom());
			splash->wait = 6;
			fdActorStartScriptStub(splash, FieldEffectSharedParticleScript);
		}
	}
}

/* Per-tick update: drifts from side to side, up to a pixel a tick, as it
   falls 0.3 pixels a tick, and is released on reaching its ground height. */
void fdEffectUpdateDriftingFallActor(ACTOR *particle)
{
	Sint32 drift;

	particle->y.fixed -= toFIXED(3) / 10;
	drift = syRandom();
	particle->x.fixed += drift - syRandom();
	if(particle->y.fixed <= particle->groundHeight) {
		fdActorStartScriptStub(particle, FieldEffectSharedParticleScript);
	}
}

/* Runs Douse cast on a target, through fdPsynergyDouse. */
void fdPsynergyDouseOnTarget(Sint32 unused)
{
	fdPsynergyDouse();
}

/* A rain cloud rises from 16 pixels above the caster to 32 pixels above the
   destination (80 with sceneVariant) over GROW_STEPS ticks, growing as it
   goes, rains drops, and returns. */
void fdPsynergyDouse(void)
{
	VECTOR3				pos;
	VECTOR3				start;
	VECTOR3				end;
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	ACTOR			   *caster = effectState->caster;
	ACTOR			   *cloud;
	Sint32				step;
	Sint32				steps = GROW_STEPS + 1;
	Sint32				cnt;
	Sint32				y;
	ACTOR			   *particle;

	cloud = fdActorSpawn(SPRITE_RAIN_CLOUD, step = 0, 0, 0);
	if(cloud == 0)
		return;
	fdPsynergyFadeToCastCol();
	sySndPlayStub(SE_ITEM_BREAK);
	/* No target: 16 pixels in front of the caster, at the ground's height. */
	if(effectState->tgt == 0) {
		effectState->x = caster->x.fixed;
		effectState->z = caster->z.fixed;
		syVectorAddPolarOffset(
			toFIXED(16), effectState->ang, (FIXED *)&effectState->x);
		effectState->y = fdMapGridGetTerrainHeightStub(
			MAPGRID_LAYER_GROUND, effectState->x, effectState->z);
	}
	SET_POSITION(&start, caster->x.fixed, caster->y.fixed + toFIXED(16),
		caster->z.fixed);
	end.x = effectState->x;
	y = effectState->y;
	end.y = y + toFIXED(32);
	end.z = effectState->z;
	if(effectState->sceneVariant != 0)
		end.y = y + toFIXED(80);

	/* Grow from a quarter to full size on the way. */
	for(; step < steps; step++) {
		Sint32 scale;

		SET_ACTOR_POSITION(cloud,
			start.x + (end.x - start.x) * step / GROW_STEPS,
			start.y + (end.y - start.y) * step / GROW_STEPS,
			start.z + (end.z - start.z) * step / GROW_STEPS);
		scale = fdEffectInterpolate(toFIXED(1), toFIXED(1) / 4, step);
		cloud->scaleX = scale;
		cloud->scaleY = scale;
		sySynch(1);
	}
	sySynch(10);

	if((Sint8)effectState->lit == 0) {
		/* A drop every 6 ticks from 3 to 8 pixels round the cloud (the last
		   right under it); aimAhead is cleared once a target is dispatched. */
		cnt = DOUSE_DROPS;
		if(effectState->aimAhead == 0)
			cnt = DOUSE_DROPS_ON_TARGET;
		for(step = 0; step < cnt; step++) {
			pos.x = cloud->x.fixed;
			pos.y = cloud->y.fixed;
			pos.z = cloud->z.fixed;
			syVectorAddPolarOffset(
				syRandom() * 5 + toFIXED(3), syRandom(), (FIXED *)&pos);
			if(step == cnt - 1) {
				sySynch(25);
				pos.x = cloud->x.fixed;
				pos.y = cloud->y.fixed;
				pos.z = cloud->z.fixed;
			}
			particle = fdActorSpawn(SPRITE_DROPLET, pos.x, pos.y, pos.z);
			if(particle != 0) {
				particle->groundHeight = pos.y - toFIXED(32);
				particle->update = fdEffectUpdateDouseDrop;
				particle->motionFlg = ACTOR_FALLS;
			}
			sySndPlayStub(SE_WATER);
			sySynch(6);
		}
		sySynch(10);
	} else {
		/* Every 6 ticks, a blue drop sways down from 3 to 8 pixels round the
		   cloud. */
		if(effectState->aimAhead != 0)
			cnt = DOUSE_DROPS;
		else
			cnt = DOUSE_SWAYING_DROPS_ON_TARGET;
		for(step = 0; step < cnt; step++) {
			pos.x = cloud->x.fixed;
			pos.y = cloud->y.fixed;
			pos.z = cloud->z.fixed;
			syVectorAddPolarOffset(
				syRandom() * 5 + toFIXED(3), syRandom(), (FIXED *)&pos);
			particle = fdActorSpawn(SPRITE_PARTICLE, pos.x, pos.y, pos.z);
			if(particle != 0) {
				particle->update = fdEffectUpdateDriftingFallActor;
				particle->motionFlg = 0;
				particle->spr->image.pri = 2;
				fdActorSelectAnimationStub(particle, 8);
				fdActorSetSprColModeStub(particle, SPRITE_COLOR_WHITE);
			}
			sySynch(6);
		}
		sySynch(70);
	}

	/* Shrink back to a quarter on the way back to the caster. */
	step = 0;
	do {
		Sint32 scale;

		SET_ACTOR_POSITION(cloud, end.x + (start.x - end.x) * step / GROW_STEPS,
			end.y + (start.y - end.y) * step / GROW_STEPS,
			end.z + (start.z - end.z) * step / GROW_STEPS);
		scale = fdEffectInterpolate(toFIXED(1) / 4, toFIXED(1), step);
		cloud->scaleX = scale;
		cloud->scaleY = scale;
		sySynch(1);
		step++;
	} while(step < steps);
	fdActorReleaseStub(cloud);
	fdPsynergyFadeFromCastCol();
}

#undef DOUSE_DROPS
#undef DOUSE_DROPS_ON_TARGET
#undef DOUSE_SWAYING_DROPS_ON_TARGET
