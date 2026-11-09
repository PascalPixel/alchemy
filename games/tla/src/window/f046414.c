#include "types.h"
#include "edition.h"
#include "window.h"
#include "uiwindow.h"
#include "iwram_call.h"

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

extern u8 MsgAbilityNothing[];
extern u8 MsgClassName[];
extern u8 MsgKey0d0d[];
extern u8 MsgKey0d0e[];
extern u8 MsgKey10d3[];
extern u8 MsgKey10d4[];
extern u8 MsgKey10d9[];

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

struct PreviewAbility {
  u8 unk00[2];
  u8 element; ///< 4 for none
  u8 unk03[5];
  u8 range; ///< 255 for everyone
  u8 ppCost;
};

// Text colours (Func_08041f70).
#define PREVIEW_RED 2
#define PREVIEW_YELLOW 4
#define PREVIEW_WHITE 15

extern const u8 gDjinnMenu_ArrowUpTiles[];
extern const u8 gDjinnMenu_ArrowDownTiles[];

struct PreviewUnit *Unit_Get(s32 member);
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
void *Heap_AllocEwram(s32 size);
s32 Djinn_IsActiveFar(s32 member, s32 element, s32 index);
void Djinn_DeactivateFar(s32 member, s32 element, s32 index);
void Djinn_ActivateFar(s32 member, s32 element, s32 index);
void Unit_CalculateFar(s32 member);
s32 Func_08046284(u16 *oldList, u16 *newList, u16 *changed, s32 *gained,
                  s32 *lost);
void Text_DrawCharacterAtOffset(s32 message, struct Window *window, s32 x,
                                s32 y);
s32 String_FormatNumberToHalfwords(s16 *out, s32 value);
// Text_RenderWideStringInWindow is called with no prototype in scope, as The
// Broken Seal's window functions are from its menu object: an implicit int
// call sets r0, which is what orders each call's arguments.
void Window_DrawDividerLine(struct Window *window, s32 x, s32 y, s32 width,
                            s32 height);
void Func_08041f70(s32 color);
struct PreviewAbility *BattleAction_Get(s32 id);
void Window_SetTilemapEntry(struct Window *window, s32 entry, s32 x, s32 y,
                            s32 flags);
void Text_DrawNumberInWindow(s32 value, s32 digits, struct Window *window,
                             s32 x, s32 y);
s32 VramCache_AllocEntry(s32 size);
s32 VramCache_Reload(s32 entry, const void *tiles);

// The sprite cell at WindowSprite.sprite: a link, then the OAM words.
struct ArrowSprite {
  u32 link;
  union {
    u32 word;
    struct {
      unsigned y : 8;
      unsigned flags : 8;
      unsigned x : 9;
      unsigned high : 7;
    } bits;
  } attributes;
  union {
    u32 value;
    struct {
      unsigned index : 10;
      unsigned high : 6;
    } bits;
  } tile;
};

