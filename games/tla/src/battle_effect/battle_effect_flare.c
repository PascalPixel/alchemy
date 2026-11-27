/* Flare, and an unused function that lists the indices around a value. Part of
   battle_effect.c. */

#define FLARE_FRAMES 60 /* frames of the flare */

#define FLARE_ARC_COUNT		 9 /* flames on the arc */
/* The half ellipse's radii, across and up and down, in canvas pixels. */
#define FLARE_ARC_RADIUS_X	 32
#define FLARE_ARC_RADIUS_Y	 16
/* A flame's six pictures, four frames each, before it goes out. */
#define FLARE_FLAME_PICTURES 6
#define FLARE_PICTURE_FRAMES 4

/* Nine flames rise one after another along half an ellipse.

   Abilities: Flare, Flare Wall, Flare Storm. */
void btEffectRunFlare(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	DRAW_RECTANGLE		 draw;
	Sint32				 i;
	Sint32				 picture;
	Uint32				 width;
	Sint32				 direction;
	Sint32				 originX;
	PARTICLE			*arcFlame;
	PARTICLE			*flame;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	btEffectStartCanvasLayer(0);
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
		BLDCNT_TGT2_ALL;
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btResourceLoadAndDecompress(
		RESOURCE_EMBER_STREAK_SHEET, state->sheet, 1, 1);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	draw = Ram_HeapSlots->ramCode;

	/* The arc of flames, from the canvas's left edge over the enemies and back
	   from its right half over the party, each flame starting four frames after
	   the one before. */
	if(args->tgts[0] >= FIRST_ENEMY_UNIT) {
		originX = 0;
		direction = 1;
	} else {
		originX = CANVAS_SIZE / 2;
		direction = -1;
	}
	for(i = 0; i != FLARE_ARC_COUNT; i++) {
		Sint32 angle = i * (TRIG_TURN / 16) - DEGtoANG(90);

		arcFlame = &state->particles[i];
		arcFlame->x =
			((direction * FIXEDtoINT(sySin(angle) * FLARE_ARC_RADIUS_X)) +
				originX) +
			20;
		arcFlame->y = FIXEDtoINT(syCos(angle) * FLARE_ARC_RADIUS_Y) + 40;
		arcFlame->variant = -i * 4;
	}

	state->transferMode = CANVAS_COPY_AND_FADE;
	if(args->variant == EFFECT_VARIANT_STRONGEST)
		state->transferValue = CANVAS_FADE_THREE_QUARTERS;
	else
		state->transferValue = CANVAS_FADE_HALF;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	sySndPlayStub(SE_WHOOSH);
	for(frame = 0; frame != FLARE_FRAMES; frame++) {
		if(frame == 24)
			EFFECT_SHOW_DAMAGE(SE_IMPACT);

		/* Each flame plays its pictures and goes out; Flare Wall's and Flare
		   Storm's stack a second and a third above it. */
		for(i = 0; i != FLARE_ARC_COUNT; i++) {
			flame = &state->particles[i];
			if(flame->variant >= 0 &&
				flame->variant < FLARE_FLAME_PICTURES * FLARE_PICTURE_FRAMES) {
				picture = flame->variant / FLARE_PICTURE_FRAMES;
				/* UB: width is assigned in one argument and read in another. */
#if FIX_UNSEQUENCED_SIDE_EFFECT
				width = BattleEffect_EmberStreakPictureWidths[picture];
				draw(canvas,
					state->sheet + BattleEffect_EmberStreakPictures[picture],
					flame->x - (width >> 1),
					flame->y + BattleEffect_EmberStreakPictureBiasY[picture],
					width, BattleEffect_EmberStreakPictureHeights[picture]);
#else
				draw(canvas,
					state->sheet + BattleEffect_EmberStreakPictures[picture],
					flame->x -
						((width = BattleEffect_EmberStreakPictureWidths
								 [picture]) >>
							1),
					flame->y + BattleEffect_EmberStreakPictureBiasY[picture],
					width, BattleEffect_EmberStreakPictureHeights[picture]);
#endif
				if(args->variant != EFFECT_VARIANT_BASIC) {
#if FIX_UNSEQUENCED_SIDE_EFFECT
					width = BattleEffect_EmberStreakPictureWidths[picture];
					draw(canvas,
						state->sheet +
							BattleEffect_EmberStreakPictures[picture],
						flame->x - (width >> 1),
						(flame->y +
							BattleEffect_EmberStreakPictureBiasY[picture]) -
							16,
						width, BattleEffect_EmberStreakPictureHeights[picture]);
#else
					draw(canvas,
						state->sheet +
							BattleEffect_EmberStreakPictures[picture],
						flame->x -
							((width = BattleEffect_EmberStreakPictureWidths
									 [picture]) >>
								1),
						(flame->y +
							BattleEffect_EmberStreakPictureBiasY[picture]) -
							16,
						width, BattleEffect_EmberStreakPictureHeights[picture]);
#endif
				}
				if(args->variant == EFFECT_VARIANT_STRONGEST) {
#if FIX_UNSEQUENCED_SIDE_EFFECT
					width = BattleEffect_EmberStreakPictureWidths[picture];
					draw(canvas,
						state->sheet +
							BattleEffect_EmberStreakPictures[picture],
						flame->x - (width >> 1),
						(flame->y +
							BattleEffect_EmberStreakPictureBiasY[picture]) -
							32,
						width, BattleEffect_EmberStreakPictureHeights[picture]);
#else
					draw(canvas,
						state->sheet +
							BattleEffect_EmberStreakPictures[picture],
						flame->x -
							((width = BattleEffect_EmberStreakPictureWidths
									 [picture]) >>
								1),
						(flame->y +
							BattleEffect_EmberStreakPictureBiasY[picture]) -
							32,
						width, BattleEffect_EmberStreakPictureHeights[picture]);
#endif
				}
			}
			flame->variant++;
		}

		/* The targets flash red, eight frames apart. */
		EFFECT_FOR_EACH_TARGET(i, args) {
			if(frame == (i * 8) + 16)
				btEffectTintBattler(
					args->tgts[i], SPRITE_COLOR_RED_TINT, POSE_DAMAGE, i, 12);
		}
		EFFECT_END_FRAME(state);
	}
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

/* Lists the indices within idxRadius of where curValue sits in values, up to
   entryLimit, and returns how many. */
UNUSED Sint32 btEffectBuildCenteredIndexList(
	BATTLE_EFFECT_INDEX_WINDOW *window, Sint16 *output)
{
	Sint32 windowLength = window->idxRadius * 2 + 1;
	Sint32 outputCount = 0;
	Sint32 centerIndex = 0;
	Sint32 windowIndex;

	for(windowIndex = 0; windowIndex != windowLength; windowIndex++) {
		if(window->curValue == window->values[windowIndex]) {
			centerIndex = windowIndex;
			break;
		}
	}
	for(windowIndex = 0; windowIndex != windowLength; windowIndex++) {
		Sint32 index = centerIndex + windowIndex - window->idxRadius;

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
