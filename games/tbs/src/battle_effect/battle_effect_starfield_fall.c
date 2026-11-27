/* Meteor and Outer Space. Part of battle_effect.c. */

#define STARFIELD_FALL_FALL_TICKS  192 /* ticks of the fall */
#define STARFIELD_FALL_BURST_TICKS 54  /* ticks of the burst */

/* The starfield fall's modes (btEffectRunStarfieldFall). */
typedef enum STARFIELD_FALL_MODE {
	STARFIELD_FALL_METEOR, /* one spawned sprite falls */
	/* the user leaps, and two animated sprites fall in the lightning bolt's
	   colours */
	STARFIELD_FALL_OUTER_SPACE
} STARFIELD_FALL_MODE;

/* The pictures in the flame sprite sheet (ResourceId_FlameSheetB), each
   starting where the one before ends: six flames, then three large and three
   small glows, each glow brighter at its centre than the one before. The fall
   draws only the six and the brightest glow of each size. */
#define STARFIELD_FALL_FLAME_SIZE	   32
#define STARFIELD_FALL_LARGE_GLOW_SIZE 24
#define STARFIELD_FALL_SMALL_GLOW_SIZE 8

/* Where each picture begins in the flame sprite sheet, in bytes. */
typedef enum FLAME_SHEET_B_PICTURE {
	/* The six 32x32 flames, in a row */
	STARFIELD_FALL_FLAMES = 0,
	/* The brightest 24x24 glow */
	STARFIELD_FALL_LARGE_GLOW = STARFIELD_FALL_FLAMES +
		6 * STARFIELD_FALL_FLAME_SIZE * STARFIELD_FALL_FLAME_SIZE +
		2 * STARFIELD_FALL_LARGE_GLOW_SIZE * STARFIELD_FALL_LARGE_GLOW_SIZE,
	/* The brightest 8x8 glow */
	STARFIELD_FALL_SMALL_GLOW = STARFIELD_FALL_LARGE_GLOW +
		STARFIELD_FALL_LARGE_GLOW_SIZE * STARFIELD_FALL_LARGE_GLOW_SIZE +
		2 * STARFIELD_FALL_SMALL_GLOW_SIZE * STARFIELD_FALL_SMALL_GLOW_SIZE,
	/* Past the sprite sheet's end: the backdrop's 160 colours, one a line, that
	   the fall writes there. */
	STARFIELD_FALL_BACKDROP_LINES = STARFIELD_FALL_SMALL_GLOW +
		STARFIELD_FALL_SMALL_GLOW_SIZE * STARFIELD_FALL_SMALL_GLOW_SIZE
} FLAME_SHEET_B_PICTURE;

/* Brightens background colours 1 to 63 by a step in each channel. */
void btPaletteBrightenBgEntries(
	Sint32 blueStep, Sint32 greenStep, Sint32 redStep)
{
	Uint16 *col = (Uint16 *)&BG_PLTT[1];
	Sint32	i;

	for(i = 0; i != SHEET_PALETTE_COLORS - 1; i++) {
		Sint32 blue = RGB_BLUE(*col);
		Sint32 green = RGB_GREEN(*col);
		Sint32 red = RGB_RED(*col);

		blue += blueStep;
		green += greenStep;
		red += redStep;
		if(blue > RGB_LEVEL_MAX)
			blue = RGB_LEVEL_MAX;
		if(green > RGB_LEVEL_MAX)
			green = RGB_LEVEL_MAX;
		if(red > RGB_LEVEL_MAX)
			red = RGB_LEVEL_MAX;
		*col++ = RGB(red, green, blue);
	}
}

/* Streams the fall's backdrop colours into colour 0, one each line. */
void btEffectArmPaletteHBlankDMA(void)
{
	BATTLE_EFFECT_STATE *state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	volatile Uint16		*channel = REG_DMA0;
	btDMAStop(channel);
	syDMAXCopy(state->sheet + STARFIELD_FALL_BACKDROP_LINES, BG_PLTT,
		DMA_HBLANK_HALFWORD_PER_LINE, (volatile Uint32 *)channel);
}