struct Window *Func_080464dc(struct Window *window, s32 member,
                             s32 encodedDjinn, s32 page, s32 *pageCount) {
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
  Iwram_CopyWords(savedUnit, unit, sizeof(*savedUnit));

  element = (encodedDjinn >> 8) & 15;
  djinnIndex = encodedDjinn & 255;
  if (Djinn_IsActiveFar(member, element, djinnIndex) != 0)
    Djinn_DeactivateFar(member, element, djinnIndex);
  else
    Djinn_ActivateFar(member, element, djinnIndex);
  Unit_CalculateFar(member);

  abilityCount = Func_08046284(savedUnit->psynergy, unit->psynergy,
                               changedAbilities, &gained, &lost);
  pages = ((abilityCount - 1) / 5) + 1;
  *pageCount = pages;
  if (page * 5 - 5 >= abilityCount)
    page = pages;

  if (page == 0) {
    s32 label = (s32)MsgKey0d0e;

    Text_DrawCharacterAtOffset(label, window, 0, 8);
    Text_DrawCharacterAtOffset(label + 1, window, 0, 16);
    Text_DrawCharacterAtOffset(label + 2, window, 0, 24);
    Text_DrawCharacterAtOffset(label + 3, window, 0, 32);
    Text_DrawCharacterAtOffset(label + 4, window, 0, 40);
    Text_DrawCharacterAtOffset(label + 5, window, 0, 48);

    oldHp = savedUnit->hp;
    String_FormatNumberToHalfwords(numberText, oldHp);
    Text_RenderWideStringInWindow(numberText + 7, window, OLD_VALUE_X, 1);
    oldPp = savedUnit->pp;
    String_FormatNumberToHalfwords(numberText, oldPp);
    Text_RenderWideStringInWindow(numberText + 7, window, OLD_VALUE_X, 2);
    oldAtk = savedUnit->atk;
    String_FormatNumberToHalfwords(numberText, oldAtk);
    Text_RenderWideStringInWindow(numberText + 7, window, OLD_VALUE_X, 3);
    oldDef = savedUnit->def;
    String_FormatNumberToHalfwords(numberText, oldDef);
    Text_RenderWideStringInWindow(numberText + OLD_DEFENSE_AGILITY_TEXT, window,
                                  OLD_DEFENSE_AGILITY_X, 4);
    oldAgi = savedUnit->agi;
    String_FormatNumberToHalfwords(numberText, oldAgi);
    Text_RenderWideStringInWindow(numberText + OLD_DEFENSE_AGILITY_TEXT, window,
                                  OLD_DEFENSE_AGILITY_X, 5);
    String_FormatNumberToHalfwords(numberText, savedUnit->luck);
    Text_RenderWideStringInWindow(numberText + 8, window, OLD_VALUE_X + 1, 6);

    Window_DrawDividerLine(window, 0, 8, 19, 8);
    if (gained != 0 || lost != 0)
      Func_08041f70(PREVIEW_RED);
    Text_DrawCharacterAtOffset((s32)MsgKey0d0d, window, PSYNERGY_PROMPT_X, 64);
    Func_08041f70(PREVIEW_WHITE);
  }

  if (page > 0) {
    summaryRows = 0;
    visibleRows = abilityCount;
    if (abilityCount > 4)
      visibleRows = 5;
    i = page * 5 - 5;
    for (visibleRow = 0; visibleRow < visibleRows && i < abilityCount;
         visibleRow++, i++) {
      ability = BattleAction_Get(changedAbilities[i]);
      if (ability->element != 4)
        Window_SetTilemapEntry(window, 0x5001 + ability->element, 15,
                               visibleRow * 2, 0);
      rangePattern = ability->range;
      if (rangePattern == 255)
        rangePattern = 11;
      else
        rangePattern--;
      Window_DrawThreeTileColumn(window, 16, visibleRow * 2, rangePattern, 0);
      WindowSprite_CreateFromResourceSlot((struct RenderInput *)window, 0,
                                          visibleRow * 16 - 4,
                                          changedAbilities[i] & 0x3fff);
      if (changedAbilities[i] & 0x8000)
        Func_08041f70(PREVIEW_YELLOW);
      else if (changedAbilities[i] & 0x4000)
        Func_08041f70(PREVIEW_RED);
      else
        Func_08041f70(PREVIEW_WHITE);
      Text_DrawCharacterAtOffset((changedAbilities[i] & 0x3fff) +
                                     (s32)MsgAbilityNothing,
                                 window, 16, visibleRow * 16);
      Window_SetTilemapEntry(window, WINDOW_TILE_PP_LABEL, 11, visibleRow * 2,
                             0);
      Window_SetTilemapEntry(window, WINDOW_TILE_PP_LABEL_END, 12,
                             visibleRow * 2, 0);
      Text_DrawNumberInWindow(BattleAction_Get(changedAbilities[i])->ppCost, 2,
                              window, 104, visibleRow * 16);
    }
    if (gained != 0) {
      Func_08041f70(PREVIEW_YELLOW);
      Text_DrawCharacterAtOffset((s32)MsgKey10d3, window, TOTAL_LABEL_X, 80);
      summaryRows = 1;
    }
    if (lost != 0) {
      Func_08041f70(PREVIEW_RED);
      Text_DrawCharacterAtOffset((s32)MsgKey10d4, window, TOTAL_LABEL_X,
                                 summaryRows * 8 + 80);
      summaryRows++;
    }
    if (summaryRows == 0)
      Text_DrawCharacterAtOffset((s32)MsgKey10d9, window, TOTAL_LABEL_X, 80);
    Func_08041f70(PREVIEW_WHITE);
    Func_08041f70(PREVIEW_WHITE);
    Window_DrawDividerLine(window, 0, 10, 19, 10);
  }

  if (page == 0) {
    Text_DrawCharacterAtOffset((s32)MsgClassName + savedUnit->classId, window,
                               0, 0);
    Text_DrawCharacterAtOffset((s32)MsgClassName + unit->classId, window, 80,
                               0);
    if (savedUnit->classId != unit->classId)
      Window_SetTilemapEntry(window, 0xf728, 9, 0, page);
    else
      Window_SetTilemapEntry(window, 0xf729, 9, 0, page);

    String_FormatNumberToHalfwords(numberText, unit->hp);
    Text_RenderWideStringInWindow(numberText + 7, window, 11, 1);
    if (unit->hp != oldHp) {
      rising = 0;
      if (unit->hp > oldHp)
        rising = 1;
      Func_08046414(80, 14, rising);
    }
    String_FormatNumberToHalfwords(numberText, unit->pp);
    Text_RenderWideStringInWindow(numberText + 7, window, 11, 2);
    if (unit->pp != oldPp) {
      rising = 0;
      if (unit->pp > oldPp)
        rising = 1;
      Func_08046414(80, 22, rising);
    }
    String_FormatNumberToHalfwords(numberText, unit->atk);
    Text_RenderWideStringInWindow(numberText + 7, window, 11, 3);
    if (unit->atk != oldAtk) {
      rising = 0;
      if (unit->atk > oldAtk)
        rising = 1;
      Func_08046414(80, 30, rising);
    }
    String_FormatNumberToHalfwords(numberText, unit->def);
    Text_RenderWideStringInWindow(numberText + 7, window, 11, 4);
    if (unit->def != oldDef) {
      rising = 0;
      if (unit->def > oldDef)
        rising = 1;
      Func_08046414(80, 38, rising);
    }
    String_FormatNumberToHalfwords(numberText, unit->agi);
    Text_RenderWideStringInWindow(numberText + 7, window, 11, 5);
    if (unit->agi != oldAgi) {
      rising = 0;
      if (unit->agi > oldAgi)
        rising = 1;
      Func_08046414(80, 46, rising);
    }
    String_FormatNumberToHalfwords(numberText, unit->luck);
    Text_RenderWideStringInWindow(numberText + 8, window, 12, 6);
    if (unit->luck != savedUnit->luck) {
      rising = 0;
      if (unit->luck > savedUnit->luck)
        rising = 1;
      Func_08046414(80, 54, rising);
    }
  }

  Iwram_CopyWords(unit, savedUnit, sizeof(*savedUnit));
  Heap_Release(changedAbilities);
  Heap_Release(savedUnit);
  Heap_Release(numberText);
  return window;
}
