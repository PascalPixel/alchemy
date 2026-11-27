/* Cruel Ruin, the shattering opening and the element burst, and the
   light-streak backdrop with its halved canvas and dimming background. Part of
   battle_effect.c. */

/* Puts colour 2 at (x, y) in the 256x120 8bpp tile sheet (32 tiles wide) in the
   map cell buffer. */
#define PLOT(x, y) \
	(*(Ram_MapMetatileBuffer + ((x) & 7) + (x) / 8 * 64 + ((y) & 7) * 8 + \
		 (y) / 8 * 2048) = 2)

#define SHATTERING_OPENING_TICKS 128

/* flashes thrown as the screen breaks */
#define SHATTERING_OPENING_FLASHES	   32
/* How long a flash floats up, its six pictures four ticks each and the last
   held. */
#define SHATTERING_OPENING_FLASH_TICKS 40
#define SHATTERING_OPENING_PIECES	   33 /* pieces the screen breaks into */
#define SHATTERING_OPENING_DISC_X	   96 /* the centre of the opening disc */
#define SHATTERING_OPENING_DISC_Y	   60 /* the centre of the opening disc */

/* As the battle with the Doom Dragon opens over the Serpent's stars, the screen
   turns white and breaks into pieces that fly apart and fall, while a grey disc
   grows over it and the battle background's colours return. */
void btEffectRunShatteringOpening(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE		*state;
	void					*canvas;
	Sint32					 frm;
	BLITTER_PAIR			 draw;
	BATTLE_STATE			*battle;
	Sint32					 j;
	Sint32					 k;
	Sint32					 rmax;
	Sint32					 r;
	Sint32					 d;
	BATTLE_BACKGROUND_STATE *view;
	PARTICLE				*piece;
	Sint32					 i;
	Sint32					 px;
	Sint32					 py;
	Sint32					 x1;
	Sint32					 left;
	Sint32					 spanLeft;
	Sint32					 y1;
	Sint32					 x2;
	Sint32					 y2;
	Sint32					 cx;
	Sint32					 cy;

	state = syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_STATE_SIZE);
	canvas = syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
	battle = Ram_HeapSlots->battleState;
	view = Ram_HeapSlots->battleBg;
	state->args = args;
	state->framesSinceTransfer = 1;

	/* Make BG2 a 128x128 canvas, as btEffectStartCanvasLayer does. */
	btEffectClearTintTimers();
	view->bg1ScrollMode = BATTLE_BACKGROUND_FIXED;
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	btBackgroundSetModeStub(BATTLE_BACKGROUND_FIXED, battle->bg, 0);
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
	btCameraSetViewStub(0, 0, 0, BATTLE_ZOOM_NORMAL);
	view->bg1ScrollMode = BATTLE_BACKGROUND_NONE;
	REG_BG2X = 0;
	REG_BG2Y = -(BATTLE_VIEW_TOP << 8);
	REG_BG2PA = Q_8_8(0.5);
	REG_BG2PB = 0;
	REG_BG2PC = 0;
	REG_BG2PD = Q_8_8(1);
	EFFECT_SET_BATTLE_WINDOWS();
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
		WININ_WIN1_OBJ | WININ_WIN1_CLR;
	REG_WINOUT = EFFECT_WINOUT;

	i = 0;
	for(k = 0; k != CANVAS_TILES; k++) {
		for(j = 0; j != 8; j++, i += 2) {
			Sint16 entry;

			entry = ((k * CANVAS_TILES + j * 2 + 1) << 8) | ((k * 8 + j) * 2);
			*(volatile Sint16 *)(BG_SCREEN_BLOCK(7) + i) = entry;
		}
	}

	syWordsClear(canvas, CANVAS_BYTES);
	syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
	IO_WRITE_QUEUE_PUSH(
		(Uint32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	REG_BLDCNT = 0;
	btResourceLoadAndDecompress(
		RESOURCE_SHATTERING_OPENING_SHEET, state->sheet, 1, 1);
	/* each transfer clears the canvas */
	state->transferMode = CANVAS_COPY_AND_CLEAR;
	state->transferValue = 0;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_TRANSPARENT);
	draw[0] = Ram_HeapSlots->ramCode;
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	draw[1] = Ram_HeapSlots->ramCodePaired;

	/* The pieces fly out from (32, 60) at a quarter of their distance a
	   tick. */
	r = 0;
	for(i = 0; i != SHATTERING_OPENING_PIECES; i++) {
		FIXED  x, y;
		Sint32 vx, vy;

		piece = &state->particles[i];
		x = toFIXED(ShatteringOpeningPieceStartX[i]);
		y = toFIXED(ShatteringOpeningPieceStartY[i]);
		piece->x = x;
		piece->y = y;
		vx = (x - toFIXED(32)) >> 2;
		vy = (y - toFIXED(60)) >> 2;
		piece->velX = vx;
		piece->velY = vy;
	}

	/* Copy the battle background's image aside, fill it with colour 1, and make
	   its colours white. */
	syWordsCopy(Ram_MapMetatileBuffer, (void *)BG_CHAR_BLOCK(2),
		BATTLE_BACKGROUND_BYTES);
	syWordsFill(
		Ram_MapMetatileBuffer, BATTLE_BACKGROUND_BYTES, CANVAS_FILL_WORD(1));
	view->busy = 1;
	state->savedBg1X = BgScrollWork[1].x;
	state->savedBg1Y = BgScrollWork[1].y;
	BgScrollWork[1].x = 0;
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BG1CNT,
		BGCNT_PRIORITY(1) | BGCNT_256_COLORS |
			BGCNT_SCREEN_BASE(BATTLE_BG1_SCREEN_BLOCK),
		IO_WRITE_HALFWORD);
	syWordsFill((void *)&BG_PLTT_COLOR(6, 0), BATTLE_BACKGROUND_COLORS * 2,
		COLOR_PAIR(RGB_WHITE));
	sySndPlayStub(SE_POWER_UP);
	btEffectTintBattler(args->tgts[0], SPRITE_COLOR_WHITE, POSE_CAST, 0, 30);

	for(frm = 0; frm != SHATTERING_OPENING_TICKS; frm++) {
		if(frm == 2)
			sySndPlayStub(SE_POWER_UP);
		if(frm == 3)
			sySndPlayStub(SE_POWER_UP);
		if(frm == 28)
			btEffectTintBattler(
				args->tgts[0], TINT_KEEP, POSE_CAST, TINT_KEEP, 0);
		if(frm == 32)
			sySndPlayStub(SE_MONSTER_ROAR);
		if(frm == 5)
			sySndPlayStub(SE_CRASH);
		if(frm == 5) {
			BgScrollWork[1].x = state->savedBg1X;
			btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, battle->bg, -1);
		}

		/* From tick 8, step each colour of BG palettes 6 to 13 one shade
		   towards the battle background's. */
		if(frm > 7) {
			Uint16 *pal;

			pal = (Uint16 *)&BG_PLTT_COLOR(6, 0);
			for(i = 0; i != BATTLE_BACKGROUND_COLORS; i++) {
				Sint32 red = RGB_RED(*pal);
				Sint32 green = RGB_GREEN(*pal);
				Sint32 blue = RGB_BLUE(*pal);
				Sint32 goalRed = RGB_RED(battle->pal[i]);
				Sint32 goalGreen = RGB_GREEN(battle->pal[i]);
				Sint32 goalBlue = RGB_BLUE(battle->pal[i]);

				if(red < goalRed)
					red++;
				else if(red > goalRed)
					red--;
				if(green < goalGreen)
					green++;
				else if(green > goalGreen)
					green--;
				if(blue < goalBlue)
					blue++;
				else if(blue > goalBlue)
					blue--;
				*pal++ = RGB(red, green, blue);
			}
		}

		if(frm == 4)
			syWordsFill((void *)BG_CHAR_BLOCK(2), BATTLE_BACKGROUND_BYTES,
				CANVAS_FILL_WORD(2));

		/* Ticks 0 to 3: widen the circle by 32 pixels of radius, brightening
		   its grey. */
		if(frm <= 3) {
			Sint32 v;

			v = frm * 4 + 8;
			rmax = frm * 32;
			BG_PLTT_COLOR(0, 2) = RGB(v, v, v);
			cx = SHATTERING_OPENING_DISC_X;
			cy = SHATTERING_OPENING_DISC_Y;
			for(; r != rmax; r++) {
				/* The midpoint circle: plot each eighth by symmetry. */
				px = r;
				py = 0;
				d = r;
				for(; px >= py; py++) {
					/* BUG: j and k are the tile map's finished counters, 8 and
					   16, not the circle's centre. */
					x2 = px + j;
					x1 = j - px;
					y2 = py + k;
					y1 = k - py;
					if(y1 < 0)
						y1 = 0;
					if(y2 > 119)
						y2 = 119;
					if(x1 < 0)
						x1 = 0;
					if(x2 > 255)
						x2 = 255;
					PLOT(x2, y2);
					PLOT(x2, y1);
					PLOT(x1, y2);
					PLOT(x1, y1);
					left = cx - px;
					x1 = left + 1;
					x2 = px + (SHATTERING_OPENING_DISC_X + 1);
					if(x1 < 0)
						x1 = 0;
					if(x2 > 255)
						x2 = 255;
					PLOT(x2, y2);
					PLOT(x2, y1);
					PLOT(x1, y2);
					PLOT(x1, y1);

					x2 = py + cx;
					x1 = cx - py;
					spanLeft = x1;
					y2 = px + cy;
					y1 = cy - px;
					if(x1 < 0)
						x1 = 0;
					if(x2 > 255)
						x2 = 255;
					if(y1 < 0)
						y1 = 0;
					if(y2 > 119)
						y2 = 119;
					PLOT(x2, y2);
					PLOT(x2, y1);
					PLOT(x1, y2);
					PLOT(x1, y1);
					x1 = spanLeft + 1;
					x2 = py + (SHATTERING_OPENING_DISC_X + 1);
					if(x1 < 0)
						x1 = 0;
					if(x2 > 255)
						x2 = 255;
					PLOT(x2, y2);
					PLOT(x2, y1);
					PLOT(x1, y2);
					PLOT(x1, y1);

					d -= py * 2 + 1;
					if(d < 0) {
						d += px * 2 - 2;
						px--;
					}
				}
			}
			syWordsCopy((void *)BG_CHAR_BLOCK(2), Ram_MapMetatileBuffer,
				BATTLE_BACKGROUND_BYTES);
		}

		/* Up to tick 50, draw the pieces; from tick 4 they fall. */
		if(frm <= 50) {
			for(i = 0; i != SHATTERING_OPENING_PIECES; i++) {
				piece = &state->particles[i];
				draw[0](canvas,
					state->sheet + ShatteringOpeningPiecePictures[i],
					FIXED_INTEGER_PART(piece->x), FIXED_INTEGER_PART(piece->y),
					ShatteringOpeningPieceWidths[i],
					ShatteringOpeningPieceHeights[i]);
				if(frm > 3)
					btParticleAdvanceWithGravity2D(
						piece, PARTICLE_DAMPING_NONE, toFIXED(1) / 4);
			}
		}

		/* Ticks 8 to 50: colour 1 from black to white. */
		if(frm >= 8 && frm <= 50) {
			Sint32 v;

			v = frm - 8;
			if(v > RGB_LEVEL_MAX)
				v = RGB_LEVEL_MAX;
			BG_PLTT_COLOR(0, 1) = RGB(v, v, v);
		}

		/* Tick 51: the flash pictures and colours, and 32 flashes placed at x
		   32-63, y 80-111. */
		if(frm == 51) {
			btResourceLoadAndDecompress(
				RESOURCE_FLASH_BURST_SHEET, state->sheet, 1, 0);
			for(i = 1; i != SHEET_PALETTE_COLORS; i++) {
				Sint32 v;

				v = i / 2;
				/* DEAD: v is never below 0; the ROM keeps the test. */
				if(v < 0)
					v = 0;
				BG_PLTT[i] = RGB(v, v / 2, v);
			}
			REG_BLDCNT =
				BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
			for(i = 0; i != SHATTERING_OPENING_FLASHES; i++) {
				piece = &state->particles[i];
				piece->x = INTtoFIXED((syRandom() & 31) + 32);
				piece->y = INTtoFIXED((syRandom() & 31) + 80);
				piece->velX = ((syRandom() & 0x1ff) - 256) << 12;
				piece->velY = 0;
				piece->variant = 0;
			}
			/* From here each transfer halves the canvas, leaving a trail. */
			state->transferMode = CANVAS_COPY_AND_FADE;
			state->transferValue = CANVAS_FADE_HALF;
		}

		/* The flashes start i/4 ticks late and rise, a picture every 4
		   ticks. */
		if(frm > 52) {
			for(i = 0; i != SHATTERING_OPENING_FLASHES; i++) {
				piece = &state->particles[i];
				if(frm >= i / 4 + 52 &&
					piece->variant < SHATTERING_OPENING_FLASH_TICKS) {
					Sint32 picture;

					picture = piece->variant / 4;
					if(picture > 5)
						picture = 5;
					draw[1](canvas,
						state->sheet + picture * PICTURE_BYTES(FLASH_BURST),
						FIXED_INTEGER_PART(piece->x) - FLASH_BURST_WIDTH / 2,
						FIXED_INTEGER_PART(piece->y) - FLASH_BURST_HEIGHT / 2,
						FLASH_BURST_WIDTH, FLASH_BURST_HEIGHT);
					btParticleAdvanceWithGravity2D(piece, 60, -toFIXED(1) / 16);
					piece->variant++;
				}
			}
		}

		EFFECT_END_TICK(state);
	}

	/* Put the background and the display back. */
	EFFECT_RELEASE_BLITTERS();
	syCloseEvent(btEffectTransferCanvas);
	btEffectTintBattler(args->tgts[0], TINT_KEEP, POSE_STANDING, TINT_KEEP, 0);
	BgScrollWork[1].x = state->savedBg1X;
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	btBackgroundSetModeStub(BATTLE_BACKGROUND_ZOOMED, battle->bg, 0);
	sySynch(1);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG2_ON |
			DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON,
		IO_WRITE_HALFWORD);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_SHEET);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

