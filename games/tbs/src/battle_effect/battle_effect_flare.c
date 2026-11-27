/* @file */
/* Flare. */
/* Part of battle_effect.c. */

#define FLARE_FRAMES 80 /* frames of the flare */

#define FLARE_ARC_COUNT 9 /* flames on the arc */
/* The half ellipse's radii, across and up and down, in canvas pixels. */
#define FLARE_ARC_RADIUS_X 32
#define FLARE_ARC_RADIUS_Y 16
/* A flame's six pictures, four frames each, before it goes out. */
#define FLARE_FLAME_PICTURES 6
#define FLARE_PICTURE_FRAMES 4

/* Nine flames rise one after another along half an ellipse. */
/* */
/* Abilities: Flare, Flare Wall, Flare Storm. */
void btEffectRunFlare(BATTLE_EFFECT_PARM *args) {
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	s32 frame;
	DRAW_RECTANGLE draw;
	s32 i;
	s32 picture;
	u32 width;
	s32 direction;
	s32 originX;
	PARTICLE *arcFlame;
	PARTICLE *flame;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	btEffectStartCanvasLayer(0);
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
				 BLDCNT_TGT2_ALL;
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(CANVAS_BLEND_LEVEL);
	btResourceLoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET, state->sheet, 1,
								1);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
						COMPOSITE_ADDITIVE);
	draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);

	/* The arc of flames. */
	if(state->args->tgts[0] >= FIRST_ENEMY_UNIT) {
		originX = 0;
		direction = 1;
	}
	else {
		originX = 64;
		direction = -1;
	}
	for(i = 0; i != FLARE_ARC_COUNT; i++) {
		s32 angle = i * (TRIG_TURN / 16) - DEGtoANG(90);

		arcFlame = &state->particles[i];
		arcFlame->x =
			((direction * FIXEDtoINT(sySin(angle) * FLARE_ARC_RADIUS_X)) +
			 originX) +
			20;
		arcFlame->y = FIXEDtoINT(syCos(angle) * FLARE_ARC_RADIUS_Y) + 40;
		arcFlame->variant = -i * 4;
	}

	state->transferMode = CANVAS_COPY_AND_FADE;
	if(state->args->variant == EFFECT_VARIANT_STRONGEST) {
		state->transferValue = CANVAS_FADE_THREE_QUARTERS;
	}
	else {
		state->transferValue = CANVAS_FADE_HALF;
	}
	sySetEvent(btEffectTransferCanvas, TASK_PASS_VBLANK);
	sySndPlayStub(SE_WHOOSH);
	for(frame = 0; frame != FLARE_FRAMES; frame++) {
		if(frame == 24) {
			EFFECT_SHOW_DAMAGE(SE_IMPACT);
		}

		/* Each flame plays its pictures and goes out. */
		for(i = 0; i != FLARE_ARC_COUNT; i++) {
			flame = &state->particles[i];
			if(flame->variant >= 0 &&
			   flame->variant < FLARE_FLAME_PICTURES * FLARE_PICTURE_FRAMES) {
				picture = flame->variant / FLARE_PICTURE_FRAMES;
				/* UB: width is assigned in one argument and read in another. */
#ifdef UBFIX
				width = BattleEffect_PuffArcPictureWidths[picture];
				draw(canvas,
					 state->sheet + BattleEffect_PuffArcPictures[picture],
					 flame->x - (width >> 1),
					 flame->y + BattleEffect_PuffArcPictureBiasY[picture],
					 width, BattleEffect_PuffArcPictureHeights[picture]);
#else
				draw(
					canvas,
					state->sheet + BattleEffect_PuffArcPictures[picture],
					flame->x -
						((width = BattleEffect_PuffArcPictureWidths[picture]) >>
						 1),
					flame->y + BattleEffect_PuffArcPictureBiasY[picture], width,
					BattleEffect_PuffArcPictureHeights[picture]);
#endif
				/* Flare Wall stacks a flame 16 pixels higher, and Flare Storm
				 * another */
				/* 16 above that. */
				if(state->args->variant != EFFECT_VARIANT_BASIC) {
#ifdef UBFIX
					width = BattleEffect_PuffArcPictureWidths[picture];
					draw(
						canvas,
						state->sheet + BattleEffect_PuffArcPictures[picture],
						flame->x - (width >> 1),
						(flame->y + BattleEffect_PuffArcPictureBiasY[picture]) -
							16,
						width, BattleEffect_PuffArcPictureHeights[picture]);
#else
					draw(
						canvas,
						state->sheet + BattleEffect_PuffArcPictures[picture],
						flame->x -
							((width =
								  BattleEffect_PuffArcPictureWidths[picture]) >>
							 1),
						(flame->y + BattleEffect_PuffArcPictureBiasY[picture]) -
							16,
						width, BattleEffect_PuffArcPictureHeights[picture]);
#endif
				}
				if(state->args->variant == EFFECT_VARIANT_STRONGEST) {
#ifdef UBFIX
					width = BattleEffect_PuffArcPictureWidths[picture];
					draw(
						canvas,
						state->sheet + BattleEffect_PuffArcPictures[picture],
						flame->x - (width >> 1),
						(flame->y + BattleEffect_PuffArcPictureBiasY[picture]) -
							32,
						width, BattleEffect_PuffArcPictureHeights[picture]);
#else
					draw(
						canvas,
						state->sheet + BattleEffect_PuffArcPictures[picture],
						flame->x -
							((width =
								  BattleEffect_PuffArcPictureWidths[picture]) >>
							 1),
						(flame->y + BattleEffect_PuffArcPictureBiasY[picture]) -
							32,
						width, BattleEffect_PuffArcPictureHeights[picture]);
#endif
				}
			}
			flame->variant++;
		}

		/* The targets flash red. */
		for(i = 0; i != state->args->tgtCnt; i++) {
			if(frame == (i * 8) + 16) {
				btEffectTintBattler(state->args->tgts[i], SPRITE_COLOR_RED_TINT,
									POSE_DAMAGE, i, 12);
			}
		}
		EFFECT_END_FRAME(state);
	}
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

/* Lists the indices within idxRadius of where curValue sits in values, */
/* up to entryLimit, and returns how many. */
UNUSED s32 btEffectBuildCenteredIndexList(BATTLE_EFFECT_INDEX_WINDOW *window,
										  s16 *output) {
	s32 windowLength = window->idxRadius * 2 + 1;
	s32 outputCount = 0;
	s32 centerIndex = 0;
	s32 windowIndex;

	for(windowIndex = 0; windowIndex != windowLength; windowIndex++) {
		if(window->curValue == window->values[windowIndex]) {
			centerIndex = windowIndex;
			break;
		}
	}
	for(windowIndex = 0; windowIndex != windowLength; windowIndex++) {
		s32 index = centerIndex + windowIndex - window->idxRadius;

		if(index >= 0 && index < window->entryLimit) {
			output[outputCount++] = index;
		}
	}
	return outputCount;
}

#undef FLARE_FRAMES
#undef FLARE_ARC_COUNT
#undef FLARE_ARC_RADIUS_X
#undef FLARE_ARC_RADIUS_Y
#undef FLARE_FLAME_PICTURES
#undef FLARE_PICTURE_FRAMES
