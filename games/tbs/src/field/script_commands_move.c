// Move: a hand pushes the target, steered with the d-pad. Part of script_commands.c.

#include "script_commands_internal.h"

// Returns the direction of the d-pad (TRIG_TURN a full turn): right FACING_EAST, down FACING_SOUTH,
// left FACING_WEST, up FACING_NORTH; DPAD_ANGLE_NONE when none or a diagonal is held.
Sint32 fdInputGetDpadAngle(Uint32 unused)
{
	// Indexed by the right, left, up and down bits.
	return InputDpadAngles[DPAD_INDEX(PerData)];
}

// Per-tick update of Move's hand (fdEffectSpawnMoveHand): turns it toward the actor it watches, at
// most 1/16 turn a tick, and sheds a particle (SPRITE_SPARKLE) round it.
void fdEffectUpdateMoveHand(ACTOR *hand)
{
	ACTOR  *target;
	VECTOR3 pos;
	ACTOR  *sparkle;
	Sint32  turn;
	Sint32  radius;

	target = (ACTOR *)hand->watchedActor;
	if (target != 0)
	{
		FIXED dx = target->x.fixed - hand->x.fixed;
		FIXED dz = target->z.fixed - hand->z.fixed;

		if (dx != 0 || dz != 0)
		{
			turn = (Sint16)(syAtan(dz, dx) - hand->facing);
			if (turn > FACING_STEP)
				turn = FACING_STEP;
			if (turn < -FACING_STEP)
				turn = -FACING_STEP;
			hand->facing += turn;
		}
		// ACTOR_FACES_MOTION off: its motion must not undo the turn.
		hand->steeringFlags = 0;
	}

	// The particle starts 8 to 24 pixels lower and up to 48 pixels away in a random direction.
	pos.x    = hand->x.fixed;
	pos.y    = hand->y.fixed - (syRandom() << 4) - toFIXED(8);
	pos.z    = hand->z.fixed;
	radius   = syRandom() * 3;
	radius <<= 4;
	syVectorAddPolarOffset(radius, syRandom(), (FIXED *)&pos);

	sparkle = fdActorSpawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
	fdEffectInitFallingFragment(sparkle);
}

