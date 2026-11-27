/* @file */
/* Catch: a hand reaches out from the caster, grabs what is ahead and takes */
/* it off the map. */
/* Part of event_commands.c. */

#include "event_commands_internal.h"

/* Runs Catch cast on a target, through fdPsynergyCatch. */
void fdPsynergyCatchOnTarget(s32 unused) { fdPsynergyCatch(); }

/* A hand (SPRITE_MOVE_HAND) reaches from 16 pixels above the caster to the */
/* target over GROW_STEPS frames, growing from a quarter to full size, clenches
 */
/* and presses down 20 pixels, taking what it caught off the map, then rises */
/* 19.5 pixels and shrinks back into the caster. */
void fdPsynergyCatch(void) {
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	ACTOR *caster = effectState->caster;
	ACTOR *caught = effectState->tgt;
	ACTOR *hand;
	POS spawn;
	POS origin;
	POS target;
	s32 step;
	s32 i;
	s32 steps = GROW_STEPS + 1;

	origin.x = caster->x.fixed;
	origin.y = caster->y.fixed + toFIXED(16);
	origin.z = caster->z.fixed;
	if(effectState->aimAhead) {
		target.x = caster->x.fixed;
		target.y = caster->y.fixed + toFIXED(32);
		target.z = caster->z.fixed;
		syVectorAddPolarOffset(toFIXED(32), effectState->ang, &target.x);
	}
	else {
		target.x = effectState->x;
		target.y = effectState->y + toFIXED(32);
		target.z = effectState->z;
	}
	spawn.x = effectState->x;
	spawn.y = effectState->y + toFIXED(32);
	spawn.z = effectState->z;
	hand = fdActorSpawn(SPRITE_MOVE_HAND, spawn.x, spawn.y, spawn.z);
	if(hand == NULL)
		return;
	fdPsynergyFadeToCastCol();
	sySndPlayStub(SE_ITEM_BREAK);
	hand->facing = caster->facing;
	hand->speedLimit = toFIXED(13) / 10;
	hand->motionFlg = 0;
	fdActorSelectAnimationStub(hand, MOVE_HAND_ANIMATION_FIST);
	fdActorSetSprColModeStub(hand, SPRITE_COLOR_GRB);

	/* Reach out to the target, a step of the way each frame. */
	step = 0;
	do {
		s32 scale;

		SET_ACTOR_POSITION(
			hand, origin.x + (target.x - origin.x) * step / GROW_STEPS,
			origin.y + (target.y - origin.y) * step / GROW_STEPS,
			origin.z + (target.z - origin.z) * step / GROW_STEPS);
		scale = fdEffectInterpolate(FIXED_ONE, FIXED_ONE / 4, step);
		hand->scaleX = scale;
		hand->scaleY = scale;
		sySynch(1);
		step++;
	} while(step < steps);
	sySynch(10);
	fdActorSelectAnimationStub(hand, MOVE_HAND_ANIMATION_CLENCH);
	sySynch(15);

	/* Press down 2 pixels a frame for 10 frames. */
	for(i = 0; i < 10; i++) {
		hand->y.fixed -= toFIXED(2);
		sySynch(1);
	}
	fdActorSelectAnimationStub(hand, MOVE_HAND_ANIMATION_FIST);
	sySndPlayStub(SE_WATER);
	/* As the hand presses down, the caught actor vanishes: (-9, -9) pixels is
	 */
	/* just off the map's corner, where no view reaches. */
	if(caught != NULL)
		fdActorSetPosAndResetMotionStub(caught, -toFIXED(9), caught->y.fixed,
										-toFIXED(9));
	sySynch(20);

	/* Rise 1.5 pixels a frame for 13 frames. */
	for(i = 0; i < 13; i++) {
		hand->y.fixed += toFIXED(3) / 2;
		sySynch(1);
	}
	sySynch(10);
	sySndPlayStub(SE_MENU_ERROR);

	/* Shrink back to the caster. */
	step = 0;
	do {
		s32 scale;

		SET_ACTOR_POSITION(
			hand, target.x + (origin.x - target.x) * step / GROW_STEPS,
			target.y + (origin.y - target.y) * step / GROW_STEPS,
			target.z + (origin.z - target.z) * step / GROW_STEPS);
		scale = fdEffectInterpolate(FIXED_ONE / 4, FIXED_ONE, step);
		hand->scaleX = scale;
		hand->scaleY = scale;
		sySynch(1);
		step++;
	} while(step < steps);
	fdActorReleaseStub(hand);
	fdPsynergyFadeFromCastCol();
}
