/* @file */
/* Move: a hand pushes the target, steered with the d-pad. */
/* Part of event_commands.c. */

#include "event_commands_internal.h"

/* Returns the direction of the d-pad (TRIG_TURN a full turn): right */
/* FACING_EAST, down FACING_SOUTH, left FACING_WEST, up FACING_NORTH; */
/* DPAD_ANGLE_NONE when none or a diagonal is held. */
s32 fdInputGetDpadAng(u32 unused) {
	/* Indexed by the right, left, up and down bits. */
	return Input_DpadAngles[DPAD_INDEX(KeysHeld)];
}

/* Per-frame update of Move's hand (fdEffectSpawnMoveHand): turns it */
/* toward the actor it watches, at most 1/16 turn a frame, and sheds a */
/* particle (SPRITE_SPARKLE) round it. */
void fdEffectUpdateMoveHand(ACTOR *hand) {
	ACTOR *target;
	VECTOR3 position;
	ACTOR *sparkle;
	s32 turn;
	s32 radius;

	target = (ACTOR *)hand->watchedActor;
	if(target != 0) {
		FIXED dx = target->x.fixed - hand->x.fixed;
		FIXED dz = target->z.fixed - hand->z.fixed;

		if(dx != 0 || dz != 0) {
			turn = (s16)(syAtan(dz, dx) - hand->facing);
			if(turn > FACING_STEP)
				turn = FACING_STEP;
			if(turn < -FACING_STEP)
				turn = -FACING_STEP;
			hand->facing += turn;
		}
		/* ACTOR_FACES_MOTION off: its motion must not undo the turn. */
		hand->steeringFlg = 0;
	}

	/* The particle starts 8 to 24 pixels lower and up to 48 pixels away in a */
	/* random direction. */
	position.x = hand->x.fixed;
	position.y = hand->y.fixed - (syRandom() << 4) - toFIXED(8);
	position.z = hand->z.fixed;
	radius = syRandom() * 3;
	radius <<= 4;
	syVectorAddPolarOffset(radius, syRandom(), (FIXED *)&position);

	sparkle = fdActorSpawn(SPRITE_SPARKLE, position.x, position.y, position.z);
	fdEffectInitFallingFragment(sparkle);
}

/* When Move is cast on a target, the pushing hand appears beside it and pushes
 */
/* it 16 pixels the way the d-pad points, for as long as the player pushes; A,
 */
