/// @file
/// The pieces the menu screens share: windows, the status panel, numbers,
/// icons, the party figures, the hand cursor, paged lists, palettes and the
/// Start watch.
/// Part of menu.c, the menu object, compiled only through it.

#include "../menu_internal.h"

// The status panel's name and ailment column starts further left in the
// international editions, whose ailment labels are longer, and so does
// "Can't equip" (MsgCannotEquip).
#if TBS_PATCH_1
#define STATUS_PANEL_TEXT_X 32
#define STATUS_PANEL_CANNOT_EQUIP_X 0
#else
#define STATUS_PANEL_TEXT_X 40
#define STATUS_PANEL_CANNOT_EQUIP_X 16
#endif

// A Psynergy item's panel: Spanish puts "Learned" or "Will learn" above the
// Psynergy's name, the other editions below it.
#if LANGUAGE == LANGUAGE_SPANISH
#define STATUS_PANEL_LEARN_NAME_Y 56
#define STATUS_PANEL_LEARN_STATUS_Y 48
#else
#define STATUS_PANEL_LEARN_NAME_Y 48
#define STATUS_PANEL_LEARN_STATUS_Y 56
#endif

/// UiWindow_UpdateOrCreate: keep an existing window as it is, without redrawing
/// its saved rectangle. The low byte of the flags is the window style.
#define WINDOW_KEEP_EXISTING 0x100
#define WINDOW_STYLE_MASK 0xff

/// Resumes the overlay tasks and tile animations when a menu closes, and
/// clears game flags 0x166 and 0x152: undoes Menu_SuspendFieldTasks. Called
/// as DjinnMenu_Run and StatusMenu_Run close, and as ItemMenu_Run
/// (TBS_PATCH_1) and PsynergyMenu_Run (TBS_PATCH_2) close.
/// Returns nothing useful; callers ignore the value.
s32 Menu_ResumeFieldTasks(void) {
  Scheduler_ResumeOverlayTasks();
  MapAnimation_ResumeFar();
  // Flag 0x166, while set, freezes the field weather: its particles hold
  // their frame and the storm's lightning waits (psynergy_effects/weather.c).
  GameFlag_ClearBitFar(0x166);
  // Flag 0x152, while set, stops BattlePalette_UpdateBlend.
  GameFlag_ClearBitFar(0x152);
#ifdef UBFIX
  return 0;
#endif
}

/// Suspends the overlay tasks and tile animations while a menu is open, and
/// sets flags 0x152 and 0x166, so that the field's palette blend and weather
/// stand still behind it (see Menu_ResumeFieldTasks, which undoes it). Returns
/// Scheduler_SuspendOverlayTasks' result, the last task index changed or -1;
/// callers ignore it.
s32 Menu_SuspendFieldTasks(void) {
  GameFlag_SetBitFar(0x152);
  GameFlag_SetBitFar(0x166);
  MapAnimation_SuspendFar();
  return Scheduler_SuspendOverlayTasks();
}

/// Clears the whole menu work and sets its defaults: no pane selected, one
/// entry in each pane, and a 1x1 cursor grid. Each screen's run function calls
/// it once the menu work's heap block is allocated, so every window handle and
/// icon pointer in the work starts as NULL.
void UiWindow_InitializeWork(s32 unused) {
  struct InventoryMenuState *work = gMenuWork;

  Dma_Fill32(0, work, MENU_WORK_SIZE);
  work->paneIndex[0] = -1;
  work->paneCount[0] = 1;
  work->paneCount[1] = 1;
  work->columnCount = 1;
  work->rowCount = 1;
}

/// Opens the window kept in *handle, or redraws it if it is already open.
///
/// If *handle is empty, creates a window at (x, y), w by h tiles, in the style
/// given by the low byte of flags, stores it in *handle and returns 1. If the
/// window exists, redraws its saved rectangle (unless flags has
/// WINDOW_KEEP_EXISTING) and returns 0.
s32 UiWindow_UpdateOrCreate(struct UiWindow **handle, s32 x, s32 y, s32 w,
                            s32 h, s32 flags) {
  struct UiWindow *window;
  s32 style;

  window = *handle;
  if (window != 0) {
    if (WINDOW_KEEP_EXISTING & flags) {
      return 0;
    }
    RenderOutput_RedrawSavedRectFar(window);
    return 0;
  }
  style = flags & WINDOW_STYLE_MASK;
  *handle = UiWindow_CreateFar(x, y, w, h, style);
  return 1;
}

/// Closes the window kept in *handle, if any, and clears the handle. style is
/// passed on to UiWork_FinalizeFar and chooses how the window closes.
void UiWindow_CloseIfOpen(struct UiWindow **handle, s32 style) {
  if (*handle != NULL) {
    UiWork_FinalizeFar(*handle, style);
    *handle = NULL;
  }
}

/// Copies one BattleUnit with the resident IWRAM word copy.
static inline void Owner_Copy(struct BattleUnit *destination,
                              struct BattleUnit *source) {
  _call_via_r3(destination, source, sizeof(struct BattleUnit), IwramCopyWords);
}

