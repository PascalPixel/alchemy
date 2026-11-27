/* @file */
/* The Sleep, Baffle, Death and Sword Cards. */
/* Part of battle_effect.c. */

/* The Death Card's RedCrescentSheetA, decoded at the start of the map cell */
/* buffer: its four pictures' bytes. */
#define RED_CRESCENT_BYTES                                                     \
	(RED_CRESCENT_FADE_D + PICTURE_BYTES(RED_CRESCENT_FADE_D))
/* The card's picture, CARD_WIDTH by CARD_HEIGHT, in the map cell buffer after
 */
/* the crescent's; SheepCardSheet's four sheep follow it. CARD_SHEEP reaches */
/* the same buffer by its fixed address. */
#define CARD (MapGridCellBuffer + RED_CRESCENT_BYTES)
#define CARD_SHEEP                                                             \
	(Ram_MapCellBuffer + RED_CRESCENT_BYTES + CARD_WIDTH * CARD_HEIGHT)
/* After the Death and Sword cards' pictures: the step of the dissolve, 0 to */
/* 15, at which each pixel clears, for eight rows repeated down the card. */
#define CARD_DISSOLVE_STEPS (CARD + CARD_WIDTH * CARD_HEIGHT)

/* The card flies to the targets for CARD_FLIGHT_FRAMES, turning back a */
/* CARD_SPIN a frame about its face and its width; it would stand square at */
/* CARD_SQUARE_FRAME, and arrives a sixteenth of a turn short of it. */
#define CARD_FLIGHT_FRAMES 48
#define CARD_SQUARE_FRAME  52
#define CARD_SPIN		   (TRIG_TURN / 64)
/* The card's turn about its upright axis, about 33 degrees. */
#define CARD_SLANT 6000
/* The frames the card is drawn: the Sleep Card's past the effect's end, the */
/* Sword Card's until its dagger strikes. */
#define CARD_SHOWN_FRAMES		80
#define SLEEP_CARD_SHOWN_FRAMES 120
/* The Death and Sword cards dissolve a step a frame from CARD_FLIGHT_FRAMES. */
#define CARD_DISSOLVE_FRAMES 16

/* The Death Card's Reaper rises from REAPER_RISE_FRAME and swings at */
/* REAPER_SWING_FRAME; two frames of flash, then the red crescent and its four
 */
/* fading pictures, two frames each. */
#define REAPER_RISE_FRAME  52
#define REAPER_SWING_FRAME 64

/* Each puff of the Baffle Card's mist lives BAFFLE_PUFF_FRAMES, through */
/* BlueCloudPuffSheet's four pictures. */
#define BAFFLE_PUFF_FRAMES 16
/* Each sheep tumbles out of the Sleep Card for SHEEP_FRAMES, through its four
 */
/* pictures every 16 frames. */
#define SHEEP_FRAMES 64

/* The Sword Card's dagger glints over the target for SWORD_CARD_DAGGER_FRAMES,
 */
/* then strikes and SWORD_CARD_PARTICLES fly, of which SWORD_CARD_SHARDS are */
/* drawn as shards for SWORD_CARD_SHARD_FRAMES, a copy darker every four frames
 */
/* after the first four. The shards start one a line from SWORD_CARD_SHARD_TOP,
 */
/* down the dagger, and the slowest flies SWORD_CARD_SHARD_SPEED, a pixel a */
/* frame in 64ths. */
#define SWORD_CARD_DAGGER_FRAMES 24
#define SWORD_CARD_PARTICLES	 64
#define SWORD_CARD_SHARDS		 32
#define SWORD_CARD_SHARD_FRAMES	 36
#define SWORD_CARD_SHARD_TOP	 48
#define SWORD_CARD_SHARD_SPEED	 64
/* Each darker copy of the stat reduction's shard pictures */
/* (STAT_REDUCTION_SHARD_PICTURES) is this many levels darker than the one */
/* before, but never blank. */
#define SWORD_CARD_SHARD_DIMMING 6