/* B, L or R end it. */
void fdPsynergySteerPushedTarget(s32 targetId) {
	VECTOR3 position;
	VECTOR3 beyond;
	FIELD_EFFECT_STATE *effectState;
	ACTOR *caster;
	ACTOR *target;
	ACTOR *pusher;
	ACTOR *hit;
	FIXED x;
	FIXED z;
	s32 angle;
	s32 moved;
	s32 direction;
	s32 keys;
	s32 cellX;
	s32 cellZ;

	effectState = FieldEffectStateWork;
	caster = effectState->caster;
	target = effectState->tgt;
	/* effectState->ang is the way the caster faces; the hand's side of the */
	/* target is the opposite way, toward the caster. */
	angle = effectState->ang + DEGtoANG(180);
	moved = 0;
	if(target == NULL)
		return;
	fdPsynergyFadeToCastCol();
	caster->watchedActor = (union ACTOR_OR_EFFECT *)target;
	fdActorStartScriptStub(caster, FieldEffect_SourceHoldScript);
	pusher = fdEffectSpawnMoveHand(caster);
	if(pusher == NULL) {
		fdPsynergyFadeFromCastCol();
		return;
	}

	/* The hand goes 16 pixels above the target and 16 pixels toward angle. */
	pusher->watchedActor = (union ACTOR_OR_EFFECT *)target;
	SET_POSITION(&position, target->x.fixed, target->y.fixed + toFIXED(16),
				 target->z.fixed);
	syVectorAddPolarOffset(toFIXED(16), angle, (FIXED *)&position);
	fdActorSetMoveTargetStub(pusher, position.x, position.y, position.z);
	fdEffectGrowHandAndWait(pusher);
	/* The hand floats at up to 4 pixels a frame. */
	pusher->speedLimit = toFIXED(4);
	pusher->acceleration = toFIXED(1) / 2;
	pusher->motionFlg = ACTOR_FLOATS;
	target->update = fdActorCycleSprCols;
	target->speedLimit = toFIXED(2) / 5;
	target->acceleration = toFIXED(1) / 5;
	target->steeringFlg = moved;
	target->layer = MAPGRID_LAYER_OVERHEAD;
	for(;;) {
		sySynch(1);
		keys = KeysPressed & (KEY_A | KEY_B | KEYS_SHOULDERS);
		if(keys != 0)
			break;

		/* With the d-pad released, the hand goes back to its place. */
		direction = (u16)fdInputGetDpadAng(KeysHeld);
		if(direction == DPAD_ANGLE_NONE) {
			position.x = target->x.fixed;
			position.y = target->y.fixed + toFIXED(16);
			position.z = target->z.fixed;
			syVectorAddPolarOffset(toFIXED(16), angle, (FIXED *)&position);
			fdActorSetMoveTargetStub(pusher, position.x, position.y,
									 position.z);
			fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_OPEN);
			SET_VELOCITY(pusher, 0, 0, 0);
			continue;
		}

		/* The hand leans 2 pixels the way it pushes; check the cells 16 and 32
		 */
		/* pixels beyond the target. */
		position.x = target->x.fixed;
		position.y = target->y.fixed + toFIXED(16);
		position.z = target->z.fixed;
		syVectorAddPolarOffset(toFIXED(16), angle, (FIXED *)&position);
		syVectorAddPolarOffset(toFIXED(2), direction, (FIXED *)&position);
		fdActorSetMoveTargetStub(pusher, position.x, position.y, position.z);
		fdActorWaitForMoveStub(pusher);
		position.x = target->x.fixed;
		position.y = target->y.fixed;
		position.z = target->z.fixed;
		syVectorAddPolarOffset(toFIXED(16), direction, (FIXED *)&position);
		beyond.x = target->x.fixed;
		beyond.y = target->y.fixed;
		beyond.z = target->z.fixed;
		syVectorAddPolarOffset(toFIXED(32), direction, (FIXED *)&beyond);
		/* A wall or higher ground stops the push; level ground or a drop does
		 */
		/* not. */
		if(fdMapGridCheckCollisionStub(target, (FIELD_POS *)&position) >
		   COLLISION_NONE) {
			fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
			if((FrameCount & 15) == 0)
				sySndPlayStub(SE_MENU_ERROR);
			continue;
		}
		hit = fdActorScriptFindOverlappingActorStub(target, (s32 *)&position);
		if(hit != NULL) {
			if(hit != caster) {
				fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
				if((FrameCount & 15) == 0)
					sySndPlayStub(SE_MENU_ERROR);
				continue;
			}

			/* The push runs into the caster: if the caster stands on the cell
			 * the */
			/* target would move to, it cannot be pushed; if the caster stands
			 * on the */
			/* cell 32 pixels on, the caster moves along when the cell beyond is
			 * free. */
			cellX = caster->x.fixed & FIELD_CELL_MASK;
			cellZ = caster->z.fixed & FIELD_CELL_MASK;
			if(cellX == (position.x & FIELD_CELL_MASK) &&
			   cellZ == (position.z & FIELD_CELL_MASK)) {
				fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
				if((FrameCount & 15) == 0)
					sySndPlayStub(SE_MENU_ERROR);
				continue;
			}
			if(cellX == (beyond.x & FIELD_CELL_MASK) &&
			   cellZ == (beyond.z & FIELD_CELL_MASK)) {
				/* The caster can be pushed only onto flat ground. */
				if(fdMapGridGetTerrainKindAtStub(caster->layer, beyond.x,
												 beyond.z) != TERRAIN_FLAT) {
					fdActorSelectAnimationStub(pusher,
											   MOVE_HAND_ANIMATION_FLEX);
					if((FrameCount & 15) == 0)
						sySndPlayStub(SE_MENU_ERROR);
					continue;
				}
				moved = 1;
			}
		}

		/* Push: the hand's animation comes from the turn between angle and the
		 * push */
		/* (in quarter turns): 2 pushing away from its side, 3 drawing toward
		 * it, 1 */
		/* sideways. */
		sySndPlayStub(SE_NOTE_HIGH);
		x = position.x;
		z = position.z;
		fdActorSelectAnimationStub(
			pusher, FieldEffect_QuadrantModes[(u16)(angle - direction) /
											  (TRIG_TURN / 4)]);
		sySynch(15);
		target->paused = 0;
		target->speedLimit = toFIXED(1) / 5;
		target->acceleration = toFIXED(1) / 5;
		fdActorSetMoveTargetStub(target, position.x, position.y, position.z);
		pusher->motionFlg = 0;
		pusher->speedLimit = toFIXED(1) / 5;
		pusher->acceleration = toFIXED(1) / 5;
		syVectorAddPolarOffset(toFIXED(16), direction, (FIXED *)&position);
		fdActorSetMoveTargetStub(pusher, position.x, position.y + toFIXED(16),
								 position.z);
		if(moved == 1) {
			fdActorGetById(effectState->casterId)->steeringFlg &=
				~ACTOR_FACES_MOTION;
			caster->speedLimit = toFIXED(1) / 5;
			caster->acceleration = toFIXED(1) / 5;
			fdActorSetMoveTargetStub(caster, beyond.x, beyond.y, beyond.z);
		}
		/* Land exactly on the cell, whatever the braking left. */
		fdActorWaitForMoveStub(target);
		target->x.fixed = x;
		target->z.fixed = z;
		target->velX = 0;
		target->velZ = 0;
		break;
	}

	/* Restore the target's colours, script and update, and remove the hand. */
	fdActorSetSprColModeStub(target, effectState->savedPal);
	fdActorStartScriptStub(target, effectState->savedScript);
	target->update = effectState->savedFunc;
	fdPsynergyStopCasterUpdate();
	if(moved == 1)
		fdActorGetById(effectState->casterId)->steeringFlg |=
			ACTOR_FACES_MOTION;
	fdPsynergyFadeFromCastCol();
	fdEffectBurstHand(pusher);
}

