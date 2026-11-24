/// @file
/// The preview of what setting or removing a Djinni changes.
// Part of screens.c.

#include "window_internal.h"
#include "window/djinn_preview.h"
#include "gamepak/far_call/game.h"
#include "window/screens.h"
#include "window/window.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "graphics/render_input.h"
#include "types.h"
#include "graphics/resource.h"
#include "graphics/vram_tab.h"

/// How many bytes of gNumberTextBuffer are widened: the eleven characters
/// String_FormatDecimal writes, its terminator and the two bytes after.
#define NUMBER_TEXT_WIDE_LENGTH 14
/// Where a stat's last four characters start in its widened text.
#define DJINN_PREVIEW_STAT_DIGIT_OFFSET 7

/// Writes `value` into `out` as String_FormatDecimal's eleven-character,
/// right-aligned decimal text, widened to halfwords for
/// Text_DrawWideStringInWindow. `out` needs room for NUMBER_TEXT_WIDE_LENGTH
/// halfwords.
s32 String_FormatNumberToHalfwords(s16 *out, s32 value) {
  s16 *destination;
  s32 i;
  u8 *source;

  destination = out;
  String_FormatDecimal(value);
  source = gNumberTextBuffer;
  for (i = 0; i < NUMBER_TEXT_WIDE_LENGTH; i++) {
    *destination++ = *source++;
  }
}

/// A changed stat's arrow, right of its value on window tile row `row` (HP
/// on row 1 to Luck on row 6).
#define PREVIEW_STAT_ARROW_X 80
#define PREVIEW_STAT_ARROW_Y(row) ((row) * 8 + 6)

/// Entries of the change list: the new list's 32 and the lost ones after.
#define DJINN_PREVIEW_ABILITY_CAPACITY 48

/// The marks BattleDjinnMenu_ListChangedPsynergy gives an ability in its list.
#define DJINN_PREVIEW_ABILITY_GAINED 0x8000 ///< new with the change
#define DJINN_PREVIEW_ABILITY_LOST 0x4000   ///< gone with the change

/// Lists into `out` the abilities usable in battle (ABILITY_USABLE_IN_BATTLE)
/// of the action list `newGrid`, in its order, marking with
/// DJINN_PREVIEW_ABILITY_GAINED those `oldGrid` lacks; then appends those of
/// `oldGrid` that `newGrid` lacks, marked DJINN_PREVIEW_ABILITY_LOST.
/// Both lists are 32 slots, ended early by a 0 action. Returns the entries
/// written, and the gained and lost counts through `gained` and `lost`.
s32 BattleDjinnMenu_ListChangedPsynergy(const struct PsynergyEntry *oldGrid,
                                        const struct PsynergyEntry *newGrid,
                                        u16 *out, s32 *gained, s32 *lost) {
  s32 i;
  s32 j;
  s32 count;
  s32 gainedCount;
  s32 lostCount;

  gainedCount = 0;
  lostCount = 0;
  count = 0;
  for (i = 0; i < PSYNERGY_SLOTS && newGrid[i].encodedAction != 0; i++) {
    if (Ability_Get(newGrid[i].encodedAction)->flags &
        ABILITY_USABLE_IN_BATTLE) {
      out[count] = newGrid[i].encodedAction & PSYNERGY_ABILITY_MASK;
      count++;
      for (j = 0; j < PSYNERGY_SLOTS; j++) {
        if (((newGrid[i].encodedAction ^ oldGrid[j].encodedAction) &
             PSYNERGY_ABILITY_MASK) == 0)
          break;
      }
      if (j == PSYNERGY_SLOTS) {
        gainedCount++;
        out[count - 1] |= DJINN_PREVIEW_ABILITY_GAINED;
      }
    }
  }
  for (i = 0; i < PSYNERGY_SLOTS && oldGrid[i].encodedAction != 0; i++) {
    if (Ability_Get(oldGrid[i].encodedAction)->flags &
        ABILITY_USABLE_IN_BATTLE) {
      for (j = 0; j < PSYNERGY_SLOTS; j++) {
        if (((oldGrid[i].encodedAction ^ newGrid[j].encodedAction) &
             PSYNERGY_ABILITY_MASK) == 0)
          break;
      }
      if (j == PSYNERGY_SLOTS) {
        lostCount++;
        out[count] = (oldGrid[i].encodedAction & PSYNERGY_ABILITY_MASK) |
                     DJINN_PREVIEW_ABILITY_LOST;
        count++;
      }
    }
  }
  *gained = gainedCount;
  *lost = lostCount;
  return count;
}

