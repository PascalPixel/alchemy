/* Condemn. Part of battle_effect.c. */

#define CONDEMN_TICKS	   104
#define CONDEMN_SWING_TICK 72 /* the reaper swings, in a white flash */
/* How far the reaper sways across and up and down as it circles, in pixels. */
#define CONDEMN_SWAY_X	   16
#define CONDEMN_SWAY_Y	   4

/* The pictures Condemn draws: the reaper raised and swinging, the crescent its
   scythe leaves and the crescent's four fades. */
#define CONDEMN_PICTURES 7
/* Where `picture`, one of Condemn's pictures, is drawn, by the casting side's
   `column` and `row`. */
#define CONDEMN_PICTURE_X(column, picture) \
	CondemnPictureX[(column) * CONDEMN_PICTURES + (picture)]
#define CONDEMN_PICTURE_Y(row, picture) \
	CondemnPictureY[(row) * CONDEMN_PICTURES + (picture)]

/* A hooded reaper with a scythe circles in, fading out through a ripple as it
   jumps closer twice, the second time onto the first target; the screen flashes
   white, the reaper swings, and the red crescent its scythe leaves fades while
   the target is knocked back and the screen shakes.

   Abilities: Condemn. */
void btEffectRunCondemn(BATTLE_EFFECT_PARM *args)
{
	BATTLE_EFFECT_STATE *state = Ram_HeapSlots->battleEff;
	void				*canvas = Ram_HeapSlots->battleCanvas;
	/* what the canvas is cleared to after each copy */
	Sint32		 transferValue = 0;
	BLITTER_PAIR draw;
	/* LEFTOVER(frame): position[1], which the ROM's frame reserves and never */
	/* touches; as one SCREEN_POINT. */
	SCREEN_POINT pos[2];
	FIXED		 swayCenter[2];
	Sint32		 drawReaper;
	Sint32		 pictureColumn;
	Sint32		 pictureRow;
	Uint8 *cells; /* the cell buffer, held across the reveal sheet's load */
	Sint32 frm;

	btEffectStartTiledCanvas(BGCNT_WRAP);
	REG_BG2PA = Q_8_8(1);
	cells = MapGridMetatileBuffer;
	REG_BLDALPHA = EFFECT_BLEND_ADD;
	btResourceLoadAndDecompress(
		RESOURCE_COUNTER_REVEAL_SHEET, state->sheet, 1, 1);
	btResourceLoadAndDecompress(RESOURCE_RED_CRESCENT_SHEET_A, cells, 1, 0);
	EFFECT_LOAD_BLITTER(
		HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_TRANSPARENT);
	draw[0] = Ram_HeapSlots->ramCode;
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
		EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, BLITTER_TRANSPARENT);
	draw[1] = Ram_HeapSlots->ramCodePaired;
	sySetEvent(btEffectArmBg2AffineHBlankDMA, EVENT_PASS_VBLANK);
	EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, transferValue);

	/* The reaper starts off to the side, above the field. */
	drawReaper = 1;
	if(args->side == EFFECT_SIDE_PARTY)
		swayCenter[0] = -toFIXED(80);
	else
		swayCenter[0] = toFIXED(112);
	swayCenter[1] = -toFIXED(32);

	for(frm = 0; frm != CONDEMN_TICKS; frm++) {
		Sint32	spin;
		Sint32	screenX;
		Sint32	screenY;
		Sint32	ripple;
		Sint32	lineOfs;
		Sint32	ang;
		Sint32	i;
		Sint32 *scanline;

		spin = frm * (TRIG_TURN / 128);
		screenX = FIXEDtoINT(swayCenter[0]) +
			FIXEDtoINT(sySin(spin) * CONDEMN_SWAY_X) + 48;
		screenY = FIXEDtoINT(swayCenter[1]) +
			FIXEDtoINT(syCos(spin) * CONDEMN_SWAY_Y) + 16;

		/* The second pass, lower and further across, hidden for the tick of
		   the jump. */
		if(frm == 24) {
			if(args->side == EFFECT_SIDE_PARTY)
				swayCenter[0] = -toFIXED(32);
			else
				swayCenter[0] = toFIXED(72);
			swayCenter[1] = toFIXED(24);
			drawReaper = 0;
		}
		if(frm == 25) {
			REG_BLDALPHA = EFFECT_BLEND_ADD;
			drawReaper = 1;
		}
		/* The last pass, on the first target. */
		if(frm == 48) {
			btEffectPosProjectScaledBattler(args->tgts[0], pos);
			if(args->side == EFFECT_SIDE_PARTY)
				swayCenter[0] = INTtoFIXED(pos[0].x - 128);
			else
				swayCenter[0] = INTtoFIXED(pos[0].x - 64);
			swayCenter[1] = 0;
			drawReaper = 0;
		}
		if(frm == 49) {
			REG_BLDALPHA = EFFECT_BLEND_ADD;
			drawReaper = 1;
		}

		/* Each of the first two passes ripples harder as the reaper fades out
		   over its last eight ticks. */
		scanline = state->bg2xLines;
		ripple = 0;
		if(frm <= 23) {
			if(frm > 15) {
				ripple = (frm * 2) - 32;
				REG_BLDALPHA = EFFECT_BLEND_CANVAS(31 - frm);
			}
		} else if(frm <= 47) {
			if(frm > 31) {
				ripple = (frm * 2) - 64;
				REG_BLDALPHA = EFFECT_BLEND_CANVAS(47 - frm);
			}
		}
		if(ripple < 0)
			ripple = 0;

		/* The reaper moves by scrolling the canvas line by line, not by where
		   it is drawn. */
		i = 0;
		lineOfs = (6 - screenX) << 8;
		ang = frm * (TRIG_TURN / 32);
		for(; i != DISPLAY_HEIGHT; i++) {
			*scanline++ = lineOfs - CANVAS_WAVE_BG2X(sySin(ang) * ripple);
			ang += TRIG_TURN / 32;
		}

		/* The reaper, then from the swing its swung pose and the crescent
		   fading, each picture shown for two ticks. */
		if(drawReaper != 0) {
			/* FAKEMATCH(temporary): side, the side copied into a local for the
			 * test. */
			Sint32 side = args->side;

			if(side == EFFECT_SIDE_ENEMIES) {
				pictureColumn = 0;
				pictureRow = 0;
			} else {
				pictureColumn = 1;
				/* FAKEMATCH(dead): i is 160 here. */
				pictureRow = i >> 31;
				/* FAKEMATCH(dead): side set again, never read. */
				side = EFFECT_SIDE_ENEMIES;
			}
			if(frm < CONDEMN_SWING_TICK) {
				draw[args->side](canvas, state->sheet,
					CONDEMN_PICTURE_X(pictureColumn, 0),
					CONDEMN_PICTURE_Y(pictureRow, 0) + screenY,
					REAPER_SHEET_RAISED_WIDTH, REAPER_SHEET_RAISED_HEIGHT);
			} else {
				if(frm <= CONDEMN_SWING_TICK + 3)
					draw[args->side](canvas, state->sheet,
						CONDEMN_PICTURE_X(pictureColumn, 0),
						CONDEMN_PICTURE_Y(pictureRow, 0) + screenY,
						REAPER_SHEET_RAISED_WIDTH, REAPER_SHEET_RAISED_HEIGHT);
				EFFECT_DRAW_PICTURE(draw[args->side], canvas, state->sheet,
					REAPER_SHEET_SWUNG, CONDEMN_PICTURE_X(pictureColumn, 1),
					CONDEMN_PICTURE_Y(pictureRow, 1) + screenY);
				if(frm >= CONDEMN_SWING_TICK && frm <= CONDEMN_SWING_TICK + 1)
					syWordsFillEntry(canvas, CANVAS_BYTES,
						CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
				if(frm >= CONDEMN_SWING_TICK + 2 &&
					frm <= CONDEMN_SWING_TICK + 3)
					EFFECT_DRAW_PICTURE(draw[args->side], canvas, state->sheet,
						REAPER_SHEET_CRESCENT,
						CONDEMN_PICTURE_X(pictureColumn, 2),
						CONDEMN_PICTURE_Y(pictureRow, 2) + screenY);
				if(frm >= CONDEMN_SWING_TICK + 4 &&
					frm <= CONDEMN_SWING_TICK + 5)
					EFFECT_DRAW_PICTURE(draw[args->side], canvas,
						MapGridMetatileBuffer, RED_CRESCENT_FADE_A,
						CONDEMN_PICTURE_X(pictureColumn, 3),
						CONDEMN_PICTURE_Y(pictureRow, 3) + screenY);
				if(frm >= CONDEMN_SWING_TICK + 6 &&
					frm <= CONDEMN_SWING_TICK + 7)
					EFFECT_DRAW_PICTURE(draw[args->side], canvas,
						MapGridMetatileBuffer, RED_CRESCENT_FADE_B,
						CONDEMN_PICTURE_X(pictureColumn, 4),
						CONDEMN_PICTURE_Y(pictureRow, 4) + screenY);
				if(frm >= CONDEMN_SWING_TICK + 8 &&
					frm <= CONDEMN_SWING_TICK + 9)
					EFFECT_DRAW_PICTURE(draw[args->side], canvas,
						MapGridMetatileBuffer, RED_CRESCENT_FADE_C,
						CONDEMN_PICTURE_X(pictureColumn, 5),
						CONDEMN_PICTURE_Y(pictureRow, 5) + screenY);
				if(frm >= CONDEMN_SWING_TICK + 10 &&
					frm <= CONDEMN_SWING_TICK + 11)
					EFFECT_DRAW_PICTURE(draw[args->side], canvas,
						MapGridMetatileBuffer, RED_CRESCENT_FADE_D,
						CONDEMN_PICTURE_X(pictureColumn, 6),
						CONDEMN_PICTURE_Y(pictureRow, 6) + screenY);
			}
		}

		/* A sound as the scythe rises; on the swing the target is knocked back,
		   the screen shakes and the results begin. */
		if(frm == CONDEMN_SWING_TICK - 4)
			sySndPlayStub(SE_POWER_UP);
		if(frm == CONDEMN_SWING_TICK) {
			btMotionApplyKnockbackStub(args->tgts[0], KNOCKBACK_NUDGE);
			state->shakeFrm = 8;
			EFFECT_SHOW_DAMAGE(SE_CLATTER);
		}
		btCameraShake(8, 8);
		EFFECT_END_TICK(state);
	}

	syCloseEvent(btEffectTransferCanvas);
	syCloseEvent(btEffectArmBg2AffineHBlankDMA);
	btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED,
		Ram_HeapSlots->battleState->bg, BACKGROUND_DARKENED_LEVEL);
	EFFECT_RELEASE_BLITTERS();
	btEffectEndCanvasLayer();
}

#undef CONDEMN_TICKS
#undef CONDEMN_SWING_TICK
#undef CONDEMN_SWAY_X
#undef CONDEMN_SWAY_Y
#undef CONDEMN_PICTURES
#undef CONDEMN_PICTURE_X
#undef CONDEMN_PICTURE_Y