/* Field effect sprite callback of a particle that flies 30 pixels out from */
/* where it started, comes back and is freed. */
void fdEffectUpdateOutAndBack(FIELD_PARTICLE *effect) {
	VECTOR3 position;
	s32 state;

next_state:
	state = effect->state;
	if(state == OUT_AND_BACK_AIM_OUT) {
		u32 angle;

		position.x = effect->originX;
		position.z = effect->originZ;
		angle = syRandom();
		syVectorAddPolarOffset(toFIXED(30), (u16)angle, (FIXED *)&position);
		effect->tgtX = position.x;
		effect->tgtZ = position.z;
		effect->acceleration = toFIXED(4);
		effect->speedMax = toFIXED(4);
		effect->turnStepLimited = state;
		effect->state++;
		return;
	}

	if(state == OUT_AND_BACK_FLYING_OUT) {
		if(fdParticleHasPendingTarget(effect) == 0) {
			effect->state++;
			goto next_state;
		}
		return;
	}

	if(state == OUT_AND_BACK_AIM_BACK) {
		effect->tgtX = effect->originX;
		effect->tgtZ = effect->originZ;
		effect->turnStepMax = TRIG_TURN / 64;
		effect->turnStepLimited = 1;
		effect->state++;
		return;
	}

	if(state == OUT_AND_BACK_FLYING_BACK &&
	   fdParticleHasPendingTarget(effect) == 0)
		fdEffectClearOwnedSlot(effect);
}

/* When Move is cast with no target, the hand appears beside the caster for 30
 */
/* frames and then bursts into sparkles. */
void fdPsynergyMove(void) {
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED s32 work[3];
	FIELD_EFFECT_STATE *effectState;
	ACTOR *caster;
	ACTOR *hand;

	effectState = FieldEffectStateWork;
	caster = effectState->caster;

	do {
		fdPsynergyFadeToCastCol();
		hand = fdEffectSpawnMoveHand(caster);
		fdEffectGrowHandAndWait(hand);
		/* With no hand there is nothing to flex. */
		if(hand == 0)
			break;
		fdActorSelectAnimationStub(hand, MOVE_HAND_ANIMATION_FLEX);
		sySynch(30);
	} while(0);
	fdPsynergyFadeFromCastCol();
	fdEffectBurstHand(hand);
}

/* Spawns Move's hand (SPRITE_MOVE_HAND) 16 pixels above the caster, at quarter
 */
