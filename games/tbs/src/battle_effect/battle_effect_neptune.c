/* The Neptune summon. Part of battle_effect.c. */

/* Frames of the first stage, Neptune's leap, which A or B cuts short after its
   first 16, and of the second, the columns of water. */
#define NEPTUNE_LEAP_FRAMES		   244
#define NEPTUNE_COLUMN_FRAMES	   144
/* The first stage's sounds: flares converge on Neptune's head from
   NEPTUNE_FLARES_FRAME, looked at for NEPTUNE_FLARES_WINDOW frames, while the
   flare on the head grows and from NEPTUNE_BEAM_FRAME the beam joins it to the
   sky; the flare swells from NEPTUNE_SWELL_FRAME, and again from
   NEPTUNE_SECOND_SWELL_FRAME with a second flare beside it, until the splash
   curls up from NEPTUNE_SPLASH_FRAME and it bursts at NEPTUNE_BURST_FRAME. */
#define NEPTUNE_FLARES_FRAME	   152
#define NEPTUNE_FLARES_WINDOW	   88
#define NEPTUNE_BEAM_FRAME		   176
#define NEPTUNE_SWELL_FRAME		   192
#define NEPTUNE_SECOND_SWELL_FRAME 200
#define NEPTUNE_SPLASH_FRAME	   214
#define NEPTUNE_BURST_FRAME		   222

/* Sets BG2's horizontal scale back to one texel a pixel above the stretched
   lines. */
void btEffectResetBg2Pa(void)
{
	REG_BG2PA = Q_8_8(1);
}

/* Stretches BG2 to twice its width from the current line down. */
void btEffectSetBg2AffineScaleHalf(void)
{
	REG_BG2PA = Q_8_8(0.5);
}

#define NEPTUNE_FLARE_SHEET 20000 /* the flare pictures, after the spray */
/* The splash where the beam meets the head, growing and curling. */
#define NEPTUNE_SPLASH		PICTURE_BYTES(WATER_WAVE)
/* A water column's head, after its shaft: BlueFlameSheet's pictures. */
#define NEPTUNE_COLUMN_HEAD PICTURE_BYTES(BLUE_FLAME_COLUMN)

/* Neptune is drawn by eight 32x32 sprites (Neptune_ObjectX and _ObjectY). */
#define NEPTUNE_SPRITES				 8
/* The drops of the first spray, as Neptune leaps out of the sea. */
#define NEPTUNE_FIRST_SPRAY_DROPS	 712
/* The sea's waves along the canvas, from this row down; the spray is gone once
   it falls halfway into them. */
#define NEPTUNE_SEA_Y				 88
/* From NEPTUNE_FLARES_FRAME the canvas is drawn at its own width above this
   screen line and twice as wide below it, rather than all twice as wide. */
#define NEPTUNE_NARROW_LINES		 96
/* The flares that gather on Neptune's head, each closing in for
   NEPTUNE_FLARE_FRAMES frames on the canvas point NEPTUNE_FLARES_X,
   NEPTUNE_FLARES_Y. */
#define NEPTUNE_FLARE_COUNT			 32
#define NEPTUNE_FLARE_FRAMES		 32
#define NEPTUNE_FLARES_X			 112
#define NEPTUNE_FLARES_Y			 62
/* The sparks of the burst, falling from it. */
#define NEPTUNE_BURST_SPARKS		 64
/* The beam's width in lines: thin as the flares gather, wide from the burst,
   and wider still on the burst's second and third frames. Its middle line is
   the brightest, each line out four levels dimmer, none below
   NEPTUNE_BEAM_LEVEL_MIN. */
