/* @file */
/* Carry: two hands lift the target, move it with the d-pad and drop it. */
/* Part of event_commands.c. */

#include "event_commands_internal.h"

STATIC_ASSERT(sizeof(VECTOR3) == 12);

/* Task while Carry holds its target: drops one particle a */
/* frame round the lifted actor, 8 to 24 pixels above it and 0 to 48 pixels */
/* away in a random direction (fdEffectInitFallingFragment). */
void fdEffectSpawnFallingParticles(void) {
	ACTOR *liftedActor;
	VECTOR3 position;
	ACTOR *fragment;
	u32 radius;

	liftedActor = ((FIELD_EFFECT_STATE *)FieldEffectStateWork)->tgt;
	position.x = liftedActor->x.fixed;
	position.y = liftedActor->y.fixed - syRandom() * 16 + toFIXED(24);
	position.z = liftedActor->z.fixed;

	radius = syRandom() * 3;
	syVectorAddPolarOffset(radius * 16, syRandom(), (FIXED *)&position);

	fragment = fdActorSpawn(SPRITE_SPARKLE, position.x, position.y, position.z);
	fdEffectInitFallingFragment(fragment);
}

/* The sparkles Carry scatters from the target, 8 to 32 pixels out. */
#define CARRY_SPARKLE_COUNT 20

/* Tests the target's move with its ground height raised 16 pixels, so a */
/* carried target passes over ground up to 24 pixels above its own (a step */
/* allows 8). Restores the height before returning the collision result. */
static __inline__ s32 fdEffectCheckRaisedTargetCollision(ACTOR *target,
														 VECTOR3 *position) {
	s32 collision;

	target->groundHeight += toFIXED(16);
	collision = fdMapGridCheckCollisionStub(target, (FIELD_POS *)position);
	target->groundHeight -= toFIXED(16);
	return collision;
}