/* The battle dissolves into a starfield and a meteor falls through it in flames
   among falling fireballs, then bursts in embers and flashes.

   Abilities: Meteor. */
void btEffectRunMeteor(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStarfieldFall(args, STARFIELD_FALL_METEOR);
}

/* The Fusion Dragon leaps out of sight and falls back through a starfield in
   flames, then bursts in embers and flashes, all in the lightning bolt's
   colours.

   Abilities: Outer Space, the Fusion Dragon's. */
void btEffectRunOuterSpace(BATTLE_EFFECT_PARM *args)
{
	btEffectRunStarfieldFall(args, STARFIELD_FALL_OUTER_SPACE);
}

/* The burst's flashes in PARTICLES, then its embers, and how many of each it
   draws. */
#define STARFIELD_FALL_FLASHES			 128
#define STARFIELD_FALL_EMBER_COUNT		 512
#define STARFIELD_FALL_EMBERS			 (&PARTICLES[STARFIELD_FALL_FLASHES])
#define STARFIELD_FALL_FLASHES_DRAWN	 64
#define STARFIELD_FALL_EMBERS_DRAWN		 64
/* The fall's flames, from state->particles[0], and its landings after them. */
#define STARFIELD_FALL_FLAME_COUNT		 32
#define STARFIELD_FALL_LANDINGS			 16
/* The burst's rising motes, one at each of the 64 state->particles: the
   flames', which it sets out, the landings', drawn where they were left, and 16
   this effect never places, which rise from wherever an earlier effect left
   them. */
#define STARFIELD_FALL_MOTES_DRAWN		 64
/* The starfield's stars. */
#define STARFIELD_FALL_STARS			 256
/* How far down each column's ragged edge starts: the dissolve sweeps the
   picture's rows and this many more. */
#define STARFIELD_FALL_RAGGED_EDGE		 64
/* The backdrop's glow turns blue, and later green, once its level passes
   these. */
#define STARFIELD_FALL_GLOW_BLUE_LEVEL	 32
#define STARFIELD_FALL_GLOW_GREEN_LEVEL	 80
/* The burst's flashes rise from this tick on, three more a tick, and the
   background brightens from STARFIELD_FALL_BRIGHTEN_DELAY ticks after it. */
#define STARFIELD_FALL_FLASH_TICK		 16
#define STARFIELD_FALL_BRIGHTEN_DELAY	 20
/* The blast sprite sheet's flash pictures, each shown two ticks. */
#define STARFIELD_FALL_FLASH_PICTURES	 7
/* An ember is drawn at size STARFIELD_FALL_EMBER_SIZE_MAX at MOTE_DEPTH_NEAR or
   nearer, one size smaller every MOTE_DEPTH_STEP farther. */
#define STARFIELD_FALL_EMBER_SIZE_MAX	 9
/* The fall's own VBlank events run first in the pass, ahead of the image copy:
   the starfield's scroll, then the backdrop's HBlank DMA, so that both are set
   before the tick's first line. */
#define STARFIELD_FALL_SCROLL_PRIORITY	 EVENT_PASS_VBLANK_FIRST
#define STARFIELD_FALL_BACKDROP_PRIORITY (EVENT_PASS_VBLANK_FIRST - 1)

/* The mote pictures in the blast sprite sheet, after its seven blast pictures
   (BLAST_PICTURE_SIZES): two sets, MOTE_SET_BYTES apart. */
#define STARFIELD_FALL_MOTE_PICTURES (16 * 16 + 32 * 32 + 5 * 48 * 48)

