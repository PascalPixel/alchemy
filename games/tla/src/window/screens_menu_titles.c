/* Pictures and command icons loaded for the menus, and BG1's priority. Part of
   screens.c. */

/* A row of a command icon's 8x8 tiles, four across, as a sprite holds them. */
#define COMMAND_ICON_TILE_ROW_BYTES (4 * TILE_SIZE_8BPP)
/* A menu heading: 64 by 16 pixels of 4bpp tiles, two 32x16 sprites of 8
   tiles. */
#define MENU_TITLE_BYTES			(64 * 16 / 2)
#define MENU_TITLE_HALF_TILES		8

/* Decompresses heading `picture` (MENU_TITLE_PICTURE) of MenuTitlePictures into
   VRAM slot `slot`, through a 1 KB staging block in HEAP_SLOT_TILE_SCRATCH. The
   heap block is not freed when `slot` is not a valid VRAM slot. */
void mnTitleLoad(Sint32 picture, Sint32 slot)
{
	void  *stagingBuf = syHeapSlotAllocEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
	Sint32 resource = MenuTitlePictures[picture];

	if(slot < VRAM_CACHE_ENTRY_COUNT) {
		syDecompressByteLZ((void *)resource, stagingBuf);
		syVRAMCacheLoad(slot, MENU_TITLE_BYTES, stagingBuf);
		syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	}
}

/* Shows heading `picture` of MenuTitlePictures as two 32x16 sprites side by
   side at (x, y) in `win`, at depth OAM_DEPTH_FRONT - 2, and returns the
   left one (the right one follows it in the window's list). */
WINDOW_SPR *mnTitleCreate(Sint32 picture, WINDOW *win, Sint32 x, Sint32 y)
{
	Sint32		slot;
	WINDOW_SPR *first;
	WINDOW_SPR *second;

	slot = syVRAMCacheFindFreeEntry();
	if(slot >= VRAM_CACHE_ENTRY_COUNT)
		return NULL;

	/* Neither sprite is checked for NULL. */
	mnTitleLoad(picture, slot);
	first = mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_32X16, win, x, y);
	first->depth = OAM_DEPTH_FRONT - 2;
	second = mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_32X16, win, x + 32, y);
	second->depth = OAM_DEPTH_FRONT - 2;
	/* The right half's tiles, after the left half's. */
	second->spr.oam.idx += MENU_TITLE_HALF_TILES;
	return first;
}

#ifndef MODERN
/* A current GCC refuses the volatile redeclaration and reads the one
   system/globals.h declares. Counted up by every VBlank. The ROM's callers here
   are compiled against this form, volatile where system/globals.h's is not. */
extern volatile Uint32 FrameCount;
#endif

/* Bobs the two sprites of a pair (see mnTitleCreate) up and down by
   MenuTitleBobOffsets, one step every four frames. */
void mnTitleBob(WINDOW_SPR *obj)
{
	Sint32 value;

	value = (Uint8)obj->y + MenuTitleBobOffsets[(FrameCount >> 2) & 7];
	obj->spr.oam.y = value;
	obj = obj->next;
	value = (Uint8)obj->y + MenuTitleBobOffsets[(FrameCount >> 2) & 7];
	obj->spr.oam.y = value;
}

/* Decompresses command icon `icon` (a 32x32 picture of 256 colours, 1 KB) into
   VRAM slot `slot`, remapping the colours of its top three rows of tiles by
   table when `remap` is nonzero. */
void mnCommandIconLoad(Sint32 icon, Sint32 slot, Sint32 remap)
{
	Sint32	size = COMMAND_ICON_BYTES;
	void   *buf = syHeapSlotAllocEWRAM(HEAP_SLOT_TILE_SCRATCH, size);
	Uint16 *ofsTbl = syResourceGetTableEntry(RESOURCE_COMMAND_ICONS);

	if(slot < VRAM_CACHE_ENTRY_COUNT) {
		syDecompressByteLZ((void *)((Uint32)ofsTbl + ofsTbl[icon]), buf);
		if(remap != 0)
			fdSprColRemapPixelsStub(buf, 3 * COMMAND_ICON_TILE_ROW_BYTES);
		syVRAMCacheLoad(slot, size, buf);
		syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	}
}

/* Shows command icon `icon` (see mnCommandIconLoad) as a 32x32 256-colour
   sprite at (x, y) in `win`, at depth OAM_DEPTH_FRONT - 4. Returns NULL when
   no VRAM slot is free. The settings menu's icons come through here
   (window_settings_menu.c). */
