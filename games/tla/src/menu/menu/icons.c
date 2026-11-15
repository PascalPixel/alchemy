/// @file
/// The stat arrows and the other icons the menus load into VRAM blocks, the
/// five menu icons, and which party members the screens offer.
// Part of menu.c.

#include "../menu_internal.h"
#include "fixed_math.h"
#include "sounds.h"

extern char ResourceId_ItemMenuTilesA;
extern char ResourceId_ItemMenuTilesB;

/// Loads each menu icon resource whose cache slot was acquired: the Djinn
/// menu's up and down arrows (djinn_arrow_up and djinn_arrow_down) into the
/// work's resourceSlots[0] and [1], for Icon_DrawVariantWithTileOffset.
void Resource_LoadPairedBlocksIfAvailable(void) {
  s32 firstSlot;
  s32 secondSlot;
  struct ItemMenuWork *activeState;

  activeState = Ram_HeapSlots->menuWork;
  firstSlot = VramCache_FindFreeEntry();
  activeState->resourceSlots[0] = (s16)firstSlot;
  if (firstSlot != -1) {
    VramBlock_LoadResourceFar(firstSlot, 0x80, (s32)&ResourceId_ItemMenuTilesA);
  }
  secondSlot = VramCache_FindFreeEntry();
  activeState->resourceSlots[1] = (s16)secondSlot;
  if (secondSlot != -1) {
    VramBlock_LoadResourceFar(secondSlot, 0x80,
                              (s32)&ResourceId_ItemMenuTilesB);
  }
}

/// Frees the two VRAM cache entries of the item menu's tiles.
void Menu_ResetTwoResourceEntries(void) {
  struct ItemMenuWork *state = Ram_HeapSlots->menuWork;

  VramCache_FreeEntry((u16)state->resourceSlots[0]);
  VramCache_FreeEntry((u16)state->resourceSlots[1]);
}

