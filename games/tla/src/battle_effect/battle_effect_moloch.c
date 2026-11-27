/* @file */
/* The Moloch and Iris summons. */
/* Part of battle_effect.c. */

struct RENDER_MODEL;

/* Moloch's frames, and the frame A or B skips to. */
#define MOLOCH_FRAMES  296
#define MOLOCH_SKIP_TO 232
/* The motes, in the map cell buffer; the flame columns, four pixels apart */
/* from MOLOCH_FLAME_LEFT (MOLOCH_FLAMES_SET_UP are set up, the first */
/* MOLOCH_FLAMES drawn); the embers, from state->particles[MOLOCH_FIRST_EMBER];
 */
/* and the summoned sprites. */
#define MOLOCH_MOTES		 256
#define MOLOCH_FLAMES		 12
#define MOLOCH_FLAMES_SET_UP 16
#define MOLOCH_FLAME_LEFT	 44
#define MOLOCH_FIRST_EMBER	 24
#define MOLOCH_EMBERS		 16
#define MOLOCH_SPRITES		 12
/* A flame or ember lives four frames on each of FlashBurstSheet's six */
/* pictures; the ring texture follows them. */
#define MOLOCH_FLAME_LIFE	(6 * 4)
#define MOLOCH_RING_TEXTURE (6 * PICTURE_BYTES(FLASH_BURST))
/* The motes fall to the ground here; each falls a quarter pixel a frame for */
/* every pixel of its size. */
#define MOLOCH_GROUND_Y 114
/* At this frame the flame columns part and the rings start to spread. */
#define MOLOCH_SPREAD_FRAME 136
/* How far the rings lean back. */
#define MOLOCH_RING_TILT 3000
/* The level the canvas is cleared to each frame as the screen brightens to */
/* white, before its last two frames at the brightest. */
#define MOLOCH_WHITENING_LEVEL 16

/* Moloch. A shower of 256 motes drifts down the screen and, from frame */
/* 91, gathers toward the middle; from frame 97 flames rise in twelve columns */
/* and from frame 136 four textured rings spread out from the summoned */
/* objects. At frames 212 to 268 the targets are struck in turn, and the */
/* scene closes with a glow of the whole palette. A or B skips to frame 232. */
/* */
/* The state's particles 0-11 are the flames (velocityZ picks the blitter, 2 */
/* retires the column), particles 24-39 the embers of the closing glow; the */
/* map cell buffer holds the motes (variant counts down the frames before a */
/* mote appears, 0 is a live mote). */
void btEffectRunMoloch(BATTLE_EFFECT_PARM *args) {
	DRAW_RECTANGLE draw[2];
	MODEL_TEXTURE texture;
	void *canvas;
	BATTLE_EFFECT_STATE *state;
	u8 *graphics;
	PARTICLE *mote;
	PARTICLE *flame;
	PARTICLE *motes;
	RENDER_MODEL *model;
	void *projected;
	s32 i;
	s32 k;

	canvas = Ram_HeapSlots->battleCanvas;
	state = Ram_HeapSlots->battleEff;
	graphics = Ram_HeapSlots->battleSheet;
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btEffectBuildCanvasBlitters(0, draw);
	btResourceLoadAndDecompress(RESOURCE_FLASH_BURST_SHEET, state->sheet, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_FLAME_BAND_TEXTURE,
								state->sheet + MOLOCH_RING_TEXTURE, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, graphics, 0, 0);
	btGraphicsLoadGreyRamp();
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	{
		POINT2_D focus;
		s32 frame;

		focus.x = toFIXED(128);
		focus.y = toFIXED(64);
		btEffectShowLivingTargets(state->args);
		sySynch(1);
		btEffectCreateSprs(MOLOCH_SPRITES, SPRITE_MOLOCH, 2);

		frame = 0;
		do {
			if((InputWork.repeat & (KEY_A | KEY_B)) && frame < MOLOCH_SKIP_TO &&
			   frame > 15) {
				sySndPlayStub(SE_CLATTER);
				frame = MOLOCH_SKIP_TO;
			}

			if(frame == 0) {
				EFFECT_FREE_PARTICLES(i, state->particles, 64);

				/* The motes start along the top, each after its own delay. */
				motes = MapGridCellBuffer;
				for(i = 0; i != MOLOCH_MOTES; i++) {
					mote = &motes[i];
					syRandom(); /* a number drawn and not used */
					mote->variant = i;
					mote->x = INTtoFIXED((syRandom() & 255) - 64);
					mote->y = 0;
					mote->velX = 0;
					mote->velY = 0;
				}

				/* The flame columns rise from y = 72. */
				for(i = 0; i != MOLOCH_FLAMES_SET_UP; i++) {
					flame = &state->particles[i];
					flame->x = INTtoFIXED(i * 3 + i + MOLOCH_FLAME_LEFT);
					flame->y = toFIXED(72);
					flame->velX = 0;
					flame->velY = INTtoFIXED(-((syRandom() & 3) + 2));
					flame->velZ = 0;
					flame->variant = -i - (syRandom() & 15);
				}

				/* The embers of the closing glow start at (54, 56). */
				for(i = 0; i != MOLOCH_EMBERS; i++) {
					flame = &state->particles[MOLOCH_FIRST_EMBER + i];
					flame->x = toFIXED(54);
					flame->y = toFIXED(56);
					flame->variant = i * -2;
					flame->velX = -(INTtoFIXED(syRandom() & 3) + toFIXED(2));
					flame->velY = -(syRandom() & 3) << 15;
				}
			}

			if(frame <= 271) {
				motes = MapGridCellBuffer;
				for(i = 0; i != MOLOCH_MOTES; i++) {
					mote = &motes[i];
					if(mote->variant == 0) {
						s32 size = Moloch_MoteSizes[i & 7];

						draw[0](canvas,
								graphics + BattleEffect_MotePictures[size - 1],
								FIXED_INTEGER_PART(mote->x) - (size >> 1),
								FIXED_INTEGER_PART(mote->y) - size, size,
								size * 2);

						/* Frames 91-119: motes still at rest head for a random
						 * point of */
						/* the middle, timed to arrive as they reach the ground.
						 */
						if((u32)(frame - 91) <= 28 && mote->velX == 0 &&
						   FIXED_INTEGER_PART(mote->y) < MOLOCH_GROUND_Y) {
							s32 dx;

							mote->velX =
								INTtoFIXED(syRandom() & 63) + toFIXED(56);
							dx = mote->velX - mote->x;
							mote->velX =
								dx / ((toFIXED(MOLOCH_GROUND_Y) - mote->y) /
									  (size * (toFIXED(1) / 4)));
						}

						/* Frames 142-161: they swirl about the centre line. */
						if((u32)(frame - 142) <= 19) {
							FIXED distance = mote->x - toFIXED(96);
							FIXED pull;

							if(distance < 0)
								distance = -distance;
							pull = (mote->y / 2 - distance +
									(s32)INTtoFIXED(syRandom() & 31) +
									toFIXED(96)) /
								   32;
							if(mote->x < toFIXED(96) && pull > 0)
								pull = -pull;
							if(mote->x > toFIXED(96) && pull < 0)
								pull = -pull;
							mote->velX += pull / 8;
						}

						mote->x += mote->velX + sySin(mote->y >> 5) / 5;
						mote->y += mote->velY + size * (toFIXED(1) / 4);
						if(mote->y > toFIXED(MOLOCH_GROUND_Y)) {
							if(frame > 215) {
								/* Late motes fly back in from the right. */
								mote->variant = 0;
								mote->x = toFIXED(120);
								mote->y = INTtoFIXED((syRandom() & 127) - 32);
								mote->velX = INTtoFIXED((syRandom() & 3) - 8);
								mote->velY = 0;
							}
							else {
								mote->variant = syRandom() & 255;
								mote->x = INTtoFIXED((syRandom() & 255) - 64);
								mote->y = -toFIXED(8);
								mote->velX = 0;
								mote->velY = 0;
							}
						}
					}
					else {
						mote->variant--;
					}

					/* Frames 217-245: the wind turns. */
					if((u32)(frame - 217) <= 28) {
						mote->velX -= toFIXED(1) / 8;
						mote->velY += toFIXED(1) / 8;
					}
				}
			}

			/* Frames 91-255: the flame columns, a 32x64 cell for every four
			 * frames. */
			if((u32)(frame - 91) <= 164) {
				for(i = 0; i != MOLOCH_FLAMES; i++) {
					flame = &state->particles[i];
					if(flame->velZ != 2) {
						if((u32)flame->variant < MOLOCH_FLAME_LIFE) {
							draw[flame->velZ](
								canvas,
								state->sheet + (flame->variant >> 2) *
												   PICTURE_BYTES(FLASH_BURST),
								FIXED_INTEGER_PART(flame->x),
								FIXED_INTEGER_PART(flame->y), FLASH_BURST_WIDTH,
								FLASH_BURST_HEIGHT);
							flame->x += flame->velX;
							flame->y += flame->velY;
						}
						if(++flame->variant == MOLOCH_FLAME_LIFE) {
							flame->x =
								INTtoFIXED(i * 3 + i + MOLOCH_FLAME_LEFT);
							flame->y = toFIXED(72);
							flame->variant = 0;
							flame->velY = INTtoFIXED(-((syRandom() & 3) + 2));
							flame->velZ = 0;
							if((u32)(frame - 97) <= 42)
								flame->velZ = 1;
							else if(frame > 140)
								flame->velZ = 2;
						}
						if(frame == MOLOCH_SPREAD_FRAME) {
							if(flame->x < toFIXED(80))
								flame->velX -= toFIXED(4);
							else
								flame->velX += toFIXED(4);
						}
					}
				}
			}

			/* Frames 201-271: the embers of the closing glow. */
			if((u32)(frame - 200) <= 71) {
				if(frame > 200) {
					for(i = 0; i != MOLOCH_EMBERS; i++) {
						flame = &state->particles[MOLOCH_FIRST_EMBER + i];
						if((u32)flame->variant < MOLOCH_FLAME_LIFE) {
							draw[0](canvas,
									state->sheet +
										(flame->variant >> 2) *
											PICTURE_BYTES(FLASH_BURST),
									FIXED_INTEGER_PART(flame->x),
									FIXED_INTEGER_PART(flame->y),
									FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
							flame->x += flame->velX;
							flame->y += flame->velY;
						}
						if(++flame->variant == MOLOCH_FLAME_LIFE) {
							flame->x = toFIXED(54);
							flame->y = toFIXED(56);
							flame->velX =
								-(INTtoFIXED(syRandom() & 3) + toFIXED(2));
							flame->variant = 0;
						}
					}
				}
			}

			/* Four rings, each starting at its own frame, grow and fade out
			 * over */
			/* eight frames. */
			if(frame >= MOLOCH_SPREAD_FRAME) {
				/* Room for 128 projected corners; a ring has 32. */
				projected = syHeapAllocEwram(128 * sizeof(PROJECTED_VERTEX));
				model = btRenderAllocateModelList(1);
				texture.logWidth = 7;
				texture.logHeight = 5;
				texture.pixels = state->sheet + MOLOCH_RING_TEXTURE;
				model->mesh = BattleEffect_CylinderMesh;
				model->kind = MODEL_SHADED_ADDITIVE;
				model->texture = &texture;
				model->vertices = projected;
				model->scrollU = 0;
				model->scrollV = 0;
				for(i = 0; i != ARRAY_COUNT(Moloch_RingStarts); i++) {
					s32 start = Moloch_RingStarts[i] + MOLOCH_SPREAD_FRAME;
					FIXED scale;
					s32 shade;

					if(frame <= start)
						continue;
					scale = (frame - start) * Moloch_RingGrowth[i] * 2000 +
							FIXED_ONE / 2;
					shade = (start - frame) * 8 + 56;
					if(shade > 0)
						shade = 0;
					if(shade <= MODEL_SHADE_INVISIBLE)
						continue;
					model->shade = shade;
					syInitMatrix();
					syTranslate(toFIXED(16), INTtoFIXED(Moloch_RingHeights[i]),
								0);
					syScale(scale, scale, scale);
					syRotX(MOLOCH_RING_TILT);
					model->scrollU = ((frame + i * 5) & 7) << 4;
					btRenderTransformVertices(BattleEffect_CylinderCorners,
											  projected, 32);
					btRenderDrawModelList(model);
				}
				syHeapRelease(model);
				syHeapRelease(projected);
			}

			if(frame > 139)
				btEffectPlaceFormationSprs(SUMMON_FORMATION_MOLOCH, focus.x,
										   focus.y);

			/* Frames 212-268, every eight: the targets flash and are thrown
			 * back, */
			/* each strike's reach 16 percent further than the last's. */
			for(i = 0; i != 8; i++) {
				if(frame == i * 8 + 212) {
					for(k = 0; k != state->args->tgtCnt; k++) {
						btEffectTintBattler(state->args->tgts[k],
											SPRITE_COLOR_WHITE, POSE_DAMAGE, k,
											8);
						btEffectStartActorMotion(args->tgts[k], TRUE,
												 toFIXED(1), toFIXED(4),
												 toFIXED(1), 100 + i * 16);
					}
				}
			}
			if((u32)(frame - 201) <= 54)
				state->shakeFrm = 2;

			if(frame == 32)
				sySndPlayStub(SE_GLITTER);
			if(frame == 90)
				sySndPlayStub(SE_CLOUD_BILLOWS);
			if(frame == 140)
				sySndPlayStub(SE_CAST);
			if(frame == 200)
				sySndPlayStub(SE_BANE_ESCAPE);
			if(frame == 232)
				sySndPlayStub(SE_CLATTER);

			/* Frames 256-271 brighten the whole screen to white; at 272 the
			 * battle */
			/* phase resumes and the canvas fades back. */
			if(frame == 272) {
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
				EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_DARKEN,
									CANVAS_FILL_WORD(2));
				btGraphicsLoadGreyRamp();
			}
			else if((u32)(frame - 256) <= 15) {
				EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR,
									CANVAS_FILL_WORD(MOLOCH_WHITENING_LEVEL));
				if(frame > 269)
					state->transferValue =
						CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST);
				btPaletteBrightenBgEntries(2, 2, 2);
			}

			btCameraShake(4, 8);
			btEffectCountDownBattlerTints();
			frame++;
			EFFECT_PRESENT_CANVAS(state);
		} while(frame != MOLOCH_FRAMES);

		syCloseEvent(btEffectTransferCanvas);
		EFFECT_RELEASE_BLITTERS();
		btEffectRunImpactBurst(SUMMON_FORMATION_MOLOCH, focus.x, focus.y);
		for(i = 0; i != MOLOCH_SPRITES; i++)
			fdSprDestroyStub(state->sprs[i]);
	}
	btEffectEndCanvasLayer();
}