// When Move is cast on a target, the pushing hand appears beside it and pushes it 16 pixels the way
// the d-pad points, for as long as the player pushes; A, B, L or R end it.
void fdPsynergySteerPushedTarget(Sint32 targetId)
{
	VECTOR3             pos;
	VECTOR3             beyond;
	FIELD_EFFECT_STATE *effectState;
	ACTOR              *caster;
	ACTOR              *target;
	ACTOR              *pusher;
	ACTOR              *hit;
	FIXED               x;
	FIXED               z;
	Sint32              angle;
	Sint32              moved;
	Sint32              direction;
	Sint32              keys;
	Sint32              metatileX;
	Sint32              metatileZ;

	effectState = FieldEffectStateWork;
	caster      = effectState->caster;
	target      = effectState->target;
	// effectState->angle is the way the caster faces; the hand's side of the target is the opposite
	// way, toward the caster.
	angle       = effectState->angle + DEGtoANG(180);
	moved       = 0;
	if (target == NULL)
		return;
	fdPsynergyFadeToCastColor();
	caster->watchedActor = (union ACTOR_OR_EFFECT *)target;
	fdActorStartScriptStub(caster, FieldEffectSourceHoldScript);
	pusher = fdEffectSpawnMoveHand(caster);
	if (pusher == NULL)
	{
		fdPsynergyFadeFromCastColor();
		return;
	}

	// The hand goes 16 pixels above the target and 16 pixels toward angle.
	pusher->watchedActor = (union ACTOR_OR_EFFECT *)target;
	SET_POSITION(&pos, target->x.fixed, target->y.fixed + toFIXED(16), target->z.fixed);
	syVectorAddPolarOffset(toFIXED(16), angle, (FIXED *)&pos);
	fdActorSetMoveTargetStub(pusher, pos.x, pos.y, pos.z);
	fdEffectGrowHandAndWait(pusher);
	// The hand floats at up to 4 pixels a tick.
	pusher->speedLimit    = toFIXED(4);
	pusher->acceleration  = toFIXED(1) / 2;
	pusher->motionFlags   = ACTOR_FLOATS;
	target->update        = fdActorCycleSpriteColors;
	target->speedLimit    = toFIXED(2) / 5;
	target->acceleration  = toFIXED(1) / 5;
	target->steeringFlags = moved;
	target->layer         = MAPGRID_LAYER_OVERHEAD;
	for (;;)
	{
		sySynch(1);
		keys = PerPush & (KEY_A | KEY_B | KEYS_SHOULDERS);
		if (keys != 0)
			break;

		// With the d-pad released, the hand goes back to its place.
		direction = (Uint16)fdInputGetDpadAngle(PerData);
		if (direction == DPAD_ANGLE_NONE)
		{
			pos.x = target->x.fixed;
			pos.y = target->y.fixed + toFIXED(16);
			pos.z = target->z.fixed;
			syVectorAddPolarOffset(toFIXED(16), angle, (FIXED *)&pos);
			fdActorSetMoveTargetStub(pusher, pos.x, pos.y, pos.z);
			fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_OPEN);
			SET_VELOCITY(pusher, 0, 0, 0);
			continue;
		}

		// The hand leans 2 pixels the way it pushes; check the metatiles 16 and 32 pixels beyond
		// the target.
		pos.x = target->x.fixed;
		pos.y = target->y.fixed + toFIXED(16);
		pos.z = target->z.fixed;
		syVectorAddPolarOffset(toFIXED(16), angle, (FIXED *)&pos);
		syVectorAddPolarOffset(toFIXED(2), direction, (FIXED *)&pos);
		fdActorSetMoveTargetStub(pusher, pos.x, pos.y, pos.z);
		fdActorWaitForMoveStub(pusher);
		pos.x = target->x.fixed;
		pos.y = target->y.fixed;
		pos.z = target->z.fixed;
		syVectorAddPolarOffset(toFIXED(16), direction, (FIXED *)&pos);
		beyond.x = target->x.fixed;
		beyond.y = target->y.fixed;
		beyond.z = target->z.fixed;
		syVectorAddPolarOffset(toFIXED(32), direction, (FIXED *)&beyond);
		// A wall or higher ground stops the push; level ground or a drop does not.
		if (fdMapGridCheckCollisionStub(target, (FIELD_POS *)&pos) > COLLISION_NONE)
		{
			fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
			if ((TickCount & 15) == 0)
				sySoundPlayStub(SE_MENU_ERROR);
			continue;
		}
		hit = fdActorScriptFindOverlappingActorStub(target, (Sint32 *)&pos);
		if (hit != NULL)
		{
			if (hit != caster)
			{
				fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
				if ((TickCount & 15) == 0)
					sySoundPlayStub(SE_MENU_ERROR);
				continue;
			}

			// The push runs into the caster: if the caster stands on the metatile the target would
			// move to, it cannot be pushed; if the caster stands on the metatile 32 pixels on, the
			// caster moves along when the metatile beyond is free.
			metatileX = caster->x.fixed & FIELD_METATILE_MASK;
			metatileZ = caster->z.fixed & FIELD_METATILE_MASK;
			if (metatileX == (pos.x & FIELD_METATILE_MASK)
			    && metatileZ == (pos.z & FIELD_METATILE_MASK))
			{
				fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
				if ((TickCount & 15) == 0)
					sySoundPlayStub(SE_MENU_ERROR);
				continue;
			}
			if (metatileX == (beyond.x & FIELD_METATILE_MASK)
			    && metatileZ == (beyond.z & FIELD_METATILE_MASK))
			{
				// The caster can be pushed only onto flat ground.
				if (fdMapGridGetTerrainKindAtStub(caster->layer, beyond.x, beyond.z)
				    != TERRAIN_FLAT)
				{
					fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
					if ((TickCount & 15) == 0)
						sySoundPlayStub(SE_MENU_ERROR);
					continue;
				}
				moved = 1;
			}
		}

		// Push: the hand's animation comes from the turn between angle and the push (in quarter
		// turns): 2 pushing away from its side, 3 drawing toward it, 1 sideways.
		sySoundPlayStub(SE_NOTE_HIGH);
		x = pos.x;
		z = pos.z;
		fdActorSelectAnimationStub(
			pusher, FieldEffectQuadrantModes[(Uint16)(angle - direction) / (TRIG_TURN / 4)]);
		sySynch(15);
		target->paused       = 0;
		target->speedLimit   = toFIXED(1) / 5;
		target->acceleration = toFIXED(1) / 5;
		fdActorSetMoveTargetStub(target, pos.x, pos.y, pos.z);
		pusher->motionFlags  = 0;
		pusher->speedLimit   = toFIXED(1) / 5;
		pusher->acceleration = toFIXED(1) / 5;
		syVectorAddPolarOffset(toFIXED(16), direction, (FIXED *)&pos);
		fdActorSetMoveTargetStub(pusher, pos.x, pos.y + toFIXED(16), pos.z);
		if (moved == 1)
		{
			fdActorGetById(effectState->casterId)->steeringFlags &= ~ACTOR_FACES_MOTION;
			caster->speedLimit                                    = toFIXED(1) / 5;
			caster->acceleration                                  = toFIXED(1) / 5;
			fdActorSetMoveTargetStub(caster, beyond.x, beyond.y, beyond.z);
		}
		// Land exactly on the metatile, whatever the braking left.
		fdActorWaitForMoveStub(target);
		target->x.fixed   = x;
		target->z.fixed   = z;
		target->velocityX = 0;
		target->velocityZ = 0;
		break;
	}

	// Restore the target's colours, script and update, and remove the hand.
	fdActorSetSpriteColorModeStub(target, effectState->savedPalette);
	fdActorStartScriptStub(target, effectState->savedScript);
	target->update = effectState->savedCallback;
	fdPsynergyStopCasterUpdate();
	if (moved == 1)
		fdActorGetById(effectState->casterId)->steeringFlags |= ACTOR_FACES_MOTION;
	fdPsynergyFadeFromCastColor();
	fdEffectBurstHand(pusher);
}