/// Draws the status panel of the Item and Psynergy screens for the party
/// member owner (window at tile 0,5, 13 by 12): the name, up to two ailments
/// or the level, the member's portrait, and below them what mode's panel kind
/// asks for. slot is the inventory slot of the item the panel is about, for
/// the equipment and Psynergy item panels. The first parameter, the status
/// window, is not used: the window is the menu work's.
///
/// Opens the window on first use and keeps it in the work's statusWindow;
/// unless mode has STATUS_PANEL_LOWER_ONLY, the portrait it creates replaces
/// the work's cursor. The equipment panel equips the item on the real unit for
/// a moment and puts the unit back from a heap copy it frees before returning.
void Menu_DrawOwnerStatusPanel(struct UiWindow *unused, s32 owner, s32 slot,
                               s32 mode) {
  s32 created;
  struct ItemDefinition *def;
  u32 item;
  struct InventoryMenuState *state;
  struct BattleUnit *unit;
  struct UiWindow *window;
  s32 ailmentRows;
  s32 value;
  u8 ailments[8];

  created = 0;
  state = gMenuWork;
  unit = UnitGetFar(owner);
  item = unit->inventory[slot];
  def = Item_GetDirectFar(item & ITEM_ID_MASK);
  if (!(mode & STATUS_PANEL_LOWER_ONLY))
    created = UiWindow_UpdateOrCreate(&state->statusWindow, 0, 5, 13, 12,
                                      WINDOW_KEEP_EXISTING | 2);
  window = state->statusWindow;

  // The name, then each ailment the member has, one row each; the level
  // takes the third row when no more than one ailment is shown.
  if (!(mode & STATUS_PANEL_LOWER_ONLY)) {
    // A window that already existed still shows the last member: clear its
    // top 88 by 32 pixels a frame later. A new window is blank.
    if (!created) {
      WaitFrames(1);
      UiWindow_ClearInteriorTilesFar(state->statusWindow, 0, 0, 88, 32);
    }
    UiText_DrawStringAtOffsetFar(unit->name, window, STATUS_PANEL_TEXT_X, 0);
    CharacterMenu_BuildAvailability(ailments, 1, owner);
    ailmentRows = 0;
    if (ailments[CHARACTER_POISON]) {
      UiText_DrawCharacterAtOffsetFar(MsgPoisonLabel, window,
                                      STATUS_PANEL_TEXT_X, ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[CHARACTER_VENOM]) {
      UiText_DrawCharacterAtOffsetFar(MsgVenomLabel, window,
                                      STATUS_PANEL_TEXT_X, ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[CHARACTER_CURSE]) {
      UiText_DrawCharacterAtOffsetFar(MsgCurseLabel, window,
                                      STATUS_PANEL_TEXT_X, ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[CHARACTER_HAUNT]) {
      UiText_DrawCharacterAtOffsetFar(MsgHauntLabel, window,
                                      STATUS_PANEL_TEXT_X, ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailmentRows < 2) {
      value = unit->level;
      MENU_DRAW_STATUS_LABEL(gUi_LvString, window, 40, 16);
      UiText_DrawNumberInWindowFar(value, 4, window, 56, 16);
    }
  }

  // Clear the lower part and redraw the member's portrait.
  if (!created) {
    WaitFrames(1);
    UiWindow_ClearInteriorTilesFar(state->statusWindow, 0, 32, 88, 80);
  }
  RenderOutput_ClearListFar(window);
  if (!(mode & STATUS_PANEL_LOWER_ONLY))
    state->cursor = SideObject_CreateFar(owner, 0, 0, window, 0, 0);

  switch (mode & STATUS_PANEL_KIND_MASK) {
  case STATUS_PANEL_EXPERIENCE:
    value = unit->classIndex + MsgClassName;
    UiText_DrawCharacterAtOffsetFar(value, window, 0, 32);
    Ui_DrawValuePairRows(unit, window);
    value = unit->experience;
    UiText_DrawCharacterAtOffsetFar(MsgExpLabel, window, 0, 64);
    UiText_DrawNumberInWindowFar(value, 8, window, 24, 72);
    break;
  case STATUS_PANEL_CLASS:
    value = unit->classIndex + MsgClassName;
    UiText_DrawCharacterAtOffsetFar(value, window, 0, 32);
    Ui_DrawValuePairRows(unit, window);
    break;
  case STATUS_PANEL_EQUIP:
  case STATUS_PANEL_EQUIP + 1: { // draws the same panel
    struct BattleUnit *backup;

    if (!Item_CanOwnerEquip(owner, item)) {
      UiText_DrawCharacterAtOffsetFar(MsgCannotEquip, window,
                                      STATUS_PANEL_CANNOT_EQUIP_X, 48);
      break;
    }
    // Equip (or, previewing a removal, unequip) the item on the real unit,
    // recalculate its stats, show them against the saved copy, then put the
    // unit back as it was.
    backup = MemAlloc(sizeof(struct BattleUnit));
    Owner_Copy(backup, unit);
    if (state->equipPreview)
      unit->inventory[slot] &= ~INVENTORY_EQUIPPED;
    else
      Inventory_EquipFar(owner, slot);
    UnitCalcFar(owner);
    UiText_DrawStatComparison(unit, backup, window);
    Owner_Copy(unit, backup);
    MemFree(backup);
    break;
  }
  case STATUS_PANEL_LEARN: {
    s32 psynergy = def->actionId;
    s32 known = 0;
    s32 i;

    // Whether the member already knows it, from any of the 32 action slots.
    for (i = 0; i < 32; i++) {
      if ((unit->actionSlots[i].encodedAction & OWNER_ACTION_ID_MASK) ==
          psynergy) {
        known = 1;
        break;
      }
    }
    // The Psynergy's name and "Learned" or "Will learn"; Spanish puts the
    // label above the name.
    if (known) {
      UiText_DrawCharacterAtOffsetFar(psynergy + MsgAbilityName, window, 0,
                                      STATUS_PANEL_LEARN_NAME_Y);
      UiText_DrawCharacterAtOffsetFar(MsgLearned, window, 0,
                                      STATUS_PANEL_LEARN_STATUS_Y);
    } else {
      UiText_DrawCharacterAtOffsetFar(psynergy + MsgAbilityName, window, 0,
                                      STATUS_PANEL_LEARN_NAME_Y);
      UiText_DrawCharacterAtOffsetFar(MsgWillLearn, window, 0,
                                      STATUS_PANEL_LEARN_STATUS_Y);
    }
    break;
  }
  case STATUS_PANEL_STATS: {
    // The labels are consecutive messages: Attack, Defense, Status, Luck and
    // Agility.
    s32 labels = MsgPanelStatLabel;

    UiText_DrawCharacterAtOffsetFar(labels, window, 0, 40); // "Attack"
    value = unit->atk;
    UiText_DrawNumberInWindowFar(value, 3, window, 64, 40);
    UiText_DrawCharacterAtOffsetFar(labels + 1, window, 0, 48); // "Defense"
    value = unit->def;
    UiText_DrawNumberInWindowFar(value, 3, window, 64, 48);
    UiText_DrawCharacterAtOffsetFar(labels + 4, window, 0, 56); // "Agility"
    value = unit->agi;
    UiText_DrawNumberInWindowFar(value, 3, window, 64, 56);
    UiText_DrawCharacterAtOffsetFar(labels + 3, window, 0, 64); // "Luck"
    value = unit->luck;
    UiText_DrawNumberInWindowFar(value, 3, window, 64, 64);
    break;
  }
  }
}

#if LANGUAGE != LANGUAGE_JAPANESE
/// The pixels the field allows each digit. The Latin font's digits are 7
/// pixels wide, so the digits themselves take a pixel less each.
#define NUMBER_PIXELS_PER_DIGIT 8

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
  // The leading digit.
  count++;
  return count;
}

/// Draws number right-aligned in a field that ends at pixel column x and
/// allows 8 pixels for each digit, at least two digits wide.
///
/// The international editions draw a character's HP and PP with it
/// (Ui_DrawValuePairRows). The Japanese editions draw those figures in
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
/// x:      the field's right edge, in pixels within the window
/// y:      the pixel row of the text, within the window
void UiText_DrawNumberRightAligned(s32 number, struct UiWindow *window, s32 x,
                                   s32 y) {
  s32 width = CountDigitsAtLeastTwo(number);

  // Step x back to the field's left edge; the number writer right-aligns the
  // number in a field of width characters. x changes in place: a separate
  // left-edge local changes the registers.
  x -= width * NUMBER_PIXELS_PER_DIGIT;
  UiText_DrawNumberAtOffsetFar(number, width, window, x, y);
}

#endif

/// Draws a character's HP and PP into window as two rows, "HP current/maximum"
/// at pixel row 40 and "PP current/maximum" at row 48. Current HP is drawn in
/// colour 4 when it is below a quarter of maximum HP and in colour 2 at 0;
/// then the colour goes back to 15, the default text colour.
void Ui_DrawValuePairRows(struct BattleUnit *unit, struct UiWindow *window) {
  // Each figure in turn, as an s16. Read straight from unit, the figures
  // compile differently.
  s16 value;

#if LANGUAGE != LANGUAGE_JAPANESE
  // The label at the left and a slash at 48. A current figure ends before
  // the slash, at 48, and a maximum at 88, each in a field as wide as its
  // digits.
  UiText_DrawStringAtOffsetFar(gUi_HpString, window, 0, 40);
  UiText_DrawStringInWindowFar((u8 *)gUi_SlashString, window, 48, 40);
  value = unit->maxHp;
  UiText_DrawNumberRightAligned(value, window, 88, 40);
  value = unit->hp;
  if (value < unit->maxHp >> 2) { // below a quarter of maximum HP
    UiWork_SetParamNibbleFar(TEXT_COLOR_LOW_HP);
  }
  if (value == 0) {
    UiWork_SetParamNibbleFar(TEXT_COLOR_NO_HP);
  }
  UiText_DrawNumberRightAligned(value, window, 48, 40);
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  UiText_DrawStringAtOffsetFar(gUi_PpString, window, 0, 48);
  UiText_DrawStringInWindowFar((u8 *)gUi_SlashString, window, 48, 48);
  value = unit->pp;
  UiText_DrawNumberRightAligned(value, window, 48, 48);
  value = unit->maxPp;
  UiText_DrawNumberRightAligned(value, window, 88, 48);
#else
  // Each row string ("HP    /" and "EP    /") holds the label and the slash,
  // one character to an 8-pixel cell, the slash in the cell at 48. Each
  // figure is four characters padded with spaces: the current one drawn
  // from 16 at the font's widths, the maximum from 56 a cell a character,
  // ending at 88.
  UiText_DrawStringInWindowFar((u8 *)gUi_HpRowString, window, 0, 40);
  value = unit->maxHp;
  UiText_DrawNumberInWindowFar(value, 4, window, 56, 40);
  value = unit->hp;
  if (value < unit->maxHp >> 2) { // below a quarter of maximum HP
    UiWork_SetParamNibbleFar(TEXT_COLOR_LOW_HP);
  }
  if (value == 0) {
    UiWork_SetParamNibbleFar(TEXT_COLOR_NO_HP);
  }
  UiText_DrawNumberAtOffsetFar(value, 4, window, 16, 40);
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  UiText_DrawStringInWindowFar((u8 *)gUi_PpRowString, window, 0, 48);
  value = unit->pp;
  UiText_DrawNumberAtOffsetFar(value, 4, window, 16, 48);
  value = unit->maxPp;
  UiText_DrawNumberInWindowFar(value, 4, window, 56, 48);
#endif
}

/// The stat arrows: the up and down arrow sprites the menu loads into its two
/// resource slots (Resource_LoadPairedBlocks).
enum StatArrow { STAT_ARROW_UP, STAT_ARROW_DOWN };

/// Writes Attack, Defense and Agility into a status window as they are now
/// (before), and beside each one that changes its new value (after) with an up
/// or down arrow: the equipment preview.
void UiText_DrawStatComparison(const struct BattleUnit *after,
                               const struct BattleUnit *before,
                               struct UiWindow *window) {
  UiText_DrawCharacterAtOffsetFar(MsgPanelStatLabel, window, 0, 32);
  UiText_DrawNumberAtOffsetFar(before->atk, 3, window, 16, 40);
  if (after->atk != before->atk) {
    UiText_DrawNumberAtOffsetFar(after->atk, 3, window, 64, 40);
    if (after->atk > before->atk) {
      UiIcon_CreateStatChangeArrow(window, 44, 36, STAT_ARROW_UP);
    } else {
      UiIcon_CreateStatChangeArrow(window, 44, 36, STAT_ARROW_DOWN);
    }
  }
  UiText_DrawCharacterAtOffsetFar(MsgPanelDefenseLabel, window, 0, 48);
  UiText_DrawNumberAtOffsetFar(before->def, 3, window, 16, 56);
  if (after->def != before->def) {
    UiText_DrawNumberAtOffsetFar(after->def, 3, window, 64, 56);
    if (after->def > before->def) {
      UiIcon_CreateStatChangeArrow(window, 44, 52, STAT_ARROW_UP);
    } else {
      UiIcon_CreateStatChangeArrow(window, 44, 52, STAT_ARROW_DOWN);
    }
  }
  UiText_DrawCharacterAtOffsetFar(MsgAgilityLabel, window, 0, 64);
  UiText_DrawNumberAtOffsetFar(before->agi, 3, window, 16, 72);
  if (after->agi != before->agi) {
    UiText_DrawNumberAtOffsetFar(after->agi, 3, window, 64, 72);
    if (after->agi > before->agi) {
      UiIcon_CreateStatChangeArrow(window, 44, 68, STAT_ARROW_UP);
    } else {
      UiIcon_CreateStatChangeArrow(window, 44, 68, STAT_ARROW_DOWN);
    }
  }
}

/// Size of one 16x16 4bpp sprite tile block.
#define ICON_TILE_BYTES 0x80

/// Creates a hand cursor sprite in window at (x, y), loading the hand's tiles
/// into a free VRAM block. Returns the sprite, or 0 when the free block found
/// is block 0 (see UiIcon_CreateWithResourceVariant).
/// Nothing calls it; UiIcon_CreateWithResourceVariant is the one in use.
UNUSED s32 UiIcon_CreateWithResource(struct UiWindow *window, s32 unused, s32 x,
                                     s32 y) {
  s32 block;
  s32 icon;
  icon = 0;
  block = Resource_FindFreeEntry();
  if (block != 0) {
    VramCache_Load(block, ICON_TILE_BYTES, gUiIcon_ResourceTiles);
    icon = (s32)RenderOutput_CreateFar(block, ICON_SPRITE_16X16, window, x, y);
  }
  return icon;
}

/// Creates the menu's hand cursor sprite in window at (x, y), loading the
/// hand's tiles into a free VRAM block. Returns the sprite, or 0 when the free
/// block found is block 0. Reached from UiMenu_CreateCursor, from
/// PsynergyMenu_CreateEntryGrid, which nothing calls, and through an overlay
/// veneer in MENU.S.
s32 UiIcon_CreateWithResourceVariant(struct UiWindow *window, s32 x, s32 y) {
  s32 block;
  s32 icon;

  icon = 0;
  block = Resource_FindFreeEntry();
  // XXX: likely meant as the no-free-block test, because 0 is a valid block:
  // Resource_FindFreeEntry returns VRAM_CACHE_ENTRY_COUNT (96) when none is
  // free, which ItemMenu_SelectGiveQuantity tests. With none free, the sprite
  // is created on block 96, which VramCache_Load refuses.
  if (block != 0) {
    VramCache_Load(block, ICON_TILE_BYTES, gUiIcon_ResourceTiles);
    icon = (s32)RenderOutput_CreateFar(block, ICON_SPRITE_16X16, window, x, y);
  }
  return icon;
}

/// Shows a menu icon at its stored position, without rotation or scaling.
void UiIcon_PrepareObject(struct RenderOutput *object) {
  if (object != NULL) {
    object->active = ICON_SHOWN;
    object->oam.x = object->x;
    object->oam.y = object->y;
    object->oam.matrix = 0;
    object->oam.affineMode = 0;
  }
}

/// Loads the graphics of character unit into menu entry slot 0. The calling
/// menu argument is not used.
void PsynergyMenu_CallIconRoutineWithValue(const void *unused, s32 unit) {
  Ui_LoadCharacterEntryForSlotFar(0, unit, 0);
}

/// Opens the menu's top-left window (13 by 5 tiles) and puts the hand cursor
/// in it, hidden for now, as paneIcon[0]. Selects no tab yet (the s8
/// tabIndex[0] = 255 reads as -1). Returns the window, which
/// ItemMenu_Initialize (the Item, Psynergy and Djinn screens) and
/// Menu_InitSelectorCursorAndEntries (the Status screen and the member
/// chooser) pass on to PsynergyMenu_InitializeEntryObjects for the party
/// figures.
struct UiWindow *UiMenu_CreateCursor(void *source) {
  struct PsynergyMenuState *work = source;
  struct UiWindow *window;
  s32 zero = 0;
  s32 sentinel;
  struct RenderOutput *cursor;

  work->auxiliaryWindow = NULL;
  UiWindow_UpdateOrCreate(&work->auxiliaryWindow, zero, zero, 13, 5, 2);
  window = work->auxiliaryWindow;
  cursor =
      (struct RenderOutput *)UiIcon_CreateWithResourceVariant(window, -8, 11);
  cursor->active = ICON_HIDDEN;
  work->tabIndex[0] = 255;
  work->tabIndex[1] = zero;
  work->paneIcon[0] = cursor;
  // sentinel is the depth slot an icon is drawn at: the hand at 254, with
  // the glyphs.
  sentinel = 254;
  cursor->sentinel = sentinel;
  sentinel -= 255;
  // paneIcon[1] gets slot 255. Nothing in TBS stores paneIcon[1], so it is
  // still NULL from UiWindow_InitializeWork and this byte goes to address 0xf,
  // in the BIOS, which ignores writes.
  work->paneIcon[1]->sentinel = sentinel; // -1
  return window;
}

/// Creates a walking figure for each active party member, side by side under
/// the window source: origin (originX, originY) in tiles from the window's
/// corner, each figure 16 + spacing pixels after the last, at full scale.
/// Clears the unused figure slots and starts the callback that places them
/// every frame. Every screen of the field menu calls it as it opens, and the
/// shops through PsynergyMenu_InitializeEntryObjectsFar;
/// Menu_ReleaseEntryObjects frees the figures.
void PsynergyMenu_InitializeEntryObjects(void *source, s32 originX, s32 originY,
                                         s32 spacing, s32 unused) {
  u16 ownerIds[14];
  struct PsynergyMenuState *menu = gMenuWork;
  struct UiWindow *window = source;
  s32 ownerCount = (u16)Party_ListActiveOwnersFar(ownerIds);
  s32 i;

  menu->tabCounts[0] = ownerCount;
  // Create and position the figures before scheduling their updates.
  for (i = 0; i < ownerCount; i++) {
    struct AnimationObject *figure =
        ResourceObject_CreateFar(Party_RemapCharacterIdByFlagsFar(ownerIds[i]));
    if (figure != 0) {
      s32 offsetX;
      s32 windowX;
      s32 tileX;

      menu->tabObjects[i] = figure;
      windowX = window->x;
      offsetX = spacing + 16;
      offsetX *= i;
      tileX = originX + windowX;
      menu->tabX[i] = tileX * 8 + offsetX;
      menu->rowPositions[i] = (originY + window->y) * 8 + 16;
      menu->ownerScale[i] = 0x10000; // 1.0 in 16.16
      figure->image.priority = 0;
      figure->flags = 0;
      AnimationObjects_SelectAnimationFar(figure, 1);
    }
  }
  // Clear the unused party slots so the callback can skip them.
  for (; i < 8; i++) {
    menu->tabObjects[i] = 0;
  }
  // 200 << 4 is 0xc80, SCHEDULER_KEY_MAIN_LOOP: once a frame in the main
  // loop, rank 0x80.
  {
    s32 priority = 200;
    priority <<= 4;
    Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, priority);
  }
}

/// Releases the party figures PsynergyMenu_InitializeEntryObjects created and
/// stops their placement callback.
void Menu_ReleaseEntryObjects(void) {
  u16 ownerIds[14];
  struct PsynergyMenuState *menu = gMenuWork;
  s32 count;
  struct AnimationObject **figures;
  s32 i;

  count = (u16)Party_ListActiveOwnersFar(ownerIds);
  if (count != 0) {
    figures = menu->tabObjects;
    i = count;
    do {
      struct AnimationObject *figure = *figures++;

      if (figure != 0) {
        ResourceObject_ReleaseFar(figure);
      }
    } while (--i != 0);
  }
  Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
}

/// Scheduler callback: each frame, places every party member sprite created by
/// PsynergyMenu_InitializeEntryObjects at its menu position and scale, through
/// the field's projected placement.
void Menu_UpdateEntryObjectTransforms(void) {
  struct PsynergyMenuState *menu = gMenuWork;
  struct SpriteScale scale;
  struct SpritePlacement position;
  s32 i;
  s32 count;

  count = (u16)Party_CountActiveOwnersFar();
  for (i = 0; i < count; i++) {
    struct AnimationObject *figure = menu->tabObjects[i];
    // 482 less the row's screen y, in 16.16. The ground y is always 482,
    // which puts every figure in the same depth slot ((482 >> 1) + 10 = 251,
    // Render_ApplyProjectedPlacement), and raising the image by this height
    // brings it back up to its row.
    s32 placeY = 0x01e20000 - (menu->rowPositions[i] << 16);

    if (figure != 0) {
      figure->image.priority = 0;
      scale.x = menu->ownerScale[i];
      scale.y = menu->ownerScale[i];
      position.x = menu->tabX[i] << 16;
      position.height = placeY;
      position.y = (menu->rowPositions[i] << 16) + placeY; // always 482
      position.groundHeight = 0;
      Object_ApplyProjectedPlacementFar(figure, &position, &scale, 0x4000);
    }
  }
}

/// The cursor bobs through an 8-step table, one step every two frames.
#define CURSOR_BOB_STEP() ((gFrameCount >> 1) & 7)

/// Places the hand cursor at (xOffset, yOffset) inside the menu's top-left
/// window, bobbing.
void UiMenu_PositionCursor(s32 xOffset, s32 yOffset) {
  struct PsynergyMenuState *work = gMenuWork;
  struct RenderOutput *cursor = work->paneIcon[0];

  cursor->oam.x = cursor->x = gUiMenu_CursorBobX[CURSOR_BOB_STEP()] + xOffset +
                              (work->auxiliaryWindow)->x * 8 + 8;
  cursor->oam.y = cursor->y = gUiMenu_CursorBobY[CURSOR_BOB_STEP()] + yOffset +
                              (work->auxiliaryWindow)->y * 8 + 8;
}

/// Slides the hand cursor from where it is to (x, y) inside the menu's
/// top-left window over two frames, in 1/16 pixel steps. Positions carry a
/// 64-pixel bias while moving so that they stay positive. Does nothing, once,
/// after skipSlide is set.
void UiMenu_SlideCursor(s32 x, s32 y) {

  struct PsynergyMenuState *work = gMenuWork;
  struct RenderOutput *cursor;
  s32 steps;
  s32 startX;
  s32 startY;
  s32 fineX;
  s32 fineY;
  s32 stepX;
  s32 stepY;

  steps = 2;
  if (work->skipSlide != 0) {
    work->skipSlide = 0;
    return;
  }
  cursor = work->paneIcon[0];
  {
    s32 biasedX = cursor->oam.x + 64;
    s32 biasedY = cursor->oam.y + 64;

    cursor->x = biasedX;
    cursor->y = biasedY;
  }
  x += 64;
  y += 64;
  // Take the 8-pixel window border off the start, which is in screen pixels,
  // so that it compares with the target inside the window. The window's own
  // place is not taken off: right for the top-left window at tile 0,0 that
  // UiMenu_CreateCursor opens.
  if (cursor->x - 8 > 0)
    cursor->x -= 8;
  fineY = cursor->y;
  if (fineY - 8 > 0) {
    cursor->y -= 8;
    fineY = cursor->y;
  }
  startX = cursor->x << 4;
  stepX = ((x << 4) - startX + 1) / steps;
  startY = fineY << 4;
  stepY = ((y << 4) - startY + 1) / steps;
  fineX = startX;
  fineY = startY;
  do {
    // 64 - 8 = 56: the bias less the window border.
    fineX += stepX;
    cursor->oam.x = cursor->x =
        (fineX >> 4) + ((work->auxiliaryWindow)->x << 3) - 56;
    fineY += stepY;
    cursor->oam.y = cursor->y =
        (fineY >> 4) + ((work->auxiliaryWindow)->y << 3) - 56;
    steps--;
    if (steps != 0)
      WaitFrames(1);
  } while (steps != 0);
}

/// Nothing calls it.
UNUSED void PsynergyMenu_ReservedNoOp(void) {}

/// Lays the entry icons out eight to a row from (108, 40). Nothing calls it.
UNUSED void PsynergyMenu_RefreshOwnerEntriesDefault(void) {
  PsynergyMenu_RefreshOwnerEntries(108, 40, 8);
}

/// Lays the menu's entry icons (up to 32) out in a grid of 16x16 cells,
/// columns wide, from (originX, originY); empty slots keep their place.
void PsynergyMenu_RefreshOwnerEntries(s32 originX, s32 originY, s32 columns) {
  struct RenderOutput **slot;
  struct RenderOutput **scan;
  s32 index;

  index = 0;
  slot = ((struct PsynergyMenuState *)gMenuWork)->entryIcons;
  scan = slot;
  do {
    if (*scan++ != NULL) {
      PsynergyMenu_PositionOwnerEntry(slot, index, originX, originY, columns);
    }
    index++;
    slot++;
  } while (index < 32);
}

/// Places the icon in *slot at grid cell index (0 above 31) of a grid of
/// 16x16 cells, columns wide, from (originX, originY), and shows it.
void PsynergyMenu_PositionOwnerEntry(struct RenderOutput **slot, s32 index,
                                     s32 originX, s32 originY, s32 columns) {
  struct RenderOutput *icon;

  if (index > 31) {
    index = 0;
  }
  icon = *slot;
  icon->y = (index / columns) * 16 + originY;
  icon->x = (index % columns) * 16 + originX;
  UiIcon_PrepareObject(icon);
}

/// Places the icon at slotIndex (0 above 15) in a columns-wide grid of
/// 24x16 cells starting at (originX, originY), and shows it.
void ItemMenu_PlaceIcon(struct RenderOutput **slot, s32 slotIndex, s32 originX,
                        s32 originY, s32 columns) {
  struct RenderOutput *icon;

  if (slotIndex > 15)
    slotIndex = 0;
  icon = *slot;
  icon->y = slotIndex / columns * 16 + originY;
  icon->x = slotIndex % columns * 24 + originX;
  UiIcon_PrepareObject(icon);
}

/// Lays the inventory's 15 item icons out five to a row from (116, y): y is
/// 56 for layout 1 and 40 otherwise.
void ItemMenu_RefreshEntry(s32 layout) {
  struct RenderOutput **slot;
  struct RenderOutput **scan;
  struct RenderOutput *icon;
  s32 index;
  s32 originY;
  struct InventoryMenuState *menu;

  menu = gMenuWork;
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
  } while (index < 15);
}

/// Shows message in the item menu, with the hand cursor hidden meanwhile.
///
/// With y == -1 the message goes into the info window; otherwise a modal
/// window sized to the message opens at (x, y). With x == -1 the message stays
/// up and FLAG_MENU_INFO_KEPT records that the info window holds it (the next
/// selection keeps it instead of drawing the item name); otherwise it waits
/// for A, B or Start and clears it again.
void InventoryMenu_ShowModalMessage(s32 message, s32 x, s32 y) {
  struct InventoryMenuState *menu = gMenuWork;
  struct UiWindow *window;
  s32 height;
  s32 width;
  s32 top;
  s32 left;

  menu->paneIcons[0]->active = ICON_HIDDEN;
  if (y != -1) {
    UiText_GetResourceDimensionsFar(message, &left, &top, &width, &height);
    if (UiWindow_UpdateOrCreate(&menu->modalWindow, x, y, width, height,
                                WINDOW_KEEP_EXISTING | 2) == 0)
      UiWindow_SetBounds(menu->modalWindow, x, y, width, height);
    window = menu->modalWindow;
  } else {
    window = menu->infoWindow;
  }
  RenderOutput_RedrawSavedRectFar(window);
  RenderOutput_ClearListFar((void *)window);
#if TBS_PATCH_1
  if (y == -1)
    UiText_DrawCharacterAtOffsetFar(message, window, 0, 0);
  else
    UiText_DrawResourceFar(message, window, 0, 0);
#else
  UiText_DrawResourceFar(message, window, 0, 0);
#endif
  if (x != -1) {
    WaitFrames(1);
    do {
      WaitFrames(1);
    } while (!(gKeysPressed & KEY_A) && !(gKeysPressed & KEY_B) &&
             !(gKeysPressed & KEY_START));
#if TBS_PATCH_1
    if (y == -1)
      RenderOutput_RedrawSavedRectFar(window);
#endif
    RenderOutput_ClearListFar((void *)window);
  } else {
    GameFlag_SetBitFar(FLAG_MENU_INFO_KEPT);
  }
  menu->completionFlag = 1;
  menu->paneIcons[0]->active = ICON_SHOWN;
#if TBS_PATCH_1
  if (y != -1)
    UiWindow_CloseIfOpen(&menu->modalWindow, 1);
#endif
}

/// Sorts a party member's 15 inventory slots by the list order of mode (see
/// Menu_CopyListOrder): items are grouped by the order's item types and, within
/// a type, run from the highest item id down. Empty slots go to the end.
/// L + A in the item menu cycles through the modes. Returns 1.
s32 InventoryMenu_SortByListOrder(u16 *inventory, s32 mode) {
  u16 sorted[16];
  u16 items[16];
  u8 order[32];
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

  for (i = 0; i < 15; i++)
    items[i] = inventory[i];

  count = 0;
  for (i = 0; i < 15; i++) {
    if (items[i] != 0)
      count++;
  }

  for (i = count; i < 15; i++)
    sorted[i] = 0;

  // One pass for each entry of the order: move its items across, the highest
  // id first, until none is left.
  pass = 0;
  while (order[pass] != LIST_ORDER_END) {
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
        value = items[i] & ITEM_ID_MASK;
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
    pass++;
  }

  for (i = 0; i < count; i++)
    inventory[i] = sorted[i];
  return 1;
}

/// Copies the list order of mode 0 to 2 (any other mode: the default order)
/// into order, up to and including its LIST_ORDER_END and at most 32 bytes.
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
  if (*order != LIST_ORDER_END) {
    do {
      if (++n > 31)
        break;
      src++;
      order++;
      *order = *src;
    } while (*order != LIST_ORDER_END);
  }
}