/* Iris's change of backdrop, made behind the wipe: each target is set over */
/* the middle, a little above the ground and halfway nearer, no actors stand */
/* on the field, and the violet sky is loaded with its 128 colours (BG palette
 */
/* entries 96 on) darkened and their channels turned round (red to green, */
/* green to blue, blue to red). */
void btIrisChangeBackdrop(void) {
	BATTLE_BACKGROUND_STATE *transition = Ram_HeapSlots->battleBg;
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;
	s16 list[UNIT_MAX];
	u16 *palette;
	s32 i;

	REG_BG1HOFS = 0;
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;

	/* The targets fall in from just above the ground, halfway closer in depth.
	 */
	for(i = 0; i != state->args->tgtCnt; i++) {
		MOTION_OBJECT *object = btObjectGetStub(state->args->tgts[i])->obj;

		object->x = 0;
		object->y = INTtoFIXED(16 - (syRandom() & 15));
		object->z /= 2;
		object->verticalMotionStrength = 0;
	}

	/* No actor objects over the new backdrop. */
	list[0] = BATTLE_UNIT_LIST_END;
	btActorSpawnObjectsForListStub(list, 0);
	btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, RESOURCE_VIOLET_SKY_BACKDROP,
						 0);

	palette = (u16 *)&BG_PLTT_COLOR(6, 0);
	for(i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
		s32 color = *palette;
		s32 blue = RGB_BLUE((u16)color);
		s32 green = RGB_GREEN((u16)color);
		s32 red = RGB_LEVEL_MAX;

		red &= color;
		red -= 16;
		green -= 24;
		blue -= 20;

		if(red < 0)
			red = 0;
		if(green < 0)
			green = 0;
		if(blue < 0)
			blue = 0;
		*palette++ = RGB(blue, red, green);
	}
	transition->busy = 1;
	BgScrollWork[1].x = 0;
}

/* Iris's sprite sheet, state->sheet: the mote set and its fading copies, then
 */
/* the drops' pictures (eight of each size, each a shaded circle mirrored and */
/* dimmer than the last), later overwritten by the strips' and the glow's */
/* 32x64 textures. */
#define IRIS_MOTE_FADES		   8
#define IRIS_DROPS			   6930
#define IRIS_DROP_WIDTH		   20
#define IRIS_DROP_HEIGHT	   40
#define IRIS_SMALL_DROPS	   19474
#define IRIS_SMALL_DROP_WIDTH  12
#define IRIS_SMALL_DROP_HEIGHT 24
#define IRIS_DROP_PICTURES	   8
#define IRIS_TEXTURES		   (IRIS_MOTE_FADES * MOTE_SET_BYTES)
#define IRIS_TEXTURE_WIDTH	   32
#define IRIS_TEXTURE_HEIGHT	   64
#define IRIS_TEXTURE_BYTES	   PICTURE_BYTES(IRIS_TEXTURE)
/* The pillars' 16x16 texture follows the first mote set; its shaded circle */
/* is drawn first at IRIS_PILLAR_CIRCLE and mirrored into it. */
#define IRIS_PILLAR_TEXTURE		 MOTE_SET_BYTES
#define IRIS_PILLAR_TEXTURE_SIZE 16
#define IRIS_PILLAR_CIRCLE		 1000

/* The splashes' pictures, in the particle sprite sheet */
/* (Ram_HeapSlots->battleSheet): IRIS_RING_FADES fades each of a large and a */
/* small ring mote, IRIS_DOT_FADES of a dot and the spray's column. */
#define IRIS_RING_FADES 3
#define IRIS_DOT_FADES	8
/* How much dimmer each fade of a ring mote and of a dot is than the last. */
#define IRIS_RING_FADE_STEP	   16
#define IRIS_DOT_FADE_STEP	   8
#define IRIS_LARGE_RING_WIDTH  7
#define IRIS_LARGE_RING_HEIGHT 14
#define IRIS_SMALL_RING_WIDTH  5
#define IRIS_SMALL_RING_HEIGHT 10
#define IRIS_DOT_WIDTH		   2
#define IRIS_DOT_HEIGHT		   4
#define IRIS_SPRAY_WIDTH	   8
#define IRIS_SPRAY_HEIGHT	   32
/* Where each run of pictures starts in IrisSplashSheet, in bytes. */
typedef enum IRIS_SPLASH_PICTURE {
	IRIS_LARGE_RINGS = 0,
	IRIS_SMALL_RINGS =
		IRIS_LARGE_RINGS + IRIS_RING_FADES * PICTURE_BYTES(IRIS_LARGE_RING),
	IRIS_DOTS =
		IRIS_SMALL_RINGS + IRIS_RING_FADES * PICTURE_BYTES(IRIS_SMALL_RING),
	IRIS_SPRAY = IRIS_DOTS + IRIS_DOT_FADES * PICTURE_BYTES(IRIS_DOT)
} IRIS_SPLASH_PICTURE;
/* A splash's rings of dots are drawn at this many points around; its ring */
/* motes turn about it as spokes, taking their next picture every */
/* IRIS_RING_FADE_FRAMES. */
#define IRIS_SPLASH_POINTS	  64
#define IRIS_SPLASH_SPOKES	  8
#define IRIS_RING_FADE_FRAMES 12
/* An angle of n 64ths of a turn (TRIG_TURN / 64 each): a splash's ring has */
/* IRIS_SPLASH_POINTS points about a turn. */
#define IRIS_TURN_64THS(n) ((n) << 10)

/* Iris: draws one falling drop at (x, y), large (20x40) when small is 0 */
/* and small (12x24) otherwise, its animation picture chosen by frame / 2, and
 */
/* under it a trail of particles every half size down to row 112. */
void btIrisDrawDrop(u32 frame, s32 x, s32 y, s32 small) {
	DRAW_RECTANGLE draw[2];
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	u32 size;
	u32 half;
	s32 i;
	s32 trailY;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	draw[0] = Ram_HeapSlots->ramCode;
	draw[1] = Ram_HeapSlots->ramCodePaired;

	/* The drop: one picture of its eight, large or small. */
	if(small == 0)
		draw[1](canvas,
				state->sheet + IRIS_DROPS +
					frame / 2 * PICTURE_BYTES(IRIS_DROP),
				x - IRIS_DROP_WIDTH / 2, y - IRIS_DROP_HEIGHT / 2,
				IRIS_DROP_WIDTH, IRIS_DROP_HEIGHT);
	else
		draw[1](canvas,
				state->sheet + IRIS_SMALL_DROPS +
					frame / 2 * PICTURE_BYTES(IRIS_SMALL_DROP),
				x - IRIS_SMALL_DROP_WIDTH / 2, y - IRIS_SMALL_DROP_HEIGHT / 2,
				IRIS_SMALL_DROP_WIDTH, IRIS_SMALL_DROP_HEIGHT);

	/* The trail: particles of size 8 (6 for a small drop), half a size apart.
	 */
	size = 8 - small * 2;
	half = size / 2;
	i = 1;
	trailY = y + i * half;
	if(trailY > 112)
		return;
next:
	draw[0](canvas,
			state->sheet + (BattleEffect_MotePictures[size - 1] +
							frame / 2 * MOTE_SET_BYTES),
			x - size / 2, trailY, size, size * 2);
	i++;
	trailY = y + i * half;
	if(trailY <= 112)
		goto next;
}

extern const MODEL_VERTEX Iris_PillarVertices[6];

/* Iris: draws one rising pillar, a textured strip of six vertices whose */
/* two lower rows come from Iris_PillarVertices and whose top row is raised to
 */
/* height. x is the pillar's screen column (63 is the centre); the base sits */
/* height pixels below row 63, so the pillar grows upward from there. */
void btIrisDrawPillar(s32 halfWidth, s32 x, s32 height) {
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;
	void *projected;
	RENDER_MODEL *model;
	MODEL_VERTEX vertices[6];
	MODEL_TEXTURE texture;
	VECTOR3 position;

	projected = syHeapAllocEwram(48);
	model = btRenderAllocateModelList(1);
	memcpy(vertices, Iris_PillarVertices, sizeof(vertices));

	texture.logWidth = 4;
	texture.logHeight = 4;
	texture.pixels = state->sheet + IRIS_PILLAR_TEXTURE;

	/* Each row is halfWidth to either side; the top row reaches height. */
	vertices[0].x = -halfWidth;
	vertices[1].x = halfWidth;
	vertices[2].x = -halfWidth;
	vertices[3].x = halfWidth;
	vertices[4].x = -halfWidth;
	vertices[5].x = halfWidth;
	vertices[5].y = height;
	vertices[4].y = height;

	syInitMatrix();
	SET_POSITION(&position, INTtoFIXED(x) - toFIXED(63),
				 INTtoFIXED(63 - height), 0);
	syMatrixTranslateVector(&position);

	model->kind = MODEL_TEXTURED_MAX;
	model->texture = &texture;
	model->mesh = Iris_PillarMesh;
	model->vertices = projected;
	btRenderTransformVertices(vertices, projected, 6);
	btRenderDrawModelList(model);
	syHeapRelease(model);
	syHeapRelease(projected);
}

#define DIAGONAL_LINE_MAX 48 /* the most pixels a diagonal line runs */

/* Draws a 45-degree canvas line from (x + w/2, y - w/2) down-left toward */
/* (x, y), at most DIAGONAL_LINE_MAX pixels long, in the given colour. */
void btEffectDrawDiagonalCanvasLine(s32 x, s32 y, s32 w, u8 color) {
	s32 h = w >> 1;
	s32 x1;
	s32 y1;

	x += h;
	y -= h;
	if(h > DIAGONAL_LINE_MAX)
		h = DIAGONAL_LINE_MAX;
	y1 = y + h;
	x1 = x - h;
	btEffectDrawClippedCanvasLine(x, y, x1, y1, color);
}

/* Iris's runs: the pink petals (PinkPetalsTiles, fifteen pictures of a petal */
/* tumbling), the motes in state->particles, and the opening's streaks, each a
 */
/* head and the points that trail it. */
#define IRIS_PETALS		  36
#define IRIS_PETAL_FRAMES 15
#define IRIS_PETAL_SIZE                                                        \
	16 /* a petal's 16-colour sprite, in IRIS_PETAL_PALETTE */
#define IRIS_PETAL_BYTES   (IRIS_PETAL_SIZE * IRIS_PETAL_SIZE / 2)
#define IRIS_PETAL_PALETTE 15
/* One frame of a petal's life, which counts down in its high byte; the petal */
/* is drawn at twice its life's scale. */
#define IRIS_PETAL_FRAME (1 << 8)
/* As she appears her petals fly out, shrinking a 32nd of their size */
/* (FIXED_ONE / 32) every two frames; IRIS_PETAL_SHRINK is how much after so */
/* many frames. */
#define IRIS_PETAL_SHRINK(frames) ((frames) >> 1 << 11)
/* A canvas x as a 16.16 x on the screen, where the canvas is shown at twice */
/* its width. */
#define IRIS_SCREEN_X_FX(canvasX) ((canvasX) << 17)
#define IRIS_MOTES				  32
/* A mote's variant: its size in pixels in the low bits; IRIS_MOTE_FADING to */
/* fade through the faded mote sets once it is IRIS_MOTE_FADE_AGE frames old; */
/* IRIS_MOTE_GLOWING to be drawn instead as a glow, a textured quad that grows
 */
/* and fades through the IRIS_GLOW_TEXTURES once it is IRIS_GLOW_FADE_AGE */
/* frames old. */
#define IRIS_MOTE_SIZE	   0xf
#define IRIS_MOTE_FADING   0x10
#define IRIS_MOTE_GLOWING  0x100
#define IRIS_MOTE_FADE_AGE 32
#define IRIS_GLOW_FADE_AGE 24
#define IRIS_GLOW_TEXTURES 8
/* The motes that burst from her as she appears, half each way, and the */
/* random part of their speed, in sixteenths of a pixel a frame. */
#define IRIS_FIRST_MOTES	 12
#define IRIS_MOTE_DRIFT_MASK 31
#define IRIS_STREAKS		 15
#define IRIS_STREAK_LENGTH	 10
/* Iris herself, RobedGoddessTiles as a 4 x 4 grid of 32x32 sprites. */
#define IRIS_GODDESS_COLUMNS 4
#define IRIS_GODDESS_ROWS	 4
#define IRIS_GODDESS_SPRITES (IRIS_GODDESS_COLUMNS * IRIS_GODDESS_ROWS)
#define IRIS_GODDESS_TILE	 32
/* A sprite of her 256-colour figure, a byte a pixel. */
#define IRIS_GODDESS_SPRITE_BYTES (IRIS_GODDESS_TILE * IRIS_GODDESS_TILE)
/* Whether frame is one of first to last, both included. */
#define IRIS_DURING(frame, first, last)                                        \
	((u32)((frame) - (first)) <= (last) - (first))
