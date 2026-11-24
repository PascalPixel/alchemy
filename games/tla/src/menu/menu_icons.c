/// @file
/// The stat arrows and the other icons the menus load into VRAM blocks, the
/// party and Djinn figures, the five status icons, and the statuses the
/// Status screens list.
// Part of menu.c.

#include "menu_internal.h"
#include "resource_ids.h"
#include "fixed_math.h"
#include "sounds.h"

/// Loads the Djinn menu's up and down arrows (djinn_arrow_up and
/// djinn_arrow_down) into free VRAM cache slots, the state's resourceSlots,
/// for Icon_CreateDjinnArrow.
void Menu_LoadDjinnArrows(void) {
  s32 firstSlot;
  s32 secondSlot;
  struct ItemMenuState *activeState;

  activeState = Ram_HeapSlots->menuState;
  firstSlot = VramCache_FindFreeEntry();
  activeState->resourceSlots[0] = (s16)firstSlot;
  if (firstSlot != -1) {
    VramBlock_LoadResourceFar(firstSlot, ICON_TILE_BYTES,
                              RESOURCE_ITEM_MENU_TILES_A);
  }
  secondSlot = VramCache_FindFreeEntry();
  activeState->resourceSlots[1] = (s16)secondSlot;
  if (secondSlot != -1) {
    VramBlock_LoadResourceFar(secondSlot, ICON_TILE_BYTES,
                              RESOURCE_ITEM_MENU_TILES_B);
  }
}

/// Frees the VRAM cache slots of the two arrows the menu loaded
/// (Menu_LoadStatArrows or Menu_LoadDjinnArrows).
void Menu_FreeArrows(void) {
  struct ItemMenuState *state = Ram_HeapSlots->menuState;

  VramCache_FreeEntry((u16)state->resourceSlots[0]);
  VramCache_FreeEntry((u16)state->resourceSlots[1]);
}

/// Decodes one icon into a temporary buffer and reloads its VRAM slot: the
/// Psynergy icon of ability resourceId (Ability_BuildIconTilesFar), built in a
/// IconTransfer in HEAP_SLOT_ICON_TRANSFER, which is released again.
s32 Icon_LoadResourceIntoSlot(s32 resourceId, s32 slot) {
  s32 resourceInfo;
  s32 selectedSlot;
  s32 buffer;
  IconTransfer *allocation;

  allocation =
      HeapSlot_AllocEwram(HEAP_SLOT_ICON_TRANSFER, sizeof(IconTransfer));
  selectedSlot = slot;
  Ability_BuildIconTilesFar(resourceId, 0, &selectedSlot, &resourceInfo, 1);
  buffer = VramCache_Reload(slot, (s32)allocation->tiles);
  HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
  return buffer;
}

/// Loads an icon into a free cache slot and creates its sprite when possible:
/// ability resourceId's Psynergy icon at (x, y) of window, for the Djinn
/// preview's Psynergy lists. Returns no value; callers ignore it.
s32 Icon_CreatePsynergyIcon(struct Window *window, s32 x, s32 y,
                            s32 resourceId) {
  s32 slot;

  slot = VramCache_FindFreeEntry();
  if (slot != VRAM_CACHE_ENTRY_COUNT) {
    Icon_LoadResourceIntoSlot(resourceId, slot);
    WindowSprite_CreateFar(slot, WINDOW_SPRITE_SIZE_16X16, window, x, y);
  }
}

