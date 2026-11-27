/* @file */
/* Item, Psynergy and command icons. */
/* Part of window.c. */

/* The builders that load an icon into VRAM take HEAP_SLOT_ICON_TRANSFER */
/* (GlyphState) for the build and free it before returning; mnItemIconCompose */
/* and the Icon_Draw functions draw into the slot their caller holds. */

/* Returns how many item icons the icon table holds; mnIconBuildItemIconTiles */
/* draws icon 0 for a number past them. */
s32 mnIconCountItemIcons(void) { return ItemIconsEnd - ItemIcons; }

/* Returns how many Psynergy icons the icon table holds; */
/* mnIconBuildAbilityIconTiles draws icon 0 for a number past them. */
s32 mnIconCountPsynergyIcons(void) { return PsynergyIconsEnd - PsynergyIcons; }

/* Builds a command's icon, its picture over a frame (ICON_FRAME), and */
/* loads it into a sprite tile slot: *slot, or a free one when reuse is 0. */
void mnIconBuildCommandIconTiles(s32 overlayIcon, s32 frame, s32 *slot,
								 s32 *tileOut, s32 reuse) {
	ICON_TRANSFER *state;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	state->encoded = Icon_BasePointerTable[frame];
	state->width = 2;
	state->height = 2;
	mnIconDecode(state, 0);
	state->encoded = Icon_OverlayPointerTable[overlayIcon];
	state->width = 2;
	state->height = 2;
	mnIconDecode(state, 1);
	if(reuse == 0) {
		*slot = syVramCacheFindFreeEntry();
	}
	*tileOut = syVramCacheLoad(*slot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
}

/* Unused: builds an ability's icon from the item icons rather than the */
/* Psynergy icons, as mnIconBuildItemIconTiles. */
UNUSED void mnAbilityBuildItemIconTiles(s32 action, s32 withBase, s32 *slot,
										s32 *tile, s32 reuse) {
	mnIconBuildItemIconTiles(mnAbilityGet(action)->icon, withBase, slot, tile,
							 reuse);
}

/* Builds item icon number icon, over the item frame when withBase is set, and
 */
/* loads it into *cacheSlot (a free slot unless reuse is set). */
void mnIconBuildItemIconTiles(u32 icon, s32 withBase, s32 *cacheSlot,
							  s32 *tileOut, s32 reuse) {
	ICON_TRANSFER *state;
	s32 overlay;

	overlay = 0;
	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));

	if(icon >= mnIconCountItemIcons())
		icon = 0;

	if(withBase != 0) {
		state->encoded = Icon_BasePointerTable[ICON_FRAME_ITEM];
		state->width = 2;
		state->height = 2;
		mnIconDecode(state, 0);
		overlay = 1;
	}

	state->encoded = ItemIcons[icon];
	state->width = 2;
	state->height = 2;
	mnIconDecode(state, overlay);

	if(reuse == 0)
		*cacheSlot = syVramCacheFindFreeEntry();

	*tileOut =
		syVramCacheLoad(*cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
}

/* Draws an item's icon into the glyph transfer buffer from its inventory code
 */
/* (INVENTORY_ENCODING) with the layers asked for (ICON_ flags): the frame,
 */
