/* The Sleep, Baffle, Death and Sword Cards. Part of battle_effect.c. */

/* The Death Card's RedCrescentSheetA, decoded at the start of the map cell
   buffer: its four pictures' bytes. */
#define RED_CRESCENT_BYTES \
	(RED_CRESCENT_FADE_D + PICTURE_BYTES(RED_CRESCENT_FADE_D))
/* The card's picture, CARD_WIDTH by CARD_HEIGHT, in the map cell buffer after
   the crescent's; SheepCardSheet's four sheep follow it. CARD_SHEEP reaches the
   same buffer by its fixed address. */
#define CARD (MapGridMetatileBuffer + RED_CRESCENT_BYTES)
#define CARD_SHEEP \
	(RamMapMetatileBuffer + RED_CRESCENT_BYTES + CARD_WIDTH * CARD_HEIGHT)
/* After the Death and Sword cards' pictures: the step of the dissolve, 0 to 15,
   at which each pixel clears, for eight rows repeated down the card. */
#define CARD_DISSOLVE_STEPS (CARD + CARD_WIDTH * CARD_HEIGHT)

/* The card flies to the targets for CARD_FLIGHT_TICKS, turning back a
   CARD_SPIN a tick about its face and its width; it would stand square at
   CARD_SQUARE_TICK, and arrives a sixteenth of a turn short of it. */
#define CARD_FLIGHT_TICKS	   48
#define CARD_SQUARE_TICK	   52
#define CARD_SPIN			   (TRIG_TURN / 64)
/* The card's turn about its upright axis, about 33 degrees. */
#define CARD_SLANT			   6000
/* The ticks the card is drawn: the Sleep Card's past the effect's end, the
   Sword Card's until its dagger strikes. */
#define CARD_SHOWN_TICKS	   80
#define SLEEP_CARD_SHOWN_TICKS 120
/* The Death and Sword cards dissolve a step a tick from CARD_FLIGHT_TICKS. */
#define CARD_DISSOLVE_TICKS	   16

/* The Death Card's Reaper rises from REAPER_RISE_TICK and swings at
   REAPER_SWING_TICK; two ticks of flash, then the red crescent and its four
   fading pictures, two ticks each. */
#define REAPER_RISE_TICK  52
#define REAPER_SWING_TICK 64

/* Each puff of the Baffle Card's mist lives BAFFLE_PUFF_TICKS, through
   BlueCloudPuffSheet's four pictures. */
#define BAFFLE_PUFF_TICKS 16
/* Each sheep tumbles out of the Sleep Card for SHEEP_TICKS, through its four
   pictures every 16 ticks. */
#define SHEEP_TICKS		  64

/* The Sword Card's dagger glints over the target for SWORD_CARD_DAGGER_TICKS,
   then strikes and SWORD_CARD_PARTICLES fly, of which SWORD_CARD_SHARDS are
   drawn as shards for SWORD_CARD_SHARD_TICKS, a copy darker every four ticks
   after the first four. The shards start one a line from SWORD_CARD_SHARD_TOP,
   down the dagger, and the slowest flies SWORD_CARD_SHARD_SPEED, a pixel a
   tick in 64ths. */
#define SWORD_CARD_DAGGER_TICKS	 24
#define SWORD_CARD_PARTICLES	 64
#define SWORD_CARD_SHARDS		 32
#define SWORD_CARD_SHARD_TICKS	 36
#define SWORD_CARD_SHARD_TOP	 48
#define SWORD_CARD_SHARD_SPEED	 64
/* Each darker copy of the stat reduction's shard pictures
   (STAT_REDUCTION_SHARD_PICTURES) is this many levels darker than the one
   before, but never blank. */
#define SWORD_CARD_SHARD_DIMMING 6

/* The four cards (Sleep, Baffle, Death and Sword Card): a card flies from the
   user to above the targets' middle, spinning upright and growing, and the
   background scrolls with it as far as it can. Then its picture comes to life:
   sheep tumble out of the Sleep Card; a mist pours from the Baffle Card and
   spreads over the ground while the screen waves; the Death and Sword cards
   dissolve, and the Reaper rises and swings his scythe in a red crescent, or a
   dagger glints, strikes and breaks into shards. The screen fades in the last
   eight ticks. */
