/* Annihilation. Part of battle_effect.c. */

#define ANNIHILATION_TICKS			 64
#define ANNIHILATION_SHARDS			 64
/* The first four shards' life in ticks; each four after live two ticks
   longer. */
#define ANNIHILATION_SHARD_LIFE		 16
/* The fire streak in front of the user, its pictures a tick each from here,
   and the target's first red flash. */
#define ANNIHILATION_STREAK_TICK	 6
#define ANNIHILATION_STREAK_PICTURES 6
/* The white flash, the damage and the knock-back. */
#define ANNIHILATION_HIT_TICK		 8
#define ANNIHILATION_FLASH_TICK		 14 /* the target's second red flash */
/* The fire burst over the target, from ANNIHILATION_BURST_TICK to
   ANNIHILATION_BURST_LAST_TICK: three pictures two ticks each, the last
   held. */
#define ANNIHILATION_BURST_TICK		 16
#define ANNIHILATION_BURST_LAST_TICK 47
#define ANNIHILATION_BURST_PICTURES	 3

/* The user closes on the first target and fire bursts from it in a shower of
   shards, the flames settling into the kanji for destroy over it (the stronger
   variant, which no ability plays, draws the kanji for kill).

   Abilities: Annihilation. */
void btEffectRunAnnihilation(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Uint8				*particleSheet;
	BATTLE_CAMERA		*camera;
	ACTOR				*targetActor;
	PARTICLE			*shard;
	SCREEN_POINT		 pos;
	SCREEN_POINT		 origin;
	SCREEN_POINT		 out;
	DRAW_RECTANGLE		 blitters[2];
	Sint32				 i;
	Sint32				 frame;
	Sint32				 size;
	Uint8				*burstPixels;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	particleSheet = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	camera = HEAP_SLOT_BLOCK(HEAP_SLOT_CAMERA);
	burstPixels = RamMapGridMetatileBuffer;
	state->args = args;
	btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(RESOURCE_FIRE_STREAK_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_FIRE_BURST_SHEET, burstPixels, 1, 1);
	btResourceLoadAndDecompress(
		RESOURCE_YELLOW_GLOW_SHEET, particleSheet, 0, 0);
	btMotionApproachTargetStub(
		state->args->user, state->args->targets[0], 4, 0);
	sySynch(1);
	targetActor = btObjectGetStub(state->args->targets[0])->actor;

	for(i = 0; i != ANNIHILATION_SHARDS; i++) {
		/* Shards burst from the target. */
		PARTICLE *seed = &state->particles[i];

		SET_POSITION(seed, targetActor->x.fixed, targetActor->y.fixed,
			targetActor->z.fixed);
		SET_VELOCITY(seed, (syRandom() & 255) << 11,
			((syRandom() & 255) - 127) << 12, ((syRandom() & 255) - 127) << 12);
		if(seed->x > 0)
			seed->velocityX = -seed->velocityX;
		seed->variant = i / 4 * 2 + ANNIHILATION_SHARD_LIFE;
	}

	btProjectionScaledUnit(state->args->targets[0], &origin);
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	EFFECT_SET_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	for(frame = 0; frame != ANNIHILATION_TICKS; frame++) {
		if(frame == ANNIHILATION_HIT_TICK)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		if(state->args->variant != EFFECT_VARIANT_BASIC &&
			frame == ANNIHILATION_HIT_TICK)
			sySoundPlayStub(SE_POWER_UP);
		btProjectionScaledUnit(state->args->user, &pos);
		/* The fire streak in front of the user. */
		if(frame >= ANNIHILATION_STREAK_TICK &&
			frame < ANNIHILATION_STREAK_TICK + ANNIHILATION_STREAK_PICTURES) {
			if(state->args->side == EFFECT_SIDE_ENEMIES)
				EFFECT_LOAD_BLITTER(
					HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
			else
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
					EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_ADDITIVE);
			blitters[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
			if(state->args->side == EFFECT_SIDE_ENEMIES)
				blitters[0](canvas,
					state->sheet +
						(frame - ANNIHILATION_STREAK_TICK) *
							PICTURE_BYTES(FIRE_STREAK),
					pos.x / 2 - FIRE_STREAK_WIDTH / 2, pos.y - 24,
					FIRE_STREAK_WIDTH, FIRE_STREAK_HEIGHT);
			else
				blitters[0](canvas,
					state->sheet +
						(frame - ANNIHILATION_STREAK_TICK) *
							PICTURE_BYTES(FIRE_STREAK),
					pos.x / 2, pos.y - 24, FIRE_STREAK_WIDTH,
					FIRE_STREAK_HEIGHT);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
		}
		/* The fire burst over the target grows. */
		if(frame >= ANNIHILATION_BURST_TICK &&
			frame <= ANNIHILATION_BURST_LAST_TICK) {
			Sint32 step = (frame - ANNIHILATION_BURST_TICK) / 2;
			Sint32 strip;

			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
			blitters[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
			if(step > ANNIHILATION_BURST_PICTURES - 1)
				step = ANNIHILATION_BURST_PICTURES - 1;
			if(state->args->variant == EFFECT_VARIANT_BASIC)
				strip = 0;
			else
				strip = ANNIHILATION_BURST_PICTURES * PICTURE_BYTES(FIRE_BURST);
			blitters[0](canvas,
				RamMapGridMetatileBuffer + strip +
					step * PICTURE_BYTES(FIRE_BURST),
				origin.x / 2 - FIRE_BURST_WIDTH / 2, origin.y - 48,
				FIRE_BURST_WIDTH, FIRE_BURST_HEIGHT);
			/* A busy delay before the blitter is freed. XXX (no evidence): why
			   it waits; the delay changes nothing the game draws or computes,
			   so only the author's intent would say. */
			btEffectCallNoOperation(10000);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
		}
		/* The strike flashes the canvas. */
		if(frame == ANNIHILATION_HIT_TICK) {
			DMA_FILL32(
				CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST), canvas, CANVAS_BYTES);
		}
		EFFECT_VIEW_FROM_CAMERA(camera);
		/* Shards shrink and fall. */
		if(frame > 3) {
			btEffectBuildCanvasBlitters(state->args->side, blitters);
			for(i = 0; i != ANNIHILATION_SHARDS; i++) {
				Sint32 life;

				shard = &state->particles[i];
				life = shard->variant;
				if(life > 0) {
					btProjectionPoint(shard, &out);
					out.x >>= 1;
					out.y = out.y + origin.y - 112;
					size = (life >> 3) + 2;
					EFFECT_DRAW_MOTE(blitters[(i / 2) & 1], canvas,
						particleSheet, out.x, out.y, size);
					btParticleAdvanceWithGravity3D(shard, 60, -toFIXED(1) / 64);
					shard->variant--;
				}
			}
			EFFECT_RELEASE_BLITTERS();
		}
		/* The strike throws the target back. */
		if(frame == ANNIHILATION_HIT_TICK) {
			btMotionApplyKnockbackStub(state->args->targets[0], KNOCKBACK_HURL);
			state->shakeFrame = 4;
		}
		if(frame == ANNIHILATION_STREAK_TICK)
			btEffectTintBattler(state->args->targets[0], SPRITE_COLOR_RED_TINT,
				TINT_KEEP, TINT_KEEP, 0);
		if(frame == ANNIHILATION_FLASH_TICK)
			btEffectTintBattler(state->args->targets[0], SPRITE_COLOR_RED_TINT,
				TINT_KEEP, TINT_KEEP, 0);
		btCameraShake(16, 16);
		EFFECT_END_TICK(state);
	}
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#undef ANNIHILATION_TICKS
#undef ANNIHILATION_SHARDS
#undef ANNIHILATION_SHARD_LIFE
#undef ANNIHILATION_STREAK_TICK
#undef ANNIHILATION_STREAK_PICTURES
#undef ANNIHILATION_HIT_TICK
#undef ANNIHILATION_FLASH_TICK
#undef ANNIHILATION_BURST_TICK
#undef ANNIHILATION_BURST_LAST_TICK
#undef ANNIHILATION_BURST_PICTURES