/* The four cards (Sleep, Baffle, Death and Sword Card): a card flies from */
/* the user to above the targets' middle, spinning upright and growing, and */
/* the background scrolls with it as far as it can. Then its picture comes to */
/* life: sheep tumble out of the Sleep Card; a mist pours from the Baffle Card
 */
/* and spreads over the ground while the screen waves; the Death and Sword */
/* cards dissolve, and the Reaper rises and swings his scythe in a red */
/* crescent, or a dagger glints, strikes and breaks into shards. The screen */
/* fades in the last eight frames. */
void btEffectRunCards(BATTLE_EFFECT_PARM *args, s32 mode) {
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	u8 *graphics;
	DRAW_RECTANGLE draw[2];
	FIXED scale;
	s32 scroll;
	s32 overflow;
	s32 totalFrames;
	s32 frame;
	SCR_POINT card;
	SCR_POINT middle;
	MODEL_TEXTURE texture;
	PARTICLE *step;
	RENDER_MODEL *model;
	void *vertices;
	s32 i;
	s32 j;
	s32 cardFrames;
	s32 first;
	s32 slot;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	graphics = Ram_HeapSlots->battleSheet;

	/* Set up a tiled canvas, filled on the user's side, and a blitter that
	 * keeps */
	/* the brighter pixel (that adds, for the Sword Card), mirrored to face the
	 */
	/* targets. */
	btEffectStartTiledCanvas(BGCNT_WRAP | BGCNT_PRIORITY(1));
	if(args->side == EFFECT_SIDE_ENEMIES)
		btEffectStartTiledCanvasFilled(BGCNT_WRAP | BGCNT_PRIORITY(1));
	else
		btEffectStartTiledCanvas(BGCNT_WRAP | BGCNT_PRIORITY(1));
	REG_BLDALPHA = BLEND_ALPHA(16, 2);
	if(mode == CARD_SWORD) {
		if(args->side == EFFECT_SIDE_ENEMIES)
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
								BLITTER_ADDITIVE);
		else
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
								EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
								BLITTER_ADDITIVE);
	}
	else {
		if(args->side == EFFECT_SIDE_ENEMIES)
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
								BLITTER_MAX);
		else
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
								EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
								BLITTER_MAX);
	}
	draw[0] = Ram_HeapSlots->ramCode;
	scale = 0;
	scroll = 0;
	overflow = 0;

	/* Load the card and what comes out of it; the Sword Card makes seven */
	/* darker copies of its shards for their fade. */
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
	if(mode == CARD_SLEEP) {
		btResourceLoadAndDecompress(RESOURCE_SHEEP_CARD_SHEET, CARD, 1, 1);
		btResourceLoadAndDecompress(RESOURCE_BLUE_CLOUD_PUFF_SHEET,
									state->sheet, 0, 0);
		totalFrames = 96;
	}
	else if(mode == CARD_BAFFLE) {
		btResourceLoadAndDecompress(RESOURCE_BUTTERFLY_CARD_SHEET, CARD, 1, 1);
		btResourceLoadAndDecompress(RESOURCE_BLUE_CLOUD_PUFF_SHEET,
									state->sheet, 0, 0);
		totalFrames = 80;
	}
	else if(mode == CARD_SWORD) {
		btResourceLoadAndDecompress(RESOURCE_SWORD_CARD_SHEET, CARD, 1, 1);
		btResourceLoadAndDecompress(RESOURCE_DAGGER_SHEET, state->sheet, 1, 0);
		btResourceLoadAndDecompress(
			RESOURCE_PALETTE_RAMP_IMAGE,
			state->sheet + STAT_REDUCTION_SHARD_PICTURES, 0, 0);
		for(i = 1; i != DIMMED_COPIES; i++) {
			for(j = 0; j != STAT_REDUCTION_SHARD_BYTES; j++) {
				s32 shade = state->sheet[STAT_REDUCTION_SHARD_PICTURES + j];

				if(shade > 0) {
					shade -= i * SWORD_CARD_SHARD_DIMMING;
					if(shade <= 0)
						shade = 1;
				}
				state->sheet[STAT_REDUCTION_SHARD_PICTURES +
							 i * STAT_REDUCTION_SHARD_BYTES + j] = shade;
			}
		}
		totalFrames = 116;
	}
	else {
		btResourceLoadAndDecompress(RESOURCE_SKELETON_CARD_SHEET, CARD, 1, 1);
		btResourceLoadAndDecompress(RESOURCE_COUNTER_REVEAL_SHEET, state->sheet,
									1, 0);
		btResourceLoadAndDecompress(RESOURCE_RED_CRESCENT_SHEET_A,
									Ram_MapCellBuffer, 1, 0);
		totalFrames = 88;
	}
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
	if(mode == CARD_BAFFLE)
		sySetEvent(btEffectArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
	REG_BG2PA = Q_8_8(1);

	for(frame = 0; frame != totalFrames; frame++) {
		/* Frame 0: find the user and the targets' middle (the Death Card's 32
		 */
		/* pixels toward the user), and scroll the background to the user. */
		if(frame == 0) {
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

		/* The last eight frames: the screen fades. */
		if(frame >= totalFrames - 8) {
			s32 fade = frame - totalFrames + 8;

			REG_BLDALPHA = ((fade + 8) << 8) | (16 - fade * 2);
		}
		if(frame == CARD_FLIGHT_FRAMES && mode == CARD_BAFFLE)
			sySndPlayStub(SE_ITEM_BREAK);

		/* The card flies to the middle, rising and growing; the background */
		/* follows it as far as it can scroll. */
		if(frame < CARD_FLIGHT_FRAMES) {
			s32 t = frame * (middle.x - card.x);
			s32 q;
			s32 x;

			first = card.x;
			q = t / CARD_FLIGHT_FRAMES;
			first -= CANVAS_SIZE / 2;
			x = first + q;

			if(x < 0) {
				overflow = x;
				x = 0;
			}
			else if(x > DISPLAY_WIDTH - CANVAS_SIZE) {
				overflow = x - (DISPLAY_WIDTH - CANVAS_SIZE);
				x = DISPLAY_WIDTH - CANVAS_SIZE;
			}
			x <<= 8;
			REG_BG2X = -x;
			scroll = -x;
			card.y--;
			scale += FIXED_ONE / 64;
			if(scale > FIXED_ONE / 2)
				scale = FIXED_ONE / 2;
		}

		if(mode != CARD_SLEEP) {
			if(mode == CARD_BAFFLE) {
				/* The screen waves, line by line. */
				s32 *line = state->bg2xLines;

				for(i = 0; i != DISPLAY_HEIGHT; i++)
					*line++ = scroll -
							  CANVAS_WAVE_BG2X(sySin(frame * (TRIG_TURN / 16) +
													 i * (TRIG_TURN / 16)) *
											   4);
			}
			else {
				/* The canvas's colours step to the smoke's, then to the
				 * Reaper's or */
				/* the dagger's; the card dissolves in 16 random steps. */
				u8 *steps = CARD_DISSOLVE_STEPS;

				if((mode == CARD_DEATH || mode == CARD_SWORD) &&
				   (frame >= CARD_FLIGHT_FRAMES - 8 &&
					frame < CARD_FLIGHT_FRAMES))
					btEffectStepPaletteToResource(RESOURCE_SMOKE_SHEET);
				if(frame >= REAPER_RISE_FRAME) {
					if(mode == CARD_DEATH)
						btEffectStepPaletteToResource(
							RESOURCE_RED_CRESCENT_SHEET_A);
					else
						btEffectStepPaletteToResource(RESOURCE_DAGGER_SHEET);
				}
				if(frame == CARD_FLIGHT_FRAMES) {
					for(i = 0; i != 8 * CARD_WIDTH; i++)
						steps[i] = syRandom() & (CARD_DISSOLVE_FRAMES - 1);
				}
				if(frame >= CARD_FLIGHT_FRAMES &&
				   frame < CARD_FLIGHT_FRAMES + CARD_DISSOLVE_FRAMES) {
					u8 *card = CARD;

					for(i = 0; i != CARD_HEIGHT; i++) {
						/* first, unused after the flight, holds the cleared
						 * pixel. */
						for(j = 0; j != CARD_WIDTH; j++) {
							/* FAKEMATCH(dead): first cleared inside the test,
							 * or'd into the step; as first = 0 at the top of
							 * the outer body. */
							if(steps[(i & 7) * CARD_WIDTH + j] ==
							   ((frame - CARD_FLIGHT_FRAMES) | (first = 0)))
								card[i * CARD_WIDTH + j] = first;
						}
					}
				}
			}
		}

		/* The card, until its picture takes over. */
		if(mode == CARD_SLEEP)
			cardFrames = SLEEP_CARD_SHOWN_FRAMES;
		else
			cardFrames = mode == CARD_SWORD
							 ? CARD_FLIGHT_FRAMES + SWORD_CARD_DAGGER_FRAMES
							 : CARD_SHOWN_FRAMES;
		if(frame < cardFrames) {
			vertices = syHeapAllocEwram(32);
			model = btRenderAllocateModelList(1);
			texture.logWidth = 5;
			texture.logHeight = 6;
			model->vertices = vertices;
			texture.pixels = CARD;
			model->kind = MODEL_TEXTURED;
			model->texture = &texture;
			model->mesh = BattleEffect_TallMesh;
			syInitMatrix();
			syScale(FIXED_ONE, FIXED_ONE, FIXED_ONE);
			syTranslate(INTtoFIXED(overflow), INTtoFIXED(card.y) - toFIXED(64),
						0);
			if(frame < CARD_FLIGHT_FRAMES) {
				if(args->side == EFFECT_SIDE_ENEMIES)
					syRotZ((frame - CARD_SQUARE_FRAME) * CARD_SPIN);
				else
					syRotZ((CARD_SQUARE_FRAME - frame) * CARD_SPIN);
				syRotX((CARD_SQUARE_FRAME - frame) * CARD_SPIN);
			}
			else {
				if(args->side == EFFECT_SIDE_ENEMIES)
					syRotZ(-TRIG_TURN / 16);
				else
					syRotZ(TRIG_TURN / 16);
				syRotX(TRIG_TURN / 16);
			}
			syRotY(CARD_SLANT);
			syMatrixScaleUniform(scale);
			syScale(FIXED_ONE, FIXED_ONE * 2, FIXED_ONE);
			btRenderTransformVertices(BattleEffect_SquareCorners, vertices, 4);
			btRenderDrawModelList(model);
			syHeapRelease(model);
			syHeapRelease(vertices);
		}

		if(mode == CARD_BAFFLE) {
			/* The mist pours out, two puffs a frame for 32 frames; each falls
			 * to */
			/* the ground and spreads along it. */
			if(frame == CARD_FLIGHT_FRAMES) {
				EFFECT_SHOW_DAMAGE(SE_WHOOSH);
				EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
									CANVAS_FADE_HALF);
				REG_BLDALPHA = EFFECT_BLEND_ADD;
			}
			if(frame >= CARD_FLIGHT_FRAMES && frame < CARD_FLIGHT_FRAMES + 32) {
				s32 time = frame - CARD_FLIGHT_FRAMES;

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
				if(step->variant >= 0 && step->variant < BAFFLE_PUFF_FRAMES) {
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
			/* A sheep a frame for 32 frames tumbles out of the card. */
			if(frame == CARD_FLIGHT_FRAMES)
				sySndPlayStub(SE_POWER_UP);
			if(frame == 64)
				EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
			if(frame >= CARD_FLIGHT_FRAMES && frame < CARD_FLIGHT_FRAMES + 32) {
				PARTICLE *sheep;
				u32 mask = 127;

				slot = frame - CARD_FLIGHT_FRAMES;
				sheep = &state->particles[slot];
				sheep->x = toFIXED(64);
				sheep->y = toFIXED(48);
				sheep->velX = ((syRandom() & mask) - 64) << 11;
				sheep->velY = ((syRandom() & mask) - 64) << 11;
				sheep->variant = mode;
			}
			for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
				step = &state->particles[i];
				if(step->variant >= 0 && step->variant < SHEEP_FRAMES) {
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

		if(mode == CARD_SWORD && frame >= CARD_FLIGHT_FRAMES) {
			s32 time = frame - CARD_FLIGHT_FRAMES;

			/* The dagger glints. */
			if(time < SWORD_CARD_DAGGER_FRAMES)
				draw[0](canvas,
						state->sheet + time / 4 % 6 * PICTURE_BYTES(DAGGER),
						overflow + 52, 44, DAGGER_WIDTH, DAGGER_HEIGHT);

			/* It strikes: the screen flashes and the shards burst. */
			if(time == SWORD_CARD_DAGGER_FRAMES) {
				sySndPlayStub(SE_POWER_UP);
				EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
				state->shakeFrm = 4;
				syWordsFillEntry(canvas, CANVAS_BYTES,
								 CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
				btEffectTintBattler(args->tgts[0], SPRITE_COLOR_WHITE,
									TINT_KEEP, 0, 16);
				for(i = 0; i != SWORD_CARD_PARTICLES; i++) {
					s32 angle = syRandom() & TRIG_ANGLE_MASK;
					s32 force = 127;

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
				EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
									CANVAS_FADE_HALF);
				REG_BLDALPHA = EFFECT_BLEND_ADD;
			}

			/* The shards tumble through three pictures each and darken. */
			if(time >= SWORD_CARD_DAGGER_FRAMES &&
			   time <= SWORD_CARD_DAGGER_FRAMES + SWORD_CARD_SHARD_FRAMES) {
				s32 fade = 0;

				if(time > SWORD_CARD_DAGGER_FRAMES + 4) {
					fade = (time - SWORD_CARD_DAGGER_FRAMES) / 4;
					if(fade > DIMMED_COPIES - 1)
						fade = DIMMED_COPIES - 1;
				}
				for(i = 0; i != SWORD_CARD_SHARDS; i++) {
					s32 picture;

					step = &state->particles[i];
					picture = i % 4 * 3 + (step->variant + time) / 8 % 3;
					draw[0](canvas,
							state->sheet + STAT_REDUCTION_SHARD_PICTURES +
								(fade * STAT_REDUCTION_SHARD_BYTES +
								 StatChange_ShardPictures[picture]),
							overflow + FIXED_INTEGER_PART(step->x) -
								StatChange_ShardWidths[picture] / 2,
							FIXED_INTEGER_PART(step->y) -
								StatChange_ShardHeights[picture] / 2,
							StatChange_ShardWidths[picture],
							StatChange_ShardHeights[picture]);
					btParticleAdvanceWithGravity2D(step, 60, 0);
				}
			}
		}

		if(mode == CARD_DEATH) {
			/* The Reaper rises and swings: his scythe cuts a red crescent that
			 */
			/* fades in four pictures. */
			if(frame == REAPER_SWING_FRAME - 4)
				sySndPlayStub(SE_POWER_UP);
			if(frame == REAPER_SWING_FRAME) {
				sySndPlayStub(SE_CLATTER);
				EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
				btEffectStartActorMotion(args->tgts[0], 1, toFIXED(2),
										 toFIXED(8), toFIXED(2), 110);
				btEffectTintBattler(args->tgts[0], SPRITE_COLOR_WHITE,
									POSE_DAMAGE, 0, 8);
				state->shakeFrm = 4;
			}
			if(frame >= REAPER_RISE_FRAME) {
				if(frame < REAPER_SWING_FRAME) {
					draw[0](canvas, state->sheet,
							overflow + Reaper_PictureX[args->side * 7],
							Reaper_PictureY[0], REAPER_SHEET_RAISED_WIDTH,
							REAPER_SHEET_RAISED_HEIGHT);
				}
				else {
					if(frame <= REAPER_SWING_FRAME + 3)
						draw[0](canvas, state->sheet,
								overflow + Reaper_PictureX[args->side * 7],
								Reaper_PictureY[0], REAPER_SHEET_RAISED_WIDTH,
								REAPER_SHEET_RAISED_HEIGHT);
					EFFECT_DRAW_PICTURE(
						draw[0], canvas, state->sheet, REAPER_SHEET_SWUNG,
						overflow + Reaper_PictureX[args->side * 7 + 1],
						Reaper_PictureY[1]);
					if(frame >= REAPER_SWING_FRAME &&
					   frame <= REAPER_SWING_FRAME + 1)
						syWordsFillEntry(
							canvas, CANVAS_BYTES,
							CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
					if(frame >= REAPER_SWING_FRAME + 2 &&
					   frame <= REAPER_SWING_FRAME + 3)
						EFFECT_DRAW_PICTURE(
							draw[0], canvas, state->sheet,
							REAPER_SHEET_CRESCENT,
							overflow + Reaper_PictureX[args->side * 7 + 2],
							Reaper_PictureY[2]);
					if(frame >= REAPER_SWING_FRAME + 4 &&
					   frame <= REAPER_SWING_FRAME + 5)
						EFFECT_DRAW_PICTURE(
							draw[0], canvas, Ram_MapCellBuffer,
							RED_CRESCENT_FADE_A,
							overflow + Reaper_PictureX[args->side * 7 + 3],
							Reaper_PictureY[3]);
					if(frame >= REAPER_SWING_FRAME + 6 &&
					   frame <= REAPER_SWING_FRAME + 7)
						EFFECT_DRAW_PICTURE(
							draw[0], canvas, Ram_MapCellBuffer,
							RED_CRESCENT_FADE_B,
							overflow + Reaper_PictureX[args->side * 7 + 4],
							Reaper_PictureY[4]);
					if(frame >= REAPER_SWING_FRAME + 8 &&
					   frame <= REAPER_SWING_FRAME + 9)
						EFFECT_DRAW_PICTURE(
							draw[0], canvas, Ram_MapCellBuffer,
							RED_CRESCENT_FADE_C,
							overflow + Reaper_PictureX[args->side * 7 + 5],
							Reaper_PictureY[5]);
					if(frame >= REAPER_SWING_FRAME + 10 &&
					   frame <= REAPER_SWING_FRAME + 11)
						EFFECT_DRAW_PICTURE(
							draw[0], canvas, Ram_MapCellBuffer,
							RED_CRESCENT_FADE_D,
							overflow + Reaper_PictureX[args->side * 7 + 6],
							Reaper_PictureY[6]);
				}
			}
		}
		btCameraShake(4, 4);
		EFFECT_END_FRAME(state);
	}

	if(mode == CARD_BAFFLE)
		syCloseEvent(btEffectArmBg2AffineHBlankDma);
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}
#undef RED_CRESCENT_BYTES
#undef CARD
#undef CARD_SHEEP
#undef CARD_DISSOLVE_STEPS
#undef CARD_FLIGHT_FRAMES
#undef CARD_SQUARE_FRAME
#undef CARD_SPIN
#undef CARD_SLANT
#undef CARD_SHOWN_FRAMES
#undef SLEEP_CARD_SHOWN_FRAMES
#undef CARD_DISSOLVE_FRAMES
#undef BAFFLE_PUFF_FRAMES
#undef SHEEP_FRAMES
#undef SWORD_CARD_DAGGER_FRAMES
#undef SWORD_CARD_PARTICLES
#undef SWORD_CARD_SHARDS
#undef SWORD_CARD_SHARD_FRAMES
#undef SWORD_CARD_SHARD_TOP
#undef SWORD_CARD_SHARD_SPEED
#undef SWORD_CARD_SHARD_DIMMING
#undef REAPER_RISE_FRAME
#undef REAPER_SWING_FRAME
