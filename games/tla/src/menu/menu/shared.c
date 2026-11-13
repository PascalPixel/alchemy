/// @file
/// The pieces the menu screens share: windows, the status panel, numbers,
/// icons, the party figures, the hand cursor, paged lists, palettes, the
/// cancel sound and text colours.
// Part of menu.c.

#include "../menu_internal.h"

/// @file
/// The pieces the menu screens share: windows, the status panel, numbers,
/// icons, the party figures, the hand cursor, paged lists, palettes, the
/// cancel sound and text colours.
// Part of menu.c.

s32 GameFlag_SetBit(s32 flag_no);

void Menu_ResumeFieldTasks(void) {
  Task_ResumeOverlayTasks();
  Map_DisableUpdateCallbackFar();
  GameFlag_ClearBit(0x166);
  GameFlag_ClearBit(0x152);
}

s32 Menu_SuspendFieldTasks(void) {
  GameFlag_SetBit(0x152);
  GameFlag_SetBit(0x166);
  Map_EnableUpdateCallbackFar();
  return Task_SuspendOverlayTasks();
}

void Window_InitializeWork(s32 unused) {
  struct ItemMenuWork *work = Ram_HeapSlots->menuRuntime;

  Dma_Fill32(0, work, 0x33b * 4);
  work->sideIndex[0] = -1;
  work->sideCount[0] = 1;
  work->sideCount[1] = 1;
  work->columnCount = 1;
  work->rowCount = 1;
}

void Window_CloseFar();
struct Window *Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void WindowSprite_RedrawSavedRectFar(struct Window *window);

/// Creates *handle's window if absent and returns 1. Otherwise returns 0,
/// redrawing its saved rectangle unless flags has 0x100 set.
/// flags: the low 8 bits select the window type.
s32 Window_UpdateOrCreate(s32 *handle, s32 x, s32 y, s32 w, s32 h, s32 flags) {
  s32 cur;
  s32 style;

  cur = *handle;
  if (cur != 0) {
    if (0x100 & flags) {
      return 0;
    }
    WindowSprite_RedrawSavedRectFar((struct Window *)cur);
    return 0;
  }
  style = flags & 0xff;
  *handle = (s32)Window_CreateFar(x, y, w, h, style);
  return 1;
}

/// Closes *handle's window if present, then sets the handle to NULL.
/// Although declared with one argument, the caller's second argument stays in
/// r1 and reaches Window_CloseFar.
void Window_CloseIfOpen(void **handle, s32 mode) {
  if (*handle != NULL) {
    Window_CloseFar(*handle, mode);
    *handle = NULL;
  }
}

// Japanese draws the name and the ailment labels 8 pixels further right, the
// Lv label through Text_DrawStringInWindowFar, and the equip panel's "cannot
// equip" line 16 pixels in, as the class panel's is. Spanish swaps the learn
// panel's two lines: the note above the Psynergy's name.
#if LANGUAGE == LANGUAGE_JAPANESE
#define STATUS_PANEL_NAME_X 40
#define DRAW_LEVEL_LABEL Text_DrawStringInWindowFar
#define STATUS_PANEL_CANNOT_EQUIP_X 16
#else
#define STATUS_PANEL_NAME_X 32
#define DRAW_LEVEL_LABEL Text_DrawStringAtOffsetFar
#define STATUS_PANEL_CANNOT_EQUIP_X 0
#endif
#if LANGUAGE == LANGUAGE_SPANISH
#define STATUS_PANEL_LEARN_NAME_Y 56
#define STATUS_PANEL_LEARN_NOTE_Y 48
#else
#define STATUS_PANEL_LEARN_NAME_Y 48
#define STATUS_PANEL_LEARN_NOTE_Y 56
#endif

#define STATUS_PANEL_LOWER_ONLY 0x100
#define STATUS_PANEL_KIND_MASK 0xff

enum StatusPanelKind {
  STATUS_PANEL_EXPERIENCE = 0,
  STATUS_PANEL_EQUIP = 2,
  STATUS_PANEL_LEARN = 4,
  STATUS_PANEL_CLASS = 6,
  STATUS_PANEL_STATS = 8,
  STATUS_PANEL_EQUIP_CLASS = 9
};

// Messages are linker symbols; the unnamed ones by their numbers.
#define MESSAGE_CLASS_NAME ((s32)&MsgClassName)
#define MESSAGE_EXP_LABEL ((s32)&MsgExpLabel)
#define MESSAGE_PANEL_STAT_LABEL ((s32)&MsgPanelStatLabel)
#define MESSAGE_ABILITY_NAME ((s32)&MsgAbilityNothing)
#define MESSAGE_POISON_LABEL ((s32)&MsgPoisonLabel)
#define MESSAGE_VENOM_LABEL ((s32)&MsgVenomLabel)
#define MESSAGE_CURSE_LABEL ((s32)&MsgCurseLabel)
#define MESSAGE_HAUNT_LABEL ((s32)&MsgHauntLabel)
#define MESSAGE_CANNOT_EQUIP ((s32)&MsgCannotEquip)
#define MESSAGE_WILL_LEARN ((s32)&MsgWillLearn)
#define MESSAGE_LEARNED ((s32)&MsgLearned)