/// The page indicator: the window font's digit tiles for pages 1, 2, ...,
/// between the two page arrows.
#define PAGE_LABEL_FIRST 49

/// Moves a paged list's cursor and page with the d-pad's repeating keys.
///
/// count entries are shown pageSize at a time; *cursor is the row on the page
/// and *page the page. Vertical lists (horizontal 0) move the cursor with Up
/// and Down and turn pages with Left and Right; horizontal lists the other way
/// round. Moving past either end wraps round; turning to a shorter page pulls
/// the cursor up to its last entry. Returns 1 after a page turn, 0 after a
/// cursor move and -1 when nothing moved or the list is empty.
s32 Menu_HandlePageInput(s32 horizontal, s32 count, s32 pageSize, s32 *cursor,
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
  Link_DrawShiftedTilePairFar(UI_PAGE_ARROW_VRAM);
  pages = count / pageSize;
  if (count % pageSize != 0)
    pages++;
  // horizontal is reused for the page-back key.
  if (horizontal) {
    next = gKeysRepeat & KEY_RIGHT;
    previous = gKeysRepeat & KEY_LEFT;
    horizontal = gKeysRepeat & KEY_UP;
    pageForward = gKeysRepeat & KEY_DOWN;
  } else {
    next = gKeysRepeat & KEY_DOWN;
    previous = gKeysRepeat & KEY_UP;
    horizontal = gKeysRepeat & KEY_LEFT;
    pageForward = gKeysRepeat & KEY_RIGHT;
  }
  if (horizontal) {
    AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
    if (--*page < 0)
      *page = pages - 1;
    // Past the end of a shorter last page: onto its last entry.
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
    AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
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
    AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
    // Back past the first row wraps to the page's last entry: its last row,
    // or the list's last entry on a shorter last page. The first store is
    // overwritten at once.
    if (--*cursor < 0) {
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
  // Forward past the list's last entry or the page's last row wraps to the
  // first row.
  {
    AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
    result = 0;
    if (++*cursor == count - *page * pageSize)
      *cursor = result;
    if (*cursor > pageSize - 1)
      *cursor = result;
  }
  return 0;
}

/// Copies the last background palette (15) into background palette bank and
/// lightens its colour 4 by 9 steps in each channel: the highlight colour of
/// the menu's selected rows. Each screen sets up bank 14 as it opens, and again
/// after the Details window has borrowed it (Menu_SelectQuantity); only
/// Item_PrepareUsePalette, which nothing calls, uses bank 13.
void Palette_LightenBankHighlight(s32 bank) {
  u16 *palette = (u16 *)&BG_PLTT_COLOR(bank, 0);
  u32 red;
  u32 green;
  u32 blue;

  // Both transfers copy the same 32 bytes, first as 16 halfwords and then as
  // 8 words.
  Dma_Set((const void *)&BG_PLTT_COLOR(15, 0), palette, DMA_ENABLE << 16 | 16,
          REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(15, 0), palette,
          (DMA_ENABLE | DMA_32BIT) << 16 | 8, REG_DMA3);
  // BGR555: five bits a channel, 0 to 31, red lowest.
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
  palette[4] = (blue << 10) | (green << 5) | red;
}

/// Draws a paged list's page numbers on the top border of window, ending at
/// tile rightEdge: one digit per page between the page arrows, selectedPage in
/// palette 14 and the others in palette 15. Draws nothing for a single page.
void Menu_DrawPageIndicator(struct UiWindow *window, s32 itemCount,
                            s32 pageSize, s32 selectedPage, s32 rightEdge) {
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
    UiWindow_SetTilemapEntryFar(window, UI_PAGE_ARROW_LEFT, x - 1, -1, 0);

    for (page = 0; page < pageCount; page++) {
      if (page == selectedPage)
        UiWindow_SetTilemapEntryFar(window, tile, x, -1, 2); // palette 14
      else
        UiWindow_SetTilemapEntryFar(window, tile, x, -1, 3); // palette 15
      tile++;
      x++;
    }

    UiWindow_SetTilemapEntryFar(window, UI_PAGE_ARROW_RIGHT, x, -1, 0);
  }
}

/// Switches a rectangle of a window's text between palette 15 (normal) and 14
/// (the highlight made by Palette_LightenBankHighlight): palette is the new
/// palette number. Only bit 12 of each map entry is cleared before it is set,
/// so this works between those two palettes. (x, y) and the size are in tiles
/// inside the window's border; the rectangle is clipped to the screen's 30
/// columns, and to its 20 rows only when it reaches row 29 (see the body).
/// Used for the highlighted rows of the item, Psynergy and Status lists.
/// Marks the whole tilemap for upload.
void Render_SetTilemapFlagRect(const struct UiWindow *window, s32 x, s32 y,
                               s32 width, s32 height, u32 palette) {
  struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
  u8 *tilemap = (u8 *)work->tilemap;

  x += window->x + 1;
  y += window->y + 1;
  palette <<= 12;
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
  // The test is the columns' (past 29) but the clip is to the screen's 20
  // rows, so a rectangle ending in rows 20 to 28 would be kept whole and
  // write past the 32 by 20 tilemap into the windows after it. No caller
  // reaches row 20: the lowest is row 18, in the Status menu's equipment
  // window.
  if (y + height > 29) {
    height = 20 - y;
  }
  if (width > 0 && height > 0) {
    // Byte offset of the first entry: 32 two-byte entries a row.
    y <<= 6;
    x = y + (x << 1);
    do {
      u16 *entry = (u16 *)(x + (u32)tilemap);
      s32 remaining = width;
      while (remaining != 0) {
        u32 value = *entry;
        value &= 0xFFFFEFFF;
        value |= palette;
        remaining--;
        *entry = value;
        entry++;
      }
      height--;
      x += 64;
    } while (height != 0);
    work->dirty = 1;
  }
}

/// Copies the first sprite palette into background palette 14, then colour 4
/// of background palette 15, the window's row colour, over its colour 14 (as
/// StatusMenu_Run does for palettes 0 and 1). The Details window draws its
/// element icons (window tiles 1 to 4) in palette 14, so Menu_SelectQuantity
/// calls this first, as do ItemMenu_OpenDetail and Item_PrepareUsePalette,
/// which nothing calls. It replaces the row highlight until
/// Palette_LightenBankHighlight(14) makes it again.
void Palette_CopyObjectBankToBackground14(void) {
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(14, 0),
          DMA_ENABLE << 16 | 16, REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(15, 4), (void *)&BG_PLTT_COLOR(14, 14),
          DMA_ENABLE << 16 | 1, REG_DMA3);
}