#define NEPTUNE_BEAM_THIN			 5
#define NEPTUNE_BEAM_WIDE			 40
#define NEPTUNE_BEAM_FLASH			 60
#define NEPTUNE_BEAM_LEVEL_MIN		 12
/* The columns of water: NEPTUNE_COLUMN_SLOTS set out, the first NEPTUNE_COLUMNS
   falling NEPTUNE_COLUMN_INTERVAL frames apart at NEPTUNE_COLUMN_SPEED pixels a
   frame until they reach the canvas row NEPTUNE_COLUMN_GROUND, where each
   stands NEPTUNE_COLUMN_LANDED_FRAMES frames, shaking the battle for the first
   NEPTUNE_COLUMN_SHAKE_FRAMES. */
#define NEPTUNE_COLUMN_SLOTS		 16
#define NEPTUNE_COLUMNS				 5
#define NEPTUNE_COLUMN_INTERVAL		 16
#define NEPTUNE_COLUMN_SPEED		 16
#define NEPTUNE_COLUMN_GROUND		 112
#define NEPTUNE_COLUMN_LANDED_FRAMES 32
#define NEPTUNE_COLUMN_SHAKE_FRAMES	 8

/* Under storm clouds Neptune leaps out of the sea in a burst of spray, light
   gathers at its head and a beam of light joins its head to the sky, with a
   splash there; then five columns of water crash down on the targets, throwing
   up drops.

   Abilities: Neptune. */
