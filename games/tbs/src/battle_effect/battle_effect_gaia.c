/* The Gaia and Rockfall Psynergy. Part of battle_effect.c. */

#define GAIA_TICKS			 192 /* ticks of Gaia */
/* The shaft of light widens into the wall a row every three ticks from
   GAIA_WALL_WIDEN_TICK, stands once it has risen, and sinks away a row every
   three ticks. */
#define GAIA_WALL_WIDEN_TICK 64
#define GAIA_WALL_STAND_TICK 88
#define GAIA_WALL_SINK_TICK	 160

#define GAIA_ROCKS			   11 /* rock sprites tumbling up through the wall */
/* The rocks that fly from the start; the rest join them from tick 72. */
#define GAIA_EARLY_ROCKS	   6
/* The rocks' motion is state->particles[GAIA_FIRST_ROCK] on. */
#define GAIA_FIRST_ROCK		   10
/* The columns GaiaLateRockColumns gives the rocks after the early ones. */
#define GAIA_LATE_ROCK_COLUMNS 6
/* A rock waits this many ticks longer than the rock after it before it first
   flies. */
#define GAIA_ROCK_STAGGER	   4
#define GAIA_DUST			   256 /* dust in PARTICLES */
/* The dust drawn: the rest is set out and never shown. */
#define GAIA_DUST_SHOWN		   64

/* The pictures Gaia rebuilds from the earth wall sprite sheet: the rising
   wall's rows, the standing wall's top and foot, then the flames. */
#define GAIA_ROW_HEIGHT		  32
/* The standing wall's top, stretched from a 48x32 row, and its foot. */
#define GAIA_WALL_TOP_WIDTH	  48
#define GAIA_WALL_TOP_HEIGHT  96
#define GAIA_WALL_FOOT_WIDTH  48
#define GAIA_WALL_FOOT_HEIGHT 21

/* Where the wall's top and foot begin in the earth wall sprite sheet, after its
   rows, in bytes. */
typedef enum EARTH_WALL_SHEET_PICTURE {
	GAIA_WALL_TOP = GAIA_ROW_HEIGHT * (5 + 6 + 9 + 12 + 15 + 18 + 21 + 32 + 40),
	GAIA_WALL_FOOT = GAIA_WALL_TOP + PICTURE_BYTES(GAIA_WALL_TOP)
} EARTH_WALL_SHEET_PICTURE;

/* Flames flicker at the targets' feet under a narrow shaft of orange light,
   which widens into a glowing wall and stands while rocks tumble up through it
   and the targets are battered, then narrows away again.

   Abilities: Gaia, Mother Gaia, Grand Gaia, Rockfall, Rockslide, Avalanche. */
