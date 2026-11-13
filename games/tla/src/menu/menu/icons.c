/// @file
/// The stat arrows and the other icons the menus load into VRAM blocks, the
/// five menu icons, and which party members the screens offer.
// Part of menu.c.

#include "../menu_internal.h"

struct ItemMenuBlocks {
  u8 unk000[0x4ee];
  s16 resourceSlots[2];
};

extern char ResourceId_ItemMenuTilesA;
extern char ResourceId_ItemMenuTilesB;

void Resource_LoadPairedBlocksIfAvailable(void) {
  s32 firstSlot;
  s32 secondSlot;
  struct ItemMenuBlocks *activeState;

  activeState = Ram_HeapSlots->menuRuntime;
  firstSlot = Resource_FindFreeEntry();
  activeState->resourceSlots[0] = (s16)firstSlot;
  if (firstSlot != -1) {
    VramBlock_LoadResourceFar(firstSlot, 0x80, (s32)&ResourceId_ItemMenuTilesA);
  }
  secondSlot = Resource_FindFreeEntry();
  activeState->resourceSlots[1] = (s16)secondSlot;
  if (secondSlot != -1) {
    VramBlock_LoadResourceFar(secondSlot, 0x80,
                              (s32)&ResourceId_ItemMenuTilesB);
  }
}

void Menu_ResetTwoResourceEntries(void) {
  struct ItemMenuBlocks *state = Ram_HeapSlots->menuRuntime;

  Resource_ResetEntry((u16)state->resourceSlots[0]);
  Resource_ResetEntry((u16)state->resourceSlots[1]);
}

s32 Icon_LoadResourceIntoSlot(s32 resourceId, s32 slot) {
  s32 resourceInfo;
  s32 selectedSlot;
  s32 buffer;
  u8 *allocation;

  allocation = HeapSlot_AllocEwram(0x44, 0x608);
  selectedSlot = slot;
  Ability_LoadGlyphFar(resourceId, 0, &selectedSlot, &resourceInfo, 1);
  buffer = VramCache_Reload(slot, (s32)(allocation + 0x400));
  HeapSlot_Release(0x44);
  return buffer;
}

s32 Icon_CreateWithLoadedResource(struct RenderInput *window, s32 x, s32 y,
                                    s32 resourceId) {
  s32 slot;

  slot = Resource_FindFreeEntry();
  if (slot != 0x60) {
    Icon_LoadResourceIntoSlot(resourceId, slot);
    WindowSprite_CreateFar(slot, OUTPUT_SIZE_16X16, window, x, y);
  }
}

/// Creates the stat change arrow from the up or down arrow resource the
/// menu loaded.
// The Broken Seal's Icon_CreateStatChangeArrow.
s32 Icon_CreateStatChangeArrow(struct RenderInput *window, s32 x, s32 y,
                                 s32 variant) {
  struct WindowSprite *object;
  u32 resource;
  s32 v = variant;
  struct RenderInput *w = window;
  s32 xx = x;
  struct ItemMenuBlocks *data = Ram_HeapSlots->menuRuntime;

  if (v == 0)
    resource = (u16)data->resourceSlots[0];
  else
    resource = (u16)data->resourceSlots[1];
  object = WindowSprite_CreateFar(resource, OUTPUT_SIZE_16X16, w, xx, y);
  if (object == 0) {
    /* FAKEMATCH(dead): a test whose two arms are alike, which keeps the
     * arguments' lifetimes through CSE, as in The Broken Seal; as a plain
     * return -1, 20 of 38 instructions differ in tla-en. */
    if (variant || xx)
      return -1;
    else
      return -1;
  }
  object->kind = 0;
  object->animStep = 0;
  object->active = 1;
  return 1;
}

