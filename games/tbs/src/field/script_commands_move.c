/* Move: a hand pushes the target, steered with the d-pad. Part of
   script_commands.c. */

#include "script_commands_internal.h"

/* Returns the direction of the d-pad (TRIG_TURN a full turn): right
   FACING_EAST, down FACING_SOUTH, left FACING_WEST, up FACING_NORTH;
   DPAD_ANGLE_NONE when none or a diagonal is held. */
Sint32 fdInputGetDpadAng(Uint32 unused)
{
	/* Indexed by the right, left, up and down bits. */
	return InputDpadAngles[DPAD_INDEX(PerData)];
}

/* Per-tick update of Move's hand (fdEffectSpawnMoveHand): turns it toward the
   actor it watches, at most 1/16 turn a tick, and sheds a particle
   (SPRITE_SPARKLE) round it. */
void fdEffectUpdateMoveHand(ACTOR *hand)
{
	ACTOR  *tgt;
	VECTOR3 pos;
	ACTOR  *sparkle;
	Sint32	turn;
	Sint32	radius;

	tgt = (ACTOR *)hand->watchedActor;
	if(tgt != 0) {
		FIXED dx = tgt->x.fixed - hand->x.fixed;
		FIXED dz = tgt->z.fixed - hand->z.fixed;

		if(dx != 0 || dz != 0) {
			turn = (Sint16)(syAtan(dz, dx) - hand->facing);
			if(turn > FACING_STEP)
				turn = FACING_STEP;
			if(turn < -FACING_STEP)
				turn = -FACING_STEP;
			hand->facing += turn;
		}
		/* ACTOR_FACES_MOTION off: its motion must not undo the turn. */
		hand->steeringFlg = 0;
	}

	/* The particle starts 8 to 24 pixels lower and up to 48 pixels away in a
	   random direction. */
	pos.x = hand->x.fixed;
	pos.y = hand->y.fixed - (syRandom() << 4) - toFIXED(8);
	pos.z = hand->z.fixed;
	radius = syRandom() * 3;
	radius <<= 4;
	syVectorAddPolarOffset(radius, syRandom(), (FIXED *)&pos);

	sparkle = fdActorSpawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
	fdEffectInitFallingFragment(sparkle);
}

/* When Move is cast on a target, the pushing hand appears beside it and pushes
   it 16 pixels the way the d-pad points, for as long as the player pushes; A,
   B, L or R end it. */
