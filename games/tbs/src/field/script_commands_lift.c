/* Lift: two hands grip the target and raise it, and Force's flashes and heavy
   impact, linked beside it. Part of script_commands.c. */

#include "script_commands_internal.h"

/* A hand appears 32 pixels to either side, each moves 14 pixels in, and they
   lift the target 32 pixels with them. */
void fdPsynergyLiftOnTarget(Sint32 unused)
{
	FIELD_EFFECT_STATE *effectState;
	ACTOR			   *caster;
	ACTOR			   *target;
	void			   *hands[2]; /* right then left */
	FIXED				pos[3];
	Sint32				x;
	FIXED				y;
	Sint32				z;
	Sint32				handIndex;
	FIXED				startY;

	effectState = FieldEffectStateWork;
	target = effectState->target;
	caster = effectState->caster;
	if(target == NULL)
		return;

	fdPsynergyFadeToCastColor();
	/* The caster faces its target while Lift holds it. */
	caster->watchedActor = (union ACTOR_OR_EFFECT *)target;
	fdActorStartScriptStub(caster, FieldEffectSourceHoldScript);

	x = effectState->x;
	pos[0] = x;
	y = effectState->y + toFIXED(16);
	pos[1] = y;
	z = effectState->z;
	pos[2] = z;
	hands[0] = fdEffectSpawnLiftHand(x + toFIXED(32), y, z, FACING_WEST);
	hands[1] = fdEffectSpawnLiftHand(
		pos[0] - toFIXED(32), pos[1], pos[2], FACING_EAST);

	/* Each hand faces the target (FACING_WEST on the right, FACING_EAST on the
	   left) and moves 14 pixels that way. */
	sySynch(15);
	for(handIndex = 0; handIndex < 2; handIndex++) {
		ACTOR *hand = hands[handIndex];
		if(hand != NULL)
			fdActorSetTargetPosFromMagnitudeAngle(
				hand, toFIXED(14), hand->facing);
	}

	fdActorWaitForMoveStub(hands[0]);
	target->update = fdActorCycleSpriteColors;
	sySoundPlayStub(SE_PSYNERGY_START);
	target->motionFlags = ACTOR_FLOATS;

	/* Lift 32 pixels, a quarter pixel at a time. */
	startY = target->y.fixed;
	if(hands[0] != NULL && hands[1] != NULL) {
		while(target->y.fixed <= startY + toFIXED(32)) {
			((ACTOR *)hands[0])->y.fixed += toFIXED(1) / 4;
			((ACTOR *)hands[1])->y.fixed += toFIXED(1) / 4;
			target->y.fixed += toFIXED(1) / 4;
			sySynch(1);
		}
	}

	/* The hands burst into particles; the target stays up. */
	fdEffectBurstHand(hands[0]);
	fdEffectBurstHand(hands[1]);
	fdPsynergyFadeFromCastColor();
}

/* Two hands appear 16 pixels above the spot in front of the caster, 32 pixels
   to either side, then close in 24 pixels and clap, and sparkles scatter. */
DEFINE_CLAPPING_HANDS_CAST(fdPsynergyLift, fdEffectSpawnLiftHand)

/* Spawns a hand (SPRITE_MOVE_HAND) and grows it, spinning, from a quarter to
   full size over 24 ticks; angle is the way it faces once grown. */
ACTOR *fdEffectSpawnLiftHand(Sint32 x, Sint32 y, Sint32 z, Sint32 angle)
{
	ACTOR *hand;
	FIXED  scale;

	sySoundPlayStub(SE_ITEM_BREAK);
	hand = fdActorSpawn(SPRITE_MOVE_HAND, x, y, z);
	if(hand != NULL) {
		hand->scaleX = hand->scaleY = toFIXED(1) / 4;
		hand->acceleration = hand->speedLimit = toFIXED(3);
		((SPR *)hand->sprite)->image.priority = 0;
		fdActorSelectAnimationStub(hand, MOVE_HAND_ANIMATION_GRIP);
		scale = hand->scaleX;
		if(scale < toFIXED(1)) {
			do {
				scale += toFIXED(1) / 32;
				hand->scaleY = scale;
				hand->scaleX = scale;
				hand->facing += DEGtoANG(45);
				sySynch(1);
				scale = hand->scaleX;
			} while(scale < toFIXED(1));
		}
		hand->facing = angle;
	}
	return hand;
}