void btEffectRunNeptune(BATTLE_EFFECT_PARM *args)
{
	static SPR_SCALE		 unitScale = {{{toFIXED(3) / 2, toFIXED(3) / 2}}};
	SPR_PLACEMENT			 place;
	SCREEN_POINT			 targetPosition;
	void					*canvas;
	Uint8					*motes;
	Sint32					 windowEdgeStep;
	FIXED					 neptuneX;
	FIXED					 neptuneZ;
	Sint32					 neptuneVelocityX;
	Sint32					 neptuneVelocityZ;
	Sint32					 savedBg1ScrollX;
	BATTLE_BACKGROUND_STATE *view;
	Sint32					 j;
	Sint32					 emittedDrops;
	Sint32					 dropOriginX;
	Uint32					*heap;
	BATTLE_EFFECT_STATE		*state;
	Sint32					 frame;
	Sint32					 flareFrame;
	Sint32					 beamLines;
	Sint32					 span;
	Sint32					 i;
	PARTICLE				*flareSeed;
	PARTICLE				*flare;
	PARTICLE				*burstSeed;
	PARTICLE				*column;
	DRAW_RECTANGLE			 blit[2];
	SPR_SCALE				 scale;
	Sint32					 ground = NEPTUNE_COLUMN_GROUND;

	heap = (Uint32 *)&HeapSlots[HEAP_SLOT_BATTLE_CANVAS];
	canvas = (void *)heap[0];
	state = (BATTLE_EFFECT_STATE *)HEAP_SLOT_FROM(
		heap, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_BATTLE_EFFECT);
	motes = (Uint8 *)HEAP_SLOT_FROM(
		heap, HEAP_SLOT_BATTLE_CANVAS, HEAP_SLOT_BATTLE_SHEET);
	state->args = args;
	btEffectStartCanvasLayer(0);
	btEffectConfigureEffectDisp();
	PLTT_BACKDROP = 0;
	BG_PLTT_COLOR(0, 1) = 0;
	sySetEvent(btEffectResetBg2Pa, EVENT_PASS_VBLANK);
	syIntFunction(IRQ_VCOUNT, 0, btEffectSetBg2AffineScaleHalf);
	state->transferMode = CANVAS_COPY;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	/* Setup: wipe the battle away, swap in the storm clouds and load the
	   pictures. */
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_CLR;
	btEffectWipeCanvas(CANVAS_WIPE_ACROSS, CANVAS_WIPE_FILL);
	REG_WIN0H = WIN_RANGE(240, 240);
	btEffectShowLivingTargets(state->args);
	btEffectCreateSprs(NEPTUNE_SPRITES, SPRITE_NEPTUNE, 1);
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
	sySynch(1);
	btBackgroundLoadStub(
		BATTLE_BACKGROUND_FIXED, RESOURCE_DUSK_CLOUDS_BACKDROP, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motes, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_WATER_SPRAY_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(
		RESOURCE_BLUE_GLOW_SHEET, state->sheet + NEPTUNE_FLARE_SHEET, 0, 0);
	EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
	windowEdgeStep = 1;
	neptuneX = toFIXED(280);
	neptuneZ = toFIXED(200);
	neptuneVelocityX = -toFIXED(4);
	neptuneVelocityZ = -toFIXED(8);
	savedBg1ScrollX = BgScrollWork[1].x;
	view = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);
	EFFECT_SET_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	view->busy = 1;
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
	blit[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	blit[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
	REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(7);

	for(i = 0; i != PARTICLE_POOL_COUNT; i++) {
		PARTICLES[i].variant = -1;
	}
	for(i = 0; i != NEPTUNE_FLARE_COUNT; i++) {
		Sint32 a = (syRandom() & 0x3fff) + DEGtoANG(180);
		Sint32 r = (syRandom() & 127) + 255;

		flareSeed = &state->particles[i];
		flareSeed->x = (sySin(a) * r) >> 2;
		flareSeed->y = (syCos(a) * r) >> 2;
		flareSeed->velX = -flareSeed->x / NEPTUNE_FLARE_FRAMES;
		flareSeed->velY = -flareSeed->y / NEPTUNE_FLARE_FRAMES;
		flareSeed->variant = 0;
	}
	/* The first spray around Neptune. */
	for(i = 0; i != NEPTUNE_FIRST_SPRAY_DROPS; i++) {
		Sint32 r = syRandom() & 255;
		Sint32 a = syRandom() & TRIG_ANGLE_MASK;
		PARTICLES[i].x = ((sySin(a) * r) >> 3) + toFIXED(128);
		PARTICLES[i].y = ((syCos(a) * r) >> 4) + toFIXED(96);
		PARTICLES[i].velX = (-(syRandom() & 0x1fff) - toFIXED(1) / 32) << 5;
		PARTICLES[i].velY = (-(syRandom() & 0xfff) - toFIXED(3) / 32) << 5;
		PARTICLES[i].variant = 0;
	}

	for(frame = 0; frame != NEPTUNE_LEAP_FRAMES &&
		(frame <= 16 || !(PerRepeat & (KEY_A | KEY_B)));
		frame++) {
		/* The first stage: Neptune leaps from the sea and the beam joins it to
		   the sky. */
		if(frame == 24) {
			sySndPlayStub(SE_WAVE);
		}
		if(frame == 76) {
			sySndPlayStub(SE_SCATTER);
		}
		if(frame == 154) {
			sySndPlayStub(SE_SWIRL);
		}
		if(frame == NEPTUNE_BURST_FRAME) {
			sySndPlayStub(SE_CRASH);
		}
		/* The window opens with a ragged edge. */
		if(frame >= 0 && frame < 16) {
			Uint16 *phase = WINDOW_EDGE_RIGHT;
			if(frame == 1) {
				Uint8 *noise = WINDOW_EDGE_INSETS;
				Sint32 k;
				for(k = 0; k != WINDOW_EDGE_INSET_LINES; k++) {
					noise[k] = syRandom() & 63;
				}
				*phase = 0;
				sySetEvent(btEffectBuildWindowEdgeTable, EVENT_PASS_VBLANK);
			}
			*phase += windowEdgeStep;
			windowEdgeStep += 3;
			if(frame == 15) {
				syCloseEvent(btEffectBuildWindowEdgeTable);
			}
		}

		/* Neptune leaps in and settles. */
		if(frame > 21) {
			scale = unitScale;
			if(frame <= 83) {
				if(frame > 48) {
					neptuneVelocityX = neptuneVelocityX * 60 / DAMPING_SCALE;
				}
				neptuneVelocityZ = neptuneVelocityZ * 60 / DAMPING_SCALE;
				if(neptuneZ > toFIXED(132)) {
					neptuneVelocityZ -= toFIXED(1) / 8;
				} else {
					neptuneVelocityZ += toFIXED(1) / 8;
				}
				neptuneX += neptuneVelocityX;
				neptuneZ += neptuneVelocityZ;
			} else if(frame <= 170) {
				if(frame <= 107) {
					neptuneVelocityX += toFIXED(1) / 8;
				}
				neptuneVelocityX = neptuneVelocityX * 60 / DAMPING_SCALE;
				neptuneX += neptuneVelocityX;
				if(frame <= 117) {
					if(neptuneZ > toFIXED(138)) {
						neptuneVelocityZ -= toFIXED(1) / 8;
					} else {
						neptuneVelocityZ += toFIXED(1) / 8;
					}
					neptuneVelocityZ = neptuneVelocityZ * 56 / DAMPING_SCALE;
					neptuneZ += neptuneVelocityZ;
				}
			}
			place.groundHeight = 0;
			place.height = 0;
			for(i = 0; i != NEPTUNE_SPRITES; i++) {
				place.x = toFIXED((Neptune_ObjectX[i] * 3) >> 1) + neptuneX -
					toFIXED(32);
				place.y = toFIXED((Neptune_ObjectY[i] * 3) >> 1) + neptuneZ -
					toFIXED(24);
				fdRenderApplyProjectedPlacementStub(
					state->sprs[i], &place, &scale, 0);
			}
		}

		if(frame == 76) {
			for(i = 0; i != PARTICLE_POOL_COUNT; i++) {
				/* A second spray. */
				if(PARTICLES[i].variant == -1) {
					Sint32 r = syRandom() & 255;
					Sint32 a = syRandom() & TRIG_ANGLE_MASK;
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
		if(flareFrame >= 0 && flareFrame < NEPTUNE_FLARES_WINDOW) {
			for(i = 0; i != NEPTUNE_FLARE_COUNT; i++) {
				flare = &state->particles[i];
				if(frame >= i / 4 + NEPTUNE_FLARES_FRAME &&
					frame <
						i / 4 + NEPTUNE_FLARES_FRAME + NEPTUNE_FLARE_FRAMES) {
					Sint32 fx = FIXEDtoINT(flare->x) + NEPTUNE_FLARES_X;
					Sint32 fy = FIXEDtoINT(flare->y) + NEPTUNE_FLARES_Y;
					Sint32 size = (i & 3) + 5;
					span = size * 2;
					blit[0](canvas,
						state->sheet + BattleEffect_FlarePictures[size - 1] +
							NEPTUNE_FLARE_SHEET,
						fx - size, fy - size, span, span);
					flare->x += flare->velX;
					flare->y += flare->velY;
				}
			}
		}

		/* The burst's sparks, one more every two frames. Each keeps its start
		   in z and velocityY, to start again from there once it rises past the
		   top, but velocityY is also its fall: it drops by its start height a
		   frame, so it shows at its start and once or twice lower and is off
		   the canvas's foot within three frames, never to start again. In a
		   recording they are too brief to pick out under the burst's flash. */
		if(frame == NEPTUNE_BURST_FRAME) {
			for(i = 0; i != NEPTUNE_BURST_SPARKS; i++) {
				burstSeed = &state->particles[i];
				burstSeed->x = (syRandom() & 15) - 8;
				burstSeed->y = (syRandom() & 15) - 8;
				if(burstSeed->x < 0) {
					burstSeed->x -= 4;
				} else {
					burstSeed->x += 4;
				}
				if(burstSeed->y < 0) {
					burstSeed->y -= 4;
				} else {
					burstSeed->y += 4;
				}
				burstSeed->x += 100;
				burstSeed->y += 52;
				burstSeed->velX = -6;
				burstSeed->z = burstSeed->x;
				burstSeed->velY = burstSeed->y;
				burstSeed->variant = 0;
			}
		}
		if(frame >= NEPTUNE_BURST_FRAME) {
			Uint16 *pictures = BattleEffect_FlarePictures;

			for(i = 0; i != NEPTUNE_BURST_SPARKS; i++) {
				/* The smallest flare's size. */
				Sint32 r = 1;
				if(frame >= i / 2 + NEPTUNE_BURST_FRAME) {
					blit[0](canvas,
						state->sheet + pictures[r - 1] + NEPTUNE_FLARE_SHEET,
						state->particles[i].x - r, state->particles[i].y - r,
						r * 2, r * 2);
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
		if(frame >= NEPTUNE_BEAM_FRAME) {
			beamLines = NEPTUNE_BEAM_THIN;
			if(frame >= NEPTUNE_BURST_FRAME) {
				beamLines = NEPTUNE_BEAM_FLASH;
				if(frame != NEPTUNE_BURST_FRAME + 1 &&
					frame != NEPTUNE_BURST_FRAME + 2) {
					beamLines = NEPTUNE_BEAM_WIDE;
				}
			}
			/* DEAD: never true, as beamLines is one of the three widths here;
			   the ROM keeps the compare and the store. */
			if(beamLines > NEPTUNE_BEAM_FLASH) {
				beamLines = NEPTUNE_BEAM_FLASH;
			}
			for(i = 0; i != beamLines; i++) {
				Sint32 level;
				Sint32 offset = (i + 1) / 2;
				level = beamLines * 2 - offset * 4;
				if(level < NEPTUNE_BEAM_LEVEL_MIN) {
					level = NEPTUNE_BEAM_LEVEL_MIN;
				}
				if(level > CANVAS_LEVEL_BRIGHTEST) {
					level = CANVAS_LEVEL_BRIGHTEST;
				}
				if(i & 1) {
					offset = -offset;
				}
				btEffectDrawClippedCanvasLine(offset + 48, 0, offset / 2 + 111,
					63 - (offset + 1) / 2, level);
			}
		}

		/* The flare at the burst grows, then the splash covers it. */
		if(flareFrame >= 0 &&
			flareFrame < NEPTUNE_SWELL_FRAME - NEPTUNE_FLARES_FRAME) {
			Sint32 size;
			size = flareFrame / 2 + 1;
			if(size > 4) {
				size = 4;
			}
			EFFECT_DRAW_FLARE(blit[1], canvas,
				state->sheet + NEPTUNE_FLARE_SHEET, 108, 60, size);
		}
		if(frame >= NEPTUNE_SWELL_FRAME && frame < NEPTUNE_SECOND_SWELL_FRAME) {
			Sint32 size;
			size = (frame - NEPTUNE_SWELL_FRAME) / 2 + 3;
			if(size > 6) {
				size = 6;
			}
			EFFECT_DRAW_FLARE(blit[1], canvas,
				state->sheet + NEPTUNE_FLARE_SHEET, 108, 60, size);
		}
		if(frame >= NEPTUNE_SECOND_SWELL_FRAME) {
			Sint32 size;
			Sint32 swellFrame = frame - NEPTUNE_SECOND_SWELL_FRAME;
			size = swellFrame / 4 + 5;
			if(size > 8) {
				size = 8;
			}
			EFFECT_DRAW_FLARE(blit[1], canvas,
				state->sheet + NEPTUNE_FLARE_SHEET, 108, 60, size);
			if(frame < NEPTUNE_SPLASH_FRAME) {
				size = swellFrame / 4 + 1;
				if(size > 4) {
					size = 4;
				}
				EFFECT_DRAW_FLARE(blit[1], canvas,
					state->sheet + NEPTUNE_FLARE_SHEET, 100, 52, size);
			}
		}
		if(frame >= NEPTUNE_SPLASH_FRAME && frame < NEPTUNE_BURST_FRAME) {
			Sint32 picture;
			picture = (frame - NEPTUNE_SPLASH_FRAME) / 4;
			if(picture > 2) {
				picture = 2;
			}
			blit[1](canvas,
				state->sheet + NEPTUNE_SPLASH +
					picture * PICTURE_BYTES(WATER_SPLASH),
				63, 18, WATER_SPLASH_WIDTH, WATER_SPLASH_HEIGHT);
		}
		if(frame >= NEPTUNE_BURST_FRAME) {
			Sint32 picture;
			picture = (frame - NEPTUNE_BURST_FRAME) / 2 % 4 + 3;
			blit[0](canvas,
				state->sheet + NEPTUNE_SPLASH +
					picture * PICTURE_BYTES(WATER_SPLASH),
				72, 30, WATER_SPLASH_WIDTH, WATER_SPLASH_HEIGHT);
			if(picture == 5) {
				blit[0](canvas,
					state->sheet + NEPTUNE_SPLASH +
						2 * PICTURE_BYTES(WATER_SPLASH),
					66, 22, WATER_SPLASH_WIDTH, WATER_SPLASH_HEIGHT);
			}
		}

		/* The spray falls. */
		if(frame > 27) {
			DRAW_RECTANGLE draw = blit[1];
			for(i = 0; i != PARTICLE_POOL_COUNT; i++) {
				if(PARTICLES[i].variant >= 0) {
					Sint32 k = i % 3 + 2;
					EFFECT_DRAW_MOTE(draw, canvas, motes,
						FIXEDtoINT(PARTICLES[i].x), FIXEDtoINT(PARTICLES[i].y),
						k);
					btParticleAdvanceWithGravity2D(
						&PARTICLES[i], 62, Neptune_Gravity[i & 3]);
					PARTICLES[i].variant++;
					if(PARTICLES[i].velY > 0 &&
						FIXEDtoINT(PARTICLES[i].y) >
							NEPTUNE_SEA_Y + WATER_WAVE_HEIGHT / 2) {
						PARTICLES[i].variant = -1;
					}
				}
			}
		}
		for(i = 0; i != 5; i++) {
			/* The sea scrolls along the bottom. */
			blit[1](canvas, state->sheet,
				i * WATER_WAVE_WIDTH + ((frame / 4) & 31) - WATER_WAVE_WIDTH,
				NEPTUNE_SEA_Y, WATER_WAVE_WIDTH, WATER_WAVE_HEIGHT);
		}
		EFFECT_PRESENT_CANVAS(state);
	}

	/* The second stage: five columns of water crash down on the targets. */
	BgScrollWork[1].x = savedBg1ScrollX;
	view->busy = 0;
	syIntFunction(IRQ_VCOUNT, 0, 0);
	btEffectSetupBlendedDisp();
	syCloseEvent(btEffectResetBg2Pa);
	REG_WIN0H = WIN_RANGE(0, 240);
	EFFECT_SET_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	for(i = 0; i != PARTICLE_POOL_COUNT; i++) {
		PARTICLES[i].variant = 0;
	}
	for(i = 0; i != NEPTUNE_COLUMN_SLOTS; i++) {
		state->particles[i].x = (syRandom() & 31) + 32;
		state->particles[i].y = 0;
		state->particles[i].variant = 0;
	}
	EFFECT_FOR_EACH_TARGET(i, state->args) {
		btProjectionScaledUnit(state->args->tgts[i], &targetPosition);
		state->particles[i].x = targetPosition.x / 2;
	}
	btResourceLoadAndDecompress(RESOURCE_BLUE_FLAME_SHEET, state->sheet, 1, 1);
	sySndPlayStub(SOUND_FADE_OUT_ACTION_SE);
	REG_BG2PA = Q_8_8(0.5);
	REG_BLDALPHA = EFFECT_BLEND_ADD;

	for(frame = 0; frame != NEPTUNE_COLUMN_FRAMES; frame++) {
		if(frame == 96) {
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		}
		for(i = 0; i != NEPTUNE_COLUMNS; i++) {
			column = &state->particles[i];
			if(frame == i * NEPTUNE_COLUMN_INTERVAL + 7) {
				sySndPlayStub(SE_CAST);
			}
			/* A column falls. */
			if(frame >= i * NEPTUNE_COLUMN_INTERVAL &&
				column->variant < NEPTUNE_COLUMN_LANDED_FRAMES) {
				Sint32 wobble =
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
				if(column->y < NEPTUNE_COLUMN_GROUND) {
					column->y += NEPTUNE_COLUMN_SPEED;
				} else {
					column->variant++;
				}
				if(column->y >= NEPTUNE_COLUMN_GROUND) {
					emittedDrops = 0;
					dropOriginX = column->x;
					if(column->variant < NEPTUNE_COLUMN_SHAKE_FRAMES) {
						state->shakeFrm = 4;
					}
					for(j = 0; j != PARTICLE_POOL_COUNT; j++) {
						if(PARTICLES[j].variant == 0) {
							Sint32 r = (syRandom() & 0x3ff) + 32;
							Sint32 a;

							a = (syRandom() & 0x7fff) - DEGtoANG(90);
							PARTICLES[j].x = toFIXED(dropOriginX);
							PARTICLES[j].y = toFIXED(ground);
							PARTICLES[j].velX = (sySin(a) * r) >> 7;
							PARTICLES[j].velY = -(syCos(a) * r * 2) >> 7;
							PARTICLES[j].variant = (syRandom() & 7) + 32;
							if(++emittedDrops == 4) {
								break;
							}
						}
					}
				}
			}
		}
		EFFECT_FOR_EACH_TARGET(i, state->args) {
			if(frame == i * NEPTUNE_COLUMN_INTERVAL + 8) {
				btEffectTintBattler(state->args->tgts[i], SPRITE_COLOR_WHITE,
					POSE_DAMAGE, i, 8);
				btMotionApplyKnockbackStub(
					state->args->tgts[i], KNOCKBACK_LEAP);
			}
		}
		for(i = 0; i != PARTICLE_POOL_COUNT; i++) {
			if(PARTICLES[i].variant > 0) {
				Sint32 x;
				Sint32 y;
				/* Drops fall and bounce. */
				PARTICLES[i].variant--;
				btParticleAdvanceWithGravity2D(
					&PARTICLES[i], 60, toFIXED(1) / 4);
				y = FIXEDtoINT(PARTICLES[i].y);
				if(y > 120) {
					PARTICLES[i].velY = -PARTICLES[i].velY / 2;
				} else if(PARTICLES[i].x >= 0 &&
					(x = FIXEDtoINT(PARTICLES[i].x)) <= 126 &&
					PARTICLES[i].y >= 0) {
					Sint32 k = PARTICLES[i].variant / 8 + 3;
					EFFECT_DRAW_MOTE(blit[i & 1], canvas, motes, x, y, k);
				}
			}
		}
		btCameraShake(4, 8);
		EFFECT_END_FRAME(state);
	}

	for(i = 0; i != NEPTUNE_SPRITES; i++) {
		fdSprDestroyStub(state->sprs[i]);
	}
	EFFECT_FINISH_CANVAS();
}

#undef NEPTUNE_LEAP_FRAMES
#undef NEPTUNE_COLUMN_FRAMES
#undef NEPTUNE_FLARES_FRAME
#undef NEPTUNE_FLARES_WINDOW
#undef NEPTUNE_BEAM_FRAME
#undef NEPTUNE_SWELL_FRAME
#undef NEPTUNE_SECOND_SWELL_FRAME
#undef NEPTUNE_SPLASH_FRAME
#undef NEPTUNE_BURST_FRAME
#undef NEPTUNE_FLARE_SHEET
#undef NEPTUNE_SPLASH
#undef NEPTUNE_COLUMN_HEAD
#undef NEPTUNE_SPRITES
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
