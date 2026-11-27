/* The body that sixteen abilities' effects share, from Gasp to Demon Eye. Part
   of battle_effect.c. */

/* Where the small Djinni sprite sheet is decoded in the effect's pictures, as
   The Broken Seal places it. */
#define GASP_DJINNI_PICTURE 26048
/* The small Djinni: a 20 x 40 picture drawn over the canvas. */
#define FIGURE(state)		((state)->sheet + GASP_DJINNI_PICTURE)
/* Sets the transform for one of Beat Dance's and Clarion Cry's spinning notes:
   at its position, half as wide (the canvas holds every other column), turned
   by z. */
#define PLACE_PARTICLE(p) \
	do { \
		syInitMatrix(); \
		syTranslate((p)->x - toFIXED(64), (p)->y - toFIXED(64), 0); \
		syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1)); \
		syRotZ((p)->z); \
	} while(0)

/* The last three particles: the point a mode centres on (Shine's Djinni in
   flight, Emu's target), Shine's fall towards the target, and where its flash
   strikes. */
#define GASP_CENTRE				 63
#define GASP_FALL				 62
#define GASP_FLASH				 61
/* The motes of Djinn Counter's blow and of Reflux's and Counter-rush's swirl:
   every particle but the last. */
#define GASP_MOTES				 63
/* Mold's 40x40 Djinni is copied to the middle of a 64x64 picture. */
#define GASP_DJINNI_SLOT		 64
#define GASP_DJINNI_SLOT_MARGIN	 ((GASP_DJINNI_SLOT - DJINNI_WIDTH) / 2)
/* Call Dullahan's four bolts, LightningBoltSheet's pictures, each copied into a
   32x128 picture GASP_BOLT_PICTURE_INSET pixels in from its left. */
#define GASP_BOLT_PICTURE_COUNT	 4
#define GASP_BOLT_PICTURE_WIDTH	 32
#define GASP_BOLT_PICTURE_HEIGHT 128
#define GASP_BOLT_PICTURE_INSET \
	((GASP_BOLT_PICTURE_WIDTH - LIGHTNING_BOLT_WIDTH) / 2)
#define GASP_BOLT_PICTURE_BYTES \
	(GASP_BOLT_PICTURE_WIDTH * GASP_BOLT_PICTURE_HEIGHT)

/* Where VenusDjinnSheet, CornerGlowSheet and FireRingTexture are decoded in the
   effect's pictures. */
#define GASP_FAMILY_VENUS_DJINN_PICTURES 4096
#define GASP_FAMILY_CORNER_GLOW_PICTURES 16384
#define GASP_FIRE_RING					 12416

/* The textures the model layer turns, 32 by 32: Gasp's two burning skulls, and
   the three music notes of Lull, Beat Dance and Clarion Cry. */
#define GASP_TEXTURE_BYTES (32 * 32)
/* The particles Gasp and Shine set up at their start, more than either draws:
   Gasp's GASP_SKULLS and Shine's GASP_SHINE_RAYS. */
#define GASP_SEEDS		   16

/* Gasp and Lull: the small Djinni sways GASP_DJINNI_SWAY pixels either side and
   bobs, a 32nd of a turn a tick. Gasp's burning skulls rise one every 8
   ticks, each for GASP_SKULL_TICKS before it fades. */
#define GASP_DJINNI_SWAY 20
#define GASP_SKULLS		 6
#define GASP_SKULL_TICKS 32
#define GASP_SKULL_SCALE (toFIXED(1875) / 4096)

/* Reflux: the Djinni circles wider until GASP_REFLUX_RADIUS. Reflux and
   Counter-rush: of GASP_MOTES set up, GASP_SWIRL_MOTES close in on the swirl,
   each for GASP_SWIRL_MOTE_TICKS, as the swirl grows to GASP_SWIRL_SCALE. */
#define GASP_REFLUX_RADIUS	  12
#define GASP_SWIRL_MOTES	  48
#define GASP_SWIRL_MOTE_TICKS 32
#define GASP_SWIRL_SCALE	  (toFIXED(375) / 512)
/* The swirl grows to its full scale over its first 32 ticks, and shrinks away
   twice as fast. */
#define GASP_SWIRL_GROWTH	  (GASP_SWIRL_SCALE / 32)
#define GASP_SWIRL_SHRINK	  (GASP_SWIRL_SCALE / 16)

/* Shine: the Djinni glides a GASP_SHINE_GLIDE-th of the way to over the user a
   tick, then falls onto the target a GASP_SHINE_FALL-th of the way more each
   tick, turning rays of light about it. Each ray's texture scrolls two texels
   a tick, seven behind the last ray's, within the 32 texels its mesh spans. */
#define GASP_SHINE_GLIDE		   40
#define GASP_SHINE_FALL			   32
#define GASP_SHINE_RAYS			   8
#define GASP_SHINE_RAY_TICKS	   32
#define GASP_SHINE_RAY_SCROLL_MASK 0x1f

/* Lull: a ring of music notes, leaning back GASP_LULL_TILT (about 100 degrees)
   and righting itself a 1024th of a turn a tick, turning about 2 degrees a
   tick, fades after GASP_LULL_RING_TICKS. */
#define GASP_LULL_NOTES		 12
#define GASP_LULL_RING_TICKS 64
#define GASP_LULL_TILT		 18288

/* Mold: the Djinni's sprite shrinks GASP_MOLD_SHRINK a tick (about 7 percent
   of its size); for its first GASP_MOLD_SLOW_FADE_TICKS the canvas keeps three
   quarters of each pixel, then half. */
#define GASP_MOLD_SHRINK		  4592
#define GASP_MOLD_SLOW_FADE_TICKS 24

/* Emu: puffs of dust trail the bird, one each two ticks for its first ten; the
   blow scatters motes, each at least GASP_EMU_MOTE_SPEED_MIN fast. */
#define GASP_EMU_PUFFS			32
#define GASP_EMU_MOTES			256
#define GASP_EMU_MOTE_SPEED_MIN 64

/* Beat Dance and Clarion Cry: the notes that fly from their unit, one every two
   ticks, each for GASP_NOTE_TICKS before it fades, at least
   GASP_NOTE_SPEED_MIN fast. */
#define GASP_NOTE_SEEDS		32
#define GASP_NOTES			12
#define GASP_NOTE_TICKS		24
#define GASP_NOTE_SPEED_MIN 128

/* Call Dullahan: the sparks his bolt scatters, each at least
   GASP_DULLAHAN_SPARK_SPEED_MIN fast, and the first of them drawn; each bolt
   flies out GASP_DULLAHAN_REACH steps, turning GASP_DULLAHAN_SWING (about 265
   degrees) as it goes. */
#define GASP_DULLAHAN_SPARKS		  128
#define GASP_DULLAHAN_SPARKS_DRAWN	  64
#define GASP_DULLAHAN_SPARK_SPEED_MIN 32
#define GASP_DULLAHAN_REACH			  80
#define GASP_DULLAHAN_SWING			  48152

/* Cage: its rings lean, each a further eighth of a turn, and close on the
   target from GASP_CAGE_CLOSE_TICK, each two ticks after the last. */
#define GASP_CAGE_TILT		 4000
#define GASP_CAGE_CLOSE_TICK 16

/* Element Swap: a mote's distance from the user is kept in sixteenths of a
   pixel, GASP_SWAP_MOTE_SHIFT bits of fraction. */
#define GASP_SWAP_MOTE_SHIFT 4

/* Psy Boost: each of its two rings shrinks onto the user over
   GASP_PSY_BOOST_RING_TICKS. */
#define GASP_PSY_BOOST_RING_TICKS 16