/* The summon's milestones. The opening's circle of light and streaks run to */
/* IRIS_APPEAR_FRAME, when she appears; A or B skips the opening to it. */
#define IRIS_APPEAR_FRAME 176
/* She comes to rest, and the next frame the drops' pictures are made. */
#define IRIS_ARRIVAL_FRAME 229
#define IRIS_DROPS_FRAME   230
/* The sunset darkens from here. */
#define IRIS_DARKEN_FRAME 240
/* She turns white and sinks, then rises away, and is gone. */
#define IRIS_LEAVE_FRAME 351
#define IRIS_RISE_FRAME	 366
#define IRIS_GONE_FRAME	 386
/* The pillars of light climb, until a band of light rises over them. */
#define IRIS_PILLARS_FRAME	   370
#define IRIS_PILLARS_END_FRAME 422
/* The fountain starts under the starfield; A or B after her appearance skips */
/* to it. */
#define IRIS_FOUNTAIN_FRAME 446
/* The rings of the healing. */
#define IRIS_HEAL_FRAME 639
/* The glow's textures are made; the canvas flashes white for three frames, */
/* the cone of light reaches out a frame later, and the tall ring of motes */
/* follows. */
#define IRIS_GLOW_FRAME		 754
#define IRIS_CONE_FRAME		 759
#define IRIS_TALL_RING_FRAME 783
/* The backdrop whitens, and the summon ends. */
#define IRIS_WHITE_OUT_FRAME 817
#define IRIS_END_FRAME		 850
/* From her appearance to IRIS_ARRIVAL_FRAME she drops into place on a sine, a
 */
/* 1024th of a turn a frame and 64 pixels high; this is how far it has carried
 */
/* her by then, taken off so that she comes to rest there. */
#define IRIS_ARRIVAL_DROP toFIXED(20.4453125)
/* The five light bars of the fountain, 45-degree lines growing a pixel a */
/* frame up and right from where they start. Each lasts IRIS_BAR_FRAMES from */
/* its own first frame, fading over its last 31. */
#define IRIS_BAR_FRAMES			   210
#define IRIS_CORNER_BAR_FRAME	   513
#define IRIS_INNER_LEFT_BAR_FRAME  522
#define IRIS_INNER_LOWER_BAR_FRAME 531
#define IRIS_OUTER_LEFT_BAR_FRAME  535
#define IRIS_OUTER_LOWER_BAR_FRAME 554
#define IRIS_BAR_SHOWN(frame, start)                                           \
	IRIS_DURING(frame, start, (start) + IRIS_BAR_FRAMES - 1)
#define IRIS_BAR_SHADE(frame, start)  ((start) + IRIS_BAR_FRAMES - (frame))
#define IRIS_BAR_LENGTH(frame, start) ((frame) - ((start) - 1))
/* The opening's streaks rise until their heads reach this row. */
#define IRIS_STREAK_TOP 59
/* The sounds at the frames of Iris_PsynergySoundFrames, */
/* Iris_DoorSoundFrames and Iris_PowerUpSoundFrames. */
#define IRIS_PSYNERGY_SOUNDS 11
#define IRIS_DOOR_SOUNDS	 5
#define IRIS_POWER_UP_SOUNDS 6
/* As she appears, a ring of IRIS_APPEAR_RING_POINTS motes spreads about her, a
 */
/* pixel a frame. */
#define IRIS_APPEAR_RING_POINTS 256
#define IRIS_APPEAR_RING_MOTE	4 /* its motes' size */
/* The share of each pixel's level her canvas keeps a frame: any share but */
/* CANVAS_FADE_HALF keeps three quarters. */
#define IRIS_CANVAS_FADE_SHARE 70
/* The bands of light that rise up the canvas, eight rows a frame. */
#define IRIS_BAND_HEIGHT 40
/* The fountain's first frame draws three shaded circles from this size, two */
/* pixels apart, that the mote fades overwrite before they are seen. */
#define IRIS_UNSEEN_CIRCLE_SIZE 14
/* The pillars of light: those that widen or narrow change width every so many
 */
/* pixels they rise. */
#define IRIS_WIDENING_STEP	19
#define IRIS_NARROWING_STEP 21
/* The healing's rings: IRIS_HEAL_RINGS of IRIS_HEAL_RING_POINTS motes each, */
/* IRIS_HEAL_RING_GAP frames apart; then a wide arc and a tall ring of motes, */
/* IRIS_SWEEP_STEP apart, of IRIS_SWEEP_MOTE pixels. */
#define IRIS_HEAL_RINGS		  10
#define IRIS_HEAL_RING_GAP	  8
#define IRIS_HEAL_RING_FRAMES 31 /* each ring's life */
#define IRIS_HEAL_RING_POINTS 128
#define IRIS_WIDE_ARC_MOTES	  96
#define IRIS_TALL_RING_MOTES  197
#define IRIS_SWEEP_STEP		  (TRIG_TURN * 3 / 512)
#define IRIS_SWEEP_MOTE		  6
/* A speed of n half pixels a frame, as 16.16 (toFIXED(n) / 2). */
#define IRIS_HALF_PIXELS_FX(n) ((n) << 15)
/* The cone of light's texture, after the glow textures. */
#define IRIS_CONE_TEXTURE                                                      \
	(IRIS_TEXTURES + IRIS_GLOW_TEXTURES * IRIS_TEXTURE_BYTES)
#define IRIS_CONE_TEXTURE_SIZE 32
/* How far the cone stretches toward its full size (FIXED_ONE) in so many */
/* frames: a 64th a frame. */
#define IRIS_CONE_GROWTH(frames) ((frames) << 10)
/* The battle camera's yaw (BATTLE_CAMERA.yaw), read as a signed halfword of */
/* the camera state. */
#define IRIS_CAMERA_YAW (offsetof(BATTLE_CAMERA, yaw) / sizeof(s16))
/* The 128 background colours the scene fades (entries 96 to 223). */
#define IRIS_FADE_COLORS 128
/* The colour they step toward, a level at a time, once the circle of light */
/* widens. */
#define IRIS_FADE_TARGET_RED   23
#define IRIS_FADE_TARGET_GREEN 1
#define IRIS_FADE_TARGET_BLUE  15

/* The petal objects' state, in the map cell buffer at byte 8000. */
typedef struct IRIS_PETAL_SET {
	s16 life[IRIS_PETALS]; /* high byte: frames left; 0 frees the slot */
	u16 tiles[IRIS_PETAL_FRAMES];
	u16 frames[IRIS_PETALS];
	s32 x[IRIS_PETALS];
	s32 y[IRIS_PETALS];
	s32 vx[IRIS_PETALS];
	s32 vy[IRIS_PETALS];
} IRIS_PETAL_SET;

/* A point of a streak: the head moves, the other nine trail it. */
typedef struct IRIS_POINT {
	union {
		s32 x;
		struct {
			u16 xFraction;
			s16 xPixels;
		};
	};
	union {
		s32 y;
		struct {
			u16 yFraction;
			s16 yPixels;
		};
	};
	s32 ang;
	s32 speed;
	s32 unused; /* cleared as the opening starts; nothing reads it */
} IRIS_POINT;

/* The opening's circle of light and its fifteen streaks, at byte 12000. */
typedef struct IRIS_WORK {
	union {
		s32 x;
		struct {
			u16 xFraction;
			s16 xPixels;
		};
	};
	union {
		s32 y;
		struct {
			u16 yFraction;
			s16 yPixels;
		};
	};
	s32 unused[2]; /* nothing reads or writes them */
	union {
		s32 radius;
		struct {
			u16 radiusFraction;
			s16 radiusPixels;
		};
	};
	IRIS_POINT streaks[IRIS_STREAKS][IRIS_STREAK_LENGTH];
} IRIS_WORK;

/* Iris's state, laid out in the map cell buffer: the petals, the scene, and */
/* the streaks' points. */
#define IRIS_PETAL_STATE (*(IRIS_PETAL_SET *)(Ram_MapCellBuffer + 8000))
#define IRIS_SCENE		 (*(IRIS_WORK *)(Ram_MapCellBuffer + 12000))
#define IRIS_STREAK_POINTS                                                     \
	((IRIS_POINT(*)[IRIS_STREAK_LENGTH])(Ram_MapCellBuffer + 12000 +           \
										 offsetof(IRIS_WORK, streaks)))

extern const FIVE_WIDTHS Iris_RisingPillarWidths;
extern const SEVEN_WIDTHS Iris_WideningPillarWidths;
extern const SEVEN_WIDTHS Iris_NarrowingPillarWidths;
extern const FOUR_SPANS Iris_LeftStripSpans;
extern const FOUR_SPANS Iris_RightStripSpans;

/* Point j of streak i of the iris's streaks. */
#define STREAK(i, j)                                                           \
	(((IRIS_POINT *)IRIS_STREAK_POINTS)[(i) * IRIS_STREAK_LENGTH + (j)])
/* BG palette 6, where the fade's colours are written. */
#define FADE_PLTT ((u16 *)&BG_PLTT_COLOR(6, 0))

/* A splash where a drop lands: rings spread from (x, y) for twice the */
/* splash's duration, then a smaller pair of rings and a column of spray */
/* rises while the screen shakes; the drop itself is drawn last. */
static inline void btIrisDrawSplash(s32 t, s32 x, s32 y, s32 small) {
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;
	u8 *canvas = Ram_HeapSlots->battleCanvas;
	DRAW_RECTANGLE drawAlt;
	DRAW_RECTANGLE draw;
	u8 *sprites;
	s32 r;
	s32 fade;
	s32 dotPicture;
	s32 moteFade;
	s32 point;
	s32 segment;
	s32 column;
	s32 dead;

	sprites = Ram_HeapSlots->battleSheet;
	draw = Ram_HeapSlots->ramCode;
	drawAlt = Ram_HeapSlots->ramCodePaired;
	if(t < *(Iris_SplashRadii + small) * 2) {
		r = *(Iris_SplashRadii + small) * 2 - t;
		fade = r / 6;
		dotPicture = fade * PICTURE_BYTES(IRIS_DOT);
		moteFade = fade * MOTE_SET_BYTES;
		for(point = 0; point != IRIS_SPLASH_POINTS; point++) {
			s32 px;
			s32 a;
			s32 py;
			a = IRIS_TURN_64THS(point);
			px = (FIXEDtoINT(r * sySin(a) / 2)) + x;
			py = (FIXEDtoINT(r * syCos(a))) + y;
			draw(canvas, sprites + dotPicture + IRIS_DOTS, px, py - 2,
				 IRIS_DOT_WIDTH, IRIS_DOT_HEIGHT);
		}
		if(small == 0) {
			s32 offset = 0;
			u8 *ring;
			DRAW_RECTANGLE blit;
			s32 spoke;

			if(t < IRIS_RING_FADE_FRAMES)
				offset = IRIS_LARGE_RINGS + 2 * PICTURE_BYTES(IRIS_LARGE_RING);
			else if(t < 2 * IRIS_RING_FADE_FRAMES)
				offset = IRIS_LARGE_RINGS + PICTURE_BYTES(IRIS_LARGE_RING);
			/* FAKEMATCH(dead): t * 4 / 3 into dead. */
			dead = t * 4;
			dead /= 3;
			ring = sprites + offset;
			/* FAKEMATCH(temporary): draw copied into blit for the ring's draws.
			 */
			blit = draw;
			for(spoke = 0; spoke != IRIS_SPLASH_SPOKES; spoke++) {
				s32 px;
				s32 a;
				s32 py;
				a = spoke * (TRIG_TURN / IRIS_SPLASH_SPOKES) + (t * 5 << 9);
				px = (FIXEDtoINT(r * sySin(a))) + x;
				py = (FIXEDtoINT(r * syCos(a) * 2)) + y;
				blit(canvas, ring, px - 3, py - 7, IRIS_LARGE_RING_WIDTH,
					 IRIS_LARGE_RING_HEIGHT);
				a += a >> 1;
				px =
					(FIXEDtoINT(syMulFX(FIXED_ONE * 5 / 2, r * sySin(a) / 2))) +
					x;
				py = (FIXEDtoINT(syMulFX(FIXED_ONE * 5, r * syCos(a) / 2))) + y;
				blit(canvas, ring, px - 3, py - 7, IRIS_LARGE_RING_WIDTH,
					 IRIS_LARGE_RING_HEIGHT);
			}
			draw(canvas,
				 state->sheet + (BattleEffect_MotePictures[8] + moteFade),
				 x - 4, y - 9, 9, 18);
		}
		else {
			s32 offset = IRIS_SMALL_RINGS;
			u8 *ring;
			DRAW_RECTANGLE blit;
			s32 spoke;

			if(t < IRIS_RING_FADE_FRAMES)
				offset = IRIS_SMALL_RINGS + 2 * PICTURE_BYTES(IRIS_SMALL_RING);
			else if(t < 2 * IRIS_RING_FADE_FRAMES)
				offset = IRIS_SMALL_RINGS + PICTURE_BYTES(IRIS_SMALL_RING);
			/* FAKEMATCH(dead): t * 4 / 3 into dead, as in the large splash's
			 * arm. */
			dead = t * 4;
			dead /= 3;
			ring = sprites + offset;
			/* FAKEMATCH(temporary): draw copied into blit for the ring's draws.
			 */
			blit = draw;
			for(spoke = 0; spoke != IRIS_SPLASH_SPOKES; spoke++) {
				s32 px;
				s32 a;
				s32 py;
				a = spoke * (TRIG_TURN / IRIS_SPLASH_SPOKES) + (t * 5 << 9);
				px = (FIXEDtoINT(r * sySin(a))) + x;
				py = (FIXEDtoINT(r * syCos(a) * 2)) + y;
				blit(canvas, ring, px - 2, py - 5, IRIS_SMALL_RING_WIDTH,
					 IRIS_SMALL_RING_HEIGHT);
				a += a >> 1;
				px =
					(FIXEDtoINT(syMulFX(FIXED_ONE * 5 / 2, r * sySin(a) / 2))) +
					x;
				py = (FIXEDtoINT(syMulFX(FIXED_ONE * 5, r * syCos(a) / 2))) + y;
				blit(canvas, ring, px - 2, py - 5, IRIS_SMALL_RING_WIDTH,
					 IRIS_SMALL_RING_HEIGHT);
			}
			draw(canvas,
				 state->sheet + (BattleEffect_MotePictures[6] + moteFade),
				 x - 3, y - 7, 7, 14);
		}
	}
	else {
		if(t < *(Iris_SplashRadii + small) * 2 + *(Iris_SplashRadii + small)) {
			s32 dots;

			r = t - *(Iris_SplashRadii + small) * 2;
			dots = r / 3 * PICTURE_BYTES(IRIS_DOT);
			if(dots == IRIS_DOT_FADES * PICTURE_BYTES(IRIS_DOT))
				dots = (IRIS_DOT_FADES - 1) * PICTURE_BYTES(IRIS_DOT);
			dots += IRIS_DOTS;
			for(point = 0; point != IRIS_SPLASH_POINTS; point++) {
				s32 px;
				s32 a;
				s32 py;
				a = IRIS_TURN_64THS(point);
				px = (FIXEDtoINT(r * sySin(a))) + x;
				py = (FIXEDtoINT(r * syCos(a) * 2)) + y;
				draw(canvas, sprites + dots, px - 1, py - 2, IRIS_DOT_WIDTH,
					 IRIS_DOT_HEIGHT);
			}
			for(point = 0; point != IRIS_SPLASH_POINTS; point++) {
				s32 px;
				s32 a;
				s32 py;
				a = IRIS_TURN_64THS(point);
				px = (FIXEDtoINT(r * sySin(a) * 2)) + x;
				py = (FIXEDtoINT(r * syCos(a))) + y +
					 Iris_SplashOffsetsY[small] * 2;
				draw(canvas, sprites + dots, px - 1, py - 2, IRIS_DOT_WIDTH,
					 IRIS_DOT_HEIGHT);
			}
		}
		for(segment = 0; segment < t - *(Iris_SplashRadii + small) * 2 &&
						 y + segment * IRIS_SPRAY_HEIGHT <= 112;
			segment++)
			draw(canvas, sprites + IRIS_SPRAY, x - IRIS_SPRAY_WIDTH / 2,
				 y + segment * IRIS_SPRAY_HEIGHT, IRIS_SPRAY_WIDTH,
				 IRIS_SPRAY_HEIGHT);
		if(t < *(Iris_SplashRadii + small) * 2 + 8) {
			s32 shake = (*(Iris_SplashRadii + small) * 2 - t + 8) >> 1;
			typeof(BgScrollWork[0]) *scroll = BgScrollWork;
			s16 base = BATTLE_BACKGROUND_SCROLL_Y;

			/* DEAD: both arms below set it again; the ROM keeps the store. */
			scroll[1].y = base;
			if(t & 1)
				scroll[1].y = shake + BATTLE_BACKGROUND_SCROLL_Y;
			else
				scroll[1].y = base - shake;
		}
		if(small == 0)
			drawAlt(canvas, state->sheet + IRIS_DROPS, x - IRIS_DROP_WIDTH / 2,
					y - IRIS_DROP_HEIGHT / 2, IRIS_DROP_WIDTH,
					IRIS_DROP_HEIGHT);
		else
			drawAlt(canvas, state->sheet + IRIS_SMALL_DROPS,
					x - IRIS_SMALL_DROP_WIDTH / 2,
					y - IRIS_SMALL_DROP_HEIGHT / 2, IRIS_SMALL_DROP_WIDTH,
					IRIS_SMALL_DROP_HEIGHT);
	}
}

