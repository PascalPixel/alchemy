/* Ice and Douse: shards and drops of water falling on the targets' side. Part
   of battle_effect.c. */

/* Fills window 0's per-line edges: from line 8 to line last, slant, the band
   the shards and drops fall down, which moves along as the lines go down until
   the line before it reaches span, the targets' side; span on the other lines
   to the battle view's bottom; nothing below it. */
#define FALLING_BUILD_SLANTED_WINDOW(line, i, last, slant, span) \
	for((i) = 0; (i) != DISPLAY_HEIGHT; (i)++) { \
		if((i) >= 8 && (i) <= (last)) \
			(line)[i] = (slant); \
		else if((i) < BATTLE_VIEW_BOTTOM) \
			(line)[i] = (span); \
		else \
			(line)[i] = WINDOW_SPAN_INVERTED; \
	}
/* The band from left, as wide as the canvas. */
#define FALLING_BAND(left) WIN_RANGE(left, (left) + CANVAS_SIZE)

/* The falling shards (Ice) and drops of water (Douse) rain diagonally onto the
   targets' side, seen only through a slanted band of window 0 whose edges an
   HBlank DMA feeds line by line; each lands just above line 80, shards
   shattering into sparkle and drops bursting in a splash that throws up
   droplets. */

/* The shards or drops that fall, and the slots for the sparkle or splash they
   burst into, which follow them in PARTICLES. */
enum { FALLING_SHARD_COUNT = 32, FALLING_SHARD_PUFF_COUNT = 32 };

/* A falling shard or drop keeps falling while its y, in eighths of a pixel, is
   at most this. */
#define FALLING_LAND_Y			   (80 * 8 - 1)
/* IceShardSheet: the shard, then its shatter, each picture 32x32. */
#define ICE_SHARD_WIDTH			   32
#define ICE_SHARD_HEIGHT		   32
/* A landed shard or drop counts its frames since landing up to one past
   this. */
#define FALLING_LANDED_LAST_FRAME  14
/* A landed shard shows its shatter's pictures from its first frame to
   FALLING_LANDED_LAST_FRAME, ICE_SHATTER_PICTURE_FRAMES each, and kicks up a
   puff on frames ICE_PUFF_FIRST_FRAME to ICE_PUFF_LAST_FRAME. */
#define ICE_SHATTER_PICTURE_FRAMES 3
#define ICE_PUFF_FIRST_FRAME	   9
#define ICE_PUFF_LAST_FRAME		   11
/* A puff's frames: the nine sparkle pictures, two frames each. */
#define ICE_PUFF_FRAMES			   18

/* Ice shards rain down diagonally onto the targets' side of the field and
   shatter into puffs of sparkle, flashing and shaking each target as they land.

   Abilities: Ice, Ice Horn, Ice Missile. */