/// Shows a stat change arrow, up or down (enum StatArrow), at (x, y) of
/// window. Returns 1, or -1 when no window sprite is free.
s32 Icon_CreateStatChangeArrow(struct Window *window, s32 x, s32 y,
                               s32 variant) {
  struct WindowSprite *object;
  u32 resource;
  struct ItemMenuState *data = Ram_HeapSlots->menuState;

  if (variant == STAT_ARROW_UP)
    resource = (u16)data->resourceSlots[0];
  else
    resource = (u16)data->resourceSlots[1];
  object =
      WindowSprite_CreateFar(resource, WINDOW_SPRITE_SIZE_16X16, window, x, y);
  if (object == 0) {
    /* FAKEMATCH(dead): a test whose two arms are alike. */
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

/// As Icon_CreateStatChangeArrow, with the arrow raised 3 pixels for variant
/// 0 and 4 for any other. Returns 1, or -1 when no window sprite is free.
s32 Icon_CreateDjinnArrow(struct Window *window, s32 x, s32 y, s32 variant) {
  struct WindowSprite *object;
  u32 resource;
  struct ItemMenuState *data = Ram_HeapSlots->menuState;

  if (variant == STAT_ARROW_UP) {
    resource = (u16)data->resourceSlots[0];
    y -= 3;
  } else {
    resource = (u16)data->resourceSlots[1];
    y -= 4;
  }
  object =
      WindowSprite_CreateFar(resource, WINDOW_SPRITE_SIZE_16X16, window, x, y);
  if (object == 0) {
    /* FAKEMATCH(dead): a test whose two arms are alike. */
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

/// Moves the selection *cursor among `count` entries in rows of `columns`:
/// left and right step and wrap, up and down move a row, the last row ending
/// at the last entry. Returns 0 when it stays, 1 when it moves within its row
/// and 2 when it reaches another row (another page of the party members).
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
void Menu_SetPageArrowsUpDown(s32 index, s32 count) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
  struct WindowSprite *arrow;

  if (index < 0)
    index += 3;
  index = index >> 2 << 2;
  arrow = menu->pageArrows[0];
  arrow->motion = WINDOW_SPRITE_MOTION_HIDDEN;
  if (index != 0) {
    arrow->motion = WINDOW_SPRITE_MOTION_HOP_UP;
    arrow->animationStep = 0;
  }
  arrow = menu->pageArrows[1];
  arrow->motion = WINDOW_SPRITE_MOTION_HIDDEN;
  if (index + 4 < count) {
    arrow->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE;
    arrow->animationStep = 0;
  }
}

/// As Menu_SetPageArrowsUpDown, with the arrows hopping left and right: the
/// Djinn menu's columns of party members.
void Menu_SetPageArrowsLeftRight(s32 index, s32 count) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
  struct WindowSprite *arrow;

  if (index < 0)
    index += 3;
  index = index >> 2 << 2;
  arrow = menu->pageArrows[0];
  arrow->motion = WINDOW_SPRITE_MOTION_HIDDEN;
  if (index != 0) {
    arrow->motion = WINDOW_SPRITE_MOTION_HOP_LEFT;
    arrow->animationStep = 0;
  }
  arrow = menu->pageArrows[1];
  arrow->motion = WINDOW_SPRITE_MOTION_HIDDEN;
  if (index + 4 < count) {
    arrow->motion = WINDOW_SPRITE_MOTION_HOP_RIGHT;
    arrow->animationStep = 0;
  }
}

/// Sets the page arrows for `index` and sets the chosen party member's figure
/// apart from the others on its page of four: its y MENU_MEMBER_CHOSEN_Y,
/// theirs MENU_MEMBER_Y.
void Menu_MarkChosenMember(s32 unused, s32 index, s32 count) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
  s32 top = index / 4 * 4;
  s32 i;

  Menu_SetPageArrowsUpDown(index, count);
  for (i = 0; i <= 3; i++) {
    if (top + i == index)
      menu->memberY[i] = MENU_MEMBER_CHOSEN_Y;
    else
      menu->memberY[i] = MENU_MEMBER_Y;
  }
}

/// A task while the party figures show: places each party member's
/// figure at its menu position and scale through the projected placement.
void Menu_UpdatePartyFigures(void) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
  s32 scale[2];
  FIXED transform[4];
  s32 i;

  if (menu->memberSpritesShown != 0) {
    for (i = 0; i <= 7; i++) {
      // 114.5 less half the figure's screen y, in 16.16.
      FIXED top = toFIXED(229) / 2 - menu->memberY[i] * (FIXED_ONE / 2);

      if (menu->memberSprites[i] != NULL) {
        scale[0] = menu->memberScale[i];
        scale[1] = menu->memberScale[i];
        transform[0] = toFIXED(menu->memberX[i]);
        transform[1] = top;
        transform[2] = toFIXED(menu->memberY[i]) + top;
        transform[3] = 0;
        Render_ApplyProjectedPlacementAtDepthFar(menu->memberSprites[i],
                                                 transform, scale, FACING_SOUTH,
                                                 MENU_FIGURE_DEPTH);
      }
    }
  }
}

