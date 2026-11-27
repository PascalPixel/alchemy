/* Catch: a hand reaches out from the caster, grabs what is ahead and takes it
   off the map. Part of script_commands.c. */

#include "script_commands_internal.h"

/* Runs Catch cast on a target, through fdPsynergyCatch. */
void fdPsynergyCatchOnTarget(Sint32 unused)
{
	fdPsynergyCatch();
}

/* A hand (SPRITE_MOVE_HAND) reaches from 16 pixels above the caster to the
   target over GROW_STEPS ticks, growing from a quarter to full size, clenches
   and presses down 20 pixels, taking what it caught off the map, then rises
   19.5 pixels and shrinks back into the caster. */
void fdPsynergyCatch(void)
{
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	ACTOR			   *caster = effectState->caster;
	ACTOR			   *caught = effectState->tgt;
	ACTOR			   *hand;
	POS					spawn;
	POS					origin;
	POS					tgt;
	Sint32				step;
	Sint32				i;
	Sint32				steps = GROW_STEPS + 1;

	origin.x = caster->x.fixed;
	origin.y = caster->y.fixed + toFIXED(16);
	origin.z = caster->z.fixed;
	if(effectState->aimAhead) {
		tgt.x = caster->x.fixed;
		tgt.y = caster->y.fixed + toFIXED(32);
		tgt.z = caster->z.fixed;
		syVectorAddPolarOffset(toFIXED(32), effectState->ang, &tgt.x);
	} else {
		tgt.x = effectState->x;
		tgt.y = effectState->y + toFIXED(32);
		tgt.z = effectState->z;
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

	/* Reach out to the target, a step of the way each tick. */
	step = 0;
	do {
		Sint32 scale;

		SET_ACTOR_POSITION(hand,
			origin.x + (tgt.x - origin.x) * step / GROW_STEPS,
			origin.y + (tgt.y - origin.y) * step / GROW_STEPS,
			origin.z + (tgt.z - origin.z) * step / GROW_STEPS);
		scale = fdEffectInterpolate(toFIXED(1), toFIXED(1) / 4, step);
		hand->scaleX = scale;
		hand->scaleY = scale;
		sySynch(1);
		step++;
	} while(step < steps);
	sySynch(10);
	fdActorSelectAnimationStub(hand, MOVE_HAND_ANIMATION_CLENCH);
	sySynch(15);

	/* Press down 2 pixels a tick for 10 ticks. */
	for(i = 0; i < 10; i++) {
		hand->y.fixed -= toFIXED(2);
		sySynch(1);
	}
	fdActorSelectAnimationStub(hand, MOVE_HAND_ANIMATION_FIST);
	sySndPlayStub(SE_WATER);
	/* As the hand presses down, the caught actor vanishes: (-9, -9) pixels is
	   just off the map's corner, where no view reaches. */
	if(caught != NULL)
		fdActorSetPosAndResetMotionStub(
			caught, -toFIXED(9), caught->y.fixed, -toFIXED(9));
	sySynch(20);

	/* Rise 1.5 pixels a tick for 13 ticks. */
	for(i = 0; i < 13; i++) {
		hand->y.fixed += toFIXED(3) / 2;
		sySynch(1);
	}
	sySynch(10);
	sySndPlayStub(SE_MENU_ERROR);

	/* Shrink back to the caster. */
	step = 0;
	do {
		Sint32 scale;

		SET_ACTOR_POSITION(hand, tgt.x + (origin.x - tgt.x) * step / GROW_STEPS,
			tgt.y + (origin.y - tgt.y) * step / GROW_STEPS,
			tgt.z + (origin.z - tgt.z) * step / GROW_STEPS);
		scale = fdEffectInterpolate(toFIXED(1) / 4, toFIXED(1), step);
		hand->scaleX = scale;
		hand->scaleY = scale;
		sySynch(1);
		step++;
	} while(step < steps);
	fdActorReleaseStub(hand);
	fdPsynergyFadeFromCastCol();
}