extern const u8 gStatusPanel_LvString[];

void Window_ClearInteriorTilesFar(struct RenderInput *window, s32 x, s32 y,
                                  s32 width, s32 height);
void WindowSprite_ClearListFar(struct RenderInput *window);
void *SideObject_CreateFar(s32 owner, s32 x, s32 y, struct RenderInput *window,
                           s32 a, s32 b);
void Unit_UpdateClassFar(s32 owner);

static inline void RestoreUnit(struct BattleUnit *unit,
                               struct BattleUnit *backup) {
  /* FAKEMATCH(wrapper): both restores forwarded through an inline, so the size
     loads after the call's other arguments; as direct Iwram_CopyWords calls, 2
     of 494 instructions differ in tla-en. */
  Iwram_CopyWordsEntry(unit, backup, sizeof(struct BattleUnit));
}

/// Draws the status panel of the Item and Psynergy screens for the party
/// party member `member` (window at tile 0,5, 13 by 12): the name, up to two
/// statuses or the level, the party member's portrait, and below them what
/// mode's panel kind asks for. slot is the inventory slot of the item the panel
/// is about, for the equipment and Psynergy item panels. The first parameter,
/// the status window, is not used: the window is the menu work's.
///
/// Opens the window on first use and keeps it in the work's statusWindow;
/// unless mode has STATUS_PANEL_LOWER_ONLY, the portrait it creates replaces
/// the work's cursor. The equipment panel equips the item on the real unit for
/// a moment and puts the unit back from a heap copy it frees before returning.
void StatusPanel_Draw(struct RenderInput *unused, s32 owner, s32 slot, s32 mode) {
  s32 created;
  struct ItemDefinition *def;
  u32 item;
  struct ItemMenuWork *state;
  struct BattleUnit *unit;
  struct RenderInput *window;
  s32 ailmentRows;
  s32 value;
  struct BattleUnit *backup;
  u8 ailments[8];

  created = 0;
  state = (struct ItemMenuWork *)Ram_HeapSlots->menuRuntime;
  unit = Unit_Get(owner);
  item = unit->inventory[slot];
  def = Item_Get(item & ITEM_ID_MASK);
  if (!(mode & STATUS_PANEL_LOWER_ONLY))
    created =
        Window_UpdateOrCreate((s32 *)&state->statusWindow, 0, 5, 13, 12, 0x102);
  window = state->statusWindow;

  if (!(mode & STATUS_PANEL_LOWER_ONLY)) {
    if (!created) {
      wait(1);
      Window_ClearInteriorTilesFar(state->statusWindow, 0, 0, 88, 32);
    }
    Text_DrawStringAtOffsetFar(unit->name, (s32)window, STATUS_PANEL_NAME_X, 0);
    CharacterMenu_BuildAvailability(ailments, 1, owner);
    ailmentRows = 0;
    if (ailments[1]) {
      Text_DrawMessageAsTilesFar(MESSAGE_POISON_LABEL, window,
                                    STATUS_PANEL_NAME_X, ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[2]) {
      Text_DrawMessageAsTilesFar(MESSAGE_VENOM_LABEL, window,
                                    STATUS_PANEL_NAME_X, ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[3]) {
      Text_DrawMessageAsTilesFar(MESSAGE_CURSE_LABEL, window,
                                    STATUS_PANEL_NAME_X, ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[4]) {
      Text_DrawMessageAsTilesFar(MESSAGE_HAUNT_LABEL, window,
                                    STATUS_PANEL_NAME_X, ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailmentRows < 2) {
      value = unit->level;
      DRAW_LEVEL_LABEL((u8 *)gStatusPanel_LvString, (s32)window, 40, 16);
      Text_DrawNumberInWindowFar(value, 4, (s32)window, 56, 16);
    }
  }

  if (!created) {
    wait(1);
    Window_ClearInteriorTilesFar(state->statusWindow, 0, 32, 88, 80);
  }
  WindowSprite_ClearListFar(window);
  if (!(mode & STATUS_PANEL_LOWER_ONLY))
    state->cursor = SideObject_CreateFar(owner, 0, 0, window, 0, 0);

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
    value = unit->classIndex + MESSAGE_CLASS_NAME;
    Text_DrawMessageAsTilesFar(value, window, 0, 32);
    if (!Item_CanUnitEquip(owner, item)) {
      Text_DrawMessageAsTilesFar(MESSAGE_CANNOT_EQUIP, window, 16, 48);
      break;
    }
    backup = Heap_Alloc(sizeof(struct BattleUnit));
    Iwram_CopyWordsEntry(backup, unit, sizeof(struct BattleUnit));
    if (state->equipPreview) {
      unit->inventory[slot] &= ~INVENTORY_EQUIPPED;
      Unit_UpdateClassFar(owner);
    } else {
      Inventory_EquipFar(owner, slot);
    }
    Unit_CalculateFar(owner);
    Window_SetTilemapEntryFar((s32)window, 0xf12a, 3, 5, 0);
    value = unit->classIndex + MESSAGE_CLASS_NAME;
    Text_DrawMessageAsTilesFar(value, window, 0, 48);
    RestoreUnit(unit, backup);
    Heap_Release(backup);
    break;
  case STATUS_PANEL_EXPERIENCE:
  experience:
    value = unit->classIndex + MESSAGE_CLASS_NAME;
    Text_DrawMessageAsTilesFar(value, window, 0, 32);
    StatusPanel_DrawHpPp(unit, (s32)window);
    value = unit->experience;
    Text_DrawMessageAsTilesFar(MESSAGE_EXP_LABEL, window, 0, 64);
    Text_DrawNumberInWindowFar(value, 8, (s32)window, 24, 72);
    break;
  case STATUS_PANEL_CLASS:
    value = unit->classIndex + MESSAGE_CLASS_NAME;
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
    backup = Heap_Alloc(sizeof(struct BattleUnit));
    Iwram_CopyWordsEntry(backup, unit, sizeof(struct BattleUnit));
    if (state->equipPreview)
      unit->inventory[slot] &= ~INVENTORY_EQUIPPED;
    else
      Inventory_EquipFar(owner, slot);
    Unit_CalculateFar(owner);
    Text_DrawStatComparison(unit, backup, (s32)window);
    RestoreUnit(unit, backup);
    Heap_Release(backup);
    break;
  case STATUS_PANEL_LEARN: {
    s32 psynergy = def->actionId;
    s32 known = 0;
    s32 i;

    for (i = 0; i < 32; i++) {
      if ((unit->actionSlots[i].encodedAction & OWNER_ACTION_ID_MASK) ==
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
    value = unit->atk;
    Text_DrawNumberInWindowFar(value, 3, (s32)window, 64, 40);
    Text_DrawMessageAsTilesFar(labels + 1, window, 0, 48); // Defense
    value = unit->def;
    Text_DrawNumberInWindowFar(value, 3, (s32)window, 64, 48);
    Text_DrawMessageAsTilesFar(labels + 4, window, 0, 56); // Agility
    value = unit->agi;
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
#define FIELD_PIXELS_PER_DIGIT 8

/// Returns how many decimal digits number has, but at least 2: 0-99 give 2,
/// 100-999 give 3, 1000-9999 give 4. The count starts with the units digit,
/// adds one for each division by ten that leaves 10 or more, then adds the
/// leading digit; a number under 10 has its one digit counted as both, which
/// is where the minimum of 2 comes from.
static inline s32 CountDigitsAtLeastTwo(s32 number) {
  s32 count;

  // 16 only bounds the loop: an s32 has at most 10 digits, so the count
  // always stops first.
  for (count = 1; count < 16; count++) {
    number /= 10;
    if (number < 10) {
      break;
    }
  }
  count++; // the leading digit
  return count;
}

/// Draws number right-aligned in a field that ends at pixel column x and
/// allows 8 pixels for each digit, at least two digits wide.
///
/// The international editions draw a character's HP and PP with it
/// (StatusPanel_DrawHpPp). The Japanese editions draw those figures in
/// four-character fields padded with spaces, which keeps the digits in line
/// because the Japanese font's space is as wide as a digit (7 pixels). The
/// Latin font's space is 5 pixels, so here each figure gets a field as wide
/// as its digits instead.
///
/// The number writer draws the digits 7 pixels apart, so a number ends about
/// a pixel per digit before x: 99 ends 2 pixels before it, 100 ends 3. A
/// number under 10 draws as a 5-pixel space and its digit, and ends 4 pixels
/// before x.
///
/// number: the value to draw, 0 or more
/// window: the window to draw in, its struct RenderInput pointer held as an
///         s32, as its callers hold it
/// x:      the field's right edge, in pixels within the window
/// y:      the pixel row of the text, within the window
void Text_DrawNumberRightAligned(s32 number, s32 window, s32 x, s32 y) {
  s32 width = CountDigitsAtLeastTwo(number);

  // Step x back to the field's left edge; the number writer right-aligns the
  // number in a field of width characters. x changes in place: a separate
  // left-edge local changes the registers.
  x -= width * FIELD_PIXELS_PER_DIGIT;
  Text_DrawNumberAtOffsetFar(number, width, window, x, y);
}

#endif

/// Draws a character's HP and PP into window as two rows, "HP current/maximum"
/// at pixel row 40 and "PP current/maximum" at row 48. Current HP is drawn in
/// colour 4 when it is below a quarter of maximum HP and in colour 2 at 0;
/// then the colour goes back to 15, the default text colour.
///
/// unit:   the character whose figures are drawn
/// window: the window to draw in, its struct RenderInput pointer held as an
///         s32
void StatusPanel_DrawHpPp(struct BattleUnit *unit, s32 window) {
  // Each figure in turn, as an s16. Read straight from unit, the figures
  // compile differently.
  s16 value;

#if (LANGUAGE != LANGUAGE_JAPANESE)
  // The label at the left and a slash at 48. A current figure ends before
  // the slash, at 48, and a maximum at 88, each in a field as wide as its
  // digits.
  Text_DrawStringAtOffsetFar(gStatusPanel_HpString, window, 0, 40);
  Text_DrawStringInWindowFar((u8 *)gStatusPanel_SlashString, window, 48, 40);
  value = unit->maxHp;
  Text_DrawNumberRightAligned(value, window, 88, 40);
  value = unit->hp;
  if (value < unit->maxHp >> 2) { // below a quarter of maximum HP
    Text_SetColorFar(4);
  }
  if (value == 0) {
    Text_SetColorFar(2);
  }
  Text_DrawNumberRightAligned(value, window, 48, 40);
  Text_SetColorFar(15);
  Text_DrawStringAtOffsetFar(gStatusPanel_PpString, window, 0, 48);
  Text_DrawStringInWindowFar((u8 *)gStatusPanel_SlashString, window, 48, 48);
  value = unit->pp;
  Text_DrawNumberRightAligned(value, window, 48, 48);
  value = unit->maxPp;
  Text_DrawNumberRightAligned(value, window, 88, 48);
#else
  // Each row string ("HP    /" and "EP    /") holds the label and the slash,
  // one character to an 8-pixel cell, the slash in the cell at 48. Each
  // figure is four characters padded with spaces: the current one drawn
  // from 16 at the font's widths, the maximum from 56 a cell a character,
  // ending at 88.
  Text_DrawStringInWindowFar((u8 *)gStatusPanel_HpRowString, window, 0, 40);
  value = unit->maxHp;
  Text_DrawNumberInWindowFar(value, 4, window, 56, 40);
  value = unit->hp;
  if (value < unit->maxHp >> 2) { // below a quarter of maximum HP
    Text_SetColorFar(4);
  }
  if (value == 0) {
    Text_SetColorFar(2);
  }
  Text_DrawNumberAtOffsetFar(value, 4, window, 16, 40);
  Text_SetColorFar(15);
  Text_DrawStringInWindowFar((u8 *)gStatusPanel_PpRowString, window, 0, 48);
  value = unit->pp;
  Text_DrawNumberAtOffsetFar(value, 4, window, 16, 48);
  value = unit->maxPp;
  Text_DrawNumberInWindowFar(value, 4, window, 56, 48);
#endif
}

s32 Icon_CreateStatChangeArrow(struct RenderInput *window, s32 x, s32 y, s32 variant);

void Text_DrawStatComparison(const struct BattleUnit *alt,
                             const struct BattleUnit *base, s32 work) {
  Text_DrawMessageAsTilesFar((s32)MsgPanelStatLabel,
                                (struct RenderInput *)work, 0, 32);
  Text_DrawNumberAtOffsetFar(base->atk, 3, work, 16, 40);
  if (alt->atk != base->atk) {
    Text_DrawNumberAtOffsetFar(alt->atk, 3, work, 64, 40);
    if (alt->atk > base->atk) {
      Icon_CreateStatChangeArrow(work, 44, 36, 0);
    } else {
      Icon_CreateStatChangeArrow(work, 44, 36, 1);
    }
  }
  Text_DrawMessageAsTilesFar((s32)MsgPanelDefenseLabel,
                                (struct RenderInput *)work, 0, 48);
  Text_DrawNumberAtOffsetFar(base->def, 3, work, 16, 56);
  if (alt->def != base->def) {
    Text_DrawNumberAtOffsetFar(alt->def, 3, work, 64, 56);
    if (alt->def > base->def) {
      Icon_CreateStatChangeArrow(work, 44, 52, 0);
    } else {
      Icon_CreateStatChangeArrow(work, 44, 52, 1);
    }
  }
  Text_DrawMessageAsTilesFar((s32)MsgAgilityLabel,
                                (struct RenderInput *)work, 0, 64);
  Text_DrawNumberAtOffsetFar(base->agi, 3, work, 16, 72);
  if (alt->agi != base->agi) {
    Text_DrawNumberAtOffsetFar(alt->agi, 3, work, 64, 72);
    if (alt->agi > base->agi) {
      Icon_CreateStatChangeArrow(work, 44, 68, 0);
    } else {
      Icon_CreateStatChangeArrow(work, 44, 68, 1);
    }
  }
}

s32 Icon_CreateWithResourceVariant(struct RenderInput *window, s32 x, s32 y) {
  s32 slot;
  s32 icon;

  icon = 0;
  slot = Resource_FindFreeEntry();
  if (slot != 0) {
    VramBlock_LoadResourceFar(slot, 0x80, (s32)&ResourceId_UiIconTiles);
    icon = (s32)WindowSprite_CreateFar(slot, OUTPUT_SIZE_16X16, window, x, y);
  }
  return icon;
}

/// Turns an icon's window sprite on and copies its screen position into its
/// OAM attributes, with no affine matrix.
void Icon_PrepareActor(struct WindowSprite *object) {
  if (object != NULL) {
    object->active = 1;
    object->sprite.oam.x = object->x;
    object->sprite.oam.y = object->y;
    object->sprite.oam.matrix = 0;
    object->sprite.oam.affineMode = 0;
  }
}

/// Loads the character's graphics into slot 0.
/// menu: calling menu (unused).
/// unit: character ID.
void PsynergyMenu_CallIconRoutineWithValue(s32 menu, s32 unit) {
  Portrait_ReloadFar(0, unit, 0);
}

// The menu work, Ram_HeapSlots->menuRuntime, as the hand cursor and the party
// figures read it: The Broken Seal's PsynergyMenuWork, whose names the first
// three fields take.

extern char ResourceId_ShopUpArrowTiles;
extern char ResourceId_ShopDownArrowTiles;

/// Opens the menu's top-left window (13 by 5 tiles) with a divider, puts the
/// hand cursor in it, hidden, and adds the up and down arrow icons at its right
/// edge when a VRAM entry is free. Returns the window.
// The twin of The Broken Seal's MenuCursor_Create, which has no divider or
// arrows.
struct RenderInput *Func_080f88d0(struct OwnerMenuState *work) {
  struct RenderInput *window;
  struct WindowSprite *icon;
  s32 zero = 0;
  s32 sentinel;
  s32 slot;

  work->auxiliaryWindow = NULL;
  Window_UpdateOrCreate((s32 *)&work->auxiliaryWindow, 0, 0, 13, 5, 2);
  window = work->auxiliaryWindow;
  Window_DrawDividerLineFar((s32)window, 0, 3, 12, 3);
  icon = (struct WindowSprite *)Icon_CreateWithResourceVariant(window, -8, 11);
  icon->active = 13;
  work->tabIndex[0] = 255;
  work->sideIcon[0] = icon;
  sentinel = 254;
  icon->sentinel = sentinel;
  sentinel -= 255;
  work->tabIndex[1] = zero;
  work->sideIcon[1]->sentinel = sentinel;
  slot = Resource_FindFreeEntry();
  if (slot <= 95) {
    VramBlock_LoadResourceFar(slot, 128, (s32)&ResourceId_ShopUpArrowTiles);
    icon = WindowSprite_CreateFar(slot, OUTPUT_SIZE_16X16, window, 84, -4);
    icon->active = 13;
    work->arrows[0] = icon;
  }
  slot = Resource_FindFreeEntry();
  if (slot <= 95) {
    VramBlock_LoadResourceFar(slot, 128, (s32)&ResourceId_ShopDownArrowTiles);
    icon = WindowSprite_CreateFar(slot, OUTPUT_SIZE_16X16, window, 84, 8);
    icon->active = 13;
    work->arrows[1] = icon;
  }
  return window;
}

void Func_080f89a4(void) {
  struct OwnerMenuState *menu =
      (struct OwnerMenuState *)Ram_HeapSlots->menuRuntime;
  struct SpriteScale scale;
  struct SpritePlacement position;
  s32 i;
  s32 count;

  count = (u16)Party_CountMembersFar();
  for (i = 0; i < count; i++) {
    struct AnimationObject *figure = menu->tabObjects[i];
    s32 placeY = 0x00728000 - (menu->rowPositions[i] << 15);

    if (figure != 0) {
      figure->image.priority = 0;
      scale.x = menu->ownerScale[i];
      scale.y = menu->ownerScale[i];
      position.x = menu->tabX[i] << 16;
      position.height = placeY;
      position.y = (menu->rowPositions[i] << 16) + placeY;
      position.groundHeight = 0;
      Func_08020018(figure, &position, &scale, 0x4000, 250);
    }
  }
}

extern u8 gUiMenu_CursorBobX[];
extern u8 gUiMenu_CursorBobY[];

#define CURSOR_BOB_STEP() ((gFrameCount >> 1) & 7)

/// Places the hand cursor at (xOffset, yOffset) inside the menu's window,
/// bobbing; the twin of The Broken Seal's MenuCursor_Position.
void Func_080f8a44(s32 xOffset, s32 yOffset) {
  struct OwnerMenuState *work =
      (struct OwnerMenuState *)Ram_HeapSlots->menuRuntime;
  struct WindowSprite *cursor = work->sideIcon[0];

  cursor->sprite.oam.x = cursor->x = gUiMenu_CursorBobX[CURSOR_BOB_STEP()] +
                                     xOffset + work->auxiliaryWindow->x * 8 + 8;
  cursor->sprite.oam.y = cursor->y = gUiMenu_CursorBobY[CURSOR_BOB_STEP()] +
                                     yOffset + work->auxiliaryWindow->y * 8 + 8;
}

/// The twin of The Broken Seal's MenuCursor_Slide.
void MenuCursor_Slide(s32 x, s32 y) {
  struct OwnerMenuState *work =
      (struct OwnerMenuState *)Ram_HeapSlots->menuRuntime;
  struct WindowSprite *cursor;
  s32 steps;
  s32 startX;
  s32 startY;
  s32 px;
  s32 py;
  s32 dx;
  s32 dy;

  steps = 2;
  if (work->skipSlide != 0) {
    work->skipSlide = 0;
    return;
  }
  cursor = work->sideIcon[0];
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
      s32 spriteX = (px >> 4) + (work->auxiliaryWindow->x << 3);

      cursor->sprite.oam.x = cursor->x = spriteX - 56;
    }
    py += dy;
    cursor->sprite.oam.y = cursor->y =
        (py >> 4) + (work->auxiliaryWindow->y << 3) - 56;
    steps--;
    if (steps != 0)
      wait(1);
  } while (steps != 0);
}

void PsynergyMenu_RefreshOwnerEntries(s32 x, s32 y, s32 width);

void Func_080f8bbc(void) { PsynergyMenu_RefreshOwnerEntries(108, 40, 8); }

void PsynergyMenu_RefreshOwnerEntries(s32 originX, s32 originY, s32 columns) {
  struct WindowSprite **slot;
  struct WindowSprite **scan;
  s32 index;

  index = 0;
  slot = (struct WindowSprite **)((u8 *)Ram_HeapSlots->menuRuntime + 0x4c);
  scan = slot;
  do {
    if (*scan++ != NULL) {
      PsynergyMenu_PositionOwnerEntry(slot, index, originX, originY, columns);
    }
    index++;
    slot++;
  } while (index <= 0x1F);
}

void PsynergyMenu_PositionOwnerEntry(struct WindowSprite **slot, s32 index,
                                     s32 originX, s32 originY, s32 columns) {
  struct WindowSprite *object;

  if (index > 0x1F) {
    index = 0;
  }
  object = *slot;
  object->y = (index / columns) * 0x10 + originY;
  object->x = (index % columns) * 0x10 + originX;
  Icon_PrepareActor(object);
}

/// Places item i (or 0 if above 15) in a grid with cols columns of
/// 24x16-pixel cells.
/// x0, y0: top left of the grid.
void ItemMenu_PlaceIcon(struct WindowSprite **slot, s32 i, s32 x0, s32 y0,
                        s32 cols) {
  struct WindowSprite *obj;

  if (i > 15)
    i = 0;
  obj = *slot;
  obj->y = i / cols * 16 + y0;
  obj->x = i % cols * 24 + x0;
  Icon_PrepareActor(obj);
}

void ItemMenu_RefreshEntry(s32 layout) {
  struct WindowSprite **slot;
  struct WindowSprite **scan;
  struct WindowSprite *object;
  s32 index;
  s32 originY;
  s32 base;

  base = (s32)Ram_HeapSlots->menuRuntime;
  originY = 0x38;
  if (layout != 1) {
    originY = 0x28;
  }
  slot = (struct WindowSprite **)(base + 0x4c);
  index = 0;
  scan = slot;
  do {
    object = *scan++;
    if (object != NULL) {
      ItemMenu_PlaceIcon(slot, index, 0x74, originY, 5);
    }
    index++;
    slot++;
  } while (index <= 0xE);
}

void Text_GetMessageSizeFar(s32 message, s32 *left, s32 *top, s32 *width,
                            s32 *height);

// gInput through a volatile view: its one use below says why.
#define INPUT ((volatile struct InputState *)&gInput)

/// Shows message in the item menu, with the hand cursor hidden meanwhile.
///
/// With y == -1 the message goes into the info window; otherwise a modal
/// window sized to the message opens at (x, y). With x == -1 the message stays
/// up and flag 337 records that the info window holds it; otherwise it waits
/// for A, B or Start and clears it again.
// As The Broken Seal's ItemMenu_ShowMessage: in Japanese its first
// form, from patch 1 its TBS_PATCH_1 form less the redraw after the wait.
void InventoryMenu_ShowModalMessage(s32 message, s32 x, s32 y) {
  struct ItemMenuWork *menu = (struct ItemMenuWork *)Ram_HeapSlots->menuRuntime;
  struct RenderInput *window;
  s32 height;
  s32 width;
  s32 top;
  s32 left;

  menu->sideIcons[0]->active = 13;
  if (y != -1) {
    Text_GetMessageSizeFar(message, &left, &top, &width, &height);
    if (Window_UpdateOrCreate((s32 *)&menu->modalWindow, x, y, width, height,
                              0x102) == 0)
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
      /* FAKEMATCH(volatile): a volatile view of gInput (INPUT), which makes
       * each of the three tests load the keys afresh; as gInput.pressed &
       * KEY_A and so on, 76 of 127 instructions differ in tla-en. */
    } while (!(INPUT->pressed & KEY_A) && !(INPUT->pressed & KEY_B) &&
             !(INPUT->pressed & KEY_START));
    WindowSprite_ClearListFar(window);
  } else {
    GameFlag_SetBit(337);
  }
  menu->completionFlag = 1;
  menu->sideIcons[0]->active = 1;
#if TLA_PATCH_1
  if (y != -1)
    // With the second argument Window_CloseIfOpen passes on in r1.
    ((void (*)(void **, s32))Window_CloseIfOpen)((void **)&menu->modalWindow,
                                                 1);
#endif
}

void Menu_CopyListOrder(s32 mode, u8 *order);

s32 InventoryMenu_SortByListOrder(u16 *inventory, s32 mode) {
  u16 sorted[16];
  u16 items[16];
  u8 order[32];
  struct ItemDefinition *item;
  s32 pass;
  s32 i;
  s32 count;
  s32 best;
  s32 best_slot;
  s32 sorted_count;
  s32 value;

  best_slot = 0;
  sorted_count = 0;
  Menu_CopyListOrder(mode, order);

  for (i = 0; i < 15; i++)
    items[i] = inventory[i];

  count = 0;
  for (i = 0; i < 15; i++) {
    if (items[i] != 0)
      count++;
  }

  for (i = count; i < 15; i++)
    sorted[i] = 0;

  pass = 0;
  while (order[pass] != 0xff) {
    for (;;) {
      best = 0;
      for (i = 0; i < count; i++) {
        if (items[i] == 0)
          continue;
        item = Item_Get(items[i]);
        if ((order[pass] & 0x7f) != item->type)
          continue;
        if ((order[pass] & 0x80) && (items[i] & 0x200) == 0)
          continue;
        value = items[i] & 0x1ff;
        if (best < value) {
          best_slot = i;
          best = value;
        }
      }
      if (best == 0)
        break;
      sorted[sorted_count] = items[best_slot];
      items[best_slot] = 0;
      sorted_count++;
    }
    pass++;
  }

  for (i = 0; i < count; i++)
    inventory[i] = sorted[i];
  return 1;
}

/// Copies the order table for modes 0-2, or the default table, into order.
/// Tables end with 0xff; at most 32 bytes are copied, the terminator
/// included.
void Menu_CopyListOrder(s32 mode, u8 *order) {
  u8 *src;
  s32 n;

  src = gMenu_ListOrderDefault;
  switch (mode) {
  case 0:
    src = gMenu_ListOrderMode0;
    break;
  case 1:
    src = gMenu_ListOrderMode1;
    break;
  case 2:
    src = gMenu_ListOrderMode2;
    break;
  }
  *order = *src;
  n = 0;
  if (*order != 0xff) {
    do {
      if (++n > 31)
        break;
      src++;
      order++;
      *order = *src;
    } while (*order != 0xff);
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
    goto done;
  Link_DrawPageArrowsFar(0x06002500);
  pages = __divsi3(count, pageSize);
  if (__modsi3(count, pageSize) != 0)
    pages++;
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
    Sound_PlayFar(111);
    if (--*page < 0)
      *page = pages - 1;
    if (*cursor + *page * pageSize > count - 1) {
      *cursor = count - *page * pageSize - 1;
      if (*cursor > pageSize - 1)
        *cursor = pageSize - 1;
    }
    Input_ResetKeyRepeat();
    result = 1;
    goto done;
  }
  if (pageForward) {
    Sound_PlayFar(111);
    if (++*page > pages - 1)
      *page = 0;
    if (*cursor + *page * pageSize > count - 1) {
      *cursor = count - *page * pageSize - 1;
      if (*cursor > pageSize - 1)
        *cursor = pageSize - 1;
    }
    Input_ResetKeyRepeat();
    result = 1;
    goto done;
  }
  if (previous) {
    Sound_PlayFar(111);
    if (--*cursor < 0) {
      *cursor = pageSize - 1;
      *cursor = count - *page * pageSize - 1;
      if (*cursor > pageSize - 1)
        *cursor = pageSize - 1;
    }
    goto row_changed;
  }
  result = -1;
  if (next == 0)
    goto done;
  {
    Sound_PlayFar(111);
    result = 0;
    if (++*cursor == count - *page * pageSize)
      *cursor = result;
    if (*cursor > pageSize - 1)
      *cursor = result;
  }
row_changed:
  result = 0;
done:
  return result;
}

void Palette_LightenHighlight(s32 paletteNumber) {
  u16 *palette = (u16 *)(0x05000000 + (paletteNumber << 5));
  u32 red;
  u32 green;
  u32 blue;

  Dma_Set((const void *)&BG_PLTT_COLOR(15, 0), palette, DMA_COPY_HALFWORDS(16),
          REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(15, 0), palette, DMA_COPY_WORDS(8),
          REG_DMA3);
  blue = palette[4] >> 10;
  green = (palette[4] >> 5) & 31;
  red = palette[4] & 31;
  blue += 9;
  if (blue > 31)
    blue = 31;
  green += 9;
  if (green > 31)
    green = 31;
  red += 9;
  if (red > 31)
    red = 31;
  palette[4] = RGB(red, green, blue);
}

#define PAGE_LABEL_FIRST 49
#define PAGE_CAP_LEFT 0xf128
#define PAGE_CAP_RIGHT 0xf129

void Menu_DrawPageIndicator(s32 window, s32 itemCount, s32 pageSize,
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
    Window_SetTilemapEntryFar(window, PAGE_CAP_LEFT, x - 1, -1, 0);

    for (page = 0; page < pageCount; page++) {
      if (page == selectedPage)
        Window_SetTilemapEntryFar(window, tile, x, -1, 2);
      else
        Window_SetTilemapEntryFar(window, tile, x, -1, 3);
      tile++;
      x++;
    }

    Window_SetTilemapEntryFar(window, PAGE_CAP_RIGHT, x, -1, 0);
  }
}

void Render_SetTilemapFlagRect(const struct Window *object, s32 x, s32 y,
                               s32 width, s32 height, u32 field) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;

  x += (s16)object->x + 1;
  y += (s16)object->y + 1;
  field <<= 12;
  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29) {
    width = 30 - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29) {
    height = 20 - y;
  }
  if (width > 0 && height > 0) {
    do {
      u16 *pixel = &work->tilemap[y][x];
      s32 remaining = width;
      while (remaining != 0) {
        u32 value = *pixel;
        value &= 0xFFFFEFFF;
        value |= field;
        remaining--;
        *pixel = value;
        pixel++;
      }
      height--;
      y++;
    } while (height != 0);
    work->dirty = 1;
  }
}

void Palette_CopyObjectBankToBackground14(void) {
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(14, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(15, 4), (void *)&BG_PLTT_COLOR(14, 14),
          DMA_COPY_HALFWORDS(1), REG_DMA3);
}

struct MenuPageIcon {
  u8 reserved00[5];
  u8 state;
  u16 x;
  u16 y;
};

void Menu_SetPageIcons(s32 pageSize, s32 first, s32 window, s32 x, s32 y) {
  struct MenuPageWork *menu = Ram_HeapSlots->menuRuntime;
  struct MenuPageIcon *icon;
  s32 i;

  for (i = 0; i < 32; i++) {
    icon = menu->icons[i];
    if (icon != NULL)
      icon->state = 13;
  }
  for (i = first; i < pageSize + first && (icon = menu->icons[i]) != NULL &&
                  i <= menu->entryCount - 1;
       i++) {
    icon->x = x;
    icon->y = y + (i - first) * 16;
    Icon_PrepareActor((struct WindowSprite *)icon);
    icon->state = 1;
  }
}

void Text_DrawWorkValueWithLabel(s32 work) {
  Text_DrawNumberAtOffsetFar(gMainRoutineWork.coins, 7, work, 8, 0);
  Text_DrawMessageAsTilesFar((s32)MsgCoinsLabel, (struct RenderInput *)work,
                                0x40, 0);
#if (LANGUAGE == LANGUAGE_JAPANESE)
  Window_DrawDividerLineFar(work, 12, 0, 12, 2);
#endif
}

void Window_SetBounds(struct RenderInput *window, s32 x, s32 y, s32 width,
                      s32 height) {
  if (window != NULL) {
    window->width = width;
    window->x = x;
    window->height = height;
    window->y = y;
  }
}

typedef s32 (*CopyFn)(void *, const void *, s32);
typedef s32 (*FillFn)(void *, s32, u32);

static __inline__ void CopyWords(const void *src, CopyFn copy, void *dst,
                                 s32 size) {
  /* FAKEMATCH(wrapper): the copy forwarded through an inline, which keeps the
   * load in r1 instead of r0; as a direct Iwram_CopyWords call, 6 of 47
   * instructions differ in tla-en. */
  copy(dst, src, size);
}

static __inline__ void FillWords(FillFn fill, void *dst, s32 size, u32 value) {
  /* FAKEMATCH(wrapper): the fill forwarded through an inline, which keeps the
   * r3 load and the literal pool's position; as a direct Iwram_FillWords
   * call, 4 of 47 instructions differ in tla-en. */
  fill(dst, size, value);
}

s32 Sound_PlayReturnOne(s32 cue) {
  Sound_PlayFar(cue);
  return 1;
}

void Menu_UpdateCancelSound(void) {
  u32 frame;
  u32 *node;
  struct InputState *input;
  u32 mask;

  frame = gFrameCount;
  if ((frame & 3) == 0) {
    if (frame & 4)
      CopyWords(gData_08105948, Iwram_CopyWordsEntry, (void *)0x06002540, 32);
    else
      FillWords(Iwram_FillWordsEntry, (void *)0x06002540, 32, 0x44444444);
  }
  input = &gInput;
  mask = 8;
  if (input->pressed & mask) {
    Sound_PlayFar(113);
    GameFlag_SetBit(0x150);
    Task_Remove((u32)((s32)Menu_UpdateCancelSound));
  }
  node = (u32 *)(Ram_HeapSlots->windowTiles + 0xE30);
  while ((node = (u32 *)*node) != 0)
    ;
}

void Menu_CancelSoundReset(void) {
  GameFlag_ClearBit(0x150);
  Task_Add((s32)Menu_UpdateCancelSound, TASK_PASS_VBLANK);
}

void Menu_EnsureCancelSound(void) {
  if (GameFlag_Test(0x150) == 0) {
    Task_Remove((u32)((s32)Menu_UpdateCancelSound));
  }
}

void UiPalette_SetColor15(void) { Text_SetColorFar(15); }

void UiPalette_SetColor2(void) { Text_SetColorFar(2); }

void UiPalette_SetColor4(void) { Text_SetColorFar(4); }

#undef DRAW_LEVEL_LABEL
#undef STATUS_PANEL_LEARN_NOTE_Y

#undef DRAW_LEVEL_LABEL
#undef STATUS_PANEL_LEARN_NOTE_Y
