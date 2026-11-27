/* @file */
/* The field Psynergy from Mind Read to Cloak, as The Broken Seal's */
/* event_commands.c has them in its parts mind_read.c to cloak.c: Mind Read's */
/* wave, Move, Reveal, Growth, Lift, Force, Frost, Ply and the start of Cloak.
 */
/* Part of event_commands.c. */

/* A line's offset in Mind Read's wave at a phase: a sine 4 pixels either way,
 */
/* a turn every 160 phase steps. */
#define MIND_READ_WAVE_OFFSET(phase) (sySin(((phase) << 16) / 160) >> 14)
/* The wave's cycle runs to this count and starts again, 61 frames; at counts */
/* 0, 8 and 16 a particle flies from the target to the caster. */
#define MIND_READ_CYCLE_LAST   60
#define MIND_READ_PARTICLE_GAP 8

/* Mind Read's wave each frame: once its delay has run out, writes the next */
/* page of line offsets, steps the colour fade and the hue every other frame, */
/* keeps the caster's colours lit, and three times a cycle sends a particle */
/* (SPRITE_CRESCENT) from the target to the caster, with SE_PSYNERGY_START. */
void fdPsynergyUpdateMindReadWave(void) {
	MIND_READ_STATE *state = Ram_HeapSlots->mindRead;
	VECTOR3 from;
	VECTOR3 to;
	ACTOR_PLACE *descriptor;
	s32 spriteId;
	ANIMATION_METADATA *metadata;
	PLACED_SPR *sprite;
	ACTOR *source;
	ACTOR *target;
	ACTOR *actor;
	u16 *line;
	s32 i;

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
		fdPaletteFadeSetTarget(RGB(state->red, state->green, state->blue) |
								   PALETTE_TINT_CUSTOM,
							   1);
		fdPaletteFadeStartInterpolation(1);
	}
	fdActorSetLayersColMode(fdActorGetById(state->srcId), LAYERS_COLOR_CYCLING);

	if(state->cnt == 0 || state->cnt == MIND_READ_PARTICLE_GAP ||
	   state->cnt == 2 * MIND_READ_PARTICLE_GAP) {
		source = fdActorGetById(state->srcId);
		target = fdActorGetById(state->tgtId);
		if(source != NULL && target != NULL) {
			descriptor = (ACTOR_PLACE *)scFindActorPlacement(state->srcId);
			spriteId = descriptor->spr;
			from.x = source->x.fixed;
			metadata = syResourceGetMetadataRecordFarStub(spriteId);
			from.y =
				source->y.fixed + INTtoFIXED((s8)metadata->sprTop) - toFIXED(2);
			from.z = source->z.fixed;
			sprite = target->spr;
			descriptor = (ACTOR_PLACE *)scFindActorPlacement(state->tgtId);
			spriteId = descriptor->spr;
			to.x = target->x.fixed;
			metadata = syResourceGetMetadataRecordFarStub(spriteId);
			{
				s32 box = (s8)metadata->sprTop;
				FIXED y = target->y.fixed;

				y += INTtoFIXED(box);
				to.y = y - toFIXED(2);
			}
			to.z = target->z.fixed;
			if(sprite->rotation == DEGtoANG(180)) {
				to.y -= toFIXED(24);
			}
			else if(sprite->rotation == DEGtoANG(90)) {
				to.x += toFIXED(6);
				to.y -= toFIXED(12);
			}
			else if(sprite->rotation == TRIG_TURN * 3 / 4) {
				to.x -= toFIXED(6);
				to.y -= toFIXED(12);
			}
			actor = fdActorCreateStub(SPRITE_CRESCENT, to.x, to.y, to.z);
			if(actor != NULL) {
				sprite = actor->spr;
				actor->motionFlg = 0;
				actor->speedLimit = toFIXED(16) / 25; /* 0.64 pixels a frame */
				actor->acceleration = toFIXED(16) / 25;
				actor->facing = syAtan(from.z - to.z, from.x - to.x);
				actor->update = (OBJECT_UPDATE)fdMindReadUpdateCrescent;
				sprite->flg = 0;
				sprite->pri = 1;
				fdActorSetMoveTargetStub(actor, from.x, from.y, from.z);
			}
		}
	}

	if(state->cnt != MIND_READ_QUIET) {
		if(state->cnt == 0)
			sySndPlayStub(SE_PSYNERGY_START);
		if(++state->cnt > MIND_READ_CYCLE_LAST)
			state->cnt = 0;
	}
}

/* Sets Mind Read's ripple going at each VBlank: HBlank DMA 0 gives BG0 the */
/* page of line offsets last written, a line at a time. */
void fdMindReadStartRipple(void) {
	MIND_READ_STATE *state = Ram_HeapSlots->mindRead;
	if(!state->delay) {
		u16 *line = state->lines[state->page];
		volatile u16 *dma = REG_DMA0;
		dma[DMA0_CONTROL] &= ~DMA_START_TIMING_DRQ_REPEAT;
		dma[DMA0_CONTROL] &= ~DMA_ENABLE;
		(void)dma[DMA0_CONTROL];
		syDMAXCopy(line, REG_BG0HOFS, DMA_HBLANK_HALFWORD_PER_LINE,
				   (volatile u32 *)dma);
	}
}

/* One colour channel's level (0 to 31) at a hue in degrees: rising from 0 */
/* to 60, full from 60 to 180, falling from 180 to 240, else 0. */
s32 fdEffectHueChannelRamp(s16 hue) {
	s32 level = 0;
	s16 angle = hue;

	angle = (angle + HUE_DEGREES) % HUE_DEGREES;
	if(angle < HUE_DEGREES / 6) {
		level = angle * RGB_LEVEL_MAX / (HUE_DEGREES / 6);
	}
	else if(angle < HUE_DEGREES / 2) {
		level = RGB_LEVEL_MAX;
	}
	else if(angle < HUE_DEGREES * 2 / 3) {
		level =
			(HUE_DEGREES * 2 / 3 - angle) * RGB_LEVEL_MAX / (HUE_DEGREES / 6);
	}
	return level;
}

/* Turns Mind Read's tint four degrees round the colour wheel, and takes the */
/* tint's colour, each channel dimmed to 4 to 11, from the hue before. */
void fdMindReadAdvanceTint(void) {
	MIND_READ_STATE *state = Ram_HeapSlots->mindRead;
	s16 r;
	s16 g;
	s16 b;
	u32 limit;

	r = fdEffectHueChannelRamp(state->hue);
	g = fdEffectHueChannelRamp(state->hue + HUE_DEGREES / 3);
	b = fdEffectHueChannelRamp(state->hue + HUE_DEGREES * 2 / 3);

	state->red = (r / 4 + 4) & RGB_LEVEL_MAX;
	state->green = (g / 4 + 4) & RGB_LEVEL_MAX;
	state->blue = (b / 4 + 4) & RGB_LEVEL_MAX;
	limit = toFIXED(HUE_DEGREES - 1);
	if((u32)INTtoFIXED(state->hue += 4) > limit)
		state->hue = 0;
}

/* A Mind Read crescent's update: once it has reached the caster, it goes. */
void fdMindReadUpdateCrescent(ACTOR *target) {
	s32 tx;
	s32 ty;

	tx = target->tgtX;
	if(tx == ACTOR_NO_TARGET) {
		ty = target->tgtY;
		if((ty == tx) && (target->tgtZ == ty)) {
			fdActorStartScriptStub((ACTOR *)target,
								   (u32)MindRead_CrescentEndScript);
		}
	}
}

/* Gives an actor's sprite colour mode value - 1 (SPR_COL_MODE), or for
 */
/* LAYERS_COLOR_CYCLING the next of MindRead_CasterColorCycle's swapped */
/* colours, a step every 2 frames, as Mind Read's wave does to its caster. */
void fdActorSetLayersColMode(void *object, s32 value) {
	s32 child = value - 1;

	if(value == LAYERS_COLOR_CYCLING)
		child = MindRead_CasterColorCycle[(FrameCount >> 1) & 7];
	fdActorSetSprColModeStub(object, child);
}

/* The way the +Control Pad points, in four ways: FACING_EAST to FACING_NORTH,
 */
/* or DPAD_ANGLE_NONE when none or a diagonal is held. */
s32 fdInputGetDpadAng(u32 unused) {
	/* Indexed by the right, left, up and down bits. */
	return Input_DpadAngles[DPAD_INDEX(InputWork.held)];
}