/* the equipped, broken and artifact marks, and the count. Returns -1 with no */
/* glyph transfer buffer and 256 otherwise; no caller reads the result. */
s32 mnItemIconCompose(u32 code, u32 layers) {
	s32 overlay;
	ITEM_DEFINITION *item;
	ICON_TRANSFER *state;
	s32 count;

	overlay = 0;
	count = 0;
	item = gmItemGetDirectStub(code & INVENTORY_ITEM_ID_MASK);
	state = GlyphState;
	if(state == NULL)
		return -1;
	if(layers & ICON_BASE) {
		state->encoded = Icon_BasePointerTable[ICON_FRAME_ITEM];
		state->width = 2;
		state->height = 2;
		mnIconDecode(state, 0);
		overlay = 1;
	}
	state->encoded = ItemIcons[item->icon];
	state->width = 2;
	state->height = 2;
	mnIconDecode(state, overlay);
	if((layers & ICON_BROKEN) && (code & INVENTORY_BROKEN)) {
		state->encoded = Icon_MarkPointers[ICON_MARK_BROKEN];
		state->width = 2;
		state->height = 2;
		mnIconDecode(state, 1);
	}
	if((layers & ICON_EQUIPPED) && (code & INVENTORY_EQUIPPED)) {
		state->encoded = Icon_MarkPointers[ICON_MARK_EQUIPPED];
		state->width = 2;
		state->height = 2;
		mnIconDecode(state, 1);
	}
	if((layers & ICON_CURSED) && (code & INVENTORY_EQUIPPED) &&
	   (item->flg & ITEM_CURSED) && (item->flg & ITEM_CANNOT_UNEQUIP)) {
		state->encoded = Icon_MarkPointers[ICON_MARK_CURSED];
		state->width = 2;
		state->height = 2;
		mnIconDecode(state, 1);
	}
	if(layers & ICON_COUNT_ABOVE_ONE) {
		count = (code & INVENTORY_QUANTITY_MASK) >> INVENTORY_QUANTITY_SHIFT;
		count++;
		if(count <= 1)
			count = 0;
	}
	if(layers & ICON_COUNT) {
		count = (code & INVENTORY_QUANTITY_MASK) >> INVENTORY_QUANTITY_SHIFT;
		count++;
	}
	/* Counts up to a full stack show: the ones digit, and a tens digit from the
	 */
	/* second set (1 to 3) over it. */
	if(count != 0 && count <= INVENTORY_STACK_MAX) {
		s32 ones = count % 10;

		state->encoded = Icon_DigitPointers[ones];
		state->width = 2;
		state->height = 2;
		mnIconDecode(state, 1);
		if(count / 10 != 0) {
			state->encoded =
				Icon_DigitPointers[ICON_DIGIT_TENS_FIRST - 1 + count / 10];
			state->width = 2;
			state->height = 2;
			mnIconDecode(state, 1);
		}
	}
	return 256;
}