/* Iris: a circle of light opens over a dark field while fifteen */
/* streaks swirl up out of it; Iris herself appears, a 4 x 4 grid of */
/* sprites, amid two bursts of pink petals and later rises away; drops fall and
 */
/* splash in rings; pillars of light climb, the scene fills with rising motes,
 */
/* rays turn about the centre, and the screen whitens as she heals the party. */
void btEffectRunIris(BATTLE_EFFECT_PARM *args) {
	IRIS_WORK *scene = &IRIS_SCENE;
	IRIS_POINT(*streaks)[IRIS_STREAK_LENGTH] = IRIS_STREAK_POINTS;
	u8 *canvas = Ram_HeapSlots->battleCanvas;
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;
	SPR **objects = (SPR **)MapGridCellBuffer;
	u8 *sprites = Ram_HeapSlots->battleSheet;
	IRIS_PETAL_SET *petals = &IRIS_PETAL_STATE;
	BATTLE_STATE *battle = Ram_HeapSlots->battleState;
	s32 savedScroll = BgScrollWork[1].x;
	s16 *camera = Ram_HeapSlots->cameraState;
	s32 savedYaw;
	s32 frame;
	DRAW_RECTANGLE draw[2];

	/* The first petal slot with no life left, or -1. */
	s32 FindFreePetal(void) {
		s32 i;
		s32 slot = IRIS_PETALS;

		for(i = 0; i != IRIS_PETALS; i++) {
			if(petals->life[i] == 0) {
				slot = i;
				break;
			}
		}
		if(slot == IRIS_PETALS)
			slot = -1;
		return slot;
	}

	/* Each live petal: placed at its size, moved, dropped when it leaves the */
	/* screen, otherwise animated and aged. */
	void UpdatePetals(void) {
		SCALE scale;
		s32 place[4];
		s32 i;

		for(i = 0; i != IRIS_PETALS; i++) {
			if(petals->life[i] != 0) {
				scale.x = scale.y = petals->life[i] * 2;
				place[3] = 0;
				place[1] = toFIXED(255);
				place[0] = petals->x[i];
				place[2] = petals->y[i];
				btRenderApplyProjectedPlacementStub(objects[i], place,
													(s32 *)&scale, 0);
				petals->x[i] += petals->vx[i];
				petals->y[i] += petals->vy[i];
				if(petals->x[i] < -toFIXED(16) || petals->x[i] > toFIXED(240) ||
				   petals->y[i] < 0 || petals->y[i] > toFIXED(160)) {
					petals->life[i] = 0;
					continue;
				}
				if(!(frame & 3))
					petals->frames[i]++;
				if(petals->frames[i] >= IRIS_PETAL_FRAMES)
					petals->frames[i] = 0;
				objects[i]->image.tile = petals->tiles[petals->frames[i]];
				petals->life[i] -= IRIS_PETAL_FRAME;
			}
		}
	}

	/* The first free mote of IRIS_MOTES, or -1. */
	s32 FindFreeParticle(void) {
		s32 i;
		s32 slot = IRIS_MOTES;

		for(i = 0; i != IRIS_MOTES; i++) {
			if(state->particles[i].variant == 0) {
				slot = i;
				break;
			}
		}
		if(slot == IRIS_MOTES)
			slot = -1;
		return slot;
	}

	/* The motes: a picture of their size that fades once IRIS_MOTE_FADE_AGE */
	/* frames old when IRIS_MOTE_FADING, or as IRIS_MOTE_GLOWING a glow that */
	/* grows and fades. */
	void DrawParticles(void) {
		MODEL_TEXTURE texture;
		s32 place[3];
		s32 i;

		for(i = 0; i != IRIS_MOTES; i++) {
			if(state->particles[i].variant != 0) {
				s32 n = state->particles[i].variant & IRIS_MOTE_SIZE;
				s32 px = FIXEDtoINT(state->particles[i].x);
				s32 py = FIXEDtoINT(state->particles[i].y);
				s32 offset = 0;

				if(state->particles[i].x < 0 ||
				   state->particles[i].x > toFIXED(127) ||
				   state->particles[i].y < 0 ||
				   state->particles[i].y > toFIXED(127)) {
					state->particles[i].variant = 0;
					continue;
				}
				if(state->particles[i].variant & IRIS_MOTE_FADING) {
					s32 age = frame - state->particles[i].z;

					if(age > IRIS_MOTE_FADE_AGE)
						offset = (age - IRIS_MOTE_FADE_AGE) * MOTE_SET_BYTES;
					if(offset == IRIS_MOTE_FADES * MOTE_SET_BYTES) {
						state->particles[i].variant = 0;
						continue;
					}
				}
				if(state->particles[i].variant & IRIS_MOTE_GLOWING) {
					void *verts = syHeapAllocEwram(32);
					RENDER_MODEL *model = btRenderAllocateModelList(1);
					s32 age;

					offset = 0;
					age = frame - state->particles[i].z;
					if(age > IRIS_GLOW_FADE_AGE)
						offset =
							(age - IRIS_GLOW_FADE_AGE) * IRIS_TEXTURE_BYTES;
					texture.logWidth = 5;
					texture.logHeight = 6;
					texture.pixels = state->sheet + IRIS_TEXTURES + offset;
					syMatrixResetUnit();
					place[0] = state->particles[i].x - toFIXED(64);
					place[1] = state->particles[i].y - toFIXED(64);
					place[2] = 0;
					syMatrixTranslateVector(place);
					place[1] = place[0] = state->particles[i].velZ;
					place[2] = 0;
					syMatrixScaleVector(place);
					state->particles[i].velZ += toFIXED(1) / 8;
					model->kind = MODEL_TEXTURED_MAX;
					model->texture = &texture;
					model->mesh = Iris_ParticleMesh;
					model->vertices = verts;
					btRenderTransformVertices(Iris_ParticleCorners, verts, 4);
					btRenderDrawModelList(model);
					syHeapRelease(model);
					syHeapRelease(verts);
					if(offset == (IRIS_GLOW_TEXTURES - 1) * IRIS_TEXTURE_BYTES)
						state->particles[i].variant = 0;
				}
				else {
					draw[0](canvas,
							state->sheet +
								(BattleEffect_MotePictures[n - 1] + offset),
							px - n / 2, py - n, n, n * 2);
				}
				state->particles[i].x += state->particles[i].velX;
				state->particles[i].y += state->particles[i].velY;
			}
		}
	}

	s32 motesMask;
	u8 *tileBuffer;
	FIXED pos[4];
	SCALE scale;
	SCALE shrink;
	MODEL_TEXTURE texture;
	MODEL_TEXTURE coneTexture;
	s32 lift;
	s32 i;
	s32 j;

	s32 column;
	s32 m;
	s32 off;
	u8 *dst;
	u8 *tiles;
	/* FAKEMATCH(dead): 274 locals nothing uses (any count from 264 to 283
	 * does).
	 */
	UNUSED s32 z0, z1, z2, z3, z4, z5, z6, z7, z8, z9, z10, z11, z12, z13, z14,
		z15, z16, z17, z18, z19, z20, z21, z22, z23, z24, z25, z26, z27, z28,
		z29, z30, z31, z32, z33, z34, z35, z36, z37, z38, z39, z40, z41, z42,
		z43, z44, z45, z46, z47, z48, z49, z50, z51, z52, z53, z54, z55, z56,
		z57, z58, z59, z60, z61, z62, z63, z64, z65, z66, z67, z68, z69, z70,
		z71, z72, z73, z74, z75, z76, z77, z78, z79, z80, z81, z82, z83, z84,
		z85, z86, z87, z88, z89, z90, z91, z92, z93, z94, z95, z96, z97, z98,
		z99, z100, z101, z102, z103, z104, z105, z106, z107, z108, z109, z110,
		z111, z112, z113, z114, z115, z116, z117, z118, z119, z120, z121, z122,
		z123, z124, z125, z126, z127, z128, z129, z130, z131, z132, z133, z134,
		z135, z136, z137, z138, z139, z140, z141, z142, z143, z144, z145, z146,
		z147, z148, z149, z150, z151, z152, z153, z154, z155, z156, z157, z158,
		z159, z160, z161, z162, z163, z164, z165, z166, z167, z168, z169, z170,
		z171, z172, z173, z174, z175, z176, z177, z178, z179, z180, z181, z182,
		z183, z184, z185, z186, z187, z188, z189, z190, z191, z192, z193, z194,
		z195, z196, z197, z198, z199, z200, z201, z202, z203, z204, z205, z206,
		z207, z208, z209, z210, z211, z212, z213, z214, z215, z216, z217, z218,
		z219, z220, z221, z222, z223, z224, z225, z226, z227, z228, z229, z230,
		z231, z232, z233, z234, z235, z236, z237, z238, z239, z240, z241, z242,
		z243, z244, z245, z246, z247, z248, z249, z250, z251, z252, z253, z254,
		z255, z256, z257, z258, z259, z260, z261, z262, z263, z264, z265, z266,
		z267, z268, z269, z270, z271, z272, z273;

	state->args = args;
	btEffectStartCanvasLayer(BGCNT_WRAP);
	btEffectConfigureEffectDisp();
	PLTT_BACKDROP = 0;
	BG_PLTT_COLOR(0, 1) = 0;
	EFFECT_START_TRANSFER(state, CANVAS_COPY, CANVAS_FADE_THREE_QUARTERS);
	REG_DISPCNT = EFFECT_DISPCNT;
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
	REG_BG2PA = Q_8_8(0.5);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	btIrisWipeScr(0, 4, btIrisChangeBackdrop);
	IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
	while(((volatile DMA_REGISTERS *)REG_DMA3)->ctrl & DMA_BUSY) {
	}
	EFFECT_PRESENT_CANVAS(state);
	REG_BG2PA = Q_8_8(0.5);
	REG_BG2X = 0;
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
						CANVAS_FADE_THREE_QUARTERS);
	/* The camera is turned a quarter turn, the other way on the party's side,
	 */
	/* until the summon ends. */
	savedYaw = camera[IRIS_CAMERA_YAW];
	if(args->side == EFFECT_SIDE_ENEMIES) {
		s32 yaw = DEGtoANG(90);

		camera[IRIS_CAMERA_YAW] = yaw;
	}
	else {
		s32 yaw = TRIG_TURN * 3 / 4;

		camera[IRIS_CAMERA_YAW] = yaw;
	}
	tileBuffer = Iris_TileBuffer;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
						BLITTER_ADDITIVE);
	draw[0] = Ram_HeapSlots->ramCode;
	draw[1] = Ram_HeapSlots->ramCodePaired;
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, tileBuffer, 0, 0);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
	for(i = 0; i != MOTE_SET_BYTES; i++)
		sprites[i] >>= 1;
	btEffectDrawShadedCircles(state->sheet, 32, FIXED_ONE);
	btGraphicsLoadGreyRamp();
	frame = 0;
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
	syInitMatrix();

	for(;;) {
		motesMask = IRIS_MOTE_DRIFT_MASK;
		/* A or B: skips the opening, or the rest of the scene. */
		if(InputWork.repeat & (KEY_A | KEY_B)) {
			if(frame < IRIS_APPEAR_FRAME) {
				frame = IRIS_APPEAR_FRAME;
			}
			else if(IRIS_DURING(frame, IRIS_APPEAR_FRAME + 1,
								IRIS_FOUNTAIN_FRAME - 1)) {
				REG_BLDALPHA = EFFECT_BLEND_ADD;
				IWRAM_FILL_WORDS(canvas, CANVAS_BYTES,
								 CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
				EFFECT_PRESENT_CANVAS(state);
				if(frame <= IRIS_GONE_FRAME) {
					for(i = 0; i != IRIS_GODDESS_SPRITES; i++)
						fdSprDestroyStub(state->sprs[i]);
					btEffectShowLivingTargets(state->args);
					syPaletteLoadBasicSprCols();
					for(i = 0; i != state->args->tgtCnt; i++)
						btEffectTintBattler(state->args->tgts[i], TINT_KEEP,
											POSE_DAMAGE, TINT_KEEP, 0);
				}
				frame = IRIS_FOUNTAIN_FRAME;
			}
			else {
				syWordsFillEntry(canvas, CANVAS_BYTES,
								 CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
				EFFECT_PRESENT_CANVAS(state);
				break;
			}
		}
		if(frame == IRIS_END_FRAME)
			break;

		if(frame == 0)
			sySndPlayStub(SE_CURSE_FALL);
		for(i = 0; i != IRIS_PSYNERGY_SOUNDS; i++)
			if(frame == Iris_PsynergySoundFrames[i])
				sySndPlayStub(SE_PSYNERGY_START);
		if(frame == IRIS_APPEAR_FRAME - 1)
			sySndPlayStub(SE_POWER_UP);
		for(i = 0; i != IRIS_DOOR_SOUNDS; i++)
			if(frame == Iris_DoorSoundFrames[i])
				sySndPlayStub(SE_MERCURY_LIGHTHOUSE_DOOR_OPEN);
		for(i = 0; i != IRIS_POWER_UP_SOUNDS; i++)
			if(frame == Iris_PowerUpSoundFrames[i])
				sySndPlayStub(SE_POWER_UP);
		if(frame == IRIS_PILLARS_FRAME - 1)
			sySndPlayStub(SE_CURSE_FALL);
		for(i = 0; i != 6; i++)
			if(frame == 644 + i * 12)
				sySndPlayStub(SE_LAUNCH);
		if(frame == IRIS_GLOW_FRAME + 1)
			sySndPlayStub(SE_POWER_UP);
		for(i = 0; i != 6; i++)
			if(frame == IRIS_CONE_FRAME + i * 9)
				sySndPlayStub(SE_CRASH);
		if(frame == 820)
			sySndPlayStub(SE_BANE_ESCAPE);

		/* The opening: the circle and the streaks. */
		if(frame == 0) {
			scene->x = toFIXED(60);
			scene->y = toFIXED(128);
			scene->radius = toFIXED(12);
			for(i = 0; i != IRIS_STREAKS; i++) {
				IRIS_POINT *s = streaks[i];

				s->x = INTtoFIXED(Iris_StreakX[i]);
				s->y = INTtoFIXED(Iris_StreakY[i]);
				s->ang = Iris_StreakAngles[i];
				s->speed = Iris_StreakSpeeds[i];
				s->unused = 0;
				for(j = 1; j != IRIS_STREAK_LENGTH; j++) {
					STREAK(i, j).x = toFIXED(150);
					STREAK(i, j).y = toFIXED(150);
				}
			}
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
		}
		if(frame < IRIS_APPEAR_FRAME) {
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
								BLITTER_ADDITIVE);
			draw[1] = Ram_HeapSlots->ramCodePaired;
			for(i = 0; i != IRIS_STREAKS; i++) {
				if(!(frame & 1)) {
					for(j = IRIS_STREAK_LENGTH - 1; j != 0; j--) {
						STREAK(i, j).x = STREAK(i, j - 1).x;
						STREAK(i, j).y = STREAK(i, j - 1).y;
					}
				}
				if(frame >= i * 8 + 24) {
					IRIS_POINT *s = streaks[i];
					if(s->yPixels > IRIS_STREAK_TOP) {
						s32 size;

						s->x += sySin(s->ang) * 3 / 2;
						s->y -= syCos(s->ang) * 3 / 2;
						s->ang += s->speed * 2 / 3;
						size = (i & 3) + 5;
						for(j = 0; j != IRIS_STREAK_LENGTH; j++) {
							u8 *to = canvas;
							u8 *cell =
								sprites + BattleEffect_MotePictures[size - 1];
							IRIS_POINT *p = &STREAK(i, j);

							draw[1](to, cell, p->xPixels - (size >> 1),
									p->yPixels - size, size, size * 2);
						}
					}
				}
			}
			/* To frame 71 the circle rises and narrows while the background */
			/* brightens a level every 16 frames; then it widens, and every
			 * eight */
			/* frames each background colour steps toward the IRIS_FADE_TARGET
			 * colour. */
			if(frame <= 71) {
				scene->y -= toFIXED(1);
				scene->radius -= toFIXED(1) / 16;
				if(!(frame & 15)) {
					{
						u16 *fade = FADE_PLTT;

						for(i = 0; i != IRIS_FADE_COLORS; i++) {
							s32 c = *fade;
							s32 b = RGB_BLUE((u16)c);
							s32 g = RGB_GREEN((u16)c);
							s32 r = RGB_RED(c);

							r += 1;
							*fade++ = RGB(r, g + 1, b + 1);
						}
					}
				}
			}
			else {
				scene->radius += toFIXED(1) / 8;
				if(!(frame & 7)) {
					{
						u16 *fade = FADE_PLTT;

						for(i = 0; i != IRIS_FADE_COLORS; i++) {
							s32 c = *fade;
							s32 b = RGB_BLUE((u16)c);
							s32 g = RGB_GREEN((u16)c);
							s32 r = RGB_RED(c);

							if(r < IRIS_FADE_TARGET_RED)
								r++;
							if(g > IRIS_FADE_TARGET_GREEN)
								g--;
							if(b < IRIS_FADE_TARGET_BLUE)
								b++;
							{
								u16 v = RGB(r, g, b);

								*fade++ = v;
							}
						}
					}
				}
			}
			syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			{
				s32 size = scene->radiusPixels * 2;

				btEffectDrawShadedCircles(state->sheet, size, FIXED_ONE);
				btCanvasBlitMirroredQuarters(state->sheet, scene->xPixels,
											 scene->yPixels, size);
			}
			if(frame == IRIS_APPEAR_FRAME - 1) {
				REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) |
							 BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
				IWRAM_FILL_WORDS(canvas, CANVAS_BYTES,
								 CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
				IWRAM_FILL_WORDS(FADE_PLTT, 256, COLOR_PAIR(RGB_WHITE));
			}
		}

		/* Iris appears: her sixteen sprites, the petals and the first motes. */
		if(frame == IRIS_APPEAR_FRAME) {
			u8 *colors;
			VRAM_CACHE_ENTRY *table;

			syWordsFillEntry(canvas, CANVAS_BYTES,
							 CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			EFFECT_PRESENT_CANVAS(state);
			battle->keepBackdropPal = TRUE;
			btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED,
								 RESOURCE_SUNSET_BACKDROP, 0);
			colors = syResourceGetTableEntry(RESOURCE_ROBED_GODDESS_TILES);
			syDMAXCopy(colors, (void *)OBJ_PLTT,
					   DMA_COPY_WORDS(OBJ_PLTT_SIZE / 4), REG_DMA3);
			colors += OBJ_PLTT_SIZE;
			syDecompressTaggedLz(colors, tileBuffer);
			tiles = Iris_TileBuffer;
			for(i = 0; i != IRIS_GODDESS_SPRITES; i++) {
				SPR *obj = btEffectSprCreateWithTiles(
					IRIS_GODDESS_TILE, IRIS_GODDESS_TILE,
					OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);

				state->sprs[i] = obj;
				obj->image.pri = 1;
				syDMAXCopy(tiles + i * IRIS_GODDESS_SPRITE_BYTES,
						   (void *)OBJ_TILES(obj),
						   DMA_COPY_WORDS(IRIS_GODDESS_SPRITE_BYTES / 4),
						   REG_DMA3);
			}
			colors = syResourceGetTableEntry(RESOURCE_PINK_PETALS_TILES);
			/* The petals are white: their palette is filled, not loaded. */
			IWRAM_FILL_WORDS((void *)&OBJ_PLTT_COLOR(IRIS_PETAL_PALETTE, 0),
							 PALETTE_BANK_BYTES, -1);
			colors += PALETTE_BANK_BYTES;
			syDecompressTaggedLz(colors, state->sheet);
			table = VramCacheEntries;
			for(i = 0; i != IRIS_PETAL_FRAMES; i++) {
				SPR *obj = btEffectSprCreateWithTiles(
					IRIS_PETAL_SIZE, IRIS_PETAL_SIZE, OBJ_SIZE_16X16,
					OBJ_PALETTE(IRIS_PETAL_PALETTE));

				objects[i] = obj;
				obj->image.pri = 1;
				petals->tiles[i] = obj->image.tile;
				petals->frames[i] = i;
				syDMAXCopy(state->sheet + i * IRIS_PETAL_BYTES,
						   (void *)(OBJ_VRAM0 + table[obj->slot].ofs),
						   DMA_COPY_WORDS(IRIS_PETAL_BYTES / 4), REG_DMA3);
			}
			for(; i != IRIS_PETALS; i++) {
				SPR *obj = btEffectSprAcquire(IRIS_PETAL_SIZE, IRIS_PETAL_SIZE,
											  OBJ_SIZE_16X16,
											  OBJ_PALETTE(IRIS_PETAL_PALETTE));

				objects[i] = obj;
				syDMAXCopy(objects[0], obj, DMA_COPY_WORDS(6), REG_DMA3);
				petals->frames[i] = syRandom() % IRIS_PETAL_FRAMES;
				objects[i]->image.tile = petals->tiles[petals->frames[i]];
			}
			for(i = 0; i != IRIS_PETALS / 2; i++) {
				petals->x[i] = INTtoFIXED(syRandom() % 15) + toFIXED(57);
				petals->y[i] = INTtoFIXED(syRandom() % 15) + toFIXED(79);
				petals->vx[i] = ((syRandom() & 63) - 32) << 12;
				petals->vy[i] = ((syRandom() & 63) - 32) << 12;
			}
			for(; i != IRIS_PETALS; i++) {
				petals->x[i] = INTtoFIXED(syRandom() % 15) + toFIXED(169);
				petals->y[i] = INTtoFIXED(syRandom() % 15) + toFIXED(79);
				petals->vx[i] = ((syRandom() & 63) - 32) << 12;
				petals->vy[i] = ((syRandom() & 63) - 32) << 12;
			}
			btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET,
										state->sheet, 0, 0);
			for(i = 0; i != IRIS_FIRST_MOTES / 2; i++) {
				state->particles[i].x =
					INTtoFIXED(syRandom() % 15) + toFIXED(33);
				state->particles[i].y =
					INTtoFIXED(syRandom() % 15) + toFIXED(57);
				state->particles[i].velX = -(syRandom() & motesMask) << 12;
			}
			for(; i != IRIS_FIRST_MOTES; i++) {
				state->particles[i].x =
					INTtoFIXED(syRandom() % 15) + toFIXED(81);
				state->particles[i].y =
					INTtoFIXED(syRandom() % 15) + toFIXED(57);
				state->particles[i].velX = (syRandom() & motesMask) << 12;
			}
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
								IRIS_CANVAS_FADE_SHARE);
		}
		if(IRIS_DURING(frame, IRIS_APPEAR_FRAME + 1, IRIS_GONE_FRAME + 7)) {
			scale = Iris_UnitScale;
			pos[3] = 0;
			pos[1] = toFIXED(255);
			lift = 0;
			if(frame >= IRIS_LEAVE_FRAME) {
				u16 *pal = (u16 *)OBJ_PLTT;

				/* Every sprite colour but the last steps toward white. */
				for(i = 0; i != OBJ_PLTT_SIZE / 2 - 1; i++) {
					s32 c = *pal;
					s32 r = RGB_LEVEL_MAX;
					s32 g;
					s32 b;

					r &= c;
					g = RGB_GREEN((u16)c);
					b = RGB_BLUE((u16)c);

					if(r != RGB_LEVEL_MAX)
						r++;
					if(g != RGB_LEVEL_MAX)
						g++;
					if(b != RGB_LEVEL_MAX)
						b++;
					*pal++ = RGB(r, g, b);
				}
			}
			/* She drops into place on a sine until IRIS_ARRIVAL_FRAME; from */
			/* IRIS_LEAVE_FRAME she sinks 64 pixels on a sine, a 64th of a turn
			 * a */
			/* frame, then from IRIS_RISE_FRAME rises away ten pixels a frame.
			 */
			if(frame < IRIS_GONE_FRAME) {
				if(IRIS_DURING(frame, IRIS_LEAVE_FRAME, IRIS_RISE_FRAME - 1))
					lift =
						sySin(IRIS_TURN_64THS(frame - (IRIS_LEAVE_FRAME - 1)))
						<< 6;
				else if(frame >= IRIS_RISE_FRAME)
					lift =
						(IRIS_RISE_FRAME - frame) * toFIXED(10) + toFIXED(64);
				if(frame <= IRIS_ARRIVAL_FRAME)
					lift =
						(sySin((frame << 6) - (IRIS_APPEAR_FRAME << 6)) << 6) -
						IRIS_ARRIVAL_DROP;
				for(j = 0; j != IRIS_GODDESS_ROWS; j++) {
					for(column = 0; column != IRIS_GODDESS_COLUMNS; column++) {
						pos[0] =
							toFIXED(72) + column * toFIXED(IRIS_GODDESS_TILE);
						pos[2] =
							lift + toFIXED(56) + j * toFIXED(IRIS_GODDESS_TILE);
						btRenderApplyProjectedPlacementStub(
							state->sprs[j * IRIS_GODDESS_COLUMNS + column], pos,
							(s32 *)&scale, 0);
					}
				}
			}
			/* She is gone: the targets show again, in their damage pose. */
			if(frame == IRIS_GONE_FRAME) {
				for(i = 0; i != IRIS_GODDESS_SPRITES; i++)
					fdSprDestroyStub(state->sprs[i]);
				btEffectShowLivingTargets(state->args);
				syPaletteLoadBasicSprCols();
				for(i = 0; i != state->args->tgtCnt; i++)
					btEffectTintBattler(state->args->tgts[i], TINT_KEEP,
										POSE_DAMAGE, TINT_KEEP, 0);
			}
			/* For her first frames a line of light crosses the canvas's middle,
			 */
			/* three rows of it. */
			if(frame <= IRIS_APPEAR_FRAME + 15) {
				for(i = 0; i != 3; i++) {
					s32 y = i + 62;
					u8 v = frame + (i & 1) * 47 + 80;
					s32 off;
					s32 x;
					s32 m;

					off = CANVAS_ROW_OFFSET(y);
					for(x = 0; x != CANVAS_SIZE; x += 8) {
						for(m = x; m != x + 8; m++) {
							if(canvas[off] < v)
								canvas[off] = v;
							off++;
						}
						off += CANVAS_TILE_BYTES - 8;
					}
				}
			}
		}
		/* As she appears her petals fly out, shrinking. */
		if(IRIS_DURING(frame, IRIS_APPEAR_FRAME + 1, IRIS_APPEAR_FRAME + 47)) {
			for(i = 0; i != IRIS_PETALS; i++) {
				shrink.x = shrink.y =
					FIXED_ONE -
					IRIS_PETAL_SHRINK(frame - (IRIS_APPEAR_FRAME + 1));
				pos[3] = 0;
				pos[1] = toFIXED(255);
				pos[0] = petals->x[i];
				pos[2] = petals->y[i];
				btRenderApplyProjectedPlacementStub(objects[i], pos,
													(s32 *)&shrink, 0);
				petals->x[i] += petals->vx[i];
				petals->y[i] += petals->vy[i];
				if(!(frame & 3))
					petals->frames[i]++;
				if(petals->frames[i] >= IRIS_PETAL_FRAMES)
					petals->frames[i] = 0;
				objects[i]->image.tile = petals->tiles[petals->frames[i]];
			}
		}
		/* A ring of motes spreads about her and the first motes fly out, while
		 */
		/* the canvas's weight over the battle falls. */
		if(IRIS_DURING(frame, IRIS_APPEAR_FRAME + 3, IRIS_APPEAR_FRAME + 41)) {
			s32 radius = frame - 170;

			for(i = 0; i != IRIS_APPEAR_RING_POINTS; i++) {
				s32 a = i << 8;
				s32 px = sySin(a) * radius;
				s32 py = syCos(a) * radius;

				draw[0](
					canvas,
					state->sheet +
						BattleEffect_MotePictures[IRIS_APPEAR_RING_MOTE - 1],
					(FIXEDtoINT(px)) + 58, (FIXEDtoINT(py * 2)) + 48,
					IRIS_APPEAR_RING_MOTE, 2 * IRIS_APPEAR_RING_MOTE);
			}
			for(i = 0; i != IRIS_FIRST_MOTES; i++) {
				draw[0](canvas, state->sheet + BattleEffect_MotePictures[1],
						(FIXEDtoINT(state->particles[i].x)) - 2,
						(FIXEDtoINT(state->particles[i].y)) - 2, 2, 4);
				state->particles[i].x += state->particles[i].velX;
			}
		}
		if(IRIS_DURING(frame, IRIS_APPEAR_FRAME + 3, IRIS_APPEAR_FRAME + 34))
			REG_BLDALPHA = EFFECT_BLEND_CANVAS((210 - frame) >> 1);

		/* The drops: the motes' fading sets, the drops' pictures and the
		 * spray's */
		/* column. */
		if(frame == IRIS_DROPS_FRAME) {
			REG_BLDALPHA = EFFECT_BLEND_ADD;
			REG_BG2CNT =
				BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
			for(i = 1; i != IRIS_MOTE_FADES; i++) {
				s32 next;

				for(j = 0; j != MOTE_PICTURES; j = next) {
					u8 *src = state->sheet + BattleEffect_MotePictures[j];
					u8 *out = state->sheet + (BattleEffect_MotePictures[j] +
											  i * MOTE_SET_BYTES);
					s32 off = 0;

					next = j + 1;

					for(; off != (next << 1) * next; off++) {
						u8 v = *src - i * 8;

						if(v > CANVAS_LEVEL_BRIGHTEST)
							v = 0;
						*out = v;
						src++;
						out++;
					}
				}
			}
			for(i = 0; i != IRIS_DROP_PICTURES; i++) {
				btEffectDrawShadedCirclesMirrored(
					state->sheet + IRIS_DROPS + i * PICTURE_BYTES(IRIS_DROP),
					IRIS_DROP_WIDTH,
					FIXED_ONE - i * (FIXED_ONE / IRIS_DROP_PICTURES));
				btEffectDrawShadedCirclesMirrored(
					state->sheet + IRIS_SMALL_DROPS +
						i * PICTURE_BYTES(IRIS_SMALL_DROP),
					IRIS_SMALL_DROP_WIDTH,
					FIXED_ONE - i * (FIXED_ONE / IRIS_DROP_PICTURES));
			}
			for(i = 0; i != IRIS_PETALS; i++)
				petals->life[i] = 0;
			btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0,
										0);
			for(i = 0; i != IRIS_SPRAY_HEIGHT; i++)
				for(j = 0; j != IRIS_SPRAY_WIDTH; j++)
					sprites[IRIS_SPRAY + i * IRIS_SPRAY_WIDTH + j] =
						Iris_LineShades[j];
			dst = sprites;
			for(i = 0; i != 2; i++) {
				for(j = 0; j != IRIS_RING_FADES; j++) {
					u8 *src = state->sheet +
							  BattleEffect_MotePictures[Iris_MoteSizes[i] - 1];

					for(m = 0;
						m != *(Iris_MoteSizes + i) * *(Iris_MoteSizes + i) * 2;
						m++) {
						s32 v = *src - j * IRIS_RING_FADE_STEP;

						if(v < 0)
							v = 0;
						*dst = v;
						dst++;
						src++;
					}
				}
			}
			for(j = 0; j != IRIS_DOT_FADES; j++) {
				u8 *src = state->sheet +
						  BattleEffect_MotePictures[IRIS_DOT_WIDTH - 1];

				for(m = 0; m != PICTURE_BYTES(IRIS_DOT); m++) {
					s32 v = *src - j * IRIS_DOT_FADE_STEP;

					if(v < 0)
						v = 0;
					*dst = v;
					src++;
					dst++;
				}
			}
		}
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
							BLITTER_ADDITIVE);
		/* The sunset steps darker every 16 frames, to seven levels below its
		 * own */
		/* colours. */
		if(IRIS_DURING(frame, IRIS_DARKEN_FRAME, IRIS_DARKEN_FRAME + 127) &&
		   !(frame & 15)) {
			s32 v = (IRIS_DARKEN_FRAME - frame) / 16;

			btPaletteBrightenSavedBattleBg(v, v, v);
		}
		for(j = 0; j != 6; j++) {
			if(frame > Iris_SplashStarts[j]) {
				if(frame < Iris_SplashEnds[j]) {
					btIrisDrawSplash(frame - Iris_SplashStarts[j],
									 Iris_SplashX[j], Iris_SplashY[j],
									 Iris_SplashSmall[j]);
					if(Iris_SplashStarts[j] - Iris_SplashSmall[j] * 16 + 48 ==
					   frame) {
						for(i = 0; i != 6; i++) {
							s32 s = FindFreePetal();

							petals->life[s] = 121 * IRIS_PETAL_FRAME;
							petals->x[s] = IRIS_SCREEN_X_FX(Iris_SplashX[j]);
							petals->y[s] =
								INTtoFIXED(Iris_SplashY[j]) + toFIXED(32);
							petals->vx[s] = ((syRandom() & 31) - 16) << 12;
							petals->vy[s] = -(syRandom() & 31) << 12;
						}
					}
				}
				else if(frame < Iris_SplashEnds[j] + Iris_DropFrames[j]) {
					s32 t = frame - Iris_SplashEnds[j];

					btIrisDrawDrop(t / Iris_DropPictureSteps[j],
								   Iris_SplashX[j],
								   Iris_SplashY[j] - Iris_DropSpeeds[j] * t / 2,
								   Iris_SplashSmall[j]);
				}
			}
		}
		if(IRIS_DURING(frame, 279, 411) || frame > IRIS_FOUNTAIN_FRAME)
			UpdatePetals();
		syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);

		/* The pillars of light. */
		if(frame == IRIS_PILLARS_FRAME) {
			btEffectDrawShadedCircles(state->sheet + IRIS_PILLAR_CIRCLE,
									  IRIS_PILLAR_TEXTURE_SIZE, FIXED_ONE);
			for(i = 0; i != IRIS_PILLAR_TEXTURE_SIZE; i++) {
				syDMAXCopy(state->sheet + IRIS_PILLAR_CIRCLE + (i << 3),
						   state->sheet + IRIS_PILLAR_TEXTURE + (i << 4),
						   DMA_COPY_HALFWORDS(8), REG_DMA3);
				for(j = 0; j != 8; j++)
					state->sheet[IRIS_PILLAR_TEXTURE + 8 +
								 i * IRIS_PILLAR_TEXTURE_SIZE + j] =
						state->sheet[IRIS_PILLAR_CIRCLE + 7 + i * 8 - j];
			}
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(8);
		}
		/* Over the pillars' last eight frames the canvas's weight rises from 8
		 * to */
		/* 15. */
		if(IRIS_DURING(frame, IRIS_PILLARS_END_FRAME - 8,
					   IRIS_PILLARS_END_FRAME - 1)) {
			s32 alpha = frame - (IRIS_PILLARS_END_FRAME - 16);
			REG_BLDALPHA = BLEND_ALPHA(alpha, BLEND_WEIGHT_MAX);
		}
		/* Once she is gone the projection's centre climbs six lines a frame
		 * from */
		/* SUMMON_PROJECTION_CENTER_Y, past the battle's own at frame 406. */
		if(frame >= IRIS_GONE_FRAME)
			ProjectionWork.centerY =
				SUMMON_PROJECTION_CENTER_Y + IRIS_GONE_FRAME * 6 - frame * 6;
		if(frame <= IRIS_PILLARS_END_FRAME) {
			if(frame > IRIS_PILLARS_FRAME) {
				s32 h = (frame - IRIS_PILLARS_FRAME) << 3;

				if(h > CANVAS_SIZE - 1)
					h = CANVAS_SIZE - 1;
				btIrisDrawPillar(3, 34, h);
			}
			if(frame > IRIS_PILLARS_FRAME + 5) {
				FIVE_WIDTHS widths = Iris_RisingPillarWidths;
				s32 h = (frame - (IRIS_PILLARS_FRAME + 5)) << 3;

				if(h > CANVAS_SIZE - 1)
					h = CANVAS_SIZE - 1;
				btIrisDrawPillar(widths.widths[h >> 5], (h >> 5) * 2 + 94, h);
			}
			if(frame > IRIS_PILLARS_FRAME + 10) {
				SEVEN_WIDTHS widths = Iris_WideningPillarWidths;
				s32 h = (frame - (IRIS_PILLARS_FRAME + 10)) << 2;
				s32 q;

				if(h > CANVAS_SIZE - 1)
					h = CANVAS_SIZE - 1;
				q = h / IRIS_WIDENING_STEP;
				btIrisDrawPillar(widths.widths[q], q + 12, h);
			}
			if(frame > IRIS_PILLARS_FRAME + 12) {
				SEVEN_WIDTHS widths = Iris_NarrowingPillarWidths;
				s32 h = (frame - (IRIS_PILLARS_FRAME + 12)) << 3;
				s32 q;

				if(h > CANVAS_SIZE - 1)
					h = CANVAS_SIZE - 1;
				q = h / IRIS_NARROWING_STEP;
				btIrisDrawPillar(widths.widths[q], q * 2 + 70, h);
			}
			if(frame > IRIS_PILLARS_FRAME + 15) {
				s32 h = (frame - (IRIS_PILLARS_FRAME + 15)) << 2;

				if(h > CANVAS_SIZE - 1)
					h = CANVAS_SIZE - 1;
				btIrisDrawPillar(14, (h >> 3) + 48, h);
			}
			if(frame == IRIS_PILLARS_END_FRAME) {
				EFFECT_SET_TRANSFER(state, CANVAS_COPY, 0);
				REG_BLDALPHA = EFFECT_BLEND_ADD;
			}
		}
		/* A band of light rises up the canvas. */
		if(IRIS_DURING(frame, IRIS_PILLARS_END_FRAME + 1,
					   IRIS_PILLARS_END_FRAME + 20)) {
			s32 step = frame - IRIS_PILLARS_END_FRAME;
			s32 first;
			s32 dead;
			s32 top;
			s32 off;
			s32 k;
			s32 m;
			u8 v;

			top = CANVAS_SIZE - 8;
			first = top - step * 8;
			/* FAKEMATCH(dead): off set to 0. */
			off = 0;
			top = first;
			for(i = first; i != top + IRIS_BAND_HEIGHT &&
						   (v = (i - top) * 2, i <= CANVAS_SIZE - 1);
				i++) {
				if(i >= 0) {
					/* FAKEMATCH(dead): i / 3 into dead. */
					dead = i / 3;
					if(v > CANVAS_LEVEL_BRIGHTEST)
						v = CANVAS_LEVEL_BRIGHTEST;
					off = CANVAS_ROW_OFFSET(i);
					for(k = 0; k != CANVAS_SIZE; k += 8) {
						for(m = 0; m != 8; m++) {
							if(canvas[off] < v)
								canvas[off] = v;
							off++;
						}
						off += CANVAS_TILE_BYTES - 8;
					}
				}
			}
		}
		if(IRIS_DURING(frame, 401, 431) && (frame & 1)) {
			/* DEAD: the violet sky's entry is looked up and not used; the ROM
			 * keeps */
			/* the call. */
			syResourceGetTableEntry(RESOURCE_VIOLET_SKY_BACKDROP);
			{
				u16 *fade = FADE_PLTT;

				for(i = 0; i != IRIS_FADE_COLORS; i++) {
					s32 c = *fade;
					s32 r = RGB_LEVEL_MAX;
					s32 g;
					s32 b;

					r &= c;
					g = RGB_GREEN((u16)c);
					b = RGB_BLUE((u16)c);

					if(r > 3)
						r -= 2;
					if(g > 3)
						g -= 2;
					if(b > 3)
						b -= 2;
					*fade++ = RGB(r, g, b);
				}
			}
		}

		/* The fountain: the second backdrop, the motes and the light shafts. */
		if(frame == IRIS_FOUNTAIN_FRAME) {
			syWordsFillEntry(canvas, CANVAS_BYTES,
							 CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			syInitMatrix();
			btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED,
								 RESOURCE_STARFIELD_FLARE_BACKDROP, 0);
			off = MOTE_SET_BYTES;
			{
				u16 *fade = FADE_PLTT;
				s32 red = RGB_LEVEL_MAX;

				for(i = 0; i != IRIS_FADE_COLORS; i++) {
					s32 c = *fade;
					s32 r = red & c;
					s32 g = RGB_GREEN((u16)c);
					s32 b = RGB_BLUE((u16)c);

					g += 1;
					*fade++ = RGB(r + 1, g, b + 1);
				}
			}
			btGraphicsLoadGreyRamp();
			/* DEAD: the mote fades below overwrite these three circles before
			 */
			/* anything draws them; the ROM keeps their drawing. */
			for(i = 0; i != 6; i += 2) {
				s32 n = i + IRIS_UNSEEN_CIRCLE_SIZE;
				s32 w = n * 2;

				btEffectDrawShadedCircles(state->sheet + off, n, FIXED_ONE);
				off += n * w;
			}
			IWRAM_FILL_WORDS(state->sheet + IRIS_TEXTURES,
							 2 * IRIS_TEXTURE_BYTES, 0);
			btEffectBuildSteepShadeRampWave(state->sheet + IRIS_TEXTURES +
												IRIS_TEXTURE_BYTES,
											16, IRIS_TEXTURE_HEIGHT, FIXED_ONE);
			btEffectBuildSteepShadeRampWave(state->sheet + IRIS_TEXTURES +
												2 * IRIS_TEXTURE_BYTES,
											16, IRIS_TEXTURE_HEIGHT, FIXED_ONE);
			{
				u8 *ramp = state->sheet + IRIS_TEXTURES + IRIS_TEXTURE_BYTES;

				for(i = 0; i != IRIS_TEXTURE_HEIGHT; i++) {
					for(j = 0; j != IRIS_TEXTURE_WIDTH; j++) {
						if(*ramp > i)
							*ramp = i;
						ramp++;
					}
				}
			}
			for(i = 0; i != IRIS_PETALS; i++)
				petals->life[i] = 0;
			for(i = 0; i != IRIS_MOTES; i++)
				state->particles[i].variant = 0;
			for(i = 1; i != IRIS_MOTE_FADES; i++) {
				s32 next;

				for(j = 0; j != MOTE_PICTURES; j = next) {
					u8 *src = state->sheet + BattleEffect_MotePictures[j];
					u8 *out = state->sheet + (BattleEffect_MotePictures[j] +
											  i * MOTE_SET_BYTES);
					s32 off = 0;

					next = j + 1;

					for(; off != (next << 1) * next; off++) {
						u8 v = *src - i * 8;

						if(v > CANVAS_LEVEL_BRIGHTEST)
							v = 0;
						*out = v;
						src++;
						out++;
					}
				}
			}
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE,
								IRIS_CANVAS_FADE_SHARE);
		}
		if(IRIS_DURING(frame, IRIS_FOUNTAIN_FRAME + 1,
					   IRIS_FOUNTAIN_FRAME + 16)) {
			s32 step = frame - IRIS_FOUNTAIN_FRAME;
			s32 top;
			s32 off;
			s32 x;
			s32 m;

			top = CANVAS_SIZE + 8 - step * 8;
			for(i = top; i != 0; i--) {
				u8 v = (top - i) * 2;

				if(i <= CANVAS_SIZE - 1) {
					if(v > CANVAS_LEVEL_BRIGHTEST)
						v = CANVAS_LEVEL_BRIGHTEST;
					if(top - 8 < i)
						v = 0;
					off = CANVAS_ROW_OFFSET(i);
					for(x = 0; x != CANVAS_SIZE; x += 8) {
						for(m = 0; m != 8; m++) {
							if(canvas[off] < v)
								canvas[off] = v;
							off++;
						}
						off += CANVAS_TILE_BYTES - 8;
					}
				}
			}
		}
		if(frame > IRIS_FOUNTAIN_FRAME)
			DrawParticles();
		if(IRIS_DURING(frame, IRIS_FOUNTAIN_FRAME + 1,
					   IRIS_FOUNTAIN_FRAME + 195)) {
			void *verts = syHeapAllocEwram(80);
			RENDER_MODEL *model = btRenderAllocateModelList(1);
			s32 scroll;
			FOUR_SPANS leftSpans;
			FOUR_SPANS rightSpans;

			texture.logWidth = 5;
			texture.logHeight = 6;
			syMatrixResetUnit();
			model->kind = MODEL_TEXTURED_MAX;
			model->mesh = Iris_StripMesh;
			model->vertices = verts;
			if(frame <= 563) {
				scroll = 2 * IRIS_TEXTURE_BYTES;
				if(frame > 500) {
					scroll = (s16)(scroll - (frame - 500) * 64);
					if(scroll < 0)
						scroll = 0;
				}
				texture.pixels = state->sheet + IRIS_TEXTURES + scroll;
				if(!(frame & 7)) {
					s32 s;

					leftSpans = Iris_LeftStripSpans;
					s = FindFreePetal();

					if(s != -1) {
						s32 r = syRandom() & 127;

						petals->life[s] = -124 * IRIS_PETAL_FRAME;
						petals->x[s] =
							(leftSpans.spans[(r + 1) >> 5].left +
							 syRandom() % leftSpans.spans[(r + 1) >> 5].width)
							<< 16;
						petals->y[s] = INTtoFIXED(r) + toFIXED(16);
						petals->vx[s] = (syRandom() & 15) * 3 << 11;
						petals->vy[s] =
							-toFIXED(0.5) - ((syRandom() & 31) * 3 << 11);
					}
				}
				model->texture = &texture;
				btRenderTransformVertices(Iris_LeftStripCorners, verts, 10);
				btRenderDrawModelList(model);
			}
			if(frame <= 563) {
				scroll = 2 * IRIS_TEXTURE_BYTES;
				if(frame > 470) {
					scroll = (s16)(scroll - (frame - 470) * 64);
					if(scroll < 0)
						scroll = 0;
				}
				texture.pixels = state->sheet + IRIS_TEXTURES + scroll;
				if(!(frame & 7) && frame <= 501) {
					s32 s;

					rightSpans = Iris_RightStripSpans;
					s = FindFreePetal();

					if(s != -1) {
						s32 r = syRandom() & 127;

						petals->life[s] = -124 * IRIS_PETAL_FRAME;
						petals->x[s] =
							(rightSpans.spans[(r + 1) >> 5].left +
							 syRandom() % rightSpans.spans[(r + 1) >> 5].width)
							<< 16;
						petals->y[s] = INTtoFIXED(r) + toFIXED(16);
						petals->vx[s] = (syRandom() & 15) * 3 << 11;
						petals->vy[s] =
							-toFIXED(0.5) - ((syRandom() & 31) * 3 << 11);
					}
				}
				if(!(frame & 3)) {
					s32 n = FindFreeParticle();

					if(n != -1) {
						state->particles[n].x = toFIXED(32);
						if(frame <= 509)
							state->particles[n].x +=
								INTtoFIXED(syRandom() & 63);
						else
							state->particles[n].x +=
								INTtoFIXED(syRandom() & motesMask);
						state->particles[n].y = toFIXED(127);
						state->particles[n].velX = (syRandom() & 15) << 12;
						state->particles[n].velY =
							-toFIXED(1) / 2 - ((syRandom() & 31) * 7 << 11);
						state->particles[n].variant = (syRandom() & 3) + 1;
					}
				}
				model->texture = &texture;
				btRenderTransformVertices(Iris_RightStripCorners, verts, 10);
				btRenderDrawModelList(model);
			}
			syHeapRelease(model);
			syHeapRelease(verts);
		}

		/* The five light bars, each from its own frame: the corner's, then an
		 */
		/* inner and an outer pair either side of it, one starting on the */
		/* canvas's left edge and slipping down it, one on its bottom edge and
		 */
		/* slipping left. */
		if(IRIS_BAR_SHOWN(frame, IRIS_CORNER_BAR_FRAME)) {
			s16 shade = IRIS_BAR_SHADE(frame, IRIS_CORNER_BAR_FRAME);

			if(shade < 0 || shade > CANVAS_LEVEL_BRIGHTEST)
				shade = CANVAS_LEVEL_BRIGHTEST;
			btEffectDrawDiagonalCanvasLine(
				0, 127, IRIS_BAR_LENGTH(frame, IRIS_CORNER_BAR_FRAME), shade);
		}
		if(IRIS_BAR_SHOWN(frame, IRIS_INNER_LEFT_BAR_FRAME)) {
			s16 shade = IRIS_BAR_SHADE(frame, IRIS_INNER_LEFT_BAR_FRAME);

			if(shade < 0 || shade > CANVAS_LEVEL_BRIGHTEST)
				shade = CANVAS_LEVEL_BRIGHTEST;
			btEffectDrawDiagonalCanvasLine(
				0,
				FIXEDtoINT(
					syMulFX(IRIS_BAR_LENGTH(frame, IRIS_INNER_LEFT_BAR_FRAME) *
								toFIXED(1),
							FIXED_ONE / 64) +
					toFIXED(123)),
				IRIS_BAR_LENGTH(frame, IRIS_INNER_LEFT_BAR_FRAME), shade);
		}
		if(IRIS_BAR_SHOWN(frame, IRIS_INNER_LOWER_BAR_FRAME)) {
			s32 x = toFIXED(4);
			s16 shade = IRIS_BAR_SHADE(frame, IRIS_INNER_LOWER_BAR_FRAME);

			if(shade < 0 || shade > CANVAS_LEVEL_BRIGHTEST)
				shade = CANVAS_LEVEL_BRIGHTEST;
			x -= syMulFX(IRIS_BAR_LENGTH(frame, IRIS_INNER_LOWER_BAR_FRAME) *
							 toFIXED(1),
						 toFIXED(1) / 64);
			btEffectDrawDiagonalCanvasLine(
				FIXEDtoINT(x), 127,
				IRIS_BAR_LENGTH(frame, IRIS_INNER_LOWER_BAR_FRAME), shade);
		}
		if(IRIS_BAR_SHOWN(frame, IRIS_OUTER_LEFT_BAR_FRAME)) {
			s16 shade = IRIS_BAR_SHADE(frame, IRIS_OUTER_LEFT_BAR_FRAME);

			if(shade < 0 || shade > CANVAS_LEVEL_BRIGHTEST)
				shade = CANVAS_LEVEL_BRIGHTEST;
			btEffectDrawDiagonalCanvasLine(
				0,
				FIXEDtoINT(
					syMulFX(IRIS_BAR_LENGTH(frame, IRIS_OUTER_LEFT_BAR_FRAME) *
								toFIXED(1),
							FIXED_ONE * 3 / 128) +
					toFIXED(120)),
				IRIS_BAR_LENGTH(frame, IRIS_OUTER_LEFT_BAR_FRAME), shade);
		}
		if(IRIS_BAR_SHOWN(frame, IRIS_OUTER_LOWER_BAR_FRAME)) {
			s32 x = toFIXED(6);
			s16 shade = IRIS_BAR_SHADE(frame, IRIS_OUTER_LOWER_BAR_FRAME);

			if(shade < 0 || shade > CANVAS_LEVEL_BRIGHTEST)
				shade = CANVAS_LEVEL_BRIGHTEST;
			x -= syMulFX(IRIS_BAR_LENGTH(frame, IRIS_OUTER_LOWER_BAR_FRAME) *
							 toFIXED(1),
						 toFIXED(3) / 128);
			btEffectDrawDiagonalCanvasLine(
				FIXEDtoINT(x), 127,
				IRIS_BAR_LENGTH(frame, IRIS_OUTER_LOWER_BAR_FRAME), shade);
		}

		/* The healing: rings turning about the centre, then the great glow. */
		if(IRIS_DURING(frame, IRIS_HEAL_FRAME, IRIS_HEAL_FRAME + 110)) {
			s32 sinT = sySin(TRIG_TURN * 15 / 32);
			s32 cosT = syCos(TRIG_TURN * 15 / 32);

			for(j = 0; j != IRIS_HEAL_RINGS * IRIS_HEAL_RING_GAP;
				j += IRIS_HEAL_RING_GAP) {
				if(IRIS_HEAL_FRAME - 1 + j < frame &&
				   frame < IRIS_HEAL_FRAME + IRIS_HEAL_RING_FRAMES + j) {
					s32 t = frame - j - (IRIS_HEAL_FRAME - 1);
					s32 ry = t >> 1;
					s32 tile =
						BattleEffect_MotePictures[0] + t / 4 * MOTE_SET_BYTES;

					for(i = 0; i != IRIS_HEAL_RING_POINTS; i++) {
						s32 a = i << 9;
						s32 sx = sySin(a) * (ry >> 1);
						s32 sy = syCos(a) * ry;
						s32 px = syMulFX(sx, cosT) - syMulFX(sy * 2, sinT);
						s32 py = syMulFX(sx, sinT) + syMulFX(sy * 2, cosT);
						s32 rowY;

						px = (FIXEDtoINT(px)) + (j >> 1) + CANVAS_SIZE / 2;
						rowY = (FIXEDtoINT(py)) - (j >> 1) + 63;
						draw[0](canvas, state->sheet + tile, px, rowY, 1, 2);
					}
				}
			}
		}
		/* The canvas flashes white for three frames after the glow is made, and
		 */
		/* once before the tall ring. */
		if(IRIS_DURING(frame, IRIS_GLOW_FRAME + 1, IRIS_GLOW_FRAME + 3) ||
		   frame == IRIS_TALL_RING_FRAME - 2)
			IWRAM_FILL_WORDS(canvas, CANVAS_BYTES,
							 CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		if(frame == IRIS_GLOW_FRAME) {
			s32 white;
			btEffectBuildSteepShadeRampWave(state->sheet + IRIS_CONE_TEXTURE,
											16, IRIS_CONE_TEXTURE_SIZE,
											FIXED_ONE);
			btEffectDrawShadedCircles(state->sheet + IRIS_TEXTURES +
										  IRIS_TEXTURE_BYTES / 2,
									  IRIS_TEXTURE_WIDTH, FIXED_ONE);
			/* The glow: the shaded circle's half laid into the top half's left
			 * side */
			/* and mirrored across, then the top half mirrored down; each later
			 */
			/* texture is the same capped eight levels dimmer. */
			for(i = 0; i != IRIS_TEXTURE_HEIGHT / 2; i++) {
				syDMAXCopy(state->sheet + (i << 4) + IRIS_TEXTURES +
							   IRIS_TEXTURE_BYTES / 2,
						   state->sheet + (i << 5) + IRIS_TEXTURES,
						   DMA_COPY_WORDS(8), REG_DMA3);
				for(j = 0; j != IRIS_TEXTURE_WIDTH / 2; j++)
					state->sheet[IRIS_TEXTURES + IRIS_TEXTURE_WIDTH / 2 +
								 i * IRIS_TEXTURE_WIDTH + j] =
						state->sheet[IRIS_TEXTURES + IRIS_TEXTURE_WIDTH / 2 -
									 1 + i * IRIS_TEXTURE_WIDTH - j];
			}
			for(i = 0; i != IRIS_TEXTURE_HEIGHT / 2; i++)
				syDMAXCopy(state->sheet + IRIS_TEXTURES +
							   IRIS_TEXTURE_BYTES / 2 - IRIS_TEXTURE_WIDTH -
							   (i << 5),
						   state->sheet + IRIS_TEXTURES +
							   IRIS_TEXTURE_BYTES / 2 + (i << 5),
						   DMA_COPY_WORDS(16), REG_DMA3);
			white = CANVAS_LEVEL_BRIGHTEST + 1;
			for(i = 1; i != IRIS_GLOW_TEXTURES; i++) {
				u8 *src = state->sheet + IRIS_TEXTURES;
				u8 *out = state->sheet + IRIS_TEXTURES + i * IRIS_TEXTURE_BYTES;
				u8 v;

				j = 0;
				v = white - i * 8;
				for(; j != IRIS_TEXTURE_BYTES; j++) {
					if(v < *src)
						*out = v;
					else
						*out = *src;
					out++;
					src++;
				}
			}
		}
		/* The wide arc of motes, spreading with the cone. */
		if(IRIS_DURING(frame, IRIS_CONE_FRAME, IRIS_CONE_FRAME + 30)) {
			s32 sinT = sySin(TRIG_TURN * 11 / 32);
			s32 cosT = syCos(TRIG_TURN * 11 / 32);
			s32 t = frame - (IRIS_CONE_FRAME - 1);
			s32 tile = BattleEffect_MotePictures[IRIS_SWEEP_MOTE - 1] +
					   t / 4 * MOTE_SET_BYTES;

			for(i = 0; i != IRIS_WIDE_ARC_MOTES; i++) {
				s32 a = i * IRIS_SWEEP_STEP;
				s32 sx = sySin(a) * (t << 1);
				s32 sy = syCos(a) * t;
				s32 px = FIXEDtoINT(syMulFX(sx, cosT) - syMulFX(sy, sinT));
				s32 py = FIXEDtoINT(syMulFX(sx, sinT) + syMulFX(sy, cosT));

				draw[0](canvas, state->sheet + tile, px + 101, py + 26,
						IRIS_SWEEP_MOTE, 2 * IRIS_SWEEP_MOTE);
			}
		}
		/* The tall ring of motes. */
		if(IRIS_DURING(frame, IRIS_TALL_RING_FRAME,
					   IRIS_TALL_RING_FRAME + 46)) {
			FIXED sinT = sySin(TRIG_TURN * 29 / 32);
			FIXED cosT = syCos(TRIG_TURN * 7 / 8);
			s32 t = frame - (IRIS_TALL_RING_FRAME - 1);
			s32 tile = BattleEffect_MotePictures[IRIS_SWEEP_MOTE - 1] +
					   t / 6 * MOTE_SET_BYTES;

			for(i = 0; i != IRIS_TALL_RING_MOTES; i++) {
				s32 a = i * IRIS_SWEEP_STEP;
				FIXED sx = sySin(a) * (t >> 1);
				FIXED sy = syCos(a) * (t << 1);
				s32 px = FIXEDtoINT(syMulFX(sx, cosT) - syMulFX(sy, sinT));
				s32 py = FIXEDtoINT(syMulFX(sx, sinT) + syMulFX(sy, cosT));

				draw[0](canvas, state->sheet + tile, px + 93, py + 32,
						IRIS_SWEEP_MOTE, 2 * IRIS_SWEEP_MOTE);
			}
		}
		/* The cone of light, stretching out to its full length over 64 frames.
		 */
		if(frame >= IRIS_CONE_FRAME) {
			void *verts = syHeapAllocEwram(64);
			RENDER_MODEL *model = btRenderAllocateModelList(1);
			FIXED place[3];
			s32 grow[3];
			s32 s;

			coneTexture.logWidth = 5;
			coneTexture.logHeight = 5;
			coneTexture.pixels = state->sheet + IRIS_CONE_TEXTURE;
			syMatrixResetUnit();
			place[0] = toFIXED(32);
			place[1] = -toFIXED(24);
			place[2] = 0;
			syMatrixTranslateVector(place);
			syRotZ(TRIG_TURN * 19 / 32);
			grow[0] = FIXED_ONE;
			grow[1] = IRIS_CONE_GROWTH(frame - (IRIS_CONE_FRAME - 1));
			if(grow[1] > FIXED_ONE)
				grow[1] = FIXED_ONE;
			grow[2] = 0;
			syMatrixScaleVector(grow);
			model->kind = MODEL_TEXTURED_MAX;
			model->mesh = Iris_ConeMesh;
			model->texture = &coneTexture;
			model->vertices = verts;
			btRenderTransformVertices(Iris_ConeCorners, verts, 8);
			btRenderDrawModelList(model);
			syHeapRelease(model);
			syHeapRelease(verts);
			s = FindFreePetal();
			if(s != -1) {
				petals->life[s] = (syRandom() & 15) << 15;
				petals->x[s] = toFIXED(239);
				petals->y[s] = INTtoFIXED(syRandom() & 255);
				petals->vx[s] = -(syRandom() & 31) << 12;
				petals->vy[s] = ((syRandom() & 31) - 16) << 12;
			}
			/* Every four frames a mote flies out along the cone, one to two and
			 * a */
			/* half pixels a frame, and a glow drifts down and to the left. */
			if(!(frame & 3)) {
				s32 n = FindFreeParticle();

				if(n != -1) {
					s32 speed = syRandom() & 3;

					state->particles[n].x = toFIXED(96);
					state->particles[n].y =
						INTtoFIXED(syRandom() & 7) + toFIXED(36);
					state->particles[n].z = frame;
					state->particles[n].velX =
						syMulFX(IRIS_HALF_PIXELS_FX(speed + 2),
								sySin(TRIG_TURN * 19 / 32));
					state->particles[n].velY =
						-syMulFX(IRIS_HALF_PIXELS_FX(speed + 2),
								 syCos(TRIG_TURN * 19 / 32));
					state->particles[n].variant =
						(syRandom() & 3) | IRIS_MOTE_FADING;
				}
				n = FindFreeParticle();
				if(n != -1) {
					state->particles[n].x =
						INTtoFIXED(syRandom() & 31) + toFIXED(96);
					state->particles[n].y = INTtoFIXED(syRandom() & 31);
					state->particles[n].z = frame;
					state->particles[n].velX = -((syRandom() & 15) << 13);
					state->particles[n].velY = (syRandom() & 15) << 14;
					state->particles[n].velZ = toFIXED(1) / 2;
					state->particles[n].variant = ((syRandom() & 3) + 1) |
												  IRIS_MOTE_GLOWING |
												  IRIS_MOTE_FADING;
				}
			}
		}
		/* From the tall ring on the battle background shakes up and down. */
		if(frame >= IRIS_TALL_RING_FRAME) {
			if(frame & 2)
				BgScrollWork[1].y = BgScrollWork[1].y + (frame & 1) + 2;
			else
				BgScrollWork[1].y = BgScrollWork[1].y - (frame & 1) - 2;
		}
		/* At the end the battle background's colours step to white. */
		if(frame >= IRIS_WHITE_OUT_FRAME) {
			{
				u16 *fade = FADE_PLTT;

				for(i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
					s32 c = *fade;
					s32 b = RGB_BLUE((u16)c);
					s32 g = RGB_GREEN((u16)c);
					s32 r = RGB_RED(c);

					if(r < RGB_LEVEL_MAX)
						r++;
					if(g < RGB_LEVEL_MAX)
						g++;
					if(b < RGB_LEVEL_MAX)
						b++;
					{
						u16 v = RGB(r, g, b);

						*fade++ = v;
					}
				}
			}
		}
		frame++;
		EFFECT_PRESENT_CANVAS(state);
	}

	camera[IRIS_CAMERA_YAW] = savedYaw;
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	btIrisRestoreBattle();
	IWRAM_FILL_WORDS(canvas, CANVAS_BYTES, 0);
	while(((volatile DMA_REGISTERS *)REG_DMA3)->ctrl & DMA_BUSY) {
	}
	EFFECT_PRESENT_CANVAS(state);
	REG_BG2PA = Q_8_8(0.5);
	REG_BG2X = 0;
	BgScrollWork[1].x = savedScroll;
	for(i = 0; i != IRIS_PETALS; i++)
		fdSprDestroyStub(objects[i]);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#undef MOLOCH_FRAMES
