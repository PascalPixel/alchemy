/* Rime and Haze, whose Djinni rises while its sign turns over the target, then
   the entry points of the abilities btEffectRunGaspFamily plays. Part of
   battle_effect.c. */

/* The Djinni's picture, after the sign's pictures in the state's sprite
   sheet. */
#define DJINNI_SIGN_DJINNI_PICTURE 26048
/* How far the Djinni sways either side as it rises, in pixels. */
#define DJINNI_SIGN_DJINNI_SWAY	   32
#define DJINNI_SIGN_TICKS		   48

/* Rime's seal: the 64x64 texture after RuneSheet's half, which the rune's half
   and its mirror fill (RUNE_TEXTURE_RUNE); the stretch of the sprite sheet
   cleared for it is four times the texture. */
#define RIME_SEAL_TEXTURE		PICTURE_BYTES(RUNE_HALF)
#define RIME_SEAL_CLEARED_BYTES 16384

/* Haze's stars: thirty-two placed, of which the first sixteen bloom, each over
   sixteen ticks. */
#define HAZE_PLACED_STARS 32
#define HAZE_STARS		  16
#define HAZE_STAR_TICKS	  16

/* Rime and Haze: the Djinni rises swaying out of sight over the targets while
   its sign turns in the air: for Rime a seal that spins in two mirrored copies
   and shrinks onto the target, for Haze sixteen stars that bloom and fade one
   after another. */
