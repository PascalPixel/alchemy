/// @file
/// The pieces the menu screens share: windows, the status panel, numbers,
/// icons, the party figures, the hand cursor, paged lists, palettes and the
/// Start watch.
// Part of menu.c.

#include "../menu_internal.h"

// The status panel's name and status column starts further left in the
// international editions, whose status labels are longer, and so does
// "Can't equip" (MsgCannotEquip).
#if LANGUAGE != LANGUAGE_JAPANESE
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

/// Window_UpdateOrCreate: keep an existing window as it is, without redrawing
/// its saved rectangle. The low byte of the flags is the window style.
#define WINDOW_KEEP_EXISTING 0x100
#define WINDOW_STYLE_MASK 0xff

/// Resumes the overlay tasks and tileset animations when a menu closes, and
/// clears FLAG_WEATHER_HELD and FLAG_PALETTE_BLEND_HELD: undoes
/// Menu_SuspendFieldTasks. Called as DjinnMenu_Run and StatusScreen_Run close,
/// and as ItemMenu_Run (TBS_PATCH_1) and PsynergyMenu_Run (TBS_PATCH_2) close.
/// Returns nothing useful; callers ignore the value.
s32 Menu_ResumeFieldTasks(void) {
  Task_ResumeOverlayTasks();
  TilesetAnim_ResumeAllFar();
  // FLAG_WEATHER_HELD freezes the field weather: its particles hold
  // their frame and the storm's lightning waits (event_commands/weather.c).
  GameFlag_ClearFar(FLAG_WEATHER_HELD);
  // FLAG_PALETTE_BLEND_HELD stops PaletteFade_Update.
  GameFlag_ClearFar(FLAG_PALETTE_BLEND_HELD);
#ifdef UBFIX
  return 0;
#endif
}

/// Suspends the overlay tasks and tileset animations while a menu is open, and
/// sets FLAG_PALETTE_BLEND_HELD and FLAG_WEATHER_HELD, so that the field's
/// palette blend and weather stand still behind it (see Menu_ResumeFieldTasks,
/// which undoes it). Returns Task_SuspendOverlayTasks' result, the last
/// task index changed or -1; callers ignore it. The Djinn and Status screens
/// call it in every edition, the Item screen from TBS_PATCH_1 (the
/// international editions) and the Psynergy screen from TBS_PATCH_2 (German,
/// French, Spanish and Italian).
s32 Menu_SuspendFieldTasks(void) {
  GameFlag_SetFar(FLAG_PALETTE_BLEND_HELD);
  GameFlag_SetFar(FLAG_WEATHER_HELD);
  TilesetAnim_SuspendAllFar();
  return Task_SuspendOverlayTasks();
}

/// Clears the whole menu work and sets its defaults: no side selected, one
/// entry in each side, and a 1x1 cursor grid. Each screen's run function calls
/// it once the menu work's heap block is allocated, so every window handle and
/// icon pointer in the work starts as NULL.
void Window_InitializeWork(s32 unused) {
  struct ItemMenuWork *menu = gMenuWork;

  Dma_Fill32(0, menu, MENU_WORK_SIZE);
  menu->sideIndex[0] = -1;
  menu->sideCount[0] = 1;
  menu->sideCount[1] = 1;
  menu->columnCount = 1;
  menu->rowCount = 1;
}