WINDOW_SPR *mnCommandIconCreate(
	Sint32 icon, Sint32 remap, WINDOW *win, Sint32 x, Sint32 y)
{
	Sint32		slot;
	WINDOW_SPR *spr;

	slot = syVRAMCacheFindFreeEntry();
	spr = NULL;
	if(slot != VRAM_CACHE_ENTRY_COUNT) {
		mnCommandIconLoad(icon, slot, remap);
		spr = mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_32X32, win, x, y);
		spr->spr.oam.fullCol = 1;
		spr->depth = OAM_DEPTH_FRONT - 4;
	}
	return spr;
}

/* Makes the sprite `obj` pulse: scales it about its centre by
   WindowSpritePulseScales (16.16, used as 8.8), one step every two ticks, and
   puts it at depth OAM_DEPTH_FRONT - 3. */
void mnWindowSprPulse(WINDOW_SPR *obj)
{
	Sint32 scale = WindowSpritePulseScales[(FrameCount >> 1) & 7] /
		(toFIXED(1) / Q_8_8(1));
	AFFINE_TRANSFORM effect;

	if(obj != 0) {
		effect.scaleX = scale;
		effect.scaleY = scale;
		effect.ang = 0;
		obj->spr.oam.matrix = syOAMMatrixAllocFromTransform(&effect);
		/* Affine mode 3 doubles the sprite's box, so it moves up and left by 16
		   pixels (-16 in the 16-bit x and 8-bit y) to keep its centre. */
		obj->spr.oam.affineMode = 3;
		obj->spr.oam.x = obj->x + (Uint16)-16;
		obj->spr.oam.y = (Uint8)obj->y + (Uint8)-16;
		obj->depth = OAM_DEPTH_FRONT - 3;
	}
}

/* Returns the Djinn tutorial's message on giving a Djinni: the second set's
   while FLAG_ALREADY_KNOWS_DJINN is set. */
Sint32 mnDjinnGetGiveTutorialMessage(void)
{
	if(syGameFlagGet(FLAG_ALREADY_KNOWS_DJINN))
		return (Sint32)MsgDjinnGiveTutorialB;
	return (Sint32)MsgDjinnGiveTutorialA;
}

#define RANGE_PATTERN_PAIR_BYTES (2 * TILE_SIZE_4BPP)
/* The first of the range icons' BG tiles, which mnRangeIconDraw draws as
   tilemap entries. */
#define RANGE_ICON_FIRST_TILE	 788

/* Builds the twelve range icons mnRangeIconDraw draws, as the 24 BG tiles from
   RANGE_ICON_FIRST_TILE: icon n is a centre tile and an end tile, filled with
   colour 4, then rows 1 to 7 marked by XOR with MenuBarPatternRows. The first
   set grows its pattern with the row, the second skips row 1. */