#define ELEMENT_BURST_TICKS 56 /* ticks of the element burst */

/* CyanSparkSheet: five small sparks, then the burst's pictures. */
#define ELEMENT_BURST_SPARK_WIDTH  32
#define ELEMENT_BURST_SPARK_HEIGHT 32
#define ELEMENT_BURST_BURST_WIDTH  48
#define ELEMENT_BURST_BURST_HEIGHT 48
#define ELEMENT_BURST_BURST		   (5 * PICTURE_BYTES(ELEMENT_BURST_SPARK))
/* A burst mote's shortest life in ticks; by its index a mote lives up to twice
   as long, less one. */
#define ELEMENT_BURST_MOTE_LIFE	   16

/* Sparks in an element's colours flare at four corners around a unit and motes
   spiral in, then a burst of light plays over it. The battle's element glow
   runs it, and it sets up its own heap blocks. */
void btEffectRunElementBurst(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT		 ground;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	BLITTER_PAIR		 draw;
	Uint8				*sheet;
	void				*palette;
	Sint32				 frm;
	Sint32				 i;

	state = syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, BATTLE_EFFECT_STATE_SIZE);
	canvas = syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
	sheet = syHeapSlotAlloc(HEAP_SLOT_BATTLE_SHEET, BATTLE_SHEET_BYTES);
	state->framesSinceTransfer = 1;
	state->args = args;
	btEffectStartCanvasLayer(0);
	state->fadeFrm = BACKGROUND_DARKEN_TICKS;
	state->fadeStep = 0;
	REG_BLDALPHA = EFFECT_BLEND_CANVAS(ELEMENT_BLEND_LEVEL);
	REG_BG2PA = Q_8_8(1);
	btResourceLoadAndDecompress(RESOURCE_CYAN_SPARK_SHEET, state->sheet, 1, 0);
	btResourceLoadAndDecompress(RESOURCE_BLUE_GLOW_SHEET, sheet, 0, 0);
	switch(args->kind) {
	case ELEMENT_VENUS:
		palette = syResourceGetTableEntry(RESOURCE_YELLOW_PALETTE_A);
		break;
	case ELEMENT_MERCURY:
		palette = syResourceGetTableEntry(RESOURCE_ICE_TILE_SHEET);
		break;
	case ELEMENT_MARS:
		palette = syResourceGetTableEntry(RESOURCE_RED_PALETTE_A);
		break;
	default:
		palette = syResourceGetTableEntry(RESOURCE_VIOLET_PALETTE_A);
		break;
	}
	syWordsCopy((void *)BG_PLTT, palette, SHEET_PALETTE_BYTES);

	for(i = 0; i != ELEMENT_SPIRAL_MOTES; i++) {
		PARTICLES[i].y = toFIXED(128);
		PARTICLES[i].x = syRandom() & TRIG_ANGLE_MASK;
		PARTICLES[i].z = (syRandom() & 0x1ff) + 1024;
		PARTICLES[i].variant = -i;
	}

	/* A mote's x is its angle and y its distance from the user; its variant
	   counts down its life. */
	for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
		state->particles[i].x = syRandom() & TRIG_ANGLE_MASK;
		state->particles[i].y = (syRandom() & 31) + 16;
		state->particles[i].variant =
			(i & (ELEMENT_BURST_MOTE_LIFE - 1)) + ELEMENT_BURST_MOTE_LIFE;
	}
	EFFECT_START_TRANSFER(
		state, CANVAS_COPY_AND_FADE, CANVAS_FADE_THREE_QUARTERS);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_ADDITIVE);
	draw[0] = Ram_HeapSlots->ramCode;
	sySndPlayStub(SE_SPIN);

	for(frm = 0; frm != ELEMENT_BURST_TICKS; frm++) {
		btEffectPosProjectScaledBattler(args->user, &ground);
		REG_BG2X = (CANVAS_SIZE / 2 - ground.x) << 8;
		if(frm > 49)
			REG_BLDALPHA = EFFECT_BLEND_CANVAS(112 - frm * 2);
		if(frm == 26) {
			sySndPlayStub(SE_POWER_UP);
			btEffectTintBattler(
				args->tgts[0], SPRITE_COLOR_WHITE, TINT_KEEP, 0, 20);
		}

		/* The burst, a picture every three ticks. */
		if(frm >= 28 && frm <= 48)
			draw[0](canvas,
				state->sheet +
					(frm - 28) / 3 * PICTURE_BYTES(ELEMENT_BURST_BURST) +
					ELEMENT_BURST_BURST,
				40, ground.y - 24, ELEMENT_BURST_BURST_WIDTH,
				ELEMENT_BURST_BURST_HEIGHT);

		/* The corner sparks, each with its own flips. */
		if(frm >= 0 && frm <= 14) {
			Sint32 picture = frm / 3 % 5;
			Sint32 mode;

			for(i = 0, mode = BLITTER_MAX; i != 4; i++) {
				EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
					ElementBurstGlintFlips[i] | EFFECT_CLIP_BOUNDS, mode);
				draw[1] = Ram_HeapSlots->ramCodePaired;
				draw[1](canvas,
					state->sheet + picture * PICTURE_BYTES(ELEMENT_BURST_SPARK),
					ElementBurstSparkX[i] + 32,
					ground.y + ElementBurstSparkY[i] - 32,
					ELEMENT_BURST_SPARK_WIDTH, ELEMENT_BURST_SPARK_HEIGHT);
				syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
			}
		}

		/* The motes close in 2 pixels a tick. */
		if(frm >= 0) {
			for(i = 0; i != ARRAY_COUNT(state->particles); i++) {
				PARTICLE *mote = &state->particles[i];

				if(mote->variant >= 0 && mote->y > 0) {
					Sint32 size = (mote->variant >> 3) + 1;
					Sint32 x = (FIXEDtoINT(mote->y * sySin(mote->x))) + 64;
					Sint32 y =
						(FIXEDtoINT(mote->y * syCos(mote->x))) + ground.y;

					/* DEAD: never true, as the variant is not negative here;
					   the ROM keeps the test and the store. */
					if(size <= 0)
						size = 1;
					EFFECT_DRAW_FLARE(draw[0], canvas, sheet, x, y, size);
					mote->y -= 2;
					mote->variant--;
				}
			}
		}
		EFFECT_END_TICK(state);
	}

	syHeapSlotRelease(HEAP_SLOT_RAM_CODE);
	syCloseEvent(btEffectTransferCanvas);
	btEffectEndCanvasLayer();
	syHeapSlotRelease(HEAP_SLOT_BATTLE_SHEET);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* Tick callback: when a transfer is pending, writes the canvas to BG character
   block 1 at half brightness. */
