/* Pictures and command icons loaded for the menus, and BG1's priority. Part of
   screens.c. */

/* A row of a command icon's 8x8 tiles, four across, as a sprite holds them. */
#define COMMAND_ICON_TILE_ROW_BYTES (4 * TILE_SIZE_8BPP)

/* Decompresses picture `picture` of MenuTitlePictures, a 64x16 heading (Save,
   Continue, Copy, Erase, Battle, Link, Pause, Setup) of 512 bytes, into VRAM
   slot `slot`, through a 1 KB staging block in HEAP_SLOT_TILE_SCRATCH. */
void mnTitleLoad(Sint32 picture, Sint32 slot)
{
	void *stagingBuf = syHeapSlotAllocEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
	const Uint8 *resource = MenuTitlePictures[picture];

	if(slot < VRAM_CACHE_ENTRY_COUNT) {
		syDecompressByteLZ(resource, stagingBuf);
		syVRAMCacheLoad(slot, 512, stagingBuf);
		syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	}
}

/* Shows heading `picture` of MenuTitlePictures as two 32x16 sprites side by
   side at (x, y) in `win`, on draw layer 253, and returns the left one (the
   right one follows it in the window's list). */
WINDOW_SPR *mnTitleCreate(Sint32 picture, WINDOW *win, Sint32 x, Sint32 y)
{
	Sint32		slot;
	WINDOW_SPR *first;
	WINDOW_SPR *second;

	slot = syVRAMCacheFindFreeEntry();
	if(slot >= VRAM_CACHE_ENTRY_COUNT)
		return NULL;

	/* Each half is 8 tiles. Neither sprite is checked for NULL. */
	mnTitleLoad(picture, slot);
	first = mnWindowSprCreate(slot, OUTPUT_SIZE_32X16, win, x, y);
	first->depth = OAM_DEPTH_FRONT - 2;
	second = mnWindowSprCreate(slot, OUTPUT_SIZE_32X16, win, x + 32, y);
	second->depth = OAM_DEPTH_FRONT - 2;
	/* The right half's tiles, after the left half's 8. */
	second->spr.oam.idx += 8;
	return first;
}

/* Bobs the two sprites of a pair (see mnTitleCreate) up and down by
   MenuTitleBobOffsets, one step every four ticks. */
void mnTitleBob(WINDOW_SPR *spr)
{
	Sint32 value;

	/* The VBlank interrupt counts FrameCount, so each sprite reads it
	   afresh. */
	value = (Uint8)spr->y +
		MenuTitleBobOffsets[(*(volatile Uint32 *)&FrameCount >> 2) & 7];
	spr->spr.oam.y = value;
	spr = spr->next;
	value = (Uint8)spr->y +
		MenuTitleBobOffsets[(*(volatile Uint32 *)&FrameCount >> 2) & 7];
	spr->spr.oam.y = value;
}

#include "window/command_icon_load.c"

/* Shows command icon `icon` (see mnCommandIconLoad) as a 32x32 256-colour
   sprite at (x, y) in `win`, on draw layer 251. */
WINDOW_SPR *mnCommandIconCreate(
	Sint32 icon, Sint32 remap, WINDOW *win, Sint32 x, Sint32 y)
{
	Sint32		slot;
	WINDOW_SPR *spr;

	slot = syVRAMCacheFindFreeEntry();
	spr = NULL;
	if(slot != VRAM_CACHE_ENTRY_COUNT) {
		mnCommandIconLoad(icon, slot, remap);
		spr = mnWindowSprCreate(slot, OUTPUT_SIZE_32X32, win, x, y);
		spr->spr.oam.fullCol = 1;
		spr->depth = OAM_DEPTH_FRONT - 4;
	}
	return spr;
}

/* Makes `spr` pulse: scales it about its centre by WindowSpritePulseScales
   (in 1/65536, used in 1/256), one step every two ticks, and puts it on draw
   layer 252. */
void mnWindowSprPulse(WINDOW_SPR *spr)
{
	Sint32 scale = WindowSpritePulseScales[(FrameCount >> 1) & 7] / 256;
	AFFINE_TRANSFORM effect;

	if(spr != 0) {
		effect.scaleX = scale;
		effect.scaleY = scale;
		effect.ang = 0;
		spr->spr.oam.matrix = syOAMMatrixAllocFromTransform(&effect);
		/* Affine mode 3 doubles the sprite's box, so it moves up and left by 16
		   pixels (-16 in the 16-bit x and 8-bit y) to keep its centre. */
		spr->spr.oam.affineMode = 3;
		spr->spr.oam.x = spr->x + (Uint16)-16;
		spr->spr.oam.y = (Uint8)spr->y + (Uint8)-16;
		spr->depth = OAM_DEPTH_FRONT - 3;
	}
}

