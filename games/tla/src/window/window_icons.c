/* @file */
/* The 16x16 icons of items, Psynergy and commands, built from their pictures,
 */
/* a frame under them and the marks over them, and the 32x32 icons and */
/* portraits loaded with their colours. */
/* Part of window.c. */

/* The encoded icons: the command icons' bases and the pictures drawn over */
/* them, the marks an item icon may carry (equipped, broken), and the digits of
 */
/* its count. */
extern u8 *Icon_BasePointerTable[];
extern u8 *Icon_OverlayPointerTable[];
extern u8 *Icon_MarkPointers[];
extern u8 *Icon_DigitPointers[];

/* The frames an icon is drawn over (Icon_BasePointerTable). */
typedef enum ICON_BASE_KIND {
	ICON_BASE_COMMAND = 0,
	ICON_BASE_COMMAND_RED = 1,
	ICON_BASE_ITEM = 2
} ICON_BASE_KIND;

/* The marks drawn over an item's icon (Icon_MarkPointers). */
typedef enum ICON_MARK {
	ICON_MARK_EQUIPPED = 0, /* the E of an item equipped */
	ICON_MARK_BROKEN = 1,	/* the cross of a broken item */
	/* The phi of an equipped item that cannot be removed (ICON_CURSED) */
	ICON_MARK_ARTIFACT = 2
} ICON_MARK;

/* Icon_DigitPointers: the digits 0 to 9, then the tens 10 to 30 from this */
/* entry. */
#define ICON_DIGIT_TENS_FIRST 10

void mnIconDecode(ICON_TRANSFER *glyph, s32 overlay);

/* Returns how many item icons the icon table holds; mnIconBuildItemIconTiles */
/* draws icon 0 for a number past them. */
s32 mnIconCountItemIcons(void) { return ItemIconsEnd - ItemIcons; }

/* Returns how many Psynergy icons the icon table holds; */
/* mnIconBuildAbilityIconTiles draws icon 0 for a number past them. */
s32 mnIconCountPsynergyIcons(void) { return PsynergyIconsEnd - PsynergyIcons; }

/* Builds a command icon (overlay overlayIcon over base baseIcon) and loads it
 */
