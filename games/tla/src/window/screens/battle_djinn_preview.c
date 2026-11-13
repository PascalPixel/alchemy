/// @file
/// The preview of what setting or removing a Djinni changes.
// Part of screens.c.

#include "../screens_internal.h"
#include "djinn_preview.h"
#include "gamepak/far_call/game.h"
#include "module/tla.h"
#include "module/window/screens.h"
#include "module/window/window.h"

struct PreviewUnit {
  u8 unk000[0x38];
  s16 hp;
  s16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u8 luck;
  u8 unk043[0x15];
  u16 psynergy[1];
  u8 unk05a[0xcf];
  u8 classId;
  u8 unk12a[0x22];
};

s32 String_FormatNumberToHalfwords(s16 *out, s32 value) {
  s16 *dst;
  s32 n;
  u8 *src;

  dst = out;
  String_FormatDecimal(value);
  src = gNumberTextBuffer;
  n = 0xD;
  do {
    n -= 1;
    *dst = *src;
    src += 1;
    dst += 1;
  } while (n >= 0);
}

/// Lists into `out` the abilities usable in battle (flag 0x80) of the action
/// list `newGrid`, in its order, marking with bit 15 those `oldGrid` lacks;
/// then appends those of `oldGrid` that `newGrid` lacks, marked with bit 14.
/// Both lists are 32 slots, ended early by a 0 action. Returns the entries
/// written, and the gained and lost counts through `gained` and `lost`.
s32 BattleDjinnMenu_ListChangedDjinn(struct DjinnSlot *oldGrid,
                                     struct DjinnSlot *newGrid, u16 *out,
                                     s32 *gained, s32 *lost) {
  s32 i;
  s32 j;
  s32 count;
  s32 gainedCount;
  s32 lostCount;

  gainedCount = 0;
  lostCount = 0;
  count = 0;
  for (i = 0; i < 32 && newGrid[i].id != 0; i++) {
    if (Ability_Get(newGrid[i].id)->flags & 0x80) {
      out[count] = newGrid[i].id & 0x3fff;
      count++;
      for (j = 0; j < 32; j++) {
        if (((newGrid[i].id ^ oldGrid[j].id) & 0x3fff) == 0)
          break;
      }
      if (j == 32) {
        gainedCount++;
        out[count - 1] |= 0x8000;
      }
    }
  }
  for (i = 0; i < 32 && oldGrid[i].id != 0; i++) {
    if (Ability_Get(oldGrid[i].id)->flags & 0x80) {
      for (j = 0; j < 32; j++) {
        if (((oldGrid[i].id ^ newGrid[j].id) & 0x3fff) == 0)
          break;
      }
      if (j == 32) {
        lostCount++;
        out[count] = (oldGrid[i].id & 0x3fff) | 0x4000;
        count++;
      }
    }
  }
  *gained = gainedCount;
  *lost = lostCount;
  return count;
}

/// @file
/// Display: place an ability icon in a window as a sprite.

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "render_input.h"
#include "types.h"
#include "resource.h"
#include "vram_tab.h"

/// Loads the icon into a free VRAM cache slot and draws a 16x16 sprite;
/// does nothing if no slot is free.
/// input: destination window.
/// x, y: position within the window, in pixels.
/// ability: ability index.
s32 WindowSprite_CreateAbilityIcon(struct RenderInput *input, s32 x, s32 y,
                                   s32 ability) {
  s32 slot;

  slot = VramCache_FindFreeEntry();
  if (slot != VRAM_CACHE_ENTRY_COUNT) {
    Resource_LoadIndexedEntryToBuffer(ability, slot);
    WindowSprite_Create(slot, OUTPUT_SIZE_16X16, input, x, y);
  }
}

// Where the line at the bottom starts: further left in French.
#if (LANGUAGE == LANGUAGE_FRENCH)
#define TOTAL_LABEL_X 24
#else
#define TOTAL_LABEL_X 32
#endif

// The stats page: Japanese makes its window a tile wider, draws the old values
// a column further left, Defense and Agility in four places as HP, PP and
// Attack (the other editions give those two three places from the next
// column), and starts the prompt under them (MsgKey0d0d) 16 pixels further
// right.
#if LANGUAGE == LANGUAGE_JAPANESE
#define STATS_WINDOW_WIDTH 21
#define OLD_VALUE_X 4
#define OLD_DEFENSE_AGILITY_TEXT 7
#define OLD_DEFENSE_AGILITY_X 4
#define PSYNERGY_PROMPT_X 40
#else
#define STATS_WINDOW_WIDTH 20
#define OLD_VALUE_X 5
#define OLD_DEFENSE_AGILITY_TEXT 8
#define OLD_DEFENSE_AGILITY_X 6
#define PSYNERGY_PROMPT_X 24
#endif

