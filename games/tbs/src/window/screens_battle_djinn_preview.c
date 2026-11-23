/// @file
/// The preview of what setting or removing a Djinni changes.
// Part of screens.c.

#include "screens_internal.h"

/// How many bytes of gNumberBuffer are widened: the eleven characters
/// String_FormatDecimal writes, its terminator and the two bytes after.
#define NUMBER_TEXT_WIDE_LENGTH 14

/// A changed stat's arrow, right of its value on window tile row `row` (HP
/// on row 1 to Luck on row 6).
#define PREVIEW_STAT_ARROW_X 80
#define PREVIEW_STAT_ARROW_Y(row) ((row) * 8 + 6)

/// Writes `value` into `out` as String_FormatDecimal's eleven-character,
/// right-aligned decimal text, widened to halfwords for
/// Text_DrawWideStringInWindow. `out` needs room for NUMBER_TEXT_WIDE_LENGTH
/// halfwords.
UNREAD_RESULT String_FormatNumberToHalfwords(s16 *out, s32 value) {
  s16 *destination;
  s32 i;
  u8 *source;

  destination = out;
  String_FormatDecimal(value);
  source = gNumberBuffer;
  for (i = 0; i < NUMBER_TEXT_WIDE_LENGTH; i++) {
    *destination++ = *source++;
  }
}

/// The marks BattleDjinnMenu_ListChangedDjinn gives an ability in its list,
/// above its id: the preview draws a gained one in TEXT_COLOR_YELLOW and a
/// lost one in TEXT_COLOR_RED.
#define DJINN_PREVIEW_ABILITY_GAINED 0x8000 ///< new with the change
#define DJINN_PREVIEW_ABILITY_LOST 0x4000   ///< gone with the change
/// Entries of the change list: the new list's 32 and the lost ones after.
#define DJINN_PREVIEW_ABILITY_CAPACITY 48
/// The abilities a page of the preview lists.
#define DJINN_PREVIEW_ABILITIES_PER_PAGE 5