/// Loads ability `abilityId`'s icon into a free VRAM cache slot and draws it
/// as a 16x16 sprite at (x, y) of `window`, in pixels; does nothing when no
/// slot is free.
s32 WindowSprite_CreateAbilityIcon(struct Window *window, s32 x, s32 y,
                                   s32 abilityId) {
  s32 iconSlot;

  iconSlot = VramCache_FindFreeEntry();
  if (iconSlot != VRAM_CACHE_ENTRY_COUNT) {
    Resource_LoadIndexedEntryToBuffer(abilityId, iconSlot);
    // If the sprite pool is full, the creator releases iconSlot itself.
    WindowSprite_Create(iconSlot, WINDOW_SPRITE_SIZE_16X16, window, x, y);
  }
}

/// Where the line at the bottom starts: further left in French.
#if (LANGUAGE == LANGUAGE_FRENCH)
#define TOTAL_LABEL_X 24
#else
#define TOTAL_LABEL_X 32
#endif

/// The stats page: Japanese makes its window a tile wider, draws the old values
/// a column further left, Defense and Agility in four places as HP, PP and
/// Attack (the other editions give those two three places from the next
/// column), and starts the prompt under them (MsgBattleAdditionalPsynergyHelp)
/// 16 pixels further right.
#if LANGUAGE == LANGUAGE_JAPANESE
#define STAT_WINDOW_WIDTH 21
#define OLD_VALUE_X 4
#define OLD_DEFENSE_AGILITY_TEXT 7
#define OLD_DEFENSE_AGILITY_X 4
#define PSYNERGY_PROMPT_X 40
#else
#define STAT_WINDOW_WIDTH 20
#define OLD_VALUE_X 5
#define OLD_DEFENSE_AGILITY_TEXT 8
#define OLD_DEFENSE_AGILITY_X 6
#define PSYNERGY_PROMPT_X 24
#endif