/// As Icon_CreateStatChangeArrow, with the arrow raised 3 pixels for variant
/// 0 and 4 for any other. Returns 1, or -1 when no window sprite is free.
s32 Icon_DrawVariantWithTileOffset(struct RenderInput *window, s32 x, s32 y,
                                   s32 variant) {
  struct WindowSprite *object;
  u32 resource;
  s32 v = variant;
  struct RenderInput *w = window;
  s32 xx = x;
  struct ItemMenuBlocks *data = Ram_HeapSlots->menuRuntime;

  if (v == 0) {
    resource = (u16)data->resourceSlots[0];
    y -= 3;
  } else {
    resource = (u16)data->resourceSlots[1];
    y -= 4;
  }
  object = WindowSprite_CreateFar(resource, OUTPUT_SIZE_16X16, w, xx, y);
  if (object == 0) {
    /* FAKEMATCH(dead): a test whose two arms are alike, which keeps the
     * arguments' lifetimes through CSE, as in The Broken Seal; as a plain
     * return -1, 26 of 42 instructions differ in tla-en. */
    if (v || x)
      return -1;
    else
      return -1;
  }
  object->kind = 0;
  object->animStep = 0;
  object->active = 1;
  return 1;
}

s32 Menu_MoveGridCursor(s32 *cursor, s32 count, s32 columns) {
  struct InputState *input = &gInput;
  s32 result = 0;
  s32 old = *cursor;
  s32 pos = old;

  if (input->repeat & 0x20) {
    pos--;
    pos = (pos + count) % count;
  }
  if (input->repeat & 0x10) {
    pos++;
    pos = (pos + count) % count;
  }
  if (input->repeat & 0x40) {
    if (pos - columns >= 0)
      pos -= columns;
  }
  if (input->repeat & 0x80) {
    if (pos + columns < (columns + count - 1) / columns * columns)
      pos += columns;
    if (pos > count - 1)
      pos = count - 1;
  }
  if (old != pos) {
    Sound_PlayFar(111);
    result = 1;
  }
  if (old / columns * columns != pos / columns * columns)
    result = 2;
  *cursor = pos;
  return result;
}

void Menu_SetPageArrowsA(s32 index, s32 count) {
  struct MenuPageWork *menu = Ram_HeapSlots->menuRuntime;
  struct MenuArrow *arrow;

  if (index < 0)
    index += 3;
  index = index >> 2 << 2;
  arrow = menu->up;
  arrow->mode = 13;
  if (index != 0) {
    arrow->mode = 17;
    arrow->timer = 0;
  }
  arrow = menu->down;
  arrow->mode = 13;
  if (index + 4 < count) {
    arrow->mode = 15;
    arrow->timer = 0;
  }
}

void Menu_SetPageArrowsB(s32 index, s32 count) {
  struct MenuPageWork *menu = Ram_HeapSlots->menuRuntime;
  struct MenuArrow *arrow;

  if (index < 0)
    index += 3;
  index = index >> 2 << 2;
  arrow = menu->up;
  arrow->mode = 13;
  if (index != 0) {
    arrow->mode = 16;
    arrow->timer = 0;
  }
  arrow = menu->down;
  arrow->mode = 13;
  if (index + 4 < count) {
    arrow->mode = 14;
    arrow->timer = 0;
  }
}

void Menu_HighlightPageEntry(s32 unused, s32 index, s32 count) {
  struct MenuPageWork *menu = Ram_HeapSlots->menuRuntime;
  s32 top = index / 4 * 4;
  s32 i;

  Menu_SetPageArrowsA(index, count);
  for (i = 0; i <= 3; i++) {
    if (top + i == index)
      menu->colours[i] = 30;
    else
      menu->colours[i] = 26;
  }
}

struct MenuEntryWork {
  u8 unk000[0x1e];
  u8 count;
  u8 unk01f[0xd9];
  void *objects[8];
  s16 x[8];
  s16 y[8];
  s32 scale[8];
  s16 ids[8];
  u8 unk168[0x3a1];
  s8 active;
  u8 hidden;
  u8 unk50b[1];
};