/* The range icons' two sets of six, each a pair of tiles (16 words). */
#define RANGE_PATTERN_SET_COUNT	 2
#define RANGE_PATTERN_COUNT		 6
#define RANGE_PATTERN_PAIR_WORDS 16
/* The first of the range icons' BG tiles, which mnRangeIconDraw draws as
   tilemap entries. */
#define RANGE_ICON_FIRST_TILE	 788

/* Builds the twelve range icons of the battle lists, which mnRangeIconDraw
   draws, as the 24 BG tiles from RANGE_ICON_FIRST_TILE: icon n is a centre tile
   and an end tile, filled with WINDOW_BACKGROUND_COLOR, then marked row by row
   by XOR with MenuBarPatternRows (its colour 5 turns the fill to colour 1). */
UNREAD_RESULT mnRangeIconBuildTiles(void)
{
	Uint32 *vram =
		(Uint32 *)(BG_CHAR_BLOCK(0) + RANGE_ICON_FIRST_TILE * TILE_SIZE_4BPP);
	Sint32 patternSet;
	Sint32 patternIdx;

	for(patternSet = 0; patternSet < RANGE_PATTERN_SET_COUNT; patternSet++) {
		for(patternIdx = 0; patternIdx < RANGE_PATTERN_COUNT; patternIdx++) {
			Uint32 *tile = vram +
				patternSet * (RANGE_PATTERN_COUNT * RANGE_PATTERN_PAIR_WORDS) +
				patternIdx * RANGE_PATTERN_PAIR_WORDS;
			Sint32 row;

			syWordsFill(tile, 64, TEXT_CANVAS_BACKGROUND_WORD);
			for(row = 1; row <= 7; row++) {
				Sint32 srcPattern = patternIdx;

				if(patternSet == 1 && row <= 1) {
					continue;
				}
				if(patternSet == 0 && patternIdx > row - 2) {
					srcPattern = row - 2;
					if(srcPattern < 0) {
						srcPattern = 0;
					}
				}
				tile[row] = mnXorWord(
					tile[row], MenuBarPatternRows[srcPattern].firstTile);
				tile[row + 8] = mnXorWord(
					tile[row + 8], MenuBarPatternRows[srcPattern].secondTile);
			}
		}
	}
}

#include "window/range_icon_draw.c"

/* Draws the two page arrows (PageArrowTiles) into the two tiles at VRAM address
   `ofs`, sliding outward: the left arrow 2 or 3 pixels to the left, the
   right one as far to the right, eight ticks each. */
void mnPageArrowsDraw(Sint32 ofs)
{
	Sint32 phase = (TickCount >> 2) & 3;

	/* Phases 0 to 3 of a 16-tick cycle become shifts 2, 2, 3, 3. */
	if(phase > 2) {
		phase = 2;
	}
	if(phase <= 0) {
		phase = 1;
	}
	phase++;
	mnGraphicsMergeShiftedTileRows(
		(Uint32 *)(BG_CHAR_BLOCK(0) +
			(WINDOW_TILE_BORDER_TOP & TILE_NUMBER_MASK) * TILE_SIZE_4BPP),
		(Uint32 *)PageArrowTiles, (Uint32 *)ofs, -phase);
	mnGraphicsMergeShiftedTileRows(
		(Uint32 *)(BG_CHAR_BLOCK(0) +
			(WINDOW_TILE_BORDER_TOP_RIGHT & TILE_NUMBER_MASK) * TILE_SIZE_4BPP),
		(Uint32 *)(PageArrowTiles + TILE_SIZE_4BPP),
		(Uint32 *)(ofs + TILE_SIZE_4BPP), phase);
}

/* Copies the UI's ten digit tiles ('0' to '9', tiles 48 to 57) twice into
   `dst`, each pixel mapped through GraphicsExpandNibbleTable: first with
   colour 1 drawn as colour 10, then unchanged. */