void btEffectTransferHalvedCanvas(void)
{
	ELEMENT_STREAKS_STATE *state = Ram_HeapSlots->battleEff;
	void				  *buf = Ram_HeapSlots->battleCanvas;
	Uint32				   ime;

	IRQ_LOCK(ime);
	if(state->halvePending == 1) {
		state->halvePending = 0;
		IRQ_UNLOCK(ime);
		{
			void *destination = (void *)BG_CHAR_BLOCK(1);

			syColBufferHalve(buf, destination, CANVAS_BYTES);
		}
	} else {
		IRQ_UNLOCK(ime);
	}
}

/* The element streaks seeded at the start, and those drawn. */
#define STREAK_SEEDS		  256
#define STREAKS				  64
/* A streak's head starts 48 to 63 from the centre, its tail STREAK_LENGTH
   behind it. */
#define STREAK_START_DISTANCE 48
#define STREAK_LENGTH		  8

/* Draws a tick of the element streaks, lines of light closing in on the centre
   of the canvas, and has the canvas halved into VRAM. */
void btEffectDrawStreaks(void)
{
	ELEMENT_STREAKS_STATE *state = Ram_HeapSlots->battleEff;
	Sint32				   frm;
	Sint32				   i;
	PLUNGE_STREAK		  *streak;
	STREAK_POINT		   point;
	STREAK_POINT		   head;
	STREAK_POINT		   tail;

	frm = (state->frm)++;
	if(frm == 0) {
		for(i = 0; i != STREAK_SEEDS; i++) {
			PLUNGE_STREAK *seed = &((PLUNGE_STREAK *)Ram_MapMetatileBuffer)[i];
			Sint32		   r = syRandom() & 15;

			seed->head = r + STREAK_START_DISTANCE;
			seed->tail = r + STREAK_START_DISTANCE - STREAK_LENGTH;
			seed->pitch = syRandom();
			seed->yaw = syRandom();
			seed->roll = syRandom();
		}
	}
	point.y = 0;
	point.z = 0;
	for(i = 0; i != STREAKS; i++) {
		streak = &((PLUNGE_STREAK *)Ram_MapMetatileBuffer)[i];
		if(frm > i / 4 && streak->head > 0) {
			Sint32 fade;

			syInitMatrix();
			syRotZ(streak->roll);
			syRotX(streak->pitch);
			syRotY(streak->yaw);
			point.x = streak->head;
			btEffectPosProjectPoint((Sint32 *)&point, &head);
			head.x += 64;
			head.y += 80;
			point.x = streak->tail;
			btEffectPosProjectPoint((Sint32 *)&point, &tail);
			tail.x += 64;
			tail.y += 80;
			streak->tail -= 4;
			streak->head -= 4;
			if(streak->tail < 0)
				streak->tail = 0;
			/* The tail stops at the centre, and the head goes out once it
			   passes it. */
			fade = -streak->tail / 2;
			btEffectDrawClippedCanvasLine(
				tail.x - 1, tail.y, head.x - 1, head.y, fade + 48);
			btEffectDrawClippedCanvasLine(
				tail.x, tail.y - 1, head.x, head.y - 1, fade + 48);
			btEffectDrawClippedCanvasLine(
				tail.x, tail.y, head.x, head.y, fade + 56);
		}
	}
	state->halvePending = 1;
}

/* Each tick while ticks are left, darkens the battle background's 128 colours
   by one more BACKGROUND_LEVEL_STEP into BG palettes 6 to 13. */
void btEffectDimBackgroundStep(void)
{
	ELEMENT_STREAKS_STATE *state = Ram_HeapSlots->battleEff;
	BATTLE_STATE		  *battle = Ram_HeapSlots->battleState;

	if(state->fadeFrm > 0) {
		state->fadeStep++;
		btColBufferScaleStub(battle->pal, (void *)&BG_PLTT_COLOR(6, 0),
			toFIXED(1) - state->fadeStep * BACKGROUND_LEVEL_STEP,
			BATTLE_BACKGROUND_COLORS);
		state->fadeFrm--;
	}
}

/* Starts the element streaks, which then draw by themselves while the battle
   runs on. */
void btEffectStartElementStreaks(Sint32 kind)
{
	ELEMENT_STREAKS_STATE	*state;
	void					*canvas;
	BATTLE_STATE			*battle;
	BATTLE_BACKGROUND_STATE *view;
	Sint32					 id;
	Sint32					 i;
	Sint32					 j;
	Sint32					 k;

	state = syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, sizeof *state);
	canvas = syHeapSlotAlloc(HEAP_SLOT_BATTLE_CANVAS, CANVAS_BYTES);
	battle = Ram_HeapSlots->battleState;
	view = Ram_HeapSlots->battleBg;

	/* Turn BG2 into a 128x128 canvas, one step a tick. */
	btEffectConfigureEffectDisp();
	view->bg1ScrollMode = BATTLE_BACKGROUND_FIXED;
	sySynch(1);
	REG_BLDCNT = 0;
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
			DISPCNT_BG2_ON | DISPCNT_OBJ_ON,
		IO_WRITE_HALFWORD);
	BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
	sySynch(1);
	btBackgroundSetModeStub(BATTLE_BACKGROUND_FIXED, battle->bg, 0);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
			DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON,
		IO_WRITE_HALFWORD);
	sySynch(1);
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_DISPCNT,
		DISPCNT_MODE_1 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
			DISPCNT_OBJ_ON | DISPCNT_WIN0_ON | DISPCNT_WIN1_ON,
		IO_WRITE_HALFWORD);
	btCameraSetViewStub(0, 0, 0, BATTLE_ZOOM_NORMAL);
	view->bg1ScrollMode = BATTLE_BACKGROUND_NONE;
	sySynch(1);
	btEffectSetupCanvasWindows();
	REG_BG2PA = Q_8_8(1);
	REG_WININ = EFFECT_WIN0_LAYERS | WININ_WIN1_BG0 | WININ_WIN1_BG2 |
		WININ_WIN1_OBJ | WININ_WIN1_CLR;
	IO_WRITE_QUEUE_PUSH(
		(Uint32)&REG_DISPCNT, EFFECT_DISPCNT, IO_WRITE_HALFWORD);

	i = 0;
	for(k = 0; k != CANVAS_TILES; k++) {
		for(j = 0; j != 8; j++, i += 2) {
			Sint32 entry;

			entry = (Sint16)(((k * CANVAS_TILES + j * 2 + 1) << 8) |
				((k * 8 + j) * 2));
			*(volatile Uint16 *)(BG_SCREEN_BLOCK(7) + i) = *(Uint16 *)&entry;
		}
	}
	syWordsClear(canvas, CANVAS_BYTES);
	syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
	sySynch(1);

	switch(kind) {
	case ELEMENT_VENUS:
		id = RESOURCE_YELLOW_PALETTE_B;
		break;
	case ELEMENT_MERCURY:
		id = RESOURCE_CRYSTAL_SHEET;
		break;
	case ELEMENT_MARS:
		id = RESOURCE_EMBER_STREAK_SHEET;
		break;
	case ELEMENT_JUPITER:
		id = RESOURCE_VIOLET_PALETTE_D;
		break;
	case ELEMENT_NONE:
	default:
		id = RESOURCE_DUST_PUFF_SHEET;
		break;
	}
	syWordsCopyEntry(
		(void *)BG_PLTT, syResourceGetTableEntry(id), SHEET_PALETTE_BYTES);

	/* Fade the background over 24 ticks, and draw and halve the streaks every
	   tick. */
	state->fadeFrm = BACKGROUND_DARKEN_TICKS;
	state->fadeStep = 0;
	sySetEvent(btEffectDimBackgroundStep, EVENT_PASS_TICK);
	state->halvePending = 0;
	sySetEvent(btEffectTransferHalvedCanvas, EVENT_PASS_VBLANK);
	state->frm = 0;
	sySetEvent(btEffectDrawStreaks, EVENT_PASS_TICK);
}

/* Ends the element streaks: removes their three events, clears the canvas in
   VRAM and frees what they took. */
