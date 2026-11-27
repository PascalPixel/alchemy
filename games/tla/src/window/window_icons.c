/* The 16x16 icons of items, Psynergy and commands, built from their pictures, a
   tick under them and the marks over them, and the 32x32 icons and portraits
   loaded with their colours. Part of window.c. */

/* The encoded icons: the command icons' bases and the pictures drawn over them,
   the marks an item icon may carry (equipped, broken), and the digits of its
   count. */
extern Uint8 *IconBasePointerTable[];
extern Uint8 *Icon_OverlayPointerTable[];
extern Uint8 *IconMarkPointers[];
extern Uint8 *IconDigitPointers[];

/* The ticks an icon is drawn over (IconBasePointerTable). */
typedef enum ICON_BASE_KIND {
	ICON_BASE_COMMAND = 0,
	ICON_BASE_COMMAND_RED = 1,
	ICON_BASE_ITEM = 2
} ICON_BASE_KIND;

/* The marks drawn over an item's icon (IconMarkPointers). */
typedef enum ICON_MARK {
	ICON_MARK_EQUIPPED = 0, /* the E of an item equipped */
	ICON_MARK_BROKEN = 1,	/* the cross of a broken item */
	/* The phi of an equipped item that cannot be removed (ICON_CURSED) */
	ICON_MARK_ARTIFACT = 2
} ICON_MARK;

/* IconDigitPointers: the digits 0 to 9, then the tens 10 to 30 from this
   entry. */
#define ICON_DIGIT_TENS_FIRST 10

void mnIconDecode(ICON_TRANSFER *glyph, Sint32 overlay);

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

/* Builds a command icon (overlay overlayIcon over base baseIcon) and loads it
   into a sprite tile slot: *slot, or a free one when reuse is 0. */
void mnIconBuildCommandIconTiles(Sint32 overlayIcon, Sint32 baseIcon,
	Sint32 *slot, Sint32 *tileOut, Sint32 reuse)
{
	ICON_TRANSFER *state;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	state->encoded = IconBasePointerTable[baseIcon];
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

/* Unused: builds the icon of a battle action as an item's icon
   (mnIconBuildItemIconTiles), where mnAbilityBuildIconTiles builds its Psynergy
   icon. */
UNUSED void mnAbilityBuildItemIconTiles(
	Sint32 action, Sint32 withBase, Sint32 *slot, Sint32 *tile, Sint32 reuse)
{
	mnIconBuildItemIconTiles(
		gmAbilityGetDirectStub(action)->icon, withBase, slot, tile, reuse);
}

/* Builds item icon glyph, over the icon base when withBase is set, and loads it
   into *cacheSlot (a free slot unless reuse is set). Returns the tile in
   *tileOut. */
void mnIconBuildItemIconTiles(Uint32 glyph, Sint32 withBase, Sint32 *cacheSlot,
	Sint32 *tileOut, Sint32 reuse)
{
	ICON_TRANSFER *state;
	Sint32		   overlay;

	overlay = 0;
	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));

	if(glyph >= mnIconCountItemIcons())
		glyph = 0;

	if(withBase != 0) {
		state->encoded = IconBasePointerTable[ICON_BASE_ITEM];
		state->width = 2;
		state->height = 2;
		mnIconDecode(state, 0);
		overlay = 1;
	}

	state->encoded = ItemIcons[glyph];
	state->width = 2;
	state->height = 2;
	mnIconDecode(state, overlay);

	if(reuse == 0)
		*cacheSlot = syVRAMCacheFindFreeEntry();

	*tileOut =
		syVRAMCacheLoad(*cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
}

/* Draws an item's icon into the glyph buffer from its inventory code
   (INVENTORY_ENCODING) with the layers asked for (ICON_ flags): the tick, the
   equipped, broken and artifact marks, and the count. Returns 256, which no
   caller reads, or -1 without the glyph buffer. */