/// Decodes one icon into a temporary buffer and reloads its VRAM slot: the
/// Psynergy icon of ability resourceId (Ability_LoadGlyphFar), built in 1544
/// bytes of HEAP_SLOT_GLYPH_TRANSFER, which is released again.
s32 Icon_LoadResourceIntoSlot(s32 resourceId, s32 slot) {
  s32 resourceInfo;
  s32 selectedSlot;
  s32 buffer;
  u8 *allocation;

  allocation = HeapSlot_AllocEwram(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
  selectedSlot = slot;
  Ability_LoadGlyphFar(resourceId, 0, &selectedSlot, &resourceInfo, 1);
  buffer = VramCache_Reload(slot, (s32)(allocation + 0x400));
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  return buffer;
}

/// Loads an icon into a free cache slot and creates its sprite when possible:
/// ability resourceId's Psynergy icon at (x, y) of window, for the Djinn
/// preview's Psynergy lists. Returns no value; callers ignore it.
s32 Icon_CreateWithLoadedResource(struct RenderInput *window, s32 x, s32 y,
                                  s32 resourceId) {
  s32 slot;

  slot = VramCache_FindFreeEntry();
  if (slot != 0x60) {
    Icon_LoadResourceIntoSlot(resourceId, slot);
    WindowSprite_CreateFar(slot, OUTPUT_SIZE_16X16, window, x, y);
  }
}

/// Creates the stat change arrow from the up or down arrow resource the
/// menu loaded.
s32 Icon_CreateStatChangeArrow(struct RenderInput *window, s32 x, s32 y,
                               s32 variant) {
  struct WindowSprite *object;
  u32 resource;
  struct ItemMenuWork *data = Ram_HeapSlots->menuWork;

  if (variant == 0)
    resource = (u16)data->resourceSlots[0];
  else
    resource = (u16)data->resourceSlots[1];
  object = WindowSprite_CreateFar(resource, OUTPUT_SIZE_16X16, window, x, y);
  if (object == 0) {
    /* FAKEMATCH(dead): a test whose two arms are alike. */
    if (variant || x)
      return -1;
    else
      return -1;
  }
  object->kind = 0;
  object->animStep = 0;
  object->motion = ICON_SHOWN;
  return 1;
}

/// As Icon_CreateStatChangeArrow, with the arrow raised 3 pixels for variant
/// 0 and 4 for any other. Returns 1, or -1 when no window sprite is free.
s32 Icon_DrawVariantWithTileOffset(struct RenderInput *window, s32 x, s32 y,
                                   s32 variant) {
  struct WindowSprite *object;
  u32 resource;
  struct ItemMenuWork *data = Ram_HeapSlots->menuWork;

  if (variant == 0) {
    resource = (u16)data->resourceSlots[0];
    y -= 3;
  } else {
    resource = (u16)data->resourceSlots[1];
    y -= 4;
  }
  object = WindowSprite_CreateFar(resource, OUTPUT_SIZE_16X16, window, x, y);
  if (object == 0) {
    /* FAKEMATCH(dead): a test whose two arms are alike. */
    if (variant || x)
      return -1;
    else
      return -1;
  }
  object->kind = 0;
  object->animStep = 0;
  object->motion = ICON_SHOWN;
  return 1;
}

/// Moves through the status grid's irregular rows and returns row * 9 + column.
/// Nothing calls it.
s32 Menu_MoveGridCursor(s32 *cursor, s32 count, s32 columns) {
  struct InputState *input = &gInput;
  s32 result = 0;
  s32 old = *cursor;
  s32 pos = old;

  if (input->repeat & KEY_LEFT) {
    pos--;
    pos = (pos + count) % count;
  }
  if (input->repeat & KEY_RIGHT) {
    pos++;
    pos = (pos + count) % count;
  }
  if (input->repeat & KEY_UP) {
    if (pos - columns >= 0)
      pos -= columns;
  }
  if (input->repeat & KEY_DOWN) {
    if (pos + columns < (columns + count - 1) / columns * columns)
      pos += columns;
    if (pos > count - 1)
      pos = count - 1;
  }
  if (old != pos) {
    Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    result = 1;
  }
  if (old / columns * columns != pos / columns * columns)
    result = 2;
  *cursor = pos;
  return result;
}

/// Shows the up arrow when the page of four starting at `index`'s page has one
/// above it and the down arrow when one follows, among `count` entries.
void Menu_SetPageArrowsA(s32 index, s32 count) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  struct WindowSprite *arrow;

  if (index < 0)
    index += 3;
  index = index >> 2 << 2;
  arrow = menu->pageArrows[0];
  arrow->motion = WINDOW_SPRITE_MOTION_HIDDEN;
  if (index != 0) {
    arrow->motion = WINDOW_SPRITE_MOTION_HOP_UP;
    arrow->animStep = 0;
  }
  arrow = menu->pageArrows[1];
  arrow->motion = WINDOW_SPRITE_MOTION_HIDDEN;
  if (index + 4 < count) {
    arrow->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE;
    arrow->animStep = 0;
  }
}

/// As Menu_SetPageArrowsA, with the arrows hopping left and right: the Djinn
/// menu's columns of party members.
void Menu_SetPageArrowsB(s32 index, s32 count) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  struct WindowSprite *arrow;

  if (index < 0)
    index += 3;
  index = index >> 2 << 2;
  arrow = menu->pageArrows[0];
  arrow->motion = WINDOW_SPRITE_MOTION_HIDDEN;
  if (index != 0) {
    arrow->motion = WINDOW_SPRITE_MOTION_HOP_LEFT;
    arrow->animStep = 0;
  }
  arrow = menu->pageArrows[1];
  arrow->motion = WINDOW_SPRITE_MOTION_HIDDEN;
  if (index + 4 < count) {
    arrow->motion = WINDOW_SPRITE_MOTION_HOP_RIGHT;
    arrow->animStep = 0;
  }
}

/// Sets the page arrows for `index` and sets the chosen party member's figure
/// apart from the others on its page of four: its y 30, theirs 26.
void Menu_HighlightPageEntry(s32 unused, s32 index, s32 count) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  s32 top = index / 4 * 4;
  s32 i;

  Menu_SetPageArrowsA(index, count);
  for (i = 0; i <= 3; i++) {
    if (top + i == index)
      menu->memberY[i] = 30;
    else
      menu->memberY[i] = 26;
  }
}