/// Lists into `out` the abilities usable in battle (ABILITY_USABLE_IN_BATTLE)
/// of the action list `newGrid`, in its order, marking with
/// DJINN_PREVIEW_ABILITY_GAINED those `oldGrid` lacks; then appends those of
/// `oldGrid` that `newGrid` lacks, marked DJINN_PREVIEW_ABILITY_LOST. Returns
/// the entries written, and the gained and lost counts through `gained` and
/// `lost`.
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
  for (i = 0; i < PSYNERGY_SLOTS && newGrid[i].encodedAction != 0; i++) {
    if (Ability_Get(newGrid[i].encodedAction)->flags &
        ABILITY_USABLE_IN_BATTLE) {
      out[count] = newGrid[i].encodedAction & ACTION_ID_MASK;
      count++;
      for (j = 0; j < PSYNERGY_SLOTS; j++) {
        if (((newGrid[i].encodedAction ^ oldGrid[j].encodedAction) &
             ACTION_ID_MASK) == 0)
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
             ACTION_ID_MASK) == 0)
          break;
      }
      if (j == PSYNERGY_SLOTS) {
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

/// Loads the icon of ability `abilityId` into the first free VRAM cache slot
/// and shows it as a 16x16 sprite at pixel (x, y) inside `window`, whose
/// sprite list then owns the slot. Does nothing when the cache is full.
UNREAD_RESULT WindowSprite_CreateAbilityIcon(struct Window *window, s32 x,
                                             s32 y, s32 abilityId) {
  s32 iconSlot;

  iconSlot = VramCache_FindFreeEntry();
  if (iconSlot != VRAM_CACHE_ENTRY_COUNT) {
    Resource_LoadIndexedEntryToBuffer(abilityId, iconSlot);
    // If the sprite pool is full, the creator releases iconSlot itself.
    WindowSprite_Create(iconSlot, OUTPUT_SIZE_16X16, window, x, y);
  }
}

/// Shows what setting or removing Djinni `encodedDjinn` (element << 8 | index)
/// would change for `member`: closes the preview window `win`, opens a new one
/// and returns it, or NULL when none could be opened. Page 0 compares the
/// class and stats, old against new, with an arrow by each stat that rises or
/// falls; pages 1 on list the unit's battle abilities after the change and
/// then those it would lose, and *pageCount is set to their number (at least
/// 1). The Djinni is toggled and the unit recalculated only to measure: the
/// unit is restored from a copy before it returns.
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

  // Draws a rising or falling stat arrow at pixel (x, y) of the preview
  // window: a 16x16 sprite in a VRAM cache entry of its own, which the
  // window's sprite list frees with it (WINDOW_SPRITE_ICON).
  void DjinnMenu_DrawStatArrow(s32 x, s32 y, s32 rising) {
    struct WindowSprite *sprite = WindowSprite_AcquireFree();
    struct PreviewSprite *entry;
    if (sprite) {
      sprite->motion = WINDOW_SPRITE_MOTION_STILL;
      sprite->kind = WINDOW_SPRITE_ICON;
      sprite->index = VramCache_AllocEntry(128);
      entry = (struct PreviewSprite *)sprite->link;
      sprite->depth = OAM_DEPTH_WINDOW;
      sprite->x = 120;
      sprite->y = 120;
      *(u32 *)&entry->attributes =
          OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
      *(u32 *)&entry->tile = 0;
      /* FAKEMATCH(dead): an empty once-only loop after the two words are
       * stored. */
      do {
      } while (0);
      /* FAKEMATCH(dead): advancing and restoring the captured window. */
      win++;
      win--;
      entry->attributes.bits.x = win->x * 8 + x;
      entry->attributes.bits.y = win->y * 8 + y;
      entry->tile.bits.index = VramCache_Reload(
          (u8)sprite->index,
          (s32)(rising ? gDjinnMenu_ArrowUpTiles : gDjinnMenu_ArrowDownTiles));
      WindowSprite_AppendToList(&win->sprites, sprite);
    }
  }

  unit = Unit_GetFar(member);
  if (win != NULL)
    Window_Close(win, 1);

  if (page == 0)
    win = Window_Create(0, 8, 21, 11, MENU_WINDOW_FLAGS);
  else
    win = Window_Create(0, 5, 21, 14, MENU_WINDOW_FLAGS);
  if (win == NULL)
    return NULL;

  numberText = Heap_Alloc(64 * sizeof(*numberText));
  savedUnit = Heap_Alloc(sizeof(*savedUnit));
  changedAbilities = Heap_AllocEwram(DJINN_PREVIEW_ABILITY_CAPACITY *
                                     sizeof(*changedAbilities));
  Iwram_CopyWords(savedUnit, unit, sizeof(*savedUnit));

  element = DJINN_COMMAND_ELEMENT(encodedDjinn);
  djinnIndex = DJINN_COMMAND_INDEX(encodedDjinn);
  if (Djinn_IsSetFar(member, element, djinnIndex) != 0)
    Djinn_PutOnStandbyFar(member, element, djinnIndex);
  else
    Djinn_SetFar(member, element, djinnIndex);
  Unit_CalculateFar(member);

  abilityCount = BattleDjinnMenu_ListChangedDjinn(
      savedUnit->psynergy, unit->psynergy, changedAbilities, &gained, &lost);
  // A page past the list shows the last.
  pages = (abilityCount - 1) / DJINN_PREVIEW_ABILITIES_PER_PAGE + 1;
  *pageCount = pages;
  if ((page - 1) * DJINN_PREVIEW_ABILITIES_PER_PAGE >= abilityCount)
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
    Text_DrawWideStringInWindow(numberText + NUMBER_TEXT_FOUR_PLACES, win,
                                DJINN_PREVIEW_VITALS_X, 1);
    oldPp = savedUnit->pp;
    String_FormatNumberToHalfwords(numberText, oldPp);
    Text_DrawWideStringInWindow(numberText + NUMBER_TEXT_FOUR_PLACES, win,
                                DJINN_PREVIEW_VITALS_X, 2);
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
    Text_DrawWideStringInWindow(numberText + NUMBER_TEXT_THREE_PLACES, win, 5,
                                6);

    Window_DrawDividerLine(win, 0, 8, 19, 8);
    // The prompt for the Psynergy pages, its button and ":Psynergy Gained",
    // stands out in TEXT_COLOR_RED when the Psynergy changes.
    if (gained != 0 || lost != 0)
      Text_SetColor(TEXT_COLOR_RED);
    Text_DrawMessageAsTiles(MsgBattleAdditionalPsynergyHelp, win,
                            DJINN_PREVIEW_HELP_X, 64);
    Text_SetColor(TEXT_COLOR_WHITE);
  }

  if (page > 0) {
    summaryRows = 0;
    visibleRows = abilityCount;
    if (abilityCount >= DJINN_PREVIEW_ABILITIES_PER_PAGE)
      visibleRows = DJINN_PREVIEW_ABILITIES_PER_PAGE;
    i = (page - 1) * DJINN_PREVIEW_ABILITIES_PER_PAGE;
    for (visibleRow = 0; visibleRow < visibleRows && i < abilityCount;
         visibleRow++, i++) {
      ability = Ability_Get(changedAbilities[i]);
      if (ability->element != ELEMENT_NONE)
        Window_SetTilemapEntry(win, LIST_TILE_SUMMON_ELEMENT + ability->element,
                               15, visibleRow * 2, 0);

      // Its element icon, range icon, icon, name and PP cost.
      rangePattern = ability->range;
      if (rangePattern == ABILITY_RANGE_ALL)
        rangePattern = RANGE_ICON_ALL;
      else
        rangePattern--;
      Window_DrawThreeTileColumn(win, 16, visibleRow * 2, rangePattern, 0);

      WindowSprite_CreateAbilityIcon(win, 0, visibleRow * 16 - 4,
                                     changedAbilities[i] & ACTION_ID_MASK);
      if ((changedAbilities[i] & DJINN_PREVIEW_ABILITY_GAINED) != 0)
        Text_SetColor(TEXT_COLOR_YELLOW);
      else if ((changedAbilities[i] & DJINN_PREVIEW_ABILITY_LOST) != 0)
        Text_SetColor(TEXT_COLOR_RED);
      else
        Text_SetColor(TEXT_COLOR_WHITE);

      Text_DrawMessageAsTiles((changedAbilities[i] & ACTION_ID_MASK) +
                                  MsgAbilityName,
                              win, 16, visibleRow * 16);
      Window_SetTilemapEntry(win, PSYNERGY_COST_FIRST_TILE, 11, visibleRow * 2,
                             0);
      Window_SetTilemapEntry(win, PSYNERGY_COST_SECOND_TILE, 12, visibleRow * 2,
                             0);
      Text_DrawNumberInWindow(Ability_Get(changedAbilities[i])->ppCost, 2, win,
                              104, visibleRow * 16);
    }

    if (gained != 0) {
      Text_SetColor(TEXT_COLOR_YELLOW);
      Text_DrawMessageAsTiles(MsgPsynergyGained, win, 32, 80);
      summaryRows = 1;
    }
    if (lost != 0) {
      Text_SetColor(TEXT_COLOR_RED);
      Text_DrawMessageAsTiles(MsgPsynergyLost, win, 32, summaryRows * 8 + 80);
      summaryRows++;
    }
    if (summaryRows == 0)
      Text_DrawMessageAsTiles(MsgPsynergyUnchanged, win, 32, 80);
    Text_SetColor(TEXT_COLOR_WHITE);
    Text_SetColor(TEXT_COLOR_WHITE);
    Window_DrawDividerLine(win, 0, 10, 19, 10);
  }

  if (page == 0) {
    Text_DrawMessageAsTiles(MsgClassName + savedUnit->classId, win, 0, 0);
    Text_DrawMessageAsTiles(MsgClassName + unit->classId, win, 80, 0);
    // The arrow between the classes, flipped: DJINN_MENU_ARROW_TILE blinks
    // when the class changes, the tile after it stays (gDjinnMenu_BlinkTile).
    if (savedUnit->classId != unit->classId)
      Window_SetTilemapEntry(
          win, WINDOW_TILE(DJINN_MENU_ARROW_TILE) | TILE_HFLIP, 9, 0, page);
    else
      Window_SetTilemapEntry(
          win, WINDOW_TILE(DJINN_MENU_ARROW_TILE + 1) | TILE_HFLIP, 9, 0, page);

    String_FormatNumberToHalfwords(numberText, unit->hp);
    Text_DrawWideStringInWindow(numberText + NUMBER_TEXT_FOUR_PLACES, win, 11,
                                1);
    if (unit->hp != oldHp) {
      rising = 0;
      if (unit->hp > oldHp)
        rising = 1;
      DjinnMenu_DrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(1),
                              rising);
    }

    String_FormatNumberToHalfwords(numberText, unit->pp);
    Text_DrawWideStringInWindow(numberText + NUMBER_TEXT_FOUR_PLACES, win, 11,
                                2);
    if (unit->pp != oldPp) {
      rising = 0;
      if (unit->pp > oldPp)
        rising = 1;
      DjinnMenu_DrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(2),
                              rising);
    }

    String_FormatNumberToHalfwords(numberText, unit->atk);
    Text_DrawWideStringInWindow(numberText + NUMBER_TEXT_FOUR_PLACES, win, 11,
                                3);
    if (unit->atk != oldAtk) {
      rising = 0;
      if (unit->atk > oldAtk)
        rising = 1;
      DjinnMenu_DrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(3),
                              rising);
    }

    String_FormatNumberToHalfwords(numberText, unit->def);
    Text_DrawWideStringInWindow(numberText + NUMBER_TEXT_FOUR_PLACES, win, 11,
                                4);
    if (unit->def != oldDef) {
      rising = 0;
      if (unit->def > oldDef)
        rising = 1;
      DjinnMenu_DrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(4),
                              rising);
    }

    String_FormatNumberToHalfwords(numberText, unit->agi);
    Text_DrawWideStringInWindow(numberText + NUMBER_TEXT_FOUR_PLACES, win, 11,
                                5);
    if (unit->agi != oldAgi) {
      rising = 0;
      if (unit->agi > oldAgi)
        rising = 1;
      DjinnMenu_DrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(5),
                              rising);
    }

    String_FormatNumberToHalfwords(numberText, unit->luck);
    Text_DrawWideStringInWindow(numberText + NUMBER_TEXT_THREE_PLACES, win, 12,
                                6);
    if (unit->luck != savedUnit->luck) {
      rising = 0;
      if (unit->luck > savedUnit->luck)
        rising = 1;
      DjinnMenu_DrawStatArrow(PREVIEW_STAT_ARROW_X, PREVIEW_STAT_ARROW_Y(6),
                              rising);
    }
  }

  // Previewing must not change the party member's Djinn, class or battle stats.
  Iwram_CopyWords(unit, savedUnit, sizeof(*savedUnit));
  Heap_Release(changedAbilities);
  Heap_Release(savedUnit);
  Heap_Release(numberText);
  return win;
}