void btEffectRunDjinniSign(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	Sint32				 center[3];
	Sint32				 base[3];
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	/* LEFTOVER(frame): blit[1], the pair's idle second blitter, which the ROM's
	 * tick reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE blit[2];
	void		  *vertices;
	MODEL_TEXTURE  texture;
	RENDER_MODEL  *model;
	Sint32		   frame;
	Sint32		   i;

	state = RamHeapSlots->battleEffect;
	canvas = RamHeapSlots->battleCanvas;
	btEffectStartCanvasLayer(0);
	if(mode == DJINNI_SIGN_RIME) {
		Sint32 row;
		Sint32 column;

		btEffectRunDjinniEntrance(args,
			DJINNI_ENTRANCE(
				ELEMENT_MERCURY, args->side ^ 1, DJINNI_ANCHOR_OVER_TARGET),
			center, base);
		btResourceLoadAndDecompress(RESOURCE_MERCURY_DJINN_SHEET,
			state->sheet + DJINNI_SIGN_DJINNI_PICTURE, 1, 1);
		btResourceLoadAndDecompress(RESOURCE_RUNE_SHEET, state->sheet, 1, 0);

		/* The rune's half and its mirror make the seal. */
		syWordsClearEntry(
			state->sheet + RIME_SEAL_TEXTURE, RIME_SEAL_CLEARED_BYTES);
		for(row = 0; row != RUNE_HALF_HEIGHT; row++) {
			for(column = 0; column != RUNE_HALF_WIDTH; column++) {
				state->sheet[RIME_SEAL_TEXTURE + RUNE_TEXTURE_RUNE +
					row * RUNE_TEXTURE_SIZE + column] =
					state->sheet[row * RUNE_HALF_WIDTH + column];
				state->sheet[RIME_SEAL_TEXTURE + RUNE_TEXTURE_RUNE +
					row * RUNE_TEXTURE_SIZE + 2 * RUNE_HALF_WIDTH - 1 -
					column] = state->sheet[row * RUNE_HALF_WIDTH + column];
			}
		}
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	} else {
		Sint32 haze[3];

		btEffectRunDjinniEntrance(args,
			DJINNI_ENTRANCE(
				ELEMENT_JUPITER, args->side ^ 1, DJINNI_ANCHOR_OVER_TARGET),
			center, haze);
		btResourceLoadAndDecompress(RESOURCE_JUPITER_DJINN_SHEET,
			state->sheet + DJINNI_SIGN_DJINNI_PICTURE, 1, 1);
		btResourceLoadAndDecompress(
			RESOURCE_STAR_CROSS_SHEET, state->sheet, 1, 0);
		EFFECT_SET_TRANSFER(
			state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	}
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	REG_BG2PA = Q_8_8(1);
	REG_BG2X = (CANVAS_SIZE / 2 - center[0]) << 8;
	if(args->side == EFFECT_SIDE_ENEMIES)
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
	else
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	blit[0] = RamHeapSlots->ramCode;

	for(frame = 0; frame != DJINNI_SIGN_TICKS; frame++) {
		/* Tick 0: Haze's stars take random places about the centre, one more
		   blooming every two ticks. */
		if(frame == 0) {
			for(i = 0; i != HAZE_PLACED_STARS; i++) {
				PARTICLE *star = &state->particles[i];

				SET_POSITION(star, toFIXED((syRandom() & 31) - 16),
					toFIXED((syRandom() & 63) - 16), (syRandom() & 127) + 128);
				star->velocityY = syRandom();
				star->variant = -i * 2;
			}
		}
		if(frame == 38)
			EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
		if(mode == DJINNI_SIGN_RIME) {
			if(frame == 32)
				sySoundPlayStub(SE_POWER_UP);
		} else {
			if(frame > 7)
				btEffectStepPaletteToResource(RESOURCE_STAR_CROSS_SHEET);
			if(frame == 32)
				btEffectTintBattler(
					args->targets[0], SPRITE_COLOR_WHITE, TINT_KEEP, 0, 24);
		}

		/* The Djinni rises a pixel a tick, swaying, until it leaves the
		   top. */
		if(--center[1] > 0) {
			Sint32 height = center[1];

			if(height > DJINNI_HEIGHT - 1)
				height = DJINNI_HEIGHT;
			blit[0](canvas, state->sheet + DJINNI_SIGN_DJINNI_PICTURE,
				FIXEDtoINT(
					sySin(frame * (TRIG_TURN / 64)) * DJINNI_SIGN_DJINNI_SWAY) +
					CANVAS_SIZE / 2 - DJINNI_WIDTH / 2,
				center[1] - DJINNI_HEIGHT / 2, DJINNI_WIDTH, height);
		}

		syInitMatrix();
		ProjectionWork.centerY = 104;
		vertices = syHeapAllocateEWRAM(32);
		model = btRenderAllocateModelList(1);
		/* 64x64: Rime's seal, or Haze's star. */
		texture.logWidth = 6;
		texture.logHeight = 6;
		model->kind = MODEL_SHADED_MAX;
		model->texture = &texture;
		model->mesh = BattleEffectSquareMesh;
		model->vertices = vertices;
		if(mode == DJINNI_SIGN_RIME) {
			/* Rime's seal fades in over the first eight ticks and turns in two
			   mirrored copies that come together at tick 32, shrinking from
			   twice its size. */
			Sint32 brightness = frame * 8 - 64;

			if(brightness > 0)
				brightness = 0;
			/* DEAD: the effect ends after tick 47, so this fade-out never
			   runs; the ROM keeps its test and store. */
			if(frame > 55)
				brightness = (56 - frame) * 8;
			model->shade = brightness;
			texture.pixels = state->sheet + RIME_SEAL_TEXTURE;
			for(i = 0; i != 2; i++) {
				FIXED scale = toFIXED(2) - frame * (toFIXED(1) / 32);

				if(frame > 31 && i == 1)
					continue;
				if(scale < toFIXED(1))
					scale = toFIXED(1);
				syInitMatrix();
				syRotX(-DEGtoANG(90));
				if(frame <= 31) {
					if(i == 0)
						syRotY((32 - frame) << 9);
					else
						syRotY((frame << 9) - DEGtoANG(90));
				}
				syScale(scale >> 1, scale >> 1, scale >> 1);
				btRenderTransformVertices(
					BattleEffectGroundSquareCorners, vertices, 4);
				btRenderDrawModelList(model);
			}
		} else {
			/* Each of Haze's stars swells and shrinks over 16 ticks, and every
			   fourth one chimes as it opens. */
			PARTICLE *star;

			model->shade = 0;
			texture.pixels = state->sheet;
			for(i = 0, star = state->particles; i != HAZE_STARS; i++, star++) {
				if(star->variant == 4 && (i & 3) == 0)
					sySoundPlayStub(SE_PSYNERGY_START);
				if(star->variant >= 0 && star->variant < HAZE_STAR_TICKS) {
					FIXED swell =
						sySin(star->variant * (TRIG_TURN / 32)) * star->z;
					Sint32 size;

					syInitMatrix();
					syTranslate(star->x, star->y, 0);
					size = swell >> 8;
					syRotX(DEGtoANG(90));
					syRotY(frame << 9);
					size /= 2;
					syScale(size, size, size);
					btRenderTransformVertices(
						BattleEffectGroundSquareCorners, vertices, 4);
					btRenderDrawModelList(model);
				}
				star->variant++;
			}
		}
		syHeapRelease(model);
		syHeapRelease(vertices);
		ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
		ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;

		EFFECT_END_TICK(state);
	}

	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

/* Abilities: Gasp. */
void btEffectRunGasp(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGaspFamily(args, GASP_MODE_GASP);
}

/* Abilities: Reflux. */
void btEffectRunReflux(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGaspFamily(args, GASP_MODE_REFLUX);
}

/* Abilities: Djinn Counter. */
void btEffectRunDjinnCounter(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGaspFamily(args, GASP_MODE_DJINN_COUNTER);
}

/* Abilities: Counter-rush. */
void btEffectRunCounterRush(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGaspFamily(args, GASP_MODE_COUNTER_RUSH);
}

/* Abilities: Mold. */
void btEffectRunMold(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGaspFamily(args, GASP_MODE_MOLD);
}

/* Abilities: Meld. */
void btEffectRunMeld(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGaspFamily(args, GASP_MODE_MOLD);
}

/* Abilities: Shine. */
void btEffectRunShine(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGaspFamily(args, GASP_MODE_SHINE);
}

/* Abilities: Lull. */
void btEffectRunLull(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGaspFamily(args, GASP_MODE_LULL);
}

/* Abilities: Demon Eye. */
void btEffectRunDemonEye(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGaspFamily(args, GASP_MODE_DEMON_EYE);
}

/* Abilities: Emu, Call Demon, Claw Slash, Mega Slash, Vital Moon. */
void btEffectRunEmu(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGaspFamily(args, GASP_MODE_EMU);
}

/* Abilities: Call Dullahan, Formina Sage. */
void btEffectRunCallDullahan(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGaspFamily(args, GASP_MODE_CALL_DULLAHAN);
}

/* Abilities: Beat Dance. */
void btEffectRunBeatDance(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGaspFamily(args, GASP_MODE_BEAT_DANCE);
}

/* Abilities: Clarion Cry. */
void btEffectRunClarionCry(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGaspFamily(args, GASP_MODE_CLARION_CRY);
}

/* Abilities: Element Swap. */
void btEffectRunElementSwap(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGaspFamily(args, GASP_MODE_ELEMENT_SWAP);
}

/* Abilities: Psy Boost. */
void btEffectRunPsyBoost(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGaspFamily(args, GASP_MODE_PSY_BOOST);
}

/* Abilities: Cage. */
void btEffectRunCage(BATTLE_EFFECT_PARM *args)
{
	btEffectRunGaspFamily(args, GASP_MODE_CAGE);
}

#undef DJINNI_SIGN_DJINNI_PICTURE
#undef DJINNI_SIGN_DJINNI_SWAY
#undef RIME_SEAL_TEXTURE
#undef RIME_SEAL_CLEARED_BYTES
#undef HAZE_PLACED_STARS
#undef HAZE_STARS
#undef HAZE_STAR_TICKS