/// A VBlank task while the party figures show: places each party member's
/// figure at its menu position and scale through the projected placement.
void Menu_UpdateEntryObjectTransforms(void) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  s32 scale[2];
  s32 transform[4];
  s32 i;

  if (menu->memberSpritesShown != 0) {
    for (i = 0; i <= 7; i++) {
      s32 top = 0x728000 - (menu->memberY[i] << 15);

      if (menu->memberSprites[i] != NULL) {
        scale[0] = menu->memberScale[i];
        scale[1] = menu->memberScale[i];
        transform[0] = menu->memberX[i] << 16;
        transform[1] = top;
        transform[2] = (menu->memberY[i] << 16) + top;
        transform[3] = 0;
        Render_ApplyProjectedPlacementAtDepthFar(
            menu->memberSprites[i], transform, scale, Q16_ONE / 4, 250);
      }
    }
  }
}

s32 Party_GetCharacterSpriteFar(s32 id);

/// Menu_CreateOwnerEntryObjects for two characters (-1 for none): their
/// figures in entries 0 and 1, as the Djinn preview shows the pair a Djinni
/// passes between.
void Menu_CreatePairEntryObjects(s32 first, s32 second) {
  struct ItemMenuWork *menu = (struct ItemMenuWork *)Ram_HeapSlots->menuWork;
  struct Sprite *figure;
  s32 id;
  s32 i;

  for (i = 0; i < 8; i++) {
    figure = menu->memberSprites[i];
    if (figure != NULL) {
      Sprite_DestroyFar(figure);
      menu->memberSprites[i] = NULL;
      menu->memberSpriteIds[i] = -1;
    }
  }
  for (i = 0; i < 2; i++) {
    id = first;
    if (i != 0)
      id = second;
    if (id != -1) {
      menu->memberSpriteIds[i] = id;
      figure = Sprite_CreateFar(Party_GetCharacterSpriteFar(id));
      if (figure != NULL) {
        Sprite_StartAnimationFar(figure, 1);
        figure->image.priority = 0;
        ((u8 *)figure)[26] = 0;
      }
      menu->memberSprites[i] = figure;
    }
  }
}

/// Replaces the menu's party figures with up to four party members' figures,
/// from party place first on, standing (animation 1), and counts the party.
void Menu_CreateOwnerEntryObjects(s32 first) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  u16 owners[14];
  u8 *object;
  s32 i;

  for (i = 0; i < 8; i++) {
    object = menu->memberSprites[i];
    if (object != NULL) {
      Sprite_DestroyFar(object);
      menu->memberSprites[i] = NULL;
      menu->memberSpriteIds[i] = -1;
    }
  }
  menu->sideCount[0] = Party_ListMembersFar(owners);
  for (i = 0; i < 4 && owners[first + i] != 255; i++) {
    menu->memberSpriteIds[i] = owners[first + i];
    object = Sprite_CreateFar(Party_GetCharacterSpriteFar(owners[first + i]));
    if (object != NULL) {
      Sprite_StartAnimationFar(object, 1);
      ((s8 *)object)[9] &= -13;
      object[26] = 0;
    }
    menu->memberSprites[i] = object;
  }
}

/// Creates the first four party members' figures in a row from tile (originX,
/// originY) inside window, three tiles apart, and starts placing them each
/// frame (Menu_UpdateEntryObjectTransforms).
void Menu_PlaceOwnerEntryObjects(struct RenderInput *window, s32 originX,
                                 s32 originY) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  s32 i;

  for (i = 7; i >= 0; i--)
    menu->memberSprites[i] = NULL;
  Menu_CreateOwnerEntryObjects(0);
  for (i = 0; i <= 3; i++) {
    if (menu->memberSprites[i] != NULL) {
      menu->memberX[i] = (i * 3 + ((s16)window->x + originX)) * 8;
      menu->memberY[i] = ((s16)window->y + originY) * 8 + 16;
      menu->memberScale[i] = Q16_ONE;
    }
  }
  menu->memberSpritesShown = ICON_SHOWN;
  Task_Add(Menu_UpdateEntryObjectTransforms, TASK_PASS_VBLANK);
}

/// Destroys the menu's party figures and stops placing them.
void Menu_ReleaseEntryObjects(void) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  void **objects = menu->memberSprites;
  s32 i;

  for (i = 7; i >= 0; i--) {
    void *object = *objects++;

    if (object != NULL)
      Sprite_DestroyFar(object);
  }
  Task_Remove(Menu_UpdateEntryObjectTransforms);
}