void btEffectRunIce(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	Uint8				*sparkles;
	BLITTER_PAIR		 draw;
	PARTICLE			*shard;
	PARTICLE			*puff;
	Uint16				*line;
	Sint32				 i;
	Sint32				 k;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	sparkles = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	EFFECT_START_NARROW_CANVAS(BGCNT_WRAP | BGCNT_PRIORITY(1));
	btResourceLoadAndDecompress(RESOURCE_ICE_SHARD_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_SPARKLE_DOTS, sparkles, 0, 0);
	btEffectConfigureEffectDisp();
	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
		WININ_WIN1_OBJ | WININ_WIN1_CLR;

	/* Line the shards up above the screen, each 8 pixels higher and further out
	   than the one before, with a little scatter. */
	{
		Sint32 y;
		Sint32 slant;

		for(i = 0, y = -128, slant = -16; i != FALLING_SHARD_COUNT;
			y -= 64, i++, slant -= 8) {
			Sint32 x;

			shard = &state->particles[i];
			x = (syRandom() & 63) + (syRandom() & 7) + 24;
			if(state->args->side == EFFECT_SIDE_PARTY)
				x = x + slant + 24;
			else
				x = x - slant + 80;
			shard->y = y;
			shard->x = x << 3;
			shard->variant = -1; /* still falling */
		}
	}
	/* puff slot free */
	for(i = 0; i != FALLING_SHARD_PUFF_COUNT; i++)
		state->particles[FALLING_SHARD_COUNT + i].variant = -1;

	/* Falling shards draw at their maximum; the puffs add their light. Both
	   mirror for the other side. */
	if(state->args->side == EFFECT_SIDE_ENEMIES) {
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, BLITTER_CLIP_Y, COMPOSITE_LIGHTEN);
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE_PAIRED, BLITTER_CLIP_Y, COMPOSITE_ADDITIVE);
	} else {
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
			BLITTER_CLIP_Y | COMPOSITE_FLIP_X, COMPOSITE_LIGHTEN);
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
			BLITTER_CLIP_Y | COMPOSITE_FLIP_X, COMPOSITE_ADDITIVE);
	}
	draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	draw[1] = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED];

	if(state->args->side == EFFECT_SIDE_ENEMIES) {
		line = (Uint16 *)Ram_MapGridMetatileBuffer;
		FALLING_BUILD_SLANTED_WINDOW(
			line, i, 103, FALLING_BAND(112 - i), FALLING_BAND(8));
	} else {
		line = (Uint16 *)Ram_MapGridMetatileBuffer;
		FALLING_BUILD_SLANTED_WINDOW(
			line, i, 95, FALLING_BAND(i + 24), FALLING_BAND(120));
	}

	sySetEvent(btEffectArmWin0HBlankDMA, EVENT_PASS_VBLANK);
	state->transferMode = CANVAS_COPY_AND_FADE;
	if(state->args->variant == EFFECT_VARIANT_STRONGER)
		state->transferValue = CANVAS_FADE_THREE_QUARTERS;
	else
		state->transferValue = CANVAS_FADE_HALF;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);

	for(frame = 0; frame != Ice_Counts[state->args->variant][ICE_FRAMES];
		frame++) {
		if(frame == Ice_Counts[state->args->variant][ICE_FRAMES] - 16)
			EFFECT_SHOW_DAMAGE(SE_WATER);

		/* Shards fall down the slant, and on landing shatter, kicking up
		   puffs. */
		for(i = 0; i != Ice_Counts[state->args->variant][ICE_SHARD_COUNT];
			i++) {
			shard = &state->particles[i];
			if(shard->variant == -1) {
				Sint32 x = shard->x / 8;
				Sint32 y = shard->y / 8;

				draw[state->args->variant == EFFECT_VARIANT_STRONGEST ? 1 : 0](
					canvas, state->sheet, x, y, ICE_SHARD_WIDTH,
					ICE_SHARD_HEIGHT);
				if(shard->y <= FALLING_LAND_Y) { /* above y 80 */
					if(state->args->side == EFFECT_SIDE_ENEMIES)
						shard->x -= 64;
					else
						shard->x += 64;
					shard->y += 64;
				} else {
					/* Every target flashes blue as the shards land. */
					if((i & 3) == 0)
						sySndPlayStub(SE_ICE);
					state->shakeFrm = 2;
					shard->variant = 0;
					EFFECT_FOR_EACH_TARGET(k, state->args)
						btEffectTintBattler(state->args->tgts[k],
							SPRITE_COLOR_BLUE_TINT, POSE_DAMAGE, k, 8);
				}
			}
			if(shard->variant != -1) {
				Sint32 x = shard->x / 8;
				Sint32 y = shard->y / 8;

				if(shard->variant >= 1 &&
					shard->variant <= FALLING_LANDED_LAST_FRAME) {
					draw[state->args->variant == EFFECT_VARIANT_STRONGEST ? 1
																		  : 0](
						canvas,
						state->sheet + PICTURE_BYTES(ICE_SHARD) +
							shard->variant / ICE_SHATTER_PICTURE_FRAMES *
								PICTURE_BYTES(ICE_SHARD),
						x, y, ICE_SHARD_WIDTH, ICE_SHARD_HEIGHT);
				}
				if(shard->variant >= ICE_PUFF_FIRST_FRAME &&
					shard->variant <= ICE_PUFF_LAST_FRAME) {
					for(k = 0; k != FALLING_SHARD_PUFF_COUNT; k++) {
						puff = &state->particles[FALLING_SHARD_COUNT + k];
						if(puff->variant == -1) {
							puff->variant = ICE_PUFF_FRAMES;
							puff->x =
								(((syRandom() & 31) + shard->x / 8) << 3) + 8;
							puff->y = ((syRandom() & 15) + shard->y / 8 - 15)
								<< 3;
							break;
						}
					}
				}
				if(shard->variant <= FALLING_LANDED_LAST_FRAME)
					shard->variant++;
			}
		}

		/* Puffs twinkle out. */
		for(i = 0; i != FALLING_SHARD_PUFF_COUNT; i++) {
			puff = &state->particles[FALLING_SHARD_COUNT + i];
			if(puff->variant != -1) {
				if(puff->variant < ICE_PUFF_FRAMES) {
					Sint32 image = puff->variant / 2;
					Sint32 x =
						puff->x / 8 - (BattleEffect_SparkleSizes[image] >> 1);
					Sint32 y =
						puff->y / 8 - (BattleEffect_SparkleSizes[image] >> 1);

					draw[state->args->variant == EFFECT_VARIANT_STRONGEST ? 1
																		  : 0](
						canvas, sparkles + BattleEffect_SparklePictures[image],
						x, y, BattleEffect_SparkleSizes[image],
						BattleEffect_SparkleSizes[image]);
				}
				if(puff->variant > -1)
					puff->variant--;
			}
		}
		btEffectCountDownBattlerTints();
		btCameraShake(4, 4);
		EFFECT_PRESENT_CANVAS(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	syCloseEvent(btEffectArmWin0HBlankDMA);
	EFFECT_RELEASE_BLITTERS();
	btEffectEndCanvasLayer();
	btEffectConfigureEffectDisp();
}

/* Ram_MapGridMetatileBuffer as Douse uses it: window 0's table, then the drops'
   droplets. */
typedef struct DROP_SPLASH_BUFFER {
	/* WIN0H for each line, read by the HBlank DMA */
	Uint16	 lines[DISPLAY_HEIGHT];
	PARTICLE droplets[512]; /* free while -1 */
} DROP_SPLASH_BUFFER;

/* Ram_MapGridMetatileBuffer seen as Douse's DROP_SPLASH_BUFFER. */
#define DOUSE_SPLASH_BUFFER ((DROP_SPLASH_BUFFER *)Ram_MapGridMetatileBuffer)

/* BlueBeamSheet, after BlueGlowSheet's ten flare pictures (as many bytes as two
   sets of motes): the falling drop, a blue bubble; the white flash it lands in,
   as large as the drop; then the white splash that spreads. */
#define DOUSE_DROP_WIDTH		   24
#define DOUSE_DROP_HEIGHT		   24
#define DOUSE_BURST_PICTURE_WIDTH  42
#define DOUSE_BURST_PICTURE_HEIGHT 42
#define DOUSE_FALLING_PICTURE	   (2 * MOTE_SET_BYTES)
#define DOUSE_FLASH_PICTURE		   (DOUSE_FALLING_PICTURE + PICTURE_BYTES(DOUSE_DROP))
#define DOUSE_BURST_PICTURE		   (DOUSE_FLASH_PICTURE + PICTURE_BYTES(DOUSE_DROP))

/* Drops of water fall diagonally onto the targets' side of the field, 8 pixels
   down and 4 across a frame, each bursting in a splash that throws up droplets
   where it lands, while the targets flash; Deluge throws them higher.

   Abilities: Douse, Drench, Deluge. */
void btEffectRunDouse(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 frame;
	DRAW_RECTANGLE		 drawDroplet;
	DRAW_RECTANGLE		 drawDrop;
	PARTICLE			*drop;
	PARTICLE			*droplet;
	Uint16				*line;
	Sint32				 i;
	Sint32				 k;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	EFFECT_START_NARROW_CANVAS(BGCNT_WRAP | BGCNT_PRIORITY(1));
	btResourceLoadAndDecompress(
		RESOURCE_BLUE_BEAM_SHEET, state->sheet + DOUSE_FALLING_PICTURE, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET, state->sheet, 0, 0);
	btEffectConfigureEffectDisp();
	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG1 |
		WININ_WIN1_OBJ | WININ_WIN1_CLR;

	/* Drops draw at their maximum; droplets add their light. */
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, BLITTER_CLIP_Y, COMPOSITE_LIGHTEN);
	drawDrop = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, BLITTER_CLIP_Y, COMPOSITE_ADDITIVE);
	drawDroplet = HEAP_SLOT_BLOCK(HEAP_SLOT_RAM_CODE_PAIRED);

	EFFECT_FREE_PARTICLES(i, DOUSE_SPLASH_BUFFER->droplets,
		ARRAY_COUNT(DOUSE_SPLASH_BUFFER->droplets));

	/* Line the drops up above the screen along the slant, each the variant's
	   spacing higher than the one before. */
	for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
		Sint32 x;
		Sint32 y;

		drop = &state->particles[i];
		x = syRandom() & 63;
		y = -(i * Douse_Counts[state->args->variant][DOUSE_DROP_SPACING] + 16);
		if(state->args->side == EFFECT_SIDE_PARTY)
			x = x + y / 2 - 48;
		else
			x = x - y / 2 + 72;
		drop->x = x << 3;
		drop->y = y << 3;
		drop->variant = -1; /* still falling */
	}

	if(state->args->side == EFFECT_SIDE_ENEMIES) {
		line = (Uint16 *)Ram_MapGridMetatileBuffer;
		FALLING_BUILD_SLANTED_WINDOW(
			line, i, 103, FALLING_BAND(52 - i / 2), FALLING_BAND(0));
	} else {
		line = (Uint16 *)Ram_MapGridMetatileBuffer;
		FALLING_BUILD_SLANTED_WINDOW(
			line, i, 103, FALLING_BAND(i / 2 + 60), FALLING_BAND(112));
	}

	/* Douse copies the canvas plainly; Drench and Deluge blend it in. */
	sySetEvent(btEffectArmWin0HBlankDMA, EVENT_PASS_VBLANK);
	if(state->args->variant == EFFECT_VARIANT_BASIC) {
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	} else if(state->args->variant == EFFECT_VARIANT_STRONGER) {
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	} else {
		EFFECT_SET_TRANSFER(
			state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	}
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);

	for(frame = 0; frame != Douse_Counts[state->args->variant][DOUSE_FRAMES];
		frame++) {
		if(frame == Douse_Counts[state->args->variant][DOUSE_FRAMES] - 64)
			EFFECT_SHOW_DAMAGE(SE_WATER);

		/* Drops fall down the slant, then splash. */
		for(i = 0; i != Douse_Counts[state->args->variant][DOUSE_DROP_COUNT];
			i++) {
			Sint32 x;
			Sint32 y;

			drop = &state->particles[i];
			x = drop->x / 8;
			y = drop->y / 8;
			if(drop->variant == -1) {
				drawDrop(canvas, state->sheet + DOUSE_FALLING_PICTURE, x, y,
					DOUSE_DROP_WIDTH, DOUSE_DROP_HEIGHT);
				if(drop->y <= FALLING_LAND_Y) {
					if(state->args->side == EFFECT_SIDE_ENEMIES)
						drop->x -= 32;
					else
						drop->x += 32;
					drop->y += 64;
				} else {
					/* Droplets fly up from the middle of the landed drop. */
					drop->variant = 0;
					for(k = 0; k !=
						Douse_Counts[state->args->variant]
									[DOUSE_DROPLETS_PER_DROP];
						k++) {
						droplet = &DOUSE_SPLASH_BUFFER->droplets[i *
								Douse_Counts[state->args->variant]
											[DOUSE_DROPLETS_PER_DROP] +
							k];
						droplet->x = INTtoFIXED(x + DOUSE_DROP_WIDTH / 2);
						droplet->y = toFIXED(y);
						droplet->velX = ((syRandom() & 255) - 128) << 9;
						if(state->args->variant == EFFECT_VARIANT_STRONGEST)
							droplet->velY = ((syRandom() & 0x1ff) - 384) << 10;
						else
							droplet->velY = ((syRandom() & 255) - 255) << 10;
						droplet->variant = (syRandom() & 15) + 16;
					}
					/* Every target flashes white as the drops land. */
					if((i & 3) == 0)
						sySndPlayStub(SE_WATER);
					EFFECT_FOR_EACH_TARGET(k, state->args)
						btEffectTintBattler(state->args->tgts[k],
							SPRITE_COLOR_WHITE, POSE_DAMAGE, k, 2);
				}
			} else {
				/* Four frames of the flash, four of the spreading splash. */
				if(drop->variant >= 0 && drop->variant <= 3)
					drawDrop(canvas, state->sheet + DOUSE_FLASH_PICTURE, x, y,
						DOUSE_DROP_WIDTH, DOUSE_DROP_HEIGHT);
				else if(drop->variant <= 7)
					drawDrop(canvas, state->sheet + DOUSE_BURST_PICTURE,
						x - (DOUSE_BURST_PICTURE_WIDTH - DOUSE_DROP_WIDTH) / 2,
						y -
							(DOUSE_BURST_PICTURE_HEIGHT - DOUSE_DROP_HEIGHT) /
								2,
						DOUSE_BURST_PICTURE_WIDTH, DOUSE_BURST_PICTURE_HEIGHT);
				if(drop->variant <= FALLING_LANDED_LAST_FRAME)
					drop->variant++;
			}
		}
		/* Droplets fall and shrink as they die. */
		for(i = 0; i != ARRAY_COUNT(DOUSE_SPLASH_BUFFER->droplets); i++) {
			droplet = &DOUSE_SPLASH_BUFFER->droplets[i];
			if(droplet->variant != -1) {
				Sint32 size = droplet->variant + 1;

				if(size > 6)
					size = 6;
				EFFECT_DRAW_FLARE(drawDroplet, canvas, state->sheet,
					FIXEDtoINT(droplet->x), FIXEDtoINT(droplet->y), size);
				btParticleAdvanceWithGravity2D(droplet, 60, toFIXED(1) / 8);
				droplet->variant--;
			}
		}
		EFFECT_END_FRAME(state);
	}

	syCloseEvent(btEffectArmWin0HBlankDMA);
	EFFECT_FINISH_CANVAS();
	btEffectConfigureEffectDisp();
}

#undef FALLING_BUILD_SLANTED_WINDOW
#undef FALLING_BAND
#undef FALLING_LANDED_LAST_FRAME
#undef ICE_SHATTER_PICTURE_FRAMES
#undef ICE_PUFF_FIRST_FRAME
#undef ICE_PUFF_LAST_FRAME
#undef ICE_PUFF_FRAMES
#undef FALLING_LAND_Y
#undef DOUSE_SPLASH_BUFFER
#undef DOUSE_DROP_WIDTH
#undef DOUSE_DROP_HEIGHT
#undef DOUSE_BURST_PICTURE_WIDTH
#undef DOUSE_BURST_PICTURE_HEIGHT
#undef DOUSE_FALLING_PICTURE
#undef DOUSE_FLASH_PICTURE
#undef DOUSE_BURST_PICTURE
