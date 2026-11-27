/* Annihilation: a fire streak from the user, then a burst of fire and shards on
   the target. Part of battle_effect.c. */

/* The shards the burst throws. */
#define SHARD_COUNT		  64
/* The ticks the first four shards live; each four after live two longer. */
#define SHARD_LIFE		  16
/* The stronger variant's three burst pictures, after the basic one's. */
#define BURST_STRIP_BYTES (3 * PICTURE_BYTES(FIRE_BURST))

/* The effect's length and the ticks of its strike, flash and burst. */
#define ANNIHILATION_TICKS 64
#define STREAK_TICK		   6 /* the user's fire streak, six ticks */
#define HIT_TICK		   8
#define FLASH_TICK		   14
#define BURST_TICK		   16 /* the burst on the target, until BURST_LAST_TICK */
#define BURST_LAST_TICK	   47

/* The user closes on the first target and fire bursts from it in a shower of
   shards.

   Abilities: Annihilation. */
void btEffectRunAnnihilation(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	Uint8				*particleSheet;
	BATTLE_CAMERA		*camera;
	MOTION_OBJECT		*tgtActor;
	PARTICLE			*shard;
	SCREEN_POINT		 pos;
	SCREEN_POINT		 origin;
	SCREEN_POINT		 out;
	DRAW_RECTANGLE		 blitters[2];
	Sint32				 i;
	Sint32				 size;
	Uint8				*burstPixels;

	state = RamHeapSlots->battleEff;
	canvas = RamHeapSlots->battleCanvas;
	particleSheet = RamHeapSlots->battleSheet;
	camera = RamHeapSlots->cameraState;
	burstPixels = RamMapMetatileBuffer;
	btEffectStartCanvasLayer(0);
	btResourceLoadAndDecompress(RESOURCE_FIRE_STREAK_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_FIRE_BURST_SHEET, burstPixels, 1, 1);
	btResourceLoadAndDecompress(
		RESOURCE_YELLOW_GLOW_SHEET, particleSheet, 0, 0);
	btMotionApproachTargetStub(args->user, args->tgts[0], 4, 0);
	sySynch(1);
	tgtActor = btObjectGetStub(args->tgts[0])->obj;

	/* Shards burst from the target. */
	for(i = 0; i != SHARD_COUNT; i++) {
		PARTICLE *seed = &state->particles[i];

		SET_POSITION(seed, tgtActor->x, tgtActor->y, tgtActor->z);
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

	for(frm = 0; frm != ANNIHILATION_TICKS; frm++) {
		if(frm == HIT_TICK)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		if(args->variant != EFFECT_VARIANT_BASIC && frm == HIT_TICK)
			sySndPlayStub(SE_POWER_UP);
		btEffectPosProjectScaledBattler(args->user, &pos);
		/* The fire streak in front of the user. */
		if(frm >= STREAK_TICK && frm <= STREAK_TICK + 5) {
			if(args->side == EFFECT_SIDE_ENEMIES)
				EFFECT_LOAD_BLITTER(
					HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
			else
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
					EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
			blitters[0] = RamHeapSlots->ramCode;
			if(args->side == EFFECT_SIDE_ENEMIES)
				blitters[0](canvas,
					state->sheet +
						(frm - STREAK_TICK) * PICTURE_BYTES(FIRE_STREAK),
					pos.x / 2 - FIRE_STREAK_WIDTH / 2, pos.y - 24,
					FIRE_STREAK_WIDTH, FIRE_STREAK_HEIGHT);
			else
				blitters[0](canvas,
					state->sheet +
						(frm - STREAK_TICK) * PICTURE_BYTES(FIRE_STREAK),
					pos.x / 2, pos.y - 24, FIRE_STREAK_WIDTH,
					FIRE_STREAK_HEIGHT);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
		}
		/* The fire burst over the target grows. */
		if(frm >= BURST_TICK && frm <= BURST_LAST_TICK) {
			Sint32 step = (frm - BURST_TICK) / 2;
			Sint32 strip;

			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
			blitters[0] = RamHeapSlots->ramCode;
			if(step > 2)
				step = 2;
			if(args->variant == EFFECT_VARIANT_BASIC)
				strip = 0;
			else
				strip = BURST_STRIP_BYTES;
			blitters[0](canvas,
				RamMapMetatileBuffer + strip + step * PICTURE_BYTES(FIRE_BURST),
				origin.x / 2 - FIRE_BURST_WIDTH / 2, origin.y - 48,
				FIRE_BURST_WIDTH, FIRE_BURST_HEIGHT);
			/* A busy delay before the blitter is freed. XXX (no evidence): why
			   it waits; the delay changes nothing the game draws or computes,
			   so only the author's intent would say. */
			btEffectCallNoOperation(10000);
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
		}
		/* The strike flashes the canvas. */
		if(frm == HIT_TICK)
			syWordsFillEntry(
				canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		EFFECT_VIEW_FROM_CAMERA(camera);
		/* Shards shrink and fall. */
		if(frm > 3) {
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
		if(frm == HIT_TICK) {
			btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_HURL);
			state->shakeFrm = 4;
		}
		if(frm == STREAK_TICK)
			btEffectTintBattler(
				args->tgts[0], SPRITE_COLOR_RED_TINT, TINT_KEEP, TINT_KEEP, 0);
		if(frm == FLASH_TICK)
			btEffectTintBattler(
				args->tgts[0], SPRITE_COLOR_RED_TINT, TINT_KEEP, TINT_KEEP, 0);
		btCameraShake(16, 16);
		EFFECT_END_TICK(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#undef SHARD_COUNT
#undef SHARD_LIFE
#undef BURST_STRIP_BYTES
#undef ANNIHILATION_TICKS
#undef STREAK_TICK
#undef HIT_TICK
#undef FLASH_TICK
#undef BURST_TICK
#undef BURST_LAST_TICK