void Menu_UpdateEntryObjectTransforms(void) {
  struct MenuEntryWork *menu = Ram_HeapSlots->menuRuntime;
  s32 scale[2];
  s32 transform[4];
  s32 i;

  if (menu->active != 0) {
    for (i = 0; i <= 7; i++) {
      s32 top = 0x728000 - (menu->y[i] << 15);

      if (menu->objects[i] != NULL) {
        scale[0] = menu->scale[i];
        scale[1] = menu->scale[i];
        transform[0] = menu->x[i] << 16;
        transform[1] = top;
        transform[2] = (menu->y[i] << 16) + top;
        transform[3] = 0;
        Func_08020018(menu->objects[i], transform, scale, 0x4000, 250);
      }
    }
  }
}

struct FigureMenuState {
  u8 unk000[0xf8];
  struct AnimationObject *entryObjects[8];
  u8 unk118[0x10];
  s16 rows[4];
  u8 unk130[0x28];
  s16 ids[8];
  u8 unk168[0xb8];
  struct AnimationObject *figures[4];
  s16 tabX[4];
  s16 tabY[4];
  s32 scales[4];
  u16 depths[4];
  u8 unk258[0x2b2];
  s8 active;
  u8 unk50b[1];
};

s32 Func_080c82b8(s32 id);

void Func_08104e38(s32 first, s32 second) {
  struct FigureMenuState *menu =
      (struct FigureMenuState *)Ram_HeapSlots->menuRuntime;
  struct AnimationObject *figure;
  s32 id;
  s32 i;

  for (i = 0; i < 8; i++) {
    figure = menu->entryObjects[i];
    if (figure != NULL) {
      ResourceObject_ReleaseFar(figure);
      menu->entryObjects[i] = NULL;
      menu->ids[i] = -1;
    }
  }
  for (i = 0; i < 2; i++) {
    id = first;
    if (i != 0)
      id = second;
    if (id != -1) {
      menu->ids[i] = id;
      figure = ResourceObject_CreateFar(Func_080c82b8(id));
      if (figure != NULL) {
        Animation_ApplyChildArgumentFar(figure, 1);
        figure->image.priority = 0;
        ((u8 *)figure)[26] = 0;
      }
      menu->entryObjects[i] = figure;
    }
  }
}

void Menu_CreateOwnerEntryObjects(s32 first) {
  struct MenuEntryWork *menu = Ram_HeapSlots->menuRuntime;
  u16 owners[14];
  u8 *object;
  s32 i;

  for (i = 0; i < 8; i++) {
    object = menu->objects[i];
    if (object != NULL) {
      ResourceObject_ReleaseFar(object);
      menu->objects[i] = NULL;
      menu->ids[i] = -1;
    }
  }
  menu->count = Party_ListActiveOwnersFar(owners);
  for (i = 0; i < 4 && owners[first + i] != 255; i++) {
    menu->ids[i] = owners[first + i];
    object = ResourceObject_CreateFar(Func_080c82b8(owners[first + i]));
    if (object != NULL) {
      Animation_ApplyChildArgumentFar(object, 1);
      ((s8 *)object)[9] &= -13;
      object[26] = 0;
    }
    menu->objects[i] = object;
  }
}

struct MenuEntryWindow {
  u8 unk00[12];
  s16 x;
  s16 y;
};

void Menu_PlaceOwnerEntryObjects(struct MenuEntryWindow *window, s32 originX,
                                 s32 originY) {
  struct MenuEntryWork *menu = Ram_HeapSlots->menuRuntime;
  s32 i;

  for (i = 7; i >= 0; i--)
    menu->objects[i] = NULL;
  Menu_CreateOwnerEntryObjects(0);
  for (i = 0; i <= 3; i++) {
    if (menu->objects[i] != NULL) {
      menu->x[i] = (i * 3 + (window->x + originX)) * 8;
      menu->y[i] = (window->y + originY) * 8 + 16;
      menu->scale[i] = Q16_ONE;
    }
  }
  menu->active = 1;
  Task_Add(Menu_UpdateEntryObjectTransforms, TASK_PASS_VBLANK);
}

