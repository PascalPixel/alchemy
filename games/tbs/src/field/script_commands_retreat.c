/* Retreat, the caster spinning away in a spray of sparks, and the golden rings
   that fall round the caster of Avoid. Part of script_commands.c. */

#include "script_commands_internal.h"

/* Per-frame update: spins by FACING_STEP + timer * 80 a frame, speeding up
   (timer counts up) until timer * 80 reaches FACING_STEP: from 1/16 turn a
   frame to just over 1/8 in 52 frames, as fdEffectAdvanceAngWithRamp. */
void fdEffectUpdateSpinAng(ACTOR *actor)
{
	Uint32 step;

	step = actor->tmr * 80;
	actor->facing = actor->facing + step + FACING_STEP;
	if(step < FACING_STEP)
		actor->tmr = actor->tmr + 1;
}

/* Per-frame update: spins 1/8 turn a frame as it shrinks by 5/512 of its full
   size, rising a pixel a frame; once its scale falls below 3/16 (84 frames from
   full size) its sprites are hidden, but it is not released. Retreat gives it
   to the caster. */
void fdEffectUpdateRetreatCaster(ACTOR *actor)
{
	Uint32 scale;

	actor->scaleY -= toFIXED(5) / 512;
	scale = actor->scaleX - toFIXED(5) / 512;
	actor->facing += DEGtoANG(45);
	actor->y.fixed += toFIXED(1);
	actor->scaleX = scale;
	if((Sint32)scale < toFIXED(3) / 16)
		actor->sprStorageKind = ACTOR_SPRITE_NONE;
}

/* The states of a Retreat particle (fdRetreatUpdateSpark). */
typedef enum RETREAT_PARTICLE_STATE {
	RETREAT_PARTICLE_AIM = 0,	 /* placed on the main actor and aimed */
	RETREAT_PARTICLE_FLYING = 1, /* back to RETREAT_PARTICLE_AIM on arrival */
	/* Set by fdPsynergyRetreat: freed on arrival. */
	RETREAT_PARTICLE_LAST_FLIGHT = 2
} RETREAT_PARTICLE_STATE;

/* Field effect sprite callback: a particle flies 120 pixels from the main
   actor's screen position (8 pixels lower) in a direction within 1/8 turn of up
   the screen, at up to 5 pixels a frame, and starts again from the main actor
   on arrival until it takes its last flight. */
void fdRetreatUpdateSpark(FIELD_PARTICLE *effect)
{
	VECTOR3 pos;
	ACTOR  *main;
	Sint32	state;
	Uint32	spread;

	main = ((FIELD_EFFECT_STATE *)FieldEffectStateWork)->caster;
	state = effect->state;

	if(state == RETREAT_PARTICLE_AIM) {
		pos.x = main->x.fixed;
		pos.y = main->y.fixed;
		pos.z = main->z.fixed;
		fdCameraWorldToScreen(&pos);

		effect->x = pos.x;
		effect->z = pos.z + toFIXED(8);
		effect->originZ = effect->z;
		effect->originX = effect->x;
		pos.z = effect->originZ;
		pos.x = effect->originX;

		/* Up the screen, within 1/8 turn either way. */
		spread = syRandom();
		syVectorAddPolarOffset(toFIXED(120),
			FIXEDtoINT(spread * (TRIG_TURN / 8)) -
				syRandom16Below(TRIG_TURN / 8) + FACING_NORTH,
			(FIXED *)&pos);

		effect->tgtX = pos.x;
		effect->tgtZ = pos.z;
		effect->acceleration = toFIXED(5);
		effect->speedMax = toFIXED(5);
		effect->turnStepLimited = state;
		effect->state++;

		if((FrameCount & 2) != 0)
			sySndPlayStub(SE_GLITTER);
	} else if(state == RETREAT_PARTICLE_FLYING) {
		if(fdParticleHasPendingTarget(effect) == 0)
			effect->state--;
	} else if(state == RETREAT_PARTICLE_LAST_FLIGHT) {
		if(fdParticleHasPendingTarget(effect) == 0)
			fdEffectClearOwnedSlot(effect);
	}
}

/* Spins the main actor and shrinks it while 24 particles fly out, one a
   frame. */