UNREAD_RESULT mnRangeIconBuildTiles(void)
{
	Sint32 patternSet;
	Sint32 patternIdx;

	for(patternSet = 0; patternSet < RANGE_PATTERN_SET_COUNT; patternSet++) {
		for(patternIdx = 0; patternIdx < RANGE_PATTERN_COUNT; patternIdx++) {
			Uint32 *tile = (Uint32 *)(patternSet *
					(RANGE_PATTERN_COUNT * RANGE_PATTERN_PAIR_BYTES) +
				patternIdx * RANGE_PATTERN_PAIR_BYTES +
				(Uint32)BG_CHAR_BLOCK(0) +
				RANGE_ICON_FIRST_TILE * TILE_SIZE_4BPP);
			Sint32	row;

			syWordsFillEntry(
				tile, RANGE_PATTERN_PAIR_BYTES, TEXT_CANVAS_BACKGROUND_WORD);
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

/* Draws range icon `pattern` across tiles (x, y) to (x + 2, y) of `win`'s
   interior: its end tile mirrored, its centre tile and its end tile, in the UI
   palette. */
UNREAD_RESULT mnRangeIconDraw(
	WINDOW *win, Sint32 x, Sint32 y, Sint32 pattern, Sint32 unused)
{
	Sint32 patternOfs = pattern * 2;
	Sint32 rightTile = patternOfs + WINDOW_TILE(RANGE_ICON_FIRST_TILE + 1);

	mnWindowSetTilemapEntry(win, TILE_HFLIP | rightTile, x, y, 0);
	mnWindowSetTilemapEntry(
		win, patternOfs + WINDOW_TILE(RANGE_ICON_FIRST_TILE), x + 1, y, 0);
	mnWindowSetTilemapEntry(win, rightTile, x + 2, y, 0);
}

/* A 4bpp tile row's eighth pixel, in the top four bits of its word. */
#define ROW_LAST_PIXEL_SHIFT 28

/* Merges two 8x8 4bpp tiles row by row into `output`: `second` moved `shift`
   pixels (right when positive, left when negative), drawn over `first`, which
   shows wherever the moved pixel is colour 0. */
void mnGraphicsMergeShiftedTileRows(
	Uint32 *first, Uint32 *second, Uint32 *output, Sint32 shift)
{
	Sint32 oppositeShift;
	Sint32 rightShift;
	Sint32 leftShift;
	Sint32 row;

	oppositeShift = -shift;
	row = 0;
	rightShift = oppositeShift * 4;
	leftShift = shift * 4;

	for(; row <= 7; row++) {
		Uint32 result;
		Uint32 shifted;
		Uint32 fallback;
		Sint32 pixel;

		shifted = *second++;
		result = 0;
		fallback = *first++;
		if(shift < 0)
			shifted >>= rightShift;
		else
			shifted <<= leftShift;

		for(pixel = 0; pixel < 8; pixel++) {
			result <<= 4;
			if(shifted >= 1u << ROW_LAST_PIXEL_SHIFT)
				result += shifted >> ROW_LAST_PIXEL_SHIFT;
			else
				result += fallback >> ROW_LAST_PIXEL_SHIFT;
			shifted <<= 4;
			fallback <<= 4;
		}

		*output++ = result;
	}
}

/* Draws the two page arrows (PageArrowTiles) into the two tiles at VRAM address
   `ofs`, sliding outward: the left arrow 2 or 3 pixels to the left, the
   right one as far to the right, eight ticks each. They are drawn over the
   border's top edge tile (WINDOW_TILE_BORDER_TOP) and top-right corner tile
   (WINDOW_TILE_BORDER_TOP_RIGHT). The battle menus call it each tick for
   LIST_PAGE_ARROW_VRAM; the field menus (by stub call) and the password pages
   of the title's Send call it for PAGE_ARROW_VRAM. */
void mnPageArrowsDraw(Sint32 ofs)
{
	Sint32 phase = (TickCount >> 2) & 3;

	if(phase > 2) {
		phase = 2;
	}
	if(phase <= 0) {
		phase = 1;
	}
	phase = phase + 1;
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

/* mnPageArrowsDraw for the Status screen's own arrows: both slide over the
   window's fill tile (WINDOW_TILE_WINDOW_FILL) rather than the border's. The
   battle's status screen calls it each tick. */
void mnBattleStatDrawPageArrows(Sint32 ofs)
{
	Sint32 phase = (TickCount >> 2) & 3;

	if(phase > 2) {
		phase = 2;
	}
	if(phase <= 0) {
		phase = 1;
	}
	phase = phase + 1;
	mnGraphicsMergeShiftedTileRows(
		(Uint32 *)(BG_CHAR_BLOCK(0) +
			(WINDOW_TILE_WINDOW_FILL & TILE_NUMBER_MASK) * TILE_SIZE_4BPP),
		(Uint32 *)PageArrowTiles, (Uint32 *)ofs, -phase);
	mnGraphicsMergeShiftedTileRows(
		(Uint32 *)(BG_CHAR_BLOCK(0) +
			(WINDOW_TILE_WINDOW_FILL & TILE_NUMBER_MASK) * TILE_SIZE_4BPP),
		(Uint32 *)(PageArrowTiles + TILE_SIZE_4BPP),
		(Uint32 *)(ofs + TILE_SIZE_4BPP), phase);
}

/* Copies the UI's ten digit tiles ('0' to '9', tiles 48 to 57) twice into
   `dst`, each pixel mapped through GraphicsExpandNibbleTable: first with
   colour 1 drawn as colour 10, then unchanged. */
void mnGraphicsExpandVRAMTilesByColTable(Uint16 *dst)
{
	Sint32 set = 0;
	Sint32 paletteOfs = 0;
	Sint32 setStep = 0;

	/* The second set of ten digit tiles goes right after the first. */
	for(; set <= 1; set++) {
		Sint32 digit = 0;
		Sint32 dstOfs = (setStep + set) << 6;

		do {
			Uint16 *output = (Uint16 *)((Uint8 *)dst + dstOfs);
			Uint16 *src = (Uint16 *)(BG_CHAR_BLOCK(0) + '0' * TILE_SIZE_4BPP +
				(digit << 5));
			Sint32	column;

			for(column = 0; column < TILE_SIZE_4BPP / 2; column++) {
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
		setStep += 4;
	}
}

/* Builds icon `resource` (0 to 31) of IconMiscIconPointers, the status
   patterns (mnIconDrawMisc), in a transfer block taken from
   HEAP_SLOT_ICON_TRANSFER, reloads VRAM cache entry `idx` with it and returns
   the entry's first tile. */
Sint32 mnMiscIconLoadIntoSlot(Sint32 resource, Sint32 idx)
{
	Sint32		   result;
	ICON_TRANSFER *state;

	state = syHeapSlotAllocEWRAM(HEAP_SLOT_ICON_TRANSFER, sizeof(*state));
	mnIconDrawMisc(resource);
	result = syVRAMCacheReload(idx, (Sint32)state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return result;
}

/* Builds the icon of inventory item `resource` (the count above one and both
   marks) in a transfer block taken from HEAP_SLOT_ICON_TRANSFER, reloads VRAM
   cache entry `idx` with it and returns the entry's first tile. */
Sint32 mnItemIconLoadIntoSlot(Sint32 resource, Sint32 idx)
{
	Sint32		   result;
	ICON_TRANSFER *state;

	state = syHeapSlotAllocEWRAM(HEAP_SLOT_ICON_TRANSFER, sizeof(*state));
	mnItemIconCompose(
		resource, ICON_COUNT_ABOVE_ONE | ICON_EQUIPPED | ICON_BROKEN);
	result = syVRAMCacheReload(idx, (Sint32)state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return result;
}

/* Builds the icon of ability `resource` in a transfer block taken from
   HEAP_SLOT_ICON_TRANSFER, reloads VRAM cache entry `idx` with it and returns
   the entry's first tile. */
Sint32 mnAbilityIconLoadIntoSlot(Sint32 resource, Sint32 idx)
{
	Sint32		   tile;
	Sint32		   slot;
	Sint32		   result;
	ICON_TRANSFER *state;

	state = syHeapSlotAllocEWRAM(HEAP_SLOT_ICON_TRANSFER, sizeof(*state));
	slot = idx;
	mnAbilityBuildIconTiles(resource, 0, &slot, &tile, 1);
	result = syVRAMCacheReload(idx, (Sint32)state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return result;
}

/* Loads the portrait of character `member` (ISAAC to PIERS, 0 to 7; any higher
   value shows Isaac's) into VRAM cache entry `cacheSlot`, in PORTRAIT_PALETTE,
   and returns its first tile. */
Sint32 mnPortraitLoad(Uint32 member, Sint32 cacheSlot)
{
	Sint32 tile;

	if(member > PIERS)
		member = ISAAC;
	mnIconLoadWithPalette(mnPortraitGetForSpeaker(member), 0, &cacheSlot, &tile,
		PORTRAIT_PALETTE, 1);
	return tile;
}

/* The 4bpp decoder mnIconDecompress copies into RAM. */
extern Uint8 mnTileDecompress4bppCode[];
/* The decoder's byte count, as this symbol's address (ld_script.ld). */
extern Uint8 TileDecompress4bppCodeSize[];

/* Returns WindowFixedTileBlocks[0], whatever no is. */
Sint32 mnWindowSprGetPointerTiles(Sint32 no)
{
	if(no != 0)
		no = 0;
	return WindowFixedTileBlocks[no];
}

/* Copies the 4bpp decoder into RAM, calls it with the transfer's encoded source
   and byte buffer, then releases the copied code. */
UNUSED void mnIconDecompress(ICON_TRANSFER *state)
{
	void (*routine)(void *, void *);

	RAM_CODE_LOAD(
		routine, mnTileDecompress4bppCode, TileDecompress4bppCodeSize);
	routine(state->encoded, state->input);
	syHeapRelease(routine);
}

/* Opens the link battle timer's 6x4 window at the upper left, labelled "TIME",
   and returns it; mnBattleMenuUpdateEachTick draws the seconds. */
Sint32 mnLinkCreateCountdownLabelWindow(void)
{
	Sint32 win = (Sint32)mnWindowCreate(
		0, 0, 6, 4, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) || \
	(LANGUAGE == LANGUAGE_ITALIAN)
	mnTextDrawStringAtOffset(LinkTimeLabelString, (WINDOW *)win, 0, 0);
#else
	mnTextDrawStringInWindow(LinkTimeLabelString, win, 0, 0);
#endif
	return win;
}

/* Decodes command icon `icon` (RESOURCE_COMMAND_ICONS, 1 KB of 256-colour
   pixels) in a block taken from HEAP_SLOT_ICON_TRANSFER, reloads VRAM cache
   entry `slotIdx` with it and returns the entry's first tile. */
Sint32 mnCommandIconReloadSlot(Sint32 slotIdx, Sint32 icon)
{
	ICON_TRANSFER *buf =
		syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	Uint16 *ofsTbl = syResourceGetTableEntry(RESOURCE_COMMAND_ICONS);
	Uint8 **encoded = &buf->encoded;
	void   *tgt = (void *)((Uint32)ofsTbl + ofsTbl[icon]);
	Sint32	tile;

	*encoded = tgt;
	syDecompressByteLZ(tgt, buf);
	tile = syVRAMCacheReload(slotIdx, (Sint32)buf);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return tile;
}

/* The command icons on BG1 share at most 64 BG colours, given out in the order
   their pixels first use them; a colour met after that takes colour 64. */
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

	decoded = (ICON_TRANSFER *)syHeapSlotAlloc(
		HEAP_SLOT_ICON_TRANSFER, sizeof(*decoded));
	font = syResourceGetTableEntry(RESOURCE_COMMAND_ICONS);
	decoded->encoded = font + ((Uint16 *)font)[glyph];
	syDecompressByteLZ(decoded->encoded, decoded);
	remapped = syHeapAlloc(COMMAND_ICON_BYTES);
	src = decoded->input;
	dst = remapped;
	for(i = 0; i < COMMAND_ICON_BYTES; i++) {
		idx = *src++;
		if(palette->slot[idx] == GLYPH_PALETTE_SLOT_FREE) {
			palette->slot[idx] = palette->cnt;
			if(palette->cnt < COMMAND_ICON_COLORS) {
				BG_PLTT[palette->cnt] = OBJ_PLTT[idx];
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

/* Loads the command row's icon `idx` as command icon `icon`: as BG1 tiles
   from tile index * COMMAND_ICON_BG_TILES (mnBattleCommandRowLoadIconTiles),
   and into the row's shared VRAM cache entry for its 32x32 sprite, which
   mnBattleCommandRowRun shows only for the selected icon. */
void mnBattleCommandRowLoadIcon(
	BATTLE_COMMAND_ROW_STATE *state, Sint32 idx, Uint32 icon)
{
	BATTLE_COMMAND_ROW_ICON *entry = &state->entries[idx];

	mnBattleCommandRowLoadIconTiles(
		&state->pal, icon, idx * COMMAND_ICON_BG_TILES);
	state->entries[idx].kind = icon;
	MENU_SPR_INIT(entry->spr, OBJ_SIZE_32X32 | OBJ_256_COLORS);
	entry->spr.oam.f.tile =
		mnCommandIconReloadSlot(state->entries[idx].resource, icon);
}

/* Puts BG1 behind the other layers (priority 3). The command row runs it as an
   event each VBlank, in the VBlank pass, with mnCommandRowPutBg1InFront at line
   COMMAND_ROW_TOP * 8, so that BG1 is behind above the strip and in front from
   it down. */
void mnCommandRowPutBg1Behind(void)
{
	Uint32 value = REG_BG1CNT;

	value &= ~BGCNT_PRIORITY(3);
	value |= BGCNT_PRIORITY(3);
	REG_BG1CNT = value;
}

/* Brings BG1 to the front (priority 0): the command row's VCount handler, run
   at line COMMAND_ROW_TOP * 8, the top of its icon strip. */
void mnCommandRowPutBg1InFront(void)
{
	Uint32 value = REG_BG1CNT;

	value &= ~BGCNT_PRIORITY(3);
	REG_BG1CNT = value;
}

/* Clears BG0's vertical scroll (REG_BG0VOFS) and installs
   mnCommandRowPutBg1InFront, which clears BG1's priority bits, as the VCount
   handler at line COMMAND_ROW_TOP * 8. Nothing calls it. */
UNUSED void mnGraphicsInstallBg1VCountHandler(void)
{
	syIOWrite16(0, &REG_BG0VOFS);
	syIntFunction(IRQ_VCOUNT, COMMAND_ROW_TOP * 8, mnCommandRowPutBg1InFront);
}

#undef COMMAND_ICON_TILE_ROW_BYTES
#undef ROW_LAST_PIXEL_SHIFT
#undef COMMAND_ICON_COLORS
#undef COMMAND_ICON_BG_TILES
