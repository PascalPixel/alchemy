/// @file
/// The pieces the menu screens share: windows, the status panel, numbers,
/// icons, the party figures, the hand cursor, paged lists, palettes, the
/// cancel sound and text colours.
// Part of menu.c.

#include "menu_internal.h"
#include "resource_ids.h"
#include "sounds.h"

/// Window_UpdateOrCreate: keep an existing window as it is, without redrawing
/// its saved rectangle. The low byte of its flags is the new window's
/// (WINDOW_FLAG_*).
#define WINDOW_KEEP_EXISTING 0x100
#define WINDOW_STYLE_MASK 0xff

s32 GameFlag_Set(s32 flag);

/// Resumes the overlay tasks and tileset animations when a menu closes, and
/// clears FLAG_WEATHER_HELD and FLAG_PALETTE_BLEND_HELD: undoes
/// Menu_SuspendFieldTasks.
void Menu_ResumeFieldTasks(void) {
  Task_ResumeOverlayTasks();
  TilesetAnim_ResumeAllFar();
  GameFlag_Clear(FLAG_WEATHER_HELD);
  GameFlag_Clear(FLAG_PALETTE_BLEND_HELD);
}

/// Suspends the overlay tasks and tileset animations while a menu is open, and
/// sets FLAG_PALETTE_BLEND_HELD and FLAG_WEATHER_HELD, so that the field's
/// palette blend and weather stand still behind it (see Menu_ResumeFieldTasks,
/// which undoes it).
s32 Menu_SuspendFieldTasks(void) {
  GameFlag_Set(FLAG_PALETTE_BLEND_HELD);
  GameFlag_Set(FLAG_WEATHER_HELD);
  TilesetAnim_SuspendAllFar();
  return Task_SuspendOverlayTasks();
}

/// Clears the whole menu state and sets its defaults: no side selected, one
/// entry in each side, and a 1x1 cursor grid.
void Menu_InitializeState(s32 unused) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;

  Dma_Fill32(0, menu, MENU_STATE_SIZE);
  menu->sideIndex[0] = -1;
  menu->sideCount[0] = 1;
  menu->sideCount[1] = 1;
  menu->columnCount = 1;
  menu->rowCount = 1;
}

void Window_CloseFar();
struct Window *Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
/// Redraws a window empty, its border and fill, at its current rectangle.
void WindowSprite_RedrawSavedRectFar(struct Window *window);

/// Creates *handle's window if absent and returns 1. Otherwise returns 0,
/// redrawing its saved rectangle unless flags has WINDOW_KEEP_EXISTING set.
s32 Window_UpdateOrCreate(s32 *handle, s32 x, s32 y, s32 w, s32 h, s32 flags) {
  struct Window *window;
  s32 style;

  window = *handle;
  if (window != 0) {
    if (WINDOW_KEEP_EXISTING & flags) {
      return 0;
    }
    WindowSprite_RedrawSavedRectFar(window);
    return 0;
  }
  style = flags & WINDOW_STYLE_MASK;
  *handle = Window_CreateFar(x, y, w, h, style);
  return 1;
}

/// Closes *handle's window if present, erasing it at once when release is set,
/// then sets the handle to NULL.
void Window_CloseIfOpen(void **handle, s32 release) {
  if (*handle != NULL) {
    Window_CloseFar(*handle, release);
    *handle = NULL;
  }
}

/// Japanese draws the name and the ailment labels 8 pixels further right, the
/// Lv label through Text_DrawStringInWindowFar, and the equip panel's "cannot
/// equip" line 16 pixels in, as the class panel's is. Spanish swaps the learn
/// panel's two lines: the note above the Psynergy's name.
#if LANGUAGE == LANGUAGE_JAPANESE
#define STATUS_PANEL_NAME_X 40
#define STATUS_PANEL_CANNOT_EQUIP_X 16
#else
#define STATUS_PANEL_NAME_X 32
#define STATUS_PANEL_CANNOT_EQUIP_X 0
#endif
#if LANGUAGE == LANGUAGE_SPANISH
#define STATUS_PANEL_LEARN_NAME_Y 56
#define STATUS_PANEL_LEARN_NOTE_Y 48
#else
#define STATUS_PANEL_LEARN_NAME_Y 48
#define STATUS_PANEL_LEARN_NOTE_Y 56
#endif

/// The tile of BG character block 0 that holds the class change's down arrow,
/// which Menu_WatchStart blinks.
#define CLASS_CHANGE_ARROW_TILE 298

/// A status panel request: the panel's kind in the low byte, and a bit to draw
/// only its lower part.
#define STATUS_PANEL_LOWER_ONLY 0x100
#define STATUS_PANEL_KIND_MASK 0xff

/// What the status panel shows in its lower part (STATUS_PANEL_KIND_MASK's
/// bits): the experience, the equipment, the Psynergy learned, the class, the
/// stats, or the equipment with the class it would give.
enum StatusPanelKind {
  STATUS_PANEL_EXPERIENCE = 0,
  STATUS_PANEL_EQUIP = 2,
  STATUS_PANEL_LEARN = 4,
  STATUS_PANEL_CLASS = 6,
  STATUS_PANEL_STATS = 8,
  STATUS_PANEL_EQUIP_CLASS = 9
};

// The status panel's messages, as its routines pass them.
#define MESSAGE_CLASS_NAME ((s32)&MsgClassName)
#define MESSAGE_EXP_LABEL ((s32)&MsgExpLabel)
#define MESSAGE_PANEL_STAT_LABEL ((s32)&MsgPanelAttackLabel)
#define MESSAGE_ABILITY_NAME ((s32)&MsgAbilityName)
#define MESSAGE_POISON_LABEL ((s32)&MsgPoisonLabel)
#define MESSAGE_VENOM_LABEL ((s32)&MsgVenomLabel)
#define MESSAGE_CURSE_LABEL ((s32)&MsgCurseLabel)
#define MESSAGE_HAUNT_LABEL ((s32)&MsgHauntLabel)
#define MESSAGE_CANNOT_EQUIP ((s32)&MsgCannotEquip)
#define MESSAGE_WILL_LEARN ((s32)&MsgWillLearn)
#define MESSAGE_LEARNED ((s32)&MsgLearned)

/// The status panel's level label: `Lv`; `Nv.` in Spanish, `Niv` in French.
extern const u8 gStatusPanel_LvString[];