/* Draws an item's icon from its inventory code with the layers asked for */
/* (mnItemIconCompose) and loads it into sprite tile slot cacheSlot. */
s32 mnItemIconLoadLayersIntoSlot(s32 inventoryCode, s32 layers, s32 cacheSlot) {
	ICON_TRANSFER *state;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	mnItemIconCompose(inventoryCode, layers);
	syVramCacheLoad(cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return 1;
}

/* Builds one of the other icons (Icon_MiscIconPointers) and loads it into */
/* sprite tile slot cacheSlot. */
s32 mnIconBuildMiscIconTiles(s32 icon, s32 unused, s32 cacheSlot) {
	ICON_TRANSFER *state;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	mnIconDrawMisc(icon);
	syVramCacheLoad(cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return 1;
}

/* Loads 32 bytes, one sprite tile, from the start of MenuTitlePictures into */
/* the VRAM cache entry slot, whichever entry is asked for: those bytes are the
 */
/* table's first pointers, not a picture. */
s32 mnTitleLoadFirstTile(u32 entry, s32 unused, u32 slot) {
	/* FAKEMATCH(dead): a switch whose every case picks MenuTitlePictures, three
	 * of them through other names for it; the plain form loads it directly. */
	extern const u8 *MenuTitlePicturesCaseA[], *MenuTitlePicturesCaseB[],
		*MenuTitlePicturesCaseC[];
	void *source;
	switch(entry) {
	case 1:
		source = MenuTitlePicturesCaseA;
		break;
	case 2:
		source = MenuTitlePicturesCaseB;
		break;
	case 3:
		source = MenuTitlePicturesCaseC;
		break;
	case 0:
	default:
		source = MenuTitlePictures;
		break;
	}
	syVramCacheLoad(slot, 32, source);
	return 1;
}

/* Draws an item's icon from its inventory code into the glyph transfer buffer,
 */
/* with no frame or marks; code 0 draws icon 0. The caller holds the buffer. */
void mnIconDrawItem(s32 code) {
	ICON_TRANSFER *glyph = GlyphState;
	ITEM_DEFINITION *item = gmItemGetDirectStub(INVENTORY_ITEM_ID_MASK & code);

	if(code != 0) {
		glyph->encoded = ItemIcons[item->icon];
	}
	else {
		glyph->encoded = ItemIcons[0];
	}
	glyph->width = 2;
	glyph->height = 2;
	mnIconDecode(glyph, 0);
}

/* Builds an ability's Psynergy icon, as mnIconBuildAbilityIconTiles. */
void mnAbilityBuildIconTiles(s32 action, s32 withBase, s32 *slot, s32 *tile,
							 s32 reuse) {
	mnIconBuildAbilityIconTiles(mnAbilityGet(action)->icon, withBase, slot,
								tile, reuse);
}

/* Builds Psynergy icon number icon, over the item frame when withBase is set,
 */
/* and loads it into *cacheSlot (a free slot unless reuse is set). */
void mnIconBuildAbilityIconTiles(u32 icon, s32 withBase, s32 *cacheSlot,
								 s32 *tileOut, s32 reuse) {
	ICON_TRANSFER *state;
	s32 overlay;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	overlay = 0;

	if(icon >= mnIconCountPsynergyIcons())
		icon = 0;

	if(withBase != 0) {
		state->encoded = Icon_BasePointerTable[ICON_FRAME_ITEM];
		state->width = 2;
		state->height = 2;
		mnIconDecode(state, 0);
		overlay = 1;
	}

	state->encoded = PsynergyIcons[icon];
	state->width = 2;
	state->height = 2;
	mnIconDecode(state, overlay);

	if(reuse == 0)
		*cacheSlot = syVramCacheFindFreeEntry();

	*tileOut =
		syVramCacheLoad(*cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
}

/* Draws one of the other icons into the glyph transfer buffer. */
void mnIconDrawMisc(u32 iconIndex) {
	ICON_TRANSFER *glyph = GlyphState;

	glyph->encoded = Icon_MiscIconPointers[iconIndex];
	glyph->width = 2;
	glyph->height = 2;
	mnIconDecode(glyph, 0);
}

/* Loads a 32x32 icon from the icon resource into sprite tile slot *slot (a */
/* free slot unless reuse is set) and its colours into the sprite palette named
 */
/* by palette. */
void mnIconLoadWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
						   s32 palette, s32 reuse) {
	ICON_TRANSFER *state;
	u16 *offsets;
	u8 *encodedEntry;
	u32 index;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	offsets = syResourceGetTableEntry(RESOURCE_ICONS);
	/* A number below PORTRAIT_SPRITE_MAP_FIRST is the entry itself; the other
	 */
	/* speakers' portraits, numbered from it (mnPortraitGetForSpeaker), are the
	 */
	/* entries from PORTRAIT_JUPITER_DJINNI on. Each entry's colours come before
	 */
	/* its encoded tiles. */
	if(icon < PORTRAIT_SPRITE_MAP_FIRST)
		index = icon;
	else
		index = icon - PORTRAIT_SPRITE_MAP_FIRST + PORTRAIT_JUPITER_DJINNI;
	encodedEntry = (u8 *)offsets + offsets[index];
	state->encoded = encodedEntry + PALETTE_COLORS * sizeof(u16);
	state->width = 4;
	state->height = 4;
	mnIconDecode(state, 0);
	if(reuse == 0)
		*slot = syVramCacheFindFreeEntry();
	*tile = syVramCacheLoad(*slot, 4 * 4 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	syDMAXCopy(encodedEntry, (void *)&OBJ_PLTT_COLOR(palette, 0),
			   DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
}

/* Unused: an empty function among the icon builders. */
UNUSED void mnIconReservedNoOp(void) {}

/* Decodes a compressed icon into the glyph buffer and expands it to tiles: */
/* opaque for the first layer, or drawn over what is there when overlay is set.
 */
void mnIconDecode(ICON_TRANSFER *glyph, s32 overlay) {
	syRamCodeLoadSlot(HEAP_SLOT_RAM_CODE_TEMP, Tile_Decompress4bpp,
					  Tile_Decompress4bppCodeSize);
	((void (*)(const void *, u8 *))HeapSlots[HEAP_SLOT_RAM_CODE_TEMP])(
		glyph->encoded, glyph->input);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE_TEMP);
	if(overlay)
		syRamCodeLoadSlot(HEAP_SLOT_RAM_CODE_TEMP, Tile_ExpandMasked,
						  Tile_ExpandMaskedCodeSize);
	else
		syRamCodeLoadSlot(HEAP_SLOT_RAM_CODE_TEMP, Tile_ExpandOpaque,
						  Tile_ExpandOpaqueCodeSize);
	((void (*)(u8 *, u8 *, u32, u32))HeapSlots[HEAP_SLOT_RAM_CODE_TEMP])(
		glyph->input, glyph->tiles, (u16)glyph->width, (u16)glyph->height);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE_TEMP);
}
