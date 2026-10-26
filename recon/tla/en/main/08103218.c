// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 57 of 906 in tla-en. First difference: this issues `lsls r3, r3, #18` earlier
// (row 18) than the native does (row 20).
// First draft of Func_08103218: draws a member's status with a djinni change
// applied, the twin of ☀️'s DjinnMenu_DrawStatPreview
// (games/tbs/src/menu/menu/djinn.c): the unit is saved, the change applied
// and measured, its differences drawn with arrows (Func_08104ba8) or, past
// page 0, its Psynergy pages, and the saved unit copied back. The messages
// take the twin's names (03342, 02915, 01447, 04124, 04307, 04308 and 04313
// in the English catalog, still unnamed there). Differ 81 of 906: the
// element tile loop's s8 counter takes r5 where the native keeps it in r6
// (with one copy more here), and the rest is loads and stores the native
// issues a step earlier. The block that opens with the twin's i = 0 keeps
// the second page test, and with it x * 8 and y * 8 recomputed in each
// stat's block (649 with the two page-0 blocks one test apart).
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_unit.h"
#include "window.h"

struct DjinnMenuWork {
  u8 unk000[0x16c];
  u16 pairEntries[2];
  u8 unk170[0x258 - 0x170];
  u8 pairIndex[2];
  u8 pairElement[2];
};

extern const char gDjinnMenu_TextSlash[];
extern const char gDjinnMenu_TextLevel[];
extern char MsgPreviewStatLabel[], MsgClassName[], MsgAbilityName[],
    MsgPsynergyPp[], MsgPsynergyGained[], MsgPsynergyLost[],
    MsgPsynergyUnchanged[];

struct BattleUnit *Unit_Get(s32 owner);
void Unit_CalculateFar(s32 owner);
void *MemAlloc(s32 size);
void MemFree(void *block);
struct BattleAct *BattleAction_Get(s32 actionId);
s32 Djinn_AddToOwnerFar(s32 owner, s32 element, s32 index);
s32 Djinn_ActivateFar(s32 owner, s32 element, s32 index);
s32 Djinn_DeactivateFar(s32 owner, s32 element, s32 index);
void UiText_DrawNumberAtOffsetFar(s32 value, s32 width, s32 window, s32 x,
                                  s32 y);
void UiText_DrawNumberInWindowFar(s32 value, s32 width, s32 window, s32 x,
                                  s32 y);
void UiText_DrawStringAtOffsetFar(s32 text, s32 window, s32 x, s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 mode);
void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 y, s32 width,
                                 s32 height);
void UiIcon_CreateWithLoadedResource(s32 window, s32 x, s32 y, s32 action);
void Func_08104ba8(s32 window, s32 x, s32 y, s32 variant);
s32 UiWork_SetParamNibbleFar(s32 colour);
void SideObject_CreateFar(s32 owner, s32 a, s32 side, s32 window, s32 b, s32 c);
s32 OwnerAction_DiffSlots(struct OwnerActionSlot *a, struct OwnerActionSlot *b,
                          u16 *out, s32 *firstCount, s32 *secondCount);

