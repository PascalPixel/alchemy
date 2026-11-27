// The Atalanta summon. Part of battle_effect.c.

#define ATALANTA_FALL_TICKS                  88 // ticks of the shafts' fall

#define ATALANTA_SHAFTS                      16 // shafts of light that gather over the field
#define ATALANTA_TRAIL_MOTES                 24 // motes in each gathering shaft's trail
#define ATALANTA_FALLING_SHAFTS              24 // shafts that rain down on the targets
// The particles the falling shafts are set up in, more than fall.
#define ATALANTA_FALLING_SHAFT_SLOTS         32

// The gathering shafts' timing: the first one's trail gathers from tick ATALANTA_SHAFT_FIRST_TICK
// and each next one ATALANTA_SHAFT_INTERVAL ticks later; a trail gathers for ATALANTA_TRAIL_TICKS,
// the shaft flashes in for ATALANTA_FLASH_TICKS, one a picture, then flies off.
#define ATALANTA_SHAFT_FIRST_TICK            64
#define ATALANTA_SHAFT_INTERVAL              8
#define ATALANTA_TRAIL_TICKS                 16
#define ATALANTA_FLASH_TICKS                 5

// A landed shaft's burst of light: the ember streak sprite sheet's six pictures, four ticks each,
// the shaft still drawn over its first twelve ticks.
#define ATALANTA_BURST_PICTURES              6
#define ATALANTA_BURST_PICTURE_TICKS         4
#define ATALANTA_BURST_SHAFT_TICKS           12

// Atalanta is three rows of three 32x32 sprites.
#define ATALANTA_SPRITE_ROWS                 3
#define ATALANTA_SPRITES                     (ATALANTA_SPRITE_ROWS * ATALANTA_SPRITE_ROWS)

// The trail particles' random rotations, after the first 512 particles of PARTICLES, which hold the
// 384 trail particles.
#define ATALANTA_MATRICES                    (RamMapGridMetatileBuffer + 512 * sizeof(PARTICLE))

// How far below its projected point a trail mote is drawn: the Japanese release draws the trails 16
// pixels higher than the later releases.
#define ATALANTA_TRAIL_DROP                  WHEN(ATALANTA_TRAILS_LOWER, 16, 0)

// The pictures in the slash sprite sheet: five flash pictures, then the shaft in three parts, each
// in its own size.
#define ATALANTA_SHAFT_FLASH_A_WIDTH         14
#define ATALANTA_SHAFT_FLASH_A_HEIGHT        28
#define ATALANTA_SHAFT_FLASH_B_WIDTH         23
#define ATALANTA_SHAFT_FLASH_B_HEIGHT        44
#define ATALANTA_SHAFT_FLASH_C_TOP_WIDTH     20
#define ATALANTA_SHAFT_FLASH_C_TOP_HEIGHT    30
#define ATALANTA_SHAFT_FLASH_C_BOTTOM_WIDTH  22
#define ATALANTA_SHAFT_FLASH_C_BOTTOM_HEIGHT 33
#define ATALANTA_SHAFT_FLASH_D_TOP_WIDTH     18
#define ATALANTA_SHAFT_FLASH_D_TOP_HEIGHT    27
#define ATALANTA_SHAFT_FLASH_D_MIDDLE_WIDTH  22
#define ATALANTA_SHAFT_FLASH_D_MIDDLE_HEIGHT 22
#define ATALANTA_SHAFT_FLASH_D_BOTTOM_WIDTH  19
#define ATALANTA_SHAFT_FLASH_D_BOTTOM_HEIGHT 28
#define ATALANTA_SHAFT_FLASH_E_TOP_WIDTH     16
#define ATALANTA_SHAFT_FLASH_E_TOP_HEIGHT    23
#define ATALANTA_SHAFT_FLASH_E_MIDDLE_WIDTH  23
#define ATALANTA_SHAFT_FLASH_E_MIDDLE_HEIGHT 28
#define ATALANTA_SHAFT_FLASH_E_BOTTOM_WIDTH  20
#define ATALANTA_SHAFT_FLASH_E_BOTTOM_HEIGHT 28
#define ATALANTA_SHAFT_TOP_WIDTH             16
#define ATALANTA_SHAFT_TOP_HEIGHT            21
#define ATALANTA_SHAFT_MIDDLE_WIDTH          29
#define ATALANTA_SHAFT_MIDDLE_HEIGHT         35
#define ATALANTA_SHAFT_BOTTOM_WIDTH          21
#define ATALANTA_SHAFT_BOTTOM_HEIGHT         24