/* The way the +Control Pad points, in eight ways, or DPAD_ANGLE_NONE. */
u16 fdInputGetDpadAngEightWay(void) {
	INPUT_STATE *input = &InputWork;
	const u16 *words = Input_DpadAnglesEightWay;

	return *(u16 *)((u8 *)words + (DPAD_INDEX(input->held) << 1));
}

/* Per-frame update of Move's hand: turns it toward the actor it watches, at */
/* most 1/16 turn a frame, and sheds a particle (SPRITE_SPARKLE) round it. */
void fdEffectUpdateMoveHand(ACTOR *source) {
	ACTOR *target;
	VECTOR3 position;
	ACTOR *fragment;
	s32 turn;
	s32 radius;

	target = (ACTOR *)source->watchedActor;
	if(target != 0) {
		FIXED dx = target->x.fixed - source->x.fixed;
		FIXED dz = target->z.fixed - source->z.fixed;

		if(dx != 0 || dz != 0) {
			turn = (s16)(syAtan(dz, dx) - source->facing);
			if(turn > FACING_STEP)
				turn = FACING_STEP;
			if(turn < -FACING_STEP)
				turn = -FACING_STEP;
			source->facing += turn;
		}
		/* ACTOR_FACES_MOTION off: its motion must not undo the turn. */
		source->steeringFlg = 0;
	}

	/* The particle starts 8 to 24 pixels lower and up to 48 pixels away in a */
	/* random direction. */
	position.x = source->x.fixed;
	position.y = source->y.fixed - (syRandom() << 4) - toFIXED(8);
	position.z = source->z.fixed;
	radius = syRandom() * 3;
	radius <<= 4;
	syVectorAddPolarOffset(radius, syRandom(), &position);

	fragment = fdActorSpawn(SPRITE_SPARKLE, position.x, position.y, position.z);
	fdEffectInitFallingFragment(fragment);
}

struct PENDING_EVENT;

/* When Move is cast on a target, the pushing hand appears beside it and pushes
 */
/* it 16 pixels the way the d-pad points, for as long as the player pushes; A,
 */
/* B, L or R end it. */
void fdPsynergySteerPushedTarget(s32 unused) {
	VECTOR3 position;
	VECTOR3 beyond;
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	ACTOR *user = scene->caster;
	ACTOR *target = scene->tgt;
	ACTOR *pusher;
	ACTOR *hit;
	FIXED x;
	FIXED z;
	s32 angle = (u16)scene->ang + DEGtoANG(180);
	s32 moved = 0;
	s32 direction;
	s32 cellX;
	s32 cellZ;
	s32 (*callback)(void);
	u8 savedLayer = target->layer;
	PENDING_EVENT *event;
	s32 pushed = 0;
	const u8 *modes;

	if(target == NULL)
		return;
	fdPsynergyFadeToCastCol();
	user->watchedActor = (union FIELD_OBJECT *)target;
	fdActorStartScriptStub(user, FieldEffect_SourceHoldScript);
	pusher = fdEffectSpawnMoveHand(user);
	if(pusher == NULL) {
		fdPsynergyFadeFromCastCol();
		return;
	}

	event = scFindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
								  scene->fieldPsynergy,
								  scSelectPsynergyTarget(scene->fieldPsynergy));
	pusher->watchedActor = (union FIELD_OBJECT *)target;
	SET_POSITION(&position, target->x.fixed, target->y.fixed + toFIXED(16),
				 target->z.fixed);
	syVectorAddPolarOffset(toFIXED(16), angle, &position);
	fdActorSetMoveTargetStub(pusher, position.x, position.y, position.z);
	fdEffectGrowHandAndWait(pusher);
	pusher->speedLimit = toFIXED(4);
	pusher->acceleration = FIXED_ONE / 2;
	pusher->motionFlg = ACTOR_FLOATS;
	target->update = (OBJECT_UPDATE)fdActorCycleSprCols;
	target->speedLimit = toFIXED(2) / 5;
	target->acceleration = toFIXED(1) / 5;
	target->steeringFlg = moved;
	target->layer = 2;
	while(1) {
		sySynch(1);
		if((s8)scene->mode == 0 &&
		   (InputWork.pressed & (KEY_A | KEY_B | KEYS_SHOULDERS)))
			break;

		direction = (u16)fdInputGetDpadAng(InputWork.held);
		/* A scene's hook gives the way to push in place of the d-pad, or */
		/* DPAD_ANGLE_NONE to stop. */
		callback = (s32 (*)(void))scene->finishFunc;
		if(callback != NULL) {
			direction = callback();
			if(direction == DPAD_ANGLE_NONE)
				break;
		}
		else if(direction == DPAD_ANGLE_NONE) {
			SET_POSITION(&position, target->x.fixed,
						 target->y.fixed + toFIXED(16), target->z.fixed);
			syVectorAddPolarOffset(toFIXED(16), angle, &position);
			fdActorSetMoveTargetStub(pusher, position.x, position.y,
									 position.z);
			fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_OPEN);
			pusher->velX = 0;
			pusher->velY = 0;
			pusher->velZ = 0;
			continue;
		}

		SET_POSITION(&position, target->x.fixed, target->y.fixed + toFIXED(16),
					 target->z.fixed);
		syVectorAddPolarOffset(toFIXED(16), angle, &position);
		syVectorAddPolarOffset(toFIXED(2), direction, &position);
		fdActorSetMoveTargetStub(pusher, position.x, position.y, position.z);
		fdActorWaitForMoveStub(pusher);
		SET_POSITION(&position, target->x.fixed, target->y.fixed,
					 target->z.fixed);
		syVectorAddPolarOffset(toFIXED(16), direction, &position);
		SET_POSITION(&beyond, target->x.fixed, target->y.fixed,
					 target->z.fixed);
		syVectorAddPolarOffset(toFIXED(32), direction, &beyond);
		if(fdMapGridCheckCollisionStub(target, &position) > 0) {
			fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
			if((FrameCount & 15) == 0)
				sySndPlayStub(SE_MENU_ERROR);
			continue;
		}
		hit = fdActorScriptFindOverlappingActorStub(target, &position);
		if(hit != NULL) {
			if(hit != user) {
				fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
				if((FrameCount & 15) == 0)
					sySndPlayStub(SE_MENU_ERROR);
				continue;
			}
			cellX = user->x.fixed & FIELD_CELL_MASK;
			cellZ = user->z.fixed & FIELD_CELL_MASK;
			if(cellX == (position.x & FIELD_CELL_MASK) &&
			   cellZ == (position.z & FIELD_CELL_MASK)) {
				fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
				if((FrameCount & 15) == 0)
					sySndPlayStub(SE_MENU_ERROR);
				continue;
			}
			if(cellX == (beyond.x & FIELD_CELL_MASK) &&
			   cellZ == (beyond.z & FIELD_CELL_MASK)) {
				if(fdMapGridGetTerrainKindStub(user->layer, beyond.x,
											   beyond.z) != 0) {
					fdActorSelectAnimationStub(pusher,
											   MOVE_HAND_ANIMATION_FLEX);
					if((FrameCount & 15) == 0)
						sySndPlayStub(SE_MENU_ERROR);
					continue;
				}
				moved = 1;
			}
		}

		sySndPlayStub(SE_NOTE_HIGH);
		pushed = 1;
		x = position.x;
		z = position.z;
		modes = MoveHand_PushPoses;
		/* The hand's pose for the quarter turn between its side and the push.
		 */
		fdActorSelectAnimationStub(
			pusher, modes[(u16)(angle - direction) / (TRIG_TURN / 4)]);
		sySynch(15);
		target->paused = 0;
		target->speedLimit = toFIXED(1) / 5;
		target->acceleration = toFIXED(1) / 5;
		fdActorSetMoveTargetStub(target, position.x, position.y, position.z);
		pusher->motionFlg = 0;
		pusher->speedLimit = toFIXED(1) / 5;
		pusher->acceleration = toFIXED(1) / 5;
		syVectorAddPolarOffset(toFIXED(16), direction, &position);
		fdActorSetMoveTargetStub(pusher, position.x, position.y + toFIXED(16),
								 position.z);
		if(moved == 1) {
			fdActorGetById(scene->casterId)->steeringFlg &= ~ACTOR_FACES_MOTION;
			user->speedLimit = toFIXED(1) / 5;
			user->acceleration = toFIXED(1) / 5;
			fdActorSetMoveTargetStub(user, beyond.x, beyond.y, beyond.z);
		}
		fdActorWaitForMoveStub(target);
		if(event == NULL) {
			target->x.fixed = x;
			target->z.fixed = z;
			target->velX = 0;
			target->velZ = 0;
		}
		break;
	}

	target->layer = savedLayer;
	fdActorSetSprColModeStub(target, scene->savedPal);
	fdActorStartScriptStub(target, scene->savedScript);
	target->update = (OBJECT_UPDATE)scene->savedFunc;
	if(pushed) {
		pusher->update = NULL;
		fdActorSelectAnimationStub(pusher, MOVE_HAND_ANIMATION_FLEX);
		fdActorSetPosAndResetMotionStub(pusher, pusher->x.fixed,
										pusher->y.fixed, pusher->z.fixed);
		fdRunPsynergyEvent(event, scene->casterId, scene->tgtId);
	}
	fdBattleEffectStopMainActor();
	if(moved == 1) {
		fdActorGetById(scene->casterId)->steeringFlg |= ACTOR_FACES_MOTION;
	}
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
		syVectorAddPolarOffset(toFIXED(30), (u16)angle, &position);
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
/* frames and then turns into particles. */
void fdPsynergyMove(void) {
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	s32 state[3];
	FIELD_EFFECT_STATE *scene;
	void *object;

	scene = Ram_HeapSlots->fieldEffState;
	object = scene->caster;

	fdPsynergyFadeToCastCol();
	object = fdEffectSpawnMoveHand(object);
	fdEffectGrowHandAndWait(object);
	if(object != 0) {
		fdActorSelectAnimationStub(object, MOVE_HAND_ANIMATION_FLEX);
		sySynch(30);
	}
	fdPsynergyFadeFromCastCol();
	fdEffectBurstHand(object);
}