/* into a sprite tile slot: *slot, or a free one when reuse is 0. */
void mnIconBuildCommandIconTiles(s32 overlayIcon, s32 baseIcon, s32 *slot,
								 s32 *tileOut, s32 reuse) {
	ICON_TRANSFER *state;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	state->encoded = Icon_BasePointerTable[baseIcon];
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

/* Unused: builds the icon of a battle action as an item's icon */
/* (mnIconBuildItemIconTiles), where mnAbilityBuildIconTiles builds its */
/* Psynergy icon. */
UNUSED void mnAbilityBuildItemIconTiles(s32 action, s32 withBase, s32 *slot,
										s32 *tile, s32 reuse) {
	mnIconBuildItemIconTiles(btAbilityGet(action)->icon, withBase, slot, tile,
							 reuse);
}

/* Builds item icon glyph, over the icon base when withBase is set, and loads */
/* it into *cacheSlot (a free slot unless reuse is set). Returns the tile in */
/* *tileOut. */
void mnIconBuildItemIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
							  s32 *tileOut, s32 reuse) {
	ICON_TRANSFER *state;
	s32 overlay;

	overlay = 0;
	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));

	if(glyph >= mnIconCountItemIcons())
		glyph = 0;

	if(withBase != 0) {
		state->encoded = Icon_BasePointerTable[ICON_BASE_ITEM];
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
		*cacheSlot = syVramCacheFindFreeEntry();

	*tileOut =
		syVramCacheLoad(*cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
}

/* Draws an item's icon into the glyph buffer from its inventory code */
/* (INVENTORY_ENCODING) with the layers asked for (ICON_ flags): the frame,
 */
/* the equipped, broken and artifact marks, and the count. Returns 256, which */
/* no caller reads, or -1 without the glyph buffer. */
s32 mnItemIconCompose(u32 code, u32 layers) {
	s32 overlay;
	ITEM_DEFINITION *item;
	ICON_TRANSFER *state;
	s32 count;

	overlay = 0;
	count = 0;
	item = gmItemGetDirectStub(code & INVENTORY_ITEM_ID_MASK);
	state = Ram_HeapSlots->glyphTransfer;
	if(state == NULL)
		return -1;
	if(layers & ICON_BASE) {
		state->encoded = Icon_BasePointerTable[ICON_BASE_ITEM];
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
		state->encoded = Icon_MarkPointers[ICON_MARK_ARTIFACT];
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
	/* Counts up to a full stack show: the ones digit, and over it the tens. */
	if(count != 0 && count <= INVENTORY_STACK_MAX) {
		s32 ones = count % 10;

		state->encoded = Icon_DigitPointers[ones];
		state->width = 2;
		state->height = 2;
		mnIconDecode(state, 1);
		if(count / 10 != 0) {
			state->encoded =
				Icon_DigitPointers[count / 10 + (ICON_DIGIT_TENS_FIRST - 1)];
			state->width = 2;
			state->height = 2;
			mnIconDecode(state, 1);
		}
	}
	return 256;
}

/* Draws an item's icon from its inventory code with the layers asked for */
/* (mnItemIconCompose) and loads it into VRAM cache entry cacheSlot. */
s32 mnItemIconLoadLayersIntoSlot(s32 inventoryCode, s32 layers, s32 cacheSlot) {
	ICON_TRANSFER *state;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	mnItemIconCompose(inventoryCode, layers);
	syVramCacheLoad(cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return 1;
}

/* Draws one of the other icons (ICON_KIND_PATTERN) and loads it into */
/* cacheSlot, as mnItemIconLoadLayersIntoSlot does an item's. */
s32 mnIconBuildMiscIconTiles(s32 icon, s32 unused, s32 cacheSlot) {
	ICON_TRANSFER *state;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	mnIconDrawMisc(icon);
	syVramCacheLoad(cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return 1;
}

/* Decodes item code's icon (the first icon for code 0) into the glyph buffer.
 */
void mnIconDrawItem(s32 code) {
	ICON_TRANSFER *state = Ram_HeapSlots->glyphTransfer;
	ITEM_DEFINITION *item = gmItemGetDirectStub(INVENTORY_ITEM_ID_MASK & code);

	if(code != 0)
		state->encoded = ItemIcons[item->icon];
	else
		state->encoded = ItemIcons[0];
	state->width = 2;
	state->height = 2;
	mnIconDecode(state, 0);
}

/* Builds the Psynergy icon of a battle action, as mnIconBuildAbilityIconTiles.
 */
void mnAbilityBuildIconTiles(s32 action, s32 withBase, s32 *slot, s32 *tile,
							 s32 reuse) {
	mnIconBuildAbilityIconTiles(btAbilityGet(action)->icon, withBase, slot,
								tile, reuse);
}

/* Builds Psynergy icon glyph, over the icon base when withBase is set, and */
/* loads it into *cacheSlot (a free slot unless reuse is set). Returns the tile
 */
/* in *tileOut. */
void mnIconBuildAbilityIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
								 s32 *tileOut, s32 reuse) {
	ICON_TRANSFER *state;
	s32 overlay;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	overlay = 0;

	if(glyph >= mnIconCountPsynergyIcons())
		glyph = 0;

	if(withBase != 0) {
		state->encoded = Icon_BasePointerTable[ICON_BASE_ITEM];
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
		*cacheSlot = syVramCacheFindFreeEntry();

	*tileOut =
		syVramCacheLoad(*cacheSlot, 2 * 2 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
}

/* Draws one of the other icons into the glyph buffer. */
void mnIconDrawMisc(u32 index) {
	ICON_TRANSFER *state = Ram_HeapSlots->glyphTransfer;

	state->encoded = Icon_MiscIconPointers[index];
	state->width = 2;
	state->height = 2;
	mnIconDecode(state, 0);
}

/* The icon resource's row of the first sprite portrait, */
/* PORTRAIT_FIRST_SPRITE_PORTRAIT. */
#define PORTRAIT_FIRST_SPRITE_ICON 16

/* Loads a 32x32 icon from the icon resource into sprite tile slot *slot (a */
/* free slot unless reuse is set) and its colours into the sprite palette */
/* named by palette. Returns the tile in *tile. */
/* Each icon of the resource has its PALETTE_COLORS colours before its encoded
 */
/* tiles. Portraits (mnPortraitCreate) come through here. */
void mnIconLoadWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
						   s32 palette, s32 reuse) {
	ICON_TRANSFER *state;
	u16 *table;
	u8 *entry;
	u32 index;

	state = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	table = syResourceGetTableEntry(RESOURCE_ICONS);
	if(icon < PORTRAIT_FIRST_SPRITE_PORTRAIT)
		index = icon;
	else
		index =
			icon - PORTRAIT_FIRST_SPRITE_PORTRAIT + PORTRAIT_FIRST_SPRITE_ICON;
	entry = (u8 *)table + table[index];
	state->encoded = entry + PALETTE_COLORS * sizeof(u16);
	state->width = 4;
	state->height = 4;
	mnIconDecode(state, 0);
	if(reuse == 0)
		*slot = syVramCacheFindFreeEntry();
	*tile = syVramCacheLoad(*slot, 4 * 4 * TILE_SIZE_4BPP, state->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	syDMAXCopy(entry, (void *)&OBJ_PLTT_COLOR(palette, 0),
			   DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
}

/* Unused: does nothing. */
UNUSED void mnIconReservedNoOp(void) {}

/* The tile functions syRamCodeLoad copies to RAM to run, and their sizes, as */
/* these symbols' addresses. */
extern u8 Tile_Decompress4bppCode[];
extern u8 Tile_ExpandMaskedCode[];
extern u8 Tile_ExpandOpaqueCode[];
extern u8 Tile_Decompress4bppCodeSize[];
extern u8 Tile_ExpandMaskedCodeSize[];
extern u8 Tile_ExpandOpaqueCodeSize[];

/* Decodes a compressed icon into the glyph buffer and expands it to tiles, */
/* opaque for the first layer, or over what is there, its colour 0 clear, when
 */
/* overlay is set. glyph->width and height are the icon's size in tiles. */
void mnIconDecode(ICON_TRANSFER *glyph, s32 overlay) {
	void *code;

	syRamCodeLoad(code, Tile_Decompress4bppCode, Tile_Decompress4bppCodeSize);
	((void (*)(const void *, u8 *))code)(glyph->encoded, glyph->input);
	syHeapRelease(code);
	if(overlay)
		syRamCodeLoad(code, Tile_ExpandMaskedCode, Tile_ExpandMaskedCodeSize);
	else
		syRamCodeLoad(code, Tile_ExpandOpaqueCode, Tile_ExpandOpaqueCodeSize);
	((void (*)(u8 *, u8 *, u32, u32))code)(
		glyph->input, glyph->tiles, (u16)glyph->width, (u16)glyph->height);
	syHeapRelease(code);
}