void mnGraphicsExpandVRAMTilesByColTable(Uint16 *dst)
{
	Sint32 copy = 0;
	Sint32 paletteOfs = 0;
	Sint32 dstStep = 0;

	/* The first copy goes from byte 0, the second from byte 320, after the
	   first's ten tiles. */
	for(; copy <= 1; copy++) {
		Sint32 digit = 0;
		Sint32 dstOfs = (dstStep + copy) << 6;

		do {
			Uint16 *output = (Uint16 *)((Uint8 *)dst + dstOfs);
			Uint16 *src = (Uint16 *)(BG_CHAR_BLOCK(0) + '0' * TILE_SIZE_4BPP +
				digit * TILE_SIZE_4BPP);
			Sint32	column;

			for(column = 0; column <= 15; column++) {
				Uint32 packed = *src++;
				Uint32 expanded = 0;
				Sint32 nibble = 0;

				do {
					Uint32 col =
						GraphicsExpandNibbleTable[(packed & 0xf) + paletteOfs]
						<< (nibble * 4);
					nibble++;
					packed >>= 4;
					expanded |= col;
				} while(nibble <= 3);

				*output++ = expanded;
			}

			digit++;
			dstOfs += TILE_SIZE_4BPP;
		} while(digit <= 9);

		paletteOfs += PALETTE_COLORS;
		dstStep += 4;
	}
}

/* Builds icon `resource` (0 to 31) of IconMiscIconPointers, the table the
   status icons come from (mnIconDrawMisc), in a transfer block taken from
   HEAP_SLOT_ICON_TRANSFER, reloads VRAM cache entry `idx` with it and returns
   the entry's first tile. */
Sint32 mnStatIconLoadIntoSlot(Sint32 resource, Sint32 idx)
{
	Sint32		   result;
	ICON_TRANSFER *state;

	state = syHeapSlotAllocEWRAM(HEAP_SLOT_ICON_TRANSFER, sizeof(*state));
	mnIconDrawMisc(resource);
	result = syVRAMCacheReload(idx, (Sint32)state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return result;
}

#include "window/icon_load_into_slot.c"

/* Loads the portrait of character `member` (ISAAC to PIERS, 0 to 7; any higher
   value shows Isaac's) into VRAM cache entry `cacheSlot`, in PORTRAIT_PALETTE,
   and returns its first tile. */
Sint32 mnPortraitLoad(Uint32 member, Sint32 cacheSlot)
{
	Sint32 tile;
	Sint32 slot;
	Uint32 portrait;

	portrait = member;
	slot = cacheSlot;
	if(portrait > (Uint32)PIERS) {
		portrait = 0;
	}
	if(gmFlagGetStub(FLAG_CHILDHOOD) != 0) {
		switch(portrait) {
		case ISAAC:
			portrait = PORTRAIT_ISAAC_CHILD;
			break;
		case GARET:
			portrait = PORTRAIT_GARET_CHILD;
			break;
		}
	}
	mnIconLoadWithPalette(portrait, 0, &slot, &tile, PORTRAIT_PALETTE, 1);
	return tile;
}

/* Returns WindowSpritePointerTiles[0], whatever `idx` is. The battle
   engine's prompt reads it by stub call (btMessageWaitForAdvanceAt). */
Sint32 mnWindowSprGetPointerTiles(Sint32 idx)
{
	if(idx != 0)
		idx = 0;
	return WindowSpritePointerTiles[idx];
}

/* Copies the 4bpp decoder into RAM, calls it with the transfer's encoded source
   and byte buffer, then releases the copied code. */
UNUSED void mnIconDecompress(ICON_TRANSFER *state)
{
	RAM_CODE_LOAD_SLOT(HEAP_SLOT_RAM_CODE_TEMP, mnTileDecompress4bpp,
		TileDecompress4bppCodeSize);
	((void (*)(const void *, Uint8 *))((HEAP_STATE *)HeapSlots)
			->slots[HEAP_SLOT_RAM_CODE_TEMP])(state->encoded, state->input);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE_TEMP);
}

/* How the link timer's heading is drawn: Spanish and French at a pixel offset
   (mnTextDrawStringAtOffset), the others one tile a character
   (mnTextDrawStringInWindow). */
#define LINK_COUNTDOWN_DRAW_HEADING \
	BY_LANGUAGE(mnTextDrawStringInWindow, mnTextDrawStringInWindow, \
		mnTextDrawStringInWindow, mnTextDrawStringAtOffset, \
		mnTextDrawStringAtOffset, mnTextDrawStringInWindow)