// Field effect sprite callback of a particle that flies 30 pixels out from where it started, comes
// back and is freed.
void fdEffectUpdateOutAndBack(FIELD_PARTICLE *effect)
{
	VECTOR3 pos;
	Sint32  state;

next_state:
	state = effect->state;
	if (state == OUT_AND_BACK_AIM_OUT)
	{
		Uint32 angle;

		pos.x = effect->originX;
		pos.z = effect->originZ;
		angle = syRandom();
		syVectorAddPolarOffset(toFIXED(30), (Uint16)angle, (FIXED *)&pos);
		effect->targetX         = pos.x;
		effect->targetZ         = pos.z;
		effect->acceleration    = toFIXED(4);
		effect->speedMax        = toFIXED(4);
		effect->turnStepLimited = state;
		effect->state++;
		return;
	}

	if (state == OUT_AND_BACK_FLYING_OUT)
	{
		if (fdParticleHasPendingTarget(effect) == 0)
		{
			effect->state++;
			goto next_state;
		}
		return;
	}

	if (state == OUT_AND_BACK_AIM_BACK)
	{
		effect->targetX         = effect->originX;
		effect->targetZ         = effect->originZ;
		effect->turnStepMax     = TRIG_TURN / 64;
		effect->turnStepLimited = 1;
		effect->state++;
		return;
	}

	if (state == OUT_AND_BACK_FLYING_BACK && fdParticleHasPendingTarget(effect) == 0)
		fdEffectClearOwnedSlot(effect);
}

// When Move is cast with no target, the hand appears beside the caster for 30 ticks and then bursts
// into sparkles.
void fdPsynergyMove(void)
{
	// LEFTOVER(frame): an unused local the ROM's frame reserves and never
	// * touches.
	UNUSED Sint32       work[3];
	FIELD_EFFECT_STATE *effectState;
	ACTOR              *caster;
	ACTOR              *hand;

	effectState = FieldEffectStateWork;
	caster      = effectState->caster;

	do
	{
		fdPsynergyFadeToCastColor();
		hand = fdEffectSpawnMoveHand(caster);
		fdEffectGrowHandAndWait(hand);
		// With no hand there is nothing to flex.
		if (hand == 0)
			break;
		fdActorSelectAnimationStub(hand, MOVE_HAND_ANIMATION_FLEX);
		sySynch(30);
	} while (0);
	fdPsynergyFadeFromCastColor();
	fdEffectBurstHand(hand);
}