/* The shared body of sixteen abilities' effects, named for the first; the
   wrappers in rime.c pick a GASP_MODE: Gasp, Reflux, Djinn Counter, Mold and
   Meld, Shine, Lull, Counter-rush, Demon Eye, Emu and its kind's four other
   abilities, Call Dullahan and Formina Sage, Beat Dance, Element Swap, Cage,
   Clarion Cry and Psy Boost. GASP_MODE_UNUSED has no caller. Each mode loads
   its own sprite sheets, then draws a canvas layer and a model layer every
   tick for the mode's length. */
void btEffectRunGaspFamily(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	BATTLE_EFFECT_STATE *state = RamHeapSlots->battleEff;
	Uint8				*canvas = RamHeapSlots->battleCanvas;
	/* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
	 * tick reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE draw[2];
	Sint32		   ticks;
	Uint8		  *sprs = RamHeapSlots->battleSheet;
	BATTLE_CAMERA *camera = RamHeapSlots->cameraState;
	Uint8		  *cells = RamMapMetatileBuffer;
	RENDER_MODEL  *model;
	void		  *verts;
	MODEL_TEXTURE  texture;
	Sint32		   screen[3];
	Sint32		   other[3];
	Sint32		   frm;
	Sint32		   i;

	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	if(mode == GASP_MODE_GASP) {
		btEffectRunDjinniEntrance(args,
			DJINNI_ENTRANCE(
				ELEMENT_JUPITER, args->side ^ 1, DJINNI_ANCHOR_TARGET_X),
			screen, other);
		btResourceLoadAndDecompress(
			RESOURCE_JUPITER_DJINN_SMALL_SHEET, FIGURE(state), 1, 1);
		btResourceLoadAndDecompress(
			RESOURCE_BURNING_SKULL_SHEET, state->sheet, 0, 0);
		REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
			BGCNT_SCREEN_BASE(7);
		ticks = 84;
	} else if(mode == GASP_MODE_UNUSED) {
		btEffectRunDjinniEntrance(args,
			DJINNI_ENTRANCE(
				ELEMENT_MERCURY, args->side ^ 1, DJINNI_ANCHOR_TARGET_X),
			screen, other);
		btResourceLoadAndDecompress(
			RESOURCE_LIGHT_BURST_SHEET, state->sheet, 1, 0);
		btResourceLoadAndDecompress(
			RESOURCE_MERCURY_DJINN_SMALL_SHEET, FIGURE(state), 1, 1);
		LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
		ticks = 48;
	} else if(mode == GASP_MODE_MOLD) {
		Sint32 j;
		Sint32 k;

		btEffectRunDjinniEntrance(args,
			DJINNI_ENTRANCE(
				ELEMENT_VENUS, args->side ^ 1, DJINNI_ANCHOR_HIGH_TARGET_X),
			screen, other);
		btResourceLoadAndDecompress(
			RESOURCE_VENUS_DJINN_SMALL_SHEET, FIGURE(state), 1, 0);
		btResourceLoadAndDecompress(RESOURCE_VENUS_DJINN_SHEET,
			state->sheet + GASP_FAMILY_VENUS_DJINN_PICTURES, 1, 0);
		syWordsClear(state->sheet, GASP_DJINNI_SLOT * GASP_DJINNI_SLOT);
		for(k = 0; k != DJINNI_HEIGHT; k++)
			for(j = 0; j != DJINNI_WIDTH; j++)
				state->sheet[(k + GASP_DJINNI_SLOT_MARGIN) * GASP_DJINNI_SLOT +
					GASP_DJINNI_SLOT_MARGIN + j] =
					state->sheet[GASP_FAMILY_VENUS_DJINN_PICTURES +
						k * DJINNI_WIDTH + j];
		ticks = 42;
	} else if(mode == GASP_MODE_LULL) {
		btEffectRunDjinniEntrance(args,
			DJINNI_ENTRANCE(
				ELEMENT_JUPITER, args->side ^ 1, DJINNI_ANCHOR_CENTRE),
			screen, other);
		btResourceLoadAndDecompress(
			RESOURCE_JUPITER_DJINN_SMALL_SHEET, FIGURE(state), 1, 1);
		btResourceLoadAndDecompress(
			RESOURCE_MUSIC_NOTE_SHEET, state->sheet, 0, 0);
		ticks = 80;
		sySndPlayStub(MUS_POISON_CURED);
	} else if(mode == GASP_MODE_BEAT_DANCE) {
		btResourceLoadAndDecompress(
			RESOURCE_MUSIC_NOTE_SHEET, state->sheet, 0, 0);
		LOAD_BG_PALETTE(RESOURCE_MARS_DJINN_SMALL_SHEET);
		ticks = 64;
	} else if(mode == GASP_MODE_CLARION_CRY) {
		btResourceLoadAndDecompress(
			RESOURCE_MUSIC_NOTE_SHEET, state->sheet, 0, 0);
		LOAD_BG_PALETTE(RESOURCE_MERCURY_DJINN_SMALL_SHEET);
		state->fadeFrm = BACKGROUND_DARKEN_TICKS;
		state->fadeStep = 0;
		ticks = 64;
	} else if(mode == GASP_MODE_CAGE) {
		btResourceLoadAndDecompress(
			RESOURCE_SMOKE_BAND_SHEET, state->sheet, 1, 1);
		ticks = 40;
	} else if(mode == GASP_MODE_ELEMENT_SWAP) {
		btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, cells, 0, 0);
		btResourceLoadAndDecompress(RESOURCE_CORNER_GLOW_SHEET,
			state->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES, 1, 1);
		btPictureMirrorIntoQuadrants(
			state->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES, state->sheet, 64,
			64);
		state->fadeFrm = BACKGROUND_DARKEN_TICKS;
		state->fadeStep = 0;
		ticks = 48;
	} else if(mode == GASP_MODE_PSY_BOOST) {
		btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, cells, 0, 0);
		btResourceLoadAndDecompress(RESOURCE_CORNER_GLOW_SHEET,
			state->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES, 1, 1);
		btPictureMirrorIntoQuadrants(
			state->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES, state->sheet, 64,
			64);
		state->fadeFrm = BACKGROUND_DARKEN_TICKS;
		state->fadeStep = 0;
		ticks = 48;
	} else if(mode == GASP_MODE_SHINE) {
		btEffectRunDjinniEntrance(args,
			DJINNI_ENTRANCE(ELEMENT_MARS, args->side, DJINNI_ANCHOR_LOW_CENTRE),
			screen, other);
		REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
			BGCNT_SCREEN_BASE(7);
		btResourceLoadAndDecompress(
			RESOURCE_MARS_DJINN_SMALL_SHEET, FIGURE(state), 1, 0);
		btResourceLoadAndDecompress(
			RESOURCE_FLAME_BAND_TEXTURE, state->sheet, 0, 0);
		btResourceLoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_A,
			state->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES, 0, 0);
		btResourceLoadAndDecompress(RESOURCE_FIRE_RING_TEXTURE, cells, 0, 0);
		LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
		ticks = 82;
	} else if(mode == GASP_MODE_CALL_DULLAHAN) {
		Sint32 j;
		Sint32 k;

		btResourceLoadAndDecompress(RESOURCE_LIGHTNING_BOLT_SHEET, cells, 1, 1);
		syWordsClearEntry(
			state->sheet, GASP_BOLT_PICTURE_COUNT * GASP_BOLT_PICTURE_BYTES);
		for(i = 0; i != GASP_BOLT_PICTURE_COUNT; i++)
			for(k = 0; k != LIGHTNING_BOLT_HEIGHT; k++)
				for(j = 0; j != LIGHTNING_BOLT_WIDTH; j++)
					state->sheet[GASP_BOLT_PICTURE_INSET +
						i * GASP_BOLT_PICTURE_BYTES +
						k * GASP_BOLT_PICTURE_WIDTH + j] =
						cells[i * LIGHTNING_BOLT_PICTURE_BYTES +
							k * LIGHTNING_BOLT_WIDTH + j];
		LOAD_BG_PALETTE(RESOURCE_VIOLET_PALETTE_F);
		ticks = 60;
	} else if(mode == GASP_MODE_COUNTER_RUSH) {
		btResourceLoadAndDecompress(RESOURCE_SWIRL_TEXTURE, state->sheet, 0, 0);
		LOAD_BG_PALETTE(RESOURCE_MERCURY_DJINN_SMALL_SHEET);
		ticks = 76;
	} else if(mode == GASP_MODE_DEMON_EYE) {
		btResourceLoadAndDecompress(RESOURCE_CORNER_GLOW_SHEET,
			state->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES, 1, 1);
		btPictureMirrorIntoQuadrants(
			state->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES, state->sheet, 64,
			64);
		REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
			BGCNT_SCREEN_BASE(7);
		ticks = 16;
	} else if(mode == GASP_MODE_EMU) {
		Uint8 *p;

		REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
			BGCNT_SCREEN_BASE(7);
		btResourceLoadAndDecompress(
			RESOURCE_FIRE_RING_TEXTURE, state->sheet + GASP_FIRE_RING, 0, 0);
		btResourceLoadAndDecompress(RESOURCE_SMOKE_SHEET, state->sheet, 1, 0);
		for(i = 0, p = state->sheet; i != 6 * PICTURE_BYTES(SMOKE_PUFF);
			i++, p++) {
			*p <<= 1;
			if(*p > CANVAS_LEVEL_BRIGHTEST)
				*p = CANVAS_LEVEL_BRIGHTEST;
		}
		if(args->variant == EFFECT_VARIANT_STRONGEST) {
			LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
		} else {
			LOAD_BG_PALETTE(RESOURCE_BLAST_SHEET);
		}
		btEffectSetApproachMotion(args->user, args->tgts[0], 10, 80);
		ticks = 41;
	} else {
		if(mode == GASP_MODE_REFLUX) {
			btEffectRunDjinniEntrance(args,
				DJINNI_ENTRANCE(
					ELEMENT_MARS, args->side ^ 1, DJINNI_ANCHOR_OVER_TARGET),
				screen, other);
			ticks = 76;
		} else {
			state->fadeFrm = BACKGROUND_DARKEN_TICKS;
			state->fadeStep = 0;
			ticks = 70;
		}
		btResourceLoadAndDecompress(RESOURCE_SWIRL_TEXTURE, state->sheet, 0, 0);
		btResourceLoadAndDecompress(
			RESOURCE_MARS_DJINN_SMALL_SHEET, FIGURE(state), 1, 1);
		LOAD_BG_PALETTE(RESOURCE_ORANGE_PALETTE_B);
	}
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sprs, 0, 0);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	if(mode == GASP_MODE_GASP || mode == GASP_MODE_LULL)
		sySetEvent(btEffectArmBg2AffineHBlankDMA, EVENT_PASS_VBLANK);
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	if(mode == GASP_MODE_REFLUX || mode == GASP_MODE_COUNTER_RUSH ||
		mode == GASP_MODE_MOLD) {
		if(args->side == EFFECT_SIDE_ENEMIES)
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
		else
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	} else if(mode == GASP_MODE_DJINN_COUNTER || mode == GASP_MODE_SHINE) {
		if(args->side == EFFECT_SIDE_PARTY)
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
		else
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	} else {
		if(args->side == EFFECT_SIDE_ENEMIES)
			EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
				EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_MAX);
		else
			EFFECT_LOAD_BLITTER(
				HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	}
	draw[0] = RamHeapSlots->ramCode;

	for(frm = 0; frm != ticks; frm++) {
		Sint32 a;
		Sint32 twice;
		Sint32 x;
		Sint32 y;

		/* The canvas layer. */
		if(mode == GASP_MODE_GASP) {
			Sint32 *lines = state->bg2xLines;

			for(i = 0; i != ARRAY_COUNT(state->bg2xLines); i++)
				*lines++ = (syRandom() & 31) << 4;
		} else if(mode == GASP_MODE_LULL) {
			Sint32 *lines = state->bg2xLines;

			for(i = 0; i != ARRAY_COUNT(state->bg2xLines); i++)
				*lines++ = CANVAS_WAVE_BG2X(toFIXED(6) -
					sySin(frm * (TRIG_TURN / 32) + i * (TRIG_TURN / 32)) * 6);
		} else if(mode == GASP_MODE_UNUSED && frm > 31) {
			Uint16 *pal = (Uint16 *)&BG_PLTT_COLOR(0, 1);

			for(i = 0; i != SHEET_PALETTE_COLORS - 1; i++, pal++) {
				Sint32 r = RGB_RED(*pal);
				Sint32 g = RGB_GREEN(*pal);
				Sint32 b = RGB_BLUE(*pal);

				if(frm > 40) {
					r += 4;
					g += 4;
					b += 4;
				} else {
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
		   images, and the blow bursts into motes. */
		if(mode == GASP_MODE_DJINN_COUNTER) {
			SCREEN_POINT to;
			SCREEN_POINT from;

			btEffectPosProjectScaledBattler(args->user, &from);
			btEffectPosProjectScaledBattler(args->tgts[0], &to);
			from.x /= 2;
			to.x /= 2;
			if((Uint32)(frm - 32) <= 8) {
				Sint32 n = frm - 31;
				Sint32 dx;
				Sint32 head;
				Sint32 tail;
				Sint32 step;
				Sint32 acc;

				if(n > 8)
					n = 8;
				dx = to.x - from.x;
				head = from.x + GaspFamilyStreakSteps[n] * dx / 16;
				tail = from.x + GaspFamilyStreakSteps[n - 1] * dx / 16;
				for(i = 0, step = head - tail, acc = 0; i != 4;
					i++, acc += step)
					draw[0](canvas, FIGURE(state),
						tail + acc / 3 - SMALL_DJINNI_WIDTH / 2,
						from.y - SMALL_DJINNI_HEIGHT / 2, SMALL_DJINNI_WIDTH,
						SMALL_DJINNI_HEIGHT);
			}
			if(frm > 37) {
				if(frm == 38) {
					for(i = 0; i != GASP_MOTES; i++) {
						PARTICLE *p = &state->particles[i];
						Sint32	  s;

						p->x = toFIXED(to.x);
						p->y = toFIXED(to.y);
						a = syRandom();
						s = (syRandom() & 511) + 256;
						p->velX = sySin(a) * s >> 8;
						p->velY = syCos(a) * s >> 7;
						p->variant = (syRandom() & 15) + 16;
					}
					EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
					btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_HURL);
					state->shakeFrm = 8;
					btEffectTintBattler(
						args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 8);
				}
				for(i = 0; i != GASP_MOTES; i++) {
					PARTICLE *p = &state->particles[i];

					if(p->variant >= 0) {
						Sint32 n = (p->variant >> 3) + 2;

						EFFECT_DRAW_MOTE(draw[0], canvas, sprs,
							FIXED_INTEGER_PART(p->x), FIXED_INTEGER_PART(p->y),
							n);
						btParticleAdvanceWithGravity2D(p, 60, toFIXED(1) / 64);
						p->variant--;
					}
				}
			}
		} else if(mode == GASP_MODE_LULL) {
			/* The small Djinni sways over the targets and rises from tick
			   45. */
			Sint32 b = frm * (TRIG_TURN / 32);
			Sint32 x;
			Sint32 y;

			x = (FIXEDtoINT(sySin(b) * GASP_DJINNI_SWAY)) + screen[0] / 2 -
				SMALL_DJINNI_WIDTH / 2;
			y = (FIXEDtoINT(syCos(b) * 4)) + screen[1] - 24;
			if(frm > 44) {
				twice = frm * 2;
				y = y - twice + 88;
			}
			draw[0](canvas, FIGURE(state), x, y, SMALL_DJINNI_WIDTH,
				SMALL_DJINNI_HEIGHT);
			btEffectUpdatePhasePalette(
				frm, TRIG_TURN - DEGtoANG(120), DEGtoANG(120), 0);
		} else if(mode == GASP_MODE_REFLUX || mode == GASP_MODE_COUNTER_RUSH) {
			/* Reflux's Djinni circles over the target; motes swirl in on it. */
			SCREEN_POINT tgt;

			if(mode == GASP_MODE_REFLUX) {
				Sint32 r = frm;

				x = screen[0] / 2 - SMALL_DJINNI_WIDTH / 2;
				y = screen[1] - SMALL_DJINNI_HEIGHT / 2;
				if(r > GASP_REFLUX_RADIUS)
					r = GASP_REFLUX_RADIUS;
				a = frm * (TRIG_TURN / 128);
				x -= FIXEDtoINT(sySin(a) * r * 2);
				y -= (FIXEDtoINT(syCos(a) * r)) - r;
				if(frm <= 61)
					draw[0](canvas, FIGURE(state), x, y, SMALL_DJINNI_WIDTH,
						SMALL_DJINNI_HEIGHT);
			}
			if(frm == 0)
				sySndPlayStub(SE_SWIRL);
			if(frm == 62)
				sySndPlayStub(SE_DJINNI_SET);
			if(frm == 0) {
				for(i = 0; i != GASP_MOTES; i++) {
					PARTICLE *p = &state->particles[i];

					p->z = syRandom();
					p->velZ = (syRandom() & 63) + 64;
					syRandom();
					p->variant = i + GASP_SWIRL_MOTE_TICKS;
				}
			}
			btEffectPosProjectScaledBattler(args->tgts[0], &tgt);
			for(i = 0; i != GASP_SWIRL_MOTES; i++) {
				PARTICLE *p = &state->particles[i];

				if((Uint32)p->variant < GASP_SWIRL_MOTE_TICKS) {
					Sint32 n = p->variant / 8 + 2;
					Sint32 x =
						(FIXEDtoINT(sySin(p->z) * p->velZ / 2)) + tgt.x / 2;
					Sint32 y = (FIXEDtoINT(syCos(p->z) * p->velZ)) + tgt.y;

					p->z += 512;
					p->velZ -= 4;
					if(p->velZ > 0)
						EFFECT_DRAW_MOTE(draw[0], canvas, sprs, x, y, n);
				}
				p->variant--;
			}
		} else if(mode == GASP_MODE_SHINE) {
			/* The Djinni glides up over the user, then falls on the target and
			   strikes at tick 55; its place in the battle is in the
			   velocity. */
			PARTICLE	*ball = &state->particles[GASP_CENTRE];
			SCREEN_POINT pos;
			Sint32		 x;
			Sint32		 y;

			if(frm == 0) {
				ball->velX = other[0];
				ball->velY = other[1];
				ball->velZ = other[2];
			}
			if(frm <= 47) {
				MOTION_OBJECT *user = btObjectGetStub(args->user)->obj;

				ball->velX += (user->x - other[0]) / GASP_SHINE_GLIDE;
				ball->velY += (toFIXED(60) - other[1]) / GASP_SHINE_GLIDE;
				ball->velZ += (user->z - other[2]) / GASP_SHINE_GLIDE;
			} else {
				MOTION_OBJECT *tgt = btObjectGetStub(args->tgts[0])->obj;
				PARTICLE	  *fall = &state->particles[GASP_FALL];

				if(frm == 48) {
					SET_POSITION(fall, 0, 0, 0);
					fall->velX = (tgt->x - ball->velX) / GASP_SHINE_FALL;
					fall->velY = -ball->velY / GASP_SHINE_FALL;
					fall->velZ = (tgt->z - ball->velZ) / GASP_SHINE_FALL;
				}
				ball->velX += fall->x;
				ball->velY += fall->y;
				ball->velZ += fall->z;
				fall->x += fall->velX;
				fall->y += fall->velY;
				fall->z += fall->velZ;
			}
			syInitMatrix();
			syLookAtLoad(camera->eye, camera->focus);
			btEffectPosProjectPoint(&ball->velX, &pos);
			x = pos.x >> 1;
			pos.x = x;
			ball->x = x;
			y = pos.y;
			ball->y = y;
			draw[0](canvas, FIGURE(state), x - SMALL_DJINNI_WIDTH / 2,
				y - SMALL_DJINNI_HEIGHT / 2, SMALL_DJINNI_WIDTH,
				SMALL_DJINNI_HEIGHT);
			if(frm == 0)
				sySndPlayStub(SE_SPIN);
			if(frm == 55) {
				PARTICLE *flash = &state->particles[GASP_FLASH];

				flash->x = pos.x;
				flash->y = pos.y;
				EFFECT_SHOW_DAMAGE(SE_CRASH);
				btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_HURL);
				btEffectTintBattler(
					args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 24);
				state->shakeFrm = 16;
			}
			btCameraShake(8, 8);
		} else if(mode == GASP_MODE_GASP) {
			/* The small Djinni sways over the targets and rises from tick
			   49. */
			Sint32 b = frm * (TRIG_TURN / 32);
			Sint32 x;
			Sint32 y;

			x = (FIXEDtoINT(sySin(b) * GASP_DJINNI_SWAY)) + screen[0] / 2 -
				SMALL_DJINNI_WIDTH / 2;
			y = (FIXEDtoINT(syCos(b) * 4)) + screen[1] - 24;
			if(frm > 48) {
				twice = frm * 2;
				y = y - twice + 96;
			}
			if(y > -SMALL_DJINNI_HEIGHT)
				draw[0](canvas, FIGURE(state), x, y, SMALL_DJINNI_WIDTH,
					SMALL_DJINNI_HEIGHT);
		}

		/* The model layer. */
		syInitMatrix();
		ProjectionWork.centerY = 104;
		verts = syHeapAllocEWRAM(1024);
		model = btRenderAllocateModelList(1);
		model->texture = &texture;
		model->vertices = verts;
		if(mode == GASP_MODE_GASP) {
			/* Burning skulls rise from the ground, swaying, one every 8
			   ticks. */
			if(frm == 0) {
				for(i = 0; i != GASP_SEEDS; i++) {
					PARTICLE *p = &state->particles[i];

					p->x = i * DEGtoANG(180);
					p->y = toFIXED(56);
					p->velX = (syRandom() & 511) + 512;
					p->velY = -((syRandom() & 127) + 612) << 7;
					p->variant = -i * 8;
				}
			}
			if(frm == 4)
				sySndPlayStub(SE_BEAST_CRY);
			if(frm == 48)
				EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
			texture.logWidth = 5;
			texture.logHeight = 5;
			model->kind = MODEL_SHADED_ADDITIVE;
			model->mesh = BattleEffectSmallSquareMeshReversed;
			for(i = 0; i != GASP_SKULLS; i++) {
				PARTICLE *p = &state->particles[i];

				if(p->variant >= 0) {
					FIXED  size = GASP_SKULL_SCALE;
					Sint32 shade = -16;

					if(p->variant <= 6)
						shade = p->variant * 8 + MODEL_SHADE_INVISIBLE;
					if(p->variant >= GASP_SKULL_TICKS) {
						shade = (GASP_SKULL_TICKS - p->variant) * 4;
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
					btRenderTransformVertices(
						BattleEffectGroundSquareCorners, verts, 4);
					btRenderDrawModelList(model);
				}
				p->variant++;
			}
		} else if(mode == GASP_MODE_LULL) {
			/* A ring of music notes turns and spreads. */
			Sint32 k;

			texture.logWidth = 5;
			texture.logHeight = 5;
			model->kind = MODEL_SHADED_MAX;
			model->texture = &texture;
			model->mesh = BattleEffectSmallSquareMeshReversed;
			model->vertices = verts;
			/* One ring: the loop that could start another every 32 ticks runs
			   once. */
			for(k = 0; k != 1; k++) {
				if(frm >= k * 32) {
					Sint32 t = frm - k * 32;
					Sint32 scale = t * (toFIXED(1) / 64) + toFIXED(1) / 16;
					Sint32 shade = 0;

					if(t >= GASP_LULL_RING_TICKS) {
						shade = (GASP_LULL_RING_TICKS - t) * 4;
						if(shade < MODEL_SHADE_INVISIBLE)
							continue;
					}
					model->shade = shade;
					syInitMatrix();
					syScale(scale >> 1, scale, scale);
					syRotZ(frm * (TRIG_TURN / 256) + TRIG_TURN * 3 / 4);
					syRotX(frm * (TRIG_TURN / 1024) - GASP_LULL_TILT);
					for(i = 0; i != GASP_LULL_NOTES; i++) {
						texture.pixels =
							state->sheet + i % 3 * GASP_TEXTURE_BYTES;
						syPushMatrix();
						syRotZ(frm * 356 + i * (TRIG_TURN / GASP_LULL_NOTES));
						syTranslate(0, toFIXED(116), 0);
						syScale(scale >> 1, scale >> 1, scale >> 1);
						btRenderTransformVertices(
							BattleEffectGroundSquareCorners, verts, 4);
						syPopMatrix();
						btRenderDrawModelList(model);
					}
				}
			}
		} else if(mode == GASP_MODE_UNUSED) {
			/* A burst of light swells at each target. */
			model->shade = 0;
			texture.logWidth = 6;
			texture.logHeight = 6;
			model->mesh = BattleEffectSquareMesh;
			model->kind = MODEL_SHADED_MAX;
			texture.pixels = state->sheet;
			for(i = 0; i != args->tgtCnt; i++) {
				SCREEN_POINT pos;

				btEffectPosProjectScaledBattler(args->tgts[i], &pos);
				syInitMatrix();
				syTranslate(
					INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48), 0);
				syRotX(DEGtoANG(90));
				syRotY((frm << 9) - TRIG_TURN / 16);
				syScale(frm * 2596 >> 1, frm * 2596 >> 1, frm * 2596 >> 1);
				btRenderTransformVertices(
					BattleEffectGroundSquareCorners, verts, 4);
				btRenderDrawModelList(model);
			}
		} else if(mode == GASP_MODE_SHINE) {
			/* Rays of light turn about the Djinni as it glides; three rings
			   flare at the target as it strikes. */
			if(frm <= 47) {
				PARTICLE *ball;
				Sint32	  twist;

				model->shade = -16;
				texture.logWidth = 5;
				texture.logHeight = 2;
				texture.pixels =
					state->sheet + GASP_FAMILY_CORNER_GLOW_PICTURES;
				model->texture = &texture;
				model->kind = MODEL_SHADED_ADDITIVE;
				model->mesh = GaspFamilyStreakMesh;
				model->vertices = verts;
				if(frm == 0) {
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
				twist = frm * 2;
					i != GASP_SHINE_RAYS; i++) {
					PARTICLE *p = &state->particles[i];

					p->variant++;
					p->x += p->y * 2;
					model->scrollV =
						(twist + i * 7) & GASP_SHINE_RAY_SCROLL_MASK;
					if((Uint32)p->variant < GASP_SHINE_RAY_TICKS) {
						syInitMatrix();
						syTranslate(toFIXED(ball->x) - toFIXED(64),
							toFIXED(ball->y) - toFIXED(64), 0);
						syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1));
						syRotZ(p->x);
						syScale(sySin(p->variant * (TRIG_TURN / 96)) * 3 +
								toFIXED(3) / 4,
							toFIXED(2), toFIXED(1));
						btRenderTransformVertices(
							BattleEffectTaperedBeamCorners, verts, 4);
						btRenderDrawModelList(model);
					}
				}
			}
			{
				SCREEN_POINT pos;

				btEffectPosProjectBattler(args->tgts[0], &pos);
				texture.logWidth = 7;
				texture.logHeight = 7;
				texture.pixels = cells;
				model->kind = MODEL_SHADED_ADDITIVE;
				model->texture = &texture;
				model->mesh = BattleEffectLargeSquareMesh;
				model->vertices = verts;
				for(i = 0; i != 3; i++) {
					Sint32 t = frm - i * 4 - 55;

					if(t >= 0) {
						Sint32 scale =
							t * 3 * (toFIXED(1) / 16) + toFIXED(1) / 4;
						Sint32 shade = 0;

						if(t > 7)
							shade = (8 - t) * 8;
						model->shade = shade;
						if(shade > MODEL_SHADE_INVISIBLE) {
							syInitMatrix();
							syTranslate(INTtoFIXED(pos.x / 2 - 64),
								INTtoFIXED(pos.y - 64), 0);
							syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
							syRotZ(i * (DEGtoANG(90)) + DEGtoANG(45));
							syRotX(-TRIG_TURN / 16);
							syMatrixScaleUniform(scale);
							btRenderTransformVertices(
								BattleEffectGroundSquareCorners, verts, 4);
							btRenderDrawModelList(model);
						}
					}
				}
			}
		} else if(mode == GASP_MODE_MOLD) {
			/* The Djinni's sprite, twice, shrinks onto the target. */
			if((Uint32)(frm - 16) <= 64) {
				Sint32		 t = frm - 16;
				Sint32		 scale;
				SCREEN_POINT pos;

				if(t < GASP_MOLD_SLOW_FADE_TICKS)
					state->transferValue = CANVAS_FADE_THREE_QUARTERS;
				else
					state->transferValue = CANVAS_FADE_HALF;
				texture.logWidth = 6;
				texture.logHeight = 6;
				model->mesh = BattleEffectSquareMesh;
				model->kind = MODEL_SHADED_ADDITIVE;
				texture.pixels = state->sheet;
				btEffectPosProjectScaledBattler(args->tgts[0], &pos);
				for(i = 0, scale = t * -GASP_MOLD_SHRINK + toFIXED(1); i != 2;
					scale += toFIXED(1) / 4, i++) {
					if(scale > 0) {
						Sint32 shade = i * (MODEL_SHADE_INVISIBLE / 4);

						if(shade > 0)
							shade = 0;
						model->shade = shade;
						syInitMatrix();
						syTranslate(INTtoFIXED(pos.x / 2 - 64),
							INTtoFIXED(pos.y - 48), 0);
						syRotX(TRIG_TURN * 3 / 4);
						syScale(scale, scale, scale);
						btRenderTransformVertices(
							BattleEffectGroundSquareCorners, verts, 4);
						btRenderDrawModelList(model);
					}
				}
			}
			if(frm == 14) {
				sySndPlayStub(SE_POWER_UP);
			}
			if(frm == 24) {
				btEffectTintBattler(
					args->tgts[0], SPRITE_COLOR_WHITE, TINT_KEEP, 0, 12);
			}
		} else if(mode == GASP_MODE_EMU) {
			/* The user dashes in a trail of dust; at the blow motes burst and a
			   ring of fire swells at the target, larger for the stronger
			   variants. */
			SCREEN_POINT from;
			PARTICLE	*p;

			btEffectPosProjectBattler(args->user, &from);
			if(frm == 0) {
				SCREEN_POINT to;
				PARTICLE	*ball = &state->particles[GASP_CENTRE];
				PARTICLE	*q;

				btEffectPosProjectScaledBattler(args->tgts[0], &to);
				ball->x = to.x;
				ball->y = to.y;
				EFFECT_FREE_PARTICLES(i, state->particles, GASP_EMU_PUFFS);
				for(i = 0, q = EXTRA_PARTICLES; i != GASP_EMU_MOTES; i++, q++) {
					Sint32 speed = 511;
					Sint32 a;

					speed &= syRandom();
					a = syRandom();
					speed += GASP_EMU_MOTE_SPEED_MIN;
					q->velX = sySin(a) * speed >> 6;
					q->velY = syCos(a) * speed >> 4;
					if(args->variant == EFFECT_VARIANT_BASIC) {
						q->velX /= 4;
						q->velY /= 4;
					} else if(args->variant == EFFECT_VARIANT_STRONGER) {
						q->velX /= 2;
						q->velY /= 2;
					}
					q->x = toFIXED(to.x / 2);
					q->y = INTtoFIXED(to.y + 16);
					q->variant = (syRandom() & 15) + 8;
				}
			}
			if(frm <= 9) {
				Sint32 v;
				Sint32 mask = 255;

				i = 0;
				twice = frm * 2;
				p = (void *)(twice * sizeof(PARTICLE) +
					(Sint32)state->particles);
				p->x = toFIXED(from.x / 2);
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

				if((Uint32)p->variant < SMOKE_PUFF_TICKS) {
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
			if(frm > 9) {
				for(i = 0; i != GaspFamilyCounts[args->variant]; i++) {
					PARTICLE *q = &EXTRA_PARTICLES[i];

					if(q->variant >= 0) {
						Sint32 n = q->variant / 4 + 3;

						EFFECT_DRAW_MOTE(draw[0], canvas, sprs,
							FIXED_INTEGER_PART(q->x), FIXED_INTEGER_PART(q->y),
							n);
						btParticleAdvanceWithGravity2D(q, 62, 0);
						q->variant--;
					}
				}
			}
			if(frm > 9) {
				PARTICLE *ball = &state->particles[GASP_CENTRE];
				Sint32	  t = frm - 10;
				Sint32	  scale = t * (toFIXED(1) / 2) + toFIXED(1) / 4;
				Sint32	  shade = 0;

				if(args->variant == EFFECT_VARIANT_BASIC)
					scale /= 4;
				else if(args->variant == EFFECT_VARIANT_STRONGER)
					scale /= 2;
				if(frm > 15)
					shade = (16 - frm) * 8;
				if(shade > MODEL_SHADE_INVISIBLE) {
					model->shade = shade;
					texture.logWidth = 7;
					texture.logHeight = 7;
					model->mesh = BattleEffectLargeSquareMesh;
					model->kind = MODEL_SHADED_ADDITIVE;
					texture.pixels = state->sheet + GASP_FIRE_RING;
					syInitMatrix();
					syTranslate(INTtoFIXED(ball->x / 2 - 64),
						INTtoFIXED(ball->y - 48), 0);
					syScale(scale / 4, scale, scale / 2);
					btRenderTransformVertices(
						BattleEffectSquareCorners, verts, 4);
					btRenderDrawModelList(model);
				}
			}
			if(frm == 10) {
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
				state->shakeFrm = args->variant * 2 + 8;
				btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_NUDGE);
				btEffectTintBattler(
					args->tgts[0], SPRITE_COLOR_WHITE, POSE_DAMAGE, 0, 12);
			}
			btCameraShake(args->variant * 2 + 8, args->variant * 2 + 8);
		} else if(mode == GASP_MODE_BEAT_DANCE) {
			/* Music notes fly up from the target as it flushes red. */
			if(frm == 0) {
				SCREEN_POINT pos;

				btEffectPosProjectBattler(args->tgts[0], &pos);
				for(i = 0; i != GASP_NOTE_SEEDS; i++) {
					PARTICLE *p = &state->particles[i];
					Sint32	  speed = 255;
					Sint32	  a;

					speed &= syRandom();
					a = (syRandom() & (DEGtoANG(180) - 1)) + DEGtoANG(90);
					speed += GASP_NOTE_SPEED_MIN;
					p->velX = sySin(a) * speed >> 7;
					p->velY = syCos(a) * speed >> 6;
					p->velZ = toFIXED(1) / 64;
					SET_POSITION(p, toFIXED(pos.x / 2),
						INTtoFIXED(pos.y - (syRandom() & 31) + 16), syRandom());
				}
				sySndPlayStub(SE_POWER_UP);
				state->shakeFrm = 16;
				btEffectTintBattler(
					args->tgts[0], SPRITE_COLOR_RED_TINT, TINT_KEEP, 0, 32);
			}
			texture.logWidth = 5;
			texture.logHeight = 5;
			model->kind = MODEL_SHADED_MAX;
			model->texture = &texture;
			model->mesh = BattleEffectSmallSquareMeshReversed;
			model->vertices = verts;
			for(i = 0; i != GASP_NOTES; i++) {
				PARTICLE *p = &state->particles[i];

				if(frm >= i * 2) {
					Sint32 t = frm - i * 2;
					Sint32 shade = 0;

					if(t >= GASP_NOTE_TICKS) {
						shade = (GASP_NOTE_TICKS - t) * 4;
						if(shade < MODEL_SHADE_INVISIBLE)
							continue;
					}
					model->shade = shade;
					texture.pixels = state->sheet + i % 3 * GASP_TEXTURE_BYTES;
					PLACE_PARTICLE(p);
					syMatrixScaleUniform(toFIXED(1) / 2);
					btRenderTransformVertices(
						BattleEffectSquareCorners, verts, 4);
					btRenderDrawModelList(model);
					btParticleAdvanceWithGravity3D(p, 60, 0);
				}
			}
			btCameraShake(8, 8);
		} else if(mode == GASP_MODE_CLARION_CRY) {
			/* Music notes fly up from the user. */
			if(frm == 0) {
				SCREEN_POINT pos;

				btEffectPosProjectBattler(args->user, &pos);
				for(i = 0; i != GASP_NOTE_SEEDS; i++) {
					PARTICLE *p = &state->particles[i];
					Sint32	  speed = 255;
					Sint32	  a;

					speed &= syRandom();
					a = (syRandom() & (DEGtoANG(180) - 1)) + DEGtoANG(90);
					speed += GASP_NOTE_SPEED_MIN;
					p->velX = sySin(a) * speed >> 6;
					p->velY = syCos(a) * speed >> 6;
					p->velZ = toFIXED(1) / 64;
					SET_POSITION(p, toFIXED(pos.x / 2),
						toFIXED(pos.y - (syRandom() & 31) - 16), syRandom());
				}
				sySndPlayStub(SE_BIRD_CALL);
			}
			texture.logWidth = 5;
			texture.logHeight = 5;
			model->kind = MODEL_SHADED_MAX;
			model->texture = &texture;
			model->mesh = BattleEffectSmallSquareMeshReversed;
			model->vertices = verts;
			for(i = 0; i != GASP_NOTES; i++) {
				PARTICLE *p = &state->particles[i];

				if(frm >= i * 2) {
					Sint32 t = frm - i * 2;
					Sint32 shade = 0;

					if(t >= GASP_NOTE_TICKS) {
						shade = (GASP_NOTE_TICKS - t) * 4;
						if(shade < MODEL_SHADE_INVISIBLE)
							continue;
					}
					model->shade = shade;
					texture.pixels = state->sheet + i % 3 * GASP_TEXTURE_BYTES;
					PLACE_PARTICLE(p);
					syMatrixScaleUniform(toFIXED(1) / 2);
					btRenderTransformVertices(
						BattleEffectSquareCorners, verts, 4);
					btRenderDrawModelList(model);
					btParticleAdvanceWithGravity3D(p, 60, 0);
				}
			}
		} else if(mode == GASP_MODE_CALL_DULLAHAN) {
			/* The user raises its arm, four bolts swing out from it onto the
			   target, and sparks burst and bounce there. */
			SCREEN_POINT pos;

			if(frm == 0) {
				void *user = btObjectGetStub(args->user)->obj;

				/* Another pose first, so that the leap starts over, held on its
				   first picture until tick 14. */
				fdActorSelectAnimationStub(user, POSE_STILL);
				fdActorSelectAnimationStub(user, POSE_LEAP);
				fdActorSetAnimationSpeedStub(user, 0);
				sySndPlayStub(SE_POWER_UP);
			}
			if(frm == 14)
				fdActorSetAnimationSpeedStub(
					btObjectGetStub(args->user)->obj, ANIM_STEP_NORMAL);
			if(frm == 22)
				sySndPlayStub(SE_POWER_UP);
			if(frm == 26) {
				EFFECT_SHOW_DAMAGE(SE_CLATTER);
				state->shakeFrm = 8;
				EFFECT_STRIKE_TARGET(args, 0, 12, KNOCKBACK_HURL);
				syWordsFillEntry(canvas, CANVAS_BYTES,
					CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			}
			if(frm > 27) {
				PARTICLE *q;
				PARTICLE *p;

				if(frm == 28) {
					btEffectPosProjectScaledBattler(args->tgts[0], &pos);
					for(q = EXTRA_PARTICLES, i = 0; i != GASP_DULLAHAN_SPARKS;
						i++, q++) {
						Sint32 speed = 511;
						Sint32 a;

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
						Sint32 j;

						/* Each spark moves four steps a tick, drawn at
						   each. */
						for(j = 0; j != 4; j++) {
							Sint32 n = p->variant / 8 + 1;

							EFFECT_DRAW_MOTE(draw[0], canvas, sprs,
								FIXED_INTEGER_PART(p->x),
								FIXED_INTEGER_PART(p->y), n);
							btParticleAdvanceWithGravity2D(
								p, 63, toFIXED(1) / 32);
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
			model->mesh = BattleEffectColumnMesh;
			model->vertices = verts;
			syInitMatrix();
			syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1));
			if(args->side == EFFECT_SIDE_ENEMIES)
				syTranslate(INTtoFIXED(pos.x - 136), INTtoFIXED(pos.y - 48), 0);
			else
				syTranslate(INTtoFIXED(pos.x - 108), INTtoFIXED(pos.y - 56), 0);
			for(i = 0; i != 4; i++) {
				Sint32 scale;

				syPushMatrix();
				texture.pixels =
					state->sheet + (syRandom() & 3) * GASP_BOLT_PICTURE_BYTES;
				texture.logWidth = 5;
				texture.logHeight = 7;
				if(frm > 23) {
					Sint32 d = frm * 16 - i * 2 - 352;
					Sint32 ang;

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
					ang = d * GASP_DULLAHAN_SWING / GASP_DULLAHAN_REACH +
						TRIG_TURN * 3 / 8;
					if(args->side == EFFECT_SIDE_ENEMIES)
						ang = -ang;
					syRotZ(ang);
				} else {
					if(i != 0)
						continue;
					syRotZ(args->side == EFFECT_SIDE_ENEMIES
							? -TRIG_TURN * 3 / 8
							: TRIG_TURN * 3 / 8);
				}
				model->shade = -i * 8;
				if(frm <= 23)
					scale = frm * (toFIXED(1) / 24);
				else if(frm > 31)
					scale = toFIXED(4) - frm * 3 * (toFIXED(1) / 32);
				else
					scale = toFIXED(1);
				if(scale > 0) {
					syScale(scale * 6, scale * 2, toFIXED(1));
					btRenderTransformVertices(
						BattleEffectLongBeamCorners, verts, 4);
					btRenderDrawModelList(model);
					syPopMatrix();
				}
			}
			btCameraShake(8, 8);
		} else if(mode == GASP_MODE_DEMON_EYE) {
			/* Two eyes of light open and close at the user, and the targets
			   flash. */
			SCREEN_POINT pos;

			state->transferValue = CANVAS_FADE_HALF;
			texture.logWidth = 7;
			texture.logHeight = 7;
			model->mesh = BattleEffectLargeSquareMesh;
			model->kind = MODEL_SHADED_MAX;
			EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_CLEAR, 0);
			texture.pixels = state->sheet;
			btEffectPosProjectScaledBattler(args->user, &pos);
			if(frm <= 15) {
				for(i = 0; i != 2; i++) {
					Sint32 scale = sySin(frm * (TRIG_TURN / 32)) * 5;

					if(scale > 0) {
						model->shade = 0;
						syInitMatrix();
						syTranslate(INTtoFIXED(pos.x / 2 + i * 16 - 72),
							INTtoFIXED(pos.y - 64), 0);
						syScale(scale / 2, scale, scale / 2);
						syRotZ(DEGtoANG(45));
						btRenderTransformVertices(
							BattleEffectSquareCorners, verts, 4);
						btRenderDrawModelList(model);
					}
				}
			}
			if(frm == 8) {
				sySndPlayStub(SE_POWER_UP);
				state->shakeFrm = 8;
				syWordsFillEntry(canvas, CANVAS_BYTES,
					CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
				for(i = 0; i != args->tgtCnt; i++)
					btEffectTintBattler(
						args->tgts[i], SPRITE_COLOR_WHITE, POSE_DAMAGE, i, 7);
			}
			btCameraShake(8, 8);
		} else if(mode == GASP_MODE_CAGE) {
			/* Eight turning rings make a cage around the target, then close on
			   it. */
			SCREEN_POINT pos;
			Sint32		 shade;

			if(frm == 0)
				sySndPlayStub(SE_LAUNCH);
			btEffectPosProjectBattler(args->tgts[0], &pos);
			shade = frm * 4 + MODEL_SHADE_INVISIBLE;
			if(shade >= -16)
				shade = -16;
			model->shade = shade;
			model->texture = &texture;
			model->vertices = verts;
			model->kind = MODEL_SHADED_MAX;
			model->mesh = BattleEffectCylinderMesh;
			texture.pixels = state->sheet;
			texture.logWidth = 7;
			texture.logHeight = 4;
			if(frm == 26) {
				state->shakeFrm = 8;
				EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			}
			for(i = 0; i != 8; i++) {
				Sint32 scale = toFIXED(2);
				Sint32 start = i * 2 + GASP_CAGE_CLOSE_TICK;

				if(frm >= start)
					scale = toFIXED(2) - (frm - start) * (toFIXED(1) / 8);
				if(scale > toFIXED(1) / 2) {
					model->scrollU = ((frm + i * 5) & 7) << 4;
					syInitMatrix();
					syTranslate(INTtoFIXED(pos.x / 2 - 60), toFIXED(8), 0);
					syScale(scale >> 1, scale, toFIXED(1));
					syRotX(i * DEGtoANG(45) + GASP_CAGE_TILT);
					syRotY(frm * (TRIG_TURN / 128));
					syRotZ((frm + i * 16) * (TRIG_TURN / 128));
					syScale(toFIXED(1), toFIXED(1) / 2, toFIXED(1));
					btRenderTransformVertices(
						BattleEffectCylinderCorners, verts, 32);
					btRenderDrawModelList(model);
				}
			}
		} else if(mode == GASP_MODE_ELEMENT_SWAP) {
			/* A ring shrinks onto the user and a glow turns there while motes
			   close in, and the colours step through Mercury's, Mars's and
			   Jupiter's. */
			SCREEN_POINT pos;
			Sint32		 j;

			texture.logWidth = 7;
			texture.logHeight = 7;
			texture.pixels = cells;
			model->mesh = BattleEffectLargeSquareMesh;
			model->kind = MODEL_SHADED_MAX;
			btEffectPosProjectScaledBattler(args->user, &pos);
			syInitMatrix();
			syTranslate(INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48), 0);
			if(frm <= 15) {
				Sint32 scale = toFIXED(3) - frm * 3 * (toFIXED(1) / 16);

				model->shade = 0;
				syScale(scale >> 1, scale, scale >> 1);
				btRenderTransformVertices(BattleEffectSquareCorners, verts, 4);
				btRenderDrawModelList(model);
			}
			syInitMatrix();
			syTranslate(INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48), 0);
			if((Uint32)(frm - 12) <= 31) {
				Sint32 scale =
					sySin(frm * (TRIG_TURN / 64) - TRIG_TURN * 3 / 16) * 3;

				texture.pixels = state->sheet;
				syScale(scale / 2, scale, scale / 2);
				syRotZ((frm << 9) + DEGtoANG(45));
				btRenderTransformVertices(BattleEffectSquareCorners, verts, 4);
				btRenderDrawModelList(model);
			}
			/* A mote's x is its distance from the user in sixteenths of a
			   pixel, 48 to 142 pixels at first, closing in over 48 steps, three
			   a tick; its y is its angle. */
			if(frm == 0) {
				for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
					PARTICLE *p = &state->particles[i];

					syRandom();
					p->x = 768 + i * 24;
					p->velX = -p->x / 48;
					p->y = syRandom();
				}
				LOAD_BG_PALETTE(RESOURCE_BLAST_SHEET);
			}
			if(frm <= 15) {
				for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
					PARTICLE *p = &state->particles[i];

					for(j = 0; j != 3; j++) {
						Sint32 x = (FIXEDtoINT(sySin(p->y) * p->x) >>
									   (GASP_SWAP_MOTE_SHIFT + 1)) +
							pos.x / 2;
						Sint32 y = (FIXEDtoINT(p->x * syCos(p->y)) >>
									   GASP_SWAP_MOTE_SHIFT) +
							60;
						Sint32 n = 4;

						EFFECT_DRAW_MOTE(draw[0], canvas, sprs, x, y, n);
						p->x += p->velX;
					}
				}
			}
			/* Three steps a tick towards each Djinni's colours in turn. */
			for(i = 0; i != 3; i++) {
				if(frm > 7) {
					if(frm <= 15)
						btEffectStepPaletteToResource(
							RESOURCE_MERCURY_DJINN_SHEET);
					else if(frm <= 23)
						btEffectStepPaletteToResource(
							RESOURCE_MARS_DJINN_SHEET);
					else
						btEffectStepPaletteToResource(
							RESOURCE_JUPITER_DJINN_SHEET);
				}
			}
			if(frm == 16) {
				sySndPlayStub(SE_POWER_UP);
				btEffectTintBattler(
					args->tgts[0], SPRITE_COLOR_WHITE, TINT_KEEP, 0, 30);
			}
			if(frm == 28)
				EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
		} else if(mode == GASP_MODE_PSY_BOOST) {
			/* Two rings shrink onto the user and a glow turns there, in
			   grey. */
			SCREEN_POINT pos;

			texture.logWidth = 7;
			texture.logHeight = 7;
			texture.pixels = cells;
			model->mesh = BattleEffectLargeSquareMesh;
			model->kind = MODEL_SHADED_MAX;
			btEffectPosProjectScaledBattler(args->user, &pos);
			for(i = 0; i != 2; i++) {
				Sint32 t = frm - i * 8;

				syInitMatrix();
				syTranslate(
					INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48), 0);
				if((Uint32)t < GASP_PSY_BOOST_RING_TICKS) {
					Sint32 scale = toFIXED(4) - t * (toFIXED(1) / 4);
					Sint32 shade = 0;

					if(t <= 7)
						shade = t * 8 + MODEL_SHADE_INVISIBLE;
					model->shade = shade;
					syScale(scale / 2, scale, scale / 2);
					syRotZ(i * (DEGtoANG(90)) + DEGtoANG(45));
					syRotX(-TRIG_TURN / 16);
					btRenderTransformVertices(
						BattleEffectGroundSquareCorners, verts, 4);
					btRenderDrawModelList(model);
				}
			}
			syInitMatrix();
			syTranslate(INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48), 0);
			if((Uint32)(frm - 12) <= 31) {
				Sint32 scale =
					sySin(frm * (TRIG_TURN / 64) - TRIG_TURN * 3 / 16) * 2;

				texture.pixels = state->sheet;
				syScale(scale / 2, scale, scale / 2);
				syRotZ((frm << 9) + DEGtoANG(45));
				btRenderTransformVertices(BattleEffectSquareCorners, verts, 4);
				btRenderDrawModelList(model);
			}
			if(frm == 0)
				btGraphicsLoadGreyRamp();
			if(frm == 16) {
				sySndPlayStub(SE_POWER_UP);
				btEffectTintBattler(
					args->tgts[0], SPRITE_COLOR_WHITE, TINT_KEEP, 0, 30);
			}
			if(frm == 28)
				EFFECT_SHOW_DAMAGE(HIT_SOUND_NONE);
		} else {
			/* Reflux, Counter-rush and Djinn Counter: a swirl grows and turns,
			   at the target or at the user. */
			Sint32 scale;

			model->shade = 0;
			texture.logWidth = 6;
			texture.logHeight = 6;
			model->mesh = BattleEffectSquareMesh;
			model->kind = MODEL_SHADED_ADDITIVE;
			texture.pixels = state->sheet;
			if(mode == GASP_MODE_REFLUX || mode == GASP_MODE_COUNTER_RUSH) {
				if(frm <= 31)
					scale = frm * GASP_SWIRL_GROWTH;
				else if(frm > 59)
					scale = GASP_SWIRL_SCALE + 60 * GASP_SWIRL_SHRINK -
						frm * GASP_SWIRL_SHRINK;
				else
					scale = GASP_SWIRL_SCALE;
			} else {
				if(frm <= 31)
					scale = frm * GASP_SWIRL_GROWTH;
				else if(frm > 39)
					scale = GASP_SWIRL_SCALE + 40 * GASP_SWIRL_SHRINK -
						frm * GASP_SWIRL_SHRINK;
				else
					scale = GASP_SWIRL_SCALE;
			}
			if(scale > 0) {
				SCREEN_POINT pos;

				if(mode == GASP_MODE_REFLUX || mode == GASP_MODE_COUNTER_RUSH)
					btEffectPosProjectScaledBattler(args->tgts[0], &pos);
				else
					btEffectPosProjectScaledBattler(args->user, &pos);
				syInitMatrix();
				syTranslate(
					INTtoFIXED(pos.x / 2 - 64), INTtoFIXED(pos.y - 48), 0);
				syRotX(DEGtoANG(90));
				syScale(scale / 3, scale, scale);
				if(mode == GASP_MODE_REFLUX)
					syRotY(-frm * (TRIG_TURN / 64));
				else
					syRotY(frm * (TRIG_TURN / 64));
				btRenderTransformVertices(
					BattleEffectGroundSquareCorners, verts, 4);
				btRenderDrawModelList(model);
			}
		}
		syHeapRelease(model);
		syHeapRelease(verts);
		ProjectionWork.centerX = BATTLE_PROJECTION_CENTER_X;
		ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
		btEffectCountDownBattlerTints();
		if(mode == GASP_MODE_UNUSED)
			WAIT_TRANSFER(state)
			{
			}
		EFFECT_PRESENT_CANVAS(state);
	}
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	if(mode == GASP_MODE_GASP || mode == GASP_MODE_LULL)
		syCloseEvent(btEffectArmBg2AffineHBlankDMA);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
}

#undef FIGURE
#undef PLACE_PARTICLE