/// Shows the Djinn figures of the top row (nonzero) or hides them (0).
void DjinnFigures_SetShown(s32 hidden) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;

  menu->figuresShown = hidden;
}

/// A VBlank task while the Djinn figures show: places each at its position,
/// depth and scale; a positive scale eases a quarter of the way to full size
/// each frame, a negative one holds at its size.
void DjinnFigures_UpdateTopRow(void) {
  struct ItemMenuWork *menu = (struct ItemMenuWork *)Ram_HeapSlots->menuWork;
  struct SpriteScale scale;
  struct SpritePlacement position;
  s32 i;

  if (menu->figuresShown == 0)
    return;
  for (i = 0; i <= 3; i++) {
    struct Sprite *figure = menu->figures[i];

    if (figure != NULL) {
      s32 placeY = 0x00728000 - (menu->memberY[i] << 15);
      s32 value = menu->figureScales[i];

      s32 next;

      if (value < 0) {
        next = -value;
        scale.x = next;
        scale.y = next;
      } else {
        next = value + (0x10000 - value) / 4;
        scale.x = next;
        scale.y = next;
        menu->figureScales[i] = next;
      }
      position.x = menu->figureX[i] << 16;
      position.height = placeY;
      position.y = (menu->figureY[i] << 16) + placeY;
      position.groundHeight = 0;
      Render_ApplyProjectedPlacementAtDepthFar(figure, &position, &scale,
                                               menu->figureDepths[i], 250);
    }
  }
}

extern s32 gDjinnFigures_SpriteIds[];

/// Replaces the four Djinn figures with one Djinni of each element
/// (gDjinnFigures_SpriteIds) walking (animation 2), at (16, 200) until the
/// screen moves them, and starts placing them each frame
/// (DjinnFigures_UpdateTopRow).
void DjinnFigures_InitializeTopRow(void) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  s8 *object;
  s32 i;

  for (i = 0; i < 4; i++) {
    if (menu->figures[i] != NULL) {
      Sprite_DestroyFar(menu->figures[i]);
      menu->figures[i] = NULL;
    }
  }
  for (i = 0; i < 4; i++) {
    object = Sprite_CreateFar(gDjinnFigures_SpriteIds[i]);
    if (object != NULL) {
      Sprite_StartAnimationFar(object, 2);
      object[9] &= -13;
    }
    menu->figures[i] = object;
    menu->figureX[i] = 16;
    menu->figureY[i] = 200;
    menu->figureScales[i] = Q16_ONE;
    menu->figureDepths[i] = 0x4000;
  }
  menu->figuresShown = 1;
  Task_Add(DjinnFigures_UpdateTopRow, TASK_PASS_VBLANK);
}

/// Destroys the four Djinn figures and stops placing them.
void DjinnFigures_ReleaseTopRow(void) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  s32 i;

  for (i = 0; i < 4; i++) {
    if (menu->figures[i] != NULL) {
      Sprite_DestroyFar(menu->figures[i]);
      menu->figures[i] = NULL;
    }
  }
  Task_Remove(DjinnFigures_UpdateTopRow);
}

/// Replaces Djinn figure index with a Djinni of element kind
/// (gDjinnFigures_SpriteIds) playing animation.
void DjinnFigures_ReplaceSlot(s32 index, s32 kind, s32 animation) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  s8 *object;

  if (menu->figures[index] != NULL) {
    Sprite_DestroyFar(menu->figures[index]);
    menu->figures[index] = NULL;
  }
  object = Sprite_CreateFar(gDjinnFigures_SpriteIds[kind]);
  if (object != NULL) {
    Sprite_StartAnimationFar(object, animation);
    object[9] &= -13;
  }
  menu->figures[index] = object;
}

/// Moves Djinn figure index to screen pixel (x, y), if it exists.
void DjinnFigures_SetSlotPosition(s32 index, s32 x, s32 y) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;

  if (menu->figures[index] != NULL) {
    menu->figureX[index] = x;
    menu->figureY[index] = y;
  }
}

/// Sets the depth Djinn figure index is drawn at, if it exists.
void DjinnFigures_SetSlotDepth(s32 index, s32 frame) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;

  if (menu->figures[index] != NULL)
    menu->figureDepths[index] = frame;
}