/* Meteor and Outer Space: the fall through the starfield, then the burst. */
void btEffectRunStarfieldFall(BATTLE_EFFECT_PARM *args, Sint32 mode)
{
	volatile Uint32		 fill;
	Uint8				 offsets[128];
	void				*canvas;
	DRAW_RECTANGLE		 draw;
	Sint32				 direction;
	BATTLE_EFFECT_STATE *state;
	void				*colors = (void *)BG_PLTT;

	/* Starts a landing where a fireball reaches the ground; it is never
	   drawn. */
	void syStarfieldFallSpawnLanding(Sint32 x, Sint32 y, Sint32 velocityX)
	{
		Sint32 i;

		for(i = 0; i != STARFIELD_FALL_LANDINGS; i++) {
			PARTICLE *landing =
				&state->particles[STARFIELD_FALL_FLAME_COUNT + i];

			if(landing->variant == -1) {
				landing->variant = 0;
				landing->x = x;
				landing->y = y;
				landing->velX = velocityX;
				break;
			}
		}
	}

	/* Starts a flame; the fall starts its own inline. */
	UNUSED void syStarfieldFallSpawnFlame(Sint32 x, Sint32 y)
	{
		Sint32 i;

		for(i = 0; i != STARFIELD_FALL_FLAME_COUNT; i++) {
			PARTICLE *flame = &state->particles[i];

			if(flame->variant == -1) {
				flame->variant = 0;
				flame->x = x;
				flame->y = y;
				break;
			}
		}
	}

	SPR_PLACEMENT pos;
	Sint32		  vector[3];
	SCREEN_POINT  spot;
	SCREEN_POINT  screen;
	SPR_SCALE	  scale;
	Sint32		  frm;
	Sint32		  i;

	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);

	/* In Outer Space the Fusion Dragon leaps out of sight. */
	state->args = args;
	EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
	direction = mode;
	if(direction == STARFIELD_FALL_OUTER_SPACE) {
		ACTOR *actor = btObjectGetStub(state->args->user)->actor;

		actor->velY = toFIXED(10);
		actor->gravity = toFIXED(57) / 100;
		btEffectTintBattler(state->args->user, -1, POSE_LEAP, -1, 0);
		sySndPlayStub(SE_CRASH);
		direction = state->args->side == EFFECT_SIDE_PARTY ? 1 : -1;
	} else {
		direction = -1;
	}
	btEffectConfigureEffectDisp();
	/* The backdrop and colour 1 go black for the wipe. */
	PLTT_BACKDROP = 0;
	BG_PLTT_COLOR(0, 1) = 0;
	state->transferMode = CANVAS_COPY;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	btEffectWipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
	syCloseEvent(btEffectTransferCanvas);
	if(mode == STARFIELD_FALL_OUTER_SPACE) {
		for(i = 0; i != 2; i++) {
			SPR *spr = fdSprCreateStub(
				SPRITE_BATTLE_FUSION_DRAGON + i * SPRITE_SECOND_PART);

			state->sprs[i] = spr;
			if(spr != 0) {
				spr->flg = 0;
				fdSprStartAnimationStub(spr, 2);
				state->sprs[i]->image.pri = 3;
			}
		}
	} else {
		btEffectCreateSprs(1, SPRITE_METEOR, 3);
	}
	/* The starfield. */
	btResourceLoadAndDecompress(RESOURCE_FLAME_SHEET_B, state->sheet, 1, 1);
	if(mode == STARFIELD_FALL_OUTER_SPACE)
		syWordsCopy(colors,
			syResourceGetTableEntry(RESOURCE_LIGHTNING_BOLT_SHEET),
			SHEET_PALETTE_BYTES);
	DMA_FILL32_CELL(
		fill, CANVAS_FILL_WORD(1), Ram_MapGridMetatileBuffer, 256 * 128);
	syWordsCopy(
		BG_CHAR_BLOCK(2), Ram_MapGridMetatileBuffer, BATTLE_BACKGROUND_BYTES);
	sySynch(1);
	REG_BLDCNT = 0;
	REG_BG2PA = Q_8_8(1);
	REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(0) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(31);
	REG_BG2CNT = BGCNT_PRIORITY(3) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
	/* Pale colours at random for BG colours 128 to 190. Nothing shows them: the
	   stars below are colours 64 to 127, which a recording shows in whatever
	   those entries already hold, some of them the battle background's. */
	for(i = 0; i != SHEET_PALETTE_COLORS - 1; i++) {
		Sint32 red = (syRandom() & 15) + 16;
		Sint32 green = (syRandom() & 15) + 16;
		Sint32 blue = (syRandom() & 15) + 16;

		BG_PLTT_COLOR(8, i) = RGB(red, green, blue);
	}
	DMA_FILL32_CELL(fill, 0, canvas, CANVAS_BYTES);
	for(i = 0; i != STARFIELD_FALL_STARS; i++) {
		Sint32 x = syRandom() & (CANVAS_SIZE - 1);
		Sint32 y = syRandom() & (CANVAS_SIZE - 1);
		Sint32 shade = (syRandom() & 63) + 64;

		((Uint8 *)canvas)[CANVAS_PIXEL_OFFSET(x, y)] = shade;
	}
	syWordsCopy(BG_CHAR_BLOCK(1), canvas, CANVAS_BYTES);
	/* The dissolve: the battle background is erased from the top in ragged
	   columns, uncovering the starfield behind it. */
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
	btEffectShowLivingTargets(state->args);
	state->bg2x = 0;
	state->bg2y = 0;
	state->scrollTmr = 0;
	state->scrollInterval = 2;
	state->scrollStepX = direction * Q_8_8(0.5);
	state->scrollStepY = Q_8_8(1);
	sySetEvent(btEffectAdvanceBg2OnInterval, STARFIELD_FALL_SCROLL_PRIORITY);
	sySetEvent(btEffectTransferFullScreenImage, EVENT_PASS_VBLANK);
	for(i = 0; i != ARRAY_COUNT(offsets); i++)
		offsets[i] = syRandom() & (STARFIELD_FALL_RAGGED_EDGE - 1);
	{
		Sint32 limit = 0;
		Sint32 step = 1;
		Sint32 row = 0;

		do {
			limit += step / 2;
			step += 4;
			for(; row != limit; row++) {
				Sint32 x;

				for(x = 0; x != BATTLE_BACKGROUND_WIDTH; x++) {
					Sint32 y = row - offsets[x & 127];

					/* Background pixel (x, y), laid out in 8x8 tiles. */
					if(y >= 0)
						if(y <= 127)
							Ram_MapGridMetatileBuffer
								[((y / 8 * 32 + x / 8) * 8 + (y & 7)) * 8 +
									(x & 7)] = 0;
				}
			}
			EFFECT_PRESENT_CANVAS(state);
		} while(limit < CANVAS_SIZE + STARFIELD_FALL_RAGGED_EDGE);
	}
	/* The fall, drawn on the battle background's image (BG1, over the
	   starfield), which darkens a little each tick to leave trails. */
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	{
		Sint32					 savedX = BgScrollWork[1].x;
		Sint32					 savedY = BgScrollWork[1].y;
		BATTLE_BACKGROUND_STATE *view =
			HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_BACKGROUND);

		BgScrollWork[1].x = 0;
		BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
		/* A blitter for the background's 256-pixel rows. */
		btEffectBuildBlitter(
			HEAP_SLOT_RAM_CODE, 8, 7, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
		draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
		sySetEvent(
			btEffectArmPaletteHBlankDMA, STARFIELD_FALL_BACKDROP_PRIORITY);
		EFFECT_FREE_PARTICLES(
			i, state->particles, ARRAY_COUNT(state->particles));
		view->busy = 1;
		state->frm = 0;
		frm = 0;
		do {
			Sint32	growth = frm << 8;
			Sint32	value = state->frm;
			Sint32	dividend;
			Sint32	glow;
			Uint16 *line;
			Sint32	size;
			Sint32	x;
			Sint32	y;
			Sint32	hold;

			/* FAKEMATCH(temporary): the division written out on a copy of
			 * value. */
			dividend = value;
			/* FAKEMATCH(dead): updates of value that cancel out. */
			if(state) {
				value++;
				value--;
			}
			if(dividend < 0)
				dividend += 3;
			glow = dividend >> 2;
			/* FAKEMATCH(barrier): hold set by an empty asm that reads value and
			 * clobbers r1 and r6. */
			__asm__("" : "=r"(hold) : "r"(value) : "r1", "r6");
			line = (Uint16 *)(state->sheet + STARFIELD_FALL_BACKDROP_LINES);
			if(mode == STARFIELD_FALL_OUTER_SPACE) {
				if((PerRepeat & (KEY_A | KEY_B)) && frm > 16)
					break;
			} else {
				if((PerRepeat & (KEY_A | KEY_B)) && frm > 4)
					break;
			}
			if(frm == 0)
				sySndPlayStub(SE_RUMBLE);
			/* FAKEMATCH(dead): updates of hold that cancel out. */
			if(hold) {
				hold++;
				hold--;
			}
			/* The backdrop: a blue glow that climbs and pales as the fall goes
			   on, over the battle view. Entry i colours line i + 1: the HBlank
			   DMA writes it after line i. */
			for(i = 0; i != BATTLE_VIEW_TOP - 1; i++)
				*line++ = 0;
			for(; i != BATTLE_VIEW_BOTTOM - 1; i++) {
				Sint32 green = i - BATTLE_VIEW_TOP;
				Sint32 blue = green / 4;

				value = blue + glow;
				blue = value - STARFIELD_FALL_GLOW_BLUE_LEVEL;
				green = value - STARFIELD_FALL_GLOW_GREEN_LEVEL;

				CLAMP(blue, 0, RGB_LEVEL_MAX);
				CLAMP(green, 0, RGB_LEVEL_MAX);
				*line++ = RGB(green >> 1, green, blue);
			}
			for(; i != DISPLAY_HEIGHT; i++)
				*line++ = 0;
			if(direction == 1)
				x = frm / 4;
			else
				x = 64 - frm / 4;
			y = 96 - frm;
			/* The falling sprite drifts with the stream and grows. */
			pos.groundHeight = 0;
			pos.height = toFIXED(255);
			if(mode == STARFIELD_FALL_OUTER_SPACE) {
				size = growth + toFIXED(5) / 8;
				scale.x = size;
				scale.y = size;
				pos.x = toFIXED(x) + toFIXED(80);
				pos.y = toFIXED(64 - y);
				fdRenderApplyProjectedPlacementStub(
					state->sprs[0], &pos, &scale, 0);
				fdRenderApplyProjectedPlacementStub(
					state->sprs[1], &pos, &scale, 0);
			} else {
				size = growth + toFIXED(1);
				scale.x = size;
				scale.y = size;
				pos.x = toFIXED(x) + toFIXED(96);
				pos.y = toFIXED(96 - y);
				fdRenderApplyProjectedPlacementStub(
					state->sprs[0], &pos, &scale, 0);
			}
			/* A new flame below the falling sprite. */
			for(i = 0, y = 32 - y; i != STARFIELD_FALL_FLAME_COUNT; i++) {
				PARTICLE *flame = &state->particles[i];

				if(flame->variant == -1) {
					Sint32 angle = (syRandom() & 0x7fff) + DEGtoANG(90);

					flame->variant = 0;
					flame->x = INTtoFIXED(x + 96) +
						sySin(angle) * 30 / toFIXED(1) * size;
					flame->y =
						toFIXED(y) - syCos(angle) * 30 / toFIXED(1) * size;
					break;
				}
			}
			/* Seven sparks orbit the falling sprite. The orbit's centre is set
			   in screen, which the burst's embers are later projected into: the
			   tick has no other slot for it. */
			screen.x = 0;
			screen.y = 0;
			screen.depth = toFIXED(512);
			syInitMatrix();
			syMatrixTranslateVector(syScreenPointCoordinates(&screen));
			syRotZ(TRIG_TURN / 32);
			syRotY(frm << 8);
			for(i = 0; i != 7; i++) {
				SET_VECTOR(vector, toFIXED(StarfieldFallOrbitPoints[i][0] / 2),
					toFIXED(StarfieldFallOrbitPoints[i][1] + frm),
					toFIXED(StarfieldFallOrbitPoints[i][2] / 2));
				btProjectionPoint(vector, &spot);
				spot.x = FIXEDtoINT(spot.x) + 128;
				spot.y = FIXEDtoINT(spot.y) + 60;
				draw(Ram_MapGridMetatileBuffer,
					state->sheet + STARFIELD_FALL_SMALL_GLOW,
					spot.x - STARFIELD_FALL_SMALL_GLOW_SIZE / 2,
					spot.y - STARFIELD_FALL_SMALL_GLOW_SIZE / 2,
					STARFIELD_FALL_SMALL_GLOW_SIZE,
					STARFIELD_FALL_SMALL_GLOW_SIZE);
			}
			if(direction == 1)
				x = frm / 4 - 16;
			else
				x = 16 - frm / 4;
			/* Seven fireballs fall; one that reaches the ground becomes a
			   landing. */
			y = frm - 96;
			for(i = 0; i != 7; i++) {
				Sint32 fireballY = StarfieldFallFireballPoints[i][1] + y;

				if(fireballY <= 93)
					draw(Ram_MapGridMetatileBuffer,
						state->sheet + STARFIELD_FALL_LARGE_GLOW,
						StarfieldFallFireballPoints[i][0] + x -
							STARFIELD_FALL_LARGE_GLOW_SIZE / 2,
						fireballY - STARFIELD_FALL_LARGE_GLOW_SIZE / 2,
						STARFIELD_FALL_LARGE_GLOW_SIZE,
						STARFIELD_FALL_LARGE_GLOW_SIZE);
				else if(fireballY <= 95)
					syStarfieldFallSpawnLanding(
						toFIXED(StarfieldFallFireballPoints[i][0] + x),
						toFIXED(fireballY), 1);
			}
			for(i = 0; i != STARFIELD_FALL_FLAME_COUNT; i++) {
				PARTICLE *flame = &state->particles[i];

				/* A flame rises against the stream through its six pictures. */
				if(flame->variant >= 0) {
					draw(Ram_MapGridMetatileBuffer,
						state->sheet + STARFIELD_FALL_FLAMES +
							flame->variant *
								(STARFIELD_FALL_FLAME_SIZE *
									STARFIELD_FALL_FLAME_SIZE),
						FIXEDtoINT(flame->x) - STARFIELD_FALL_FLAME_SIZE / 2,
						FIXEDtoINT(flame->y) - STARFIELD_FALL_FLAME_SIZE / 2,
						STARFIELD_FALL_FLAME_SIZE, STARFIELD_FALL_FLAME_SIZE);
					flame->x -= direction * (toFIXED(5) / 4);
					flame->y -= toFIXED(5);
					flame->variant++;
					if(flame->variant == 6)
						flame->variant = -1;
				}
			}
			EFFECT_PRESENT_CANVAS(state);
			frm++;
			state->frm++;
		} while(frm != STARFIELD_FALL_FALL_TICKS);
		sySynch(1);
		view->busy = 0;
		syCloseEvent(btEffectAdvanceBg2OnInterval);
		syCloseEvent(btEffectArmPaletteHBlankDMA);
		syCloseEvent(btEffectTransferFullScreenImage);
		BgScrollWork[1].x = savedX;
		BgScrollWork[1].y = savedY;
	}
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	/* The burst: embers, rising motes and growing flashes from the impact. */
	btEffectSetupBlendedDisp();
	REG_BG2PA = Q_8_8(0.5);
	REG_BG2X = 0;
	REG_BG2Y = -(BATTLE_VIEW_TOP << 8);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
		BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	draw = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
	btResourceLoadAndDecompress(RESOURCE_BLAST_SHEET, state->sheet, 1, 0);
	for(i = 0; i != STARFIELD_FALL_FLAME_COUNT; i++) {
		PARTICLE *mote = &state->particles[i];

		mote->x = syRandom() & 127;
		mote->y = (syRandom() & 127) + 127;
	}
	for(i = 0; i != STARFIELD_FALL_FLASHES; i++) {
		PARTICLE *flash = &PARTICLES[i];

		SET_POSITION(flash, 0, 0, 0);
		SET_VELOCITY(flash, ((syRandom() & 255) - 127) << 12,
			(syRandom() & 255) << 11, ((syRandom() & 255) - 127) << 12);
		flash->variant = 0;
	}
	for(i = 0; i != STARFIELD_FALL_EMBER_COUNT; i++) {
		PARTICLE *ember = &STARFIELD_FALL_EMBERS[i];

		SET_POSITION(ember, 0, 0, 0);
		SET_VELOCITY(ember, ((syRandom() & 255) - 128) << 13,
			(syRandom() & 255) << 11, ((syRandom() & 255) - 128) << 13);
		ember->variant = 0;
	}
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(16));

	for(frm = 0; frm != STARFIELD_FALL_BURST_TICKS; frm++) {
		FIXED		   dragonScale = (frm + 464) << 8;
		BATTLE_CAMERA *camera = BattleCameraWork;
		Sint32		   elapsed = frm - STARFIELD_FALL_FLASH_TICK;
		SPR_SCALE	   scale;
		Sint32		   x;
		Sint32		   y;
		FIXED		   meteorScale;

		if(elapsed >= STARFIELD_FALL_BRIGHTEN_DELAY)
			btPaletteBrightenBgEntries(2, 2, 2);
		if(frm == 0)
			sySndPlayStub(SE_SWITCH_MECHANISM);
		if(frm == 40)
			sySndPlayStub(SE_CRASH);
		if(frm == 48) {
			if(mode == STARFIELD_FALL_OUTER_SPACE) {
				fdSprDestroyStub(state->sprs[0]);
				fdSprDestroyStub(state->sprs[1]);
				btObjectPutStub();
			}
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		}
		EFFECT_VIEW_FROM_CAMERA(camera);
		for(i = 0; i != STARFIELD_FALL_EMBERS_DRAWN; i++) {
			/* Embers fall and bounce back up from the ground. */
			PARTICLE *ember = &STARFIELD_FALL_EMBERS[i];

			if(ember->y >= 0) {
				Sint32 size;

				btProjectionPoint(&ember->x, &screen);
				screen.x >>= 1;
				if(screen.depth < MOTE_DEPTH_NEAR)
					screen.depth = MOTE_DEPTH_NEAR;
				if(screen.depth > MOTE_DEPTH_FAR)
					screen.depth = MOTE_DEPTH_FAR;
				/* UB: from depth 736 the size is 0, a picture 0 pixels wide,
				   and its picture is read from BattleEffectMotePictures[-1],
				   the last halfword of the blitter template, which ends at the
				   table (btEffectBuildBlitter). No UBFIX form yet. */
				size = STARFIELD_FALL_EMBER_SIZE_MAX -
					(screen.depth - MOTE_DEPTH_NEAR) / MOTE_DEPTH_STEP;
				draw(canvas,
					state->sheet +
						(BattleEffectMotePictures[size - 1] +
							(i & 1) * MOTE_SET_BYTES) +
						STARFIELD_FALL_MOTE_PICTURES,
					screen.x - size / 2, screen.y - size, size, size * 2);
				btParticleAdvanceWithGravity3D(
					ember, PARTICLE_DAMPING_NONE, -toFIXED(1) / 8);
				if(ember->y <= toFIXED(20)) {
					ember->x = 0;
					ember->z = 0;
					ember->y = toFIXED(20);
					SET_VELOCITY(ember, ((syRandom() & 63) - 32) << 15,
						(syRandom() & 63) << 13,
						((syRandom() & 63) - 32) << 15);
				}
			}
		}
		for(i = 0; i != STARFIELD_FALL_MOTES_DRAWN; i++) {
			PARTICLE *mote = &state->particles[i];
			/* Motes rise up the screen and wrap round. */
			Uint32 rise = i & 7;
			Sint32 size = rise + 3;

			draw(canvas,
				state->sheet +
					(BattleEffectMotePictures[size - 1] +
						(i & 1) * MOTE_SET_BYTES) +
					STARFIELD_FALL_MOTE_PICTURES,
				mote->x - size / 2, mote->y - size, size, size * 2);
			mote->y = mote->y - rise - 8;
			if(mote->y < -10)
				mote->y = 128;
		}
		for(i = 0; i != STARFIELD_FALL_FLASHES_DRAWN; i++) {
			/* Flashes grow and rise, three more each tick. */
			PARTICLE *flash = &PARTICLES[i];

			if(i / 3 < elapsed && flash->y >= 0) {
				btProjectionPoint(&flash->x, &screen);
				screen.x >>= 1;
				if(flash->variant >= 0 &&
					flash->variant < 2 * STARFIELD_FALL_FLASH_PICTURES) {
					Sint32 picture = flash->variant / 2;

					draw(canvas,
						state->sheet + StarfieldFallFlashPictures[picture],
						screen.x - StarfieldFallFlashSizes[picture] / 2,
						screen.y - StarfieldFallFlashSizes[picture] / 2,
						StarfieldFallFlashSizes[picture],
						StarfieldFallFlashSizes[picture]);
				}
				flash->variant++;
				if(flash->variant == 2 * STARFIELD_FALL_FLASH_PICTURES) {
					flash->y = toFIXED(20);
					flash->x = 0;
					flash->z = toFIXED((syRandom() & 255) - 127);
					SET_VELOCITY(flash, 0, (syRandom() & 255) << 11, 0);
					flash->variant = 0;
				} else {
					btParticleAdvanceWithGravity3D(
						flash, PARTICLE_DAMPING_NONE, 1);
				}
			}
		}
		if(direction == 1)
			x = frm / 2 + 24;
		else
			x = 56 - frm / 2;
		y = 64 - frm * 2;
		meteorScale = (frm << 8) + toFIXED(2);
		pos.groundHeight = 0;
		pos.height = toFIXED(255);
		if(mode == STARFIELD_FALL_OUTER_SPACE) {
			scale.x = dragonScale;
			scale.y = dragonScale;
			pos.x = toFIXED(x) + toFIXED(96);
			pos.y = toFIXED(96 - y);
			fdRenderApplyProjectedPlacementStub(
				state->sprs[0], &pos, &scale, 0);
			fdRenderApplyProjectedPlacementStub(
				state->sprs[1], &pos, &scale, 0);
		} else {
			scale.x = meteorScale;
			scale.y = meteorScale;
			pos.x = toFIXED(x) + toFIXED(96);
			pos.y = toFIXED(96 - y);
			fdRenderApplyProjectedPlacementStub(
				state->sprs[0], &pos, &scale, 0);
		}
		state->shakeFrm = 1;
		btCameraShake(8, 8);
		EFFECT_PRESENT_CANVAS(state);
	}
	syCloseEvent(btEffectTransferCanvas);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	if(mode == STARFIELD_FALL_METEOR)
		fdSprDestroyStub(state->sprs[0]);
	btEffectEndCanvasLayer();
}

#undef STARFIELD_FALL_EMBERS
#undef STARFIELD_FALL_FALL_TICKS
#undef STARFIELD_FALL_FLASHES_DRAWN
#undef STARFIELD_FALL_EMBERS_DRAWN
#undef STARFIELD_FALL_FLAME_COUNT
#undef STARFIELD_FALL_LANDINGS
#undef STARFIELD_FALL_MOTES_DRAWN
#undef STARFIELD_FALL_STARS
#undef STARFIELD_FALL_RAGGED_EDGE
#undef STARFIELD_FALL_GLOW_BLUE_LEVEL
#undef STARFIELD_FALL_GLOW_GREEN_LEVEL
#undef STARFIELD_FALL_FLASH_TICK
#undef STARFIELD_FALL_BRIGHTEN_DELAY
#undef STARFIELD_FALL_FLASH_PICTURES
#undef STARFIELD_FALL_MOTE_PICTURES
#undef STARFIELD_FALL_EMBER_SIZE_MAX
#undef STARFIELD_FALL_SCROLL_PRIORITY
#undef STARFIELD_FALL_BACKDROP_PRIORITY