/// Returns the sprite party member id's figure shows.
s32 Party_GetCharacterSpriteFar(s32 id);

/// Menu_CreatePartyFigures for two characters (-1 for none): their
/// figures in entries 0 and 1, as the Djinn preview shows the pair a Djinni
/// passes between.
void Menu_CreatePairFigures(s32 first, s32 second) {
  struct ItemMenuState *menu = (struct ItemMenuState *)Ram_HeapSlots->menuState;
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
        Sprite_StartAnimationFar(figure, ANIM_STAND);
        figure->image.priority = 0;
        figure->flags = 0;
      }
      menu->memberSprites[i] = figure;
    }
  }
}

/// Replaces the menu's party figures with up to four party members' figures,
/// from party place first on, standing (ANIM_STAND), and counts the party.
void Menu_CreatePartyFigures(s32 first) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
  u16 owners[14];
  struct Sprite *object;
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
  for (i = 0; i < 4 && owners[first + i] != BATTLE_UNIT_LIST_END; i++) {
    menu->memberSpriteIds[i] = owners[first + i];
    object = Sprite_CreateFar(Party_GetCharacterSpriteFar(owners[first + i]));
    if (object != NULL) {
      Sprite_StartAnimationFar(object, ANIM_STAND);
      object->image.priority = 0;
      object->flags = 0;
    }
    menu->memberSprites[i] = object;
  }
}

/// Creates the first four party members' figures in a row from tile (originX,
/// originY) inside window, three tiles apart, and starts placing them each
/// frame (Menu_UpdatePartyFigures).
void Menu_ShowPartyFigures(struct Window *window, s32 originX, s32 originY) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
  s32 i;

  for (i = 0; i < 8; i++)
    menu->memberSprites[i] = NULL;
  Menu_CreatePartyFigures(0);
  for (i = 0; i <= 3; i++) {
    if (menu->memberSprites[i] != NULL) {
      menu->memberX[i] = (i * 3 + ((s16)window->x + originX)) * 8;
      menu->memberY[i] = ((s16)window->y + originY) * 8 + 16;
      menu->memberScale[i] = FIXED_ONE;
    }
  }
  menu->memberSpritesShown = ICON_SHOWN;
  Task_Add(Menu_UpdatePartyFigures, TASK_PASS_FRAME);
}

/// Destroys the menu's party figures and stops placing them.
void Menu_ReleasePartyFigures(void) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
  void **objects = menu->memberSprites;
  s32 i;

  for (i = 0; i < 8; i++) {
    void *object = *objects++;

    if (object != NULL)
      Sprite_DestroyFar(object);
  }
  Task_Remove(Menu_UpdatePartyFigures);
}

/// Shows the Djinn figures of the top row (nonzero) or hides them (0).
void DjinnFigures_SetShown(s32 shown) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;

  menu->figuresShown = shown;
}

/// A task while the Djinn figures show: places each at its position,
/// depth and scale; a positive scale eases a quarter of the way to full size
/// each frame, a negative one holds at its size.
void DjinnFigures_UpdateTopRow(void) {
  struct ItemMenuState *menu = (struct ItemMenuState *)Ram_HeapSlots->menuState;
  struct SpriteScale scale;
  struct SpritePlacement position;
  s32 i;

  if (menu->figuresShown == 0)
    return;
  for (i = 0; i <= 3; i++) {
    struct Sprite *figure = menu->figures[i];

    if (figure != NULL) {
      // 114.5 less half the figure's screen y, in 16.16.
      FIXED placeY = toFIXED(229) / 2 - menu->memberY[i] * (FIXED_ONE / 2);
      s32 value = menu->figureScales[i];

      s32 next;

      if (value < 0) {
        next = -value;
        scale.x = next;
        scale.y = next;
      } else {
        next = value + (FIXED_ONE - value) / 4;
        scale.x = next;
        scale.y = next;
        menu->figureScales[i] = next;
      }
      position.x = toFIXED(menu->figureX[i]);
      position.height = placeY;
      position.y = toFIXED(menu->figureY[i]) + placeY;
      position.groundHeight = 0;
      Render_ApplyProjectedPlacementAtDepthFar(
          figure, &position, &scale, menu->figureFacings[i], MENU_FIGURE_DEPTH);
    }
  }
}

