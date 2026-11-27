/* Annihilation: a fire streak from the user, then a burst of fire and shards on
   the target. Part of battle_effect.c. */

/* The shards the burst throws. */
#define SHARD_COUNT		  64
/* The frames the first four shards live; each four after live two longer. */
#define SHARD_LIFE		  16
/* The stronger variant's three burst pictures, after the basic one's. */
#define BURST_STRIP_BYTES (3 * PICTURE_BYTES(FIRE_BURST))

/* The effect's length and the frames of its strike, flash and burst. */
#define ANNIHILATION_FRAMES 64
#define STREAK_FRAME		6 /* the user's fire streak, six frames */
#define HIT_FRAME			8
#define FLASH_FRAME			14
#define BURST_FRAME			16 /* the burst on the target, until BURST_LAST_FRAME */
#define BURST_LAST_FRAME	47

/* The user closes on the first target and fire bursts from it in a shower of
   shards.

   Abilities: Annihilation. */
void btEffectRunAnnihilation(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	Uint8				*particleSheet;
	BATTLE_CAMERA		*camera;
	MOTION_OBJECT		*targetActor;
	PARTICLE			*shard;
	SCREEN_POINT		 position;
	SCREEN_POINT		 origin;
	SCREEN_POINT		 out;
	DRAW_RECTANGLE		 blitters[2];
	Sint32				 i;
	Sint32				 size;
	Uint8				*burstPixels;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	particleSheet = Ram_HeapSlots->battleSheet;
	camera = Ram_HeapSlots->cameraState;
	burstPixels = Ram_MapMetatileBuffer;
	btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(RESOURCE_FIRE_STREAK_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_FIRE_BURST_SHEET, burstPixels, 1, 1);
	btResourceLoadAndDecompress(
		RESOURCE_YELLOW_GLOW_SHEET, particleSheet, 0, 0);
	btMotionApproachTargetStub(args->user, args->tgts[0], 4, 0);
	sySynch(1);
	targetActor = btObjectGetStub(args->tgts[0])->obj;

	/* Shards burst from the target. */
	for(i = 0; i != SHARD_COUNT; i++) {
		PARTICLE *seed = &state->particles[i];

		SET_POSITION(seed, targetActor->x, targetActor->y, targetActor->z);
		seed->velX = (syRandom() & 255) << 11;
		seed->velY = ((syRandom() & 255) - 127) << 12;
		seed->velZ = ((syRandom() & 255) - 127) << 12;
		if(seed->x > 0)
			seed->velX = -seed->velX;
		seed->variant = i / 4 * 2 + SHARD_LIFE;
	}

	btEffectPosProjectScaledBattler(args->tgts[0], &origin);
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	EFFECT_SET_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	for(frame = 0; frame != ANNIHILATION_FRAMES; frame++) {
		if(frame == HIT_FRAME)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		if(args->variant != EFFECT_VARIANT_BASIC && frame == HIT_FRAME)
			sySndPlayStub(SE_POWER_UP);
		btEffectPosProjectScaledBattler(args->user, &position);
		/* The fire streak in front of the user. */
		if(frame >= STREAK_FRAME && frame <= STREAK_FRAME + 5) {
			if(args->side == EFFECT_SIDE_ENEMIES)
				EFFECT_LOAD_BLITTER(
					HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
			else
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
					EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
			blitters[0] = Ram_HeapSlots->ramCode;
			if(args->side == EFFECT_SIDE_ENEMIES)
				blitters[0](canvas,
					state->sheet +
						(frame - STREAK_FRAME) * PICTURE_BYTES(FIRE_STREAK),
					position.x / 2 - FIRE_STREAK_WIDTH / 2, position.y - 24,
					FIRE_STREAK_WIDTH, FIRE_STREAK_HEIGHT);
			else
				blitters[0](canvas,
					state->sheet +
						(frame - STREAK_FRAME) * PICTURE_BYTES(FIRE_STREAK),
					position.x / 2, position.y - 24, FIRE_STREAK_WIDTH,
					FIRE_STREAK_HEIGHT);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
		}
		/* The fire burst over the target grows. */
		if(frame >= BURST_FRAME && frame <= BURST_LAST_FRAME) {
			Sint32 step = (frame - BURST_FRAME) / 2;
			Sint32 strip;

			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
			blitters[0] = Ram_HeapSlots->ramCode;
			if(step > 2)
				step = 2;
			if(args->variant == EFFECT_VARIANT_BASIC)
				strip = 0;
			else
				strip = BURST_STRIP_BYTES;
			blitters[0](canvas,
				Ram_MapMetatileBuffer + strip +
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
		if(frame == HIT_FRAME)
			syWordsFillEntry(
				canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		EFFECT_VIEW_FROM_CAMERA(camera);
		/* Shards shrink and fall. */
		if(frame > 3) {
			btEffectBuildCanvasBlitters(args->side, blitters);
			for(i = 0; i != SHARD_COUNT; i++) {
				Sint32 life;

				shard = &state->particles[i];
				life = shard->variant;
				if(life > 0) {
					btEffectPosProjectPoint(shard, &out);
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
		if(frame == HIT_FRAME) {
			btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_HURL);
			state->shakeFrm = 4;
		}
		if(frame == STREAK_FRAME)
			btEffectTintBattler(
				args->tgts[0], SPRITE_COLOR_RED_TINT, TINT_KEEP, TINT_KEEP, 0);
		if(frame == FLASH_FRAME)
			btEffectTintBattler(
				args->tgts[0], SPRITE_COLOR_RED_TINT, TINT_KEEP, TINT_KEEP, 0);
		btCameraShake(16, 16);
		EFFECT_END_FRAME(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#undef SHARD_COUNT
#undef SHARD_LIFE
#undef BURST_STRIP_BYTES
#undef ANNIHILATION_FRAMES
#undef STREAK_FRAME
#undef HIT_FRAME
#undef FLASH_FRAME
#undef BURST_FRAME
#undef BURST_LAST_FRAME