/// Shows the icons of one page of a list, one row of 16 pixels each from
/// (x, y): entries first to first + pageSize - 1, stopping early at an empty
/// icon slot or the work's itemCount. Hides all the others. Used by the item
/// and Psynergy lists and the Status screen's lists.
void Menu_SetPageIcons(s32 pageSize, s32 first, struct UiWindow *window, s32 x,
                       s32 y) {
  struct InventoryMenuState *menu = gMenuWork;
  struct RenderOutput *icon;
  s32 i;

  for (i = 0; i < 32; i++) {
    icon = menu->entryIcons[i];
    if (icon != NULL)
      icon->active = ICON_HIDDEN;
  }
  for (i = first;
       i < pageSize + first && (icon = menu->entryIcons[i]) != NULL &&
       i <= menu->itemCount - 1;
       i++) {
    icon->x = x;
    icon->y = y + (i - first) * 16;
    UiIcon_PrepareObject(icon);
    icon->active = ICON_SHOWN;
  }
}

/// Writes the party's coins and the "Coins" label into window: the info
/// window of the Item and Psynergy screens' member choice.
void UiText_DrawWorkValueWithLabel(struct UiWindow *window) {
  UiText_DrawNumberAtOffsetFar(gGameState.coins, 7, window, 8, 0);
  UiText_DrawCharacterAtOffsetFar(MsgCoinsLabel, window, 64, 0);
  // Only the Japanese editions draw a vertical divider at tile column 12.
#if !TBS_PATCH_1
  UiWindow_DrawDividerLineFar(window, 12, 0, 12, 2);
#endif
}