// Where each picture begins in the slash sprite sheet, in bytes.
typedef enum SLASH_SHEET_PICTURE
{
	ATALANTA_SHAFT_FLASH_A     = 0,
	ATALANTA_SHAFT_FLASH_B     = ATALANTA_SHAFT_FLASH_A + PICTURE_BYTES(ATALANTA_SHAFT_FLASH_A),
	ATALANTA_SHAFT_FLASH_C_TOP = ATALANTA_SHAFT_FLASH_B + PICTURE_BYTES(ATALANTA_SHAFT_FLASH_B),
	ATALANTA_SHAFT_FLASH_C_BOTTOM =
		ATALANTA_SHAFT_FLASH_C_TOP + PICTURE_BYTES(ATALANTA_SHAFT_FLASH_C_TOP),
	ATALANTA_SHAFT_FLASH_D_TOP =
		ATALANTA_SHAFT_FLASH_C_BOTTOM + PICTURE_BYTES(ATALANTA_SHAFT_FLASH_C_BOTTOM),
	ATALANTA_SHAFT_FLASH_D_MIDDLE =
		ATALANTA_SHAFT_FLASH_D_TOP + PICTURE_BYTES(ATALANTA_SHAFT_FLASH_D_TOP),
	ATALANTA_SHAFT_FLASH_D_BOTTOM =
		ATALANTA_SHAFT_FLASH_D_MIDDLE + PICTURE_BYTES(ATALANTA_SHAFT_FLASH_D_MIDDLE),
	ATALANTA_SHAFT_FLASH_E_TOP =
		ATALANTA_SHAFT_FLASH_D_BOTTOM + PICTURE_BYTES(ATALANTA_SHAFT_FLASH_D_BOTTOM),
	ATALANTA_SHAFT_FLASH_E_MIDDLE =
		ATALANTA_SHAFT_FLASH_E_TOP + PICTURE_BYTES(ATALANTA_SHAFT_FLASH_E_TOP),
	ATALANTA_SHAFT_FLASH_E_BOTTOM =
		ATALANTA_SHAFT_FLASH_E_MIDDLE + PICTURE_BYTES(ATALANTA_SHAFT_FLASH_E_MIDDLE),
	ATALANTA_SHAFT_TOP =
		ATALANTA_SHAFT_FLASH_E_BOTTOM + PICTURE_BYTES(ATALANTA_SHAFT_FLASH_E_BOTTOM),
	ATALANTA_SHAFT_MIDDLE = ATALANTA_SHAFT_TOP + PICTURE_BYTES(ATALANTA_SHAFT_TOP),
	ATALANTA_SHAFT_BOTTOM = ATALANTA_SHAFT_MIDDLE + PICTURE_BYTES(ATALANTA_SHAFT_MIDDLE)
} SLASH_SHEET_PICTURE;