void fdPsynergySteerPushedTarget(Sint32 tgtId)
{
	VECTOR3				pos;
	VECTOR3				beyond;
	FIELD_EFFECT_STATE *effectState;
	ACTOR			   *caster;
	ACTOR			   *tgt;
	ACTOR			   *pusher;
	ACTOR			   *hit;
	FIXED				x;
	FIXED				z;
	Sint32				ang;
	Sint32				moved;
	Sint32				direction;
	Sint32				keys;
	Sint32				metatileX;
	Sint32				metatileZ;

	effectState = FieldEffectStateWork;
	caster = effectState->caster;
	tgt = effectState->tgt;
	/* effectState->ang is the way the caster faces; the hand's side of the
	   target is the opposite way, toward the caster. */
	ang = effectState->ang + DEGtoANG(180);
	moved = 0;
	if(tgt == NULL)
		return;
	fdPsynergyFadeToCastCol();
	caster->watchedActor = (union ACTOR_OR_EFFECT *)tgt;
	fdActorStartScriptStub(caster, FieldEffectSourceHoldScript);
	pusher = fdEffectSpawnMoveHand(caster);
	if(pusher == NULL) {
		fdPsynergyFadeFromCastCol();
		return;
	}

	/* The hand goes 16 pixels above the target and 16 pixels toward angle. */
	pusher->watchedActor = (union ACTOR_OR_EFFECT *)tgt;
	SET_POSITION(&pos, tgt->x.fixed, tgt->y.fixed + toFIXED(16), tgt->z.fixed);
	syVectorAddPolarOffset(toFIXED(16), ang, (FIXED *)&pos);
	fdActorSetMoveTargetStub(pusher, pos.x, pos.y, pos.z);
	fdEffectGrowHandAndWait(pusher);
	/* The hand floats at up to 4 pixels a tick. */
	pusher->speedLimit = toFIXED(4);
	pusher->acceleration = toFIXED(1) / 2;
	pusher->motionFlg = ACTOR_FLOATS;
	tgt->update = fdActorCycleSprCols;
	tgt->speedLimit = toFIXED(2) / 5;
	tgt->acceleration = toFIXED(1) / 5;
	tgt->steeringFlg = moved;
	tgt->layer = MAPGRID_LAYER_OVERHEAD;
	for(;;) {
		sySynch(1);
		keys = PerPush & (KEY_A | KEY_B | KEYS_SHOULDERS);
		if(keys != 0)
			break;

		/* With the d-pad released, the hand goes back to its place. */
		direction = (Uint16)fdInputGetDpadAng(PerData);
		if(direction == DPAD_ANGLE_NONE) {
			pos.x = tgt->x.fixed;
			pos.y = tgt->y.fixed + toFIXED(16);
			pos.z = tgt->z.fixed;
			syVectorAddPolarOffset(toFIXED(16), ang, (FIXED *)&pos);
			fdActorSetMoveTargetStub(pusher, pos.x, pos.y, pos.z);
			fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_OPEN);
			SET_VELOCITY(pusher, 0, 0, 0);
			continue;
		}

		/* The hand leans 2 pixels the way it pushes; check the metatiles 16 and
		   32 pixels beyond the target. */
		pos.x = tgt->x.fixed;
		pos.y = tgt->y.fixed + toFIXED(16);
		pos.z = tgt->z.fixed;
		syVectorAddPolarOffset(toFIXED(16), ang, (FIXED *)&pos);
		syVectorAddPolarOffset(toFIXED(2), direction, (FIXED *)&pos);
		fdActorSetMoveTargetStub(pusher, pos.x, pos.y, pos.z);
		fdActorWaitForMoveStub(pusher);
		pos.x = tgt->x.fixed;
		pos.y = tgt->y.fixed;
		pos.z = tgt->z.fixed;
		syVectorAddPolarOffset(toFIXED(16), direction, (FIXED *)&pos);
		beyond.x = tgt->x.fixed;
		beyond.y = tgt->y.fixed;
		beyond.z = tgt->z.fixed;
		syVectorAddPolarOffset(toFIXED(32), direction, (FIXED *)&beyond);
		/* A wall or higher ground stops the push; level ground or a drop does
		   not. */
		if(fdMapGridCheckCollisionStub(tgt, (FIELD_POS *)&pos) >
			COLLISION_NONE) {
			fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
			if((TickCount & 15) == 0)
				sySndPlayStub(SE_MENU_ERROR);
			continue;
		}
		hit = fdActorScriptFindOverlappingActorStub(tgt, (Sint32 *)&pos);
		if(hit != NULL) {
			if(hit != caster) {
				fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
				if((TickCount & 15) == 0)
					sySndPlayStub(SE_MENU_ERROR);
				continue;
			}

			/* The push runs into the caster: if the caster stands on the
			   metatile the target would move to, it cannot be pushed; if the
			   caster stands on the metatile 32 pixels on, the caster moves
			   along when the metatile beyond is free. */
			metatileX = caster->x.fixed & FIELD_METATILE_MASK;
			metatileZ = caster->z.fixed & FIELD_METATILE_MASK;
			if(metatileX == (pos.x & FIELD_METATILE_MASK) &&
				metatileZ == (pos.z & FIELD_METATILE_MASK)) {
				fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
				if((TickCount & 15) == 0)
					sySndPlayStub(SE_MENU_ERROR);
				continue;
			}
			if(metatileX == (beyond.x & FIELD_METATILE_MASK) &&
				metatileZ == (beyond.z & FIELD_METATILE_MASK)) {
				/* The caster can be pushed only onto flat ground. */
				if(fdMapGridGetTerrainKindAtStub(
					   caster->layer, beyond.x, beyond.z) != TERRAIN_FLAT) {
					fdActorSelectAnimationStub(
						pusher, MOVE_HAND_ANIMATION_FLEX);
					if((TickCount & 15) == 0)
						sySndPlayStub(SE_MENU_ERROR);
					continue;
				}
				moved = 1;
			}
		}

		/* Push: the hand's animation comes from the turn between angle and the
		   push (in quarter turns): 2 pushing away from its side, 3 drawing
		   toward it, 1 sideways. */
		sySndPlayStub(SE_NOTE_HIGH);
		x = pos.x;
		z = pos.z;
		fdActorSelectAnimationStub(pusher,
			FieldEffectQuadrantModes[(Uint16)(ang - direction) /
				(TRIG_TURN / 4)]);
		sySynch(15);
		tgt->paused = 0;
		tgt->speedLimit = toFIXED(1) / 5;
		tgt->acceleration = toFIXED(1) / 5;
		fdActorSetMoveTargetStub(tgt, pos.x, pos.y, pos.z);
		pusher->motionFlg = 0;
		pusher->speedLimit = toFIXED(1) / 5;
		pusher->acceleration = toFIXED(1) / 5;
		syVectorAddPolarOffset(toFIXED(16), direction, (FIXED *)&pos);
		fdActorSetMoveTargetStub(pusher, pos.x, pos.y + toFIXED(16), pos.z);
		if(moved == 1) {
			fdActorGetById(effectState->casterId)->steeringFlg &=
				~ACTOR_FACES_MOTION;
			caster->speedLimit = toFIXED(1) / 5;
			caster->acceleration = toFIXED(1) / 5;
			fdActorSetMoveTargetStub(caster, beyond.x, beyond.y, beyond.z);
		}
		/* Land exactly on the metatile, whatever the braking left. */
		fdActorWaitForMoveStub(tgt);
		tgt->x.fixed = x;
		tgt->z.fixed = z;
		tgt->velX = 0;
		tgt->velZ = 0;
		break;
	}

	/* Restore the target's colours, script and update, and remove the hand. */
	fdActorSetSprColModeStub(tgt, effectState->savedPal);
	fdActorStartScriptStub(tgt, effectState->savedScript);
	tgt->update = effectState->savedFunc;
	fdPsynergyStopCasterUpdate();
	if(moved == 1)
		fdActorGetById(effectState->casterId)->steeringFlg |=
			ACTOR_FACES_MOTION;
	fdPsynergyFadeFromCastCol();
	fdEffectBurstHand(pusher);
}

