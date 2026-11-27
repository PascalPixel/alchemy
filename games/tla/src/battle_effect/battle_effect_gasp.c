/* @file */
/* The body that sixteen abilities' effects share, from Gasp to Demon Eye. */
/* Part of battle_effect.c. */

/* Where the small Djinni sprite sheet is decoded in the effect's pictures, as
 */
/* The Broken Seal places it. */
#define GASP_DJINNI_PICTURE 26048
/* The small Djinni: a 20 x 40 picture drawn over the canvas. */
#define FIGURE(state) ((state)->sheet + GASP_DJINNI_PICTURE)
/* Sets the transform for one of Beat Dance's and Clarion Cry's spinning notes:
 */
/* at its position, half as wide (the canvas holds every other column), turned
 */
/* by z. */
#define PLACE_PARTICLE(p)                                                      \
	do {                                                                       \
		syInitMatrix();                                                        \
		syTranslate((p)->x - toFIXED(64), (p)->y - toFIXED(64), 0);            \
		syScale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);                          \
		syRotZ((p)->z);                                                        \
	} while(0)

/* The last three particles: the point a mode centres on (Shine's Djinni in */
/* flight, Emu's target), Shine's fall towards the target, and where its */
/* flash strikes. */
#define GASP_CENTRE 63
#define GASP_FALL	62
#define GASP_FLASH	61
/* The motes of Djinn Counter's blow and of Reflux's and Counter-rush's swirl:
 */
/* every particle but the last. */
#define GASP_MOTES 63
/* Mold's 40x40 Djinni is copied to the middle of a 64x64 cel. */
#define GASP_DJINNI_CEL		   64
#define GASP_DJINNI_CEL_MARGIN ((GASP_DJINNI_CEL - DJINNI_WIDTH) / 2)
/* Call Dullahan's four bolts, LightningBoltSheet's pictures, each copied into
 */
/* a 32x128 cel GASP_BOLT_CEL_INSET pixels in from its left. */
#define GASP_BOLT_PICTURE_COUNT 4
#define GASP_BOLT_CEL_WIDTH		32
#define GASP_BOLT_CEL_HEIGHT	128
#define GASP_BOLT_CEL_INSET		((GASP_BOLT_CEL_WIDTH - LIGHTNING_BOLT_WIDTH) / 2)
#define GASP_BOLT_CEL_BYTES		(GASP_BOLT_CEL_WIDTH * GASP_BOLT_CEL_HEIGHT)

/* Where VenusDjinnSheet, CornerGlowSheet and FireRingTexture are decoded in */
/* the effect's pictures. */
#define GASP_FAMILY_VENUS_DJINN_PICTURES 4096
#define GASP_FAMILY_CORNER_GLOW_PICTURES 16384
#define GASP_FIRE_RING					 12416

/* The textures the model layer turns, 32 by 32: Gasp's two burning skulls, */
/* and the three music notes of Lull, Beat Dance and Clarion Cry. */
#define GASP_TEXTURE_BYTES (32 * 32)
/* The particles Gasp and Shine set up at their start, more than either */
/* draws: Gasp's GASP_SKULLS and Shine's GASP_SHINE_RAYS. */
#define GASP_SEEDS 16

/* Gasp and Lull: the small Djinni sways GASP_DJINNI_SWAY pixels either side */
/* and bobs, a 32nd of a turn a frame. Gasp's burning skulls rise one every 8 */
/* frames, each for GASP_SKULL_FRAMES before it fades. */
#define GASP_DJINNI_SWAY  20
#define GASP_SKULLS		  6
#define GASP_SKULL_FRAMES 32
#define GASP_SKULL_SCALE  (FIXED_ONE * 1875 / 4096)

/* Reflux: the Djinni circles wider until GASP_REFLUX_RADIUS. Reflux and */
/* Counter-rush: of GASP_MOTES set up, GASP_SWIRL_MOTES close in on the swirl,
 */
/* each for GASP_SWIRL_MOTE_FRAMES, as the swirl grows to GASP_SWIRL_SCALE. */
#define GASP_REFLUX_RADIUS	   12
#define GASP_SWIRL_MOTES	   48
#define GASP_SWIRL_MOTE_FRAMES 32
#define GASP_SWIRL_SCALE	   (FIXED_ONE * 375 / 512)
/* The swirl grows to its full scale over its first 32 frames, and shrinks */
/* away twice as fast. */
#define GASP_SWIRL_GROWTH (GASP_SWIRL_SCALE / 32)
#define GASP_SWIRL_SHRINK (GASP_SWIRL_SCALE / 16)

/* Shine: the Djinni glides a GASP_SHINE_GLIDE-th of the way to over the user */
/* a frame, then falls onto the target a GASP_SHINE_FALL-th of the way more */
/* each frame, turning rays of light about it. Each ray's texture scrolls two */
/* texels a frame, seven behind the last ray's, within the 32 texels its mesh */
/* spans. */
#define GASP_SHINE_GLIDE		   40
#define GASP_SHINE_FALL			   32
#define GASP_SHINE_RAYS			   8
#define GASP_SHINE_RAY_FRAMES	   32
#define GASP_SHINE_RAY_SCROLL_MASK 0x1f

/* Lull: a ring of music notes, leaning back GASP_LULL_TILT (about 100 */
/* degrees) and righting itself a 1024th of a turn a frame, turning */
/* about 2 degrees a frame, fades after GASP_LULL_RING_FRAMES. */
#define GASP_LULL_NOTES		  12
#define GASP_LULL_RING_FRAMES 64
#define GASP_LULL_TILT		  18288

/* Mold: the Djinni's sprite shrinks GASP_MOLD_SHRINK a frame (about 7 */
/* percent of its size); for its first */
/* GASP_MOLD_SLOW_FADE_FRAMES the canvas keeps three quarters of each pixel, */
/* then half. */
#define GASP_MOLD_SHRINK		   4592
#define GASP_MOLD_SLOW_FADE_FRAMES 24

/* Emu: puffs of dust trail the bird, one each two frames for its first ten; */
/* the blow scatters motes, each at least GASP_EMU_MOTE_SPEED_MIN fast. */
#define GASP_EMU_PUFFS			32
#define GASP_EMU_MOTES			256
#define GASP_EMU_MOTE_SPEED_MIN 64

/* Beat Dance and Clarion Cry: the notes that fly from their unit, one every */
/* two frames, each for GASP_NOTE_FRAMES before it fades, at least */
/* GASP_NOTE_SPEED_MIN fast. */
#define GASP_NOTE_SEEDS		32
#define GASP_NOTES			12
#define GASP_NOTE_FRAMES	24
#define GASP_NOTE_SPEED_MIN 128

/* Call Dullahan: the sparks his bolt scatters, each at least */
/* GASP_DULLAHAN_SPARK_SPEED_MIN fast, and the first of them drawn; each bolt */
/* flies out GASP_DULLAHAN_REACH steps, turning GASP_DULLAHAN_SWING (about 265
 */
/* degrees) as it goes. */
#define GASP_DULLAHAN_SPARKS		  128
#define GASP_DULLAHAN_SPARKS_DRAWN	  64
#define GASP_DULLAHAN_SPARK_SPEED_MIN 32
#define GASP_DULLAHAN_REACH			  80
#define GASP_DULLAHAN_SWING			  48152

/* Cage: its rings lean, each a further eighth of a turn, and close on the */
/* target from GASP_CAGE_CLOSE_FRAME, each two frames after the last. */
#define GASP_CAGE_TILT		  4000
#define GASP_CAGE_CLOSE_FRAME 16

/* Element Swap: a mote's distance from the user is kept in sixteenths of a */
/* pixel, GASP_SWAP_MOTE_SHIFT bits of fraction. */
#define GASP_SWAP_MOTE_SHIFT 4

/* Psy Boost: each of its two rings shrinks onto the user over */
/* GASP_PSY_BOOST_RING_FRAMES. */
#define GASP_PSY_BOOST_RING_FRAMES 16