void btEffectRunGaia(BATTLE_EFFECT_PARM *args)
{
	static SPR_SCALE	 unitScale = {{{toFIXED(1), toFIXED(1)}}};
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	BLITTER_PAIR		 draw;
	Uint8				*sprs;
	Sint32				 wallX;
	Sint32				 rockX;
	SPR_SCALE			 scale;
	SPR_PLACEMENT		 pos;
	Uint8				*src;
	volatile Uint16		*col;
	Sint32				 sheetIdx;
	Sint32				 frm;
	Sint32				 i;
	Sint32				 j;
	Sint32				 k;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	sprs = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	btEffectStartCanvasLayer(0);

	/* The higher the variant, the wider the wall. */
	if(state->args->variant == EFFECT_VARIANT_BASIC)
		REG_BG2PA = Q_8_8(0.8);
	else if(state->args->variant == EFFECT_VARIANT_STRONGER)
		REG_BG2PA = Q_8_8(1.0 / 1.5);
	if(state->args->side == EFFECT_SIDE_PARTY) {
		wallX = 8;
		if(state->args->variant == EFFECT_VARIANT_BASIC)
			rockX = 40;
		else if(state->args->variant == EFFECT_VARIANT_STRONGER)
			rockX = 36;
		else
			rockX = 40;
	} else {
		wallX = -16;
		rockX = -12;
	}
	REG_BG2X = GaiaBg2Shifts[state->args->side][state->args->variant] << 8;

	btResourceLoadAndDecompress(
		RESOURCE_EARTH_WALL_SHEET, Ram_MapGridMetatileBuffer, 1, 0);
	syWordsCopy(BG_PLTT, syResourceGetTableEntry(RESOURCE_GAIA_ORANGE_PALETTE),
		SHEET_PALETTE_BYTES);
	col = &BG_PLTT[1];
	for(i = 0; i != SHEET_PALETTE_COLORS - 1; i++) {
		Sint32 entry = *col;
		Sint32 blue = RGB_BLUE((Uint16)entry);
		Sint32 green = RGB_GREEN((Uint16)entry);
		Sint32 red = RGB_RED(entry);

		blue -= 8;
		green -= 8;
		red -= 8;

		if(blue < 0)
			blue = 0;
		if(green < 0)
			green = 0;
		if(red < 0)
			red = 0;
		*col++ = RGB(red, green, blue);
	}

	src = Ram_MapGridMetatileBuffer;
	sheetIdx = 0;
	for(j = 0; j != 9; j++) {
		for(i = 0; i != GAIA_ROW_HEIGHT; i++) {
			for(k = 0; k != GaiaRowWidths[j]; k++, sheetIdx++)
				state->sheet[sheetIdx] = src[k];
		}
		src += GaiaRowWidths[j];
	}
	for(j = 0; j != GAIA_ROW_HEIGHT; j++) {
		for(i = 0; i != GAIA_WALL_TOP_HEIGHT / GAIA_ROW_HEIGHT; i++) {
			for(k = 0; k != GAIA_WALL_TOP_WIDTH; k++, sheetIdx++)
				state->sheet[sheetIdx] = src[k];
		}
		src += GAIA_WALL_TOP_WIDTH;
	}
	for(i = 0; i != GAIA_WALL_FOOT_WIDTH * GAIA_WALL_FOOT_HEIGHT; i++)
		state->sheet[sheetIdx++] = *src++;
	for(i = 0; i != 3; i++) {
		for(k = 0; k != GaiaFlameWidths[i] * GaiaFlameHeights[i]; k++)
			state->sheet[sheetIdx++] = *src++;
	}
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprs, 0, 0);

	for(i = 0; i != GAIA_ROCKS; i++) {
		SPR *rock = fdSprCreateStub(SPRITE_BATTLE_ROCKS);

		state->sprs[i] = rock;
		if(rock != 0) {
			rock->flg = 0;
			fdSprStartAnimationStub(rock, i / 4);
			state->sprs[i]->image.pri = 1;
		}
	}
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
	draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND |
		BLDCNT_TGT2_ALL;
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	/* Rocks wait their turn (variant counts the ticks) and tumble up through
	   the wall, y counting down from 128 by z a tick. */
	for(i = 0; i != GAIA_ROCKS; i++) {
		PARTICLE *rock = &state->particles[GAIA_FIRST_ROCK + i];

		SET_POSITION(rock, (syRandom() & 15) + 88, 128, (syRandom() & 15) + 2);
		SET_VELOCITY(rock, i, 1, toFIXED(1) / 2);
		rock->variant = GAIA_ROCKS * GAIA_ROCK_STAGGER - i * GAIA_ROCK_STAGGER;
	}
	/* DEAD: the sixth column lands on the particle after the last rock, which
	   nothing reads; the ROM keeps the store. */
	for(i = 0; i != GAIA_LATE_ROCK_COLUMNS; i++)
		state->particles[GAIA_FIRST_ROCK + GAIA_EARLY_ROCKS + i].x =
			GaiaLateRockColumns[i];

	/* Dust rises from the foot of the wall. */
	for(i = 0; i != GAIA_DUST; i++) {
		PARTICLE *dust = &PARTICLES[i];

		dust->x = INTtoFIXED((syRandom() & 63) + wallX + 32);
		dust->y = INTtoFIXED((syRandom() & 7) + 96);
		dust->velY = ((syRandom() & 63) + 32) << 13;
		dust->variant = syRandom() & 31;
	}
	REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(7);
	/* The screen shakes throughout. */
	state->shakeFrm = 250;
	for(frm = 0; frm != GAIA_TICKS; frm++) {
		if(frm == 0)
			sySndPlayStub(SE_POWER_UP);
		if(frm == 40)
			sySndPlayStub(SE_RUMBLE);
		if(frm == 96)
			sySndPlayStub(SE_CRASH);
		if(frm == 120)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);

		/* The flames flicker at the wall's foot. */
		if(frm <= 81) {
			Sint32 picture = frm / 4;

			if(picture > 2)
				picture = (picture & 1) + 1;
			draw[0](canvas, state->sheet + GaiaFlamePictures[picture],
				wallX - GaiaFlameWidths[picture] + 64,
				116 - GaiaFlameHeights[picture], GaiaFlameWidths[picture],
				GaiaFlameHeights[picture]);
			draw[1](canvas, state->sheet + GaiaFlamePictures[picture],
				wallX + 64, 116 - GaiaFlameHeights[picture],
				GaiaFlameWidths[picture], GaiaFlameHeights[picture]);
		}

		/* The rising wall widens a row at a time. */
		if(frm >= 12 && frm < GAIA_WALL_STAND_TICK) {
			Sint32 row = (frm - GAIA_WALL_WIDEN_TICK) / 3;

			CLAMP(row, 0, 7);
			for(i = 0; i != 4; i++) {
				draw[0](canvas, state->sheet + GaiaRowPictures[row],
					wallX - GaiaRowWidths[row] + 64, i * GAIA_ROW_HEIGHT - 12,
					GaiaRowWidths[row], GAIA_ROW_HEIGHT);
				draw[1](canvas, state->sheet + GaiaRowPictures[row], wallX + 64,
					i * GAIA_ROW_HEIGHT - 12, GaiaRowWidths[row],
					GAIA_ROW_HEIGHT);
			}
		}

		/* The wall sinks away. */
		if(frm >= GAIA_WALL_SINK_TICK && frm < GAIA_WALL_SINK_TICK + 24) {
			Sint32 row = 7 - (frm - GAIA_WALL_SINK_TICK) / 3;

			CLAMP(row, 0, 7);
			for(i = 0; i != 4; i++) {
				draw[0](canvas, state->sheet + GaiaRowPictures[row],
					wallX - GaiaRowWidths[row] + 64, i * GAIA_ROW_HEIGHT - 12,
					GaiaRowWidths[row], GAIA_ROW_HEIGHT);
				draw[1](canvas, state->sheet + GaiaRowPictures[row], wallX + 64,
					i * GAIA_ROW_HEIGHT - 12, GaiaRowWidths[row],
					GAIA_ROW_HEIGHT);
			}
		}

		/* The standing wall. */
		if(frm >= GAIA_WALL_STAND_TICK && frm < GAIA_WALL_SINK_TICK) {
			Sint32 width = GAIA_WALL_TOP_WIDTH;

			draw[0](canvas, &state->sheet[GAIA_WALL_TOP], wallX + 16, 0, width,
				GAIA_WALL_TOP_HEIGHT);
			draw[1](canvas, &state->sheet[GAIA_WALL_TOP], wallX + 64, 0, width,
				GAIA_WALL_TOP_HEIGHT);
			draw[0](canvas, &state->sheet[GAIA_WALL_FOOT], wallX + 16,
				GAIA_WALL_TOP_HEIGHT, width, GAIA_WALL_FOOT_HEIGHT);
			draw[1](canvas, &state->sheet[GAIA_WALL_FOOT], wallX + 64,
				GAIA_WALL_TOP_HEIGHT, width, GAIA_WALL_FOOT_HEIGHT);
		}

		/* Dust keeps rising up the wall. */
		if(frm >= GAIA_WALL_STAND_TICK) {
			for(i = 0; i != GAIA_DUST_SHOWN; i++) {
				PARTICLE *dust = &PARTICLES[i];

				if(dust->variant == 0) {
					Sint32 size = (i & 3) + 5;

					EFFECT_DRAW_MOTE(draw[0], canvas, sprs, FIXEDtoINT(dust->x),
						FIXEDtoINT(dust->y), size);
					dust->y -= dust->velY;
					if(dust->y < 0 && frm < GAIA_WALL_SINK_TICK)
						dust->y = toFIXED(96);
				} else {
					dust->variant--;
				}
			}
		}

		/* Rocks tumble up through the wall, larger with the variant. */
		if(frm > 4) {
			Sint32 rocks = GAIA_EARLY_ROCKS;

			if(frm > 71)
				rocks = GAIA_ROCKS;
			for(k = 0; k != rocks; k++) {
				PARTICLE *rock = &state->particles[GAIA_FIRST_ROCK + k];
				Sint32	  slot;

				if(rock->variant == 0) {
					/* DEAD: scale copied from unitScale before both its fields
					   are set; the ROM keeps the copy. */
					scale = unitScale;
					if(frm > 71) {
						scale.x = k * (toFIXED(1) / 16) + toFIXED(1) / 2;
						scale.x += state->args->variant * (toFIXED(1) / 4);
					} else {
						scale.x = toFIXED(1) / 2;
					}
					scale.y = scale.x;
					pos.groundHeight = 0;
					pos.x = toFIXED(rock->x + rockX * 2);
					pos.height = toFIXED(512) - toFIXED(rock->y);
					pos.y = toFIXED(512);
					slot = (frm / 2 + k) % GAIA_ROCKS;
					/* DEAD: slot != -1 always holds, slot being a remainder of
					   a positive count; the ROM keeps the test. */
					if(slot != -1)
						fdRenderApplyProjectedPlacementStub(
							state->sprs[slot], &pos, &scale, 0);
					rock->y -= rock->z;
					/* DEAD: velocityX steps from 1 to 12 by velocityY and
					   nothing reads it; the ROM keeps the count. */
					rock->velX += rock->velY;
					if(rock->velX > 12)
						rock->velX -= 12;
					if(rock->y < 0) {
						if(frm >= GAIA_WALL_SINK_TICK) {
							rock->variant = -1;
						} else {
							if(frm >= GAIA_WALL_STAND_TICK) {
								rock->z = (syRandom() & 7) + 8;
								if(state->args->variant == EFFECT_VARIANT_BASIC)
									rock->x = syRandom() % 96 + 42;
								else if(state->args->variant ==
									EFFECT_VARIANT_STRONGER)
									rock->x = syRandom() % 112 + 34;
								else
									rock->x = syRandom() % 160 + 10;
							}
							rock->y = 128;
							rock->variant = 8;
						}
					}
				} else {
					rock->variant--;
				}
			}
		}

		/* The targets flash and recoil. */
		if(frm <= 158) {
			EFFECT_FOR_EACH_TARGET(i, state->args) {
				if(frm > 85) {
					if(frm % 12 == 0)
						btEffectTintBattler(state->args->tgts[i],
							SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 6);
					if((frm & 3) == 0)
						btMotionApplyKnockbackStub(
							state->args->tgts[i], KNOCKBACK_SKID);
				}
			}
		}
		if(frm < 90 || frm > 160)
			btCameraShake(2, 2);
		else
			btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}
	syCloseEvent(btEffectTransferCanvas);
	EFFECT_RELEASE_BLITTERS();
	for(i = 0; i != GAIA_ROCKS; i++)
		fdSprDestroyStub(state->sprs[i]);
	btEffectEndCanvasLayer();
}

#undef GAIA_TICKS
#undef GAIA_WALL_WIDEN_TICK
#undef GAIA_WALL_STAND_TICK
#undef GAIA_WALL_SINK_TICK
#undef GAIA_ROCKS
#undef GAIA_EARLY_ROCKS
#undef GAIA_FIRST_ROCK
#undef GAIA_LATE_ROCK_COLUMNS
#undef GAIA_ROCK_STAGGER
#undef GAIA_DUST
#undef GAIA_DUST_SHOWN
#undef GAIA_ROW_HEIGHT
#undef GAIA_WALL_TOP_WIDTH
#undef GAIA_WALL_TOP_HEIGHT
#undef GAIA_WALL_FOOT_WIDTH
#undef GAIA_WALL_FOOT_HEIGHT