/* Opens the link battle timer's window, 6 by 4 tiles at the top left, with its
   heading (LinkTimeLabelString), and returns it; the seconds go under the
   heading (mnBattleMenuUpdate), which keeps the window in
   BATTLE_MENU_STATE.tmrWin until the commands are chosen. */
WINDOW *mnLinkCreateCountdownLabelWindow(void)
{
	WINDOW *win = mnWindowCreate(0, 0, 6, 4, MENU_WINDOW_FLAGS);

	LINK_COUNTDOWN_DRAW_HEADING(LinkTimeLabelString, win, 0, 0);
	return win;
}

/* Decodes command icon `icon` (ResourceIdCommandIcons, 1 KB of 256-colour
   pixels) in a block taken from HEAP_SLOT_ICON_TRANSFER, reloads VRAM cache
   entry `slotIdx` with it and returns the entry's first tile. */
Sint32 mnCommandIconReloadSlot(Sint32 slotIdx, Sint32 icon)
{
	ICON_TRANSFER *buf =
		syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	Uint16 *ofsTbl = syResourceGetTableEntry(RESOURCE_COMMAND_ICONS);
	Uint8 **encoded = &buf->encoded;
	void   *tgt = (void *)((Uint32)ofsTbl + ofsTbl[icon]);
	Sint32	result;

	*encoded = tgt;
	syDecompressByteLZ(tgt, buf);
	result = syVRAMCacheReload(slotIdx, (Sint32)buf);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return result;
}

/* The command icons on BG1 share at most COMMAND_ICON_COLORS BG colours, given
   out in the order their pixels first use them; a colour met after that takes
   colour COMMAND_ICON_COLORS. A ICON_PALETTE slot holds ICON_PALETTE_SLOT_UNSET
   until its colour is given out. */
#define COMMAND_ICON_COLORS	  64
/* A command icon as BG1 tiles: 32 by 32 pixels, 16 tiles of 256 colours. */
#define COMMAND_ICON_BG_TILES (COMMAND_ICON_BYTES / TILE_SIZE_8BPP)

/* Loads command icon `glyph` as 8bpp BG tiles into character block 1 from
   64-byte tile `tile` on (COMMAND_ICON_BG_TILES, 32x32), for the command row's
   BG1. */
void mnBattleCommandRowLoadIconTiles(
	ICON_PALETTE *palette, Sint32 glyph, Sint32 tile)
{
	ICON_TRANSFER *decoded;
	Uint8		  *font;
	Uint8		  *remapped;
	Uint8		  *src;
	Uint8		  *dst;
	Sint32		   i;
	Uint32		   idx;

	decoded = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(*decoded));
	font = syResourceGetTableEntry((Uint32)RESOURCE_COMMAND_ICONS);
	decoded->encoded = font + ((Uint16 *)font)[glyph];
	syDecompressByteLZ(decoded->encoded, decoded);
	remapped = syHeapAlloc(COMMAND_ICON_BYTES);
	src = decoded->input;
	dst = remapped;
	for(i = 0; i < COMMAND_ICON_BYTES; i++) {
		idx = *src++;
		if(palette->slot[idx] == ICON_PALETTE_SLOT_UNSET) {
			palette->slot[idx] = palette->cnt;
			if(palette->cnt < COMMAND_ICON_COLORS) {
				((Uint16 *)BG_PLTT)[palette->cnt] = ((Uint16 *)OBJ_PLTT)[idx];
				palette->cnt++;
			}
		}
		*dst++ = palette->slot[idx];
	}
	syDMAXCopy(remapped, BG_CHAR_BLOCK(1) + tile * TILE_SIZE_8BPP,
		DMA_COPY_WORDS(COMMAND_ICON_BYTES / 4), REG_DMA3);
	syHeapRelease(remapped);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
}

#include "window/battle_command_row.c"

/* Clears BG0's vertical scroll (REG_BG0VOFS) and installs
   mnCommandRowPutBg1InFront as the VCount handler at line COMMAND_ROW_TOP * 8,
   as the command row does. Nothing calls it. */
UNUSED void mnGraphicsInstallBg1VCountHandler(void)
{
	IO_WRITE16(0, &REG_BG0VOFS);
	syIntFunction(IRQ_VCOUNT, COMMAND_ROW_TOP * 8, mnCommandRowPutBg1InFront);
}

#undef COMMAND_ICON_TILE_ROW_BYTES
#undef ROW_LAST_PIXEL_SHIFT
#undef COMMAND_ICON_COLORS
#undef COMMAND_ICON_BG_TILES
