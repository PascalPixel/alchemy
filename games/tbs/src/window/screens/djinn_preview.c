/// @file
/// The preview of what setting or removing a Djinni changes.
// Part of screens.c.

#include "../screens_internal.h"

/// How many bytes of gNumberBuffer are widened: the eleven characters
/// String_FormatDecimal writes, its terminator and the two bytes after.
#define NUMBER_TEXT_WIDE_LENGTH 14

// Writes `value` into `out` as String_FormatDecimal's eleven-character,
// right-aligned decimal text, widened to halfwords for
// Text_DrawWideStringInWindow: out + 7 is its last four characters,
// out + 8 its last three. `out` needs room for 14 halfwords.
// It is declared to return s32 but returns no value, and no caller reads one;
// UBFIX makes it void, which changes its epilogue and the argument setup of
// three calls in BattleDjinnMenu_ShowChangePreview.
#ifdef UBFIX
void String_FormatNumberToHalfwords(s16 *out, s32 value) {
#else
s32 String_FormatNumberToHalfwords(s16 *out, s32 value) {
#endif
  s16 *destination;
  s32 remaining;
  u8 *source;

  destination = out;
  String_FormatDecimal(value);
  source = gNumberBuffer;
  for (remaining = 0; remaining < NUMBER_TEXT_WIDE_LENGTH; remaining++) {
    *destination++ = *source++;
  }
}

// Preview marks are separate from the action ID in the low fourteen bits.
#define DJINN_PREVIEW_ABILITY_GAINED 0x8000 ///< new with the change; colour 4
#define DJINN_PREVIEW_ABILITY_LOST 0x4000   ///< gone with the change; colour 2
/// Entries of the change list: the new list's 32 and the lost ones after.
#define DJINN_PREVIEW_ABILITY_CAPACITY 48

/// Lists into `out` the abilities usable in battle (BATTLE_OK) of the action
/// list `newGrid`, in its order, marking with DJINN_PREVIEW_ABILITY_GAINED
/// those `oldGrid` lacks; then appends those of `oldGrid` that `newGrid`
/// lacks, marked DJINN_PREVIEW_ABILITY_LOST. Both lists are 32 slots, ended
/// early by a 0 action. Returns the entries written, and the gained and lost
/// counts through `gained` and `lost`.
s32 BattleDjinnMenu_ListChangedDjinn(const struct PsynergyEntry *oldGrid,
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
  for (i = 0; i < 32 && newGrid[i].encodedAction != 0; i++) {
    if (Ability_Get(newGrid[i].encodedAction)->flags & 0x80) {
      out[count] = newGrid[i].encodedAction & ACTION_ID_MASK;
      count++;
      for (j = 0; j < 32; j++) {
        if (((newGrid[i].encodedAction ^ oldGrid[j].encodedAction) &
             ACTION_ID_MASK) == 0)
          break;
      }
      if (j == 32) {
        gainedCount++;
        out[count - 1] |= DJINN_PREVIEW_ABILITY_GAINED;
      }
    }
  }
  for (i = 0; i < 32 && oldGrid[i].encodedAction != 0; i++) {
    if (Ability_Get(oldGrid[i].encodedAction)->flags & 0x80) {
      for (j = 0; j < 32; j++) {
        if (((oldGrid[i].encodedAction ^ newGrid[j].encodedAction) &
             ACTION_ID_MASK) == 0)
          break;
      }
      if (j == 32) {
        lostCount++;
        out[count] = (oldGrid[i].encodedAction & ACTION_ID_MASK) |
                     DJINN_PREVIEW_ABILITY_LOST;
        count++;
      }
    }
  }
  *gained = gainedCount;
  *lost = lostCount;
  return count;
}

// Loads the icon of ability `abilityId` into the first free VRAM cache slot
// and shows it as a 16x16 sprite at pixel (x, y) inside `window`, whose
// sprite list then owns the slot. Does nothing when the cache is full.
// It is declared to return s32 but returns no value, and no caller reads one;
// UBFIX makes it void, which changes the register its epilogue returns
// through.
#ifdef UBFIX
void WindowSprite_CreateAbilityIcon(struct Window *window, s32 x, s32 y,
                                    s32 abilityId) {
#else
s32 WindowSprite_CreateAbilityIcon(struct Window *window, s32 x, s32 y,
                                   s32 abilityId) {
#endif
  s32 iconSlot;

  iconSlot = VramCache_FindFreeEntry();
  if (iconSlot != VRAM_CACHE_ENTRY_COUNT) {
    Resource_LoadIndexedEntryToBuffer(abilityId, iconSlot);
    // If the sprite pool is full, the creator releases iconSlot itself.
    WindowSprite_Create(iconSlot, OUTPUT_SIZE_16X16, window, x, y);
  }
}

/// Shows what setting or removing Djinni `encodedDjinn` (element << 8 |
/// index) would change for `member`: closes the preview window `win`, opens a
/// new one and returns it, or NULL when none could be opened. Page 0
/// compares the class and stats, old against new, with an arrow by each stat
/// that rises or falls; pages 1 on list, five a page, the unit's battle
/// abilities after the change (unchanged ones in colour 15, new ones in
/// colour 4) and then those it would lose (colour 2), and *pageCount is set
/// to their number (at least 1).
/// The Djinni is toggled and the unit recalculated (Unit_CalculateFar) only to
/// measure: the unit is restored from a copy before it returns.
/// BattleDjinnMenu_Run calls it as the cursor moves.
struct Window *BattleDjinnMenu_ShowChangePreview(struct Window *win, s32 member,
                                                 s32 encodedDjinn, s32 page,
                                                 s32 *pageCount) {
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

  // Attach the rising/falling stat arrow to the preview window captured here:
  // a 16x16 sprite on layer 240 at pixel (x, y) of the window, in a VRAM
  // cache entry of its own that the window's sprite list frees (kind 1).
  void DjinnMenu_DrawStatArrow(s32 x, s32 y, s32 rising) {
    struct WindowSprite *output = WindowSprite_AcquireFree();
    struct PreviewSprite *entry;
    u32 zero;
    if (output) {
      output->motion = 1;
      output->kind = WINDOW_SPRITE_ICON;
      // Stored through a cast, the index stays ahead of the attribute word.
      *(s8 *)&output->index = VramCache_AllocEntry(128);
      entry = (struct PreviewSprite *)((u8 *)output + 16);
      output->sentinel = 240;
      output->x = 120;
      output->y = 120;
      // UB: the two stores below write the attribute and tile words through
      // pointer types, which keeps sched2's order of the stores around them;
      // UBFIX writes the unions' words.
#ifdef UBFIX
      entry->attributes.word = 0x40000400;
      zero = 0;
      entry->tile.value = zero;
#else
      *(struct Window **)&entry->attributes = (struct Window *)0x40000400;
      zero = 0;
      *(void **)&entry->tile = (void *)zero;
#endif
      entry->attributes.bits.x = win->x * 8 + x;
      entry->attributes.bits.y = win->y * 8 + y;
      /* FAKEMATCH(dead): advancing and restoring the captured pointer keeps
       * the window chain in r9; without the pair, 8 of 80 instructions of
       * the nested DjinnMenu_DrawStatArrow differ in tbs-en. */
      win++;
      win--;
      entry->tile.bits.index = VramCache_Reload(
          (u8)output->index,
          (s32)(rising ? gDjinnMenu_ArrowUpTiles : gDjinnMenu_ArrowDownTiles));
      WindowSprite_AppendToList((struct WindowSpriteList *)win, output);
    }
  }

  unit = Unit_GetFar(member);
  if (win != 0)
    Window_Close(win, 1);

  if (page == 0)
    win = Window_Create(0, 8, 21, 11, 6);
  else
    win = Window_Create(0, 5, 21, 14, 6);
  if (win == 0)
    return 0;

  // Save the unit before the temporary Djinn toggle changes its class/stats.
  // Fourteen widened characters fit well inside the 64-halfword text buffer.
  // With a valid refreshed old list, class rebuilding can remove at most
  // sixteen actions, including any equipment actions displaced by the class.
  // Its new 32-slot list and these lost entries fit the 48-entry allocation.
  numberText = Alloc(64 * sizeof(*numberText));
  savedUnit = Alloc(sizeof(*savedUnit));
  changedAbilities =
      AllocEwram(DJINN_PREVIEW_ABILITY_CAPACITY * sizeof(*changedAbilities));
  Iwram_CopyWords(savedUnit, unit, sizeof(*savedUnit));

  element = (encodedDjinn >> 8) & 15;
  djinnIndex = encodedDjinn & 255;
  if (Djinn_IsSetFar(member, element, djinnIndex) != 0)
    Djinn_PutOnStandbyFar(member, element, djinnIndex);
  else
    Djinn_SetFar(member, element, djinnIndex);
  Unit_CalculateFar(member);

  abilityCount = BattleDjinnMenu_ListChangedDjinn(
      savedUnit->psynergy, unit->psynergy, changedAbilities, &gained, &lost);
  // Five abilities a page; a page past the list shows the last.
  pages = ((abilityCount - 1) / 5) + 1;
  *pageCount = pages;
  if (page * 5 - 5 >= abilityCount)
    page = pages;

  // The first page compares stats; the remaining pages compare Psynergy.
  if (page == 0) {
    s32 label = MsgPreviewStatLabel;

    Text_DrawMessageAsTiles(label, win, 0, 8);
    Text_DrawMessageAsTiles(label + 1, win, 0, 16);
    Text_DrawMessageAsTiles(label + 2, win, 0, 24);
    Text_DrawMessageAsTiles(label + 3, win, 0, 32);
    Text_DrawMessageAsTiles(label + 4, win, 0, 40);
    Text_DrawMessageAsTiles(label + 5, win, 0, 48);

    // The old values, by the labels; the new ones go at column 11 below.
    oldHp = savedUnit->hp;
    String_FormatNumberToHalfwords(numberText, oldHp);
    Text_DrawWideStringInWindow(numberText + 7, win, DJINN_PREVIEW_VITALS_X, 1);
    oldPp = savedUnit->pp;
    String_FormatNumberToHalfwords(numberText, oldPp);
    Text_DrawWideStringInWindow(numberText + 7, win, DJINN_PREVIEW_VITALS_X, 2);
    oldAtk = savedUnit->atk;
    String_FormatNumberToHalfwords(numberText, oldAtk);
    Text_DrawWideStringInWindow(numberText + DJINN_PREVIEW_STAT_DIGIT_OFFSET,
                                win, DJINN_PREVIEW_STAT_X, 3);
    oldDef = savedUnit->def;
    String_FormatNumberToHalfwords(numberText, oldDef);
    Text_DrawWideStringInWindow(numberText + DJINN_PREVIEW_STAT_DIGIT_OFFSET,
                                win, DJINN_PREVIEW_STAT_X, 4);
    oldAgi = savedUnit->agi;
    String_FormatNumberToHalfwords(numberText, oldAgi);
    Text_DrawWideStringInWindow(numberText + DJINN_PREVIEW_STAT_DIGIT_OFFSET,
                                win, DJINN_PREVIEW_STAT_X, 5);
    String_FormatNumberToHalfwords(numberText, savedUnit->luck);
    Text_DrawWideStringInWindow(numberText + 8, win, 5, 6);

    Window_DrawDividerLine(win, 0, 8, 19, 8);
    // "R: Psynergy Gained" stands out in colour 2 when the Psynergy changes.
    if (gained != 0 || lost != 0)
      Text_SetColor(2);
    Text_DrawMessageAsTiles(MsgBattleAdditionalPsynergyHelp, win,
                            DJINN_PREVIEW_HELP_X, 64);
    Text_SetColor(15);
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
        Window_SetTilemapEntry(win, 0x5001 + ability->element, 15,
                               visibleRow * 2, 0);

      // Its element icon, range icon, icon, name and PP cost.
      rangePattern = ability->range;
      if (rangePattern == ABILITY_RANGE_ALL)
        rangePattern = 11;
      else
        rangePattern--;
      Window_DrawThreeTileColumn(win, 16, visibleRow * 2, rangePattern, 0);

      WindowSprite_CreateAbilityIcon(win, 0, visibleRow * 16 - 4,
                                     changedAbilities[i] & ACTION_ID_MASK);
      if ((changedAbilities[i] & DJINN_PREVIEW_ABILITY_GAINED) != 0)
        Text_SetColor(4);
      else if ((changedAbilities[i] & DJINN_PREVIEW_ABILITY_LOST) != 0)
        Text_SetColor(2);
      else
        Text_SetColor(15);

      Text_DrawMessageAsTiles((changedAbilities[i] & ACTION_ID_MASK) +
                                  MsgAbilityName,
                              win, 16, visibleRow * 16);
      Window_SetTilemapEntry(win, 0xf01f, 11, visibleRow * 2, 0);
      Window_SetTilemapEntry(win, PSYNERGY_COST_SECOND_TILE, 12, visibleRow * 2,
                             0);
      Text_DrawNumberInWindow(Ability_Get(changedAbilities[i])->ppCost, 2, win,
                              104, visibleRow * 16);
    }

    if (gained != 0) {
      Text_SetColor(4);
      Text_DrawMessageAsTiles(MsgPsynergyGained, win, 32, 80);
      summaryRows = 1;
    }
    if (lost != 0) {
      Text_SetColor(2);
      Text_DrawMessageAsTiles(MsgPsynergyLost, win, 32, summaryRows * 8 + 80);
      summaryRows++;
    }
    if (summaryRows == 0)
      Text_DrawMessageAsTiles(MsgPsynergyUnchanged, win, 32, 80);
    Text_SetColor(15);
    Text_SetColor(15);
    Window_DrawDividerLine(win, 0, 10, 19, 10);
  }

  if (page == 0) {
    Text_DrawMessageAsTiles(MsgClassName + savedUnit->classId, win, 0, 0);
    Text_DrawMessageAsTiles(MsgClassName + unit->classId, win, 80, 0);
    // The arrow between the classes, flipped (0x400): tile 0x328 blinks
    // when the class changes, 0x329 stays (gDjinnMenu_BlinkTile).
    if (savedUnit->classId != unit->classId)
      Window_SetTilemapEntry(win, 0xf728, 9, 0, page);
    else
      Window_SetTilemapEntry(win, 0xf729, 9, 0, page);

    String_FormatNumberToHalfwords(numberText, unit->hp);
    Text_DrawWideStringInWindow(numberText + 7, win, 11, 1);
    if (unit->hp != oldHp) {
      rising = 0;
      if (unit->hp > oldHp)
        rising = 1;
      DjinnMenu_DrawStatArrow(80, 14, rising);
    }

    String_FormatNumberToHalfwords(numberText, unit->pp);
    Text_DrawWideStringInWindow(numberText + 7, win, 11, 2);
    if (unit->pp != oldPp) {
      rising = 0;
      if (unit->pp > oldPp)
        rising = 1;
      DjinnMenu_DrawStatArrow(80, 22, rising);
    }

    String_FormatNumberToHalfwords(numberText, unit->atk);
    Text_DrawWideStringInWindow(numberText + 7, win, 11, 3);
    if (unit->atk != oldAtk) {
      rising = 0;
      if (unit->atk > oldAtk)
        rising = 1;
      DjinnMenu_DrawStatArrow(80, 30, rising);
    }

    String_FormatNumberToHalfwords(numberText, unit->def);
    Text_DrawWideStringInWindow(numberText + 7, win, 11, 4);
    if (unit->def != oldDef) {
      rising = 0;
      if (unit->def > oldDef)
        rising = 1;
      DjinnMenu_DrawStatArrow(80, 38, rising);
    }

    String_FormatNumberToHalfwords(numberText, unit->agi);
    Text_DrawWideStringInWindow(numberText + 7, win, 11, 5);
    if (unit->agi != oldAgi) {
      rising = 0;
      if (unit->agi > oldAgi)
        rising = 1;
      DjinnMenu_DrawStatArrow(80, 46, rising);
    }

    String_FormatNumberToHalfwords(numberText, unit->luck);
    Text_DrawWideStringInWindow(numberText + 8, win, 12, 6);
    if (unit->luck != savedUnit->luck) {
      rising = 0;
      if (unit->luck > savedUnit->luck)
        rising = 1;
      DjinnMenu_DrawStatArrow(80, 54, rising);
    }
  }

  // Previewing must not change the member's Djinn, class or battle stats.
  Iwram_CopyWords(unit, savedUnit, sizeof(*savedUnit));
  Free(changedAbilities);
  Free(savedUnit);
  Free(numberText);
  return win;
}
