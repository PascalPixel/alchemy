/* Condemn. Part of battle_effect.c. */

#define CONDEMN_TICKS		132
/* The reaper's first two passes, in ticks; it fades out, rippling harder, over
   the last CONDEMN_FADE_TICKS of each. */
#define CONDEMN_PASS_TICKS	32
#define CONDEMN_FADE_TICKS	16
#define CONDEMN_SWING_TICK	88	/* the reaper swings, in a white flash */
#define CONDEMN_BOUNCE_TICK 120 /* the falling target bounces */
/* How far the reaper sways across and up and down as it circles, in pixels. */
#define CONDEMN_SWAY_X		16
#define CONDEMN_SWAY_Y		4

/* Where `picture`, one of the reaper's seven pictures, is drawn
   (REAPER_PICTURE_X), by the casting side's `column` and `row`. */
#define CONDEMN_PICTURE_X(column, picture) CondemnPictureX[(column)][(picture)]
#define CONDEMN_PICTURE_Y(row, picture)	   CondemnPictureY[(row)][(picture)]

/* A hooded reaper with a scythe sways across the screen three times, fading out
   through a ripple, the last pass over the first target; the screen flashes
   white, the reaper swings, and the red crescent its scythe leaves fades while
   the target is knocked back.

   Abilities: Condemn. */
