/* @file */
/* Carry cast where there is nothing to carry, and the hands Carry spawns. */
/* Part of event_commands.c. */

/* Carry cast with no target: two hands appear 32 pixels to either side of */
/* the spot, close 24 pixels in and meet with a crash that scatters */
/* ITEM_BREAK_SHARD_MAX sparkles, then vanish. */
void fdPsynergyCarry(void) {
	FIELD_EFFECT_STATE *state;
	ACTOR *actor;
	ACTOR *pair[2];
	VECTOR3 pos;
	ACTOR **hand;
	s32 n;
	FIXED distance;

	state = Ram_HeapSlots->fieldEffState;
	fdPsynergyFadeToCastCol();
	SET_POSITION(&pos, state->x, state->y + toFIXED(16), state->z);
	pair[0] =
		fdEffectSpawnCarryHand(pos.x + toFIXED(32), pos.y, pos.z, FACING_WEST);
	pair[1] =
		fdEffectSpawnCarryHand(pos.x - toFIXED(32), pos.y, pos.z, FACING_EAST);

	sySynch(15);
	hand = pair;
	for(n = 0; n < 2; n++) {
		actor = *hand++;
		if(actor != 0)
			fdActorSetTargetPosFromMagnitudeAng(actor, toFIXED(24),
												actor->facing);
	}

	fdActorWaitForMoveStub(pair[0]);
	sySndPlayStub(SE_HEAVY_IMPACT);
	for(n = 0; n < ITEM_BREAK_SHARD_MAX; n++) {
		SET_POSITION(&pos, state->x, state->y + toFIXED(16), state->z);
		actor = fdActorSpawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
		if(actor != 0) {
			fdActorStartScriptStub(actor, FieldEffect_FragmentScript);
			actor->speedLimit = syRandom() + toFIXED(2);
			actor->acceleration = toFIXED(2);
			actor->motionFlg = 0;
			distance = syRandom() * 24 + toFIXED(8);
			fdActorSetTargetPosFromMagnitudeAng(actor, distance, syRandom());
		}
	}

	fdActorReleaseStub(pair[0]);
	fdActorReleaseStub(pair[1]);
	fdPsynergyFadeFromCastCol();
}

/* Spawns a hand (SPRITE_MOVE_HAND) with the item break sound and grows it, */
/* spinning 1/8 turn a frame, from a quarter to full size over 24 frames; angle
 */
/* is the way it faces once grown. */
ACTOR *fdEffectSpawnCarryHand(s32 x, s32 y, s32 z, s32 angle) {
	ACTOR *actor;
	FIXED scale;

	sySndPlayStub(SE_ITEM_BREAK);
	actor = fdActorSpawn(SPRITE_MOVE_HAND, x, y, z);
	if(actor != NULL) {
		actor->scaleY = FIXED_ONE / 4;
		actor->scaleX = FIXED_ONE / 4;
		actor->speedLimit = toFIXED(3);
		actor->acceleration = toFIXED(3);
		actor->steeringFlg = 0;
		fdActorSelectAnimationStub(actor, MOVE_HAND_ANIMATION_OPEN);
		scale = actor->scaleX;
		if(scale < FIXED_ONE) {
			do {
				scale += FIXED_ONE / 32;
				actor->scaleY = scale;
				actor->scaleX = scale;
				actor->facing += DEGtoANG(45);
				sySynch(1);
				scale = actor->scaleX;
			} while(scale < FIXED_ONE);
		}
		actor->facing = angle;
	}
	return actor;
}
