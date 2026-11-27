/* The Psynergy gauge over the leader as the ship flies, which the world map and
   the staff credits' overlays keep a copy of: its tiles redrawn for the party's
   PP, the event that drains it while A or B is held aloft, and its set-up and
   end. */

#include "system.h"

/* syWordsCopy reached through the linked IWRAM code's symbol, syIWRAMIRQMain,
   at its offset rather than at its fixed address, as the Daedalus effect and
   the item menu reach it. */
#define GAUGE_COPY_WORDS \
	((Sint32 (*)(void *, const void *, Sint32))( \
		syIWRAMIRQMain + syWordsCopyOffset))

/* The gauge's source tiles (WorldMapPackedTilesC): a bar of four tiles with its
   PP line lit, the same bar dark, and a chequered one for no PP. The bar is
   filled a pixel a level, eight levels to a tile. */
#define GAUGE_LIT_BAR	   0
#define GAUGE_DARK_BAR	   4
#define GAUGE_EMPTY_BAR	   8
#define GAUGE_TILE_PIXELS  8
/* The PP line's rows in a bar tile, from the second. */
#define GAUGE_LINE_ROWS	   3
/* A 4bpp tile row's byte holds two pixels, the left one in its low nibble. */
#define LEFT_PIXEL		   0x0f
#define RIGHT_PIXEL		   0xf0
/* The frames between two PP drained while the ship is held aloft. */
#define GAUGE_DRAIN_FRAMES 10

/* Redraws the gauge for `progress`, the party's PP as a 16.16 fraction of its
   most; at half or less it blinks, and faster at a quarter. */
void scGaugeDraw(Sint32 progress)
{
	Sint32 level = FIXEDtoINT(progress * GAUGE_LEVELS + toFIXED(1) / 2) + 1;
	Sint32 kind = GAUGE_EMPTY;
	Sint32 count, base, i;

	if(progress != 0)
		kind = GAUGE_SHOWN;
	if(level <= GAUGE_LEVELS / 2 && (FrameCount & 15) <= 1)
		kind = GAUGE_LOW;
	if(level <= GAUGE_LEVELS / 4 && (FrameCount & 7) <= 1)
		kind = GAUGE_LOW;
	if(kind + level == Gauge_Shown)
		return;
	Gauge_Shown = kind + level;
	syWordsClear((Sint32)(Gauge_Tiles), (Sint32)(GAUGE_SIZE));
	if(kind == GAUGE_EMPTY) {
		GAUGE_COPY_WORDS(Gauge_Tiles, Gauge_SourceTiles[GAUGE_EMPTY_BAR],
			4 * TILE_SIZE_4BPP);
	} else {
		if(kind == GAUGE_SHOWN)
			GAUGE_COPY_WORDS(Gauge_Tiles, Gauge_SourceTiles[GAUGE_DARK_BAR],
				4 * TILE_SIZE_4BPP);
		else
			GAUGE_COPY_WORDS(Gauge_Tiles, Gauge_SourceTiles[GAUGE_EMPTY_BAR],
				4 * TILE_SIZE_4BPP);
		/* Whole tiles of the lit bar first, then the rest a pixel at a time. */
		base = 0;
		count = level;
		if(count >= GAUGE_TILE_PIXELS) {
			GAUGE_COPY_WORDS(
				Gauge_Tiles, Gauge_SourceTiles[GAUGE_LIT_BAR], TILE_SIZE_4BPP);
			base = GAUGE_TILE_PIXELS;
			count -= GAUGE_TILE_PIXELS;
		}
		if(level >= 2 * GAUGE_TILE_PIXELS) {
			GAUGE_COPY_WORDS(Gauge_Tiles[1],
				Gauge_SourceTiles[GAUGE_LIT_BAR + 1], TILE_SIZE_4BPP);
			count = level;
			base = 2 * GAUGE_TILE_PIXELS;
			count -= 2 * GAUGE_TILE_PIXELS;
		}
		if(level >= 3 * GAUGE_TILE_PIXELS) {
			GAUGE_COPY_WORDS(Gauge_Tiles[2],
				Gauge_SourceTiles[GAUGE_LIT_BAR + 2], TILE_SIZE_4BPP);
			count = level;
			base = 3 * GAUGE_TILE_PIXELS;
			count -= 3 * GAUGE_TILE_PIXELS;
		}
		if(level > GAUGE_LEVELS) {
			GAUGE_COPY_WORDS(Gauge_Tiles[3],
				Gauge_SourceTiles[GAUGE_LIT_BAR + 3], TILE_SIZE_4BPP);
			base = 0;
			count = 0;
		}
		if(count != 0) {
			for(i = 0; i < count; i++) {
				Uint8 *source = &Gauge_SourceTiles[GAUGE_LIT_BAR +
									base / GAUGE_TILE_PIXELS][4] +
					i / 2;
				Uint8 *dest = &Gauge_Tiles[base / GAUGE_TILE_PIXELS][4] + i / 2;
				Sint32 j;

				if((i & 1) == 0) {
					for(j = 1; j <= GAUGE_LINE_ROWS; j++) {
						*dest = (*dest & RIGHT_PIXEL) | (*source & LEFT_PIXEL);
						source += 4;
						dest += 4;
					}
				} else {
					for(j = 0; j < GAUGE_LINE_ROWS; j++) {
						*dest = (*dest & LEFT_PIXEL) | (*source & RIGHT_PIXEL);
						source += 4;
						dest += 4;
					}
				}
			}
		}
	}
	mnVRAMLoad(Gauge_Vram, GAUGE_SIZE, Gauge_Tiles);
}