/// Moves and resizes a window, in tiles.
void UiWindow_SetBounds(struct UiWindow *window, s32 x, s32 y, s32 width,
                        s32 height) {
  if (window != NULL) {
    window->width = width;
    window->x = x;
    window->height = height;
    window->y = y;
  }
}

/// Holds back the window tilemap's upload (menuBusy), so that what a menu
/// draws meanwhile appears at once when UiWork_ClearMenuBusy lets it go.
/// Nothing calls it: the screens set menuBusy themselves.
void UiWork_SetMenuBusy(void) {
  ((struct UiRenderWork *)gWindowWork[0])->menuBusy = 1;
}

/// Lets the window tilemap be uploaded again after UiWork_SetMenuBusy.
/// Nothing calls it.
void UiWork_ClearMenuBusy(void) {
  ((struct UiRenderWork *)gWindowWork[0])->menuBusy = 0;
}

/// Plays cue (a sound or a song) and returns 1. Called by
/// Ability_PlayUseAnimation, which ignores the value.
s32 Audio_PlayCueReturnOne(s32 cue) {
  AudioCommand_PlayFar(cue);
  return 1;
}

/// Scheduler callback while a menu runs: on Start, plays the cancel sound,
/// sets FLAG_MENU_START_PRESSED and removes itself.
void Menu_CancelSoundTick(void) {
  if (gKeysPressed & KEY_START) {
    AudioCommand_PlayFar(SE_MENU_CANCEL);
    GameFlag_SetBitFar(FLAG_MENU_START_PRESSED);
    Scheduler_RemoveCallback(Menu_CancelSoundTick);
  }
}