void fdPsynergyRetreat(void)
{
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	ACTOR			   *actor = effectState->caster;
	VECTOR3				pos;
	FIELD_PARTICLE	   *effect;
	Uint32				i;

	fdScriptStart();
	fdCameraMoveWithinMapBounds(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdPsynergyFadeToCastCol();
	sySynch(10);
	fdActorFaceDirectionById(effectState->casterId, FACING_SOUTH, 0);
	sySynch(30);
	actor->update = fdActorCycleSprCols;
	sySndPlayStub(SE_PSYNERGY_SPARKLE);
	fdActorSelectAnimationStub(actor, ANIM_LEADER_ARMS_RAISED);
	sySynch(40);

	/* Start spinning. */
	sySndPlayStub(SE_SHIMMER);
	fdActorSetSprColModeStub(actor, SPRITE_COLOR_NORMAL);
	fdActorSelectAnimationStub(actor, ANIM_NOD);
	actor->update = fdEffectUpdateSpinAng;
	actor->tmr = 0;
	sySynch(70);

	/* Start shrinking, and send particles from where it is on screen. */
	fdActorSetSprFlagsStub(actor, 0);
	actor->motionFlg = 0;
	actor->update = fdEffectUpdateRetreatCaster;
	actor->tgtX = ACTOR_NO_TARGET;
	pos.x = actor->x.fixed;
	pos.y = actor->y.fixed;
	pos.z = actor->z.fixed;
	fdCameraWorldToScreen(&pos);
	i = 0;
	effect = effectState->particles;
	do {
		FIXED scale;
		fdParticleInit(effect, SPRITE_PARTICLE, pos.x, pos.z);
		fdParticleSetCallback(effect, fdRetreatUpdateSpark);
		fdParticleSetAnimation(effect, PARTICLE_ANIMATION_FLICKER);
		fdSprSetAnimationColModesStub(effect->spr, syRandom16Below(7));
		scale = (syRandom() >> 1) + toFIXED(6) / 5; /* 1.2 to 1.7 */
		effect->scaleY = scale;
		effect->scaleX = scale;
		i++;
		sySynch(1);
		effect++;
	} while(i < PSYNERGY_EFFECT_SLOT_COUNT);
	sySynch(70);

	/* The flying particles vanish once they arrive. */
	for(i = 0; i < PSYNERGY_EFFECT_SLOT_COUNT; i++) {
		if(effectState->particles[i].active != 0)
			effectState->particles[i].state = RETREAT_PARTICLE_LAST_FLIGHT;
	}
	sySynch(40);
	fdPsynergyFadeFromCastCol();
	sySynch(10);
}

/* Per-frame update of a falling arc: grows toward full size (3/64 a frame) as
   it falls 2 pixels a frame on the actor it follows, 9 pixels in front of it
   and up to 5 pixels further while small; released 10 pixels above the ground
   (4 on the World Map, MAP_INDEX_WORLD). */
void fdEffectUpdateDescendingParticlePositiveArc(ACTOR *particle)
{
	ACTOR *source;
	FIXED  y;
	FIXED  limit;
	FIXED  scale;

	limit = particle->groundHeight + toFIXED(10);
	source = (ACTOR *)particle->watchedActor;
	if(MainRoutineStateWork.mapId == MAP_INDEX_WORLD)
		limit = particle->groundHeight + toFIXED(4);

	y = particle->y.fixed;
	if(y <= limit) {
		fdActorReleaseStub(particle);
		return;
	}

	scale = particle->scaleX + toFIXED(3) / 64;
	if(scale > toFIXED(1))
		scale = toFIXED(1);
	particle->scaleX = scale;
	particle->scaleY = scale;
	particle->x.fixed = source->x.fixed;
	particle->y.fixed = y - toFIXED(2);
	particle->z.fixed =
		source->z.fixed + ((toFIXED(1) - scale) * 5) + toFIXED(9);
}

/* As fdEffectUpdateDescendingParticlePositiveArc for the arc drawn upside down,
   16 pixels in front of the actor and up to 5 pixels nearer. */
void fdEffectUpdateDescendingParticleNegativeArc(ACTOR *particle)
{
	ACTOR *source;
	FIXED  y;
	FIXED  limit;
	FIXED  scale;

	limit = particle->groundHeight + toFIXED(10);
	source = (ACTOR *)particle->watchedActor;
	if(MainRoutineStateWork.mapId == MAP_INDEX_WORLD)
		limit = particle->groundHeight + toFIXED(4);

	y = particle->y.fixed;
	if(y <= limit) {
		fdActorReleaseStub(particle);
		return;
	}

	scale = particle->scaleX + toFIXED(3) / 64;
	if(scale > toFIXED(1))
		scale = toFIXED(1);
	particle->scaleX = scale;
	particle->scaleY = -scale;
	particle->x.fixed = source->x.fixed;
	particle->y.fixed = y - toFIXED(2);
	particle->z.fixed =
		source->z.fixed - ((toFIXED(1) - scale) * 5) + toFIXED(16);
}

/* Avoid's rings: a pair of falling arcs (one upside down) 64 pixels above
   source, at 0.4 of full size, drawn with the tiles in
   effectState->arcTileSlot. */
void fdEffectSpawnDescendingArcParticles(ACTOR *source)
{
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	ACTOR			   *leader = effectState->caster;
	ACTOR			   *actors[2];
	ACTOR			   *actor;
	SPR				   *sprite;
	Sint32				arcIndex;

	for(arcIndex = 0; arcIndex <= 1; arcIndex++) {
		actor = fdActorCreateStub(SPRITE_GOLDEN_RINGS, source->x.fixed,
			source->y.fixed + toFIXED(64), source->z.fixed);
		actors[arcIndex] = actor;
		if(actor == NULL)
			continue;
		actor->groundHeight = source->groundHeight;
		sprite = actor->spr;
		actor->motionFlg = 0;
		actor->tmr = 0;
		actor->watchedActor = (union ACTOR_OR_EFFECT *)source;
		actor->scaleY = toFIXED(2) / 5;
		actor->scaleX = toFIXED(2) / 5;
		if(sprite == NULL)
			continue;
		fdSprStartAnimationStub(sprite, 0);
		sprite->flg = 0;
		syVRAMCacheFreeEntry(sprite->slot);
		sprite->slot = effectState->arcTileSlot;
		sprite->retainCacheEntry = 1;
		sprite->image.tile = VramCacheEntries[sprite->slot].ofs >> 5;
		/* In 16 colours, 32 by 16 pixels. */
		sprite->image.fullCol = 0;
		sprite->image.shape = RING_HALF_SHAPE;
		sprite->image.size = RING_HALF_SIZE;
		sprite->entries[0]->picture = 0;
	}
	/* BUG: failed allocations above leave a missing arc or sprite here. */
	actors[0]->update = fdEffectUpdateDescendingParticleNegativeArc;
	actors[0]->spr->image.pri = 0;
	actors[1]->update = fdEffectUpdateDescendingParticlePositiveArc;
	actors[1]->spr->image.pri = leader->spr->image.pri;
}
