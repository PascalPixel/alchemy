/// @file
/// An ability classified for target selection, and a reserve member chosen for
/// a party slot.
// Part of window.c.

#include "../menus_internal.h"
#include "gamepak/far_call/battle.h"
#include "battle/motobj.h"

/// Classifies an ability's effect for target selection.
/// 0 other, 1 restore HP, 2 restore PP, 3 cure poison, 4 cure an ailment,
/// 5 revive, 6 cure all ailments.
/// act: ability data.
s32 Battle_ClassifyEntryKind(const struct BattleAct *act) {
  s32 val;
  s32 type;
  s32 kind;
  u8 effect;

  kind = 0;
  type = act->flags & 15;
  if (type == 1) // Restore HP.
    kind = 1;
  if (type == 11) // Restore PP.
    kind = 2;
  val = (s8)act->effect;
  effect = val;
  if (effect == 3) // Cure poison.
    kind = 3;
  if (effect == 4) // Cure an ailment.
    kind = 4;
  if (effect == 64) // Cure all ailments.
    kind = 6;
  if (Battle_IsReviveEffectFar(act->effect) != 0) // Revive.
    kind = 5;
  return kind;
}

// The keys, read through a volatile view: the loop reloads them on each test.
#define INPUT ((volatile struct InputState *)&gInput)

/// Lets the player choose which reserve member takes a party slot in battle,
/// previewing each choice in place: left and right cycle through the reserve,
/// A confirms and swaps the two in gMainRoutineWork.activeOwners, and B puts
/// the slot's member back.
s32 BattleParty_ChooseReserve(s32 slot) {
  u16 owners[6];
  u16 selected[2];
  struct BattleWork *battle;
  s32 count;

  count = BattleParty_PrepareReserveOwnersFar(owners);
  {
    s32 sel;
    s32 redraw;
    s32 result;
    s32 *reserve;
    s32 current;
    s32 newId;
    s32 i;

    sel = 0;
    redraw = 1;
    battle = Ram_HeapSlots->battleWork;
    result = 0;
    reserve = Ram_HeapSlots->unke4[0];
    if (count <= 0)
      return -1;
    slot &= 0xff;
    current = battle->partyUnits[slot];
    Sound_PlayFar(112);
    // Once-only block: the native's code order after the first sound.
    do {
      goto test;
      for (;;) {
        // Japanese also takes A on the first frame, before the first preview
        // has been drawn.
#if TLA_PATCH_1
        if ((INPUT->pressed & 1) && !redraw) {
#else
        if (INPUT->pressed & 1) {
#endif
          if (BattleUnit_ListIdsFar(1, 0) == 0) {
            Sound_PlayFar(114);
            goto test;
          }
          Sound_PlayFar(110);
          reserve[65] = current;
          reserve[66] = slot;
          for (i = 0; i < 8; i++) {
            if (gMainRoutineWork.activeOwners[i] == current)
              gMainRoutineWork.activeOwners[i] = newId;
            else if (gMainRoutineWork.activeOwners[i] == newId)
              gMainRoutineWork.activeOwners[i] = current;
          }
          goto done;
        }
        if (INPUT->repeat & 0x20) {
          sel = (sel + count - 1) % count;
          Sound_PlayFar(112);
          redraw = 1;
        } else if (INPUT->repeat & 0x10) {
          sel = (sel + 1) % count;
          Sound_PlayFar(112);
          redraw = 1;
        }
        if (redraw) {
          struct BattleObj *from;
          struct BattleObj *to;
          struct Actor *object;
          struct Actor *place;

          redraw = 0;
          newId = battle->partyUnits[slot] = owners[sel];
          from = BattleObject_GetFar(current);
          to = BattleObject_GetFar(newId);
          to->anchorX = from->anchorX;
          place = (struct Actor *)from->object;
          object = (struct Actor *)to->object;
          to->anchorZ = from->anchorZ;
          Actor_SetPositionAndResetMotionFar(object, place->x, place->y,
                                             place->z);
          PartyStatus_Draw(Ram_HeapSlots->battleWork->partyStatus);
          BattleObject_PutFar();
          selected[0] = newId;
          selected[1] = 0xff;
          BattleHighlight_SetModesFar(selected, 2);
          PartyStatus_HighlightMembers(selected);
        }
        wait(1);
      test:
        newId = battle->partyUnits[slot] = owners[sel];
        if (INPUT->pressed & 2)
          break;
      }
      battle->partyUnits[slot] = current;
      result = -1;
      Sound_PlayFar(113);
    done:
      selected[0] = 0xff;
      BattleHighlight_SetModesFar(selected, 0);
      PartyStatus_HighlightMembers(selected);
      PartyStatus_Draw(Ram_HeapSlots->battleWork->partyStatus);
      BattleObject_PutFar();
      wait(1);
    } while (0);
    return result;
  }
}

s32 BattleParty_ListLivingUnitsFar(s32 sideMask, u16 *unitIds);

/// The first half of The Broken Seal's Battle_BuildPartyActions: each action
/// of a living party member who is asleep, confused, charmed or stunned is lost
/// (COMMAND_NONE) and written at *cursor, which moves past them, their count
/// going to *forcedCount; every other member's actions are listed in ready to
/// be chosen. Returns how many are listed.
s32 BattleParty_SplitChoosers(u16 *ready, struct BattleActionRecord **cursor,
                              s32 *forcedCount) {
  u16 *unitIds = Heap_AllocEwram(9);
  struct BattleActionRecord *entries = *cursor;
  s32 count = BattleParty_ListLivingUnitsFar(1, unitIds);
  s32 readyCount = 0;
  s32 forced = 0;
  s32 i;
  s32 action;

  for (i = 0; i < count; i++) {
    struct BattleUnit *unit = Unit_Get(unitIds[i]);

    for (action = 0; action < unit->actionEntryCount; action++) {
      if (unit->sleep != 0 || unit->confusion != 0 || unit->charm != 0 ||
          unit->stun != 0) {
        struct BattleActionRecord *entry = &entries[forced];

        entry->unitId = unitIds[i];
        entry->priority = unit->agi;
        entry->command = 8;
        entry->parameter = 0;
        entry->target = 384;
        forced++;
      } else {
        ready[readyCount++] = unitIds[i];
      }
    }
  }
  *forcedCount = forced;
  entries += forced;
  *cursor = entries;
  return readyCount;
}
#undef INPUT
