/* @file */
/* The stat and Djinn arrows and the Psynergy icons the menus load into VRAM */
/* blocks. */
/* Part of menu.c. */

#include "menu_internal.h"

/* Loads the Djinn menu's up and down arrows (djinn_arrow_up and */
/* djinn_arrow_down) into two free VRAM blocks, kept in the state's */
/* resourceSlots[0] and [1] for mnIconCreateDjinnArrow. */
void mnLoadDjinnArrows(void) {
	s32 firstSlot;
	s32 secondSlot;
	ITEM_MENU_STATE *activeState;

	activeState = MenuState;
	/* syVramCacheFindFreeEntry returns VRAM_CACHE_ENTRY_COUNT (96), never -1,
	 */
	/* when no slot is free, so these tests always pass; syVramCacheLoad refuses
	 */
	/* slot 96 itself. */
	firstSlot = syVramCacheFindFreeEntry();
	activeState->resourceSlots[0] = firstSlot;
	if(firstSlot != -1) {
		syVramCacheLoad(firstSlot, ICON_TILE_BYTES, Menu_DjinnArrowUpTiles);
	}
	secondSlot = syVramCacheFindFreeEntry();
	activeState->resourceSlots[1] = secondSlot;
	if(secondSlot != -1) {
		syVramCacheLoad(secondSlot, ICON_TILE_BYTES, Menu_DjinnArrowDownTiles);
	}
}

/* Releases both cached icon resources used by the menu: the two arrows in the
 */
/* state's resourceSlots, which mnItemMenuRun and mnDjinnMenuRun free as they */
/* close. */
void mnIconMenuReleaseArrows(void) {
	ITEM_MENU_STATE *menu;

	menu = MenuState;
	syVramCacheFreeEntry(menu->resourceSlots[0]);
	syVramCacheFreeEntry(menu->resourceSlots[1]);
}

/* Loads the Psynergy icon of ability abilityId into VRAM block slot, building
 */
/* it (mnAbilityBuildIconTilesStub) in a ICON_TRANSFER in */
/* HEAP_SLOT_ICON_TRANSFER, which is released again. Returns the block's first
 */
/* tile. */
s32 mnIconLoadResourceIntoSlot(s32 abilityId, s32 slot) {
	s32 decodedResource;
	s32 selectedSlot;
	s32 firstTile;
	ICON_TRANSFER *allocation;

	allocation =
		syHeapSlotAllocEwram(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
	selectedSlot = slot;
	mnAbilityBuildIconTilesStub(abilityId, 0, &selectedSlot, &decodedResource,
								1);
	firstTile = syVramCacheReload(slot, (s32)allocation->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	return firstTile;
}

/* Shows ability abilityId's Psynergy icon at (x, y) of window, in a free VRAM
 */
/* block, for the Djinn preview's Psynergy lists; with no block free, nothing.
 */
/* Returns no value; callers ignore it. */
s32 mnIconCreatePsynergyIcon(WINDOW *window, s32 x, s32 y, s32 abilityId) {
	s32 slot;

	slot = syVramCacheFindFreeEntry();
	/* VRAM_CACHE_ENTRY_COUNT means no slot was free. */
	if(slot != VRAM_CACHE_ENTRY_COUNT) {
		mnIconLoadResourceIntoSlot(abilityId, slot);
		mnWindowSprCreateStub(slot, WINDOW_SPRITE_SIZE_16X16, window, x, y);
	}
#ifdef UBFIX
	return 0;
#endif
}

/* Creates a stat-change arrow from one of the two loaded resources: the up */
/* arrow (STAT_ARROW_UP) or the down arrow at (x, y) of window, for */
/* the equipment preview (mnTextDrawStatComparison). */
s32 mnIconCreateStatChangeArrow(WINDOW *window, s32 x, s32 y, s32 variant) {
	WINDOW_SPR *object;
	u32 resource;
	ITEM_MENU_STATE *menu = MenuState;

	if(variant == STAT_ARROW_UP)
		resource = menu->resourceSlots[0];
	else
		resource = menu->resourceSlots[1];
	object =
		mnWindowSprCreateStub(resource, WINDOW_SPRITE_SIZE_16X16, window, x, y);
	if(object == 0) {
		/* FAKEMATCH(dead): a test whose two arms both return -1. */
		if(variant || x)
			return -1;
		else
			return -1;
	}
	object->kind = 0;
	object->anmStep = 0;
	object->motion = ICON_SHOWN;
	return 1;
}

/* Shows the Djinn menu's up arrow (variant 0) or down arrow at (x, y) of */
/* window, as mnIconCreateStatChangeArrow does, raised 3 pixels for the up */
/* arrow and 4 for the down one. */
s32 mnIconCreateDjinnArrow(WINDOW *window, s32 x, s32 y, s32 variant) {
	WINDOW_SPR *object;
	u32 resource;
	ITEM_MENU_STATE *menu = MenuState;

	if(variant == 0) {
		resource = menu->resourceSlots[0];
		y -= 3;
	}
	else {
		resource = menu->resourceSlots[1];
		y -= 4;
	}
	object =
		mnWindowSprCreateStub(resource, WINDOW_SPRITE_SIZE_16X16, window, x, y);
	if(object == 0) {
		/* FAKEMATCH(dead): a test whose two arms both return -1. */
		if(variant || x)
			return -1;
		else
			return -1;
	}
	object->kind = 0;
	object->anmStep = 0;
	object->motion = ICON_SHOWN;
	return 1;
}