/* size, and moves it 16 pixels the way the caster faces (to the nearest */
/* quarter turn), with 8 sparkles scattering from it. */
ACTOR *fdEffectSpawnMoveHand(ACTOR *caster) {
	ACTOR *hand;
	ACTOR *sparkle;
	s32 angle;
	FIXED radius;
	FIXED speed;
	s32 sparkleIndex;
	u32 spread;
	u32 jitter;

	angle = FACING_NEAREST_CARDINAL(caster->facing);
	hand = fdActorSpawn(SPRITE_MOVE_HAND, caster->x.fixed,
						caster->y.fixed + toFIXED(16), caster->z.fixed);
	if(hand == 0)
		return 0;
	hand->scaleY = FIXED_ONE / 4;
	hand->scaleX = FIXED_ONE / 4;
	hand->update = fdEffectUpdateMoveHand;
	hand->speedLimit = toFIXED(2);
	hand->acceleration = toFIXED(2);
	hand->motionFlg = 0;
	fdActorSelectAnimationStub(hand, MOVE_HAND_ANIMATION_GRIP);
	fdActorSetTargetPosFromMagnitudeAng(hand, toFIXED(16), angle);

	/* Sparkles: 8 to 32 pixels within 1/8 turn of the way it faces, at 1 to 2
	 */
	/* pixels a frame. */
	for(sparkleIndex = 0; sparkleIndex < 8; sparkleIndex++) {
		sparkle = fdActorSpawn(SPRITE_SPARKLE, caster->x.fixed,
							   caster->y.fixed + toFIXED(16), caster->z.fixed);
		if(sparkle != 0) {
			fdActorStartScriptStub(sparkle, FieldEffect_FragmentScript);
			speed = syRandom() + FIXED_ONE;
			sparkle->acceleration = toFIXED(1);
			sparkle->speedLimit = speed;
			sparkle->motionFlg = ACTOR_FALLS;
			sparkle->gravity = toFIXED(1) / 50;
			spread = syRandom();
			sparkle->velY = spread - syRandom();
			radius = syRandom() * 24 + toFIXED(8);
			jitter = syRandom();
			fdActorSetTargetPosFromMagnitudeAng(
				sparkle, radius, ((jitter - syRandom()) >> 3) + caster->facing);
		}
	}
	sySndPlayStub(SE_ITEM_BREAK);
	return hand;
}

/* Sets an actor's scale at once to full size (its first step of 1/16 past */
/* FIXED_ONE - 1: exactly FIXED_ONE from a multiple of 1/16) when it is */
/* smaller, then waits, at most 600 frames, for it to arrive at its target. */
void fdEffectGrowHandAndWait(ACTOR *actor) {
	FIXED next;
	FIXED scale;

	if(actor != NULL) {
		scale = actor->scaleX;
		if(scale < FIXED_ONE) {
			do {
				next = scale + FIXED_ONE / 16;
				scale = next;
			} while(next < FIXED_ONE);
			actor->scaleX = next;
			actor->scaleY = next;
		}
		fdActorWaitForMoveStub(actor);
	}
}

/* Ends a hand: over 31 frames it rises 1 pixel, turns 1/8 turn and shrinks by
 */
/* 1/32 of full size a frame, then bursts into 8 sparkles and is released. */
void fdEffectBurstHand(ACTOR *hand) {
	ACTOR *sparkle;
	s32 i;
	FIXED dist;
	u32 spread;

	sySndPlayStub(SE_CAST);
	for(i = 0; i < 31; i++) {
		hand->y.fixed += toFIXED(1);
		hand->facing += DEGtoANG(45);
		hand->scaleX += -FIXED_ONE / 32;
		hand->scaleY += -FIXED_ONE / 32;
		sySynch(1);
	}
	for(i = 0; i < 8; i++) {
		sparkle = fdActorSpawn(SPRITE_SPARKLE, hand->x.fixed, hand->y.fixed,
							   hand->z.fixed);
		if(sparkle != 0) {
			fdActorStartScriptStub(sparkle, FieldEffect_FragmentScript);
			sparkle->speedLimit = syRandom() + FIXED_ONE;
			sparkle->acceleration = FIXED_ONE;
			sparkle->motionFlg = ACTOR_FALLS;
			sparkle->gravity = toFIXED(1) / 25;
			spread = syRandom();
			sparkle->velY = spread - syRandom();
			dist = syRandom() * 24 + toFIXED(8);
			fdActorSetTargetPosFromMagnitudeAng(sparkle, dist, syRandom());
		}
	}
	sySndPlayStub(SE_PSYNERGY_SPARKLE);
	fdActorReleaseStub(hand);
}
