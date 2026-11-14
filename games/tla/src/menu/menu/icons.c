/// @file
/// The stat arrows and the other icons the menus load into VRAM blocks, the
/// five menu icons, and which party members the screens offer.
// Part of menu.c.

#include "../menu_internal.h"
#include "fixed_math.h"
#include "sounds.h"

extern char ResourceId_ItemMenuTilesA;
extern char ResourceId_ItemMenuTilesB;

void Resource_LoadPairedBlocksIfAvailable(void) {
  s32 firstSlot;
  s32 secondSlot;
  struct ItemMenuWork *activeState;

  activeState = Ram_HeapSlots->menuRuntime;
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

void Menu_ResetTwoResourceEntries(void) {
  struct ItemMenuWork *state = Ram_HeapSlots->menuRuntime;

  VramCache_FreeEntry((u16)state->resourceSlots[0]);
  VramCache_FreeEntry((u16)state->resourceSlots[1]);
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

  slot = VramCache_FindFreeEntry();
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
  struct ItemMenuWork *data = Ram_HeapSlots->menuRuntime;

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
  object->active = ICON_SHOWN;
  return 1;
}

/// As Icon_CreateStatChangeArrow, with the arrow raised 3 pixels for variant
/// 0 and 4 for any other. Returns 1, or -1 when no window sprite is free.
s32 Icon_DrawVariantWithTileOffset(struct RenderInput *window, s32 x, s32 y,
                                   s32 variant) {
  struct WindowSprite *object;
  u32 resource;
  struct ItemMenuWork *data = Ram_HeapSlots->menuRuntime;

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
  object->active = ICON_SHOWN;
  return 1;
}

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

void Menu_UpdateEntryObjectTransforms(void) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;
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
  struct ItemMenuWork *menu = (struct ItemMenuWork *)Ram_HeapSlots->menuRuntime;
  struct AnimationObject *figure;
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
        Animation_ApplyChildArgumentFar(figure, 1);
        figure->image.priority = 0;
        ((u8 *)figure)[26] = 0;
      }
      menu->memberSprites[i] = figure;
    }
  }
}

void Menu_CreateOwnerEntryObjects(s32 first) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;
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
      Animation_ApplyChildArgumentFar(object, 1);
      ((s8 *)object)[9] &= -13;
      object[26] = 0;
    }
    menu->memberSprites[i] = object;
  }
}

struct MenuEntryWindow {
  u8 unk00[12];
  s16 x;
  s16 y;
};

void Menu_PlaceOwnerEntryObjects(struct MenuEntryWindow *window, s32 originX,
                                 s32 originY) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;
  s32 i;

  for (i = 7; i >= 0; i--)
    menu->memberSprites[i] = NULL;
  Menu_CreateOwnerEntryObjects(0);
  for (i = 0; i <= 3; i++) {
    if (menu->memberSprites[i] != NULL) {
      menu->memberX[i] = (i * 3 + (window->x + originX)) * 8;
      menu->memberY[i] = (window->y + originY) * 8 + 16;
      menu->memberScale[i] = Q16_ONE;
    }
  }
  menu->memberSpritesShown = ICON_SHOWN;
  Task_Add(Menu_UpdateEntryObjectTransforms, TASK_PASS_VBLANK);
}

void Menu_ReleaseEntryObjects(void) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;
  void **objects = menu->memberSprites;
  s32 i;

  for (i = 7; i >= 0; i--) {
    void *object = *objects++;

    if (object != NULL)
      Sprite_DestroyFar(object);
  }
  Task_Remove(Menu_UpdateEntryObjectTransforms);
}

void Menu_SetEntryObjectsHidden(s32 hidden) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;

  menu->figuresShown = hidden;
}

void MenuIcon_UpdateTransforms(void) {
  struct ItemMenuWork *menu = (struct ItemMenuWork *)Ram_HeapSlots->menuRuntime;
  struct SpriteScale scale;
  struct SpritePlacement position;
  s32 i;

  if (menu->figuresShown == 0)
    return;
  for (i = 0; i <= 3; i++) {
    struct AnimationObject *figure = menu->figures[i];

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

void DjinnFigures_InitializeTopRow(void) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;
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
      Animation_ApplyChildArgumentFar(object, 2);
      object[9] &= -13;
    }
    menu->figures[i] = object;
    menu->figureX[i] = 16;
    menu->figureY[i] = 200;
    menu->figureScales[i] = Q16_ONE;
    menu->figureDepths[i] = 0x4000;
  }
  menu->figuresShown = 1;
  Task_Add(MenuIcon_UpdateTransforms, TASK_PASS_VBLANK);
}

void DjinnFigures_ReleaseTopRow(void) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;
  s32 i;

  for (i = 0; i < 4; i++) {
    if (menu->figures[i] != NULL) {
      Sprite_DestroyFar(menu->figures[i]);
      menu->figures[i] = NULL;
    }
  }
  Task_Remove(MenuIcon_UpdateTransforms);
}

void MenuIcon_ReplaceObject(s32 index, s32 kind, s32 animation) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;
  s8 *object;

  if (menu->figures[index] != NULL) {
    Sprite_DestroyFar(menu->figures[index]);
    menu->figures[index] = NULL;
  }
  object = Sprite_CreateFar(gDjinnFigures_SpriteIds[kind]);
  if (object != NULL) {
    Animation_ApplyChildArgumentFar(object, animation);
    object[9] &= -13;
  }
  menu->figures[index] = object;
}

void MenuIcon_SetPosition(s32 index, s32 x, s32 y) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;

  if (menu->figures[index] != NULL) {
    menu->figureX[index] = x;
    menu->figureY[index] = y;
  }
}

void MenuIcon_SetPicture(s32 index, s32 frame) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;

  if (menu->figures[index] != NULL)
    menu->figureDepths[index] = frame;
}

void DjinnFigures_SetSlotPhase(s32 index, s32 scale) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;

  if (menu->figures[index] != NULL)
    menu->figureScales[index] = scale;
}

void MenuIcon_PushSlots(void) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;
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
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;
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
      slot->attr.tile = gResourceTableEntries[entry].offset >> 5;
    }
    *resource++ = entry;
    slot++;
    offset += 2;
  }
  menu->menuIconsShown = ICON_SHOWN;
  Task_Add(MenuIcon_PushSlots, TASK_PASS_VBLANK + 10);
}

void MenuIcon_ReleaseSlots(void) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;
  u16 *resource;
  s32 i;

  Task_Remove(MenuIcon_PushSlots);
  resource = menu->menuIconSlots;
  for (i = 4; i >= 0; i--)
    VramCache_FreeEntry(*resource++);
}

void MenuIcon_SetSlot(s32 index, u32 x, s32 y) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;

  menu->menuIcons[index].attr.x = x;
  menu->menuIcons[index].attr.y = y;
}

void MenuIcon_SetActive(s32 active) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;

  menu->menuIconsShown = active;
}

struct MenuPanel {
  u8 unk00[12];
  s16 x;
  s16 y;
};

/// Lists usable entries with icons and names, or "none" when no entries
/// qualify.
void MenuPanel_DrawEntryList(struct MenuPanel *panel, s32 x, s32 y,
                             const u8 *enabled) {
  s32 count = 0;
  s32 icon_x = panel->x * 8 + x + 8;
  s32 icon_y = panel->y * 8 + y + 8;
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

s32 StatusScreen_BuildAvailability(u8 *output, s32 requested, s32 id) {
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
