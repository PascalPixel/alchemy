// The Atalanta summon. Part of battle_effect.c.

// The pictures in the slash sprite sheet (Atalanta's arrows of light): five flash pictures, then
// the shaft in three parts, each in its own size.
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

// Where each picture starts in SlashSheet, in bytes.
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

// The trail particles' random rotations, after the particles in PARTICLES.
#define ATALANTA_MATRICES            (RamMapMetatileBuffer + 14336)

// Atalanta's sprites: four rows of three 32x32 pieces, the last row two.
#define ATALANTA_PIECES              11

// The gathering's length, unless A or B cuts it short, and the ticks Atalanta flies across in it.
#define ATALANTA_GATHER_TICKS        220
#define ATALANTA_FLIGHT_LAST_TICK    209
// The tick of the gathering the sky starts to darken.
#define ATALANTA_DARKEN_TICK         48
#define ATALANTA_FALL_TICKS          88 // ticks of the shafts' fall

#define ATALANTA_SHAFTS              16 // shafts of light that gather over the field
#define ATALANTA_TRAIL_MOTES         24 // motes in each gathering shaft's trail
#define ATALANTA_FALLING_SHAFTS      24 // shafts that rain down on the targets
// The particles the falling shafts are set up in, more than fall.
#define ATALANTA_FALLING_SHAFT_SLOTS 32

// The gathering shafts' timing: the first one's trail gathers from tick ATALANTA_SHAFT_FIRST_TICK
// and each next one ATALANTA_SHAFT_INTERVAL ticks later; a trail gathers for ATALANTA_TRAIL_TICKS,
// the shaft flashes in for ATALANTA_FLASH_TICKS, one a picture, then flies off.
#define ATALANTA_SHAFT_FIRST_TICK    64
#define ATALANTA_SHAFT_INTERVAL      8
#define ATALANTA_TRAIL_TICKS         16
#define ATALANTA_FLASH_TICKS         5

// How far below its projected point a trail mote is drawn.
#define ATALANTA_TRAIL_DROP          16

