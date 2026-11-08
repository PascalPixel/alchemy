/// @file
/// Previews a djinni change on a member's status, the twin of The Broken
/// Seal's DjinnMenu_DrawStatPreview: the unit is saved, the change applied and
/// the unit recalculated; page 0 draws the stats with an arrow beside each one
/// that changes, later pages the Psynergy gained and lost; then the saved unit
/// is copied back.

#include "battle_unit.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "types.h"
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
extern char MsgKey0d0e[], MsgClassName[], MsgAbilityNothing[], MsgPsynergyPp[],
    MsgKey10d3[], MsgKey10d4[], MsgKey10d9[];

struct BattleUnit *Unit_Get(s32 owner);
void Unit_CalculateFar(s32 owner);
void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
struct BattleAct *BattleAction_Get(s32 actionId);
s32 Djinn_AddToOwnerFar(s32 owner, s32 element, s32 index);
s32 Djinn_ActivateFar(s32 owner, s32 element, s32 index);
s32 Djinn_DeactivateFar(s32 owner, s32 element, s32 index);
void Text_DrawNumberAtOffsetFar(s32 value, s32 width, s32 window, s32 x, s32 y);
void Text_DrawNumberInWindowFar(s32 value, s32 width, s32 window, s32 x, s32 y);
void Text_DrawStringAtOffsetFar(s32 text, s32 window, s32 x, s32 y);
void Text_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Window_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 mode);
void Window_DrawDividerLineFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void Icon_CreateWithLoadedResource(s32 window, s32 x, s32 y, s32 action);
void Func_08104ba8(s32 window, s32 x, s32 y, s32 variant);
s32 Text_SetColorFar(s32 colour);
void SideObject_CreateFar(s32 owner, s32 a, s32 side, s32 window, s32 b, s32 c);
s32 Func_08101860(struct OwnerActionSlot *a, struct OwnerActionSlot *b,
                  u16 *out, s32 *firstCount, s32 *secondCount);

s32 Func_08103218(s32 window, s32 x, s32 y, s32 ownerId, s32 removePairSlot,
                  s32 addPairSlot, s32 previewMode, s32 page, s32 side) {
  // One constant for its whole life: the djinn tile loop starts from it.
  s32 firstElement = 0;
  // One constant for its whole life: the closing copy's size.
  s32 size;
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
  originalUnit = Heap_Alloc(sizeof(struct BattleUnit));
  Iwram_CopyWords(originalUnit, unit, sizeof(struct BattleUnit));

  if (page == 0) {
    if (previewMode == 3) {
      Text_DrawNumberInWindowFar(unit->maxHp, 4, window, x * 8 + 72,
                                 y * 8 + 56);
      Text_DrawNumberInWindowFar(unit->maxPp, 4, window, x * 8 + 72,
                                 y * 8 + 64);
      Text_DrawNumberInWindowFar(unit->hp, 4, window, x * 8 + 32, y * 8 + 56);
      Text_DrawNumberInWindowFar(unit->pp, 4, window, x * 8 + 32, y * 8 + 64);
      Text_DrawStringAtOffsetFar((s32)gDjinnMenu_TextSlash, window, x * 8 + 64,
                                 y * 8 + 56);
      Text_DrawStringAtOffsetFar((s32)gDjinnMenu_TextSlash, window, x * 8 + 64,
                                 y * 8 + 64);
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
    Text_DrawNumberInWindowFar(unit->level, 2, window, x * 8 + 88, y * 8 + 16);
    label = (s32)MsgKey0d0e;
    Text_DrawCharacterAtOffsetFar(label, window, x * 8, y * 8 + 56);
    Text_DrawCharacterAtOffsetFar(label + 1, window, x * 8, y * 8 + 64);
    Text_DrawCharacterAtOffsetFar(label + 2, window, x * 8, y * 8 + 72);
    Text_DrawCharacterAtOffsetFar(label + 3, window, x * 8, y * 8 + 80);
    Text_DrawCharacterAtOffsetFar(label + 4, window, x * 8, y * 8 + 88);
    Text_DrawCharacterAtOffsetFar(label + 5, window, x * 8, y * 8 + 96);
    Text_DrawCharacterAtOffsetFar((s32)MsgClassName + originalUnit->classIndex,
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
      for (i = firstElement; i < 4; i++) {
        s32 tileColumn;
        s32 tile = 0x5001 + i;

        tileColumn = column + i * 2;
        Window_SetTilemapEntryFar(window, tile, tileColumn, y + 5, 0);
        tileColumn++;
        Window_SetTilemapEntryFar(window, unit->djinnActiveCounts[i] + 0xf030,
                                  tileColumn, y + 5, 0);
      }
      // An empty once-only block: it ends the tile loop's block before the
      // stats.
      do {
      } while (0);
      if (unit->hp != originalUnit->hp) {
        Text_DrawNumberInWindowFar(unit->hp, 4, window, x * 8 + 72, y * 8 + 56);
        if (unit->hp > originalUnit->hp)
          Func_08104ba8(window, x * 8 + 62, y * 8 + 56, 0);
        else
          Func_08104ba8(window, x * 8 + 62, y * 8 + 56, 1);
      }
      if (unit->pp != originalUnit->pp) {
        Text_DrawNumberInWindowFar(unit->pp, 4, window, x * 8 + 72, y * 8 + 64);
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
    count = Func_08101860(originalUnit->actionSlots, unit->actionSlots, actions,
                          &gained, &lost);
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
      Text_DrawCharacterAtOffsetFar((s32)MsgAbilityNothing +
                                        (actions[first] & 0x3fff),
                                    window, x * 8 + 16, (y + line * 2) * 8 + 8);
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
        Text_DrawCharacterAtOffsetFar((s32)MsgKey10d3, window, x * 8,
                                      y * 8 + 88);
        lines = 1;
      }
      if (lost) {
        Text_SetColorFar(2);
        Text_DrawCharacterAtOffsetFar((s32)MsgKey10d4, window, x * 8,
                                      (y + lines) * 8 + 88);
        lines++;
      }
      if (lines == 0)
        Text_DrawCharacterAtOffsetFar((s32)MsgKey10d9, window, x * 8,
                                      y * 8 + 88);
      Text_SetColorFar(15);
      Window_DrawDividerLineFar(window, 0, 11, 13, 11);
    }
    ((struct WindowWork *)Ram_HeapSlots->windowTiles)->dirty = 1;
  }

  size = sizeof(struct BattleUnit);
  if (page == 0)
    SideObject_CreateFar(ownerId, 0, side, window, 0, 0);
  Iwram_CopyWords(unit, originalUnit, size);
  Heap_Release(originalUnit);
  return 1;
}