// Text colours (Text_SetColor).
#define PREVIEW_RED 2
#define PREVIEW_YELLOW 4
#define PREVIEW_WHITE 15

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
  struct PreviewUnit *unit;
  struct PreviewUnit *savedUnit;
  struct PreviewAbility *ability;
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

  void Func_08046414(s32 x, s32 y, s32 up) {
    struct WindowSprite *output = WindowSprite_AcquireFree();
    struct ArrowSprite *entry;

    if (output) {
      output->active = 1;
      output->kind = 1;
      output->index = VramCache_AllocEntry(128);
      entry = (struct ArrowSprite *)&output->sprite;
      output->sentinel = 240;
      output->x = 120;
      output->y = 120;
      entry->attributes.word = 0x40000400;
      entry->tile.value = 0;
      entry->attributes.bits.x = (s16)window->x * 8 + x;
      entry->attributes.bits.y = (s16)window->y * 8 + y;
      /* FAKEMATCH(dead): advancing and restoring the captured pointer keeps
       * the window chain in r9; as no pair, 32 of 80 instructions differ in
       * tla-en. */
      window++;
      window--;
      entry->tile.bits.index =
          VramCache_Reload((u8)output->index, up ? gDjinnMenu_ArrowUpTiles
                                                 : gDjinnMenu_ArrowDownTiles);
      WindowSprite_AppendToList(&window->output, output);
    }
  }

  unit = Unit_Get(member);
  if (window != NULL)
    Window_Close(window, 1);
  if (page == 0)
    window = Window_Create(0, 8, STATS_WINDOW_WIDTH, 11, 6);
  else
    window = Window_Create(0, 5, 21, 14, 6);
  if (window == NULL)
    return NULL;

  numberText = Heap_Alloc(64 * sizeof(*numberText));
  savedUnit = Heap_Alloc(sizeof(*savedUnit));
  changedAbilities = Heap_AllocEwram(48 * sizeof(*changedAbilities));
  Iwram_CopyWordsEntry(savedUnit, unit, sizeof(*savedUnit));

  element = (encodedDjinn >> 8) & 15;
  djinnIndex = encodedDjinn & 255;
  if (Djinn_IsSetFar(member, element, djinnIndex) != 0)
    Djinn_PutOnStandbyFar(member, element, djinnIndex);
  else
    Djinn_SetFar(member, element, djinnIndex);
  Unit_CalculateFar(member);

  abilityCount = BattleDjinnMenu_ListChangedDjinn(
      savedUnit->psynergy, unit->psynergy, changedAbilities, &gained, &lost);
  pages = ((abilityCount - 1) / 5) + 1;
  *pageCount = pages;
  if (page * 5 - 5 >= abilityCount)
    page = pages;

  if (page == 0) {
    s32 label = (s32)MsgKey0d0e;

    Text_DrawMessageAsTiles(label, window, 0, 8);
    Text_DrawMessageAsTiles(label + 1, window, 0, 16);
    Text_DrawMessageAsTiles(label + 2, window, 0, 24);
    Text_DrawMessageAsTiles(label + 3, window, 0, 32);
    Text_DrawMessageAsTiles(label + 4, window, 0, 40);
    Text_DrawMessageAsTiles(label + 5, window, 0, 48);

    oldHp = savedUnit->hp;
    String_FormatNumberToHalfwords(numberText, oldHp);
    Text_DrawWideStringInWindow(numberText + 7, window, OLD_VALUE_X, 1);
    oldPp = savedUnit->pp;
    String_FormatNumberToHalfwords(numberText, oldPp);
    Text_DrawWideStringInWindow(numberText + 7, window, OLD_VALUE_X, 2);
    oldAtk = savedUnit->atk;
    String_FormatNumberToHalfwords(numberText, oldAtk);
    Text_DrawWideStringInWindow(numberText + 7, window, OLD_VALUE_X, 3);
    oldDef = savedUnit->def;
    String_FormatNumberToHalfwords(numberText, oldDef);
    Text_DrawWideStringInWindow(numberText + OLD_DEFENSE_AGILITY_TEXT, window,
                                OLD_DEFENSE_AGILITY_X, 4);
    oldAgi = savedUnit->agi;
    String_FormatNumberToHalfwords(numberText, oldAgi);
    Text_DrawWideStringInWindow(numberText + OLD_DEFENSE_AGILITY_TEXT, window,
                                OLD_DEFENSE_AGILITY_X, 5);
    String_FormatNumberToHalfwords(numberText, savedUnit->luck);
    Text_DrawWideStringInWindow(numberText + 8, window, OLD_VALUE_X + 1, 6);

    Window_DrawDividerLine(window, 0, 8, 19, 8);
    if (gained != 0 || lost != 0)
      Text_SetColor(PREVIEW_RED);
    Text_DrawMessageAsTiles((s32)MsgKey0d0d, window, PSYNERGY_PROMPT_X, 64);
    Text_SetColor(PREVIEW_WHITE);
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
      if (ability->element != 4)
        Window_SetTilemapEntry(window, 0x5001 + ability->element, 15,
                               visibleRow * 2, 0);
      rangePattern = ability->range;
      if (rangePattern == 255)
        rangePattern = 11;
      else
        rangePattern--;
      Window_DrawThreeTileColumn(window, 16, visibleRow * 2, rangePattern, 0);
      WindowSprite_CreateAbilityIcon((struct RenderInput *)window, 0,
                                     visibleRow * 16 - 4,
                                     changedAbilities[i] & 0x3fff);
      if (changedAbilities[i] & 0x8000)
        Text_SetColor(PREVIEW_YELLOW);
      else if (changedAbilities[i] & 0x4000)
        Text_SetColor(PREVIEW_RED);
      else
        Text_SetColor(PREVIEW_WHITE);
      Text_DrawMessageAsTiles((changedAbilities[i] & 0x3fff) +
                                  (s32)MsgAbilityNothing,
                              window, 16, visibleRow * 16);
      Window_SetTilemapEntry(window, WINDOW_TILE_PP_LABEL, 11, visibleRow * 2,
                             0);
      Window_SetTilemapEntry(window, WINDOW_TILE_PP_LABEL_END, 12,
                             visibleRow * 2, 0);
      Text_DrawNumberInWindow(Ability_Get(changedAbilities[i])->ppCost, 2,
                              window, 104, visibleRow * 16);
    }
    if (gained != 0) {
      Text_SetColor(PREVIEW_YELLOW);
      Text_DrawMessageAsTiles((s32)MsgKey10d3, window, TOTAL_LABEL_X, 80);
      summaryRows = 1;
    }
    if (lost != 0) {
      Text_SetColor(PREVIEW_RED);
      Text_DrawMessageAsTiles((s32)MsgKey10d4, window, TOTAL_LABEL_X,
                              summaryRows * 8 + 80);
      summaryRows++;
    }
    if (summaryRows == 0)
      Text_DrawMessageAsTiles((s32)MsgKey10d9, window, TOTAL_LABEL_X, 80);
    Text_SetColor(PREVIEW_WHITE);
    Text_SetColor(PREVIEW_WHITE);
    Window_DrawDividerLine(window, 0, 10, 19, 10);
  }

  if (page == 0) {
    Text_DrawMessageAsTiles((s32)MsgClassName + savedUnit->classId, window, 0,
                            0);
    Text_DrawMessageAsTiles((s32)MsgClassName + unit->classId, window, 80, 0);
    if (savedUnit->classId != unit->classId)
      Window_SetTilemapEntry(window, 0xf728, 9, 0, page);
    else
      Window_SetTilemapEntry(window, 0xf729, 9, 0, page);

    String_FormatNumberToHalfwords(numberText, unit->hp);
    Text_DrawWideStringInWindow(numberText + 7, window, 11, 1);
    if (unit->hp != oldHp) {
      rising = 0;
      if (unit->hp > oldHp)
        rising = 1;
      Func_08046414(80, 14, rising);
    }
    String_FormatNumberToHalfwords(numberText, unit->pp);
    Text_DrawWideStringInWindow(numberText + 7, window, 11, 2);
    if (unit->pp != oldPp) {
      rising = 0;
      if (unit->pp > oldPp)
        rising = 1;
      Func_08046414(80, 22, rising);
    }
    String_FormatNumberToHalfwords(numberText, unit->atk);
    Text_DrawWideStringInWindow(numberText + 7, window, 11, 3);
    if (unit->atk != oldAtk) {
      rising = 0;
      if (unit->atk > oldAtk)
        rising = 1;
      Func_08046414(80, 30, rising);
    }
    String_FormatNumberToHalfwords(numberText, unit->def);
    Text_DrawWideStringInWindow(numberText + 7, window, 11, 4);
    if (unit->def != oldDef) {
      rising = 0;
      if (unit->def > oldDef)
        rising = 1;
      Func_08046414(80, 38, rising);
    }
    String_FormatNumberToHalfwords(numberText, unit->agi);
    Text_DrawWideStringInWindow(numberText + 7, window, 11, 5);
    if (unit->agi != oldAgi) {
      rising = 0;
      if (unit->agi > oldAgi)
        rising = 1;
      Func_08046414(80, 46, rising);
    }
    String_FormatNumberToHalfwords(numberText, unit->luck);
    Text_DrawWideStringInWindow(numberText + 8, window, 12, 6);
    if (unit->luck != savedUnit->luck) {
      rising = 0;
      if (unit->luck > savedUnit->luck)
        rising = 1;
      Func_08046414(80, 54, rising);
    }
  }

  Iwram_CopyWordsEntry(unit, savedUnit, sizeof(*savedUnit));
  Heap_Release(changedAbilities);
  Heap_Release(savedUnit);
  Heap_Release(numberText);
  return window;
}

#undef TOTAL_LABEL_X
#undef STATS_WINDOW_WIDTH
#undef OLD_VALUE_X
#undef OLD_DEFENSE_AGILITY_TEXT
#undef OLD_DEFENSE_AGILITY_X
#undef PSYNERGY_PROMPT_X
#undef PREVIEW_RED
#undef PREVIEW_YELLOW
#undef PREVIEW_WHITE