/// Sets Djinn figure index's scale, if it exists: positive grows towards full
/// size, negative holds at its size.
void DjinnFigures_SetSlotPhase(s32 index, s32 scale) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;

  if (menu->figures[index] != NULL)
    menu->figureScales[index] = scale;
}

/// Queues the item menu's five menu icons for drawing while they show.
void MenuIcon_PushSlots(void) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  struct IconSprite *slot;
  s32 i;

  if (menu->menuIconsShown != 0) {
    slot = menu->menuIcons;
    for (i = 4; i >= 0; i--) {
      Oam_AddSpriteAtDepth(slot, 240);
      slot++;
    }
  }
}

extern u16 gData_08105a50[5];

/// Loads the five menu icons of gData_08105a50 into free VRAM entries, gives
/// each slot its sprite attributes and tile, keeps the entries for
/// MenuIcon_ReleaseSlots and starts MenuIcon_PushSlots.
void MenuIcon_LoadSlots(void) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  struct IconSprite *slot = menu->menuIcons;
  u16 *ids = gData_08105a50;
  u16 *resource = menu->menuIconSlots;
  s32 entry;
  s32 i;
  s32 offset = 0;

  for (i = 0; i < 5; i++) {
    entry = VramCache_FindFreeEntry();
    if (entry <= 95) {
      u32 *attributes;

      /* FAKEMATCH(temporary): the table read at a byte offset of its own. */
      Resource_LoadByModeIntoSlotFar(8, *(u16 *)(offset + (u32)ids), entry, 0);
      attributes = (u32 *)&slot->attr;
      *attributes++ = OUTPUT_SIZE_16X16;
      *attributes = 0;
      slot->attr.tile = gVramCacheEntries[entry].offset >> 5;
    }
    *resource++ = entry;
    slot++;
    offset += 2;
  }
  menu->menuIconsShown = ICON_SHOWN;
  Task_Add(MenuIcon_PushSlots, TASK_PASS_VBLANK + 10);
}

void MenuIcon_ReleaseSlots(void) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;
  u16 *resource;
  s32 i;

  Task_Remove(MenuIcon_PushSlots);
  resource = menu->menuIconSlots;
  for (i = 4; i >= 0; i--)
    VramCache_FreeEntry(*resource++);
}

void MenuIcon_SetSlot(s32 index, u32 x, s32 y) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;

  menu->menuIcons[index].attr.x = x;
  menu->menuIcons[index].attr.y = y;
}

void MenuIcon_SetActive(s32 active) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuWork;

  menu->menuIconsShown = active;
}

/// Lists usable entries with icons and names, or "none" when no entries
/// qualify.
void MenuPanel_DrawEntryList(struct RenderInput *panel, s32 x, s32 y,
                             const u8 *enabled) {
  s32 count = 0;
  s32 icon_x = (s16)panel->x * 8 + x + 8;
  s32 icon_y = (s16)panel->y * 8 + y + 8;
  s32 i;

  for (i = 0; i <= 4; i++) {
    if (enabled[i] != 0) {
      MenuIcon_SetSlot(i, icon_x, icon_y);
      Text_DrawMessageAsTilesFar((s32)MsgStatusNameDown + i, panel, x + 16, y);
      y += 16;
      icon_y += 16;
      count++;
    } else {
      MenuIcon_SetSlot(i, -16, -16);
    }
  }
  if (count == 0)
    Text_DrawMessageAsTilesFar((s32)MsgStatusNameNormal, panel, x, y);
}

/// Fills ailments[0..4] with party member memberId's statuses:
/// down (only when requested is 1), poison or venom, curse and haunt.
s32 StatusScreen_BuildAvailability(u8 *output, s32 requested, s32 id) {
  struct Unit *state;
  s32 i;
  s32 zero;
  s32 count;
  s32 mode;

  state = Unit_Get(id);
  zero = 0;
  for (i = 4; i >= 0; i--)
    output[i] = zero;

  count = 0;
  if (state->hp == 0 && requested == 1) {
    output[0] = requested;
    count = 1;
  }

  mode = state->poison;
  if (mode != 0) {
    if (mode == 1)
      output[1] = mode;
    else
      output[2] = 1;
    count++;
  }
  if (state->curse != 0) {
    output[3] = 1;
    count++;
  }
  if (state->haunt != 0) {
    output[4] = 1;
    count++;
  }
  return count;
}
