/* @file */
/* The Neptune summon. */
/* Part of battle_effect.c. */

/* Frames of the first stage, Neptune's leap, which A or B cuts short after */
/* its first 16, and of the second, the columns of water. */
#define NEPTUNE_LEAP_FRAMES	  244
#define NEPTUNE_COLUMN_FRAMES 144
/* The first stage's flares converging on Neptune's head, and the burst at */
/* its end. */
#define NEPTUNE_FLARES_FRAME 152
#define NEPTUNE_BURST_FRAME	 222

/* BlueGlowSheet's flare pictures, after the spray. */
#define NEPTUNE_FLARE_SHEET 20000
/* The splash where the beam meets the head, growing and curling. */
#define NEPTUNE_SPLASH PICTURE_BYTES(WATER_WAVE)
/* A water column's head, after its shaft: BlueFlameSheet's pictures. */
#define NEPTUNE_COLUMN_HEAD PICTURE_BYTES(BLUE_FLAME_COLUMN)

/* Neptune is drawn by eight 32x32 sprites in two rows of four, each */
/* NEPTUNE_SPRITE_SPACING pixels from the next at its scale of 1.5 */
/* (Neptune_UnitScale). */
#define NEPTUNE_SPRITES		   8
#define NEPTUNE_SPRITE_ROW	   4
#define NEPTUNE_SPRITE_SPACING (32 * 3 / 2)
/* The drops of the first spray, as Neptune leaps out of the sea. */
#define NEPTUNE_FIRST_SPRAY_DROPS 712
/* The sea's waves along the canvas, from this row down; the spray is gone */
/* once it falls halfway into them. */
#define NEPTUNE_SEA_Y 88
/* From NEPTUNE_FLARES_FRAME the canvas is drawn at its own width above this */
/* screen line and twice as wide below it, rather than all twice as wide. */
#define NEPTUNE_NARROW_LINES 96
/* The flares that gather on Neptune's head, each closing in for */
/* NEPTUNE_FLARE_FRAMES frames on the canvas point NEPTUNE_FLARES_X, */
/* NEPTUNE_FLARES_Y. */
#define NEPTUNE_FLARE_COUNT	 32
#define NEPTUNE_FLARE_FRAMES 32
#define NEPTUNE_FLARES_X	 112
#define NEPTUNE_FLARES_Y	 62
/* The sparks of the burst, falling from it. */
#define NEPTUNE_BURST_SPARKS 64
/* The beam's width in lines: thin as the flares gather, wide from the burst, */
/* and wider still on the burst's second and third frames. Its middle line is */
/* the brightest, each line out four levels dimmer, none below */
/* NEPTUNE_BEAM_LEVEL_MIN. */
#define NEPTUNE_BEAM_THIN	   5
#define NEPTUNE_BEAM_WIDE	   40
#define NEPTUNE_BEAM_FLASH	   60
#define NEPTUNE_BEAM_LEVEL_MIN 12
/* The columns of water: NEPTUNE_COLUMN_SLOTS set out, the first */
/* NEPTUNE_COLUMNS falling NEPTUNE_COLUMN_INTERVAL frames apart at */
/* NEPTUNE_COLUMN_SPEED pixels a frame until they reach the canvas row */
/* NEPTUNE_COLUMN_GROUND, where each stands NEPTUNE_COLUMN_LANDED_FRAMES */
/* frames, shaking the battle for the first NEPTUNE_COLUMN_SHAKE_FRAMES. */
#define NEPTUNE_COLUMN_SLOTS		 16
#define NEPTUNE_COLUMNS				 5
#define NEPTUNE_COLUMN_INTERVAL		 16
#define NEPTUNE_COLUMN_SPEED		 16
#define NEPTUNE_COLUMN_GROUND		 112
#define NEPTUNE_COLUMN_LANDED_FRAMES 32
#define NEPTUNE_COLUMN_SHAKE_FRAMES	 8