s32 Func_08103218(s32 window, s32 x, s32 y, s32 ownerId, s32 removePairSlot,
                  s32 addPairSlot, s32 previewMode, s32 page, s32 side) {
  struct BattleUnit *unit;
  struct BattleUnit *originalUnit;
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

  unit = Unit_Get(ownerId);
  menu = Ram_HeapSlots->menuRuntime;
  giveElement = menu->pairElement[removePairSlot];
  giveIndex = menu->pairIndex[removePairSlot];
  giveSet = menu->pairEntries[removePairSlot] & 0x8000;
  takeElement = menu->pairElement[addPairSlot];
  takeIndex = menu->pairIndex[addPairSlot];
  takeSet = menu->pairEntries[addPairSlot] & 0x8000;
  originalUnit = MemAlloc(sizeof(struct BattleUnit));
  Iwram_CopyWords(originalUnit, unit, sizeof(struct BattleUnit));

  if (page == 0) {
    if (previewMode == 3) {
      UiText_DrawNumberInWindowFar(unit->maxHp, 4, window, x * 8 + 72,
                                   y * 8 + 56);
      UiText_DrawNumberInWindowFar(unit->maxPp, 4, window, x * 8 + 72,
                                   y * 8 + 64);
      UiText_DrawNumberInWindowFar(unit->hp, 4, window, x * 8 + 32, y * 8 + 56);
      UiText_DrawNumberInWindowFar(unit->pp, 4, window, x * 8 + 32, y * 8 + 64);
      UiText_DrawStringAtOffsetFar((s32)gDjinnMenu_TextSlash, window,
                                   x * 8 + 64, y * 8 + 56);
      UiText_DrawStringAtOffsetFar((s32)gDjinnMenu_TextSlash, window,
                                   x * 8 + 64, y * 8 + 64);
    } else {
      UiText_DrawNumberInWindowFar(unit->hp, 4, window, x * 8 + 32, y * 8 + 56);
      UiText_DrawNumberInWindowFar(unit->pp, 4, window, x * 8 + 32, y * 8 + 64);
    }
    UiText_DrawNumberInWindowFar(unit->atk, 3, window, x * 8 + 48, y * 8 + 72);
    UiText_DrawNumberInWindowFar(unit->def, 3, window, x * 8 + 48, y * 8 + 80);
    UiText_DrawNumberInWindowFar(unit->agi, 3, window, x * 8 + 48, y * 8 + 88);
    UiText_DrawNumberInWindowFar(unit->luck, 2, window, x * 8 + 56, y * 8 + 96);
  }

  switch (previewMode) {
  case 0:
    Djinn_AddToOwnerFar(ownerId, takeElement, takeIndex & 31);
    Djinn_ActivateFar(ownerId, takeElement, takeIndex & 31);
    break;
  case 1:
    giveIndex &= 31;
    Djinn_DeactivateFar(ownerId, giveElement, giveIndex);
    break;
  case 2:
    if (giveSet) {
      giveIndex &= 31;
      Djinn_DeactivateFar(ownerId, giveElement, giveIndex);
    }
    Djinn_AddToOwnerFar(ownerId, takeElement, takeIndex & 31);
    if (takeSet)
      Djinn_ActivateFar(ownerId, takeElement, takeIndex & 31);
    break;
  case 4:
    Djinn_AddToOwnerFar(ownerId, takeElement, takeIndex & 31);
    if (takeSet)
      Djinn_ActivateFar(ownerId, takeElement, takeIndex & 31);
    break;
  }

  Unit_CalculateFar(ownerId);
  unit = Unit_Get(ownerId);

  if (page == 0) {
    s32 label;

    UiText_DrawStringAtOffsetFar((s32)unit->name, window, x * 8 + 40, y * 8);
    UiText_DrawStringAtOffsetFar((s32)gDjinnMenu_TextLevel, window, x * 8 + 40,
                                 y * 8 + 16);
    UiText_DrawNumberInWindowFar(unit->level, 2, window, x * 8 + 88,
                                 y * 8 + 16);
    label = (s32)MsgPreviewStatLabel;
    UiText_DrawCharacterAtOffsetFar(label, window, x * 8, y * 8 + 56);
    UiText_DrawCharacterAtOffsetFar(label + 1, window, x * 8, y * 8 + 64);
    UiText_DrawCharacterAtOffsetFar(label + 2, window, x * 8, y * 8 + 72);
    UiText_DrawCharacterAtOffsetFar(label + 3, window, x * 8, y * 8 + 80);
    UiText_DrawCharacterAtOffsetFar(label + 4, window, x * 8, y * 8 + 88);
    UiText_DrawCharacterAtOffsetFar(label + 5, window, x * 8, y * 8 + 96);
    UiText_DrawCharacterAtOffsetFar((s32)MsgClassName +
                                        originalUnit->classIndex,
                                    window, x * 8, y * 8 + 32);
  }

  {
    s8 i = 0;

    if (page == 0) {
      s32 column;

      if (originalUnit->classIndex != unit->classIndex) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgClassName + unit->classIndex,
                                        window, x * 8, y * 8 + 48);
        UiWindow_SetTilemapEntryFar(window, 0xf296, x + 2, 5, 0);
      }
      column = x;
      if (originalUnit->classIndex != unit->classIndex)
        column += 5;
      for (i = 0; i < 4; i++) {
        UiWindow_SetTilemapEntryFar(window, 0x5001 + i, column + i * 2, y + 5,
                                    0);
        UiWindow_SetTilemapEntryFar(window, unit->djinnActiveCounts[i] + 0xf030,
                                    column + i * 2 + 1, y + 5, 0);
      }
      if (unit->hp != originalUnit->hp) {
        UiText_DrawNumberInWindowFar(unit->hp, 4, window, x * 8 + 72,
                                     y * 8 + 56);
        if (unit->hp > originalUnit->hp)
          Func_08104ba8(window, x * 8 + 62, y * 8 + 56, 0);
        else
          Func_08104ba8(window, x * 8 + 62, y * 8 + 56, 1);
      }
      if (unit->pp != originalUnit->pp) {
        UiText_DrawNumberInWindowFar(unit->pp, 4, window, x * 8 + 72,
                                     y * 8 + 64);
        if (unit->pp > originalUnit->pp)
          Func_08104ba8(window, x * 8 + 62, y * 8 + 64, 0);
        else
          Func_08104ba8(window, x * 8 + 62, y * 8 + 64, 1);
      }
      if (unit->atk != originalUnit->atk) {
        UiText_DrawNumberInWindowFar(unit->atk, 4, window, x * 8 + 72,
                                     y * 8 + 72);
        if (unit->atk > originalUnit->atk)
          Func_08104ba8(window, x * 8 + 70, y * 8 + 72, 0);
        else
          Func_08104ba8(window, x * 8 + 70, y * 8 + 72, 1);
      }
      if (unit->def != originalUnit->def) {
        UiText_DrawNumberInWindowFar(unit->def, 4, window, x * 8 + 72,
                                     y * 8 + 80);
        if (unit->def > originalUnit->def)
          Func_08104ba8(window, x * 8 + 70, y * 8 + 80, 0);
        else
          Func_08104ba8(window, x * 8 + 70, y * 8 + 80, 1);
      }
      if (unit->agi != originalUnit->agi) {
        UiText_DrawNumberInWindowFar(unit->agi, 4, window, x * 8 + 72,
                                     y * 8 + 88);
        if (unit->agi > originalUnit->agi)
          Func_08104ba8(window, x * 8 + 70, y * 8 + 88, 0);
        else
          Func_08104ba8(window, x * 8 + 70, y * 8 + 88, 1);
      }
      if (unit->luck != originalUnit->luck) {
        UiText_DrawNumberInWindowFar(unit->luck, 2, window, x * 8 + 88,
                                     y * 8 + 96);
        if (unit->luck > originalUnit->luck)
          Func_08104ba8(window, x * 8 + 70, y * 8 + 96, 0);
        else
          Func_08104ba8(window, x * 8 + 70, y * 8 + 96, 1);
      }
    }
  }

  if (page > 0) {
    s32 rows;
    s32 first;
    s8 count;
    s8 row;
    s8 line;

    rows = 6 - (previewMode != 3);
    first = rows * (page - 1);
    count = OwnerAction_DiffSlots(originalUnit->actionSlots, unit->actionSlots,
                                  actions, &gained, &lost);
    line = 0;
    for (row = 0; first < count && row < rows; row++, first++) {
      UiIcon_CreateWithLoadedResource(window, x * 8, (y + line * 2) * 8 + 4,
                                      actions[first] & 0x3fff);
      if (actions[first] & 0x8000)
        UiWork_SetParamNibbleFar(4);
      else if (actions[first] & 0x4000)
        UiWork_SetParamNibbleFar(2);
      else
        UiWork_SetParamNibbleFar(15);
      UiText_DrawCharacterAtOffsetFar(
          (s32)MsgAbilityName + (actions[first] & 0x3fff), window, x * 8 + 16,
          (y + line * 2) * 8 + 8);
      UiText_DrawNumberAtOffsetFar(BattleAction_Get(actions[first])->ppCost, 2,
                                   window, x * 8 + 88, (y + line * 2) * 8 + 8);
      line++;
    }
    UiWork_SetParamNibbleFar(15);
    UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyPp, window, x * 8 + 88,
                                    y * 8);
    if (previewMode != 3) {
      s32 lines;

      lines = 0;
      if (gained) {
        UiWork_SetParamNibbleFar(4);
        UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyGained, window, x * 8,
                                        y * 8 + 88);
        lines = 1;
      }
      if (lost) {
        UiWork_SetParamNibbleFar(2);
        UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyLost, window, x * 8,
                                        (y + lines) * 8 + 88);
        lines++;
      }
      if (lines == 0)
        UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyUnchanged, window,
                                        x * 8, y * 8 + 88);
      UiWork_SetParamNibbleFar(15);
      UiWindow_DrawDividerLineFar(window, 0, 11, 13, 11);
    }
    ((struct UiRenderWork *)Ram_HeapSlots->windowTiles)->dirty = 1;
  }

  if (page == 0)
    SideObject_CreateFar(ownerId, 0, side, window, 0, 0);
  Iwram_CopyWords(unit, originalUnit, sizeof(struct BattleUnit));
  MemFree(originalUnit);
  return 1;
}
