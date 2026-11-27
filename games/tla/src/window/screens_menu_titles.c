/* @file */
/* Pictures and command icons loaded for the menus, and BG1's priority. */
/* Part of screens.c. */

/* A row of a command icon's 8x8 tiles, four across, as a sprite holds them. */
#define COMMAND_ICON_TILE_ROW_BYTES (4 * TILE_SIZE_8BPP)
/* A menu heading: 64 by 16 pixels of 4bpp tiles, two 32x16 sprites of 8 tiles.
 */
#define MENU_TITLE_BYTES	  (64 * 16 / 2)
#define MENU_TITLE_HALF_TILES 8

/* Decompresses heading `picture` (MENU_TITLE_PICTURE) of */
/* MenuTitlePictures into VRAM slot `slot`, through a 1 KB staging block in */
/* HEAP_SLOT_TILE_SCRATCH. The heap block is not freed when `slot` is not a */
/* valid VRAM slot. */
void mnTitleLoad(s32 picture, s32 slot) {
	void *stagingBuffer = syHeapSlotAllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
	s32 resource = MenuTitlePictures[picture];

	if(slot < VRAM_CACHE_ENTRY_COUNT) {
		syDecompressByteLz((void *)resource, stagingBuffer);
		syVramCacheLoad(slot, MENU_TITLE_BYTES, stagingBuffer);
		syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	}
}

/* Shows heading `picture` of MenuTitlePictures as two 32x16 sprites side by */
/* side at (x, y) in `window`, at depth OAM_DEPTH_FRONT - 2, and returns the */
/* left one (the right one follows it in the window's list). */
WINDOW_SPR *mnTitleCreate(s32 picture, WINDOW *window, s32 x, s32 y) {
	s32 slot;
	WINDOW_SPR *first;
	WINDOW_SPR *second;

	slot = syVramCacheFindFreeEntry();
	if(slot >= VRAM_CACHE_ENTRY_COUNT)
		return NULL;

	/* Neither sprite is checked for NULL. */
	mnTitleLoad(picture, slot);
	first = mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_32X16, window, x, y);
	first->depth = OAM_DEPTH_FRONT - 2;
	second =
		mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_32X16, window, x + 32, y);
	second->depth = OAM_DEPTH_FRONT - 2;
	/* The right half's tiles, after the left half's. */
	second->spr.oam.idx += MENU_TITLE_HALF_TILES;
	return first;
}

#ifndef MODERN
/* A current GCC refuses the volatile redeclaration and reads the one */
/* system/globals.h declares. */
/* Counted up by every VBlank. The ROM's callers here are compiled against */
/* this form, volatile where system/globals.h's is not. */
extern volatile u32 VBlankCount;
#endif

/* Bobs the two sprites of a pair (see mnTitleCreate) up and down */
/* by MenuTitle_BobOffsets, one step every four VBlanks. */
void mnTitleBob(WINDOW_SPR *obj) {
	s32 value;

	value = (u8)obj->y + MenuTitle_BobOffsets[(VBlankCount >> 2) & 7];
	obj->spr.oam.y = value;
	obj = obj->next;
	value = (u8)obj->y + MenuTitle_BobOffsets[(VBlankCount >> 2) & 7];
	obj->spr.oam.y = value;
}

/* Decompresses command icon `icon` (a 32x32 picture of 256 colours, 1 KB) */
/* into VRAM slot `slot`, remapping the colours of its top three rows of tiles
 */
/* by table when `remap` is nonzero. */
void mnCommandIconLoad(s32 icon, s32 slot, s32 remap) {
	s32 size = COMMAND_ICON_BYTES;
	void *buffer = syHeapSlotAllocEwram(HEAP_SLOT_TILE_SCRATCH, size);
	u16 *offsetTable = syResourceGetTableEntry(RESOURCE_COMMAND_ICONS);

	if(slot < VRAM_CACHE_ENTRY_COUNT) {
		syDecompressByteLz((void *)((u32)offsetTable + offsetTable[icon]),
						   buffer);
		if(remap != 0)
			fdSprColRemapPixelsStub(buffer, 3 * COMMAND_ICON_TILE_ROW_BYTES);
		syVramCacheLoad(slot, size, buffer);
		syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	}
}