// Spawns Move's hand (SPRITE_MOVE_HAND) 16 pixels above the caster, at quarter size, and moves it
// 16 pixels the way the caster faces (to the nearest quarter turn), with 8 sparkles scattering from
// it.
ACTOR *fdEffectSpawnMoveHand(ACTOR *caster)
{
	ACTOR *hand;
	ACTOR *sparkle;
	Sint32 angle;
	FIXED  radius;
	FIXED  speed;
	Sint32 sparkleIndex;
	Uint32 spread;
	Uint32 jitter;

	angle = FACING_NEAREST_CARDINAL(caster->facing);
	hand  = fdActorSpawn(
        SPRITE_MOVE_HAND, caster->x.fixed, caster->y.fixed + toFIXED(16), caster->z.fixed);
	if (hand == 0)
		return 0;
	hand->scaleY       = toFIXED(1) / 4;
	hand->scaleX       = toFIXED(1) / 4;
	hand->update       = fdEffectUpdateMoveHand;
	hand->speedLimit   = toFIXED(2);
	hand->acceleration = toFIXED(2);
	hand->motionFlags  = 0;
	fdActorSelectAnimationStub(hand, MOVE_HAND_ANIMATION_GRIP);
	fdActorSetTargetPosFromMagnitudeAngle(hand, toFIXED(16), angle);

	// Sparkles: 8 to 32 pixels within 1/8 turn of the way it faces, at 1 to 2 pixels a tick.
	for (sparkleIndex = 0; sparkleIndex < 8; sparkleIndex++)
	{
		sparkle = fdActorSpawn(
			SPRITE_SPARKLE, caster->x.fixed, caster->y.fixed + toFIXED(16), caster->z.fixed);
		if (sparkle != 0)
		{
			fdActorStartScriptStub(sparkle, FieldEffectFragmentScript);
			speed                 = syRandom() + toFIXED(1);
			sparkle->acceleration = toFIXED(1);
			sparkle->speedLimit   = speed;
			sparkle->motionFlags  = ACTOR_FALLS;
			sparkle->gravity      = toFIXED(1) / 50;
			spread                = syRandom();
			sparkle->velocityY    = spread - syRandom();
			radius                = syRandom() * 24 + toFIXED(8);
			jitter                = syRandom();
			fdActorSetTargetPosFromMagnitudeAngle(
				sparkle, radius, ((jitter - syRandom()) >> 3) + caster->facing);
		}
	}
	sySoundPlayStub(SE_ITEM_BREAK);
	return hand;
}

// Sets an actor's scale at once to full size (its first step of 1/16 past toFIXED(1) - 1: exactly
// toFIXED(1) from a multiple of 1/16) when it is smaller, then waits, at most 600 ticks, for it to
// arrive at its target.
void fdEffectGrowHandAndWait(ACTOR *actor)
{
	FIXED next;
	FIXED scale;

	if (actor != NULL)
	{
		scale = actor->scaleX;
		if (scale < toFIXED(1))
		{
			do
			{
				next  = scale + toFIXED(1) / 16;
				scale = next;
			} while (next < toFIXED(1));
			actor->scaleX = next;
			actor->scaleY = next;
		}
		fdActorWaitForMoveStub(actor);
	}
}

// Ends a hand: over 31 ticks it rises 1 pixel, turns 1/8 turn and shrinks by 1/32 of full size a
// tick, then bursts into 8 sparkles and is released.
void fdEffectBurstHand(ACTOR *hand)
{
	ACTOR *sparkle;
	Sint32 i;
	FIXED  distance;
	Uint32 spread;

	sySoundPlayStub(SE_CAST);
	for (i = 0; i < 31; i++)
	{
		hand->y.fixed += toFIXED(1);
		hand->facing  += DEGtoANG(45);
		hand->scaleX  += -toFIXED(1) / 32;
		hand->scaleY  += -toFIXED(1) / 32;
		sySynch(1);
	}
	for (i = 0; i < 8; i++)
	{
		sparkle = fdActorSpawn(SPRITE_SPARKLE, hand->x.fixed, hand->y.fixed, hand->z.fixed);
		if (sparkle != 0)
		{
			fdActorStartScriptStub(sparkle, FieldEffectFragmentScript);
			sparkle->speedLimit   = syRandom() + toFIXED(1);
			sparkle->acceleration = toFIXED(1);
			sparkle->motionFlags  = ACTOR_FALLS;
			sparkle->gravity      = toFIXED(1) / 25;
			spread                = syRandom();
			sparkle->velocityY    = spread - syRandom();
			distance              = syRandom() * 24 + toFIXED(8);
			fdActorSetTargetPosFromMagnitudeAngle(sparkle, distance, syRandom());
		}
	}
	sySoundPlayStub(SE_PSYNERGY_SPARKLE);
	fdActorReleaseStub(hand);
}