// A landed shaft's burst: EmberStreakSheet's six flame pictures, four ticks each, the shaft still
// showing over the first three.
#define ATALANTA_BURST_PICTURES      6
#define ATALANTA_BURST_PICTURE_TICKS 4
#define ATALANTA_BURST_TICKS         (ATALANTA_BURST_PICTURES * ATALANTA_BURST_PICTURE_TICKS)
#define ATALANTA_BURST_SHAFT_TICKS   (3 * ATALANTA_BURST_PICTURE_TICKS)

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
	DRAW_RECTANGLE           draw[2];
	BATTLE_BACKGROUND_STATE *view;
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
	Uint8                   *tiles;
	Uint8                   *picture;

	view    = RamHeapSlots->battleBg;
	tiles   = RamMapMetatileBuffer;
	canvas  = RamHeapSlots->battleCanvas;
	state   = RamHeapSlots->battleEffect;
	sheet   = RamHeapSlots->battleSheet;
	scrollX = BgScrollWork[1].x;
	// The screen wipes to black before Atalanta appears.
	EFFECT_START_NARROW_CANVAS(BGCNT_WRAP);
	btEffectConfigureEffectDisplay();
	PLTT_BACKDROP       = 0;
	BG_PLTT_COLOR(0, 1) = 0;
	state->transferMode = CANVAS_COPY;
	sySetEvent(btEffectTransferCanvas, EVENT_PASS_VBLANK);
	btEffectWipeCanvas(CANVAS_WIPE_DOWN, CANVAS_WIPE_FILL);
	btEffectShowLivingTargets(state->args);

	// Atalanta's eleven sprites, with her palette and tiles.
	RamHeapSlots->battleState->keepBackdropPalette = 1;
	picture = syResourceGetTableEntry(RESOURCE_LEAF_WARRIOR_TILES);
	syWordsCopy(OBJ_PLTT, picture, OBJ_PLTT_SIZE);
	picture += OBJ_PLTT_SIZE;
	syDecompressTaggedLZ(picture, tiles);
	for (i = 0; i != ATALANTA_PIECES; i++)
	{
		SPR *piece = btEffectSpriteCreateWithTiles(32, 32, OBJ_SIZE_32X32 | OBJ_256_COLORS, 0);

		piece->image.priority = 1;
		state->sprites[i]     = piece;
		syWordsCopyEntry(OBJ_VRAM + VramCacheEntries[piece->slot].offset,
		                 tiles + i * OBJ_32X32_8BPP_BYTES,
		                 OBJ_32X32_8BPP_BYTES);
	}

	// The shafts, the sky and Atalanta's path.
	btResourceLoadAndDecompress(RESOURCE_SLASH_SHEET, state->sheet, 1, 1);
	picture = syResourceGetTableEntry(RESOURCE_LIME_PALETTE);
	syWordsCopyEntry((void *)BG_PLTT, picture, SHEET_PALETTE_BYTES);
	btResourceLoadAndDecompress(RESOURCE_YELLOW_GLOW_SHEET, sheet, 0, 0);
	path = syResourceGetTableEntry(RESOURCE_ATALANTA_PATH);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE, EFFECT_CLIP_BOUNDS, BLITTER_MAX);
	EFFECT_LOAD_BLITTER(HEAP_SLOT_RAM_CODE_PAIRED, EFFECT_CLIP_BOUNDS, BLITTER_ADDITIVE);
	draw[0]                = RamHeapSlots->ramCode;
	draw[1]                = RamHeapSlots->ramCodePaired;
	ProjectionWork.centerY = SUMMON_PROJECTION_CENTER_Y;
	sySynch(1);
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

	// Each shaft starts at a random spot with its trail of motes, each turned at random.
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
			syGetMatrix(
				(FIXED *)(ATALANTA_MATRICES + (i * ATALANTA_TRAIL_MOTES + j) * sizeof(MATRIX)));
		}
	}
	EFFECT_SET_TRANSFER(state, CANVAS_COPY_AND_FADE, CANVAS_FADE_HALF);
	REG_BG2CNT = BGCNT_CHAR_BASE(1) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(7);

	// The gathering (A or B ends it).
	for (frame = 0; frame != ATALANTA_GATHER_TICKS && !(PerWork.repeat & (KEY_A | KEY_B)); frame++)
	{
		// Atalanta flies along her path: where she starts, then a step a tick.
		if (frame <= ATALANTA_FLIGHT_LAST_TICK)
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
			for (row = 0; row != 4; row++)
			{
				for (column = 0; column != 3; column++)
				{
					if (row != 3 || column != 2)
					{
						pos.x = toFIXED(pathX + 80 + column * 32);
						pos.y = toFIXED(64 - pathY + row * 32);
						if (row == 3)
							pos.x = toFIXED(pathX + 80 + column * 32 + 32);
						fdRenderApplyProjectedPlacementStub(state->sprites[row * 3 + column],
						                                    sySpritePlacementValues(&pos),
						                                    AtalantaUnitScale,
						                                    0);
					}
				}
			}
		}
		point[1] = 0;
		point[2] = 0;
		if (frame == ATALANTA_DARKEN_TICK)
		{
			state->fadeFrame = BACKGROUND_DARKEN_TICKS;
			state->fadeStep  = 0;
		}

		// The shafts appear one after another: each one's trail draws in, it flashes and flies off.
		for (i = 0; i != ATALANTA_SHAFTS; i++)
		{
			Sint32 start = i * ATALANTA_SHAFT_INTERVAL + ATALANTA_SHAFT_FIRST_TICK;

			if (frame >= start)
			{
				PARTICLE *shaft = &state->particles[i];
				Sint32    x     = FIXED_INTEGER_PART(shaft->x);
				Sint32    y     = FIXED_INTEGER_PART(shaft->y);

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

						if (trail->x > 0)
						{
							syLoadMatrix(
								(const FIXED *)(ATALANTA_MATRICES
							                    + (i * ATALANTA_TRAIL_MOTES + j) * sizeof(MATRIX)));
							point[0] = trail->x;
							btEffectPosProjectPoint(point, &screen);
							screen.x  = (screen.x >> 1) + x;
							screen.y  = screen.y + y + ATALANTA_TRAIL_DROP;
							trail->x -= 4;
							EFFECT_DRAW_MOTE(draw[1], canvas, sheet, screen.x, screen.y, 5);
						}
					}
				}
			}
		}
		EFFECT_PRESENT_CANVAS(state);
	}

	// Atalanta leaves: the sky stops, the palettes are restored and her sprites go.
	syCloseEvent(btEffectAdvanceScrollOnInterval);
	view->busy        = 0;
	BgScrollWork[1].x = scrollX;
	btEffectSetupBlendedDisplay();
	syPaletteLoadBasicSpriteColors();
	syDMAXCopy(OBJ_PLTT,
	           (void *)&BG_PLTT_COLOR(BASIC_COLORS_PALETTE, 0),
	           DMA_COPY_HALFWORDS(PALETTE_COLORS),
	           REG_DMA3);
	BG_PLTT_COLOR(BASIC_COLORS_PALETTE, TEXT_BACKGROUND_COLOR) =
		BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR);
	for (i = 0; i != ATALANTA_PIECES; i++)
		fdSpriteDestroyStub(state->sprites[i]);
	REG_BG2PA   = Q_8_8(0.5);
	REG_DISPCNT = EFFECT_DISPCNT;
	btResourceLoadAndDecompress(RESOURCE_EMBER_STREAK_SHEET, RamMapMetatileBuffer, 1, 0);

	// The shafts start above the sky. They take the six places of a side in turn: one whose place
	// holds a target is aimed at it, the others fall at random.
	for (i = 0; i != ATALANTA_FALLING_SHAFT_SLOTS; i++)
	{
		PARTICLE *shaft  = &state->particles[i];
		Sint32    member = i % 6;

		if (member < args->targetCount)
		{
			btEffectPosProjectScaledBattler(args->targets[member], &target);
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
	// The shafts rain down on the targets, two ticks apart.
	for (frame = 0; frame != ATALANTA_FALL_TICKS; frame++)
	{
		for (i = 0; i != ATALANTA_FALLING_SHAFTS; i++)
		{
			PARTICLE *shaft = &state->particles[i];

			if (frame >= i * 2 || frame > 40)
			{
				if (shaft->variant >= 0)
				{
					// A landed shaft bursts in light.
					if (shaft->variant < ATALANTA_BURST_TICKS)
					{
						Sint32 flame = shaft->variant / ATALANTA_BURST_PICTURE_TICKS;
						Uint32 width;

						// UB: width is assigned in one argument and read in another.
#if FIX_UNSEQUENCED_SIDE_EFFECT
						width = BattleEffectEmberStreakPictureWidths[flame];
						draw[i & 1](canvas,
						            RamMapMetatileBuffer + BattleEffectEmberStreakPictures[flame],
						            shaft->x - (width >> 1) - 8,
						            shaft->y + BattleEffectEmberStreakPictureBiasY[flame] - 40,
						            width,
						            BattleEffectEmberStreakPictureHeights[flame]);
#else
						draw[i & 1](
							canvas,
							RamMapMetatileBuffer + BattleEffectEmberStreakPictures[flame],
							shaft->x - ((width = BattleEffectEmberStreakPictureWidths[flame]) >> 1)
								- 8,
							shaft->y + BattleEffectEmberStreakPictureBiasY[flame] - 40,
							width,
							BattleEffectEmberStreakPictureHeights[flame]);
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
					// Falling: the shaft sinks into the ground below y = 56.
					Sint32 height = ATALANTA_SHAFT_BOTTOM_HEIGHT;

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
						member = i % 6;
						if (member < args->targetCount)
						{
							EFFECT_STRIKE_TARGET(args, member, 8, KNOCKBACK_HOP);
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

#undef ATALANTA_MATRICES
#undef ATALANTA_GATHER_TICKS
#undef ATALANTA_FLIGHT_LAST_TICK
#undef ATALANTA_DARKEN_TICK
#undef ATALANTA_PIECES
#undef ATALANTA_FALLING_SHAFT_SLOTS
#undef ATALANTA_TRAIL_DROP
#undef ATALANTA_SHAFT_FIRST_TICK
#undef ATALANTA_SHAFT_INTERVAL
#undef ATALANTA_TRAIL_TICKS
#undef ATALANTA_FLASH_TICKS
#undef ATALANTA_BURST_PICTURES
#undef ATALANTA_BURST_PICTURE_TICKS
#undef ATALANTA_BURST_TICKS
#undef ATALANTA_BURST_SHAFT_TICKS
