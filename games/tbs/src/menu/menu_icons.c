/// @file
/// The stat arrows and the other icons the menus load into VRAM blocks.
// Part of menu.c.

#include "menu_internal.h"

/// Loads the Djinn menu's up and down arrows (djinn_arrow_up and
/// djinn_arrow_down) into two free VRAM blocks, kept in the state's
/// resourceSlots[0] and [1] for Icon_CreateDjinnArrow.
void Menu_LoadDjinnArrows(void) {
  s32 firstSlot;
  s32 secondSlot;
  struct ItemMenuState *activeState;

  activeState = gMenuState;
  // VramCache_FindFreeEntry returns VRAM_CACHE_ENTRY_COUNT (96), never -1,
  // when no slot is free, so these tests always pass; VramCache_Load refuses
  // slot 96 itself.
  firstSlot = VramCache_FindFreeEntry();
  activeState->resourceSlots[0] = firstSlot;
  if (firstSlot != -1) {
    VramCache_Load(firstSlot, 128, gMenu_DjinnArrowUpTiles);
  }
  secondSlot = VramCache_FindFreeEntry();
  activeState->resourceSlots[1] = secondSlot;
  if (secondSlot != -1) {
    VramCache_Load(secondSlot, 128, gMenu_DjinnArrowDownTiles);
  }
}

/// Releases both cached icon resources used by the menu: the two arrows in the
/// state's resourceSlots, which ItemMenu_Run and DjinnMenu_Run free as they
/// close.
void IconMenu_ReleaseArrows(void) {
  struct ItemMenuState *menu;

  menu = gMenuState;
  VramCache_FreeEntry(menu->resourceSlots[0]);
  VramCache_FreeEntry(menu->resourceSlots[1]);
}

/// Loads the Psynergy icon of ability abilityId into VRAM block slot, building
/// it (Ability_LoadGlyphFar) in a IconTransfer in HEAP_SLOT_ICON_TRANSFER,
/// which is released again. Returns the block's first tile.
s32 Icon_LoadResourceIntoSlot(s32 abilityId, s32 slot) {
  s32 decodedResource;
  s32 selectedSlot;
  s32 firstTile;
  IconTransfer *allocation;

  allocation =
      HeapSlot_AllocEwram(HEAP_SLOT_ICON_TRANSFER, sizeof(IconTransfer));
  selectedSlot = slot;
  Ability_LoadGlyphFar(abilityId, 0, &selectedSlot, &decodedResource, 1);
  firstTile = VramCache_Reload(slot, (s32)allocation->tiles);
  HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
  return firstTile;
}

/// Shows ability abilityId's Psynergy icon at (x, y) of window, in a free VRAM
/// block, for the Djinn preview's Psynergy lists; with no block free, nothing.
/// Returns no value; callers ignore it.
s32 Icon_CreatePsynergyIcon(struct Window *window, s32 x, s32 y,
                            s32 abilityId) {
  s32 slot;

  slot = VramCache_FindFreeEntry();
  // VRAM_CACHE_ENTRY_COUNT means no slot was free.
  if (slot != VRAM_CACHE_ENTRY_COUNT) {
    Icon_LoadResourceIntoSlot(abilityId, slot);
    WindowSprite_CreateFar(slot, OUTPUT_SIZE_16X16, window, x, y);
  }
#ifdef UBFIX
  return 0;
#endif
}

/// Creates a stat-change arrow from one of the two loaded resources: the up
/// arrow (variant 0, STAT_ARROW_UP) or the down arrow at (x, y) of window, for
/// the equipment preview (Text_DrawStatComparison).
s32 Icon_CreateStatChangeArrow(struct Window *window, s32 x, s32 y,
                               s32 variant) {
  struct WindowSprite *object;
  u32 resource;
  struct ItemMenuState *menu = gMenuState;

  if (variant == 0)
    resource = menu->resourceSlots[0];
  else
    resource = menu->resourceSlots[1];
  object = WindowSprite_CreateFar(resource, OUTPUT_SIZE_16X16, window, x, y);
  if (object == 0) {
    /* FAKEMATCH(dead): a test whose two arms both return -1. */
    if (variant || x)
      return -1;
    else
      return -1;
  }
  object->kind = 0;
  object->animationStep = 0;
  object->motion = ICON_SHOWN;
  return 1;
}

/// Shows the Djinn menu's up arrow (variant 0) or down arrow at (x, y) of
/// window, as Icon_CreateStatChangeArrow does, raised 3 pixels for the up
/// arrow and 4 for the down one.
s32 Icon_CreateDjinnArrow(struct Window *window, s32 x, s32 y, s32 variant) {
  struct WindowSprite *object;
  u32 resource;
  struct ItemMenuState *menu = gMenuState;

  if (variant == 0) {
    resource = menu->resourceSlots[0];
    y -= 3;
  } else {
    resource = menu->resourceSlots[1];
    y -= 4;
  }
  object = WindowSprite_CreateFar(resource, OUTPUT_SIZE_16X16, window, x, y);
  if (object == 0) {
    /* FAKEMATCH(dead): a test whose two arms both return -1. */
    if (variant || x)
      return -1;
    else
      return -1;
  }
  object->kind = 0;
  object->animationStep = 0;
  object->motion = ICON_SHOWN;
  return 1;
}
