/* The field Psynergy from Mind Read to Cloak, as The Broken Seal's
   script_commands.c has them in its parts mind_read.c to cloak.c: Mind Read's
   wave, Move, Reveal, Growth, Lift, Force, Frost, Ply and the start of Cloak.
   Part of script_commands.c. */

/* A line's offset in Mind Read's wave at a phase: a sine 4 pixels either way, a
   turn every 160 phase steps. */
#define MIND_READ_WAVE_OFFSET(phase) (sySin(((phase) << 16) / 160) >> 14)
/* The wave's cycle runs to this count and starts again, 61 ticks; at counts 0,
   8 and 16 a particle flies from the target to the caster. */
#define MIND_READ_CYCLE_LAST		 60
#define MIND_READ_PARTICLE_GAP		 8

/* Mind Read's wave each tick: once its delay has run out, writes the next page
   of line offsets, steps the colour fade and the hue every other tick, keeps
   the caster's colours lit, and three times a cycle sends a particle
   (SPRITE_CRESCENT) from the target to the caster, with SE_PSYNERGY_START. */
void fdPsynergyUpdateMindReadWave(void)
{
	MIND_READ_STATE	   *state = RamHeapSlots->mindRead;
	VECTOR3				from;
	VECTOR3				to;
	ACTOR_PLACE		   *descriptor;
	Sint32				spriteId;
	ANIMATION_METADATA *metadata;
	PLACED_SPR		   *sprite;
	ACTOR			   *src;
	ACTOR			   *target;
	ACTOR			   *actor;
	Uint16			   *line;
	Sint32				i;

	if(state->delay != 0) {
		state->delay--;
		return;
	}

	line = state->lines[state->page ^ 1];
	for(i = 0; i < DISPLAY_HEIGHT; i++)
		*line++ = MIND_READ_WAVE_OFFSET(state->phase + i * 8);
	state->phase += 4;
	state->page ^= 1;
	if(state->page != 0) {
		fdMindReadAdvanceTint();
		fdPaletteFadeSetTarget(
			RGB(state->red, state->green, state->blue) | PALETTE_TINT_CUSTOM,
			1);
		fdPaletteFadeStartInterpolation(1);
	}
	fdActorSetLayersColorMode(
		fdActorGetById(state->srcId), LAYERS_COLOR_CYCLING);

	if(state->count == 0 || state->count == MIND_READ_PARTICLE_GAP ||
		state->count == 2 * MIND_READ_PARTICLE_GAP) {
		src = fdActorGetById(state->srcId);
		target = fdActorGetById(state->targetId);
		if(src != NULL && target != NULL) {
			descriptor = (ACTOR_PLACE *)fdFindActorPlacement(state->srcId);
			spriteId = descriptor->sprite;
			from.x = src->x.fixed;
			metadata = fdResourceGetMetadataRecordStubStub(spriteId);
			from.y =
				src->y.fixed + toFIXED((Sint8)metadata->spriteTop) - toFIXED(2);
			from.z = src->z.fixed;
			sprite = target->sprite;
			descriptor = (ACTOR_PLACE *)fdFindActorPlacement(state->targetId);
			spriteId = descriptor->sprite;
			to.x = target->x.fixed;
			metadata = fdResourceGetMetadataRecordStubStub(spriteId);
			{
				Sint32 box = (Sint8)metadata->spriteTop;
				FIXED  y = target->y.fixed;

				y += toFIXED(box);
				to.y = y - toFIXED(2);
			}
			to.z = target->z.fixed;
			if(sprite->rotation == DEGtoANG(180)) {
				to.y -= toFIXED(24);
			} else if(sprite->rotation == DEGtoANG(90)) {
				to.x += toFIXED(6);
				to.y -= toFIXED(12);
			} else if(sprite->rotation == TRIG_TURN * 3 / 4) {
				to.x -= toFIXED(6);
				to.y -= toFIXED(12);
			}
			actor = fdActorCreateStub(SPRITE_CRESCENT, to.x, to.y, to.z);
			if(actor != NULL) {
				sprite = actor->sprite;
				actor->motionFlags = 0;
				actor->speedLimit = toFIXED(16) / 25; /* 0.64 pixels a tick */
				actor->acceleration = toFIXED(16) / 25;
				actor->facing = syAtan(from.z - to.z, from.x - to.x);
				actor->update = (OBJECT_UPDATE)fdMindReadUpdateCrescent;
				sprite->flags = 0;
				sprite->priority = 1;
				fdActorSetMoveTargetStub(actor, from.x, from.y, from.z);
			}
		}
	}

	if(state->count != MIND_READ_QUIET) {
		if(state->count == 0)
			sySoundPlayStub(SE_PSYNERGY_START);
		if(++state->count > MIND_READ_CYCLE_LAST)
			state->count = 0;
	}
}

/* Sets Mind Read's ripple going at each VBlank: HBlank DMA 0 gives BG0 the page
   of line offsets last written, a line at a time. */
void fdMindReadStartRipple(void)
{
	MIND_READ_STATE *state = RamHeapSlots->mindRead;
	if(!state->delay) {
		Uint16			*line = state->lines[state->page];
		volatile Uint16 *dma = REG_DMA0;
		dma[DMA0_CONTROL] &= ~DMA_START_TIMING_DRQ_REPEAT;
		dma[DMA0_CONTROL] &= ~DMA_ENABLE;
		(void)dma[DMA0_CONTROL];
		syDMAXCopy(line, REG_BG0HOFS, DMA_HBLANK_HALFWORD_PER_LINE,
			(volatile Uint32 *)dma);
	}
}

/* One colour channel's level (0 to 31) at a hue in degrees: rising from 0 to
   60, full from 60 to 180, falling from 180 to 240, else 0. */
Sint32 fdEffectHueChannelRamp(Sint16 hue)
{
	Sint32 level = 0;
	Sint16 angle = hue;

	angle = (angle + HUE_DEGREES) % HUE_DEGREES;
	if(angle < HUE_DEGREES / 6) {
		level = angle * RGB_LEVEL_MAX / (HUE_DEGREES / 6);
	} else if(angle < HUE_DEGREES / 2) {
		level = RGB_LEVEL_MAX;
	} else if(angle < HUE_DEGREES * 2 / 3) {
		level =
			(HUE_DEGREES * 2 / 3 - angle) * RGB_LEVEL_MAX / (HUE_DEGREES / 6);
	}
	return level;
}

/* Turns Mind Read's tint four degrees round the colour wheel, and takes the
   tint's colour, each channel dimmed to 4 to 11, from the hue before. */
void fdMindReadAdvanceTint(void)
{
	MIND_READ_STATE *state = RamHeapSlots->mindRead;
	Sint16			 r;
	Sint16			 g;
	Sint16			 b;
	Uint32			 limit;

	r = fdEffectHueChannelRamp(state->hue);
	g = fdEffectHueChannelRamp(state->hue + HUE_DEGREES / 3);
	b = fdEffectHueChannelRamp(state->hue + HUE_DEGREES * 2 / 3);

	state->red = (r / 4 + 4) & RGB_LEVEL_MAX;
	state->green = (g / 4 + 4) & RGB_LEVEL_MAX;
	state->blue = (b / 4 + 4) & RGB_LEVEL_MAX;
	limit = toFIXED(HUE_DEGREES - 1);
	if((Uint32)toFIXED(state->hue += 4) > limit)
		state->hue = 0;
}

/* A Mind Read crescent's update: once it has reached the caster, it goes. */
void fdMindReadUpdateCrescent(ACTOR *target)
{
	Sint32 tx;
	Sint32 ty;

	tx = target->targetX;
	if(tx == ACTOR_NO_TARGET) {
		ty = target->targetY;
		if((ty == tx) && (target->targetZ == ty)) {
			fdActorStartScriptStub(
				(ACTOR *)target, (Uint32)MindReadCrescentEndScript);
		}
	}
}

/* Gives an actor's sprite colour mode value - 1 (SPR_COL_MODE), or for
   LAYERS_COLOR_CYCLING the next of MindReadCasterColorCycle's swapped colours,
   a step every 2 ticks, as Mind Read's wave does to its caster. */
void fdActorSetLayersColorMode(void *object, Sint32 value)
{
	Sint32 child = value - 1;

	if(value == LAYERS_COLOR_CYCLING)
		child = MindReadCasterColorCycle[(TickCount >> 1) & 7];
	fdActorSetSpriteColorModeStub(object, child);
}

/* The way the +Control Pad points, in four ways: FACING_EAST to FACING_NORTH,
   or DPAD_ANGLE_NONE when none or a diagonal is held. */
Sint32 fdInputGetDpadAngle(Uint32 unused)
{
	/* Indexed by the right, left, up and down bits. */
	return InputDpadAngles[DPAD_INDEX(PerWork.data)];
}

