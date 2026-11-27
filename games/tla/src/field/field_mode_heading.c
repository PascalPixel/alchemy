// A heading picture shown in a window that opens from the centre, and wiped away from the left.
// Nothing calls these but the stub table. Part of field_mode.c.

// A heading picture: 26 by 5 tiles of 16 colours, drawn from tile 16 of character block 0 and shown
// in the window palette on BG0, 2 metatiles from the screen's left and 3 from its top.
#define HEADING_COLUMNS        26
#define HEADING_ROWS           5
#define HEADING_LEFT           2
#define HEADING_TOP            3
#define HEADING_FIRST_TILE     16
#define HEADING_TILES          ((Uint8 *)BG_CHAR_BLOCK(0) + HEADING_FIRST_TILE * TILE_SIZE_4BPP)
#define HEADING_TILE_BYTES     (HEADING_COLUMNS * HEADING_ROWS * TILE_SIZE_4BPP)
// A blank BG0 metatile: tile 0 in the window palette.
#define HEADING_BLANK_METATILE BG_TILE_PALETTE(WINDOW_PALETTE)
// The ticks the heading's window takes to open from the centre, and the pixels it widens by on each
// side a tick.
#define HEADING_OPEN_TICKS     24
#define HEADING_OPEN_STEP      5

// Unused: only the stub table refers to it. Draws heading picture `heading` of the headings
// resource into a 26 by 5 metatile window and opens the window from the centre.
void fdHeadingShow(Sint32 heading)
{
	Uint16  tile;
	Sint32 *headings = syResourceGetTableEntry(RESOURCE_HEADINGS);
	Uint8  *tiles;
	Uint16 *metatile;
	Uint32  row, column;
	Sint32  left, right;

	tile = HEADING_FIRST_TILE;
	fdScriptWaitTransitionTicks();
	metatile  = (Uint16 *)BG_SCREEN_BLOCK(4);
	tiles     = syHeapAllocateEWRAM(HEADING_TILE_BYTES);
	headings += headings[heading] / 4;
	syDecompressByteLZ(headings, tiles);
	syDMAXCopy(tiles, HEADING_TILES, DMA_COPY_WORDS(HEADING_TILE_BYTES / 4), REG_DMA3);
	syHeapRelease(tiles);
	REG_DISPCNT &= ~DISPCNT_BG0_ON;
	DMA_FILL32(HEADING_BLANK_METATILE, metatile, BG_SCREEN_SIZE);
	metatile = (Uint16 *)BG_SCREEN_BLOCK(4) + HEADING_TOP * BG_SCREEN_WIDTH;
	for (row = 0; row < HEADING_ROWS; row++)
	{
		metatile += HEADING_LEFT;
		for (column = 0; column < HEADING_COLUMNS; column++)
		{
			*metatile++ = (Uint16)(tile++) | HEADING_BLANK_METATILE;
		}
		metatile += BG_SCREEN_WIDTH - HEADING_LEFT - HEADING_COLUMNS;
	}
	REG_DISPCNT &= ~(DISPCNT_WIN0_ON | DISPCNT_WIN1_ON | DISPCNT_OBJWIN_ON);
	REG_DISPCNT |= DISPCNT_WIN0_ON;
	REG_WININ    = WININ_WIN0_ALL;
	REG_WINOUT   = WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ
	           | WINOUT_WIN01_CLR;
	REG_WIN0H = WIN_RANGE(DISPLAY_WIDTH / 2, DISPLAY_WIDTH / 2);
	REG_WIN0V = WIN_RANGE(24, 72);
	fdPaletteFadeSetSource(PALETTE_TINT_NONE, 0);
	fdPaletteFadeSetTarget(PALETTE_TINT_DIM_BRIGHT, 0);
	fdPaletteFadeStartInterpolation(24);
	REG_DISPCNT ^= DISPCNT_BG0_ON;
	left         = DISPLAY_WIDTH / 2;
	right        = DISPLAY_WIDTH / 2;
	for (column = 0; column < HEADING_OPEN_TICKS; column++)
	{
		left      -= HEADING_OPEN_STEP;
		right     += HEADING_OPEN_STEP;
		REG_WIN0H  = WIN_RANGE(left, right);
		sySynch(1);
	}
}

// Waits up to two seconds, stopping early when a button is pressed.
void fdScriptWaitTwoSecondsOrPress(void)
{
	Uint32 i;

	for (i = 0; i < 2 * TICKS_PER_SECOND; i++)
	{
		sySynch(1);
		if (PerWork.push != 0)
			break;
	}
}

// Clears pixel `pixel` (its row times 8 plus its column) of the 4bpp tile at (x, y) of the
// heading's picture, and the pixels left of it in its halfword: a tile's row is two halfwords of
// four pixels each, the leftmost in the low bits, so halfword pixel >> 2 loses its low (pixel & 3)
// + 1 pixels.
void fdHeadingWipeTilePixel(Sint32 x, Sint32 y, Uint32 pixel)
{
	Uint16 *metatile  = (Uint16 *)(HEADING_TILES + (y * HEADING_COLUMNS + x) * TILE_SIZE_4BPP);

	metatile         += pixel >> 2;

	switch (pixel & 3)
	{
		case 0: *metatile &= 0xfff0; break;
		case 1: *metatile &= 0xff00; break;
		case 2: *metatile &= 0xf000; break;
		case 3: *metatile &= 0; break;
	}
}

extern Sint16 HeadingWipeSteps[];
// HeadingWipeSteps' last entry, after its last diagonal.
#define HEADING_WIPE_END (-9)

// Unused: only the stub table refers to it. Closes fdHeadingShow's picture window: fades the
// palette out while wiping every one of the picture's 26 by 5 tiles from its top left corner to its
// bottom right, a diagonal every two ticks (HeadingWipeSteps: a tile's pixels a diagonal at a time,
// each ended by a negative entry and the last by HEADING_WIPE_END), then turns BG0 and the window
// off, clears the screen block, reloads the window tiles and turns BG0 back on.
void fdHeadingClose(void)
{
	Sint32          step = 0;
	Uint32          x;
	Uint32          y;
	volatile Uint32 fill;

	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
	fdPaletteFadeStartInterpolation(24);
	for (;;)
	{
		for (; HeadingWipeSteps[step] >= 0; step++)
		{
			for (x = 0; x < HEADING_COLUMNS; x++)
			{
				for (y = 0; y < HEADING_ROWS; y++)
					fdHeadingWipeTilePixel(x, y, HeadingWipeSteps[step]);
			}
		}
		sySynch(2);
		if (HeadingWipeSteps[step] == HEADING_WIPE_END)
			break;
		step++;
	}
	REG_DISPCNT ^= DISPCNT_BG0_ON | DISPCNT_WIN0_ON;
	DMA_FILL32_CELL(fill, HEADING_BLANK_METATILE, (void *)BG_SCREEN_BLOCK(4), BG_SCREEN_SIZE);
	syDMAXCopy(syResourceGetTableEntry(RESOURCE_WINDOW_TILES),
	           (void *)BG_CHAR_BLOCK(0),
	           DMA_COPY_WORDS(2048),
	           REG_DMA3);
	REG_DISPCNT ^= DISPCNT_BG0_ON;
}

#undef HEADING_COLUMNS
#undef HEADING_ROWS
#undef HEADING_LEFT
#undef HEADING_TOP
#undef HEADING_FIRST_TILE
#undef HEADING_TILES
#undef HEADING_TILE_BYTES
#undef HEADING_BLANK_METATILE
#undef HEADING_OPEN_TICKS
#undef HEADING_OPEN_STEP
#undef HEADING_WIPE_END
