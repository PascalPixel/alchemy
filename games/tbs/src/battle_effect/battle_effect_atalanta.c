/* @file */
/* The Atalanta summon. */
/* Part of battle_effect.c. */

#define ATALANTA_FALL_FRAMES 88 /* frames of the shafts' fall */

#define ATALANTA_SHAFTS			16 /* shafts of light that gather over the field */
#define ATALANTA_TRAIL_MOTES	24 /* motes in each gathering shaft's trail */
#define ATALANTA_FALLING_SHAFTS 24 /* shafts that rain down on the targets */
/* The particles the falling shafts are set up in, more than fall. */
#define ATALANTA_FALLING_SHAFT_SLOTS 32

/* The gathering shafts' timing: the first one's trail gathers from frame */
/* ATALANTA_SHAFT_FIRST_FRAME and each next one ATALANTA_SHAFT_INTERVAL frames
 */
/* later; a trail gathers for ATALANTA_TRAIL_FRAMES, the shaft flashes in for */
/* ATALANTA_FLASH_FRAMES, one a picture, then flies off. */
#define ATALANTA_SHAFT_FIRST_FRAME 64
#define ATALANTA_SHAFT_INTERVAL	   8
#define ATALANTA_TRAIL_FRAMES	   16
#define ATALANTA_FLASH_FRAMES	   5

/* A landed shaft's burst of light: the ember streak sprite sheet's six */
/* pictures, four frames each, the shaft still drawn over its first twelve */
/* frames. */
#define ATALANTA_BURST_PICTURES		  6
#define ATALANTA_BURST_PICTURE_FRAMES 4
#define ATALANTA_BURST_SHAFT_FRAMES	  12

/* Atalanta is three rows of three 32x32 sprites. */
#define ATALANTA_SPRITE_ROWS 3
#define ATALANTA_SPRITES	 (ATALANTA_SPRITE_ROWS * ATALANTA_SPRITE_ROWS)

/* The trail particles' random rotations, after the first 512 particles of */
/* PARTICLES, which hold the 384 trail particles. */
#define ATALANTA_MATRICES (Ram_MapGridCellBuffer + 512 * sizeof(PARTICLE))

/* How far below its projected point a trail mote is drawn: the Japanese */
/* release draws the trails 16 pixels higher than the later releases. */
#if !TBS_PATCH_1
#define ATALANTA_TRAIL_DROP 0
#else
#define ATALANTA_TRAIL_DROP 16
#endif

/* The pictures in the slash sprite sheet: five flash pictures, then the shaft
 */