void Menu_ReleaseEntryObjects(void) {
  struct MenuEntryWork *menu = Ram_HeapSlots->menuRuntime;
  void **objects = menu->objects;
  s32 i;

  for (i = 7; i >= 0; i--) {
    void *object = *objects++;

    if (object != NULL)
      ResourceObject_ReleaseFar(object);
  }
  Task_Remove(Menu_UpdateEntryObjectTransforms);
}

void Menu_SetEntryObjectsHidden(s32 hidden) {
  struct MenuEntryWork *menu = Ram_HeapSlots->menuRuntime;

  menu->hidden = hidden;
}

void MenuIcon_UpdateTransforms(void) {
  struct FigureMenuState *menu =
      (struct FigureMenuState *)Ram_HeapSlots->menuRuntime;
  struct SpriteScale scale;
  struct SpritePlacement position;
  s32 i;

  if (menu->active == 0)
    return;
  for (i = 0; i <= 3; i++) {
    struct AnimationObject *figure = menu->figures[i];

    if (figure != NULL) {
      s32 placeY = 0x00728000 - (menu->rows[i] << 15);
      s32 value = menu->scales[i];

      s32 next;

      if (value < 0) {
        next = -value;
        scale.x = next;
        scale.y = next;
      } else {
        next = value + (0x10000 - value) / 4;
        scale.x = next;
        scale.y = next;
        menu->scales[i] = next;
      }
      position.x = menu->tabX[i] << 16;
      position.height = placeY;
      position.y = (menu->tabY[i] << 16) + placeY;
      position.groundHeight = 0;
      Func_08020018(figure, &position, &scale, menu->depths[i], 250);
    }
  }
}

struct MenuSlotEntry {
  u8 unk00[4];
  u32 attributes;
  u16 tile : 10;
  u16 unk08 : 6;
  u8 unk0a[2];
};

struct MenuIconWork {
  u8 unk000[0x220];
  void *objects[4];
  s16 x[4];
  s16 y[4];
  s32 scale[4];
  s16 frame[4];
  u8 unk258[0x2b2];
  s8 transformsActive;
  u8 unk50b[0x6c1];
  struct MenuSlotEntry slots[5];
  u16 resources[5];
  s8 active;
  u8 unkc13[1];
};

extern s32 gMenuIcon_ResourceIds[];

void Func_081051a8(void) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;
  s8 *object;
  s32 i;

  for (i = 0; i < 4; i++) {
    if (menu->objects[i] != NULL) {
      ResourceObject_ReleaseFar(menu->objects[i]);
      menu->objects[i] = NULL;
    }
  }
  for (i = 0; i < 4; i++) {
    object = ResourceObject_CreateFar(gMenuIcon_ResourceIds[i]);
    if (object != NULL) {
      Animation_ApplyChildArgumentFar(object, 2);
      object[9] &= -13;
    }
    menu->objects[i] = object;
    menu->x[i] = 16;
    menu->y[i] = 200;
    menu->scale[i] = 0x10000;
    menu->frame[i] = 0x4000;
  }
  menu->transformsActive = 1;
  Task_Add(MenuIcon_UpdateTransforms, TASK_PASS_VBLANK);
}

void MenuIcon_ReleaseObjects(void) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;
  s32 i;

  for (i = 0; i < 4; i++) {
    if (menu->objects[i] != NULL) {
      ResourceObject_ReleaseFar(menu->objects[i]);
      menu->objects[i] = NULL;
    }
  }
  Task_Remove(MenuIcon_UpdateTransforms);
}

void MenuIcon_ReplaceObject(s32 index, s32 kind, s32 animation) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;
  s8 *object;

  if (menu->objects[index] != NULL) {
    ResourceObject_ReleaseFar(menu->objects[index]);
    menu->objects[index] = NULL;
  }
  object = ResourceObject_CreateFar(gMenuIcon_ResourceIds[kind]);
  if (object != NULL) {
    Animation_ApplyChildArgumentFar(object, animation);
    object[9] &= -13;
  }
  menu->objects[index] = object;
}

void MenuIcon_SetPosition(s32 index, s32 x, s32 y) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;

  if (menu->objects[index] != NULL) {
    menu->x[index] = x;
    menu->y[index] = y;
  }
}