#undef MOLOCH_SKIP_TO
#undef MOLOCH_MOTES
#undef MOLOCH_FLAMES
#undef MOLOCH_FLAME_LEFT
#undef MOLOCH_FIRST_EMBER
#undef MOLOCH_EMBERS
#undef MOLOCH_SPRITES
#undef MOLOCH_FLAME_LIFE
#undef MOLOCH_RING_TEXTURE
#undef MOLOCH_WHITENING_LEVEL
#undef IRIS_MOTE_FADES
#undef IRIS_DROPS
#undef IRIS_DROP_WIDTH
#undef IRIS_DROP_HEIGHT
#undef IRIS_SMALL_DROPS
#undef IRIS_SMALL_DROP_WIDTH
#undef IRIS_SMALL_DROP_HEIGHT
#undef IRIS_DROP_PICTURES
#undef IRIS_TEXTURES
#undef IRIS_TEXTURE_WIDTH
#undef IRIS_TEXTURE_HEIGHT
#undef IRIS_TEXTURE_BYTES
#undef IRIS_PILLAR_TEXTURE
#undef IRIS_PILLAR_TEXTURE_SIZE
#undef IRIS_PILLAR_CIRCLE
#undef IRIS_LARGE_RING_WIDTH
#undef IRIS_LARGE_RING_HEIGHT
#undef IRIS_SMALL_RING_WIDTH
#undef IRIS_SMALL_RING_HEIGHT
#undef IRIS_DOT_WIDTH
#undef IRIS_DOT_HEIGHT
#undef IRIS_SPRAY_WIDTH
#undef IRIS_SPRAY_HEIGHT
#undef IRIS_PETALS
#undef IRIS_PETAL_FRAMES
#undef IRIS_MOTES
#undef IRIS_STREAKS
#undef IRIS_STREAK_LENGTH
#undef IRIS_GODDESS_COLUMNS
#undef IRIS_GODDESS_ROWS
#undef IRIS_GODDESS_SPRITES
#undef IRIS_GODDESS_TILE
#undef IRIS_FADE_COLORS
#undef IRIS_FADE_TARGET_RED
#undef IRIS_FADE_TARGET_GREEN
#undef IRIS_FADE_TARGET_BLUE
#undef IRIS_PETAL_STATE
#undef IRIS_SCENE
#undef IRIS_STREAK_POINTS
#undef STREAK
#undef FADE_PLTT