/* Move's hand appears at a quarter size 16 pixels above the caster, with a */
/* crack and eight sparkles that scatter from it, and sets out 16 pixels the */
/* way the caster faces, to the nearest quarter turn. */
void *fdEffectSpawnMoveHand(void *object) {
	ACTOR *source = object;
	ACTOR *parent;
	ACTOR *child;
	s32 angle;
	FIXED radius;
	FIXED speed;
	s32 fragmentIndex;
	u32 spread;
	u32 jitter;

	angle = (source->facing + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
	parent = fdActorSpawn(SPRITE_MOVE_HAND, source->x.fixed,
						  source->y.fixed + toFIXED(16), source->z.fixed);
	if(parent == 0)
		return 0;
	parent->scaleY = FIXED_ONE / 4;
	parent->scaleX = FIXED_ONE / 4;
	parent->update = fdEffectUpdateMoveHand;
	parent->speedLimit = toFIXED(2);
	parent->acceleration = toFIXED(2);
	parent->motionFlg = 0;
	fdActorSelectAnimationStub(parent, MOVE_HAND_ANIMATION_GRIP);
	fdActorSetTargetPosFromMagnitudeAng(parent, toFIXED(16), angle);

	for(fragmentIndex = 0; fragmentIndex < 8; fragmentIndex++) {
		child = fdActorSpawn(SPRITE_SPARKLE, source->x.fixed,
							 source->y.fixed + toFIXED(16), source->z.fixed);
		if(child != 0) {
			fdActorStartScriptStub(child, FieldEffect_FragmentScript);
			speed = syRandom() + FIXED_ONE;
			child->acceleration = toFIXED(1);
			child->speedLimit = speed;
			child->motionFlg = ACTOR_FALLS;
			child->gravity = toFIXED(1) / 50;
			spread = syRandom();
			child->velY = spread - syRandom();
			radius = syRandom() * 24 + toFIXED(8);
			jitter = syRandom();
			fdActorSetTargetPosFromMagnitudeAng(
				child, radius, ((jitter - syRandom()) >> 3) + source->facing);
		}
	}
	sySndPlayStub(SE_ITEM_BREAK);
	return parent;
}

/* Grows a hand to full size at once and waits for it to reach where it is */
/* going. */
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

#define BURST_RISE_FRAMES 31
#define BURST_SPARKLES	  8

/* The hand spins as it rises for BURST_RISE_FRAMES, shrinks, and bursts into */
/* BURST_SPARKLES sparkles (SPRITE_SPARKLE). */
void fdEffectBurstHand(void *object) {
	ACTOR *src = object;
	ACTOR *child;
	s32 i;
	s32 dist;
	u32 spread;

	sySndPlayStub(SE_CAST);
	for(i = 0; i < BURST_RISE_FRAMES; i++) {
		src->y.fixed += toFIXED(1);
		src->facing += DEGtoANG(45);
		src->scaleX += -FIXED_ONE / 32;
		src->scaleY += -FIXED_ONE / 32;
		sySynch(1);
	}
	for(i = BURST_SPARKLES - 1; i >= 0; i--) {
		child = fdActorSpawn(SPRITE_SPARKLE, src->x.fixed, src->y.fixed,
							 src->z.fixed);
		if(child != 0) {
			s32 speed;
			s32 unit = toFIXED(1);

			fdActorStartScriptStub(child, FieldEffect_FragmentScript);
			speed = syRandom();
			child->acceleration = unit;
			child->speedLimit = speed + unit;
			child->motionFlg = ACTOR_FALLS;
			child->gravity = toFIXED(1) / 25;
			spread = syRandom();
			child->velY = spread - syRandom();
			dist = syRandom() * 24;
			dist += toFIXED(8);
			fdActorSetTargetPosFromMagnitudeAng(child, dist, syRandom());
		}
	}
	sySndPlayStub(SE_PSYNERGY_SPARKLE);
	fdActorReleaseStub(src);
}

/* Reveal's colour for the rocks it shows (sprite 200), given at its start */
/* and taken back at its end. Only the loop over the actors is left; value */
/* goes unused. */
void fdActorSetRevealSprCol(s32 value) {
	ACTOR *actors = Ram_HeapSlots->actors;
	s32 i;

	/* DEAD: The Broken Seal's loop over the actors with what it did to a rock's
	 */
	/* sprite gone, so its tests do nothing; the ROM keeps its count and its */
	/* branch. */
	for(i = 0; i < ACTOR_COUNT; i++) {
		ACTOR *actor = &actors[i];

		if(actor->script != NULL) {
			u8 kind = actor->sprStorageKind;

			if(kind == ACTOR_SPRITE_ONE) {
			}
		}
	}
}

/* How far the leader may walk from where Reveal was cast, in screen pixels up
 */
/* and down; sideways x counts at about 0.82 of itself, so the edge is an */
/* ellipse about 73 pixels to either side. */
#define REVEAL_RADIUS 60

/* As The Broken Seal's, with this game's offsets: counts the timer down and, */
/* once the leader is REVEAL_RADIUS from the centre or the timer has run out, */
/* requests the effect's end. */
void fdPsynergyWatchLeaderDistance(void) {
	FIELD_STATE *state;
	ACTOR *actor;
	FIXED distanceX;
	s32 distanceZ;

	state = FieldStateWork;
	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	if(state->effTmrRunning != 0) {
		if(state->revealFrm != 0)
			state->revealFrm--;
	}

	/* In screen pixels: y is z less the height. */
	distanceX = syMulFX(state->revealCenterX - actor->x.fixed / FIXED_ONE,
						toFIXED(0.81649));
	distanceZ =
		state->revealCenterY - (actor->z.fixed - actor->y.fixed) / FIXED_ONE;
	if(distanceX * distanceX + distanceZ * distanceZ >=
		   REVEAL_RADIUS * REVEAL_RADIUS ||
	   state->revealFrm == 0) {
		/* Raise Reveal as a Psynergy request that pays no PP (ACT_PACKED */
		/* | ABILITY_REVEAL), which runs its ending events. */
		u16 *slot = (u16 *)&state->psynergy;
		u32 request = ABILITY_REVEAL | ACT_PACKED;

		*slot = request;
	}
}

#define REVEAL_FRAMES 600 /* ten seconds of Reveal */

/* Reveal: open a circle of the transition on the caster, wash the screen */
/* plain, run the Reveal event that matches, then show the markers nearby */
/* and watch how far the leader walks. */
void fdPsynergyReveal(void) {
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	FIELD_STATE *event;
	SCR_TRANSITION_STATE *state;
	ACTOR *caster;
	void *pending;
	s32 frame;

	fdActorSetRevealSprCol(6);
	event = Ram_HeapSlots->fieldState;
	fdScrTransitionStartWindowHeld(FIELD_PSYNERGY_REVEAL);
	state = Ram_HeapSlots->scrTransition;
	caster = scene->caster;
	state->centerX = caster->x.fixed;
	state->centerY = caster->z.fixed - caster->y.fixed;
	fdPaletteFadeSetSource(FIXED_ONE, 0);
	fdPaletteFadeSetTarget(PALETTE_TINT_GRAYSCALE, 1);
	fdPaletteFadeStartInterpolation(1);
	event->revealActive = 0;
	sySynch(1);

	pending = scFindPsynergyTrigger(
		PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS), FIELD_PSYNERGY_REVEAL,
		scSelectPsynergyTarget(FIELD_PSYNERGY_REVEAL));
	if(pending != 0)
		fdRunPsynergyEvent(pending, scene->casterId, scene->tgtId);
	sySndPlayStub(SE_PSYNERGY_SPARKLE);
	syIoWrite8(1, &event->revealActive);
	event->revealCenterX = state->centerX / FIXED_ONE;
	event->revealCenterY = state->centerY / FIXED_ONE;
	event->revealFrm = REVEAL_FRAMES;
	event->effTmrRunning = 1;
	fdEffectSpawnRevealMarkers();
	for(frame = 0; frame <= 18; frame++) {
		sySynch(1);
		state->value = frame;
	}
	if((s8)scene->mode == 0)
		sySetEvent(fdPsynergyWatchLeaderDistance, TASK_PASS_FRAME);
}

