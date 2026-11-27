/* @file */
/* Thrash, Mighty Press and Severe Blow, and the flashing strikes: Quick */
/* Strike, Vorpal Slash, Lunar Slash, Boost Hack and Twin Shear. */
/* Part of battle_effect.c. */

/* The flashing strike's frames, from the user's arrival: four pictures flash */
/* four frames each, then the canvas floods at its brightest for four frames, */
/* the damage shows and the target is hurled. */
#define FLASHING_STRIKE_FRAMES		 21
#define FLASHING_STRIKE_FLOOD_FRAME	 16
#define FLASHING_STRIKE_DAMAGE_FRAME 18
#define FLASHING_STRIKE_HIT_FRAME	 20

/* One 120x120 picture of a flashing strike; Twin Shear's are half as tall. */
#define FLASHING_STRIKE_PICTURE_BYTES (SLASH_PICTURE_SIZE * SLASH_PICTURE_SIZE)
#define FLASHING_STRIKE_HALF_BYTES	  (SLASH_PICTURE_SIZE * SLASH_PICTURE_SIZE / 2)

/* The flashing strike's modes (btEffectRunFlashingStrike), each named for */
/* the ability that plays it: which pictures it shows and how the user moves. */
typedef enum FLASHING_STRIKE_MODE {
	FLASHING_STRIKE_UNUSED, /* UnusedFlashingStrikeImageA and F; nothing passes
							 */
							/* it */
	FLASHING_STRIKE_QUICK_STRIKE, /* QuickStrikeImageA and D, with a hop */
	FLASHING_STRIKE_VORPAL_SLASH, /* VorpalSlashImageA and B */
	FLASHING_STRIKE_LUNAR_SLASH,  /* QuickStrikeImageA and D, then Weaken */
	FLASHING_STRIKE_BOOST_HACK,	  /* VorpalSlashImageA and B in other colours, a
								   */
								  /* pose */
	FLASHING_STRIKE_TWIN_SHEAR /* the arc fragments, each with its reflection */
} FLASHING_STRIKE_MODE;

/* The user leaps high at the first target, then sparks burst beneath it. */
/* */
/* Abilities: Thrash. */
void btEffectRunThrash(BATTLE_EFFECT_PARM *args) {
	btMotionApproachTargetStub(args->user, args->tgts[0], 24, toFIXED(61) / 5);
	sySynch(29);
	args->variant = SPARK_GROUPS_THRASH;
	btEffectRunSparkGroups(args, SPARK_GROUPS_RISING);
}

/* The user leaps high at the first target, as in Thrash, then sparks burst */
/* beneath it, many more than Thrash's. */
/* */
/* Abilities: Mighty Press. */
void btEffectRunMightyPress(BATTLE_EFFECT_PARM *args) {
	btMotionApproachTargetStub(args->user, args->tgts[0], 24, toFIXED(61) / 5);
	sySynch(26);
	args->variant = SPARK_GROUPS_MIGHTY_PRESS;
	btEffectRunSparkGroups(args, SPARK_GROUPS_RISING);
}

/* The user leaps at the first target, then sparks burst beneath it and the */
/* canvas flashes. */
/* */
/* Abilities: Severe Blow. */
void btEffectRunSevereBlow(BATTLE_EFFECT_PARM *args) {
	btMotionApproachTargetStub(args->user, args->tgts[0], 24, toFIXED(36) / 5);
	sySynch(12);
	args->variant = SPARK_GROUPS_SEVERE_BLOW;
	btEffectRunSparkGroups(args, SPARK_GROUPS_RISING);
}

/* The user leaps at the first target, four pictures flash across the effect */
/* layer, then it fills with one colour as the target is struck. */
/* */
/* Abilities: Quick Strike. */
void btEffectRunQuickStrike(BATTLE_EFFECT_PARM *args) {
	btEffectRunFlashingStrike(args, FLASHING_STRIKE_QUICK_STRIKE);
}

/* The user steps up to the first target, four pictures flash across the */
/* effect layer, then it fills with one colour as the target is struck. */
/* */
/* Abilities: Vorpal Slash. */
void btEffectRunVorpalSlash(BATTLE_EFFECT_PARM *args) {
	btEffectRunFlashingStrike(args, FLASHING_STRIKE_VORPAL_SLASH);
}

/* The user leaps where it stands, four pictures flash over the battle, then */
/* the effect layer fills with one colour as the target is struck and Weaken */
/* follows. */
/* */
/* Abilities: Lunar Slash. */
void btEffectRunLunarSlash(BATTLE_EFFECT_PARM *args) {
	btEffectRunFlashingStrike(args, FLASHING_STRIKE_LUNAR_SLASH);
}

/* The user takes its leap pose, Vorpal Slash's four pictures flash over the */
/* battle in other colours, then the effect layer fills with one colour as the
 */
/* target is struck. */
/* */
/* Abilities: Boost Hack. */
void btEffectRunBoostHack(BATTLE_EFFECT_PARM *args) {
	btEffectRunFlashingStrike(args, FLASHING_STRIKE_BOOST_HACK);
}