/* in three parts, each in its own size. */
#define ATALANTA_SHAFT_FLASH_A_WIDTH		 14
#define ATALANTA_SHAFT_FLASH_A_HEIGHT		 28
#define ATALANTA_SHAFT_FLASH_B_WIDTH		 23
#define ATALANTA_SHAFT_FLASH_B_HEIGHT		 44
#define ATALANTA_SHAFT_FLASH_C_TOP_WIDTH	 20
#define ATALANTA_SHAFT_FLASH_C_TOP_HEIGHT	 30
#define ATALANTA_SHAFT_FLASH_C_BOTTOM_WIDTH	 22
#define ATALANTA_SHAFT_FLASH_C_BOTTOM_HEIGHT 33
#define ATALANTA_SHAFT_FLASH_D_TOP_WIDTH	 18
#define ATALANTA_SHAFT_FLASH_D_TOP_HEIGHT	 27
#define ATALANTA_SHAFT_FLASH_D_MIDDLE_WIDTH	 22
#define ATALANTA_SHAFT_FLASH_D_MIDDLE_HEIGHT 22
#define ATALANTA_SHAFT_FLASH_D_BOTTOM_WIDTH	 19
#define ATALANTA_SHAFT_FLASH_D_BOTTOM_HEIGHT 28
#define ATALANTA_SHAFT_FLASH_E_TOP_WIDTH	 16
#define ATALANTA_SHAFT_FLASH_E_TOP_HEIGHT	 23
#define ATALANTA_SHAFT_FLASH_E_MIDDLE_WIDTH	 23
#define ATALANTA_SHAFT_FLASH_E_MIDDLE_HEIGHT 28
#define ATALANTA_SHAFT_FLASH_E_BOTTOM_WIDTH	 20
#define ATALANTA_SHAFT_FLASH_E_BOTTOM_HEIGHT 28
#define ATALANTA_SHAFT_TOP_WIDTH			 16
#define ATALANTA_SHAFT_TOP_HEIGHT			 21
#define ATALANTA_SHAFT_MIDDLE_WIDTH			 29
#define ATALANTA_SHAFT_MIDDLE_HEIGHT		 35
#define ATALANTA_SHAFT_BOTTOM_WIDTH			 21
#define ATALANTA_SHAFT_BOTTOM_HEIGHT		 24
/* Where each picture begins in the slash sprite sheet, in bytes. */
typedef enum SLASH_SHEET_PICTURE {
	ATALANTA_SHAFT_FLASH_A = 0,
	ATALANTA_SHAFT_FLASH_B =
		ATALANTA_SHAFT_FLASH_A + PICTURE_BYTES(ATALANTA_SHAFT_FLASH_A),
	ATALANTA_SHAFT_FLASH_C_TOP =
		ATALANTA_SHAFT_FLASH_B + PICTURE_BYTES(ATALANTA_SHAFT_FLASH_B),
	ATALANTA_SHAFT_FLASH_C_BOTTOM =
		ATALANTA_SHAFT_FLASH_C_TOP + PICTURE_BYTES(ATALANTA_SHAFT_FLASH_C_TOP),
	ATALANTA_SHAFT_FLASH_D_TOP = ATALANTA_SHAFT_FLASH_C_BOTTOM +
								 PICTURE_BYTES(ATALANTA_SHAFT_FLASH_C_BOTTOM),
	ATALANTA_SHAFT_FLASH_D_MIDDLE =
		ATALANTA_SHAFT_FLASH_D_TOP + PICTURE_BYTES(ATALANTA_SHAFT_FLASH_D_TOP),
	ATALANTA_SHAFT_FLASH_D_BOTTOM =
		ATALANTA_SHAFT_FLASH_D_MIDDLE +
		PICTURE_BYTES(ATALANTA_SHAFT_FLASH_D_MIDDLE),
	ATALANTA_SHAFT_FLASH_E_TOP = ATALANTA_SHAFT_FLASH_D_BOTTOM +
								 PICTURE_BYTES(ATALANTA_SHAFT_FLASH_D_BOTTOM),
	ATALANTA_SHAFT_FLASH_E_MIDDLE =
		ATALANTA_SHAFT_FLASH_E_TOP + PICTURE_BYTES(ATALANTA_SHAFT_FLASH_E_TOP),
	ATALANTA_SHAFT_FLASH_E_BOTTOM =
		ATALANTA_SHAFT_FLASH_E_MIDDLE +
		PICTURE_BYTES(ATALANTA_SHAFT_FLASH_E_MIDDLE),
	ATALANTA_SHAFT_TOP = ATALANTA_SHAFT_FLASH_E_BOTTOM +
						 PICTURE_BYTES(ATALANTA_SHAFT_FLASH_E_BOTTOM),
	ATALANTA_SHAFT_MIDDLE =
		ATALANTA_SHAFT_TOP + PICTURE_BYTES(ATALANTA_SHAFT_TOP),
	ATALANTA_SHAFT_BOTTOM =
		ATALANTA_SHAFT_MIDDLE + PICTURE_BYTES(ATALANTA_SHAFT_MIDDLE)
} SLASH_SHEET_PICTURE;

/* Under a violet sky Atalanta flies across the screen while sixteen shafts of
 */