Sint32 mnItemIconCompose(Uint32 code, Uint32 layers)
{
	Bool			 overlay;
	ITEM_DEFINITION *item;
	ICON_TRANSFER	*state;
	Sint32			 cnt;

	overlay = FALSE;
	cnt = 0;
	item = gmItemGetDirectStub(code & INVENTORY_ITEM_ID_MASK);
	state = Ram_HeapSlots->glyphTransfer;
	if(state == NULL)
		return -1;
	if(layers & ICON_BASE) {
		state->encoded = IconBasePointerTable[ICON_BASE_ITEM];
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
		state->encoded = IconMarkPointers[ICON_MARK_ARTIFACT];
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
	/* Counts up to a full stack show: the ones digit, and over it the tens. */
	if(cnt != 0 && cnt <= INVENTORY_STACK_MAX) {
		Sint32 ones = cnt % 10;

		state->encoded = IconDigitPointers[ones];
		state->width = 2;
		state->height = 2;
		mnIconDecode(state, 1);
		if(cnt / 10 != 0) {
			state->encoded =
				IconDigitPointers[cnt / 10 + (ICON_DIGIT_TENS_FIRST - 1)];
			state->width = 2;
			state->height = 2;
			mnIconDecode(state, 1);
		}
	}
	return 256;
}

/* Draws an item's icon from its inventory code with the layers asked for
   (mnItemIconCompose) and loads it into VRAM cache entry cacheSlot. */
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

/* Draws one of the other icons (ICON_KIND_PATTERN) and loads it into cacheSlot,
   as mnItemIconLoadLayersIntoSlot does an item's. */
Sint32 mnIconBuildMiscIconTiles(Sint32 icon, Sint32 unused, Sint32 cacheSlot)
{
	ICON_TRANSFER *state;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	mnIconDrawMisc(icon);
	syVRAMCacheLoad(cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return 1;
}

/* Decodes item code's icon (the first icon for code 0) into the glyph
   buffer. */
void mnIconDrawItem(Sint32 code)
{
	ICON_TRANSFER	*state = Ram_HeapSlots->glyphTransfer;
	ITEM_DEFINITION *item = gmItemGetDirectStub(INVENTORY_ITEM_ID_MASK & code);

	if(code != 0)
		state->encoded = ItemIcons[item->icon];
	else
		state->encoded = ItemIcons[0];
	state->width = 2;
	state->height = 2;
	mnIconDecode(state, 0);
}

/* Builds the Psynergy icon of a battle action, as
   mnIconBuildAbilityIconTiles. */
void mnAbilityBuildIconTiles(
	Sint32 action, Sint32 withBase, Sint32 *slot, Sint32 *tile, Sint32 reuse)
{
	mnIconBuildAbilityIconTiles(
		gmAbilityGetDirectStub(action)->icon, withBase, slot, tile, reuse);
}

/* Builds Psynergy icon glyph, over the icon base when withBase is set, and
   loads it into *cacheSlot (a free slot unless reuse is set). Returns the tile
   in *tileOut. */
void mnIconBuildAbilityIconTiles(Uint32 glyph, Sint32 withBase,
	Sint32 *cacheSlot, Sint32 *tileOut, Sint32 reuse)
{
	ICON_TRANSFER *state;
	Sint32		   overlay;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	overlay = 0;

	if(glyph >= mnIconCountPsynergyIcons())
		glyph = 0;

	if(withBase != 0) {
		state->encoded = IconBasePointerTable[ICON_BASE_ITEM];
		state->width = 2;
		state->height = 2;
		mnIconDecode(state, 0);
		overlay = 1;
	}

	state->encoded = PsynergyIcons[glyph];
	state->width = 2;
	state->height = 2;
	mnIconDecode(state, overlay);

	if(reuse == 0)
		*cacheSlot = syVRAMCacheFindFreeEntry();

	*tileOut =
		syVRAMCacheLoad(*cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
}

/* Draws one of the other icons into the glyph buffer. */
void mnIconDrawMisc(Uint32 idx)
{
	ICON_TRANSFER *state = Ram_HeapSlots->glyphTransfer;

	state->encoded = Icon_MiscIconPointers[idx];
	state->width = 2;
	state->height = 2;
	mnIconDecode(state, 0);
}

/* The icon resource's row of the first sprite portrait,
   PORTRAIT_FIRST_SPRITE_PORTRAIT. */
#define PORTRAIT_FIRST_SPRITE_ICON 16

/* Loads a 32x32 icon from the icon resource into sprite tile slot *slot (a free
   slot unless reuse is set) and its colours into the sprite palette named by
   palette. Returns the tile in *tile. Each icon of the resource has its
   PALETTE_COLORS colours before its encoded tiles. Portraits (mnPortraitCreate)
   come through here. */
void mnIconLoadWithPalette(Uint32 icon, Sint32 unused, Sint32 *slot,
	Sint32 *tile, Sint32 palette, Sint32 reuse)
{
	ICON_TRANSFER *state;
	Uint16		  *tbl;
	Uint8		  *entry;
	Uint32		   idx;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	tbl = syResourceGetTableEntry(RESOURCE_ICONS);
	if(icon < PORTRAIT_FIRST_SPRITE_PORTRAIT)
		idx = icon;
	else
		idx =
			icon - PORTRAIT_FIRST_SPRITE_PORTRAIT + PORTRAIT_FIRST_SPRITE_ICON;
	entry = (Uint8 *)tbl + tbl[idx];
	state->encoded = entry + PALETTE_COLORS * sizeof(Uint16);
	state->width = 4;
	state->height = 4;
	mnIconDecode(state, 0);
	if(reuse == 0)
		*slot = syVRAMCacheFindFreeEntry();
	*tile = syVRAMCacheLoad(*slot, 4 * 4 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	syDMAXCopy(entry, (void *)&OBJ_PLTT_COLOR(palette, 0),
		DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
}

/* Unused: does nothing. */
UNUSED void mnIconReservedNoOperation(void)
{
}

/* The tile functions RAM_CODE_LOAD copies to RAM to run, and their sizes, as
   these symbols' addresses. */
extern Uint8 mnTileDecompress4bppCode[];
extern Uint8 mnTileExpandMaskedCode[];
extern Uint8 mnTileExpandOpaqueCode[];
extern Uint8 Tile_Decompress4bppCodeSize[];
extern Uint8 Tile_ExpandMaskedCodeSize[];
extern Uint8 Tile_ExpandOpaqueCodeSize[];

/* Decodes a compressed icon into the glyph buffer and expands it to tiles,
   opaque for the first layer, or over what is there, its colour 0 clear, when
   overlay is set. glyph->width and height are the icon's size in tiles. */
void mnIconDecode(ICON_TRANSFER *glyph, Sint32 overlay)
{
	void *code;

	RAM_CODE_LOAD(code, mnTileDecompress4bppCode, Tile_Decompress4bppCodeSize);
	((void (*)(const void *, Uint8 *))code)(glyph->encoded, glyph->input);
	syHeapRelease(code);
	if(overlay)
		RAM_CODE_LOAD(code, mnTileExpandMaskedCode, Tile_ExpandMaskedCodeSize);
	else
		RAM_CODE_LOAD(code, mnTileExpandOpaqueCode, Tile_ExpandOpaqueCodeSize);
	((void (*)(Uint8 *, Uint8 *, Uint32, Uint32))code)(glyph->input,
		glyph->tiles, (Uint16)glyph->width, (Uint16)glyph->height);
	syHeapRelease(code);
}