void btEffectRunCondemn(BATTLE_EFFECT_PARM *args)
{
	void			   **slots;
	void			   **cursor;
	BATTLE_EFFECT_STATE *state;
	void				*canvas;
	Uint8				*sheet;
	void				*cells;
	BLITTER_PAIR		 draw;
	ACTOR				*tgt;
	/* LEFTOVER(frame): position[1], which the ROM's frame reserves and never
	 * touches. */
	SCREEN_POINT pos[2];
	FIXED		 swayCenter[2];
	Sint32		 drawReaper;
	Sint32		 pictureColumn;
	Sint32		 pictureRow;
	Sint32		 frm;

	slots = &HeapSlots[HEAP_SLOT_BATTLE_EFFECT];
	cursor = slots;
	state = *cursor++;
	canvas = *cursor;
	{
		/* The canvas is cleared to 0 as the transfer starts. */
		Sint32 transferValue = 0;

		state->args = args;
		btEffectStartTiledCanvas(0);
		REG_BG2PA = Q_8_8(1);
		REG_BLDALPHA = EFFECT_BLEND_ADD;

		sheet = syResourceGetTableEntry(RESOURCE_REAPER_SHEET);
		syWordsCopy(BG_PLTT, sheet, SHEET_PALETTE_BYTES);
		sheet = sheet + SHEET_PALETTE_BYTES;
		syDecompressTaggedLZ(sheet, state->sheet);
		cells = RamMapGridMetatileBuffer;
		sheet = syResourceGetTableEntry(RESOURCE_RED_CRESCENT_SHEET_A);
		sheet = sheet + SHEET_PALETTE_BYTES;
		syDecompressTaggedLZ(sheet, cells);
		EFFECT_LOAD_BLITTER(
			HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_TRANSPARENT);
		draw[0] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE);
		EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED,
			EFFECT_CLIP_BOUNDS | COMPOSITE_FLIP_X, COMPOSITE_TRANSPARENT);
		draw[1] = EFFECT_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED);
		sySetEvent(btEffectArmBg2AffineHBlankDMA, EVENT_PASS_VBLANK);
		EFFECT_START_TRANSFER(state, CANVAS_COPY_AND_CLEAR, transferValue);
	}
	drawReaper = 1;
	if(state->args->side == EFFECT_SIDE_PARTY) {
		swayCenter[0] = -toFIXED(80);
	} else {
		swayCenter[0] = toFIXED(112);
	}
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
		if(frm == CONDEMN_SWING_TICK) {
			sySndPlayStub(SE_HEAVY_IMPACT);
		}
		/* The second pass, lower and further across. */
		if(frm == CONDEMN_PASS_TICKS) {
			if(state->args->side == EFFECT_SIDE_PARTY) {
				swayCenter[0] = -toFIXED(32);
			} else {
				swayCenter[0] = toFIXED(72);
			}
			swayCenter[1] = toFIXED(24);
			drawReaper = 0;
		}
		if(frm == CONDEMN_PASS_TICKS + 1) {
			REG_BLDALPHA = EFFECT_BLEND_ADD;
			drawReaper = 1;
		}
		/* The last pass, on the first target. */
		if(frm == 2 * CONDEMN_PASS_TICKS) {
			btProjectionScaledUnit(state->args->tgts[0], pos);
			if(state->args->side == EFFECT_SIDE_PARTY) {
				swayCenter[0] = INTtoFIXED(pos[0].x - 128);
			} else {
				swayCenter[0] = INTtoFIXED(pos[0].x - 64);
			}
			swayCenter[1] = 0;
			drawReaper = 0;
		}
		if(frm == 2 * CONDEMN_PASS_TICKS + 1) {
			REG_BLDALPHA = EFFECT_BLEND_ADD;
			drawReaper = 1;
		}
		/* Each of the first two passes ripples harder as the reaper fades
		   out. */
		scanline = state->bg2xLines;
		ripple = 0;
		if(frm < CONDEMN_PASS_TICKS) {
			if(frm >= CONDEMN_PASS_TICKS - CONDEMN_FADE_TICKS) {
				ripple = (frm - (CONDEMN_PASS_TICKS - CONDEMN_FADE_TICKS)) * 2;
				REG_BLDALPHA =
					EFFECT_BLEND_CANVAS(CONDEMN_PASS_TICKS - 1 - frm);
			}
		} else if(frm < 2 * CONDEMN_PASS_TICKS) {
			if(frm >= 2 * CONDEMN_PASS_TICKS - CONDEMN_FADE_TICKS) {
				ripple =
					(frm - (2 * CONDEMN_PASS_TICKS - CONDEMN_FADE_TICKS)) * 2;
				REG_BLDALPHA =
					EFFECT_BLEND_CANVAS(2 * CONDEMN_PASS_TICKS - 1 - frm);
			}
		}
		if(ripple < 0) {
			ripple = 0;
		}

		/* The reaper moves by scrolling the canvas line by line, not by where
		   it is drawn. */
		i = 0;
		lineOfs = (6 - screenX) << 8;
		ang = frm * (TRIG_TURN / 32);
		for(; i != DISPLAY_HEIGHT; i++) {
			*scanline++ = lineOfs - CANVAS_WAVE_BG2X(sySin(ang) * ripple);
			ang += TRIG_TURN / 32;
		}
		if(drawReaper != 0) {
			if(state->args->side == EFFECT_SIDE_ENEMIES) {
				pictureColumn = 0;
				pictureRow = 0;
			} else {
				pictureColumn = 1;
				/* FAKEMATCH(dead): i is 160 here. */
				pictureRow = i >> 31;
			}
			if(frm < CONDEMN_SWING_TICK) {
				draw[state->args->side](canvas, state->sheet,
					CONDEMN_PICTURE_X(pictureColumn, 0),
					CONDEMN_PICTURE_Y(pictureRow, 0) + screenY,
					REAPER_SHEET_RAISED_WIDTH, REAPER_SHEET_RAISED_HEIGHT);
			} else {
				if(frm <= 91) {
					draw[state->args->side](canvas, state->sheet,
						CONDEMN_PICTURE_X(pictureColumn, 0),
						CONDEMN_PICTURE_Y(pictureRow, 0) + screenY,
						REAPER_SHEET_RAISED_WIDTH, REAPER_SHEET_RAISED_HEIGHT);
				}
				EFFECT_DRAW_PICTURE(draw[state->args->side], canvas,
					state->sheet, REAPER_SHEET_SWUNG,
					CONDEMN_PICTURE_X(pictureColumn, 1),
					CONDEMN_PICTURE_Y(pictureRow, 1) + screenY);
				/* Two ticks each: the white flash, the swung crescent, then
				   the crescent fading in four pictures. */
				if(frm >= 88 && frm <= 89) {
					syWordsFill(canvas, CANVAS_BYTES,
						CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
				}
				if(frm >= 90 && frm <= 91) {
					EFFECT_DRAW_PICTURE(draw[state->args->side], canvas,
						state->sheet, REAPER_SHEET_CRESCENT,
						CONDEMN_PICTURE_X(pictureColumn, 2),
						CONDEMN_PICTURE_Y(pictureRow, 2) + screenY);
				}
				if(frm >= 92 && frm <= 93) {
					EFFECT_DRAW_PICTURE(draw[state->args->side], canvas,
						RamMapGridMetatileBuffer, RED_CRESCENT_FADE_A,
						CONDEMN_PICTURE_X(pictureColumn, 3),
						CONDEMN_PICTURE_Y(pictureRow, 3) + screenY);
				}
				if(frm >= 94 && frm <= 95) {
					EFFECT_DRAW_PICTURE(draw[state->args->side], canvas,
						RamMapGridMetatileBuffer, RED_CRESCENT_FADE_B,
						CONDEMN_PICTURE_X(pictureColumn, 4),
						CONDEMN_PICTURE_Y(pictureRow, 4) + screenY);
				}
				if(frm >= 96 && frm <= 97) {
					EFFECT_DRAW_PICTURE(draw[state->args->side], canvas,
						RamMapGridMetatileBuffer, RED_CRESCENT_FADE_C,
						CONDEMN_PICTURE_X(pictureColumn, 5),
						CONDEMN_PICTURE_Y(pictureRow, 5) + screenY);
				}
				if(frm >= 98 && frm <= 99) {
					EFFECT_DRAW_PICTURE(draw[state->args->side], canvas,
						RamMapGridMetatileBuffer, RED_CRESCENT_FADE_D,
						CONDEMN_PICTURE_X(pictureColumn, 6),
						CONDEMN_PICTURE_Y(pictureRow, 6) + screenY);
				}
			}
		}
		/* Knock the target back. */
		if(frm == CONDEMN_SWING_TICK) {
			tgt = btObjectGetStub(state->args->tgts[0])->actor;
			tgt->velY = toFIXED(1);
			tgt->acceleration = toFIXED(2);
			tgt->speedLimit = toFIXED(2);
			tgt->gravity = 0;
			tgt->steeringFlg = 0;
			tgt->snapToTgt = 0;
			fdActorSetMoveTargetStub(tgt, tgt->x.fixed << 1, 0, tgt->z.fixed);
			btEffectTintBattler(
				state->args->tgts[0], TINT_KEEP, POSE_DAMAGE, TINT_KEEP, 0);
		}
		if(frm == CONDEMN_BOUNCE_TICK) {
			btObjectGetStub(state->args->tgts[0])->actor->gravity =
				BATTLE_UNIT_GRAVITY;
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	/* Finish: the events, then the battle background reloaded, darkened as a
	   canvas leaves it, which repairs nothing this effect changed. XXX (no
	   evidence): why Condemn reloads the background; the reload puts back the
	   picture already there, so nothing the game shows says why. */
	syCloseEvent(btEffectTransferCanvas);
	syCloseEvent(btEffectArmBg2AffineHBlankDMA);
	btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, BattleStateWork->bg,
		BACKGROUND_DARKENED_LEVEL);
	EFFECT_RELEASE_BLITTERS();
	btEffectEndCanvasLayer();
}

#undef CONDEMN_TICKS
#undef CONDEMN_PASS_TICKS
#undef CONDEMN_FADE_TICKS
#undef CONDEMN_SWING_TICK
#undef CONDEMN_BOUNCE_TICK
#undef CONDEMN_SWAY_X
#undef CONDEMN_SWAY_Y
#undef CONDEMN_PICTURE_X
#undef CONDEMN_PICTURE_Y
