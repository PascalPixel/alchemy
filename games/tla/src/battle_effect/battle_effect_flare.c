/* Flare, and an unused function that lists the indices around a value. Part of
   battle_effect.c. */

#define FLARE_TICKS 60 /* ticks of the flare */

#define FLARE_ARC_COUNT		 9 /* flames on the arc */
/* The half ellipse's radii, across and up and down, in canvas pixels. */
#define FLARE_ARC_RADIUS_X	 32
#define FLARE_ARC_RADIUS_Y	 16
/* A flame's six pictures, four ticks each, before it goes out. */
#define FLARE_FLAME_PICTURES 6
#define FLARE_PICTURE_TICKS	 4

/* Nine flames rise one after another along half an ellipse.

   Abilities: Flare, Flare Wall, Flare Storm. */
void btEffectRunFlare(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frm;
	DRAW_RECTANGLE		 draw;
	Sint32				 i;
	Sint32				 picture;
	Uint32				 width;
	Sint32				 direction;
	Sint32				 originX;
	PARTICLE			*arcFlame;
	PARTICLE			*flame;

	state = RamHeapSlots->battleEff;
	canvas = RamHeapSlots->battleCanvas;
	btEffectStartCanvasLayer(0);
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
		BLDCNT_TGT2_ALL;
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btResourceLoadAndDecompress(
		RESOURCE_EMBER_STREAK_SHEET, state->sheet, 1, 1);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	draw = RamHeapSlots->ramCode;

	/* The arc of flames, from the canvas's left edge over the enemies and back
	   from its right half over the party, each flame starting four ticks after
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
	for(frm = 0; frm != FLARE_TICKS; frm++) {
		if(frm == 24)
			EFFECT_SHOW_DAMAGE(SE_IMPACT);

		/* Each flame plays its pictures and goes out; Flare Wall's and Flare
		   Storm's stack a second and a third above it. */
		for(i = 0; i != FLARE_ARC_COUNT; i++) {
			flame = &state->particles[i];
			if(flame->variant >= 0 &&
				flame->variant < FLARE_FLAME_PICTURES * FLARE_PICTURE_TICKS) {
				picture = flame->variant / FLARE_PICTURE_TICKS;
				/* UB: width is assigned in one argument and read in another. */
#if FIX_UNSEQUENCED_SIDE_EFFECT
				width = BattleEffectEmberStreakPictureWidths[picture];
				draw(canvas,
					state->sheet + BattleEffectEmberStreakPictures[picture],
					flame->x - (width >> 1),
					flame->y + BattleEffectEmberStreakPictureBiasY[picture],
					width, BattleEffectEmberStreakPictureHeights[picture]);
#else
				draw(canvas,
					state->sheet + BattleEffectEmberStreakPictures[picture],
					flame->x -
						((width = BattleEffectEmberStreakPictureWidths
								 [picture]) >>
							1),
					flame->y + BattleEffectEmberStreakPictureBiasY[picture],
					width, BattleEffectEmberStreakPictureHeights[picture]);
#endif
				if(args->variant != EFFECT_VARIANT_BASIC) {
#if FIX_UNSEQUENCED_SIDE_EFFECT
					width = BattleEffectEmberStreakPictureWidths[picture];
					draw(canvas,
						state->sheet + BattleEffectEmberStreakPictures[picture],
						flame->x - (width >> 1),
						(flame->y +
							BattleEffectEmberStreakPictureBiasY[picture]) -
							16,
						width, BattleEffectEmberStreakPictureHeights[picture]);
#else
					draw(canvas,
						state->sheet + BattleEffectEmberStreakPictures[picture],
						flame->x -
							((width = BattleEffectEmberStreakPictureWidths
									 [picture]) >>
								1),
						(flame->y +
							BattleEffectEmberStreakPictureBiasY[picture]) -
							16,
						width, BattleEffectEmberStreakPictureHeights[picture]);
#endif
				}
				if(args->variant == EFFECT_VARIANT_STRONGEST) {
#if FIX_UNSEQUENCED_SIDE_EFFECT
					width = BattleEffectEmberStreakPictureWidths[picture];
					draw(canvas,
						state->sheet + BattleEffectEmberStreakPictures[picture],
						flame->x - (width >> 1),
						(flame->y +
							BattleEffectEmberStreakPictureBiasY[picture]) -
							32,
						width, BattleEffectEmberStreakPictureHeights[picture]);
#else
					draw(canvas,
						state->sheet + BattleEffectEmberStreakPictures[picture],
						flame->x -
							((width = BattleEffectEmberStreakPictureWidths
									 [picture]) >>
								1),
						(flame->y +
							BattleEffectEmberStreakPictureBiasY[picture]) -
							32,
						width, BattleEffectEmberStreakPictureHeights[picture]);
#endif
				}
			}
			flame->variant++;
		}

		/* The targets flash red, eight ticks apart. */
		EFFECT_FOR_EACH_TARGET(i, args) {
			if(frm == (i * 8) + 16)
				btEffectTintBattler(
					args->tgts[i], SPRITE_COLOR_RED_TINT, POSE_DAMAGE, i, 12);
		}
		EFFECT_END_TICK(state);
	}
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#include "battle_effect/effect_build_centered_index_list.c"

#undef FLARE_TICKS
#undef FLARE_ARC_COUNT
#undef FLARE_ARC_RADIUS_X
#undef FLARE_ARC_RADIUS_Y
#undef FLARE_FLAME_PICTURES
#undef FLARE_PICTURE_TICKS