/// Opens the window kept in *handle, or redraws it if it is already open.
///
/// If *handle is empty, creates a window at (x, y), w by h tiles, in the style
/// given by the low byte of flags, stores it in *handle and returns 1. If the
/// window exists, redraws its saved rectangle (unless flags has
/// WINDOW_KEEP_EXISTING) and returns 0.
s32 Window_UpdateOrCreate(struct Window **handle, s32 x, s32 y, s32 w, s32 h,
                          s32 flags) {
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

/// Closes the window kept in *handle, if any, and clears the handle. style is
/// passed on to Window_CloseFar and chooses how the window closes.
void Window_CloseIfOpen(struct Window **handle, s32 style) {
  if (*handle != NULL) {
    Window_CloseFar(*handle, style);
    *handle = NULL;
  }
}

/// Copies one Unit through _call_via_r3 to the resident IWRAM word copy, where
/// menu/djinn.c's Unit_Copy calls Iwram_CopyWords.
static inline void StatusPanel_CopyUnit(struct Unit *destination,
                                        struct Unit *source) {
  _call_via_r3(destination, source, sizeof(struct Unit), IwramCopyWords);
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
void StatusPanel_Draw(struct Window *unused, s32 member, s32 slot, s32 mode) {
  s32 created;
  struct ItemDefinition *def;
  u32 item;
  struct ItemMenuWork *menu;
  struct Unit *unit;
  struct Window *window;
  s32 ailmentRows;
  s32 value;
  u8 ailments[8];

  created = 0;
  menu = gMenuWork;
  unit = Unit_GetFar(member);
  item = unit->inventory[slot];
  def = Item_GetDirectFar(item & INVENTORY_ITEM_ID_MASK);
  if (!(mode & STATUS_PANEL_LOWER_ONLY))
    created = Window_UpdateOrCreate(&menu->statusWindow, 0, 5, 13, 12,
                                    WINDOW_KEEP_EXISTING | 2);
  window = menu->statusWindow;

  // The name, then each status the party member has, one row each; the level
  // takes the third row when no more than one status is shown.
  if (!(mode & STATUS_PANEL_LOWER_ONLY)) {
    // A window that already existed still shows the last party member: clear
    // its top 88 by 32 pixels a frame later. A new window is blank.
    if (!created) {
      WaitFrames(1);
      Window_ClearInteriorTilesFar(menu->statusWindow, 0, 0, 88, 32);
    }
    Text_DrawStringAtOffsetFar(unit->name, window, STATUS_PANEL_TEXT_X, 0);
    StatusScreen_BuildAvailability(ailments, 1, member);
    ailmentRows = 0;
    if (ailments[CHARACTER_POISON]) {
      Text_DrawMessageAsTilesFar(MsgPoisonLabel, window, STATUS_PANEL_TEXT_X,
                                 ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[CHARACTER_VENOM]) {
      Text_DrawMessageAsTilesFar(MsgVenomLabel, window, STATUS_PANEL_TEXT_X,
                                 ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[CHARACTER_CURSE]) {
      Text_DrawMessageAsTilesFar(MsgCurseLabel, window, STATUS_PANEL_TEXT_X,
                                 ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[CHARACTER_HAUNT]) {
      Text_DrawMessageAsTilesFar(MsgHauntLabel, window, STATUS_PANEL_TEXT_X,
                                 ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailmentRows < 2) {
      value = unit->level;
      MENU_DRAW_STATUS_LABEL(gStatusPanel_LevelString, window, 40, 16);
      Text_DrawNumberInWindowFar(value, 4, window, 56, 16);
    }
  }

  // Clear the lower part and redraw the party member's portrait.
  if (!created) {
    WaitFrames(1);
    Window_ClearInteriorTilesFar(menu->statusWindow, 0, 32, 88, 80);
  }
  WindowSprite_ClearListFar(window);
  if (!(mode & STATUS_PANEL_LOWER_ONLY))
    menu->cursor = Portrait_CreateFar(member, 0, 0, window, 0, 0);

  switch (mode & STATUS_PANEL_KIND_MASK) {
  case STATUS_PANEL_EXPERIENCE:
    value = unit->classId + MsgClassName;
    Text_DrawMessageAsTilesFar(value, window, 0, 32);
    StatusPanel_DrawHpPp(unit, window);
    value = unit->experience;
    Text_DrawMessageAsTilesFar(MsgExpLabel, window, 0, 64);
    Text_DrawNumberInWindowFar(value, 8, window, 24, 72);
    break;
  case STATUS_PANEL_CLASS:
    value = unit->classId + MsgClassName;
    Text_DrawMessageAsTilesFar(value, window, 0, 32);
    StatusPanel_DrawHpPp(unit, window);
    break;
  case STATUS_PANEL_EQUIP:
  case STATUS_PANEL_EQUIP + 1: { // draws the same panel
    struct Unit *backup;

    if (!Item_CanUnitEquip(member, item)) {
      Text_DrawMessageAsTilesFar(MsgCannotEquip, window,
                                 STATUS_PANEL_CANNOT_EQUIP_X, 48);
      break;
    }
    // Equip (or, previewing a removal, unequip) the item on the real unit,
    // recalculate its stats, show them against the saved copy, then put the
    // unit back as it was.
    backup = Heap_Alloc(sizeof(struct Unit));
    StatusPanel_CopyUnit(backup, unit);
    if (menu->equipPreview)
      unit->inventory[slot] &= ~INVENTORY_EQUIPPED;
    else
      Inventory_EquipFar(member, slot);
    Unit_CalculateFar(member);
    Text_DrawStatComparison(unit, backup, window);
    StatusPanel_CopyUnit(unit, backup);
    Heap_Release(backup);
    break;
  }
  case STATUS_PANEL_LEARN: {
    s32 psynergy = def->abilityId;
    s32 known = 0;
    s32 i;

    // Whether the party member already knows it, from any of the 32 action
    // slots.
    for (i = 0; i < 32; i++) {
      if ((unit->psynergy[i].encodedAction & PSYNERGY_ABILITY_MASK) ==
          psynergy) {
        known = 1;
        break;
      }
    }
    // The Psynergy's name and "Learned" or "Will learn"; Spanish puts the
    // label above the name.
    if (known) {
      Text_DrawMessageAsTilesFar(psynergy + MsgAbilityName, window, 0,
                                 STATUS_PANEL_LEARN_NAME_Y);
      Text_DrawMessageAsTilesFar(MsgLearned, window, 0,
                                 STATUS_PANEL_LEARN_STATUS_Y);
    } else {
      Text_DrawMessageAsTilesFar(psynergy + MsgAbilityName, window, 0,
                                 STATUS_PANEL_LEARN_NAME_Y);
      Text_DrawMessageAsTilesFar(MsgWillLearn, window, 0,
                                 STATUS_PANEL_LEARN_STATUS_Y);
    }
    break;
  }
  case STATUS_PANEL_STATS: {
    // The labels are consecutive messages: Attack, Defense, Status, Luck and
    // Agility.
    s32 labels = MsgPanelStatLabel;

    Text_DrawMessageAsTilesFar(labels, window, 0, 40); // "Attack"
    value = unit->atk;
    Text_DrawNumberInWindowFar(value, 3, window, 64, 40);
    Text_DrawMessageAsTilesFar(labels + 1, window, 0, 48); // "Defense"
    value = unit->def;
    Text_DrawNumberInWindowFar(value, 3, window, 64, 48);
    Text_DrawMessageAsTilesFar(labels + 4, window, 0, 56); // "Agility"
    value = unit->agi;
    Text_DrawNumberInWindowFar(value, 3, window, 64, 56);
    Text_DrawMessageAsTilesFar(labels + 3, window, 0, 64); // "Luck"
    value = unit->luck;
    Text_DrawNumberInWindowFar(value, 3, window, 64, 64);
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
/// x:      the field's right edge, in pixels within the window
/// y:      the pixel row of the text, within the window
void Text_DrawNumberRightAligned(s32 number, struct Window *window, s32 x,
                                 s32 y) {
  s32 width = CountDigitsAtLeastTwo(number);

  // Step x back to the field's left edge; the number writer right-aligns the
  // number in a field of width characters. x changes in place: a separate
  // left-edge local changes the registers.
  x -= width * NUMBER_PIXELS_PER_DIGIT;
  Text_DrawNumberAtOffsetFar(number, width, window, x, y);
}

#endif

/// Draws a character's HP and PP into window as two rows, "HP current/maximum"
/// at pixel row 40 and "PP current/maximum" at row 48. Current HP is drawn in
/// TEXT_COLOR_LOW_HP when it is below a quarter of maximum HP and in
/// TEXT_COLOR_NO_HP at 0; then the colour goes back to TEXT_COLOR_NORMAL.
void StatusPanel_DrawHpPp(struct Unit *unit, struct Window *window) {
  // Each figure in turn, as an s16. Read straight from unit, the figures
  // compile differently.
  s16 value;

#if LANGUAGE != LANGUAGE_JAPANESE
  // The label at the left and a slash at 48. A current figure ends before
  // the slash, at 48, and a maximum at 88, each in a field as wide as its
  // digits.
  Text_DrawStringAtOffsetFar(gStatusPanel_HpString, window, 0, 40);
  Text_DrawStringInWindowFar((u8 *)gStatusPanel_SlashString, window, 48, 40);
  value = unit->hpMax;
  Text_DrawNumberRightAligned(value, window, 88, 40);
  value = unit->hp;
  if (value < unit->hpMax >> 2) { // below a quarter of maximum HP
    Text_SetColorFar(TEXT_COLOR_LOW_HP);
  }
  if (value == 0) {
    Text_SetColorFar(TEXT_COLOR_NO_HP);
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
    Text_SetColorFar(TEXT_COLOR_LOW_HP);
  }
  if (value == 0) {
    Text_SetColorFar(TEXT_COLOR_NO_HP);
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

/// The stat arrows: the up and down arrow sprites the menu loads into its two
/// sprite picture cache slots (Resource_LoadPairedBlocks).
enum StatArrow { STAT_ARROW_UP, STAT_ARROW_DOWN };

/// Writes Attack, Defense and Agility into a status window as they are now
/// (before), and beside each one that changes its new value (after) with an up
/// or down arrow: the equipment preview.
void Text_DrawStatComparison(const struct Unit *after,
                             const struct Unit *before, struct Window *window) {
  Text_DrawMessageAsTilesFar(MsgPanelStatLabel, window, 0, 32);
  Text_DrawNumberAtOffsetFar(before->atk, 3, window, 16, 40);
  if (after->atk != before->atk) {
    Text_DrawNumberAtOffsetFar(after->atk, 3, window, 64, 40);
    if (after->atk > before->atk) {
      Icon_CreateStatChangeArrow(window, 44, 36, STAT_ARROW_UP);
    } else {
      Icon_CreateStatChangeArrow(window, 44, 36, STAT_ARROW_DOWN);
    }
  }
  Text_DrawMessageAsTilesFar(MsgPanelDefenseLabel, window, 0, 48);
  Text_DrawNumberAtOffsetFar(before->def, 3, window, 16, 56);
  if (after->def != before->def) {
    Text_DrawNumberAtOffsetFar(after->def, 3, window, 64, 56);
    if (after->def > before->def) {
      Icon_CreateStatChangeArrow(window, 44, 52, STAT_ARROW_UP);
    } else {
      Icon_CreateStatChangeArrow(window, 44, 52, STAT_ARROW_DOWN);
    }
  }
  Text_DrawMessageAsTilesFar(MsgAgilityLabel, window, 0, 64);
  Text_DrawNumberAtOffsetFar(before->agi, 3, window, 16, 72);
  if (after->agi != before->agi) {
    Text_DrawNumberAtOffsetFar(after->agi, 3, window, 64, 72);
    if (after->agi > before->agi) {
      Icon_CreateStatChangeArrow(window, 44, 68, STAT_ARROW_UP);
    } else {
      Icon_CreateStatChangeArrow(window, 44, 68, STAT_ARROW_DOWN);
    }
  }
}

/// Size of one 16x16 4bpp sprite tile block.
#define ICON_TILE_BYTES 0x80

/// Creates a hand cursor sprite in window at (x, y), loading the hand's tiles
/// into a free VRAM block. Returns the sprite, or 0 when the free block found
/// is block 0 (see Icon_CreateWithResourceVariant).
/// Nothing calls it; Icon_CreateWithResourceVariant is the one in use.
UNUSED s32 Icon_CreateWithResource(struct Window *window, s32 unused, s32 x,
                                   s32 y) {
  s32 block;
  s32 icon;
  icon = 0;
  block = VramCache_FindFreeEntry();
  if (block != 0) {
    VramCache_Load(block, ICON_TILE_BYTES, gIcon_ResourceTiles);
    icon = (s32)WindowSprite_CreateFar(block, OUTPUT_SIZE_16X16, window, x, y);
  }
  return icon;
}

/// Creates the menu's hand cursor sprite in window at (x, y), loading the
/// hand's tiles into a free VRAM block. Returns the sprite, or 0 when the free
/// block found is block 0. Reached from MenuCursor_Create, from
/// PsynergyMenu_CreateEntryGrid, which nothing calls, and through an overlay
/// veneer in MENU.S.
s32 Icon_CreateWithResourceVariant(struct Window *window, s32 x, s32 y) {
  s32 block;
  s32 icon;

  icon = 0;
  block = VramCache_FindFreeEntry();
  // BUG: block 0 is a valid block, and VramCache_FindFreeEntry says none is
  // free with VRAM_CACHE_ENTRY_COUNT (96), which ItemMenu_SelectGiveQuantity
  // tests. With none free, the sprite is created on block 96, which
  // VramCache_Load refuses; with block 0 free, no hand is made.
#ifdef BUGFIX
  if (block != VRAM_CACHE_ENTRY_COUNT) {
#else
  if (block != 0) {
#endif
    VramCache_Load(block, ICON_TILE_BYTES, gIcon_ResourceTiles);
    icon = (s32)WindowSprite_CreateFar(block, OUTPUT_SIZE_16X16, window, x, y);
  }
  return icon;
}

/// Shows a menu icon at its stored position, without rotation or scaling.
void Icon_PrepareActor(struct WindowSprite *object) {
  if (object != NULL) {
    object->motion = ICON_SHOWN;
    object->oam.x = object->x;
    object->oam.y = object->y;
    object->oam.matrix = 0;
    object->oam.affineMode = 0;
  }
}

/// Loads the graphics of character unit into menu entry slot 0. The calling
/// menu argument is not used.
void PsynergyMenu_CallIconRoutineWithValue(const void *unused, s32 unit) {
  Portrait_ReloadFar(0, unit, 0);
}

/// Opens the menu's top-left window (13 by 5 tiles) and puts the hand cursor
/// in it, hidden for now, as sideIcon[0]. Selects no tab yet (the s8
/// tabIndex[0] = 255 reads as -1). Returns the window, which
/// ItemMenu_Initialize (the Item, Psynergy and Djinn screens) and
/// Menu_InitializeSelectorCursorAndEntries (the Status screen and the party
/// member chooser) pass on to PsynergyMenu_InitializeEntryActors for the party
/// figures.
struct Window *MenuCursor_Create(void *source) {
  struct PsynergyMenuWork *menu = source;
  struct Window *window;
  s32 zero = 0;
  s32 sentinel;
  struct WindowSprite *cursor;

  menu->auxiliaryWindow = NULL;
  Window_UpdateOrCreate(&menu->auxiliaryWindow, zero, zero, 13, 5, 2);
  window = menu->auxiliaryWindow;
  cursor =
      (struct WindowSprite *)Icon_CreateWithResourceVariant(window, -8, 11);
  cursor->motion = ICON_HIDDEN;
  menu->tabIndex[0] = 255;
  menu->tabIndex[1] = zero;
  menu->sideIcon[0] = cursor;
  // sentinel is the depth slot an icon is drawn at: the hand at 254, with
  // the glyphs.
  sentinel = 254;
  cursor->sentinel = sentinel;
  sentinel -= 255;
  // sideIcon[1] gets slot 255. Nothing in TBS stores sideIcon[1], so it is
  // still NULL from Window_InitializeWork and this byte goes to address 0xf,
  // in the BIOS, which ignores writes.
  menu->sideIcon[1]->sentinel = sentinel; // -1
  return window;
}

/// Creates a walking figure for each active party member, side by side under
/// the window source: origin (originX, originY) in tiles from the window's
/// corner, each figure 16 + spacing pixels after the last, at full scale.
/// Clears the unused figure slots and starts the task that places them
/// every frame. Every screen of the field menu calls it as it opens, and the
/// shops through PsynergyMenu_InitializeEntryActorsFar;
/// Menu_ReleaseEntryActors releases the figures.
void PsynergyMenu_InitializeEntryActors(void *source, s32 originX, s32 originY,
                                        s32 spacing, s32 unused) {
  u16 memberIds[14];
  struct PsynergyMenuWork *menu = gMenuWork;
  struct Window *window = source;
  s32 memberCount = (u16)Party_ListMembersFar(memberIds);
  s32 i;

  menu->tabCounts[0] = memberCount;
  // Create and position the figures before scheduling their updates.
  for (i = 0; i < memberCount; i++) {
    struct Sprite *figure =
        Sprite_CreateFar(Party_RemapCharacterIdByFlagsFar(memberIds[i]));
    if (figure != 0) {
      s32 offsetX;
      s32 windowX;
      s32 tileX;

      menu->tabSprites[i] = figure;
      windowX = window->x;
      offsetX = spacing + 16;
      offsetX *= i;
      tileX = originX + windowX;
      menu->tabX[i] = tileX * 8 + offsetX;
      menu->rowPositions[i] = (originY + window->y) * 8 + 16;
      menu->memberScale[i] = Q16_ONE;
      figure->image.priority = 0;
      figure->flags = 0;
      Sprite_StartAnimationFar(figure, 1);
    }
  }
  // Clear the unused party slots so the task can skip them.
  for (; i < 8; i++) {
    menu->tabSprites[i] = 0;
  }
  // 200 << 4 is 0xc80, TASK_PASS_FRAME: once a frame in the frame
  // pass, rank 0x80.
  {
    s32 priority = 200;
    priority <<= 4;
    Task_Add(Menu_UpdateEntryActorTransforms, priority);
  }
}

/// Releases the party figures PsynergyMenu_InitializeEntryActors created and
/// stops their placement task.
void Menu_ReleaseEntryActors(void) {
  u16 memberIds[14];
  struct PsynergyMenuWork *menu = gMenuWork;
  s32 count;
  struct Sprite **figures;
  s32 i;

  count = (u16)Party_ListMembersFar(memberIds);
  if (count != 0) {
    figures = menu->tabSprites;
    i = count;
    do {
      struct Sprite *figure = *figures++;

      if (figure != 0) {
        Sprite_DestroyFar(figure);
      }
    } while (--i != 0);
  }
  Task_Remove(Menu_UpdateEntryActorTransforms);
}

/// Task: each frame, places every party member sprite created by
/// PsynergyMenu_InitializeEntryActors at its menu position and scale, through
/// the field's projected placement.
void Menu_UpdateEntryActorTransforms(void) {
  struct PsynergyMenuWork *menu = gMenuWork;
  struct SpriteScale scale;
  struct SpritePlacement position;
  s32 i;
  s32 count;

  count = (u16)Party_CountMembersFar();
  for (i = 0; i < count; i++) {
    struct Sprite *figure = menu->tabSprites[i];
    // 482 less the row's screen y, in 16.16. The ground y is always 482,
    // which puts every figure in the same depth slot ((482 >> 1) + 10 = 251,
    // Render_ApplyProjectedPlacement), and raising the image by this height
    // brings it back up to its row.
    s32 placeY = 0x01e20000 - (menu->rowPositions[i] << 16);

    if (figure != 0) {
      figure->image.priority = 0;
      scale.x = menu->memberScale[i];
      scale.y = menu->memberScale[i];
      position.x = menu->tabX[i] << 16;
      position.height = placeY;
      position.y = (menu->rowPositions[i] << 16) + placeY; // always 482
      position.groundHeight = 0;
      Actor_ApplyProjectedPlacementFar(figure, &position, &scale, FACING_SOUTH);
    }
  }
}

/// The cursor bobs through an 8-step table, one step every two frames.
#define CURSOR_BOB_STEP() ((gFrameCount >> 1) & 7)

/// Places the hand cursor at (xOffset, yOffset) inside the menu's top-left
/// window, bobbing.
void MenuCursor_Position(s32 xOffset, s32 yOffset) {
  struct PsynergyMenuWork *menu = gMenuWork;
  struct WindowSprite *cursor = menu->sideIcon[0];

  cursor->oam.x = cursor->x = gMenuCursor_BobX[CURSOR_BOB_STEP()] + xOffset +
                              (menu->auxiliaryWindow)->x * 8 + 8;
  cursor->oam.y = cursor->y = gMenuCursor_BobY[CURSOR_BOB_STEP()] + yOffset +
                              (menu->auxiliaryWindow)->y * 8 + 8;
}

/// Slides the hand cursor from where it is to (x, y) inside the menu's
/// top-left window over two frames, in 1/16 pixel steps. Positions carry a
/// 64-pixel bias while moving so that they stay positive. Does nothing, once,
/// after skipSlide is set.
void MenuCursor_Slide(s32 x, s32 y) {

  struct PsynergyMenuWork *menu = gMenuWork;
  struct WindowSprite *cursor;
  s32 steps;
  s32 startX;
  s32 startY;
  s32 fineX;
  s32 fineY;
  s32 stepX;
  s32 stepY;

  steps = 2;
  if (menu->skipSlide != 0) {
    menu->skipSlide = 0;
    return;
  }
  cursor = menu->sideIcon[0];
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
  // MenuCursor_Create opens.
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
        (fineX >> 4) + ((menu->auxiliaryWindow)->x << 3) - 56;
    fineY += stepY;
    cursor->oam.y = cursor->y =
        (fineY >> 4) + ((menu->auxiliaryWindow)->y << 3) - 56;
    steps--;
    if (steps != 0)
      WaitFrames(1);
  } while (steps != 0);
}

UNUSED void PsynergyMenu_ReservedNoOp(void) {}

/// Lays the entry icons out eight to a row from (108, 40). Nothing calls it.
UNUSED void PsynergyMenu_RefreshMemberEntriesDefault(void) {
  PsynergyMenu_RefreshMemberEntries(108, 40, 8);
}

/// Lays the menu's entry icons (up to 32) out in a grid of 16x16 cells,
/// columns wide, from (originX, originY); empty slots keep their place.
void PsynergyMenu_RefreshMemberEntries(s32 originX, s32 originY, s32 columns) {
  struct WindowSprite **slot;
  struct WindowSprite **scan;
  s32 index;

  index = 0;
  slot = ((struct PsynergyMenuWork *)gMenuWork)->entryIcons;
  scan = slot;
  do {
    if (*scan++ != NULL) {
      PsynergyMenu_PositionMemberEntry(slot, index, originX, originY, columns);
    }
    index++;
    slot++;
  } while (index < 32);
}

/// Places the icon in *slot at grid cell index (0 above 31) of a grid of
/// 16x16 cells, columns wide, from (originX, originY), and shows it.
void PsynergyMenu_PositionMemberEntry(struct WindowSprite **slot, s32 index,
                                      s32 originX, s32 originY, s32 columns) {
  struct WindowSprite *icon;

  if (index > 31) {
    index = 0;
  }
  icon = *slot;
  icon->y = (index / columns) * 16 + originY;
  icon->x = (index % columns) * 16 + originX;
  Icon_PrepareActor(icon);
}

/// Places the icon at slotIndex (0 above 15) in a columns-wide grid of
/// 24x16 cells starting at (originX, originY), and shows it.
void ItemMenu_PlaceIcon(struct WindowSprite **slot, s32 slotIndex, s32 originX,
                        s32 originY, s32 columns) {
  struct WindowSprite *icon;

  if (slotIndex > 15)
    slotIndex = 0;
  icon = *slot;
  icon->y = slotIndex / columns * 16 + originY;
  icon->x = slotIndex % columns * 24 + originX;
  Icon_PrepareActor(icon);
}

/// Lays the inventory's 15 item icons out five to a row from (116, y): y is
/// 56 for layout 1 and 40 otherwise.
void ItemMenu_RefreshEntry(s32 layout) {
  struct WindowSprite **slot;
  struct WindowSprite **scan;
  struct WindowSprite *icon;
  s32 index;
  s32 originY;
  struct ItemMenuWork *menu;

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
void ItemMenu_ShowMessage(s32 message, s32 x, s32 y) {
  struct ItemMenuWork *menu = gMenuWork;
  struct Window *window;
  s32 height;
  s32 width;
  s32 top;
  s32 left;

  menu->sideIcons[0]->motion = ICON_HIDDEN;
  if (y != -1) {
    Text_GetMessageSizeFar(message, &left, &top, &width, &height);
    if (Window_UpdateOrCreate(&menu->modalWindow, x, y, width, height,
                              WINDOW_KEEP_EXISTING | 2) == 0)
      Window_SetBounds(menu->modalWindow, x, y, width, height);
    window = menu->modalWindow;
  } else {
    window = menu->descriptionWindow;
  }
  WindowSprite_RedrawSavedRectFar(window);
  WindowSprite_ClearListFar((void *)window);
#if LANGUAGE != LANGUAGE_JAPANESE
  if (y == -1)
    Text_DrawMessageAsTilesFar(message, window, 0, 0);
  else
    Text_DrawMessageFar(message, window, 0, 0);
#else
  Text_DrawMessageFar(message, window, 0, 0);
#endif
  if (x != -1) {
    WaitFrames(1);
    do {
      WaitFrames(1);
    } while (!(gKeysPressed & KEY_A) && !(gKeysPressed & KEY_B) &&
             !(gKeysPressed & KEY_START));
#if TBS_PATCH_1
    if (y == -1)
      WindowSprite_RedrawSavedRectFar(window);
#endif
    WindowSprite_ClearListFar((void *)window);
  } else {
    GameFlag_SetFar(FLAG_MENU_INFO_KEPT);
  }
  menu->completionFlag = 1;
  menu->sideIcons[0]->motion = ICON_SHOWN;
#if TBS_PATCH_1
  if (y != -1)
    Window_CloseIfOpen(&menu->modalWindow, 1);
#endif
}

/// Sorts a party member's 15 inventory slots by the list order of mode (enum
/// ListSort): items are grouped by the order's item types and, within
/// a type, run from the highest item id down. Empty slots go to the end.
/// L + A in the item menu cycles through the modes. Returns 1.
s32 ItemMenu_Sort(u16 *inventory, s32 mode) {
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
    pass++;
  }

  for (i = 0; i < count; i++)
    inventory[i] = sorted[i];
  return 1;
}

/// Copies the list order of mode (enum ListSort; any other mode takes
/// LIST_SORT_ITEMS_FIRST's) into order, up to and including its LIST_ORDER_END
/// and at most 32 bytes.
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
  Link_DrawPageArrowsFar(PAGE_ARROW_VRAM);
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
    Sound_PlayFar(SE_MENU_CURSOR_MOVE);
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
    Sound_PlayFar(SE_MENU_CURSOR_MOVE);
    result = 0;
    if (++*cursor == count - *page * pageSize)
      *cursor = result;
    if (*cursor > pageSize - 1)
      *cursor = result;
  }
  return 0;
}

/// Copies WINDOW_PALETTE into background palette paletteNumber and lightens its
/// WINDOW_BACKGROUND_COLOR by 9 steps in each channel: the highlight colour of
/// the menu's selected rows. Each screen sets up HIGHLIGHT_PALETTE as it opens,
/// and again after the Details window has borrowed it (ItemMenu_ShowDetails);
/// only Item_PrepareUsePalette, which nothing calls, uses palette 13.
void Palette_LightenHighlight(s32 paletteNumber) {
  u16 *palette = (u16 *)&BG_PLTT_COLOR(paletteNumber, 0);
  u32 red;
  u32 green;
  u32 blue;

  // Both transfers copy the same 32 bytes, first as 16 halfwords and then as
  // 8 words.
  Dma_Set((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette,
          DMA_ENABLE << 16 | 16, REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, 0), palette,
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
/// HIGHLIGHT_PALETTE and the others in WINDOW_PALETTE. Draws nothing for a
/// single page.
void PagedList_DrawPageNumbers(struct Window *window, s32 itemCount,
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
/// HIGHLIGHT_PALETTE (the highlight made by Palette_LightenHighlight):
/// palette is the new palette number. Only bit 12 of each map entry is cleared
/// before it is set, so this works between those two palettes. (x, y) and the
/// size are in tiles inside the window's border; the rectangle is clipped to
/// the screen's 30 columns, and to its 20 rows only when it reaches row 29 (see
/// the body). Used for the highlighted rows of the item, Psynergy and Status
/// lists. Marks the whole tilemap for upload.
void Render_SetTilemapFlagRect(const struct Window *window, s32 x, s32 y,
                               s32 width, s32 height, u32 palette) {
  struct WindowWork *work = (struct WindowWork *)gWindowWork[0];
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

/// Lends HIGHLIGHT_PALETTE to the element icons:
/// copies the first sprite palette, the icons' colours, into it, then
/// WINDOW_BACKGROUND_COLOR over its TEXT_BACKGROUND_COLOR (as
/// StatusScreen_Run does for palettes 0 and 1). The Details window draws its
/// element icons (window tiles 1 to 4) in HIGHLIGHT_PALETTE, so
/// ItemMenu_ShowDetails calls this first, as do ItemMenu_OpenDetail and
/// Item_PrepareUsePalette, which nothing calls. It replaces the row highlight
/// until Palette_LightenHighlight(HIGHLIGHT_PALETTE) makes it again.
void Palette_LoadIconsIntoHighlight(void) {
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, 0),
          DMA_ENABLE << 16 | 16, REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(WINDOW_PALETTE, WINDOW_BACKGROUND_COLOR),
          (void *)&BG_PLTT_COLOR(HIGHLIGHT_PALETTE, TEXT_BACKGROUND_COLOR),
          DMA_ENABLE << 16 | 1, REG_DMA3);
}

/// Shows the icons of one page of a list, one row of 16 pixels each from
/// (x, y): entries first to first + pageSize - 1, stopping early at an empty
/// icon slot or the work's itemCount. Hides all the others. Used by the item
/// and Psynergy lists and the Status screen's lists.
void Menu_SetPageIcons(s32 pageSize, s32 first, struct Window *window, s32 x,
                       s32 y) {
  struct ItemMenuWork *menu = gMenuWork;
  struct WindowSprite *icon;
  s32 i;

  for (i = 0; i < 32; i++) {
    icon = menu->entryIcons[i];
    if (icon != NULL)
      icon->motion = ICON_HIDDEN;
  }
  for (i = first;
       i < pageSize + first && (icon = menu->entryIcons[i]) != NULL &&
       i <= menu->itemCount - 1;
       i++) {
    icon->x = x;
    icon->y = y + (i - first) * 16;
    Icon_PrepareActor(icon);
    icon->motion = ICON_SHOWN;
  }
}

/// Writes the party's coins and the "Coins" label into window: the info
/// window of the Item and Psynergy screens' party member choice.
void Text_DrawWorkValueWithLabel(struct Window *window) {
  Text_DrawNumberAtOffsetFar(gAdventure.coins, 7, window, 8, 0);
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

/// Holds back the window tilemap's upload (holdTilemapUpload), so that what a
/// menu draws meanwhile appears at once when Window_ReleaseTilemapUpload lets
/// it go. Nothing calls it: the screens set holdTilemapUpload themselves.
void Window_HoldTilemapUpload(void) {
  ((struct WindowWork *)gWindowWork[0])->holdTilemapUpload = 1;
}

/// Lets the window tilemap be uploaded again after Window_HoldTilemapUpload.
/// Nothing calls it.
void Window_ReleaseTilemapUpload(void) {
  ((struct WindowWork *)gWindowWork[0])->holdTilemapUpload = 0;
}

/// Plays a sound and returns 1. Called by
/// Ability_PlayUseAnimation, which ignores the value.
s32 Sound_PlayReturnOne(s32 sound) {
  Sound_PlayFar(sound);
  return 1;
}

/// Task while a menu runs: on Start, plays the cancel sound,
/// sets FLAG_MENU_START_PRESSED and removes itself.
void Menu_UpdateCancelSound(void) {
  if (gKeysPressed & KEY_START) {
    Sound_PlayFar(SE_MENU_CANCEL);
    // XXX (no evidence): why this is a game flag rather than a variable;
    // nothing outside the menu's own code sets or tests it.
    GameFlag_SetFar(FLAG_MENU_START_PRESSED);
    Task_Remove(Menu_UpdateCancelSound);
  }
}

/// Starts watching for Start: clears FLAG_MENU_START_PRESSED and adds
/// Menu_UpdateCancelSound. ItemMenu_Run, PsynergyMenu_Run and StatusScreen_Run
/// call it before their command loops and Menu_EnsureCancelSound after them.
void Menu_CancelSoundReset(void) {
  GameFlag_ClearFar(FLAG_MENU_START_PRESSED);
  // TASK_PASS_FRAME: once a frame, in the frame pass.
  Task_Add(Menu_UpdateCancelSound, TASK_PASS_FRAME);
}

/// Stops watching for Start, if Start has not removed the watch already.
void Menu_EnsureCancelSound(void) {
  if (GameFlag_GetFar(FLAG_MENU_START_PRESSED) == 0) {
    Task_Remove(Menu_UpdateCancelSound);
  }
}

/// Makes the text drawn after it TEXT_COLOR_NORMAL. Nothing calls
/// it or the two after it; the status panel sets its colours with
/// Text_SetColorFar.
void WindowPalette_SetNormalTextColor(void) {
  Text_SetColorFar(TEXT_COLOR_NORMAL);
}

/// Makes the text drawn after it TEXT_COLOR_NO_HP.
void WindowPalette_SetNoHpTextColor(void) {
  Text_SetColorFar(TEXT_COLOR_NO_HP);
}

/// Makes the text drawn after it TEXT_COLOR_LOW_HP.
void WindowPalette_SetLowHpTextColor(void) {
  Text_SetColorFar(TEXT_COLOR_LOW_HP);
}

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