/* Under storm clouds Neptune leaps out of the sea in a burst of spray, light */
/* gathers at its head and a beam of light joins its head to the sky, with a */
/* splash there; then five columns of water crash down on the targets, */
/* throwing up drops. */
/* */
/* Abilities: Neptune. */
void btEffectRunNeptune(BATTLE_EFFECT_PARM *args) {
	SPR_PLACEMENT pos;
	SCR_POINT targetPosition;
	void *canvas;
	BATTLE_EFFECT_STATE *state;
	u8 *particleSheet;
	s32 windowEdgeStep;
	FIXED neptuneX;
	FIXED neptuneZ;
	s32 neptuneVelocityX;
	s32 neptuneVelocityZ;
	s32 savedBg1ScrollX;
	BATTLE_BACKGROUND_STATE *view;
	s32 j;
	s32 emittedDrops;
	s32 frame;
	s32 flareFrame;
	s32 i;
	s32 width;
	s32 size;
	PARTICLE *flareSeed;
	PARTICLE *flare;
	PARTICLE *burstSeed;
	PARTICLE *column;
	DRAW_RECTANGLE blit[2];
	SCALE scale;
	s32 ground = NEPTUNE_COLUMN_GROUND;
	u8 *tiles;

	canvas = Ram_HeapSlots->battleCanvas;
	state = Ram_HeapSlots->battleEff;
	particleSheet = Ram_HeapSlots->battleSheet;
	tiles = MapGridCellBuffer;

	btEffectStartCanvasLayer(0);
	btEffectConfigureEffectDisp();
	PLTT_BACKDROP = 0;
	BG_PLTT_COLOR(0, 1) = 0;
	sySetEvent(btEffectResetBg2Pa, TASK_PASS_VBLANK);
	syIntFunction(IRQ_VCOUNT, 0, btEffectSetBg2AffineScaleHalf);
	state->transferMode = CANVAS_COPY;
	sySetEvent(btEffectTransferCanvas, TASK_PASS_VBLANK);
	/* Setup: wipe the battle away, swap in the storm clouds and load the */
	/* pictures. */
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_CLR;
	btEffectWipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
	REG_WIN0H = WIN_RANGE(DISPLAY_WIDTH, DISPLAY_WIDTH);
	btEffectShowLivingTargets(state->args);
	/* Neptune: its colours over the sprites', its tiles, and its sprites. */
	Ram_HeapSlots->battleState->keepBackdropPal = 1;
	{
		u8 *sheet = syResourceGetTableEntry(RESOURCE_SEA_SERPENT_TILES);
		syWordsCopy((void *)OBJ_PLTT, sheet, OBJ_PLTT_SIZE);
		sheet += OBJ_PLTT_SIZE;
		syDecompressTaggedLz(sheet, tiles);
		for(i = 0; i != NEPTUNE_SPRITES; i++) {
			SPR *sprite = btEffectSprCreateWithTiles(
				32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);

			sprite->image.pri = 1;
			state->sprs[i] = sprite;
			syWordsCopy(
				(void *)(OBJ_VRAM0 + VramCacheEntries[sprite->slot].ofs),
				tiles + i * OBJ_32X32_8BPP_BYTES, OBJ_32X32_8BPP_BYTES);
		}
	}
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
	sySynch(1);
	btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, RESOURCE_DUSK_CLOUDS_BACKDROP,
						 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, particleSheet, 0,
								0);
	btResourceLoadAndDecompress(RESOURCE_WATER_SPRAY_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET,
								state->sheet + NEPTUNE_FLARE_SHEET, 0, 0);
	EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
	windowEdgeStep = 1;
	/* Neptune leaps in from low on the right, rising to the left. */
	neptuneX = toFIXED(280);
	neptuneZ = toFIXED(200);
	neptuneVelocityX = -toFIXED(4);
	neptuneVelocityZ = -toFIXED(8);
	savedBg1ScrollX = BgScrollWork[1].x;
	view = Ram_HeapSlots->battleBg;
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
						CANVAS_FADE_THREE_QUARTERS);
	view->busy = 1;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
						BLITTER_ADDITIVE);
	blit[0] = Ram_HeapSlots->ramCode;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
						BLITTER_MAX);
	blit[1] = Ram_HeapSlots->ramCodePaired;
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);

	for(i = 0; i != PARTICLE_POOL_COUNT; i++)
		PARTICLES[i].variant = -1;
	for(i = 0, flareSeed = state->particles; i != NEPTUNE_FLARE_COUNT;
		i++, flareSeed++) {
		s32 a = (syRandom() & 0x3fff) + DEGtoANG(180);
		s32 r = (syRandom() & 127) + 255;

		flareSeed->x = (sySin(a) * r) >> 2;
		flareSeed->y = (syCos(a) * r) >> 2;
		flareSeed->velX = -flareSeed->x / NEPTUNE_FLARE_FRAMES;
		flareSeed->velY = -flareSeed->y / NEPTUNE_FLARE_FRAMES;
		flareSeed->variant = 0;
	}
	/* The first spray around Neptune. */
	for(i = 0; i != NEPTUNE_FIRST_SPRAY_DROPS; i++) {
		s32 r = syRandom() & 255;
		s32 a = syRandom() & TRIG_ANGLE_MASK;

		PARTICLES[i].x = ((sySin(a) * r) >> 3) + toFIXED(128);
		PARTICLES[i].y = ((syCos(a) * r) >> 4) + toFIXED(96);
		PARTICLES[i].velX = (-(syRandom() & 0x1fff) - toFIXED(1) / 32) << 5;
		PARTICLES[i].velY = (-(syRandom() & 0xfff) - toFIXED(3) / 32) << 5;
		PARTICLES[i].variant = 0;
	}

	for(frame = 0; frame != NEPTUNE_LEAP_FRAMES &&
				   (frame <= 16 || !(InputWork.repeat & (KEY_A | KEY_B)));
		frame++) {
		/* The first stage: Neptune leaps from the sea and the beam joins it to
		 * the */
		/* sky. */
		if(frame == 24)
			sySndPlayStub(SE_WAVE);
		if(frame == 76)
			sySndPlayStub(SE_SCATTER);
		if(frame == 154)
			sySndPlayStub(SE_SWIRL);
		if(frame == NEPTUNE_BURST_FRAME)
			sySndPlayStub(SE_CRASH);

		/* The window opens with a ragged edge. */
		if(frame >= 0 && frame < 16) {
			u16 *phase = (u16 *)Ram_MapCellBuffer;

			if(frame == 1) {
				u8 *noise = WINDOW_EDGE_INSETS;
				s32 k;

				for(k = 0; k != WINDOW_EDGE_INSET_LINES; k++)
					noise[k] = syRandom() & 63;
				*phase = 0;
				sySetEvent(btEffectBuildWindowEdgeTable, TASK_PASS_VBLANK);
			}
			*phase += windowEdgeStep;
			windowEdgeStep += 3;
			if(frame == 15)
				syCloseEvent(btEffectBuildWindowEdgeTable);
		}

		/* Neptune leaps in, slows, then glides back right and settles. */
		if(frame > 21) {
			scale = Neptune_UnitScale;
			if(frame <= 83) {
				if(frame > 48)
					neptuneVelocityX = neptuneVelocityX * 60 / DAMPING_SCALE;
				neptuneVelocityZ = neptuneVelocityZ * 60 / DAMPING_SCALE;
				if(neptuneZ > toFIXED(132))
					neptuneVelocityZ -= toFIXED(1) / 8;
				else
					neptuneVelocityZ += toFIXED(1) / 8;
				neptuneX += neptuneVelocityX;
				neptuneZ += neptuneVelocityZ;
			}
			else if(frame <= 170) {
				if(frame <= 107)
					neptuneVelocityX += toFIXED(1) / 8;
				neptuneVelocityX = neptuneVelocityX * 60 / DAMPING_SCALE;
				neptuneX += neptuneVelocityX;
				if(frame <= 117) {
					if(neptuneZ > toFIXED(138))
						neptuneVelocityZ -= toFIXED(1) / 8;
					else
						neptuneVelocityZ += toFIXED(1) / 8;
					neptuneVelocityZ = neptuneVelocityZ * 56 / DAMPING_SCALE;
					neptuneZ += neptuneVelocityZ;
				}
			}
			pos.groundHeight = 0;
			pos.height = 0;
			for(i = 0; i != NEPTUNE_SPRITES; i++) {
				pos.x = INTtoFIXED((i & (NEPTUNE_SPRITE_ROW - 1)) *
								   NEPTUNE_SPRITE_SPACING) +
						neptuneX - toFIXED(32);
				pos.y = INTtoFIXED((i / NEPTUNE_SPRITE_ROW) *
								   NEPTUNE_SPRITE_SPACING) +
						neptuneZ - toFIXED(24);
				btRenderApplyProjectedPlacementStub(
					state->sprs[i], sySprPlacementValues(&pos), &scale.x, 0);
			}
		}

		if(frame == 76) {
			for(i = 0; i != PARTICLE_POOL_COUNT; i++) {
				/* A second spray. */
				if(PARTICLES[i].variant == -1) {
					s32 r = syRandom() & 255;
					s32 a = syRandom() & TRIG_ANGLE_MASK;

					PARTICLES[i].x = ((sySin(a) * r) >> 3) + toFIXED(64);
					PARTICLES[i].y = ((syCos(a) * r) >> 4) + toFIXED(96);
					PARTICLES[i].velX = (32 - (syRandom() & 63)) << 12;
					PARTICLES[i].velY = (-(syRandom() & 31) - 16) << 13;
					PARTICLES[i].variant = 0;
				}
			}
		}
		if(frame == NEPTUNE_FLARES_FRAME) {
			syIntFunction(IRQ_VCOUNT, NEPTUNE_NARROW_LINES,
						  btEffectSetBg2AffineScaleHalf);
			state->fadeFrm = BACKGROUND_DARKEN_FRAMES;
			state->fadeStep = 0;
		}

		/* Flares converge on Neptune's head. */
		flareFrame = frame - NEPTUNE_FLARES_FRAME;
		if(flareFrame >= 0 && flareFrame < 88) {
			for(i = 0, flare = state->particles; i != NEPTUNE_FLARE_COUNT;
				i++, flare++) {
				if(frame >= i / 4 + NEPTUNE_FLARES_FRAME &&
				   frame <
					   i / 4 + NEPTUNE_FLARES_FRAME + NEPTUNE_FLARE_FRAMES) {
					s32 fx = FIXED_INTEGER_PART(flare->x) + NEPTUNE_FLARES_X;
					s32 fy = FIXED_INTEGER_PART(flare->y) + NEPTUNE_FLARES_Y;
					s32 r = (i & 3) + 5;

					size = r * 2;
					blit[0](canvas,
							state->sheet + BattleEffect_FlarePictures[r - 1] +
								NEPTUNE_FLARE_SHEET,
							fx - r, fy - r, size, size);
					flare->x += flare->velX;
					flare->y += flare->velY;
				}
			}
		}

		if(frame == NEPTUNE_BURST_FRAME) {
			for(i = 0; i != NEPTUNE_BURST_SPARKS; i++) {
				burstSeed = &state->particles[i];
				burstSeed->x = (syRandom() & 15) - 8;
				burstSeed->y = (syRandom() & 15) - 8;
				if(burstSeed->x < 0)
					burstSeed->x -= 4;
				else
					burstSeed->x += 4;
				if(burstSeed->y < 0)
					burstSeed->y -= 4;
				else
					burstSeed->y += 4;
				burstSeed->x += 100;
				burstSeed->y += 52;
				burstSeed->velX = -6;
				burstSeed->z = burstSeed->x;
				burstSeed->velY = burstSeed->y;
				burstSeed->variant = 0;
			}
		}
		if(frame >= NEPTUNE_BURST_FRAME) {
			for(i = 0; i != NEPTUNE_BURST_SPARKS; i++) {
				u16 *pictures = BattleEffect_FlarePictures;
				s32 r = 1;

				if(frame >= i / 2 + NEPTUNE_BURST_FRAME) {
					blit[0](canvas,
							state->sheet + pictures[r - 1] +
								NEPTUNE_FLARE_SHEET,
							state->particles[i].x - r,
							state->particles[i].y - r, r * 2, r * 2);
					state->particles[i].x += state->particles[i].velX;
					state->particles[i].y += state->particles[i].velY;
					if(state->particles[i].y < 0) {
						state->particles[i].y = state->particles[i].velY;
						state->particles[i].x = state->particles[i].z;
					}
				}
			}
		}

		/* The beam runs from the top of the canvas to the burst. */
		if(frame > 175) {
			width = NEPTUNE_BEAM_THIN;
			if(frame >= NEPTUNE_BURST_FRAME) {
				width = NEPTUNE_BEAM_FLASH;
				if(frame != NEPTUNE_BURST_FRAME + 1 &&
				   frame != NEPTUNE_BURST_FRAME + 2)
					width = NEPTUNE_BEAM_WIDE;
			}
			/* DEAD: never true, as width is one of the three widths here; the
			 * ROM */
			/* keeps the compare and the store. */
			if(width > NEPTUNE_BEAM_FLASH)
				width = NEPTUNE_BEAM_FLASH;
			for(i = 0; i != width; i++) {
				s32 level;
				s32 offset = (i + 1) / 2;

				level = width * 2 - offset * 4;
				if(level < NEPTUNE_BEAM_LEVEL_MIN)
					level = NEPTUNE_BEAM_LEVEL_MIN;
				if(level > CANVAS_LEVEL_BRIGHTEST)
					level = CANVAS_LEVEL_BRIGHTEST;
				if(i & 1)
					offset = -offset;
				btEffectDrawClippedCanvasLine(offset + 48, 0, offset / 2 + 111,
											  63 - (offset + 1) / 2, level);
			}
		}

		/* The flare at the burst grows, then the splash covers it. */
		if(flareFrame >= 0 && flareFrame < 40) {
			s32 flareSize;

			flareSize = flareFrame / 2 + 1;
			if(flareSize > 4)
				flareSize = 4;
			EFFECT_DRAW_FLARE(blit[1], canvas,
							  state->sheet + NEPTUNE_FLARE_SHEET, 108, 60,
							  flareSize);
		}
		if(frame >= 192 && frame < 200) {
			s32 flareSize;

			flareSize = (frame - 192) / 2 + 3;
			if(flareSize > 6)
				flareSize = 6;
			EFFECT_DRAW_FLARE(blit[1], canvas,
							  state->sheet + NEPTUNE_FLARE_SHEET, 108, 60,
							  flareSize);
		}
		if(frame > 199) {
			s32 flareSize;
			s32 u = frame - 200;

			flareSize = u / 4 + 5;
			if(flareSize > 8)
				flareSize = 8;
			EFFECT_DRAW_FLARE(blit[1], canvas,
							  state->sheet + NEPTUNE_FLARE_SHEET, 108, 60,
							  flareSize);
			if(frame <= 213) {
				flareSize = u / 4 + 1;
				if(flareSize > 4)
					flareSize = 4;
				EFFECT_DRAW_FLARE(blit[1], canvas,
								  state->sheet + NEPTUNE_FLARE_SHEET, 100, 52,
								  flareSize);
			}
		}
		if(frame >= NEPTUNE_BURST_FRAME - 8 && frame < NEPTUNE_BURST_FRAME) {
			s32 picture;

			picture = (frame - (NEPTUNE_BURST_FRAME - 8)) / 4;
			if(picture > 2)
				picture = 2;
			blit[1](canvas,
					state->sheet + picture * PICTURE_BYTES(WATER_SPLASH) +
						NEPTUNE_SPLASH,
					63, 18, WATER_SPLASH_WIDTH, WATER_SPLASH_HEIGHT);
		}
		if(frame >= NEPTUNE_BURST_FRAME) {
			s32 picture;

			picture = (frame - NEPTUNE_BURST_FRAME) / 2 % 4 + 3;
			blit[0](canvas,
					state->sheet + picture * PICTURE_BYTES(WATER_SPLASH) +
						NEPTUNE_SPLASH,
					72, 30, WATER_SPLASH_WIDTH, WATER_SPLASH_HEIGHT);
			if(picture == 5)
				blit[0](canvas,
						state->sheet + NEPTUNE_SPLASH +
							2 * PICTURE_BYTES(WATER_SPLASH),
						66, 22, WATER_SPLASH_WIDTH, WATER_SPLASH_HEIGHT);
		}

		/* The spray falls. */
		if(frame > 27) {
			DRAW_RECTANGLE draw = blit[1];

			for(i = 0; i != PARTICLE_POOL_COUNT; i++) {
				if(PARTICLES[i].variant >= 0) {
					s32 k = i % 3 + 2;

					EFFECT_DRAW_MOTE(draw, canvas, particleSheet,
									 FIXED_INTEGER_PART(PARTICLES[i].x),
									 FIXED_INTEGER_PART(PARTICLES[i].y), k);
					btParticleAdvanceWithGravity2D(&PARTICLES[i], 62,
												   Neptune_Gravity[i & 3]);
					PARTICLES[i].variant++;
					if(PARTICLES[i].velY > 0 &&
					   FIXED_INTEGER_PART(PARTICLES[i].y) >
						   NEPTUNE_SEA_Y + WATER_WAVE_HEIGHT / 2)
						PARTICLES[i].variant = -1;
				}
			}
		}

		/* The sea scrolls along the bottom. */
		for(i = 0; i != 5; i++)
			blit[1](canvas, state->sheet,
					i * WATER_WAVE_WIDTH + ((frame / 4) & 31) -
						WATER_WAVE_WIDTH,
					NEPTUNE_SEA_Y, WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
		EFFECT_PRESENT_CANVAS(state);
	}

	/* The second stage: five columns of water crash down on the targets. */
	BgScrollWork[1].x = savedBg1ScrollX;
	view->busy = 0;
	syIntFunction(IRQ_VCOUNT, 0, 0);
	btEffectSetupBlendedDisp();
	syPaletteLoadBasicSprCols();
	syDMAXCopy((const void *)OBJ_PLTT,
			   (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
			   DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
	BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
		BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
	syCloseEvent(btEffectResetBg2Pa);
	REG_WIN0H = WIN_RANGE(0, DISPLAY_WIDTH);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
						CANVAS_FADE_THREE_QUARTERS);
	for(i = 0; i != PARTICLE_POOL_COUNT; i++)
		PARTICLES[i].variant = 0;
	for(i = 0; i != NEPTUNE_COLUMN_SLOTS; i++) {
		state->particles[i].x = (syRandom() & 31) + 32;
		state->particles[i].y = 0;
		state->particles[i].variant = 0;
	}
	EFFECT_FOR_EACH_TARGET(i, args) {
		btEffectPosProjectScaledBattler(args->tgts[i], &targetPosition);
		state->particles[i].x = targetPosition.x / 2;
	}
	btResourceLoadAndDecompress(RESOURCE_BLUE_FLAME_SHEET, state->sheet, 1, 1);
	sySndPlayStub(SOUND_FADE_OUT_ACTION_SE);
	REG_BG2PA = Q_8_8(0.5);
	REG_BLDALPHA = EFFECT_BLEND_ADD;

	for(frame = 0; frame != NEPTUNE_COLUMN_FRAMES; frame++) {
		if(frame == 96)
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		for(i = 0; i != NEPTUNE_COLUMNS; i++) {
			column = &state->particles[i];
			if(frame == i * NEPTUNE_COLUMN_INTERVAL + 7)
				sySndPlayStub(SE_CAST);
			/* A column falls. */
			if(frame >= i * NEPTUNE_COLUMN_INTERVAL &&
			   column->variant < NEPTUNE_COLUMN_LANDED_FRAMES) {
				s32 wobble =
					frame * NEPTUNE_COLUMN_SPEED % BLUE_FLAME_COLUMN_HEIGHT;

				blit[1](canvas, state->sheet,
						column->x - BLUE_FLAME_COLUMN_WIDTH / 2,
						column->y + wobble - 216, BLUE_FLAME_COLUMN_WIDTH,
						BLUE_FLAME_COLUMN_HEIGHT);
				blit[1](canvas, state->sheet,
						column->x - BLUE_FLAME_COLUMN_WIDTH / 2,
						column->y + wobble - 112, BLUE_FLAME_COLUMN_WIDTH,
						BLUE_FLAME_COLUMN_HEIGHT - wobble);
				blit[1](canvas, state->sheet + NEPTUNE_COLUMN_HEAD,
						column->x - BLUE_FLAME_SPLASH_WIDTH / 2,
						column->y - BLUE_FLAME_SPLASH_HEIGHT,
						BLUE_FLAME_SPLASH_WIDTH, BLUE_FLAME_SPLASH_HEIGHT);
				if(column->y < NEPTUNE_COLUMN_GROUND)
					column->y += NEPTUNE_COLUMN_SPEED;
				else
					column->variant++;
				if(column->y >= NEPTUNE_COLUMN_GROUND) {
					s32 dropOriginX;

					emittedDrops = 0;
					dropOriginX = column->x;
					if(column->variant < NEPTUNE_COLUMN_SHAKE_FRAMES)
						state->shakeFrm = 4;
					for(j = 0; j != PARTICLE_POOL_COUNT; j++) {
						if(PARTICLES[j].variant == 0) {
							s32 r = (syRandom() & 0x3ff) + 32;
							s32 a;

							a = (syRandom() & 0x7fff) - DEGtoANG(90);
							PARTICLES[j].x = INTtoFIXED(dropOriginX);
							PARTICLES[j].y = INTtoFIXED(ground);
							PARTICLES[j].velX = (sySin(a) * r) >> 7;
							PARTICLES[j].velY = -(syCos(a) * r * 2) >> 7;
							PARTICLES[j].variant = (syRandom() & 7) + 32;
							if(++emittedDrops == 4)
								break;
						}
					}
				}
			}
		}
		EFFECT_FOR_EACH_TARGET(i, args) {
			if(frame == i * NEPTUNE_COLUMN_INTERVAL + 8)
				EFFECT_STRIKE_TARGET(args, i, 8, KNOCKBACK_LEAP);
		}
		for(i = 0; i != PARTICLE_POOL_COUNT; i++) {
			if(PARTICLES[i].variant > 0) {
				s32 x;
				s32 y;

				/* Drops fall and bounce. */
				PARTICLES[i].variant--;
				btParticleAdvanceWithGravity2D(&PARTICLES[i], 60,
											   toFIXED(1) / 4);
				y = FIXEDtoINT(PARTICLES[i].y);
				if(y > 120) {
					PARTICLES[i].velY = -PARTICLES[i].velY / 2;
				}
				else if(PARTICLES[i].x >= 0 &&
						(x = FIXEDtoINT(PARTICLES[i].x)) <= 126 &&
						PARTICLES[i].y >= 0) {
					s32 k = PARTICLES[i].variant / 8 + 3;

					EFFECT_DRAW_MOTE(blit[i & 1], canvas, particleSheet, x, y,
									 k);
				}
			}
		}
		btCameraShake(4, 8);
		EFFECT_END_FRAME(state);
	}

	for(i = 0; i != NEPTUNE_SPRITES; i++)
		fdSprDestroyStub(state->sprs[i]);
	EFFECT_FINISH_CANVAS();
}

#undef NEPTUNE_LEAP_FRAMES
#undef NEPTUNE_COLUMN_FRAMES
#undef NEPTUNE_FLARES_FRAME
#undef NEPTUNE_BURST_FRAME
#undef NEPTUNE_FLARE_SHEET
#undef NEPTUNE_SPLASH
#undef NEPTUNE_COLUMN_HEAD
#undef NEPTUNE_SPRITES
#undef NEPTUNE_SPRITE_ROW
#undef NEPTUNE_SPRITE_SPACING
#undef NEPTUNE_FIRST_SPRAY_DROPS
#undef NEPTUNE_SEA_Y
#undef NEPTUNE_NARROW_LINES
#undef NEPTUNE_FLARE_COUNT
#undef NEPTUNE_FLARE_FRAMES
#undef NEPTUNE_FLARES_X
#undef NEPTUNE_FLARES_Y
#undef NEPTUNE_BURST_SPARKS
#undef NEPTUNE_BEAM_THIN
#undef NEPTUNE_BEAM_WIDE
#undef NEPTUNE_BEAM_FLASH
#undef NEPTUNE_BEAM_LEVEL_MIN
#undef NEPTUNE_COLUMN_SLOTS
#undef NEPTUNE_COLUMNS
#undef NEPTUNE_COLUMN_INTERVAL
#undef NEPTUNE_COLUMN_SPEED
#undef NEPTUNE_COLUMN_GROUND
#undef NEPTUNE_COLUMN_LANDED_FRAMES
#undef NEPTUNE_COLUMN_SHAKE_FRAMES