/* The way the +Control Pad points, in eight ways, or DPAD_ANGLE_NONE. */
Uint16 fdInputGetDpadAngleEightWay(void)
{
	INPUT_STATE	 *input = &PerWork;
	const Uint16 *words = InputDpadAnglesEightWay;

	return *(Uint16 *)((Uint8 *)words + (DPAD_INDEX(input->data) << 1));
}

/* Per-tick update of Move's hand: turns it toward the actor it watches, at
   most 1/16 turn a tick, and sheds a particle (SPRITE_SPARKLE) round it. */
void fdEffectUpdateMoveHand(ACTOR *src)
{
	ACTOR  *target;
	VECTOR3 pos;
	ACTOR  *fragment;
	Sint32	turn;
	Sint32	radius;

	target = (ACTOR *)src->watchedActor;
	if(target != 0) {
		FIXED dx = target->x.fixed - src->x.fixed;
		FIXED dz = target->z.fixed - src->z.fixed;

		if(dx != 0 || dz != 0) {
			turn = (Sint16)(syAtan(dz, dx) - src->facing);
			if(turn > FACING_STEP)
				turn = FACING_STEP;
			if(turn < -FACING_STEP)
				turn = -FACING_STEP;
			src->facing += turn;
		}
		/* ACTOR_FACES_MOTION off: its motion must not undo the turn. */
		src->steeringFlags = 0;
	}

	/* The particle starts 8 to 24 pixels lower and up to 48 pixels away in a
	   random direction. */
	pos.x = src->x.fixed;
	pos.y = src->y.fixed - (syRandom() << 4) - toFIXED(8);
	pos.z = src->z.fixed;
	radius = syRandom() * 3;
	radius <<= 4;
	syVectorAddPolarOffset(radius, syRandom(), &pos);

	fragment = fdActorSpawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
	fdEffectInitFallingFragment(fragment);
}

struct PENDING_CUE;

/* When Move is cast on a target, the pushing hand appears beside it and pushes
   it 16 pixels the way the d-pad points, for as long as the player pushes; A,
   B, L or R end it. */
void fdPsynergySteerPushedTarget(Sint32 unused)
{
	VECTOR3				pos;
	VECTOR3				beyond;
	FIELD_EFFECT_STATE *scene = RamHeapSlots->fieldEffectState;
	ACTOR			   *user = scene->caster;
	ACTOR			   *target = scene->target;
	ACTOR			   *pusher;
	ACTOR			   *hit;
	FIXED				x;
	FIXED				z;
	Sint32				angle = (Uint16)scene->angle + DEGtoANG(180);
	Sint32				moved = 0;
	Sint32				direction;
	Sint32				metatileX;
	Sint32				metatileZ;
	Sint32 (*callback)(void);
	Uint8		 savedLayer = target->layer;
	PENDING_CUE *event;
	Sint32		 pushed = 0;
	const Uint8 *modes;

	if(target == NULL)
		return;
	fdPsynergyFadeToCastColor();
	user->watchedActor = (union FIELD_OBJECT *)target;
	fdActorStartScriptStub(user, FieldEffectSourceHoldScript);
	pusher = fdEffectSpawnMoveHand(user);
	if(pusher == NULL) {
		fdPsynergyFadeFromCastColor();
		return;
	}

	event = fdFindPsynergyCue(PSYNERGY_CUE(PSYNERGY_PHASE_ACTS),
		scene->fieldPsynergy, fdSelectPsynergyTarget(scene->fieldPsynergy));
	pusher->watchedActor = (union FIELD_OBJECT *)target;
	SET_POSITION(
		&pos, target->x.fixed, target->y.fixed + toFIXED(16), target->z.fixed);
	syVectorAddPolarOffset(toFIXED(16), angle, &pos);
	fdActorSetMoveTargetStub(pusher, pos.x, pos.y, pos.z);
	fdEffectGrowHandAndWait(pusher);
	pusher->speedLimit = toFIXED(4);
	pusher->acceleration = toFIXED(1) / 2;
	pusher->motionFlags = ACTOR_FLOATS;
	target->update = (OBJECT_UPDATE)fdActorCycleSpriteColors;
	target->speedLimit = toFIXED(2) / 5;
	target->acceleration = toFIXED(1) / 5;
	target->steeringFlags = moved;
	target->layer = 2;
	while(1) {
		sySynch(1);
		if((Sint8)scene->mode == 0 &&
			(PerWork.push & (KEY_A | KEY_B | KEYS_SHOULDERS)))
			break;

		direction = (Uint16)fdInputGetDpadAngle(PerWork.data);
		/* A scene's hook gives the way to push in place of the d-pad, or
		   DPAD_ANGLE_NONE to stop. */
		callback = (Sint32 (*)(void))scene->finishCallback;
		if(callback != NULL) {
			direction = callback();
			if(direction == DPAD_ANGLE_NONE)
				break;
		} else if(direction == DPAD_ANGLE_NONE) {
			SET_POSITION(&pos, target->x.fixed, target->y.fixed + toFIXED(16),
				target->z.fixed);
			syVectorAddPolarOffset(toFIXED(16), angle, &pos);
			fdActorSetMoveTargetStub(pusher, pos.x, pos.y, pos.z);
			fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_OPEN);
			pusher->velocityX = 0;
			pusher->velocityY = 0;
			pusher->velocityZ = 0;
			continue;
		}

		SET_POSITION(&pos, target->x.fixed, target->y.fixed + toFIXED(16),
			target->z.fixed);
		syVectorAddPolarOffset(toFIXED(16), angle, &pos);
		syVectorAddPolarOffset(toFIXED(2), direction, &pos);
		fdActorSetMoveTargetStub(pusher, pos.x, pos.y, pos.z);
		fdActorWaitForMoveStub(pusher);
		SET_POSITION(&pos, target->x.fixed, target->y.fixed, target->z.fixed);
		syVectorAddPolarOffset(toFIXED(16), direction, &pos);
		SET_POSITION(
			&beyond, target->x.fixed, target->y.fixed, target->z.fixed);
		syVectorAddPolarOffset(toFIXED(32), direction, &beyond);
		if(fdMapGridCheckCollisionStub(target, &pos) > 0) {
			fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
			if((TickCount & 15) == 0)
				sySoundPlayStub(SE_MENU_ERROR);
			continue;
		}
		hit = fdActorScriptFindOverlappingActorStub(target, &pos);
		if(hit != NULL) {
			if(hit != user) {
				fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
				if((TickCount & 15) == 0)
					sySoundPlayStub(SE_MENU_ERROR);
				continue;
			}
			metatileX = user->x.fixed & FIELD_METATILE_MASK;
			metatileZ = user->z.fixed & FIELD_METATILE_MASK;
			if(metatileX == (pos.x & FIELD_METATILE_MASK) &&
				metatileZ == (pos.z & FIELD_METATILE_MASK)) {
				fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
				if((TickCount & 15) == 0)
					sySoundPlayStub(SE_MENU_ERROR);
				continue;
			}
			if(metatileX == (beyond.x & FIELD_METATILE_MASK) &&
				metatileZ == (beyond.z & FIELD_METATILE_MASK)) {
				if(fdMapGridGetTerrainKindStub(
					   user->layer, beyond.x, beyond.z) != 0) {
					fdActorSelectAnimationStub(
						pusher, MOVE_HAND_ANIMATION_FLEX);
					if((TickCount & 15) == 0)
						sySoundPlayStub(SE_MENU_ERROR);
					continue;
				}
				moved = 1;
			}
		}

		sySoundPlayStub(SE_NOTE_HIGH);
		pushed = 1;
		x = pos.x;
		z = pos.z;
		modes = MoveHandPushPoses;
		/* The hand's pose for the quarter turn between its side and the
		   push. */
		fdActorSelectAnimationStub(
			pusher, modes[(Uint16)(angle - direction) / (TRIG_TURN / 4)]);
		sySynch(15);
		target->paused = 0;
		target->speedLimit = toFIXED(1) / 5;
		target->acceleration = toFIXED(1) / 5;
		fdActorSetMoveTargetStub(target, pos.x, pos.y, pos.z);
		pusher->motionFlags = 0;
		pusher->speedLimit = toFIXED(1) / 5;
		pusher->acceleration = toFIXED(1) / 5;
		syVectorAddPolarOffset(toFIXED(16), direction, &pos);
		fdActorSetMoveTargetStub(pusher, pos.x, pos.y + toFIXED(16), pos.z);
		if(moved == 1) {
			fdActorGetById(scene->casterId)->steeringFlags &=
				~ACTOR_FACES_MOTION;
			user->speedLimit = toFIXED(1) / 5;
			user->acceleration = toFIXED(1) / 5;
			fdActorSetMoveTargetStub(user, beyond.x, beyond.y, beyond.z);
		}
		fdActorWaitForMoveStub(target);
		if(event == NULL) {
			target->x.fixed = x;
			target->z.fixed = z;
			target->velocityX = 0;
			target->velocityZ = 0;
		}
		break;
	}

	target->layer = savedLayer;
	fdActorSetSpriteColorModeStub(target, scene->savedPalette);
	fdActorStartScriptStub(target, scene->savedScript);
	target->update = (OBJECT_UPDATE)scene->savedCallback;
	if(pushed) {
		pusher->update = NULL;
		fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
		fdActorSetPosAndResetMotionStub(
			pusher, pusher->x.fixed, pusher->y.fixed, pusher->z.fixed);
		fdRunPsynergyScript(event, scene->casterId, scene->targetId);
	}
	fdBattleEffectStopMainActor();
	if(moved == 1) {
		fdActorGetById(scene->casterId)->steeringFlags |= ACTOR_FACES_MOTION;
	}
	fdPsynergyFadeFromCastColor();
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
		Uint32 angle;

		pos.x = effect->originX;
		pos.z = effect->originZ;
		angle = syRandom();
		syVectorAddPolarOffset(toFIXED(30), (Uint16)angle, &pos);
		effect->targetX = pos.x;
		effect->targetZ = pos.z;
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
		effect->targetX = effect->originX;
		effect->targetZ = effect->originZ;
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
   ticks and then turns into particles. */