/* Shows command icon `icon` (see mnCommandIconLoad) as a 32x32 */
/* 256-colour sprite at (x, y) in `window`, at depth OAM_DEPTH_FRONT - 4. */
/* Returns NULL when no VRAM slot is free. The settings menu's icons come */
/* through here (window_settings_menu.c). */
WINDOW_SPR *mnCommandIconCreate(s32 icon, s32 remap, WINDOW *window, s32 x,
								s32 y) {
	s32 slot;
	WINDOW_SPR *sprite;

	slot = syVramCacheFindFreeEntry();
	sprite = NULL;
	if(slot != VRAM_CACHE_ENTRY_COUNT) {
		mnCommandIconLoad(icon, slot, remap);
		sprite =
			mnWindowSprCreate(slot, WINDOW_SPRITE_SIZE_32X32, window, x, y);
		sprite->spr.oam.fullCol = 1;
		sprite->depth = OAM_DEPTH_FRONT - 4;
	}
	return sprite;
}

/* Makes the sprite `obj` pulse: scales it about its centre by */
/* WindowSprite_PulseScales (16.16, used as 8.8), one step every two frames, */
/* and puts it at depth OAM_DEPTH_FRONT - 3. */
void mnWindowSprPulse(WINDOW_SPR *obj) {
	s32 scale = WindowSprite_PulseScales[(VBlankCount >> 1) & 7] /
				(FIXED_ONE / Q_8_8(1));
	AFFINE_TRANSFORM effect;

	if(obj != 0) {
		effect.scaleX = scale;
		effect.scaleY = scale;
		effect.ang = 0;
		obj->spr.oam.matrix = syOamMatrixAllocFromTransform(&effect);
		/* Affine mode 3 doubles the sprite's box, so it moves up and left by 16
		 */
		/* pixels (-16 in the 16-bit x and 8-bit y) to keep its centre. */
		obj->spr.oam.affineMode = 3;
		obj->spr.oam.x = obj->x + (u16)-16;
		obj->spr.oam.y = (u8)obj->y + (u8)-16;
		obj->depth = OAM_DEPTH_FRONT - 3;
	}
}

/* Returns the Djinn tutorial's message on giving a Djinni: the second set's */
/* while FLAG_ALREADY_KNOWS_DJINN is set. */
s32 mnDjinnGetGiveTutorialMessage(void) {
	if(syGameFlagGet(FLAG_ALREADY_KNOWS_DJINN))
		return (s32)MsgDjinnGiveTutorialB;
	return (s32)MsgDjinnGiveTutorialA;
}

#define RANGE_PATTERN_PAIR_BYTES (2 * TILE_SIZE_4BPP)
/* The first of the range icons' BG tiles, which mnRangeIconDraw */
/* draws as tilemap entries. */
#define RANGE_ICON_FIRST_TILE 788

/* Builds the twelve range icons mnRangeIconDraw draws, as the 24 BG */
/* tiles from RANGE_ICON_FIRST_TILE: icon n is a centre tile and an end tile, */
/* filled with colour 4, then rows 1 to 7 marked by XOR with */
/* Menu_BarPatternRows. The first set grows its pattern with the row, the */
/* second skips row 1. */
UNREAD_RESULT mnRangeIconBuildTiles(void) {
	s32 patternSet;
	s32 patternIndex;

	for(patternSet = 0; patternSet < RANGE_PATTERN_SET_COUNT; patternSet++) {
		for(patternIndex = 0; patternIndex < RANGE_PATTERN_COUNT;
			patternIndex++) {
			u32 *tile = (u32 *)(patternSet * (RANGE_PATTERN_COUNT *
											  RANGE_PATTERN_PAIR_BYTES) +
								patternIndex * RANGE_PATTERN_PAIR_BYTES +
								(u32)BG_CHAR_BLOCK(0) +
								RANGE_ICON_FIRST_TILE * TILE_SIZE_4BPP);
			s32 row;

			syWordsFillEntry(tile, RANGE_PATTERN_PAIR_BYTES,
							 TEXT_CANVAS_BACKGROUND_WORD);
			for(row = 1; row <= 7; row++) {
				s32 sourcePattern = patternIndex;

				if(patternSet == 1 && row <= 1) {
					continue;
				}
				if(patternSet == 0 && patternIndex > row - 2) {
					sourcePattern = row - 2;
					if(sourcePattern < 0) {
						sourcePattern = 0;
					}
				}
				tile[row] = XorWord(
					tile[row], Menu_BarPatternRows[sourcePattern].firstTile);
				tile[row + 8] =
					XorWord(tile[row + 8],
							Menu_BarPatternRows[sourcePattern].secondTile);
			}
		}
	}
}

