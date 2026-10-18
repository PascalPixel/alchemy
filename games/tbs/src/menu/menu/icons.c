// Part of menu.c, the menu object, compiled only through it.
// The stat arrows and the other icons the menus load into VRAM blocks.

#include "../menu_internal.h"

// Loads each menu icon resource whose cache slot was acquired.
void Resource_LoadPairedBlocksIfAvailable(void) {
  s32 firstSlot;
  s32 secondSlot;
  struct InventoryMenuState *activeState;

  activeState = gMenuWork;
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

// Releases both cached icon resources used by the menu.
void Menu_ResetTwoResourceEntries(void) {
  struct InventoryMenuState *state;

  state = gMenuWork;
  Resource_ResetEntry((u16)state->resourceSlots[0]);
  Resource_ResetEntry((u16)state->resourceSlots[1]);
}

// Decodes one icon into a temporary buffer and reloads its VRAM slot.
s32 UiIcon_LoadResourceIntoSlot(s32 resourceId, s32 slot) {
  s32 decodedResource;
  s32 selectedSlot;
  s32 firstTile;
  s32 allocationBits;

  allocationBits = (s32)Runtime_AllocateBlock(0x11, 0x608);
  selectedSlot = slot;
  Ability_LoadGlyphFar(resourceId, 0, &selectedSlot, &decodedResource, 1);
  firstTile = VramCache_Reload(slot, allocationBits + 0x400);
  Runtime_ReleaseHeapBlock(0x11);
  return firstTile;
}

// Loads an icon into a free cache slot and creates its sprite when possible.
s32 UiIcon_CreateWithLoadedResource(struct UiWindow *window, s32 x, s32 y,
                                    s32 resourceId) {
  s32 slot;

  slot = Resource_FindFreeEntry();
  if (slot != 0x60) {
    UiIcon_LoadResourceIntoSlot(resourceId, slot);
    RenderOutput_CreateFar(slot, ICON_SPRITE_16X16, window, x, y);
  }
}

// Creates a stat-change arrow from one of the two loaded resources.
s32 UiIcon_CreateStatChangeArrow(struct UiWindow *window, s32 x, s32 y,
                                 s32 variant) {
  struct RenderOutput *object;
  u32 resource;
  s32 selectedVariant = variant;
  struct UiWindow *targetWindow = window;
  s32 targetX = x;
  struct InventoryMenuState *menu = gMenuWork;

  if (selectedVariant == 0)
    resource = (u16)menu->resourceSlots[0];
  else
    resource = (u16)menu->resourceSlots[1];
  object = RenderOutput_CreateFar(resource, ICON_SPRITE_16X16, targetWindow,
                                  targetX, y);
  if (object == 0) {
    /* FAKEMATCH: the redundant failure branch retains the argument
     * lifetimes through CSE; plain return -1 changes 16 aligned rows. */
    if (variant || targetX)
      return -1;
    else
      return -1;
  }
  object->kind = 0;
  object->animStep = 0;
  object->active = ICON_SHOWN;
  return 1;
}

// Creates an arrow with the vertical adjustment for its resource variant.
s32 UiIcon_DrawVariantWithTileOffset(struct UiWindow *window, s32 x, s32 y,
                                     s32 variant) {
  struct RenderOutput *object;
  u32 resource;
  s32 selectedVariant = variant;
  struct UiWindow *targetWindow = window;
  s32 targetX = x;
  struct InventoryMenuState *menu = gMenuWork;

  if (selectedVariant == 0) {
    resource = (u16)menu->resourceSlots[0];
    y -= 3;
  } else {
    resource = (u16)menu->resourceSlots[1];
    y -= 4;
  }
  object = RenderOutput_CreateFar(resource, ICON_SPRITE_16X16, targetWindow,
                                  targetX, y);
  if (object == 0) {
    /* FAKEMATCH: the redundant failure branch retains the argument
     * lifetimes through CSE; plain return -1 changes 22 aligned rows. */
    if (selectedVariant || x)
      return -1;
    else
      return -1;
  }
  object->kind = 0;
  object->animStep = 0;
  object->active = ICON_SHOWN;
  return 1;
}