/// The four Djinn figures' sprites, one an element (menu_tables.c).
extern s32 gDjinnFigures_SpriteIds[];

/// The x a new row's figures stand at, parked below the screen
/// (MENU_FIGURE_HIDDEN_Y), until their screen places them.
#define FIGURE_START_X 16

/// Replaces the four Djinn figures with one Djinni of each element
/// (gDjinnFigures_SpriteIds) walking (ANIM_WALK), at FIGURE_START_X and
/// MENU_FIGURE_HIDDEN_Y until the screen moves them, and starts placing them
/// each frame (DjinnFigures_UpdateTopRow).
void DjinnFigures_InitializeTopRow(void) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
  struct Sprite *object;
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
      Sprite_StartAnimationFar(object, ANIM_WALK);
      object->image.priority = 0;
    }
    menu->figures[i] = object;
    menu->figureX[i] = FIGURE_START_X;
    menu->figureY[i] = MENU_FIGURE_HIDDEN_Y;
    menu->figureScales[i] = FIXED_ONE;
    menu->figureFacings[i] = FACING_SOUTH;
  }
  menu->figuresShown = 1;
  Task_Add(DjinnFigures_UpdateTopRow, TASK_PASS_FRAME);
}

/// Destroys the four Djinn figures and stops placing them.
void DjinnFigures_ReleaseTopRow(void) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
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
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
  struct Sprite *object;

  if (menu->figures[index] != NULL) {
    Sprite_DestroyFar(menu->figures[index]);
    menu->figures[index] = NULL;
  }
  object = Sprite_CreateFar(gDjinnFigures_SpriteIds[kind]);
  if (object != NULL) {
    Sprite_StartAnimationFar(object, animation);
    object->image.priority = 0;
  }
  menu->figures[index] = object;
}

/// Moves Djinn figure index to screen pixel (x, y), if it exists.
void DjinnFigures_SetSlotPosition(s32 index, s32 x, s32 y) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;

  if (menu->figures[index] != NULL) {
    menu->figureX[index] = x;
    menu->figureY[index] = y;
  }
}

/// Turns Djinn figure index to face facing (enum Facing), if it exists.
void DjinnFigures_SetSlotFacing(s32 index, s32 facing) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;

  if (menu->figures[index] != NULL)
    menu->figureFacings[index] = facing;
}

/// Sets Djinn figure index's scale, if it exists: positive grows towards full
/// size, negative holds at its size.
void DjinnFigures_SetSlotPhase(s32 index, s32 scale) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;

  if (menu->figures[index] != NULL)
    menu->figureScales[index] = scale;
}

/// A task: queues the five status icons for drawing while they show.
void MenuIcon_PushSlots(void) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
  struct IconSprite *slot;
  s32 i;

  if (menu->menuIconsShown != 0) {
    slot = menu->menuIcons;
    for (i = 0; i < CHARACTER_AILMENT_COUNT; i++) {
      Oam_AddSpriteAtDepth(slot, OAM_DEPTH_WINDOW);
      slot++;
    }
  }
}

extern u16 gStatusScreen_StatusIcons[5];