/* Draws range icon `pattern` across tiles (x, y) to (x + 2, y) of `window`'s */
/* interior: its end tile mirrored, its centre tile and its end tile, in the UI
 */
/* palette. */
UNREAD_RESULT mnRangeIconDraw(WINDOW *window, s32 x, s32 y, s32 pattern,
							  s32 unused) {
	s32 patternOffset = pattern * 2;
	s32 rightTile = patternOffset + WINDOW_TILE(RANGE_ICON_FIRST_TILE + 1);

	mnWindowSetTilemapEntry(window, TILE_HFLIP | rightTile, x, y, 0);
	mnWindowSetTilemapEntry(window,
							patternOffset + WINDOW_TILE(RANGE_ICON_FIRST_TILE),
							x + 1, y, 0);
	mnWindowSetTilemapEntry(window, rightTile, x + 2, y, 0);
}

/* A 4bpp tile row's eighth pixel, in the top four bits of its word. */
#define ROW_LAST_PIXEL_SHIFT 28

/* Merges two 8x8 4bpp tiles row by row into `output`: `second` moved `shift` */
/* pixels (right when positive, left when negative), drawn over `first`, which
 */
/* shows wherever the moved pixel is colour 0. */
void mnGraphicsMergeShiftedTileRows(u32 *first, u32 *second, u32 *output,
									s32 shift) {
	s32 oppositeShift;
	s32 rightShift;
	s32 leftShift;
	s32 row;

	oppositeShift = -shift;
	row = 0;
	rightShift = oppositeShift * 4;
	leftShift = shift * 4;

	for(; row <= 7; row++) {
		u32 result;
		u32 shifted;
		u32 fallback;
		s32 pixel;

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

/* Draws the two page arrows (PageArrowTiles) into the two tiles at */
/* VRAM address `offset`, sliding outward: the left arrow 2 or 3 pixels to */
/* the left, the right one as far to the right, eight frames each. They are */
/* drawn over the border's top edge tile (WINDOW_TILE_BORDER_TOP) and top-right
 */
/* corner tile (WINDOW_TILE_BORDER_TOP_RIGHT). The battle menus call it each */
/* frame for LIST_PAGE_ARROW_VRAM; the field menus (by stub call) and the */
/* password pages of the title's Send call it for PAGE_ARROW_VRAM. */
void mnPageArrowsDraw(s32 offset) {
	s32 phase = (FrameCount >> 2) & 3;

	if(phase > 2) {
		phase = 2;
	}
	if(phase <= 0) {
		phase = 1;
	}
	phase = phase + 1;
	mnGraphicsMergeShiftedTileRows(
		(u32 *)(BG_CHAR_BLOCK(0) +
				(WINDOW_TILE_BORDER_TOP & TILE_NUMBER_MASK) * TILE_SIZE_4BPP),
		(u32 *)PageArrowTiles, (u32 *)offset, -phase);
	mnGraphicsMergeShiftedTileRows(
		(u32 *)(BG_CHAR_BLOCK(0) +
				(WINDOW_TILE_BORDER_TOP_RIGHT & TILE_NUMBER_MASK) *
					TILE_SIZE_4BPP),
		(u32 *)(PageArrowTiles + TILE_SIZE_4BPP),
		(u32 *)(offset + TILE_SIZE_4BPP), phase);
}

/* mnPageArrowsDraw for the Status screen's own arrows: both slide over the */
/* window's fill tile (WINDOW_TILE_WINDOW_FILL) rather than the border's. The */
/* battle's status screen calls it each frame. */
void mnBattleStatDrawPageArrows(s32 offset) {
	s32 phase = (FrameCount >> 2) & 3;

	if(phase > 2) {
		phase = 2;
	}
	if(phase <= 0) {
		phase = 1;
	}
	phase = phase + 1;
	mnGraphicsMergeShiftedTileRows(
		(u32 *)(BG_CHAR_BLOCK(0) +
				(WINDOW_TILE_WINDOW_FILL & TILE_NUMBER_MASK) * TILE_SIZE_4BPP),
		(u32 *)PageArrowTiles, (u32 *)offset, -phase);
	mnGraphicsMergeShiftedTileRows(
		(u32 *)(BG_CHAR_BLOCK(0) +
				(WINDOW_TILE_WINDOW_FILL & TILE_NUMBER_MASK) * TILE_SIZE_4BPP),
		(u32 *)(PageArrowTiles + TILE_SIZE_4BPP),
		(u32 *)(offset + TILE_SIZE_4BPP), phase);
}

/* Copies the UI's ten digit tiles ('0' to '9', tiles 48 to 57) twice into */
/* `dst`, each pixel mapped through Graphics_ExpandNibbleTable: first with */
/* colour 1 drawn as colour 10, then unchanged. */
void mnGraphicsExpandVramTilesByColTable(u16 *dst) {
	s32 set = 0;
	s32 paletteOffset = 0;
	s32 setStep = 0;

	/* The second set of ten digit tiles goes right after the first. */
	for(; set <= 1; set++) {
		s32 digit = 0;
		s32 destinationOffset = (setStep + set) << 6;

		do {
			u16 *output = (u16 *)((u8 *)dst + destinationOffset);
			u16 *source =
				(u16 *)(BG_CHAR_BLOCK(0) + '0' * TILE_SIZE_4BPP + (digit << 5));
			s32 column;

			for(column = 0; column < TILE_SIZE_4BPP / 2; column++) {
				u32 packed = *source++;
				u32 expanded = 0;
				s32 nibble = 0;

				do {
					u32 color = Graphics_ExpandNibbleTable[(packed & 0xf) +
														   paletteOffset]
								<< (nibble * 4);
					nibble++;
					packed >>= 4;
					expanded |= color;
				} while(nibble <= 3);

				*output++ = expanded;
			}

			digit++;
			destinationOffset += TILE_SIZE_4BPP;
		} while(digit <= 9);

		paletteOffset += PALETTE_COLORS;
		setStep += 4;
	}
}

/* Builds icon `resource` (0 to 31) of Icon_MiscIconPointers, the status */
/* patterns (mnIconDrawMisc), in a transfer block taken from */
/* HEAP_SLOT_ICON_TRANSFER, reloads VRAM cache entry `index` with it and */
/* returns the entry's first tile. */
s32 mnMiscIconLoadIntoSlot(s32 resource, s32 index) {
	s32 result;
	ICON_TRANSFER *state;

	state = syHeapSlotAllocEwram(HEAP_SLOT_ICON_TRANSFER, sizeof(*state));
	mnIconDrawMisc(resource);
	result = syVramCacheReload(index, (s32)state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return result;
}

/* Builds the icon of inventory item `resource` (the count above one and both */
/* marks) in a transfer block taken from HEAP_SLOT_ICON_TRANSFER, reloads VRAM
 */
/* cache entry `index` with it and returns the entry's first tile. */
s32 mnItemIconLoadIntoSlot(s32 resource, s32 index) {
	s32 result;
	ICON_TRANSFER *state;

	state = syHeapSlotAllocEwram(HEAP_SLOT_ICON_TRANSFER, sizeof(*state));
	mnItemIconCompose(resource,
					  ICON_COUNT_ABOVE_ONE | ICON_EQUIPPED | ICON_BROKEN);
	result = syVramCacheReload(index, (s32)state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return result;
}

/* Builds the icon of ability `resource` in a transfer block taken from */
/* HEAP_SLOT_ICON_TRANSFER, reloads VRAM cache entry `index` with it and */
/* returns the entry's first tile. */
s32 mnAbilityIconLoadIntoSlot(s32 resource, s32 index) {
	s32 tile;
	s32 slot;
	s32 result;
	ICON_TRANSFER *state;

	state = syHeapSlotAllocEwram(HEAP_SLOT_ICON_TRANSFER, sizeof(*state));
	slot = index;
	mnAbilityBuildIconTiles(resource, 0, &slot, &tile, 1);
	result = syVramCacheReload(index, (s32)state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return result;
}

/* Loads the portrait of character `member` (ISAAC to PIERS, 0 to 7; any higher
 */
/* value shows Isaac's) into VRAM cache entry `cacheSlot`, in PORTRAIT_PALETTE,
 */
/* and returns its first tile. */
s32 mnPortraitLoad(u32 member, s32 cacheSlot) {
	s32 tile;

	if(member > PIERS)
		member = ISAAC;
	mnIconLoadWithPalette(mnPortraitGetForSpeaker(member), 0, &cacheSlot, &tile,
						  PORTRAIT_PALETTE, 1);
	return tile;
}

/* The 4bpp decoder mnIconDecompress copies into RAM. */
extern u8 Tile_Decompress4bppCode[];
/* The decoder's byte count, as this symbol's address (ld_script.ld). */
extern u8 Tile_Decompress4bppCodeSize[];

/* Returns Window_FixedTileBlocks[0], whatever no is. */
s32 mnWindowSprGetPointerTiles(s32 no) {
	if(no != 0)
		no = 0;
	return Window_FixedTileBlocks[no];
}

/* Copies the 4bpp decoder into RAM, calls it with the transfer's encoded */
/* source and byte buffer, then releases the copied code. */
UNUSED void mnIconDecompress(ICON_TRANSFER *state) {
	void (*routine)(void *, void *);

	syRamCodeLoad(routine, Tile_Decompress4bppCode,
				  Tile_Decompress4bppCodeSize);
	routine(state->encoded, state->input);
	syHeapRelease(routine);
}

/* Opens the link battle timer's 6x4 window at the upper left, labelled */
/* "TIME", and returns it; mnBattleMenuUpdateEachFrame draws the seconds. */
s32 mnLinkCreateCountdownLabelWindow(void) {
	s32 win = (s32)mnWindowCreate(0, 0, 6, 4,
								  WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
	(LANGUAGE == LANGUAGE_ITALIAN)
	mnTextDrawStringAtOffset(Link_TimeLabelString, (WINDOW *)win, 0, 0);
#else
	mnTextDrawStringInWindow(Link_TimeLabelString, win, 0, 0);
#endif
	return win;
}

/* Decodes command icon `icon` (RESOURCE_COMMAND_ICONS, 1 KB of 256-colour */
/* pixels) in a block taken from HEAP_SLOT_ICON_TRANSFER, reloads VRAM cache */
/* entry `slotIndex` with it and returns the entry's first tile. */
s32 mnCommandIconReloadSlot(s32 slotIndex, s32 icon) {
	ICON_TRANSFER *buffer =
		syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	u16 *offsetTable = syResourceGetTableEntry(RESOURCE_COMMAND_ICONS);
	u8 **encoded = &buffer->encoded;
	void *target = (void *)((u32)offsetTable + offsetTable[icon]);
	s32 tile;

	*encoded = target;
	syDecompressByteLz(target, buffer);
	tile = syVramCacheReload(slotIndex, (s32)buffer);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return tile;
}

/* The command icons on BG1 share at most 64 BG colours, given out in the order
 */
/* their pixels first use them; a colour met after that takes colour 64. */
#define COMMAND_ICON_COLORS 64
/* A command icon as BG1 tiles: 32 by 32 pixels, 16 tiles of 256 colours. */
#define COMMAND_ICON_BG_TILES (COMMAND_ICON_BYTES / TILE_SIZE_8BPP)

/* Loads command icon `glyph` as 8bpp BG tiles into character block 1 from */
/* 64-byte tile `tile` on (COMMAND_ICON_BG_TILES, 32x32), for the command row's
 */
/* BG1. */
void mnBattleCommandRowLoadIconTiles(ICON_PALETTE *palette, s32 glyph,
									 s32 tile) {
	ICON_TRANSFER *decoded;
	u8 *font;
	u8 *remapped;
	u8 *source;
	u8 *destination;
	s32 i;
	u32 index;

	decoded = (ICON_TRANSFER *)syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER,
											   sizeof(*decoded));
	font = syResourceGetTableEntry(RESOURCE_COMMAND_ICONS);
	decoded->encoded = font + ((u16 *)font)[glyph];
	syDecompressByteLz(decoded->encoded, decoded);
	remapped = syHeapAlloc(COMMAND_ICON_BYTES);
	source = decoded->input;
	destination = remapped;
	for(i = 0; i < COMMAND_ICON_BYTES; i++) {
		index = *source++;
		if(palette->slot[index] == GLYPH_PALETTE_SLOT_FREE) {
			palette->slot[index] = palette->cnt;
			if(palette->cnt < COMMAND_ICON_COLORS) {
				BG_PLTT[palette->cnt] = OBJ_PLTT[index];
				palette->cnt++;
			}
		}
		*destination++ = palette->slot[index];
	}
	syDMAXCopy(remapped, BG_CHAR_BLOCK(1) + tile * TILE_SIZE_8BPP,
			   DMA_COPY_WORDS(COMMAND_ICON_BYTES / 4), REG_DMA3);
	syHeapRelease(remapped);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
}

/* Loads the command row's icon `index` as command icon `icon`: as BG1 tiles */
/* from tile index * COMMAND_ICON_BG_TILES (mnBattleCommandRowLoadIconTiles), */
/* and into the row's shared VRAM cache entry for its 32x32 sprite, which */
/* mnBattleCommandRowRun shows only for the selected icon. */
void mnBattleCommandRowLoadIcon(BATTLE_COMMAND_ROW_STATE *state, s32 index,
								u32 icon) {
	BATTLE_COMMAND_ROW_ICON *entry = &state->entries[index];

	mnBattleCommandRowLoadIconTiles(&state->pal, icon,
									index * COMMAND_ICON_BG_TILES);
	state->entries[index].kind = icon;
	syMenuSprInit(entry->spr, OBJ_SIZE_32X32 | OBJ_256_COLORS);
	entry->spr.oam.f.tile =
		mnCommandIconReloadSlot(state->entries[index].resource, icon);
}

/* Puts BG1 behind the other layers (priority 3). The command row runs it as */
/* a task each VBlank, in the VBlank pass, with mnCommandRowPutBg1InFront at */
/* line COMMAND_ROW_TOP * 8, so that BG1 is behind above the strip and in front
 */
/* from it down. */
void mnCommandRowPutBg1Behind(void) {
	u32 value = REG_BG1CNT;

	value &= ~BGCNT_PRIORITY(3);
	value |= BGCNT_PRIORITY(3);
	REG_BG1CNT = value;
}

/* Brings BG1 to the front (priority 0): the command row's VCount handler, */
/* run at line COMMAND_ROW_TOP * 8, the top of its icon strip. */
void mnCommandRowPutBg1InFront(void) {
	u32 value = REG_BG1CNT;

	value &= ~BGCNT_PRIORITY(3);
	REG_BG1CNT = value;
}

/* Clears BG0's vertical scroll (REG_BG0VOFS) and installs */
/* mnCommandRowPutBg1InFront, which clears BG1's priority bits, as the VCount */
/* handler at line COMMAND_ROW_TOP * 8. Nothing calls it. */
UNUSED void mnGraphicsInstallBg1VCountHandler(void) {
	syIoWrite16(0, &REG_BG0VOFS);
	syIntFunction(IRQ_VCOUNT, COMMAND_ROW_TOP * 8, mnCommandRowPutBg1InFront);
}

#undef COMMAND_ICON_TILE_ROW_BYTES
#undef ROW_LAST_PIXEL_SHIFT
#undef COMMAND_ICON_COLORS
#undef COMMAND_ICON_BG_TILES