// Under a violet sky Atalanta flies across the screen while sixteen shafts of light gather trails
// of motes, flash and fly off; then 24 shafts rain down diagonally on the targets and burst in
// light, hitting each target as they land.
//
// Abilities: Atalanta.
void btEffectRunAtalanta(BATTLE_EFFECT_PARM *args)
{
	Sint32                   point[3];
	SCREEN_POINT             screen;
	SPR_PLACEMENT            pos;
	SCREEN_POINT             target;
	BLITTER_PAIR             draw;
	BATTLE_BACKGROUND_STATE *view;
	void                   **slots;
	void                    *canvas;
	BATTLE_EFFECT_STATE     *state;
	Sint8                   *path;
	Sint32                   i;
	Sint32                   j;
	Sint32                   frame;
	Uint8                   *sheet;
	Sint32                   scrollX;
	Sint32                   pathX;
	Sint32                   pathY;
	Sint32                   row;
	Sint32                   column;

	slots       = &HeapSlots[HEAP_SLOT_BATTLE_BACKGROUND];
	view        = slots[0];
	canvas      = HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_BACKGROUND, HEAP_SLOT_BATTLE_CANVAS);
	state       = HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_BACKGROUND, HEAP_SLOT_BATTLE_EFFECT);
	sheet       = HEAP_SLOT_FROM(slots, HEAP_SLOT_BATTLE_BACKGROUND, HEAP_SLOT_BATTLE_SHEET);
	scrollX     = BgScrollWork[1].x;
	state->args = args;
	// The screen wipes to black before Atalanta appears.
	EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
	btEffectConfigureEffectDisplay();
	BG_PLTT[0]          = 0;
	BG_PLTT[1]          = 0;
	state->transferMode = CANVAS_COPY;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	btEffectWipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
	btEffectShowLivingTargets(state->args);
	btEffectCreateSprites(ATALANTA_SPRITES, SPRITE_ATALANTA, 1);
	btResourceLoadAndDecompress(RESOURCE_SLASH_SHEET, state->sheet, 1, 1);
	syWordsCopy(BG_PLTT, syResourceGetTableEntry(RESOURCE_LIME_PALETTE), SHEET_PALETTE_BYTES);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	path = syResourceGetTableEntry(RESOURCE_ATALANTA_PATH);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, COMPOSITE_LIGHTEN);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, COMPOSITE_ADDITIVE);
	draw[0]                = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE];
	draw[1]                = EFFECT_BLITTERS[HEAP_SLOT_RAM_CODE_PAIRED];
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
	sySynch(1);
	// The violet sky scrolls.
	btBackgroundLoadStub(BATTLE_BACKGROUND_FIXED, RESOURCE_VIOLET_SKY_BACKDROP, 0);
	state->scrollTimer    = 0;
	state->scrollInterval = 4;
	state->scrollStepX    = -1;
	state->scrollStepY    = 0;
	sySetEvent(btEffectAdvanceScrollOnInterval, EVENT_PASS_VBLANK);
	view->busy = 1;
	btEffectWipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_CLEAR);
	EFFECT_SHOW_WIDE_CANVAS(BLEND_WEIGHT_MAX);
	pathX = 0;
	pathY = 0;
	// Each shaft starts at a random spot with its trail of motes.
	for (i = 0; i != ATALANTA_SHAFTS; i++)
	{
		PARTICLE *shaft  = &state->particles[i];

		shaft->x         = INTtoFIXED(syRandom() % 96 + 12);
		shaft->y         = INTtoFIXED((syRandom() & 63) + 32);
		shaft->velocityX = 0;
		shaft->velocityY = 0;
		shaft->variant   = 0;
		for (j = 0; j != ATALANTA_TRAIL_MOTES; j++)
		{
			PARTICLES[i * ATALANTA_TRAIL_MOTES + j].x = (syRandom() & 15) + 48;
			syInitMatrix();
			syRotZ(syRandom() & TRIG_ANGLE_MASK);
			syRotX(syRandom() & TRIG_ANGLE_MASK);
			syRotY(syRandom() & TRIG_ANGLE_MASK);
			syGetMatrix((FIXED *)(ATALANTA_MATRICES
			                      + (i * ATALANTA_TRAIL_MOTES + j) * sizeof(CurrentMatrix)));
		}
	}
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);
	// The gathering (A or B ends it).
	for (frame = 0; frame != 220 && !(PerRepeat & (KEY_A | KEY_B)); frame++)
	{
		// Atalanta flies along her path.
		if (frame <= 209)
		{
			if (frame == 0)
			{
				pathX  = (path[0] << 8) + (Uint8)path[1];
				pathY  = (path[2] << 8) + (Uint8)path[3];
				path  += 4;
			}
			else
			{
				pathX += path[0];
				pathY += path[1];
				path  += 2;
			}
			pos.groundHeight = 0;
			pos.height       = toFIXED(255);
			for (row = 0; row != ATALANTA_SPRITE_ROWS; row++)
			{
				for (column = 0; column != ATALANTA_SPRITE_ROWS; column++)
				{
					pos.x = INTtoFIXED(pathX + 80 + column * 32);
					pos.y = INTtoFIXED(64 - pathY + row * 32);
					fdRenderApplyProjectedPlacementStub(
						state->sprites[row * ATALANTA_SPRITE_ROWS + column],
						&pos,
						&AtalantaUnitScale,
						0);
				}
			}
		}
		point[1] = 0;
		point[2] = 0;
		// The battle background starts to darken.
		if (frame == 48)
		{
			state->fadeFrame = BACKGROUND_DARKEN_TICKS;
			state->fadeStep  = 0;
		}
		// The shafts appear one after another.
		for (i = 0; i != ATALANTA_SHAFTS; i++)
		{
			Sint32 start = i * ATALANTA_SHAFT_INTERVAL + ATALANTA_SHAFT_FIRST_TICK;

			if (frame >= start)
			{
				PARTICLE *shaft = &state->particles[i];
				Sint32    x     = FIXEDtoINT(shaft->x);
				Sint32    y     = FIXEDtoINT(shaft->y);

				if (frame == start + ATALANTA_TRAIL_TICKS + ATALANTA_FLASH_TICKS - 1)
					sySoundPlayStub(SE_POWER_UP);
				if (frame >= start + ATALANTA_TRAIL_TICKS + ATALANTA_FLASH_TICKS)
				{
					shaft->x         += shaft->velocityX;
					shaft->y         += shaft->velocityY;
					shaft->velocityX -= toFIXED(1);
					shaft->velocityY += toFIXED(2);
					EFFECT_DRAW_PICTURE(
						draw[0], canvas, state->sheet, ATALANTA_SHAFT_TOP, x + 4, y - 40);
					EFFECT_DRAW_PICTURE(
						draw[0], canvas, state->sheet, ATALANTA_SHAFT_MIDDLE, x - 16, y - 19);
					EFFECT_DRAW_PICTURE(
						draw[0], canvas, state->sheet, ATALANTA_SHAFT_BOTTOM, x - 20, y + 16);
				}
				else if (frame >= start + ATALANTA_TRAIL_TICKS)
				{
					switch (frame - start - ATALANTA_TRAIL_TICKS)
					{
						case 0:
							EFFECT_DRAW_PICTURE(draw[0],
							                    canvas,
							                    state->sheet,
							                    ATALANTA_SHAFT_FLASH_A,
							                    x - 7,
							                    y - 14);
							break;
						case 1:
							EFFECT_DRAW_PICTURE(draw[0],
							                    canvas,
							                    state->sheet,
							                    ATALANTA_SHAFT_FLASH_B,
							                    x - 11,
							                    y - 22);
							break;
						case 2:
							EFFECT_DRAW_PICTURE(draw[0],
							                    canvas,
							                    state->sheet,
							                    ATALANTA_SHAFT_FLASH_C_TOP,
							                    x - 4,
							                    y - 31);
							EFFECT_DRAW_PICTURE(draw[0],
							                    canvas,
							                    state->sheet,
							                    ATALANTA_SHAFT_FLASH_C_BOTTOM,
							                    x - 16,
							                    y - 1);
							break;
						case 3:
							EFFECT_DRAW_PICTURE(draw[0],
							                    canvas,
							                    state->sheet,
							                    ATALANTA_SHAFT_FLASH_D_TOP,
							                    x + 1,
							                    y - 38);
							EFFECT_DRAW_PICTURE(draw[0],
							                    canvas,
							                    state->sheet,
							                    ATALANTA_SHAFT_FLASH_D_MIDDLE,
							                    x - 11,
							                    y - 11);
							EFFECT_DRAW_PICTURE(draw[0],
							                    canvas,
							                    state->sheet,
							                    ATALANTA_SHAFT_FLASH_D_BOTTOM,
							                    x - 19,
							                    y + 11);
							break;
						case 4:
							EFFECT_DRAW_PICTURE(draw[0],
							                    canvas,
							                    state->sheet,
							                    ATALANTA_SHAFT_FLASH_E_TOP,
							                    x + 4,
							                    y - 40);
							EFFECT_DRAW_PICTURE(draw[0],
							                    canvas,
							                    state->sheet,
							                    ATALANTA_SHAFT_FLASH_E_MIDDLE,
							                    x - 10,
							                    y - 17);
							EFFECT_DRAW_PICTURE(draw[0],
							                    canvas,
							                    state->sheet,
							                    ATALANTA_SHAFT_FLASH_E_BOTTOM,
							                    x - 20,
							                    y + 11);
							break;
					}
				}
				else
				{
					for (j = 0; j != ATALANTA_TRAIL_MOTES; j++)
					{
						PARTICLE *trail = &PARTICLES[i * ATALANTA_TRAIL_MOTES + j];
						Sint32    size  = 5;

						if (trail->x > 0)
						{
							syLoadMatrix((const FIXED *)(ATALANTA_MATRICES
							                             + (i * ATALANTA_TRAIL_MOTES + j)
							                                   * sizeof(CurrentMatrix)));
							point[0] = trail->x;
							btProjectionPoint(point, &screen);
							screen.x  = (screen.x >> 1) + x;
							screen.y  = screen.y + y + ATALANTA_TRAIL_DROP;
							trail->x -= 4;
							EFFECT_DRAW_MOTE(draw[1], canvas, sheet, screen.x, screen.y, size);
						}
					}
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}
	syCloseEvent(btEffectAdvanceScrollOnInterval);
	view->busy        = 0;
	BgScrollWork[1].x = scrollX;
	btEffectSetupBlendedDisplay();
	for (i = 0; i != ATALANTA_SPRITES; i++)
		fdSpriteDestroyStub(state->sprites[i]);
	REG_BG2PA   = Q_8_8(0.5);
	REG_DISPCNT = EFFECT_DISPCNT;
	btResourceLoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET, RamMapGridMetatileBuffer, 1, 0);
	// The shafts are dealt round the targets as if to a full enemy group of six; a shaft dealt to a
	// missing target falls at random across the field.
	for (i = 0; i != ATALANTA_FALLING_SHAFT_SLOTS; i++)
	{
		PARTICLE *shaft  = &state->particles[i];
		Sint32    member = i % ENEMY_BATTLE_SIZE;

		if (member < state->args->targetCount)
		{
			btProjectionScaledUnit(state->args->targets[member], &target);
			shaft->y = -((syRandom() & 31) + 40);
			shaft->x = target.x / 2 + (80 - shaft->y) / 2;
		}
		else
		{
			shaft->x = (syRandom() & 63) + 80;
			shaft->y = -((syRandom() & 31) + 40);
		}
		shaft->variant = -1;
	}
	// The shafts rain down on the targets.
	for (frame = 0; frame != ATALANTA_FALL_TICKS; frame++)
	{
		for (i = 0; i != ATALANTA_FALLING_SHAFTS; i++)
		{
			PARTICLE *shaft = &state->particles[i];

			if (frame >= i * 2 || frame > 40)
			{
				// A landed shaft bursts in light.
				if (shaft->variant >= 0)
				{
					if (shaft->variant < ATALANTA_BURST_PICTURES * ATALANTA_BURST_PICTURE_TICKS)
					{
						Sint32 picture = shaft->variant / ATALANTA_BURST_PICTURE_TICKS;
						Uint32 width;

						// UB: width is assigned in one argument and read in another.
#if FIX_UNSEQUENCED_SIDE_EFFECT
						width = BattleEffectPuffArcPictureWidths[picture];
						draw[i & 1](canvas,
						            RamMapGridMetatileBuffer + BattleEffectPuffArcPictures[picture],
						            shaft->x - (width >> 1) - 8,
						            shaft->y + BattleEffectPuffArcPictureBiasY[picture] - 40,
						            width,
						            BattleEffectPuffArcPictureHeights[picture]);
#else
						draw[i & 1](canvas,
						            RamMapGridMetatileBuffer + BattleEffectPuffArcPictures[picture],
						            shaft->x
						                - ((width = BattleEffectPuffArcPictureWidths[picture]) >> 1)
						                - 8,
						            shaft->y + BattleEffectPuffArcPictureBiasY[picture] - 40,
						            width,
						            BattleEffectPuffArcPictureHeights[picture]);
#endif
						if (shaft->variant < ATALANTA_BURST_SHAFT_TICKS)
						{
							EFFECT_DRAW_PICTURE(draw[0],
							                    canvas,
							                    state->sheet,
							                    ATALANTA_SHAFT_TOP,
							                    shaft->x + 4,
							                    shaft->y - 40);
							EFFECT_DRAW_PICTURE(draw[0],
							                    canvas,
							                    state->sheet,
							                    ATALANTA_SHAFT_MIDDLE,
							                    shaft->x - 16,
							                    shaft->y - 19);
						}
					}
					shaft->variant++;
				}
				else
				{
					Sint32 height = 24;

					if (shaft->y > 56)
						height = height - shaft->y + 56;
					EFFECT_DRAW_PICTURE(draw[0],
					                    canvas,
					                    state->sheet,
					                    ATALANTA_SHAFT_TOP,
					                    shaft->x + 4,
					                    shaft->y - 40);
					EFFECT_DRAW_PICTURE(draw[0],
					                    canvas,
					                    state->sheet,
					                    ATALANTA_SHAFT_MIDDLE,
					                    shaft->x - 16,
					                    shaft->y - 19);
					if (height > 0)
						draw[0](canvas,
						        state->sheet + ATALANTA_SHAFT_BOTTOM,
						        shaft->x - 20,
						        shaft->y + 16,
						        ATALANTA_SHAFT_BOTTOM_WIDTH,
						        height);
					shaft->x -= 6;
					shaft->y += 12;
					if (shaft->y > 79)
					{
						Sint32 member;

						shaft->variant    = 0;
						state->shakeFrame = 2;
						sySoundPlayStub(SE_HEAVY_IMPACT);
						member = i % ENEMY_BATTLE_SIZE;
						if (member < state->args->targetCount)
						{
							btEffectTintBattler(state->args->targets[member],
							                    SPRITE_COLOR_WHITE,
							                    POSE_DAMAGE,
							                    member,
							                    8);
							btMotionApplyKnockbackStub(state->args->targets[member], KNOCKBACK_HOP);
						}
					}
				}
			}
		}
		btCameraShake(4, 8);
		EFFECT_END_TICK(state);
	}
	EFFECT_FINISH_CANVAS();
}

#undef ATALANTA_FALL_TICKS
#undef ATALANTA_SHAFTS
#undef ATALANTA_TRAIL_MOTES
#undef ATALANTA_FALLING_SHAFTS
#undef ATALANTA_SPRITE_ROWS
#undef ATALANTA_SPRITES
#undef ATALANTA_TRAIL_DROP
#undef ATALANTA_MATRICES
#undef ATALANTA_FALLING_SHAFT_SLOTS
#undef ATALANTA_SHAFT_FIRST_TICK
#undef ATALANTA_SHAFT_INTERVAL
#undef ATALANTA_TRAIL_TICKS
#undef ATALANTA_FLASH_TICKS
#undef ATALANTA_BURST_PICTURES
#undef ATALANTA_BURST_PICTURE_TICKS
#undef ATALANTA_BURST_SHAFT_TICKS