void btEffectRunCards(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Uint8				*graphics;
	DRAW_RECTANGLE		 draw[2];
	FIXED				 scale;
	Sint32				 scroll;
	Sint32				 overflow;
	Sint32				 totalTicks;
	Sint32				 frm;
	SCREEN_POINT		 card;
	SCREEN_POINT		 middle;
	MODEL_TEXTURE		 texture;
	PARTICLE			*step;
	RENDER_MODEL		*model;
	void				*vertices;
	Sint32				 i;
	Sint32				 j;
	Sint32				 cardTicks;
	Sint32				 first;
	Sint32				 slot;

	state = RamHeapSlots->battleEff;
	canvas = RamHeapSlots->battleCanvas;
	graphics = RamHeapSlots->battleSheet;

	/* Set up a tiled canvas, filled on the user's side, and a blitter that
	   keeps the brighter pixel (that adds, for the Sword Card), mirrored to
	   face the targets. */
	btEffectStartTiledCanvas(BGCNT_WRAP | BGCNT_PRIORITY(1));
	if(args->side == EFFECT_SIDE_ENEMIES)
		btEffectStartTiledCanvasFilled(BGCNT_WRAP | BGCNT_PRIORITY(1));
	else
		btEffectStartTiledCanvas(BGCNT_WRAP | BGCNT_PRIORITY(1));
	REG_BLDALPHA = BLEND_ALPHA(16, 2);
	if(mode == CARD_SWORD) {
		if(args->side == EFFECT_SIDE_ENEMIES)
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
		else
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
	} else {
		if(args->side == EFFECT_SIDE_ENEMIES)
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
		else
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
	}
	draw[0] = RamHeapSlots->ramCode;
	scale = 0;
	scroll = 0;
	overflow = 0;

	/* Load the card and what comes out of it; the Sword Card makes seven darker
	   copies of its shards for their fade. */
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
	if(mode == CARD_SLEEP) {
		btResourceLoadAndDecompress(RESOURCE_SHEEP_CARD_SHEET, CARD, 1, 1);
		btResourceLoadAndDecompress(
			RESOURCE_BLUE_CLOUD_PUFF_SHEET, state->sheet, 0, 0);
		totalTicks = 96;
	} else if(mode == CARD_BAFFLE) {
		btResourceLoadAndDecompress(RESOURCE_BUTTERFLY_CARD_SHEET, CARD, 1, 1);
		btResourceLoadAndDecompress(
			RESOURCE_BLUE_CLOUD_PUFF_SHEET, state->sheet, 0, 0);
		totalTicks = 80;
	} else if(mode == CARD_SWORD) {
		btResourceLoadAndDecompress(RESOURCE_SWORD_CARD_SHEET, CARD, 1, 1);
		btResourceLoadAndDecompress(RESOURCE_DAGGER_SHEET, state->sheet, 1, 0);
		btResourceLoadAndDecompress(RESOURCE_PALETTE_RAMP_IMAGE,
			state->sheet + STAT_REDUCTION_SHARD_PICTURES, 0, 0);
		for(i = 1; i != DIMMED_COPIES; i++) {
			for(j = 0; j != STAT_REDUCTION_SHARD_BYTES; j++) {
				Sint32 shade = state->sheet[STAT_REDUCTION_SHARD_PICTURES + j];

				if(shade > 0) {
					shade -= i * SWORD_CARD_SHARD_DIMMING;
					if(shade <= 0)
						shade = 1;
				}
				state->sheet[STAT_REDUCTION_SHARD_PICTURES +
					i * STAT_REDUCTION_SHARD_BYTES + j] = shade;
			}
		}
		totalTicks = 116;
	} else {
		btResourceLoadAndDecompress(RESOURCE_SKELETON_CARD_SHEET, CARD, 1, 1);
		btResourceLoadAndDecompress(
			RESOURCE_COUNTER_REVEAL_SHEET, state->sheet, 1, 0);
		btResourceLoadAndDecompress(
			RESOURCE_RED_CRESCENT_SHEET_A, RamMapMetatileBuffer, 1, 0);
		totalTicks = 88;
	}
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	if(mode == CARD_BAFFLE)
		sySetEvent(btEffectArmBg2AffineHBlankDMA, EVENT_PASS_VBLANK);
	REG_BG2PA = Q_8_8(1);

	for(frm = 0; frm != totalTicks; frm++) {
		/* Tick 0: find the user and the targets' middle (the Death Card's 32
		   pixels toward the user), and scroll the background to the user. */
		if(frm == 0) {
			sySndPlayStub(SE_WARP);
			btEffectPosProjectSpr(args->user, &card);
			btEffectGetTargetsMidpoint(&middle);
			if(mode == CARD_DEATH) {
				if(args->side == EFFECT_SIDE_ENEMIES)
					middle.x += 32;
				else
					middle.x -= 32;
			}
			REG_BG2X = (56 - card.x) << 8;
			scroll = 56 - card.x;
			scale = 0;
			EFFECT_FREE_PARTICLES(i, state->particles, 64);
		}

		/* The last eight ticks: the screen fades. */
		if(frm >= totalTicks - 8) {
			Sint32 fade = frm - totalTicks + 8;

			REG_BLDALPHA = ((fade + 8) << 8) | (16 - fade * 2);
		}
		if(frm == CARD_FLIGHT_TICKS && mode == CARD_BAFFLE)
			sySndPlayStub(SE_ITEM_BREAK);

		/* The card flies to the middle, rising and growing; the background
		   follows it as far as it can scroll. */
		if(frm < CARD_FLIGHT_TICKS) {
			Sint32 t = frm * (middle.x - card.x);
			Sint32 q;
			Sint32 x;

			first = card.x;
			q = t / CARD_FLIGHT_TICKS;
			first -= CANVAS_SIZE / 2;
			x = first + q;

			if(x < 0) {
				overflow = x;
				x = 0;
			} else if(x > DISPLAY_WIDTH - CANVAS_SIZE) {
				overflow = x - (DISPLAY_WIDTH - CANVAS_SIZE);
				x = DISPLAY_WIDTH - CANVAS_SIZE;
			}
			x <<= 8;
			REG_BG2X = -x;
			scroll = -x;
			card.y--;
			scale += toFIXED(1) / 64;
			if(scale > toFIXED(1) / 2)
				scale = toFIXED(1) / 2;
		}

		if(mode != CARD_SLEEP) {
			if(mode == CARD_BAFFLE) {
				/* The screen waves, line by line. */
				Sint32 *line = state->bg2xLines;

				for(i = 0; i != DISPLAY_HEIGHT; i++)
					*line++ = scroll -
						CANVAS_WAVE_BG2X(sySin(frm * (TRIG_TURN / 16) +
											 i * (TRIG_TURN / 16)) *
							4);
			} else {
				/* The canvas's colours step to the smoke's, then to the
				   Reaper's or the dagger's; the card dissolves in 16 random
				   steps. */
				Uint8 *steps = CARD_DISSOLVE_STEPS;

				if((mode == CARD_DEATH || mode == CARD_SWORD) &&
					(frm >= CARD_FLIGHT_TICKS - 8 && frm < CARD_FLIGHT_TICKS))
					btEffectStepPaletteToResource(RESOURCE_SMOKE_SHEET);
				if(frm >= REAPER_RISE_TICK) {
					if(mode == CARD_DEATH)
						btEffectStepPaletteToResource(
							RESOURCE_RED_CRESCENT_SHEET_A);
					else
						btEffectStepPaletteToResource(RESOURCE_DAGGER_SHEET);
				}
				if(frm == CARD_FLIGHT_TICKS) {
					for(i = 0; i != 8 * CARD_WIDTH; i++)
						steps[i] = syRandom() & (CARD_DISSOLVE_TICKS - 1);
				}
				if(frm >= CARD_FLIGHT_TICKS &&
					frm < CARD_FLIGHT_TICKS + CARD_DISSOLVE_TICKS) {
					Uint8 *card = CARD;

					for(i = 0; i != CARD_HEIGHT; i++) {
						/* first, unused after the flight, holds the cleared
						   pixel. */
						for(j = 0; j != CARD_WIDTH; j++) {
							/* FAKEMATCH(dead): first cleared inside the test,
							 * or'd into the step; as first = 0 at the top of
							 * the outer body. */
							if(steps[(i & 7) * CARD_WIDTH + j] ==
								((frm - CARD_FLIGHT_TICKS) | (first = 0)))
								card[i * CARD_WIDTH + j] = first;
						}
					}
				}
			}
		}

		/* The card, until its picture takes over. */
		if(mode == CARD_SLEEP)
			cardTicks = SLEEP_CARD_SHOWN_TICKS;
		else
			cardTicks = mode == CARD_SWORD
				? CARD_FLIGHT_TICKS + SWORD_CARD_DAGGER_TICKS
				: CARD_SHOWN_TICKS;
		if(frm < cardTicks) {
			vertices = syHeapAllocEWRAM(32);
			model = btRenderAllocateModelList(1);
			texture.logWidth = 5;
			texture.logHeight = 6;
			model->vertices = vertices;
			texture.pixels = CARD;
			model->kind = MODEL_TEXTURED;
			model->texture = &texture;
			model->mesh = BattleEffectTallMesh;
			syInitMatrix();
			syScale(toFIXED(1), toFIXED(1), toFIXED(1));
			syTranslate(toFIXED(overflow), toFIXED(card.y) - toFIXED(64), 0);
			if(frm < CARD_FLIGHT_TICKS) {
				if(args->side == EFFECT_SIDE_ENEMIES)
					syRotZ((frm - CARD_SQUARE_TICK) * CARD_SPIN);
				else
					syRotZ((CARD_SQUARE_TICK - frm) * CARD_SPIN);
				syRotX((CARD_SQUARE_TICK - frm) * CARD_SPIN);
			} else {
				if(args->side == EFFECT_SIDE_ENEMIES)
					syRotZ(-TRIG_TURN / 16);
				else
					syRotZ(TRIG_TURN / 16);
				syRotX(TRIG_TURN / 16);
			}
			syRotY(CARD_SLANT);
			syMatrixScaleUniform(scale);
			syScale(toFIXED(1), toFIXED(2), toFIXED(1));
			btRenderTransformVertices(BattleEffectSquareCorners, vertices, 4);
			btRenderDrawModelList(model);
			syHeapRelease(model);
			syHeapRelease(vertices);
		}

		if(mode == CARD_BAFFLE) {
			/* The mist pours out, two puffs a tick for 32 ticks; each falls
			   to the ground and spreads along it. */
			if(frm == CARD_FLIGHT_TICKS) {
				EFFECT_SHOW_DAMAGE(SE_WHOOSH);
				EFFECT_SET_TRANSFER(
					state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
				REG_BLDALPHA = EFFECT_BLEND_ADD;
			}
			if(frm >= CARD_FLIGHT_TICKS && frm < CARD_FLIGHT_TICKS + 32) {
				Sint32 time = frm - CARD_FLIGHT_TICKS;

				slot = time * 2;
				for(i = 0; i != 2; i++) {
					step = &state->particles[slot + i];
					step->x = toFIXED(64);
					step->y = toFIXED(48);
					step->velX = ((syRandom() & 127) - 64) << 11;
					step->velY = ((syRandom() & 127) + 64) << 11;
					step->variant = 0;
				}
			}
			for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
				step = &state->particles[i];
				if(step->variant >= 0 && step->variant < BAFFLE_PUFF_TICKS) {
					draw[0](canvas,
						state->sheet +
							step->variant / 4 * PICTURE_BYTES(CLOUD_PUFF),
						overflow + FIXED_INTEGER_PART(step->x) -
							CLOUD_PUFF_WIDTH / 2,
						FIXED_INTEGER_PART(step->y) - CLOUD_PUFF_HEIGHT / 2,
						CLOUD_PUFF_WIDTH, CLOUD_PUFF_HEIGHT);
					btParticleAdvanceWithGravity2D(step, 62, -toFIXED(1) / 16);
					if(step->y > toFIXED(96)) {
						step->velY = 0;
						step->y = toFIXED(96);
						step->velX *= 2;
					}
					step->variant++;
				}
			}
		}

		if(mode == CARD_SLEEP) {
			/* A sheep a tick for 32 ticks tumbles out of the card. */
			if(frm == CARD_FLIGHT_TICKS)
				sySndPlayStub(SE_POWER_UP);
			if(frm == 64)
				EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
			if(frm >= CARD_FLIGHT_TICKS && frm < CARD_FLIGHT_TICKS + 32) {
				PARTICLE *sheep;
				Uint32	  mask = 127;

				slot = frm - CARD_FLIGHT_TICKS;
				sheep = &state->particles[slot];
				sheep->x = toFIXED(64);
				sheep->y = toFIXED(48);
				sheep->velX = ((syRandom() & mask) - 64) << 11;
				sheep->velY = ((syRandom() & mask) - 64) << 11;
				sheep->variant = mode;
			}
			for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
				step = &state->particles[i];
				if(step->variant >= 0 && step->variant < SHEEP_TICKS) {
					draw[0](canvas,
						CARD_SHEEP +
							(step->variant / 4 & 3) * PICTURE_BYTES(SHEEP),
						overflow + FIXED_INTEGER_PART(step->x) -
							SHEEP_WIDTH / 2,
						FIXED_INTEGER_PART(step->y) - SHEEP_HEIGHT / 2,
						SHEEP_WIDTH, SHEEP_HEIGHT);
					btParticleAdvanceWithGravity2D(step, 62, toFIXED(1) / 8);
					step->variant++;
				}
			}
		}

		if(mode == CARD_SWORD && frm >= CARD_FLIGHT_TICKS) {
			Sint32 time = frm - CARD_FLIGHT_TICKS;

			/* The dagger glints. */
			if(time < SWORD_CARD_DAGGER_TICKS)
				draw[0](canvas,
					state->sheet + time / 4 % 6 * PICTURE_BYTES(DAGGER),
					overflow + 52, 44, DAGGER_WIDTH, DAGGER_HEIGHT);

			/* It strikes: the screen flashes and the shards burst. */
			if(time == SWORD_CARD_DAGGER_TICKS) {
				sySndPlayStub(SE_POWER_UP);
				EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
				state->shakeFrm = 4;
				syWordsFillEntry(canvas, CANVAS_BYTES,
					CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
				btEffectTintBattler(
					args->tgts[0], SPRITE_COLOR_WHITE, TINT_KEEP, 0, 16);
				for(i = 0; i != SWORD_CARD_PARTICLES; i++) {
					Sint32 angle = syRandom() & TRIG_ANGLE_MASK;
					Sint32 force = 127;

					/* A random force below 128. */
					force &= syRandom();

					step = &state->particles[i];
					step->x = toFIXED(64);
					step->y = INTtoFIXED(i + SWORD_CARD_SHARD_TOP);
					step->velX =
						(syCos(angle) * (force + SWORD_CARD_SHARD_SPEED)) >> 6;
					step->velY =
						(sySin(angle) * (force + SWORD_CARD_SHARD_SPEED)) >> 6;
					step->variant = syRandom() & 255;
				}
				EFFECT_SET_TRANSFER(
					state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
				REG_BLDALPHA = EFFECT_BLEND_ADD;
			}

			/* The shards tumble through three pictures each and darken. */
			if(time >= SWORD_CARD_DAGGER_TICKS &&
				time <= SWORD_CARD_DAGGER_TICKS + SWORD_CARD_SHARD_TICKS) {
				Sint32 fade = 0;

				if(time > SWORD_CARD_DAGGER_TICKS + 4) {
					fade = (time - SWORD_CARD_DAGGER_TICKS) / 4;
					if(fade > DIMMED_COPIES - 1)
						fade = DIMMED_COPIES - 1;
				}
				for(i = 0; i != SWORD_CARD_SHARDS; i++) {
					Sint32 picture;

					step = &state->particles[i];
					picture = i % 4 * 3 + (step->variant + time) / 8 % 3;
					draw[0](canvas,
						state->sheet + STAT_REDUCTION_SHARD_PICTURES +
							(fade * STAT_REDUCTION_SHARD_BYTES +
								StatChangeShardPictures[picture]),
						overflow + FIXED_INTEGER_PART(step->x) -
							StatChangeShardWidths[picture] / 2,
						FIXED_INTEGER_PART(step->y) -
							StatChangeShardHeights[picture] / 2,
						StatChangeShardWidths[picture],
						StatChangeShardHeights[picture]);
					btParticleAdvanceWithGravity2D(step, 60, 0);
				}
			}
		}

		if(mode == CARD_DEATH) {
			/* The Reaper rises and swings: his scythe cuts a red crescent that
			   fades in four pictures. */
			if(frm == REAPER_SWING_TICK - 4)
				sySndPlayStub(SE_POWER_UP);
			if(frm == REAPER_SWING_TICK) {
				sySndPlayStub(SE_CLATTER);
				EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
				btEffectStartActorMotion(
					args->tgts[0], 1, toFIXED(2), toFIXED(8), toFIXED(2), 110);
				btEffectTintBattler(
					args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
				state->shakeFrm = 4;
			}
			if(frm >= REAPER_RISE_TICK) {
				if(frm < REAPER_SWING_TICK) {
					draw[0](canvas, state->sheet,
						overflow + ReaperPictureX[args->side * 7],
						ReaperPictureY[0], REAPER_SHEET_RAISED_WIDTH,
						REAPER_SHEET_RAISED_HEIGHT);
				} else {
					if(frm <= REAPER_SWING_TICK + 3)
						draw[0](canvas, state->sheet,
							overflow + ReaperPictureX[args->side * 7],
							ReaperPictureY[0], REAPER_SHEET_RAISED_WIDTH,
							REAPER_SHEET_RAISED_HEIGHT);
					EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
						REAPER_SHEET_SWUNG,
						overflow + ReaperPictureX[args->side * 7 + 1],
						ReaperPictureY[1]);
					if(frm >= REAPER_SWING_TICK && frm <= REAPER_SWING_TICK + 1)
						syWordsFillEntry(canvas, CANVAS_BYTES,
							CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
					if(frm >= REAPER_SWING_TICK + 2 &&
						frm <= REAPER_SWING_TICK + 3)
						EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
							REAPER_SHEET_CRESCENT,
							overflow + ReaperPictureX[args->side * 7 + 2],
							ReaperPictureY[2]);
					if(frm >= REAPER_SWING_TICK + 4 &&
						frm <= REAPER_SWING_TICK + 5)
						EFFECT_DRAW_PICTURE(draw[0], canvas,
							RamMapMetatileBuffer, RED_CRESCENT_FADE_A,
							overflow + ReaperPictureX[args->side * 7 + 3],
							ReaperPictureY[3]);
					if(frm >= REAPER_SWING_TICK + 6 &&
						frm <= REAPER_SWING_TICK + 7)
						EFFECT_DRAW_PICTURE(draw[0], canvas,
							RamMapMetatileBuffer, RED_CRESCENT_FADE_B,
							overflow + ReaperPictureX[args->side * 7 + 4],
							ReaperPictureY[4]);
					if(frm >= REAPER_SWING_TICK + 8 &&
						frm <= REAPER_SWING_TICK + 9)
						EFFECT_DRAW_PICTURE(draw[0], canvas,
							RamMapMetatileBuffer, RED_CRESCENT_FADE_C,
							overflow + ReaperPictureX[args->side * 7 + 5],
							ReaperPictureY[5]);
					if(frm >= REAPER_SWING_TICK + 10 &&
						frm <= REAPER_SWING_TICK + 11)
						EFFECT_DRAW_PICTURE(draw[0], canvas,
							RamMapMetatileBuffer, RED_CRESCENT_FADE_D,
							overflow + ReaperPictureX[args->side * 7 + 6],
							ReaperPictureY[6]);
				}
			}
		}
		btCameraShake(4, 4);
		EFFECT_END_TICK(state);
	}

	if(mode == CARD_BAFFLE)
		syCloseEvent(btEffectArmBg2AffineHBlankDMA);
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef RED_CRESCENT_BYTES
#undef CARD
#undef CARD_SHEEP
#undef CARD_DISSOLVE_STEPS
#undef CARD_FLIGHT_TICKS
#undef CARD_SQUARE_TICK
#undef CARD_SPIN
#undef CARD_SLANT
#undef CARD_SHOWN_TICKS
#undef SLEEP_CARD_SHOWN_TICKS
#undef CARD_DISSOLVE_TICKS
#undef BAFFLE_PUFF_TICKS
#undef SHEEP_TICKS
#undef SWORD_CARD_DAGGER_TICKS
#undef SWORD_CARD_PARTICLES
#undef SWORD_CARD_SHARDS
#undef SWORD_CARD_SHARD_TICKS
#undef SWORD_CARD_SHARD_TOP
#undef SWORD_CARD_SHARD_SPEED
#undef SWORD_CARD_SHARD_DIMMING
#undef REAPER_RISE_TICK
#undef REAPER_SWING_TICK