/* Field effect sprite callback of a particle that scatters 25 pixels from its
   origin in a random direction, then gathers in front of the caster and is
   freed: each of Force's FORCE_FLASHES flashes (script_commands_force.c). */
void fdEffectUpdateRadialBurst(FIELD_PARTICLE *effect)
{
	/* OUT_AND_BACK_FLYING_OUT must enter OUT_AND_BACK_AIM_BACK immediately. */
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	VECTOR3				pos;
	Sint32				state;

again:
	state = effect->state;
	if(state == OUT_AND_BACK_AIM_OUT) {
		/* 25 pixels from the origin, in a random direction. */
		pos.x = effect->originX;
		pos.z = effect->originZ;
		syVectorAddPolarOffset(toFIXED(25), (Uint16)syRandom(), (FIXED *)&pos);
		effect->targetX = pos.x;
		effect->targetZ = pos.z;
		effect->acceleration = toFIXED(3);
		effect->speedMax = toFIXED(3);
		effect->turnStepLimited = 0;
		effect->state++;
		return;
	}

	if(state == OUT_AND_BACK_FLYING_OUT) {
		/* On arrival, go straight on. */
		if(fdParticleHasPendingTarget(effect) == 0) {
			effect->state++;
			goto again;
		}
		return;
	}

	if(state == OUT_AND_BACK_AIM_BACK) {
		/* 8 pixels toward effectState->angle from 16 pixels above the caster,
		   on screen, give or take 4 pixels. */
		ACTOR *caster = effectState->caster;

		pos.x = caster->x.fixed;
		pos.y = caster->y.fixed + toFIXED(16);
		pos.z = caster->z.fixed;
		syVectorAddPolarOffset(toFIXED(8), effectState->angle, (FIXED *)&pos);
		fdCameraWorldToScreen(&pos);
		syVectorAddPolarOffset(toFIXED(4), syRandom(), (FIXED *)&pos);
		effect->targetX = pos.x;
		effect->targetZ = pos.z;
		effect->turnStepMax = TRIG_TURN / 32;
		effect->turnStepLimited = 1;
		effect->state++;
		return;
	}

	if(state == OUT_AND_BACK_FLYING_BACK &&
		fdParticleHasPendingTarget(effect) == 0)
		fdEffectClearOwnedSlot(effect);
}

#define HEAVY_IMPACT_SPARKLES 12

/* A heavy impact: places a star burst (SPRITE_STAR_BURST) 32 pixels below the
   source for 20 ticks and scatters HEAVY_IMPACT_SPARKLES sparkles
   (SPRITE_SPARKLE) from it. */
Sint32 fdEffectSpawnHeavyImpact(ACTOR *src)
{
	VECTOR3 pos;
	ACTOR  *impact;
	ACTOR  *sparkle;
	Sint32	sparkleIndex;

	sySoundPlayStub(SE_HEAVY_IMPACT);
	pos.x = src->x.fixed;
	pos.y = src->y.fixed;
	pos.z = src->z.fixed;
	impact = fdActorSpawn(SPRITE_STAR_BURST, pos.x, pos.y - toFIXED(32), pos.z);
	if(impact != NULL) {
		impact->motionFlags = 0;
		impact->wait = 20;
		fdActorStartScriptStub(impact, FieldEffectSharedParticleScript);
	}

	/* Sparkles: 1 to 2 pixels a tick, 8 to 32 pixels in a random direction. */
	for(sparkleIndex = 0; sparkleIndex < HEAVY_IMPACT_SPARKLES;
		sparkleIndex++) {
		sparkle = fdActorSpawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
		if(sparkle != NULL) {
			fdActorStartScriptStub(sparkle, FieldEffectFragmentScript);
			sparkle->speedLimit = syRandom() + toFIXED(1);
			sparkle->acceleration = toFIXED(1);
			sparkle->motionFlags = 0;
			fdActorSetTargetPosFromMagnitudeAngle(
				sparkle, syRandom() * 24 + toFIXED(8), syRandom());
		}
	}
	return 0;
}

#undef HEAVY_IMPACT_SPARKLES