void fdPsynergyMove(void)
{
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	Sint32				state[3];
	FIELD_EFFECT_STATE *scene;
	void			   *object;

	scene = RamHeapSlots->fieldEffectState;
	object = scene->caster;

	fdPsynergyFadeToCastColor();
	object = fdEffectSpawnMoveHand(object);
	fdEffectGrowHandAndWait(object);
	if(object != 0) {
		fdActorSelectAnimationStub(object, MOVE_HAND_ANIMATION_FLEX);
		sySynch(30);
	}
	fdPsynergyFadeFromCastColor();
	fdEffectBurstHand(object);
}

/* Move's hand appears at a quarter size 16 pixels above the caster, with a
   crack and eight sparkles that scatter from it, and sets out 16 pixels the way
   the caster faces, to the nearest quarter turn. */
void *fdEffectSpawnMoveHand(void *object)
{
	ACTOR *src = object;
	ACTOR *parent;
	ACTOR *child;
	Sint32 angle;
	FIXED  radius;
	FIXED  speed;
	Sint32 fragmentIndex;
	Uint32 spread;
	Uint32 jitter;

	angle = (src->facing + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
	parent = fdActorSpawn(SPRITE_MOVE_HAND, src->x.fixed,
		src->y.fixed + toFIXED(16), src->z.fixed);
	if(parent == 0)
		return 0;
	parent->scaleY = toFIXED(1) / 4;
	parent->scaleX = toFIXED(1) / 4;
	parent->update = fdEffectUpdateMoveHand;
	parent->speedLimit = toFIXED(2);
	parent->acceleration = toFIXED(2);
	parent->motionFlags = 0;
	fdActorSelectAnimationStub(parent, MOVE_HAND_ANIMATION_GRIP);
	fdActorSetTargetPosFromMagnitudeAngle(parent, toFIXED(16), angle);

	for(fragmentIndex = 0; fragmentIndex < 8; fragmentIndex++) {
		child = fdActorSpawn(SPRITE_SPARKLE, src->x.fixed,
			src->y.fixed + toFIXED(16), src->z.fixed);
		if(child != 0) {
			fdActorStartScriptStub(child, FieldEffectFragmentScript);
			speed = syRandom() + toFIXED(1);
			child->acceleration = toFIXED(1);
			child->speedLimit = speed;
			child->motionFlags = ACTOR_FALLS;
			child->gravity = toFIXED(1) / 50;
			spread = syRandom();
			child->velocityY = spread - syRandom();
			radius = syRandom() * 24 + toFIXED(8);
			jitter = syRandom();
			fdActorSetTargetPosFromMagnitudeAngle(
				child, radius, ((jitter - syRandom()) >> 3) + src->facing);
		}
	}
	sySoundPlayStub(SE_ITEM_BREAK);
	return parent;
}

/* Grows a hand to full size at once and waits for it to reach where it is
   going. */
void fdEffectGrowHandAndWait(ACTOR *actor)
{
	FIXED next;
	FIXED scale;

	if(actor != NULL) {
		scale = actor->scaleX;
		if(scale < toFIXED(1)) {
			do {
				next = scale + toFIXED(1) / 16;
				scale = next;
			} while(next < toFIXED(1));
			actor->scaleX = next;
			actor->scaleY = next;
		}
		fdActorWaitForMoveStub(actor);
	}
}

#define BURST_RISE_TICKS 31
#define BURST_SPARKLES	 8

/* The hand spins as it rises for BURST_RISE_TICKS, shrinks, and bursts into
   BURST_SPARKLES sparkles (SPRITE_SPARKLE). */
void fdEffectBurstHand(void *object)
{
	ACTOR *src = object;
	ACTOR *child;
	Sint32 i;
	Sint32 distance;
	Uint32 spread;

	sySoundPlayStub(SE_CAST);
	for(i = 0; i < BURST_RISE_TICKS; i++) {
		src->y.fixed += toFIXED(1);
		src->facing += DEGtoANG(45);
		src->scaleX += -toFIXED(1) / 32;
		src->scaleY += -toFIXED(1) / 32;
		sySynch(1);
	}
	for(i = BURST_SPARKLES - 1; i >= 0; i--) {
		child = fdActorSpawn(
			SPRITE_SPARKLE, src->x.fixed, src->y.fixed, src->z.fixed);
		if(child != 0) {
			Sint32 speed;
			Sint32 unit = toFIXED(1);

			fdActorStartScriptStub(child, FieldEffectFragmentScript);
			speed = syRandom();
			child->acceleration = unit;
			child->speedLimit = speed + unit;
			child->motionFlags = ACTOR_FALLS;
			child->gravity = toFIXED(1) / 25;
			spread = syRandom();
			child->velocityY = spread - syRandom();
			distance = syRandom() * 24;
			distance += toFIXED(8);
			fdActorSetTargetPosFromMagnitudeAngle(child, distance, syRandom());
		}
	}
	sySoundPlayStub(SE_PSYNERGY_SPARKLE);
	fdActorReleaseStub(src);
}

/* Reveal's colour for the rocks it shows (sprite 200), given at its start and
   taken back at its end. Only the loop over the actors is left; value goes
   unused. */
void fdActorSetRevealSpriteColor(Sint32 value)
{
	ACTOR *actors = RamHeapSlots->actors;
	Sint32 i;

	/* DEAD: The Broken Seal's loop over the actors with what it did to a rock's
	   sprite gone, so its tests do nothing; the ROM keeps its count and its
	   branch. */
	for(i = 0; i < ACTOR_COUNT; i++) {
		ACTOR *actor = &actors[i];

		if(actor->script != NULL) {
			Uint8 kind = actor->spriteStorageKind;

			if(kind == ACTOR_SPRITE_ONE) {
			}
		}
	}
}

/* How far the leader may walk from where Reveal was cast, in screen pixels up
   and down; sideways x counts at about 0.82 of itself, so the edge is an
   ellipse about 73 pixels to either side. */
#define REVEAL_RADIUS 60

/* As The Broken Seal's, with this game's offsets: counts the timer down and,
   once the leader is REVEAL_RADIUS from the centre or the timer has run out,
   requests the effect's end. */
void fdPsynergyWatchLeaderDistance(void)
{
	FIELD_STATE *state;
	ACTOR		*actor;
	FIXED		 distanceX;
	Sint32		 distanceZ;

	state = FieldStateWork;
	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	if(state->effectTimerRunning != 0) {
		if(state->revealFrame != 0)
			state->revealFrame--;
	}

	/* In screen pixels: y is z less the height. */
	distanceX = syMulFX(
		state->revealCenterX - actor->x.fixed / toFIXED(1), toFIXED(0.81649));
	distanceZ =
		state->revealCenterY - (actor->z.fixed - actor->y.fixed) / toFIXED(1);
	if(distanceX * distanceX + distanceZ * distanceZ >=
			REVEAL_RADIUS * REVEAL_RADIUS ||
		state->revealFrame == 0) {
		/* Raise Reveal as a Psynergy request that pays no PP (ACT_PACKED |
		   ABILITY_REVEAL), which runs its ending scripts. */
		Uint16 *slot = (Uint16 *)&state->psynergy;
		Uint32	request = ABILITY_REVEAL | ACT_PACKED;

		*slot = request;
	}
}

#define REVEAL_TICKS 600 /* ten seconds of Reveal */

/* Reveal: open a circle of the transition on the caster, wash the screen plain,
   run the Reveal script that matches, then show the markers nearby and watch
   how far the leader walks. */
void fdPsynergyReveal(void)
{
	FIELD_EFFECT_STATE		*scene = RamHeapSlots->fieldEffectState;
	FIELD_STATE				*event;
	SCREEN_TRANSITION_STATE *state;
	ACTOR					*caster;
	void					*pending;
	Sint32					 frame;

	fdActorSetRevealSpriteColor(6);
	event = RamHeapSlots->fieldState;
	fdScreenTransitionStartWindowHeld(FIELD_PSYNERGY_REVEAL);
	state = RamHeapSlots->screenTransition;
	caster = scene->caster;
	state->centerX = caster->x.fixed;
	state->centerY = caster->z.fixed - caster->y.fixed;
	fdPaletteFadeSetSource(toFIXED(1), 0);
	fdPaletteFadeSetTarget(PALETTE_TINT_GRAYSCALE, 1);
	fdPaletteFadeStartInterpolation(1);
	event->revealActive = 0;
	sySynch(1);

	pending = fdFindPsynergyCue(PSYNERGY_CUE(PSYNERGY_PHASE_ACTS),
		FIELD_PSYNERGY_REVEAL, fdSelectPsynergyTarget(FIELD_PSYNERGY_REVEAL));
	if(pending != 0)
		fdRunPsynergyScript(pending, scene->casterId, scene->targetId);
	sySoundPlayStub(SE_PSYNERGY_SPARKLE);
	syIOWrite8(1, &event->revealActive);
	event->revealCenterX = state->centerX / toFIXED(1);
	event->revealCenterY = state->centerY / toFIXED(1);
	event->revealFrame = REVEAL_TICKS;
	event->effectTimerRunning = 1;
	fdEffectSpawnRevealMarkers();
	for(frame = 0; frame <= 18; frame++) {
		sySynch(1);
		state->value = frame;
	}
	if((Sint8)scene->mode == 0)
		sySetEvent(fdPsynergyWatchLeaderDistance, EVENT_PASS_TICK);
}

/* Ends Reveal if it is running: stops its watch, brings the colours back from
   black over 30 ticks and runs the scene's PSYNERGY_PHASE_FADES script. */
void fdPsynergyEndReveal(void)
{
	FIELD_EFFECT_STATE		*scene = RamHeapSlots->fieldEffectState;
	SCREEN_TRANSITION_STATE *state = RamHeapSlots->screenTransition;
	FIELD_STATE				*event = RamHeapSlots->fieldState;
	void					*pending;

	if(event->revealActive != 0) {
		sySoundPlayStub(SE_MAP_TRANSITION);
		syCloseEvent(fdPsynergyWatchLeaderDistance);
		event->revealActive = 0;
		event->revealFrame = 0;
		fdActorSetRevealSpriteColor(0);
		fdPaletteFadeSetTarget(toFIXED(1), 1);
		fdPaletteFadeStartInterpolation(1);
		fdPaletteFadeSetSource(0, 0);
		fdPaletteFadeSetTarget(toFIXED(1), 0);
		fdPaletteFadeStartInterpolation(30);
		sySynch(1);
		pending = fdFindPsynergyCue(PSYNERGY_CUE(PSYNERGY_PHASE_FADES),
			FIELD_PSYNERGY_REVEAL,
			fdSelectPsynergyTarget(FIELD_PSYNERGY_REVEAL));
		if(pending != NULL)
			fdRunPsynergyScript(pending, scene->casterId, scene->targetId);
		if(scene->sceneVariant == 0) {
			state->nextMode = 0;
			state->duration = 1;
			state->step = 1;
			sySynch(10);
		}
	}
}

/* What Growth does to its target. */
typedef enum GROWTH_TARGET_KIND {
	GROWTH_ON_PLANT,			/* it grows (its animation 2) */
	GROWTH_ON_SPROUTING_LEAVES, /* a vine hangs from it */
	GROWTH_ON_ARROW_PLANT		/* the scene's script makes it point */
} GROWTH_TARGET_KIND;

/* Growth cast on a target: the burst of particles, then the target takes its
   grown animation and the underfoot priority, with SE_RECOVERY. Sprouting
   leaves (SPRITE_SPROUTING_LEAVES) instead drop five vines round the Psynergy's
   position and hang a vine (fdHangingVineHang). On an arrow plant Growth's own
   effect only plays SE_RECOVERY; the scene's script turns it into an arrow
   (scGaiaRockInsidePlantPointsTheWay). */
void fdPsynergyGrowthOnTarget(Sint32 unused)
{
	FIELD_EFFECT_STATE *scene = RamHeapSlots->fieldEffectState;
	ACTOR			   *target = scene->target;
	Sint32				kind = GROWTH_ON_PLANT;

	if(target == 0)
		return;

	fdEffectSpawnBurstParticleField();
	if(fdActorGetSpriteIdStub(target) == SPRITE_SPROUTING_LEAVES)
		kind = GROWTH_ON_SPROUTING_LEAVES;
	if(fdActorGetSpriteIdStub(target) == SPRITE_ARROW_PLANT)
		kind = GROWTH_ON_ARROW_PLANT;

	if(kind == GROWTH_ON_PLANT) {
		fdActorSelectAnimationStub(target, 2);
		fdActorSetSpriteFlagsStub(target, 0);
		target->collisionFlags = 0;
		target->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
		sySynch(10);
		sySoundPlayStub(SE_RECOVERY);
		sySynch(40);
	} else if(kind == GROWTH_ON_SPROUTING_LEAVES) {
		BURST_PARTICLE_VECTOR pos;
		Sint32				  n;

		for(n = 0; n < 5; n++) {
			ACTOR *piece;
			FIXED  distance;

			pos.values[0] = scene->x;
			pos.values[2] = scene->z;
			distance = syRandom() * 6 + toFIXED(4);
			syVectorAddPolarOffset(distance, syRandom(), pos.values);
			pos.values[1] = scene->y;
			piece = fdActorSpawn(
				SPRITE_VINE, pos.values[0], pos.values[1], pos.values[2]);
			if(piece != 0) {
				fdActorStartScriptStub(piece, GrowthLeafScript);
				piece->motionFlags = ACTOR_FALLS;
			}
			sySynch((syRandomBelow(2)) + 2);
		}
		fdHangingVineHang(scene->targetId, 1);
	} else {
		sySynch(10);
		sySoundPlayStub(SE_RECOVERY);
	}
	fdPsynergyFadeFromCastColor();
}

/* When Growth is cast with no target, particles fly out from in front of the
   caster, and then five vine leaves drop 4 to 10 pixels round the Psynergy's
   position. */
void fdPsynergyGrowth(void)
{
	FIELD_EFFECT_STATE	 *state = RamHeapSlots->fieldEffectState;
	BURST_PARTICLE_VECTOR pos;
	Sint32				  entryCount;

	fdEffectSpawnBurstParticleField();
	sySoundPlayStub(SE_HEAVY_IMPACT);
	for(entryCount = 0; entryCount < 5; entryCount++) {
		void  *object;
		Sint32 randomValue;

		pos.values[0] = state->x;
		pos.values[2] = state->z;
		randomValue = (syRandom() * 6) + toFIXED(4);
		syVectorAddPolarOffset(randomValue, syRandom(), pos.values);
		pos.values[1] = state->y;
		object = fdActorSpawn(
			SPRITE_VINE, pos.values[0], pos.values[1], pos.values[2]);
		if(object != 0) {
			fdActorStartScriptStub(object, GrowthLeafScript);
			((ACTOR *)object)->motionFlags = ACTOR_FALLS;
		}
		sySynch((syRandomBelow(2)) + 2);
	}
	sySynch(30);
	fdPsynergyFadeFromCastColor();
}

#define BURST_FIELD_PARTICLES 24

/* Sends BURST_FIELD_PARTICLES particles, one every 2 ticks, from in front of
   the caster to points round the Psynergy's position. */
void fdEffectSpawnBurstParticleField(void)
{
	FIELD_EFFECT_STATE *state;
	ACTOR			   *user;
	FIXED				pos[3];
	Sint32				n;
	Sint32			   *p;

	state = RamHeapSlots->fieldEffectState;
	user = state->caster;
	fdPsynergyFadeToCastColor();

	p = pos;
	for(n = 0; n < BURST_FIELD_PARTICLES; n++) {
		ACTOR				  *object;
		BURST_PARTICLE_VISUAL *visual;
		BURST_PARTICLE_VISUAL *child;
		FIXED				   distance;

		if(*(Uint16 *)&state->angle == DEGtoANG(90)) {
			pos[0] = user->x.fixed;
			pos[1] = user->y.fixed + toFIXED(10);
			pos[2] = user->z.fixed;
		} else if(*(Uint16 *)&state->angle == TRIG_TURN * 3 / 4) {
			pos[0] = user->x.fixed;
			pos[1] = user->y.fixed + toFIXED(24);
			pos[2] = user->z.fixed;
		} else {
			pos[0] = user->x.fixed;
			pos[1] = user->y.fixed + toFIXED(10);
			pos[2] = user->z.fixed;
			syVectorAddPolarOffset(toFIXED(10), *(Uint16 *)&state->angle, pos);
		}

		object = fdActorSpawn(SPRITE_PARTICLE, pos[0], pos[1], pos[2]);
		visual = &((BURST_PARTICLE_VISUAL_GROUP *)object->sprite)->primary;
		child = &((BURST_PARTICLE_VISUAL_GROUP *)object->sprite)->child;
		child->fullColor = visual->fullColor;
		child->size = visual->size;
		child->shape = visual->shape;
		child->attributes.bits.tile = visual->attributes.bits.tile;
		child->attributes.bytes.palette = visual->attributes.bytes.palette;

		if(object != 0) {
			object->scaleY = toFIXED(0.7);
			object->scaleX = toFIXED(0.7);
			object->acceleration = toFIXED(3) / 2;
			object->speedLimit = toFIXED(3) / 2;
			object->motionFlags = 0;
			fdActorSetSpriteColorModeStub(object, SPRITE_COLOR_GREEN_TINT);
			fdActorSelectAnimationStub(object, ANIM_PARTICLE_TWINKLE);
			fdActorStartScriptStub(object, FieldEffectReleaseScript + 1);

			fdActorSetSpriteFlagsStub(object, SPRITE_FLAG_SHADOW);

			p[0] = state->x;
			p[1] = state->y;
			p[2] = state->z;
			if(*(Uint16 *)&state->angle == TRIG_TURN * 3 / 4)
				syVectorAddPolarOffset(
					toFIXED(14), *(Uint16 *)&state->angle, p);
			distance = syRandom() * 6 + toFIXED(4);
			syVectorAddPolarOffset(distance, syRandom(), p);
			fdActorSetMoveTargetStub(object, p[0], p[1], p[2]);
		}
		sySoundPlayStub(SE_PSYNERGY_SPARKLE);
		sySynch(2);
	}
	sySynch(8);
}

/* A hand appears 32 pixels to either side, each moves 14 pixels in, and they
   lift the target 32 pixels with them. */
void fdPsynergyLiftOnTarget(void)
{
	FIELD_EFFECT_STATE *effectState;
	ACTOR			   *caster;
	void			   *child;
	void			   *anchors[2];
	FIXED				pos[3];
	Sint32				x;
	FIXED				y;
	Sint32				z;
	Sint32				index;
	Sint32				startY;

	effectState = RamHeapSlots->fieldEffectState;
	child = effectState->target;
	caster = effectState->caster;
	if(child == 0)
		return;

	fdPsynergyFadeToCastColor();
	caster->watchedActor = child;
	fdActorStartScriptStub(caster, FieldEffectSourceHoldScript);

	x = effectState->x;
	pos[0] = x;
	y = effectState->y + toFIXED(16);
	pos[1] = y;
	z = effectState->z;
	pos[2] = z;
	anchors[0] = fdEffectSpawnLiftHand(x + toFIXED(32), y, z, FACING_WEST);
	anchors[1] = fdEffectSpawnLiftHand(
		pos[0] - toFIXED(32), pos[1], pos[2], FACING_EAST);

	sySynch(15);
	for(index = 0; index < 2; index++) {
		void *anchor = anchors[index];
		if(anchor != 0)
			fdActorSetTargetPosFromMagnitudeAngle(
				anchor, toFIXED(14), ((ACTOR *)anchor)->facing);
	}

	fdActorWaitForMoveStub(anchors[0]);
	((EFFECT_CHILD *)child)->callback =
		(void (*)(void))fdActorCycleSpriteColors;
	sySoundPlayStub(SE_PSYNERGY_START);
	((EFFECT_CHILD *)child)->flags = 4;

	startY = ((EFFECT_CHILD *)child)->y;
	if(anchors[0] != 0 && anchors[1] != 0 && startY <= startY + toFIXED(32)) {
		do {
			((ACTOR *)anchors[0])->y.fixed += toFIXED(1) / 4;
			((ACTOR *)anchors[1])->y.fixed += toFIXED(1) / 4;
			((EFFECT_CHILD *)child)->y += toFIXED(1) / 4;
			sySynch(1);
		} while(((EFFECT_CHILD *)child)->y <= startY + toFIXED(32));
	}

	fdEffectBurstHand(anchors[0]);
	fdEffectBurstHand(anchors[1]);
	fdPsynergyFadeFromCastColor();
}

/* Lift cast with no target: two hands appear 32 pixels to either side of the
   spot, close 24 pixels in and meet with a crash that scatters
   ITEM_BREAK_SHARD_MAX sparkles, then vanish. */
void fdPsynergyLift(void)
{
	FIELD_EFFECT_STATE *state;
	ACTOR			   *object;
	ACTOR			   *pair[2];
	VECTOR3				pos;
	ACTOR			  **walk;
	Sint32				n;
	FIXED				distance;

	state = RamHeapSlots->fieldEffectState;
	fdPsynergyFadeToCastColor();
	SET_POSITION(&pos, state->x, state->y + toFIXED(16), state->z);
	pair[0] =
		fdEffectSpawnLiftHand(pos.x + toFIXED(32), pos.y, pos.z, DEGtoANG(180));
	pair[1] = fdEffectSpawnLiftHand(pos.x - toFIXED(32), pos.y, pos.z, 0);

	sySynch(15);
	walk = pair;
	for(n = 0; n < 2; n++) {
		object = *walk++;
		if(object != 0)
			fdActorSetTargetPosFromMagnitudeAngle(
				object, toFIXED(24), object->facing);
	}

	fdActorWaitForMoveStub(pair[0]);
	sySoundPlayStub(SE_HEAVY_IMPACT);
	for(n = 0; n < ITEM_BREAK_SHARD_MAX; n++) {
		SET_POSITION(&pos, state->x, state->y + toFIXED(16), state->z);
		object = fdActorSpawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
		if(object != 0) {
			fdActorStartScriptStub(object, FieldEffectFragmentScript);
			object->speedLimit = syRandom() + toFIXED(2);
			object->acceleration = toFIXED(2);
			object->motionFlags = 0;
			distance = syRandom() * 24 + toFIXED(8);
			fdActorSetTargetPosFromMagnitudeAngle(object, distance, syRandom());
		}
	}

	fdActorReleaseStub(pair[0]);
	fdActorReleaseStub(pair[1]);
	fdPsynergyFadeFromCastColor();
}

/* Spawns one of Lift's hands (SPRITE_MOVE_HAND) and grows it from a quarter to
   full size, an eighth of a turn a tick, then faces it to angle. */
ACTOR *fdEffectSpawnLiftHand(Sint32 x, Sint32 y, Sint32 z, Sint32 angle)
{
	ACTOR *actor;
	Sint32 scale;

	sySoundPlayStub(SE_ITEM_BREAK);
	actor = fdActorSpawn(SPRITE_MOVE_HAND, x, y, z);
	if(actor != NULL) {
		actor->scaleX = actor->scaleY = toFIXED(1) / 4;
		actor->acceleration = actor->speedLimit = toFIXED(3);
		((SPR *)actor->sprite)->image.priority = 0;
		fdActorSelectAnimationStub(actor, MOVE_HAND_ANIMATION_GRIP);
		scale = actor->scaleX;
		if(scale < toFIXED(1)) {
			do {
				scale += toFIXED(1) / 32;
				actor->scaleY = scale;
				actor->scaleX = scale;
				actor->facing += DEGtoANG(45);
				sySynch(1);
				scale = actor->scaleX;
			} while(scale < toFIXED(1));
		}
		actor->facing = angle;
	}
	return actor;
}

/* Field effect sprite callback of Force's particles (OUT_AND_BACK_STATE): each
   flies 25 pixels from where it appeared, then back to a point 8 pixels toward
   the caster's facing from 16 pixels above it, on screen, give or take 4
   pixels, and is freed on arrival. */
void fdEffectUpdateRadialBurst(FIELD_PARTICLE *effect)
{
	/* OUT_AND_BACK_FLYING_OUT must enter OUT_AND_BACK_AIM_BACK immediately. */
	FIELD_EFFECT_STATE *effectState = RamHeapSlots->fieldEffectState;
	VECTOR3				pos;
	Sint32				state;

again:
	state = effect->state;
	if(state == OUT_AND_BACK_AIM_OUT) {
		/* 25 pixels from the origin, in a random direction. */
		pos.x = effect->originX;
		pos.z = effect->originZ;
		syVectorAddPolarOffset(toFIXED(25), (Uint16)syRandom(), &pos);
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
		/* 8 pixels toward effectState->angle from 16 pixels above the main
		   actor, on screen, give or take 4 pixels. */
		ACTOR *caster = effectState->caster;

		pos.x = caster->x.fixed;
		pos.y = caster->y.fixed + toFIXED(16);
		pos.z = caster->z.fixed;
		syVectorAddPolarOffset(toFIXED(8), effectState->angle, &pos);
		fdCameraWorldToScreen(&pos);
		syVectorAddPolarOffset(toFIXED(4), syRandom(), &pos);
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

#define IMPACT_SHARDS 12

/* A heavy impact, as The Broken Seal's: an actor placed below the source for a
   moment, scattering IMPACT_SHARDS shards from it. */
Sint32 fdEffectSpawnHeavyImpact(ACTOR *src)
{
	VECTOR3 pos;
	ACTOR  *impact;
	ACTOR  *fragment;
	Sint32	fragmentIndex;

	sySoundPlayStub(SE_HEAVY_IMPACT);
	pos.x = src->x.fixed;
	pos.y = src->y.fixed;
	pos.z = src->z.fixed;
	impact = fdActorSpawn(SPRITE_STAR_BURST, pos.x, pos.y - toFIXED(32), pos.z);
	if(impact != NULL) {
		impact->motionFlags = 0;
		impact->wait = 20;
		fdActorStartScriptStub(impact, FieldEffectReleaseScript);
	}

	/* Shards: 1 to 2 pixels a tick, 8 to 32 pixels in a random direction. */
	for(fragmentIndex = 0; fragmentIndex < IMPACT_SHARDS; fragmentIndex++) {
		fragment = fdActorSpawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
		if(fragment != NULL) {
			fdActorStartScriptStub(fragment, FieldEffectFragmentScript);
			fragment->speedLimit = syRandom() + toFIXED(1);
			fragment->acceleration = toFIXED(1);
			fragment->motionFlags = 0;
			fdActorSetTargetPosFromMagnitudeAngle(
				fragment, syRandom() * 24 + toFIXED(8), syRandom());
		}
	}
	return 0;
}

/* Runs Force cast on a target, through fdPsynergyForce. */
void fdPsynergyForceOnTarget(Sint32 unused)
{
	fdPsynergyForce();
}

/* Force's ring of particles round the caster, one every 2 ticks, and the
   ticks it waits at most for the fist to strike. */
#define FORCE_RING_PARTICLES 12
#define FORCE_STRIKE_TICKS	 60

/* Force: a ring of particles round the caster, then a fist (SPRITE_MOVE_HAND)
   grows in front of them and strikes 56 pixels ahead, or at the target, with
   three afterimages; the target is pushed 16 pixels the way the fist flew (and
   thrown up when its option asks), and the scene's Force script runs. */
void fdPsynergyForce(void)
{
	ACTOR			   *copies[4];
	VECTOR3				pos;
	FIELD_EFFECT_STATE *scene;
	ACTOR			   *target;
	ACTOR			   *src;
	ACTOR			   *actor;
	ACTOR			   *copy;
	FIELD_PARTICLE	   *slot;
	SPR				   *resource;
	PENDING_CUE		   *event;
	Sint32				scale;
	Sint32				i;
	Uint8				marker;

	scene = RamHeapSlots->fieldEffectState;
	target = scene->target;
	fdPsynergyFadeToCastColor();
	sySoundPlayStub(SE_PSYNERGY_START);
	slot = scene->particles;

	for(i = 0; i < FORCE_RING_PARTICLES; i++) {
		src = scene->caster;
		SET_POSITION(
			&pos, src->x.fixed, src->y.fixed + toFIXED(16), src->z.fixed);
		fdCameraWorldToScreen(&pos);
		fdParticleInit(slot, SPRITE_PARTICLE, pos.x, pos.z);
		fdParticleSetCallback(slot, fdEffectUpdateRadialBurst);
		fdParticleSetAnimation(slot, ANIM_PARTICLE_TWINKLE);
		fdSpriteSetAnimationColorModesStub(
			slot->sprite, SPRITE_COLOR_BLUE_TINT);
		slot->scaleY = toFIXED(7) / 10;
		slot->scaleX = toFIXED(7) / 10;
		sySynch(2);
		slot++;
	}

	src = scene->caster;
	SET_POSITION(&pos, src->x.fixed, src->y.fixed + toFIXED(16), src->z.fixed);
	syVectorAddPolarOffset(toFIXED(8), scene->angle, &pos);
	actor = fdActorSpawn(SPRITE_MOVE_HAND, pos.x, pos.y, pos.z);
	if(actor == NULL) {
		fdPsynergyFadeFromCastColor();
		return;
	}
	actor->scaleY = toFIXED(1) / 4;
	actor->scaleX = toFIXED(1) / 4;
	actor->facing = scene->angle;
	actor->speedLimit = toFIXED(4);
	actor->acceleration = toFIXED(4);
	actor->motionFlags = 0;
	fdActorSelectAnimationStub(actor, MOVE_HAND_ANIMATION_FIST);
	fdActorSetSpriteColorModeStub(actor, SPRITE_COLOR_BGR);
	scale = actor->scaleX;
	if(scale < toFIXED(1)) {
		do {
			scale += toFIXED(5) / 256;
			actor->scaleY = scale;
			actor->scaleX = scale;
			sySynch(1);
			scale = actor->scaleX;
		} while(scale < toFIXED(1));
	}
	sySynch(3);

	resource = NULL;
	for(i = 2; i >= 0; i--) {
		copy = copies[i] = fdActorSpawn(
			SPRITE_MOVE_HAND, actor->x.fixed, actor->y.fixed, actor->z.fixed);
		if(copy != NULL) {
			copy->scaleY = AFTERIMAGE_SCALE;
			copy->scaleX = AFTERIMAGE_SCALE;
			copy->facing = scene->angle;
			copy->speedLimit = toFIXED(4);
			copy->acceleration = toFIXED(4);
			copy->motionFlags = 0;
			fdActorSelectAnimationStub(copy, MOVE_HAND_ANIMATION_FIST);
			fdActorSetSpriteColorModeStub(copy, SPRITE_COLOR_RBG);
			resource = fdSpriteShareVRAMCacheEntry(copy->sprite, resource);
		}
	}
	marker = resource->slot;

	if(scene->aimAhead != 0) {
		src = scene->caster;
		SET_POSITION(
			&pos, src->x.fixed, src->y.fixed + toFIXED(16), src->z.fixed);
		syVectorAddPolarOffset(toFIXED(56), scene->angle, &pos);
	} else {
		SET_POSITION(&pos, scene->x, scene->y + toFIXED(16), scene->z);
	}
	fdActorSetMoveTargetStub(actor, pos.x, pos.y, pos.z);
	fdActorStartScriptStub(actor, ForceFistScript);
	for(i = 0; i < 3; i++) {
		copy = copies[i];
		if(copy != NULL) {
			sySynch(3);
			fdActorSetMoveTargetStub(copy, pos.x, pos.y, pos.z);
			fdActorStartScriptStub(copy, FieldEffectReleaseScript + 1);
		}
	}

	for(i = 0; i < FORCE_STRIKE_TICKS && actor->script != NULL; i++)
		sySynch(1);

	if(target != NULL && scene->childMode == 0) {
		if(scene->sceneVariant != 0)
			target->velocityY = toFIXED(8);
		SET_POSITION(&pos, target->x.fixed, target->y.fixed, target->z.fixed);
		syVectorAddPolarOffset(toFIXED(16), scene->angle, &pos);
		if(fdMapGridCheckCollisionStub(target, &pos) == 0 &&
			fdActorScriptCheckOverlapStub(target, &pos) == 0) {
			target->acceleration = toFIXED(1);
			target->speedLimit = toFIXED(1);
			fdActorSetMoveTargetStub(target, pos.x, pos.y, pos.z);
		}
	}

	event = fdFindPsynergyCue(PSYNERGY_CUE(PSYNERGY_PHASE_ACTS),
		FIELD_PSYNERGY_FORCE, fdSelectPsynergyTarget(FIELD_PSYNERGY_FORCE));
	if(event != NULL)
		fdRunPsynergyScript(event, scene->casterId, scene->targetId);
	sySynch(10);
	fdPsynergyFadeFromCastColor();
	sySynch(20);
	/* VRAM_CACHE_ENTRY_COUNT: no afterimage took a cache entry. */
	if(marker != VRAM_CACHE_ENTRY_COUNT)
		syVRAMCacheFreeEntry(marker);
}

struct ORBITING_PARTICLE;

/* Per-tick update of a Frost glint: of every eight ticks, two show the
   particle in swapped colours (SPRITE_COLOR_RBG). */
Sint32 fdEffectUpdateFrostGlint(struct ORBITING_PARTICLE *particle)
{
	Sint32 phase = TickCount & 7;

	if(phase == 0) {
		fdActorSetSpriteColorModeStub((ACTOR *)particle, SPRITE_COLOR_RBG);
	} else if(phase == 2) {
		fdActorSetSpriteColorModeStub((ACTOR *)particle, SPRITE_COLOR_NORMAL);
	}
	return 0;
}

/* Per-tick update: shrinks by 1/16 a tick and is released at 1/16. */
void fdEffectUpdateOrbitingParticleFade(ACTOR *particle)
{
	FIXED scale;

	if(particle != NULL) {
		scale = particle->scaleX - toFIXED(1) / 16;
		particle->scaleY -= toFIXED(1) / 16;
		particle->scaleX = scale;
		if(scale <= toFIXED(1) / 16)
			fdActorStartScriptStub(particle, FieldEffectReleaseScript);
	}
}

/* Spirals anticlockwise at twice the ticks left in pixels, then fades. */
void fdEffectUpdateOrbitingParticleLeft(ACTOR *particle)
{
	VECTOR3 pos;
	Sint16	life;
	Sint32	count;

	if(particle != NULL) {
		count = (Uint16)particle->timer - 1;
		particle->timer = count;
		life = count;
		if(life != 0) {
			SET_POSITION(
				&pos, particle->targetX, particle->targetY, particle->targetZ);
			syVectorAddPolarOffset(life * toFIXED(2),
				particle->userValue + life * (TRIG_TURN / 32), &pos);
			SET_ACTOR_POSITION(particle, pos.x, pos.y, pos.z);
		} else {
			particle->update =
				(OBJECT_UPDATE)fdEffectUpdateOrbitingParticleFade;
		}
	}
}

/* Per-tick update: as fdEffectUpdateOrbitingParticleLeft, clockwise. */
void fdEffectUpdateOrbitingParticleRight(ACTOR *particle)
{
	VECTOR3 pos;
	Sint16	life;
	Sint32	count;

	if(particle != NULL) {
		count = (Uint16)particle->timer - 1;
		particle->timer = count;
		life = count;
		if(life != 0) {
			SET_POSITION(
				&pos, particle->targetX, particle->targetY, particle->targetZ);
			syVectorAddPolarOffset(life * toFIXED(2),
				particle->userValue - life * (TRIG_TURN / 32), &pos);
			SET_ACTOR_POSITION(particle, pos.x, pos.y, pos.z);
		} else {
			particle->update =
				(OBJECT_UPDATE)fdEffectUpdateOrbitingParticleFade;
		}
	}
}

/* Frost cast on a target. A target with a mode set takes only the snowflakes,
   and its flash and freezing are left to the scene. */
void fdPsynergyFrostOnTarget(Sint32 unused)
{
	FIELD_EFFECT_STATE *effectState = RamHeapSlots->fieldEffectState;
	ACTOR			   *target = effectState->target;

	if(target != 0) {
		if(effectState->childMode != 0) {
			effectState->aimAhead = 1;
		}
		target->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
		fdPsynergyFrost();
	}
}

#define FROST_SNOWFLAKES 16 /* circling the target */

/* FROST_SNOWFLAKES snowflakes, one every three ticks, spiral in on points 2 to
   6 pixels round the Psynergy's position; a target then flashes white eight
   times, freezes (its animation 2, unless its option says otherwise), glints
   from then on, and the scene's Frost script runs. */
void fdPsynergyFrost(void)
{
	VECTOR3				pos;
	FIELD_EFFECT_STATE *scene;
	ACTOR			   *target;
	ACTOR			   *particle;
	PENDING_CUE		   *event;
	Sint32				i;

	scene = RamHeapSlots->fieldEffectState;
	target = scene->target;
	fdPsynergyFadeToCastColor();
	sySoundPlayStub(SE_ICE);
	for(i = 0; i < FROST_SNOWFLAKES; i++) {
		particle = fdActorSpawn(SPRITE_SNOWFLAKE, 0, 0, 0);
		if(particle != NULL) {
			Uint32 scale;
			FIXED  distance;

			scale = (syRandom() >> 1) + toFIXED(1) / 2;
			particle->scaleY = scale;
			particle->scaleX = scale;
			if((syRandom() & 1) != 0)
				particle->update =
					(OBJECT_UPDATE)fdEffectUpdateOrbitingParticleLeft;
			else
				particle->update =
					(OBJECT_UPDATE)fdEffectUpdateOrbitingParticleRight;

			particle->facing = syRandom();
			particle->timer = 60;
			particle->userValue = syRandom();
			fdActorSetSpriteColorModeStub(
				(ACTOR *)particle, SPRITE_COLOR_BLUE_TINT);

			SET_POSITION(&pos, scene->x, scene->y, scene->z);
			distance = (syRandom() << 2) + toFIXED(2);
			syVectorAddPolarOffset(distance, syRandom(), &pos);
			particle->targetX = pos.x;
			particle->targetY = pos.y;
			particle->targetZ = pos.z;
		}

		sySynch(3);
	}

	sySynch(10);
	sySoundPlayStub(SE_ICE);
	sySynch(50);

	if(target != NULL && scene->aimAhead == 0) {
		sySoundPlayStub(SE_POWER_UP);

		for(i = 0; i < 8; i++) {
			fdActorSetSpriteColorModeStub((ACTOR *)target, SPRITE_COLOR_WHITE);
			sySynch(1);
			fdActorSetSpriteColorModeStub((ACTOR *)target, SPRITE_COLOR_NORMAL);
			sySynch(4);
		}

		if(scene->sceneVariant == 0) {
			sySoundPlayStub(SE_SHIMMER);
			fdActorSelectAnimationStub(target, 2);
		}

		target->update = (OBJECT_UPDATE)fdEffectUpdateFrostGlint;
		event = fdFindPsynergyCue(PSYNERGY_CUE(PSYNERGY_PHASE_ACTS),
			FIELD_PSYNERGY_FROST, fdSelectPsynergyTarget(FIELD_PSYNERGY_FROST));
		if(event != NULL)
			fdRunPsynergyScript(event, scene->casterId, scene->targetId);
		sySynch(20);
	}

	fdPsynergyFadeFromCastColor();
}

/* Per-tick update: sways 4 pixels either side of targetX, once every 128
   ticks. */
void fdEffectSwayParticle(ACTOR *actor)
{
	FIXED amplitude = toFIXED(4);

	actor->x.fixed = actor->targetX +
		syMulFX(amplitude, sySin(actor->timer * (TRIG_TURN / 128)));
	actor->timer++;
	actor->timer = (actor->timer + 128) % 128;
}

/* The particle circles 10 pixels above the scene's position, its radius in
   pixels the ticks it has left; at 0 it runs the shared particle script. */
void fdEffectUpdateShrinkingOrbitActor(ACTOR *particle)
{
	FIELD_EFFECT_STATE *effectState = RamHeapSlots->fieldEffectState;
	VECTOR3				pos;
	Sint16				remainingTicks;
	Sint32				decrementedTimer;

	if(particle != NULL) {
		decrementedTimer = (Uint16)particle->timer - 1;
		particle->timer = decrementedTimer;
		remainingTicks = decrementedTimer;
		if(remainingTicks != 0) {
			/* The angle is 1/32 turn further round for each tick left. */
			pos.x = effectState->x;
			pos.y = effectState->y + toFIXED(10);
			pos.z = effectState->z;
			syVectorAddPolarOffset(toFIXED(remainingTicks),
				particle->userValue + remainingTicks * (TRIG_TURN / 32), &pos);
			particle->x.fixed = pos.x;
			particle->y.fixed = pos.y;
			particle->z.fixed = pos.z;
		} else {
			fdActorStartScriptStub(particle, FieldEffectReleaseScript);
		}
	}
}

/* Per-tick update: sways 6 pixels either side (a 64-tick cycle) 16 pixels
   above the Psynergy's position, and every 3 ticks sheds a spark that sways as
   it slowly falls. */
void fdEffectRunSparkEmitter(ACTOR *emitter)
{
	FIELD_EFFECT_STATE *effectState;
	ACTOR			   *spark;
	FIXED				pos[3];

	effectState = RamHeapSlots->fieldEffectState;
	if(emitter->timer != -1) {
		FIXED amplitude = toFIXED(6);

		SET_ACTOR_POSITION(emitter,
			effectState->x +
				syMulFX(amplitude, sySin(emitter->timer * (TRIG_TURN / 64))),
			effectState->y + toFIXED(16), effectState->z);
		emitter->timer++;
		emitter->timer = (emitter->timer + 64) % 64;
	}
	if(TickCount % 3 == 0) {
		/* From 2 pixels up, 0 to 6 pixels in a random direction. */
		pos[0] = emitter->x.fixed;
		pos[1] = emitter->y.fixed + toFIXED(2);
		pos[2] = emitter->z.fixed;
		syVectorAddPolarOffset(syRandom() * 6, syRandom(), pos);
		spark = fdActorSpawn(SPRITE_SPARKLE, pos[0], pos[1], pos[2]);
		/* Each is released after 72 ticks, its script's wait. */
		if(spark != 0) {
			spark->update = (OBJECT_UPDATE)fdEffectSwayParticle;
			spark->scaleX = spark->scaleY = toFIXED(3) / 5;
			spark->motionFlags = ACTOR_FALLS;
			spark->gravity = toFIXED(0.007);
			spark->timer = syRandom() >> 9;
			spark->targetX = spark->x.fixed;
			fdActorSetSpriteColorModeStub(spark, SPRITE_COLOR_BLUE_TINT);
			spark->wait = 72;
			fdActorStartScriptStub(spark, FieldEffectReleaseScript);
		}
	}
}

/* Ply cast on a target: runs Ply, then stops the caster's hook. */
void fdPsynergyPlyOnTarget(void)
{
	fdPsynergyPly();
	fdBattleEffectStopMainActor();
}

/* Ply's flashing figure: its steps, of 2 ticks each. */
#define PLY_FLASH_STEPS 30

/* Ply: eight winged figures spiral in on the Psynergy's position, one a tick;
   then one in the centre flashes for 60 ticks, sheds sparks and flies off. */
void fdPsynergyPly(void)
{
	FIELD_EFFECT_STATE *effectState = RamHeapSlots->fieldEffectState;
	ACTOR			   *target = effectState->caster;
	ACTOR			   *actor;
	SPR				   *last;
	Uint8				resourceSlot;
	Sint32				i;

	fdPsynergyFadeToCastColor();

	/* The spiralling particles: one a tick, each 1/8 turn on from the last. */
	last = 0;
	for(i = 0; i < 8; i++) {
		actor = fdActorSpawn(SPRITE_WINGED_FIGURE, target->x.fixed,
			target->y.fixed + toFIXED(32), target->z.fixed);
		if(actor != 0) {
			actor->scaleY = toFIXED(7) / 10;
			actor->scaleX = toFIXED(7) / 10;
			actor->update = (OBJECT_UPDATE)fdEffectUpdateShrinkingOrbitActor;
			actor->timer = 120;
			actor->userValue = i * (TRIG_TURN / 8);
			actor->motionFlags = ACTOR_FLOATS;
			last = fdSpriteShareVRAMCacheEntry(actor->sprite, last);
		}
		sySynch(1);
	}

	/* The particles share one sprite cache; keep its slot to free at the end.
	   last is read with no check that any particle spawned. */
	resourceSlot = last->slot;
	sySoundPlayStub(SE_PSYNERGY_START);
	/* The last particle spawned 7 ticks ago with 120 ticks to live: wait
	   until the spiral is nearly closed. */
	sySynch(110);

	/* The one in the centre. */
	target = fdActorSpawn(SPRITE_WINGED_FIGURE, 0, 0, 0);
	/* FAKEMATCH(temporary): actor, a copy of target, the figure read by both
	 * names. */
	actor = target;
	if(target != 0) {
		target->scaleY = toFIXED(7) / 10;
		target->scaleX = toFIXED(7) / 10;
		target->x.fixed = effectState->x;
		target->y.fixed = effectState->y + toFIXED(16);
		target->z.fixed = effectState->z;
		target->motionFlags = ACTOR_FLOATS;
		fdActorSetSpriteColorModeStub(target, SPRITE_COLOR_WHITE);
	}

	sySoundPlayStub(SE_PSYNERGY_SPARKLE);
	sySynch(12);
	if(target != 0) {
		/* Red one step in four, else blue, a step each 2 ticks. */
		for(i = 0; i < PLY_FLASH_STEPS; i++) {
			/* FAKEMATCH(dead): target set again to actor at each step; as the
			 * loop without it. */
			target = actor;
			if(i & 3)
				fdActorSetSpriteColorModeStub(actor, SPRITE_COLOR_BLUE_TINT);
			else
				fdActorSetSpriteColorModeStub(actor, SPRITE_COLOR_RED_TINT);
			sySynch(2);
		}
	}

	/* Sparks. */
	fdActorSetSpriteColorModeStub(actor, SPRITE_COLOR_NORMAL);
	sySoundPlayStub(MUS_POISON_CURED);
	if(actor != 0) {
		target->update = (OBJECT_UPDATE)fdEffectRunSparkEmitter;
		target->timer = 0;
		/* Cast on a target (enabled cleared by fdPsynergyDispatch), the sparks
		   fall for 192 ticks instead of 128. */
		if(effectState->aimAhead != 0)
			sySynch(128);
		else
			sySynch(192);
	}

	/* Fly away: 192 pixels at angle TRIG_TURN * 29 / 32, and release it once it
	   arrives. */
	if(target != 0) {
		target->timer = -1;
		target->speedLimit = toFIXED(5);
		target->acceleration = toFIXED(2) / 5;
		target->steeringFlags = 0;
		fdActorSetTargetPosFromMagnitudeAngle(
			target, toFIXED(192), TRIG_TURN * 29 / 32);
		fdActorWaitForMoveStub(target);
		fdActorReleaseStub(target);
	}
	/* syVRAMCacheFindFreeEntry returns VRAM_CACHE_ENTRY_COUNT for no entry. */
	if(resourceSlot != VRAM_CACHE_ENTRY_COUNT)
		syVRAMCacheFreeEntry(resourceSlot);
	if(effectState->finishCallback != 0)
		effectState->finishCallback();
	fdPsynergyFadeFromCastColor();
}

/* An event of Cloak: when the controlled character does not stand on a tile
   Cloak can hide on, requests CLOAK_END (Cloak, packed). */
void fdEffectCheckCloakTile(void)
{
	FIELD_STATE *event = RamHeapSlots->fieldState;
	ACTOR		*leader = fdActorLookup(fdPartyGetLeaderActor());

	if(fdMapGridIsCloakMetatile(
		   leader->x.fixed, leader->z.fixed, leader->layer) == 0)
		event->psynergy = ABILITY_CLOAK | ACT_PACKED;
}

/* Show the Cloaked caster again: stop checking the ground under it, then
   flicker it back into view over twenty ticks and clear the cloak
   (fdPsynergyRunExtraHandler). */
void fdPsynergyEndCloak(void)
{
	ACTOR *caster =
		((FIELD_EFFECT_STATE *)RamHeapSlots->fieldEffectState)->caster;
	SPR			  *sprite = caster->sprite;
	SPR_ANIMATION *entry = sprite->entries[0];
	Uint32		   i;

	sySoundPlayStub(SE_CAST);
	syCloseEvent(fdEffectCheckCloakTile);
	fdActorSelectAnimationStub(caster, ANIM_STILL);
	caster->update = 0;

	/* Blink between the flicker palette half transparent and solid... */
	for(i = 0; i < 5; ++i) {
		entry->colorMode = SPRITE_COLOR_WHITE;
		sprite->flags = SPRITE_FLAG_OUTLINED;
		sprite->dirty = 1;
		sySynch(2);
		sprite->dirty = 1;
		sprite->flags = 0;
		sySynch(2);
	}

	/* ...then between the flicker palette and the caster's own. */
	for(i = 0; i < 5; ++i) {
		entry->colorMode = SPRITE_COLOR_WHITE;
		sprite->flags = 0;
		sprite->dirty = 1;
		sySynch(2);
		entry->colorMode = SPRITE_COLOR_NORMAL;
		sprite->dirty = 1;
		sySynch(2);
	}

	sprite->flags = SPRITE_FLAG_SHADOW;
	MainRoutineStateWork.cloaked = 0;
}

/* Starts the Cloak metatile check if the leader is hidden. fdModeRun calls it
   as a scene loads, since MainRoutineStateWork.cloaked outlives the scene. */
void fdPsynergyResumeCloakCheck(void)
{
	if(MainRoutineStateWork.cloaked != 0)
		sySetEvent(fdEffectCheckCloakTile, EVENT_PASS_TICK);
}
