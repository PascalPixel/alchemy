/* Item, Psynergy and command icons. Part of window.c. */

/* The builders that load an icon into VRAM take HEAP_SLOT_ICON_TRANSFER
   (GlyphState) for the build and free it before returning; mnItemIconCompose
   and the Icon_Draw functions draw into the slot their caller holds. */

/* Returns how many item icons the icon table holds; mnIconBuildItemIconTiles
   draws icon 0 for a number past them. */
Sint32 mnIconCountItemIcons(void)
{
	return ItemIconsEnd - ItemIcons;
}

/* Returns how many Psynergy icons the icon table holds;
   mnIconBuildAbilityIconTiles draws icon 0 for a number past them. */
Sint32 mnIconCountPsynergyIcons(void)
{
	return PsynergyIconsEnd - PsynergyIcons;
}

/* Builds a command's icon, its picture over a tick (ICON_FRAME), and loads it
   into a sprite tile slot: *slot, or a free one when reuse is 0. */
void mnIconBuildCommandIconTiles(
	Sint32 overlayIcon, Sint32 frm, Sint32 *slot, Sint32 *tileOut, Sint32 reuse)
{
	ICON_TRANSFER *state;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	state->encoded = IconBasePointerTable[frm];
	state->width = 2;
	state->height = 2;
	mnIconDecode(state, 0);
	state->encoded = Icon_OverlayPointerTable[overlayIcon];
	state->width = 2;
	state->height = 2;
	mnIconDecode(state, 1);
	if(reuse == 0) {
		*slot = syVRAMCacheFindFreeEntry();
	}
	*tileOut = syVRAMCacheLoad(*slot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
}

/* Unused: builds an ability's icon from the item icons rather than the Psynergy
   icons, as mnIconBuildItemIconTiles. */
UNUSED void mnAbilityBuildItemIconTiles(
	Sint32 action, Sint32 withBase, Sint32 *slot, Sint32 *tile, Sint32 reuse)
{
	mnIconBuildItemIconTiles(
		gmAbilityGetDirectStub(action)->icon, withBase, slot, tile, reuse);
}

/* Builds item icon number icon, over the item frame when withBase is set, and
   loads it into *cacheSlot (a free slot unless reuse is set). */
void mnIconBuildItemIconTiles(Uint32 icon, Sint32 withBase, Sint32 *cacheSlot,
	Sint32 *tileOut, Sint32 reuse)
{
	ICON_TRANSFER *state;
	Sint32		   overlay;

	overlay = 0;
	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));

	if(icon >= mnIconCountItemIcons())
		icon = 0;

	if(withBase != 0) {
		state->encoded = IconBasePointerTable[ICON_FRAME_ITEM];
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
		*cacheSlot = syVRAMCacheFindFreeEntry();

	*tileOut =
		syVRAMCacheLoad(*cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
}

/* Draws an item's icon into the glyph transfer buffer from its inventory code
   (INVENTORY_ENCODING) with the layers asked for (ICON_ flags): the tick, the
   equipped, broken and artifact marks, and the count. Returns -1 with no glyph
   transfer buffer and 256 otherwise; no caller reads the result. */
Sint32 mnItemIconCompose(Uint32 code, Uint32 layers)
{
	Bool			 overlay;
	ITEM_DEFINITION *item;
	ICON_TRANSFER	*state;
	Sint32			 cnt;

	overlay = FALSE;
	cnt = 0;
	item = gmItemGetDirectStub(code & INVENTORY_ITEM_ID_MASK);
	state = GlyphState;
	if(state == NULL)
		return -1;
	if(layers & ICON_BASE) {
		state->encoded = IconBasePointerTable[ICON_FRAME_ITEM];
		state->width = 2;
		state->height = 2;
		mnIconDecode(state, 0);
		overlay = TRUE;
	}
	state->encoded = ItemIcons[item->icon];
	state->width = 2;
	state->height = 2;
	mnIconDecode(state, overlay);
	if((layers & ICON_BROKEN) && (code & INVENTORY_BROKEN)) {
		state->encoded = IconMarkPointers[ICON_MARK_BROKEN];
		state->width = 2;
		state->height = 2;
		mnIconDecode(state, 1);
	}
	if((layers & ICON_EQUIPPED) && (code & INVENTORY_EQUIPPED)) {
		state->encoded = IconMarkPointers[ICON_MARK_EQUIPPED];
		state->width = 2;
		state->height = 2;
		mnIconDecode(state, 1);
	}
	if((layers & ICON_CURSED) && (code & INVENTORY_EQUIPPED) &&
		(item->flg & ITEM_CURSED) && (item->flg & ITEM_CANNOT_UNEQUIP)) {
		state->encoded = IconMarkPointers[ICON_MARK_CURSED];
		state->width = 2;
		state->height = 2;
		mnIconDecode(state, 1);
	}
	if(layers & ICON_COUNT_ABOVE_ONE) {
		cnt = (code & INVENTORY_QUANTITY_MASK) >> INVENTORY_QUANTITY_SHIFT;
		cnt++;
		if(cnt <= 1)
			cnt = 0;
	}
	if(layers & ICON_COUNT) {
		cnt = (code & INVENTORY_QUANTITY_MASK) >> INVENTORY_QUANTITY_SHIFT;
		cnt++;
	}
	/* Counts up to a full stack show: the ones digit, and a tens digit from the
	   second set (1 to 3) over it. */
	if(cnt != 0 && cnt <= INVENTORY_STACK_MAX) {
		Sint32 ones = cnt % 10;

		state->encoded = IconDigitPointers[ones];
		state->width = 2;
		state->height = 2;
		mnIconDecode(state, 1);
		if(cnt / 10 != 0) {
			state->encoded =
				IconDigitPointers[ICON_DIGIT_TENS_FIRST - 1 + cnt / 10];
			state->width = 2;
			state->height = 2;
			mnIconDecode(state, 1);
		}
	}
	return 256;
}

/* Draws an item's icon from its inventory code with the layers asked for
   (mnItemIconCompose) and loads it into sprite tile slot cacheSlot. */
Sint32 mnItemIconLoadLayersIntoSlot(
	Sint32 inventoryCode, Sint32 layers, Sint32 cacheSlot)
{
	ICON_TRANSFER *state;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	mnItemIconCompose(inventoryCode, layers);
	syVRAMCacheLoad(cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return 1;
}

/* Builds one of the other icons (Icon_MiscIconPointers) and loads it into
   sprite tile slot cacheSlot. */
Sint32 mnIconBuildMiscIconTiles(Sint32 icon, Sint32 unused, Sint32 cacheSlot)
{
	ICON_TRANSFER *state;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	mnIconDrawMisc(icon);
	syVRAMCacheLoad(cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return 1;
}

/* Loads 32 bytes, one sprite tile, from the start of MenuTitlePictures into the
   VRAM cache entry slot, whichever entry is asked for: those bytes are the
   table's first pointers, not a picture. */
Sint32 mnTitleLoadFirstTile(Uint32 entry, Sint32 unused, Uint32 slot)
{
	/* FAKEMATCH(dead): a switch whose every case picks MenuTitlePictures, three
	 * of them through other names for it; the plain form loads it directly. */
	extern const Uint8 *MenuTitlePicturesCaseA[], *MenuTitlePicturesCaseB[],
		*MenuTitlePicturesCaseC[];
	void *src;
	switch(entry) {
	case 1:
		src = MenuTitlePicturesCaseA;
		break;
	case 2:
		src = MenuTitlePicturesCaseB;
		break;
	case 3:
		src = MenuTitlePicturesCaseC;
		break;
	case 0:
	default:
		src = MenuTitlePictures;
		break;
	}
	syVRAMCacheLoad(slot, 32, src);
	return 1;
}

/* Draws an item's icon from its inventory code into the glyph transfer buffer,
   with no tick or marks; code 0 draws icon 0. The caller holds the buffer. */
void mnIconDrawItem(Sint32 code)
{
	ICON_TRANSFER	*glyph = GlyphState;
	ITEM_DEFINITION *item = gmItemGetDirectStub(INVENTORY_ITEM_ID_MASK & code);

	if(code != 0) {
		glyph->encoded = ItemIcons[item->icon];
	} else {
		glyph->encoded = ItemIcons[0];
	}
	glyph->width = 2;
	glyph->height = 2;
	mnIconDecode(glyph, 0);
}

/* Builds an ability's Psynergy icon, as mnIconBuildAbilityIconTiles. */
void mnAbilityBuildIconTiles(
	Sint32 action, Sint32 withBase, Sint32 *slot, Sint32 *tile, Sint32 reuse)
{
	mnIconBuildAbilityIconTiles(
		gmAbilityGetDirectStub(action)->icon, withBase, slot, tile, reuse);
}

/* Builds Psynergy icon number icon, over the item frame when withBase is set,
   and loads it into *cacheSlot (a free slot unless reuse is set). */
void mnIconBuildAbilityIconTiles(Uint32 icon, Sint32 withBase,
	Sint32 *cacheSlot, Sint32 *tileOut, Sint32 reuse)
{
	ICON_TRANSFER *state;
	Sint32		   overlay;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	overlay = 0;

	if(icon >= mnIconCountPsynergyIcons())
		icon = 0;

	if(withBase != 0) {
		state->encoded = IconBasePointerTable[ICON_FRAME_ITEM];
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
		*cacheSlot = syVRAMCacheFindFreeEntry();

	*tileOut =
		syVRAMCacheLoad(*cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
}

/* Draws one of the other icons into the glyph transfer buffer. */
void mnIconDrawMisc(Uint32 iconIdx)
{
	ICON_TRANSFER *glyph = GlyphState;

	glyph->encoded = Icon_MiscIconPointers[iconIdx];
	glyph->width = 2;
	glyph->height = 2;
	mnIconDecode(glyph, 0);
}

/* Loads a 32x32 icon from the icon resource into sprite tile slot *slot (a free
   slot unless reuse is set) and its colours into the sprite palette named by
   palette. */
void mnIconLoadWithPalette(Uint32 icon, Sint32 unused, Sint32 *slot,
	Sint32 *tile, Sint32 palette, Sint32 reuse)
{
	ICON_TRANSFER *state;
	Uint16		  *offsets;
	Uint8		  *encodedEntry;
	Uint32		   idx;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	offsets = syResourceGetTableEntry(RESOURCE_ICONS);
	/* A number below PORTRAIT_SPRITE_MAP_FIRST is the entry itself; the other
	   speakers' portraits, numbered from it (mnPortraitGetForSpeaker), are the
	   entries from PORTRAIT_JUPITER_DJINNI on. Each entry's colours come before
	   its encoded tiles. */
	if(icon < PORTRAIT_SPRITE_MAP_FIRST)
		idx = icon;
	else
		idx = icon - PORTRAIT_SPRITE_MAP_FIRST + PORTRAIT_JUPITER_DJINNI;
	encodedEntry = (Uint8 *)offsets + offsets[idx];
	state->encoded = encodedEntry + PALETTE_COLORS * sizeof(Uint16);
	state->width = 4;
	state->height = 4;
	mnIconDecode(state, 0);
	if(reuse == 0)
		*slot = syVRAMCacheFindFreeEntry();
	*tile = syVRAMCacheLoad(*slot, 4 * 4 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	syDMAXCopy(encodedEntry, (void *)&OBJ_PLTT_COLOR(palette, 0),
		DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
}

/* Unused: an empty function among the icon builders. */
UNUSED void mnIconReservedNoOperation(void)
{
}

/* Decodes a compressed icon into the glyph buffer and expands it to tiles:
   opaque for the first layer, or drawn over what is there when overlay is
   set. */
void mnIconDecode(ICON_TRANSFER *glyph, Sint32 overlay)
{
	RAM_CODE_LOAD_SLOT(HEAP_SLOT_RAM_CODE_TEMP, mnTileDecompress4bpp,
		Tile_Decompress4bppCodeSize);
	((void (*)(const void *, Uint8 *))HeapSlots[HEAP_SLOT_RAM_CODE_TEMP])(
		glyph->encoded, glyph->input);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE_TEMP);
	if(overlay)
		RAM_CODE_LOAD_SLOT(HEAP_SLOT_RAM_CODE_TEMP, mnTileExpandMasked,
			Tile_ExpandMaskedCodeSize);
	else
		RAM_CODE_LOAD_SLOT(HEAP_SLOT_RAM_CODE_TEMP, mnTileExpandOpaque,
			Tile_ExpandOpaqueCodeSize);
	((void (*)(Uint8 *, Uint8 *, Uint32,
		Uint32))HeapSlots[HEAP_SLOT_RAM_CODE_TEMP])(glyph->input, glyph->tiles,
		(Uint16)glyph->width, (Uint16)glyph->height);
	syHeapSlotRelease(HEAP_SLOT_RAM_CODE_TEMP);
}
