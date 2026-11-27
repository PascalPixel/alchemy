/* Condemn. Part of battle_effect.c. */

#define CONDEMN_FRAMES		 132
/* The reaper's first two passes, in frames; it fades out, rippling harder, over
   the last CONDEMN_FADE_FRAMES of each. */
#define CONDEMN_PASS_FRAMES	 32
#define CONDEMN_FADE_FRAMES	 16
#define CONDEMN_SWING_FRAME	 88	 /* the reaper swings, in a white flash */
#define CONDEMN_BOUNCE_FRAME 120 /* the falling target bounces */
/* How far the reaper sways across and up and down as it circles, in pixels. */
#define CONDEMN_SWAY_X		 16
#define CONDEMN_SWAY_Y		 4

/* Where `picture`, one of the reaper's seven pictures, is drawn
   (REAPER_PICTURE_X), by the casting side's `column` and `row`. */
#define CONDEMN_PICTURE_X(column, picture) Condemn_PictureX[(column)][(picture)]
#define CONDEMN_PICTURE_Y(row, picture)	   Condemn_PictureY[(row)][(picture)]

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
	ACTOR				*target;
	/* LEFTOVER(frame): position[1], which the ROM's frame reserves and never
	 * touches. */
	SCREEN_POINT position[2];
	FIXED		 swayCenter[2];
	Sint32		 drawReaper;
	Sint32		 pictureColumn;
	Sint32		 pictureRow;
	Sint32		 frame;

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
		cells = Ram_MapGridMetatileBuffer;
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
	for(frame = 0; frame != CONDEMN_FRAMES; frame++) {
		Sint32	spin;
		Sint32	screenX;
		Sint32	screenY;
		Sint32	ripple;
		Sint32	lineOffset;
		Sint32	angle;
		Sint32	i;
		Sint32 *scanline;

		spin = frame * (TRIG_TURN / 128);
		screenX = FIXEDtoINT(swayCenter[0]) +
			FIXEDtoINT(sySin(spin) * CONDEMN_SWAY_X) + 48;
		screenY = FIXEDtoINT(swayCenter[1]) +
			FIXEDtoINT(syCos(spin) * CONDEMN_SWAY_Y) + 16;
		if(frame == CONDEMN_SWING_FRAME) {
			sySndPlayStub(SE_HEAVY_IMPACT);
		}
		/* The second pass, lower and further across. */
		if(frame == CONDEMN_PASS_FRAMES) {
			if(state->args->side == EFFECT_SIDE_PARTY) {
				swayCenter[0] = -toFIXED(32);
			} else {
				swayCenter[0] = toFIXED(72);
			}
			swayCenter[1] = toFIXED(24);
			drawReaper = 0;
		}
		if(frame == CONDEMN_PASS_FRAMES + 1) {
			REG_BLDALPHA = EFFECT_BLEND_ADD;
			drawReaper = 1;
		}
		/* The last pass, on the first target. */
		if(frame == 2 * CONDEMN_PASS_FRAMES) {
			btProjectionScaledUnit(state->args->tgts[0], position);
			if(state->args->side == EFFECT_SIDE_PARTY) {
				swayCenter[0] = INTtoFIXED(position[0].x - 128);
			} else {
				swayCenter[0] = INTtoFIXED(position[0].x - 64);
			}
			swayCenter[1] = 0;
			drawReaper = 0;
		}
		if(frame == 2 * CONDEMN_PASS_FRAMES + 1) {
			REG_BLDALPHA = EFFECT_BLEND_ADD;
			drawReaper = 1;
		}
		/* Each of the first two passes ripples harder as the reaper fades
		   out. */
		scanline = state->bg2xLines;
		ripple = 0;
		if(frame < CONDEMN_PASS_FRAMES) {
			if(frame >= CONDEMN_PASS_FRAMES - CONDEMN_FADE_FRAMES) {
				ripple =
					(frame - (CONDEMN_PASS_FRAMES - CONDEMN_FADE_FRAMES)) * 2;
				REG_BLDALPHA =
					EFFECT_BLEND_CANVAS(CONDEMN_PASS_FRAMES - 1 - frame);
			}
		} else if(frame < 2 * CONDEMN_PASS_FRAMES) {
			if(frame >= 2 * CONDEMN_PASS_FRAMES - CONDEMN_FADE_FRAMES) {
				ripple =
					(frame - (2 * CONDEMN_PASS_FRAMES - CONDEMN_FADE_FRAMES)) *
					2;
				REG_BLDALPHA =
					EFFECT_BLEND_CANVAS(2 * CONDEMN_PASS_FRAMES - 1 - frame);
			}
		}
		if(ripple < 0) {
			ripple = 0;
		}

		/* The reaper moves by scrolling the canvas line by line, not by where
		   it is drawn. */
		i = 0;
		lineOffset = (6 - screenX) << 8;
		angle = frame * (TRIG_TURN / 32);
		for(; i != DISPLAY_HEIGHT; i++) {
			*scanline++ = lineOffset - CANVAS_WAVE_BG2X(sySin(angle) * ripple);
			angle += TRIG_TURN / 32;
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
			if(frame < CONDEMN_SWING_FRAME) {
				draw[state->args->side](canvas, state->sheet,
					CONDEMN_PICTURE_X(pictureColumn, 0),
					CONDEMN_PICTURE_Y(pictureRow, 0) + screenY,
					REAPER_SHEET_RAISED_WIDTH, REAPER_SHEET_RAISED_HEIGHT);
			} else {
				if(frame <= 91) {
					draw[state->args->side](canvas, state->sheet,
						CONDEMN_PICTURE_X(pictureColumn, 0),
						CONDEMN_PICTURE_Y(pictureRow, 0) + screenY,
						REAPER_SHEET_RAISED_WIDTH, REAPER_SHEET_RAISED_HEIGHT);
				}
				EFFECT_DRAW_PICTURE(draw[state->args->side], canvas,
					state->sheet, REAPER_SHEET_SWUNG,
					CONDEMN_PICTURE_X(pictureColumn, 1),
					CONDEMN_PICTURE_Y(pictureRow, 1) + screenY);
				/* Two frames each: the white flash, the swung crescent, then
				   the crescent fading in four pictures. */
				if(frame >= 88 && frame <= 89) {
					syWordsFill(canvas, CANVAS_BYTES,
						CANVAS_FILL_WORD(CANVAS_LEVEL_BRIGHTEST));
				}
				if(frame >= 90 && frame <= 91) {
					EFFECT_DRAW_PICTURE(draw[state->args->side], canvas,
						state->sheet, REAPER_SHEET_CRESCENT,
						CONDEMN_PICTURE_X(pictureColumn, 2),
						CONDEMN_PICTURE_Y(pictureRow, 2) + screenY);
				}
				if(frame >= 92 && frame <= 93) {
					EFFECT_DRAW_PICTURE(draw[state->args->side], canvas,
						Ram_MapGridMetatileBuffer, RED_CRESCENT_FADE_A,
						CONDEMN_PICTURE_X(pictureColumn, 3),
						CONDEMN_PICTURE_Y(pictureRow, 3) + screenY);
				}
				if(frame >= 94 && frame <= 95) {
					EFFECT_DRAW_PICTURE(draw[state->args->side], canvas,
						Ram_MapGridMetatileBuffer, RED_CRESCENT_FADE_B,
						CONDEMN_PICTURE_X(pictureColumn, 4),
						CONDEMN_PICTURE_Y(pictureRow, 4) + screenY);
				}
				if(frame >= 96 && frame <= 97) {
					EFFECT_DRAW_PICTURE(draw[state->args->side], canvas,
						Ram_MapGridMetatileBuffer, RED_CRESCENT_FADE_C,
						CONDEMN_PICTURE_X(pictureColumn, 5),
						CONDEMN_PICTURE_Y(pictureRow, 5) + screenY);
				}
				if(frame >= 98 && frame <= 99) {
					EFFECT_DRAW_PICTURE(draw[state->args->side], canvas,
						Ram_MapGridMetatileBuffer, RED_CRESCENT_FADE_D,
						CONDEMN_PICTURE_X(pictureColumn, 6),
						CONDEMN_PICTURE_Y(pictureRow, 6) + screenY);
				}
			}
		}
		/* Knock the target back. */
		if(frame == CONDEMN_SWING_FRAME) {
			target = btObjectGetStub(state->args->tgts[0])->actor;
			target->velY = toFIXED(1);
			target->acceleration = toFIXED(2);
			target->speedLimit = toFIXED(2);
			target->gravity = 0;
			target->steeringFlg = 0;
			target->snapToTgt = 0;
			fdActorSetMoveTargetStub(
				target, target->x.fixed << 1, 0, target->z.fixed);
			btEffectTintBattler(
				state->args->tgts[0], TINT_KEEP, POSE_DAMAGE, TINT_KEEP, 0);
		}
		if(frame == CONDEMN_BOUNCE_FRAME) {
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

#undef CONDEMN_FRAMES
#undef CONDEMN_PASS_FRAMES
#undef CONDEMN_FADE_FRAMES
#undef CONDEMN_SWING_FRAME
#undef CONDEMN_BOUNCE_FRAME
#undef CONDEMN_SWAY_X
#undef CONDEMN_SWAY_Y
#undef CONDEMN_PICTURE_X
#undef CONDEMN_PICTURE_Y