/* The shared body of sixteen abilities' effects, named for the first; the */
/* wrappers in rime.c pick a GASP_MODE: Gasp, Reflux, Djinn Counter, Mold and */
/* Meld, Shine, Lull, Counter-rush, Demon Eye, Emu and its kind's four other */
/* abilities, Call Dullahan and Formina Sage, Beat Dance, Element Swap, Cage, */
/* Clarion Cry and Psy Boost. GASP_MODE_UNUSED has no caller. Each mode loads */
/* its own sprite sheets, then draws a canvas layer and a model layer every */
/* frame for the mode's length. */
void btEffectRunGaspFamily(BATTLE_EFFECT_PARM *args, s32 mode) {
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;
	u8 *canvas = Ram_HeapSlots->battleCanvas;
	/* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
	 * frame reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE draw[2];
	s32 frames;
	u8 *sprites = Ram_HeapSlots->battleSheet;
	BATTLE_CAMERA *camera = Ram_HeapSlots->cameraState;
	u8 *cells = Ram_MapCellBuffer;
	RENDER_MODEL *model;
	void *verts;
	MODEL_TEXTURE texture;
	s32 screen[3];
	s32 other[3];
	s32 frame;
	s32 i;

	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	if(mode == GASP_MODE_GASP) {
		btEffectRunDjinniEntrance(args,
								  DJINNI_ENTRANCE(ELEMENT_JUPITER,
												  args->side ^ 1,
												  DJINNI_ANCHOR_TARGET_X),
								  screen, other);
		btResourceLoadAndDecompress(RESOURCE_JUPITER_DJINN_SMALL_SHEET,
									FIGURE(state), 1, 1);
		btResourceLoadAndDecompress(RESOURCE_BURNING_SKULL_SHEET, state->sheet,
									0, 0);
		REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
					 BGCNT_SCREEN_BASE(7);
		frames = 84;
	}
	else if(mode == GASP_MODE_UNUSED) {
		btEffectRunDjinniEntrance(args,
								  DJINNI_ENTRANCE(ELEMENT_MERCURY,
												  args->side ^ 1,
												  DJINNI_ANCHOR_TARGET_X),
								  screen, other);
		btResourceLoadAndDecompress(RESOURCE_LIGHT_BURST_SHEET, state->sheet, 1,
									0);
		btResourceLoadAndDecompress(RESOURCE_MERCURY_DJINN_SMALL_SHEET,
									FIGURE(state), 1, 1);
		LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
		frames = 48;
	}
	else if(mode == GASP_MODE_MOLD) {
		s32 j;
		s32 k;

		btEffectRunDjinniEntrance(args,
								  DJINNI_ENTRANCE(ELEMENT_VENUS, args->side ^ 1,
												  DJINNI_ANCHOR_HIGH_TARGET_X),
								  screen, other);
		btResourceLoadAndDecompress(RESOURCE_VENUS_DJINN_SMALL_SHEET,
									FIGURE(state), 1, 0);
		btResourceLoadAndDecompress(
			RESOURCE_VENUS_DJINN_SHEET,
			state->sheet + GASP_FAMILY_VENUS_DJINN_PICTURES, 1, 0);
		syWordsClear(state->sheet, GASP_DJINNI_CEL * GASP_DJINNI_CEL);
		for(k = 0; k != DJINNI_HEIGHT; k++)
			for(j = 0; j != DJINNI_WIDTH; j++)
				state->sheet[(k + GASP_DJINNI_CEL_MARGIN) * GASP_DJINNI_CEL +
							 GASP_DJINNI_CEL_MARGIN + j] =
					state->sheet[GASP_FAMILY_VENUS_DJINN_PICTURES +
								 k * DJINNI_WIDTH + j];
		frames = 42;
	}
	else if(mode == GASP_MODE_LULL) {
		btEffectRunDjinniEntrance(args,
								  DJINNI_ENTRANCE(ELEMENT_JUPITER,
												  args->side ^ 1,
												  DJINNI_ANCHOR_CENTRE),
								  screen, other);
		btResourceLoadAndDecompress(RESOURCE_JUPITER_DJINN_SMALL_SHEET,
									FIGURE(state), 1, 1);
		btResourceLoadAndDecompress(RESOURCE_MUSIC_NOTE_SHEET, state->sheet, 0,
									0);
		frames = 80;
		sySndPlayStub(MUS_POISON_CURED);
	}
	else if(mode == GASP_MODE_BEAT_DANCE) {
		btResourceLoadAndDecompress(RESOURCE_MUSIC_NOTE_SHEET, state->sheet, 0,
									0);
		LOAD_BG_PALETTE(RESOURCE_MARS_DJINN_SMALL_SHEET);
		frames = 64;
	}
	else if(mode == GASP_MODE_CLARION_CRY) {
		btResourceLoadAndDecompress(RESOURCE_MUSIC_NOTE_SHEET, state->sheet, 0,
									0);
		LOAD_BG_PALETTE(RESOURCE_MERCURY_DJINN_SMALL_SHEET);
		state->fadeFrm = BACKGROUND_DARKEN_FRAMES;
		state->fadeStep = 0;
		frames = 64;
	}
	else if(mode == GASP_MODE_CAGE) {
		btResourceLoadAndDecompress(RESOURCE_SMOKE_BAND_SHEET, state->sheet, 1,
									1);
		frames = 40;
	}
	else if(mode == GASP_MODE_ELEMENT_SWAP) {
		btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, cells, 0, 0);
		btResourceLoadAndDecompress(
			RESOURCE_CORNER_GLOW_SHEET,
			state->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES, 1, 1);
		btPictureMirrorIntoQuadrants(state->sheet +
										 GASP_FAMILY_CORNER_GLOW_PICTURES,
									 state->sheet, 64, 64);
		state->fadeFrm = BACKGROUND_DARKEN_FRAMES;
		state->fadeStep = 0;
		frames = 48;
	}
	else if(mode == GASP_MODE_PSY_BOOST) {
		btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, cells, 0, 0);
		btResourceLoadAndDecompress(
			RESOURCE_CORNER_GLOW_SHEET,
			state->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES, 1, 1);
		btPictureMirrorIntoQuadrants(state->sheet +
										 GASP_FAMILY_CORNER_GLOW_PICTURES,
									 state->sheet, 64, 64);
		state->fadeFrm = BACKGROUND_DARKEN_FRAMES;
		state->fadeStep = 0;
		frames = 48;
	}
	else if(mode == GASP_MODE_SHINE) {
		btEffectRunDjinniEntrance(
			args,
			DJINNI_ENTRANCE(ELEMENT_MARS, args->side, DJINNI_ANCHOR_LOW_CENTRE),
			screen, other);
		REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
					 BGCNT_SCREEN_BASE(7);
		btResourceLoadAndDecompress(RESOURCE_MARS_DJINN_SMALL_SHEET,
									FIGURE(state), 1, 0);
		btResourceLoadAndDecompress(RESOURCE_FLAME_BAND_TEXTURE, state->sheet,
									0, 0);
		btResourceLoadAndDecompress(
			RESOURCE_GLOW_BAND_TEXTURE_A,
			state->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES, 0, 0);
		btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, cells, 0, 0);
		LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
		frames = 82;
	}
	else if(mode == GASP_MODE_CALL_DULLAHAN) {
		s32 j;
		s32 k;

		btResourceLoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET, cells, 1, 1);
		syWordsClearEntry(state->sheet,
						  GASP_BOLT_PICTURE_COUNT * GASP_BOLT_CEL_BYTES);
		for(i = 0; i != GASP_BOLT_PICTURE_COUNT; i++)
			for(k = 0; k != LIGHTNING_BOLT_HEIGHT; k++)
				for(j = 0; j != LIGHTNING_BOLT_WIDTH; j++)
					state->sheet[GASP_BOLT_CEL_INSET + i * GASP_BOLT_CEL_BYTES +
								 k * GASP_BOLT_CEL_WIDTH + j] =
						cells[i * LIGHTNING_BOLT_PICTURE_BYTES +
							  k * LIGHTNING_BOLT_WIDTH + j];
		LOAD_BG_PALETTE(RESOURCE_VIOLET_PALETTE_F);
		frames = 60;
	}
	else if(mode == GASP_MODE_COUNTER_RUSH) {
		btResourceLoadAndDecompress(RESOURCE_SWIRL_TEXTURE, state->sheet, 0, 0);
		LOAD_BG_PALETTE(RESOURCE_MERCURY_DJINN_SMALL_SHEET);
		frames = 76;
	}
	else if(mode == GASP_MODE_DEMON_EYE) {
		btResourceLoadAndDecompress(
			RESOURCE_CORNER_GLOW_SHEET,
			state->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES, 1, 1);
		btPictureMirrorIntoQuadrants(state->sheet +
										 GASP_FAMILY_CORNER_GLOW_PICTURES,
									 state->sheet, 64, 64);
		REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
					 BGCNT_SCREEN_BASE(7);
		frames = 16;
	}
	else if(mode == GASP_MODE_EMU) {
		u8 *p;

		REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
					 BGCNT_SCREEN_BASE(7);
		btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE,
									state->sheet + GASP_FIRE_RING, 0, 0);
		btResourceLoadAndDecompress(RESOURCE_SMOKE_SHEET, state->sheet, 1, 0);
		for(i = 0, p = state->sheet; i != 6 * PICTURE_BYTES(SMOKE_PUFF);
			i++, p++) {
			*p <<= 1;
			if(*p > CANVAS_LEVEL_BRIGHTEST)
				*p = CANVAS_LEVEL_BRIGHTEST;
		}
		if(args->variant == EFFECT_VARIANT_STRONGEST) {
			LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
		}
		else {
			LOAD_BG_PALETTE(RESOURCE_BLAST_SHEET);
		}
		btEffectSetApproachMotion(args->user, args->tgts[0], 10, 80);
		frames = 41;
	}
	else {
		if(mode == GASP_MODE_REFLUX) {
			btEffectRunDjinniEntrance(
				args,
				DJINNI_ENTRANCE(ELEMENT_MARS, args->side ^ 1,
								DJINNI_ANCHOR_OVER_TARGET),
				screen, other);
			frames = 76;
		}
		else {
			state->fadeFrm = BACKGROUND_DARKEN_FRAMES;
			state->fadeStep = 0;
			frames = 70;
		}
		btResourceLoadAndDecompress(RESOURCE_SWIRL_TEXTURE, state->sheet, 0, 0);
		btResourceLoadAndDecompress(RESOURCE_MARS_DJINN_SMALL_SHEET,
									FIGURE(state), 1, 1);
		LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
	}
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprites, 0, 0);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	if(mode == GASP_MODE_GASP || mode == GASP_MODE_LULL)
		sySetEvent(btEffectArmBg2AffineHBlankDma, TASK_PASS_VBLANK);
	sySetEvent(btEffectTransferCanvas, TASK_PASS_VBLANK);
	if(mode == GASP_MODE_REFLUX || mode == GASP_MODE_COUNTER_RUSH ||
	   mode == GASP_MODE_MOLD) {
		if(args->side == EFFECT_SIDE_ENEMIES)
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
								EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
								BLITTER_ADDITIVE);
		else
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
								BLITTER_ADDITIVE);
	}
	else if(mode == GASP_MODE_DJINN_COUNTER || mode == GASP_MODE_SHINE) {
		if(args->side == EFFECT_SIDE_PARTY)
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
								EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
								BLITTER_ADDITIVE);
		else
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
								BLITTER_ADDITIVE);
	}
	else {
		if(args->side == EFFECT_SIDE_ENEMIES)
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
								EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X,
								BLITTER_MAX);
		else
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
								BLITTER_MAX);
	}
	draw[0] = Ram_HeapSlots->ramCode;

	for(frame = 0; frame != frames; frame++) {
		s32 a;
		s32 twice;
		s32 x;
		s32 y;

		/* The canvas layer. */
		if(mode == GASP_MODE_GASP) {
			s32 *lines = state->bg2xLines;

			for(i = 0; i != ARRAY_COUNT(state->bg2xLines); i++)
				*lines++ = (syRandom() & 31) << 4;
		}
		else if(mode == GASP_MODE_LULL) {
			s32 *lines = state->bg2xLines;

			for(i = 0; i != ARRAY_COUNT(state->bg2xLines); i++)
				*lines++ = CANVAS_WAVE_BG2X(
					toFIXED(6) -
					sySin(frame * (TRIG_TURN / 32) + i * (TRIG_TURN / 32)) * 6);
		}
		else if(mode == GASP_MODE_UNUSED && frame > 31) {
			u16 *pal = (u16 *)&BG_PLTT_COLOR(0, 1);

			for(i = 0; i != SHEET_PALETTE_COLORS - 1; i++, pal++) {
				s32 r = RGB_RED(*pal);
				s32 g = RGB_GREEN(*pal);
				s32 b = RGB_BLUE(*pal);

				if(frame > 40) {
					r += 4;
					g += 4;
					b += 4;
				}
				else {
					r += 2;
					g += 2;
					b += 2;
				}
				if(r > RGB_LEVEL_MAX)
					r = RGB_LEVEL_MAX;
				if(g > RGB_LEVEL_MAX)
					g = RGB_LEVEL_MAX;
				if(b > RGB_LEVEL_MAX)
					b = RGB_LEVEL_MAX;
				*pal = RGB(r, g, b);
			}
			syWordsFillEntry(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(1));
		}

		/* Djinn Counter: the Djinni streaks from the user to the target in four
		 */
		/* images, and the blow bursts into motes. */
		if(mode == GASP_MODE_DJINN_COUNTER) {
			SCR_POINT to;
			SCR_POINT from;

			btEffectPosProjectScaledBattler(args->user, &from);
			btEffectPosProjectScaledBattler(args->tgts[0], &to);
			from.x /= 2;
			to.x /= 2;
			if((u32)(frame - 32) <= 8) {
				s32 n = frame - 31;
				s32 dx;
				s32 head;
				s32 tail;
				s32 step;
				s32 acc;

				if(n > 8)
					n = 8;
				dx = to.x - from.x;
				head = from.x + GaspFamily_StreakSteps[n] * dx / 16;
				tail = from.x + GaspFamily_StreakSteps[n - 1] * dx / 16;
				for(i = 0, step = head - tail, acc = 0; i != 4;
					i++, acc += step)
					draw[0](canvas, FIGURE(state),
							tail + acc / 3 - SMALL_DJINNI_WIDTH / 2,
							from.y - SMALL_DJINNI_HEIGHT / 2,
							SMALL_DJINNI_WIDTH, SMALL_DJINNI_HEIGHT);
			}
			if(frame > 37) {
				if(frame == 38) {
					for(i = 0; i != GASP_MOTES; i++) {
						PARTICLE *p = &state->particles[i];
						s32 s;

						p->x = INTtoFIXED(to.x);
						p->y = INTtoFIXED(to.y);
						a = syRandom();
						s = (syRandom() & 511) + 256;
						p->velX = sySin(a) * s >> 8;
						p->velY = syCos(a) * s >> 7;
						p->variant = (syRandom() & 15) + 16;
					}
					EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
					btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_HURL);
					state->shakeFrm = 8;
					btEffectTintBattler(args->tgts[0], SPRITE_COLOR_WHITE,
										POSE_DAMAGE, 0, 8);
				}
				for(i = 0; i != GASP_MOTES; i++) {
					PARTICLE *p = &state->particles[i];

					if(p->variant >= 0) {
						s32 n = (p->variant >> 3) + 2;

						EFFECT_DRAW_MOTE(draw[0], canvas, sprites,
										 FIXED_INTEGER_PART(p->x),
										 FIXED_INTEGER_PART(p->y), n);
						btParticleAdvanceWithGravity2D(p, 60, toFIXED(1) / 64);
						p->variant--;
					}
				}
			}
		}
		else if(mode == GASP_MODE_LULL) {
			/* The small Djinni sways over the targets and rises from frame 45.
			 */
			s32 b = frame * (TRIG_TURN / 32);
			s32 x;
			s32 y;

			x = (FIXEDtoINT(sySin(b) * GASP_DJINNI_SWAY)) + screen[0] / 2 -
				SMALL_DJINNI_WIDTH / 2;
			y = (FIXEDtoINT(syCos(b) * 4)) + screen[1] - 24;
			if(frame > 44) {
				twice = frame * 2;
				y = y - twice + 88;
			}
			draw[0](canvas, FIGURE(state), x, y, SMALL_DJINNI_WIDTH,
					SMALL_DJINNI_HEIGHT);
			btEffectUpdatePhasePalette(frame, TRIG_TURN - DEGtoANG(120),
									   DEGtoANG(120), 0);
		}
		else if(mode == GASP_MODE_REFLUX || mode == GASP_MODE_COUNTER_RUSH) {
			/* Reflux's Djinni circles over the target; motes swirl in on it. */
			SCR_POINT target;

			if(mode == GASP_MODE_REFLUX) {
				s32 r = frame;

				x = screen[0] / 2 - SMALL_DJINNI_WIDTH / 2;
				y = screen[1] - SMALL_DJINNI_HEIGHT / 2;
				if(r > GASP_REFLUX_RADIUS)
					r = GASP_REFLUX_RADIUS;
				a = frame * (TRIG_TURN / 128);
				x -= FIXEDtoINT(sySin(a) * r * 2);
				y -= (FIXEDtoINT(syCos(a) * r)) - r;
				if(frame <= 61)
					draw[0](canvas, FIGURE(state), x, y, SMALL_DJINNI_WIDTH,
							SMALL_DJINNI_HEIGHT);
			}
			if(frame == 0)
				sySndPlayStub(SE_SWIRL);
			if(frame == 62)
				sySndPlayStub(SE_DJINNI_SET);
			if(frame == 0) {
				for(i = 0; i != GASP_MOTES; i++) {
					PARTICLE *p = &state->particles[i];

					p->z = syRandom();
					p->velZ = (syRandom() & 63) + 64;
					syRandom();
					p->variant = i + GASP_SWIRL_MOTE_FRAMES;
				}
			}
			btEffectPosProjectScaledBattler(args->tgts[0], &target);
			for(i = 0; i != GASP_SWIRL_MOTES; i++) {
				PARTICLE *p = &state->particles[i];

				if((u32)p->variant < GASP_SWIRL_MOTE_FRAMES) {
					s32 n = p->variant / 8 + 2;
					s32 x =
						(FIXEDtoINT(sySin(p->z) * p->velZ / 2)) + target.x / 2;
					s32 y = (FIXEDtoINT(syCos(p->z) * p->velZ)) + target.y;

					p->z += 512;
					p->velZ -= 4;
					if(p->velZ > 0)
						EFFECT_DRAW_MOTE(draw[0], canvas, sprites, x, y, n);
				}
				p->variant--;
			}
		}
		else if(mode == GASP_MODE_SHINE) {
			/* The Djinni glides up over the user, then falls on the target and
			 */
			/* strikes at frame 55; its place in the battle is in the velocity.
			 */
			PARTICLE *ball = &state->particles[GASP_CENTRE];
			SCR_POINT pos;
			s32 x;
			s32 y;

			if(frame == 0) {
				ball->velX = other[0];
				ball->velY = other[1];
				ball->velZ = other[2];
			}
			if(frame <= 47) {
				MOTION_OBJECT *user = btObjectGetStub(args->user)->obj;

				ball->velX += (user->x - other[0]) / GASP_SHINE_GLIDE;
				ball->velY += (toFIXED(60) - other[1]) / GASP_SHINE_GLIDE;
				ball->velZ += (user->z - other[2]) / GASP_SHINE_GLIDE;
			}
			else {
				MOTION_OBJECT *target = btObjectGetStub(args->tgts[0])->obj;
				PARTICLE *fall = &state->particles[GASP_FALL];

				if(frame == 48) {
					SET_POSITION(fall, 0, 0, 0);
					fall->velX = (target->x - ball->velX) / GASP_SHINE_FALL;
					fall->velY = -ball->velY / GASP_SHINE_FALL;
					fall->velZ = (target->z - ball->velZ) / GASP_SHINE_FALL;
				}
				ball->velX += fall->x;
				ball->velY += fall->y;
				ball->velZ += fall->z;
				fall->x += fall->velX;
				fall->y += fall->velY;
				fall->z += fall->velZ;
			}
			syInitMatrix();
			syMatrixSetLookAt(camera->eye, camera->focus);
			btEffectPosProjectPoint(&ball->velX, &pos);
			x = pos.x >> 1;
			pos.x = x;
			ball->x = x;
			y = pos.y;
			ball->y = y;
			draw[0](canvas, FIGURE(state), x - SMALL_DJINNI_WIDTH / 2,
					y - SMALL_DJINNI_HEIGHT / 2, SMALL_DJINNI_WIDTH,
					SMALL_DJINNI_HEIGHT);
			if(frame == 0)
				sySndPlayStub(SE_SPIN);
			if(frame == 55) {
				PARTICLE *flash = &state->particles[GASP_FLASH];

				flash->x = pos.x;
				flash->y = pos.y;
				EFFECT_SHOW_DAMAGE(SE_CRASH);
				btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_HURL);
				btEffectTintBattler(args->tgts[0], SPRITE_COLOR_WHITE,
									POSE_DAMAGE, 0, 24);
				state->shakeFrm = 16;
			}
			btCameraShake(8, 8);
		}
		else if(mode == GASP_MODE_GASP) {
			/* The small Djinni sways over the targets and rises from frame 49.
			 */
			s32 b = frame * (TRIG_TURN / 32);
			s32 x;
			s32 y;

			x = (FIXEDtoINT(sySin(b) * GASP_DJINNI_SWAY)) + screen[0] / 2 -
				SMALL_DJINNI_WIDTH / 2;
			y = (FIXEDtoINT(syCos(b) * 4)) + screen[1] - 24;
			if(frame > 48) {
				twice = frame * 2;
				y = y - twice + 96;
			}
			if(y > -SMALL_DJINNI_HEIGHT)
				draw[0](canvas, FIGURE(state), x, y, SMALL_DJINNI_WIDTH,
						SMALL_DJINNI_HEIGHT);
		}

		/* The model layer. */
		syInitMatrix();
		ProjectionWork.centerY = 104;
		verts = syHeapAllocEwram(1024);
		model = btRenderAllocateModelList(1);
		model->texture = &texture;
		model->vertices = verts;
		if(mode == GASP_MODE_GASP) {
			/* Burning skulls rise from the ground, swaying, one every 8 frames.
			 */
			if(frame == 0) {
				for(i = 0; i != GASP_SEEDS; i++) {
					PARTICLE *p = &state->particles[i];

					p->x = i * DEGtoANG(180);
					p->y = toFIXED(56);
					p->velX = (syRandom() & 511) + 512;
					p->velY = -((syRandom() & 127) + 612) << 7;
					p->variant = -i * 8;
				}
			}
			if(frame == 4)
				sySndPlayStub(SE_BEAST_CRY);
			if(frame == 48)
				EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
			texture.logWidth = 5;
			texture.logHeight = 5;
			model->kind = MODEL_SHADED_ADDITIVE;
			model->mesh = BattleEffect_SmallSquareMeshReversed;
			for(i = 0; i != GASP_SKULLS; i++) {
				PARTICLE *p = &state->particles[i];

				if(p->variant >= 0) {
					FIXED size = GASP_SKULL_SCALE;
					s32 shade = -16;

					if(p->variant <= 6)
						shade = p->variant * 8 + MODEL_SHADE_INVISIBLE;
					if(p->variant >= GASP_SKULL_FRAMES) {
						shade = (GASP_SKULL_FRAMES - p->variant) * 4;
						if(shade < MODEL_SHADE_INVISIBLE)
							continue;
					}
					model->shade = shade;
					texture.pixels = state->sheet + i % 2 * GASP_TEXTURE_BYTES;
					syInitMatrix();
					p->y += p->velY;
					p->x += p->velX;
					syTranslate(sySin(p->x) * 16 +
									INTtoFIXED(screen[0] / 2 - CANVAS_SIZE / 2),
								p->y, 0);
					syScale(size >> 1, size, size >> 1);
					syRotX(-DEGtoANG(90));
					btRenderTransformVertices(BattleEffect_GroundSquareCorners,
											  verts, 4);
					btRenderDrawModelList(model);
				}
				p->variant++;
			}
		}
		else if(mode == GASP_MODE_LULL) {
			/* A ring of music notes turns and spreads. */
			s32 k;

			texture.logWidth = 5;
			texture.logHeight = 5;
			model->kind = MODEL_SHADED_MAX;
			model->texture = &texture;
			model->mesh = BattleEffect_SmallSquareMeshReversed;
			model->vertices = verts;
			/* One ring: the loop that could start another every 32 frames runs
			 * once. */
			for(k = 0; k != 1; k++) {
				if(frame >= k * 32) {
					s32 t = frame - k * 32;
					s32 scale = t * (FIXED_ONE / 64) + FIXED_ONE / 16;
					s32 shade = 0;

					if(t >= GASP_LULL_RING_FRAMES) {
						shade = (GASP_LULL_RING_FRAMES - t) * 4;
						if(shade < MODEL_SHADE_INVISIBLE)
							continue;
					}
					model->shade = shade;
					syInitMatrix();
					syScale(scale >> 1, scale, scale);
					syRotZ(frame * (TRIG_TURN / 256) + TRIG_TURN * 3 / 4);
					syRotX(frame * (TRIG_TURN / 1024) - GASP_LULL_TILT);
					for(i = 0; i != GASP_LULL_NOTES; i++) {
						texture.pixels =
							state->sheet + i % 3 * GASP_TEXTURE_BYTES;
						syPushMatrix();
						syRotZ(frame * 356 + i * (TRIG_TURN / GASP_LULL_NOTES));
						syTranslate(0, toFIXED(116), 0);
						syScale(scale >> 1, scale >> 1, scale >> 1);
						btRenderTransformVertices(
							BattleEffect_GroundSquareCorners, verts, 4);
						syPopMatrix();
						btRenderDrawModelList(model);
					}
				}
			}
		}
		else if(mode == GASP_MODE_UNUSED) {
			/* A burst of light swells at each target. */
			model->shade = 0;
			texture.logWidth = 6;
			texture.logHeight = 6;
			model->mesh = BattleEffect_SquareMesh;
			model->kind = MODEL_SHADED_MAX;
			texture.pixels = state->sheet;
			for(i = 0; i != args->tgtCnt; i++) {
				SCR_POINT pos;

				btEffectPosProjectScaledBattler(args->tgts[i], &pos);
				syInitMatrix();
				syTranslate(INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48),
							0);
				syRotX(DEGtoANG(90));
				syRotY((frame << 9) - TRIG_TURN / 16);
				syScale(frame * 2596 >> 1, frame * 2596 >> 1,
						frame * 2596 >> 1);
				btRenderTransformVertices(BattleEffect_GroundSquareCorners,
										  verts, 4);
				btRenderDrawModelList(model);
			}
		}
		else if(mode == GASP_MODE_SHINE) {
			/* Rays of light turn about the Djinni as it glides; three rings
			 * flare at */
			/* the target as it strikes. */
			if(frame <= 47) {
				PARTICLE *ball;
				s32 twist;

				model->shade = -16;
				texture.logWidth = 5;
				texture.logHeight = 2;
				texture.pixels =
					state->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES;
				model->texture = &texture;
				model->kind = MODEL_SHADED_ADDITIVE;
				model->mesh = GaspFamily_StreakMesh;
				model->vertices = verts;
				if(frame == 0) {
					for(i = 0; i != GASP_SEEDS; i++) {
						PARTICLE *p = &state->particles[i];

						p->x = syRandom() & TRIG_ANGLE_MASK;
						p->y = (syRandom() & 255) + 128;
						if(syRandom() & 1)
							p->y = -p->y;
						p->variant = -i * 2;
					}
				}
				for(i = 0, ball = &state->particles[GASP_CENTRE],
				twist = frame * 2;
					i != GASP_SHINE_RAYS; i++) {
					PARTICLE *p = &state->particles[i];

					p->variant++;
					p->x += p->y * 2;
					model->scrollV =
						(twist + i * 7) & GASP_SHINE_RAY_SCROLL_MASK;
					if((u32)p->variant < GASP_SHINE_RAY_FRAMES) {
						syInitMatrix();
						syTranslate(INTtoFIXED(ball->x) - toFIXED(64),
									INTtoFIXED(ball->y) - toFIXED(64), 0);
						syScale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
						syRotZ(p->x);
						syScale(sySin(p->variant * (TRIG_TURN / 96)) * 3 +
									FIXED_ONE * 3 / 4,
								FIXED_ONE * 2, FIXED_ONE);
						btRenderTransformVertices(
							BattleEffect_TaperedBeamCorners, verts, 4);
						btRenderDrawModelList(model);
					}
				}
			}
			{
				SCR_POINT pos;

				btEffectPosProjectBattler(args->tgts[0], &pos);
				texture.logWidth = 7;
				texture.logHeight = 7;
				texture.pixels = cells;
				model->kind = MODEL_SHADED_ADDITIVE;
				model->texture = &texture;
				model->mesh = BattleEffect_LargeSquareMesh;
				model->vertices = verts;
				for(i = 0; i != 3; i++) {
					s32 t = frame - i * 4 - 55;

					if(t >= 0) {
						s32 scale = t * 3 * (FIXED_ONE / 16) + FIXED_ONE / 4;
						s32 shade = 0;

						if(t > 7)
							shade = (8 - t) * 8;
						model->shade = shade;
						if(shade > MODEL_SHADE_INVISIBLE) {
							syInitMatrix();
							syTranslate(INTtoFIXED(pos.x / 2 - 64),
										INTtoFIXED(pos.y - 64), 0);
							syScale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE / 2);
							syRotZ(i * (DEGtoANG(90)) + DEGtoANG(45));
							syRotX(-TRIG_TURN / 16);
							syMatrixScaleUniform(scale);
							btRenderTransformVertices(
								BattleEffect_GroundSquareCorners, verts, 4);
							btRenderDrawModelList(model);
						}
					}
				}
			}
		}
		else if(mode == GASP_MODE_MOLD) {
			/* The Djinni's sprite, twice, shrinks onto the target. */
			if((u32)(frame - 16) <= 64) {
				s32 t = frame - 16;
				s32 scale;
				SCR_POINT pos;

				if(t < GASP_MOLD_SLOW_FADE_FRAMES)
					state->transferValue = CANVAS_FADE_THREE_QUARTERS;
				else
					state->transferValue = CANVAS_FADE_HALF;
				texture.logWidth = 6;
				texture.logHeight = 6;
				model->mesh = BattleEffect_SquareMesh;
				model->kind = MODEL_SHADED_ADDITIVE;
				texture.pixels = state->sheet;
				btEffectPosProjectScaledBattler(args->tgts[0], &pos);
				for(i = 0, scale = t * -GASP_MOLD_SHRINK + FIXED_ONE; i != 2;
					scale += FIXED_ONE / 4, i++) {
					if(scale > 0) {
						s32 shade = i * (MODEL_SHADE_INVISIBLE / 4);

						if(shade > 0)
							shade = 0;
						model->shade = shade;
						syInitMatrix();
						syTranslate(INTtoFIXED(pos.x / 2 - 64),
									INTtoFIXED(pos.y - 48), 0);
						syRotX(TRIG_TURN * 3 / 4);
						syScale(scale, scale, scale);
						btRenderTransformVertices(
							BattleEffect_GroundSquareCorners, verts, 4);
						btRenderDrawModelList(model);
					}
				}
			}
			if(frame == 14) {
				sySndPlayStub(SE_POWER_UP);
			}
			if(frame == 24) {
				btEffectTintBattler(args->tgts[0], SPRITE_COLOR_WHITE,
									TINT_KEEP, 0, 12);
			}
		}
		else if(mode == GASP_MODE_EMU) {
			/* The user dashes in a trail of dust; at the blow motes burst and a
			 * ring */
			/* of fire swells at the target, larger for the stronger variants.
			 */
			SCR_POINT from;
			PARTICLE *p;

			btEffectPosProjectBattler(args->user, &from);
			if(frame == 0) {
				SCR_POINT to;
				PARTICLE *ball = &state->particles[GASP_CENTRE];
				PARTICLE *q;

				btEffectPosProjectScaledBattler(args->tgts[0], &to);
				ball->x = to.x;
				ball->y = to.y;
				EFFECT_FREE_PARTICLES(i, state->particles, GASP_EMU_PUFFS);
				for(i = 0, q = EXTRA_PARTICLES; i != GASP_EMU_MOTES; i++, q++) {
					s32 speed = 511;
					s32 a;

					speed &= syRandom();
					a = syRandom();
					speed += GASP_EMU_MOTE_SPEED_MIN;
					q->velX = sySin(a) * speed >> 6;
					q->velY = syCos(a) * speed >> 4;
					if(args->variant == EFFECT_VARIANT_BASIC) {
						q->velX /= 4;
						q->velY /= 4;
					}
					else if(args->variant == EFFECT_VARIANT_STRONGER) {
						q->velX /= 2;
						q->velY /= 2;
					}
					q->x = INTtoFIXED(to.x / 2);
					q->y = INTtoFIXED(to.y + 16);
					q->variant = (syRandom() & 15) + 8;
				}
			}
			if(frame <= 9) {
				s32 v;
				s32 mask = 255;

				i = 0;
				twice = frame * 2;
				p = (void *)(twice * sizeof(PARTICLE) + (s32)state->particles);
				p->x = INTtoFIXED(from.x / 2);
				p->y = INTtoFIXED(from.y + 8);
				v = ((syRandom() & mask) + 256) << 8;
				p->velX = v;
				if(args->side == EFFECT_SIDE_PARTY)
					p->velX = -v;
				p->velY = -((syRandom() & mask) + 256) << 4;
				p->variant = i;
			}
			for(i = 0; i != GASP_EMU_PUFFS; i++) {
				p = &state->particles[i];

				if((u32)p->variant < SMOKE_PUFF_FRAMES) {
					draw[0](canvas,
							state->sheet +
								p->variant / 4 * PICTURE_BYTES(SMOKE_PUFF),
							FIXED_INTEGER_PART(p->x) - SMOKE_PUFF_WIDTH / 2,
							FIXED_INTEGER_PART(p->y) - SMOKE_PUFF_HEIGHT / 2,
							SMOKE_PUFF_WIDTH, SMOKE_PUFF_HEIGHT);
					btParticleAdvanceWithGravity2D(p, 60, -toFIXED(1) / 16);
					p->variant++;
				}
			}
			if(frame > 9) {
				for(i = 0; i != GaspFamily_Counts[args->variant]; i++) {
					PARTICLE *q = &EXTRA_PARTICLES[i];

					if(q->variant >= 0) {
						s32 n = q->variant / 4 + 3;

						EFFECT_DRAW_MOTE(draw[0], canvas, sprites,
										 FIXED_INTEGER_PART(q->x),
										 FIXED_INTEGER_PART(q->y), n);
						btParticleAdvanceWithGravity2D(q, 62, 0);
						q->variant--;
					}
				}
			}
			if(frame > 9) {
				PARTICLE *ball = &state->particles[GASP_CENTRE];
				s32 t = frame - 10;
				s32 scale = t * (FIXED_ONE / 2) + FIXED_ONE / 4;
				s32 shade = 0;

				if(args->variant == EFFECT_VARIANT_BASIC)
					scale /= 4;
				else if(args->variant == EFFECT_VARIANT_STRONGER)
					scale /= 2;
				if(frame > 15)
					shade = (16 - frame) * 8;
				if(shade > MODEL_SHADE_INVISIBLE) {
					model->shade = shade;
					texture.logWidth = 7;
					texture.logHeight = 7;
					model->mesh = BattleEffect_LargeSquareMesh;
					model->kind = MODEL_SHADED_ADDITIVE;
					texture.pixels = state->sheet + GASP_FIRE_RING;
					syInitMatrix();
					syTranslate(INTtoFIXED(ball->x / 2 - 64),
								INTtoFIXED(ball->y - 48), 0);
					syScale(scale / 4, scale, scale / 2);
					btRenderTransformVertices(BattleEffect_SquareCorners, verts,
											  4);
					btRenderDrawModelList(model);
				}
			}
			if(frame == 10) {
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
				state->shakeFrm = args->variant * 2 + 8;
				btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_NUDGE);
				btEffectTintBattler(args->tgts[0], SPRITE_COLOR_WHITE,
									POSE_DAMAGE, 0, 12);
			}
			btCameraShake(args->variant * 2 + 8, args->variant * 2 + 8);
		}
		else if(mode == GASP_MODE_BEAT_DANCE) {
			/* Music notes fly up from the target as it flushes red. */
			if(frame == 0) {
				SCR_POINT pos;

				btEffectPosProjectBattler(args->tgts[0], &pos);
				for(i = 0; i != GASP_NOTE_SEEDS; i++) {
					PARTICLE *p = &state->particles[i];
					s32 speed = 255;
					s32 a;

					speed &= syRandom();
					a = (syRandom() & (DEGtoANG(180) - 1)) + DEGtoANG(90);
					speed += GASP_NOTE_SPEED_MIN;
					p->velX = sySin(a) * speed >> 7;
					p->velY = syCos(a) * speed >> 6;
					p->velZ = toFIXED(1) / 64;
					SET_POSITION(p, INTtoFIXED(pos.x / 2),
								 INTtoFIXED(pos.y - (syRandom() & 31) + 16),
								 syRandom());
				}
				sySndPlayStub(SE_POWER_UP);
				state->shakeFrm = 16;
				btEffectTintBattler(args->tgts[0], SPRITE_COLOR_RED_TINT,
									TINT_KEEP, 0, 32);
			}
			texture.logWidth = 5;
			texture.logHeight = 5;
			model->kind = MODEL_SHADED_MAX;
			model->texture = &texture;
			model->mesh = BattleEffect_SmallSquareMeshReversed;
			model->vertices = verts;
			for(i = 0; i != GASP_NOTES; i++) {
				PARTICLE *p = &state->particles[i];

				if(frame >= i * 2) {
					s32 t = frame - i * 2;
					s32 shade = 0;

					if(t >= GASP_NOTE_FRAMES) {
						shade = (GASP_NOTE_FRAMES - t) * 4;
						if(shade < MODEL_SHADE_INVISIBLE)
							continue;
					}
					model->shade = shade;
					texture.pixels = state->sheet + i % 3 * GASP_TEXTURE_BYTES;
					PLACE_PARTICLE(p);
					syMatrixScaleUniform(FIXED_ONE / 2);
					btRenderTransformVertices(BattleEffect_SquareCorners, verts,
											  4);
					btRenderDrawModelList(model);
					btParticleAdvanceWithGravity3D(p, 60, 0);
				}
			}
			btCameraShake(8, 8);
		}
		else if(mode == GASP_MODE_CLARION_CRY) {
			/* Music notes fly up from the user. */
			if(frame == 0) {
				SCR_POINT pos;

				btEffectPosProjectBattler(args->user, &pos);
				for(i = 0; i != GASP_NOTE_SEEDS; i++) {
					PARTICLE *p = &state->particles[i];
					s32 speed = 255;
					s32 a;

					speed &= syRandom();
					a = (syRandom() & (DEGtoANG(180) - 1)) + DEGtoANG(90);
					speed += GASP_NOTE_SPEED_MIN;
					p->velX = sySin(a) * speed >> 6;
					p->velY = syCos(a) * speed >> 6;
					p->velZ = toFIXED(1) / 64;
					SET_POSITION(p, INTtoFIXED(pos.x / 2),
								 INTtoFIXED(pos.y - (syRandom() & 31) - 16),
								 syRandom());
				}
				sySndPlayStub(SE_BIRD_CALL);
			}
			texture.logWidth = 5;
			texture.logHeight = 5;
			model->kind = MODEL_SHADED_MAX;
			model->texture = &texture;
			model->mesh = BattleEffect_SmallSquareMeshReversed;
			model->vertices = verts;
			for(i = 0; i != GASP_NOTES; i++) {
				PARTICLE *p = &state->particles[i];

				if(frame >= i * 2) {
					s32 t = frame - i * 2;
					s32 shade = 0;

					if(t >= GASP_NOTE_FRAMES) {
						shade = (GASP_NOTE_FRAMES - t) * 4;
						if(shade < MODEL_SHADE_INVISIBLE)
							continue;
					}
					model->shade = shade;
					texture.pixels = state->sheet + i % 3 * GASP_TEXTURE_BYTES;
					PLACE_PARTICLE(p);
					syMatrixScaleUniform(FIXED_ONE / 2);
					btRenderTransformVertices(BattleEffect_SquareCorners, verts,
											  4);
					btRenderDrawModelList(model);
					btParticleAdvanceWithGravity3D(p, 60, 0);
				}
			}
		}
		else if(mode == GASP_MODE_CALL_DULLAHAN) {
			/* The user raises its arm, four bolts swing out from it onto the
			 * target, */
			/* and sparks burst and bounce there. */
			SCR_POINT pos;

			if(frame == 0) {
				void *user = btObjectGetStub(args->user)->obj;

				/* Another pose first, so that the leap starts over, held on its
				 * first */
				/* picture until frame 14. */
				fdActorSelectAnimationStub(user, POSE_STILL);
				fdActorSelectAnimationStub(user, POSE_LEAP);
				fdActorSetAnimationSpeedStub(user, 0);
				sySndPlayStub(SE_POWER_UP);
			}
			if(frame == 14)
				fdActorSetAnimationSpeedStub(btObjectGetStub(args->user)->obj,
											 ANIM_STEP_NORMAL);
			if(frame == 22)
				sySndPlayStub(SE_POWER_UP);
			if(frame == 26) {
				EFFECT_SHOW_DAMAGE(SE_CLATTER);
				state->shakeFrm = 8;
				EFFECT_STRIKE_TARGET(args, 0, 12, KNOCKBACK_HURL);
				syWordsFillEntry(canvas, CANVAS_BYTES,
								 CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			}
			if(frame > 27) {
				PARTICLE *q;
				PARTICLE *p;

				if(frame == 28) {
					btEffectPosProjectScaledBattler(args->tgts[0], &pos);
					for(q = EXTRA_PARTICLES, i = 0; i != GASP_DULLAHAN_SPARKS;
						i++, q++) {
						s32 speed = 511;
						s32 a;

						speed &= syRandom();
						a = syRandom();
						speed += GASP_DULLAHAN_SPARK_SPEED_MIN;
						q->velX = sySin(a) * speed >> 7;
						q->velY = syCos(a) * speed >> 6;
						q->x = INTtoFIXED(pos.x / 2) + q->velX * 2;
						q->y = INTtoFIXED(pos.y + 16) + q->velY * 2;
						q->variant = (syRandom() & 15) + 16;
					}
				}
				for(p = EXTRA_PARTICLES, i = 0; i != GASP_DULLAHAN_SPARKS_DRAWN;
					i++, p++) {
					if(p->variant >= 0) {
						s32 j;

						/* Each spark moves four steps a frame, drawn at each.
						 */
						for(j = 0; j != 4; j++) {
							s32 n = p->variant / 8 + 1;

							EFFECT_DRAW_MOTE(draw[0], canvas, sprites,
											 FIXED_INTEGER_PART(p->x),
											 FIXED_INTEGER_PART(p->y), n);
							btParticleAdvanceWithGravity2D(p, 63,
														   toFIXED(1) / 32);
							if(p->y > toFIXED(104)) {
								p->y = toFIXED(104);
								p->velY = -p->velY / 2;
							}
						}
						p->variant--;
					}
				}
			}
			btEffectPosProjectScaledBattler(args->user, &pos);
			texture.logWidth = 5;
			texture.logHeight = 7;
			model->kind = MODEL_SHADED_ADDITIVE;
			model->texture = &texture;
			model->mesh = BattleEffect_ColumnMesh;
			model->vertices = verts;
			syInitMatrix();
			syScale(FIXED_ONE / 2, FIXED_ONE, FIXED_ONE);
			if(args->side == EFFECT_SIDE_ENEMIES)
				syTranslate(INTtoFIXED(pos.x - 136), INTtoFIXED(pos.y - 48), 0);
			else
				syTranslate(INTtoFIXED(pos.x - 108), INTtoFIXED(pos.y - 56), 0);
			for(i = 0; i != 4; i++) {
				s32 scale;

				syPushMatrix();
				texture.pixels =
					state->sheet + (syRandom() & 3) * GASP_BOLT_CEL_BYTES;
				texture.logWidth = 5;
				texture.logHeight = 7;
				if(frame > 23) {
					s32 d = frame * 16 - i * 2 - 352;
					s32 angle;

					if(d < 0)
						continue;
					if(d > GASP_DULLAHAN_REACH)
						d = GASP_DULLAHAN_REACH;
					if(args->side == EFFECT_SIDE_ENEMIES)
						syTranslate(d * 3 * (toFIXED(1) / 8),
									d * 7 * (toFIXED(1) / 32), 0);
					else
						syTranslate(-(d * 3 * (toFIXED(1) / 8)),
									d * 7 * (toFIXED(1) / 32), 0);
					angle = d * GASP_DULLAHAN_SWING / GASP_DULLAHAN_REACH +
							TRIG_TURN * 3 / 8;
					if(args->side == EFFECT_SIDE_ENEMIES)
						angle = -angle;
					syRotZ(angle);
				}
				else {
					if(i != 0)
						continue;
					syRotZ(args->side == EFFECT_SIDE_ENEMIES
							   ? -TRIG_TURN * 3 / 8
							   : TRIG_TURN * 3 / 8);
				}
				model->shade = -i * 8;
				if(frame <= 23)
					scale = frame * (FIXED_ONE / 24);
				else if(frame > 31)
					scale = FIXED_ONE * 4 - frame * 3 * (FIXED_ONE / 32);
				else
					scale = FIXED_ONE;
				if(scale > 0) {
					syScale(scale * 6, scale * 2, FIXED_ONE);
					btRenderTransformVertices(BattleEffect_LongBeamCorners,
											  verts, 4);
					btRenderDrawModelList(model);
					syPopMatrix();
				}
			}
			btCameraShake(8, 8);
		}
		else if(mode == GASP_MODE_DEMON_EYE) {
			/* Two eyes of light open and close at the user, and the targets
			 * flash. */
			SCR_POINT pos;

			state->transferValue = CANVAS_FADE_HALF;
			texture.logWidth = 7;
			texture.logHeight = 7;
			model->mesh = BattleEffect_LargeSquareMesh;
			model->kind = MODEL_SHADED_MAX;
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
			texture.pixels = state->sheet;
			btEffectPosProjectScaledBattler(args->user, &pos);
			if(frame <= 15) {
				for(i = 0; i != 2; i++) {
					s32 scale = sySin(frame * (TRIG_TURN / 32)) * 5;

					if(scale > 0) {
						model->shade = 0;
						syInitMatrix();
						syTranslate(INTtoFIXED(pos.x / 2 + i * 16 - 72),
									INTtoFIXED(pos.y - 64), 0);
						syScale(scale / 2, scale, scale / 2);
						syRotZ(DEGtoANG(45));
						btRenderTransformVertices(BattleEffect_SquareCorners,
												  verts, 4);
						btRenderDrawModelList(model);
					}
				}
			}
			if(frame == 8) {
				sySndPlayStub(SE_POWER_UP);
				state->shakeFrm = 8;
				syWordsFillEntry(canvas, CANVAS_BYTES,
								 CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
				for(i = 0; i != args->tgtCnt; i++)
					btEffectTintBattler(args->tgts[i], SPRITE_COLOR_WHITE,
										POSE_DAMAGE, i, 7);
			}
			btCameraShake(8, 8);
		}
		else if(mode == GASP_MODE_CAGE) {
			/* Eight turning rings make a cage around the target, then close on
			 * it. */
			SCR_POINT pos;
			s32 shade;

			if(frame == 0)
				sySndPlayStub(SE_LAUNCH);
			btEffectPosProjectBattler(args->tgts[0], &pos);
			shade = frame * 4 + MODEL_SHADE_INVISIBLE;
			if(shade >= -16)
				shade = -16;
			model->shade = shade;
			model->texture = &texture;
			model->vertices = verts;
			model->kind = MODEL_SHADED_MAX;
			model->mesh = BattleEffect_CylinderMesh;
			texture.pixels = state->sheet;
			texture.logWidth = 7;
			texture.logHeight = 4;
			if(frame == 26) {
				state->shakeFrm = 8;
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			}
			for(i = 0; i != 8; i++) {
				s32 scale = FIXED_ONE * 2;
				s32 start = i * 2 + GASP_CAGE_CLOSE_FRAME;

				if(frame >= start)
					scale = FIXED_ONE * 2 - (frame - start) * (FIXED_ONE / 8);
				if(scale > FIXED_ONE / 2) {
					model->scrollU = ((frame + i * 5) & 7) << 4;
					syInitMatrix();
					syTranslate(INTtoFIXED(pos.x / 2 - 60), toFIXED(8), 0);
					syScale(scale >> 1, scale, FIXED_ONE);
					syRotX(i * DEGtoANG(45) + GASP_CAGE_TILT);
					syRotY(frame * (TRIG_TURN / 128));
					syRotZ((frame + i * 16) * (TRIG_TURN / 128));
					syScale(FIXED_ONE, FIXED_ONE / 2, FIXED_ONE);
					btRenderTransformVertices(BattleEffect_CylinderCorners,
											  verts, 32);
					btRenderDrawModelList(model);
				}
			}
		}
		else if(mode == GASP_MODE_ELEMENT_SWAP) {
			/* A ring shrinks onto the user and a glow turns there while motes
			 * close */
			/* in, and the colours step through Mercury's, Mars's and Jupiter's.
			 */
			SCR_POINT pos;
			s32 j;

			texture.logWidth = 7;
			texture.logHeight = 7;
			texture.pixels = cells;
			model->mesh = BattleEffect_LargeSquareMesh;
			model->kind = MODEL_SHADED_MAX;
			btEffectPosProjectScaledBattler(args->user, &pos);
			syInitMatrix();
			syTranslate(INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48), 0);
			if(frame <= 15) {
				s32 scale = FIXED_ONE * 3 - frame * 3 * (FIXED_ONE / 16);

				model->shade = 0;
				syScale(scale >> 1, scale, scale >> 1);
				btRenderTransformVertices(BattleEffect_SquareCorners, verts, 4);
				btRenderDrawModelList(model);
			}
			syInitMatrix();
			syTranslate(INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48), 0);
			if((u32)(frame - 12) <= 31) {
				s32 scale =
					sySin(frame * (TRIG_TURN / 64) - TRIG_TURN * 3 / 16) * 3;

				texture.pixels = state->sheet;
				syScale(scale / 2, scale, scale / 2);
				syRotZ((frame << 9) + DEGtoANG(45));
				btRenderTransformVertices(BattleEffect_SquareCorners, verts, 4);
				btRenderDrawModelList(model);
			}
			/* A mote's x is its distance from the user in sixteenths of a
			 * pixel, 48 */
			/* to 142 pixels at first, closing in over 48 steps, three a frame;
			 * its */
			/* y is its angle. */
			if(frame == 0) {
				for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
					PARTICLE *p = &state->particles[i];

					syRandom();
					p->x = 768 + i * 24;
					p->velX = -p->x / 48;
					p->y = syRandom();
				}
				LOAD_BG_PALETTE(RESOURCE_BLAST_SHEET);
			}
			if(frame <= 15) {
				for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
					PARTICLE *p = &state->particles[i];

					for(j = 0; j != 3; j++) {
						s32 x = (FIXEDtoINT(sySin(p->y) * p->x) >>
								 (GASP_SWAP_MOTE_SHIFT + 1)) +
								pos.x / 2;
						s32 y = (FIXEDtoINT(p->x * syCos(p->y)) >>
								 GASP_SWAP_MOTE_SHIFT) +
								60;
						s32 n = 4;

						EFFECT_DRAW_MOTE(draw[0], canvas, sprites, x, y, n);
						p->x += p->velX;
					}
				}
			}
			/* Three steps a frame towards each Djinni's colours in turn. */
			for(i = 0; i != 3; i++) {
				if(frame > 7) {
					if(frame <= 15)
						btEffectStepPaletteToResource(
							RESOURCE_MERCURY_DJINN_SHEET);
					else if(frame <= 23)
						btEffectStepPaletteToResource(
							RESOURCE_MARS_DJINN_SHEET);
					else
						btEffectStepPaletteToResource(
							RESOURCE_JUPITER_DJINN_SHEET);
				}
			}
			if(frame == 16) {
				sySndPlayStub(SE_POWER_UP);
				btEffectTintBattler(args->tgts[0], SPRITE_COLOR_WHITE,
									TINT_KEEP, 0, 30);
			}
			if(frame == 28)
				EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
		}
		else if(mode == GASP_MODE_PSY_BOOST) {
			/* Two rings shrink onto the user and a glow turns there, in grey.
			 */
			SCR_POINT pos;

			texture.logWidth = 7;
			texture.logHeight = 7;
			texture.pixels = cells;
			model->mesh = BattleEffect_LargeSquareMesh;
			model->kind = MODEL_SHADED_MAX;
			btEffectPosProjectScaledBattler(args->user, &pos);
			for(i = 0; i != 2; i++) {
				s32 t = frame - i * 8;

				syInitMatrix();
				syTranslate(INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48),
							0);
				if((u32)t < GASP_PSY_BOOST_RING_FRAMES) {
					s32 scale = FIXED_ONE * 4 - t * (FIXED_ONE / 4);
					s32 shade = 0;

					if(t <= 7)
						shade = t * 8 + MODEL_SHADE_INVISIBLE;
					model->shade = shade;
					syScale(scale / 2, scale, scale / 2);
					syRotZ(i * (DEGtoANG(90)) + DEGtoANG(45));
					syRotX(-TRIG_TURN / 16);
					btRenderTransformVertices(BattleEffect_GroundSquareCorners,
											  verts, 4);
					btRenderDrawModelList(model);
				}
			}
			syInitMatrix();
			syTranslate(INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48), 0);
			if((u32)(frame - 12) <= 31) {
				s32 scale =
					sySin(frame * (TRIG_TURN / 64) - TRIG_TURN * 3 / 16) * 2;

				texture.pixels = state->sheet;
				syScale(scale / 2, scale, scale / 2);
				syRotZ((frame << 9) + DEGtoANG(45));
				btRenderTransformVertices(BattleEffect_SquareCorners, verts, 4);
				btRenderDrawModelList(model);
			}
			if(frame == 0)
				btGraphicsLoadGreyRamp();
			if(frame == 16) {
				sySndPlayStub(SE_POWER_UP);
				btEffectTintBattler(args->tgts[0], SPRITE_COLOR_WHITE,
									TINT_KEEP, 0, 30);
			}
			if(frame == 28)
				EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
		}
		else {
			/* Reflux, Counter-rush and Djinn Counter: a swirl grows and turns,
			 * at */
			/* the target or at the user. */
			s32 scale;

			model->shade = 0;
			texture.logWidth = 6;
			texture.logHeight = 6;
			model->mesh = BattleEffect_SquareMesh;
			model->kind = MODEL_SHADED_ADDITIVE;
			texture.pixels = state->sheet;
			if(mode == GASP_MODE_REFLUX || mode == GASP_MODE_COUNTER_RUSH) {
				if(frame <= 31)
					scale = frame * GASP_SWIRL_GROWTH;
				else if(frame > 59)
					scale = GASP_SWIRL_SCALE + 60 * GASP_SWIRL_SHRINK -
							frame * GASP_SWIRL_SHRINK;
				else
					scale = GASP_SWIRL_SCALE;
			}
			else {
				if(frame <= 31)
					scale = frame * GASP_SWIRL_GROWTH;
				else if(frame > 39)
					scale = GASP_SWIRL_SCALE + 40 * GASP_SWIRL_SHRINK -
							frame * GASP_SWIRL_SHRINK;
				else
					scale = GASP_SWIRL_SCALE;
			}
			if(scale > 0) {
				SCR_POINT pos;

				if(mode == GASP_MODE_REFLUX || mode == GASP_MODE_COUNTER_RUSH)
					btEffectPosProjectScaledBattler(args->tgts[0], &pos);
				else
					btEffectPosProjectScaledBattler(args->user, &pos);
				syInitMatrix();
				syTranslate(INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48),
							0);
				syRotX(DEGtoANG(90));
				syScale(scale / 3, scale, scale);
				if(mode == GASP_MODE_REFLUX)
					syRotY(-frame * (TRIG_TURN / 64));
				else
					syRotY(frame * (TRIG_TURN / 64));
				btRenderTransformVertices(BattleEffect_GroundSquareCorners,
										  verts, 4);
				btRenderDrawModelList(model);
			}
		}
		syHeapRelease(model);
		syHeapRelease(verts);
		ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
		ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
		btEffectCountDownBattlerTints();
		if(mode == GASP_MODE_UNUSED)
			WAIT_TRANSFER(state) {}
		EFFECT_PRESENT_CANVAS(state);
	}
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	if(mode == GASP_MODE_GASP || mode == GASP_MODE_LULL)
		syCloseEvent(btEffectArmBg2AffineHBlankDma);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}
#undef FIGURE
#undef PLACE_PARTICLE