/* When Carry is cast on a target, two hands lift it to 24 pixels above the */
/* ground; the d-pad moves it 16 pixels at a time and A, B, L or R drops it */
/* where it is. */
void fdPsynergySteerLiftedTarget(s32 targetId) {
	FIELD_EFFECT_STATE *effectState;
	ACTOR *caster;
	ACTOR *target;
	ACTOR *sparkles[CARRY_SPARKLE_COUNT];
	VECTOR3 pos;
	ACTOR *hands[2];
	ACTOR *actor;
	ACTOR *westHand;
	s32 i;
	s32 angle;
	FIXED dist;

	effectState = FieldEffectStateWork;
	caster = effectState->caster;
	target = effectState->tgt;
	if(target == NULL)
		return;
	fdPsynergyFadeToCastCol();
	caster->watchedActor = (union ACTOR_OR_EFFECT *)target;
	fdActorStartScriptStub(caster, FieldEffect_SourceHoldScript);

	/* The hands appear 32 pixels to either side and move to 16 pixels: the east
	 */
	/* one, hands[0], is held in actor until the sparkles. */
	SET_POSITION(&pos, effectState->x, effectState->y + toFIXED(16),
				 effectState->z);
	actor =
		fdEffectSpawnCarryHand(pos.x + toFIXED(32), pos.y, pos.z, FACING_WEST);
	hands[0] = actor;
	westHand =
		fdEffectSpawnCarryHand(pos.x - toFIXED(32), pos.y, pos.z, FACING_EAST);
	hands[1] = westHand;
	if(hands[0] == NULL || westHand == NULL) {
		fdPsynergyFadeFromCastCol();
		return;
	}
	sySynch(15);
	SET_POSITION(&pos, target->x.fixed, target->y.fixed + toFIXED(16),
				 target->z.fixed);
	fdActorSetMoveTargetStub(actor, pos.x + toFIXED(16), pos.y, pos.z);
	fdActorSetMoveTargetStub(westHand, pos.x - toFIXED(16), pos.y, pos.z);
	fdActorWaitForMoveStub(actor);
	fdActorWaitForMoveStub(westHand);
	actor->x.fixed = pos.x + toFIXED(16);
	actor->velX = 0;
	westHand->x.fixed = pos.x - toFIXED(16);
	westHand->velX = 0;

	/* Lift to 24 pixels above the ground, 3/8 pixel a frame, floating with no
	 */
	/* shadow and its colours cycling. */
	target->update = fdActorCycleSprCols;
	sySetEvent(fdEffectSpawnFallingParticles, TASK_PASS_FRAME);
	sySndPlayStub(SE_PSYNERGY_START);
	target->motionFlg = ACTOR_FLOATS;
	fdActorSetSprFlagsStub(target, 0);
	if(hands[0] != NULL && hands[1] != NULL) {
		while(target->y.fixed - target->groundHeight <= toFIXED(24)) {
			actor->y.fixed += toFIXED(3) / 8;
			westHand->y.fixed += toFIXED(3) / 8;
			target->y.fixed += toFIXED(3) / 8;
			sySynch(1);
		}
	}
	hands[0]->speedLimit = toFIXED(4);
	hands[0]->acceleration = toFIXED(1) / 2;
	hands[1]->speedLimit = toFIXED(4);
	hands[1]->acceleration = toFIXED(1) / 2;
	/* As in Move: the target keeps its facing and reads its ground from the */
	/* overhead layer while it is carried. */
	target->speedLimit = toFIXED(2) / 5;
	target->acceleration = toFIXED(1) / 5;
	target->steeringFlg = 0;
	target->layer = MAPGRID_LAYER_OVERHEAD;

	/* Move with the d-pad. */
	for(;;) {
		sySynch(1);
		if((KeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS)) != 0)
			break;

		/* No direction held: the hands go back to the target's sides. */
		angle = (u16)fdInputGetDpadAng(KeysHeld);
		if(angle == DPAD_ANGLE_NONE) {
			pos.x = target->x.fixed;
			pos.y = target->y.fixed + toFIXED(16);
			pos.z = target->z.fixed;
			fdActorSetMoveTargetStub(hands[0], pos.x + toFIXED(16), pos.y,
									 pos.z);
			fdActorSetMoveTargetStub(hands[1], pos.x - toFIXED(16), pos.y,
									 pos.z);
			fdActorSelectAnimationStub(hands[0], MOVE_HAND_ANIMATION_OPEN);
			fdActorSelectAnimationStub(hands[1], MOVE_HAND_ANIMATION_OPEN);
			continue;
		}

		/* The hands lean 2 pixels that way; check the ground 16 pixels on. */
		pos.x = target->x.fixed;
		pos.y = target->y.fixed + toFIXED(16);
		pos.z = target->z.fixed;
		syVectorAddPolarOffset(toFIXED(2), angle, (FIXED *)&pos);
		fdActorSetMoveTargetStub(hands[0], pos.x + toFIXED(16), pos.y, pos.z);
		fdActorSetMoveTargetStub(hands[1], pos.x - toFIXED(16), pos.y, pos.z);
		fdActorWaitForMoveStub(hands[0]);
		fdActorWaitForMoveStub(hands[1]);
		pos.x = target->x.fixed;
		pos.y = target->groundHeight;
		pos.z = target->z.fixed;
		syVectorAddPolarOffset(toFIXED(16), angle, (FIXED *)&pos);
		i = fdActorScriptCheckOverlapStub(target, (s32 *)&pos);
		/* A solid actor or raised ground in the way: a buzzer every 16 frames.
		 */
		if(i != 0) {
			fdActorSelectAnimationStub(hands[0], MOVE_HAND_ANIMATION_FLEX);
			fdActorSelectAnimationStub(hands[1], MOVE_HAND_ANIMATION_FLEX);
			if((FrameCount & 15) == 0)
				sySndPlayStub(SE_MENU_ERROR);
			continue;
		}
		if(fdEffectCheckRaisedTargetCollision(target, &pos) > 0) {
			fdActorSelectAnimationStub(hands[0], MOVE_HAND_ANIMATION_FLEX);
			fdActorSelectAnimationStub(hands[1], MOVE_HAND_ANIMATION_FLEX);
			if((FrameCount & 15) == 0)
				sySndPlayStub(SE_MENU_ERROR);
			continue;
		}
		/* The way is free: move the target there. */
		{
			ACTOR *second;
			FIXED x;
			FIXED z;

			sySndPlayStub(SE_NOTE_HIGH);
			x = pos.x;
			z = pos.z;
			fdActorSelectAnimationStub(hands[0], MOVE_HAND_ANIMATION_FLEX);
			fdActorSelectAnimationStub(hands[1], MOVE_HAND_ANIMATION_FLEX);
			sySynch(15);
			/* i is the overlap test's 0 here: unpause the target and, once it
			 * is */
			/* on the cell, stop it dead. It moves at 0.2 pixels a frame. */
			target->paused = i;
			target->speedLimit = toFIXED(1) / 5;
			target->acceleration = toFIXED(1) / 5;
			fdActorSetMoveTargetStub(target, pos.x, pos.y, pos.z);
			hands[0]->speedLimit = toFIXED(1) / 5;
			hands[0]->acceleration = toFIXED(1) / 5;
			second = hands[1];
			second->speedLimit = toFIXED(1) / 5;
			second->acceleration = toFIXED(1) / 5;
			fdActorSetMoveTargetStub(hands[0], pos.x + toFIXED(16), pos.y,
									 pos.z);
			fdActorSetMoveTargetStub(second, pos.x - toFIXED(16), pos.y, pos.z);
			fdActorWaitForMoveStub(target);
			target->x.fixed = x;
			target->z.fixed = z;
			target->velX = i;
			target->velZ = i;
			sySynch(10);
			break;
		}
	}

	/* Sparkles fly from the target, the hands vanish, and the target hops and
	 */
	/* drops. */
	fdActorSelectAnimationStub(hands[0], MOVE_HAND_ANIMATION_FLEX);
	fdActorSelectAnimationStub(hands[1], MOVE_HAND_ANIMATION_FLEX);
	syCloseEvent(fdEffectSpawnFallingParticles);
	sySndPlayStub(SE_CLICK);
	sySynch(15);
	sySndPlayStub(SE_CLICK);
	sySynch(15);
	SET_POSITION(&pos, target->x.fixed, target->y.fixed + toFIXED(16),
				 target->z.fixed);
	/* Sparkles: 2 to 3 pixels a frame, 8 to 32 pixels in a random direction. */
	/* actor, the east hand until now, holds each sparkle in turn. */
	for(i = 0; i < CARRY_SPARKLE_COUNT; i++) {
		actor = fdActorSpawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
		/* DEAD: nothing reads the sparkles back; the ROM keeps the stores. */
		sparkles[i] = actor;
		if(actor != NULL) {
			fdActorStartScriptStub(actor, FieldEffect_FragmentScript);
			actor->speedLimit = syRandom() + toFIXED(2);
			actor->acceleration = toFIXED(2);
			actor->motionFlg = 0;
			dist = syRandom() * 24 + toFIXED(8);
			fdActorSetTargetPosFromMagnitudeAng(actor, dist, syRandom());
		}
	}
	sySndPlayStub(SE_PSYNERGY_SPARKLE);
	fdActorReleaseStub(hands[0]);
	fdActorReleaseStub(hands[1]);

	/* Restore the target's look and motion, and throw it up at 10 pixels a */
	/* frame: it falls and follows the ground again, keeping 0.2 of its speed */
	/* as it bounces, and reads its ground from the ground layer. */
	fdActorSetSprColModeStub(target, effectState->savedPal);
	fdActorStartScriptStub(target, effectState->savedScript);
	target->update = effectState->savedFunc;
	target->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	target->velY = toFIXED(10);
	target->bounce = FIXED_ONE / 5;
	target->layer = MAPGRID_LAYER_GROUND;
	caster->update = 0;
	fdActorSetSprColModeStub(caster, SPRITE_COLOR_NORMAL);
	if(effectState->sceneVariant != 0) {
		/* As the Road to Babi Lighthouse asks for its grey pillar: wait for it
		 * to */
		/* rise and land (at most 90 frames each), then a ring of dust puffs. */
		for(i = 0; i < 90 && target->velY >= 0; i++)
			sySynch(1);
		sySynch(1);
		for(i = 0; i < 90 && target->velY < 0; i++)
			sySynch(1);
		fdEffectSpawnRadialParticleRing(target);
		fdPsynergyFadeFromCastCol();
		sySynch(30);
	}
	else {
		fdPsynergyFadeFromCastCol();
	}
}

