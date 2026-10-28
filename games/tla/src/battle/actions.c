/// @file
/// Battle: build turn actions and convert formation positions.

#include "types.h"
#include "edition.h"
#include "battle.h"
#include "game.h"
#include "system.h"
#include "battle_escape.h"

#include "battle_work.h"
#include "battle_party.h"
#include "battle_unit.h"
#include "battle_system.h"
#include "battle_display.h"
#include "owner_state.h"

extern struct BattlePresentationTransition *gTransitionWork;

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBitFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);

/// Copies BG palettes 15 to 14 and 5 to 4, adjusting brightness.
/// offset: amount added to each RGB component, clamped to 0-31.
void Palette_CopyWithBrightnessOffset(s32 offset) {
  s32 n;
  s32 paletteNumber;
  s32 col;
  s32 mask;
  s32 idx;

  paletteNumber = 15;
  n = 0;
  mask = 31;
  do {
    for (col = 0; col <= 15; col++) {
      u32 color;
      s32 b;
      s32 g;
      s32 r;
      s32 val;

      idx = paletteNumber * 16 + col;
      color = ((u16 *)0x05000000)[idx];
      b = (color >> 10) & mask;
      g = (color >> 5) & mask;
      r = color & mask;
      b += offset;
      g += offset;
      r += offset;
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
      val = b << 10;
      val |= g << 5;
      val |= r;
      ((s16 *)0x04ffffe0)[idx] = val; // Into the preceding palette.
    }
    n++;
    paletteNumber = 5;
  } while (n <= 1);
}

/// Builds this turn's actions and sorts them by descending priority: the
/// allies' commands and either the link opponent's or the enemies' actions.
/// Returns the action count, or a negative value on cancellation or link
/// failure.
/// acts: action list, up to 20 entries.
s32 Battle_GatherActions(struct BattleActionRecord *acts) {
  struct BattleUnit *unit;
  struct BattleWork *work;
  u8 *mode;
  s32 cnt;
  s32 add;
  s32 i;

  work = Ram_HeapSlots->battleWork;
  {
    struct BattleActionRecord *rec = work->actions;
    u32 n;

    for (n = 0; n < 20; n++) {
      rec[n].unitId = BATTLE_UNIT_LIST_END;
      rec[n].priority = 0x8000; // Lowest priority.
    }
  }

  BattleUnit_ResetGuardLevels();
  Palette_CopyWithBrightnessOffset(8);
  // Flag set while commands are being selected.
  GameFlag_SetBit(363);
  mode = &work->encounterMode;
  Camera_ConfigureScene(0);
  Window_CloseSharedFar();

  // Ally commands (unavailable when enemies strike first).
  if (*mode != 2) {
    cnt = Battle_BuildUnitActions(acts);
    if (cnt < 0)
      goto finish;
    // A failed escape gives enemies the same advantage as striking first.
    if (cnt != 0 && acts[0].command == 99 && BattleEscape_CheckSuccess() == 0)
      *mode = 2;
  } else {
    cnt = 0;
  }

  // Link opponent or enemy actions.
  if (Ram_HeapSlots->battleWork->twoSided != 0) {
    add = Battle_AppendLinkedActions(acts, cnt);
    if (LinkBattle_WaitSync() < 0) {
      cnt = -1;
      goto finish;
    }
    cnt += add;
    if (add < 0) {
      cnt = -1;
      goto finish;
    }
  } else {
    cnt += Battle_BuildOpponentActions(acts + cnt);
  }

  // Sort by priority and set guards for Defend (3 and 7) first.
  BattleQueue_SortByPriority(acts, cnt);
  if (cnt > 0) {
    struct BattleActionRecord *act = acts;

    i = cnt;
    do {
      if (act->command == 3 || act->command == 7) {
        unit = Unit_Get(act->unitId);
        unit->guardLevel = act->command == 3 ? 1 : 3;
      }
      act++;
      i--;
    } while (i != 0);
  }

finish:
  GameFlag_ClearBit(363);
  Camera_InitializeDefaultTransform();
  Ram_HeapSlots->transitionWork->targetYaw = 0x2000;
  return cnt;
}

/// Returns the unit ID at a formation position.
/// slot: low 4 bits are the position; 0x80 selects the enemy formation.
s16 Battle_GetTaggedSlotValue(s32 slot) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;

  if ((slot & 0x80) != 0)
    return work->enemyIds[slot & 0xf];
  return work->partyUnits[slot & 0xf];
}

/// Returns a unit's formation position, or -1 if absent.
/// Allies use 0x100 | position; enemies use 0x180 | position.
/// id: unit ID (0-7 allies, all others enemies).
s32 Battle_FindTaggedSlotByValue(u32 id) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  s32 i;
  s16 unit;

  if (id <= 7) {
    for (i = 0;; i++) {
      unit = work->partyUnits[i];
      if (unit == BATTLE_UNIT_LIST_END)
        return -1;
      if (unit != BATTLE_UNIT_REMOVED && unit == id)
        return i | 0x100;
    }
  }
  for (i = 0;; i++) {
    unit = work->enemyIds[i];
    if (unit == BATTLE_UNIT_LIST_END)
      return -1;
    if (unit != BATTLE_UNIT_REMOVED && unit == id)
      return i | 0x180;
  }
}
