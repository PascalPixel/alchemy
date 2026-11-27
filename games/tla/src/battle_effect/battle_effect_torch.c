/* Torch and Kite. Part of battle_effect.c. */

#define TORCH_TICKS		  80
#define TORCH_FADE_TICKS  8	 /* the disc fades in, and later out, over these */
#define TORCH_STRIKE_TICK 48 /* the target is struck */

/* The ring level drawn brightest: the levels either side of it dim toward 0 and
   toward the disc's edge. */
#define TORCH_BRIGHTEST_LEVEL 32
/* The disc's colours are worked in eight-bit channels, each capped, then cut to
   the five bits a colour holds; blue's lower cap cuts to the same level. */
#define TORCH_CHANNEL_MAX	  255
#define TORCH_BLUE_MAX		  250
/* How far a line sways either way: 32 canvas pixels, in a sway's quarter
   pixels. */
#define TORCH_SWAY			  (32 * 4)

/* A fiery disc fades in over the battle, its rings running outward while every
   line of the display ripples; the target is struck, the disc fades out.

   Abilities: Torch. */
void btEffectRunTorch(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Sint32				 screen[3];
	Sint32				 world[3];
	DRAW_RECTANGLE		 draw[2];
	Sint32				 x;
	Sint32				 y;
	Sint32				 dx;
	Sint32				 dy;
	Sint32				 centerY;
	Sint32				 distance;
	Sint32				 i;
	Sint32				 shade;
	Sint32				 red;
	Sint32				 green;
	Sint32				 blue;
	Sint32				 frm;
	Sint32				 wave;
	Sint32				 level;
	Uint16				*ramp;
	Sint32				 ang;
	Sint32				*line;

	state = RamHeapSlots->battleEff;
	canvas = RamHeapSlots->battleCanvas;
	btEffectStartCanvasLayer(BGCNT_WRAP);
	btEffectRunDjinniEntrance(args,
		DJINNI_ENTRANCE(ELEMENT_MARS + DJINNI_ENTRANCE_LONG, args->side,
			DJINNI_ANCHOR_OVER_USER),
		screen, world);
	REG_BG2CNT = BGCNT_WRAP | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS |
		BGCNT_CHAR_BASE(1);
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(0);
	REG_BG2PA = Q_8_8(1.0 / 1.5);
	btEffectBuildCanvasBlitters(args->side, draw);
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);

	/* The disc: each pixel's level is half its distance from the middle, its
	   rows drawn a little taller; one quarter is worked out and mirrored into
	   the other three. */
	for(y = 0; y != CANVAS_SIZE / 2; y++) {
		for(x = 0; x != CANVAS_SIZE / 2; x++) {
			centerY = y / 8 + CANVAS_SIZE / 2;
			dy = y - centerY;
			dx = x - CANVAS_SIZE / 2;
			distance = sySquartIWRAM(dx * dx + dy * dy);
			distance /= 2;
			if(distance == 0)
				distance = 1;
			if(distance > CANVAS_LEVEL_BRIGHTEST)
				distance = CANVAS_LEVEL_BRIGHTEST;
			state->sheet[y * CANVAS_SIZE + x] = distance;
			state->sheet[y * CANVAS_SIZE + CANVAS_SIZE - 1 - x] = distance;
			state->sheet[(CANVAS_SIZE - 1 - y) * CANVAS_SIZE + x] = distance;
			state->sheet[(CANVAS_SIZE - 1 - y) * CANVAS_SIZE + CANVAS_SIZE - 1 -
				x] = distance;
		}
	}

	/* Its colours: red rises from the first level, green from the sixth and
	   blue from the eighth, so the rings run from dark red to pale orange at
	   TORCH_BRIGHTEST_LEVEL. */
	ramp = (Uint16 *)RamMapMetatileBuffer;
	for(i = 1; i != CANVAS_LEVEL_BRIGHTEST + 1; i++) {
		if(i >= TORCH_BRIGHTEST_LEVEL)
			shade = 2 * TORCH_BRIGHTEST_LEVEL - i;
		else
			shade = i;
		red = shade * 9;
		green = (shade - 6) * 7;
		blue = (shade - 8) * 7;
		if(red < 0)
			red = 0;
		if(green < 0)
			green = 0;
		if(blue < 0)
			blue = 0;
		if(red > TORCH_CHANNEL_MAX)
			red = TORCH_CHANNEL_MAX;
		if(green > TORCH_CHANNEL_MAX)
			green = TORCH_CHANNEL_MAX;
		if(blue > TORCH_BLUE_MAX)
			blue = TORCH_BLUE_MAX;
		red >>= 3;
		green >>= 3;
		blue >>= 3;
		ramp[i] = ((Uint16 *)BG_PLTT)[i] = RGB(red, green, blue);
	}

	draw[0](canvas, state->sheet, 0, 0, CANVAS_SIZE, CANVAS_SIZE);
	state->transferPending = 1;
	sySetEvent(btEffectArmBg2AffineHBlankDMA, EVENT_PASS_VBLANK);

	/* Fade the disc in, ripple the display, then fade it out. */
	for(frm = 0; frm != TORCH_TICKS; frm++) {
		if(frm <= TORCH_FADE_TICKS) {
			level = frm * 2;
			wave = level;
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(level);
		} else {
			wave = frm * 2;
		}
		if(frm > TORCH_TICKS - TORCH_FADE_TICKS)
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(TORCH_TICKS * 2 - frm * 2);

		if(frm == TORCH_STRIKE_TICK) {
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
			state->shakeFrm = 8;
			btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_NUDGE);
		}

		/* Each line starts one canvas pixel further along than the one above,
		   swayed either way by up to TORCH_SWAY. */
		line = state->bg2xLines;
		for(i = 0, ang = -(wave << 9); i != DISPLAY_HEIGHT; i++) {
			*line++ = CANVAS_WAVE_BG2X(
				i * toFIXED(4) - sySin(ang) * TORCH_SWAY + toFIXED(4));
			ang += TRIG_TURN / 128;
		}

		/* Rotate the colours so the rings run outward. */
		/* DEAD: never true, as tick stays below TORCH_TICKS; the ROM keeps
		   the compare and the store. */
		if(frm > 127) {
			state->transferPending = 1;
		} else {
			ramp[1] = ramp[CANVAS_LEVEL_BRIGHTEST];
			syDMAXCopy(ramp + CANVAS_LEVEL_BRIGHTEST - 1,
				ramp + CANVAS_LEVEL_BRIGHTEST,
				DMA_COPY_HALFWORDS_DOWN(CANVAS_LEVEL_BRIGHTEST - 1), REG_DMA3);
			IO_WRITE_QUEUE_PUSH((Uint32)&((Uint16 *)BG_PLTT)[1],
				(Uint32)(ramp + 1), DMA_COPY_HALFWORDS(CANVAS_LEVEL_BRIGHTEST));
		}
		btCameraShake(8, 8);
		btEffectCountDownBattlerTints();
		sySynch(1);
	}

	syCloseEvent(btEffectArmBg2AffineHBlankDMA);
	EFFECT_FINISH_CANVAS();
}

/* The long Jupiter Djinni entrance over the first target is the whole effect.

   Abilities: Kite. */
void btEffectRunKite(BATTLE_EFFECT_PARM *args)
{
	Sint32 screen[3];
	Sint32 world[3];

	btEffectStartCanvasLayer(0);
	btEffectRunDjinniEntrance(args,
		DJINNI_ENTRANCE(ELEMENT_JUPITER + DJINNI_ENTRANCE_LONG, args->side ^ 1,
			DJINNI_ANCHOR_OVER_TARGET),
		screen, world);
	btEffectEndCanvasLayer();
}

#undef TORCH_TICKS
#undef TORCH_FADE_TICKS
#undef TORCH_STRIKE_TICK
#undef TORCH_BRIGHTEST_LEVEL
#undef TORCH_CHANNEL_MAX
#undef TORCH_BLUE_MAX
#undef TORCH_SWAY