/* When Carry is cast with no target, two hands appear 16 pixels above the spot
 */
/* in front of the caster and 32 pixels to either side, close in 24 pixels and
 */
/* clap, and sparkles scatter. */
DEFINE_CLAPPING_HANDS_CAST(fdPsynergyCarry, fdEffectSpawnCarryHand)

/* Spawns a hand (SPRITE_MOVE_HAND) with the item break sound and grows it, */
/* spinning 1/8 turn a frame, from a quarter to full size over 24 frames; angle
 */
/* is the way it faces once grown. */
ACTOR *fdEffectSpawnCarryHand(s32 x, s32 y, s32 z, s32 angle) {
	ACTOR *hand;
	FIXED scale;

	sySndPlayStub(SE_ITEM_BREAK);
	hand = fdActorSpawn(SPRITE_MOVE_HAND, x, y, z);
	if(hand != NULL) {
		hand->scaleY = FIXED_ONE / 4;
		hand->scaleX = FIXED_ONE / 4;
		hand->speedLimit = toFIXED(3);
		hand->acceleration = toFIXED(3);
		hand->steeringFlg = 0;
		fdActorSelectAnimationStub(hand, MOVE_HAND_ANIMATION_OPEN);
		scale = hand->scaleX;
		if(scale < FIXED_ONE) {
			do {
				scale += FIXED_ONE / 32;
				hand->scaleY = scale;
				hand->scaleX = scale;
				hand->facing += DEGtoANG(45);
				sySynch(1);
				scale = hand->scaleX;
			} while(scale < FIXED_ONE);
		}
		hand->facing = angle;
	}
	return hand;
}

#undef CARRY_SPARKLE_COUNT
