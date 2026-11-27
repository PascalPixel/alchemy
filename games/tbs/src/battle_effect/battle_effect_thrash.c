/* Thrash, Quick Strike and Vorpal Slash. Part of battle_effect.c. */

/* The flashing strike's ticks, from the user's arrival: four pictures flash
   four ticks each, then the canvas floods at its brightest for four ticks,
   the damage shows and the target is hurled. */
#define FLASHING_STRIKE_TICKS		21
#define FLASHING_STRIKE_FLOOD_TICK	16
#define FLASHING_STRIKE_DAMAGE_TICK 18
#define FLASHING_STRIKE_HIT_TICK	20

/* One 120x120 picture of a flashing strike. */
#define FLASHING_STRIKE_PICTURE_BYTES (SLASH_PICTURE_SIZE * SLASH_PICTURE_SIZE)

/* The flashing strike's modes (btEffectRunFlashingStrike): which two pictures
   it shows. */
typedef enum FLASHING_STRIKE_MODE {
	/* UnusedFlashingStrikeImageA and B; nothing passes it */
	FLASHING_STRIKE_UNUSED,
	FLASHING_STRIKE_QUICK_STRIKE, /* QuickStrikeImageA and B, with a hop */
	FLASHING_STRIKE_VORPAL_SLASH  /* VorpalSlashImageA and B */
} FLASHING_STRIKE_MODE;

/* The user leaps high at the first target, then sparks burst beneath it.

   Abilities: Thrash. */
void btEffectRunThrash(BATTLE_EFFECT_PARM *args)
{
	btMotionApproachTargetStub(
		args->user, args->targets[0], 24, toFIXED(61) / 5);
	sySynch(29);
	args->variant = SPARK_GROUPS_THRASH;
	btEffectRunSparkGroups(args, SPARK_GROUPS_RISING);
}

/* Unused: no row of AbilityBattleEffects names its kind. The user would leap at
   the first target, then sparks would burst beneath it and the canvas flash; it
   is Severe Blow's effect in The Lost Age. */
void btEffectRunUnusedSevereBlowLeap(BATTLE_EFFECT_PARM *args)
{
	btMotionApproachTargetStub(
		args->user, args->targets[0], 24, toFIXED(36) / 5);
	sySynch(12);
	args->variant = SPARK_GROUPS_SEVERE_BLOW;
	btEffectRunSparkGroups(args, SPARK_GROUPS_RISING);
}

/* The user leaps at the first target, four pictures flash across the effect
   layer, then it fills with one colour as the target is struck.

   Abilities: Quick Strike. */
void btEffectRunQuickStrike(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFlashingStrike(args, FLASHING_STRIKE_QUICK_STRIKE);
}

/* The user steps up to the first target, four pictures flash across the effect
   layer, then it fills with one colour as the target is struck.

   Abilities: Vorpal Slash. */
void btEffectRunVorpalSlash(BATTLE_EFFECT_PARM *args)
{
	btEffectRunFlashingStrike(args, FLASHING_STRIKE_VORPAL_SLASH);
}

/* Plays a flashing strike; nothing passes FLASHING_STRIKE_UNUSED. */
void btEffectRunFlashingStrike(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	DRAW_RECTANGLE		 draw[2];
	Sint32				 frame;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	btEffectStartCanvasLayer(0);
	REG_BLDCNT = 0;

	if(mode == FLASHING_STRIKE_UNUSED) {
		btResourceLoadAndDecompress(
			RESOURCE_UNUSED_FLASHING_STRIKE_IMAGE_A, state->imageStorage, 1, 0);
		btResourceLoadAndDecompress(RESOURCE_UNUSED_FLASHING_STRIKE_IMAGE_B,
			RamMapGridMetatileBuffer, 1, 1);
	} else if(mode == FLASHING_STRIKE_QUICK_STRIKE) {
		btResourceLoadAndDecompress(
			RESOURCE_QUICK_STRIKE_IMAGE_A, state->imageStorage, 1, 0);
		btResourceLoadAndDecompress(
			RESOURCE_QUICK_STRIKE_IMAGE_B, RamMapGridMetatileBuffer, 1, 1);
	} else {
		btResourceLoadAndDecompress(
			RESOURCE_VORPAL_SLASH_IMAGE_A, state->imageStorage, 1, 0);
		btResourceLoadAndDecompress(
			RESOURCE_VORPAL_SLASH_IMAGE_B, RamMapGridMetatileBuffer, 1, 1);
	}
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);

	/* The user closes on the target in 16 ticks, Quick Strike with a hop. */
	if(mode == FLASHING_STRIKE_QUICK_STRIKE)
		btMotionApproachTargetStub(
			args->user, args->targets[0], 16, toFIXED(8));
	else
		btMotionApproachTargetStub(args->user, args->targets[0], 16, 0);
	sySynch(16);

	if(state->args->side == EFFECT_SIDE_PARTY)
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_COPY);
	else
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_COPY);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	sySoundPlayStub(SE_POWER_UP);
	for(frame = 0; frame != FLASHING_STRIKE_TICKS; frame++) {
		if(frame <= 3)
			draw[0](canvas, state->imageStorage, 0, 0, SLASH_PICTURE_SIZE,
				SLASH_PICTURE_SIZE);
		else if(frame <= 7)
			draw[0](canvas, state->imageStorage + FLASHING_STRIKE_PICTURE_BYTES,
				0, 0, SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE);
		else if(frame <= 11)
			draw[0](canvas, RamMapGridMetatileBuffer, 0, 0, SLASH_PICTURE_SIZE,
				SLASH_PICTURE_SIZE);
		else if(frame <= 15)
			draw[0](canvas,
				RamMapGridMetatileBuffer + FLASHING_STRIKE_PICTURE_BYTES, 0, 0,
				SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE);
		/* The flood: four ticks of the canvas at its brightest. */
		if(frame >= FLASHING_STRIKE_FLOOD_TICK &&
			frame <= FLASHING_STRIKE_FLOOD_TICK + 3)
			syWordsFill(
				canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		if(frame == FLASHING_STRIKE_DAMAGE_TICK)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		if(frame == FLASHING_STRIKE_HIT_TICK) {
			state->shakeFrame = 8;
			btMotionApplyKnockbackStub(state->args->targets[0], KNOCKBACK_HURL);
		}
		btCameraShake(16, 16);
		EFFECT_END_TICK(state);
	}
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#undef FLASHING_STRIKE_TICKS
#undef FLASHING_STRIKE_FLOOD_TICK
#undef FLASHING_STRIKE_DAMAGE_TICK
#undef FLASHING_STRIKE_HIT_TICK
#undef FLASHING_STRIKE_PICTURE_BYTES