/* The user leaps at the first target and a slash sweeps over the battle in */
/* four pictures, each the slash's top half with its reflection below it, then
 */
/* the effect layer fills with one colour as the target is struck. */
/* */
/* Abilities: Twin Shear. */
void btEffectRunTwinShear(BATTLE_EFFECT_PARM *args) {
	btEffectRunFlashingStrike(args, FLASHING_STRIKE_TWIN_SHEAR);
}

/* Plays a flashing strike; nothing passes FLASHING_STRIKE_UNUSED. */
void btEffectRunFlashingStrike(BATTLE_EFFECT_PARM *args, s32 mode) {
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	DRAW_RECTANGLE draw[2];
	s32 frame;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	btEffectStartCanvasLayer(0);
	/* Lunar Slash, Boost Hack and Twin Shear add the canvas to the battle; the
	 */
	/* others show it alone. */
	if(mode < FLASHING_STRIKE_LUNAR_SLASH || mode > FLASHING_STRIKE_TWIN_SHEAR)
		REG_BLDCNT = 0;
	REG_BLDALPHA = EFFECT_BLEND_ADD;

	/* The pictures: 1 and 2 in the sprite sheet, 3 and 4 in the map cell */
	/* buffer (Twin Shear's four half pictures all there). */
	if(mode == FLASHING_STRIKE_UNUSED) {
		btResourceLoadAndDecompress(RESOURCE_UNUSED_FLASHING_STRIKE_IMAGE_A,
									state->sheet, 1, 0);
		btResourceLoadAndDecompress(RESOURCE_UNUSED_FLASHING_STRIKE_IMAGE_B,
									Ram_MapCellBuffer, 1, 1);
	}
	else if(mode == FLASHING_STRIKE_QUICK_STRIKE) {
		btResourceLoadAndDecompress(RESOURCE_QUICK_STRIKE_IMAGE_A, state->sheet,
									1, 0);
		btResourceLoadAndDecompress(RESOURCE_QUICK_STRIKE_IMAGE_B,
									Ram_MapCellBuffer, 1, 1);
	}
	else if(mode == FLASHING_STRIKE_BOOST_HACK) {
		s32 i;

		btResourceLoadAndDecompress(RESOURCE_VORPAL_SLASH_IMAGE_A, state->sheet,
									1, 0);
		btResourceLoadAndDecompress(RESOURCE_VORPAL_SLASH_IMAGE_B,
									Ram_MapCellBuffer, 1, 1);
		/* Turn the canvas's colours: red to blue, blue to green, green to red.
		 */
		for(i = 0; i != SHEET_PALETTE_COLORS; i++) {
			s32 blue = RGB_BLUE(((u16 *)BG_PLTT)[i]);
			s32 green = RGB_GREEN(((u16 *)BG_PLTT)[i]);
			s32 red = RGB_RED(((u16 *)BG_PLTT)[i]);

			((u16 *)BG_PLTT)[i] = RGB(green, blue, red);
		}
	}
	else if(mode == FLASHING_STRIKE_LUNAR_SLASH) {
		btResourceLoadAndDecompress(RESOURCE_QUICK_STRIKE_IMAGE_A, state->sheet,
									1, 0);
		btResourceLoadAndDecompress(RESOURCE_QUICK_STRIKE_IMAGE_B,
									Ram_MapCellBuffer, 1, 1);
	}
	else if(mode == FLASHING_STRIKE_TWIN_SHEAR) {
		btResourceLoadAndDecompress(RESOURCE_ARC_FRAGMENTS_SHEET,
									Ram_MapCellBuffer, 1, 1);
	}
	else {
		btResourceLoadAndDecompress(RESOURCE_VORPAL_SLASH_IMAGE_A, state->sheet,
									1, 0);
		btResourceLoadAndDecompress(RESOURCE_VORPAL_SLASH_IMAGE_B,
									Ram_MapCellBuffer, 1, 1);
	}
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);

	/* The user closes on the target in 16 frames (Lunar Slash jumps in place,
	 */
	/* Boost Hack strikes a pose instead and waits 8). */
	if(mode == FLASHING_STRIKE_QUICK_STRIKE ||
	   mode == FLASHING_STRIKE_TWIN_SHEAR)
		btMotionApproachTargetStub(args->user, args->tgts[0], 16, toFIXED(8));
	else if(mode == FLASHING_STRIKE_LUNAR_SLASH)
		btMotionApproachTargetStub(args->user, args->user, 16, toFIXED(10));
	else if(mode == FLASHING_STRIKE_BOOST_HACK) {
		MOTION_OBJECT *user = btObjectGetStub(args->user)->obj;

		fdActorSelectAnimationStub(user, POSE_LEAP);
		fdActorSetAnimationSpeedStub(user, ANIM_STEP_NORMAL * 3);
	}
	else
		btMotionApproachTargetStub(args->user, args->tgts[0], 16, 0);
	if(mode == FLASHING_STRIKE_BOOST_HACK)
		sySynch(8);
	else
		sySynch(16);

	/* The blitters face the target: one for the picture, one flipped upside */
	/* down for its reflection. */
	if(args->side == EFFECT_SIDE_PARTY)
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
							EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
							BLITTER_ADDITIVE);
	else
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
							BLITTER_ADDITIVE);
	draw[0] = Ram_HeapSlots->ramCode;
	if(args->side == EFFECT_SIDE_PARTY)
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
							EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP,
							BLITTER_ADDITIVE);
	else
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
							EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_Y,
							BLITTER_ADDITIVE);
	draw[1] = Ram_HeapSlots->ramCodePaired;

	sySndPlayStub(SE_POWER_UP);
	for(frame = 0; frame != FLASHING_STRIKE_FRAMES; frame++) {
		if(mode == FLASHING_STRIKE_TWIN_SHEAR) {
			/* Half the picture, then its reflection below it. */
			if(frame <= 3) {
				draw[0](canvas, Ram_MapCellBuffer, 0, 0, SLASH_PICTURE_SIZE,
						SLASH_PICTURE_SIZE / 2);
				draw[1](canvas, Ram_MapCellBuffer, 0, SLASH_PICTURE_SIZE / 2,
						SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE / 2);
			}
			else if(frame <= 7) {
				draw[0](canvas, Ram_MapCellBuffer + FLASHING_STRIKE_HALF_BYTES,
						0, 0, SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE / 2);
				draw[1](canvas, Ram_MapCellBuffer + FLASHING_STRIKE_HALF_BYTES,
						0, SLASH_PICTURE_SIZE / 2, SLASH_PICTURE_SIZE,
						SLASH_PICTURE_SIZE / 2);
			}
			else if(frame <= 11) {
				draw[0](canvas,
						Ram_MapCellBuffer + FLASHING_STRIKE_HALF_BYTES * 2, 0,
						0, SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE / 2);
				draw[1](canvas,
						Ram_MapCellBuffer + FLASHING_STRIKE_HALF_BYTES * 2, 0,
						SLASH_PICTURE_SIZE / 2, SLASH_PICTURE_SIZE,
						SLASH_PICTURE_SIZE / 2);
			}
			else if(frame <= 15) {
				draw[0](canvas,
						Ram_MapCellBuffer + FLASHING_STRIKE_HALF_BYTES * 3, 0,
						0, SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE / 2);
				draw[1](canvas,
						Ram_MapCellBuffer + FLASHING_STRIKE_HALF_BYTES * 3, 0,
						SLASH_PICTURE_SIZE / 2, SLASH_PICTURE_SIZE,
						SLASH_PICTURE_SIZE / 2);
			}
		}
		else {
			if(frame <= 3)
				draw[0](canvas, state->sheet, 0, 0, SLASH_PICTURE_SIZE,
						SLASH_PICTURE_SIZE);
			else if(frame <= 7)
				draw[0](canvas, state->sheet + FLASHING_STRIKE_PICTURE_BYTES, 0,
						0, SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE);
			else if(frame <= 11)
				draw[0](canvas, Ram_MapCellBuffer, 0, 0, SLASH_PICTURE_SIZE,
						SLASH_PICTURE_SIZE);
			else if(frame <= 15)
				draw[0](canvas,
						Ram_MapCellBuffer + FLASHING_STRIKE_PICTURE_BYTES, 0, 0,
						SLASH_PICTURE_SIZE, SLASH_PICTURE_SIZE);
		}

		/* The flood: four frames of the canvas at its brightest. */
		if(frame >= FLASHING_STRIKE_FLOOD_FRAME &&
		   frame <= FLASHING_STRIKE_FLOOD_FRAME + 3)
			syWordsFillEntry(canvas, CANVAS_BYTES,
							 CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		if(frame == FLASHING_STRIKE_DAMAGE_FRAME)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		if(frame == FLASHING_STRIKE_HIT_FRAME) {
			state->shakeFrm = 8;
			btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_HURL);
		}
		btCameraShake(16, 16);
		EFFECT_END_FRAME(state);
	}

	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
	syCloseEvent(btEffectTransferCanvas);
	if(mode == FLASHING_STRIKE_LUNAR_SLASH) {
		/* Clear the canvas and its tiles and hand the layer to the next scene.
		 */
		syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_VIEW_BYTES);
		syWordsClear(canvas, CANVAS_VIEW_BYTES);
		syCloseEvent(btEffectDarkenBackgroundStep);
		btEffectRunWeaken(args);
	}
	else {
		if(mode == FLASHING_STRIKE_BOOST_HACK)
			fdActorSetAnimationSpeedStub(btObjectGetStub(args->user)->obj,
										 ANIM_STEP_NORMAL);
		btEffectEndCanvasLayer();
	}
}

#undef FLASHING_STRIKE_FRAMES
#undef FLASHING_STRIKE_FLOOD_FRAME
#undef FLASHING_STRIKE_DAMAGE_FRAME
#undef FLASHING_STRIKE_HIT_FRAME
#undef FLASHING_STRIKE_PICTURE_BYTES
#undef FLASHING_STRIKE_HALF_BYTES