void btEffectEndElementStreaks(void)
{
	syCloseEvent(btEffectDrawStreaks);
	syCloseEvent(btEffectTransferHalvedCanvas);
	syCloseEvent(btEffectDimBackgroundStep);
	syWordsClear((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_CANVAS);
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* Where Cruel Ruin's pictures are built or decoded in the effect's pictures:
   SwirlTexture; GlowBandTextureA (GlowBandTextureB in the second act) and
   GlowBandTextureB; PinkBarSheet; the ten shaded circles, 2 to 20 wide, at
   CruelRuinCirclePictures; and, in the last act, GlowBandTextureB with the
   shaded circle drawn after it. */
#define CRUEL_RUIN_SWIRL				 9800
#define CRUEL_RUIN_BAND_A				 13896
#define CRUEL_RUIN_BAND_B				 13960
#define CRUEL_RUIN_PINK_BAR				 14024
#define CRUEL_RUIN_CIRCLES				 14920
#define CRUEL_RUIN_BURST_BAND			 14400
#define CRUEL_RUIN_BURST_CIRCLE			 14528
/* CornerRuneASheet's 64x64 rune mirrored into a 128x128 one, in the map cell
   buffer. */
#define CRUEL_RUIN_RUNE_CIRCLE			 16384
/* The ribbons' strip mesh, in the map cell buffer. */
#define CRUEL_RUIN_STRIP_MESH			 (Ram_MapMetatileBuffer + 8192)
/* The particles of the first act: rays of light from the orb, the motes that
   swell below it, and the ball of light that falls, once in the sky and again
   on the foes. */
#define CRUEL_RUIN_RAYS					 24
#define CRUEL_RUIN_FIRST_MOTE			 32
#define CRUEL_RUIN_MOTES				 31
#define CRUEL_RUIN_BALL_PARTICLE		 63
/* The glow that swells at the caster's feet as Cruel Ruin starts grows to this
   size. */
#define CRUEL_RUIN_GLOW_SIZE			 96
/* The shaded circles built for the motes and blasts: CRUEL_RUIN_CIRCLE_SIZES of
   them, 2 to 20 wide. */
#define CRUEL_RUIN_CIRCLE_SIZES			 10
/* The glow in the sky starts this large and shrinks into the orb. Its foot
   starts at y CRUEL_RUIN_SKY_GLOW_Y, and from tick 20 it rises along a quarter
   of a cosine towards CRUEL_RUIN_SKY_GLOW_RISE pixels higher, to hold at y 20
   from tick 36. */
#define CRUEL_RUIN_SKY_GLOW_SIZE		 140
#define CRUEL_RUIN_SKY_GLOW_Y			 58
#define CRUEL_RUIN_SKY_GLOW_RISE		 44
/* Each mote swells in place over CRUEL_RUIN_MOTE_TICKS, then is gone. */
#define CRUEL_RUIN_MOTE_TICKS			 16
/* The rings of fire (FireRingTexture): four fly out from the orb, one every
   CRUEL_RUIN_RING_INTERVAL ticks, each for CRUEL_RUIN_RING_TICKS, and a fifth
   swells as the rays burst, shining for CRUEL_RUIN_RING_SHINE_TICKS before it
   fades 8 levels a tick. */
#define CRUEL_RUIN_RINGS				 4
#define CRUEL_RUIN_RING_INTERVAL		 10
#define CRUEL_RUIN_RING_TICKS			 16
#define CRUEL_RUIN_RING_SHINE_TICKS		 8
/* The circle of runes turns round the orb from tick 24 for
   CRUEL_RUIN_RUNE_TICKS, until the flash. */
#define CRUEL_RUIN_RUNE_TICKS			 100
/* The rays' length grows to CRUEL_RUIN_RAY_LENGTH_MAX; each is
   CRUEL_RUIN_RAY_WIDTH across, a little under half of the beam it is drawn
   from, after the beam is already halved. */
#define CRUEL_RUIN_RAY_LENGTH_MAX		 (toFIXED(625) / 512)
#define CRUEL_RUIN_RAY_WIDTH			 30384
/* The ball of light that falls grows to this size; its streak's width scales by
   CRUEL_RUIN_STREAK_SCALE for each pixel of the ball's size, as the falling
   streaks' do. */
#define CRUEL_RUIN_BALL_SIZE			 64
#define CRUEL_RUIN_STREAK_SCALE			 9400
/* The two ribbons of light (PinkBarSheet): a strip of CRUEL_RUIN_RIBBON_POINTS
   points each, two corners a point, swaying CRUEL_RUIN_RIBBON_SWAY_STEP further
   round a sine at each point, each standing for CRUEL_RUIN_RIBBON_TICKS. The
   first leans by CRUEL_RUIN_FIRST_RIBBON_ROLL (about 17 degrees), the second by
   TRIG_TURN / 64. */
#define CRUEL_RUIN_RIBBON_POINTS		 33
#define CRUEL_RUIN_RIBBON_QUADS			 (CRUEL_RUIN_RIBBON_POINTS - 1)
#define CRUEL_RUIN_RIBBON_POINT_BYTES	 8
#define CRUEL_RUIN_STRIP_QUAD_BYTES		 (2 * sizeof(MESH_TRIANGLE))
#define CRUEL_RUIN_RIBBON_SWAY_STEP		 2348
#define CRUEL_RUIN_RIBBON_TICKS			 46
#define CRUEL_RUIN_FIRST_RIBBON_ROLL	 3048
/* The ribbons are drawn at about 70 percent of their strip's length. */
#define CRUEL_RUIN_RIBBON_STRETCH		 45536
/* The ribbons' texture scrolls back from ~CRUEL_RUIN_RIBBON_SCROLL, 2 texels a
   tick on the first and 4 on the second. */
#define CRUEL_RUIN_RIBBON_SCROLL		 63
/* The forest rushing by below (ForestCanopyTiles, in its own sprite palette):
   two rows of CRUEL_RUIN_CANOPY_COLUMNS sprites, the last of each row sharing
   the first's tiles, their pictures cycling through
   CRUEL_RUIN_CANOPY_PICTURE_COUNT. */
#define CRUEL_RUIN_CANOPY_PALETTE		 15
#define CRUEL_RUIN_CANOPY_COLUMNS		 9
#define CRUEL_RUIN_CANOPY_SPRITES		 (2 * CRUEL_RUIN_CANOPY_COLUMNS)
#define CRUEL_RUIN_CANOPY_SIZE			 32
#define CRUEL_RUIN_CANOPY_PICTURE_COUNT	 8
/* Whole cycles of the pictures added to a sprite's picture number before it is
   taken modulo CRUEL_RUIN_CANOPY_PICTURE_COUNT. */
#define CRUEL_RUIN_CANOPY_CYCLE_BIAS	 (8 * CRUEL_RUIN_CANOPY_PICTURE_COUNT)
/* The falling streaks' particles, after the canopy's pictures. */
#define CRUEL_RUIN_FIRST_STREAK			 CRUEL_RUIN_CANOPY_SPRITES
#define CRUEL_RUIN_STREAKS				 4
/* The forest act lasts CRUEL_RUIN_FOREST_TICKS, its canvas brightening to
   white over the last CRUEL_RUIN_FOREST_BRIGHTEN_TICKS; A or B before then
   starts the brightening at once. */
#define CRUEL_RUIN_FOREST_TICKS			 102
#define CRUEL_RUIN_FOREST_BRIGHTEN_TICKS 17
/* The six blasts, each swelling and shrinking over CRUEL_RUIN_BLAST_TICKS. */
#define CRUEL_RUIN_BLASTS				 6
#define CRUEL_RUIN_BLAST_TICKS			 16
/* The wall of light that sweeps over the foes at the end: a gradient this many
   pixels square, built in the effect's pictures. */
#define CRUEL_RUIN_SWEEP_SIZE			 120
/* The battle sprites' colours, OBJ palettes 0 to 13, which the foes'
   silhouettes take. */
#define CRUEL_RUIN_SPRITE_COLORS		 (14 * PALETTE_COLORS)

/* Cruel Ruin: light swells at the caster's feet as the caster rises out of
   sight; under a sky of sunbeams a glow gathers into an orb amid a turning
   circle of runes, rings of fire fly out from it, rays burst and a ball of
   light falls; then in a sky clearing to gold over a forest rushing by, six
   blasts go off, streaks rain and two ribbons of light sway past; last the ball
   falls on the foes, who darken to silhouettes, and a wall of light sweeps over
   them. */
void btEffectRunCruelRuin(BATTLE_EFFECT_PARM *args)
{
	SCREEN_POINT		 pos;
	FIXED				 place[4];
	MODEL_TEXTURE		 texture;
	MODEL_TEXTURE		 streakTexture;
	SCALE				 canopyScale;
	MODEL_TEXTURE		 burstTexture;
	Uint8				*canvas;
	BATTLE_EFFECT_STATE *state;
	/* LEFTOVER(frame): draw[1], the pair's idle second blitter, which the ROM's
	 * tick reserves and never touches; as one DRAW_RECTANGLE. */
	DRAW_RECTANGLE			 draw[2];
	Sint32					 frm;
	Uint8					*motePictures;
	BATTLE_BACKGROUND_STATE *backdrop;
	Sint32					 savedScroll;
	FIXED					 scrollX;
	Sint32					 phase;
	MOTION_OBJECT			*actor;
	Sint32					 i;
	Sint32					 j;
	Sint32					 k;
	FIXED					 fadeRed;
	FIXED					 fadeGreen;
	FIXED					 fadeBlue;
	FIXED					 redStep;
	FIXED					 greenStep;
	FIXED					 blueStep;
	Sint32					 fadeTicks;
	Sint32					 step;
	RUIN_OBJECT				*obj;

	state = Ram_HeapSlots->battleEff;
	canvas = Ram_HeapSlots->battleCanvas;
	motePictures = Ram_HeapSlots->battleSheet;
	backdrop = Ram_HeapSlots->battleBg;
	btEffectConfigureEffectDisp();
	btEffectStartCanvasLayer(0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	draw[0] = Ram_HeapSlots->ramCode;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	savedScroll = BgScrollWork[1].x;
	scrollX = 0;
	phase = 0;
	actor = btObjectGetStub(args->user)->obj;
	actor->verticalMotionStrength = phase;
	btGraphicsLoadGreyRamp();
	sySndPlayStub(SE_SPIN);
	/* The glow swells at the caster's feet, and from tick 16 the caster rises
	   out of sight. */
	for(frm = 0; frm != 36; frm++) {
		Sint32 size;

		if(frm == 24) {
			EFFECT_SET_TRANSFER(
				state, CANVAS_COPY_AND_BRIGHTEN, CANVAS_FILL_WORD(6));
		}
		btEffectPosProjectBattler(args->user, &pos);
		size = frm * 6 + 2;
		if(size > CRUEL_RUIN_GLOW_SIZE) {
			size = CRUEL_RUIN_GLOW_SIZE;
		}
		btEffectDrawShadedCircles(state->sheet, size, toFIXED(1));
		btCanvasBlitMirroredQuarters(state->sheet, pos.x / 2, pos.y - 16, size);
		if(frm > 15) {
			actor->y += toFIXED(16);
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	actor->verticalMotionStrength = toFIXED(67) / 100;
	actor->y = 0;
	syWordsFill((void *)BG_CHAR_BLOCK(1), CANVAS_BYTES,
		CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
	btEffectShowLivingTargets(state->args);
	btBackgroundLoadStub(
		BATTLE_BACKGROUND_FIXED, RESOURCE_SUNBEAM_CLOUDS_BACKDROP, 0);
	backdrop->busy = 1;
	BgScrollWork[1].x = 0;
	REG_BG2PA = Q_8_8(0.5);
	REG_BG2X = 0;
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	draw[0] = Ram_HeapSlots->ramCode;
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, motePictures, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_FIRE_RING_TEXTURE, MapGridMetatileBuffer, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_CORNER_RUNE_A_SHEET, state->sheet, 1, 0);
	btPictureMirrorIntoQuadrants(
		state->sheet, Ram_MapMetatileBuffer + CRUEL_RUIN_RUNE_CIRCLE, 64, 64);
	btResourceLoadAndDecompress(
		RESOURCE_SWIRL_TEXTURE, state->sheet + CRUEL_RUIN_SWIRL, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_GLOW_BAND_TEXTURE_A, state->sheet + CRUEL_RUIN_BAND_A, 0, 0);
	btResourceLoadAndDecompress(
		RESOURCE_GLOW_BAND_TEXTURE_B, state->sheet + CRUEL_RUIN_BAND_B, 0, 0);
	for(i = 0; i != CRUEL_RUIN_CIRCLE_SIZES; i++) {
		Sint32 size = i * 2 + 2;

		btEffectDrawShadedCirclesMirrored(
			state->sheet + CRUEL_RUIN_CIRCLES + CruelRuinCirclePictures[i],
			size, toFIXED(1));
	}
	btGraphicsLoadGreyRamp();
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	fadeRed = 0;
	fadeGreen = 0;
	fadeBlue = 0;
	redStep = 0;
	greenStep = 0;
	blueStep = 0;
	fadeTicks = 0;
	for(i = 0; i != CRUEL_RUIN_RAYS; i++) {
		PARTICLE *p = &state->particles[i];
		p->z = i * 2730 + (syRandom() & 0xfff);
		p->variant = -(syRandom() & 15);
	}
	for(i = 0; i != CRUEL_RUIN_MOTES; i++) {
		PARTICLE *p = &state->particles[CRUEL_RUIN_FIRST_MOTE + i];
		SET_POSITION(
			p, syRandom() & 127, (syRandom() & 31) + 88, (syRandom() & 3) + 5);
		p->variant = i * -2;
	}
	{
		Uint8 *pixels = MapGridMetatileBuffer;

		/* The sky: the glow gathers into the orb and rises, the circle of runes
		   turns round it, rings of fire fly out, rays burst and the ball falls;
		   A or B skips to its landing. */
		for(frm = 0; frm != 180; frm++) {
			RENDER_MODEL *model;
			void		 *verts;
			Sint32		  shade;

			if((PerWork.repeat & (KEY_A | KEY_B)) && frm <= 168) {
				frm = 169;
			}
			if(frm == 4) {
				sySndPlayStub(SE_SHIMMER);
			}
			if(frm == 40) {
				sySndPlayStub(SE_CAST);
			}
			if(frm == 64) {
				sySndPlayStub(SE_DJINNI_SET);
			}
			if(frm == 74) {
				sySndPlayStub(SE_DJINNI_SET);
			}
			if(frm == 84) {
				sySndPlayStub(SE_DJINNI_SET);
			}
			if(frm == 94) {
				sySndPlayStub(SE_DJINNI_SET);
			}
			if(frm == 124) {
				sySndPlayStub(SE_CLATTER);
			}
			for(i = 0; i != 6; i++) {
				if(frm == 140 + i * 4) {
					sySndPlayStub(SE_LAUNCH);
				}
			}
			if(frm == 169) {
				sySndPlayStub(SE_CRASH);
				state->shakeFrm = 16;
			}
			if(frm == 0) {
				btPaletteBrightenSavedBattleBg(0, 0, 0);
			}
			if(frm == 32) {
				fadeRed = 0;
				fadeGreen = 0;
				fadeBlue = 0;
				redStep = -toFIXED(4) / 28;
				greenStep = -toFIXED(12) / 28;
				blueStep = -toFIXED(4) / 28;
				fadeTicks = 28;
			}
			if(frm == 124) {
				fadeRed = toFIXED(RGB_LEVEL_MAX);
				fadeGreen = toFIXED(8);
				fadeBlue = -toFIXED(4);
				redStep = 0;
				greenStep = 0;
				blueStep = 0;
				fadeTicks = 1;
			}
			if(frm == 128) {
				redStep = (-toFIXED(1) - fadeRed) / 16;
				greenStep = (toFIXED(6) - fadeGreen) / 16;
				blueStep = -fadeBlue / 16;
				fadeTicks = 16;
			}
			if(frm == 144) {
				redStep = (-toFIXED(9) - fadeRed) / 8;
				greenStep = (toFIXED(13) - fadeGreen) / 8;
				blueStep = (-toFIXED(5) - fadeBlue) / 8;
				fadeTicks = 8;
			}
			if(frm == 156) {
				redStep = (toFIXED(RGB_LEVEL_MAX) - fadeRed) / 16;
				greenStep = (toFIXED(8) - fadeGreen) / 16;
				blueStep = (-toFIXED(4) - fadeBlue) / 16;
				fadeTicks = 16;
			}
			if(fadeTicks > 0) {
				fadeTicks--;
				fadeRed += redStep;
				fadeGreen += greenStep;
				fadeBlue += blueStep;
				btPaletteBrightenSavedBattleBg(FIXEDtoINT(fadeRed),
					FIXEDtoINT(fadeGreen), FIXEDtoINT(fadeBlue));
			}
			if(frm == 124) {
				syWordsFill(canvas, CANVAS_BYTES,
					CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
			}
			if(frm <= 123) {
				Sint32 size = CRUEL_RUIN_SKY_GLOW_SIZE;
				Sint32 y = CRUEL_RUIN_SKY_GLOW_Y;
				Sint32 scale = toFIXED(2);

				if(frm <= 29) {
					size -= frm * 4;
				} else {
					size = 114 - frm * 3;
				}
				if(frm > 7) {
					scale = toFIXED(9) / 4 - frm * (toFIXED(1) / 32);
				}
				if(scale < toFIXED(1) / 2) {
					scale = toFIXED(1) / 2;
				}
				if(frm > 35) {
					y = 20;
				} else if(frm > 19) {
					y = (FIXEDtoINT(
							syCos(frm * (TRIG_TURN / 64) - TRIG_TURN * 5 / 16) *
							CRUEL_RUIN_SKY_GLOW_RISE)) +
						(CRUEL_RUIN_SKY_GLOW_Y - CRUEL_RUIN_SKY_GLOW_RISE);
				}
				if(size <= 7) {
					size = 8;
				}
				if(frm > 67) {
					size += (frm - 68) / 6;
				}
				btEffectDrawShadedCircles(state->sheet, size, scale);
				btCanvasBlitMirroredQuarters(state->sheet, 60, y, size);
			}
			if(frm > 127) {
				for(i = 0; i != CRUEL_RUIN_MOTES; i++) {
					PARTICLE *p = &state->particles[CRUEL_RUIN_FIRST_MOTE + i];

					if((Uint32)p->variant < CRUEL_RUIN_MOTE_TICKS) {
						Sint32 s = FIXEDtoINT(
							sySin(p->variant * (TRIG_TURN / 64)) * p->z);

						draw[0](canvas,
							state->sheet + CRUEL_RUIN_CIRCLES +
								CruelRuinCirclePictures[s - 1],
							p->x - s, p->y - s * 2, s * 2, s * 4);
					}
					p->variant++;
				}
			}
			verts = syHeapAllocEWRAM(32);
			model = btRenderAllocateModelList(1);
			{
				Sint32 s;
				Sint32 t;
				texture.logWidth = 7;
				texture.logHeight = 7;
				model->kind = MODEL_SHADED_MAX;
				model->texture = &texture;
				model->mesh = BattleEffectLargeSquareMesh;
				model->vertices = verts;
				texture.pixels = pixels;
				shade = 0;
				model->shade = shade;
				i = shade;

				for(s = frm * (toFIXED(1) / 2) - toFIXED(32), t = frm - 64;
					i != CRUEL_RUIN_RINGS;
					s -= toFIXED(5), i++, t -= CRUEL_RUIN_RING_INTERVAL) {
					if((Uint32)t < CRUEL_RUIN_RING_TICKS) {
						Sint32 sc = toFIXED(8) - s;

						syInitMatrix();
						syTranslate(-toFIXED(4), -toFIXED(44), 0);
						syScale(sc / 2, sc, sc);
						syRotY(CruelRuinDiscYaws[i]);
						syRotX(CruelRuinDiscPitches[i]);
						btRenderTransformVertices(
							BattleEffectSquareCorners, verts, 4);
						btRenderDrawModelList(model);
					}
				}
			}
			step = frm - 124;
			if((Uint32)step < CRUEL_RUIN_RING_TICKS) {
				Sint32 sc = step * (toFIXED(1) / 2);

				shade = CRUEL_RUIN_RING_SHINE_TICKS * 8 - step * 8;
				if(shade > 0) {
					shade = 0;
				}
				model->shade = shade;
				syInitMatrix();
				syTranslate(-toFIXED(4), -toFIXED(44), 0);
				syScale(sc >> 1, sc, sc);
				btRenderTransformVertices(BattleEffectSquareCorners, verts, 4);
				btRenderDrawModelList(model);
			}
			{
				Sint32 t = frm - 24;

				if((Uint32)t < CRUEL_RUIN_RUNE_TICKS) {
					Sint32 sc = toFIXED(3) - t * (toFIXED(1) / 32);

					if(sc < toFIXED(1)) {
						sc = toFIXED(1);
					}
					shade = -32;
					if(t <= 7) {
						shade = t * 4 + MODEL_SHADE_INVISIBLE;
					}
					if(frm > 63) {
						shade = 224 - frm * 4;
					}
					model->shade = shade;
					texture.pixels =
						Ram_MapMetatileBuffer + CRUEL_RUIN_RUNE_CIRCLE;
					t <<= 9;
					syInitMatrix();
					syTranslate(-toFIXED(4), -toFIXED(44), 0);
					syScale(sc, sc / 3, sc >> 1);
					syRotZ(t);
					if(shade > MODEL_SHADE_INVISIBLE) {
						btRenderTransformVertices(
							BattleEffectSquareCorners, verts, 4);
						btRenderDrawModelList(model);
					}
					texture.pixels = state->sheet + CRUEL_RUIN_SWIRL;
					texture.logWidth = 6;
					texture.logHeight = 6;
					model->mesh = BattleEffectSquareMesh;
					if(frm > 63) {
						shade = frm * 4 + -320;
						if(shade > 0) {
							shade = 0;
						}
						model->shade = shade;
						syInitMatrix();
						syTranslate(-toFIXED(4), -toFIXED(44), 0);
						syScale(sc, sc >> 1, sc >> 1);
						syRotZ(t);
						btRenderTransformVertices(
							BattleEffectSquareCorners, verts, 4);
						btRenderDrawModelList(model);
					}
				}
			}
			if(frm > 123) {
				texture.logWidth = 5;
				texture.logHeight = 2;
				texture.pixels = state->sheet + CRUEL_RUIN_BAND_A;
				model->mesh = BattleEffectSmallSquareMesh;
				model->kind = MODEL_SHADED_MAX;
				shade = -16;
				model->shade = shade;
				for(i = 0; i != CRUEL_RUIN_RAYS; i++) {
					PARTICLE *p = &state->particles[i];

					if(p->variant >= 0) {
						Sint32 sy = p->variant * (toFIXED(1) / 8);

						if(sy > CRUEL_RUIN_RAY_LENGTH_MAX) {
							sy = CRUEL_RUIN_RAY_LENGTH_MAX;
						}
						syInitMatrix();
						syTranslate(-toFIXED(4), -toFIXED(44), 0);
						syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
						syRotZ(p->z);
						syScale(CRUEL_RUIN_RAY_WIDTH, sy, toFIXED(1) / 2);
						btRenderTransformVertices(
							BattleEffectBeamCorners, verts, 4);
						btRenderDrawModelList(model);
					}
					p->variant++;
				}
				if(frm > 163) {
					EFFECT_SET_TRANSFER(
						state, CANVAS_COPY_AND_CLEAR, CANVAS_FILL_WORD(1));
					btPaletteBrightenBgEntries(2, 2, 2);
				}
				if(frm > 159) {
					PARTICLE *q = &state->particles[CRUEL_RUIN_BALL_PARTICLE];
					Sint32	  r = frm * 2 + -288;

					if(r > CRUEL_RUIN_BALL_SIZE) {
						r = CRUEL_RUIN_BALL_SIZE;
					}
					if(frm == 160) {
						q->x = toFIXED(128);
						q->y = -toFIXED(32);
					}
					if(frm <= 171) {
						q->x += -toFIXED(8);
						q->y += toFIXED(16);
						btEffectDrawShadedCircles(state->sheet, r, toFIXED(1));
						btCanvasBlitMirroredQuarters(state->sheet,
							FIXED_INTEGER_PART(q->x), FIXED_INTEGER_PART(q->y),
							r);
					}
					model->kind = MODEL_SHADED_MAX;
					shade = 0;
					model->shade = shade;
					texture.pixels = state->sheet + CRUEL_RUIN_BAND_B;
					syInitMatrix();
					syTranslate(q->x - toFIXED(63), q->y - toFIXED(64), 0);
					syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
					syRotZ(TRIG_TURN * 5 / 8);
					syScale(r * CRUEL_RUIN_STREAK_SCALE, toFIXED(2),
						toFIXED(1) / 2);
					btRenderTransformVertices(CruelRuinStreakCorners, verts, 4);
					btRenderDrawModelList(model);
				}
			}
			syHeapRelease(model);
			syHeapRelease(verts);
			if(state->shakeFrm > 0) {
				{
					Sint32 shakeY = syRandom() % 8 + 28;
					BgScrollWork[1].y = shakeY;
				}
				state->shakeFrm--;
			} else {
				BgScrollWork[1].y = state->savedBg1Y;
			}
			btEffectCountDownBattlerTints();
			WAIT_TRANSFER(state);
			EFFECT_PRESENT_CANVAS(state);
		}
	}
	{
		Sint32 skipTicks = -1;
		Sint32 fadeStep = 2;
		Uint8 *strip = CRUEL_RUIN_STRIP_MESH;
		void  *verts;
		btRenderBuildQuadStrip(strip, 8, 16, CRUEL_RUIN_RIBBON_QUADS);
		for(k = 0; k != 2; k++) {
			Sint32 c;

			for(c = 0; c != CRUEL_RUIN_RIBBON_POINTS; c++) {
				Sint32 w = CruelRuinPillarWidths[k];
				Sint8 *v;
				Sint32 acc;
				Sint32 height;
				Sint32 half;

				if(c > CRUEL_RUIN_RIBBON_QUADS / 2 - 1) {
					w = w + (c - CRUEL_RUIN_RIBBON_QUADS / 2) * 2;
				}
				acc = 0;
				{
					Sint8 *cells = (Sint8 *)MapGridMetatileBuffer;
					Sint32 cell = k * CRUEL_RUIN_RIBBON_QUADS + k + c;

					v = cells + cell * CRUEL_RUIN_RIBBON_POINT_BYTES;
				}
				height = 126 - c * 7;
				half = w / 2;
				for(i = 0; i != 2; i++, acc += w, v += 4) {
					v[0] = height;
					v[1] = (FIXEDtoINT(sySin(c * CRUEL_RUIN_RIBBON_SWAY_STEP) *
							   CruelRuinPillarSways[k])) +
						acc - half;
					v[2] = 0;
				}
			}
		}
		EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
		syWordsFill(
			canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		EFFECT_PRESENT_CANVAS(state);
		syWordsFill(canvas, CANVAS_BYTES, 0);
		REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
			BGCNT_SCREEN_BASE(7);
		btBackgroundLoadStub(
			BATTLE_BACKGROUND_FIXED, RESOURCE_VIOLET_SKY_BACKDROP, 0);
		btPaletteBrightenBattleBg(RGB_LEVEL_MAX, RGB_LEVEL_MAX, RGB_LEVEL_MAX);
		btResourceLoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_B,
			state->sheet + CRUEL_RUIN_BAND_A, 0, 0);
		btResourceLoadAndDecompress(
			RESOURCE_PINK_BAR_SHEET, state->sheet + CRUEL_RUIN_PINK_BAR, 0, 0);
		btGraphicsLoadGreyRamp();
		{
			Uint8 *canopy =
				syResourceGetTableEntry(RESOURCE_FOREST_CANOPY_TILES);

			syWordsFill((void *)&OBJ_PLTT_COLOR(CRUEL_RUIN_CANOPY_PALETTE, 0),
				PALETTE_BANK_BYTES, COLOR_PAIR(RGB_WHITE));
			syDecompressTaggedLZ(canopy + PALETTE_BANK_BYTES, state->sheet);
		}
		{
			Sint32 q;
			for(q = 0; q != 2; q++) {
				Uint8		*src;
				RUIN_OBJECT *obj;

				i = 0;
				src = state->sheet + q * 8 * OBJ_32X32_4BPP_BYTES;
				for(; i != 8; i++) {
					obj = btEffectSprCreateWithTiles(CRUEL_RUIN_CANOPY_SIZE,
						CRUEL_RUIN_CANOPY_SIZE, OBJ_SIZE_32X32,
						OBJ_PALETTE(CRUEL_RUIN_CANOPY_PALETTE));
					state->sprs[q * CRUEL_RUIN_CANOPY_COLUMNS + i] = obj;
					/* The sprite takes priority 1, in bits 2 and 3 of the high
					   byte of its OAM attribute 2. */
					{
						Sint32 b = obj->attr.bytes[1];
						Uint8  x = b & -13;
						x |= 4;
						obj->attr.bytes[1] = x;
					}
					syWordsCopy(
						(void *)OBJ_TILES(obj), src, OBJ_32X32_4BPP_BYTES);
					src += OBJ_32X32_4BPP_BYTES;
				}
				obj = btEffectSprAcquire(CRUEL_RUIN_CANOPY_SIZE,
					CRUEL_RUIN_CANOPY_SIZE, OBJ_SIZE_32X32,
					OBJ_PALETTE(CRUEL_RUIN_CANOPY_PALETTE));
				state->sprs[q * CRUEL_RUIN_CANOPY_COLUMNS + 8] = obj;
				syWordsCopy(obj, state->sprs[q * CRUEL_RUIN_CANOPY_COLUMNS],
					offsetof(SPR, lastNo));
				for(i = 0; i != 8; i++) {
					state->particles[q * CRUEL_RUIN_CANOPY_COLUMNS + i]
						.variant =
						((RUIN_OBJECT *)
								state->sprs[q * CRUEL_RUIN_CANOPY_COLUMNS + i])
							->attr.bits.tile;
				}
			}
		}
		sySynch(1);
		/* The sky, clearing from white through violet to gold, over the forest:
		   the blasts, the falling streaks and the two ribbons; A or B ends it
		   early. */
		frm = 0;
		while(1) {
			RENDER_MODEL *model;

			if((PerWork.repeat & (KEY_A | KEY_B)) &&
				frm < CRUEL_RUIN_FOREST_TICKS -
						CRUEL_RUIN_FOREST_BRIGHTEN_TICKS &&
				skipTicks == -1) {
				EFFECT_SET_TRANSFER(
					state, CANVAS_COPY_AND_BRIGHTEN, CANVAS_FILL_WORD(4));
				skipTicks = CRUEL_RUIN_FOREST_BRIGHTEN_TICKS;
				REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
					BGCNT_SCREEN_BASE(7);
			}
			if(frm <= 17) {
				Sint32 green = RGB_LEVEL_MAX - frm * fadeStep;
				Sint32 red = green;
				Sint32 blue = RGB_LEVEL_MAX - (frm * fadeStep + frm);

				if(green < 0) {
					red = 0;
				}
				if(green <= 5) {
					green = 6;
				}
				if(blue < -18) {
					blue = -18;
				}
				btPaletteBrightenSavedBattleBg(red, green, blue);
				if(frm <= 16) {
					Uint16 *canopy =
						syResourceGetTableEntry(RESOURCE_FOREST_CANOPY_TILES);
					Uint16 *dst =
						(Uint16 *)&OBJ_PLTT_COLOR(CRUEL_RUIN_CANOPY_PALETTE, 0);
					Sint32 add;

					i = 0;
					add = 32 - frm * fadeStep;
					for(; i != PALETTE_COLORS; i++, canopy++, dst++) {
						Sint32 cr = RGB_RED(*canopy) + add;
						Sint32 cg = (Uint16)RGB_GREEN(*canopy) + add;
						Sint32 cb = (Uint16)RGB_BLUE(*canopy) + add;

						if(cr > RGB_LEVEL_MAX) {
							cr = RGB_LEVEL_MAX;
						}
						if(cg > RGB_LEVEL_MAX) {
							cg = RGB_LEVEL_MAX;
						}
						if(cb > RGB_LEVEL_MAX) {
							cb = RGB_LEVEL_MAX;
						}
						*dst = RGB(cr, cg, cb);
					}
				}
			}
			if(frm == 0) {
				REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHAR_BASE(1) |
					BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
			}
			if(frm == 75) {
				REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
					BGCNT_SCREEN_BASE(7);
			}
			if(frm ==
				CRUEL_RUIN_FOREST_TICKS - CRUEL_RUIN_FOREST_BRIGHTEN_TICKS) {
				EFFECT_SET_TRANSFER(
					state, CANVAS_COPY_AND_BRIGHTEN, CANVAS_FILL_WORD(4));
			}
			verts = syHeapAllocEWRAM(528);
			model = btRenderAllocateModelList(1);
			{
				Sint32 shade = 0;
				streakTexture.logWidth = 5;
				streakTexture.logHeight = 2;
				streakTexture.pixels = state->sheet + CRUEL_RUIN_BAND_A;
				model->shade = shade;
				model->mesh = BattleEffectSmallSquareMesh;
				model->texture = &streakTexture;
				model->vertices = verts;
				model->kind = MODEL_SHADED_MAX;
				for(i = 0; i != CRUEL_RUIN_BLASTS; i++) {
					Sint32 d = frm - CruelRuinBlastTicks[i];

					if(d == 0) {
						if(i == CRUEL_RUIN_BLASTS - 1) {
							sySndPlayStub(SE_CRASH);
						} else {
							sySndPlayStub(SE_CLATTER);
						}
						state->shakeFrm = 8;
						syWordsFill(canvas, CANVAS_BYTES,
							CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
					}
					if((Uint32)d < CRUEL_RUIN_BLAST_TICKS) {
						Sint32 r = (FIXEDtoINT(sySin(d * (TRIG_TURN / 32)) *
									   CruelRuinBlastSizes[i])) +
							2;

						if(r > 1) {
							Sint32 x = CruelRuinBlastX[i] + d * 8;
							Sint32 y = CruelRuinBlastY[i];
							Sint32 half;
							Sint32 flags = EFFECT_CLIP_BOUNDS;

							btEffectDrawShadedCircles(
								state->sheet, r, toFIXED(1));
							EFFECT_LOAD_BLITTER(
								HEAP_SLOT_RAM_CODE_PAIRED, flags, BLITTER_MAX);
							half = r / 2;
							y -= r;
							ALT_BLITTER(
								canvas, state->sheet, x - half, y + 8, half, r);
							syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
							flags = EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X;
							EFFECT_LOAD_BLITTER(
								HEAP_SLOT_RAM_CODE_PAIRED, flags, BLITTER_MAX);
							ALT_BLITTER(
								canvas, state->sheet, x, y + 8, half, r);
							syHeapSlotRelease(HEAP_SLOT_RAM_CODE_PAIRED);
						}
					}
				}
			}
			for(i = 0; i != CRUEL_RUIN_STREAKS; i++) {
				PARTICLE *p = &state->particles[i + CRUEL_RUIN_FIRST_STREAK];
				Sint32	  start = CruelRuinStreakStarts[i] >> 1;

				if(frm == 0) {
					p->y = frm;
					p->variant = frm;
					p->x = toFIXED(CruelRuinStreakX[i]);
				}
				if(frm >= start && frm < start + 32) {
					Sint32 size = 0;
					FIXED  px = 0;
					FIXED  py = 0;
					Sint32 ang = 0;
					Sint32 scale = 0;
					size = CruelRuinStreakSizes[i];

					if(p->y < toFIXED(CruelRuinStreakEndY[i])) {
						p->x -= toFIXED(CruelRuinStreakStepX[i]);
						p->y += toFIXED(CruelRuinStreakStepY[i]);
					} else {
						p->variant++;
					}
					p->x += toFIXED(8);
					syInitMatrix();
					px = p->x - toFIXED(64);
					py = p->y - toFIXED(64);
					syTranslate(px, py, 0);
					syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
					ang = CruelRuinStreakRolls[i];
					syRotZ(ang);
					scale = size * CRUEL_RUIN_STREAK_SCALE;
					syScale(scale, toFIXED(2), toFIXED(1) / 2);
					btRenderTransformVertices(CruelRuinStreakCorners, verts, 4);
					btEffectDrawShadedCircles(state->sheet, size, toFIXED(1));
					btCanvasBlitMirroredQuarters(state->sheet,
						FIXED_INTEGER_PART(p->x) - 1, FIXED_INTEGER_PART(p->y),
						size);
					btRenderDrawModelList(model);
				}
			}
			streakTexture.logWidth = 6;
			streakTexture.logHeight = 5;
			streakTexture.pixels = state->sheet + CRUEL_RUIN_PINK_BAR;
			btRenderBuildQuadStrip(strip, 2, 32, CRUEL_RUIN_RIBBON_QUADS);
			model->mesh = strip;
			model->kind = MODEL_SHADED_MAX;
			model->scrollU = ~CRUEL_RUIN_RIBBON_SCROLL & TEXTURE_SCROLL_MASK;
			{
				Uint8 *cells = MapGridMetatileBuffer;
				Sint32 mask;

				for(i = 0, mask = TEXTURE_SCROLL_MASK; i != 2; cells +=
					CRUEL_RUIN_RIBBON_POINTS * CRUEL_RUIN_RIBBON_POINT_BYTES,
				i++) {
					Sint32 t = frm - i * CRUEL_RUIN_RIBBON_TICKS - 12;

					if((Uint32)t < CRUEL_RUIN_RIBBON_TICKS) {
						Sint32 x;
						Uint8 *m;
						Sint32 z;
						Sint32 v;

						if(i == 0) {
							v = t * 2 + CRUEL_RUIN_RIBBON_SCROLL;
							v = mask & ~v;
							model->scrollU = v;
							if(t <= CRUEL_RUIN_RIBBON_QUADS) {
								Uint8 *m =
									strip + t * CRUEL_RUIN_STRIP_QUAD_BYTES;
								m[0] = i;
								m[1] = i;
								m[2] = i;
							}
						} else {
							v = t * 4 + CRUEL_RUIN_RIBBON_SCROLL;
							v = mask & ~v;
							model->scrollU = v;
							{
								Sint32 z = 0;
								if(t < CRUEL_RUIN_RIBBON_QUADS / 2) {
									Uint8 *m = strip +
										t * 2 * CRUEL_RUIN_STRIP_QUAD_BYTES;
									m[0] = z;
									m[1] = z;
									m[2] = z;
								}
							}
						}
						syInitMatrix();
						x = -toFIXED(24);
						if(t >= 0) {
							x = toFIXED(t) - toFIXED(24);
						}
						if(i == 0) {
							if(t > CRUEL_RUIN_RIBBON_QUADS - 1) {
								x += INTtoFIXED(
									(t - CRUEL_RUIN_RIBBON_QUADS) * 7);
							}
						} else if(t > CRUEL_RUIN_RIBBON_QUADS / 2 - 1) {
							x += INTtoFIXED(
								(t - CRUEL_RUIN_RIBBON_QUADS / 2) * 7);
						}
						syTranslate(x, -toFIXED(16), 0);
						syScale(CRUEL_RUIN_RIBBON_STRETCH, toFIXED(1),
							toFIXED(1) / 2);
						if(i == 0) {
							syRotZ(-CRUEL_RUIN_FIRST_RIBBON_ROLL);
						} else {
							syRotZ(-(TRIG_TURN / 64));
						}
						btRenderTransformVertices(
							cells, verts, CRUEL_RUIN_RIBBON_POINTS * 2);
						btRenderDrawModelList(model);
					}
				}
			}
			syHeapRelease(model);
			syHeapRelease(verts);
			canopyScale = CruelRuinUnitScale;
			place[3] = 0;
			place[1] = toFIXED(255);
			/* The forest rushes by, a sprite's width every so often, when each
			   sprite takes the picture of the one before it. */
			scrollX += toFIXED(24);
			if(scrollX > toFIXED(CRUEL_RUIN_CANOPY_SIZE) - 1) {
				phase = (phase + CRUEL_RUIN_CANOPY_PICTURE_COUNT - 1) %
					CRUEL_RUIN_CANOPY_PICTURE_COUNT;
				scrollX -= toFIXED(CRUEL_RUIN_CANOPY_SIZE);
			}
			BgScrollWork[1].x -= 8;
			for(j = 0; j != 2; j++) {
				Sint32 row = j * CRUEL_RUIN_CANOPY_COLUMNS;
				void **last = &state->sprs[row + CRUEL_RUIN_CANOPY_COLUMNS - 1];

				for(i = 0; i != CRUEL_RUIN_CANOPY_COLUMNS; i++) {
					Sint32 mask = ~OBJ_TILE_MASK;
					Sint32 ofs = CRUEL_RUIN_CANOPY_CYCLE_BIAS;
					Sint32 tile = i + ofs + phase;
					Sint32 v;
					Sint32 x;

					place[0] = toFIXED(i * CRUEL_RUIN_CANOPY_SIZE) + scrollX;
					place[2] =
						toFIXED(121) + toFIXED(j * CRUEL_RUIN_CANOPY_SIZE);
					/* FAKEMATCH(dead): a store both arms below overwrite. */
					if(i == 300 && row == j && last == 0)
						step = 0;
					/* The sprite's picture: tile %
					   CRUEL_RUIN_CANOPY_PICTURE_COUNT, the last sprite taking
					   the one before the first's. */
					if(i == CRUEL_RUIN_CANOPY_COLUMNS - 1) {
						obj = *last;
						/* FAKEMATCH(temporary): cycle and cycleStart, the
						 * modulo by CRUEL_RUIN_CANOPY_PICTURE_COUNT written
						 * out. */
						{
							Sint32 cycle = phase +
								(CRUEL_RUIN_CANOPY_CYCLE_BIAS +
									CRUEL_RUIN_CANOPY_PICTURE_COUNT - 1);
							Sint32 cycleStart = cycle;
							if(cycle < 0)
								cycleStart = cycle +
									(CRUEL_RUIN_CANOPY_PICTURE_COUNT - 1);
							cycleStart >>= 3;
							cycleStart <<= 3;
							step = cycle - cycleStart;
						}
					} else {
						/* FAKEMATCH(temporary): j * CRUEL_RUIN_CANOPY_COLUMNS
						 * written again for the row. */
						{
							Sint32 m = j * CRUEL_RUIN_CANOPY_COLUMNS;
							m += i;
							obj = state->sprs[m];
						}
						/* FAKEMATCH(temporary): phase + i, held for the
						 * negative case. */
						{
							Sint32 next = phase + i;
							Sint32 cycleStart = tile;
							if(tile < 0)
								cycleStart = next +
									(CRUEL_RUIN_CANOPY_CYCLE_BIAS +
										CRUEL_RUIN_CANOPY_PICTURE_COUNT - 1);
							cycleStart >>= 3;
							cycleStart <<= 3;
							step = tile - cycleStart;
						}
					}
					v = state->particles[row + step].variant & OBJ_TILE_MASK;
					x = obj->attr.all & mask;
					obj->attr.all = x | v;
					if(place[0] > toFIXED(256) - 1) {
						place[0] -= toFIXED(288);
					}
					fdRenderApplyProjectedPlacementStub(
						state->sprs[j * CRUEL_RUIN_CANOPY_COLUMNS + i], place,
						(Sint32 *)&canopyScale, 0);
				}
			}
			btEffectCountDownBattlerTints();
			WAIT_TRANSFER(state);
			if(state->shakeFrm > 0) {
				{
					Sint32 shakeY = syRandom() % 8 + 28;
					BgScrollWork[1].y = shakeY;
				}
				state->shakeFrm--;
			} else {
				BgScrollWork[1].y = state->savedBg1Y;
			}
			EFFECT_PRESENT_CANVAS(state);
			frm++;
			if(frm == CRUEL_RUIN_FOREST_TICKS) {
				break;
			}
			if(skipTicks > 0 && --skipTicks == 0) {
				break;
			}
		}
	}
	BgScrollWork[1].x = savedScroll;
	backdrop->busy = 0;
	{
		BATTLE_STATE *battle = Ram_HeapSlots->battleState;

		BgScrollWork[1].y = BATTLE_BACKGROUND_SCROLL_Y;
		btBackgroundLoadStub(
			BATTLE_BACKGROUND_FIXED, battle->bg, BACKGROUND_DARKENED_LEVEL);
	}
	sySynch(1);
	ProjectionWork.centerY = BATTLE_PROJECTION_CENTER_Y;
	sySndPlayStub(SOUND_FADE_OUT_ACTION_SE);
	for(i = 0; i != CRUEL_RUIN_CANOPY_SPRITES; i++) {
		fdSprDestroyStub(state->sprs[i]);
	}
	syPaletteLoadBasicSprCols();
	syDMAXCopy((const void *)OBJ_PLTT,
		(void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0), DMA_COPY_HALFWORDS(16),
		REG_DMA3);
	BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
		BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
	syWordsFill(canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
	args->tgts[args->tgtCnt] = BATTLE_UNIT_LIST_END;
	btActorSpawnObjectsForListStub(args->tgts, 0);
	btResourceLoadAndDecompress(RESOURCE_GLOW_BAND_TEXTURE_B,
		state->sheet + CRUEL_RUIN_BURST_BAND, 0, 0);
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	for(i = 0; i != CRUEL_RUIN_SWEEP_SIZE; i++) {
		Sint32 v = i / 2;

		if(v > CANVAS_LEVEL_BRIGHTEST) {
			v = CANVAS_LEVEL_BRIGHTEST;
		}
		for(j = 0; j != CRUEL_RUIN_SWEEP_SIZE; j++) {
			state->sheet[i + j * CRUEL_RUIN_SWEEP_SIZE] = v;
		}
	}
	/* The ball falls on the foes, who darken to silhouettes as the wall of
	   light sweeps over them. */
	for(frm = 0; frm != 140; frm++) {
		Sint32 grey = frm - 80;
		Sint32 blue = RGB(0, 0, grey);
		Sint32 green = RGB(0, grey, 0);

		if(frm == 0) {
			for(i = 0; i != args->tgtCnt; i++) {
				btEffectTintBattler(
					args->tgts[i], SPRITE_COLOR_NORMAL, TINT_KEEP, i, 16);
			}
		}
		if(frm == 127) {
			EFFECT_SHOW_DAMAGE(SE_HEAVY_IMPACT);
		}
		if(frm == 107) {
			syWordsFill(
				canvas, CANVAS_BYTES, CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
		}
		if(frm > 107) {
			EFFECT_SET_TRANSFER(
				state, CANVAS_COPY_AND_DARKEN, CANVAS_FILL_WORD(2));
			btPaletteBrightenSavedBattleBg(0, 0, 0);
		}
		if(frm == 32) {
			sySndPlayStub(SE_SPIN);
		}
		if(frm == 48) {
			state->shakeFrm = 90;
			sySndPlayStub(SE_CRASH);
		}
		if(frm == 72) {
			sySndPlayStub(SE_BANE_ESCAPE);
		}
		if(frm <= 109) {
			RENDER_MODEL *model;
			void		 *verts;

			if(frm > 31) {
				Sint32 a = (frm - 32) / 4;
				Sint32 b = 0;

				if(frm > 47) {
					b = (frm - 48) / 2;
				}
				btPaletteBrightenBattleBg(a, a / 2, b);
			}
			if((Uint32)(frm - 80) <= 31) {
				Uint32	c;
				Uint16 *dst = (Uint16 *)&OBJ_PLTT_COLOR(0, 1);

				i = 0;
				c = blue | green | (frm - 80);
				for(; i != CRUEL_RUIN_SPRITE_COLORS - 1; i++) {
					*dst++ = c;
				}
			} else if(frm > 47) {
				btColBufferScaleStub((void *)OBJ_PLTT, (void *)OBJ_PLTT,
					toFIXED(15) / 16, CRUEL_RUIN_SPRITE_COLORS);
			}
			if(frm > 47) {
				draw[0](canvas, state->sheet, 216 - frm * 2, 0,
					CRUEL_RUIN_SWEEP_SIZE, CRUEL_RUIN_SWEEP_SIZE);
			}
			verts = syHeapAllocEWRAM(32);
			model = btRenderAllocateModelList(1);
			{
				Sint32 shade = 0;
				burstTexture.logWidth = 5;
				burstTexture.logHeight = 2;
				burstTexture.pixels = state->sheet + CRUEL_RUIN_BURST_BAND;
				model->shade = shade;
				model->mesh = BattleEffectSmallSquareMesh;
				model->texture = &burstTexture;
				model->vertices = verts;
				model->kind = MODEL_SHADED_MAX;
			}
			if(frm > 31) {
				PARTICLE *q = &state->particles[CRUEL_RUIN_BALL_PARTICLE];
				Sint32	  r = frm * 2 - 32;

				if(r > CRUEL_RUIN_BALL_SIZE) {
					r = CRUEL_RUIN_BALL_SIZE;
				}
				if(frm == 32) {
					q->x = toFIXED(128);
					q->y = -toFIXED(32);
				}
				if(frm <= 43) {
					q->x += -toFIXED(8);
					q->y += toFIXED(16);
					btEffectDrawShadedCircles(
						state->sheet + CRUEL_RUIN_BURST_CIRCLE, r, toFIXED(1));
					btCanvasBlitMirroredQuarters(
						state->sheet + CRUEL_RUIN_BURST_CIRCLE,
						FIXED_INTEGER_PART(q->x), FIXED_INTEGER_PART(q->y), r);
				}
				syInitMatrix();
				syTranslate(q->x - toFIXED(63), q->y - toFIXED(64), 0);
				syScale(toFIXED(1) / 2, toFIXED(1), toFIXED(1) / 2);
				syRotZ(TRIG_TURN * 5 / 8);
				syScale(
					r * CRUEL_RUIN_STREAK_SCALE, toFIXED(2), toFIXED(1) / 2);
				btRenderTransformVertices(CruelRuinStreakCorners, verts, 4);
				btRenderDrawModelList(model);
			}
			syHeapRelease(model);
			syHeapRelease(verts);
		}
		{
			Sint32 r = 8;

			if(frm > 130) {
				r = 2;
			} else if(frm > 120) {
				r = 4;
			} else if(frm > 110) {
				r = 6;
			}
			btCameraShake(r, r);
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	syPaletteLoadBasicSprCols();
	EFFECT_FINISH_CANVAS_ONE_BLITTER();
}

#undef PLOT
#undef STREAK_SEEDS
#undef STREAKS
#undef STREAK_START_DISTANCE
#undef STREAK_LENGTH