/* Ends Reveal if it is running: stops its watch, brings the colours back from
 */
/* black over 30 frames and runs the scene's PSYNERGY_PHASE_FADES event. */
void fdPsynergyEndReveal(void) {
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	SCR_TRANSITION_STATE *state = Ram_HeapSlots->scrTransition;
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	void *pending;

	if(event->revealActive != 0) {
		sySndPlayStub(SE_MAP_TRANSITION);
		syCloseEvent(fdPsynergyWatchLeaderDistance);
		event->revealActive = 0;
		event->revealFrm = 0;
		fdActorSetRevealSprCol(0);
		fdPaletteFadeSetTarget(FIXED_ONE, 1);
		fdPaletteFadeStartInterpolation(1);
		fdPaletteFadeSetSource(0, 0);
		fdPaletteFadeSetTarget(FIXED_ONE, 0);
		fdPaletteFadeStartInterpolation(30);
		sySynch(1);
		pending = scFindPsynergyTrigger(
			PSYNERGY_TRIGGER(PSYNERGY_PHASE_FADES), FIELD_PSYNERGY_REVEAL,
			scSelectPsynergyTarget(FIELD_PSYNERGY_REVEAL));
		if(pending != NULL)
			fdRunPsynergyEvent(pending, scene->casterId, scene->tgtId);
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
	GROWTH_ON_ARROW_PLANT		/* the scene's event makes it point */
} GROWTH_TARGET_KIND;

/* Growth cast on a target: the burst of particles, then the target takes its */
/* grown animation and the underfoot priority, with SE_RECOVERY. Sprouting */
/* leaves (SPRITE_SPROUTING_LEAVES) instead drop five vines round the */
/* Psynergy's position and hang a vine (fdHangingVineHang). On an arrow plant */
/* Growth's own effect only plays SE_RECOVERY; the scene's event turns it into
 */
/* an arrow (scGaiaRockInsidePlantPointsTheWay). */
void fdPsynergyGrowthOnTarget(s32 unused) {
	FIELD_EFFECT_STATE *scene = Ram_HeapSlots->fieldEffState;
	ACTOR *target = scene->tgt;
	s32 kind = GROWTH_ON_PLANT;

	if(target == 0)
		return;

	fdEffectSpawnBurstParticleField();
	if(fdActorGetSprIdStub(target) == SPRITE_SPROUTING_LEAVES)
		kind = GROWTH_ON_SPROUTING_LEAVES;
	if(fdActorGetSprIdStub(target) == SPRITE_ARROW_PLANT)
		kind = GROWTH_ON_ARROW_PLANT;

	if(kind == GROWTH_ON_PLANT) {
		fdActorSelectAnimationStub(target, 2);
		fdActorSetSprFlagsStub(target, 0);
		target->collisionFlg = 0;
		target->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		sySynch(10);
		sySndPlayStub(SE_RECOVERY);
		sySynch(40);
	}
	else if(kind == GROWTH_ON_SPROUTING_LEAVES) {
		BURST_PARTICLE_VECTOR position;
		s32 n;

		for(n = 0; n < 5; n++) {
			ACTOR *piece;
			FIXED distance;

			position.values[0] = scene->x;
			position.values[2] = scene->z;
			distance = syRandom() * 6 + toFIXED(4);
			syVectorAddPolarOffset(distance, syRandom(), position.values);
			position.values[1] = scene->y;
			piece = fdActorSpawn(SPRITE_VINE, position.values[0],
								 position.values[1], position.values[2]);
			if(piece != 0) {
				fdActorStartScriptStub(piece, Growth_LeafScript);
				piece->motionFlg = ACTOR_FALLS;
			}
			sySynch((syRandomBelow(2)) + 2);
		}
		fdHangingVineHang(scene->tgtId, 1);
	}
	else {
		sySynch(10);
		sySndPlayStub(SE_RECOVERY);
	}
	fdPsynergyFadeFromCastCol();
}

/* When Growth is cast with no target, particles fly out from in front of the */
/* caster, and then five vine leaves drop 4 to 10 pixels round the Psynergy's */
/* position. */
void fdPsynergyGrowth(void) {
	FIELD_EFFECT_STATE *state = Ram_HeapSlots->fieldEffState;
	BURST_PARTICLE_VECTOR position;
	s32 entryCount;

	fdEffectSpawnBurstParticleField();
	sySndPlayStub(SE_HEAVY_IMPACT);
	for(entryCount = 0; entryCount < 5; entryCount++) {
		void *object;
		s32 randomValue;

		position.values[0] = state->x;
		position.values[2] = state->z;
		randomValue = (syRandom() * 6) + toFIXED(4);
		syVectorAddPolarOffset(randomValue, syRandom(), position.values);
		position.values[1] = state->y;
		object = fdActorSpawn(SPRITE_VINE, position.values[0],
							  position.values[1], position.values[2]);
		if(object != 0) {
			fdActorStartScriptStub(object, Growth_LeafScript);
			((ACTOR *)object)->motionFlg = ACTOR_FALLS;
		}
		sySynch((syRandomBelow(2)) + 2);
	}
	sySynch(30);
	fdPsynergyFadeFromCastCol();
}

#define BURST_FIELD_PARTICLES 24

/* Sends BURST_FIELD_PARTICLES particles, one every 2 frames, from in front of
 */
/* the caster to points round the Psynergy's position. */
void fdEffectSpawnBurstParticleField(void) {
	FIELD_EFFECT_STATE *state;
	ACTOR *user;
	FIXED pos[3];
	s32 n;
	s32 *p;

	state = Ram_HeapSlots->fieldEffState;
	user = state->caster;
	fdPsynergyFadeToCastCol();

	p = pos;
	for(n = 0; n < BURST_FIELD_PARTICLES; n++) {
		ACTOR *obj;
		BURST_PARTICLE_VISUAL *visual;
		BURST_PARTICLE_VISUAL *child;
		FIXED dist;

		if(*(u16 *)&state->ang == DEGtoANG(90)) {
			pos[0] = user->x.fixed;
			pos[1] = user->y.fixed + toFIXED(10);
			pos[2] = user->z.fixed;
		}
		else if(*(u16 *)&state->ang == TRIG_TURN * 3 / 4) {
			pos[0] = user->x.fixed;
			pos[1] = user->y.fixed + toFIXED(24);
			pos[2] = user->z.fixed;
		}
		else {
			pos[0] = user->x.fixed;
			pos[1] = user->y.fixed + toFIXED(10);
			pos[2] = user->z.fixed;
			syVectorAddPolarOffset(toFIXED(10), *(u16 *)&state->ang, pos);
		}

		obj = fdActorSpawn(SPRITE_PARTICLE, pos[0], pos[1], pos[2]);
		visual = &((BURST_PARTICLE_VISUAL_GROUP *)obj->spr)->primary;
		child = &((BURST_PARTICLE_VISUAL_GROUP *)obj->spr)->child;
		child->fullCol = visual->fullCol;
		child->size = visual->size;
		child->shape = visual->shape;
		child->attributes.bits.tile = visual->attributes.bits.tile;
		child->attributes.bytes.pal = visual->attributes.bytes.pal;

		if(obj != 0) {
			obj->scaleY = toFIXED(0.7);
			obj->scaleX = toFIXED(0.7);
			obj->acceleration = toFIXED(3) / 2;
			obj->speedLimit = toFIXED(3) / 2;
			obj->motionFlg = 0;
			fdActorSetSprColModeStub(obj, SPRITE_COLOR_GREEN_TINT);
			fdActorSelectAnimationStub(obj, ANIM_PARTICLE_TWINKLE);
			fdActorStartScriptStub(obj, FieldEffect_ReleaseScript + 1);

			fdActorSetSprFlagsStub(obj, SPRITE_FLAG_SHADOW);

			p[0] = state->x;
			p[1] = state->y;
			p[2] = state->z;
			if(*(u16 *)&state->ang == TRIG_TURN * 3 / 4)
				syVectorAddPolarOffset(toFIXED(14), *(u16 *)&state->ang, p);
			dist = syRandom() * 6 + toFIXED(4);
			syVectorAddPolarOffset(dist, syRandom(), p);
			fdActorSetMoveTargetStub(obj, p[0], p[1], p[2]);
		}
		sySndPlayStub(SE_PSYNERGY_SPARKLE);
		sySynch(2);
	}
	sySynch(8);
}

/* A hand appears 32 pixels to either side, each moves 14 pixels in, and they */
/* lift the target 32 pixels with them. */
void fdPsynergyLiftOnTarget(void) {
	FIELD_EFFECT_STATE *effectState;
	ACTOR *caster;
	void *child;
	void *anchors[2];
	FIXED position[3];
	s32 x;
	FIXED y;
	s32 z;
	s32 index;
	s32 startY;

	effectState = Ram_HeapSlots->fieldEffState;
	child = effectState->tgt;
	caster = effectState->caster;
	if(child == 0)
		return;

	fdPsynergyFadeToCastCol();
	caster->watchedActor = child;
	fdActorStartScriptStub(caster, FieldEffect_SourceHoldScript);

	x = effectState->x;
	position[0] = x;
	y = effectState->y + toFIXED(16);
	position[1] = y;
	z = effectState->z;
	position[2] = z;
	anchors[0] = fdEffectSpawnLiftHand(x + toFIXED(32), y, z, FACING_WEST);
	anchors[1] = fdEffectSpawnLiftHand(position[0] - toFIXED(32), position[1],
									   position[2], FACING_EAST);

	sySynch(15);
	for(index = 0; index < 2; index++) {
		void *anchor = anchors[index];
		if(anchor != 0)
			fdActorSetTargetPosFromMagnitudeAng(anchor, toFIXED(14),
												((ACTOR *)anchor)->facing);
	}

	fdActorWaitForMoveStub(anchors[0]);
	((EFFECT_CHILD *)child)->func = (void (*)(void))fdActorCycleSprCols;
	sySndPlayStub(SE_PSYNERGY_START);
	((EFFECT_CHILD *)child)->flg = 4;

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
	fdPsynergyFadeFromCastCol();
}

/* Lift cast with no target: two hands appear 32 pixels to either side of the */
/* spot, close 24 pixels in and meet with a crash that scatters */
/* ITEM_BREAK_SHARD_MAX sparkles, then vanish. */
void fdPsynergyLift(void) {
	FIELD_EFFECT_STATE *state;
	ACTOR *obj;
	ACTOR *pair[2];
	VECTOR3 pos;
	ACTOR **walk;
	s32 n;
	FIXED dist;

	state = Ram_HeapSlots->fieldEffState;
	fdPsynergyFadeToCastCol();
	SET_POSITION(&pos, state->x, state->y + toFIXED(16), state->z);
	pair[0] =
		fdEffectSpawnLiftHand(pos.x + toFIXED(32), pos.y, pos.z, DEGtoANG(180));
	pair[1] = fdEffectSpawnLiftHand(pos.x - toFIXED(32), pos.y, pos.z, 0);

	sySynch(15);
	walk = pair;
	for(n = 0; n < 2; n++) {
		obj = *walk++;
		if(obj != 0)
			fdActorSetTargetPosFromMagnitudeAng(obj, toFIXED(24), obj->facing);
	}

	fdActorWaitForMoveStub(pair[0]);
	sySndPlayStub(SE_HEAVY_IMPACT);
	for(n = 0; n < ITEM_BREAK_SHARD_MAX; n++) {
		SET_POSITION(&pos, state->x, state->y + toFIXED(16), state->z);
		obj = fdActorSpawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
		if(obj != 0) {
			fdActorStartScriptStub(obj, FieldEffect_FragmentScript);
			obj->speedLimit = syRandom() + toFIXED(2);
			obj->acceleration = toFIXED(2);
			obj->motionFlg = 0;
			dist = syRandom() * 24 + toFIXED(8);
			fdActorSetTargetPosFromMagnitudeAng(obj, dist, syRandom());
		}
	}

	fdActorReleaseStub(pair[0]);
	fdActorReleaseStub(pair[1]);
	fdPsynergyFadeFromCastCol();
}

/* Spawns one of Lift's hands (SPRITE_MOVE_HAND) and grows it from a quarter */
/* to full size, an eighth of a turn a frame, then faces it to angle. */
ACTOR *fdEffectSpawnLiftHand(s32 x, s32 y, s32 z, s32 angle) {
	ACTOR *actor;
	s32 scale;

	sySndPlayStub(SE_ITEM_BREAK);
	actor = fdActorSpawn(SPRITE_MOVE_HAND, x, y, z);
	if(actor != NULL) {
		actor->scaleX = actor->scaleY = FIXED_ONE / 4;
		actor->acceleration = actor->speedLimit = toFIXED(3);
		((SPR *)actor->spr)->image.pri = 0;
		fdActorSelectAnimationStub(actor, MOVE_HAND_ANIMATION_GRIP);
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

/* Field effect sprite callback of Force's particles (OUT_AND_BACK_STATE): */
/* each flies 25 pixels from where it appeared, then back to a point 8 pixels */
/* toward the caster's facing from 16 pixels above it, on screen, give or take
 */
/* 4 pixels, and is freed on arrival. */
void fdEffectUpdateRadialBurst(FIELD_PARTICLE *effect) {
	/* OUT_AND_BACK_FLYING_OUT must enter OUT_AND_BACK_AIM_BACK immediately. */
	FIELD_EFFECT_STATE *effectState = Ram_HeapSlots->fieldEffState;
	VECTOR3 pos;
	s32 state;

again:
	state = effect->state;
	if(state == OUT_AND_BACK_AIM_OUT) {
		/* 25 pixels from the origin, in a random direction. */
		pos.x = effect->originX;
		pos.z = effect->originZ;
		syVectorAddPolarOffset(toFIXED(25), (u16)syRandom(), &pos);
		effect->tgtX = pos.x;
		effect->tgtZ = pos.z;
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
		/* 8 pixels toward effectState->ang from 16 pixels above the main
		 * actor, */
		/* on screen, give or take 4 pixels. */
		ACTOR *caster = effectState->caster;

		pos.x = caster->x.fixed;
		pos.y = caster->y.fixed + toFIXED(16);
		pos.z = caster->z.fixed;
		syVectorAddPolarOffset(toFIXED(8), effectState->ang, &pos);
		fdCameraWorldToScr(&pos);
		syVectorAddPolarOffset(toFIXED(4), syRandom(), &pos);
		effect->tgtX = pos.x;
		effect->tgtZ = pos.z;
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

/* A heavy impact, as The Broken Seal's: an actor placed below the source */
/* for a moment, scattering IMPACT_SHARDS shards from it. */
s32 fdEffectSpawnHeavyImpact(ACTOR *source) {
	VECTOR3 pos;
	ACTOR *impact;
	ACTOR *fragment;
	s32 fragmentIndex;

	sySndPlayStub(SE_HEAVY_IMPACT);
	pos.x = source->x.fixed;
	pos.y = source->y.fixed;
	pos.z = source->z.fixed;
	impact = fdActorSpawn(SPRITE_STAR_BURST, pos.x, pos.y - toFIXED(32), pos.z);
	if(impact != NULL) {
		impact->motionFlg = 0;
		impact->wait = 20;
		fdActorStartScriptStub(impact, FieldEffect_ReleaseScript);
	}

	/* Shards: 1 to 2 pixels a frame, 8 to 32 pixels in a random direction. */
	for(fragmentIndex = 0; fragmentIndex < IMPACT_SHARDS; fragmentIndex++) {
		fragment = fdActorSpawn(SPRITE_SPARKLE, pos.x, pos.y, pos.z);
		if(fragment != NULL) {
			fdActorStartScriptStub(fragment, FieldEffect_FragmentScript);
			fragment->speedLimit = syRandom() + toFIXED(1);
			fragment->acceleration = toFIXED(1);
			fragment->motionFlg = 0;
			fdActorSetTargetPosFromMagnitudeAng(
				fragment, syRandom() * 24 + toFIXED(8), syRandom());
		}
	}
	return 0;
}

/* Runs Force cast on a target, through fdPsynergyForce. */
void fdPsynergyForceOnTarget(s32 unused) { fdPsynergyForce(); }

/* Force's ring of particles round the caster, one every 2 frames, and the */
/* frames it waits at most for the fist to strike. */
#define FORCE_RING_PARTICLES 12
#define FORCE_STRIKE_FRAMES	 60

/* Force: a ring of particles round the caster, then a fist (SPRITE_MOVE_HAND)
 */
/* grows in front of them and strikes 56 pixels ahead, or at the target, with */
/* three afterimages; the target is pushed 16 pixels the way the fist flew */
/* (and thrown up when its option asks), and the scene's Force event runs. */
void fdPsynergyForce(void) {
	ACTOR *copies[4];
	VECTOR3 pos;
	FIELD_EFFECT_STATE *scene;
	ACTOR *target;
	ACTOR *source;
	ACTOR *actor;
	ACTOR *copy;
	FIELD_PARTICLE *slot;
	SPR *resource;
	PENDING_EVENT *event;
	s32 scale;
	s32 i;
	u8 marker;

	scene = Ram_HeapSlots->fieldEffState;
	target = scene->tgt;
	fdPsynergyFadeToCastCol();
	sySndPlayStub(SE_PSYNERGY_START);
	slot = scene->particles;

	for(i = 0; i < FORCE_RING_PARTICLES; i++) {
		source = scene->caster;
		SET_POSITION(&pos, source->x.fixed, source->y.fixed + toFIXED(16),
					 source->z.fixed);
		fdCameraWorldToScr(&pos);
		fdParticleInit(slot, SPRITE_PARTICLE, pos.x, pos.z);
		fdParticleSetCallback(slot, fdEffectUpdateRadialBurst);
		fdParticleSetAnimation(slot, ANIM_PARTICLE_TWINKLE);
		fdSprSetAnimationColModesStub(slot->spr, SPRITE_COLOR_BLUE_TINT);
		slot->scaleY = FIXED_ONE * 7 / 10;
		slot->scaleX = FIXED_ONE * 7 / 10;
		sySynch(2);
		slot++;
	}

	source = scene->caster;
	SET_POSITION(&pos, source->x.fixed, source->y.fixed + toFIXED(16),
				 source->z.fixed);
	syVectorAddPolarOffset(toFIXED(8), scene->ang, &pos);
	actor = fdActorSpawn(SPRITE_MOVE_HAND, pos.x, pos.y, pos.z);
	if(actor == NULL) {
		fdPsynergyFadeFromCastCol();
		return;
	}
	actor->scaleY = FIXED_ONE / 4;
	actor->scaleX = FIXED_ONE / 4;
	actor->facing = scene->ang;
	actor->speedLimit = toFIXED(4);
	actor->acceleration = toFIXED(4);
	actor->motionFlg = 0;
	fdActorSelectAnimationStub(actor, MOVE_HAND_ANIMATION_FIST);
	fdActorSetSprColModeStub(actor, SPRITE_COLOR_BGR);
	scale = actor->scaleX;
	if(scale < FIXED_ONE) {
		do {
			scale += FIXED_ONE * 5 / 256;
			actor->scaleY = scale;
			actor->scaleX = scale;
			sySynch(1);
			scale = actor->scaleX;
		} while(scale < FIXED_ONE);
	}
	sySynch(3);

	resource = NULL;
	for(i = 2; i >= 0; i--) {
		copy = copies[i] = fdActorSpawn(SPRITE_MOVE_HAND, actor->x.fixed,
										actor->y.fixed, actor->z.fixed);
		if(copy != NULL) {
			copy->scaleY = AFTERIMAGE_SCALE;
			copy->scaleX = AFTERIMAGE_SCALE;
			copy->facing = scene->ang;
			copy->speedLimit = toFIXED(4);
			copy->acceleration = toFIXED(4);
			copy->motionFlg = 0;
			fdActorSelectAnimationStub(copy, MOVE_HAND_ANIMATION_FIST);
			fdActorSetSprColModeStub(copy, SPRITE_COLOR_RBG);
			resource = fdSprShareVramCacheEntry(copy->spr, resource);
		}
	}
	marker = resource->slot;

	if(scene->aimAhead != 0) {
		source = scene->caster;
		SET_POSITION(&pos, source->x.fixed, source->y.fixed + toFIXED(16),
					 source->z.fixed);
		syVectorAddPolarOffset(toFIXED(56), scene->ang, &pos);
	}
	else {
		SET_POSITION(&pos, scene->x, scene->y + toFIXED(16), scene->z);
	}
	fdActorSetMoveTargetStub(actor, pos.x, pos.y, pos.z);
	fdActorStartScriptStub(actor, Force_FistScript);
	for(i = 0; i < 3; i++) {
		copy = copies[i];
		if(copy != NULL) {
			sySynch(3);
			fdActorSetMoveTargetStub(copy, pos.x, pos.y, pos.z);
			fdActorStartScriptStub(copy, FieldEffect_ReleaseScript + 1);
		}
	}

	for(i = 0; i < FORCE_STRIKE_FRAMES && actor->script != NULL; i++)
		sySynch(1);

	if(target != NULL && scene->childMode == 0) {
		if(scene->sceneVariant != 0)
			target->velY = toFIXED(8);
		SET_POSITION(&pos, target->x.fixed, target->y.fixed, target->z.fixed);
		syVectorAddPolarOffset(toFIXED(16), scene->ang, &pos);
		if(fdMapGridCheckCollisionStub(target, &pos) == 0 &&
		   fdActorScriptCheckOverlapStub(target, &pos) == 0) {
			target->acceleration = toFIXED(1);
			target->speedLimit = toFIXED(1);
			fdActorSetMoveTargetStub(target, pos.x, pos.y, pos.z);
		}
	}

	event = scFindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS),
								  FIELD_PSYNERGY_FORCE,
								  scSelectPsynergyTarget(FIELD_PSYNERGY_FORCE));
	if(event != NULL)
		fdRunPsynergyEvent(event, scene->casterId, scene->tgtId);
	sySynch(10);
	fdPsynergyFadeFromCastCol();
	sySynch(20);
	/* VRAM_CACHE_ENTRY_COUNT: no afterimage took a cache entry. */
	if(marker != VRAM_CACHE_ENTRY_COUNT)
		syVramCacheFreeEntry(marker);
}

struct ORBITING_PARTICLE;

/* Per-frame update of a Frost glint: of every eight frames, two show the */
/* particle in swapped colours (SPRITE_COLOR_RBG). */
s32 fdEffectUpdateFrostGlint(struct ORBITING_PARTICLE *particle) {
	s32 phase = FrameCount & 7;

	if(phase == 0) {
		fdActorSetSprColModeStub((ACTOR *)particle, SPRITE_COLOR_RBG);
	}
	else if(phase == 2) {
		fdActorSetSprColModeStub((ACTOR *)particle, SPRITE_COLOR_NORMAL);
	}
	return 0;
}

/* Per-frame update: shrinks by 1/16 a frame and is released at 1/16. */
void fdEffectUpdateOrbitingParticleFade(ACTOR *particle) {
	FIXED scale;

	if(particle != NULL) {
		scale = particle->scaleX - FIXED_ONE / 16;
		particle->scaleY -= FIXED_ONE / 16;
		particle->scaleX = scale;
		if(scale <= FIXED_ONE / 16)
			fdActorStartScriptStub(particle, FieldEffect_ReleaseScript);
	}
}

/* Spirals anticlockwise at twice the frames left in pixels, then fades. */
void fdEffectUpdateOrbitingParticleLeft(ACTOR *particle) {
	VECTOR3 pos;
	s16 life;
	s32 count;

	if(particle != NULL) {
		count = (u16)particle->tmr - 1;
		particle->tmr = count;
		life = count;
		if(life != 0) {
			SET_POSITION(&pos, particle->tgtX, particle->tgtY, particle->tgtZ);
			syVectorAddPolarOffset(
				life * toFIXED(2),
				particle->userValue + life * (TRIG_TURN / 32), &pos);
			SET_ACTOR_POSITION(particle, pos.x, pos.y, pos.z);
		}
		else {
			particle->update =
				(OBJECT_UPDATE)fdEffectUpdateOrbitingParticleFade;
		}
	}
}

/* Per-frame update: as fdEffectUpdateOrbitingParticleLeft, clockwise. */
void fdEffectUpdateOrbitingParticleRight(ACTOR *particle) {
	VECTOR3 pos;
	s16 life;
	s32 count;

	if(particle != NULL) {
		count = (u16)particle->tmr - 1;
		particle->tmr = count;
		life = count;
		if(life != 0) {
			SET_POSITION(&pos, particle->tgtX, particle->tgtY, particle->tgtZ);
			syVectorAddPolarOffset(
				life * toFIXED(2),
				particle->userValue - life * (TRIG_TURN / 32), &pos);
			SET_ACTOR_POSITION(particle, pos.x, pos.y, pos.z);
		}
		else {
			particle->update =
				(OBJECT_UPDATE)fdEffectUpdateOrbitingParticleFade;
		}
	}
}

/* Frost cast on a target. A target with a mode set takes only the snowflakes,
 */
/* and its flash and freezing are left to the scene. */
void fdPsynergyFrostOnTarget(s32 unused) {
	FIELD_EFFECT_STATE *effectState = Ram_HeapSlots->fieldEffState;
	ACTOR *target = effectState->tgt;

	if(target != 0) {
		if(effectState->childMode != 0) {
			effectState->aimAhead = 1;
		}
		target->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdPsynergyFrost();
	}
}

#define FROST_SNOWFLAKES 16 /* circling the target */

/* FROST_SNOWFLAKES snowflakes, one every three frames, spiral in on points 2 */
/* to 6 pixels round the Psynergy's position; a target then flashes white eight
 */
/* times, freezes (its animation 2, unless its option says otherwise), glints */
/* from then on, and the scene's Frost event runs. */
void fdPsynergyFrost(void) {
	VECTOR3 pos;
	FIELD_EFFECT_STATE *scene;
	ACTOR *target;
	ACTOR *particle;
	PENDING_EVENT *event;
	s32 i;

	scene = Ram_HeapSlots->fieldEffState;
	target = scene->tgt;
	fdPsynergyFadeToCastCol();
	sySndPlayStub(SE_ICE);
	for(i = 0; i < FROST_SNOWFLAKES; i++) {
		particle = fdActorSpawn(SPRITE_SNOWFLAKE, 0, 0, 0);
		if(particle != NULL) {
			u32 scale;
			FIXED dist;

			scale = (syRandom() >> 1) + FIXED_ONE / 2;
			particle->scaleY = scale;
			particle->scaleX = scale;
			if((syRandom() & 1) != 0)
				particle->update =
					(OBJECT_UPDATE)fdEffectUpdateOrbitingParticleLeft;
			else
				particle->update =
					(OBJECT_UPDATE)fdEffectUpdateOrbitingParticleRight;

			particle->facing = syRandom();
			particle->tmr = 60;
			particle->userValue = syRandom();
			fdActorSetSprColModeStub((ACTOR *)particle, SPRITE_COLOR_BLUE_TINT);

			SET_POSITION(&pos, scene->x, scene->y, scene->z);
			dist = (syRandom() << 2) + toFIXED(2);
			syVectorAddPolarOffset(dist, syRandom(), &pos);
			particle->tgtX = pos.x;
			particle->tgtY = pos.y;
			particle->tgtZ = pos.z;
		}

		sySynch(3);
	}

	sySynch(10);
	sySndPlayStub(SE_ICE);
	sySynch(50);

	if(target != NULL && scene->aimAhead == 0) {
		sySndPlayStub(SE_POWER_UP);

		for(i = 0; i < 8; i++) {
			fdActorSetSprColModeStub((ACTOR *)target, SPRITE_COLOR_WHITE);
			sySynch(1);
			fdActorSetSprColModeStub((ACTOR *)target, SPRITE_COLOR_NORMAL);
			sySynch(4);
		}

		if(scene->sceneVariant == 0) {
			sySndPlayStub(SE_SHIMMER);
			fdActorSelectAnimationStub(target, 2);
		}

		target->update = (OBJECT_UPDATE)fdEffectUpdateFrostGlint;
		event = scFindPsynergyTrigger(
			PSYNERGY_TRIGGER(PSYNERGY_PHASE_ACTS), FIELD_PSYNERGY_FROST,
			scSelectPsynergyTarget(FIELD_PSYNERGY_FROST));
		if(event != NULL)
			fdRunPsynergyEvent(event, scene->casterId, scene->tgtId);
		sySynch(20);
	}

	fdPsynergyFadeFromCastCol();
}

/* Per-frame update: sways 4 pixels either side of targetX, once every 128 */
/* frames. */
void fdEffectSwayParticle(ACTOR *actor) {
	FIXED amplitude = toFIXED(4);

	actor->x.fixed =
		actor->tgtX + syMulFX(amplitude, sySin(actor->tmr * (TRIG_TURN / 128)));
	actor->tmr++;
	actor->tmr = (actor->tmr + 128) % 128;
}

/* The particle circles 10 pixels above the scene's position, its radius in */
/* pixels the frames it has left; at 0 it runs the shared particle script. */
void fdEffectUpdateShrinkingOrbitActor(ACTOR *particle) {
	FIELD_EFFECT_STATE *effectState = Ram_HeapSlots->fieldEffState;
	VECTOR3 pos;
	s16 remainingFrames;
	s32 decrementedTimer;

	if(particle != NULL) {
		decrementedTimer = (u16)particle->tmr - 1;
		particle->tmr = decrementedTimer;
		remainingFrames = decrementedTimer;
		if(remainingFrames != 0) {
			/* The angle is 1/32 turn further round for each frame left. */
			pos.x = effectState->x;
			pos.y = effectState->y + toFIXED(10);
			pos.z = effectState->z;
			syVectorAddPolarOffset(
				INTtoFIXED(remainingFrames),
				particle->userValue + remainingFrames * (TRIG_TURN / 32), &pos);
			particle->x.fixed = pos.x;
			particle->y.fixed = pos.y;
			particle->z.fixed = pos.z;
		}
		else {
			fdActorStartScriptStub(particle, FieldEffect_ReleaseScript);
		}
	}
}

/* Per-frame update: sways 6 pixels either side (a 64-frame cycle) 16 pixels */
/* above the Psynergy's position, and every 3 frames sheds a spark that sways */
/* as it slowly falls. */
void fdEffectRunSparkEmitter(ACTOR *emitter) {
	FIELD_EFFECT_STATE *effectState;
	ACTOR *spark;
	FIXED pos[3];

	effectState = Ram_HeapSlots->fieldEffState;
	if(emitter->tmr != -1) {
		FIXED amplitude = toFIXED(6);

		SET_ACTOR_POSITION(
			emitter,
			effectState->x +
				syMulFX(amplitude, sySin(emitter->tmr * (TRIG_TURN / 64))),
			effectState->y + toFIXED(16), effectState->z);
		emitter->tmr++;
		emitter->tmr = (emitter->tmr + 64) % 64;
	}
	if(FrameCount % 3 == 0) {
		/* From 2 pixels up, 0 to 6 pixels in a random direction. */
		pos[0] = emitter->x.fixed;
		pos[1] = emitter->y.fixed + toFIXED(2);
		pos[2] = emitter->z.fixed;
		syVectorAddPolarOffset(syRandom() * 6, syRandom(), pos);
		spark = fdActorSpawn(SPRITE_SPARKLE, pos[0], pos[1], pos[2]);
		/* Each is released after 72 frames, its script's wait. */
		if(spark != 0) {
			spark->update = (OBJECT_UPDATE)fdEffectSwayParticle;
			spark->scaleX = spark->scaleY = FIXED_ONE * 3 / 5;
			spark->motionFlg = ACTOR_FALLS;
			spark->gravity = toFIXED(0.007);
			spark->tmr = syRandom() >> 9;
			spark->tgtX = spark->x.fixed;
			fdActorSetSprColModeStub(spark, SPRITE_COLOR_BLUE_TINT);
			spark->wait = 72;
			fdActorStartScriptStub(spark, FieldEffect_ReleaseScript);
		}
	}
}

/* Ply cast on a target: runs Ply, then stops the caster's hook. */
void fdPsynergyPlyOnTarget(void) {
	fdPsynergyPly();
	fdBattleEffectStopMainActor();
}

/* Ply's flashing figure: its steps, of 2 frames each. */
#define PLY_FLASH_STEPS 30

/* Ply: eight winged figures spiral in on the Psynergy's position, one a */
/* frame; then one in the centre flashes for 60 frames, sheds sparks and flies
 */
/* off. */
void fdPsynergyPly(void) {
	FIELD_EFFECT_STATE *effectState = Ram_HeapSlots->fieldEffState;
	ACTOR *target = effectState->caster;
	ACTOR *actor;
	SPR *last;
	u8 resourceSlot;
	s32 i;

	fdPsynergyFadeToCastCol();

	/* The spiralling particles: one a frame, each 1/8 turn on from the last. */
	last = 0;
	for(i = 0; i < 8; i++) {
		actor = fdActorSpawn(SPRITE_WINGED_FIGURE, target->x.fixed,
							 target->y.fixed + toFIXED(32), target->z.fixed);
		if(actor != 0) {
			actor->scaleY = FIXED_ONE * 7 / 10;
			actor->scaleX = FIXED_ONE * 7 / 10;
			actor->update = (OBJECT_UPDATE)fdEffectUpdateShrinkingOrbitActor;
			actor->tmr = 120;
			actor->userValue = i * (TRIG_TURN / 8);
			actor->motionFlg = ACTOR_FLOATS;
			last = fdSprShareVramCacheEntry(actor->spr, last);
		}
		sySynch(1);
	}

	/* The particles share one sprite cache; keep its slot to free at the end.
	 */
	/* last is read with no check that any particle spawned. */
	resourceSlot = last->slot;
	sySndPlayStub(SE_PSYNERGY_START);
	/* The last particle spawned 7 frames ago with 120 frames to live: wait */
	/* until the spiral is nearly closed. */
	sySynch(110);

	/* The one in the centre. */
	target = fdActorSpawn(SPRITE_WINGED_FIGURE, 0, 0, 0);
	/* FAKEMATCH(temporary): actor, a copy of target, the figure read by both
	 * names. */
	actor = target;
	if(target != 0) {
		target->scaleY = FIXED_ONE * 7 / 10;
		target->scaleX = FIXED_ONE * 7 / 10;
		target->x.fixed = effectState->x;
		target->y.fixed = effectState->y + toFIXED(16);
		target->z.fixed = effectState->z;
		target->motionFlg = ACTOR_FLOATS;
		fdActorSetSprColModeStub(target, SPRITE_COLOR_WHITE);
	}

	sySndPlayStub(SE_PSYNERGY_SPARKLE);
	sySynch(12);
	if(target != 0) {
		/* Red one step in four, else blue, a step each 2 frames. */
		for(i = 0; i < PLY_FLASH_STEPS; i++) {
			/* FAKEMATCH(dead): target set again to actor at each step; as the
			 * loop without it. */
			target = actor;
			if(i & 3)
				fdActorSetSprColModeStub(actor, SPRITE_COLOR_BLUE_TINT);
			else
				fdActorSetSprColModeStub(actor, SPRITE_COLOR_RED_TINT);
			sySynch(2);
		}
	}

	/* Sparks. */
	fdActorSetSprColModeStub(actor, SPRITE_COLOR_NORMAL);
	sySndPlayStub(MUS_POISON_CURED);
	if(actor != 0) {
		target->update = (OBJECT_UPDATE)fdEffectRunSparkEmitter;
		target->tmr = 0;
		/* Cast on a target (enabled cleared by fdPsynergyDispatch), */
		/* the sparks fall for 192 frames instead of 128. */
		if(effectState->aimAhead != 0)
			sySynch(128);
		else
			sySynch(192);
	}

	/* Fly away: 192 pixels at angle TRIG_TURN * 29 / 32, and release it once it
	 */
	/* arrives. */
	if(target != 0) {
		target->tmr = -1;
		target->speedLimit = toFIXED(5);
		target->acceleration = toFIXED(2) / 5;
		target->steeringFlg = 0;
		fdActorSetTargetPosFromMagnitudeAng(target, toFIXED(192),
											TRIG_TURN * 29 / 32);
		fdActorWaitForMoveStub(target);
		fdActorReleaseStub(target);
	}
	/* syVramCacheFindFreeEntry returns VRAM_CACHE_ENTRY_COUNT for no entry. */
	if(resourceSlot != VRAM_CACHE_ENTRY_COUNT)
		syVramCacheFreeEntry(resourceSlot);
	if(effectState->finishFunc != 0)
		effectState->finishFunc();
	fdPsynergyFadeFromCastCol();
}

/* A task of Cloak: when the controlled character does not */
/* stand on a tile Cloak can hide on, requests CLOAK_END (Cloak, packed). */
void fdEffectCheckCloakTile(void) {
	FIELD_STATE *event = Ram_HeapSlots->fieldState;
	ACTOR *leader = fdActorLookup(fdPartyGetLeaderActor());

	if(fdMapGridIsCloakCell(leader->x.fixed, leader->z.fixed, leader->layer) ==
	   0)
		event->psynergy = ABILITY_CLOAK | ACT_PACKED;
}

/* Show the Cloaked caster again: stop checking the ground under it, then */
/* flicker it back into view over twenty frames and clear the cloak */
/* (fdPsynergyRunExtraHandler). */
void fdPsynergyEndCloak(void) {
	ACTOR *caster =
		((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->caster;
	SPR *sprite = caster->spr;
	SPR_ANIMATION *entry = sprite->entries[0];
	u32 i;

	sySndPlayStub(SE_CAST);
	syCloseEvent(fdEffectCheckCloakTile);
	fdActorSelectAnimationStub(caster, ANIM_STILL);
	caster->update = 0;

	/* Blink between the flicker palette half transparent and solid... */
	for(i = 0; i < 5; ++i) {
		entry->colMode = SPRITE_COLOR_WHITE;
		sprite->flg = SPRITE_FLAG_OUTLINED;
		sprite->dirty = 1;
		sySynch(2);
		sprite->dirty = 1;
		sprite->flg = 0;
		sySynch(2);
	}

	/* ...then between the flicker palette and the caster's own. */
	for(i = 0; i < 5; ++i) {
		entry->colMode = SPRITE_COLOR_WHITE;
		sprite->flg = 0;
		sprite->dirty = 1;
		sySynch(2);
		entry->colMode = SPRITE_COLOR_NORMAL;
		sprite->dirty = 1;
		sySynch(2);
	}

	sprite->flg = SPRITE_FLAG_SHADOW;
	MainRoutineStateWork.cloaked = 0;
}

/* Starts the Cloak cell check if the leader is hidden. fdSequenceRun calls */
/* it as a scene loads, since MainRoutineStateWork.cloaked outlives the scene.
 */
void fdPsynergyResumeCloakCheck(void) {
	if(MainRoutineStateWork.cloaked != 0)
		sySetEvent(fdEffectCheckCloakTile, TASK_PASS_FRAME);
}