void MenuIcon_SetPicture(s32 index, s32 frame) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;

  if (menu->objects[index] != NULL)
    menu->frame[index] = frame;
}

void Func_08105350(s32 index, s32 scale) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;

  if (menu->objects[index] != NULL)
    menu->scale[index] = scale;
}

void MenuIcon_PushSlots(void) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;
  struct MenuSlotEntry *slot;
  s32 i;

  if (menu->active != 0) {
    slot = menu->slots;
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
void Func_081053a8(void) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;
  struct MenuSlotEntry *slot = menu->slots;
  u16 *ids = gData_08105a50;
  u16 *resource = menu->resources;
  s32 entry;
  s32 i;
  s32 offset = 0;

  for (i = 0; i < 5; i++) {
    entry = Resource_FindFreeEntry();
    if (entry <= 95) {
      u32 *attributes;

      // As ids[i] the table and index combine into one walking pointer; the
      // byte offset kept apart is what matches.
      Func_08038288(8, *(u16 *)(offset + (u32)ids), entry, 0);
      attributes = &slot->attributes;
      *attributes++ = 0x40000000;
      *attributes = 0;
      slot->tile = gResourceTableEntries[entry].offset >> 5;
    }
    *resource++ = entry;
    slot++;
    offset += 2;
  }
  menu->active = 1;
  Task_Add(MenuIcon_PushSlots, TASK_PASS_VBLANK + 10);
}

void MenuIcon_ReleaseSlots(void) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;
  u16 *resource;
  s32 i;

  Task_Remove(MenuIcon_PushSlots);
  resource = menu->resources;
  for (i = 4; i >= 0; i--)
    Resource_ResetEntry(*resource++);
}

struct MenuSlotAttr {
  u8 mode;
  u8 unk01;
  u16 x : 9;
  u8 unk04[8];
};

void MenuIcon_SetSlot(s32 index, u32 x, s32 mode) {
  u8 *menu = Ram_HeapSlots->menuRuntime;
  /* FAKEMATCH(temporary): the offset as a local of its own, which adds the row
   * and the field offset before the work pointer, as in the shop's ItemIconGrid_SetPosition;
   * as a plain field address, (struct MenuSlotAttr *)(menu +
   * 0xbd0) + index, 6 of 21 instructions differ in tla-en. */
  s32 offset =
      index * sizeof(struct MenuSlotAttr) + 0xbd0; /* slots[index] + 4 */
  struct MenuSlotAttr *slot = (struct MenuSlotAttr *)(menu + offset);

  slot->x = x;
  slot->mode = mode;
}

void MenuIcon_SetActive(s32 active) {
  struct MenuIconWork *menu = Ram_HeapSlots->menuRuntime;

  menu->active = active;
}

struct MenuPanel {
  u8 unk00[12];
  s16 x;
  s16 y;
};

// Lists usable entries with icons and names, or "none" when no entries qualify.
void Func_081054e0(struct MenuPanel *panel, s32 x, s32 y, const u8 *enabled) {
  s32 count = 0;
  s32 icon_x = panel->x * 8 + x + 8;
  s32 icon_y = panel->y * 8 + y + 8;
  s32 i;

  for (i = 0; i <= 4; i++) {
    if (enabled[i] != 0) {
      MenuIcon_SetSlot(i, icon_x, icon_y);
      Text_DrawCharacterAtOffsetFar((s32)MsgStatusNameDown + i, panel, x + 16,
                                    y);
      y += 16;
      icon_y += 16;
      count++;
    } else {
      MenuIcon_SetSlot(i, -16, -16);
    }
  }
  if (count == 0)
    Text_DrawCharacterAtOffsetFar((s32)MsgStatusNameNormal, panel, x, y);
}

s32 CharacterMenu_BuildAvailability(u8 *output, s32 requested, s32 id) {
  struct BattleUnit *state;
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
  if (state->restraint != 0) {
    output[3] = 1;
    count++;
  }
  if (state->haunt != 0) {
    output[4] = 1;
    count++;
  }
  return count;
}