/* The Psynergy gauge over the leader as the ship flies: while A or B is held
   aloft it drains a PP every ten frames, then redraws the gauge and queues its
   sprite. */
void scGaugeUpdate(void)
{
	Uint32 *oam = Gauge_Sprite;
	Sint32	tile = VramCacheEntries[Gauge_Vram].ofs >> 5;
	ACTOR  *leader = fdActorGet(syFieldPartyGetLeaderActor());
	Sint32	screen[3];
	Uint32	attributes;

	if((PerWork.data & (KEY_A | KEY_B)) != 0 &&
		fdScriptGetFlag(FLAG_MENU_OPEN) == 0 && leader->y.fixed > toFIXED(4)) {
		if(--Gauge_Countdown <= 0) {
			gmPartyDrainPp(1);
			Gauge_Countdown = GAUGE_DRAIN_FRAMES;
		}
	}
	scGaugeDraw(gmPartyGetPpRatio());
	attributes = OBJ_SIZE_32X16;
	Call2((void (*)())syConvert3Dto2D, (Sint32)(&leader->x.fixed),
		(Sint32)(screen));
	/* The 32 by 16 gauge, centred 32 pixels above the leader. */
	{
		Sint32 x = screen[0] - 16;
		Sint32 y = screen[1] - 32;

		*oam++ = 0;
		*oam++ = (y | OBJ_X(x)) | attributes;
	}
	*oam = tile | OBJ_PRIORITY(1);
	syOAMAddSprAtDepthClamped(Gauge_Sprite, OAM_DEPTH_FRONT);
}

/* Sets up the gauge. */
void scGaugeInit(void)
{
	syDecompressTaggedLZ(WorldMapPackedTilesC, Gauge_SourceTiles);
	Gauge_Vram = syVRAMCacheFindFreeEntry();
	mnVRAMLoad(Gauge_Vram, GAUGE_SIZE, 0);
	Gauge_Shown = -1;
	Gauge_Countdown = GAUGE_DRAIN_FRAMES;
	sySetFrameEvent(scGaugeUpdate, EVENT_PASS_FRAME - 10);
}

/* Takes the gauge down. */
void scGaugeEnd(void)
{
	syDecompressTaggedLZ(WorldMapPackedTilesC, Gauge_SourceTiles);
	syVRAMCacheFreeEntry(Gauge_Vram);
	Gauge_Shown = -1;
	syFieldCloseEvent(scGaugeUpdate);
}

#undef GAUGE_LIT_BAR
#undef GAUGE_DARK_BAR
#undef GAUGE_EMPTY_BAR
#undef GAUGE_TILE_PIXELS
#undef GAUGE_LINE_ROWS
#undef LEFT_PIXEL
#undef RIGHT_PIXEL
#undef GAUGE_DRAIN_FRAMES
