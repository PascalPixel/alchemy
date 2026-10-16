// The German, Spanish, French and Italian Psynergy menus save the window
// tiles they draw over (0x2000 bytes from 0x06004000) and restore them after.
#include "unit/menu/menu.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "inventory_menu.h"
#include "io_reg.h"
#include "uiwindow.h"
#include "mem.h"
#include "character_menu.h"
#include "battle_unit.h"
#include "item.h"
#include "edition.h"
#include "battle_system.h"
#include "ui.h"
#include "text/msg_ids.h"
#include "resource.h"
#include "vram_block.h"
#include "render_input.h"
#include "fixed_math.h"
#include "lib.h"
#include "psynergy_menu.h"
#include "animspr.h"
#include "field_sprite.h"
#include "scene.h"
#include "m7_interfaces.h"
#include "global_cells.h"
#include "keystate.h"
#include "window.h"
#include "menu_result.h"
#include "tbs_edition.h"
#include "game_state.h"
#include "sound_ids.h"
#include "iwram_call.h"
#include "heap_state.h"
#include "menu_list.h"
#include "irq.h"
#include "object_factory.h"
#include "far_runtime.h"
#include "equipment_menu.h"
#include "owner_state.h"
#include "inventory.h"
#include "layout_guard.h"
#include "battle_type.h"
#include "battle_calc.h"
#include "event_work.h"
#include "djinn_menu.h"
#include "a9_motion.h"
#include "battle_effect.h"
#include "four_object_motion.h"
#include "item_ids.h"

extern u8 IwramCopyWords[];

s32 _call_via_r3(void *, void *, s32, void *);

// UiWindow_UpdateOrCreate: keep an existing window as it is, without redrawing
// its saved rectangle. The low byte of the flags is the window style.
#define WINDOW_KEEP_EXISTING 0x100
#define WINDOW_STYLE_MASK 0xff

// BattleAct.flags: where an ability or item can be used, as the Psynergy
// list says ("Usable in the field", "Usable in battle").
#define ACTION_USABLE_IN_FIELD 0x40
#define ACTION_USABLE_IN_BATTLE 0x80

// Game flags of the menus. Start was pressed: every menu screen unwinds and
// the whole menu closes.
#define FLAG_MENU_START_PRESSED 0x150
// The info window holds a message the next selection must not draw over.
#define FLAG_MENU_INFO_KEPT 0x151

// Pauses the field while a menu is open: stops the overlay's scheduler
// callbacks and the map update, and clears game flags 0x166 and 0x152.
// Returns nothing useful; callers ignore the value.
s32 Scheduler_DisableOverlayCallbacksWithFlags(void) {
  Scheduler_ResumeOverlayTasks();
  Map_DisableUpdateCallbackFar();
  GameFlag_ClearBitFar(0x166); // meaning not known yet
  GameFlag_ClearBitFar(0x152); // set, it stops BattlePalette_UpdateBlend
}

// Undoes Scheduler_DisableOverlayCallbacksWithFlags when the menu closes:
// sets flags 0x152 and 0x166 again and restarts the map update and the
// overlay's scheduler callbacks. Returns the scheduler's result.
s32 Scheduler_EnableOverlayCallbacksWithFlags(void) {
  GameFlag_SetBitFar(0x152);
  GameFlag_SetBitFar(0x166);
  Map_EnableUpdateCallbackFar();
  return Scheduler_SuspendOverlayTasks();
}

// Clears the whole menu work area (0x29c words) and sets its defaults: no
// pane selected, one entry in each pane, and a 1x1 cursor grid.
void UiWindow_InitializeWork(s32 unused) {
  struct InventoryMenuState *work = gMenuWork;

  Dma_Fill32(0, work, 0x29c * 4);
  work->paneIndex[0] = -1;
  work->paneCount[0] = 1;
  work->paneCount[1] = 1;
  work->columnCount = 1;
  work->rowCount = 1;
}

// Opens the window kept in *handle, or redraws it if it is already open.
//
// If *handle is empty, creates a window at (x, y), w by h tiles, in the style
// given by the low byte of flags, stores it in *handle and returns 1. If the
// window exists, redraws its saved rectangle (unless flags has
// WINDOW_KEEP_EXISTING) and returns 0.
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

// Closes the window kept in *handle, if any, and clears the handle. style is
// passed on to UiWork_FinalizeFar and chooses how the window closes.
void UiWindow_CloseIfOpen(struct UiWindow **handle, s32 style) {
  if (*handle != NULL) {
    UiWork_FinalizeFar(*handle, style);
    *handle = NULL;
  }
}

// The status panel's name and ailment column starts further left in the
// international editions, whose ailment labels are longer.
#if EDITION_INTERNATIONAL
#define STATUS_X 32
#else
#define STATUS_X 40
#endif

// What Menu_DrawOwnerStatusPanel shows below the name and ailments: the low
// byte of its mode.
enum StatusPanelKind {
  STATUS_PANEL_EXPERIENCE = 0, // class, HP, PP and experience
  STATUS_PANEL_EQUIP = 2,      // stats if the item were equipped
  STATUS_PANEL_LEARN = 4,      // the Psynergy a Psynergy item teaches
  STATUS_PANEL_CLASS = 6,      // class, HP and PP
  STATUS_PANEL_STATS = 8,      // Attack, Defense, Agility and Luck
};
#define STATUS_PANEL_KIND_MASK 0xff
// Keeps the open window, the name and the ailments, and redraws only the
// part below them.
#define STATUS_PANEL_LOWER_ONLY 0x100

// Text colours of the status panel's numbers.
#define TEXT_COLOR_LOW_HP 4 // HP under a quarter of the maximum
#define TEXT_COLOR_NO_HP 2  // HP 0
#define TEXT_COLOR_NORMAL 15

// Copies one BattleUnit with the resident IWRAM word copy.
static inline void Owner_Copy(struct BattleUnit *destination,
                              struct BattleUnit *source) {
  _call_via_r3(destination, source, sizeof(struct BattleUnit), IwramCopyWords);
}

// Draws the item menu's status panel for the party member owner (window at
// tile 0,5, 13 by 12): the name, up to two ailments or the level, and below
// them what mode's panel kind asks for. slot is the inventory slot of the
// item the panel is about, for the equipment and Psynergy item panels. The
// first parameter, the status window, is not used: the window is the menu
// work's.
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
    if (!created) {
      WaitFrames(1);
      UiWindow_ClearInteriorTilesFar(state->statusWindow, 0, 0, 88, 32);
    }
    UiText_DrawStringAtOffsetFar(unit->name, window, STATUS_X, 0);
    CharacterMenu_BuildAvailability(ailments, 1, owner);
    ailmentRows = 0;
    if (ailments[CHARACTER_POISON]) {
      UiText_DrawCharacterAtOffsetFar(MsgPoisonLabel, window, STATUS_X,
                                      ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[CHARACTER_VENOM]) {
      UiText_DrawCharacterAtOffsetFar(MsgVenomLabel, window, STATUS_X,
                                      ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[CHARACTER_CURSE]) {
      UiText_DrawCharacterAtOffsetFar(MsgCurseLabel, window, STATUS_X,
                                      ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[CHARACTER_HAUNT]) {
      UiText_DrawCharacterAtOffsetFar(MsgHauntLabel, window, STATUS_X,
                                      ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailmentRows < 2) {
      value = unit->level;
#if EDITION_INTERNATIONAL
      UiText_DrawStringAtOffsetFar(gUi_LvString, window, 40, 16);
#else
      UiText_DrawStringInWindowFar(gUi_LvString, window, 40, 16);
#endif
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
#if EDITION_INTERNATIONAL
      UiText_DrawCharacterAtOffsetFar(MsgCannotEquip, window, 0, 48);
#else
      UiText_DrawCharacterAtOffsetFar(MsgCannotEquip, window, 16, 48);
#endif
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

    for (i = 0; i < 32; i++) {
      if ((unit->actionSlots[i].encodedAction & OWNER_ACTION_ID_MASK) ==
          psynergy) {
        known = 1;
        break;
      }
    }
    // The Psynergy's name and "Learned" or "Will learn"; Spanish puts the
    // label above the name.
#if defined(TBS_EDITION_ES)
    if (known) {
      UiText_DrawCharacterAtOffsetFar(psynergy + MsgAbilityName, window, 0, 56);
      UiText_DrawCharacterAtOffsetFar(MsgLearned, window, 0, 48);
    } else {
      UiText_DrawCharacterAtOffsetFar(psynergy + MsgAbilityName, window, 0, 56);
      UiText_DrawCharacterAtOffsetFar(MsgWillLearn, window, 0, 48);
    }
#else
    if (known) {
      UiText_DrawCharacterAtOffsetFar(psynergy + MsgAbilityName, window, 0, 48);
      UiText_DrawCharacterAtOffsetFar(MsgLearned, window, 0, 56);
    } else {
      UiText_DrawCharacterAtOffsetFar(psynergy + MsgAbilityName, window, 0, 48);
      UiText_DrawCharacterAtOffsetFar(MsgWillLearn, window, 0, 56);
    }
#endif
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

#if EDITION_INTERNATIONAL
// The pixels the field allows each digit. The Latin font's digits are 7
// pixels wide, so the digits themselves take a pixel less each.
#define FIELD_PIXELS_PER_DIGIT 8

// Returns how many decimal digits number has, but at least 2: 0-99 give 2,
// 100-999 give 3, 1000-9999 give 4. The count starts with the units digit,
// adds one for each division by ten that leaves 10 or more, then adds the
// leading digit; a number under 10 has its one digit counted as both, which
// is where the minimum of 2 comes from.
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

// Draws number right-aligned in a field that ends at pixel column x and
// allows 8 pixels for each digit, at least two digits wide.
//
// The international editions draw a character's HP and PP with it
// (Ui_DrawValuePairRows). The Japanese editions draw those figures in
// four-character fields padded with spaces, which keeps the digits in line
// because the Japanese font's space is as wide as a digit (7 pixels). The
// Latin font's space is 5 pixels, so here each figure gets a field as wide
// as its digits instead.
//
// The number writer draws the digits 7 pixels apart, so a number ends about
// a pixel per digit before x: 99 ends 2 pixels before it, 100 ends 3. A
// number under 10 draws as a 5-pixel space and its digit, and ends 4 pixels
// before x.
//
// number: the value to draw, 0 or more
// window: the window to draw in
// x:      the field's right edge, in pixels within the window
// y:      the pixel row of the text, within the window
void UiText_DrawNumberRightAligned(s32 number, struct UiWindow *window, s32 x,
                                   s32 y) {
  s32 width = CountDigitsAtLeastTwo(number);

  // Step x back to the field's left edge; the number writer right-aligns the
  // number in a field of width characters. x changes in place: a separate
  // left-edge local changes the registers.
  x -= width * FIELD_PIXELS_PER_DIGIT;
  UiText_DrawNumberAtOffsetFar(number, width, window, x, y);
}

#endif

// Draws a character's HP and PP into window as two rows, "HP current/maximum"
// at pixel row 40 and "PP current/maximum" at row 48. Current HP is drawn in
// colour 4 when it is below a quarter of maximum HP and in colour 2 at 0;
// then the colour goes back to 15, the default text colour.
//
// unit:   the character whose figures are drawn
// window: the window to draw in
void Ui_DrawValuePairRows(struct BattleUnit *unit, struct UiWindow *window) {
  // Each figure in turn, as an s16. Read straight from unit, the figures
  // compile differently.
  s16 value;

#if EDITION_INTERNATIONAL
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

// The stat arrows: the up and down arrow sprites the menu loads into its two
// resource slots (Resource_LoadPairedBlocks).
enum StatArrow { STAT_ARROW_UP, STAT_ARROW_DOWN };

// Writes Attack, Defense and Agility into a status window as they are now
// (before), and beside each one that changes its new value (after) with an up
// or down arrow: the equipment preview.
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

// RenderOutput.active as the menu sets it: shown, or hidden but kept.
#define ICON_SHOWN 1
#define ICON_HIDDEN 13

// RenderOutput_CreateFar's sprite attributes for the menu's 16x16 icons.
#define ICON_SPRITE_16X16 0x40000000
// Size of one 16x16 4bpp sprite tile block.
#define ICON_TILE_BYTES 0x80

// Creates a hand cursor sprite in window at (x, y), loading the hand's tiles
// into a free VRAM block. Returns the sprite, or 0 when no block is free.
// Nothing calls it; UiIcon_CreateWithResourceVariant is the one in use.
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

// Creates the menu's hand cursor sprite in window at (x, y), loading the
// hand's tiles into a free VRAM block. Returns the sprite, or 0 when no
// block is free.
s32 UiIcon_CreateWithResourceVariant(struct UiWindow *window, s32 x, s32 y) {
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

// Shows a menu icon at its stored position, without rotation or scaling.
void UiIcon_PrepareObject(struct RenderOutput *object) {
  if (object != NULL) {
    object->active = ICON_SHOWN;
    object->oam.x = object->x;
    object->oam.y = object->y;
    object->oam.matrix = 0;
    object->oam.affineMode = 0;
  }
}

// Loads the graphics of character unit into menu entry slot 0. The calling
// menu argument is not used.
void PsynergyMenu_CallIconRoutineWithValue(s32 unused, s32 unit) {
  Ui_LoadCharacterEntryForSlotFar(0, unit, 0);
}

// Opens the menu's top-left window (13 by 5 tiles) and puts the hand cursor
// in it, hidden for now. Selects no tab yet. Returns the window.
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
  sentinel = 254;
  cursor->sentinel = sentinel;
  sentinel -= 255;
  work->paneIcon[1]->sentinel = sentinel; // -1
  return window;
}

// Creates a walking figure for each active party member, side by side under
// the window source: origin (originX, originY) in tiles from the window's
// corner, each figure 16 + spacing pixels after the last, at full scale.
// Clears the unused figure slots and starts the callback that places them
// every frame.
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
        (struct AnimationObject *)ResourceObject_CreateFar(
            Party_RemapCharacterIdByFlagsFar(ownerIds[i]));
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
      ((struct FieldSprite *)figure)->priority = 0;
      figure->flags = 0;
      AnimationObjects_SelectAnimationFar(figure, 1);
    }
  }
  // Clear the unused party slots so the callback can skip them.
  for (; i < 8; i++) {
    menu->tabObjects[i] = 0;
  }
  {
    s32 priority = 200;
    priority <<= 4;
    Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, priority);
  }
}

// Releases the party figures PsynergyMenu_InitializeEntryObjects created and
// stops their placement callback.
void Menu_ReleaseEntryObjects(void) {
  u32 ownerIds[7];
  struct PsynergyMenuState *menu = gMenuWork;
  s32 count;
  void **figures;
  s32 i;

  count = (u16)Party_ListActiveOwnersFar(ownerIds);
  if (count != 0) {
    figures = (void **)menu->tabObjects;
    i = count;
    do {
      void *figure = *figures++;

      if (figure != 0) {
        ResourceObject_ReleaseFar(figure);
      }
    } while (--i != 0);
  }
  Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
}

// Scheduler callback: each frame, places every party member sprite created by
// PsynergyMenu_InitializeEntryObjects at its menu position and scale, through
// the field's projected placement.
void Menu_UpdateEntryObjectTransforms(void) {
  struct PsynergyMenuState *menu = gMenuWork;
  s32 scale[2];
  s32 position[4];
  s32 i;
  s32 count;

  count = (u16)Party_CountActiveOwnersFar();
  for (i = 0; i < count; i++) {
    struct AnimationObject *figure = menu->tabObjects[i];
    // 482 less the row's screen y, in 16.16.
    s32 placeY = 0x01e20000 - (menu->rowPositions[i] << 16);

    if (figure != 0) {
      figure->part[0].priority = 0;
      scale[0] = menu->ownerScale[i];
      scale[1] = menu->ownerScale[i];
      position[0] = menu->tabX[i] << 16;
      position[1] = placeY;
      position[2] = (menu->rowPositions[i] << 16) + placeY; // always 482
      position[3] = 0;
      Object_ApplyProjectedPlacementFar(figure, position, scale, 0x4000);
    }
  }
}

// The cursor bobs through an 8-step table, one step every two frames.
#define CURSOR_BOB_STEP() ((gFrameCount >> 1) & 7)

// Places the hand cursor at (xOffset, yOffset) inside the menu's top-left
// window, bobbing.
void UiMenu_PositionCursor(s32 xOffset, s32 yOffset) {
  struct PsynergyMenuState *work = gMenuWork;
  struct RenderOutput *cursor = work->paneIcon[0];

  cursor->oam.x = cursor->x = gUiMenu_CursorBobX[CURSOR_BOB_STEP()] + xOffset +
                              (work->auxiliaryWindow)->x * 8 + 8;
  cursor->oam.y = cursor->y = gUiMenu_CursorBobY[CURSOR_BOB_STEP()] + yOffset +
                              (work->auxiliaryWindow)->y * 8 + 8;
}

// Slides the hand cursor from where it is to (x, y) inside the menu's
// top-left window over two frames, in 1/16 pixel steps. Positions carry a
// 64-pixel bias while moving so that they stay positive. Does nothing, once,
// after skipSlide is set.
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

// Does nothing; nothing calls it.
UNUSED void PsynergyMenu_ReservedNoOp(void) {}

// Lays the entry icons out eight to a row from (108, 40). Nothing calls it.
UNUSED void PsynergyMenu_RefreshOwnerEntriesDefault(void) {
  PsynergyMenu_RefreshOwnerEntries(108, 40, 8);
}

// Lays the menu's entry icons (up to 32) out in a grid of 16x16 cells,
// columns wide, from (originX, originY); empty slots keep their place.
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

// Places the icon in *slot at grid cell index (0 above 31) of a grid of
// 16x16 cells, columns wide, from (originX, originY), and shows it.
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

//--------------------------------------------------------------------------
// position_owner_entry.c
// Item menu: place the inventory icons on their grid
//--------------------------------------------------------------------------

// Places the item icon in *slot at grid cell i (0 above 15) of a grid of
// 24x16 cells, cols wide, from (x0, y0), and shows it.
void ItemMenu_PosOwner(struct RenderOutput **slot, s32 i, s32 x0, s32 y0,
                       s32 cols) {
  struct RenderOutput *icon;

  if (i > 15)
    i = 0;
  icon = *slot;
  icon->y = i / cols * 16 + y0;
  icon->x = i % cols * 24 + x0;
  UiIcon_PrepareObject(icon);
}

// Lays the inventory's 15 item icons out five to a row from (116, y): y is
// 56 for layout 1 and 40 otherwise.
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
      ItemMenu_PosOwner(slot, index, 116, originY, 5);
    }
    index++;
    slot++;
  } while (index < 15);
}

// Shows message in the item menu, with the hand cursor hidden meanwhile.
//
// With y == -1 the message goes into the info window; otherwise a modal
// window sized to the message opens at (x, y). With x == -1 the message stays
// up and FLAG_MENU_INFO_KEPT records that the info window holds it (the next
// selection keeps it instead of drawing the item name); otherwise it waits
// for A, B or Start and clears it again.
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
#if EDITION_INTERNATIONAL
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
#if EDITION_INTERNATIONAL
    if (y == -1)
      RenderOutput_RedrawSavedRectFar(window);
#endif
    RenderOutput_ClearListFar((void *)window);
  } else {
    GameFlag_SetBitFar(FLAG_MENU_INFO_KEPT);
  }
  menu->completionFlag = 1;
  menu->paneIcons[0]->active = ICON_SHOWN;
#if EDITION_INTERNATIONAL
  if (y != -1)
    UiWindow_CloseIfOpen(&menu->modalWindow, 1);
#endif
}

// Sorts a party member's 15 inventory slots by the list order of mode (see
// Menu_CopyListOrder): items are grouped by the order's item types and, within
// a type, run from the highest item id down. Empty slots go to the end.
// L + A in the item menu cycles through the modes. Returns 1.
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

//--------------------------------------------------------------------------
// copy_list_order.c
// Menu: copy a sort order table
//--------------------------------------------------------------------------

// Copies the list order of mode 0 to 2 (any other mode: the default order)
// into order, up to and including its LIST_ORDER_END and at most 32 bytes.
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

// The page indicator: the window font's digit tiles for pages 1, 2, ..., and
// the two caps around them.
#define PAGE_LABEL_FIRST 49
#define PAGE_CAP_LEFT 0xf128
#define PAGE_CAP_RIGHT 0xf129

// Moves a paged list's cursor and page with the d-pad's repeating keys.
//
// count entries are shown per_page at a time; *cursor is the row on the page
// and *page the page. Vertical lists (horizontal 0) move the cursor with Up
// and Down and turn pages with Left and Right; horizontal lists the other way
// round. Moving past either end wraps round; turning to a shorter page pulls
// the cursor up to its last entry. Returns 1 after a page turn, 0 after a
// cursor move and -1 when nothing moved or the list is empty.
s32 Menu_HandlePageInput(s32 horizontal, s32 count, s32 per_page, s32 *cursor,
                         s32 *page) {
  s32 pages;
  s32 next;
  s32 previous;
  s32 pageForward;
  s32 result;

  result = -1;
  if (count == 0)
    return result;
  Link_DrawShiftedTilePairFar(0x06002500); // animate the page arrows
  pages = count / per_page;
  if (count % per_page != 0)
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
    AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
    if (--*page < 0)
      *page = pages - 1;
    if (*cursor + *page * per_page > count - 1) {
      *cursor = count - *page * per_page - 1;
      if (*cursor > per_page - 1)
        *cursor = per_page - 1;
    }
    Input_ResetKeyRepeat(); // restart the key repeat delay
    result = 1;
    return result;
  }
  if (pageForward) {
    AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
    if (++*page > pages - 1)
      *page = 0;
    if (*cursor + *page * per_page > count - 1) {
      *cursor = count - *page * per_page - 1;
      if (*cursor > per_page - 1)
        *cursor = per_page - 1;
    }
    Input_ResetKeyRepeat();
    result = 1;
    return result;
  }
  if (previous) {
    AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
    if (--*cursor < 0) {
      *cursor = per_page - 1;
      *cursor = count - *page * per_page - 1;
      if (*cursor > per_page - 1)
        *cursor = per_page - 1;
    }
    return 0;
  }
  result = -1;
  if (next == 0)
    return result;
  {
    AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
    result = 0;
    if (++*cursor == count - *page * per_page)
      *cursor = result;
    if (*cursor > per_page - 1)
      *cursor = result;
  }
  return 0;
}

// Copies the last background palette (15) into background palette bank and
// lightens its colour 4 by 9 steps in each channel: the highlight colour of
// the menu's selected rows.
void Palette_LightenBankHighlight(s32 bank) {
  u16 *palette = (u16 *)(0x05000000 + (bank << 5));
  u32 red;
  u32 green;
  u32 blue;

  Dma_Set((const void *)0x050001e0, palette, 0x80000010, REG_DMA3);
  Dma_Set((const void *)0x050001e0, palette, 0x84000008, REG_DMA3);
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

// Draws a paged list's page numbers on the top border of window, ending at
// tile rightEdge: one digit per page between two caps, selectedPage in
// palette 14 and the others in palette 15. Draws nothing for a single page.
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
    UiWindow_SetTilemapEntryFar(window, PAGE_CAP_LEFT, x - 1, -1, 0);

    for (page = 0; page < pageCount; page++) {
      if (page == selectedPage)
        UiWindow_SetTilemapEntryFar(window, tile, x, -1, 2); // palette 14
      else
        UiWindow_SetTilemapEntryFar(window, tile, x, -1, 3); // palette 15
      tile++;
      x++;
    }

    UiWindow_SetTilemapEntryFar(window, PAGE_CAP_RIGHT, x, -1, 0);
  }
}

// Switches a rectangle of a window's text between palette 15 (normal) and 14
// (the highlight made by Palette_LightenBankHighlight): palette is the new
// palette number. Only bit 12 of each map entry is cleared before it is set,
// so this works between those two palettes. (x, y) and the size are in tiles
// inside the window's border; the rectangle is clipped to the screen.
void Render_SetTilemapFlagRect(const struct UiWindow *object, s32 x, s32 y,
                               s32 width, s32 height, u32 palette) {
  struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
  u8 *tilemap = (u8 *)work->tilemap;

  x += object->x + 1;
  y += object->y + 1;
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

// Copies the first sprite palette into background palette 14, then colour 4
// of background palette 15 over its colour 14.
void Palette_CopyObjectBankToBackground14(void) {
  Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010, REG_DMA3);
  Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001, REG_DMA3);
}

// Shows the item icons of one page of the item list: entries first to
// first + pageSize - 1, one row of 16 pixels each from (x, y). Hides all the
// others.
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

// Writes the party's coins and the "Coins" label into window (work).
void UiText_DrawWorkValueWithLabel(struct UiWindow *work) {
  UiText_DrawNumberAtOffsetFar(gGameState.coins, 7, work, 8, 0);
  UiText_DrawCharacterAtOffsetFar(MsgCoinsLabel, work, 64, 0);
#if !EDITION_INTERNATIONAL
  UiWindow_DrawDividerLineFar(work, 12, 0, 12, 2);
#endif
}

// Moves and resizes a window, in tiles.
void UiWindow_SetBounds(struct UiWindow *window, s32 x, s32 y, s32 width,
                        s32 height) {
  if (window != NULL) {
    window->width = width;
    window->x = x;
    window->height = height;
    window->y = y;
  }
}

// Marks the window system busy with the menu, or no longer busy.
void UiWork_SetMenuBusy(void) {
  ((struct UiRenderWork *)gWindowWork[0])->menuBusy = 1;
}

void UiWork_ClearMenuBusy(void) {
  ((struct UiRenderWork *)gWindowWork[0])->menuBusy = 0;
}

// Plays a sound and returns 1.
s32 Audio_PlayCueReturnOne(s32 cue) {
  AudioCommand_PlayFar(cue);
  return 1;
}

// Scheduler callback while a menu runs: on Start, plays the cancel sound,
// sets FLAG_MENU_START_PRESSED and removes itself.
void Menu_CancelSoundTick(void) {
  if (gKeysPressed & KEY_START) {
    AudioCommand_PlayFar(SOUND_MENU_CANCEL);
    GameFlag_SetBitFar(FLAG_MENU_START_PRESSED);
    Scheduler_RemoveCallback(Menu_CancelSoundTick);
  }
}

// Starts watching for Start: clears FLAG_MENU_START_PRESSED and adds
// Menu_CancelSoundTick.
void Menu_CancelSoundReset(void) {
  GameFlag_ClearBitFar(FLAG_MENU_START_PRESSED);
  Scheduler_AddOrUpdateCallback(Menu_CancelSoundTick, 0xC80);
}

// Stops watching for Start, if Start has not removed the watch already.
void Menu_EnsureCancelSound(void) {
  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    Scheduler_RemoveCallback(Menu_CancelSoundTick);
  }
}

// Set the window text colour: normal (15), no HP (2) and low HP (4).
void UiPalette_SetColor15(void) { UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL); }

void UiPalette_SetColor2(void) { UiWork_SetParamNibbleFar(TEXT_COLOR_NO_HP); }

void UiPalette_SetColor4(void) { UiWork_SetParamNibbleFar(TEXT_COLOR_LOW_HP); }

// The event work as the item picker sees it: what the player chose, for the
// event script. The shared header has no names for these two fields yet.
struct AssetSelectionResult {
  s32 unk000[0x60];
  u16 chosenItem; // owner << 10 | item id
  u8 unk182[0x18];
  u16 chosenSlot; // the owner's inventory slot
};

// The item picker an event script opens (window command 3): the item menu,
// in which the player chooses one party member's item. Records the choice in
// the event work and returns 1 if one was made.
s32 RunAssetSelectionScreen(void) {
#if EDITION_INTERNATIONAL
  void *backup;
#endif
  struct InventoryMenuState *screen;
  s32 item;
  s32 target;
  s32 owner;
  s32 result;
#if EDITION_INTERNATIONAL
  s32 size;
#endif
  void **process;
  void **cache;

  // Open the menu over the field, which stops moving.
#if EDITION_INTERNATIONAL
  size = 0x2000;
  backup = Runtime_BumpAllocateAlternatePool(size);
#endif
  screen = Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, 0xa70);
  cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_MENU_CONTROL];
  ((struct ObjectSystemWork *)cache[0])->suspended = 1;
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
  UiWindow_InitializeWork(0);
  screen->partyCount = Party_ListActiveOwnersFar(screen->ownerIds);
  ItemMenu_Init(0, 3, 0, 7);
  Resource_LoadPairedBlocks();
  Palette_LightenBankHighlight(14);
  Link_DrawShiftedTilePairFar(0x06002500);
  screen->messageWindow = UiWindow_CreateFar(13, 0, 17, 3, 2);
#if EDITION_INTERNATIONAL
  // Save the window tiles the menu draws over, and fill them with colour 3.
  Scheduler_EnableOverlayCallbacksWithFlags();
  Iwram_CopyWords(backup, (void *)0x06004000, size);
  Iwram_FillWords((void *)0x06004000, size, 0x33333333);
  UiWork_SetAltFlagAndClearTableFar(1);
#endif

  // Let the player choose.
  Menu_CancelSoundReset();
  result = ItemMenu_RunCommands(&owner, &target, &item);
  Menu_EnsureCancelSound();
  if (result == 1) {
    struct AssetSelectionResult *selection =
        cache[HEAP_SLOT_EVENT - HEAP_SLOT_MENU_CONTROL];

    selection->chosenItem = (owner << 10) | (item & ITEM_ID_MASK);
    selection->chosenSlot = screen->selectedSlots[0];
  }

  // Close the menu and give the screen back to the field.
  RenderOutput_ClearListFar(screen->statusWindow);
  process = &cache[HEAP_SLOT_WINDOW - HEAP_SLOT_MENU_CONTROL];
  ((struct UiRenderWork *)*process)->menuBusy = 1;
  ItemMenu_Close();
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  Menu_ResetTwoResourceEntries();
  Runtime_ReleaseHeapBlock(HEAP_SLOT_MENU);
  ((struct ObjectSystemWork *)cache[0])->suspended = 0;
#if EDITION_INTERNATIONAL
  UiWindow_MarkVisibleTileAttributesFar();
  UiWork_SetAltFlagAndClearTableFar(0);
  Iwram_CopyWords((void *)0x06004000, backup, size);
  ((struct UiRenderWork *)*process)->menuBusy = 0;
  MemFree(backup);
  WaitFrames(1);
  Scheduler_DisableOverlayCallbacksWithFlags();
#endif
  WaitFrames(1);
  UiWindow_EraseBorderRectFar(0, 0, 30, 20);
  ((struct UiRenderWork *)*process)->menuBusy = 0;
  Event_ClearInvalidPackedValuesFar();
  return result;
}

// The x of the small message windows "Dropped it.", "You were cursed!" and
// "Removed".
#if EDITION_INTERNATIONAL
#define MODAL_SHORT_X 14
#define MODAL_REMOVED_X 14
#else
#define MODAL_SHORT_X 15
#define MODAL_REMOVED_X 17
#endif

// The most items of one kind a stack holds.
#define INVENTORY_STACK_MAX 30

// The resident IWRAM word copy, called through its address.
typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);

#define IWRAM_COPY_WORDS ((WordCopyFn)(IwramIrqMain + Iwram_CopyWordsOffset))

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src,
                                s32 size) {
  /* FAKEMATCH: needed to match: the copy routine as IwramIrqMain plus its
   * offset, passed first; direct calls share the 332-byte size in r5 and move
   * the routine to r6, and through Iwram_CopyWords the routine is loaded before
   * the source and size (2 instructions reordered). */
  return copy(dst, src, size);
}

// The steps of the item menu's command loop (ItemMenu_RunCommands).
enum ItemMenuStep {
  ITEM_STEP_OWNER = 0,
  ITEM_STEP_ITEM = 1,
  ITEM_STEP_USE = 2,
  ITEM_STEP_EQUIP = 3,
  ITEM_STEP_GIVE = 4,
  ITEM_STEP_DROP = 5,
  ITEM_STEP_TARGET = 6,
  ITEM_STEP_TRADE = 7,
  ITEM_STEP_IDLE = 8, // nothing sets it
  ITEM_STEP_COMMAND = 9,
  ITEM_STEP_INSPECT = 10,
  ITEM_STEP_REMOVE = 11,
  ITEM_STEP_QUANTITY = 12 // nothing sets it
};

// Runs the item menu: choose a party member, one of their items and a command
// (Use, Equip, Give, Drop, Remove or Inspect), then carry it out, until the
// player backs out or presses Start. Returns 1 when an item is to be used
// outside the menu, with its owner, target and item id in the out parameters;
// -1 when the player backed out or pressed Start; 0 otherwise.
s32 ItemMenu_RunCommands(s32 *ownerOut, s32 *targetOut, s32 *itemOut) {
  s32 done;
#if !EDITION_INTERNATIONAL
  s32 owner;
  s32 chosen;
#endif
  s32 selection;
  s32 ret;
  struct BattleUnit *source;
  struct BattleUnit *target;
  struct InventoryMenuState *menu;
  struct ItemDefinition *item;
  struct BattleUnit *sourceCopy;
  struct BattleUnit *targetCopy;
  u32 state;
  s32 command;
  s32 result;
  s32 heldCount;
  s32 amount;
  s32 aborted;
  s32 dropped;
  s32 giveTarget;
  u8 movedSource;
  u8 movedTarget;
  s8 stackSize;

  done = 0;
#if !EDITION_INTERNATIONAL
  owner = 0;
  chosen = 0;
#endif
  selection = 0;
  ret = 0;
  state = ITEM_STEP_OWNER;
  menu = gMenuWork;

  while (done == 0 && GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    switch (state) {
    // Choose whose items to look at.
    case ITEM_STEP_OWNER:
      menu->selectedSlots[0] = 0;
      ItemMenu_SetMsgWin3();
      ItemMenu_SetItemWin3();
      menu->selectedItemIcon->active = ICON_HIDDEN;
      ItemMenu_DrawMsg(0, MsgWhoseItem);
#if EDITION_INTERNATIONAL
      RenderOutput_RedrawSavedRectFar(menu->infoWindow);
      UiText_DrawWorkValueWithLabel(menu->infoWindow);
      command = ItemMenu_PrepOwner(0);
      if (command == -1) {
        selection = command = 0;
        ret = -1;
        done = 1;
      }
#else
      UiText_DrawWorkValueWithLabel(menu->infoWindow);
      owner = ItemMenu_PrepOwner(0);
      if (owner == -1) {
        chosen = selection = owner = 0;
        ret = -1;
        done = 1;
      }
#endif
      RenderOutput_RedrawSavedRectFar(menu->infoWindow);
      ItemMenu_HideAllIcons();
      state = ITEM_STEP_ITEM;
      break;

    // Choose one of their items.
    case ITEM_STEP_ITEM:
      if (ItemMenu_Count(menu->paneOwner[0]) == 0) {
        state = ITEM_STEP_OWNER;
        break;
      }
      ItemMenu_SetMsgWin3();
      ItemMenu_SetItemWin3();
      menu->selectedItemIcon->active = ICON_HIDDEN;
      menu->paneIcons[0]->active = ICON_SHOWN;
      ItemMenu_DrawMsg(0, MsgWhichItem);
      selection = ItemMenu_RunList(0);
      state = ITEM_STEP_OWNER;
      if (selection == -1) {
        break;
      }
      menu->listMode = 0xff;
      state = ITEM_STEP_COMMAND;
      break;

    // Choose what to do with it. An item that starts an effect on the field
    // (BattleFx_HasTriggerFar) closes the menu and is returned to the caller;
    // other usable items are used here.
    case ITEM_STEP_COMMAND:
      command = ItemMenu_ChooseCmd();
      if (command == -1) {
        state = ITEM_STEP_ITEM;
        menu->completionFlag = 1;
      }
      if (command == ITEM_COMMAND_USE) {
        if (BattleFx_HasTriggerFar(menu->selectedItems[0] & ITEM_ID_MASK) !=
            0) {
          done = 1;
#if EDITION_INTERNATIONAL
          *ownerOut = menu->paneOwner[0];
          *targetOut = command;
          *itemOut = menu->selectedItems[0] & ITEM_ID_MASK;
#else
          *ownerOut = owner;
          *targetOut = chosen;
          *itemOut = selection;
#endif
          ret = 1;
          break;
        }
        result =
            Item_ClassifyUseMode(menu->paneOwner[0], menu->selectedItems[0]);
        if (result == 1) {
          state = ITEM_STEP_USE;
        }
        if (result == 2) {
          ItemMenu_Use();
          RenderOutput_ClearListFar(menu->infoWindow);
          InventoryMenu_ShowModalMessage(menu->messageOffset + MsgItemUseResult,
                                         0, -1);
#if !EDITION_INTERNATIONAL
          RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#endif
          menu->paneIcons[0]->active = ICON_HIDDEN;
          menu->itemCount =
              ItemMenu_Collect(UnitGetFar(menu->paneOwner[0]), menu->items, 0);
          ItemMenu_DrawIcons(menu->items, 0);
          state = ITEM_STEP_OWNER;
        }
        if (result == -1 || result == 0) {
          done = 1;
          *ownerOut = menu->paneOwner[0];
          *targetOut = menu->paneOwner[1];
          *itemOut = menu->selectedItems[0] & ITEM_ID_MASK;
          ret = 1;
        }
      }
      if (command == ITEM_COMMAND_EQUIP) {
        state = ITEM_STEP_EQUIP;
      }
      if (command == ITEM_COMMAND_GIVE) {
        state = ITEM_STEP_TARGET;
      }
      if (command == ITEM_COMMAND_DROP) {
        state = ITEM_STEP_DROP;
      }
      if (command == ITEM_COMMAND_REMOVE) {
        state = ITEM_STEP_REMOVE;
      }
      if (command == ITEM_COMMAND_INSPECT) {
        state = ITEM_STEP_INSPECT;
      }
      break;

    // Use the item on the party member chosen.
    case ITEM_STEP_USE:
      ItemMenu_HideAllIcons();
      ItemMenu_SetMsgWin5();
      ItemMenu_SetItemWin5();
      RenderOutput_RedrawSavedRectFar(menu->messageWindow);
      ItemMenu_DrawItemHead();
      UiText_DrawCharacterAtOffsetFar(MsgUseOnWhom, menu->messageWindow, 16,
                                      16);
#if EDITION_INTERNATIONAL
      if (ItemMenu_SelectTarget(0) != -1) {
#else
      chosen = ItemMenu_SelectTarget(0);
      if (chosen != -1) {
#endif
        command = 0;
        if (ItemMenu_IsSpecial(menu->selectedItems[0] & ITEM_ID_MASK) != 0) {
          command = STATUS_PANEL_STATS;
        }
        result = ItemMenu_Use();
        Menu_DrawOwnerStatusPanel(menu->statusWindow, menu->paneOwner[1], 0,
                                  command);
        if (result != -1) {
          RenderOutput_ClearListFar(menu->infoWindow);
          InventoryMenu_ShowModalMessage(menu->messageOffset + MsgItemUseResult,
                                         0, -1);
#if !EDITION_INTERNATIONAL
          RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#endif
          menu->paneIcons[0]->active = ICON_HIDDEN;
          ItemMenu_TryBreak();
          state = ITEM_STEP_ITEM;
        }
        menu->itemCount =
            ItemMenu_Collect(UnitGetFar(menu->paneOwner[0]), menu->items, 0);
        ItemMenu_DrawIcons(menu->items, 0);
        menu->completionFlag = 1;
      } else {
        state = ITEM_STEP_COMMAND;
      }
      break;

    // Give: choose to whom.
    case ITEM_STEP_TARGET:
      ItemMenu_SetMsgWin5();
      ItemMenu_SetItemWin5();
      RenderOutput_RedrawSavedRectFar(menu->messageWindow);
      ItemMenu_DrawItemHead();
      UiText_DrawCharacterAtOffsetFar(MsgGiveToWhom, menu->messageWindow, 16,
                                      16);
#if EDITION_INTERNATIONAL
      giveTarget = ItemMenu_SelectTarget(1);
      state = ITEM_STEP_GIVE;
      if (giveTarget == -1) {
#else
      chosen = ItemMenu_SelectTarget(1);
      state = ITEM_STEP_GIVE;
      if (chosen == -1) {
#endif
        ItemMenu_DrawEquipPreview(menu->paneOwner[0], menu->selectedSlots[0], 0,
                                  menu->paneOwner[0]);
        state = ITEM_STEP_COMMAND;
      }
      break;

    // Drop the item, or some of a stack, after confirming.
    case ITEM_STEP_DROP:
      ItemMenu_HideAllIcons();
      item = Item_GetDirectFar(menu->selectedItems[0] & ITEM_ID_MASK);
      result = 0;
      if ((item->flags & ITEM_STACKABLE) != 0) {
        heldCount = (menu->selectedItems[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
        if (heldCount > 1) {
          ItemMenu_DrawItemHead();
          result = ItemMenu_SelectGiveQuantity(0, heldCount, 1);
        }
      }
      if (result == -1) {
        state = ITEM_STEP_COMMAND;
        break;
      }
      menu->paneOwner[1] = 0;
      Resource_LoadByModeIntoSlotFar(2,
                                     (menu->selectedItems[0] & ITEM_ID_MASK) |
                                         (result << INVENTORY_QUANTITY_SHIFT),
                                     (u8)menu->selectedItemIcon->index, 0);
      menu->selectedItemIcon->active = ICON_SHOWN;
      menu->selectedItemIcon->x = 120;
      menu->selectedItemIcon->y = 28;
      UiIcon_PrepareObject(menu->selectedItemIcon);
      UiWindow_ClearInteriorTilesFar((void *)menu->listWindow, 0, 72, 120, 96);
      RenderOutput_RedrawSavedRectFar(menu->messageWindow);
      if (ItemMenu_ConfirmDrop(selection) == 0) {
#if EDITION_INTERNATIONAL
        command = menu->paneOwner[0];
        UnitGetFar(command);
        for (dropped = 0; dropped < result + 1; dropped++) {
          Inventory_RemoveFar(command, menu->selectedSlots[0]);
          Item_AdjustCounterFar(menu->selectedItems[0] & ITEM_ID_MASK, 1);
        }
        UnitCalcFar(command);
#else
        owner = menu->paneOwner[0];
        UnitGetFar(owner);
        for (dropped = 0; dropped < result + 1; dropped++) {
          Inventory_RemoveFar(owner, menu->selectedSlots[0]);
          Item_AdjustCounterFar(menu->selectedItems[0] & ITEM_ID_MASK, 1);
        }
        UnitCalcFar(owner);
#endif
        ItemMenu_SetItemWin3();
        ItemMenu_RefreshOwner(menu->paneOwner[0], 0);
        menu->selectedItemIcon->active = ICON_HIDDEN;
        menu->paneIcons[0]->active = ICON_HIDDEN;
        WaitFrames(1);
        RenderOutput_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage(MsgDroppedIt, MODAL_SHORT_X, 13);
        menu->completionFlag = 1;
        state = ITEM_STEP_ITEM;
      } else {
        state = ITEM_STEP_COMMAND;
      }
      UnitCalcFar(menu->paneOwner[0]);
      menu->selectedItemIcon->active = ICON_HIDDEN;
      Event_ClearInvalidPackedValuesFar();
      break;

    // Give the item, or some of a stack, to the chosen member. A full
    // inventory offers a trade instead; the new holder may equip it.
    case ITEM_STEP_GIVE:
      aborted = 0;
      item = Item_GetDirectFar(menu->selectedItems[0] & ITEM_ID_MASK);
      if ((item->flags & ITEM_STACKABLE) != 0) {
        heldCount = InventoryMenu_GetItemQuantity(
            menu->paneOwner[1], menu->selectedItems[0] & ITEM_ID_MASK);
        if (heldCount == INVENTORY_STACK_MAX) {
          aborted = 1;
        }
        if (ItemMenu_Count(menu->paneOwner[1]) == INVENTORY_SLOTS &&
            heldCount == 0) {
          state = ITEM_STEP_TRADE;
          break;
        }
        stackSize = (menu->selectedItems[0] >> INVENTORY_QUANTITY_SHIFT) + 1;
        if (aborted == 0) {
          if (heldCount + stackSize > INVENTORY_STACK_MAX) {
            amount = INVENTORY_STACK_MAX - heldCount;
          } else {
            amount = stackSize;
          }
          if (stackSize > 1) {
            result = ItemMenu_SelectGiveQuantity(0, amount, 0);
          } else {
            result = 0;
          }
          if (result == -1) {
            state = ITEM_STEP_TARGET;
            break;
          }
          for (command = 0; command < result + 1; command++) {
            heldCount = Inventory_AddItemFar(
                menu->paneOwner[1],
                menu->selectedItems[0] & (ITEM_ID_MASK | INVENTORY_BROKEN));
            if (heldCount != -1) {
              Inventory_RemoveFar(menu->paneOwner[0], menu->selectedSlots[0]);
              menu->selectedSlots[1] = heldCount;
            } else {
              aborted = 1;
            }
          }
        }
      } else {
        result = Inventory_AddItemFar(menu->paneOwner[1],
                                      menu->selectedItems[0] &
                                          (ITEM_ID_MASK | INVENTORY_BROKEN));
        if (result == -1) {
          state = ITEM_STEP_TRADE;
          break;
        }
        menu->selectedSlots[1] = result;
        result =
            Inventory_RemoveFar(menu->paneOwner[0], menu->selectedSlots[0]);
        if (result == -1) {
          aborted = 1;
        }
      }
      UnitCalcFar(menu->paneOwner[0]);
      UnitCalcFar(menu->paneOwner[1]);
      Unit_RefreshPsynergyFar(menu->paneOwner[0]);
      Unit_RefreshPsynergyFar(menu->paneOwner[1]);
      result = 1;
      if (aborted == 0) {
        menu->paneOwner[0] = menu->paneOwner[1];
        menu->selectedItems[0] &= ITEM_ID_MASK;
#if EDITION_INTERNATIONAL
        ItemMenu_SetMsgWin6();
#endif
        RenderOutput_RedrawSavedRectFar(menu->messageWindow);
        ItemMenu_DrawItemHead();
        result = ItemMenu_ConfirmEquip(0);
      }
      if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) != 0) {
        break;
      }
      ItemMenu_RefreshOwner(menu->paneOwner[1], 1);
      menu->paneIcons[0]->active = ICON_HIDDEN;
      WaitFrames(1);
      if (aborted == 1) {
        RenderOutput_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage(MsgCannotHoldMore, 15, 14);
      } else {
        RenderOutput_ClearListFar(menu->infoWindow);
        if (result == 1) {
          InventoryMenu_ShowModalMessage(MsgGiven, 15, 14);
        } else {
          ItemMenu_DrawEquipPreview(menu->paneOwner[1], menu->selectedSlots[1],
                                    0, menu->paneOwner[1]);
          InventoryMenu_ShowModalMessage(MsgEquippedIt, 15, 14);
          item = Item_GetDirectFar(menu->selectedItems[0]);
          if ((item->flags & ITEM_CURSED) != 0) {
            AudioCommand_PlayFar(103); // the curse sound
            RenderOutput_ClearListFar(menu->infoWindow);
            InventoryMenu_ShowModalMessage(MsgEquippedIt +
                                               7, // "You were cursed!"
                                           MODAL_SHORT_X, 14);
          }
        }
      }
      Event_ClearInvalidPackedValuesFar();
      state = ITEM_STEP_OWNER;
      break;

    // Trade: the receiver gives back one of their items. Both units are saved
    // first and restored if any part of the swap fails.
    case ITEM_STEP_TRADE:
      aborted = 0;
      ItemMenu_SetMsgWin3();
      ItemMenu_SetItemWin3();
      ItemMenu_DrawMsg(0, MsgSwapForWhat);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
      if (ItemMenu_RunList(1) == -1) {
#else
      selection = ItemMenu_RunList(1);
      if (selection == -1) {
#endif
        state = ITEM_STEP_TARGET;
        break;
      }
      source = UnitGetFar(menu->paneOwner[0]);
      target = UnitGetFar(menu->paneOwner[1]);
      sourceCopy = MemAlloc(BATTLE_UNIT_SIZE);
      targetCopy = MemAlloc(BATTLE_UNIT_SIZE);
      CopyWords(IWRAM_COPY_WORDS, sourceCopy, source, BATTLE_UNIT_SIZE);
      CopyWords(IWRAM_COPY_WORDS, targetCopy, target, BATTLE_UNIT_SIZE);
      movedSource = 0;
      while (movedSource <= 29) {
        result =
            Inventory_RemoveFar(menu->paneOwner[0], menu->selectedSlots[0]);
        if (result == 2) {
          break;
        }
        if (result == -1) {
          aborted = 1;
          break;
        }
        movedSource++;
      }
      movedSource++;
      movedTarget = 0;
      while (movedTarget <= 29) {
        if ((menu->selectedItems[1] & INVENTORY_EQUIPPED) != 0) {
          item = Item_GetDirectFar(menu->selectedItems[1] & ITEM_ID_MASK);
          if ((item->flags & ITEM_CANNOT_UNEQUIP) != 0) {
            aborted = 1;
          }
        }
        result =
            Inventory_RemoveFar(menu->paneOwner[1], menu->selectedSlots[1]);
        if (result == 2) {
          break;
        }
        if (result == -1) {
          aborted = 1;
          break;
        }
        movedTarget++;
      }
      movedTarget++;
      while (movedSource != 0) {
        result = Inventory_AddItemFar(menu->paneOwner[1],
                                      menu->selectedItems[0] &
                                          (ITEM_ID_MASK | INVENTORY_BROKEN));
        if (result == -1) {
          aborted = 1;
          break;
        }
        menu->selectedSlots[1] = result;
        movedSource--;
      }
      while (movedTarget != 0) {
        result = Inventory_AddItemFar(menu->paneOwner[0],
                                      menu->selectedItems[1] &
                                          (ITEM_ID_MASK | INVENTORY_BROKEN));
        if (result == -1) {
          aborted = 1;
          break;
        }
        menu->selectedSlots[0] = result;
        movedTarget--;
      }
      WaitFrames(1);
      if (aborted == 1) {
        CopyWords(IWRAM_COPY_WORDS, source, sourceCopy, BATTLE_UNIT_SIZE);
        CopyWords(IWRAM_COPY_WORDS, target, targetCopy, BATTLE_UNIT_SIZE);
        RenderOutput_ClearListFar(menu->infoWindow);
#if defined(TBS_EDITION_FR)
        InventoryMenu_ShowModalMessage(MsgCannotTrade, 11, 14);
        RenderOutput_RedrawSavedRectFar(menu->statusWindow);
#else
        InventoryMenu_ShowModalMessage(MsgCannotTrade, 15, 14);
#endif
      } else {
        UnitCalcFar(menu->paneOwner[0]);
        UnitCalcFar(menu->paneOwner[1]);
        Unit_RefreshPsynergyFar(menu->paneOwner[0]);
        Unit_RefreshPsynergyFar(menu->paneOwner[1]);
        ItemMenu_SetMsgWin5();
#if EDITION_INTERNATIONAL
        ItemMenu_SetMsgWin6();
        ItemMenu_HidePageIcons();
#endif
        RenderOutput_RedrawSavedRectFar(menu->messageWindow);
        menu->paneOwner[0] = menu->paneOwner[1];
        menu->selectedItems[0] &= ITEM_ID_MASK;
        ItemMenu_DrawItemHead();
        result = ItemMenu_ConfirmEquip(0);
        if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
          RenderOutput_ClearListFar(menu->infoWindow);
          ItemMenu_SetItemWin5();
          ItemMenu_RefreshOwner(menu->paneOwner[1], 1);
          if (result == 0) {
            ItemMenu_DrawEquipPreview(menu->paneOwner[1],
                                      menu->selectedSlots[1], 0,
                                      menu->paneOwner[1]);
            InventoryMenu_ShowModalMessage(MsgEquippedIt, 15, 14);
            item = Item_GetDirectFar(menu->selectedItems[0]);
            if ((item->flags & ITEM_CURSED) != 0) {
              AudioCommand_PlayFar(103); // the curse sound
              RenderOutput_ClearListFar(menu->infoWindow);
              InventoryMenu_ShowModalMessage(MsgEquippedIt +
                                                 7, // "You were cursed!"
                                             MODAL_SHORT_X, 14);
            }
          } else {
#if defined(TBS_EDITION_IT)
            InventoryMenu_ShowModalMessage(MsgTraded, 13, 14);
#else
            InventoryMenu_ShowModalMessage(MsgTraded, 15, 14);
#endif
          }
        }
      }
      MemFree(targetCopy);
      MemFree(sourceCopy);
      Event_ClearInvalidPackedValuesFar();
      state = ITEM_STEP_OWNER;
      break;

    // Equip the item.
    case ITEM_STEP_EQUIP:
      result = Inventory_EquipFar(menu->paneOwner[0], menu->selectedSlots[0]);
      if (result == -1) {
        state = ITEM_STEP_ITEM;
        break;
      }
      if (result == -2) {
        RenderOutput_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage(MsgCannotRemoveItem, 0, -1);
#if !EDITION_INTERNATIONAL
        RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#endif
        state = ITEM_STEP_ITEM;
        break;
      }
      UnitCalcFar(menu->paneOwner[0]);
      Unit_RefreshPsynergyFar(menu->paneOwner[0]);
      menu->paneIcons[0]->active = ICON_HIDDEN;
      menu->itemCount =
          ItemMenu_Collect(UnitGetFar(menu->paneOwner[0]), menu->items, 0);
      ItemMenu_DrawIcons(menu->items, 0);
      WaitFrames(1);
      ItemMenu_DrawEquipPreview(menu->paneOwner[0], menu->selectedSlots[0], 0,
                                menu->paneOwner[0]);
      RenderOutput_ClearListFar(menu->infoWindow);
      InventoryMenu_ShowModalMessage(MsgEquippedIt, 15, 8);
      item = Item_GetDirectFar(menu->selectedItems[0]);
      if ((item->flags & ITEM_CURSED) != 0) {
        AudioCommand_PlayFar(103); // the curse sound
        RenderOutput_ClearListFar(menu->infoWindow);
        InventoryMenu_ShowModalMessage(MsgEquippedIt + 7, // "You were cursed!"
                                       MODAL_SHORT_X, 8);
      }
      state = ITEM_STEP_ITEM;
      break;

    // Unequip it.
    case ITEM_STEP_REMOVE:
      UnitGetFar(menu->paneOwner[0])->inventory[menu->selectedSlots[0]] &=
          ~INVENTORY_EQUIPPED;
      UnitCalcFar(menu->paneOwner[0]);
      Unit_RefreshPsynergyFar(menu->paneOwner[0]);
      menu->paneIcons[0]->active = ICON_HIDDEN;
      menu->itemCount =
          ItemMenu_Collect(UnitGetFar(menu->paneOwner[0]), menu->items, 0);
      ItemMenu_DrawIcons(menu->items, 0);
      WaitFrames(1);
      menu->equipPreview = 1;
      ItemMenu_DrawEquipPreview(menu->paneOwner[0], menu->selectedSlots[0], 0,
                                menu->paneOwner[0]);
      menu->equipPreview = 0;
      RenderOutput_ClearListFar(menu->infoWindow);
      InventoryMenu_ShowModalMessage(MsgRemoved, MODAL_REMOVED_X, 8);
      Event_ClearInvalidPackedValuesFar();
      state = ITEM_STEP_ITEM;
      break;

    // Show its description.
    case ITEM_STEP_INSPECT:
      menu->paneIcons[0]->active = ICON_HIDDEN;
      Menu_SelectQuantity(menu->selectedItems[0]);
      RenderOutput_RedrawSavedRectFar(menu->statusWindow);
      ItemMenu_DrawEquipPreview(menu->paneOwner[0], menu->selectedSlots[0], 0,
                                menu->paneOwner[0]);
      menu->paneIcons[0]->active = ICON_SHOWN;
      state = ITEM_STEP_COMMAND;
      break;

    // Not reached from any command: choose a quantity, then do nothing with
    // it.
    case ITEM_STEP_QUANTITY:
      result = ItemMenu_SelectGiveQuantity(0, 30, 0);
      if (result != -1) {
        heldCount = result;
      }
      state = ITEM_STEP_ITEM;
      break;

    case ITEM_STEP_IDLE:
      break;

    default:
      done = 1;
      break;
    }
  }

  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) != 0) {
    ret = -1;
  }
  return ret;
}

// Uses the selected item from the owner in pane 0 on the member in pane 1.
// On failure plays the error sound, shows why in the info window and returns
// -1; otherwise plays the item's animation, recalculates both members and
// returns 1.
s32 ItemMenu_Use(void) {
  struct InventoryMenuState *menu;
  s32 result;

  menu = gMenuWork;
  result =
      Item_Use(menu->selectedSlots[0], menu->paneOwner[0], menu->paneOwner[1]);

  if (result == -1) {
    AudioCommand_PlayFar(SOUND_MENU_ERROR);
    RenderOutput_ClearListFar(menu->infoWindow);
    InventoryMenu_ShowModalMessage(menu->messageOffset + MsgItemUseResult,
                                   result, result);
#if !EDITION_INTERNATIONAL
    RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#endif
    menu->completionFlag = 1;
    return result;
  }

  Item_PlayUseAnimation(menu->selectedItems[0] & ITEM_ID_MASK);
  UnitCalcFar(menu->paneOwner[0]);
  UnitCalcFar(menu->paneOwner[1]);
  return 1;
}

// Sets up the item menu's first screen: the cursor window with the party's
// figures, the info window along the bottom, and an 8 by 2 cursor grid. The
// parameters are not used.
void ItemMenu_Init(s32 x, s32 y, s32 mode, s32 columns) {
  struct InventoryMenuState *menu = gMenuWork;
  s32 index;

  PsynergyMenu_InitializeEntryObjects(UiMenu_CreateCursor(menu), 2, 2, 8, 0);
  for (index = 3; index >= 0; index--)
    menu->ownerY[index] = 30;

  {
    u8 zero = 0;
    s32 style;
    menu->helpWindow = NULL;
    menu->statusWindow = NULL;
    style = 2;
    menu->infoWindow = UiWindow_CreateFar(0, 17, 30, 3, style);
    menu->itemWindow = NULL;
    menu->selectedColumn = zero;
    menu->selectedRow = zero;
    menu->columnCount = 8;
    menu->rowCount = style;
  }
}

// Creates the 32 item icon sprites in window, off screen until placed: slots 0
// to 7 at x 248, the rest at x 256, all at y 168, each showing item graphic
// slot of the same number.
void Menu_SpawnIconEntries(struct InventoryMenuState *state,
                           struct UiWindow *window) {
  struct RenderOutput **output0;
  s32 index0;
  s32 fifth0;
  struct RenderOutput **output1;
  s32 index1;
  s32 fifth1;
  struct RenderOutput **output2;
  s32 index2;
  s32 fifth2;

  index0 = 0;
  fifth0 = 0xA8;
  output0 = &state->entryIcons[0];
  do {
    *output0++ =
        RenderOutput_CreateFromResourceFar(2, index0, window, 0xF8, fifth0);
    index0++;
  } while (index0 <= 7);

  index1 = 8;
  fifth1 = 0xA8;
  output1 = &state->entryIcons[8];
  do {
    *output1++ =
        RenderOutput_CreateFromResourceFar(2, index1, window, 0x100, fifth1);
    index1++;
  } while (index1 <= 15);

  index2 = 16;
  fifth2 = 0xA8;
  output2 = &state->entryIcons[16];
  do {
    *output2++ =
        RenderOutput_CreateFromResourceFar(2, index2, window, 0x100, fifth2);
    index2++;
  } while (index2 <= 31);
}

// Hides all 32 item icons.
void ItemMenu_HideAllIcons(void) {
  s32 hiddenState = ICON_HIDDEN;
  struct RenderOutput **icons =
      ((struct InventoryMenuState *)gMenuWork)->entryIcons;
  s32 slot;

  for (slot = 31; slot >= 0; slot--) {
    struct RenderOutput *icon = *icons++;
    if (icon != 0) {
      icon->active = hiddenState;
    }
  }
}

#if EDITION_INTERNATIONAL
// Hides the item icons in the first column of the five-wide item grid.
void ItemMenu_HidePageIcons(void) {
  struct InventoryMenuState *menu = gMenuWork;
  s32 slot = 0;
  s32 hiddenState = ICON_HIDDEN;
  struct RenderOutput **iconSlot = menu->entryIcons;

  do {
    struct RenderOutput *icon = *iconSlot++;

    if (icon != 0 && slot % 5 == 0) {
      icon->active = hiddenState;
    }
    slot++;
  } while (slot <= 31);
}
#endif

// Closes the item menu: releases the party figures, hides the icons and the
// cursor, and closes every window the menu opened.
void ItemMenu_Close(void) {
  struct InventoryMenuState *menu;
  struct RenderOutput *cursor;

  menu = gMenuWork;
  Menu_ReleaseEntryObjects();
  ItemMenu_HideAllIcons();
  WaitFrames(1);
  cursor = menu->cursor;
  cursor->active = ICON_HIDDEN;
  UiWindow_CloseIfOpen(&menu->mainWindow, 1);
  UiWindow_CloseIfOpen(&menu->itemWindow, 1);
  UiWindow_CloseIfOpen(&menu->messageWindow, 1);
  UiWindow_CloseIfOpen(&menu->statusWindow, 1);
  UiWindow_CloseIfOpen(&menu->helpWindow, 1);
  UiWindow_CloseIfOpen(&menu->infoWindow, 1);
  UiWindow_CloseIfOpen(&menu->equipWindow, 1);
  UiWindow_CloseIfOpen(&menu->listWindow, 1);
  UiWindow_CloseIfOpen(&menu->unknownWindow38, 1);
  UiWindow_CloseIfOpen(&menu->modalWindow, 1);
  UiWindow_CloseIfOpen(&menu->unknownWindow40, 1);
}

// Lets the player choose a party member in pane (0 or 1), starting from the
// member chosen last. Returns the member's id, or -1 when the player backs out.
s32 ItemMenu_PrepOwner(s32 pane) {
  struct InventoryMenuState *menu = gMenuWork;
  s32 result = 0;
  s32 index;
  struct BattleUnit *owner;

  index = menu->paneIndex[pane];
  RenderOutput_ClearListFar(menu->infoWindow);
  menu->paneCount[pane] = menu->partyCount;
  if (index == -1) {
    menu->paneIndex[pane] = result;
    index = 0;
  } else {
    UiMenu_SlideCursor(index * 24 - 10, 16);
  }
  owner = UnitGetFar(menu->ownerIds[index]);
  menu->itemCount = ItemMenu_Collect(owner, menu->items, 0);
  result = ItemMenu_RunOwnerSelection(menu->ownerIds, menu->items);
  UiIcon_PrepareObject(menu->paneIcons[pane]);
  WaitFrames(1);
  return result;
}

// The party member row of the item menu. Left and Right move between members,
// showing each one's items and status; holding R shows their equipment
// instead; L + A sorts the member's items, a new sort order each press; A
// chooses a member who has items and B backs out. Returns the chosen member's
// id, or -1.
s32 ItemMenu_RunOwnerSelection(u16 *ownerIds, u16 *items) {
  struct InventoryMenuState *menu;
  s32 selection;
  s32 count;
  s32 result;
  struct BattleUnit *owner;
  u8 sortMode;
  s32 showEquipment;
  s32 redraw;
  struct UiWindow *window;
  s32 i;

  menu = gMenuWork;
  selection = menu->paneIndex[0];
  count = menu->paneCount[0];
  redraw = 1;
  result = 0;
  sortMode = 0;
  showEquipment = 0;
  owner = UnitGetFar(ownerIds[selection]);
  if (UiWindow_UpdateOrCreate(&menu->itemWindow, 13, 3, 17, 10, 2))
    Menu_SpawnIconEntries(menu, menu->itemWindow);
  if (UiWindow_UpdateOrCreate(&menu->helpWindow, 13, 13, 17, 4, 2)) {
    menu->selectedItemIcon =
        RenderOutput_CreateFromResourceFar(2, 0, menu->helpWindow, 0, result);
    menu->selectedItemIcon->active = ICON_HIDDEN;
  }
  // The help text: L + A sorts, R shows the equipment.
#if EDITION_INTERNATIONAL
  UiText_DrawCharacterAtOffsetFar(MsgArrangeItemsHelp, menu->helpWindow, 0, 0);
  UiText_DrawCharacterAtOffsetFar(MsgArrangeItemsHelp + 1, menu->helpWindow, 0,
                                  8);
#else
  UiText_DrawStringInWindowFar(gItemMenu_ArrangeKeysString, menu->helpWindow, 0,
                               0);
  UiText_DrawCharacterAtOffsetFar(MsgArrangeItemsHelp, menu->helpWindow, 32, 0);
  UiText_DrawStringInWindowFar(gItemMenu_EquipmentKeyString, menu->helpWindow,
                               16, 8);
  UiText_DrawCharacterAtOffsetFar(MsgArrangeItemsHelp + 1, menu->helpWindow, 32,
                                  8);
#endif
  menu->paneIcons[0]->active = redraw; // ICON_SHOWN
  while (!GameFlag_TestFar(FLAG_MENU_START_PRESSED)) {
    selection = (selection + count) % count;
    UiMenu_PositionCursor(selection * 24 - 10, 16);
    if (redraw) {
      sortMode = 0;
      redraw = 0;
      window = menu->statusWindow;
      owner = UnitGetFar(ownerIds[selection]);
      if (showEquipment) {
        menu->itemCount =
            ItemMenu_Collect(UnitGetFar(ownerIds[selection]), menu->items, 0);
        ItemMenu_OpenCategory(ownerIds[selection]);
        Menu_DrawOwnerStatusPanel(window, ownerIds[selection], 0,
                                  STATUS_PANEL_STATS);
      } else {
        ItemMenu_RefreshOwner(ownerIds[selection], 0);
        Menu_DrawOwnerStatusPanel(window, ownerIds[selection], 0,
                                  STATUS_PANEL_EXPERIENCE);
      }
      // Raise the chosen member's figure.
      for (i = 3; i >= 0; i--)
        menu->ownerY[i] = 30;
      menu->ownerY[selection] = 26;
    }
    WaitFrames(1);
    // L + A sorts, cycling through the four sort orders.
    if (gKeysPressed & KEY_A) {
      if (gKeysHeld & KEY_L) {
        sortMode = (sortMode + 4) % 4;
        InventoryMenu_SortByListOrder(owner->inventory, sortMode);
        sortMode++;
        ItemMenu_RefreshOwner(ownerIds[selection], 0);
        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
      } else if (ItemMenu_Count(ownerIds[selection])) {
        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        result = ownerIds[selection];
        break;
      } else {
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
      }
    }
    if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -1;
      break;
    }
    if (gKeysPressed & KEY_R) {
      showEquipment = 1;
      redraw = 1;
    }
    if (!(gKeysHeld & KEY_R) && showEquipment == 1) {
      showEquipment = 0;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_LEFT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      selection--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      selection++;
      redraw = 1;
    }
  }
  menu->paneIndex[0] = selection;
  menu->selectedOwner = ownerIds[selection];
  menu->paneOwner[0] = ownerIds[selection];
  return result;
}

// Shows the equipment view of member ownerId's items in the item window.
void ItemMenu_OpenCategory(s32 ownerId) {
  struct InventoryMenuState *menu;

  menu = gMenuWork;
  ItemMenu_PosCategory();
  RenderOutput_RedrawSavedRectFar(menu->itemWindow);
  ItemMenu_DrawCategory(menu->itemWindow, ownerId, 0);
}

// Chooses the party member to use (mode 0) or give (mode 1) the selected item
// to. Using shows each member's status as the cursor passes; giving shows each
// member's items, how many of this item they already hold, or that they would
// have to trade, and the equipment preview. The party members who can equip
// the item move while the cursor is up. Returns the chosen member's id, or -1
// when the player backs out; giving cannot choose the item's owner.
s32 ItemMenu_SelectTarget(s32 mode) {

  struct InventoryMenuState *menu;
  struct UiWindow *window;
  s32 count;
  s32 selection;
  s32 redraw;
  u8 result;
  s32 shown;
  s32 heldCount;
  struct RenderOutput *marker;

  menu = gMenuWork;
  window = menu->itemWindow;
  selection = menu->paneIndex[1];
  count = menu->partyCount;
  redraw = 1;
  result = 0;
  shown = 0;
  UiWindow_SetBounds(window, 13, 5, 17, 12);
  RenderOutput_RedrawSavedRectFar((void *)menu->itemWindow);
  UnitGetFar(menu->ownerIds[menu->paneIndex[0]]);
  Scheduler_AddOrUpdateCallback(EquipmentMenu_UpdateCompatibilityIndicators,
                                0xc80);
  while (!GameFlag_TestFar(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      redraw = 0;
      selection = (selection + count) % count;
      window = menu->itemWindow;
      UnitGetFar(menu->ownerIds[selection]);
      // The marker over the chosen member.
      marker = menu->paneIcons[1];
      marker->oam.x = marker->x =
          ((menu->mainWindow->x + selection * 3) << 3) - 2;
      // Giving: the member's items and how many of this one they hold.
      if (mode == 1) {
        ItemMenu_RefreshOwner(menu->ownerIds[selection], 1);
        UiWindow_DrawDividerLineFar(window, 0, 9, 16, 9);
        UiWindow_ClearInteriorTilesFar((void *)window, 0, 72, 120, 80);
        if (selection != menu->paneIndex[0]) {
          heldCount = InventoryMenu_GetItemQuantity(
              menu->ownerIds[selection], menu->selectedItems[0] & ITEM_ID_MASK);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) ||                      \
    defined(TBS_EDITION_IT)
          if (ItemMenu_Count(menu->ownerIds[selection]) == INVENTORY_SLOTS &&
              heldCount == 0)
            UiText_DrawCharacterAtOffsetFar(MsgTradeForWhat, window, 0, 72);
          else if (heldCount != 0) {
            UiText_DrawNumberInWindowFar(heldCount, 2, window, 8, 72);
            UiText_DrawCharacterAtOffsetFar(MsgInStock, window, STOCK_LABEL_X,
                                            72);
          } else {
            UiText_DrawCharacterAtOffsetFar(MsgNoneInStock, window, 16, 72);
          }
#else
          if (heldCount != 0) {
            UiText_DrawNumberInWindowFar(heldCount, 2, window, 8, 72);
            UiText_DrawCharacterAtOffsetFar(MsgInStock, window, STOCK_LABEL_X,
                                            72);
          } else {
            UiText_DrawCharacterAtOffsetFar(MsgNoneInStock, window, 16, 72);
          }
          if (ItemMenu_Count(menu->ownerIds[selection]) == INVENTORY_SLOTS &&
              heldCount == 0)
#if EDITION_INTERNATIONAL
            UiText_DrawCharacterAtOffsetFar(MsgTradeForWhat, window, 0, 72);
#else
            UiText_DrawCharacterAtOffsetFar(MsgTradeForWhat, window, 16, 72);
#endif
#endif
        }
        ItemMenu_DrawEquipPreview(menu->paneOwner[0], menu->selectedSlots[0], 0,
                                  menu->ownerIds[selection]);
      }
      // Using: the member's status, and the item's name once.
      if (mode == 0) {
        if (ItemMenu_IsSpecial(menu->selectedItems[0] & ITEM_ID_MASK))
          Menu_DrawOwnerStatusPanel(menu->statusWindow,
                                    menu->ownerIds[selection],
                                    menu->selectedSlots[0], STATUS_PANEL_STATS);
        else
          Menu_DrawOwnerStatusPanel(
              menu->statusWindow, menu->ownerIds[selection],
              menu->selectedSlots[0], STATUS_PANEL_EXPERIENCE);
        if (!GameFlag_TestFar(FLAG_MENU_INFO_KEPT) && !shown) {
#if EDITION_INTERNATIONAL
          RenderOutput_RedrawSavedRectFar((void *)menu->infoWindow);
          UiText_DrawCharacterAtOffsetFar(
              (menu->selectedItems[0] & ITEM_ID_MASK) + MsgItemPlainName,
              menu->infoWindow, 0, 0);
#else
          RenderOutput_ClearListFar((void *)menu->infoWindow);
          UiText_DrawResourceFar((menu->selectedItems[0] & ITEM_ID_MASK) +
                                     MsgItemPlainName,
                                 menu->infoWindow, 0, 0);
#endif
          shown = 1;
        } else {
          GameFlag_ClearBitFar(FLAG_MENU_INFO_KEPT);
        }
      }
    }
    UiMenu_PositionCursor(selection * 24 - 10, 16);
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      if (mode == 1 && selection == menu->paneIndex[0]) {
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
        continue;
      }
      AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
      result = menu->ownerIds[selection];
      break;
    }
    if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = 255;
      break;
    }
    if (gKeysRepeat & KEY_LEFT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      selection--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      redraw = 1;
      selection++;
    }
  }
  marker = menu->paneIcons[1];
  menu->paneIndex[1] = selection;
  UiIcon_PrepareObject(marker);
  marker->active = ICON_HIDDEN;
  EquipmentMenu_StartCompatibilityIndicators();
  WaitFrames(1);
  menu->paneIndex[1] = selection;
  menu->selectedOwner = menu->ownerIds[selection];
  menu->paneOwner[1] = menu->ownerIds[selection];
  return (s8)result;
}

// Scheduler callback while choosing a member to give an item to: every 32
// frames, sets each party figure's animation by whether that member can equip
// the item (3) or not (1).
void EquipmentMenu_UpdateCompatibilityIndicators(void) {
  struct InventoryMenuState *menu;
  s8 memberIndex;

  menu = gMenuWork;
  if ((gFrameCount & 31) == 0 && menu->partyCount != 0) {
    memberIndex = 0;
    do {
      if (Item_CanOwnerEquip(menu->ownerIds[memberIndex],
                             menu->selectedItems[0] & ITEM_ID_MASK) != 0) {
        struct AnimationObject *indicator = menu->ownerObjects[memberIndex];
        AnimationObjects_SelectAnimationFar(indicator, 3);
      } else {
        struct AnimationObject *indicator = menu->ownerObjects[memberIndex];
        AnimationObjects_SelectAnimationFar(indicator, 1);
      }
      memberIndex++;
    } while (memberIndex < menu->partyCount);
  }
}

// Ends EquipmentMenu_UpdateCompatibilityIndicators: puts every party figure
// back to animation 1 and removes the callback.
void EquipmentMenu_StartCompatibilityIndicators(void) {
  struct InventoryMenuState *menu;
  s8 memberIndex;

  menu = gMenuWork;
  if (menu->partyCount != 0) {
    memberIndex = 0;
    do {
      struct AnimationObject *indicator = menu->ownerObjects[memberIndex];
      AnimationObjects_SelectAnimationFar(indicator, 1);
      memberIndex++;
    } while (memberIndex < menu->partyCount);
  }
  Scheduler_RemoveCallback(&EquipmentMenu_UpdateCompatibilityIndicators);
}

// Returns 1 for the items that raise a stat for good (Apple, Hard Nut, Mint
// and Lucky Pepper), whose use shows the stats panel instead of HP and PP.
s32 ItemMenu_IsSpecial(s32 itemId) {
  switch (itemId) {
  case ITEM_APPLE:
  case ITEM_HARD_NUT:
  case ITEM_MINT:
  case ITEM_LUCKY_PEPPER:
    return 1;
  }
  return 0;
}

// Clears the message window and writes message into it.
void ItemMenu_DrawMsg(s32 unused, s32 message) {
  struct InventoryMenuState *menu;

  menu = gMenuWork;
  RenderOutput_RedrawSavedRectFar(menu->messageWindow);
  UiText_DrawCharacterAtOffsetFar(message, menu->messageWindow, 0, 0);
}

// Hides the icon of each of the 32 list entries whose item is 0.
void Menu_HideEmptyEntryIcons(const u16 *items) {
  struct InventoryMenuState *menu = gMenuWork;
  s32 slot;

  for (slot = 0; slot < 32; slot++) {
    if (items[slot] == 0) {
      UiIcon_PrepareObject(menu->entryIcons[slot]);
      menu->entryIcons[slot]->active = ICON_HIDDEN;
    }
  }
}

// Returns how many of member ownerId's 15 inventory slots hold an item.
s32 ItemMenu_Count(s32 ownerId) {
  s32 itemId;
  s32 remaining;
  s32 count;
  u16 *slots;

  count = 0;
  slots = UnitGetFar(ownerId)->inventory;
  remaining = INVENTORY_SLOTS - 1;
  do {
    itemId = ITEM_ID_MASK & *slots;
    slots += 1;
    if (itemId != 0) {
      count += 1;
    }
    remaining -= 1;
  } while (remaining >= 0);
  return count;
}

// Returns how many of item (an item id) member owner holds: the quantity of
// its first stack, or 0. The Spanish and Italian editions add up every stack
// of it.
s32 InventoryMenu_GetItemQuantity(s32 owner, s32 item) {
  struct BattleUnit *unit;
  s32 i;
  s32 quantity;

  quantity = 0;
  unit = UnitGetFar(owner);
  i = 0;
  do {
    if (unit->inventory[i] != 0 &&
        (unit->inventory[i] & ITEM_ID_MASK) == item) {
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
      quantity += (unit->inventory[i] & INVENTORY_QUANTITY_MASK) >>
                  INVENTORY_QUANTITY_SHIFT;
      quantity++;
#else
      quantity = (unit->inventory[i] & INVENTORY_QUANTITY_MASK) >>
                 INVENTORY_QUANTITY_SHIFT;
      quantity++;
      break;
#endif
    }
    i++;
  } while (i < INVENTORY_SLOTS);
  return quantity;
}

// Copies member owner's items into the list items (32 entries), skipping empty
// slots and clearing the rest. Returns how many there are. mode is not used.
s32 ItemMenu_Collect(struct BattleUnit *owner, u16 *items, s32 mode) {
  s32 count;
  s32 i;

  for (i = 0; i < 32; i++) {
    items[i] = 0;
  }
  count = 0;
  for (i = 0; i < INVENTORY_SLOTS; i++) {
    items[i] = 0;
    if (owner->inventory[i] != 0) {
      items[count] = owner->inventory[i];
      count++;
    }
  }
  return count;
}

// Loads each listed item's icon into its item icon sprite, the first 15 entries
// of items, and hides the icons of empty entries. style 0 draws the list's
// icons; any other style the equipment view's (graphics kind 7).
void ItemMenu_DrawIcons(u16 *items, s32 style) {
  s32 remaining;
  u16 *entries;
  struct RenderOutput **icons;
  s32 itemId;

  icons = ((struct InventoryMenuState *)gMenuWork)->entryIcons;
  entries = items;
  remaining = INVENTORY_SLOTS - 1;
  do {
    itemId = *entries++;
    if (itemId != 0) {
      if (style == 0) {
        Resource_LoadByModeIntoSlotFar(2, itemId, (u8)(*icons)->index, 0);
      } else {
        Resource_LoadByModeIntoSlotFar(7, itemId, (u8)(*icons)->index, 0);
      }
    }
    icons++;
    remaining--;
  } while (remaining >= 0);
  Menu_HideEmptyEntryIcons(items);
}

// Shows member ownerId's items in the item window, laid out for layout mode
// (see ItemMenu_RefreshEntry), or "no items" when there are none.
void ItemMenu_RefreshOwner(s32 ownerId, s32 mode) {
  struct InventoryMenuState *menu;
  struct BattleUnit *owner;
  u16 *items;

  menu = gMenuWork;
  owner = UnitGetFar(ownerId);
  items = menu->items;
  menu->itemCount = ItemMenu_Collect(owner, items, 0);
  RenderOutput_RedrawSavedRectFar(menu->itemWindow);
  ItemMenu_RefreshEntry(mode);
  ItemMenu_DrawIcons(items, 0);
  if (ItemMenu_Count(ownerId) == 0)
    UiText_DrawCharacterAtOffsetFar(MsgItemMenuEmpty, menu->itemWindow, 8, 24);
}

// Does nothing; nothing calls it.
UNUSED void InventoryMenu_NoOp(void) {}

// Shows in the status panel what the item in owner's inventory slot would mean
// to member target: for a usable item, target's status; for equipment, target's
// stats with it equipped; for a Psynergy item, the Psynergy it teaches. When
// target is someone else, the item is put into a copy of target's inventory for
// the preview, making room first if it is full, and target is restored after.
// mode 1 redraws only the lower part of the panel.
void ItemMenu_DrawEquipPreview(s32 owner, s32 slot, s32 mode, s32 target) {
  /* FAKEMATCH: Reusing the expired owner value for the saved unit keeps
     owner in r8 and style in r10; a separate saved pointer swaps those
     registers (44 aligned instructions differ). The type-6 buffer alias
     preserves the native shared draw tail; without it the body adds
     three instructions and seven aligned instructions differ. */
  struct InventoryMenuState *menu = gMenuWork;
  s32 style = 0;
  struct BattleUnit *state = UnitGetFar(owner);
  s32 item = state->inventory[slot];
  s32 equipped;

  if (mode == 1)
    style = STATUS_PANEL_LOWER_ONLY;
  switch (Item_GetDirectFar(item & ITEM_ID_MASK)->type) {
  case 0: // usable items
    Menu_DrawOwnerStatusPanel(menu->statusWindow, target, slot, style);
    break;
  case 1: // weapons
  case 2: // armor
  case 3: // shields, gloves and armlets
  case 4: // helmets
  case 5: // boots
  case 7:
  case 8: // rings
  case 9: // shirts
    if (owner == target) {
      style |= STATUS_PANEL_EQUIP;
      Menu_DrawOwnerStatusPanel(menu->statusWindow, target, slot, style);
    } else {
      s32 size;

      state = UnitGetFar(target);
      size = BATTLE_UNIT_SIZE;
      owner = (s32)MemAlloc(size);
      Iwram_CopyWords((void *)owner, state, size);
      equipped = Inventory_RemoveFirstUnflagged(target);
      if (equipped != 0) {
        item &= ~INVENTORY_EQUIPPED;
        equipped = Inventory_AddItemFar(target, item);
        if (equipped != -1) {
          style |= STATUS_PANEL_EQUIP;
          Menu_DrawOwnerStatusPanel(menu->statusWindow, target, equipped,
                                    style);
        } else {
          Menu_DrawOwnerStatusPanel(menu->statusWindow, target, slot, style);
        }
      } else {
        Menu_DrawOwnerStatusPanel(menu->statusWindow, target, slot, style);
      }
      Iwram_CopyWords(state, (void *)owner, size);
      MemFree((void *)owner);
    }
    break;
  case 6: // Psynergy items
    if (target == owner) {
      style |= STATUS_PANEL_LEARN;
      Menu_DrawOwnerStatusPanel(menu->statusWindow, target, slot, style);
    } else {
      s32 size;
      void *buffer;

      state = UnitGetFar(target);
      size = BATTLE_UNIT_SIZE;
      owner = (s32)MemAlloc(size);
      buffer = (void *)owner;
      Iwram_CopyWords(buffer, state, size);
      equipped = Inventory_RemoveFirstUnflagged(target);
      if (equipped != 0) {
        equipped = Inventory_AddItemFar(target, item);
        if (equipped != -1) {
          style |= STATUS_PANEL_LEARN;
          Menu_DrawOwnerStatusPanel(menu->statusWindow, target, equipped,
                                    style);
        } else {
          Menu_DrawOwnerStatusPanel(menu->statusWindow, target, slot, style);
        }
      } else {
        Menu_DrawOwnerStatusPanel(menu->statusWindow, target, slot, style);
      }
      Iwram_CopyWords(state, (void *)owner, size);
      buffer = (void *)owner;
      MemFree(buffer);
    }
    break;
  }
}

// Makes room in member owner's inventory: if no slot is free, removes the whole
// stack in the first slot that is not equipped. Returns 1 when a slot is free
// (or was freed), 0 otherwise.
s32 Inventory_RemoveFirstUnflagged(s32 owner) {
  struct BattleUnit *unit = UnitGetFar(owner);
  s32 result = 0;
  s32 i;

  for (i = 0; i < INVENTORY_SLOTS; i++) {
    if (unit->inventory[i] == 0) {
      result = 1;
      break;
    }
    if ((unit->inventory[i] & INVENTORY_EQUIPPED) == 0) {
      s32 quantity = unit->inventory[i] >> INVENTORY_QUANTITY_SHIFT;
      s32 count = quantity + 1;
      s32 n;

      if (quantity == 0)
        count = 1;
      for (n = 0; n < count; n++)
        result = Inventory_RemoveFar(owner, i);
      if (result != 2)
        return 0;
      result = 1;
      break;
    }
  }
  return result;
}

#if EDITION_INTERNATIONAL

// The x and y of the item command cursor on column and row of the command grid
// (three commands to a row).
s32 ItemMenu_CmdCursorX(s32 column, s32 row) {
  if (column > 2 || row > 2 || column < 0 || row < 0)
    return 0;
  return gItemMenu_CommandColumnXTable[row * 3 + column];
}

s32 ItemMenu_CmdCursorY(s32 column, s32 row) {
  s32 y;

  (void)column;
  y = 30;
  if (row != 0) {
    y = 38;
  }
  return y;
}
#endif

// Each item command's state in the grid.
#define COMMAND_DISABLED (-1)
#define COMMAND_AVAILABLE 1

// The item command grid: Use, Equip and Details on the top row, Give, Remove
// and Drop below (grid index = row * 3 + column, as ItemMenuCommand counts
// them). Disabled commands are grey and refuse A. The status panel follows the
// cursor: the owner's status on Use, the stats with the item equipped on Equip
// and Details, and without it on the bottom row, where Give also shows who can
// equip it. The cursor starts on the command chosen last, or on the first
// available one. Returns the command chosen, or -1 when the player backs out.
s32 ItemMenu_ChooseCmd(void) {
  struct InventoryMenuState *menu;
  s8 commandStorage[6];
  u16 *redrawFlag;
  s32 column;
  s32 lastCommand;
  s32 row;
  s32 index;
  s32 redraw;
  s8 *commandStates;
  s32 x;
  s32 y;

  index = 0;
  column = 0;
  row = 0;
  menu = gMenuWork;
  redraw = 1;
  commandStates = commandStorage;
  ItemMenu_BuildCmd(commandStates);
  redrawFlag = &menu->flags;
  // Draw the item's name and the command grid, unless they are up already.
  if (*redrawFlag != 1) {
    struct UiWindow *messageWindow;

    ItemMenu_HideAllIcons();
    RenderOutput_RedrawSavedRectFar((void *)menu->listWindow);
    messageWindow = menu->messageWindow;
    ItemMenu_SetMsgWin7();
    RenderOutput_RedrawSavedRectFar((void *)messageWindow);
    UiWindow_DrawDividerLineFar(messageWindow, 0, 3, 16, 3);
    ItemMenu_DrawItemHead();
    ItemMenu_DrawCmd(commandStates, messageWindow);
#if EDITION_INTERNATIONAL
    RenderOutput_RedrawSavedRectFar((void *)menu->infoWindow);
    UiText_DrawCharacterAtOffsetFar((menu->selectedItems[0] & ITEM_ID_MASK) +
                                        MsgItemPlainName,
                                    menu->infoWindow, 0, 0);
#else
    RenderOutput_ClearListFar((void *)menu->infoWindow);
    UiText_DrawResourceFar((menu->selectedItems[0] & ITEM_ID_MASK) +
                               MsgItemPlainName,
                           menu->infoWindow, 0, 0);
#endif
  }
  *redrawFlag = 0;

  // Start on the command chosen last, or on the first one available.
  lastCommand = (s8)menu->listMode;
  if (lastCommand == -1) {
    if (commandStates[ITEM_COMMAND_INSPECT] == COMMAND_AVAILABLE) {
      column = 2;
      row = 0;
    }
    if (commandStates[ITEM_COMMAND_GIVE] == COMMAND_AVAILABLE) {
      column = 0;
      row = 1;
    }
    if (commandStates[ITEM_COMMAND_EQUIP] == COMMAND_AVAILABLE) {
      column = 1;
      row = 0;
    }
    if (commandStates[ITEM_COMMAND_REMOVE] == COMMAND_AVAILABLE) {
      column = 1;
      row = 1;
    }
    if (commandStates[ITEM_COMMAND_USE] == COMMAND_AVAILABLE) {
      column = 0;
      row = 0;
    }
  } else {
    column = (s8)(lastCommand % 3);
    row = (s8)(lastCommand / 3);
    index = row * 3 + column;
  }
#if EDITION_INTERNATIONAL
  x = ItemMenu_CmdCursorX(column, row);
  y = ItemMenu_CmdCursorY(column, row);
  UiMenu_SlideCursor(x, y);
#else
  UiMenu_SlideCursor(column * 40 + 86, row * 8 + 30);
#endif

  for (;;) {
    if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) != 0)
      break;
    if (redraw != 0) {
      redraw = 0;
      column = (column + 3) % 3;
      row = (row + 2) % 2;
      index = row * 3 + column;
      EquipmentMenu_StartCompatibilityIndicators();
      // Preview the item in the status panel for the command under the cursor.
      if (index > 2) {
        menu->equipPreview = 1;
        ItemMenu_DrawEquipPreview(menu->paneOwner[0], menu->selectedSlots[0], 0,
                                  menu->paneOwner[0]);
        if (index == ITEM_COMMAND_GIVE) {
          Scheduler_AddOrUpdateCallback(
              &EquipmentMenu_UpdateCompatibilityIndicators, 0xc80);
        }
      } else if (index != 0) {
        menu->equipPreview = 0;
        ItemMenu_DrawEquipPreview(menu->paneOwner[0], menu->selectedSlots[0], 0,
                                  menu->paneOwner[0]);
      } else {
        Menu_DrawOwnerStatusPanel(menu->statusWindow, menu->paneOwner[0], 0,
                                  STATUS_PANEL_EXPERIENCE);
      }
    }
#if EDITION_INTERNATIONAL
    x = ItemMenu_CmdCursorX(column, row);
    y = ItemMenu_CmdCursorY(column, row);
    UiMenu_PositionCursor(x, y);
#else
    UiMenu_PositionCursor(column * 40 + 86, row * 8 + 30);
#endif
    WaitFrames(1);

    if ((gKeysPressed & KEY_A) != 0) {
      if (commandStates[index] == COMMAND_DISABLED) {
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
      } else {
        switch (index) {
        case ITEM_COMMAND_USE:
          AudioCommand_PlayFar(174); // Use
          break;
        case ITEM_COMMAND_EQUIP:
          AudioCommand_PlayFar(175); // Equip
          break;
        case ITEM_COMMAND_INSPECT:
        case ITEM_COMMAND_GIVE:
        case ITEM_COMMAND_DROP:
          AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
          break;
        case ITEM_COMMAND_REMOVE:
          AudioCommand_PlayFar(117); // Remove
          break;
        default:
          AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
          break;
        }
        menu->listMode = index;
        break;
      }
    }
    if ((gKeysPressed & KEY_B) != 0) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      index = -1;
      menu->listMode = index;
      break;
    }

    if ((gKeysRepeat & KEY_UP) != 0) {
      row -= 1;
      redraw = 1;
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
    } else if ((gKeysRepeat & KEY_DOWN) != 0) {
      row += 1;
      redraw = 1;
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
    } else if ((gKeysRepeat & KEY_RIGHT) != 0) {
      column += 1;
      redraw = 1;
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
    } else if ((gKeysRepeat & KEY_LEFT) != 0) {
      column -= 1;
      redraw = 1;
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
    }
  }

  menu->equipPreview = 0;
  EquipmentMenu_StartCompatibilityIndicators();
  return index;
}

// Fills commandStates (indexed by ItemMenuCommand) for the selected item:
// Use for usable items the owner can use here and for field items, Equip for
// equipment the owner can wear and is not wearing, Remove for worn equipment
// that is not cursed, Give with someone to give to, Drop unless the item is
// indispensable, and neither for cursed equipment being worn.
void ItemMenu_BuildCmd(s8 *commandStates) {
  struct InventoryMenuState *menu;
  struct ItemDefinition *item;

  menu = gMenuWork;
  item = Item_GetDirectFar(ITEM_ID_MASK & menu->selectedItems[0]);

  if (item->type == 0) { // usable items
    commandStates[ITEM_COMMAND_USE] = COMMAND_AVAILABLE;
    commandStates[ITEM_COMMAND_EQUIP] = COMMAND_DISABLED;
  } else {
    commandStates[ITEM_COMMAND_USE] = COMMAND_DISABLED;
    commandStates[ITEM_COMMAND_EQUIP] = COMMAND_AVAILABLE;
  }

  if (Item_ClassifyUseMode(menu->paneOwner[0], menu->selectedItems[0]) != -1)
    commandStates[ITEM_COMMAND_USE] = COMMAND_AVAILABLE;
  else
    commandStates[ITEM_COMMAND_USE] = COMMAND_DISABLED;

  if (menu->selectedItems[0] & INVENTORY_BROKEN)
    commandStates[ITEM_COMMAND_USE] = COMMAND_DISABLED;

  if (Item_CanOwnerEquip(menu->paneOwner[0],
                         menu->selectedItems[0] & ITEM_ID_MASK) == 0) {
    commandStates[ITEM_COMMAND_EQUIP] = COMMAND_DISABLED;
  }

  commandStates[ITEM_COMMAND_GIVE] = COMMAND_AVAILABLE;
  commandStates[ITEM_COMMAND_DROP] = COMMAND_AVAILABLE;
  commandStates[ITEM_COMMAND_INSPECT] = COMMAND_AVAILABLE;

  if (menu->selectedItems[0] & INVENTORY_EQUIPPED) {
    commandStates[ITEM_COMMAND_REMOVE] = COMMAND_AVAILABLE;
    commandStates[ITEM_COMMAND_EQUIP] = COMMAND_DISABLED;
  } else {
    commandStates[ITEM_COMMAND_REMOVE] = COMMAND_DISABLED;
  }

  if (item->flags & ITEM_CANNOT_UNEQUIP) {
    commandStates[ITEM_COMMAND_REMOVE] = COMMAND_DISABLED;
    if (menu->selectedItems[0] & INVENTORY_EQUIPPED) {
      commandStates[ITEM_COMMAND_GIVE] = COMMAND_DISABLED;
      commandStates[ITEM_COMMAND_DROP] = COMMAND_DISABLED;
    }
  }

  if (BattleFx_HasTriggerFar(menu->selectedItems[0] & ITEM_ID_MASK) != 0)
    commandStates[ITEM_COMMAND_USE] = COMMAND_AVAILABLE;

  if (menu->partyCount <= 1)
    commandStates[ITEM_COMMAND_GIVE] = COMMAND_DISABLED;

  if (item->flags & ITEM_INDISPENSABLE)
    commandStates[ITEM_COMMAND_DROP] = COMMAND_DISABLED;
}

// The x of the command grid's middle column.
#if EDITION_INTERNATIONAL
#define ITEM_TEXT_X 32
#else
#define ITEM_TEXT_X 40
#endif

#define TEXT_COLOR_DISABLED 14

// Writes the six command names into window, the disabled ones in grey.
void ItemMenu_DrawCmd(const s8 *commandStates, struct UiWindow *window) {
  s32 disabled;
  s32 value;
  u32 message;

  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  value = commandStates[ITEM_COMMAND_USE];
  disabled = COMMAND_DISABLED;
  if (value == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);

  message = MsgItemCommandUse;
  UiText_DrawCharacterAtOffsetFar(message, window, 0, 24); // "Use"
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_EQUIP] == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);
  UiText_DrawCharacterAtOffsetFar(message + 1, window, ITEM_TEXT_X,
                                  24); // "Equip"
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_GIVE] == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);
  UiText_DrawCharacterAtOffsetFar(message + 2, window, 0, 32); // "Give"
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_DROP] == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);
  UiText_DrawCharacterAtOffsetFar(message + 3, window, 80, 32); // "Drop"
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_INSPECT] == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);
  UiText_DrawCharacterAtOffsetFar(message + 4, window, 80, 24); // "Details"
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  if (commandStates[ITEM_COMMAND_REMOVE] == disabled)
    UiWork_SetParamNibbleFar(TEXT_COLOR_DISABLED);
  // "Remove"
  UiText_DrawCharacterAtOffsetFar(message + 5, window, ITEM_TEXT_X, 32);
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
}

// Says how owner can use itemId (an inventory entry) from the item menu:
// 0  the item works on the field (BattleFx_HasTriggerFar), or its action is
//    usable neither in the field nor in battle, and is used once the menu
//    closes;
// 1  usable in the field: choose a party member to use it on;
// 2  usable in the field on the whole party at once (range 0xff);
// -1 it cannot be used here: it has no action, it is equipment the owner
//    cannot wear or of use type 3, or its action is usable only in battle.
// Usable items always qualify; equipment only when the owner can wear it.
s32 Item_ClassifyUseMode(s32 owner, s32 itemId) {
  s32 itemIdOnly = itemId;
  struct ItemDefinition *def;
  s32 result = -1;

  itemIdOnly &= ITEM_ID_MASK;
  def = Item_GetDirectFar(itemIdOnly);

  if (BattleFx_HasTriggerFar(itemIdOnly) != 0) {
    return 0;
  }

  {
    struct BattleAct *action =
        BattleActGet(def->actionId & OWNER_ACTION_ID_MASK);

    if (def->actionId != 0) {
      if (def->type != 0) { // equipment
        if (def->useType != 3) {
          if (Item_CanOwnerEquip(owner, itemIdOnly) != 0) {
            result = 1;
          }
        }
      } else {
        result = 1;
      }

      if (result == 1) {
        s32 mask = ACTION_USABLE_IN_BATTLE;
        u8 flags = action->flags;

        if ((flags & ACTION_USABLE_IN_FIELD) != 0) {
          u8 range = action->range;
          result = (range == 0xff) ? 2 : 1;
        } else {
          result = (flags & mask) ? -1 : 0;
        }
      }
    }
  }

  return result;
}

// After the selected item is used: an item of use type 2 breaks one time in
// eight, with the break sound and "It broke!".
void ItemMenu_TryBreak(void) {
  struct InventoryMenuState *menu;

  menu = gMenuWork;
  // One time in eight.
  if ((Item_GetDirectFar(ITEM_ID_MASK & menu->selectedItems[0])->useType ==
       2) &&
      (Random16() < 0x2000U)) {
    Inventory_BreakFar(menu->paneOwner[0], menu->selectedSlots[0]);
    AudioCommand_PlayFar(SOUND_ITEM_BREAK);
    InventoryMenu_ShowModalMessage(MsgItemBroke, 0, -1);
#if !EDITION_INTERNATIONAL
    RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#endif
  }
}

// Opens the details window (top left, 13 by 10 tiles) and describes the item
// in pane itemIndex there. Returns 1.
s32 ItemMenu_OpenDetail(s32 itemIndex) {
  struct InventoryMenuState *menu = gMenuWork;

  UiWindow_UpdateOrCreate(&menu->equipWindow, 0, 0, 13, 10, 2);
  Palette_CopyObjectBankToBackground14();

  if (menu->selectedItems[itemIndex] != 0)
    ItemMenu_DrawItemDetails(menu->equipWindow, menu->selectedItems[itemIndex]);

  return 1;
}

// The item menu's Details screen: describes the item entry value in a large
// window (the whole top half in the international editions) and waits for A or
// B. Up and Down move an unused counter from 0 to 4. Returns the counter, or
// -1 after B.
s32 Menu_SelectQuantity(s32 value) {
#if EDITION_INTERNATIONAL
  s32 changed = 1;
  struct InventoryMenuState *menu = gMenuWork;
  struct RenderOutput *itemIcon = menu->selectedItemIcon;
#else
  struct InventoryMenuState *menu = gMenuWork;
  s32 changed = 1;
#endif
  struct UiWindow *window;
  s32 counter = 0;

#if EDITION_INTERNATIONAL
  itemIcon->active = ICON_HIDDEN;
  window = UiWindow_CreateFar(0, 0, 30, 10, 2);
#else
  window = UiWindow_CreateFar(0, 0, 13, 10, 2);
#endif
  // Freeze the party figures and hide the cursor meanwhile.
  Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);

  {
    struct RenderOutput *cursor = menu->cursor;
    cursor->active = ICON_HIDDEN;
  }
  Palette_CopyObjectBankToBackground14();
  WaitFrames(1);

  for (;;) {
    if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) != 0)
      break;

    if (changed != 0) {
      changed = 0;
      counter = (counter + 5) % 5;
      ItemMenu_DrawItemDetails(window, value);
    }

    if (gKeysPressed & KEY_A)
      break;
    if (gKeysPressed & KEY_B) {
      counter = -1;
      break;
    }

    if (gKeysRepeat & KEY_UP) {
      counter -= 1;
      changed = 1;
    }
    if (gKeysRepeat & KEY_DOWN) {
      counter += 1;
      changed = 1;
    }
    WaitFrames(1);
  }

  // Close the window and bring the menu back.
  RenderOutput_RedrawSavedRectFar(window);
  WaitFrames(1);
  UiWork_FinalizeFar((void *)window, 1);
  RenderOutput_RedrawSavedRectFar(menu->mainWindow);
  Palette_LightenBankHighlight(14);
  {
    s32 priority = 0xc80;

    Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, priority);
  }

  {
    struct RenderOutput *cursor = menu->cursor;
#if EDITION_INTERNATIONAL
    cursor->active = ICON_SHOWN;
#else
    u8 one = 1;

    cursor->active = one;
    menu->flags = one;
#endif
  }
#if EDITION_INTERNATIONAL
  UiWindow_DrawFrameFar(13, 0, 17, 10);
#endif

  return counter;
}

// Writes the description of the item entry item into window, one line a row:
// for equipment its Attack and Defense bonuses, then each of its effects
// (flat bonuses, rates as multipliers, elemental Power and Resist with the
// element's icon, and the effects that are only text), whether it is cursed or
// bestows Psynergy, whether it is used up or may break, and for a stack how
// many there are. An item with nothing to say is rare, important, or
// undescribed.
void ItemMenu_DrawItemDetails(struct UiWindow *window, s32 item) {
  struct ItemDefinition *def;
  s32 i;
  s32 hasEffects;
  s32 amount;
  s32 bestowsPsynergy;
  s8 row;

  row = 0;
  bestowsPsynergy = 0;
  def = Item_GetDirectFar(item & ITEM_ID_MASK);
  // Equipment: a heading, then its Attack and Defense bonuses.
  if (def->type != 0) {
    hasEffects = 0;
    if (def->primaryBonus == 0 && def->secondaryBonus == 0) {
      for (i = 0; i < 4; i++) {
        if (def->effects[i].kind != 0 || def->useType == 3) {
          hasEffects = 1;
          break;
        }
      }
    } else {
      hasEffects = 1;
    }
    if (hasEffects == 1) {
      UiText_DrawCharacterAtOffsetFar(MsgEquipEffectHeading, window, 16, 0);
      row = 1;
    }
    if (def->primaryBonus != 0) {
      UiText_DrawCharacterAtOffsetFar(MsgStatLabel, window, 0, row * 8);
      amount = def->primaryBonus;
      ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
      row++;
    }
    if (def->secondaryBonus != 0) {
      UiText_DrawCharacterAtOffsetFar(MsgDefenseLabel, window, 0, row * 8);
      amount = def->secondaryBonus;
      ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
      row++;
    }
  }
  // Each of its effects.
  for (i = 0; i < 4; i++) {
    if (def->effects[i].kind == 0)
      continue;
    amount = def->effects[i].amount;
    switch (def->effects[i].kind) {
    case ITEM_EFFECT_NONE:
      break;
    case ITEM_EFFECT_ADD_HP:
    case ITEM_EFFECT_ADD_HP_REGEN:
    case ITEM_EFFECT_ADD_PP:
    case ITEM_EFFECT_ADD_PP_REGEN:
    case ITEM_EFFECT_ADD_AGILITY:
    case ITEM_EFFECT_ADD_LUCK:
    case 26: // Turns
      UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + MsgEquipEffectName,
                                      window, 0, row * 8);
      ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
      break;
    // Power (15 to 18) and Resist (19 to 22) of the four elements, after
    // the element's icon.
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
      UiWindow_SetTilemapEntryFar(
          window, (u8)((def->effects[i].kind - 15) % 4) + 1, 0, row, 2);
      UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + MsgEquipEffectName,
                                      window, 8, row * 8);
      ItemMenu_DrawStat(amount, 3, window, 64, row * 8);
      break;
    // Rates, in tenths: shown as a multiplier from 0.0 to 1.9.
    case ITEM_EFFECT_SCALE_HP:
    case ITEM_EFFECT_SCALE_HP_REGEN:
    case ITEM_EFFECT_SCALE_PP:
    case ITEM_EFFECT_SCALE_PP_REGEN:
    case ITEM_EFFECT_SCALE_ATTACK:
    case ITEM_EFFECT_SCALE_DEFENSE:
    case ITEM_EFFECT_SCALE_AGILITY:
    case ITEM_EFFECT_SCALE_LUCK:
      UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + MsgEquipEffectName,
                                      window, 0, row * 8);
#if EDITION_INTERNATIONAL
      UiText_DrawStringInWindowFar(gMenu_TimesString, window, 64, row * 8);
      if (amount > 9) {
        UiText_DrawNumberInWindowFar(1, 1, window, 72, row * 8);
        UiText_DrawStringInWindowFar(gMenu_PointString, window, 80, row * 8);
        UiText_DrawNumberInWindowFar(amount - 10, 1, window, 88, row * 8);
      } else {
        UiText_DrawNumberInWindowFar(0, 1, window, 72, row * 8);
        UiText_DrawStringInWindowFar(gMenu_PointString, window, 80, row * 8);
        UiText_DrawNumberInWindowFar(amount, 1, window, 88, row * 8);
      }
#else
      if (amount > 9) {
        UiText_DrawNumberInWindowFar(1, 1, window, 48, row * 8);
        UiText_DrawStringInWindowFar(gMenu_PointString, window, 56, row * 8);
        UiText_DrawNumberInWindowFar(amount - 10, 1, window, 64, row * 8);
      } else {
        UiText_DrawNumberInWindowFar(0, 1, window, 48, row * 8);
        UiText_DrawStringInWindowFar(gMenu_PointString, window, 56, row * 8);
        UiText_DrawNumberInWindowFar(amount, 1, window, 64, row * 8);
      }
      UiText_DrawCharacterAtOffsetFar(MsgEffectRateSuffix, window, 72, row * 8);
#endif
      break;
    case 23: // "Critical Hits increase"
    case 25: // "The curse can be removed"
    case 27: // "Boosts monster encounters"
      UiText_DrawCharacterAtOffsetFar(def->effects[i].kind + MsgEquipEffectName,
                                      window, 0, row * 8);
      break;
    }
    row++;
  }
  // Curses, bestowed Psynergy and how the item is used up.
  if (def->flags & ITEM_CURSED) {
    UiText_DrawCharacterAtOffsetFar(MsgItemCursed, window, 0, row * 8);
    row++;
  }
  if (def->useType == 3) { // bestows Psynergy
    UiText_DrawCharacterAtOffsetFar(MsgBestowsPsynergy, window, 0, row * 8);
    bestowsPsynergy = 1;
    row++;
  }
  if (def->useType != 4 && def->useType != 0) {
    if (!bestowsPsynergy) {
      UiText_DrawCharacterAtOffsetFar(MsgUsesHeading, window, 16, row * 8);
      row++;
    }
    switch (def->useType) {
    case 0:
      break;
    case 1: // used up
      UiText_DrawCharacterAtOffsetFar(MsgSingleUse, window, 0, row * 8);
      row++;
      break;
    case 2: { // may break
      s32 m;

      if (item & INVENTORY_BROKEN) {
        m = MsgBrokenNotice;
        UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
        m++;
        UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
      } else {
        m = MsgMightBreak;
        UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
        m++;
        UiText_DrawCharacterAtOffsetFar(m, window, 0, row * 8);
        row++;
      }
      break;
    }
    }
  }
  // How many there are in a stack.
  if (def->flags & ITEM_STACKABLE) {
    if (row != 0)
      row++;
    i = MsgNumberHeading;
    UiText_DrawCharacterAtOffsetFar(i, window, 16, row * 8);
    row++;
    amount = (item & INVENTORY_QUANTITY_MASK) / INVENTORY_QUANTITY_STEP;
#if EDITION_INTERNATIONAL
    UiWork_PushValueSlotFar(amount + 1, 5);
    UiText_DrawCharacterAtOffsetFar(i + 1, window, 0, row * 8);
#else
    UiText_DrawNumberInWindowFar(amount + 1, 2, window, 0, row * 8);
    UiText_DrawCharacterAtOffsetFar(i + 1, window, 16, row * 8);
#endif
    row++;
  }
  // Nothing written yet: rare, important, or nothing known.
  hasEffects = 0;
  if (row == 0) {
    if (def->flags & 4) { // rare
      UiText_DrawCharacterAtOffsetFar(MsgRareItem, window, 0, 0);
      hasEffects = 1;
    }
    if (!hasEffects) {
      if (def->flags & ITEM_INDISPENSABLE) {
        UiText_DrawCharacterAtOffsetFar(MsgImportantItem, window, 0, 0);
        hasEffects = 1;
      }
      if (!hasEffects)
        UiText_DrawCharacterAtOffsetFar(MsgDetailsUnknown, window, 0, 0);
    }
  }
}

// Writes a signed stat change at (x, y) of window: the number, with a plus or
// minus sign just before its first digit.
void ItemMenu_DrawStat(s32 delta, s32 unused, struct UiWindow *window, s32 x,
                       s32 y) {
  s32 digits;
  s32 magnitude;

  UiText_DrawNumberInWindowFar(delta, 3, window, x, y);
  // Count the digits of |delta| to place the sign.
  digits = 1;
  magnitude = delta;
  if (delta < 0) {
    magnitude = -delta;
  }
  if (magnitude > 9) {
    digits = 2;
  }
  magnitude = delta;
  if (delta < 0) {
    magnitude = -delta;
  }
  if (magnitude > 99) {
    digits = 3;
  }
  if (delta > 0) {
    UiText_DrawStringInWindowFar(&gMenu_PlusSignString, window,
                                 x - digits * 8 + 16, y);
  } else {
    UiText_DrawStringInWindowFar(&gMenu_MinusSignString, window,
                                 x - digits * 8 + 16, y);
  }
}

// Moves the item window to row 5 of the right-hand side.
void ItemMenu_SetItemWin5(void) {
  UiWindow_SetBounds(
      (struct UiWindow *)((struct InventoryMenuState *)gMenuWork)->itemWindow,
      13, 5, 17, 10);
}

// Moves the item window to row 3 of the right-hand side.
void ItemMenu_SetItemWin3(void) {
  UiWindow_SetBounds(
      (struct UiWindow *)((struct InventoryMenuState *)gMenuWork)->itemWindow,
      13, 3, 17, 10);
}

// Makes the message window, along the top right, 5 rows tall.
void ItemMenu_SetMsgWin5(void) {
  UiWindow_SetBounds((struct UiWindow *)((struct InventoryMenuState *)gMenuWork)
                         ->messageWindow,
                     13, 0, 17, 5);
}

// Makes the message window 6 rows tall.
void ItemMenu_SetMsgWin6(void) {
  UiWindow_SetBounds((struct UiWindow *)((struct InventoryMenuState *)gMenuWork)
                         ->messageWindow,
                     13, 0, 17, 6);
}

// Makes the message window 7 rows tall.
void ItemMenu_SetMsgWin7(void) {
  UiWindow_SetBounds((struct UiWindow *)((struct InventoryMenuState *)gMenuWork)
                         ->messageWindow,
                     13, 0, 17, 7);
}

// Makes the message window 3 rows tall.
void ItemMenu_SetMsgWin3(void) {
  UiWindow_SetBounds((struct UiWindow *)((struct InventoryMenuState *)gMenuWork)
                         ->messageWindow,
                     13, 0, 17, 3);
}

// The hand cursor and the "How many?" question of the quantity chooser.
#if EDITION_INTERNATIONAL
#define GIVE_CURSOR_X 128
#define GIVE_ASK_X 32
#define GIVE_ASK_Y 0
#else
#define GIVE_CURSOR_X 88
#define GIVE_ASK_X 0
#define GIVE_ASK_Y 32
#endif

// Asks how many of the selected item stack to give or drop: Left and Right
// choose a number from 1 to range (shown as base + 1 up to base + range on a
// dial of digit sprites), with the owner's remaining count and, when giving
// (single 0), the receiver's new count beside their names. Returns the number
// less one, or -1 when the player backs out.
s32 ItemMenu_SelectGiveQuantity(s32 base, s32 range, s32 single) {
  s32 quantity = base;
  s32 block;
  s32 changed;
  s32 receiverCount;
  s32 ownerCount;
  struct InventoryMenuState *menu = gMenuWork;
  void *tiles = Runtime_AllocateBlock(14, 0x400);
  struct UiWindow *window;
  struct RenderOutput *rightDial;

  changed = 1;
  receiverCount = 0;
  window = menu->messageWindow;
  ItemMenu_SetMsgWin7();
  RenderOutput_RedrawSavedRectFar(window);
  // How many each member holds now.
  if (single == 0)
    receiverCount = InventoryMenu_GetItemQuantity(
        menu->paneOwner[1], menu->selectedItems[0] & ITEM_ID_MASK);
  ownerCount = InventoryMenu_GetItemQuantity(
      menu->paneOwner[0], menu->selectedItems[0] & ITEM_ID_MASK);
  // Two dial sprites, left and right of the number.
  block = Resource_FindFreeEntry();
  if (block != 96) { // 96: no free VRAM block
    VramCache_Load(block, 256, 0);
    RenderOutput_CreateFar(block, 0x40004000, window, 48, 32);
    rightDial = RenderOutput_CreateFar(block, 0x40004000, window, 80, 32);
    rightDial->oam.index += 4;
    UiMenu_SlideCursor(GIVE_CURSOR_X, 40);
    while (!GameFlag_TestFar(FLAG_MENU_START_PRESSED)) {
      if (changed) {
        changed = 0;
        quantity = (range + quantity) % range;
        RenderOutput_RedrawSavedRectFar(window);
        UiText_DrawCharacterAtOffsetFar(MsgGiveHowMany, window, GIVE_ASK_X,
                                        GIVE_ASK_Y);
        Dma_Set(gMenu_QuantityTiles, tiles, 0x84000040, REG_DMA3);
        Shop_FillSelectorFar(30, 14, tiles);
        Shop_FillSelectorFar(range + base, 0, tiles);
        Shop_FillSelectorFar(base + quantity + 1, 10, tiles);
        Shop_FillSelectorFar(base, 2, tiles);
        VramCache_Load(block, 256, tiles);
        UiText_DrawNumberInWindowFar(quantity + 1, 2, window, 32, 32);
        UiText_DrawCharacterAtOffsetFar(
            (menu->selectedItems[0] & ITEM_ID_MASK) + MsgItemName, window, 16,
            8);
        UiText_DrawNumberInWindowFar(ownerCount - quantity - 1, 2, window, 16,
                                     24);
#if EDITION_INTERNATIONAL
        if (single == 0)
          UiText_DrawNumberInWindowFar(receiverCount + quantity + 1, 2, window,
                                       80, 24);
#else
        UiText_DrawCharacterAtOffsetFar(MsgItemCounter, window, 32, 24);
        if (single == 0) {
          UiText_DrawNumberInWindowFar(receiverCount + quantity + 1, 2, window,
                                       80, 24);
          UiText_DrawCharacterAtOffsetFar(MsgItemCounter, window, 96, 24);
        }
#endif
        UiText_DrawStringAtOffsetFar(UnitGetFar(menu->paneOwner[0])->name,
                                     window, 16, 16);
        if (single == 0)
          UiText_DrawStringAtOffsetFar(UnitGetFar(menu->paneOwner[1])->name,
                                       window, 80, 16);
      }
      // A confirms, B backs out, Left and Right change the number.
      if (gKeysPressed & KEY_A) {
        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        break;
      }
      if (gKeysPressed & KEY_B) {
        quantity = -1;
        AudioCommand_PlayFar(SOUND_MENU_CANCEL);
        break;
      }
      UiMenu_PositionCursor(GIVE_CURSOR_X, 40);
      if (gKeysRepeat & KEY_LEFT) {
        quantity--;
        changed = 1;
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      }
      if (gKeysRepeat & KEY_RIGHT) {
        quantity++;
        changed = 1;
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      }
      WaitFrames(1);
    }
  }
  RenderOutput_RedrawSavedRectFar(window);
  RenderOutput_ClearListFar((void *)window);
  Runtime_ReleaseHeapBlock(14);
  menu->selectedItemIcon->active = ICON_HIDDEN;
  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED))
    quantity = -1;
  return quantity;
}

// Shows the selected item at the top of the message window: its icon, its
// owner's name and its name.
void ItemMenu_DrawItemHead(void) {
  struct InventoryMenuState *menu = gMenuWork;

  Resource_LoadByModeIntoSlotFar(2, menu->selectedItems[0],
                                 (u8)menu->selectedItemIcon->index, 0);
  menu->selectedItemIcon->active = ICON_SHOWN;
  menu->selectedItemIcon->x = 112;
  menu->selectedItemIcon->y = 8;
  UiIcon_PrepareObject(menu->selectedItemIcon);
  UiText_DrawStringAtOffsetFar(UnitGetFar(menu->paneOwner[0])->name,
                               menu->messageWindow, 16, 0);
  UiText_DrawCharacterAtOffsetFar((menu->selectedItems[0] & ITEM_ID_MASK) +
                                      MsgItemName,
                                  menu->messageWindow, 16, 8);
}

// Asks whether to drop item (an inventory entry): its name, the question, and
// Yes or No, No first. Returns 0 for Yes and 1 for No, B or Start.
s32 ItemMenu_ConfirmDrop(s32 item) {
  struct UiWindow *window;
  s32 itemId;
  s32 question;
  s32 answer;
  s32 rows = 2;
  s32 selection = 1;
  s32 changed = 1;

  window = UiWindow_CreateFar(13, 3, 17, 10, 2);
  itemId = item & ITEM_ID_MASK;
  Item_GetDirectFar(itemId);
  UiText_DrawCharacterAtOffsetFar(itemId + MsgItemName, window, 24, 0);
  question = MsgConfirmDrop;
#if EDITION_INTERNATIONAL
  UiText_DrawCharacterAtOffsetFar(question, window, 0, 16);
#else
  UiText_DrawCharacterAtOffsetFar(question, window, 8, 16);
#endif
  question++;
#if EDITION_INTERNATIONAL
  UiText_DrawCharacterAtOffsetFar(question, window, 0, 24);
#else
  UiText_DrawCharacterAtOffsetFar(question, window, 8, 24);
#endif
  answer = MsgYes;
  UiText_DrawCharacterAtOffsetFar(answer, window, 24, 40);
  answer++;
  UiText_DrawCharacterAtOffsetFar(answer, window, 24, 56);
  UiMenu_SlideCursor(104, 86);
  for (;;) {
    if (GameFlag_TestFar(FLAG_MENU_START_PRESSED))
      break;
    if (changed) {
      changed = 0;
      selection = (selection + rows) % rows;
    }
    if (gKeysPressed & KEY_A) {
      AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
      break;
    }
    if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      selection = 1;
      break;
    }
    UiMenu_PositionCursor(104, (selection << 4) + 70);
    if (gKeysRepeat & KEY_UP) {
      selection--;
      changed = 1;
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
    }
    if (gKeysRepeat & KEY_DOWN) {
      selection++;
      changed = 1;
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
    }
    WaitFrames(1);
  }
  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED)) {
    selection = 1;
  }
  UiWork_FinalizeFar((void *)window, 1);
  return selection;
}

// After giving or trading the selected item to the member in pane 1, offers to
// equip it there, showing the stats it would give. Returns 0 when it is
// equipped; 1 when the player says No, backs out, or the member cannot equip
// it, and the member is left as before.
s32 ItemMenu_ConfirmEquip(s32 unused) {
  s32 rows = 2;
  s32 selection = 0;
  s32 changed = 1;
  struct InventoryMenuState *menu = gMenuWork;
  struct BattleUnit *unit = UnitGetFar(menu->paneOwner[1]);
  struct BattleUnit *saved;
  struct UiWindow *window;
  s32 slot;
  s32 owner;

  slot = menu->selectedSlots[1];
  owner = menu->paneOwner[1];
  ItemMenu_DrawEquipPreview(owner, slot, 0, owner);
  saved = MemAlloc(BATTLE_UNIT_SIZE);
  Iwram_CopyWords(saved, unit, BATTLE_UNIT_SIZE);
  window = menu->messageWindow;
  // Equip it now; a result of -1 or -2 means it cannot be equipped.
  if ((u32)(Inventory_EquipFar(menu->paneOwner[1], menu->selectedSlots[1]) +
            2) <= 1) {
    selection = 1;
  } else {
#if EDITION_INTERNATIONAL
    UiText_DrawCharacterAtOffsetFar(MsgYes, window, 24, 24);
    UiText_DrawCharacterAtOffsetFar(MsgYes + 1, window, 72, 24);
    UiWindow_ClearInteriorTilesFar((void *)window, 16, 16, 96, 24);
    UiText_DrawCharacterAtOffsetFar(MsgEquipThisItem, window, 0, 16);
    UiMenu_SlideCursor(110, 32);
#else
    UiText_DrawCharacterAtOffsetFar(MsgYes, window, 96, 0);
    UiText_DrawCharacterAtOffsetFar(MsgYes + 1, window, 96, 16);
    UiWindow_ClearInteriorTilesFar((void *)window, 16, 16, 96, 24);
    UiText_DrawCharacterAtOffsetFar(MsgEquipThisItem, window, 8, 16);
    UiMenu_SlideCursor(184, 5);
#endif
    for (;;) {
      if (GameFlag_TestFar(FLAG_MENU_START_PRESSED))
        break;
      if (changed) {
        changed = 0;
        selection = (selection + rows) % rows;
      }
      if (gKeysPressed & KEY_A) {
        AudioCommand_PlayFar(175); // the Equip sound
        break;
      }
      if (gKeysPressed & KEY_B) {
        AudioCommand_PlayFar(SOUND_MENU_CANCEL);
        selection = 1;
        break;
      }
#if EDITION_INTERNATIONAL
      UiMenu_PositionCursor(selection * 48 + 110, 32);
      if (gKeysRepeat & KEY_LEFT) {
#else
      UiMenu_PositionCursor(184, (selection << 4) + 5);
      if (gKeysRepeat & KEY_UP) {
#endif
        selection--;
        changed = 1;
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      }
#if EDITION_INTERNATIONAL
      if (gKeysRepeat & KEY_RIGHT) {
#else
      if (gKeysRepeat & KEY_DOWN) {
#endif
        selection++;
        changed = 1;
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      }
      WaitFrames(1);
    }
  }
  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED))
    selection = 1;
  // No: put the member back as they were.
  if (selection == 1)
    Iwram_CopyWords(unit, saved, BATTLE_UNIT_SIZE);
  MemFree(saved);
  UnitCalcFar(menu->paneOwner[1]);
  Unit_RefreshPsynergyFar(menu->paneOwner[1]);
  return selection;
}

// Loads the stat arrows (up and down) into two free VRAM blocks, kept in the
// menu work's resource slots.
void Resource_LoadPairedBlocks(void) {
  struct InventoryMenuState *menu = gMenuWork;
  s32 block = Resource_FindFreeEntry();

  menu->resourceSlots[0] = block;
  VramCache_Load(block, 128, gMenu_ArrowTiles);
  block = Resource_FindFreeEntry();
  menu->resourceSlots[1] = block;
  VramCache_Load(block, 128, gMenu_SmallDownArrowTiles);
}

// Fills result with the item list of the member in pane index: their unit,
// how many items they hold, and the page and row of the item chosen last for
// them (kept within the list), five items to a page. Returns 1.
s32 ItemMenu_PageResult(struct MenuResult *result, s32 index) {
  s32 unit;
  struct InventoryMenuState *menu = gMenuWork;
  s32 itemCount;
  s32 row;
  s32 page;
  s32 pageCount;
  s32 selected;

  itemCount = ItemMenu_Count(menu->paneOwner[index]);
  unit = (s32)UnitGetFar(menu->paneOwner[index]);
  selected = menu->selectedIndexByOwner[menu->paneOwner[index]];
  if ((selected + 1) > itemCount) {
    selected = itemCount - 1;
  }
  page = selected / LIST_PAGE_SIZE;
  row = selected % LIST_PAGE_SIZE;
  pageCount = itemCount / LIST_PAGE_SIZE;
  if (itemCount % LIST_PAGE_SIZE != 0) {
    pageCount++;
  }
  result->ownerState = unit;
  result->page = page;
  result->pageCount = pageCount;
  result->row = row;
  result->entryCount = itemCount;
  result->selectedIndex = selected;
  return 1;
}

// Updates the item list for the row under the cursor: puts the selected item's
// plain name in the info window and highlights its row of the five. window and
// work are not used.
s32 ItemMenu_DrawItemDetailPage(struct UiWindow *window, void *work,
                                struct MenuResult *state) {
  struct InventoryMenuState *menu;
  s32 page;
  s32 index;
  s32 row;

  page = state->page;
  menu = gMenuWork;
  index = page * LIST_PAGE_SIZE;
  index += state->row;
  state->selectedIndex = index;

#if EDITION_INTERNATIONAL
  RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#else
  RenderOutput_ClearListFar(menu->infoWindow);
#endif
  WaitFrames(1);

  index = state->selectedIndex;
  if (menu->items[index] != 0) {
    s32 name = (menu->items[index] & ITEM_ID_MASK) + MsgItemPlainName;
#if EDITION_INTERNATIONAL
    UiText_DrawCharacterAtOffsetFar(name, menu->infoWindow, 0, 0);
#else
    UiText_DrawResourceFar(name, menu->infoWindow, 0, 0);
#endif
  }

  row = 0;
  do {
    if (row == state->row) {
      Render_SetTilemapFlagRect((void *)menu->itemWindow, 1, row * 2 + 1, 14, 1,
                                14); // highlighted
    } else {
      Render_SetTilemapFlagRect((void *)menu->itemWindow, 1, row * 2 + 1, 14, 1,
                                15); // normal
    }
    row++;
  } while (row < LIST_PAGE_SIZE);

  WaitFrames(1);
  return 1;
}

// The item list's icon column and name column.
#if EDITION_INTERNATIONAL
#define PAGE_X 116
#define ENTRY_X 24
#else
#define PAGE_X 120
#define ENTRY_X 32
#endif

// Draws one page of the item list into window: up to five item names with
// their icons, and the page numbers when there is more than one page.
s32 ItemMenu_DrawNamePage(struct UiWindow *window, s32 unused,
                          const struct MenuResult *state) {
  struct InventoryMenuState *menu = gMenuWork;
  u32 page;
  u32 firstEntry;
  u32 visibleCount;
  u8 row;
  const u16 *entry;

  (void)unused;

  RenderOutput_RedrawSavedRectFar(window);
  UiWindow_DrawDividerLineFar(window, 0, 11, 16, 11);

  page = state->page;
  firstEntry = page * LIST_PAGE_SIZE;
  visibleCount = (u8)(state->entryCount - firstEntry);
  if (visibleCount > LIST_PAGE_SIZE) {
    visibleCount = LIST_PAGE_SIZE;
  }

  Menu_SetPageIcons(LIST_PAGE_SIZE, firstEntry, window, PAGE_X, 34);
  Menu_DrawPageIndicator(window, state->entryCount, LIST_PAGE_SIZE, state->page,
                         15);

  row = 0;
  if (visibleCount > row) {
    entry = &menu->items[firstEntry];
    do {
      UiText_DrawCharacterAtOffsetFar((entry[0] & ITEM_ID_MASK) + MsgItemName,
                                      menu->itemWindow, ENTRY_X, row * 16 + 8);
      row++;
      entry++;
    } while (visibleCount > row);
  }

  return 1;
}

// Return 1, and do nothing; nothing calls them.
UNUSED s32 InventoryMenu_ItemNamePageReturnTrue(void) { return 1; }

UNUSED void InventoryMenu_ItemNamePageNoOp(void) {}

// The item list of the member in pane (0: the owner, 1: the receiver of a
// trade), five items a page. The cursor's item is named in the info window and
// previewed in the status panel, its icon animates, and the party members who
// can equip it move. Up and Down move the cursor, Left and Right turn pages,
// and in pane 0 L and R switch to the previous or next member who has items.
// Returns the chosen inventory entry, or -1 when the player backs out.
s32 ItemMenu_RunList(s32 pane) {
  struct InventoryMenuState *menu;
  struct UiWindow *window;
  s32 changed;
  s32 moved;
  s32 member;
  u8 i;
  s32 result;
  s32 redraw;
  s32 done;
  struct BattleUnit *owner;
  s32 previous;
  struct RenderOutput *icon;
  s32 work[5];
  struct MenuResult state;

  menu = gMenuWork;
  result = 0;
  previous = 0;
  menu->equipPreview = result;
  UiWindow_UpdateOrCreate(&menu->listWindow, 13, 3, 17, 14, 2);
  window = menu->listWindow;
  done = 0;

  while (done == 0 && GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    owner = UnitGetFar(menu->paneOwner[pane]);
    menu->itemCount = ItemMenu_Collect(owner, menu->items, 0);
    ItemMenu_DrawIcons(menu->items, 0);
    menu->selectedItemIcon->active = ICON_HIDDEN;
    ItemMenu_PageResult(&state, pane);
    UiMenu_PositionCursor(98, state.selectedIndex * 16 + 36);
    changed = 1;
    redraw = 1;

    // One member's list, until an item is chosen or the member changes.
    while (GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
      UiMenu_PositionCursor(98, state.row * 16 + 36);

      if (changed != 0) {
        changed = 0;
        if (owner->inventory[previous] != 0) {
          icon = menu->entryIcons[previous];
          UiIcon_PrepareObject(icon);
        }
        if (redraw != 0) {
          WaitFrames(1);
          ItemMenu_DrawNamePage(window, 0, &state);
          if (pane == 0) {
            UiText_DrawCharacterAtOffsetFar(MsgChangeCharacterHelp, window,
                                            HELP_TEXT_X, 88);
          }
          redraw = 0;
        }
        ItemMenu_DrawItemDetailPage(window, work, &state);
        menu->selectedItems[pane] = menu->items[state.selectedIndex];
        ItemMenu_DrawEquipPreview(menu->paneOwner[pane], state.selectedIndex, 0,
                                  menu->paneOwner[pane]);
        if (menu->items[state.selectedIndex] != 0) {
          icon = menu->entryIcons[state.selectedIndex];
          icon->active = 9; // animate the selected icon
          icon->animStep = 0;
          icon->sentinel = 250;
        }
        for (i = 0; i < menu->partyCount; i++) {
          AnimationObjects_SelectAnimationFar(menu->ownerObjects[i], 1);
        }
      }

      // Every 32 frames, the members who can equip the item move.
      if ((gFrameCount & 31) == 0) {
        for (i = 0; i < menu->partyCount; i++) {
          if (Item_CanOwnerEquip(menu->ownerIds[i],
                                 ITEM_ID_MASK &
                                     menu->items[state.selectedIndex]) != 0) {
            AnimationObjects_SelectAnimationFar(menu->ownerObjects[i], 3);
          }
        }
      }

      WaitFrames(1);
      previous = state.selectedIndex;
      moved = Menu_HandlePageInput(0, state.entryCount, LIST_PAGE_SIZE,
                                   &state.row, &state.page);
      if (moved == 1) {
        redraw = 1;
        changed = 1;
      }
      if (moved == 0) {
        changed = 1;
      }
      if (moved == -1) {
        changed = 0;
      }

      if ((gKeysPressed & KEY_A) != 0 && menu->items[state.selectedIndex] != 0) {
        AudioCommand_PlayFar(173); // the item choice sound
        result = menu->items[state.selectedIndex];
        done = 1;
        break;
      }
      if ((gKeysPressed & KEY_B) != 0) {
        AudioCommand_PlayFar(SOUND_MENU_CANCEL);
        result = -1;
        done = 1;
        break;
      }
      // L and R: the previous or next member who has items (pane 0 only).
      if ((gKeysRepeat & KEY_R) != 0 || (gKeysRepeat & KEY_L) != 0) {
        if (pane == 1) {
          AudioCommand_PlayFar(SOUND_MENU_ERROR);
          WaitFrames(1);
        } else {
          AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
          menu->selectedIndexByOwner[menu->paneOwner[pane]] =
              state.selectedIndex;
          member = menu->paneIndex[pane];
          do {
            if ((gKeysRepeat & KEY_R) != 0) {
              member++;
            } else {
              member--;
            }
            member = (member + menu->partyCount) % menu->partyCount;
            menu->selectedOwner = menu->ownerIds[member];
            menu->paneOwner[pane] = menu->ownerIds[member];
            menu->paneIndex[pane] = member;
            menu->itemCount = ItemMenu_Collect(
                UnitGetFar(menu->paneOwner[pane]), menu->items, 0);
          } while (menu->itemCount == 0);
          for (i = 0; i <= 3; i++) {
            menu->ownerY[i] = 30;
          }
          menu->ownerY[member] = 26;
          break;
        }
      }
    }
  }

  UiWindow_ClearInteriorTilesFar((void *)window, 0, 88, 120, 96);
  UiIcon_PrepareObject(menu->gridCursor);
  menu->selectedSlots[pane] = state.selectedIndex;
  menu->selectedIndexByOwner[menu->paneOwner[pane]] = state.selectedIndex;
  menu->selectedItems[pane] = result;
  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) != 0) {
    result = -1;
  }
  WaitFrames(1);
  return result;
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) ||                      \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define PROMPT_SAVES_TILES 1
#define PROMPT_TILES ((void *)0x06004000)
#define PROMPT_TILES_SIZE 0x2000
#endif

// The Psynergy menu an event opens (window command 1): the field menu's
// Psynergy screen over the field, which stops meanwhile. When the player picks
// a Psynergy to use on the field, it is recorded in the event work's
// psynergyRequest (owner << 10 | Psynergy) and 1 is returned; -1 when the
// player backs out.
s32 Menu_OpenConfirmPrompt(void) {
#if defined(PROMPT_SAVES_TILES)
  void *saved = Runtime_BumpAllocateAlternatePool(PROMPT_TILES_SIZE);
#endif
  struct PsynergyMenuState *state =
      Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, 0xa70);
  s32 owner;
  s32 unused;
  s32 psynergy;
  s32 result;

  ((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 1;
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
  UiWindow_InitializeWork(0);
  state->ownerCount = Party_ListActiveOwnersFar(state->ownerTable);
  ItemMenu_Init(0, 3, 0, 7);
  state->messageWindow = UiWindow_CreateFar(13, 0, 17, 3, 2);
  Palette_LightenBankHighlight(14);
  Link_DrawShiftedTilePairFar(0x06002500);
#if defined(PROMPT_SAVES_TILES)
  Scheduler_EnableOverlayCallbacksWithFlags();
  Iwram_CopyWords(saved, PROMPT_TILES, PROMPT_TILES_SIZE);
  Iwram_FillWords(PROMPT_TILES, PROMPT_TILES_SIZE, 0x33333333);
  UiWork_SetAltFlagAndClearTableFar(1);
#endif
  // Let the player choose.
  Menu_CancelSoundReset();
  result = Menu_ResolveSelectedAction(&owner, &unused, &psynergy);
  Menu_EnsureCancelSound();
  if (result == 1) {
    struct EventWork *target = WORK_SLOT(HEAP_SLOT_EVENT);
    u16 request;
    BattleActGet(OWNER_ACTION_ID_MASK & state->paneAction[0]);
    request = psynergy | (owner << 10);
    target->psynergyRequest = request;
  }
  // Close the menu and give the screen back to the field.
  RenderOutput_ClearListFar((void *)state->statusWindow);
  ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menuBusy = 1;
  ItemMenu_Close();
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_MENU);
  ((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 0;
#if defined(PROMPT_SAVES_TILES)
  UiWindow_MarkVisibleTileAttributesFar();
  UiWork_SetAltFlagAndClearTableFar(0);
  Iwram_CopyWords(PROMPT_TILES, saved, PROMPT_TILES_SIZE);
  ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menuBusy = 0;
  MemFree(saved);
  WaitFrames(1);
  Scheduler_DisableOverlayCallbacksWithFlags();
#endif
  WaitFrames(1);
  UiWindow_EraseBorderRectFar(0, 0, 30, 20);
  ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menuBusy = 0;
  Event_ClearInvalidPackedValuesFar();
  return result;
}

// The steps of the Psynergy menu (Menu_ResolveSelectedAction).
enum PsynergyMenuStep {
  PSYNERGY_STEP_OWNER,
  PSYNERGY_STEP_LIST,
  PSYNERGY_STEP_CLASSIFY,
  PSYNERGY_STEP_TARGET,
  PSYNERGY_STEP_APPLY,
};

// The target id meaning the whole party.
#define TARGET_WHOLE_PARTY 9

// Runs the Psynergy menu: choose a party member and one of their Psynergy,
// then use it on a member (or the whole party), or, in the shortcut modes, set
// it as the L or R shortcut. Returns 1 with the owner and Psynergy in the out
// parameters when the chosen Psynergy works on the field, to be used once the
// menu closes; -1 when the player backs out or presses Start; otherwise the
// result of the last use.
s32 Menu_ResolveSelectedAction(s32 *outOwner, s32 *unused, s32 *outAction) {
  struct PsynergyMenuState *work;
  s32 result;
  s32 state;
  s32 done;
  s32 selection;
  s32 classification;
  s32 psynergy;
  s32 chosenTarget;
  struct BattleAct *action;

  state = PSYNERGY_STEP_OWNER;
  done = 0;
  work = gMenuWork;
  result = 0;

  while (done == 0 && GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    switch (state) {
    // Choose whose Psynergy to look at.
    case PSYNERGY_STEP_OWNER: {
      s32 cancelled;
      s32 chosen;

      work->paneRow[0] = 0;
      ItemMenu_DrawMsg(0, MsgPsynergyChooseOwner);
      chosen = PsynergyMenu_SelectPartySlot(0);
      cancelled = -1;
      /* FAKEMATCH: the redundant cancellation copy ends the CSE block, so the
       * cancellation and done constants take the native registers; XOR equality
       * keeps the copy into the result. Plain forms measured 2026-10-13, each
       * differ 12 (r2 and r3 exchanged): chosen == -1 setting result to -1 or
       * to chosen, with either store first, the call tested directly, and
       * chosen == cancelled. */
      if (chosen == -1)
        cancelled = chosen;
      if (((u32)chosen ^ (u32)cancelled) == 0) {
        done = 1;
        result = cancelled;
      }
      RenderOutput_RedrawSavedRectFar(work->infoWindow);
      state = PSYNERGY_STEP_LIST;
      break;
    }

    // Choose one of their Psynergy; in the shortcut modes, set it as the L or
    // R shortcut and start again.
    case PSYNERGY_STEP_LIST:
      WaitFrames(1);
      UnitGetFar(work->ownerIds[0]);
      state = PSYNERGY_STEP_OWNER;
      if (work->psynergyCount != 0) {
        {
          struct PsynergyMenuState *savedWork = work;

          /* FAKEMATCH: the discarded conditional restoration puts labels
           * between the psynergyCount and mode loads, so reload_cse_move2add
           * builds the mode offset from its own constant (movs #154, lsls #2)
           * as the bytes do; both arms disappear. Plain forms measured
           * 2026-10-13: without the block move2add makes it adds r2, #80 from
           * the count offset (differ 51). */
          if (done)
            work = NULL;
          if (done)
            work = savedWork;
        }
        switch (work->mode) {
        case 0: // use
          ItemMenu_DrawMsg(0, MsgPsynergyChooseAbility);
          break;
        case 1: // set the L shortcut
          ItemMenu_DrawMsg(0, MsgPsynergyChooseForL);
          break;
        case 2: // set the R shortcut
          ItemMenu_DrawMsg(0, MsgPsynergyChooseForR);
          break;
        }
        ItemMenu_PosCategory();
        Menu_DrawOwnerStatusPanel(work->statusWindow, work->ownerIds[0], 0,
                                  STATUS_PANEL_EXPERIENCE);
        selection = PsynergyMenu_RunList(0);
        state = PSYNERGY_STEP_OWNER;
        if (selection != -1) {
          state = PSYNERGY_STEP_CLASSIFY;
          if (work->mode != 0) {
            if (work->mode == 1) {
              PsynergyMenu_SetShortcut(work->ownerIds[0], selection, 0);
              RenderOutput_ClearListFar((void *)work->infoWindow);
              InventoryMenu_ShowModalMessage(MsgShortcutSetL, -1, -1);
#if !EDITION_INTERNATIONAL
              RenderOutput_RedrawSavedRectFar(work->infoWindow);
#endif
            } else {
              PsynergyMenu_SetShortcut(work->ownerIds[0], selection, 1);
              RenderOutput_ClearListFar((void *)work->infoWindow);
              InventoryMenu_ShowModalMessage(MsgShortcutSetR, -1, -1);
#if !EDITION_INTERNATIONAL
              RenderOutput_RedrawSavedRectFar(work->infoWindow);
#endif
            }
            state = PSYNERGY_STEP_OWNER;
          }
        }
      }
      break;

    // Choose the member to use it on.
    case PSYNERGY_STEP_TARGET:
      ItemMenu_DrawMsg(0, MsgPsynergyChooseTarget);
      chosenTarget = PsynergyMenu_SelectTarget(0);
      state = PSYNERGY_STEP_APPLY;
      if (chosenTarget == -1) {
        work->flags |= 1;
        state = PSYNERGY_STEP_LIST;
      }
      break;

    // How is it used? On one member, on the whole party, or on the field
    // once the menu closes.
    case PSYNERGY_STEP_CLASSIFY:
      classification = PsynergyMenu_ClassifySelectedPsynergy();
      if (classification == 1) {
        state = PSYNERGY_STEP_TARGET;
        break;
      }
      if (classification == 2) {
        work->ownerIds[1] = TARGET_WHOLE_PARTY;
        state = PSYNERGY_STEP_APPLY;
        break;
      }
      done = 1;
      result = 1;
      *outOwner = work->ownerIds[0];
      *outAction = work->paneAction[0] & OWNER_ACTION_ID_MASK;
      break;

    // Use it: pay its PP, show the result and go back to the list, or to the
    // target when it failed on one member.
    case PSYNERGY_STEP_APPLY:
      chosenTarget = 0;
      psynergy = work->paneAction[0];
      result = BattleEffect_ApplyToTargets(psynergy, work->ownerIds[0],
                                           work->ownerIds[1], 0);
      if (work->ownerIds[1] == TARGET_WHOLE_PARTY) {
        work->ownerIds[1] = work->ownerIds[0];
        chosenTarget = TARGET_WHOLE_PARTY;
      }
      if (result != -1) {
        action = BattleActGet(work->paneAction[0] & OWNER_ACTION_ID_MASK);
        Unit_AddPpFar(work->ownerIds[0], -action->ppCost);
      }
      UnitCalcFar(work->ownerIds[0]);
      if (result != -1) {
        Menu_DrawOwnerStatusPanel(work->statusWindow, work->ownerIds[1], 0,
                                  STATUS_PANEL_EXPERIENCE);
        Ability_PlayUseAnimation(work->paneAction[0] & OWNER_ACTION_ID_MASK);
        RenderOutput_ClearListFar((void *)work->infoWindow);
        InventoryMenu_ShowModalMessage(work->useResult + MsgItemUseResult, 0,
                                       -1);
      } else {
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
        RenderOutput_ClearListFar((void *)work->infoWindow);
        InventoryMenu_ShowModalMessage(work->useResult + MsgItemUseResult,
                                       result, result);
      }
      if (result != -1) {
        result = 1;
        work->flags |= 1;
        state = PSYNERGY_STEP_LIST;
      } else {
        work->skipSlide = 1;
        if (chosenTarget == TARGET_WHOLE_PARTY) {
          work->flags |= 1;
          state = PSYNERGY_STEP_LIST;
        } else {
          state = PSYNERGY_STEP_TARGET;
        }
      }
      break;

    default:
      done = 1;
      break;
    }
  }

  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) != 0) {
    result = -1;
  }
  return result;
}

// Says how the selected Psynergy is used from the menu: 0 on the field once the
// menu closes (its effect has a field event), 2 on the whole party (range
// 0xff), 1 on one chosen member when its target mode is 2, and 0 otherwise.
s32 PsynergyMenu_ClassifySelectedPsynergy(void) {
  struct BattleAct *psynergy;
  s32 diff;
  s32 ret;

  psynergy =
      BattleActGet(OWNER_ACTION_ID_MASK &
                   ((struct PsynergyMenuState *)gMenuWork)->paneAction[0]);
  if (BattleFx_HasMatchingEvent5Far(psynergy->type0c) != 0) {
    return 0;
  }
  ret = 2;
  if (psynergy->range != 0xff) {
    u8 kind = psynergy->targetMode;
    // 1 when kind is 2, else 0.
    diff = kind ^ 2;
    ret = (0 - diff) | diff;
    ret = (u32)ret >> 0x1f;
    ret = 1 - ret;
  }
  return ret;
}

// Lets the player choose a party member in pane partySlot (0 or 1), starting
// from the one chosen last, with each member's Psynergy shown as the cursor
// passes. Returns the chosen member's id, or -1 when the player backs out.
s32 PsynergyMenu_SelectPartySlot(s32 partySlot) {
  /* FAKEMATCH: retain the original scalar tab/icon address lanes. Direct
   * indexed owner fields change the 244-byte native object to 246 bytes and
   * reorder loads around the cursor call. */
  struct PsynergyMenuState *menu = gMenuWork;
  s32 tabOffset = partySlot + ((u8 *)menu->tabIndex - (u8 *)menu);
  s32 iconOffset = partySlot * sizeof(menu->paneIcon[0]) +
                   ((u8 *)menu->paneIcon - (u8 *)menu);
  struct RenderOutput *icon;
  u8 byteVal;
  s32 tab;
  s32 ownerOffset;
  s32 tableOffset;
  s32 ownerId;
  struct BattleUnit *owner;
  u16 *actions;
  u8 *counts;
  s32 badge;
  s32 result;
  s32 iconOffset2;

  result = 0;
  icon = *(struct RenderOutput **)((u8 *)menu + iconOffset);
  icon->active = ICON_SHOWN;
  icon->animStep = result;
  menu->cursorIcon->active = ICON_HIDDEN;
  counts = (u8 *)menu + ((u8 *)menu->tabCounts - (u8 *)menu->tabIndex);
  tab = *(s8 *)((u8 *)menu + tabOffset);
  byteVal = menu->ownerCount;
  counts[tabOffset] = byteVal;

  if (tab == -1) {
    *((u8 *)menu + tabOffset) = 0;
    ownerOffset = 0;
  } else {
    ownerOffset = tab * sizeof(menu->ownerTable[0]);
    UiMenu_SlideCursor(tab * 24 - 10, 16);
  }

  tableOffset = ownerOffset + ((u8 *)menu->ownerTable - (u8 *)menu);
  ownerId = *(u16 *)((u8 *)menu + tableOffset);
  owner = UnitGetFar(ownerId);
  actions = menu->psynergies;
  badge = PsynergyMenu_CollectActions(owner, actions, 2);
  menu->psynergyCount = badge;
  result = PsynergyMenu_SetupActionIcons(menu->ownerTable, actions);

  iconOffset2 = partySlot * sizeof(menu->paneIcon[0]) +
                ((u8 *)menu->paneIcon - (u8 *)menu);
  icon = *(struct RenderOutput **)((u8 *)menu + iconOffset2);
  UiIcon_PrepareObject(icon);
  WaitFrames(1);
  return result;
}

// The party member row of the Psynergy menu. Left and Right move between
// members, showing each one's Psynergy, status and shortcuts; A chooses a
// member who has Psynergy to use, L or R one who has field Psynergy to set as
// that shortcut (menu mode 1 or 2), and B backs out. Returns the chosen
// member's id, or -1.
s32 PsynergyMenu_SetupActionIcons(u16 *ownerIds, u16 *unused) {
  struct PsynergyMenuState *menu;
  s32 selection;
  s32 count;
  s32 redraw;
  s32 result;
  struct BattleUnit *owner;
  struct UiWindow *window;
  s32 shown;
  s32 i;
  u16 *actions;
  s32 shortcutCount;
  s8 fieldCount;

  menu = gMenuWork;
  selection = menu->tabIndex[0];
  count = menu->tabCounts[0];
  redraw = 1;
  result = 0;
  shown = 0;
  menu->mode = result;
  owner = UnitGetFar(ownerIds[selection]);
  if (UiWindow_UpdateOrCreate(&menu->psynergyWindow, 13, 3, 17, 10, 2))
    Menu_SpawnIconEntries((struct InventoryMenuState *)menu,
                          menu->psynergyWindow);
  if (UiWindow_UpdateOrCreate(&menu->shortcutWindow, 13, 13, 17, 4, 2)) {
    menu->cursorIcon = RenderOutput_CreateFromResourceFar(
        2, 0, menu->shortcutWindow, 0, result);
    menu->cursorIcon->active = ICON_HIDDEN;
  }
  while (!GameFlag_TestFar(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      redraw = 0;
      selection = (count + selection) % count;
      window = menu->statusWindow;
      owner = UnitGetFar(ownerIds[selection]);
      PsynergyMenu_RefreshOwnerPsynergy(ownerIds[selection]);
      Menu_DrawOwnerStatusPanel(window, ownerIds[selection], 0,
                                STATUS_PANEL_EXPERIENCE);
      PsynergyMenu_DrawShortcuts(menu->shortcutWindow, ownerIds[selection]);
      PsynergyMenu_CallIconRoutineWithValue((s32)menu, ownerIds[selection]);
      for (i = 3; i >= 0; i--)
        menu->rowPositions[i] = 30;
      menu->rowPositions[selection] = 26; // raise the chosen member
      if (!GameFlag_TestFar(FLAG_MENU_INFO_KEPT) && !shown) {
        RenderOutput_ClearListFar((void *)menu->infoWindow);
#if EDITION_INTERNATIONAL
        RenderOutput_RedrawSavedRectFar((void *)menu->infoWindow);
#endif
        UiText_DrawWorkValueWithLabel(menu->infoWindow);
        shown = 1;
      } else {
        GameFlag_ClearBitFar(FLAG_MENU_INFO_KEPT);
      }
    }
    UiMenu_PositionCursor(selection * 24 - 10, 16);
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      if (menu->psynergyCount) {
        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        result = ownerIds[selection];
        break;
      }
      AudioCommand_PlayFar(SOUND_MENU_ERROR);
    }
    // L or R: set that shortcut, if the member has field Psynergy for it.
    if ((gKeysPressed & KEY_L) || (gKeysPressed & KEY_R)) {
      result = ownerIds[selection];
      if (gKeysPressed & KEY_L)
        menu->mode = 1;
      else
        menu->mode = 2;
      actions = MemAlloc(32 * sizeof(u16));
      fieldCount =
          PsynergyMenu_CollectActions((struct BattleUnit *)owner, actions, 1);
      MemFree(actions);
      shortcutCount = fieldCount;
      if (shortcutCount == 0) {
        menu->mode = shortcutCount;
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
      } else {
        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        break;
      }
    }
    if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -1;
      break;
    }
    if (gKeysRepeat & KEY_LEFT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      selection--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      selection++;
      redraw = 1;
    }
  }
  menu->tabIndex[0] = selection;
  menu->selectedOwner = ownerIds[selection];
  menu->ownerIds[0] = ownerIds[selection];
  return result;
}

// Shows member ownerId's Psynergy icons in the Psynergy window, eight to a
// row, or "no Psynergy" when they have none.
void PsynergyMenu_RefreshOwnerPsynergy(s32 ownerId) {
  u16 *psynergies;
  struct PsynergyMenuState *menu;
  struct BattleUnit *owner;

  menu = gMenuWork;
  owner = UnitGetFar(ownerId);
  psynergies = menu->psynergies;
  menu->psynergyCount = PsynergyMenu_CollectActions(owner, psynergies, 2);
  RenderOutput_RedrawSavedRectFar((void *)menu->psynergyWindow);
  PsynergyMenu_RefreshOwnerEntries(108, 32, 8);
  PsynergyMenu_DrawPsynergyIcons(psynergies);
  if (menu->psynergyCount == 0) {
    UiText_DrawCharacterAtOffsetFar(MsgPsynergyMenuEmpty, menu->psynergyWindow,
                                    0, 24);
  }
}

// Return 1, and do nothing; nothing calls them.
UNUSED s32 PsynergyMenu_ReturnTrue(void) { return 1; }

UNUSED void PsynergyMenu_NoOp(void) {}

// Chooses the party member to use the selected Psynergy on. In mode 0 each
// member's status shows as the cursor passes, with the Psynergy's description
// once. Returns the chosen member's id, or -1 when the player backs out.
s32 PsynergyMenu_SelectTarget(s32 mode) {

  struct PsynergyMenuState *menu;
  s32 selection;
  s32 count;
  s32 redraw;
  s32 result;
  s32 shown;
  struct RenderOutput *marker;

  menu = gMenuWork;
  selection = menu->tabIndex[1];
  count = menu->ownerCount;
  redraw = 1;
  result = 0;
  shown = 0;
  UnitGetFar(menu->ownerTable[menu->tabIndex[0]]);
  UiMenu_SlideCursor(selection * 24 - 10, 16);
  while (!GameFlag_TestFar(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      redraw = 0;
      selection = (selection + count) % count;
      UnitGetFar(menu->ownerTable[selection]);
      marker = menu->paneIcon[1];
      marker->oam.x = marker->x =
          (((menu->auxiliaryWindow)->x + selection * 3) << 3) - 2;
      if (mode == 0) {
        Menu_DrawOwnerStatusPanel(menu->statusWindow,
                                  menu->ownerTable[selection], 0,
                                  STATUS_PANEL_EXPERIENCE);
        PsynergyMenu_CallIconRoutineWithValue((s32)menu,
                                              menu->ownerTable[selection]);
        if (!GameFlag_TestFar(FLAG_MENU_INFO_KEPT) && !shown) {
#if EDITION_INTERNATIONAL
          RenderOutput_RedrawSavedRectFar((void *)menu->infoWindow);
          UiText_DrawCharacterAtOffsetFar(
              (menu->paneAction[0] & OWNER_ACTION_ID_MASK) +
                  MsgAbilityDescription,
              menu->infoWindow, 0, 0);
#else
          RenderOutput_ClearListFar((void *)menu->infoWindow);
          UiText_DrawResourceFar((menu->paneAction[0] & OWNER_ACTION_ID_MASK) +
                                     MsgAbilityDescription,
                                 menu->infoWindow, 0, 0);
#endif
          shown = 1;
        } else {
          GameFlag_ClearBitFar(FLAG_MENU_INFO_KEPT);
        }
      }
    }
    UiMenu_PositionCursor(selection * 24 - 10, 16);
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
      result = menu->ownerTable[selection];
      break;
    }
    if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -1;
      break;
    }
    if (gKeysRepeat & KEY_LEFT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      selection--;
      redraw = 1;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      selection++;
      redraw = 1;
    }
  }
  marker = menu->paneIcon[1];
  menu->tabIndex[1] = selection;
  UiIcon_PrepareObject(marker);
  marker->active = ICON_HIDDEN;
  WaitFrames(1);
  menu->tabIndex[1] = selection;
  menu->selectedOwner = menu->ownerTable[selection];
  menu->ownerIds[1] = menu->ownerTable[selection];
  return result;
}

// A Psynergy shortcut: its owner above bit 10, the Psynergy's id below.
#define SHORTCUT_OWNER_SHIFT 10
#define SHORTCUT_PSYNERGY_MASK 0x3ff

// Sets the L (shortcut 0) or R (shortcut 1) Psynergy shortcut: owner << 10 |
// the Psynergy's id. Returns 1.
s32 PsynergyMenu_SetShortcut(s32 owner, s32 psynergy, s32 shortcut) {
  s32 id = psynergy & OWNER_ACTION_ID_MASK;
  s32 code = (owner << SHORTCUT_OWNER_SHIFT) | id;

  if (shortcut == 0) {
    gGameState.firstShortcut = code;
  } else {
    gGameState.secondShortcut = code;
  }
  return 1;
}

// Writes the L and R Psynergy shortcuts into window: each one's Psynergy and
// owner, or that it is empty, under a heading that says how to change them.
// A name too wide for the line hides the owner (international) or drops the
// colon after "L" and "R" (Japanese). Returns 1.
#if EDITION_INTERNATIONAL

s32 PsynergyMenu_DrawShortcuts(struct UiWindow *window, s32 owner) {
  s32 left;
  s32 top;
  s32 width;
  s32 height;
  s32 wide;

  if (gGameState.firstShortcut != 0 && gGameState.secondShortcut != 0)
    UiText_DrawCharacterAtOffsetFar(MsgShortcutChangeHelp, window, 0, -8);
  else
    UiText_DrawCharacterAtOffsetFar(MsgShortcutLabel, window, 0, -8);

  UiText_GetResourceDimensionsFar(
      (gGameState.firstShortcut & SHORTCUT_PSYNERGY_MASK) + MsgAbilityName,
      &left, &top, &width, &height);
  if (width < 0 || width > 10)
    wide = 1;
  else
    wide = 0;
  if (gGameState.firstShortcut != 0) {
    UiWork_PushValueSlotFar(gGameState.firstShortcut & SHORTCUT_PSYNERGY_MASK,
                            4);
    UiText_DrawCharacterAtOffsetFar(MsgShortcutNameL, window, 0, 0);
    if (wide == 0)
      UiText_DrawStringAtOffsetFar(
          UnitGetFar(gGameState.firstShortcut >> SHORTCUT_OWNER_SHIFT), window,
          80, 0);
  } else {
    UiText_DrawCharacterAtOffsetFar(MsgShortcutEmptyL, window, 0, 0);
  }

  UiText_GetResourceDimensionsFar(
      (gGameState.secondShortcut & SHORTCUT_PSYNERGY_MASK) + MsgAbilityName,
      &left, &top, &width, &height);
  if (width < 0 || width > 10)
    wide = 1;
  else
    wide = 0;
  if (gGameState.secondShortcut != 0) {
    UiWork_PushValueSlotFar(gGameState.secondShortcut & SHORTCUT_PSYNERGY_MASK,
                            4);
    UiText_DrawCharacterAtOffsetFar(MsgShortcutNameR, window, 0, 8);
    if (wide == 0)
      UiText_DrawStringAtOffsetFar(
          UnitGetFar(gGameState.secondShortcut >> SHORTCUT_OWNER_SHIFT), window,
          80, 8);
    UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  } else {
    UiText_DrawCharacterAtOffsetFar(MsgShortcutEmptyR, window, 0, 8);
  }
  return 1;
}
#else

s32 PsynergyMenu_DrawShortcuts(struct UiWindow *window, s32 owner) {
  s32 left;
  s32 top;
  s32 width;
  s32 height;
  u16 wide;

  if (gGameState.firstShortcut != 0 && gGameState.secondShortcut != 0)
    UiText_DrawCharacterAtOffsetFar(MsgShortcutChangeHelp, window, 0, -8);
  else
    UiText_DrawCharacterAtOffsetFar(MsgShortcutLabel, window, 0, -8);

  UiText_GetResourceDimensionsFar(
      (gGameState.firstShortcut & SHORTCUT_PSYNERGY_MASK) + MsgAbilityName,
      &left, &top, &width, &height);
  if (width < 0 || width > 11)
    wide = 1;
  else
    wide = 0;
  if (wide)
    UiText_DrawStringInWindowFar((u8 *)gMenu_ShortcutLString, window, 0, 0);
  else
    UiText_DrawStringInWindowFar((u8 *)gMenu_ShortcutLColonString, window, 0,
                                 0);
  if (gGameState.firstShortcut != 0) {
    if (wide)
      UiText_DrawCharacterAtOffsetFar(
          (gGameState.firstShortcut & SHORTCUT_PSYNERGY_MASK) + MsgAbilityName,
          window, 8, 0);
    else
      UiText_DrawCharacterAtOffsetFar(
          (gGameState.firstShortcut & SHORTCUT_PSYNERGY_MASK) + MsgAbilityName,
          window, 16, 0);
    UiText_DrawStringAtOffsetFar(
        UnitGetFar(gGameState.firstShortcut >> SHORTCUT_OWNER_SHIFT), window,
        80, 0);
  } else {
    UiText_DrawCharacterAtOffsetFar(MsgShortcutEmptyL, window, 24, 0);
  }

  UiText_GetResourceDimensionsFar(
      (gGameState.secondShortcut & SHORTCUT_PSYNERGY_MASK) + MsgAbilityName,
      &left, &top, &width, &height);
  if (width < 0 || width > 11)
    wide = 1;
  else
    wide = 0;
  if (wide)
    UiText_DrawStringInWindowFar((u8 *)gMenu_ShortcutRString, window, 0, 8);
  else
    UiText_DrawStringInWindowFar((u8 *)gMenu_ShortcutRColonString, window, 0,
                                 8);
  if (gGameState.secondShortcut != 0) {
    if (wide)
      UiText_DrawCharacterAtOffsetFar(
          (gGameState.secondShortcut & SHORTCUT_PSYNERGY_MASK) + MsgAbilityName,
          window, 8, 8);
    else
      UiText_DrawCharacterAtOffsetFar(
          (gGameState.secondShortcut & SHORTCUT_PSYNERGY_MASK) + MsgAbilityName,
          window, 16, 8);
    UiText_DrawStringAtOffsetFar(
        UnitGetFar(gGameState.secondShortcut >> SHORTCUT_OWNER_SHIFT), window,
        80, 8);
    UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  } else {
    UiText_DrawCharacterAtOffsetFar(MsgShortcutEmptyR, window, 24, 8);
  }
  return 1;
}
#endif

// Sets up a Psynergy grid screen: the cursor window with the party's figures,
// a 30 by 15 window below with a hidden grid cursor and a portrait, and 16
// Psynergy icon sprites in two rows of eight from (96, 8), on an 8 by 2 grid.
void PsynergyMenu_CreateEntryGrid(void) {
  struct UiWindow *window;
  struct PsynergyMenuState *menu;
  struct RenderOutput *cursor;
  s32 index;
  s32 x;
  s32 y;
  struct RenderOutput **output;

  menu = gMenuWork;
  window = UiMenu_CreateCursor(menu);
  PsynergyMenu_InitializeEntryObjects((void *)window, 2, 2, 8, 0);

  window = UiWindow_CreateFar(0, 5, 30, 15, 2);
  menu->psynergyWindow = window;
  menu->selectedColumn = 0;
  menu->selectedRow = 0;
  menu->columnCount = 8;
  menu->rowCount = 2;

  cursor =
      (struct RenderOutput *)UiIcon_CreateWithResourceVariant(window, 0, 4);
  cursor->active = ICON_HIDDEN;
  menu->entryGridCursor = cursor;
  SideObject_CreateFar(0, 0, 0, window, 0, 0);

  y = 8;
  index = 0;
  output = &menu->entryIcons[0];
  x = 96;
  do {
    *output++ = RenderOutput_CreateFromResourceFar(4, index, window, x, y);
    index++;
    x += 16;
  } while (index <= 7);

  index = 8;
  y = 24;
  output = &menu->entryIcons[8];
  x = 96;
  do {
    *output++ = RenderOutput_CreateFromResourceFar(4, index, window, x, y);
    index++;
    x += 16;
  } while (index <= 15);
}

// Releases the party figures and closes the Psynergy grid screen's windows.
void PsynergyMenu_CloseWindows(void) {
  struct PsynergyMenuState *menu;

  menu = gMenuWork;
  Menu_ReleaseEntryObjects();
  UiWork_FinalizeFar((void *)menu->auxiliaryWindow, 1);
  UiWork_FinalizeFar((void *)menu->psynergyWindow, 1);
  UiWork_FinalizeFar((void *)menu->messageWindow, 1);
}

// Loads each listed Psynergy's icon into its icon sprite (32 entries) and hides
// the icons of empty entries.
void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies) {
  s32 remaining;
  struct RenderOutput **icons;
  u16 *p;
  s32 psynergyId;

  icons = ((struct PsynergyMenuState *)gMenuWork)->entryIcons;
  p = psynergies;
  remaining = 31;
  do {
    psynergyId = *p++;
    if (psynergyId != 0) {
      Resource_LoadByModeIntoSlotFar(4, psynergyId, (u8)(*icons)->index, 0);
    }
    icons++;
    remaining--;
  } while (remaining >= 0);
  Menu_HideEmptyEntryIcons(psynergies);
}

// Lists owner's Psynergy into actions (32 entries, cleared first) and returns
// how many there are. mode 1 lists only the Psynergy that work on the field
// (those that can be shortcuts); mode 2 those with a field effect or usable
// in the field; any other mode those first, then the rest.
s32 PsynergyMenu_CollectActions(struct BattleUnit *owner, u16 *actions,
                                s32 mode) {
  /* FAKEMATCH: the doubled zero store through the byte-offset cursor makes
   * loop.c replace the counter with the pointer (274 bytes); a single indexed
   * clear keeps the counter (266). Measured 2026-10-13: a single store through
   * a pointer walking down from actions + 31, through the byte offset, or as a
   * pre-decrement do-while differs 31 to 33; actions[n] = 0 from 31 down
   * differs 43. */
  s32 n;
  s32 passCount;
  s32 count;
  s32 i;
  s32 pass;

  passCount = (mode != 2) ? 4 : 3;
  for (n = 62; n >= 0; n -= 2) {
    u16 *q = (u16 *)((u8 *)actions + n);
    *q = 0;
    *q = 0;
  }
  count = 0;

  // type0c is the Psynergy's field effect. Passes 1 and 2 add nothing.
  if (mode == 1) {
    for (i = 0; i < 32; i++) {
      if (owner->actionSlots[i].encodedAction != 0) {
        if (BattleActGet(owner->actionSlots[i].encodedAction &
                         OWNER_ACTION_ID_MASK)
                ->type0c != 0) {
          actions[count] = owner->actionSlots[i].encodedAction;
          count++;
        }
      }
    }
  } else {
    for (pass = 0; pass < passCount; pass++) {
      for (i = 0; i < 32; i++) {
        if (owner->actionSlots[i].encodedAction != 0) {
          struct BattleAct *ability = BattleActGet(
              owner->actionSlots[i].encodedAction & OWNER_ACTION_ID_MASK);

          if (pass == 0 &&
              (ability->type0c != 0 ||
               (ability->flags & ACTION_USABLE_IN_FIELD) != 0)) {
            actions[count] = owner->actionSlots[i].encodedAction;
            count++;
          } else if (pass == 1) {
          } else if (pass == 2) {
          } else if (pass == 3 && ability->type0c == 0 &&
                     (ability->flags & ACTION_USABLE_IN_FIELD) == 0) {
            actions[count] = owner->actionSlots[i].encodedAction;
            count++;
          }
        }
      }
    }
  }

  return count;
}

// Fills result with the Psynergy list of the member in pane index: their unit,
// how many Psynergy are listed, and the page and row of the one chosen last for
// them (kept within the list), five to a page. Returns 1.
s32 PsynergyMenu_BuildPageResult(struct MenuResult *result, s32 index) {
  s32 unit;
  struct PsynergyMenuState *menu = gMenuWork;
  s32 entryCount;
  s32 row;
  s32 page;
  s32 pageCount;
  s32 selectedIndex;
  s32 owner;

  unit = (s32)UnitGetFar(menu->ownerIds[index]);
  entryCount = menu->psynergyCount;
  owner = menu->ownerIds[index];
  selectedIndex = menu->selectedIndexByOwner[owner];
  if ((selectedIndex + 1) > entryCount) {
    selectedIndex = entryCount - 1;
  }
  page = selectedIndex / LIST_PAGE_SIZE;
  row = selectedIndex % LIST_PAGE_SIZE;
  pageCount = entryCount / LIST_PAGE_SIZE;
  if (entryCount % LIST_PAGE_SIZE != 0) {
    pageCount++;
  }
  result->ownerState = unit;
  result->page = page;
  result->pageCount = pageCount;
  result->row = row;
  result->entryCount = entryCount;
  result->selectedIndex = selectedIndex;
  return 1;
}

// PsynergyMenuState.flags: the Psynergy list shows the shortcut help (Select
// held).
#define PSYNERGY_MENU_SHORTCUT_HELP 2

// The highlight over the Psynergy list's rows.
#if EDITION_INTERNATIONAL
#define DETAIL_ROW_X 0
#define DETAIL_ROW_WIDTH 15
#else
#define DETAIL_ROW_X 1
#define DETAIL_ROW_WIDTH 14
#endif

// Updates the Psynergy list for the row under the cursor: puts the Psynergy's
// description in the info window, unless a kept message is there, and
// highlights its row of the five. window and work are not used.
s32 PsynergyMenu_DrawDetailPage(struct UiWindow *window, s32 *work,
                                struct MenuResult *state) {
  struct PsynergyMenuState *menu;
  s32 index;
  s32 row;

  menu = gMenuWork;
  index = state->page * LIST_PAGE_SIZE;
  index += state->row;
  state->selectedIndex = index;

  if (GameFlag_TestFar(FLAG_MENU_INFO_KEPT) == 0) {
#if EDITION_INTERNATIONAL
    RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#else
    RenderOutput_ClearListFar(menu->infoWindow);
#endif
    WaitFrames(1);

    index = state->selectedIndex;
    if (menu->psynergies[index] != 0) {
      s32 description =
          (menu->psynergies[index] & 0x1ff) + MsgAbilityDescription;
#if EDITION_INTERNATIONAL
      UiText_DrawCharacterAtOffsetFar(description, menu->infoWindow, 0, 0);
#else
      UiText_DrawResourceFar(description, menu->infoWindow, 0, 0);
#endif
    }
  } else {
    // BUG: clears scene flag 0x2ff instead of FLAG_MENU_INFO_KEPT, so a kept
    // message keeps the descriptions away until another screen clears it.
#ifdef BUGFIX
    GameFlag_ClearBitFar(FLAG_MENU_INFO_KEPT);
#else
    GameFlag_ClearBitFar(0x2ff);
#endif
  }

  row = 0;
  do {
    if (row == state->row) {
      Render_SetTilemapFlagRect((void *)menu->psynergyWindow, DETAIL_ROW_X,
                                row * 2 + 1, DETAIL_ROW_WIDTH, 1,
                                14); // highlighted
    } else {
      Render_SetTilemapFlagRect((void *)menu->psynergyWindow, DETAIL_ROW_X,
                                row * 2 + 1, DETAIL_ROW_WIDTH, 1, 15); // normal
    }
    row++;
  } while (row < LIST_PAGE_SIZE);

  WaitFrames(1);
  return 1;
}

// Draws one page of the Psynergy list into window: up to five names with their
// icons and PP costs, a Psynergy the member cannot afford in the no-HP colour
// and one restricted here in the low-HP colour, the page numbers, and the help
// line (how to set a shortcut while Select is held, else how to change member).
s32 PsynergyMenu_DrawActionPage(struct UiWindow *window, s32 unused,
                                const struct MenuResult *state) {
  u32 firstEntry;
  u32 visibleCount;
  u8 row;
  s32 cursor;
  struct BattleUnit *owner;
  struct BattleAct *ability;
  struct PsynergyMenuState *menu = gMenuWork;

  (void)unused;

  RenderOutput_RedrawSavedRectFar((void *)window);
  UiWindow_DrawDividerLineFar(window, 0, 11, 16, 11);

  if (PSYNERGY_MENU_SHORTCUT_HELP & menu->flags) {
    UiText_DrawCharacterAtOffsetFar(MsgShortcutHelp, window, 0, 88);
  } else {
    UiText_DrawCharacterAtOffsetFar(MsgChangeCharacterHelp, window, HELP_TEXT_X,
                                    88);
  }

  firstEntry = state->page * LIST_PAGE_SIZE;
  visibleCount = (u8)(state->entryCount - firstEntry);
  if (visibleCount > LIST_PAGE_SIZE) {
    visibleCount = LIST_PAGE_SIZE;
  }

#if EDITION_INTERNATIONAL
  Menu_SetPageIcons(LIST_PAGE_SIZE, firstEntry, window, 112, 34);
#else
  Menu_SetPageIcons(LIST_PAGE_SIZE, firstEntry, window, 120, 34);
#endif
  Menu_DrawPageIndicator(window, state->entryCount, LIST_PAGE_SIZE, state->page,
                         15);
#if EDITION_INTERNATIONAL
  UiText_DrawCharacterAtOffsetFar(MsgPsynergyPp, window, 96, 0);
#else
  UiText_DrawCharacterAtOffsetFar(MsgPsynergyPp, window, 72, 0);
#endif

  row = 0;
  if (visibleCount > row) {
    cursor = firstEntry;
    do {
      owner = UnitGetFar(menu->ownerIds[0]);
      ability = BattleActGet(OWNER_ACTION_ID_MASK & menu->psynergies[cursor]);

      if (ability->ppCost > owner->pp) {
        UiWork_SetParamNibbleFar(TEXT_COLOR_NO_HP);
      } else if (PsynergyMenu_IsActionRestricted(
                     OWNER_ACTION_ID_MASK & menu->psynergies[cursor]) != 0) {
        UiWork_SetParamNibbleFar(TEXT_COLOR_LOW_HP);
      } else {
        UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
      }

      UiText_DrawCharacterAtOffsetFar(
          (OWNER_ACTION_ID_MASK & menu->psynergies[cursor]) + MsgAbilityName,
#if EDITION_INTERNATIONAL
          window, 16, row * 16 + 8);
#else
          window, 32, row * 16 + 8);
#endif
      UiText_DrawNumberAtOffsetFar(ability->ppCost, 2, window, 104,
                                   row * 16 + 8);
      UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);

      row++;
      cursor++;
    } while (visibleCount > row);
  }

  return 1;
}

// The Psynergy list of the member in pane, five a page. The cursor's Psynergy
// is described in the info window and its icon animates; L and R switch to the
// previous or next member who has Psynergy to list. In the use mode (menu mode
// 0), A chooses a Psynergy the member can use and afford; holding Select shows
// the shortcut help, and L or R with Select set the Psynergy as that shortcut.
// In the shortcut modes (1 and 2) only field Psynergy are listed and A chooses
// one. Returns the chosen Psynergy, or -1 when the player backs out.
s32 PsynergyMenu_RunList(s32 pane) {
  struct PsynergyMenuState *menu;
  struct BattleAct *ability;
  struct RenderOutput *icon;
  struct UiWindow *window;
  s32 changed;
  s32 moved;
  s8 listMode;
  s32 member;
  u8 i;
  s32 result;
  s32 redraw;
  s32 done;
  struct BattleUnit *owner;
  s32 previous;
  s32 shortcutHelp;
  s32 work[5];
  struct MenuResult state;

  menu = gMenuWork;
  result = 0;
  previous = 0;
  shortcutHelp = 0;
  menu->paneIcon[pane]->active = ICON_HIDDEN;
  UiWindow_UpdateOrCreate(&menu->listWindow, 13, 3, 17, 14, 2);
  window = menu->listWindow;
  done = 0;

  while (done == 0 && GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    owner = UnitGetFar(menu->ownerIds[pane]);
    if (menu->mode != 0) {
      menu->psynergyCount =
          PsynergyMenu_CollectActions(owner, menu->psynergies, 1);
    } else {
      menu->psynergyCount =
          PsynergyMenu_CollectActions(owner, menu->psynergies, 2);
    }
    PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
    PsynergyMenu_BuildPageResult(&state, pane);
    redraw = 1;
    changed = 1;
    menu->paneIcon[pane]->active = ICON_SHOWN;

    while (GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
#if EDITION_INTERNATIONAL
      UiMenu_PositionCursor(88, state.row * 16 + 36);
#else
      UiMenu_PositionCursor(98, state.row * 16 + 36);
#endif

      if (changed != 0) {
        changed = 0;
        if (menu->psynergies[previous] != 0) {
          icon = menu->entryIcons[previous];
          UiIcon_PrepareObject(icon);
        }
        if (redraw != 0) {
          redraw = 0;
          WaitFrames(1);
          PsynergyMenu_DrawActionPage(window, 0, &state);
        }
        PsynergyMenu_DrawDetailPage(window, work, &state);
        menu->paneAction[pane] = menu->psynergies[state.selectedIndex];
        menu->cursorIcon->active = ICON_HIDDEN;
        if (menu->psynergies[state.selectedIndex] != 0) {
          icon = menu->entryIcons[state.selectedIndex];
          icon->active = 9; // animate the selected icon
          icon->animStep = 0;
          icon->sentinel = 250;
        }
        for (i = 0; i < menu->ownerCount; i++) {
          AnimationObjects_SelectAnimationFar(menu->tabObjects[i], 1);
        }
      }

      WaitFrames(1);
      previous = state.selectedIndex;

      if ((gKeysHeld & KEY_SELECT) == 0) {
        moved = Menu_HandlePageInput(0, state.entryCount, LIST_PAGE_SIZE,
                                     &state.row, &state.page);
      } else {
        moved = -1;
      }
      if (moved == 1) {
        redraw = 1;
        changed = 1;
      }
      if (moved == 0) {
        changed = 1;
      }
      if (moved == -1) {
        changed = 0;
      }

      // Select shows the shortcut help while held, for field Psynergy.
      if (menu->mode == 0) {
        if ((gKeysPressed & KEY_SELECT) != 0 && shortcutHelp == 0) {
          ability = BattleActGet(menu->psynergies[state.selectedIndex] &
                                 OWNER_ACTION_ID_MASK);
          if (ability->type0c == 0) {
            AudioCommand_PlayFar(SOUND_MENU_ERROR);
          } else {
            AudioCommand_PlayFar(174); // the Use sound
            shortcutHelp = 1;
            menu->flags |= PSYNERGY_MENU_SHORTCUT_HELP;
            UiWindow_ClearInteriorTilesFar((void *)window, 0, 88, 120, 96);
            UiText_DrawCharacterAtOffsetFar(MsgShortcutHelp, window, 0, 88);
          }
        }
        if ((gKeysHeld & KEY_SELECT) == 0 && shortcutHelp == 1) {
          shortcutHelp = 0;
          menu->flags &= ~PSYNERGY_MENU_SHORTCUT_HELP;
          UiWindow_ClearInteriorTilesFar((void *)window, 0, 88, 120, 96);
          UiText_DrawCharacterAtOffsetFar(MsgChangeCharacterHelp, window,
                                          HELP_TEXT_X, 88);
        }
      }

      // A: set the shortcut (modes 1 and 2), or use the Psynergy if allowed
      // and affordable.
      if ((gKeysPressed & KEY_A) != 0) {
        if (menu->mode != 0) {
          AudioCommand_PlayFar(130); // the shortcut sound
          result = menu->psynergies[state.selectedIndex];
          done = 1;
          break;
        }
        if (menu->psynergies[state.selectedIndex] != 0) {
          if (PsynergyMenu_IsActionRestricted(
                  menu->psynergies[state.selectedIndex]) != 0) {
            AudioCommand_PlayFar(SOUND_MENU_ERROR);
            continue;
          }
          ability = BattleActGet(menu->psynergies[state.selectedIndex] &
                                 OWNER_ACTION_ID_MASK);
          if (ability->ppCost > owner->pp) {
            AudioCommand_PlayFar(SOUND_MENU_ERROR);
          } else {
            AudioCommand_PlayFar(173); // the choice sound
            result = menu->psynergies[state.selectedIndex];
            done = 1;
            break;
          }
        }
      }

      if ((gKeysPressed & KEY_B) != 0) {
        AudioCommand_PlayFar(SOUND_MENU_CANCEL);
        result = -1;
        done = 1;
        break;
      }

      // L and R: the previous or next member with Psynergy to list.
      if (((gKeysRepeat & KEY_R) != 0 || (gKeysRepeat & KEY_L) != 0) &&
          (gKeysHeld & KEY_SELECT) == 0) {
        listMode = menu->mode != 0 ? 1 : 2;
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        menu->selectedIndexByOwner[menu->ownerIds[pane]] = state.selectedIndex;
        member = menu->tabIndex[pane];
        do {
          if ((gKeysRepeat & KEY_R) != 0) {
            member++;
          } else {
            member--;
          }
          member = (member + menu->ownerCount) % menu->ownerCount;
          menu->selectedOwner = menu->ownerTable[member];
          menu->ownerIds[0] = menu->ownerTable[member];
          menu->psynergyCount = PsynergyMenu_CollectActions(
              UnitGetFar(menu->ownerIds[0]), menu->psynergies, listMode);
        } while (menu->psynergyCount == 0);
        menu->tabIndex[pane] = member;
        for (i = 0; i < 4; i++) {
          menu->rowPositions[i] = 30;
        }
        menu->rowPositions[member] = 26;
        Menu_DrawOwnerStatusPanel(menu->statusWindow, menu->ownerTable[member],
                                  0, STATUS_PANEL_EXPERIENCE);
        PsynergyMenu_CallIconRoutineWithValue((s32)menu,
                                              menu->ownerTable[member]);
        break;
      }

      // Select + L or R: make the field Psynergy that shortcut.
      if ((gKeysPressed & KEY_L) != 0 && (gKeysHeld & KEY_SELECT) != 0) {
        ability = BattleActGet(menu->psynergies[state.selectedIndex] &
                               OWNER_ACTION_ID_MASK);
        if (ability->type0c == 0) {
          AudioCommand_PlayFar(SOUND_MENU_ERROR);
        } else {
          AudioCommand_PlayFar(130); // the shortcut sound
          if (PsynergyMenu_SetShortcut(menu->ownerIds[pane],
                                       menu->psynergies[state.selectedIndex],
                                       0) != 0) {
            result = menu->psynergies[state.selectedIndex];
            menu->mode = 1;
            done = 1;
            break;
          }
        }
      }

      if ((gKeysPressed & KEY_R) != 0 && (gKeysHeld & KEY_SELECT) != 0) {
        ability = BattleActGet(menu->psynergies[state.selectedIndex] &
                               OWNER_ACTION_ID_MASK);
        if (ability->type0c == 0) {
          AudioCommand_PlayFar(SOUND_MENU_ERROR);
        } else {
          if (PsynergyMenu_SetShortcut(menu->ownerIds[pane],
                                       menu->psynergies[state.selectedIndex],
                                       1) != 0) {
            AudioCommand_PlayFar(130); // the shortcut sound
            result = menu->psynergies[state.selectedIndex];
            menu->mode = 2;
            done = 1;
            break;
          }
        }
      }
    }
  }

  menu->flags &= ~PSYNERGY_MENU_SHORTCUT_HELP;
  UiIcon_PrepareObject(menu->entryGridCursor);
  menu->paneRow[pane] = state.selectedIndex;
  menu->selectedIndexByOwner[menu->ownerIds[pane]] = state.selectedIndex;
  menu->paneAction[pane] = result;
  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED) != 0) {
    result = -1;
  }
  WaitFrames(1);
  return result;
}

// Returns 1 when Psynergy no (an action slot entry) cannot be used from the
// menu: it has no field effect and is not usable both in the field and in
// battle.
s32 PsynergyMenu_IsActionRestricted(s32 no) {
  struct BattleAct *action =
      BattleActGet((u32)(no << 18) >> 18) /* & OWNER_ACTION_ID_MASK */;

  if (action->type0c != 0 ||
      (action->flags &
       (ACTION_USABLE_IN_FIELD | ACTION_USABLE_IN_BATTLE)) ==
          (ACTION_USABLE_IN_FIELD | ACTION_USABLE_IN_BATTLE))
    return 0;
  return 1;
}

// How many party figure rows Menu_OpenCharacterSelector resets.
#if EDITION_INTERNATIONAL
#define ROW_CNT 8
#else
#define ROW_CNT 4
#endif

// Lets the player choose one party member, over the field (the Colosso scenes
// use it). Returns the chosen member's id, or -1 when the player backs out.
s32 Menu_OpenCharacterSelector(void) {
  struct CharacterMenuState *state =
      Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, 0x0a70);
  s32 result;
  s32 index;

  gMenuCtrlWork->suspended = 1;
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
  UiWindow_InitializeWork(0);

  state->partyCount = Party_ListActiveOwnersFar(state->ownerIds);
  Menu_InitSelectorCursorAndEntries(0, 3, 0, 7);
  state->selectorWindow = UiWindow_CreateFar(13, 0, 17, 5, 2);
  for (index = 0; index < ROW_CNT; index++)
    state->ownerY[index] = 30;
  state->page = 3; // the member chooser

  result = CharacterSelector_Run();

  RenderOutput_ClearListFar(state->statusWindow);
  ItemMenu_Close();
  gMenuCtrlWork->suspended = 0;
  WaitFrames(1);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_MENU);
  return result;
}

// The member chooser's loop. Returns the chosen member's id, or -1.
s32 CharacterSelector_Run(void) {
  struct CharacterMenuState *work = gMenuWork;
  s32 result = 0;

  /* FAKEMATCH: the one-pass block ends the schedule after the work load, so the
   * zero is stored before the slot offset, as native; the store without it, or
   * with result declared first, differs 4 (the offset moves into the load's
   * delay), a constant 0 store 14, two returns 26, the call result kept in
   * result and tested, or a ?: on it, 19. */
  do {
    work->selectedSlots[0] = result;
  } while (0);
  if (CharacterMenu_SelectOwner(0) == -1)
    result = -1;
  else
    result = work->selectedIds[0];
  return result;
}

// The window tiles from 0x06004000 the Status menu saves and restores.
#if EDITION_INTERNATIONAL
#define SAVED_TILE_BYTES 0x2000
#else
#define SAVED_TILE_BYTES 0x800
#endif

// The Status menu an event opens (window command 4) over the field: saves the
// first two background palettes and the window tiles it draws over, sets up
// the party row and, when the party has Djinn, the four Djinn sprites, runs
// the status pages, then puts everything back. Returns their result.
s32 ActionMenu_Open(void) {
  struct CharacterMenuState *state =
      Runtime_AllocateHeapBlock(HEAP_SLOT_MENU, 0x0a70);
  void *palette = Runtime_BumpAllocateAlternatePool(64);
  void *tiles = Runtime_BumpAllocateAlternatePool(SAVED_TILE_BYTES);
  s32 result;
  s32 index;

  ((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 1;
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
  Scheduler_EnableOverlayCallbacksWithFlags();
  UiWindow_InitializeWork(0);
  state->page = 0;
  state->partyCount = Party_ListActiveOwnersFar(state->ownerIds);
  Menu_InitSelectorCursorAndEntries(0, 3, 0, 7);
  Iwram_CopyWords(palette, (void *)0x05000000, 64);
#if !EDITION_INTERNATIONAL
  Iwram_CopyWords(tiles, (void *)0x06004000, SAVED_TILE_BYTES);
#endif
  Palette_LightenBankHighlight(14);
  // The first sprite palette into background palettes 0 and 1, each with a
  // colour of palette 15 as its colour 14.
  Dma_Set((void *)0x05000200, (void *)0x05000000, 0x80000010, REG_DMA3);
  Dma_Set((void *)0x050001c8, (void *)0x0500001c, 0x80000001, REG_DMA3);
  Dma_Set((void *)0x05000200, (void *)0x05000020, 0x80000010, REG_DMA3);
  Dma_Set((void *)0x050001e8, (void *)0x0500003c, 0x80000001, REG_DMA3);
#if EDITION_INTERNATIONAL
  Iwram_CopyWords(tiles, (void *)0x06004000, SAVED_TILE_BYTES);
  Iwram_FillWords((void *)0x06004000, SAVED_TILE_BYTES, 0x33333333);
  UiWork_SetAltFlagAndClearTableFar(1);
#endif
  state->selectorWindow = UiWindow_CreateFar(13, 0, 17, 5, 2);
  for (index = 0; index < MENU_ROW_COUNT; index++)
    state->ownerY[index] = 30;
  if (Party_SumDjinnCountsFar(-1) != 0)
    FourObjectMotion_InitializeTopRow(state->selectorWindow, 0);
  for (index = 0; index < 4; index++) {
    state->rowX[index] = 130 + index * 32;
    state->rowY[index] = 128;
  }
  Link_DrawShiftedTilePairFar(0x06002500);
  // Run the pages.
  Menu_CancelSoundReset();
  state->page = 0;

  result = Menu_RunActionFlow();

  // Close, and put the palettes and tiles back.
  Menu_EnsureCancelSound();
  RenderOutput_ClearListFar(state->statusWindow);
  FourObjectMotion_ClearSlotsAndSchedule();
  Scheduler_DisableOverlayCallbacksWithFlags();
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
#if EDITION_INTERNATIONAL
  UiWindow_MarkVisibleTileAttributesFar();
  UiWork_SetAltFlagAndClearTableFar(0);
  WaitFrames(1);
#endif
  Iwram_CopyWords((void *)0x05000000, palette, 64);
  Iwram_CopyWords((void *)0x06004000, tiles, SAVED_TILE_BYTES);
  MemFree(tiles);
  MemFree(palette);
  ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menuBusy = 1;
  ItemMenu_Close();
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_MENU);
  ((struct ObjectSystemWork *)WORK_SLOT(HEAP_SLOT_MENU_CONTROL))->suspended = 0;
  WaitFrames(1);
  UiWindow_EraseBorderRectFar(0, 0, 30, 20);
  ((struct UiRenderWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->menuBusy = 0;
  return result;
}

// The Status menu's pages: choose a party member, then their status page, A
// for their Psynergy page and A again for their equipment page; B steps back.
// Returns the last page's result, or -1 when the player backs out or presses
// Start.
s32 Menu_RunActionFlow(void) {
  struct CharacterMenuState *state = gMenuWork;
  s32 step = 0;
  s32 finished = step;
  s32 result = 0;
  u32 changed;

  while (!finished && !GameFlag_TestFar(FLAG_MENU_START_PRESSED)) {
    switch (step) {
    case 0: // choose a member
      state->selectedSlots[0] = finished;
      if (CharacterMenu_SelectOwner(0) == -1) {
        result = -1;
        finished = 1;
      }
      step = 1;
      break;
    case 1: // their status
      state->ownerCursors[0]->active = ICON_HIDDEN;
      result = CharacterMenu_SelectCommand();
      step = result == -1 ? 0 : 2;
      break;
    case 2: // their Psynergy
      state->ownerCursors[0]->active = ICON_HIDDEN;
      result = PsynergyMenu_SelectAction();
      step = 0;
      if (result != -1)
        step = 3;
      break;
    case 3: // their equipment; back to the member after it
      state->ownerCursors[0]->active = ICON_HIDDEN;
      result = ItemMenu_SelectItem();
      // step 1 for -1, step 0 otherwise.
      changed = ~result;
      step = (-changed | changed) >> 31;
      break;
    default:
      finished = 1;
      break;
    }
  }

  if (GameFlag_TestFar(FLAG_MENU_START_PRESSED))
    result = -1;

  return result;
}

// Lets the player choose a party member in slot, from the one chosen last: the
// member chooser when the menu is on page 3, else the Status menu's party row.
// Game flag 0x172 adds a divider to the party window. Returns the chooser's
// result.
s32 CharacterMenu_SelectOwner(s32 slot) {
  struct CharacterMenuState *menu;
  struct RenderOutput *cursor;
  s32 result;
  s32 index;

  menu = gMenuWork;
  index = menu->ownerIndex[slot];
  result = 0;
  cursor = menu->ownerCursors[slot];
  cursor->active = ICON_SHOWN;
  cursor->animStep = 0;
  RenderOutput_RedrawSavedRectFar(menu->ownerWindow);
  if (GameFlag_TestFar(0x172))
    UiWindow_DrawDividerLineFar(menu->ownerWindow, 9, 1, 9, 3);
  if (index == -1)
    menu->ownerIndex[slot] = 0;
  else
    UiMenu_SlideCursor(index * 24 - 10, 16);
  if (menu->page == 3)
    result = PsynergyMenu_SelectOwner();
  else
    result = CharacterSelector_RunRearrange();
  cursor = menu->ownerCursors[slot];
  UiIcon_PrepareObject(cursor);
  WaitFrames(1);
  return result;
}

// Shows every Djinn the party has found, a column of seven for each element,
// until A, B or Select, then puts the Djinn menu back.
s32 DjinnMenu_ShowCurrentList(void) {
  struct DjinnMenuWork *menu;
  struct UiWindow *window;
  s32 element;
  s32 djinn;

  menu = gMenuWork;
  ItemMenu_ResetCategory();
  AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
  RenderOutput_RedrawSavedRectFar(menu->helpWindow);
  UiText_DrawCharacterAtOffsetFar(MsgReturnHelp, menu->helpWindow, 0, 16);
  menu->cursor->active = ICON_HIDDEN;
  menu->secondCursor->active = ICON_HIDDEN;
  WaitFrames(1);
  window = menu->window;
  {
    s32 i;

    for (i = 0; i < 4; i++) {
      menu->slotX[i] = 32 + i * 56;
      menu->slotY[i] = 70;
      menu->ownerY[i] = 30;
    }
  }
  RenderOutput_RedrawSavedRectFar(window);
  UiWindow_DrawDividerLineFar(window, 0, 11, 28, 11);
  UiText_DrawResourceFar(MsgCurrentDjinn, menu->helpWindow, -96, 132);
  for (element = 0; element < 4; element++) {
    for (djinn = 0; djinn < 7; djinn++) {
      s32 id = element * 20 + djinn;

      // Game flags 48 on: one per Djinn found, 20 to an element.
      if (GameFlag_TestFar(48 + id)) {
        UiWindow_SetTilemapEntryFar(window, 0x1001 + element, element * 7 + 1,
                                    djinn + 3, 0);
        UiText_DrawCharacterAtOffsetFar(MsgDjinnName + id, window,
                                        element * 56 + 16, djinn * 8 + 24);
      }
    }
  }
  (*(struct UiWork **)&gWindowWork)->dirty = 1;
  for (;;) {
    if (GameFlag_TestFar(FLAG_MENU_START_PRESSED))
      break;
    WaitFrames(1);
    if (gKeysPressed & (KEY_A | KEY_B | KEY_SELECT))
      break;
  }
  RenderOutput_RedrawSavedRectFar(menu->window);
  RenderOutput_ClearListFar(menu->helpWindow);
  {
    s32 i;

    for (i = 0; i < 4; i++) {
      menu->slotX[i] = 130 + i * 32;
      menu->slotY[i] = 128;
    }
  }
  menu->cursor->active = ICON_SHOWN;
  menu->secondCursor->active = ICON_SHOWN;
  AudioCommand_PlayFar(SOUND_MENU_CANCEL);
  // Returns no value: the bytes have the s32 epilogue (pop {r1}; bx r1) after
  // this void call.
}

struct InventoryMenuState;

// The Status menu's party row: Left and Right choose a member, showing their
// status; L and R move the member one place along the party order; Select
// lists the Djinn found (once the first is found, game flag 48); A chooses the
// member and B backs out. Returns 1 or -1.
s32 CharacterSelector_RunRearrange(void) {
  struct CharacterMenuState *work;
  s32 cursor;
  s32 count;
  s32 page;
  s32 redraw;
  s32 result;
  s32 i;

  work = gMenuWork;
  cursor = work->ownerIndex[0];
  count = work->ownerCount[0];
  redraw = 1;
  result = 0;
  page = work->page;
  UnitGetFar(work->ownerIds[cursor]);
  for (i = 0; i < 4; i++) {
    work->rowX[i] = 130 + i * 32;
    work->rowY[i] = 128;
  }
  // The highlight palette, and the first sprite palette into background
  // palettes 0 and 1.
  Palette_LightenBankHighlight(14);
  Dma_Set((void *)0x05000200, (void *)0x05000000, 0x80000010, REG_DMA3);
  Dma_Set((void *)0x050001c8, (void *)0x0500001c, 0x80000001, REG_DMA3);
  Dma_Set((void *)0x05000200, (void *)0x05000020, 0x80000010, REG_DMA3);
  Dma_Set((void *)0x050001e8, (void *)0x0500003c, 0x80000001, REG_DMA3);
  while (!GameFlag_TestFar(FLAG_MENU_START_PRESSED)) {
    if (redraw) {
      redraw = 0;
      // The help lines, and the member's status and figure.
      RenderOutput_RedrawSavedRectFar(work->selectorWindow);
      UiText_DrawCharacterAtOffsetFar(MsgRearrangeHelp, work->selectorWindow, 0,
                                      0);
      if (GameFlag_TestFar(48))
        UiText_DrawCharacterAtOffsetFar(MsgDjinnListHelp, work->selectorWindow,
                                        0, 16);
      UiText_DrawCharacterAtOffsetFar(MsgRearrangeHelp - 3, // "A: Details"
                                      work->selectorWindow, 0, 8);
      cursor = (cursor + count) % count;
      UnitGetFar(work->ownerIds[cursor]);
      page = (page + 3) % 3;
      Menu_CreateWindowAndEntryObjects(work->ownerIds[cursor], page);
      PsynergyMenu_CallIconRoutineWithValue(work, work->ownerIds[cursor]);
      for (i = MENU_ROW_COUNT - 1; i >= 0; i--)
        work->ownerY[i] = 30;
      work->ownerY[cursor] = 26;
    }
    UiMenu_PositionCursor(cursor * 24 - 10, 16);
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
      result = 1;
      break;
    }
    if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -1;
      break;
    }
    // L and R move the member along the party order.
    if (gKeysRepeat & KEY_R) {
      if (CharacterSelector_MoveEntry(cursor, 1)) {
        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        cursor++;
        Menu_ReleaseEntryObjects();
        PsynergyMenu_InitializeEntryObjects(work->ownerWindow, 2, 2, 8, 0);
        for (i = MENU_ROW_COUNT - 1; i >= 0; i--)
          work->ownerY[i] = 30;
        work->ownerY[cursor] = 26;
      } else {
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
      }
      WaitFrames(1);
    } else if (gKeysRepeat & KEY_L) {
      if (CharacterSelector_MoveEntry(cursor, 0)) {
        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        cursor--;
        Menu_ReleaseEntryObjects();
        PsynergyMenu_InitializeEntryObjects(work->ownerWindow, 2, 2, 8, 0);
        for (i = MENU_ROW_COUNT - 1; i >= 0; i--)
          work->ownerY[i] = 30;
        work->ownerY[cursor] = 26;
      } else {
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
      }
      WaitFrames(1);
    } else if ((gKeysPressed & KEY_SELECT) && GameFlag_TestFar(48)) {
      DjinnMenu_ShowCurrentList();
      redraw = 1;
    } else {
      if (gKeysRepeat & KEY_LEFT) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        if (count > 1) {
          cursor--;
          redraw = 1;
        }
      }
      if (gKeysRepeat & KEY_RIGHT) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        if (count > 1) {
          redraw = 1;
          cursor++;
        }
      }
    }
  }
  work->ownerIndex[0] = cursor;
  work->selectedOwner = work->ownerIds[cursor];
  work->selectedIds[0] = work->ownerIds[cursor];
  return result;
}

// The member chooser (menu page 3): Left and Right choose a party member,
// showing their status and Psynergy icons; A chooses and B backs out. Returns
// 1 or -1.
s32 PsynergyMenu_SelectOwner(void) {
  struct PsynergyMenuState *menu = gMenuWork;
  s32 selection = menu->tabIndex[0];
  s32 count = menu->tabCounts[0];
  s32 redraw = 1;
  s32 page = menu->flags;
  s32 result;
  s32 i;

  UnitGetFar(menu->ownerTable[selection]);
  RenderOutput_RedrawSavedRectFar(menu->messageWindow);
  UiText_DrawCharacterAtOffsetFar(MsgChooseCharacter, menu->messageWindow, 0,
                                  0);
  UiText_DrawCharacterAtOffsetFar(MsgChooseCharacter + 1, menu->messageWindow,
                                  0, 16);
  for (;;) {
    if (redraw) {
      redraw = 0;
      selection = (selection + count) % count;
      UnitGetFar(menu->ownerTable[selection]);
      page = (page + 3) % 3;
      Menu_CreateWindowAndEntryObjects(menu->ownerTable[selection], page);
      PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerTable[selection]);
      for (i = 0; i < MENU_ROW_COUNT; i++)
        menu->rowPositions[i] = 30;
      menu->rowPositions[selection] = 26;
      menu->psynergyCount = PsynergyMenu_CollectActions(
          UnitGetFar(menu->ownerTable[selection]), (u16 *)menu->psynergies, 0);
      PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
      Menu_PlaceEntryObjectsInGrid(96, 96, 8);
      Menu_HideEmptyEntryIcons(menu->psynergies);
    }
    UiMenu_PositionCursor(selection * 24 - 10, 16);
    WaitFrames(1);
    if (gKeysPressed & KEY_A) {
      AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
      result = 1;
      break;
    }
    if (gKeysPressed & KEY_B) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -1;
      break;
    }
    {
      if (gKeysRepeat & KEY_LEFT) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        if (count > 1) {
          selection--;
          redraw = 1;
        }
      }
      if (gKeysRepeat & KEY_RIGHT) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        if (count > 1) {
          selection++;
          redraw = 1;
        }
      }
    }
  }
  menu->tabIndex[0] = selection;
  menu->selectedOwner = menu->ownerTable[selection];
  menu->ownerIds[0] = menu->ownerTable[selection];
  return result;
}

// Moves the party member at selectedIndex one place later (direction 1) or
// earlier (0) in the party order. Returns 1, or 0 when it cannot move.
s32 CharacterSelector_MoveEntry(s32 selectedIndex, s32 direction) {
  struct CharacterMenuState *menu = gMenuWork;
  u32 reordered[14];
  s32 index;

  if (menu->partyCount <= 1) {
    return 0;
  }
  if (direction == 1) {
    if (selectedIndex == menu->partyCount - 1) {
      return 0;
    }
  } else {
    if (selectedIndex == 0) {
      return 0;
    }
  }

  {
    u32 zero = 0;
    u32 *clear = &reordered[13];

    do {
      *clear = zero;
      clear--;
    } while ((s32)clear >= (s32)reordered);
  }
  for (index = 0; index < menu->partyCount; index++) {
    reordered[index] = menu->ownerIds[index];
  }

  if (direction == 1) {
    index = reordered[selectedIndex];
    reordered[selectedIndex] = reordered[selectedIndex + 1];
    reordered[selectedIndex + 1] = index;
  } else {
    index = reordered[selectedIndex];
    reordered[selectedIndex] = reordered[selectedIndex - 1];
    reordered[selectedIndex - 1] = index;
  }

  // Rebuild the party in the new order.
  for (index = 0; index < menu->partyCount; index++) {
    Party_RemoveActiveOwnerFar(menu->ownerIds[index]);
  }
  for (index = 0; index < menu->partyCount; index++) {
    Party_AddActiveOwnerFar(reordered[index]);
  }
  menu->partyCount = Party_ListActiveOwnersFar(menu->ownerIds);
  return 1;
}

// Sets up the Status menu's first screen: the cursor window with the party's
// figures, no other windows yet, and an 8 by 2 cursor grid. The parameters are
// not used.
void Menu_InitSelectorCursorAndEntries(s32 unused0, s32 unused1, s32 unused2,
                                       s32 unused3) {
  struct CharacterMenuState *state;

  state = gMenuWork;
  PsynergyMenu_InitializeEntryObjects(UiMenu_CreateCursor(state), 2, 2, 8, 0);
  state->unknownWindow028 = 0;
  state->statusWindow = 0;
  state->helpWindow = 0;
  state->actionWindow = 0;
  state->selectedColumn = 0;
  state->selectedRow = 0;
  state->columnCount = 8;
  state->rowCount = 2;
}

// Shows member resource's status in the status window (0, 5, 30 by 15),
// opening it first with the member's portrait, the item icons on the chooser
// page, and the eight stat sprites; then draws the status and ailments.
void Menu_CreateWindowAndEntryObjects(s32 resource, s32 frame) {
  s32 created;
  struct UiWindow *handle;
  struct RenderOutput *object;
  struct CharacterMenuState *state;

  state = gMenuWork;
  created = 0;
  handle = state->statusWindow;
  if (handle == 0) {
    created = UiWindow_UpdateOrCreate(&state->statusWindow, 0, 5, 30, 15, 2);
    handle = state->statusWindow;
  }
  if (created != 0) {
    object = SideObject_CreateFar(resource, 0, 0, handle, 0, 0);
    state->statusCursor = object;
    if ((object->sentinel = 0xF0, state->page) == 3) {
      Menu_SpawnIconEntries((struct InventoryMenuState *)state, handle);
    }
    Menu_CreateEightEntryObjects(handle);
    CharacterMenu_DrawStatusAilments(handle, resource, 0x100);
    return;
  }
  CharacterMenu_DrawStatusAilments(handle, resource, 0);
}

// The member's status page. The left pane lists their ailments (or the stat
// help when they have none) and the right pane the status entries (name and
// level, experience, HP, PP, Djinn when the party has any, elemental levels,
// Power and Resist); the cursor's entry is highlighted and explained in the
// help window. Up and Down move in a pane, Left and Right switch panes, L and
// R switch to the previous or next member; A goes on to the Psynergy page and
// B backs out. Returns 1 or -1.
s32 CharacterMenu_SelectCommand(void) {
  struct CharacterMenuState *menu;
  s32 pane;
  s32 result;
  s32 selected;
  s8 count;
  s32 hasAilments;
  s32 done;
  s32 hasDjinn;
  s32 redraw;
  s32 member;
  s32 djinnTotal;
  s32 i;
  u8 entries[8];

  menu = gMenuWork;
  pane = 0;
  result = 0;
  selected = 0;
  djinnTotal = Party_SumDjinnCountsFar(-1);
  hasDjinn = 0;
  if (djinnTotal != 0)
    hasDjinn = 1;
  UiWindow_UpdateOrCreate(&menu->helpWindow, 0, 0, 30, 5, 2);
  Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
  for (i = 3; i >= 0; i--)
    menu->rowY[i] = 104;
  done = 0;
  UiMenu_SlideCursor(-10, 88);

  // One member's page at a time; L and R start the next.
  while (done == 0 && GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    UnitGetFar(menu->selectedIds[0]);
    CharacterMenu_DrawStatusAilments(menu->statusWindow, menu->selectedIds[0],
                                     1);
    count = CharacterMenu_BuildAvailability(entries, 1, menu->selectedIds[0]);
    hasAilments = 0;
    if (count == 0)
      count = 1;
    else
      hasAilments = 1;
    redraw = 1;

    while (GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
      if (redraw != 0) {
        s32 limit;

        redraw = 0;
        limit = count;
        pane = (pane + 2) % 2;
        if (pane == 0) {
          selected = (selected + limit) % limit;
          RenderOutput_RedrawSavedRectFar(menu->helpWindow);
          if (hasAilments == 0) {
            UiText_DrawCharacterAtOffsetFar(
                MsgSwitchCharacterHelp, menu->statusWindow, SWITCH_HELP_X, -24);
            UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp + 1,
                                            menu->statusWindow, 0, -24);
          }
        } else {
          RenderOutput_RedrawSavedRectFar(menu->helpWindow);
          if (hasDjinn != 0)
            selected = (selected + 8) % 8;
          else
            selected = (selected + 7) % 7;
        }
        CharacterMenu_DrawSelectionCursor(pane, selected, entries, 0);
        RenderOutput_ClearListFar(menu->helpWindow);
        WaitFrames(1);
        if (pane == 0)
          CharacterMenu_DrawSelectionLabels(menu->helpWindow, selected,
                                            entries);
        else
          StatusMenu_ShowOwnerProgressMessage(menu->helpWindow, selected,
                                              hasDjinn);
      }
      menu->ownerCursors[0]->active = ICON_SHOWN;
      if (pane == 0)
        UiMenu_PositionCursor(-10, selected * 16 + 88);
      else if (selected <= 3)
        UiMenu_PositionCursor(24, selected * 8 + 48);
      else
        UiMenu_PositionCursor(48, selected * 8 + 80);
      WaitFrames(1);
      if (gKeysRepeat & KEYS_DPAD)
        CharacterMenu_DrawSelectionCursor(pane, selected, entries, 1);
      if (gKeysPressed & KEY_A) {
        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        done = 1;
        result = 1;
        break;
      }
      if (gKeysPressed & KEY_B) {
        AudioCommand_PlayFar(SOUND_MENU_CANCEL);
        done = 1;
        result = -1;
        break;
      }
      if (gKeysRepeat & KEY_UP) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        redraw = 1;
        selected--;
      }
      if (gKeysRepeat & KEY_DOWN) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        redraw = 1;
        selected++;
      }
      if (gKeysRepeat & KEY_RIGHT) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        redraw = 1;
        pane++;
      }
      if (gKeysRepeat & KEY_LEFT) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        redraw = 1;
        pane--;
      }
      // L and R: the previous or next member.
      if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L)) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        member = menu->ownerIndex[0];
        if (gKeysRepeat & KEY_R)
          member++;
        else
          member--;
        member = (member + menu->partyCount) % menu->partyCount;
        menu->selectedOwner = menu->ownerIds[member];
        menu->selectedIds[0] = menu->ownerIds[member];
        menu->ownerIndex[0] = member;
        PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerIds[member]);
        break;
      }
    }
  }
  RenderOutput_ClearListFar(menu->helpWindow);
  RenderOutput_RedrawSavedRectFar(menu->helpWindow);
  UiWindow_ClearInteriorTilesFar(menu->statusWindow, 64, 56, 224, 96);
  Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
  for (i = 3; i >= 0; i--)
    menu->rowY[i] = 128;
  ItemMenu_ResetCategory();
  return result;
}

// Highlights (invert 0) or unhighlights (invert 1) the selected entry of the
// status page: in pane 0 the selected ailment row, as wide as its label
// (gCharacterMenu_CursorWidths); in pane 1 one of the first four status rows
// or of the rows below.
void CharacterMenu_DrawSelectionCursor(s32 mode, s32 selected, u8 *entries,
                                       s32 invert) {
  struct CharacterMenuState *menu = gMenuWork;
  u32 different;
  s32 count;
  s32 index;
  s32 x;
  s32 y;
  s32 palette;
  u8 width;

  if (mode == 0) {
    y = selected * 2 + 5;
    x = 0;
    width = 5;
    count = 0;
    index = 0;
    while (index <= 4) {
      if (entries[index] != 0) {
        if (selected == count) {
          width = gCharacterMenu_CursorWidths[index];
          break;
        }
        count++;
      }
      index++;
    }
  } else if (selected <= 3) {
    y = selected;
    x = 5;
    width = 13;
  } else {
    y = selected + 4;
    x = 8;
    width = 20;
  }

  // Palette 14 (highlighted) when invert is 0, 15 when it is 1.
  different = 1 ^ invert;
  palette = 15 - (((0u - different) | different) >> 31);
  Render_SetTilemapFlagRect(menu->statusWindow, x, y, width, 1, palette);
}

// Writes the advice for the selected ailment (two lines, e.g. how to revive a
// member who is down) into window target, or that the member is well when
// they have no ailment.
void CharacterMenu_DrawSelectionLabels(struct UiWindow *target, s32 selected,
                                       const u8 *entries) {
  s32 count = 0;
  s32 index = 0;

  do {
    if (entries[index] != 0) {
      if (selected == count) {
        s32 message = MsgStatusAdvice + index * 2;
        UiText_DrawResourceFar(message, target, 0, -1);
        UiText_DrawResourceFar(message + 1, target, 0, 15);
      }
      count++;
    }
    index++;
  } while (index <= 4);

  if (count == 0)
    UiText_DrawResourceFar(MsgStatusNormal, target, 0, 0);
}

// The longest status help text, in characters.
#if EDITION_INTERNATIONAL
#define PROGRESS_TEXT_MAX 0x80
#else
#define PROGRESS_TEXT_MAX 0x40
#endif

// Writes the help text of status entry messageVariant into destination: the
// entries skip Djinn when the party has none (preserveVariant 0); the
// experience entry says how many points the next level needs, or, at level 99,
// gives the plain experience text.
void StatusMenu_ShowOwnerProgressMessage(struct UiWindow *destination,
                                         s32 messageVariant,
                                         s32 preserveVariant) {
  struct CharacterMenuState *menu = gMenuWork;
  void *buffer;

  if (preserveVariant == 0 && messageVariant > 3) {
    messageVariant++;
  }
  if (messageVariant == 1) {
    struct BattleUnit *owner = UnitGetFar(menu->selectedIds[0]);

    if (owner->level == 99) {
      messageVariant = 8; // "These are your experience points."
    } else {
#if EDITION_INTERNATIONAL
      u64 remaining =
          Unit_GetExperienceForLevelFar(menu->selectedIds[0], owner->level + 1) -
          owner->experience;
#else
      u32 remaining =
          Unit_GetExperienceForLevelFar(menu->selectedIds[0], owner->level + 1) -
          owner->experience;
#endif
      UiWork_PushValueSlotFar(remaining, 5);
    }
  }

  buffer = MemAlloc(PROGRESS_TEXT_MAX * 2);
  UiText_CopyMessageStringFar(messageVariant + MsgProgressHelp, buffer,
                              PROGRESS_TEXT_MAX);
  UiText_RenderWideStringAtOffsetFar(buffer, destination, 0, -1);
  MemFree(buffer);
}

// Draws member ownerId's status page into window: the top rows (ItemMenu_
// DrawOwnerStatus), the ailment list or "Normal", and except on the chooser
// page the Djinn counts by element and, in mode 1, the elemental levels,
// Power and Resist. mode's 0x100 keeps what is drawn instead of clearing it.
void CharacterMenu_DrawStatusAilments(struct UiWindow *window, s32 ownerId,
                                      s32 mode) {
  struct CharacterMenuState *menu = gMenuWork;
  struct BattleUnit *unit;
  s32 djinnRow;
  s32 hasDjinn;
  s32 keepDrawn;
  s32 i;
  u8 ailments[8];

  if (Party_SumDjinnCountsFar(-1) != 0)
    hasDjinn = 1;
  else
    hasDjinn = 0;
  unit = UnitGetFar(ownerId);
  if ((mode & 0xff) == 1)
    djinnRow = 7;
  else
    djinnRow = 10;
  menu->statusCursor->active = ICON_SHOWN;
  ItemMenu_DrawOwnerStatus(window, ownerId, mode);
  CharacterMenu_BuildAvailability(ailments, 1, ownerId);
  CharacterMenu_UpdateSelectionIcons(ailments);
  keepDrawn = mode & 0x100;
  if (keepDrawn == 0)
    UiWindow_ClearInteriorTilesFar(window, 0, 40, 96, 96);
  // The ailments, one a row, or "Normal".
  i = 0;
  if (ailments[CHARACTER_DOWN] != 0) {
    UiText_DrawCharacterAtOffsetFar(MsgDownLabel, window, 16, 40);
    i = 1;
  }
  if (ailments[CHARACTER_POISON] != 0) {
    UiText_DrawCharacterAtOffsetFar(MsgPoisonLabel, window, 16, i * 16 + 40);
    i++;
  }
  if (ailments[CHARACTER_VENOM] != 0) {
    UiText_DrawCharacterAtOffsetFar(MsgVenomLabel, window, 16, i * 16 + 40);
    i++;
  }
  if (ailments[CHARACTER_CURSE] != 0) {
    UiText_DrawCharacterAtOffsetFar(MsgCurseLabel, window, 16, i * 16 + 40);
    i++;
  }
  if (ailments[CHARACTER_HAUNT] != 0) {
    UiText_DrawCharacterAtOffsetFar(MsgHauntLabel, window, 16, i * 16 + 40);
    i++;
  }
  if (i == 0)
    UiText_DrawCharacterAtOffsetFar(MsgNormalLabel, window, 0, 40);
  CharacterMenu_UpdateSelectionIcons(ailments);
  ItemMenu_ApplyFlags(ailments);
  if (menu->page == 3)
    return;
  if (keepDrawn == 0) {
    WaitFrames(1);
    UiWindow_ClearInteriorTilesFar(window, 64, 56, 224, 96);
  }
  // The four element icons, then the Djinn counts and, in mode 1, the
  // elemental levels, Power and Resist.
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  if (mode == 1 || hasDjinn == 1) {
    UiWindow_SetTilemapEntryFar(window, 1, 15, djinnRow, 4);
    UiWindow_SetTilemapEntryFar(window, 2, 19, djinnRow, 4);
    UiWindow_SetTilemapEntryFar(window, 3, 23, djinnRow, 4);
    UiWindow_SetTilemapEntryFar(window, 4, 27, djinnRow, 4);
  }
  if (hasDjinn)
    UiText_DrawCharacterAtOffsetFar(MsgDjinnLabel, window, 64,
                                    djinnRow * 8 + 8);
  if (mode == 1) {
    if (!hasDjinn)
      djinnRow--;
#if EDITION_INTERNATIONAL
    UiText_DrawStringAtOffsetFar(gMenu_LvString, window, 64, djinnRow * 8 + 16);
#else
    UiText_DrawStringInWindowFar(gMenu_LvString, window, 64, djinnRow * 8 + 16);
#endif
    UiText_DrawCharacterAtOffsetFar(MsgPowerLabel, window, 64,
                                    djinnRow * 8 + 24);
    UiText_DrawCharacterAtOffsetFar(MsgPowerLabel + 1, window, 64,
                                    djinnRow * 8 + 32);
  }
  for (i = 0; i <= 3; i++) {
    if (hasDjinn)
      UiText_DrawNumberInWindowFar(unit->djinnOwnedCounts[i], 1, window,
                                   i * 32 + 120, djinnRow * 8 + 8);
    if ((mode & 0xff) == 1) {
      if (hasDjinn) {
        UiText_DrawNumberInWindowFar(unit->djinnActiveCounts[i], 1, window,
                                     i * 32 + 104, djinnRow * 8 + 8);
        UiText_DrawStringInWindowFar(gMenu_SlashString, window, i * 32 + 112,
                                     djinnRow * 8 + 8);
      }
      UiText_DrawNumberInWindowFar(Unit_GetElementLevelFar(ownerId, i), 2,
                                   window, i * 32 + 112, djinnRow * 8 + 16);
      UiText_DrawNumberInWindowFar(unit->element[i].power, 3, window,
                                   i * 32 + 104, djinnRow * 8 + 24);
      UiText_DrawNumberInWindowFar(unit->element[i].resistance, 3, window,
                                   i * 32 + 104, djinnRow * 8 + 32);
    }
  }
}

#if EDITION_INTERNATIONAL
// Spins for 256 loop passes (international editions only).
void Runtime_RunBusyWaitDelay(void) {
  s32 count = 0xff;

spin:
  count--;
  if (count >= 0)
    goto spin;
}

#endif

// The stat column of the status page's top rows.
#if EDITION_INTERNATIONAL
#define STAT_DIGITS 3
#define STAT_X 200
#else
#define STAT_DIGITS 4
#define STAT_X 192
#endif

// Draws the top rows of member owner's status page into window: name, class,
// level, experience, HP and PP on the left, Attack, Defense, Agility and Luck
// on the right. flags' 0x100 keeps what is drawn instead of clearing it.
void ItemMenu_DrawOwnerStatus(struct UiWindow *window, s32 owner, s32 flags) {
  struct InventoryMenuState *menu;
  struct BattleUnit *unit;

  menu = gMenuWork;
  unit = UnitGetFar(owner);
  menu->cursor->active = ICON_SHOWN;
  flags &= 0x100;
  if (flags == 0) {
    UiWindow_ClearInteriorTilesFar(window, 0, 0, 128, 40);
  }
  UiText_DrawStringAtOffsetFar(unit->name, window, 40, 0);
  UiText_DrawCharacterAtOffsetFar(unit->classIndex + MsgClassName, window, 0,
                                  32);
#if EDITION_INTERNATIONAL
  UiText_DrawStringAtOffsetFar(gMenu_LvString, window, 104, 0);
#else
  UiText_DrawStringInWindowFar(gMenu_LvString, window, 104, 0);
#endif
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  UiText_DrawNumberInWindowFar(unit->level, 2, window, 128, 0);
#if EDITION_INTERNATIONAL
  UiText_DrawStringAtOffsetFar(gMenu_HpString, window, 40, 16);
#else
  UiText_DrawStringInWindowFar(gMenu_HpString, window, 40, 16);
#endif
  UiText_DrawNumberInWindowFar(unit->hp, 4, window, 72, 16);
  UiText_DrawNumberInWindowFar(unit->maxHp, 4, window, 112, 16);
  UiText_DrawStringInWindowFar(gMenu_SlashString, window, 104, 16);
#if EDITION_INTERNATIONAL
  UiText_DrawStringAtOffsetFar(gMenu_PpString, window, 40, 24);
#else
  UiText_DrawStringInWindowFar(gMenu_PpString, window, 40, 24);
#endif
  UiText_DrawNumberInWindowFar(unit->pp, 4, window, 72, 24);
  UiText_DrawNumberInWindowFar(unit->maxPp, 4, window, 112, 24);
  UiText_DrawStringInWindowFar(gMenu_SlashString, window, 104, 24);
  UiText_DrawCharacterAtOffsetFar(MsgExpLabel, window, 40, 8);
  UiText_DrawNumberInWindowFar(unit->experience, 7, window, 88, 8);
  if (flags == 0) {
    WaitFrames(1);
    UiWindow_ClearInteriorTilesFar(window, 144, 0, 224, 40);
  }
  // The labels "Attack", "Defense", "Agility" and "Luck", 23 to 20 messages
  // before "Exp".
  UiText_DrawCharacterAtOffsetFar(MsgExpLabel - 23, window, 152, 0);
  UiText_DrawCharacterAtOffsetFar(MsgExpLabel - 22, window, 152, 8);
  UiText_DrawCharacterAtOffsetFar(MsgExpLabel - 21, window, 152, 16);
  UiText_DrawCharacterAtOffsetFar(MsgExpLabel - 20, window, 152, 24);
  UiText_DrawNumberInWindowFar(unit->atk, STAT_DIGITS, window, STAT_X, 0);
  UiText_DrawNumberInWindowFar(unit->def, STAT_DIGITS, window, STAT_X, 8);
  UiText_DrawNumberInWindowFar(unit->agi, STAT_DIGITS, window, STAT_X, 16);
  UiText_DrawNumberInWindowFar(unit->luck, STAT_DIGITS, window, STAT_X, 24);
}

// Fills ailments[0..4] (CharacterAilment) with member id's ailments: down
// (only when requested is 1), poison or venom, curse and haunt. Returns how
// many they have.
s32 CharacterMenu_BuildAvailability(u8 *ailments, s32 requested, s32 id) {
  struct BattleUnit *unit;
  s32 i;
  s32 count;
  s32 poison;

  unit = UnitGetFar(id);
  for (i = 4; i >= 0; i--)
    ailments[i] = 0;

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
  if (unit->restraint != 0) {
    ailments[CHARACTER_CURSE] = 1;
    count++;
  }
  if (unit->haunt != 0) {
    ailments[CHARACTER_HAUNT] = 1;
    count++;
  }
  return count;
}

#if EDITION_INTERNATIONAL
#define GROUP_LEN 5
#else
#define GROUP_LEN 6
#endif

// Fills result with the item list of the member in pane index for the
// equipment pages: their unit, the menu's item count, and the page and row of
// the item chosen last for them, GROUP_LEN items to a page. Returns 1.
s32 Menu_BuildPageResult(struct MenuResult *result, s32 index) {
  s32 unit;
  struct InventoryMenuState *menu = gMenuWork;
  s32 itemCount;
  s32 row;
  s32 page;
  s32 pageCount;
  s32 selected;

  unit = (s32)UnitGetFar(menu->paneOwner[index]);
  itemCount = menu->itemCount;
  selected = menu->selectedIndexByOwner[menu->paneOwner[index]];
  if ((selected + 1) > itemCount) {
    selected = itemCount - 1;
  }
  if (itemCount == 0) {
    selected = 0;
  }
  page = selected / GROUP_LEN;
  row = selected % GROUP_LEN;
  pageCount = itemCount / GROUP_LEN;
  if (itemCount % GROUP_LEN != 0) {
    pageCount++;
  }
  result->ownerState = unit;
  result->page = page;
  result->pageCount = pageCount;
  result->row = row;
  result->entryCount = itemCount;
  result->selectedIndex = selected;
  return 1;
}

static __inline__ u32 XorWord(u32 word, u32 mask) { return word ^ mask; }

extern const struct TileMask gMenu_PatternRowMasks[];

// Builds twelve 8x8 tiles in the window tile area: two sets of six, each a
// fill of colour 4 with the rows of gMenu_PatternRowMasks XORed in, the first
// set growing row by row and the second skipping its top row.
void Menu_BuildPatternTiles(void) {
#if EDITION_INTERNATIONAL
  u32 *vram = (u32 *)0x06005000;
#else
  u32 *vram = (u32 *)0x06004000;
#endif
  s32 set;
  s32 n;

  for (set = 0; set < 2; set++) {
    for (n = 0; n < 6; n++) {
      u32 *tile = vram + set * 0x60 + n * 0x10;
      s32 x;

      Iwram_FillWords(tile, 64, 0x44444444);
      for (x = 1; x <= 7; x++) {
        s32 mask = n;

        if (set == 1 && x <= 1) {
          continue;
        }
        if (set == 0 && n > x - 2) {
          mask = x - 2;
          if (mask < 0) {
            mask = 0;
          }
        }
        tile[x] = XorWord(tile[x], gMenu_PatternRowMasks[mask].word0);
        tile[x + 8] = XorWord(tile[x + 8], gMenu_PatternRowMasks[mask].word1);
      }
    }
  }
}

// Draws a Psynergy's range icon at (x, y) of window: three tiles from the
// range icon tiles, the left one mirrored.
void PsynergyMenu_DrawRange(struct UiWindow *window, s32 x, s32 y, s32 range,
                            s32 unused) {
  s32 n;
  s32 tile;

  (void)unused;

  n = range * 2;
  tile = n + RANGE_ICON_TILE + 1;
  UiWindow_SetTilemapEntryFar(window, 0x400 | tile, x, y, 0); // flipped
  UiWindow_SetTilemapEntryFar(window, n + RANGE_ICON_TILE, x + 1, y, 0);
  UiWindow_SetTilemapEntryFar(window, tile, x + 2, y, 0);
}

// Updates the Psynergy page for the row under the cursor: the Psynergy's
// description in the info window, where it can be used (in the field, in
// battle, or anywhere), and the row highlights with the range icons.
s32 PsynergyMenu_DrawRangePage(struct UiWindow *window, s32 unused,
                               struct MenuResult *state) {
  struct PsynergyMenuState *menu;
  struct BattleAct *ability;
  s32 row;
  s32 base;

  menu = gMenuWork;
  state->selectedIndex = state->page * PAGE_ROWS + state->row;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) ||                      \
    defined(TBS_EDITION_FR)
  RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
  UiWindow_ClearInteriorTilesFar(menu->infoWindow, 0, 0, 224, 8);
#else
  RenderOutput_ClearListFar(menu->infoWindow);
#endif
  WaitFrames(1);
  if (menu->psynergies[state->selectedIndex] != 0) {
#if EDITION_INTERNATIONAL
    UiText_DrawCharacterAtOffsetFar(
#else
    UiText_DrawResourceFar(
#endif
        (menu->psynergies[state->selectedIndex] & OWNER_ACTION_ID_MASK) +
            MsgAbilityDescription,
        menu->infoWindow, 0, 0);
    ability = BattleActGet(menu->psynergies[state->selectedIndex] &
                           OWNER_ACTION_ID_MASK);
#if EDITION_INTERNATIONAL
    UiWindow_ClearInteriorTilesFar(window, 0, 96, 224, 104);
    row = 0;
    if (ability->type0c != 0 ||
        (ability->flags & ACTION_USABLE_IN_FIELD) != 0) {
      row = 2;
    }
    if ((ability->flags & ACTION_USABLE_IN_BATTLE) != 0) {
      row |= 1;
    }
    if (row == 3) {
      UiText_DrawCharacterAtOffsetFar(MsgUsableAnywhere, window, 0, 96);
    } else if (row == 2) {
      UiText_DrawCharacterAtOffsetFar(MsgUsableInField, window, 0, 96);
    } else if (row == 1) {
      UiText_DrawCharacterAtOffsetFar(MsgUsableInBattle, window, 0, 96);
    }
#else
    UiWindow_ClearInteriorTilesFar(window, 0, 72, 64, 96);
    row = 0;
    if (ability->type0c != 0 ||
        (ability->flags & ACTION_USABLE_IN_FIELD) != 0) {
      UiText_DrawCharacterAtOffsetFar(MsgUsableInField, window, 0, 72);
      row = 1;
    }
    if ((ability->flags & ACTION_USABLE_IN_BATTLE) != 0) {
      UiText_DrawCharacterAtOffsetFar(MsgUsableInBattle, window, 0,
                                      row * 8 + 72);
      row++;
    }
    UiText_DrawCharacterAtOffsetFar(MsgCanBeUsed, window, 0, row * 8 + 72);
#endif
  }

  base = state->page * PAGE_ROWS;
  for (row = 0; row <= PAGE_ROWS - 1; row++) {
    if (row == state->row) {
      ability =
          BattleActGet(menu->psynergies[base + row] & OWNER_ACTION_ID_MASK);
      if (ability->element != 4) {
        UiWindow_SetTilemapEntryFar(window, ability->element + 1, 24,
                                    row * 2 + LIST_FIRST_ROW, 0);
        Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 15, 1,
                                  14);
        Render_SetTilemapFlagRect(window, 25, row * 2 + LIST_FIRST_ROW, 3, 1,
                                  14);
      } else {
        Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 19, 1,
                                  14);
      }
    } else {
      ability =
          BattleActGet(menu->psynergies[base + row] & OWNER_ACTION_ID_MASK);
      if (ability->element != 4) {
        UiWindow_SetTilemapEntryFar(window, ability->element + 1, 24,
                                    row * 2 + LIST_FIRST_ROW, 4);
        Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 15, 1,
                                  15);
        Render_SetTilemapFlagRect(window, 25, row * 2 + LIST_FIRST_ROW, 3, 1,
                                  15);
      } else {
        Render_SetTilemapFlagRect(window, 9, row * 2 + LIST_FIRST_ROW, 19, 1,
                                  15);
      }
    }
  }
  WaitFrames(1);
  return 1;
}

s32 PsynergyMenu_DrawListPage(struct UiWindow *window, s32 unused,
                              const struct MenuResult *res) {
  struct PsynergyMenuState *menu = gMenuWork;
  struct BattleUnit *owner;
  u32 first;
  u32 rows;
  u8 row;
  s32 offset;

  (void)unused;

  owner = UnitGetFar(menu->ownerIds[0]);

  RenderOutput_RedrawSavedRectFar(window);

  first = res->page * PAGE_ROWS;
  rows = (u8)(res->entryCount - first);
  if (rows > PAGE_ROWS) {
    rows = PAGE_ROWS;
  }

  Menu_SetPageIcons(PAGE_ROWS, first, window, LIST_ICONS_X, LIST_ICONS_Y);
  Menu_DrawPageIndicator(window, res->entryCount, PAGE_ROWS, res->page, 28);

  UiText_DrawCharacterAtOffsetFar(MsgPsynergyPp, window, LIST_PP_X, 0);

  row = 0;
  if (rows > row) {
    offset = first;
    do {
      struct BattleAct *act;
      s32 msg;
      s32 y;
      s32 range;
      act = BattleActGet(OWNER_ACTION_ID_MASK & menu->psynergies[offset]);
      msg = (menu->psynergies[offset] & OWNER_ACTION_ID_MASK) + MsgAbilityName;
      y = row * 16 + LIST_FIRST_ROW * 8;

      UiText_DrawCharacterAtOffsetFar(msg, window, LIST_NAME_X, y);
      UiText_DrawNumberAtOffsetFar(act->ppCost, 2, window, LIST_PP_NUMBER_X, y);

      range = act->range;
      if (range == 0xff) {
        range = 11;
      } else {
        range--;
      }
      PsynergyMenu_DrawRange(window, 25, row * 2 + LIST_FIRST_ROW, range, 0);

      row++;
      offset++;
    } while (rows > row);
  }

  if (menu->psynergyCount == 0) {
    UiText_DrawCharacterAtOffsetFar(MsgNoPsynergy, window, 96, LIST_EMPTY_Y);
  }

  UiText_DrawStringAtOffsetFar(owner->name, (void *)window, 40, 0);
  UiText_DrawCharacterAtOffsetFar(owner->classIndex + MsgClassName, window, 0,
                                  32);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR)
  UiText_DrawStringAtOffsetFar(&gMenu_LvString, (void *)window, 0, 48);
#else
  UiText_DrawStringInWindowFar(&gMenu_LvString, window, 0, 48);
#endif
  UiText_DrawNumberInWindowFar(owner->level, 2, window, 24, 48);

  return 1;
}

struct BattleUnit;

s32 PsynergyMenu_SelectAction(void) {

  struct PsynergyMenuState *menu;
  s32 result;
  s32 done;
  s32 redraw;
  s32 first;
  s32 nav;
  s32 tab;
  s32 i;
  struct MenuResult state;

  menu = gMenuWork;
  result = 0;
  done = 0;
  Menu_BuildPatternTiles();
  RenderOutput_RedrawSavedRectFar(menu->messageWindow);
  UiWindow_UpdateOrCreate(&menu->infoWindow, 0, 0, 30, 5, 2);
  for (i = 3; i >= 0; i--)
    menu->slotY[i] = -16;
  {
    s32 priority = 245;
    struct RenderOutput **icons = menu->entryIcons;

    for (i = 31; i >= 0; i--) {
      struct RenderOutput *icon = *icons++;

      if (icon != 0)
        icon->sentinel = priority;
    }
  }
  Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
  Menu_SpawnIconEntries(menu, menu->messageWindow);
  UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp, menu->statusWindow,
                                  SWITCH_HELP_X, -24);
  UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp + 2,
                                  menu->statusWindow, 0, -24);

  while (done == 0 && GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    ItemMenu_PosCategory();
    RenderOutput_RedrawSavedRectFar(menu->statusWindow);
    menu->psynergyCount = PsynergyMenu_CollectActions(
        UnitGetFar(menu->ownerIds[0]), menu->psynergies, 0);
    WaitFrames(1);
    Menu_BuildPageResult(&state, 0);
    PsynergyMenu_DrawPreparedPsynergyIcons(menu->statusWindow,
                                           menu->ownerIds[0]);
    redraw = 1;
    first = 1;

    while (GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
      if (redraw != 0) {
        redraw = 0;
        if (first != 0) {
          first = 0;
          PsynergyMenu_DrawListPage(menu->statusWindow, 0, &state);
        }
        PsynergyMenu_DrawRangePage(menu->statusWindow, 0, &state);
        WaitFrames(1);
      }
      WaitFrames(1);
      nav = Menu_HandlePageInput(0, state.entryCount, PAGE_ROWS, &state.row,
                                 &state.page);
      menu->paneIcon[0]->active = ICON_SHOWN;
#if EDITION_INTERNATIONAL
      UiMenu_PositionCursor(55, state.row * 16 + 60);
#else
      UiMenu_PositionCursor(58, state.row * 16 + 52);
#endif
      if (nav == 1) {
        first = 1;
        redraw = 1;
      }
      if (nav == 0)
        redraw = 1;
      if (nav == -1)
        redraw = 0;
      if (gKeysPressed & KEY_A) {
        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        result = 1;
        done = 1;
        break;
      }
      if (gKeysPressed & KEY_B) {
        AudioCommand_PlayFar(SOUND_MENU_CANCEL);
        result = -1;
        done = 1;
        Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
        break;
      }
      if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L)) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        tab = menu->tabIndex[0];
        menu->selectedIndexByOwner[menu->ownerTable[tab]] = state.selectedIndex;
        if (gKeysRepeat & KEY_R)
          tab++;
        else
          tab--;
        tab = (tab + menu->ownerCount) % menu->ownerCount;
        menu->selectedOwner = menu->ownerTable[tab];
        menu->ownerIds[0] = menu->ownerTable[tab];
        menu->tabIndex[0] = tab;
        PsynergyMenu_CallIconRoutineWithValue((s32)menu, menu->ownerTable[tab]);
        break;
      }
    }
  }
  RenderOutput_ClearListFar(menu->infoWindow);
  RenderOutput_RedrawSavedRectFar(menu->infoWindow);
  ItemMenu_HideAllIcons();
  RenderOutput_ClearListFar(menu->messageWindow);
  RenderOutput_RedrawSavedRectFar(menu->statusWindow);
  return result;
}

s32 PsynergyMenu_ActionReturnTrue(void) { return 1; }

void PsynergyMenu_DrawPreparedPsynergyIcons(s32 unused, s32 owner_id) {
  struct PsynergyMenuState *menu = gMenuWork;

  UnitGetFar(owner_id);
  ItemMenu_HideAllIcons();
  PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
}

void PsynergyMenu_PreparedIconsNoOp(void) {}

void PsynergyMenu_PreparedIconsSecondNoOp(void) {}

#if EDITION_INTERNATIONAL
#define EQUIP_SLOT_X 1
#define EQUIP_SLOT_WIDTH 12
#else
#define EQUIP_SLOT_X 2
#define EQUIP_SLOT_WIDTH 9
#endif

s32 ItemMenu_DrawEquipPage(struct UiWindow *window, s32 unused,
                           struct MenuResult *state) {
  struct InventoryMenuState *menu;
  struct ItemDefinition *item;
  s32 base;
  s32 row;

  menu = gMenuWork;
  base = state->page * PAGE_ROWS;
  state->selectedIndex = base + state->row;
#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE) ||                      \
    defined(TBS_EDITION_FR)
  RenderOutput_RedrawSavedRectFar(menu->infoWindow);
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
  UiWindow_ClearInteriorTilesFar(menu->infoWindow, 0, 0, 224, 8);
#else
  RenderOutput_ClearListFar(menu->infoWindow);
#endif
  WaitFrames(1);
  if (menu->items[state->selectedIndex] != 0) {
#if EDITION_INTERNATIONAL
    UiText_DrawCharacterAtOffsetFar(
        (menu->items[state->selectedIndex] & ITEM_ID_MASK) + MsgItemPlainName,
        menu->infoWindow, 0, 0);
#else
    UiText_DrawResourceFar((menu->items[state->selectedIndex] & ITEM_ID_MASK) +
                               MsgItemPlainName,
                           menu->infoWindow, 0, 0);
#endif
  }

  for (row = 0; row < PAGE_ROWS; row++) {
    if (row == state->row) {
      item =
          Item_GetDirectFar(menu->items[state->selectedIndex] & ITEM_ID_MASK);
      if (item->element != 4) {
        UiWindow_SetTilemapEntryFar(window, item->element + 1, 27, row * 2 + 1,
                                    0);
        Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 13, 1, 14);
      } else {
        Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 14, 1, 14);
      }
    } else {
      item = Item_GetDirectFar(menu->items[base + row] & ITEM_ID_MASK);
      if (item->element != 4) {
        UiWindow_SetTilemapEntryFar(window, item->element + 1, 27, row * 2 + 1,
                                    4);
        Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 13, 1, 15);
      } else {
        Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 14, 1, 15);
      }
    }
  }

  for (row = 0; row <= 3; row++) {
    Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, row * 2 + 1,
                              EQUIP_SLOT_WIDTH, 1, 15);
  }

  if (menu->items[state->selectedIndex] & INVENTORY_EQUIPPED) {
    item = Item_GetDirectFar(menu->items[state->selectedIndex] & ITEM_ID_MASK);
    switch (item->type) {
    case 1:
      Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 1,
                                EQUIP_SLOT_WIDTH, 1, 14);
      break;
    case 4:
      Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 3,
                                EQUIP_SLOT_WIDTH, 1, 14);
      break;
    case 3:
      Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 5,
                                EQUIP_SLOT_WIDTH, 1, 14);
      break;
    case 2:
      Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 7,
                                EQUIP_SLOT_WIDTH, 1, 14);
      break;
    }
  }
  WaitFrames(1);
  return 1;
}

#if EDITION_INTERNATIONAL
#define SHOP_PAGE_ICONS_X 119
#define SHOP_ITEM_NAME_X 128
#define SHOP_STAT_X 80
#else
#define SHOP_PAGE_ICONS_X 123
#define SHOP_ITEM_NAME_X 136
#define SHOP_STAT_X 72
#endif

s32 Shop_DrawItemPage(struct UiWindow *window, s32 unused,
                      struct MenuResult *state) {
  struct InventoryMenuState *menu = gMenuWork;
  struct BattleUnit *unit;
  u16 *item;
  s32 first;
  u32 count;
  u32 row;
  s32 message;

  unit = UnitGetFar(menu->paneOwner[0]);
  UiWindow_ClearInteriorTilesFar(window, 128, 8, 224, 96);
  first = state->page * PAGE_ROWS;
  count = (u8)(state->entryCount - first);
  if (count > PAGE_ROWS)
    count = PAGE_ROWS;
  Menu_SetPageIcons(PAGE_ROWS, first, window, SHOP_PAGE_ICONS_X, 52);
  Menu_DrawPageIndicator(window, state->entryCount, PAGE_ROWS, state->page, 28);
  if (menu->itemCount == 0) {
#if defined(TBS_EDITION_DE)
    UiText_DrawCharacterAtOffsetFar(MsgItemMenuEmpty, window, 112, 8);
#else
    UiText_DrawCharacterAtOffsetFar(MsgItemMenuEmpty, window, 120, 8);
#endif
  } else {
    row = 0;
    if (count > row) {
      item = &menu->items[first];
      do {
        UiText_DrawCharacterAtOffsetFar((*item & ITEM_ID_MASK) + MsgItemName,
                                        window, SHOP_ITEM_NAME_X,
                                        (row << 4) + 8);
        row = (u8)(row + 1);
        item++;
      } while (count > row);
    }
  }
  UiText_DrawStringAtOffsetFar((s32)unit->name, window, 40, 0);
  message = MsgStatLabel;
  UiText_DrawCharacterAtOffsetFar(message, window, 32, 16);
  UiText_DrawCharacterAtOffsetFar(message + 1, window, 32, 24);
  UiText_DrawNumberInWindowFar(unit->atk, 3, window, SHOP_STAT_X, 16);
  UiText_DrawNumberInWindowFar(unit->def, 3, window, SHOP_STAT_X, 24);
  return 1;
}

s32 ItemMenu_SelectItem(void) {

  struct InventoryMenuState *menu;
  struct BattleUnit *owner;
  s32 result;
  s32 done;
  s32 redraw;
  s32 first;
  s32 nav;
  s32 tab;
  s32 i;
  u16 saved[15];
  struct MenuResult state;

  menu = gMenuWork;
  {
    s32 id = menu->paneOwner[0];

    result = 0;
    UnitGetFar(menu->ownerIds[id]);
  }
#if EDITION_INTERNATIONAL
  UiWindow_UpdateOrCreate(&menu->equipWindow, 0, 10, 15, 10, 2);
#else
  UiWindow_UpdateOrCreate(&menu->equipWindow, 0, 10, 13, 10, 2);
#endif
  ItemMenu_PosCategory();

  for (i = 0; i < 32; i++) {
    struct RenderOutput *icon = menu->entryIcons[i];

    if (icon != 0)
      icon->sentinel = 240;
  }

  Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);
  UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp, menu->statusWindow,
                                  64, -24);
  UiText_DrawCharacterAtOffsetFar(MsgSwitchCharacterHelp + 3,
                                  menu->statusWindow, 0, -24);
  ItemMenu_PosCategory();
  WaitFrames(1);
  Menu_SpawnIconEntries(menu, menu->messageWindow);

  done = 0;
  while (done == 0 && GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
    ItemMenu_PosCategory();
    RenderOutput_RedrawSavedRectFar(menu->statusWindow);
    owner = UnitGetFar(menu->paneOwner[0]);
    for (i = 0; i <= 14; i++)
      saved[i] = owner->inventory[i];
    menu->itemCount = ItemMenu_Collect(owner, menu->items, 0);
    InventoryMenu_SortByListOrder(menu->items, 0);
    Menu_BuildPageResult(&state, 0);
    RenderOutput_RedrawSavedRectFar(menu->equipWindow);
    ItemMenu_DrawCategory(menu->equipWindow, menu->paneOwner[0], 1);
    WaitFrames(1);
    ItemMenu_DrawIcons(menu->items, 0);
    redraw = 1;
    first = 1;

    while (GameFlag_TestFar(FLAG_MENU_START_PRESSED) == 0) {
      if (redraw != 0) {
        RenderOutput_ClearListFar(menu->infoWindow);
        redraw = 0;
        if (first != 0) {
          first = 0;
          Shop_DrawItemPage(menu->statusWindow, 0, &state);
        }
        ItemMenu_DrawEquipPage(menu->statusWindow, 0, &state);
      }
      menu->paneIcons[0]->active = ICON_SHOWN;
      UiMenu_PositionCursor(96, state.row * 16 + 52);
      WaitFrames(1);

      nav = Menu_HandlePageInput(0, state.entryCount, PAGE_ROWS, &state.row,
                                 &state.page);
      if (nav == 1) {
        first = 1;
        redraw = 1;
      }
      if (nav == 0)
        redraw = 1;
      if (nav == -1)
        redraw = 0;

      if (gKeysPressed & KEY_A) {
        AudioCommand_PlayFar(0x70);
        result = 1;
        done = 1;
        break;
      }
      if (gKeysPressed & KEY_B) {
        AudioCommand_PlayFar(0x71);
        result = -1;
        done = 1;
        break;
      }
      if ((gKeysRepeat & KEY_R) || (gKeysRepeat & KEY_L)) {
        AudioCommand_PlayFar(0x6f);
        tab = menu->paneIndex[0];
        menu->selectedIndexByOwner[menu->ownerIds[tab]] = state.selectedIndex;
        if (gKeysRepeat & KEY_R)
          tab++;
        else
          tab--;
        for (i = 0; i <= 14; i++)
          owner->inventory[i] = saved[i];
        tab = (tab + menu->partyCount) % menu->partyCount;
        menu->selectedOwner = menu->ownerIds[tab];
        menu->paneOwner[0] = menu->ownerIds[tab];
        menu->paneIndex[0] = tab;
        PsynergyMenu_CallIconRoutineWithValue((s32)menu, menu->ownerIds[tab]);
        break;
      }
    }
  }

  RenderOutput_ClearListFar(menu->infoWindow);
  RenderOutput_RedrawSavedRectFar(menu->infoWindow);
  RenderOutput_ClearListFar(menu->messageWindow);
  ItemMenu_HideAllIcons();
  RenderOutput_RedrawSavedRectFar(menu->statusWindow);
  Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
  return result;
}

s32 Menu_MoveGridCursor(s32 *arg0, s32 *arg1, s32 arg2) {
  s32 row;
  s32 col;

  col = *arg0;
  row = *arg1;
  switch (arg2) {
  case 0x40:
    row -= 1;
    if (row < 0) {
      row = 5;
    }
    if (row <= 3) {
      if (row == 3) {
        if (col <= 4) {
          col = 0;
        } else {
          col = 1;
        }
      } else if (col > 1) {
        col = 1;
      }
      if ((row == 3) && (col == 1)) {
        row = 2;
      }
    }
    break;
  case 0x80:
    row += 1;
    if (row > 5) {
      row = 0;
    }
    if ((row == 3) && (col == 1)) {
      row = 4;
    }
    if (row == 4) {
      col = 0;
    }
    break;
  case 0x20:
    col -= 1;
    if (row == 3) {
      col += 1;
    } else if (row > 3) {
      if (col < 0) {
        col = 7;
      }
    } else if (col < 0) {
      col = 1;
    }
    break;
  case 0x10:
    col += 1;
    if (row == 3) {
      col -= 1;
    } else if (row > 3) {
      if (col > 7) {
        col = 0;
      }
    } else if (col > 1) {
      col = 0;
    }
    break;
  }
  *arg0 = col;
  *arg1 = row;
  return (row * 9) + col;
}

s32 Menu_ReservedStatusOne(void) { return 1; }

void ItemMenu_DrawCategory(struct UiWindow *window, s32 owner_id, s32 mode) {
  struct InventoryMenuState *menu = gMenuWork;
  u16 *items;

  UnitGetFar(owner_id);
  ItemMenu_PosCategory();
  ItemMenu_HideAllIcons();
  UiText_DrawCharacterAtOffsetFar(MsgEquipSlotLabels, window, 0, 0);
  UiText_DrawCharacterAtOffsetFar(MsgEquipSlotLabels + 1, window, 0, 32);
  UiText_DrawCharacterAtOffsetFar(MsgEquipSlotLabels + 2, window, 0, 16);
  UiText_DrawCharacterAtOffsetFar(MsgEquipSlotLabels + 3, window, 0, 48);
  items = menu->items;
  ItemMenu_DrawEquippedItemNames(window, items);
  if (mode == 0) {
    WaitFrames(1);
    ItemMenu_DrawIcons(items, 1);
    ItemMenu_ArrangeCategoryItemIcons(items);
  }
}

#if EDITION_INTERNATIONAL
#define NAME_X 8
#else
#define NAME_X 16
#endif

void ItemMenu_DrawEquippedItemNames(struct UiWindow *window, u16 *items) {
  s32 i;
  s32 item;

  for (i = 0; i < 15; i++) {
    if (items[i] & INVENTORY_EQUIPPED) {
      item = items[i] & ITEM_ID_MASK;
      switch (Item_GetDirectFar(item)->type) {
      case 1:
        UiText_DrawCharacterAtOffsetFar(item + MsgItemName, window, NAME_X, 8);
        break;
      case 2:
        UiText_DrawCharacterAtOffsetFar(item + MsgItemName, window, NAME_X, 56);
        break;
      case 3:
        UiText_DrawCharacterAtOffsetFar(item + MsgItemName, window, NAME_X, 40);
        break;
      case 4:
        UiText_DrawCharacterAtOffsetFar(item + MsgItemName, window, NAME_X, 24);
        break;
      }
    }
  }
}

void Menu_PlaceEntryObjectsInGrid(s32 originX, s32 originY, s32 phase) {
  s32 i;
  struct RenderOutput *obj;
  struct RenderOutput **table;

  i = 0;
  table = ((struct InventoryMenuState *)gMenuWork)->entryIcons;
  do {
    obj = *table++;
    if (obj != NULL) {
      Menu_PlaceEntryObjectInGrid(obj, i, originX, originY, phase);
    }
    i += 1;
  } while (i <= 0x1F);
}

void Menu_PlaceEntryObjectInGrid(struct RenderOutput *obj, s32 index,
                                 s32 originX, s32 originY, s32 phase) {
  s32 no;

  no = index;
  if (no > 0x1F) {
    no = 0;
  }
  obj->y = (no / phase * 0x10) + originY;
  obj->x = (no % phase * 0x10) + originX;
  UiIcon_PrepareObject(obj);
}

void ItemMenu_ArrangeCategoryItemIcons(u16 *items) {
  struct InventoryMenuState *state;
  struct RenderOutput *icon;
  s32 i;

  state = gMenuWork;
  ItemMenu_PosCategory();
  for (i = 0; i < 15; i++) {
    if (items[i] != 0 && (items[i] & INVENTORY_EQUIPPED) != 0) {
      icon = state->entryIcons[i];
      if (icon != 0) {
        switch (Item_GetDirectFar(items[i] & ITEM_ID_MASK)->type) {
        case 1:
          icon->x = 216;
          icon->y = 32;
          break;
        case 2:
          icon->x = 216;
          icon->y = 80;
          break;
        case 3:
          icon->x = 216;
          icon->y = 64;
          break;
        case 4:
          icon->x = 216;
          icon->y = 48;
          break;
        }
        UiIcon_PrepareObject(icon);
      }
    }
  }
}

void ItemMenu_PosCategory(void) {
  struct InventoryMenuState *state = gMenuWork;
  s32 x = 248;
  struct RenderOutput **entry = state->entryIcons;
  s32 y = 168;
  s32 remaining = 31;

  do {
    struct RenderOutput *object = *entry++;

    if (object != 0) {
      object->x = x;
      object->y = y;
      UiIcon_PrepareObject(object);
    }
    remaining--;
  } while (remaining >= 0);
}

s32 Menu_CreateEightEntryObjects(struct UiWindow *window) {
  struct RenderOutput **slot;
  struct RenderOutput *obj;
  s32 i;
  struct InventoryMenuState *state;
  s32 param;

  state = gMenuWork;
  i = 0;
  param = 0xA8;
  slot = state->categoryIcons;
  do {
    obj = RenderOutput_CreateFromResourceFar(2, i, window, 0xF8, param);
    i += 1;
    *slot = obj;
    slot += 1;
  } while (i <= 7);
  return 1;
}

void ItemMenu_ApplyFlags(const u8 *flags) {
  // The y-field pointer is the loop's 18th real insn before combine; loop.c
  // reduces the flags[index] giv only while its 17 reaches the insn count.
  /* FAKEMATCH: the y-field pointer keeps the category flag index a counter, not
   * a walking pointer, as native; entry->y = value in this loop, a for loop
   * over categoryIcons[index], ++index in the test or the two tests nested
   * differ 22, value as s16, u16 or s8 23 to 28, value += 16 before the
   * sentinel store 2 (the add moves ahead of the call). */
  struct InventoryMenuState *base;
  struct RenderOutput **slot;
  struct RenderOutput *entry;
  s32 index;
  s32 value;

  base = gMenuWork;
  ItemMenu_ResetCategory();
  index = 0;
  slot = base->categoryIcons;
  value = 88;
  do {
    entry = *slot++;
    if (entry != 0 && flags[index] != 0) {
      entry->x = 8;
      {
        s16 *y = &entry->y;

        *y = value;
      }
      entry->sentinel = 240;
      UiIcon_PrepareObject(entry);
      value += 16;
    }
    index++;
  } while (index <= 4);
}

void ItemMenu_ResetCategory(void) {
  struct InventoryMenuState *state = gMenuWork;
  s32 index;

  for (index = 0; index < 5; index++) {
    struct RenderOutput *object = state->categoryIcons[index];

    if (object != 0) {
      object->x = 248;
      object->y = 168;
      object->sentinel = 240;
      UiIcon_PrepareObject(object);
    }
  }
}

s32 CharacterMenu_UpdateSelectionIcons(const u8 *enabled) {
  struct InventoryMenuState *state = gMenuWork;
  s32 index = 0;

  do {
    if (enabled[index] != 0) {
      s32 kind;
      switch (index) {
      case CHARACTER_DOWN:
        kind = 16;
        break;
      case CHARACTER_POISON:
        kind = 1;
        break;
      case CHARACTER_VENOM:
        kind = 2;
        break;
      case CHARACTER_CURSE:
        kind = 15;
        break;
      case CHARACTER_HAUNT:
        kind = 7;
        break;
      default:
        kind = 0;
        break;
      }
      Resource_LoadByModeIntoSlotFar(8, kind,
                                     (u8)state->categoryIcons[index]->index, 0);
    }
    index++;
  } while (index <= 4);
  return 1;
}

void Item_PrepareUsePalette(void) {
  Palette_CopyObjectBankToBackground14();
  Palette_LightenBankHighlight(13);
}

void Item_UseNoOpCallback(void) {}

s32 Item_Use(s32 slot, s32 ownerId, s32 targetId) {
  struct InventoryMenuState *work;
  s32 result;
  s32 itemId;
  struct BattleUnit *owner;
  struct ItemDefinition *item;

  owner = UnitGetFar(ownerId);
  work = gMenuWork;
  itemId = ITEM_ID_MASK & owner->inventory[slot];
  item = Item_GetDirectFar(itemId);
  result = BattleEffect_ApplyToTargets(OWNER_ACTION_ID_MASK & item->actionId,
                                       ownerId, targetId, 1);
  if (result != -1) {
    item = Item_GetDirectFar(owner->inventory[slot]);
    if (item->useType == 1) {
      Inventory_RemoveFar(ownerId, slot);
      work->itemCount = ItemMenu_Collect(owner, work->items, 0);
    }
    if (item->useType == 4) {
      if (itemId == 0xb8)
        itemId = 0xb9;
      owner->inventory[slot] = itemId;
    }
    result = 0;
  }
  return result;
}

s32 Item_ReturnOne(void) { return 1; }

s32 BattleEffect_ApplyToTargets(s32 effectId, s32 sourceId, s32 targetId,
                                s32 fixedScale) {
  struct BattleAct *effect;
  struct InventoryMenuState *runtime;
  struct BattleUnit *target;
  struct BattleUnit *source;
  s32 laterTarget;
  s32 changed;
  u8 index;
  s16 scale;
  s32 randomAdjust;
  s32 randomNonone;
  s32 amount;
  s32 resultCode;

  effect = BattleActGet(effectId);
  runtime = gMenuWork;
  changed = 0;
  resultCode = 0;
  laterTarget = 0;

  if (targetId != 9)
    target = UnitGetFar(targetId);
  else
    target = UnitGetFar(0);

  /* FAKEMATCH: the dead test loads effect->range ahead of the count
   * guard, where the native keeps it for the first pass through the
   * loop; it preserves the target-index and power-load registers.
   * Measured: without it the load moves past the guard and the
   * allocation shifts (485 instructions for 484, differ 49). */
  if (effect->range == 0xff && laterTarget != 0)
    resultCode = 3;

  for (index = 0; index < runtime->partyCount; index++) {
    if (effect->range == 0xff) {
      targetId = runtime->ownerIds[index];
      target = UnitGetFar(targetId);
    }

    amount = effect->power;
    switch (effect->flags & 0xf) {
    case BATTLE_DAMAGE_HP_HEAL:
      if (fixedScale == 0) {
        if (effect->element != 4) {
          s32 statOffset;

          source = UnitGetFar(sourceId);
          statOffset = effect->element;
          scale = source->element[statOffset].power;
        } else
          scale = 100;
        amount = BattleRecoveryCalc(amount, scale, 0x100);
      }

      if (target->hp <= 0) {
        if (laterTarget == 0)
          resultCode = 2;
      } else {
        if (target->hp == target->maxHp) {
          if (laterTarget == 0)
            resultCode = 4;
        } else {
          target->hp += amount;
          if (target->hp > target->maxHp) {
            amount -= target->hp - target->maxHp;
            target->hp = target->maxHp;
            if (laterTarget == 0)
              resultCode = 0;
          } else if (laterTarget == 0) {
            resultCode = 1;
          }
          UnitRatioFar(targetId);
          changed = 1;
          if (effect->range == 0xff) {
            laterTarget = 1;
            resultCode = 3;
          }
        }
      }
      break;

    case 9:
      randomAdjust = (Random16() * 4) >> 16;
      if (randomAdjust == 0) {
        randomAdjust = -1;
      } else {
        randomNonone = 1 ^ randomAdjust;
        randomAdjust = (u32)((-randomNonone) | randomNonone) >> 31;
        randomAdjust = 1 - randomAdjust;
      }

      switch (effectId & OWNER_ACTION_ID_MASK) {
      case 0x104:
        target->baseHp += amount + randomAdjust;
        resultCode = 0x10;
        changed = 1;
        break;
      case 0x105:
        target->basePp += amount + randomAdjust;
        resultCode = 0x11;
        changed = 1;
        break;
      case 0x108:
        target->baseAgility += amount + randomAdjust;
        resultCode = 0x12;
        changed = 1;
        break;
      case 0x109:
        target->baseLuck += amount;
        resultCode = 0x13;
        changed = 1;
        break;
      case 0x106:
        target->baseAttack += amount + randomAdjust;
        UiWork_PushValueSlotFar(3, 5);
        resultCode = 0x14;
        changed = 1;
        break;
      case 0x107:
        target->baseDefense += amount + randomAdjust;
        UiWork_PushValueSlotFar(4, 5);
        resultCode = 0x15;
        changed = 1;
        break;
      }
      break;

    case BATTLE_DAMAGE_PP_HEAL:
      if (target->pp == target->maxPp) {
        if (laterTarget == 0)
          resultCode = 7;
      } else {
        target->pp += amount;
        if (target->pp > target->maxPp) {
          amount -= target->pp - target->maxPp;
          target->pp = target->maxPp;
          if (laterTarget == 0)
            resultCode = 5;
        } else if (laterTarget == 0) {
          resultCode = 6;
        }
        UnitRatioFar(targetId);
        changed = 1;
        if (effect->range == 0xff) {
          laterTarget = 1;
          resultCode = 8;
        }
      }
      break;

    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 10:
      break;
    }

    switch (effect->effect) {
    case 1:
      if (target->hp <= 0 || target->hp == target->maxHp) {
        if (laterTarget == 0)
          resultCode = 2;
      } else {
        target->hp += amount;
        if (target->hp > target->maxHp) {
          target->hp = target->maxHp;
          if (laterTarget == 0)
            resultCode = 0;
        } else if (laterTarget == 0) {
          resultCode = 1;
        }
        UnitRatioFar(targetId);
        changed = 1;
      }
      break;

    case 2:
      if (target->pp == target->maxPp) {
        if (laterTarget == 0)
          resultCode = 7;
      } else {
        target->pp += amount;
        if (target->pp > target->maxPp) {
          target->pp = target->maxPp;
          if (laterTarget == 0)
            resultCode = 5;
        } else if (laterTarget == 0) {
          resultCode = 6;
        }
        UnitRatioFar(targetId);
        changed = 1;
      }
      break;

    case EFFECT_REVIVE_FULL:
      if (target->hp == 0) {
        target->hp = target->maxHp;
        UnitRatioFar(targetId);
        changed = 1;
        if (laterTarget == 0)
          resultCode = 0xc;
      } else if (laterTarget == 0) {
        resultCode = 0xd;
      }
      break;

    case EFFECT_REVIVE_HALF:
      if (target->hp == 0) {
        target->hp = target->maxHp / 2;
        UnitRatioFar(targetId);
        if (laterTarget == 0)
          resultCode = 0xc;
      } else if (laterTarget == 0) {
        resultCode = 0xd;
      }
      break;

    case EFFECT_REVIVE_80:
      if (target->hp == 0) {
        target->hp = target->maxHp * 7 / 10;
        UnitRatioFar(targetId);
        if (laterTarget == 0)
          resultCode = 0xc;
      } else if (laterTarget == 0) {
        resultCode = 0xd;
      }
      break;

    case EFFECT_CURE_POISON:
      if (target->poison != 0) {
        target->poison = 0;
        changed = 1;
        if (laterTarget == 0)
          resultCode = 0xa;
      } else if (laterTarget == 0) {
        resultCode = 0xb;
      }
      break;
    }

    if (effect->range != 0xff)
      break;
  }

  if (changed == 0) {
    runtime->messageOffset = resultCode;
    return -1;
  }

  for (index = 0; index < runtime->partyCount; index++)
    UnitCalcFar(runtime->ownerIds[index]);
  runtime->messageOffset = resultCode;
  return 0;
}

void Item_PlayUseAnimation(u32 item) {
  Ability_PlayUseAnimation(Item_GetDirectFar(item)->actionId &
                           OWNER_ACTION_ID_MASK);
}

void Ability_PlayUseAnimation(s32 action) {
  s32 animationType;
  u32 targetType;
  struct BattleAct *ability;

  ability = BattleActGet(action);
  animationType = 0xf & ability->flags;
  switch (animationType) {
  case 1:
    Audio_PlayCueReturnOne(0x7e);

  case 11:
    Audio_PlayCueReturnOne(0x7e);
    return;
  default:
    targetType = ability->effect - 1;
    switch (targetType) {
    case 4:
      Audio_PlayCueReturnOne(0x52);
      return;
    case 2:
      Audio_PlayCueReturnOne(0x54);
      return;
    default:
    case 3:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
      Audio_PlayCueReturnOne(0x5b);

    case 0:
    case 1:
    case 30:
    case 31:
      return;
    }
    break;
  }
}

s32 Menu_ReservedStatusZero(void) { return 0; }

s32 Menu_GetModuloOfSum(s32 arg0, s32 arg1) { return (arg0 + arg1) % arg1; }

s32 Menu_SetFirstObjectRowCoordinates(s32 arg0) {
  struct DjinnMenuWork *menu;
  s32 value;
  s32 i;
  menu = gMenuWork;
  arg0 += 0x3D;
  value = 0x20;
  for (i = 0; i < 4; i++) {
    menu->ownerX[i] = value;
    menu->ownerY[i] = arg0;
    value += 0x38;
  }
  return arg0;
}

s32 ItemMenu_Run(void) {
  struct DjinnMenuWork *menu;
  u32 oldMode;
  s32 state;
  struct DjinnMenuBackdrop *saved;
  s32 zero;

  menu = Runtime_AllocateHeapBlock(55, 0xa70);
  oldMode = gGameState.messageSpeed;
  gGameState.messageSpeed = 2;
  state = gMenuCtrlWork->suspended = 1;
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
  UiWindow_InitializeWork(0);
  saved = Runtime_BumpAllocateAlternatePool(0x2130);
  menu->backdrop = saved;
  saved->tutorialTimer = 0;
  saved->tutorialStep = 0;
  if (GameFlag_TestFar(0x16e)) {
    if (!GameFlag_TestFar(0x16f)) {
      if (!GameFlag_TestFar(0x171))
        saved->tutorialStep = state;
      else
        saved->tutorialStep = 14;
    } else {
      if (!GameFlag_TestFar(0x171))
        saved->tutorialStep = 27;
      else
        saved->tutorialStep = 28;
    }
  }
  Scheduler_EnableOverlayCallbacksWithFlags();
  UiWork_SetAltFlagAndClearTableFar(1);
  Link_DrawShiftedTilePairFar((void *)0x06002500);
  menu->ownerCount = Party_ListActiveOwnersFar(menu->owners);
  Resource_LoadPairedBlocksIfAvailable();
  ItemMenu_Init(0, 3, 0, 7);
  Menu_SetFirstObjectRowCoordinates(0);
  Palette_LightenBankHighlight(14);
  menu->helpWindow = UiWindow_CreateFar(13, 0, 17, 5, 2);
  menu->pairEntries[0] = 255;
  zero = 0;
  menu->selectedColumn[0] = zero;
  menu->selectedColumn[1] = zero;
  menu->selection[0] = 0;
  menu->selection[1] = 0;
  FourObjectMotion_InitializeBottomRow(menu->helpWindow, 0);
  DjinnMenu_RunScreen();
  FourObjectMotion_ClearSlotsAndScheduleAlt();
  Menu_ResetTwoResourceEntries();
  WaitFrames(1);
  ItemMenu_Close();
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  gMenuCtrlWork->suspended = 0;
  UiWindow_MarkVisibleTileAttributesFar();
  UiWork_SetAltFlagAndClearTableFar(0);
  Iwram_CopyWords((void *)0x06004000, saved->tiles, 0x2000);
  Iwram_CopyWords((void *)0x05000080, saved->palette, 128);
  WaitFrames(1);
  Scheduler_DisableOverlayCallbacksWithFlags();
  UiWindow_EraseBorderRectFar(0, 0, 30, 20);
  MemFree(menu->backdrop);
  Runtime_ReleaseHeapBlock(55);
  Event_ClearInvalidPackedValuesFar();
  gGameState.messageSpeed = oldMode;
  return 1;
}

// 移した Djinn を一覧の中で探し、その位置を選び直す
/* FAKEMATCH: a statement macro local to DjinnMenu_RunScreen, used at its two
 * places, not a habit of the game (S2): the do { } while (0) wrapper ends the
 * selection store's scheduling region. As plain statements the two shifts of
 * found * DJINN_LIST_ROWS swap (differ 2); as an inline function with its own
 * counter, one more register is saved. */
#define DjinnMenu_ReselectPairEntry(menu, i)                                   \
  do {                                                                         \
    s32 column;                                                                \
    s32 found;                                                                 \
    u8 entry;                                                                  \
                                                                               \
    column = (u16)((u32)(menu)->selection[1] % DJINN_LIST_ROWS);               \
    found = 0;                                                                 \
    entry = (menu)->pairEntries[0];                                            \
    for ((i) = 0; (i) < (menu)->backdrop->list.counts[column]; (i)++) {        \
      if (entry == (u8)(menu)->backdrop->list.ids[column][(i)]) {              \
        found = (i);                                                           \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    (menu)->selection[0] = column + found * DJINN_LIST_ROWS;                   \
  } while (0)

s32 DjinnMenu_RunScreen(void) {
  struct DjinnMenuWork *menu;
  struct RenderOutput *cursor;
  s32 result;
  s32 state;
  s32 ret;
  s32 done;
  s32 i;

  done = 0;
  result = 0;
  ret = 0;
  menu = gMenuWork;
  cursor = menu->cursor;
  cursor->active = ICON_HIDDEN;
  cursor->animStep = 0;
  Menu_OpenBackdropScreen();
  WaitFrames(1);
  state = 2;
  do {
    switch (state) {
    case 0:
      if (result < 0) {
        ret = -1;
        done = 1;
      }
      state = 2;
      break;
    case 1:
      break;
    case 2:
      Menu_SetFirstObjectRowCoordinates(0);
      FourObjectMotion_SetSlotPosition(1, 0, 200, 0);
      result = DjinnMenu_SelectDjinn(0);
      state = 15;
      if (result == 10)
        break;
      state = 0;
      if (result < 0)
        break;
      menu->selection[1] = menu->selectedColumn[0];
      state = 10;
      if (result == 7)
        break;
      state = 3;
      break;
    case 10:
      menu->owner = menu->owners[menu->selectedColumn[0]];
      menu->shownOwner[0] = menu->owners[menu->selectedColumn[0]];
      result = DjinnMenu_ShowOwnerStatus();
      if (result == -2)
        done = 1;
      state = 2;
      break;
    case 15:
      result = DjinnMenu_ShowHelp();
      if (result == -2)
        done = 1;
      state = 2;
      break;
    case 8:
      state = 0;
      if (menu->pairCount == 0)
        break;
      result = DjinnMenu_SelectDjinn(1);
      if (result == -2)
        done = 1;
      state = 4;
      if (result < 0)
        break;
      state = 9;
      break;
    case 3:
      DjinnMenu_DrawElementList(&menu->backdrop->list);
      Menu_SetFirstObjectRowCoordinates(-8);
      menu->owner = menu->owners[menu->selectedColumn[0]];
      menu->shownOwner[0] = menu->owners[menu->selectedColumn[0]];
      FourObjectMotion_SetSlotPosition(0, menu->selectedColumn[0] * 56 + 48, 54,
                                       0);
      result = DjinnMenu_SelectDjinn(1);
      for (i = 0; i < menu->ownerCount; i++)
        menu->ownerY[i] += 8;
      if (result == -2)
        done = 1;
      if (result < 0) {
        state = 2;
        break;
      }
      if (result == 3 || result == 4 || result == 8 || result == 9)
        menu->shownOwner[1] = menu->owners[menu->selectedColumn[1]];
      if (result < 0)
        state = 2;
      else if (result == 1)
        state = 5;
      else if (result == 2)
        state = 6;
      else if (result == 3) {
        menu->pending = 2;
        state = 7;
      } else if (result == 4) {
        menu->pending = 2;
        state = 9;
      } else if (result == 5)
        state = 11;
      else if (result == 6)
        state = 12;
      else if (result == 8) {
        menu->pending = 2;
        state = 13;
      } else if (result == 9) {
        menu->pending = 2;
        state = 14;
      }
      break;
    case 7:
      result = OwnerAction_RunCompareLoop(1);
      if (result == -2)
        done = 1;
      state = 3;
      if (result < 0)
        break;
    case 13:
      AudioCommand_PlayFar(SOUND_RECOVERY);
      result = Djinn_TransferFar(menu->shownOwner[0], menu->pairElement[0],
                                 menu->pairIndex[0], menu->shownOwner[1]);
      UnitCalcFar(menu->shownOwner[0]);
      UnitCalcFar(menu->shownOwner[1]);
      menu->cursor->active = ICON_HIDDEN;
      RenderOutput_ClearListFar(menu->listWindow);
      Menu_ComputeEntryValues(menu->backdrop);
      DjinnMenu_ReselectPairEntry(menu, i);
      menu->cursor->active = ICON_SHOWN;
      state = 0;
      break;
    case 6:
      result = OwnerAction_RunCompareLoop(2);
      if (result == -2)
        done = 1;
      state = 3;
      if (result < 0)
        break;
    case 12:
      AudioCommand_PlayFar(175);
      Djinn_DeactivateFar(menu->shownOwner[0], menu->pairElement[0],
                          menu->pairIndex[0]);
      result = DjinnRecovery_AddStandbyFar(menu->shownOwner[0], menu->pairElement[0],
                                 menu->pairIndex[0]);
      UnitCalcFar(menu->shownOwner[0]);
      menu->cursor->active = ICON_HIDDEN;
      RenderOutput_ClearListFar(menu->listWindow);
      menu->cursor->active = ICON_SHOWN;
      state = 2;
      break;
    case 9:
      result = OwnerAction_RunCompareLoop(0);
      if (result == -2)
        done = 1;
      state = 3;
      if (result < 0)
        break;
    case 14:
      AudioCommand_PlayFar(SOUND_RECOVERY);
      Djinn_TransferFar(menu->shownOwner[0], menu->pairElement[0],
                        menu->pairIndex[0], menu->shownOwner[1]);
      result = Djinn_TransferFar(menu->shownOwner[1], menu->pairElement[1],
                                 menu->pairIndex[1], menu->shownOwner[0]);
      UnitCalcFar(menu->shownOwner[0]);
      UnitCalcFar(menu->shownOwner[1]);
      Menu_ComputeEntryValues(menu->backdrop);
      DjinnMenu_ReselectPairEntry(menu, i);
      menu->cursor->active = ICON_SHOWN;
      state = 2;
      break;
    case 4:
      if (result == -1) {
        ret = result;
        state = 2;
      } else if (menu->pending & 1)
        state = 8;
      else if (menu->pending & 2)
        state = 7;
      break;
    case 5:
      result = OwnerAction_RunCompareLoop(3);
      if (result == -2)
        done = 1;
      state = 3;
      if (result < 0)
        break;
    case 11:
      AudioCommand_PlayFar(139);
      Djinn_ActivateFar(menu->shownOwner[0], menu->pairElement[0],
                        menu->pairIndex[0]);
      result = DjinnRecovery_RemoveFar(menu->shownOwner[0], menu->pairElement[0],
                                    menu->pairIndex[0]);
      UnitCalcFar(menu->shownOwner[0]);
      menu->cursor->active = ICON_HIDDEN;
      RenderOutput_ClearListFar(menu->listWindow);
      menu->cursor->active = ICON_SHOWN;
      state = 2;
      break;
    default:
      done = 1;
      break;
    }
  } while (done == 0);
  return ret;
}

//--------------------------------------------------------------------------
// adjust_palette_bank.c
// メニュー：BG パレットの明るさを変えた写しを作る
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// BG パレット 15・5・7 の 16 色の RGB に d を足し（0〜31 に収める）、
// それぞれ 1 つ前のパレット 14・4・6 に書く。7 は d でなく -12 を足す
//--------------------------------------------------------------------------
void Graphics_AdjustPaletteBank(s32 d) {
  s32 n;
  s32 bank;
  s32 col;
  s32 mask;
  s32 idx;

  bank = 15;
  n = 0;
  mask = 31;
  do {
    for (col = 0; col <= 15; col++) {
      u32 color;
      s32 b;
      s32 g;
      s32 r;
      s32 v;

      idx = bank * 16 + col;
      color = ((u16 *)0x05000000)[idx];
      b = (color >> 10) & mask;
      g = (color >> 5) & mask;
      r = color & mask;
      b += d;
      g += d;
      r += d;
      if (b > 31)
        b = 31;
      if (g > 31)
        g = 31;
      if (r > 31)
        r = 31;
      if (b < 0)
        b = 0;
      if (g < 0)
        g = 0;
      if (r < 0)
        r = 0;
      v = b << 10;
      v |= g << 5;
      v |= r;
      ((s16 *)0x04FFFFE0)[idx] = v; // 16 色前（1 つ前のパレット）
    }
    if (n == 0) {
      bank = 5;
    } else {
      bank = 7;
      d = -12;
    }
    n++;
  } while (n <= 2);
}

#define FRAME_TILE 150

#define FLAG_FIRST 0x8000
#define FLAG_SECOND 0x4000

s32 Menu_OpenBackdropScreen(void) {
  struct DjinnMenuWork *work = gMenuWork;
  struct DjinnMenuBackdrop *backdrop = work->backdrop;

  UiWindow_UpdateOrCreate(&work->listWindow, 0, 5, 30, 15, 2);
  WaitFrames(1);
  Iwram_CopyWords(backdrop->tiles, BG_CHAR_BLOCK(1), 0x2000);
  Iwram_CopyWords(backdrop->palette, (void *)&BG_PLTT_COLOR(4, 0), 128);
  Iwram_FillWords(BG_CHAR_BLOCK(1), 0x2000, 0x33333333);
  Iwram_FillWords((void *)&BG_PLTT_COLOR(4, 0), 128, 0x55555555);
  Graphics_ExpandVramTilesByColorTableFar(BG_CHAR_BLOCK(1) + 0x1000);
  Iwram_CopyWords(BG_CHAR_BLOCK(1) + FRAME_TILE * TILE_SIZE_4BPP,
                  gMenu_BackdropFrameTile, TILE_SIZE_4BPP);
  Dma_Set(Palette_GetBasicObjColors(), (void *)&BG_PLTT_COLOR(5, 0),
          0x80000010, REG_DMA3);
  BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
  Dma_Set((void *)&BG_PLTT_COLOR(15, 0), (void *)&BG_PLTT_COLOR(7, 0),
          0x80000010, REG_DMA3);
  Graphics_AdjustPaletteBank(8);
  BG_PLTT_COLOR(7, 4) = BG_PLTT_COLOR(15, 4);
  BG_PLTT_COLOR(6, 4) = BG_PLTT_COLOR(15, 4);
  return DjinnMenu_DrawElementList(&backdrop->list);
}

s32 OwnerAction_DiffSlots(struct OwnerActionSlot *a, struct OwnerActionSlot *b,
                          u16 *out, s32 *firstCount, s32 *secondCount) {
  s32 i;
  s32 j;
  s32 total;
  s32 first;
  s32 second;

  total = 0;
  first = 0;
  second = 0;
  for (i = 0; i < 32 && b[i].encodedAction != 0; i++) {
    out[total] = b[i].encodedAction & OWNER_ACTION_ID_MASK;
    total++;
    for (j = 0; j < 32; j++) {
      if (((b[i].encodedAction ^ a[j].encodedAction) & OWNER_ACTION_ID_MASK) ==
          0)
        break;
    }
    if (j == 32) {
      first++;
      out[total - 1] |= FLAG_FIRST;
    }
  }
  for (i = 0; i < 32 && a[i].encodedAction != 0; i++) {
    for (j = 0; j < 32; j++) {
      if (((a[i].encodedAction ^ b[j].encodedAction) & OWNER_ACTION_ID_MASK) ==
          0)
        break;
    }
    if (j == 32) {
      second++;
      out[total] = (a[i].encodedAction & OWNER_ACTION_ID_MASK) | FLAG_SECOND;
      total++;
    }
  }
  *firstCount = first;
  *secondCount = second;
  return total;
}

s32 Menu_ComputeEntryValues(void *table) {
  struct DjinnMenuWork *state;
  s32 i;
  u16 *p;
  u16 *src;
  s8 *dst;
  s16 v;
  s32 cnt;

  state = gMenuWork;
  i = 0;
  if (i < state->ownerCount) {
    dst = ((struct DjinnListTable *)table)->counts;
    src = state->owners;
    p = table;
    do {
      v = Djinn_ListOwnerEntries(p, *src, -1);
      cnt = state->ownerCount;
      i += 1;
      *dst = v;
      src += 1;
      dst += 1;
      p += DJINN_LIST_ROWS;
    } while (i < cnt);
  }
}

#define ELEMENT_TILE 0x5001
#define ENTRY list[i]

void DjinnMenu_DrawElementList(struct DjinnListTable *table) {
  struct DjinnMenuWork *menu;
  s32 i;
  s32 row;
  s32 element;
  struct UiWork *ui;
  s32 line;
  s32 usable;
  u16 *list;

  menu = WORK_SLOT(HEAP_SLOT_MENU);
  ui = WORK_SLOT(HEAP_SLOT_WINDOW);
  ui->menuBusy = 1;
  i = 0;
  if (menu->ownerCount != 0) {
    do {
      table->counts[i] =
          Djinn_ListOwnerEntries(table->ids[i], menu->owners[i], -1);
      i++;
    } while (i < menu->ownerCount);
  }
  RenderOutput_RedrawSavedRectFar(menu->listWindow);
  UiText_DrawCharacterAtOffsetFar(MsgUnleashEffect, menu->listWindow, 0, 80);
  for (row = 0; row < menu->ownerCount; row++) {
    list = table->ids[row];
    line = 0;
    for (element = 0; element < DJINN_ELEMENTS; element++) {
      for (i = 0; i < table->counts[row]; i++) {
        if (element != (ENTRY & DJINN_ENTRY_ELEMENT) >> 5)
          continue;
        if ((ENTRY & DJINN_ENTRY_IS_SET) == 0)
          UiWork_SetParamNibbleFar(2);
        usable = 0;
        if (Djinn_IsReadyFar((ENTRY & DJINN_ENTRY_OWNER) >> 8,
                                   (ENTRY & DJINN_ENTRY_ELEMENT) >> 5,
                                   ENTRY & DJINN_ENTRY_INDEX) ||
            Djinn_IsActiveFar((ENTRY & DJINN_ENTRY_OWNER) >> 8,
                              (ENTRY & DJINN_ENTRY_ELEMENT) >> 5,
                              ENTRY & DJINN_ENTRY_INDEX))
          usable = 1;
        if (!usable)
          UiWork_SetParamNibbleFar(4);
        UiWindow_SetTilemapEntryFar(menu->listWindow,
                                    ((ENTRY & DJINN_ENTRY_ELEMENT) >> 5) +
                                        ELEMENT_TILE,
                                    row * 7 + 1, line + 2, 0);
        UiText_DrawCharacterAtOffsetFar(
            MsgDjinnName +
                ((ENTRY & DJINN_ENTRY_ELEMENT) >> 5) * DJINN_PER_ELEMENT +
                (ENTRY & DJINN_ENTRY_INDEX),
            menu->listWindow, row * 56 + 16, line * 8 + 16);
        line++;
        UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
      }
    }
  }
  UiWindow_DrawDividerLineFar(menu->listWindow, 0, 10, 28, 10);
  ((struct UiWork *)WORK_SLOT(HEAP_SLOT_WINDOW))->dirty = 1;
  ui->menuBusy = 0;
}

s32 Menu_DrawAtWindowOffset(const struct UiWindow *win, s32 arg1, s32 arg2,
                            s32 arg3, s32 arg4, s32 arg5) {
  UiWindow_SetPaletteBitRectFar((win)->x + arg1 + 1, (win)->y + arg2 + 1, arg3,
                                arg4, arg5);
}

void UiWindow_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette) {
  struct UiRenderWork *work;
  u16 *p;
  s32 col;
  u32 tile;
  u32 masked;

  work = gWindowWork[0];
  palette <<= 12;
  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29)
    width = 30 - x;
  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29)
    height = 20 - y;

  if (width > 0 && height > 0) {
    do {
      p = &((u16 (*)[32])work->tilemap)[y][x];
      for (col = width; col != 0; col--) {
        tile = *p;
        if (((tile >> 12) & 15) == 15) {
          masked = tile & 0xffff0fff;
          *p = masked | palette;
        }
        p++;
      }
      UiRender_MarkRow(work, y);
      height--;
      y++;
    } while (height != 0);
  }
}

void UiWindow_ApplyRectAtObjectOrigin(void *obj, s32 x, s32 y, s32 width,
                                      s32 height, s32 palette) {
  UiWindow_SetRectPalette(((struct UiWindow *)obj)->x + x + 1,
                          ((struct UiWindow *)obj)->y + y + 1, width, height,
                          palette);
}

s32 DjinnMenu_ShowHelp(void) {
  struct UiWindow *winB;
  struct UiWindow *winA;
  struct DjinnMenuWork *menu;
  struct UiRenderWork *work;
  s32 result;
  s32 previous;
  s32 selection;
  struct UiWindow *list;
  struct UiChannelSlot *channel;
  s32 message;
  s32 cnt;
  s32 listMessage;

  menu = WORK_SLOT(HEAP_SLOT_MENU);
  work = WORK_SLOT(HEAP_SLOT_WINDOW);
  result = 0;
  previous = 0;
  selection = 0;
  RenderOutput_ClearListFar(menu->listWindow);
  WaitFrames(1);
  RenderOutput_PrepareForRedrawFar(menu->helpWindow);
  message = MsgDjinnInfoPrompt;
  UiText_DrawCharacterAtOffsetFar(message, menu->helpWindow, 0, 0);
  message++;
  UiText_DrawCharacterAtOffsetFar(message, menu->helpWindow, 0, 16);
  UiWindow_SetRectPalette(1, 1, 11, 3, 6);
  UiWindow_ApplyRectAtObjectOrigin((void *)menu->listWindow, 0, 0, 28, 10, 6);
  list = UiWindow_CreateFar(0, 9, 8, 10, 6);
  winB = UiWindow_CreateFar(8, 12, 22, 7, 2);
  winA = UiWindow_CreateFar(8, 9, 22, 3, 2);
  UiWindow_MarkVisibleTileAttributesFar();
  cnt = 0;
  listMessage = MsgDjinnInfoTopic;
  do {
    UiText_DrawCharacterAtOffsetFar(cnt + listMessage, list, 0, cnt * 8);
    cnt++;
  } while (cnt <= 6);
  do {
    RenderOutput_PrepareForRedrawFar(winA);
    UiText_DrawResourceFar(selection + MsgDjinnInfoTopic, winA, 0, 0);
    channel = (struct UiChannelSlot *)UiText_OpenEntryMessageFar(
        winB, selection + MsgDjinnInfoText);
    Menu_DrawAtWindowOffset(list, 0, previous, 6, 1, 15);
    Menu_DrawAtWindowOffset(list, 0, selection, 6, 1, 14);
    previous = selection;
    for (;;) {
      UiMenu_PositionCursor(-12, (list->y + selection) * 8 + 8);
      WaitFrames(1);
      if (gKeysRepeat & (KEY_RIGHT | KEY_DOWN)) {
        selection++;
        selection = Menu_GetModuloOfSum(selection, 7);
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        break;
      }
      if (gKeysRepeat & (KEY_LEFT | KEY_UP)) {
        selection--;
        selection = Menu_GetModuloOfSum(selection, 7);
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        break;
      } else if (gKeysPressed & KEY_START) {
        AudioCommand_PlayFar(SOUND_MENU_CANCEL);
        result = -2;
        break;
      } else if (gKeysPressed & (KEY_B | KEY_SELECT)) {
        AudioCommand_PlayFar(SOUND_MENU_CANCEL);
        result = -1;
        break;
      } else if (gKeysPressed & KEY_A) {
        if (UiWork_IsCompleteFar()) {
          selection++;
          selection = Menu_GetModuloOfSum(selection, 7);
          AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
          break;
        }
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      }
    }
    if (work->glyphResource != 99) {
      Resource_ResetEntry(work->glyphResource);
      work->glyphResource = 99;
    }
    ((struct UiRenderWork *)gWindowWork[0])->pageArrowShown = 0;
    RenderOutput_PrepareForRedrawFar(winB);
    {
      struct UiWindow *window = channel->work;

      window->frame = 0;
      window->duration = 0;
      window->state = 0;
    }
    channel->work = NULL;
  } while (result == 0);
  ((struct UiRenderWork *)gWindowWork[0])->menuBusy = 1;
  RenderOutput_ClearListFar(winA);
  RenderOutput_ClearListFar(winB);
  WaitFrames(1);
  UiWork_FinalizeFar(winA, 1);
  UiWork_FinalizeFar(list, 1);
  UiWork_FinalizeFar(winB, 1);
  UiWindow_MarkVisibleTileAttributesFar();
  if (result == -2) {
    RenderOutput_PrepareForRedrawFar(menu->helpWindow);
    RenderOutput_PrepareForRedrawFar(menu->listWindow);
    RenderOutput_PrepareForRedrawFar(menu->statusWindow);
    ((struct UiRenderWork *)gWindowWork[0])->menuBusy = 0;
  }
  Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
  return result;
}

#define DJINN_ENTRY_SET (-0x8000)

#define DJINN_OWNER(v) (((v) & 0xf00) >> 8)
#define DJINN_ELEMENT(v) (((v) & 0xe0) >> 5)
#define DJINN_NUMBER(v) ((v) & 0x1f)
#if EDITION_INTERNATIONAL
#define CURSOR_WIDTH 6
#else
#define CURSOR_WIDTH 5
#endif

s32 DjinnMenu_SelectDjinn(s32 mode) {
  s8 buf[8];
  u8 balanced[16];
  struct DjinnMenuWork *state = gMenuWork;
  struct DjinnMenuBackdrop *lists = state->backdrop;
  struct BattleUnit *owner;
  struct UiWindow *window;
  s32 redraw = 1;
  u32 refresh;
  s32 i;
  s32 j;
  s32 selection;
  s32 x = (u16)(state->selection[mode] % 10);
  s32 y = (u16)(state->selection[mode] / 10);
  s32 savedY = 0;
  u32 djinn = 0;
  s32 groupMode = 0;
  s32 setAll = 0;
  s32 result;
  s32 message;
  u32 pressed;
  s32 count;
  u32 repeat;
  struct UiWindow *work;
  s32 step;
  s8 *status;

  selection = -1;
  status = buf;
  for (i = 7; i >= 0; i--)
    status[i] = 0;
  if (mode == 0) {
    DjinnMenu_DrawElementList(&lists->list);
    for (i = 0; i < state->ownerCount; i++) {
      if (lists->list.counts[i] == 0)
        status[i] = 4;
    }
    savedY = y;
    for (i = 0; i < state->ownerCount; i++) {
      if (status[x] == 4) {
        x++;
        x = Menu_GetModuloOfSum(x, state->ownerCount);
      }
    }
  } else {
    Djinn_MarkBalancedEntries((s8 *)balanced, state->selectedColumn[0]);
    for (i = 0; i < state->ownerCount; i++) {
      if (i == state->selectedColumn[0]) {
        buf[i] = 7;
      } else if (balanced[i] != 0) {
        buf[i] = 0;
      } else {
        buf[i] = 3;
        if (lists->list.counts[i] == 0)
          status[i] = buf[i] | 4;
      }
    }
  }
  if (mode == 1) {
    s32 fromX;
    s32 fromY;
    u16 sourceCursor;
    s32 n;
    sourceCursor = state->selection[0];
    fromX = (u16)((u32)sourceCursor % (u32)10);
    fromY = (u16)((u32)sourceCursor / (u32)10);
    Menu_DrawAtWindowOffset(state->listWindow, fromX * 7 + 1, fromY + 2,
                            CURSOR_WIDTH, 1, 14);
    UiWindow_ApplyRectAtObjectOrigin(state->listWindow, fromX * 7 + 1, 2,
                                     CURSOR_WIDTH, 7, 6);
    for (n = 0; n < state->ownerCount; n++) {
      if (n == state->selectedColumn[0]) {
        if (state->pairEntries[0] & 0x8000)
          message = MsgDjinnStandbyAction;
        else
          message = MsgDjinnSetAction;
      } else if (buf[n] & 2) {
        message = MsgDjinnGiveAction;
      } else {
        message = MsgDjinnTradeAction;
      }
      UiText_DrawCharacterAtOffsetFar(message, state->listWindow, n * 56 + 8,
                                      8);
    }
  }
  RenderOutput_RedrawSavedRectFar(state->helpWindow);
  state->cursor->active = ICON_SHOWN;
  for (;;) {
    if (redraw != 0) {
      refresh = redraw;
      redraw = 0;
      selection = -1;
      if (!(buf[x] & 1))
        selection = y;
      window = state->statusWindow;
      owner = UnitGetFar(state->owners[x]);
      RenderOutput_RedrawSavedRectFar(window);
      UiText_DrawStringAtOffsetFar(owner->name, window, 0, 0);
      UiText_DrawCharacterAtOffsetFar(owner->classIndex + MsgClassName, window,
                                      0, 8);
      UiText_DrawStringAtOffsetFar(gDjinnMenu_TextLevel, window, 48, 0);
      UiText_DrawNumberInWindowFar(owner->level, 2, window, 72, 0);
      if (mode == 0)
        UiText_DrawCharacterAtOffsetFar(MsgDjinnCharacterStatus, window, 0, 16);
      if (selection != -1)
        djinn = lists->list.ids[x][selection];
      RenderOutput_RedrawSavedRectFar(state->helpWindow);
      if (mode == 1) {
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) ||                      \
    defined(TBS_EDITION_IT)
        UiWindow_SetTilemapEntryFar(
            state->helpWindow, DJINN_ELEMENT(state->pairEntries[0]) + 0x5001, 0,
            0, 0);
        UiWork_PushValueSlotFar(state->shownOwner[0], 1);
        UiWork_PushValueSlotFar(DJINN_ELEMENT(state->pairEntries[0]) * 20 +
                                    DJINN_NUMBER(state->pairEntries[0]) + 300,
                                4);
        UiText_DrawCharacterAtOffsetFar(MsgDjinnOwnerLabel, state->helpWindow,
                                        8, 0);
#else
        UiWork_PushValueSlotFar(state->shownOwner[0], 1);
        UiText_DrawCharacterAtOffsetFar(MsgDjinnOwnerLabel, state->helpWindow,
                                        0, 0);
        UiWork_PushValueSlotFar(DJINN_ELEMENT(state->pairEntries[0]) * 20 +
                                    DJINN_NUMBER(state->pairEntries[0]) + 300,
                                4);
        UiWindow_SetTilemapEntryFar(
            state->helpWindow, DJINN_ELEMENT(state->pairEntries[0]) + 0x5001, 6,
            0, 0);
        UiText_DrawCharacterAtOffsetFar(MsgDjinnOwnerLabel + 1,
                                        state->helpWindow, 56, 0);
#endif
        UiText_DrawCharacterAtOffsetFar(MsgDjinnOwnerLabel + 2,
                                        state->helpWindow, 0, 8);
      }
      if (selection == -1) {
        FourObjectMotion_SetSlotPosition(mode, 0, 200, 0);
      } else if (mode == 0) {
        if (groupMode) {
          if (setAll == 0) {
            UiText_DrawCharacterAtOffsetFar(MsgDjinnSetAll, state->helpWindow,
                                            0, 0);
          } else {
            UiText_DrawCharacterAtOffsetFar(MsgDjinnAllStandby,
                                            state->helpWindow, 0, 0);
          }
          if (Djinn_IsReadyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                                     DJINN_NUMBER(djinn)) ||
              Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                                DJINN_NUMBER(djinn))) {
            if (Djinn_IsReadyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                                       DJINN_NUMBER(djinn)))
              FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
            else
              FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
            FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 0);
          } else {
            UiText_DrawCharacterAtOffsetFar(MsgDjinnRecovering,
                                            state->helpWindow, 0, 16);
            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
            FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 1);
          }
          if (refresh & 2)
            FourObjectMotion_SetSlotPhase(mode, 0);
        } else {
          UiText_DrawCharacterAtOffsetFar(MsgDjinnChoose, state->helpWindow, 0,
                                          0);
          if (Djinn_IsReadyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                                     DJINN_NUMBER(djinn)) ||
              Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                                DJINN_NUMBER(djinn))) {
            if (Djinn_IsReadyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                                       DJINN_NUMBER(djinn))) {
              UiText_DrawCharacterAtOffsetFar(MsgDjinnChoose + 3,
                                              state->helpWindow, 0, 16);
              FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
            } else {
              UiText_DrawCharacterAtOffsetFar(MsgDjinnChoose + 2,
                                              state->helpWindow, 0, 16);
              FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
            }
            FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 0);
          } else {
            UiText_DrawCharacterAtOffsetFar(MsgDjinnChoose + 4,
                                            state->helpWindow, 0, 16);
            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
            FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 1);
          }
          if (refresh & 2)
            FourObjectMotion_SetSlotPhase(mode, 0);
        }
      } else {
        if (Djinn_IsReadyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                                   DJINN_NUMBER(djinn)) ||
            Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                              DJINN_NUMBER(djinn))) {
          if (Djinn_IsReadyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                                     DJINN_NUMBER(djinn)))
            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
          else
            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
          FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 54, 0);
        } else {
          FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
          FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 54, 1);
        }
        if (refresh & 2)
          FourObjectMotion_SetSlotPhase(mode, 0);
      }
      RenderOutput_ClearListFar(state->listWindow);
      if (selection != -1) {
        UiText_DrawCharacterAtOffsetFar(MsgUnleashEffect, state->listWindow, 0,
                                        80);
#if EDITION_INTERNATIONAL
        UiWindow_ClearInteriorTilesFar(state->listWindow, 0, 96, 224, 104);
        UiText_DrawCharacterAtOffsetFar(
            DJINN_ELEMENT(djinn) * 20 + DJINN_NUMBER(djinn) + MsgDjinnEffect,
            state->listWindow, 0, 96);
#else
        UiText_DrawResourceFar(DJINN_ELEMENT(djinn) * 20 + DJINN_NUMBER(djinn) +
                                   MsgDjinnEffect,
                               state->listWindow, 0, 96);
#endif
      }
      if (!(buf[x] & 1))
        Menu_DrawAtWindowOffset(state->listWindow, x * 7 + 1, y + 2,
                                CURSOR_WIDTH, 1, 14);
      ((struct UiWork *)gWindowWork[0])->dirty = 1;
    }
    if (buf[x] & 1)
      UiMenu_PositionCursor(x * 56 - 8, 52);
    else
      UiMenu_PositionCursor(x * 56 - 8, y * 8 + 60);
    WaitFrames(1);
    if (!(gKeysHeld & KEY_R) || (gKeysPressedLatch & 0x100)) {
      if (groupMode)
        redraw = 1;
      groupMode = 0;
      setAll = 0;
    }
    repeat = gKeysRepeat;
    pressed = gKeysPressed;
    step = lists->tutorialStep;
    if (step != 0) {
      pressed = 0;
      lists->tutorialTimer++;
      repeat = 0;
      switch (step) {
      case 28:
        while (!(gKeysPressed & KEY_A)) {
          UiMenu_PositionCursor(150, 26);
          WaitFrames(1);
        }
        pressed = 2;
        repeat = 2;
        break;
      case 27:
        if (lists->tutorialTimer == 60) {
          work = UiText_OpenMessageWindowFar(MsgDjinnTutorialRecovery, 9, 9, 1);
          gGameState.messageSpeed = 1;
          while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
          UiWork_FinalizeFar(work, 1);
          DjinnMenu_DrawElementList(&lists->list);
          WaitFrames(1);
          work = UiText_OpenMessageWindowFar(MsgDjinnTutorialSetAgain, 9, 9, 1);
          gGameState.messageSpeed = 1;
          while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
          UiWork_FinalizeFar(work, 1);
          DjinnMenu_DrawElementList(&lists->list);
          lists->tutorialTimer = 0;
          BattlePlacement_UpdateTimedEntriesFar();
          BattlePlacement_UpdateTimedEntriesFar();
          BattlePlacement_UpdateTimedEntriesFar();
          Djinn_DeactivateFar(0, 0, 0);
          DjinnRecovery_AddStandbyFar(0, 0, 0);
          UnitCalcFar(0);
          repeat = 2;
          pressed = 2;
        }
        break;
      case 1:
        if (lists->tutorialTimer == 60) {
          work = UiText_OpenMessageWindowFar(MsgDjinnTutorialGive, 9, 9, 1);
          gGameState.messageSpeed = 1;
          while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
          UiWork_FinalizeFar(work, 1);
          DjinnMenu_DrawElementList(&lists->list);
          lists->tutorialTimer = 0;
          lists->tutorialStep = 2;
        }
        break;
      case 2:
      case 4:
      case 8:
      case 9:
      case 22:
        if (lists->tutorialTimer == 90) {
          repeat = 1;
          pressed = 1;
          lists->tutorialTimer = 0;
          lists->tutorialStep++;
        }
        break;
      case 3:
        if (lists->tutorialTimer == 90) {
          repeat = 16;
          pressed = 16;
          lists->tutorialTimer = 0;
          lists->tutorialStep = 4;
        }
        break;
      case 6:
      case 7:
        if (lists->tutorialTimer == 60) {
          work = UiText_OpenMessageWindowFar(MsgDjinnTutorialSetAfterGive, 9, 9,
                                             1);
          gGameState.messageSpeed = 1;
          while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
          UiWork_FinalizeFar(work, 1);
          DjinnMenu_DrawElementList(&lists->list);
          lists->tutorialTimer = 0;
          lists->tutorialStep = 8;
        }
        break;
      case 13:
        if (lists->tutorialTimer == 40) {
          lists->tutorialTimer = 0;
          repeat = 2;
          pressed = 2;
          lists->tutorialStep++;
        }
        break;
      case 14:
      case 16:
      case 17:
        if (lists->tutorialTimer == 40) {
          lists->tutorialTimer = 0;
          lists->tutorialStep++;
        }
        break;
      case 15:
        if (lists->tutorialTimer == 60) {
          gGameState.messageSpeed = 1;
          work = UiText_OpenMessageWindowFar(MsgDjinnTutorialPowers, 9, 9, 1);
          UiMenu_SlideCursor(2, 146);
          while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
          while (!(gKeysPressed & KEY_A)) {
            UiMenu_PositionCursor(2, 146);
            WaitFrames(1);
          }
          UiWork_FinalizeFar(work, 1);
          DjinnMenu_DrawElementList(&lists->list);
          WaitFrames(1);
          work = UiText_OpenMessageWindowFar(MsgDjinnTutorialUnleash, 9, 9, 1);
          while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
          while (!(gKeysPressed & KEY_A)) {
            UiMenu_PositionCursor(2, 146);
            WaitFrames(1);
          }
          UiWork_FinalizeFar(work, 1);
          DjinnMenu_DrawElementList(&lists->list);
          WaitFrames(1);
          work =
              UiText_OpenMessageWindowFar(MsgDjinnTutorialDemonstrate, 9, 9, 1);
          while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
          while (!(gKeysPressed & KEY_A)) {
            UiMenu_PositionCursor(2, 146);
            WaitFrames(1);
          }
          UiWork_FinalizeFar(work, 1);
          DjinnMenu_DrawElementList(&lists->list);
          WaitFrames(1);
          lists->tutorialTimer = 0;
          lists->tutorialStep = 16;
        }
        break;
      case 18:
      case 20:
        if (lists->tutorialTimer == 90) {
          repeat = 1;
          pressed = 1;
          lists->tutorialTimer = 0;
          lists->tutorialStep = 21;
        }
        break;
      case 21:
        if (lists->tutorialTimer == 90) {
          lists->tutorialTimer = 0;
          repeat = 32;
          pressed = 32;
          lists->tutorialStep = 22;
        }
        break;
      case 24:
        if (lists->tutorialTimer == 60) {
          lists->tutorialTimer = 0;
          lists->tutorialStep = 25;
          repeat = 2;
          pressed = 2;
        }
        break;
      }
    }
    if (mode == 0 && (pressed & 0x100)) {
      if (selection == -1) {
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
        continue;
      }
      i = 0;
      if (Djinn_IsReadyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                                 DJINN_NUMBER(djinn)) ||
          Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                            DJINN_NUMBER(djinn)))
        i = 1;
      groupMode = 1;
      gKeysPressedLatch = 0;
      if (i == 0) {
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
        RenderOutput_ClearListFar(state->listWindow);
        UiWindow_ClearInteriorTilesFar(state->listWindow, 0, 80, 216, 104);
        UiText_DrawResourceFar(MsgDjinnNowRecovering, state->listWindow, 0, 96);
        redraw = 1;
        continue;
      }
      if (djinn & 0x8000) {
        AudioCommand_PlayFar(175);
        Djinn_DeactivateFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                            DJINN_NUMBER(djinn));
        DjinnRecovery_AddStandbyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                          DJINN_NUMBER(djinn));
      } else {
        AudioCommand_PlayFar(139);
        Djinn_ActivateFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                          DJINN_NUMBER(djinn));
        DjinnRecovery_RemoveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                             DJINN_NUMBER(djinn));
      }
      UnitCalcFar(DJINN_OWNER(djinn));
      DjinnMenu_DrawElementList(&lists->list);
      redraw = 1;
    }
    if (mode == 0 && (pressed & 0x200)) {
      result = 7;
      AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
      break;
    }
    if ((pressed & 1) || (mode == 1 && (pressed & 0x100))) {
      i = 1;
      if (!(buf[x] & 1)) {
        if (selection == -1) {
          AudioCommand_PlayFar(SOUND_MENU_ERROR);
          continue;
        }
        i = 0;
        if (Djinn_IsReadyFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                                   DJINN_NUMBER(djinn)) ||
            Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn),
                              DJINN_NUMBER(djinn)))
          i = 1;
      }
      if (i == 0) {
        AudioCommand_PlayFar(SOUND_MENU_ERROR);
        RenderOutput_ClearListFar(state->listWindow);
        UiWindow_ClearInteriorTilesFar(state->listWindow, 0, 80, 216, 104);
        UiText_DrawResourceFar(MsgDjinnNowRecovering, state->listWindow, 0, 96);
        continue;
      }
      if (mode == 1) {
        if (buf[x] & 1) {
          if (x == state->selectedColumn[0]) {
            if (state->pairEntries[0] & 0x8000)
              result = 2;
            else
              result = 1;
          } else {
            result = 3;
          }
        } else {
          result = 4;
        }
      } else {
        result = 1;
      }
      AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
      break;
    }
    if (pressed & 8) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -2;
      break;
    }
    if (pressed & 2) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -1;
      break;
    }
    if (mode == 0 && (pressed & 4)) {
      if (groupMode) {
        setAll ^= 1;
        if (setAll)
          AudioCommand_PlayFar(139);
        else
          AudioCommand_PlayFar(175);
        for (i = 0; i < state->ownerCount; i++) {
          for (j = 0; j < lists->list.counts[i]; j++) {
            u32 entry = lists->list.ids[i][j];
            s32 usable;
            usable = 0;
            if (Djinn_IsReadyFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry),
                                       DJINN_NUMBER(entry)) ||
                Djinn_IsActiveFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry),
                                  DJINN_NUMBER(entry)))
              usable = 1;
            if (usable) {
              if (setAll) {
                if (!(entry & 0x8000)) {
                  Djinn_ActivateFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry),
                                    DJINN_NUMBER(entry));
                  DjinnRecovery_RemoveFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry),
                                       DJINN_NUMBER(entry));
                  UnitCalcFar(DJINN_OWNER(entry));
                }
              } else if (entry & 0x8000) {
                Djinn_DeactivateFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry),
                                    DJINN_NUMBER(entry));
                DjinnRecovery_AddStandbyFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry),
                                  DJINN_NUMBER(entry));
                UnitCalcFar(DJINN_OWNER(entry));
              }
            }
          }
        }
        DjinnMenu_DrawElementList(&lists->list);
        redraw = 1;
      } else {
        result = 10;
        Menu_DrawAtWindowOffset(state->listWindow, x * 7 + 1, y + 2,
                                CURSOR_WIDTH, 1, 15);
        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
        break;
      }
    }
    if (repeat & 0x40) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      if (buf[x] & 4)
        continue;
      Menu_DrawAtWindowOffset(state->listWindow, x * 7 + 1, y + 2, CURSOR_WIDTH,
                              1, 15);
      if (buf[x] & 4)
        continue;
      if (buf[x] & 1) {
        buf[x] &= ~1;
        y = 0;
      } else if (y == 0 && (buf[x] & 2)) {
        buf[x] |= 1;
        redraw = 2;
        continue;
      }
      y--;
      count = lists->list.counts[x];
      if (count == 0)
        count = 1;
      y = Menu_GetModuloOfSum(y, count);
      savedY = y;
      redraw = 2;
      continue;
    }
    if (repeat & 0x80) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      if (buf[x] & 4)
        continue;
      Menu_DrawAtWindowOffset(state->listWindow, x * 7 + 1, y + 2, CURSOR_WIDTH,
                              1, 15);
      y++;
      count = lists->list.counts[x];
      if (count == 0)
        count = 1;
      y = Menu_GetModuloOfSum(y, count);
      if ((buf[x] & 1) && !(buf[x] & 4)) {
        buf[x] &= ~1;
        y = 0;
      } else if (y == 0 && (buf[x] & 2)) {
        buf[x] |= 1;
      }
      savedY = y;
      redraw = 2;
      continue;
    }
    if (repeat & 0x20) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      if (!(buf[x] & 4))
        Menu_DrawAtWindowOffset(state->listWindow, x * 7 + 1, y + 2,
                                CURSOR_WIDTH, 1, 15);
      x--;
      x = Menu_GetModuloOfSum(x, state->ownerCount);
      if (mode == 0) {
        for (i = 0; i < state->ownerCount; i++) {
          if (buf[x] == 4) {
            x--;
            x = Menu_GetModuloOfSum(x, state->ownerCount);
          }
        }
      }
      y = savedY;
      count = lists->list.counts[x];
      if (count == 0)
        count = 1;
      y = Menu_GetModuloOfSum(y, count);
      redraw = 2;
      continue;
    }
    if (repeat & 0x10) {
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      if (!(buf[x] & 4))
        Menu_DrawAtWindowOffset(state->listWindow, x * 7 + 1, y + 2,
                                CURSOR_WIDTH, 1, 15);
      x++;
      x = Menu_GetModuloOfSum(x, state->ownerCount);
      if (mode == 0) {
        for (i = 0; i < state->ownerCount; i++) {
          if (buf[x] == 4) {
            x++;
            x = Menu_GetModuloOfSum(x, state->ownerCount);
          }
        }
      }
      y = savedY;
      count = lists->list.counts[x];
      if (count == 0)
        count = 1;
      y = Menu_GetModuloOfSum(y, count);
      redraw = 2;
    }
  }
  state->selectedColumn[mode] = x;
  if (selection != -1) {
    u32 entry;
    state->pairEntries[mode] = entry = lists->list.ids[x][selection];
    state->pairIndex[mode] = entry % 32;
    state->pairElement[mode] = DJINN_ELEMENT(entry);
    state->pairOwner[mode] = DJINN_OWNER(entry);
    // Plain, entry dies at the owner's AND, so reload ties the result to entry
    // instead of the mask.
    /* FAKEMATCH: the barrier keeps entry live through owner extraction and
     * passes result through it, so the owner mask keeps its native register
     * (ands r3, r2); plain differs 8 (ands r2, r3), an input-only barrier 2 (a
     * sched2 tie swaps the final index shift and table-base copy), the fields
     * stored in other orders 13-1106, entry as s32 or u16 1186 and 1165. */
    asm("" : "+r"(result) : "r"(entry));
  }
  state->selection[mode] = x + y * 10;
  return result;
}

s32 Djinn_ListOwnerEntries(u16 *out, s32 owner, s32 element) {
  struct BattleUnit *state = UnitGetFar(owner);
  s32 count = 0;
  s32 row;
  s32 bit;
  s32 entry;

  if (element == -1) {
    for (row = 0; row < 4; row++) {
      for (bit = 0; bit < 20; bit++) {
        if (state->djinnActive[row] & (1 << bit)) {
          entry = (row << 5) | bit | DJINN_ENTRY_SET;
          entry |= owner << 8;
          out[count++] = entry;
        } else if (state->djinnAvailable[row] & (1 << bit)) {
          out[count++] = (row << 5) | bit | (owner << 8);
        }
      }
    }
  } else {
    for (bit = 0; bit < 20; bit++) {
      if (state->djinnActive[element] & (1 << bit))
        out[count++] = (element << 5) | bit | DJINN_ENTRY_SET;
      else if (state->djinnAvailable[element] & (1 << bit))
        out[count++] = (element << 5) | bit;
    }
  }
  return count;
}

#if EDITION_INTERNATIONAL
#define ALT_PARAM 2
#else
#define ALT_PARAM 4
#endif

s32 Menu_RunPairedEntryAction(s32 mode, s32 param) {
  s32 sp14;
  struct DjinnMenuWork *menu;

  menu = gMenuWork;
  if (mode == 0) {
    sp14 = mode;
    DjinnMenu_DrawStatPreview(menu->secondWindow, 0, 0, menu->pairOwner[1], 1,
                              mode, 2, param, 1);
    DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->pairOwner[0], mode, 1,
                              2, param, mode);
  } else {
    DjinnMenu_DrawStatPreview(menu->secondWindow, 0, 0, menu->shownOwner[1], 1,
                              0, ALT_PARAM, param, 1);
    DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->shownOwner[0], 0, 0, 1,
                              param, 0);
  }
  return 1;
}

#if EDITION_INTERNATIONAL
#define PREVIEW_DIGITS 3
#define PREVIEW_CURRENT_X 48
#define PREVIEW_MAX_X 80
#define PREVIEW_SLASH_X 72
#define PREVIEW_LUCK_X 56
#define PREVIEW_ARROW_X iconX
#define PREVIEW_PP_X 88
#else
#define PREVIEW_DIGITS 4
#define PREVIEW_CURRENT_X 32
#define PREVIEW_MAX_X 72
#define PREVIEW_SLASH_X 64
#define PREVIEW_LUCK_X 48
#define PREVIEW_ARROW_X (x * 8 + 64)
#define PREVIEW_PP_X 56
#endif
static __inline__ void Unit_Copy(struct BattleUnit *dst,
                                 struct BattleUnit *src) {
  Iwram_CopyWords(dst, src, sizeof(struct BattleUnit));
}

s32 DjinnMenu_DrawStatPreview(struct UiWindow *window, s32 x, s32 y, s32 owner,
                              s32 give, s32 take, s32 mode, s32 page,
                              s32 side) {
  struct BattleUnit *state;
  struct BattleUnit *saved;
  s32 giveElement;
  s32 giveIndex;
  s32 giveSet;
  s32 takeElement;
  s32 takeIndex;
  s32 takeSet;
  s32 lost;
  s32 gained;
  u16 actions[48];
  struct DjinnMenuWork *menu;

  state = UnitGetFar(owner);
  menu = gMenuWork;
  giveElement = menu->pairElement[give];
  giveIndex = menu->pairIndex[give];
  giveSet = menu->pairEntries[give] & 0x8000;
  takeElement = menu->pairElement[take];
  takeIndex = menu->pairIndex[take];
  takeSet = menu->pairEntries[take] & 0x8000;
  saved = MemAlloc(sizeof(struct BattleUnit));
  Unit_Copy(saved, state);

  if (page == 0) {
    if (mode == 3) {
      UiText_DrawNumberInWindowFar(state->maxHp, PREVIEW_DIGITS, window,
                                   x * 8 + PREVIEW_MAX_X, y * 8 + 56);
      UiText_DrawNumberInWindowFar(state->maxPp, PREVIEW_DIGITS, window,
                                   x * 8 + PREVIEW_MAX_X, y * 8 + 64);
      UiText_DrawNumberInWindowFar(state->hp, PREVIEW_DIGITS, window,
                                   x * 8 + PREVIEW_CURRENT_X, y * 8 + 56);
      UiText_DrawNumberInWindowFar(state->pp, PREVIEW_DIGITS, window,
                                   x * 8 + PREVIEW_CURRENT_X, y * 8 + 64);
      UiText_DrawStringAtOffsetFar(gDjinnMenu_TextSlash, window,
                                   x * 8 + PREVIEW_SLASH_X, y * 8 + 56);
      UiText_DrawStringAtOffsetFar(gDjinnMenu_TextSlash, window,
                                   x * 8 + PREVIEW_SLASH_X, y * 8 + 64);
    } else {
      UiText_DrawNumberInWindowFar(state->hp, PREVIEW_DIGITS, window,
                                   x * 8 + PREVIEW_CURRENT_X, y * 8 + 56);
      UiText_DrawNumberInWindowFar(state->pp, PREVIEW_DIGITS, window,
                                   x * 8 + PREVIEW_CURRENT_X, y * 8 + 64);
    }
    UiText_DrawNumberInWindowFar(state->atk, PREVIEW_DIGITS, window,
                                 x * 8 + PREVIEW_CURRENT_X, y * 8 + 72);
    UiText_DrawNumberInWindowFar(state->def, PREVIEW_DIGITS, window,
                                 x * 8 + PREVIEW_CURRENT_X, y * 8 + 80);
    UiText_DrawNumberInWindowFar(state->agi, PREVIEW_DIGITS, window,
                                 x * 8 + PREVIEW_CURRENT_X, y * 8 + 88);
    UiText_DrawNumberInWindowFar(state->luck, 2, window, x * 8 + PREVIEW_LUCK_X,
                                 y * 8 + 96);
  }

  switch (mode) {
  case 0:
    Djinn_AddToOwnerFar(owner, takeElement, takeIndex & 31);
    Djinn_ActivateFar(owner, takeElement, takeIndex & 31);
    break;
  case 1:
    giveIndex &= 31;
    Djinn_DeactivateFar(owner, giveElement, giveIndex);
    break;
  case 2:
    if (giveSet) {
      giveIndex &= 31;
      Djinn_DeactivateFar(owner, giveElement, giveIndex);
    }
    Djinn_AddToOwnerFar(owner, takeElement, takeIndex & 31);
    if (takeSet)
      Djinn_ActivateFar(owner, takeElement, takeIndex & 31);
    break;
  case 4:
    Djinn_AddToOwnerFar(owner, takeElement, takeIndex & 31);
    if (takeSet)
      Djinn_ActivateFar(owner, takeElement, takeIndex & 31);
    break;
  }

  UnitCalcFar(owner);
  state = UnitGetFar(owner);

  if (page == 0) {
    s32 label;

    UiText_DrawStringAtOffsetFar(state->name, window, x * 8 + 40, y * 8);
    UiText_DrawStringAtOffsetFar(gDjinnMenu_TextLevel, window, x * 8 + 40,
                                 y * 8 + 16);
    UiText_DrawNumberInWindowFar(state->level, 2, window, x * 8 + 88,
                                 y * 8 + 16);
    label = MsgPreviewStatLabel;
    UiText_DrawCharacterAtOffsetFar(label, window, x * 8, y * 8 + 56);
    UiText_DrawCharacterAtOffsetFar(label + 1, window, x * 8, y * 8 + 64);
    UiText_DrawCharacterAtOffsetFar(label + 2, window, x * 8, y * 8 + 72);
    UiText_DrawCharacterAtOffsetFar(label + 3, window, x * 8, y * 8 + 80);
    UiText_DrawCharacterAtOffsetFar(label + 4, window, x * 8, y * 8 + 88);
    UiText_DrawCharacterAtOffsetFar(label + 5, window, x * 8, y * 8 + 96);
    UiText_DrawCharacterAtOffsetFar(MsgClassName + saved->classIndex, window,
                                    x * 8, y * 8 + 32);
  }

  {
    s32 i = 0;

    if (page == 0) {
      s32 column;
#if EDITION_INTERNATIONAL
      s32 iconX;
#endif
      s32 px;

      if (saved->classIndex != state->classIndex) {
        px = x * 8;
        UiText_DrawCharacterAtOffsetFar(MsgClassName + state->classIndex,
                                        window, px, y * 8 + 48);
        UiWindow_SetTilemapEntryFar(window, 0xf296, x + 2, 5, 0);
      }
      column = x;
      if (saved->classIndex != state->classIndex)
        column += 5;
      for (i = 0; i < 4; i++) {
        UiWindow_SetTilemapEntryFar(window, 0x5001 + i, column + i * 2, y + 5,
                                    0);
        UiWindow_SetTilemapEntryFar(window,
                                    state->djinnActiveCounts[i] + 0xf030,
                                    column + i * 2 + 1, y + 5, 0);
      }
#if EDITION_INTERNATIONAL
      iconX = x * 8 + 70;
#endif
      if (state->hp != saved->hp) {
        UiText_DrawNumberInWindowFar(state->hp, 4, window, x * 8 + 72,
                                     y * 8 + 56);
        if (state->hp > saved->hp)
          UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 56,
                                           0);
        else
          UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 56,
                                           1);
      }
      if (state->pp != saved->pp) {
        UiText_DrawNumberInWindowFar(state->pp, 4, window, x * 8 + 72,
                                     y * 8 + 64);
        if (state->pp > saved->pp)
          UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 64,
                                           0);
        else
          UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 64,
                                           1);
      }
      if (state->atk != saved->atk) {
        UiText_DrawNumberInWindowFar(state->atk, 4, window, x * 8 + 72,
                                     y * 8 + 72);
        if (state->atk > saved->atk)
          UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 72,
                                           0);
        else
          UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 72,
                                           1);
      }
      if (state->def != saved->def) {
        UiText_DrawNumberInWindowFar(state->def, 4, window, x * 8 + 72,
                                     y * 8 + 80);
        if (state->def > saved->def)
          UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 80,
                                           0);
        else
          UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 80,
                                           1);
      }
      if (state->agi != saved->agi) {
        UiText_DrawNumberInWindowFar(state->agi, 4, window, x * 8 + 72,
                                     y * 8 + 88);
        if (state->agi > saved->agi)
          UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 88,
                                           0);
        else
          UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 88,
                                           1);
      }
      if (state->luck != saved->luck) {
        UiText_DrawNumberInWindowFar(state->luck, 2, window, x * 8 + 88,
                                     y * 8 + 96);
        if (state->luck > saved->luck)
          UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 96,
                                           0);
        else
          UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 96,
                                           1);
      }
    }
  }

  if (page > 0) {
    s32 rows;
    s32 first;
    s8 count;
    s8 row;
    s8 line;

    rows = 6 - (mode != 3);
    first = rows * (page - 1);
    count = OwnerAction_DiffSlots(saved->actionSlots, state->actionSlots,
                                  actions, &gained, &lost);
    line = 0;
    for (row = 0; first < count && row < rows; row++, first++) {
      UiIcon_CreateWithLoadedResource(window, x * 8, (y + line * 2) * 8 + 4,
                                      actions[first] & OWNER_ACTION_ID_MASK);
      if (actions[first] & 0x8000)
        UiWork_SetParamNibbleFar(4);
      else if (actions[first] & 0x4000)
        UiWork_SetParamNibbleFar(2);
      else
        UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
      UiText_DrawCharacterAtOffsetFar(
          MsgAbilityName + (actions[first] & OWNER_ACTION_ID_MASK), window,
          x * 8 + 16, (y + line * 2) * 8 + 8);
      UiText_DrawNumberAtOffsetFar(BattleActGet(actions[first])->ppCost, 2,
                                   window, x * 8 + 88, (y + line * 2) * 8 + 8);
      line++;
    }
    UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
    UiText_DrawCharacterAtOffsetFar(MsgPsynergyPp, window, x * 8 + PREVIEW_PP_X,
                                    y * 8);
    if (mode != 3) {
      s32 lines;

      lines = 0;
      if (gained) {
        UiWork_SetParamNibbleFar(4);
        UiText_DrawCharacterAtOffsetFar(MsgPsynergyGained, window, x * 8,
                                        y * 8 + 88);
        lines = 1;
      }
      if (lost) {
        UiWork_SetParamNibbleFar(2);
        UiText_DrawCharacterAtOffsetFar(MsgPsynergyLost, window, x * 8,
                                        (y + lines) * 8 + 88);
        lines++;
      }
      if (lines == 0)
        UiText_DrawCharacterAtOffsetFar(MsgPsynergyUnchanged, window, x * 8,
                                        y * 8 + 88);
      UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
      UiWindow_DrawDividerLineFar(window, 0, 11, 13, 11);
    }
    ((struct UiWork *)gWindowWork[0])->dirty = 1;
  }

  if (page == 0)
    SideObject_CreateFar(owner, 0, side, window, 0, 0);
  Unit_Copy(state, saved);
  MemFree(saved);
  return 1;
}

void FourObjectMotion_InitializeTopRow(void *unused0, s32 unused1) {
  struct FourObjectMotionState *state = gMenuWork;
  s32 index;

  for (index = 0; index < 4; index++) {
    struct AnimationObject *object = state->objects[index];

    if (object != NULL) {
      ResourceObject_ReleaseFar(object);
      state->objects[index] = NULL;
    }
  }

  for (index = 0; index < 4; index++) {
    struct AnimationObject *object =
        ResourceObject_CreateFar(gFourObjectMotion_ResourceIds[index]);

    if (object != NULL) {
      AnimationObjects_SelectAnimationFar(object, 2);
    }
    state->objects[index] = object;
    state->positionsX[index] = 0x10;
    state->positionsY[index] = 0x20;
  }
  Scheduler_AddOrUpdateCallback(FourObjectMotion_UpdateAllPositions, 0xc80);
}

// Releases the four objects and removes their scheduler callback.
#define DEFINE_FOUR_OBJECT_RELEASE(name, update)                               \
  void name(void) {                                                            \
    struct FourObjectMotionState *state = gMenuWork;                           \
    s32 index = 0;                                                             \
                                                                               \
    do {                                                                       \
      struct AnimationObject *object = state->objects[index];                  \
                                                                               \
      if (object != NULL) {                                                    \
        ResourceObject_ReleaseFar(object);                                     \
        state->objects[index] = NULL;                                          \
      }                                                                        \
      index++;                                                                 \
    } while (index < 4);                                                       \
    Scheduler_RemoveCallback(update);                                          \
  }

DEFINE_FOUR_OBJECT_RELEASE(FourObjectMotion_ClearSlotsAndSchedule,
                           FourObjectMotion_UpdateAllPositions)

#define FIXED_ONE 0x10000u
#define VERTICAL_ORIGIN 0x01e20000u

void FourObjectMotion_UpdateAllPositions(void) {
  struct FourObjectMotionState *state = gMenuWork;
  s32 index;
  u32 motion[2];
  u32 request[4];

  index = 0;
  while (1) {
    u32 object = (u32)state->objects[index];

    if (object != 0) {
      u32 vertical =
          VERTICAL_ORIGIN - ((s32)state->verticalOrigins[index] << 16);
      ((struct FieldSprite *)object)->priority = 0;
      motion[0] = FIXED_ONE;
      motion[1] = FIXED_ONE;
      request[0] = (s32)state->positionsX[index] << 16;
      request[1] = vertical;
      request[2] = ((s32)state->positionsY[index] << 16) + vertical;
      request[3] = 0;
      Object_ApplyProjectedPlacementFar(object, request, motion, 0x4000);
    }
    index += 1;
    if (index > 3)
      break;
  }
}

void FourObjectMotion_UpdateBottomRow(void) {
  struct FourObjectMotionState *work = gMenuWork;
  s32 i;
  u32 motion[2];
  u32 request[4];

  Link_DrawShiftedTilePairFar((void *)0x06002500);
  i = 0;
  do {
    u32 obj = (u32)work->objects[i];

    if (obj != 0) {
      u32 y = (241u << 17) - ((u32)(s32)work->verticalOrigins[i] << 16);
      s32 phase;
      s32 limit;
      /* Ordinary s32/s16 locals, a direct field test, an earlier limit
       * initializer and an inline sign/limit helper all differ 81 in
       * EN (119 native, 118 built), changing the outer high-register
       * allocation as well as this read. Plain locals take r3,
       * local-alloc's first free register (REG_ALLOC_ORDER 3, 2, 1, 0),
       * which costs limit its r3 preference. */
      /* FAKEMATCH: pins the row's second read to r2, as native; plain locals
       * take r3 and reshuffle the high registers (differ 81), reading py before
       * request[3] = 0 puts it in r2 but emits the ldrsh before the zero store
       * (4 in all six editions), and with py plain the loop index as u8, s8 or
       * u16 differs 130 to 140, as s16 82. */
      register s32 py asm("r2");

      ((struct FieldSprite *)obj)->priority = 0;
      phase = work->phases[i];
      if (phase < 0) {
        motion[0] = -phase;
        motion[1] = -phase;
      } else {
        motion[0] = phase + (s32)(0x10000 - phase) / 3;
        motion[1] = motion[0];
        work->phases[i] = motion[0];
      }
      request[0] = (u32)(s32)work->positionsX[i] << 16;
      request[1] = y;
      request[2] = ((u32)(s32)work->positionsY[i] << 16) + y;
      request[3] = 0;
      py = work->positionsY[i];
      limit = 0x8000;
      if (py >= 0)
        limit = 0x4000;
      Object_ApplyProjectedPlacementFar(obj, request, motion, limit);
    }
    i++;
  } while (i <= 3);
}

void FourObjectMotion_InitializeBottomRow(s32 unused0, s32 unused1) {
  struct FourObjectMotionState *state = gMenuWork;
  s32 index;

  for (index = 0; index < 4; index++) {
    struct AnimationObject *object = state->objects[index];

    if (object != NULL) {
      ResourceObject_ReleaseFar(object);
      state->objects[index] = NULL;
    }
  }
  for (index = 0; index < 4; index++) {
    struct AnimationObject *object =
        ResourceObject_CreateFar(gFourObjectMotion_ResourceIds[index]);

    if (object != NULL)
      AnimationObjects_SelectAnimationFar(object, 2);
    state->objects[index] = object;
    state->phases[index] = 0x10000;
    state->positionsX[index] = 0x10;
    state->positionsY[index] = 0xc8;
  }
  Scheduler_AddOrUpdateCallback(FourObjectMotion_UpdateBottomRow, 0xc80);
}

s32 FourObjectMotion_SetSlotPosition(s32 index, s32 x, s32 y, s32 negative) {
  struct FourObjectMotionState *state = gMenuWork;

  if (state->objects[index] != NULL) {
    state->positionsX[index] = x;
    state->positionsY[index] = negative != 0 ? y | (s32)0xffff8000 : y;
    return 0x23c + index * 2;
  }
  return index;
}

s32 FourObjectMotion_SetSlotPhase(s32 index, s32 phase) {
  ((struct FourObjectMotionState *)gMenuWork)->phases[index] = phase;
}

s32 FourObjectMotion_ReplaceSlot(s32 index, s32 kind, s32 value) {
  struct FourObjectMotionState *state = gMenuWork;

  if (state->objects[index] != NULL) {
    ResourceObject_ReleaseFar(state->objects[index]);
    state->objects[index] = NULL;
  }
  {
    struct AnimationObject *object =
        ResourceObject_CreateFar(gFourObjectMotion_ResourceIds[kind]);

    if (object != NULL)
      AnimationObjects_SelectAnimationFar(object, value);
    state->objects[index] = object;
  }
  return 1;
}

DEFINE_FOUR_OBJECT_RELEASE(FourObjectMotion_ClearSlotsAndScheduleAlt,
                           FourObjectMotion_UpdateBottomRow)

void Menu_EnableAllItemObjects(void) {
  struct InventoryMenuState *state = gMenuWork;
  s32 index;

  for (index = 0; index < state->partyCount; ++index) {
    AnimationObjects_SelectAnimationFar(state->ownerObjects[index], 1);
  }
}

s32 OwnerAction_RunCompareLoop(u32 mode) {
  struct DjinnMenuWork *state;
  struct DjinnMenuBackdrop *scene;
  struct UiRenderWork *render;
  struct UiWindow *list;
  s32 count[2];
  s32 cursor[2];
  s32 anim;
  u32 dirty;
  s32 frame;
  s32 page;
  s32 entry;
  s32 swap;
  s32 step;
  s32 row;
  s32 result;
  s32 zero;
  s32 i;
  s32 j;

  state = gMenuWork;
  anim = 0;
  result = 0;
  dirty = 1;
  frame = 0;
  cursor[0] = 0;
  page = 2;
  state->cursor->active = ICON_HIDDEN;
  cursor[1] = 0;

  for (j = 3; j >= 0; j--) {
    state->ownerY[j] = 0xc8;
  }
  state->ownerX[state->selectedColumn[1]] = 0x78;
  state->ownerY[state->selectedColumn[1]] = 0x20;
  state->ownerX[state->selectedColumn[0]] = 0x10;
  state->ownerY[state->selectedColumn[0]] = 0x20;

  RenderOutput_ClearListFar(state->listWindow);
  WaitFrames(1);
  UiWindow_CloseIfOpen(&state->helpWindow, 1);
  UiWindow_UpdateOrCreate(&state->helpWindow, 0, 0, 30, 5, 2);

  count[0] = 1;
  count[1] = 1;

  if (mode <= 1) {
    u16 *diffWork;
    struct BattleUnit *unitCopy;
    struct BattleUnit *unit;
    s32 id;
    s32 firstElement;
    s32 firstIndex;
    s32 secondElement;
    s32 secondIndex;
    s32 firstFlag;
    s32 secondFlag;
    s32 outA;
    s32 outB;
    s32 size;

    diffWork = Runtime_BumpAllocateAlternatePool(96);
    size = sizeof(*unitCopy);
    unitCopy = Runtime_BumpAllocateAlternatePool(size);
    secondElement = 0;
    secondIndex = 0;
    id = state->shownOwner[0];
    firstElement = state->pairElement[0];
    firstIndex = state->pairIndex[0] & 31;
    firstFlag = (u16)(state->pairEntries[0] & 0x8000);

    unit = UnitGetFar(id);
    Iwram_CopyWords(unitCopy, unit, size);
    Djinn_DeactivateFar(id, firstElement, firstIndex);
    if (mode == 0) {
      secondElement = state->pairElement[1];
      secondIndex = state->pairIndex[1] & 31;
      secondFlag = (u16)(state->pairEntries[1] & 0x8000);
      Djinn_AddToOwnerFar(id, secondElement, secondIndex);
      if (secondFlag != 0) {
        Djinn_ActivateFar(id, secondElement, secondIndex);
      }
    }
    UnitCalcFar(id);
    count[0] = OwnerAction_DiffSlots(unitCopy->actionSlots, unit->actionSlots,
                                     diffWork, &outB, &outA);
    Iwram_CopyWords(unit, unitCopy, size);

    id = state->shownOwner[1];
    unit = UnitGetFar(id);
    Iwram_CopyWords(unitCopy, unit, size);
    if (mode == 0) {
      Djinn_DeactivateFar(id, secondElement, secondIndex);
    }
    Djinn_AddToOwnerFar(id, firstElement, firstIndex);
    if (firstFlag != 0) {
      Djinn_ActivateFar(id, firstElement, firstIndex);
    }
    UnitCalcFar(id);
    count[1] = OwnerAction_DiffSlots(unitCopy->actionSlots, unit->actionSlots,
                                     diffWork, &outB, &outA);
    Iwram_CopyWords(unit, unitCopy, size);
    MemFree(unitCopy);
    MemFree(diffWork);
  } else if (mode - 2 <= 1) {
    u16 *diffWork;
    struct BattleUnit *unitCopy;
    struct BattleUnit *unit;
    s32 firstId;
    s32 firstElement;
    s32 firstIndex;
    s32 applySecond;
    s32 outA;
    s32 outB;

    diffWork = Runtime_BumpAllocateAlternatePool(96);
    unitCopy = Runtime_BumpAllocateAlternatePool(sizeof(*unitCopy));
    firstId = state->shownOwner[0];
    firstElement = state->pairElement[0];
    firstIndex = state->pairIndex[0] & 31;
    applySecond = (mode == 3);

    unit = UnitGetFar(firstId);
    Iwram_CopyWords(unitCopy, unit, sizeof(*unitCopy));
    Djinn_DeactivateFar(firstId, firstElement, firstIndex);
    if (applySecond != 0) {
      Djinn_ActivateFar(firstId, firstElement, firstIndex);
    }
    UnitCalcFar(firstId);
    count[0] = OwnerAction_DiffSlots(unitCopy->actionSlots, unit->actionSlots,
                                     diffWork, &outB, &outA);
    Iwram_CopyWords(unit, unitCopy, sizeof(*unitCopy));
    count[1] = count[0];
    page = 0;
    MemFree(unitCopy);
    MemFree(diffWork);
  }

  count[0] = ((count[0] - 1) / 5) + 1;
  if (count[0] == 0) {
    count[0] = 1;
  }
  count[1] = ((count[1] - 1) / 5) + 1;
  if (count[1] == 0) {
    count[1] = 1;
  }

  UiWindow_UpdateOrCreate(&state->window, 0, 5, 15, 15, 2);
  UiWindow_UpdateOrCreate(&state->secondWindow, 15, 5, 15, 15, 2);
  RenderOutput_RedrawSavedRectFar(state->helpWindow);

  if (mode == 2) {
    UiText_DrawCharacterAtOffsetFar(MsgDjinnCompareStandby, state->helpWindow,
                                    96, 0);
    UiText_DrawCharacterAtOffsetFar(MsgDjinnCompareConfirm, state->helpWindow,
                                    96, 16);
  } else if (mode == 3) {
    UiText_DrawCharacterAtOffsetFar(MsgDjinnCompareSet, state->helpWindow, 96,
                                    0);
    UiText_DrawCharacterAtOffsetFar(MsgDjinnCompareConfirm, state->helpWindow,
                                    96, 16);
  } else if (mode == 0) {
    UiText_DrawCharacterAtOffsetFar(MsgDjinnCompareTrade, state->helpWindow,
                                    128, 0);
    UiText_DrawCharacterAtOffsetFar(MsgDjinnCompareConfirm, state->helpWindow,
                                    128, 8);
  } else {
    UiText_DrawCharacterAtOffsetFar(MsgDjinnCompareGive, state->helpWindow, 128,
                                    0);
    UiText_DrawCharacterAtOffsetFar(MsgDjinnCompareConfirm, state->helpWindow,
                                    128, 8);
  }

  row = (mode == 1);
  if ((state->pairEntries[0] & 0x8000) == 0) {
    UiWork_SetParamNibbleFar(2);
  }
  UiText_DrawCharacterAtOffsetFar(((state->pairEntries[0] & 0xe0) >> 5) * 20 +
                                      (state->pairEntries[0] & 31) +
                                      MsgDjinnName,
                                  state->helpWindow, 48, row * 8);
  UiWindow_SetTilemapEntryFar(state->helpWindow,
                              ((state->pairEntries[0] & 0xe0) >> 5) + 0x5001, 5,
                              row, zero = 0);
  UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);

  if (mode == 0) {
    if ((state->pairEntries[1] & 0x8000) == 0) {
      UiWork_SetParamNibbleFar(2);
    }
    UiText_DrawCharacterAtOffsetFar(((state->pairEntries[1] & 0xe0) >> 5) * 20 +
                                        (state->pairEntries[1] & 31) +
                                        MsgDjinnName,
                                    state->helpWindow, 48, 16);
    UiWindow_SetTilemapEntryFar(state->helpWindow,
                                ((state->pairEntries[1] & 0xe0) >> 5) + 0x5001,
                                5, 2, mode);
    UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
  } else if (mode - 2 <= 1) {
    if ((state->pairEntries[0] & 0x8000) != 0) {
      UiWork_SetParamNibbleFar(2);
    }
    UiText_DrawCharacterAtOffsetFar(((state->pairEntries[0] & 0xe0) >> 5) * 20 +
                                        (state->pairEntries[0] & 31) +
                                        MsgDjinnName,
                                    state->helpWindow, 48, 16);
    UiWindow_SetTilemapEntryFar(state->helpWindow,
                                ((state->pairEntries[0] & 0xe0) >> 5) + 0x5001,
                                5, 2, zero);
    UiWork_SetParamNibbleFar(TEXT_COLOR_NORMAL);
    UiWindow_SetTilemapEntryFar(state->helpWindow, 0xf296, 7, 1, zero);
  }

  for (;;) {
    s32 inputEdge;
    s32 inputHeld;

    render = (struct UiRenderWork *)gWindowWork[0];
    scene = state->backdrop;
    inputEdge = gKeysPressed;
    inputHeld = gKeysRepeat;

    if (scene->tutorialStep != 0) {
      s16 *saved;
      s32 size;

      size = 0x200;
      saved = Runtime_BumpAllocateAlternatePool(size);
      inputEdge = 0;
      inputHeld = 0;
      scene->tutorialTimer += 1;

      switch (scene->tutorialStep) {
      case 5:
      case 12:
      case 23:
        if (scene->tutorialTimer == 100) {
          inputHeld = 1;
          inputEdge = 1;
          scene->tutorialTimer = 0;
          scene->tutorialStep += 1;
        }
        break;
      case 10:
        if (scene->tutorialTimer == 60) {
          struct UiWindow *work;

          Iwram_CopyWords(saved, render, size);
          work =
              UiText_OpenMessageWindowFar(MsgDjinnTutorialSetChanges, 8, 0, 1);
          /* FAKEMATCH: the one-pass block keeps the window copy before the
           * cursor store, as native; plain, it is scheduled after the store
           * (differ 8; all four plain 26). With all four plain: an inline
           * save-and-open helper at both cases 84, the save and open as a
           * statement macro 8, a cursor-store statement macro 12 at each of the
           * two plain stores, so no layer. */
          do {
            state->cursor->active = ICON_SHOWN;
          } while (0);
          UiMenu_SlideCursor(2, 96);
          gGameState.messageSpeed = 1;
          while (UiWork_IsCompleteFar() == 0) {
            WaitFrames(1);
          }
          UiWork_FinalizeFar(work, 1);
          Iwram_CopyWords(render, saved, size);
          UiWindow_MarkVisibleTileAttributesFar();
          render->dirty = 1;
          dirty = 1;
          scene->tutorialTimer = 0;
          scene->tutorialStep = 11;
          /* FAKEMATCH: one-pass block keeps the step store before the cursor
           * store, as native; plain it follows the cursor load (differ 4; all
           * four plain 26). Save/open as a macro: UiWork_FinalizeFar to step
           * matches, but case 11 has an extra WaitFrames there, so none serves
           * both; around dirty, timer and step 10, step alone or from
           * render->dirty 4, restore (UiWork_FinalizeFar to render->dirty) 22.
           */
          do {
            state->cursor->active = ICON_HIDDEN;
          } while (0);
        }
        break;
      case 11:
        if (scene->tutorialTimer == 60) {
          struct UiWindow *work;

          Iwram_CopyWords(saved, render, size);
          work = UiText_OpenMessageWindowFar(MsgDjinnTutorialClassPsynergy, 8,
                                             0, 1);
          /* FAKEMATCH: the one-pass block keeps the window copy before the
           * cursor store, as native; plain, it is scheduled after the store
           * (differ 10; all four plain 26). With all four plain: an inline
           * save-and-open helper at both cases 84, the save and open as a
           * statement macro 8, a cursor-store statement macro 12 at each of the
           * two plain stores, so no layer. */
          do {
            state->cursor->active = ICON_SHOWN;
          } while (0);
          UiMenu_SlideCursor(106, 56);
          gGameState.messageSpeed = 1;
          while (UiWork_IsCompleteFar() == 0) {
            WaitFrames(1);
          }
          UiWork_FinalizeFar(work, 1);
          Iwram_CopyWords(render, saved, size);
          UiWindow_MarkVisibleTileAttributesFar();
          render->dirty = 1;
          WaitFrames(1);
          dirty = 1;
          scene->tutorialTimer = 0;
          scene->tutorialStep = 12;
          /* FAKEMATCH: one-pass block keeps the step store before the cursor
           * store, as native; plain it follows the cursor load (differ 4; all
           * four plain 26). Save/open as a macro: UiWork_FinalizeFar to step
           * matches, but case 11 has an extra WaitFrames there, so none serves
           * both; around dirty, timer and step 10, step alone or from
           * render->dirty 4, restore (UiWork_FinalizeFar to render->dirty) 22.
           */
          do {
            state->cursor->active = ICON_HIDDEN;
          } while (0);
        }
        break;
      default:
        break;
      }
      MemFree(saved);
    }

    if (dirty != 0) {
      swap = 1;
      if (mode <= 1) {
        UiWindow_ClearInteriorTilesFar(state->helpWindow, 128, 16, 224, 24);
        if (page == 1) {
          UiText_DrawCharacterAtOffsetFar(MsgDjinnStatusHelp, state->helpWindow,
                                          128, 16);
        } else {
          UiWork_PushValueSlotFar(state->shownOwner[page != 2], 1);
          UiText_DrawCharacterAtOffsetFar(MsgDjinnPsyHelp, state->helpWindow,
                                          128, 16);
        }
      }
      render->menuBusy = 1;
      RenderOutput_PrepareForRedrawFar(state->window);
      RenderOutput_PrepareForRedrawFar(state->secondWindow);
      if ((dirty >> 1) != 0) {
        WaitFrames(1);
      }

      if (mode == 3) {
        entry = 0;
        swap = 0;
      } else if (mode == 2) {
        entry = 1;
        swap = 0;
      } else {
        entry = 2;
      }

      switch (page) {
      default:
        Menu_RunPairedEntryAction(mode, 0);
        break;
      case 1:
        if (mode == 1) {
          entry = 4;
        }
        DjinnMenu_DrawStatPreview(state->window, 0, 0, state->shownOwner[1],
                                  page, 0, entry, 0, 0);
        DjinnMenu_DrawStatPreview(state->secondWindow, 0, 0,
                                  state->shownOwner[1], page, 0, entry,
                                  cursor[1] + 1, 0);
        break;
      case 0:
        if (mode == 1) {
          entry = 1;
          swap = 0;
        }
        DjinnMenu_DrawStatPreview(state->window, 0, 0, state->pairOwner[0],
                                  page, swap, entry, page, 1);
        DjinnMenu_DrawStatPreview(state->secondWindow, 0, 0,
                                  state->pairOwner[0], page, swap, entry,
                                  cursor[0] + 1, 1);
        break;
      }
      render->menuBusy = 0;
    }

    if (page <= 1) {
      list = state->secondWindow;
      if (count[page] > 1) {
        s32 top;

        for (i = 0; i < count[page]; i++) {
          s32 tile;

          tile = 0xf031 + i;
          if (i > 9) {
            tile = 0xf030;
          }
          if (i == cursor[page]) {
            tile -= 0x1000;
          }
          UiWindow_SetTilemapEntryFar(list, tile,
                                      list->width - count[page] + i - 2, -1, 0);
        }
        top = -1;
        UiWindow_SetTilemapEntryFar(list, 0xf128, list->width - count[page] - 3,
                                    top, 0);
        UiWindow_SetTilemapEntryFar(list, 0xf129, list->width - 2, top, 0);
        UiRender_MarkRow(render, list->y);
      }
    }

    frame++;
    step = (frame % 60) - 5;
    if (step < 0) {
      step = 0;
    }
    if (step > 29) {
      step = 29;
    }
    FourObjectMotion_SetSlotPhase(0, 0xffff4000);
    FourObjectMotion_SetSlotPhase(1, 0xffff4000);

    if (mode <= 1) {
      s32 rise;
      s32 slide;

      rise = -Iwram_MulQ16(16, Trig_Sin((step % 30) * 0x444));
      if (rise < -6) {
        rise = -6;
      }
      if (rise > 12) {
        rise = 12;
      }
      slide = (step % 35) * 2;
      FourObjectMotion_SetSlotPosition(0, slide + 34, rise + 20, 0);
      if (mode == 0) {
        FourObjectMotion_SetSlotPosition(1, 99 - slide, 36 - rise, 0);
      }
    } else {
      FourObjectMotion_SetSlotPosition(0, 32, 30, 0);
    }

    if (dirty != 0) {
      dirty = 0;
      anim = Menu_GetModuloOfSum(anim, 2);
    }
    UiMenu_PositionCursor(anim * 56 + 80, 16);

    if ((frame & 3) == 0) {
      void *tile;
      u32 fill;

      tile = (void *)0x060052c0;
      fill = 0x44444444;
      if ((frame & 4) != 0) {
        Iwram_CopyWords(tile, gMenu_BackdropFrameTile, 32);
      } else {
        Iwram_FillWords(tile, 32, fill);
      }
    }

    if ((inputEdge & 1) != 0) {
      if (anim == 0) {
        result = 1;
        break;
      }
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -1;
      break;
    }
    if ((inputEdge & 8) != 0) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -2;
      break;
    } else if ((inputEdge & 2) != 0) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -1;
      break;
    } else if ((inputEdge & 0x100) != 0) {
      if (mode <= 1) {
        page = ((page + 1) % 3);
        dirty = 2;
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      }
    } else if ((inputHeld & 32) != 0) {
      if (page <= 1) {
        cursor[page]--;
        cursor[page] = Menu_GetModuloOfSum(cursor[page], count[page]);
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        dirty = 1;
      }
    } else if ((inputHeld & 16) != 0) {
      if (page <= 1) {
        AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
        Input_ResetKeyRepeat();
        cursor[page]++;
        dirty = 1;
        cursor[page] = Menu_GetModuloOfSum(cursor[page], count[page]);
      }
    }
    WaitFrames(1);
  }

  FourObjectMotion_SetSlotPhase(0, 0);
  FourObjectMotion_SetSlotPhase(1, 0);
  Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);

  ((struct UiRenderWork *)gWindowWork[0])->menuBusy = 1;
  UiWindow_CloseIfOpen(&state->helpWindow, 1);
  WaitFrames(1);
  UiWindow_UpdateOrCreate(&state->helpWindow, 13, 0, 17, 5, 2);
  UiWindow_CloseIfOpen((struct UiWindow **)&state->window, 1);
  UiWindow_CloseIfOpen(&state->secondWindow, 1);
  RenderOutput_RedrawSavedRectFar(state->listWindow);
  RenderOutput_RedrawSavedRectFar((struct UiWindow *)state->unknownWindow028);
  RenderOutput_RedrawSavedRectFar(state->statusWindow);
  ((struct UiRenderWork *)gWindowWork[0])->menuBusy = 0;
  WaitFrames(1);
  return result;
}

s32 DjinnMenu_ShowOwnerStatus(void) {
  s32 cursor[2];
  s32 pages[2];
  s32 gained;
  s32 lost;
  s32 phase;
  s32 pending;
  s32 frame;
  struct UiWindow **left;
  struct UiWindow **right;
  struct UiRenderWork *render;
  u32 keys;
  u32 repeat;
  struct DjinnMenuWork *menu;
  struct BattleUnit *owner;
  struct UiWindow *window;
  u16 *actions;
  void *saved;
  s32 result;
  s32 tile;
  s32 i;
  s32 side;
  s32 *page;
  s32 blink;

  menu = gMenuWork;
  phase = 0;
  pending = 1;
  result = 0;
  frame = 0;
  menu->cursor->active = ICON_HIDDEN;
  cursor[1] = cursor[0] = result;
  for (side = 3; side >= 0; side--)
    menu->ownerY[side] = 200;
  RenderOutput_ClearListFar(menu->listWindow);
  WaitFrames(1);
  page = pages;
  page[1] = page[0] = 1;
  actions = Runtime_BumpAllocateAlternatePool(96);
  saved = Runtime_BumpAllocateAlternatePool(332);
  owner = UnitGetFar(menu->shownOwner[0]);
  page[1] = page[0] = OwnerAction_DiffSlots(
      owner->actionSlots, owner->actionSlots, actions, &gained, &lost);
  MemFree(saved);
  MemFree(actions);
  page[0] = (page[0] - 1) / 6 + 1;
  if (page[0] == 0)
    page[0] = 1;
  page[1] = (page[1] - 1) / 6 + 1;
  if (page[1] == 0)
    page[1] = 1;
  left = (struct UiWindow **)&menu->window;
  UiWindow_UpdateOrCreate(left, 0, 5, 15, 15, 2);
  right = &menu->secondWindow;
  UiWindow_UpdateOrCreate(right, 15, 5, 15, 15, 2);
  RenderOutput_RedrawSavedRectFar(menu->helpWindow);
  RenderOutput_RedrawSavedRectFar(menu->statusWindow);
  UiText_DrawCharacterAtOffsetFar(MsgCurrentStatus, menu->statusWindow, 0, 0);
  UiText_DrawCharacterAtOffsetFar(MsgCurrentStatus + 2, menu->statusWindow, 0,
                                  16);
  side = 0;
  while (1) {
    render = (struct UiRenderWork *)gWindowWork[0];
    keys = gKeysPressed;
    repeat = gKeysRepeat;
    if (pending) {
      render->menuBusy = 1;
      RenderOutput_PrepareForRedrawFar(menu->window);
      RenderOutput_PrepareForRedrawFar(menu->secondWindow);
      DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->pairOwner[0], 0, 0, 3,
                                0, 1);
      DjinnMenu_DrawStatPreview(menu->secondWindow, 0, 0, menu->pairOwner[0], 0,
                                0, 3, cursor[0] + 1, 1);
      render->menuBusy = 0;
    }
    if (page[side] > 1) {
      window = menu->secondWindow;
      for (i = 0; i < page[side]; i++) {
        tile = 0xf031 + i;
        if (i > 9)
          tile = 0xf030;
        if (i == cursor[side])
          tile -= 0x1000;
        UiWindow_SetTilemapEntryFar(window, tile,
                                    window->width - page[side] + i - 2, -1, 0);
      }
      UiWindow_SetTilemapEntryFar(window, 0xf128,
                                  window->width - page[side] - 3, -1, 0);
      UiWindow_SetTilemapEntryFar(window, 0xf129, window->width - 2, -1, 0);
      UiRender_MarkRow(render, window->y);
    }
    frame++;
    blink = frame % 60 - 5;
    if (blink < 0)
      blink = 0;
    if (blink > 29)
      blink = 29;
    FourObjectMotion_SetSlotPosition(0, 32, 200, 0);
    if (pending) {
      pending = 0;
      phase = Menu_GetModuloOfSum(phase, 2);
    }
    if ((frame & 3) == 0) {
      if (frame & 4)
        Iwram_CopyWords((void *)0x060052c0, gMenu_BackdropFrameTile, 32);
      else
        Iwram_FillWords((void *)0x060052c0, 32, 0x44444444);
    }
    if (keys & KEY_START) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -2;
      break;
    }
    if (keys & (KEY_A | KEY_B | KEY_R | KEY_L)) {
      AudioCommand_PlayFar(SOUND_MENU_CANCEL);
      result = -1;
      break;
    }
    if (repeat & KEY_LEFT) {
      cursor[side]--;
      cursor[side] = Menu_GetModuloOfSum(cursor[side], page[side]);
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      Input_ResetKeyRepeat();
      pending = 1;
    } else if (repeat & KEY_RIGHT) {
      cursor[side]++;
      AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
      Input_ResetKeyRepeat();
      pending = 1;
      cursor[side] = Menu_GetModuloOfSum(cursor[side], page[side]);
    }
    WaitFrames(1);
  }
  Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
  ((struct UiRenderWork *)gWindowWork[0])->menuBusy = 1;
  UiWindow_CloseIfOpen(&menu->helpWindow, 1);
  WaitFrames(1);
  UiWindow_UpdateOrCreate(&menu->helpWindow, 13, 0, 17, 5, 2);
  UiWindow_CloseIfOpen(left, 1);
  UiWindow_CloseIfOpen(right, 1);
  RenderOutput_RedrawSavedRectFar(menu->listWindow);
  RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->unknownWindow028);
  RenderOutput_RedrawSavedRectFar(menu->statusWindow);
  ((struct UiRenderWork *)gWindowWork[0])->menuBusy = 0;
  WaitFrames(1);
  return result;
}

s32 Djinn_MarkBalancedEntries(s8 *table, s32 self) {
  s32 sp0;
  s32 cnt;
  s32 i;
  s8 *p;
  struct DjinnMenuWork *state;

  state = gMenuWork;
  cnt = 0;
  i = 0;
  if (cnt < state->ownerCount) {
    p = table;
    do {
      *p = 0;
      if (i != self) {
        sp0 = cnt;
        if (Djinn_CheckTurnBalance(self, i) == 0) {
          *p = 1;
          cnt += 1;
        }
      }
      i += 1;
      p += 1;
    } while (i < state->ownerCount);
  }
  return cnt;
}

s32 Djinn_CheckTurnBalance(s32 from, s32 to) {
  struct DjinnMenuWork *work;
  s8 counts[16];
  u8 i;
  u8 j;
  s32 difference;
  s32 balanced;

  work = gMenuWork;
  Djinn_CountTurns((u8 *)counts);
  counts[from] -= 1;
  counts[to] += 1;
  balanced = 1;
  for (i = 0; i < work->ownerCount; i++) {
    j = i;
    if (i < work->ownerCount) {
      while (1) {
        j++;
        if (j >= work->ownerCount)
          break;
        difference = counts[i] - counts[j];
        if (difference < -1 || difference > 1) {
          balanced = 0;
          break;
        }
      }
    }
  }
  return balanced;
}

void Djinn_CountTurns(u8 *counts) {
  struct DjinnMenuWork *work;
  struct BattleUnit *unit;
  s32 ownerIndex;
  s32 element;
  s32 bit;
  s32 count;

  work = gMenuWork;
  for (ownerIndex = 0; ownerIndex < work->ownerCount; ownerIndex++) {
    unit = UnitGetFar(work->owners[ownerIndex]);
    count = 0;
    for (element = 0; element <= 3; element++) {
      for (bit = 0; bit <= 19; bit++) {
        if (unit->djinnActive[element] & (1 << bit))
          count++;
        else if (unit->djinnAvailable[element] & (1 << bit))
          count++;
      }
    }
    counts[ownerIndex] = count;
  }
}

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

void Menu_ResetTwoResourceEntries(void) {
  struct InventoryMenuState *state;

  state = gMenuWork;
  Resource_ResetEntry((u16)state->resourceSlots[0]);
  Resource_ResetEntry((u16)state->resourceSlots[1]);
}

s32 UiIcon_LoadResourceIntoSlot(s32 resourceId, s32 slot) {
  s32 resourceInfo;
  s32 selectedSlot;
  s32 buffer;
  s32 allocation;

  allocation = (s32)Runtime_AllocateBlock(0x11, 0x608);
  selectedSlot = slot;
  Ability_LoadGlyphFar(resourceId, 0, &selectedSlot, &resourceInfo, 1);
  buffer = VramCache_Reload(slot, allocation + 0x400);
  Runtime_ReleaseHeapBlock(0x11);
  return buffer;
}

s32 UiIcon_CreateWithLoadedResource(struct UiWindow *window, s32 x, s32 y,
                                    s32 resourceId) {
  s32 slot;

  slot = Resource_FindFreeEntry();
  if (slot != 0x60) {
    UiIcon_LoadResourceIntoSlot(resourceId, slot);
    RenderOutput_CreateFar(slot, 0x40000000, window, x, y);
  }
}

s32 UiIcon_CreateStatChangeArrow(struct UiWindow *window, s32 x, s32 y,
                                 s32 variant) {
  struct RenderOutput *object;
  u32 resource;
  s32 v = variant;
  struct UiWindow *w = window;
  s32 xx = x;
  struct InventoryMenuState *data = gMenuWork;

  if (v == 0)
    resource = (u16)data->resourceSlots[0];
  else
    resource = (u16)data->resourceSlots[1];
  object = RenderOutput_CreateFar(resource, 0x40000000, w, xx, y);
  if (object == 0) {
    /* FAKEMATCH: the redundant failure branch retains the argument
     * lifetimes through CSE; plain return -1 changes 16 aligned rows. */
    if (variant || xx)
      return -1;
    else
      return -1;
  }
  object->kind = 0;
  object->animStep = 0;
  object->active = ICON_SHOWN;
  return 1;
}

s32 UiIcon_DrawVariantWithTileOffset(struct UiWindow *window, s32 x, s32 y,
                                     s32 variant) {
  struct RenderOutput *object;
  u32 resource;
  s32 v = variant;
  struct UiWindow *w = window;
  s32 xx = x;
  struct InventoryMenuState *data = gMenuWork;

  if (v == 0) {
    resource = (u16)data->resourceSlots[0];
    y -= 3;
  } else {
    resource = (u16)data->resourceSlots[1];
    y -= 4;
  }
  object = RenderOutput_CreateFar(resource, 0x40000000, w, xx, y);
  if (object == 0) {
    /* FAKEMATCH: the redundant failure branch retains the argument
     * lifetimes through CSE; plain return -1 changes 22 aligned rows. */
    if (v || x)
      return -1;
    else
      return -1;
  }
  object->kind = 0;
  object->animStep = 0;
  object->active = ICON_SHOWN;
  return 1;
}
