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
void Text_DrawNumberAtOffsetFar(s32 value, s32 width, s32 window, s32 x,
                                  s32 y);
void Text_DrawNumberInWindowFar(s32 value, s32 width, s32 window, s32 x,
                                  s32 y);
void Text_DrawStringAtOffsetFar(s32 text, s32 window, s32 x, s32 y);
void Text_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Window_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 mode);
void Window_DrawDividerLineFar(s32 window, s32 x, s32 y, s32 width,
                                 s32 height);
void Icon_CreateWithLoadedResource(s32 window, s32 x, s32 y, s32 action);
void Func_08104ba8(s32 window, s32 x, s32 y, s32 variant);
s32 Text_SetColorFar(s32 colour);
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
      Text_DrawNumberInWindowFar(unit->maxHp, 4, window, x * 8 + 72,
                                   y * 8 + 56);
      Text_DrawNumberInWindowFar(unit->maxPp, 4, window, x * 8 + 72,
                                   y * 8 + 64);
      Text_DrawNumberInWindowFar(unit->hp, 4, window, x * 8 + 32, y * 8 + 56);
      Text_DrawNumberInWindowFar(unit->pp, 4, window, x * 8 + 32, y * 8 + 64);
      Text_DrawStringAtOffsetFar((s32)gDjinnMenu_TextSlash, window,
                                   x * 8 + 64, y * 8 + 56);
      Text_DrawStringAtOffsetFar((s32)gDjinnMenu_TextSlash, window,
                                   x * 8 + 64, y * 8 + 64);
    } else {
      Text_DrawNumberInWindowFar(unit->hp, 4, window, x * 8 + 32, y * 8 + 56);
      Text_DrawNumberInWindowFar(unit->pp, 4, window, x * 8 + 32, y * 8 + 64);
    }
    Text_DrawNumberInWindowFar(unit->atk, 3, window, x * 8 + 48, y * 8 + 72);
    Text_DrawNumberInWindowFar(unit->def, 3, window, x * 8 + 48, y * 8 + 80);
    Text_DrawNumberInWindowFar(unit->agi, 3, window, x * 8 + 48, y * 8 + 88);
    Text_DrawNumberInWindowFar(unit->luck, 2, window, x * 8 + 56, y * 8 + 96);
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

    Text_DrawStringAtOffsetFar((s32)unit->name, window, x * 8 + 40, y * 8);
    Text_DrawStringAtOffsetFar((s32)gDjinnMenu_TextLevel, window, x * 8 + 40,
                                 y * 8 + 16);
    Text_DrawNumberInWindowFar(unit->level, 2, window, x * 8 + 88,
                                 y * 8 + 16);
    label = (s32)MsgPreviewStatLabel;
    Text_DrawCharacterAtOffsetFar(label, window, x * 8, y * 8 + 56);
    Text_DrawCharacterAtOffsetFar(label + 1, window, x * 8, y * 8 + 64);
    Text_DrawCharacterAtOffsetFar(label + 2, window, x * 8, y * 8 + 72);
    Text_DrawCharacterAtOffsetFar(label + 3, window, x * 8, y * 8 + 80);
    Text_DrawCharacterAtOffsetFar(label + 4, window, x * 8, y * 8 + 88);
    Text_DrawCharacterAtOffsetFar(label + 5, window, x * 8, y * 8 + 96);
    Text_DrawCharacterAtOffsetFar((s32)MsgClassName +
                                        originalUnit->classIndex,
                                    window, x * 8, y * 8 + 32);
  }

  {
    s8 i = 0;

    if (page == 0) {
      s32 column;

      if (originalUnit->classIndex != unit->classIndex) {
        Text_DrawCharacterAtOffsetFar((s32)MsgClassName + unit->classIndex,
                                        window, x * 8, y * 8 + 48);
        Window_SetTilemapEntryFar(window, 0xf296, x + 2, 5, 0);
      }
      column = x;
      if (originalUnit->classIndex != unit->classIndex)
        column += 5;
      for (i = 0; i < 4; i++) {
        Window_SetTilemapEntryFar(window, 0x5001 + i, column + i * 2, y + 5,
                                    0);
        Window_SetTilemapEntryFar(window, unit->djinnActiveCounts[i] + 0xf030,
                                    column + i * 2 + 1, y + 5, 0);
      }
      if (unit->hp != originalUnit->hp) {
        Text_DrawNumberInWindowFar(unit->hp, 4, window, x * 8 + 72,
                                     y * 8 + 56);
        if (unit->hp > originalUnit->hp)
          Func_08104ba8(window, x * 8 + 62, y * 8 + 56, 0);
        else
          Func_08104ba8(window, x * 8 + 62, y * 8 + 56, 1);
      }
      if (unit->pp != originalUnit->pp) {
        Text_DrawNumberInWindowFar(unit->pp, 4, window, x * 8 + 72,
                                     y * 8 + 64);
        if (unit->pp > originalUnit->pp)
          Func_08104ba8(window, x * 8 + 62, y * 8 + 64, 0);
        else
          Func_08104ba8(window, x * 8 + 62, y * 8 + 64, 1);
      }
      if (unit->atk != originalUnit->atk) {
        Text_DrawNumberInWindowFar(unit->atk, 4, window, x * 8 + 72,
                                     y * 8 + 72);
        if (unit->atk > originalUnit->atk)
          Func_08104ba8(window, x * 8 + 70, y * 8 + 72, 0);
        else
          Func_08104ba8(window, x * 8 + 70, y * 8 + 72, 1);
      }
      if (unit->def != originalUnit->def) {
        Text_DrawNumberInWindowFar(unit->def, 4, window, x * 8 + 72,
                                     y * 8 + 80);
        if (unit->def > originalUnit->def)
          Func_08104ba8(window, x * 8 + 70, y * 8 + 80, 0);
        else
          Func_08104ba8(window, x * 8 + 70, y * 8 + 80, 1);
      }
      if (unit->agi != originalUnit->agi) {
        Text_DrawNumberInWindowFar(unit->agi, 4, window, x * 8 + 72,
                                     y * 8 + 88);
        if (unit->agi > originalUnit->agi)
          Func_08104ba8(window, x * 8 + 70, y * 8 + 88, 0);
        else
          Func_08104ba8(window, x * 8 + 70, y * 8 + 88, 1);
      }
      if (unit->luck != originalUnit->luck) {
        Text_DrawNumberInWindowFar(unit->luck, 2, window, x * 8 + 88,
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
      Icon_CreateWithLoadedResource(window, x * 8, (y + line * 2) * 8 + 4,
                                      actions[first] & 0x3fff);
      if (actions[first] & 0x8000)
        Text_SetColorFar(4);
      else if (actions[first] & 0x4000)
        Text_SetColorFar(2);
      else
        Text_SetColorFar(15);
      Text_DrawCharacterAtOffsetFar(
          (s32)MsgAbilityName + (actions[first] & 0x3fff), window, x * 8 + 16,
          (y + line * 2) * 8 + 8);
      Text_DrawNumberAtOffsetFar(BattleAction_Get(actions[first])->ppCost, 2,
                                   window, x * 8 + 88, (y + line * 2) * 8 + 8);
      line++;
    }
    Text_SetColorFar(15);
    Text_DrawCharacterAtOffsetFar((s32)MsgPsynergyPp, window, x * 8 + 88,
                                    y * 8);
    if (previewMode != 3) {
      s32 lines;

      lines = 0;
      if (gained) {
        Text_SetColorFar(4);
        Text_DrawCharacterAtOffsetFar((s32)MsgPsynergyGained, window, x * 8,
                                        y * 8 + 88);
        lines = 1;
      }
      if (lost) {
        Text_SetColorFar(2);
        Text_DrawCharacterAtOffsetFar((s32)MsgPsynergyLost, window, x * 8,
                                        (y + lines) * 8 + 88);
        lines++;
      }
      if (lines == 0)
        Text_DrawCharacterAtOffsetFar((s32)MsgPsynergyUnchanged, window,
                                        x * 8, y * 8 + 88);
      Text_SetColorFar(15);
      Window_DrawDividerLineFar(window, 0, 11, 13, 11);
    }
    ((struct WindowWork *)Ram_HeapSlots->windowTiles)->dirty = 1;
  }

  if (page == 0)
    SideObject_CreateFar(ownerId, 0, side, window, 0, 0);
  Iwram_CopyWords(unit, originalUnit, sizeof(struct BattleUnit));
  MemFree(originalUnit);
  return 1;
}