/// Loads the five status icons (gStatusScreen_StatusIcons), one for each
/// status MenuPanel_DrawEntryList lists, into free VRAM cache slots, and starts
/// drawing them (MenuIcon_PushSlots).
void MenuIcon_LoadSlots(void) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
  struct IconSprite *slot = menu->menuIcons;
  u16 *ids = gStatusScreen_StatusIcons;
  u16 *resource = menu->menuIconSlots;
  s32 entry;
  s32 i;
  s32 offset = 0;

  for (i = 0; i < CHARACTER_AILMENT_COUNT; i++) {
    entry = VramCache_FindFreeEntry();
    if (entry < VRAM_CACHE_ENTRY_COUNT) {
      u32 *attributes;

      /* FAKEMATCH(temporary): the table read at a byte offset of its own; as
       * ids[i]. */
      SelectionIcon_LoadIntoSlotFar(ICON_KIND_PATTERN,
                                    *(u16 *)(offset + (u32)ids), entry, 0);
      attributes = (u32 *)&slot->attr;
      *attributes++ = WINDOW_SPRITE_SIZE_16X16;
      *attributes = 0;
      slot->attr.tile = gVramCacheEntries[entry].offset >> 5;
    }
    *resource++ = entry;
    slot++;
    offset += 2;
  }
  menu->menuIconsShown = ICON_SHOWN;
  Task_Add(MenuIcon_PushSlots, TASK_PASS_FRAME + 10);
}

/// Stops drawing the five status icons and frees their VRAM cache slots.
void MenuIcon_ReleaseSlots(void) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
  u16 *resource;
  s32 i;

  Task_Remove(MenuIcon_PushSlots);
  resource = menu->menuIconSlots;
  for (i = 0; i < CHARACTER_AILMENT_COUNT; i++)
    VramCache_FreeEntry(*resource++);
}

/// Moves status icon index to screen position (x, y).
void MenuIcon_SetSlot(s32 index, u32 x, s32 y) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;

  menu->menuIcons[index].attr.x = x;
  menu->menuIcons[index].attr.y = y;
}

/// Shows the five status icons (1) or hides them (0).
void MenuIcon_SetActive(s32 active) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;

  menu->menuIconsShown = active;
}

/// Lists the statuses enabled marks into panel from (x, y), one a 16-pixel row
/// with its icon, or "Your status is normal." when there are none; the icons
/// of the others go off screen.
void MenuPanel_DrawEntryList(struct Window *panel, s32 x, s32 y,
                             const u8 *enabled) {
  s32 count = 0;
  s32 iconX = (s16)panel->x * 8 + x + 8;
  s32 iconY = (s16)panel->y * 8 + y + 8;
  s32 i;

  for (i = 0; i < CHARACTER_AILMENT_COUNT; i++) {
    if (enabled[i] != 0) {
      MenuIcon_SetSlot(i, iconX, iconY);
      Text_DrawMessageAsTilesFar((s32)MsgStatusNameDown + i, panel, x + 16, y);
      y += 16;
      iconY += 16;
      count++;
    } else {
      MenuIcon_SetSlot(i, -16, -16);
    }
  }
  if (count == 0)
    Text_DrawMessageAsTilesFar((s32)MsgStatusNameNormal, panel, x, y);
}

/// Fills ailments (enum UnitStatus) with party member memberId's statuses,
/// down only when requested is 1, and returns how many they have.
s32 StatusScreen_ListStatuses(u8 *ailments, s32 requested, s32 memberId) {
  struct Unit *unit;
  s32 ailmentIndex;
  s32 count;
  s32 poison;

  unit = Unit_Get(memberId);
  for (ailmentIndex = 0; ailmentIndex < CHARACTER_AILMENT_COUNT; ailmentIndex++)
    ailments[ailmentIndex] = 0;

  count = 0;
  if (unit->hp == 0 && requested == 1) {
    ailments[CHARACTER_DOWN] = requested;
    count = 1;
  }

  poison = unit->poison;
  if (poison != 0) {
    if (poison == 1) // poison; any other value is venom
      ailments[CHARACTER_POISON] = poison;
    else
      ailments[CHARACTER_VENOM] = 1;
    count++;
  }
  if (unit->curse != 0) {
    ailments[CHARACTER_CURSE] = 1;
    count++;
  }
  if (unit->haunt != 0) {
    ailments[CHARACTER_HAUNT] = 1;
    count++;
  }
  return count;
}