/// Shows what setting or removing Djinni `encodedDjinn` (element << 8 |
/// index) would change for `member`: closes the preview window `win`, opens a
/// new one and returns it, or NULL when none could be opened. Page 0
/// compares the class and stats, old against new, with an arrow by each stat
/// that rises or falls; pages 1 on list, five a page, the unit's battle
/// abilities after the change (unchanged ones in TEXT_COLOR_WHITE, new ones in
/// TEXT_COLOR_YELLOW) and then those it would lose (TEXT_COLOR_RED), and
/// *pageCount is set to their number (at least 1). The Djinni is toggled and
/// the unit recalculated (Unit_CalculateFar) only to measure: the unit is
/// restored from a copy before it returns. BattleDjinnMenu_Run calls it as the
/// cursor moves.
struct Window *BattleDjinnMenu_ShowChangePreview(struct Window *window,
                                                 s32 member, s32 encodedDjinn,
                                                 s32 page, s32 *pageCount) {
  struct Unit *unit;
  struct Unit *savedUnit;
  struct AbilityDefinition *ability;
  s16 *numberText;
  s32 oldHp;
  s32 oldPp;
  s32 oldAtk;
  s32 oldDef;
  s32 oldAgi;
  u16 *changedAbilities;
  s32 abilityCount;
  s32 visibleRow;
  s32 summaryRows;
  s32 visibleRows;
  s32 element;
  s32 djinnIndex;
  s32 pages;
  s32 gained;
  s32 lost;
  s32 rangePattern;
  s32 rising;
  s32 i;

  void DjinnMenu_DrawStatArrow(s32 x, s32 y, s32 up) {
    struct WindowSprite *output = WindowSprite_AcquireFree();
    struct PreviewSprite *entry;

    if (output) {
      output->motion = WINDOW_SPRITE_MOTION_STILL;
      output->kind = WINDOW_SPRITE_ICON;
      output->index = VramCache_AllocEntry(128);
      entry = (struct PreviewSprite *)&output->sprite;
      output->depth = OAM_DEPTH_WINDOW;
      output->x = 120;
      output->y = 120;
      entry->attributes.word =
          OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
      entry->tile.value = 0;
      entry->attributes.bits.x = (s16)window->x * 8 + x;
      entry->attributes.bits.y = (s16)window->y * 8 + y;
      /* FAKEMATCH(dead): advancing and restoring the captured pointer. */
      window++;
      window--;
      entry->tile.bits.index =
          VramCache_Reload((u8)output->index, up ? gDjinnMenu_ArrowUpTiles
                                                 : gDjinnMenu_ArrowDownTiles);
      WindowSprite_AppendToList(&window->sprites, output);
    }
  }

  unit = Unit_Get(member);
  if (window != NULL)
    Window_Close(window, 1);
  if (page == 0)
    window = Window_Create(0, 8, STAT_WINDOW_WIDTH, 11,
                           WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
  else
    window =
        Window_Create(0, 5, 21, 14, WINDOW_FLAG_INSTANT | WINDOW_FLAG_UNREAD);
  if (window == NULL)
    return NULL;

  numberText = Heap_Alloc(64 * sizeof(*numberText));
  savedUnit = Heap_Alloc(sizeof(*savedUnit));
  changedAbilities = Heap_AllocEwram(DJINN_PREVIEW_ABILITY_CAPACITY *
                                     sizeof(*changedAbilities));
  Iwram_CopyWordsEntry(savedUnit, unit, sizeof(*savedUnit));

  element = DJINN_COMMAND_ELEMENT(encodedDjinn);
  djinnIndex = DJINN_COMMAND_INDEX(encodedDjinn);
  if (Djinn_IsSetFar(member, element, djinnIndex) != 0)
    Djinn_PutOnStandbyFar(member, element, djinnIndex);
  else
    Djinn_SetFar(member, element, djinnIndex);
  Unit_CalculateFar(member);

  abilityCount = BattleDjinnMenu_ListChangedPsynergy(
      savedUnit->psynergy, unit->psynergy, changedAbilities, &gained, &lost);
  pages = ((abilityCount - 1) / 5) + 1;
  *pageCount = pages;
  if (page * 5 - 5 >= abilityCount)
    page = pages;

  if (page == 0) {
    s32 label = (s32)MsgPreviewStatLabel;

    Text_DrawMessageAsTiles(label, window, 0, 8);
    Text_DrawMessageAsTiles(label + 1, window, 0, 16);
    Text_DrawMessageAsTiles(label + 2, window, 0, 24);
    Text_DrawMessageAsTiles(label + 3, window, 0, 32);
    Text_DrawMessageAsTiles(label + 4, window, 0, 40);
    Text_DrawMessageAsTiles(label + 5, window, 0, 48);

    oldHp = savedUnit->hp;
    String_FormatNumberToHalfwords(numberText, oldHp);
    Text_DrawWideStringInWindow(numberText + DJINN_PREVIEW_STAT_DIGIT_OFFSET,
                                window, OLD_VALUE_X, 1);
    oldPp = savedUnit->pp;
    String_FormatNumberToHalfwords(numberText, oldPp);
    Text_DrawWideStringInWindow(numberText + DJINN_PREVIEW_STAT_DIGIT_OFFSET,
                                window, OLD_VALUE_X, 2);
    oldAtk = savedUnit->attack;
    String_FormatNumberToHalfwords(numberText, oldAtk);
    Text_DrawWideStringInWindow(numberText + DJINN_PREVIEW_STAT_DIGIT_OFFSET,
                                window, OLD_VALUE_X, 3);
    oldDef = savedUnit->defense;
    String_FormatNumberToHalfwords(numberText, oldDef);
    Text_DrawWideStringInWindow(numberText + OLD_DEFENSE_AGILITY_TEXT, window,
                                OLD_DEFENSE_AGILITY_X, 4);
    oldAgi = savedUnit->agility;
    String_FormatNumberToHalfwords(numberText, oldAgi);
    Text_DrawWideStringInWindow(numberText + OLD_DEFENSE_AGILITY_TEXT, window,
                                OLD_DEFENSE_AGILITY_X, 5);
    String_FormatNumberToHalfwords(numberText, savedUnit->luck);
    Text_DrawWideStringInWindow(numberText + 8, window, OLD_VALUE_X + 1, 6);

    Window_DrawDividerLine(window, 0, 8, 19, 8);
    if (gained != 0 || lost != 0)
      Text_SetColor(TEXT_COLOR_RED);
    Text_DrawMessageAsTiles((s32)MsgBattleAdditionalPsynergyHelp, window,
                            PSYNERGY_PROMPT_X, 64);
    Text_SetColor(TEXT_COLOR_WHITE);
  }

  if (page > 0) {
    summaryRows = 0;
    visibleRows = abilityCount;
    if (abilityCount > 4)
      visibleRows = 5;
    i = page * 5 - 5;
    for (visibleRow = 0; visibleRow < visibleRows && i < abilityCount;
         visibleRow++, i++) {
      ability = Ability_Get(changedAbilities[i]);
      if (ability->element != ELEMENT_NONE)
        Window_SetTilemapEntry(window, ELEMENT_ICON_TILE + ability->element, 15,
                               visibleRow * 2, 0);
      rangePattern = ability->range;
      if (rangePattern == ABILITY_RANGE_ALL)
        rangePattern = RANGE_ICON_ALL;
      else
        rangePattern--;
      Window_DrawThreeTileColumn(window, 16, visibleRow * 2, rangePattern, 0);
      WindowSprite_CreateAbilityIcon((struct Window *)window, 0,
                                     visibleRow * 16 - 4,
                                     changedAbilities[i] & ACTION_ID_MASK);
      if (changedAbilities[i] & DJINN_PREVIEW_ABILITY_GAINED)
        Text_SetColor(TEXT_COLOR_YELLOW);
      else if (changedAbilities[i] & DJINN_PREVIEW_ABILITY_LOST)
        Text_SetColor(TEXT_COLOR_RED);
      else
        Text_SetColor(TEXT_COLOR_WHITE);
      Text_DrawMessageAsTiles((changedAbilities[i] & ACTION_ID_MASK) +
                                  (s32)MsgAbilityName,
                              window, 16, visibleRow * 16);
      Window_SetTilemapEntry(window, WINDOW_TILE_PP_LABEL, 11, visibleRow * 2,
                             0);
      Window_SetTilemapEntry(window, WINDOW_TILE_PP_LABEL_END, 12,
                             visibleRow * 2, 0);
      Text_DrawNumberInWindow(Ability_Get(changedAbilities[i])->ppCost, 2,
                              window, 104, visibleRow * 16);
    }
    if (gained != 0) {
      Text_SetColor(TEXT_COLOR_YELLOW);
      Text_DrawMessageAsTiles((s32)MsgPsynergyGained, window, TOTAL_LABEL_X,
                              80);
      summaryRows = 1;
    }
    if (lost != 0) {
      Text_SetColor(TEXT_COLOR_RED);
      Text_DrawMessageAsTiles((s32)MsgPsynergyLost, window, TOTAL_LABEL_X,
                              summaryRows * 8 + 80);
      summaryRows++;
    }
    if (summaryRows == 0)
      Text_DrawMessageAsTiles((s32)MsgPsynergyUnchanged, window, TOTAL_LABEL_X,
                              80);
    Text_SetColor(TEXT_COLOR_WHITE);
    Text_SetColor(TEXT_COLOR_WHITE);
    Window_DrawDividerLine(window, 0, 10, 19, 10);
  }

  if (page == 0) {
    Text_DrawMessageAsTiles((s32)MsgClassName + savedUnit->classId, window, 0,
                            0);
    Text_DrawMessageAsTiles((s32)MsgClassName + unit->classId, window, 80, 0);
    // The arrow between the classes, flipped: DJINN_MENU_ARROW_TILE blinks
    // when the class changes, the tile after it stays.
    if (savedUnit->classId != unit->classId)
      Window_SetTilemapEntry(
          window, WINDOW_TILE(DJINN_MENU_ARROW_TILE) | TILE_HFLIP, 9, 0, page);
    else
      Window_SetTilemapEntry(
          window, WINDOW_TILE(DJINN_MENU_ARROW_TILE + 1) | TILE_HFLIP, 9, 0,
          page);

    String_FormatNumberToHalfwords(numberText, unit->hp);
    Text_DrawWideStringInWindow(numberText + DJINN_PREVIEW_STAT_DIGIT_OFFSET,
                                window, 11, 1);
    if (unit->hp != oldHp) {
      rising = 0;
      if (unit->hp > oldHp)
        rising = 1;
      DjinnMenu_DrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(1),
                              rising);
    }
    String_FormatNumberToHalfwords(numberText, unit->pp);
    Text_DrawWideStringInWindow(numberText + DJINN_PREVIEW_STAT_DIGIT_OFFSET,
                                window, 11, 2);
    if (unit->pp != oldPp) {
      rising = 0;
      if (unit->pp > oldPp)
        rising = 1;
      DjinnMenu_DrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(2),
                              rising);
    }
    String_FormatNumberToHalfwords(numberText, unit->attack);
    Text_DrawWideStringInWindow(numberText + DJINN_PREVIEW_STAT_DIGIT_OFFSET,
                                window, 11, 3);
    if (unit->attack != oldAtk) {
      rising = 0;
      if (unit->attack > oldAtk)
        rising = 1;
      DjinnMenu_DrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(3),
                              rising);
    }
    String_FormatNumberToHalfwords(numberText, unit->defense);
    Text_DrawWideStringInWindow(numberText + DJINN_PREVIEW_STAT_DIGIT_OFFSET,
                                window, 11, 4);
    if (unit->defense != oldDef) {
      rising = 0;
      if (unit->defense > oldDef)
        rising = 1;
      DjinnMenu_DrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(4),
                              rising);
    }
    String_FormatNumberToHalfwords(numberText, unit->agility);
    Text_DrawWideStringInWindow(numberText + DJINN_PREVIEW_STAT_DIGIT_OFFSET,
                                window, 11, 5);
    if (unit->agility != oldAgi) {
      rising = 0;
      if (unit->agility > oldAgi)
        rising = 1;
      DjinnMenu_DrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(5),
                              rising);
    }
    String_FormatNumberToHalfwords(numberText, unit->luck);
    Text_DrawWideStringInWindow(numberText + 8, window, 12, 6);
    if (unit->luck != savedUnit->luck) {
      rising = 0;
      if (unit->luck > savedUnit->luck)
        rising = 1;
      DjinnMenu_DrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(6),
                              rising);
    }
  }

  Iwram_CopyWordsEntry(unit, savedUnit, sizeof(*savedUnit));
  Heap_Release(changedAbilities);
  Heap_Release(savedUnit);
  Heap_Release(numberText);
  return window;
}

#undef TOTAL_LABEL_X
#undef STAT_WINDOW_WIDTH
#undef OLD_VALUE_X
#undef OLD_DEFENSE_AGILITY_TEXT
#undef OLD_DEFENSE_AGILITY_X
#undef PSYNERGY_PROMPT_X