/* light gather trails of motes, flash and fly off; then 24 shafts rain down */
/* diagonally on the targets and burst in light, hitting each target as they */
/* land. */
/* */
/* Abilities: Atalanta. */
void btEffectRunAtalanta(BATTLE_EFFECT_PARM *args) {
	s32 point[3];
	SCR_POINT screen;
	SPR_PLACEMENT position;
	SCR_POINT target;
	BLITTER_PAIR draw;
	BATTLE_BACKGROUND_STATE *view;
	void **slots;
	void *canvas;
	BATTLE_EFFECT_STATE *state;
	s8 *path;
	s32 i;
	s32 j;
	s32 frame;
	u8 *sheet;
	s32 scrollX;
	s32 pathX;
	s32 pathY;
	s32 row;
	s32 column;

	slots = &HeapSlots[HEAP_SLOT_BATTLE_BACKGROUND];
	view = slots[0];
	canvas = HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_BACKGROUND,
							HEAP_SLOT_BATTLE_CANVAS);
	state = HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_BACKGROUND,
						   HEAP_SLOT_BATTLE_EFFECT);
	sheet = HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_BACKGROUND,
						   HEAP_SLOT_BATTLE_SHEET);
	scrollX = BgScrollWork[1].x;
	state->args = args;
	/* The screen wipes to black before Atalanta appears. */
	EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
	btEffectConfigureEffectDisp();
	BG_PLTT[0] = 0;
	BG_PLTT[1] = 0;
	state->transferMode = CANVAS_COPY;
	sySetEvent(btEffectTransferCanvas, TASK_PASS_VBLANK);
	btEffectWipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
	btEffectShowLivingTargets(state->args);
	btEffectCreateSprs(ATALANTA_SPRITES, SPRITE_ATALANTA, 1);
	btResourceLoadAndDecompress(RESOURCE_SLASH_SHEET, state->sheet, 1, 1);
	syWordsCopy(BG_PLTT, syResourceGetTableEntry(RESOURCE_LIME_PALETTE),
				SHEET_PALETTE_BYTES);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	path = syResourceGetTableEntry(RESOURCE_ATALANTA_PATH);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS,
						COMPOSITE_LIGHTEN);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS,
						COMPOSITE_ADDITIVE);
	draw[0] = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE];
	draw[1] = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED];
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
	sySynch(1);
	/* The violet sky scrolls. */
	btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, RESOURCE_VIOLET_SKY_BACKDROP,
						 0);
	state->scrollTmr = 0;
	state->scrollInterval = 4;
	state->scrollStepX = -1;
	state->scrollStepY = 0;
	sySetEvent(btEffectAdvanceScrollOnInterval, TASK_PASS_VBLANK);
	view->busy = 1;
	btEffectWipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_CLEAR);
	EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
	pathX = 0;
	pathY = 0;
	/* Each shaft starts at a random spot with its trail of motes. */
	for(i = 0; i != ATALANTA_SHAFTS; i++) {
		PARTICLE *shaft = &state->particles[i];

		shaft->x = INTtoFIXED(syRandom() % 96 + 12);
		shaft->y = INTtoFIXED((syRandom() & 63) + 32);
		shaft->velX = 0;
		shaft->velY = 0;
		shaft->variant = 0;
		for(j = 0; j != ATALANTA_TRAIL_MOTES; j++) {
			PARTICLES[i * ATALANTA_TRAIL_MOTES + j].x = (syRandom() & 15) + 48;
			syInitMatrix();
			syRotZ(syRandom() & TRIG_ANGLE_MASK);
			syRotX(syRandom() & TRIG_ANGLE_MASK);
			syRotY(syRandom() & TRIG_ANGLE_MASK);
			syGetMatrix(
				(FIXED *)(ATALANTA_MATRICES + (i * ATALANTA_TRAIL_MOTES + j) *
												  sizeof(CurrentMatrix)));
		}
	}
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS |
				 BGCNT_SCREEN_BASE(7);
	/* The gathering (A or B ends it). */
	for(frame = 0; frame != 220 && !(KeysRepeat & (KEY_A | KEY_B)); frame++) {
		/* Atalanta flies along her path. */
		if(frame <= 209) {
			if(frame == 0) {
				pathX = (path[0] << 8) + (u8)path[1];
				pathY = (path[2] << 8) + (u8)path[3];
				path += 4;
			}
			else {
				pathX += path[0];
				pathY += path[1];
				path += 2;
			}
			position.groundHeight = 0;
			position.height = toFIXED(255);
			for(row = 0; row != ATALANTA_SPRITE_ROWS; row++) {
				for(column = 0; column != ATALANTA_SPRITE_ROWS; column++) {
					position.x = INTtoFIXED(pathX + 80 + column * 32);
					position.y = INTtoFIXED(64 - pathY + row * 32);
					fdRenderApplyProjectedPlacementStub(
						state->sprs[row * ATALANTA_SPRITE_ROWS + column],
						&position, &Atalanta_UnitScale, 0);
				}
			}
		}
		point[1] = 0;
		point[2] = 0;
		/* The battle background starts to darken. */
		if(frame == 48) {
			state->fadeFrm = BACKGROUND_DARKEN_FRAMES;
			state->fadeStep = 0;
		}
		/* The shafts appear one after another. */
		for(i = 0; i != ATALANTA_SHAFTS; i++) {
			s32 start =
				i * ATALANTA_SHAFT_INTERVAL + ATALANTA_SHAFT_FIRST_FRAME;

			if(frame >= start) {
				PARTICLE *shaft = &state->particles[i];
				s32 x = FIXEDtoINT(shaft->x);
				s32 y = FIXEDtoINT(shaft->y);

				if(frame ==
				   start + ATALANTA_TRAIL_FRAMES + ATALANTA_FLASH_FRAMES - 1)
					sySndPlayStub(SE_POWER_UP);
				if(frame >=
				   start + ATALANTA_TRAIL_FRAMES + ATALANTA_FLASH_FRAMES) {
					shaft->x += shaft->velX;
					shaft->y += shaft->velY;
					shaft->velX -= toFIXED(1);
					shaft->velY += toFIXED(2);
					EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
										ATALANTA_SHAFT_TOP, x + 4, y - 40);
					EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
										ATALANTA_SHAFT_MIDDLE, x - 16, y - 19);
					EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
										ATALANTA_SHAFT_BOTTOM, x - 20, y + 16);
				}
				else if(frame >= start + ATALANTA_TRAIL_FRAMES) {
					switch(frame - start - ATALANTA_TRAIL_FRAMES) {
					case 0:
						EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
											ATALANTA_SHAFT_FLASH_A, x - 7,
											y - 14);
						break;
					case 1:
						EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
											ATALANTA_SHAFT_FLASH_B, x - 11,
											y - 22);
						break;
					case 2:
						EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
											ATALANTA_SHAFT_FLASH_C_TOP, x - 4,
											y - 31);
						EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
											ATALANTA_SHAFT_FLASH_C_BOTTOM,
											x - 16, y - 1);
						break;
					case 3:
						EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
											ATALANTA_SHAFT_FLASH_D_TOP, x + 1,
											y - 38);
						EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
											ATALANTA_SHAFT_FLASH_D_MIDDLE,
											x - 11, y - 11);
						EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
											ATALANTA_SHAFT_FLASH_D_BOTTOM,
											x - 19, y + 11);
						break;
					case 4:
						EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
											ATALANTA_SHAFT_FLASH_E_TOP, x + 4,
											y - 40);
						EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
											ATALANTA_SHAFT_FLASH_E_MIDDLE,
											x - 10, y - 17);
						EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
											ATALANTA_SHAFT_FLASH_E_BOTTOM,
											x - 20, y + 11);
						break;
					}
				}
				else {
					for(j = 0; j != ATALANTA_TRAIL_MOTES; j++) {
						PARTICLE *trail =
							&PARTICLES[i * ATALANTA_TRAIL_MOTES + j];
						s32 size = 5;

						if(trail->x > 0) {
							syLoadMatrix(
								(const FIXED *)(ATALANTA_MATRICES +
												(i * ATALANTA_TRAIL_MOTES + j) *
													sizeof(CurrentMatrix)));
							point[0] = trail->x;
							btProjectionPoint(point, &screen);
							screen.x = (screen.x >> 1) + x;
							screen.y = screen.y + y + ATALANTA_TRAIL_DROP;
							trail->x -= 4;
							EFFECT_DRAW_MOTE(draw[1], canvas, sheet, screen.x,
											 screen.y, size);
						}
					}
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	syCloseEvent(btEffectAdvanceScrollOnInterval);
	view->busy = 0;
	BgScrollWork[1].x = scrollX;
	btEffectSetupBlendedDisp();
	for(i = 0; i != ATALANTA_SPRITES; i++)
		fdSprDestroyStub(state->sprs[i]);
	REG_BG2PA = Q_8_8(0.5);
	REG_DISPCNT = EFFECT_DISPCNT;
	btResourceLoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET,
								Ram_MapGridCellBuffer, 1, 0);
	/* The shafts are dealt round the targets as if to a full enemy group of
	 * six; */
	/* a shaft dealt to a missing target falls at random across the field. */
	for(i = 0; i != ATALANTA_FALLING_SHAFT_SLOTS; i++) {
		PARTICLE *shaft = &state->particles[i];
		s32 member = i % ENEMY_BATTLE_SIZE;

		if(member < state->args->tgtCnt) {
			btProjectionScaledUnit(state->args->tgts[member], &target);
			shaft->y = -((syRandom() & 31) + 40);
			shaft->x = target.x / 2 + (80 - shaft->y) / 2;
		}
		else {
			shaft->x = (syRandom() & 63) + 80;
			shaft->y = -((syRandom() & 31) + 40);
		}
		shaft->variant = -1;
	}
	/* The shafts rain down on the targets. */
	for(frame = 0; frame != ATALANTA_FALL_FRAMES; frame++) {
		for(i = 0; i != ATALANTA_FALLING_SHAFTS; i++) {
			PARTICLE *shaft = &state->particles[i];

			if(frame >= i * 2 || frame > 40) {
				/* A landed shaft bursts in light. */
				if(shaft->variant >= 0) {
					if(shaft->variant < ATALANTA_BURST_PICTURES *
											ATALANTA_BURST_PICTURE_FRAMES) {
						s32 picture =
							shaft->variant / ATALANTA_BURST_PICTURE_FRAMES;
						u32 width;

						/* UB: width is assigned in one argument and read in
						 * another. */
#ifdef UBFIX
						width = BattleEffect_PuffArcPictureWidths[picture];
						draw[i & 1](
							canvas,
							Ram_MapGridCellBuffer +
								BattleEffect_PuffArcPictures[picture],
							shaft->x - (width >> 1) - 8,
							shaft->y +
								BattleEffect_PuffArcPictureBiasY[picture] - 40,
							width, BattleEffect_PuffArcPictureHeights[picture]);
#else
						draw[i & 1](
							canvas,
							Ram_MapGridCellBuffer +
								BattleEffect_PuffArcPictures[picture],
							shaft->x -
								((width = BattleEffect_PuffArcPictureWidths
									  [picture]) >>
								 1) -
								8,
							shaft->y +
								BattleEffect_PuffArcPictureBiasY[picture] - 40,
							width, BattleEffect_PuffArcPictureHeights[picture]);
#endif
						if(shaft->variant < ATALANTA_BURST_SHAFT_FRAMES) {
							EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
												ATALANTA_SHAFT_TOP,
												shaft->x + 4, shaft->y - 40);
							EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
												ATALANTA_SHAFT_MIDDLE,
												shaft->x - 16, shaft->y - 19);
						}
					}
					shaft->variant++;
				}
				else {
					s32 height = 24;

					if(shaft->y > 56)
						height = height - shaft->y + 56;
					EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
										ATALANTA_SHAFT_TOP, shaft->x + 4,
										shaft->y - 40);
					EFFECT_DRAW_PICTURE(draw[0], canvas, state->sheet,
										ATALANTA_SHAFT_MIDDLE, shaft->x - 16,
										shaft->y - 19);
					if(height > 0)
						draw[0](canvas, state->sheet + ATALANTA_SHAFT_BOTTOM,
								shaft->x - 20, shaft->y + 16,
								ATALANTA_SHAFT_BOTTOM_WIDTH, height);
					shaft->x -= 6;
					shaft->y += 12;
					if(shaft->y > 79) {
						s32 member;

						shaft->variant = 0;
						state->shakeFrm = 2;
						sySndPlayStub(SE_HEAVY_IMPACT);
						member = i % ENEMY_BATTLE_SIZE;
						if(member < state->args->tgtCnt) {
							btEffectTintBattler(state->args->tgts[member],
												SPRITE_COLOR_WHITE, POSE_DAMAGE,
												member, 8);
							btMotionApplyKnockbackStub(
								state->args->tgts[member], KNOCKBACK_HOP);
						}
					}
				}
			}
		}
		btCameraShake(4, 8);
		EFFECT_END_FRAME(state);
	}
	EFFECT_FINISH_CANVAS();
}

#undef ATALANTA_FALL_FRAMES
#undef ATALANTA_SHAFTS
#undef ATALANTA_TRAIL_MOTES
#undef ATALANTA_FALLING_SHAFTS
#undef ATALANTA_SPRITE_ROWS
#undef ATALANTA_SPRITES
#undef ATALANTA_TRAIL_DROP
#undef ATALANTA_MATRICES
#undef ATALANTA_FALLING_SHAFT_SLOTS
#undef ATALANTA_SHAFT_FIRST_FRAME
#undef ATALANTA_SHAFT_INTERVAL
#undef ATALANTA_TRAIL_FRAMES
#undef ATALANTA_FLASH_FRAMES
#undef ATALANTA_BURST_PICTURES
#undef ATALANTA_BURST_PICTURE_FRAMES
#undef ATALANTA_BURST_SHAFT_FRAMES
