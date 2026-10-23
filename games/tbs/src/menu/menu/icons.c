/// @file
/// The stat arrows and the other icons the menus load into VRAM blocks.
// Part of menu.c.

#include "../menu_internal.h"

/// Loads each menu icon resource whose cache slot was acquired: the Djinn
/// menu's up and down arrows (djinn/arrowup and arrowdn) into the work's
/// resourceSlots[0] and [1], for Icon_DrawVariantWithTileOffset.
/// DjinnMenu_Run calls it and IconMenu_ReleaseArrows frees them.
void Resource_LoadPairedBlocksIfAvailable(void) {
  s32 firstSlot;
  s32 secondSlot;
  struct ItemMenuWork *activeState;

  activeState = gMenuWork;
  // Resource_FindFreeEntry returns VRAM_CACHE_ENTRY_COUNT (96), never -1,
  // when no slot is free, so these tests always pass; VramCache_Load refuses
  // slot 96 itself.
  firstSlot = Resource_FindFreeEntry();
  activeState->resourceSlots[0] = firstSlot;
  if (firstSlot != -1) {
    VramCache_Load(firstSlot, 0x80, gMenu_PairSpriteATiles);
  }
  secondSlot = Resource_FindFreeEntry();
  activeState->resourceSlots[1] = secondSlot;
  if (secondSlot != -1) {
    VramCache_Load(secondSlot, 0x80, gMenu_PairSpriteBTiles);
  }
}

/// Releases both cached icon resources used by the menu: the two arrows in the
/// work's resourceSlots, which ItemMenu_Run and DjinnMenu_Run free as they
/// close.
void IconMenu_ReleaseArrows(void) {
  struct ItemMenuWork *menu;

  menu = gMenuWork;
  Resource_ResetEntry((u16)menu->resourceSlots[0]);
  Resource_ResetEntry((u16)menu->resourceSlots[1]);
}

/// Decodes one icon into a temporary buffer and reloads its VRAM slot: the
/// Psynergy icon of ability resourceId (Ability_LoadGlyphFar), built in 0x608
/// bytes of HEAP_SLOT_GLYPH_TRANSFER, which is released again. Returns the
/// slot's first sprite tile (VramCache_Reload).
s32 Icon_LoadResourceIntoSlot(s32 resourceId, s32 slot) {
  s32 decodedResource;
  s32 selectedSlot;
  s32 firstTile;
  s32 allocationBits;

  allocationBits = (s32)AllocHeapSlotEwram(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
  selectedSlot = slot;
  Ability_LoadGlyphFar(resourceId, 0, &selectedSlot, &decodedResource, 1);
  firstTile = VramCache_Reload(slot, allocationBits + 0x400);
  FreeHeapSlot(HEAP_SLOT_GLYPH_TRANSFER);
  return firstTile;
}

/// Loads an icon into a free cache slot and creates its sprite when possible:
/// ability resourceId's Psynergy icon at (x, y) of window, for the Djinn
/// preview's Psynergy lists. Returns no value; callers ignore it.
s32 Icon_CreateWithLoadedResource(struct Window *window, s32 x, s32 y,
                                  s32 resourceId) {
  s32 slot;

  slot = Resource_FindFreeEntry();
  // 0x60 is VRAM_CACHE_ENTRY_COUNT: no slot free.
  if (slot != 0x60) {
    Icon_LoadResourceIntoSlot(resourceId, slot);
    WindowSprite_CreateFar(slot, ICON_SPRITE_16X16, window, x, y);
  }
#ifdef UBFIX
  return 0;
#endif
}

/// Creates a stat-change arrow from one of the two loaded resources: the up
/// arrow (variant 0, STAT_ARROW_UP) or the down arrow at (x, y) of window, for
/// the equipment preview (Text_DrawStatComparison). The sprite belongs to
/// the window. Returns 1, or -1 when no render output is free, in which case
/// WindowSprite_Create has also released the arrow's shared VRAM slot.
s32 Icon_CreateStatChangeArrow(struct Window *window, s32 x, s32 y,
                               s32 variant) {
  struct WindowSprite *object;
  u32 resource;
  s32 selectedVariant = variant;
  struct Window *targetWindow = window;
  s32 targetX = x;
  struct ItemMenuWork *menu = gMenuWork;

  if (selectedVariant == 0)
    resource = (u16)menu->resourceSlots[0];
  else
    resource = (u16)menu->resourceSlots[1];
  object = WindowSprite_CreateFar(resource, ICON_SPRITE_16X16, targetWindow,
                                  targetX, y);
  if (object == 0) {
    /* FAKEMATCH(dead): a test whose two arms both return -1, which emits
     * nothing but keeps variant and x live across the call; as a plain
     * return -1, 10 of 37 instructions differ in tbs-en. */
    if (variant || targetX)
      return -1;
    else
      return -1;
  }
  object->kind = 0;
  object->animationStep = 0;
  object->motion = ICON_SHOWN;
  return 1;
}

/// Creates an arrow with the vertical adjustment for its resource variant:
/// as Icon_CreateStatChangeArrow, raised 3 pixels for the up arrow (variant
/// 0) and 4 for the down arrow. The Djinn preview draws one beside each stat
/// that would change. Returns 1, or -1 when no render output is free (and the
/// arrow's VRAM slot released, as there).
s32 Icon_DrawVariantWithTileOffset(struct Window *window, s32 x, s32 y,
                                   s32 variant) {
  struct WindowSprite *object;
  u32 resource;
  s32 selectedVariant = variant;
  struct Window *targetWindow = window;
  s32 targetX = x;
  struct ItemMenuWork *menu = gMenuWork;

  if (selectedVariant == 0) {
    resource = (u16)menu->resourceSlots[0];
    y -= 3;
  } else {
    resource = (u16)menu->resourceSlots[1];
    y -= 4;
  }
  object = WindowSprite_CreateFar(resource, ICON_SPRITE_16X16, targetWindow,
                                  targetX, y);
  if (object == 0) {
    /* FAKEMATCH(dead): a test whose two arms both return -1, which emits
     * nothing but keeps variant and x live across the call; as a plain
     * return -1, 13 of 41 instructions differ in tbs-en. */
    if (selectedVariant || x)
      return -1;
    else
      return -1;
  }
  object->kind = 0;
  object->animationStep = 0;
  object->motion = ICON_SHOWN;
  return 1;
}