/// Starts watching for Start: clears FLAG_MENU_START_PRESSED and adds
/// Menu_CancelSoundTick. ItemMenu_Run, PsynergyMenu_Run and StatusMenu_Run
/// call it before their command loops and Menu_EnsureCancelSound after them.
void Menu_CancelSoundReset(void) {
  GameFlag_ClearBitFar(FLAG_MENU_START_PRESSED);
  // SCHEDULER_KEY_MAIN_LOOP: once a frame in the main loop.
  Scheduler_AddOrUpdateCallback(Menu_CancelSoundTick, 0xC80);
}

/// Stops watching for Start, if Start has not removed the watch already.
void Menu_EnsureCancelSound(void) {
  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    Scheduler_RemoveCallback(Menu_CancelSoundTick);
  }
}

/// Makes the text drawn after it colour 15 (TEXT_COLOR_NORMAL). Nothing calls
/// it or the two after it; the status panel sets its colours with
/// UiWork_SetParamNibbleFar.
void UiPalette_SetColor15(void) { UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL); }

/// Makes the text drawn after it colour 2 (TEXT_COLOR_NO_HP).
void UiPalette_SetColor2(void) { UiWork_SetParamNibbleFar(TEXT_COLOR_NO_HP); }

/// Makes the text drawn after it colour 4 (TEXT_COLOR_LOW_HP).
void UiPalette_SetColor4(void) { UiWork_SetParamNibbleFar(TEXT_COLOR_LOW_HP); }

#undef STATUS_PANEL_TEXT_X
#undef STATUS_PANEL_CANNOT_EQUIP_X
#undef STATUS_PANEL_LEARN_NAME_Y
#undef STATUS_PANEL_LEARN_STATUS_Y
#undef WINDOW_KEEP_EXISTING
#undef WINDOW_STYLE_MASK
#undef NUMBER_PIXELS_PER_DIGIT
#undef ICON_TILE_BYTES
#undef CURSOR_BOB_STEP
#undef PAGE_LABEL_FIRST