/// Clears an area of a window's interior to the window fill: left, top, right
/// and bottom are pixels inside it, and the area covers every tile they touch.
void Window_ClearInteriorTilesFar(struct Window *window, s32 left, s32 top,
                                  s32 right, s32 bottom);
/// Releases every window sprite in a window's list and empties it.
void WindowSprite_ClearListFar(struct Window *window);
/// Creates party member owner's portrait in window at pixel (x, y), with side's
/// palette.
void *Portrait_CreateFar(s32 owner, s32 position, s32 side,
                         struct Window *window, s32 x, s32 y);
void Unit_UpdateClassFar(s32 owner);

/// Draws the status panel of the Item and Psynergy screens for party member
/// owner (window at tile 0,5, 13 by 12): the name, up to two
/// statuses or the level, the party member's portrait, and below them what
/// mode's panel kind asks for. slot is the inventory slot of the item the panel
/// is about, for the equipment and Psynergy item panels. The first parameter,
/// the status window, is not used: the window is the menu state's.
///
/// Opens the window on first use and keeps it in the state's statusWindow;
/// unless mode has STATUS_PANEL_LOWER_ONLY, the portrait it creates replaces
/// the state's cursor. The equipment panel equips the item on the real unit for
/// a moment and puts the unit back from a heap copy it frees before returning.
void StatusPanel_Draw(struct Window *unused, s32 owner, s32 slot, s32 mode) {
  s32 created;
  struct ItemDefinition *def;
  u32 item;
  struct ItemMenuState *state;
  struct Unit *unit;
  struct Window *window;
  s32 ailmentRows;
  s32 value;
  struct Unit *backup;
  u8 ailments[8];

  created = 0;
  state = (struct ItemMenuState *)Ram_HeapSlots->menuState;
  unit = Unit_Get(owner);
  item = unit->inventory[slot];
  def = Item_GetDirectFar(item & INVENTORY_ITEM_ID_MASK);
  if (!(mode & STATUS_PANEL_LOWER_ONLY))
    created = Window_UpdateOrCreate((s32 *)&state->statusWindow, 0, 5, 13, 12,
                                    WINDOW_KEEP_EXISTING | WINDOW_FLAG_INSTANT);
  window = state->statusWindow;

  if (!(mode & STATUS_PANEL_LOWER_ONLY)) {
    if (!created) {
      wait(1);
      Window_ClearInteriorTilesFar(state->statusWindow, 0, 0, 88, 32);
    }
    Text_DrawStringAtOffsetFar(unit->name, (s32)window, STATUS_PANEL_NAME_X, 0);
    StatusScreen_ListStatuses(ailments, 1, owner);
    ailmentRows = 0;
    if (ailments[CHARACTER_POISON]) {
      Text_DrawMessageAsTilesFar(MESSAGE_POISON_LABEL, window,
                                 STATUS_PANEL_NAME_X, ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[CHARACTER_VENOM]) {
      Text_DrawMessageAsTilesFar(MESSAGE_VENOM_LABEL, window,
                                 STATUS_PANEL_NAME_X, ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[CHARACTER_CURSE]) {
      Text_DrawMessageAsTilesFar(MESSAGE_CURSE_LABEL, window,
                                 STATUS_PANEL_NAME_X, ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[CHARACTER_HAUNT]) {
      Text_DrawMessageAsTilesFar(MESSAGE_HAUNT_LABEL, window,
                                 STATUS_PANEL_NAME_X, ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailmentRows < 2) {
      value = unit->level;
      MENU_DRAW_STATUS_LABEL((u8 *)gStatusPanel_LvString, (s32)window, 40, 16);
      Text_DrawNumberInWindowFar(value, 4, (s32)window, 56, 16);
    }
  }

  if (!created) {
    wait(1);
    Window_ClearInteriorTilesFar(state->statusWindow, 0, 32, 88, 80);
  }
  WindowSprite_ClearListFar(window);
  if (!(mode & STATUS_PANEL_LOWER_ONLY))
    state->cursor = Portrait_CreateFar(owner, 0, 0, window, 0, 0);

  switch (mode & STATUS_PANEL_KIND_MASK) {
  case STATUS_PANEL_EQUIP_CLASS:
    // Only removing an equipped item or equipping another can change the
    // class.
    if (item & INVENTORY_EQUIPPED) {
      if (state->equipPreview)
        goto classChange;
      goto experience;
    }
    if (state->equipPreview)
      goto experience;
  classChange:
    value = unit->classId + MESSAGE_CLASS_NAME;
    Text_DrawMessageAsTilesFar(value, window, 0, 32);
    if (!Item_CanUnitEquip(owner, item)) {
      Text_DrawMessageAsTilesFar(MESSAGE_CANNOT_EQUIP, window, 16, 48);
      break;
    }
    backup = Heap_Alloc(sizeof(struct Unit));
    Iwram_CopyWordsEntry(backup, unit, sizeof(struct Unit));
    if (state->equipPreview) {
      unit->inventory[slot] &= ~INVENTORY_EQUIPPED;
      Unit_UpdateClassFar(owner);
    } else {
      Inventory_EquipFar(owner, slot);
    }
    Unit_CalculateFar(owner);
    // The blinking arrow down from the class now to the class the equipment
    // would give.
    Window_SetTilemapEntryFar((s32)window, WINDOW_TILE(CLASS_CHANGE_ARROW_TILE),
                              3, 5, 0);
    value = unit->classId + MESSAGE_CLASS_NAME;
    Text_DrawMessageAsTilesFar(value, window, 0, 48);
    Iwram_CopyWords(unit, backup, sizeof(struct Unit));
    Heap_Release(backup);
    break;
  case STATUS_PANEL_EXPERIENCE:
  experience:
    value = unit->classId + MESSAGE_CLASS_NAME;
    Text_DrawMessageAsTilesFar(value, window, 0, 32);
    StatusPanel_DrawHpPp(unit, (s32)window);
    value = unit->experience;
    Text_DrawMessageAsTilesFar(MESSAGE_EXP_LABEL, window, 0, 64);
    Text_DrawNumberInWindowFar(value, 8, (s32)window, 24, 72);
    break;
  case STATUS_PANEL_CLASS:
    value = unit->classId + MESSAGE_CLASS_NAME;
    Text_DrawMessageAsTilesFar(value, window, 0, 32);
    StatusPanel_DrawHpPp(unit, (s32)window);
    break;
  case STATUS_PANEL_EQUIP:
  case STATUS_PANEL_EQUIP + 1:
    if (!Item_CanUnitEquip(owner, item)) {
      Text_DrawMessageAsTilesFar(MESSAGE_CANNOT_EQUIP, window,
                                 STATUS_PANEL_CANNOT_EQUIP_X, 48);
      break;
    }
    backup = Heap_Alloc(sizeof(struct Unit));
    Iwram_CopyWordsEntry(backup, unit, sizeof(struct Unit));
    if (state->equipPreview)
      unit->inventory[slot] &= ~INVENTORY_EQUIPPED;
    else
      Inventory_EquipFar(owner, slot);
    Unit_CalculateFar(owner);
    Text_DrawStatComparison(unit, backup, (s32)window);
    Iwram_CopyWords(unit, backup, sizeof(struct Unit));
    Heap_Release(backup);
    break;
  case STATUS_PANEL_LEARN: {
    s32 psynergy = def->abilityId;
    s32 known = 0;
    s32 i;

    for (i = 0; i < PSYNERGY_SLOTS; i++) {
      if ((unit->psynergy[i].encodedAction & PSYNERGY_ABILITY_MASK) ==
          psynergy) {
        known = 1;
        break;
      }
    }
    if (known) {
      Text_DrawMessageAsTilesFar(psynergy + MESSAGE_ABILITY_NAME, window, 0,
                                 STATUS_PANEL_LEARN_NAME_Y);
      Text_DrawMessageAsTilesFar(MESSAGE_LEARNED, window, 0,
                                 STATUS_PANEL_LEARN_NOTE_Y);
    } else {
      Text_DrawMessageAsTilesFar(psynergy + MESSAGE_ABILITY_NAME, window, 0,
                                 STATUS_PANEL_LEARN_NAME_Y);
      Text_DrawMessageAsTilesFar(MESSAGE_WILL_LEARN, window, 0,
                                 STATUS_PANEL_LEARN_NOTE_Y);
    }
    break;
  }
  case STATUS_PANEL_STATS: {
    s32 labels = MESSAGE_PANEL_STAT_LABEL;

    Text_DrawMessageAsTilesFar(labels, window, 0, 40); // Attack
    value = unit->attack;
    Text_DrawNumberInWindowFar(value, 3, (s32)window, 64, 40);
    Text_DrawMessageAsTilesFar(labels + 1, window, 0, 48); // Defense
    value = unit->defense;
    Text_DrawNumberInWindowFar(value, 3, (s32)window, 64, 48);
    Text_DrawMessageAsTilesFar(labels + 4, window, 0, 56); // Agility
    value = unit->agility;
    Text_DrawNumberInWindowFar(value, 3, (s32)window, 64, 56);
    Text_DrawMessageAsTilesFar(labels + 3, window, 0, 64); // Luck
    value = unit->luck;
    Text_DrawNumberInWindowFar(value, 3, (s32)window, 64, 64);
    break;
  }
  }
}

#if (LANGUAGE != LANGUAGE_JAPANESE)
/// The pixels the field allows each digit. The Latin font's digits are 7
/// pixels wide, so the digits themselves take a pixel less each.
#define NUMBER_PIXELS_PER_DIGIT 8
/// Where CountDigitsAtLeastTwo would stop counting: past an s32's 10 digits,
/// so it only bounds the loop.
#define DIGIT_COUNT_LIMIT 16

/// Returns how many decimal digits number has, but at least 2: a number under
/// 10 has its one digit counted twice, as the units and as the leading digit.
static inline s32 CountDigitsAtLeastTwo(s32 number) {
  s32 count;

  for (count = 1; count < DIGIT_COUNT_LIMIT; count++) {
    number /= 10;
    if (number < 10) {
      break;
    }
  }
  count++; // the leading digit
  return count;
}

/// Draws number right-aligned in a field ending at pixel x of window, 8 pixels
/// a digit and at least two digits wide: the HP and PP figures outside Japan.
/// Japanese pads them with spaces, its font's space as wide as a digit; the
/// Latin font's space is narrower, so each figure gets a field as wide as its
/// digits instead.
void Text_DrawNumberRightAligned(s32 number, s32 window, s32 x, s32 y) {
  s32 width = CountDigitsAtLeastTwo(number);

  // Step x back to the field's left edge; the number writer right-aligns the
  // number in a field of width characters.
  x -= width * NUMBER_PIXELS_PER_DIGIT;
  Text_DrawNumberAtOffsetFar(number, width, window, x, y);
}

#endif

/// Draws a party member's HP and PP into window as two rows, "HP
/// current/maximum" at pixel row 40 and "PP current/maximum" at row 48. The
/// current HP is yellow below a quarter of the maximum, and red at 0.
void StatusPanel_DrawHpPp(struct Unit *unit, s32 window) {
  s16 value; // each figure in turn

#if (LANGUAGE != LANGUAGE_JAPANESE)
  // The label at the left and a slash at 48. A current figure ends before
  // the slash, at 48, and a maximum at 88, each in a field as wide as its
  // digits.
  Text_DrawStringAtOffsetFar(gStatusPanel_HpString, window, 0, 40);
  Text_DrawStringInWindowFar((u8 *)gStatusPanel_SlashString, window, 48, 40);
  value = unit->hpMax;
  Text_DrawNumberRightAligned(value, window, 88, 40);
  value = unit->hp;
  if (value < unit->hpMax >> 2) { // below a quarter of maximum HP
    Text_SetColorFar(TEXT_COLOR_YELLOW);
  }
  if (value == 0) {
    Text_SetColorFar(TEXT_COLOR_RED);
  }
  Text_DrawNumberRightAligned(value, window, 48, 40);
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  Text_DrawStringAtOffsetFar(gStatusPanel_PpString, window, 0, 48);
  Text_DrawStringInWindowFar((u8 *)gStatusPanel_SlashString, window, 48, 48);
  value = unit->pp;
  Text_DrawNumberRightAligned(value, window, 48, 48);
  value = unit->ppMax;
  Text_DrawNumberRightAligned(value, window, 88, 48);
#else
  // Each row string ("HP    /" and "EP    /") holds the label and the slash,
  // one character to an 8-pixel cell, the slash in the cell at 48. Each
  // figure is four characters padded with spaces: the current one drawn
  // from 16 at the font's widths, the maximum from 56 a cell a character,
  // ending at 88.
  Text_DrawStringInWindowFar((u8 *)gStatusPanel_HpRowString, window, 0, 40);
  value = unit->hpMax;
  Text_DrawNumberInWindowFar(value, 4, window, 56, 40);
  value = unit->hp;
  if (value < unit->hpMax >> 2) { // below a quarter of maximum HP
    Text_SetColorFar(TEXT_COLOR_YELLOW);
  }
  if (value == 0) {
    Text_SetColorFar(TEXT_COLOR_RED);
  }
  Text_DrawNumberAtOffsetFar(value, 4, window, 16, 40);
  Text_SetColorFar(TEXT_COLOR_NORMAL);
  Text_DrawStringInWindowFar((u8 *)gStatusPanel_PpRowString, window, 0, 48);
  value = unit->pp;
  Text_DrawNumberAtOffsetFar(value, 4, window, 16, 48);
  value = unit->ppMax;
  Text_DrawNumberInWindowFar(value, 4, window, 56, 48);
#endif
}

s32 Icon_CreateStatChangeArrow(struct Window *window, s32 x, s32 y,
                               s32 variant);

/// The stat arrows: the small up and down arrows the menu loads into its two
/// VRAM cache slots (Menu_LoadStatArrows).
enum StatArrow { STAT_ARROW_UP, STAT_ARROW_DOWN };

/// Writes Attack, Defense and Agility into a status window as they are now
/// (before), and beside each one that changes its new value (after) with an up
/// or down arrow: the equipment preview.
void Text_DrawStatComparison(const struct Unit *after,
                             const struct Unit *before, s32 window) {
  Text_DrawMessageAsTilesFar((s32)MsgPanelAttackLabel, (struct Window *)window,
                             0, 32);
  Text_DrawNumberAtOffsetFar(before->attack, 3, window, 16, 40);
  if (after->attack != before->attack) {
    Text_DrawNumberAtOffsetFar(after->attack, 3, window, 64, 40);
    if (after->attack > before->attack) {
      Icon_CreateStatChangeArrow(window, 44, 36, STAT_ARROW_UP);
    } else {
      Icon_CreateStatChangeArrow(window, 44, 36, STAT_ARROW_DOWN);
    }
  }
  Text_DrawMessageAsTilesFar((s32)MsgPanelDefenseLabel, (struct Window *)window,
                             0, 48);
  Text_DrawNumberAtOffsetFar(before->defense, 3, window, 16, 56);
  if (after->defense != before->defense) {
    Text_DrawNumberAtOffsetFar(after->defense, 3, window, 64, 56);
    if (after->defense > before->defense) {
      Icon_CreateStatChangeArrow(window, 44, 52, STAT_ARROW_UP);
    } else {
      Icon_CreateStatChangeArrow(window, 44, 52, STAT_ARROW_DOWN);
    }
  }
  Text_DrawMessageAsTilesFar((s32)MsgAgilityLabel, (struct Window *)window, 0,
                             64);
  Text_DrawNumberAtOffsetFar(before->agility, 3, window, 16, 72);
  if (after->agility != before->agility) {
    Text_DrawNumberAtOffsetFar(after->agility, 3, window, 64, 72);
    if (after->agility > before->agility) {
      Icon_CreateStatChangeArrow(window, 44, 68, STAT_ARROW_UP);
    } else {
      Icon_CreateStatChangeArrow(window, 44, 68, STAT_ARROW_DOWN);
    }
  }
}

/// Creates a hand cursor sprite in window at (x, y), loading the hand's tiles
/// into a free VRAM block.
s32 Icon_CreateHandCursor(struct Window *window, s32 x, s32 y) {
  s32 slot;
  s32 icon;

  icon = 0;
  slot = VramCache_FindFreeEntry();
  if (slot != 0) {
    VramBlock_LoadResourceFar(slot, ICON_TILE_BYTES, RESOURCE_UI_ICON_TILES);
    icon = (s32)WindowSprite_CreateFar(slot, WINDOW_SPRITE_SIZE_16X16, window,
                                       x, y);
  }
  return icon;
}

/// Shows an icon at its place, unscaled.
void Icon_Show(struct WindowSprite *icon) {
  if (icon != NULL) {
    icon->motion = ICON_SHOWN;
    icon->sprite.oam.x = icon->x;
    icon->sprite.oam.y = icon->y;
    icon->sprite.oam.matrix = 0;
    icon->sprite.oam.affineMode = 0;
  }
}

/// Loads the graphics of party member unit into menu entry slot 0. The calling
/// menu argument is not used.
void Menu_ReloadMemberPortrait(s32 menu, s32 unit) {
  Portrait_ReloadFar(0, unit, 0);
}

/// Opens the menu's top-left window (13 by 5 tiles) with a divider, puts the
/// hand cursor in it, hidden, and adds the up and down arrow icons at its right
/// edge when a VRAM entry is free. Returns the window.
struct Window *MenuCursor_Create(void *source) {
  struct PsynergyMenuState *menu = source;
  struct Window *window;
  struct WindowSprite *icon;
  s32 slot;

  menu->auxiliaryWindow = NULL;
  Window_UpdateOrCreate((s32 *)&menu->auxiliaryWindow, 0, 0, 13, 5,
                        WINDOW_FLAG_INSTANT);
  window = menu->auxiliaryWindow;
  Window_DrawDividerLineFar((s32)window, 0, 3, 12, 3);
  icon = (struct WindowSprite *)Icon_CreateHandCursor(window, -8, 11);
  icon->motion = ICON_HIDDEN;
  menu->tabIndex[0] = -1;
  menu->sideIcon[0] = icon;
  icon->depth = OAM_DEPTH_FRONT - 1;
  menu->tabIndex[1] = 0;
  menu->sideIcon[1]->depth = OAM_DEPTH_FRONT;
  slot = VramCache_FindFreeEntry();
  if (slot < VRAM_CACHE_ENTRY_COUNT) {
    VramBlock_LoadResourceFar(slot, ICON_TILE_BYTES,
                              RESOURCE_SHOP_UP_ARROW_TILES);
    icon =
        WindowSprite_CreateFar(slot, WINDOW_SPRITE_SIZE_16X16, window, 84, -4);
    icon->motion = ICON_HIDDEN;
    menu->pageArrows[0] = icon;
  }
  slot = VramCache_FindFreeEntry();
  if (slot < VRAM_CACHE_ENTRY_COUNT) {
    VramBlock_LoadResourceFar(slot, ICON_TILE_BYTES,
                              RESOURCE_SHOP_DOWN_ARROW_TILES);
    icon =
        WindowSprite_CreateFar(slot, WINDOW_SPRITE_SIZE_16X16, window, 84, 8);
    icon->motion = ICON_HIDDEN;
    menu->pageArrows[1] = icon;
  }
  return window;
}

/// Task: each frame, places every party member sprite created by
/// Menu_CreatePartyFigures at its menu position and scale, through the field's
/// projected placement.
void Menu_PlacePartyFigures(void) {
  struct PsynergyMenuState *menu =
      (struct PsynergyMenuState *)Ram_HeapSlots->menuState;
  struct SpriteScale scale;
  struct SpritePlacement position;
  s32 i;
  s32 count;

  count = (u16)Party_CountMembersFar();
  for (i = 0; i < count; i++) {
    struct Sprite *figure = menu->tabSprites[i];
    // 114.5 less half the row's screen y, in 16.16.
    FIXED placeY = toFIXED(229) / 2 - menu->rowPositions[i] * (FIXED_ONE / 2);

    if (figure != 0) {
      figure->image.priority = 0;
      scale.x = menu->memberScale[i];
      scale.y = menu->memberScale[i];
      position.x = toFIXED(menu->tabX[i]);
      position.height = placeY;
      position.y = toFIXED(menu->rowPositions[i]) + placeY;
      position.groundHeight = 0;
      Render_ApplyProjectedPlacementAtDepthFar(figure, &position, &scale,
                                               FACING_SOUTH, MENU_FIGURE_DEPTH);
    }
  }
}

/// The hand cursor's bob in x, a step every two frames (MenuCursor_Position).
extern u8 gMenuCursor_BobX[];
/// The hand cursor's bob in y: 0, -1, -1, -2, -2, -1, -1, 0.
extern u8 gMenuCursor_BobY[];

/// The hand cursor's step through gMenuCursor_BobY, one every two frames.
#define CURSOR_BOB_STEP() ((gFrameCount >> 1) & 7)

/// Places the hand cursor at (xOffset, yOffset) inside the menu's window,
/// bobbing.
void MenuCursor_Position(s32 xOffset, s32 yOffset) {
  struct PsynergyMenuState *menu =
      (struct PsynergyMenuState *)Ram_HeapSlots->menuState;
  struct WindowSprite *cursor = menu->sideIcon[0];

  cursor->sprite.oam.x = cursor->x = gMenuCursor_BobX[CURSOR_BOB_STEP()] +
                                     xOffset +
                                     (u16)menu->auxiliaryWindow->x * 8 + 8;
  cursor->sprite.oam.y = cursor->y = gMenuCursor_BobY[CURSOR_BOB_STEP()] +
                                     yOffset +
                                     (u16)menu->auxiliaryWindow->y * 8 + 8;
}

/// Slides the hand cursor from where it is to (x, y) inside the menu's top-left
/// window over two frames, in 1/16 pixel steps.
void MenuCursor_Slide(s32 x, s32 y) {
  struct PsynergyMenuState *menu =
      (struct PsynergyMenuState *)Ram_HeapSlots->menuState;
  struct WindowSprite *cursor;
  s32 steps;
  s32 startX;
  s32 startY;
  s32 px;
  s32 py;
  s32 dx;
  s32 dy;

  steps = 2;
  if (menu->skipSlide != 0) {
    menu->skipSlide = 0;
    return;
  }
  cursor = menu->sideIcon[0];
  {
    s32 spriteX = cursor->sprite.oam.x;
    s32 spriteY = cursor->sprite.oam.y;

    cursor->x = spriteX + 64;
    cursor->y = spriteY + 64;
  }
  x += 64;
  y += 64;
  if (cursor->x - 8 > 0)
    cursor->x -= 8;
  py = cursor->y;
  if (py - 8 > 0) {
    cursor->y -= 8;
    py = cursor->y;
  }
  startX = cursor->x << 4;
  dx = ((x << 4) - startX + 1) / steps;
  startY = py << 4;
  dy = ((y << 4) - startY + 1) / steps;
  px = startX;
  py = startY;
  do {
    px += dx;
    {
      s32 spriteX = (px >> 4) + ((u16)menu->auxiliaryWindow->x << 3);

      cursor->sprite.oam.x = cursor->x = spriteX - 56;
    }
    py += dy;
    cursor->sprite.oam.y = cursor->y =
        (py >> 4) + ((u16)menu->auxiliaryWindow->y << 3) - 56;
    steps--;
    if (steps != 0)
      wait(1);
  } while (steps != 0);
}

void PsynergyMenu_PlaceEntryIcons(s32 x, s32 y, s32 width);

/// Unused: lays the entry icons out eight to a row from (108, 40).
UNUSED void PsynergyMenu_PlaceEntryIconsDefault(void) {
  PsynergyMenu_PlaceEntryIcons(108, 40, 8);
}

/// Lays the menu's entry icons (up to 32) out in a grid of 16x16 cells,
/// columns wide, from (originX, originY); empty slots keep their place.
void PsynergyMenu_PlaceEntryIcons(s32 originX, s32 originY, s32 columns) {
  struct WindowSprite **slot;
  struct WindowSprite **scan;
  s32 index;

  index = 0;
  slot = ((struct PsynergyMenuState *)Ram_HeapSlots->menuState)->entryIcons;
  scan = slot;
  do {
    if (*scan++ != NULL) {
      PsynergyMenu_PlaceEntryIcon(slot, index, originX, originY, columns);
    }
    index++;
    slot++;
  } while (index < MENU_ENTRY_ICONS);
}

/// Places the icon in *slot at cell index of a grid of 16x16 cells, columns
/// wide, from (originX, originY), and shows it; an index past the last entry
/// is taken as 0.
void PsynergyMenu_PlaceEntryIcon(struct WindowSprite **slot, s32 index,
                                 s32 originX, s32 originY, s32 columns) {
  struct WindowSprite *icon;

  if (index > MENU_ENTRY_ICONS - 1) {
    index = 0;
  }
  icon = *slot;
  icon->y = (index / columns) * 16 + originY;
  icon->x = (index % columns) * 16 + originX;
  Icon_Show(icon);
}

/// Places the icon in *slot at slotIndex in a columns-wide grid of 24x16 cells
/// from (originX, originY), and shows it; a slotIndex past INVENTORY_SLOTS is
/// taken as 0.
void ItemMenu_PlaceIcon(struct WindowSprite **slot, s32 slotIndex, s32 originX,
                        s32 originY, s32 columns) {
  struct WindowSprite *icon;

  if (slotIndex > INVENTORY_SLOTS)
    slotIndex = 0;
  icon = *slot;
  icon->y = slotIndex / columns * 16 + originY;
  icon->x = slotIndex % columns * 24 + originX;
  Icon_Show(icon);
}

/// Lays the inventory's INVENTORY_SLOTS item icons out five to a row from (116,
/// y): y is 56 for layout 1 and 40 otherwise.
void ItemMenu_PlaceIcons(s32 layout) {
  struct WindowSprite **slot;
  struct WindowSprite **scan;
  struct WindowSprite *icon;
  s32 index;
  s32 originY;
  struct ItemMenuState *menu;

  menu = Ram_HeapSlots->menuState;
  originY = 56;
  if (layout != 1) {
    originY = 40;
  }
  slot = menu->entryIcons;
  index = 0;
  scan = slot;
  do {
    icon = *scan++;
    if (icon != NULL) {
      ItemMenu_PlaceIcon(slot, index, 116, originY, MENU_ITEM_GRID_COLUMNS);
    }
    index++;
    slot++;
  } while (index < INVENTORY_SLOTS);
}

void Text_GetMessageSizeFar(s32 message, s32 *left, s32 *top, s32 *width,
                            s32 *height);

/// Shows message in the item menu, with the hand cursor hidden meanwhile.
///
/// With y == -1 the message goes into the info window; otherwise a modal
/// window sized to the message opens at (x, y). With x == -1 the message stays
/// up and FLAG_INFO_WINDOW_HOLDS_MESSAGE records that the info window holds it;
/// otherwise it waits for A, B or Start and clears it again.
void ItemMenu_ShowMessage(s32 message, s32 x, s32 y) {
  struct ItemMenuState *menu = (struct ItemMenuState *)Ram_HeapSlots->menuState;
  struct Window *window;
  s32 height;
  s32 width;
  s32 top;
  s32 left;

  menu->sideIcons[0]->motion = ICON_HIDDEN;
  if (y != -1) {
    Text_GetMessageSizeFar(message, &left, &top, &width, &height);
    if (Window_UpdateOrCreate((s32 *)&menu->modalWindow, x, y, width, height,
                              WINDOW_KEEP_EXISTING | WINDOW_FLAG_INSTANT) == 0)
      Window_SetBounds(menu->modalWindow, x, y, width, height);
    window = menu->modalWindow;
  } else {
    window = menu->infoWindow;
  }
  WindowSprite_RedrawSavedRectFar((struct Window *)window);
  WindowSprite_ClearListFar(window);
#if LANGUAGE == LANGUAGE_JAPANESE
  Text_DrawMessageFar(message, (s32)window, 0, 0);
#else
  if (y == -1)
    Text_DrawMessageAsTilesFar(message, window, 0, 0);
  else
    Text_DrawMessageFar(message, (s32)window, 0, 0);
#endif
  if (x != -1) {
    wait(1);
    do {
      wait(1);
    } while (!(gInput.pressed & KEY_A) && !(gInput.pressed & KEY_B) &&
             !(gInput.pressed & KEY_START));
    WindowSprite_ClearListFar(window);
  } else {
    GameFlag_Set(FLAG_INFO_WINDOW_HOLDS_MESSAGE);
  }
  menu->completionFlag = 1;
  menu->sideIcons[0]->motion = ICON_SHOWN;
#if TLA_PATCH_1
  if (y != -1)
    Window_CloseIfOpen((void **)&menu->modalWindow, 1);
#endif
}

/// The most bytes a list order copies, its LIST_ORDER_END included.
#define LIST_ORDER_MAX_LENGTH 32

void Menu_CopyListOrder(s32 mode, u8 *order);

/// Sorts a party member's INVENTORY_SLOTS inventory slots by the list order of
/// mode: items are grouped by the order's item types and, within a type, run
/// from the highest item id down.
s32 ItemMenu_Sort(u16 *inventory, s32 mode) {
  u16 sorted[16];
  u16 items[16];
  u8 order[LIST_ORDER_MAX_LENGTH];
  struct ItemDefinition *item;
  s32 pass;
  s32 i;
  s32 count;
  s32 best;
  s32 bestSlot;
  s32 sortedCount;
  s32 value;

  bestSlot = 0;
  sortedCount = 0;
  Menu_CopyListOrder(mode, order);

  for (i = 0; i < INVENTORY_SLOTS; i++)
    items[i] = inventory[i];

  count = 0;
  for (i = 0; i < INVENTORY_SLOTS; i++) {
    if (items[i] != 0)
      count++;
  }

  for (i = count; i < INVENTORY_SLOTS; i++)
    sorted[i] = 0;

  // One pass for each entry of the order: move its items across, the highest
  // id first, until none is left.
  for (pass = 0; order[pass] != LIST_ORDER_END; pass++) {
    for (;;) {
      best = 0;
      for (i = 0; i < count; i++) {
        if (items[i] == 0)
          continue;
        item = Item_GetDirectFar(items[i]);
        if ((order[pass] & LIST_ORDER_TYPE_MASK) != item->type)
          continue;
        if ((order[pass] & LIST_ORDER_EQUIPPED) &&
            (items[i] & INVENTORY_EQUIPPED) == 0)
          continue;
        value = items[i] & INVENTORY_ITEM_ID_MASK;
        if (best < value) {
          bestSlot = i;
          best = value;
        }
      }
      if (best == 0)
        break;
      sorted[sortedCount] = items[bestSlot];
      items[bestSlot] = 0;
      sortedCount++;
    }
  }

  for (i = 0; i < count; i++)
    inventory[i] = sorted[i];
  return 1;
}

/// Copies the list order of mode (enum ListSort; any other mode takes
/// LIST_SORT_ITEMS_FIRST's) into order, up to and including its LIST_ORDER_END
/// and at most LIST_ORDER_MAX_LENGTH bytes.
void Menu_CopyListOrder(s32 mode, u8 *order) {
  u8 *src;
  s32 n;

  src = gMenu_ListOrderItemsFirst;
  switch (mode) {
  case LIST_SORT_EQUIPPED_THEN_ITEMS:
    src = gMenu_ListOrderEquippedThenItems;
    break;
  case LIST_SORT_EQUIPPED_THEN_EQUIPMENT:
    src = gMenu_ListOrderEquippedThenEquipment;
    break;
  case LIST_SORT_EQUIPMENT_FIRST:
    src = gMenu_ListOrderEquipmentFirst;
    break;
  }
  *order = *src;
  n = 0;
  if (*order != LIST_ORDER_END) {
    do {
      if (++n >= LIST_ORDER_MAX_LENGTH)
        break;
      src++;
      order++;
      *order = *src;
    } while (*order != LIST_ORDER_END);
  }
}

/// Moves a paged list's cursor and page with the d-pad's repeating keys.
///
/// count entries are shown pageSize at a time; *cursor is the row on the page
/// and *page the page. Vertical lists (horizontal 0) move the cursor with Up
/// and Down and turn pages with Left and Right; horizontal lists the other way
/// round. Moving past either end wraps round; turning to a shorter page pulls
/// the cursor up to its last entry. Returns 1 after a page turn, 0 after a
/// cursor move and -1 when nothing moved or the list is empty.
s32 PagedList_HandleInput(s32 horizontal, s32 count, s32 pageSize, s32 *cursor,
                          s32 *page) {
  s32 pages;
  s32 next;
  s32 previous;
  s32 pageForward;
  s32 result;

  result = -1;
  if (count == 0)
    return result;
  // Animate the page arrows.
  PageArrows_DrawFar(PAGE_ARROW_VRAM);
  pages = count / pageSize;
  if (count % pageSize != 0)
    pages++;
  // horizontal is reused for the page-back key.
  if (horizontal) {
    next = gInput.repeat & KEY_RIGHT;
    previous = gInput.repeat & KEY_LEFT;
    horizontal = gInput.repeat & KEY_UP;
    pageForward = gInput.repeat & KEY_DOWN;
  } else {
    next = gInput.repeat & KEY_DOWN;
    previous = gInput.repeat & KEY_UP;
    horizontal = gInput.repeat & KEY_LEFT;
    pageForward = gInput.repeat & KEY_RIGHT;
  }
  if (horizontal) {
    Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    if (--*page < 0)
      *page = pages - 1;
    if (*cursor + *page * pageSize > count - 1) {
      *cursor = count - *page * pageSize - 1;
      if (*cursor > pageSize - 1)
        *cursor = pageSize - 1;
    }
    Input_ResetKeyRepeat();
    result = 1;
    return result;
  }
  if (pageForward) {
    Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    if (++*page > pages - 1)
      *page = 0;
    if (*cursor + *page * pageSize > count - 1) {
      *cursor = count - *page * pageSize - 1;
      if (*cursor > pageSize - 1)
        *cursor = pageSize - 1;
    }
    Input_ResetKeyRepeat();
    result = 1;
    return result;
  }
  if (previous) {
    Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    if (--*cursor < 0) {
      // DEAD: a store the next overwrites; the ROM keeps it.
      *cursor = pageSize - 1;
      *cursor = count - *page * pageSize - 1;
      if (*cursor > pageSize - 1)
        *cursor = pageSize - 1;
    }
    return 0;
  }
  result = -1;
  if (next == 0)
    return result;
  Sound_PlayFar(SE_MENU_CURSOR_MOVE);
  result = 0;
  if (++*cursor == count - *page * pageSize)
    *cursor = result;
  if (*cursor > pageSize - 1)
    *cursor = result;
  return 0;
}

/// Copies WINDOW_PALETTE into background palette paletteNumber and lightens its
/// WINDOW_BACKGROUND_COLOR by HIGHLIGHT_LIGHTEN_STEPS in each channel: the
/// highlight colour of the menu's selected rows.
void Palette_LightenHighlight(s32 paletteNumber) {
  u16 *palette = (u16 *)&BG_PLTT_COLOR(paletteNumber, 0);
  u32 red;
  u32 green;
  u32 blue;

  // Both transfers copy the same 32 bytes, first as 16 halfwords and then as
  // 8 words.
  Dma_Set((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette,
          DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette,
          DMA_COPY_WORDS(8), REG_DMA3);
  blue = palette[WINDOW_BACKGROUND_COLOR] >> RGB_BLUE_SHIFT;
  green = RGB_GREEN(palette[WINDOW_BACKGROUND_COLOR]);
  red = RGB_RED(palette[WINDOW_BACKGROUND_COLOR]);
  blue += HIGHLIGHT_LIGHTEN_STEPS;
  if (blue > RGB_LEVEL_MAX)
    blue = RGB_LEVEL_MAX;
  green += HIGHLIGHT_LIGHTEN_STEPS;
  if (green > RGB_LEVEL_MAX)
    green = RGB_LEVEL_MAX;
  red += HIGHLIGHT_LIGHTEN_STEPS;
  if (red > RGB_LEVEL_MAX)
    red = RGB_LEVEL_MAX;
  palette[WINDOW_BACKGROUND_COLOR] = RGB(red, green, blue);
}

/// The tile of page 1's number; each later page's is the tile after.
#define PAGE_LABEL_FIRST 49

/// Draws a paged list's page numbers on the top border of window, ending at
/// tile rightEdge: one digit per page between the page arrows, selectedPage in
/// the highlight palette and the others in the window's.
void PagedList_DrawPageNumbers(s32 window, s32 itemCount, s32 pageSize,
                               s32 selectedPage, s32 rightEdge) {
  s32 pageCount;
  s32 page;
  s32 x;
  s32 tile;

  x = rightEdge;
  tile = PAGE_LABEL_FIRST;
  pageCount = itemCount / pageSize;
  if (itemCount % pageSize != 0)
    pageCount++;

  x -= pageCount;
  if (pageCount > 1) {
    Window_SetTilemapEntryFar(window, PAGE_ARROW_LEFT, x - 1, -1, 0);

    for (page = 0; page < pageCount; page++) {
      if (page == selectedPage)
        Window_SetTilemapEntryFar(window, tile, x, -1, TILEMAP_ENTRY_HIGHLIGHT);
      else
        Window_SetTilemapEntryFar(window, tile, x, -1, TILEMAP_ENTRY_WINDOW);
      tile++;
      x++;
    }

    Window_SetTilemapEntryFar(window, PAGE_ARROW_RIGHT, x, -1, 0);
  }
}

/// Switches a rectangle of a window's text between WINDOW_PALETTE (normal) and
/// HIGHLIGHT_PALETTE (the highlight made by Palette_LightenHighlight): palette
/// is the new palette number.
void Window_SetHighlightRect(const struct Window *window, s32 x, s32 y,
                             s32 width, s32 height, u32 palette) {
  struct WindowState *state = (struct WindowState *)Ram_HeapSlots->windowState;

  x += window->x + 1;
  y += window->y + 1;
  palette = BG_TILE_PALETTE(palette);
  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > DISPLAY_TILE_WIDTH - 1) {
    width = DISPLAY_TILE_WIDTH - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  // BUG: tests the bottom against the screen's width but clips to its rows, so
  // a rectangle ending in rows 20 to 28 is kept whole and writes past the 32
  // by 20 tilemap into the windows after it.
#ifdef BUGFIX
  if (y + height > DISPLAY_TILE_HEIGHT - 1) {
#else
  if (y + height > DISPLAY_TILE_WIDTH - 1) {
#endif
    height = DISPLAY_TILE_HEIGHT - y;
  }
  if (width > 0 && height > 0) {
    do {
      u16 *entry = &state->tilemap[y][x];
      s32 remaining = width;
      while (remaining != 0) {
        u32 value = *entry;
        value &= ~BG_TILE_PALETTE(WINDOW_PALETTE ^ HIGHLIGHT_PALETTE);
        value |= palette;
        remaining--;
        *entry = value;
        entry++;
      }
      height--;
      y++;
    } while (height != 0);
    state->dirty = 1;
  }
}

/// Lends HIGHLIGHT_PALETTE to the element icons: copies the first sprite
/// palette, the icons' colours, into it, then WINDOW_BACKGROUND_COLOR over its
/// TEXT_BACKGROUND_COLOR.
void Palette_LoadIconsIntoHighlight(void) {
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
          (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR),
          DMA_COPY_HALFWORDS(1), REG_DMA3);
}

/// Shows the icons of one page of a list, one row of 16 pixels each from (x,
/// y): entries first to first + pageSize - 1, stopping early at an empty icon
/// slot or the state's itemCount. Hides all the others.
void Menu_SetPageIcons(s32 pageSize, s32 first, s32 window, s32 x, s32 y) {
  struct ItemMenuState *menu = Ram_HeapSlots->menuState;
  struct WindowSprite *icon;
  s32 i;

  for (i = 0; i < MENU_ENTRY_ICONS; i++) {
    icon = menu->entryIcons[i];
    if (icon != NULL)
      icon->motion = WINDOW_SPRITE_MOTION_HIDDEN;
  }
  for (i = first;
       i < pageSize + first && (icon = menu->entryIcons[i]) != NULL &&
       i <= menu->itemCount - 1;
       i++) {
    icon->x = x;
    icon->y = y + (i - first) * 16;
    Icon_Show(icon);
    icon->motion = WINDOW_SPRITE_MOTION_STILL;
  }
}

/// Writes the party's coins and the "Coins" label into window: the info
/// window of the Item and Psynergy screens' party member choice.
void Menu_DrawCoins(s32 window) {
  Text_DrawNumberAtOffsetFar(gMainRoutineState.coins, 7, window, 8, 0);
  Text_DrawMessageAsTilesFar(MsgCoinsLabel, window, 64, 0);
  // Only the Japanese editions draw a vertical divider at tile column 12.
#if LANGUAGE == LANGUAGE_JAPANESE
  Window_DrawDividerLineFar(window, 12, 0, 12, 2);
#endif
}

/// Moves and resizes a window, in tiles.
void Window_SetBounds(struct Window *window, s32 x, s32 y, s32 width,
                      s32 height) {
  if (window != NULL) {
    window->width = width;
    window->x = x;
    window->height = height;
    window->y = y;
  }
}

/// Plays sound cue and returns 1.
s32 Sound_PlayReturnOne(s32 cue) {
  Sound_PlayFar(cue);
  return 1;
}

/// A task while the menus watch for Start: blinks the class change's down
/// arrow every four frames, and on Start plays the cancel sound, sets
/// FLAG_MENU_START_PRESSED and removes itself.
void Menu_WatchStart(void) {
  u32 frame;
  u32 *node;
  struct InputState *input;
  u32 mask;

  frame = gFrameCount;
  if ((frame & 3) == 0) {
    if (frame & 4)
      Iwram_CopyWords(BG_CHAR_BLOCK(0) +
                          CLASS_CHANGE_ARROW_TILE * TILE_SIZE_4BPP,
                      gMenu_ClassChangeArrowTile, TILE_SIZE_4BPP);
    else
      Iwram_FillWords(BG_CHAR_BLOCK(0) +
                          CLASS_CHANGE_ARROW_TILE * TILE_SIZE_4BPP,
                      TILE_SIZE_4BPP, TEXT_CANVAS_BACKGROUND_WORD);
  }
  input = &gInput;
  mask = KEY_START;
  if (input->pressed & mask) {
    Sound_PlayFar(SE_MENU_CANCEL);
    GameFlag_Set(FLAG_MENU_START_PRESSED);
    Task_Remove((u32)((s32)Menu_WatchStart));
  }
  // DEAD: walks the free window sprites to the end of their list and keeps
  // nothing; the ROM keeps the loop.
  node = (u32 *)&((struct WindowState *)Ram_HeapSlots->windowState)
             ->freeSprites.head;
  while ((node = (u32 *)*node) != 0)
    ;
}

/// Starts watching for Start: clears FLAG_MENU_START_PRESSED and adds
/// Menu_WatchStart.
void Menu_StartWatchingStart(void) {
  GameFlag_Clear(FLAG_MENU_START_PRESSED);
  Task_Add((s32)Menu_WatchStart, TASK_PASS_FRAME);
}

/// Stops watching for Start, if Start has not removed the watch already.
void Menu_StopWatchingStart(void) {
  if (GameFlag_Get(FLAG_MENU_START_PRESSED) == 0) {
    Task_Remove((u32)((s32)Menu_WatchStart));
  }
}

/// Unused: draws the next text in TEXT_COLOR_NORMAL.
UNUSED void Text_SetNormalColor(void) { Text_SetColorFar(TEXT_COLOR_NORMAL); }

/// Unused: draws the next text in TEXT_COLOR_RED.
UNUSED void Text_SetRedColor(void) { Text_SetColorFar(TEXT_COLOR_RED); }

/// Unused: draws the next text in TEXT_COLOR_YELLOW.
UNUSED void Text_SetYellowColor(void) { Text_SetColorFar(TEXT_COLOR_YELLOW); }

#undef STATUS_PANEL_LEARN_NOTE_Y
#undef DIGIT_COUNT_LIMIT
