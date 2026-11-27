/* @file */
/* Torch and Kite. */
/* Part of battle_effect.c. */

#define TORCH_FRAMES	  96
#define TORCH_FADE_FRAMES 8 /* the rings fade in, and later out, over these */

/* The ring level drawn brightest: the levels either side of it dim toward 0 */
/* and toward the disc's edge. */
#define TORCH_BRIGHTEST_LEVEL 32
/* The rings' colours are worked in eight-bit channels, each capped, then cut */
/* to the five bits a colour holds; blue's lower cap cuts to the same level. */
#define TORCH_CHANNEL_MAX 255
#define TORCH_BLUE_MAX	  250
/* How far a line sways either way: 32 canvas pixels, toFIXED(4) to a pixel. */
#define TORCH_SWAY (32 * 4)

/* After the Djinni's entrance, rings of fiery light fill the screen, running */
/* outward from the middle while every line sways, so that they swirl like */
/* heat; then they fade. */
/* */
/* Abilities: Torch. */
void btEffectRunTorch(BATTLE_EFFECT_PARM *args) {
	BATTLE_EFFECT_STATE *state;
	void *canvas;
	s32 screenY;
	s32 screenX;
	DRAW_RECTANGLE draw[2];
	s32 x;
	s32 y;
	s32 dx;
	s32 dy;
	s32 centerY;
	s32 distance;
	s32 i;
	s32 shade;
	s32 red;
	s32 green;
	s32 blue;
	s32 frame;
	s32 swayPhase;
	s32 level;
	u16 *ramp;
	s32 angle;
	s32 *line;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	canvas = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_CANVAS);
	state->args = args;
	btEffectStartCanvasLayer(BGCNT_WRAP);
	btEffectRunDjinniEntrance(args, ELEMENT_MARS + DJINNI_ENTRANCE_LONG,
							  state->args->side, DJINNI_ANCHOR_OVER_USER,
							  &screenX, &screenY);
	REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
				 BGCNT_SCREEN_BASE(7) | BGCNT_WRAP;
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(0);
	REG_BG2PA = Q_8_8(1.0 / 1.5);
	btEffectBuildCanvasBlitters(state->args->side, draw);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_FADE,
						  CANVAS_FADE_THREE_QUARTERS);

	/* The rings: each pixel's level is half its distance from the middle, its
	 */
	/* rows drawn a little taller; one quarter is worked out and mirrored into
	 */
	/* the other three. */
	for(y = 0; y != CANVAS_SIZE / 2; y++) {
		for(x = 0; x != CANVAS_SIZE / 2; x++) {
			centerY = y / 8 + CANVAS_SIZE / 2;
			dy = y - centerY;
			dx = x - CANVAS_SIZE / 2;
			distance = sySquart(dx * dx + dy * dy);
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
	 * blue */
	/* from the eighth, so the rings run from dark red to pale orange at */
	/* TORCH_BRIGHTEST_LEVEL. */
	ramp = (u16 *)Ram_MapGridCellBuffer;
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
		ramp[i] = ((u16 *)BG_PLTT)[i] = RGB(red, green, blue);
	}

	draw[0](canvas, state->sheet, 0, 0, CANVAS_SIZE, CANVAS_SIZE);
	state->transferPending = 1;
	sySetEvent(btEffectArmBg2AffineHBlankDma, TASK_PASS_VBLANK);

	/* Fade the rings in, sway the lines, then fade them out. */
	for(frame = 0; frame != TORCH_FRAMES; frame++) {
		if(frame <= TORCH_FADE_FRAMES) {
			level = frame * 2;
			swayPhase = level;
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(level);
		}
		else {
			swayPhase = frame * 2;
		}
		if(frame > TORCH_FRAMES - TORCH_FADE_FRAMES)
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(TORCH_FRAMES * 2 - frame * 2);

		/* Each line starts one canvas pixel further along than the one above,
		 */
		/* swayed either way by up to TORCH_SWAY. */
		line = state->bg2xLines;
		for(i = 0, angle = -(swayPhase << 9); i != DISPLAY_HEIGHT; i++) {
			*line++ = CANVAS_WAVE_BG2X(i * toFIXED(4) -
									   sySin(angle) * TORCH_SWAY + toFIXED(4));
			angle += TRIG_TURN / 128;
		}

		/* Rotate the colours so the rings run outward. */
		/* DEAD: never true, as frame stays below TORCH_FRAMES; the ROM keeps
		 * the */
		/* compare and the store. */
		if(frame > 127) {
			state->transferPending = 1;
		}
		else {
			ramp[1] = ramp[CANVAS_LEVEL_BRIGHTEST];
			syDMAXCopy(ramp + CANVAS_LEVEL_BRIGHTEST - 1,
					   ramp + CANVAS_LEVEL_BRIGHTEST,
					   DMA_COPY_HALFWORDS_DOWN(CANVAS_LEVEL_BRIGHTEST - 1),
					   REG_DMA3);
			IO_WRITE_QUEUE_PUSH((u32)&BG_PLTT[1], (u32)(ramp + 1),
								DMA_COPY_HALFWORDS(CANVAS_LEVEL_BRIGHTEST));
		}
		sySynch(1);
	}

	syCloseEvent(btEffectArmBg2AffineHBlankDma);
	EFFECT_FINISH_CANVAS();
}

/* The long Jupiter Djinni entrance over the first target is the whole effect.
 */
/* */
/* Abilities: Kite. */
void btEffectRunKite(BATTLE_EFFECT_PARM *args) {
	BATTLE_EFFECT_STATE *state;
	s32 originX;
	s32 originY;

	state = HEAP_SLOT_BLOCK(HEAP_SLOT_BATTLE_EFFECT);
	state->args = args;
	btEffectStartCanvasLayer(0);
	btEffectRunDjinniEntrance(args, ELEMENT_JUPITER + DJINNI_ENTRANCE_LONG,
							  state->args->side ^ 1, DJINNI_ANCHOR_OVER_TARGET,
							  &originX, &originY);
	btEffectEndCanvasLayer();
}

#undef TORCH_FRAMES
#undef TORCH_FADE_FRAMES
#undef TORCH_BRIGHTEST_LEVEL
#undef TORCH_CHANNEL_MAX
#undef TORCH_BLUE_MAX
#undef TORCH_SWAY