/* Field effect sprite callback of a particle that flies 30 pixels out from
   where it started, comes back and is freed. */
void fdEffectUpdateOutAndBack(FIELD_PARTICLE *effect)
{
	VECTOR3 pos;
	Sint32	state;

next_state:
	state = effect->state;
	if(state == OUT_AND_BACK_AIM_OUT) {
		Uint32 ang;

		pos.x = effect->originX;
		pos.z = effect->originZ;
		ang = syRandom();
		syVectorAddPolarOffset(toFIXED(30), (Uint16)ang, (FIXED *)&pos);
		effect->tgtX = pos.x;
		effect->tgtZ = pos.z;
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
   ticks and then bursts into sparkles. */
void fdPsynergyMove(void)
{
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED Sint32		work[3];
	FIELD_EFFECT_STATE *effectState;
	ACTOR			   *caster;
	ACTOR			   *hand;

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
   size, and moves it 16 pixels the way the caster faces (to the nearest quarter
   turn), with 8 sparkles scattering from it. */
ACTOR *fdEffectSpawnMoveHand(ACTOR *caster)
{
	ACTOR *hand;
	ACTOR *sparkle;
	Sint32 ang;
	FIXED  radius;
	FIXED  speed;
	Sint32 sparkleIdx;
	Uint32 spread;
	Uint32 jitter;

	ang = FACING_NEAREST_CARDINAL(caster->facing);
	hand = fdActorSpawn(SPRITE_MOVE_HAND, caster->x.fixed,
		caster->y.fixed + toFIXED(16), caster->z.fixed);
	if(hand == 0)
		return 0;
	hand->scaleY = toFIXED(1) / 4;
	hand->scaleX = toFIXED(1) / 4;
	hand->update = fdEffectUpdateMoveHand;
	hand->speedLimit = toFIXED(2);
	hand->acceleration = toFIXED(2);
	hand->motionFlg = 0;
	fdActorSelectAnimationStub(hand, MOVE_HAND_ANIMATION_GRIP);
	fdActorSetTargetPosFromMagnitudeAng(hand, toFIXED(16), ang);

	/* Sparkles: 8 to 32 pixels within 1/8 turn of the way it faces, at 1 to 2
	   pixels a tick. */
	for(sparkleIdx = 0; sparkleIdx < 8; sparkleIdx++) {
		sparkle = fdActorSpawn(SPRITE_SPARKLE, caster->x.fixed,
			caster->y.fixed + toFIXED(16), caster->z.fixed);
		if(sparkle != 0) {
			fdActorStartScriptStub(sparkle, FieldEffectFragmentScript);
			speed = syRandom() + toFIXED(1);
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

#include "field/effect_grow_hand_and_wait.c"

/* Ends a hand: over 31 ticks it rises 1 pixel, turns 1/8 turn and shrinks by
   1/32 of full size a tick, then bursts into 8 sparkles and is released. */
void fdEffectBurstHand(ACTOR *hand)
{
	ACTOR *sparkle;
	Sint32 i;
	FIXED  dist;
	Uint32 spread;

	sySndPlayStub(SE_CAST);
	for(i = 0; i < 31; i++) {
		hand->y.fixed += toFIXED(1);
		hand->facing += DEGtoANG(45);
		hand->scaleX += -toFIXED(1) / 32;
		hand->scaleY += -toFIXED(1) / 32;
		sySynch(1);
	}
	for(i = 0; i < 8; i++) {
		sparkle = fdActorSpawn(
			SPRITE_SPARKLE, hand->x.fixed, hand->y.fixed, hand->z.fixed);
		if(sparkle != 0) {
			fdActorStartScriptStub(sparkle, FieldEffectFragmentScript);
			sparkle->speedLimit = syRandom() + toFIXED(1);
			sparkle->acceleration = toFIXED(1);
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
