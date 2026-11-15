/// @file
/// A turn's actions: the party's commands, the enemies' or the link
/// opponent's, and their action order.
// Part of battle.c.

#include "battle_internal.h"
#include "fixed_math.h"
#include "module/battle/battle.h"
#include "gamepak/far_call/window.h"
#include "module/battle/presentation.h"
#include "macros.h"

/// Turns the battle camera to the view the commands are chosen in: the focus 5
/// pixels up, a quarter turn of yaw, a slight downward pitch, 750 pixels away.
void BattleCamera_SetCommandView(s32 pos) {
  struct BattleCamera *state = Ram_HeapSlots->cameraWork;
  struct BattlePresentationTransition *secondary =
      Ram_HeapSlots->battleBackground;
  struct FixedPointPosition local;
  u32 result;
  s32 one;

  state->focus[1] = px(5);
  state->focus[0] = 0;
  state->focus[2] = 0;
  secondary->targetYaw = TRIG_TURN / 4;
  state->yaw = TRIG_TURN / 4;
  state->pitch = TRIG_TURN - TRIG_TURN * 3 / 64;
  state->followPosition = 0;
  state->distance = px(750);
  state->followEye = 0;

  Transform_Reset();
  SceneTransform_ApplyPosition(&state->focus[0]);
  Transform_ApplyYaw((s16)state->yaw);
  Transform_ApplyPitch((s16)state->pitch);

  SET_POSITION(&local, 0, 0, state->distance);
  Iwram_TransformVector(&local, (s32 *)state);
  one = 1;

  result = Iwram_RatioMulQ14(px(969), Q16_ONE * 3 / 4);
  Projection_Set(0, result, px(1938));

  gCameraSceneParameters[4] = pos + 120;
  secondary->active = one;
  BattleCamera_SetRange(px(120), px(118 - pos), 0, 8 << 8, Q16_ONE * 2);
  secondary->flag = one;
  secondary->active = 0;
}

#define Unit_CalculateFar ((void (*)(u16))Unit_CalculateFar)

/// Ends every unit's guard (Defend and the like) and recalculates its stats.
/// Clears every unit's defendLevel.
void BattleUnit_ResetGuardLevels(void) {
  u16 ids[14];
  s32 count;
  s32 index;

  count = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, (u16 *)ids);
  for (index = 0; index < count; index++) {
    struct Unit *actor;

    actor = Unit_Get(ids[index]);
    actor->defendLevel = 0;
    Unit_CalculateFar(ids[index]);
  }
}

/// Rolls whether the party gets away: 50 percent, 20 more for each failed
/// attempt, and 5 for each level the party's average is above the enemies'. A
/// party that struck first always gets away, and no one from a battle that
/// forbids it. Returns 1 when the party gets away.
s32 BattleFlee_CheckSuccess(void) {
  s32 escaped;
  s16 livingUnits[UNIT_MAX];
  s32 livingCount;
  s32 levelTotal;
  s32 unitIndex;
  s32 chance;
  struct BattleWork *escapeState;

  escaped = 0;
  escapeState = Ram_HeapSlots->battleWork;
  if (escapeState->firstStrike == FIRST_STRIKE_PARTY) {
    escaped = 1;
  } else {
    levelTotal = 0;
    chance = 5000 + (escapeState->fleeFailures * 2000);
    livingCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, livingUnits);
    for (unitIndex = 0; unitIndex < livingCount; unitIndex++) {
      levelTotal +=
          ((struct Unit *)Unit_Get((s32)livingUnits[unitIndex]))->level;
    }
    chance += __divsi3(levelTotal * 500, livingCount);
    livingCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, livingUnits);
    levelTotal = 0;
    for (unitIndex = 0; unitIndex < livingCount; unitIndex++) {
      levelTotal +=
          ((struct Unit *)Unit_Get((s32)livingUnits[unitIndex]))->level;
    }
    chance -= __divsi3(levelTotal * 500, livingCount);
    if ((chance > 0) &&
        // A roll from 0 to 9999 against the chance.
        ((u32)((u32)(10000 * Random16()) >> 16) < (u32)chance)) {
      escaped = 1;
    }
    escapeState->fleeFailures++;
  }
  if (gMainRoutineWork.battleType == BATTLE_TYPE_NO_ESCAPE) {
    escaped = 0;
  }
  return escaped;
}

/// Builds the party's commands for the turn into entries and returns how many
/// there are, or -1 when none could be chosen.
s32 Battle_BuildUnitActions(struct BattleActionRecord *entries) {
  void *buffer = Heap_AllocEwram(17);
  s32 count = Battle_CollectPartyCommandsFar(entries, buffer, 0);
  s32 result = -1;

  if (count >= 0)
    result = count;
  Heap_Release(buffer);
  return result;
}

/// The enemies' actions for the turn, in a shuffled order; none act when the
/// party struck first. Returns how many there are.
s32 Battle_BuildEnemyActions(struct BattleActionRecord *entries) {
  u16 unitIds[14];
  s32 entryCount = 0;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 unitCount;
  s32 shuffleOrUnitIndex;

  if (battle->firstStrike == FIRST_STRIKE_PARTY)
    return 0;

  unitCount = BattleUnit_ListLiving(2, unitIds);
  if (unitCount == 0)
    return 0;

  for (shuffleOrUnitIndex = 0; shuffleOrUnitIndex < 32; shuffleOrUnitIndex++) {
    u32 first = (unitCount * Random16()) >> 16;
    u32 second = (unitCount * Random16()) >> 16;
    s32 swap = unitIds[first];
    unitIds[first] = unitIds[second];
    unitIds[second] = swap;
  }

  if (battle->firstStrike == FIRST_STRIKE_ENEMIES) {
    s32 limit = ((Random16() * 5) >> 16) + 1;

    if (limit <= 1)
      limit = 2;
    if (limit < unitCount)
      unitCount = limit;
  }

  for (shuffleOrUnitIndex = 0; shuffleOrUnitIndex < unitCount;
       shuffleOrUnitIndex++) {
    s32 unitId = unitIds[shuffleOrUnitIndex];
    struct Unit *unit = Unit_Get(unitId);
    s32 copyIndex;

    for (copyIndex = 0; copyIndex < unit->actionsPerTurn; copyIndex++) {
      struct BattleActionRecord *entry = &entries[entryCount];
      s32 agility;

      entry->unitId = unitId;
      agility = unit->agi;
      entry->priority = agility;
      entry->unk0e = copyIndex;
      if (unit->actionsPerTurn == 4) {
        if (copyIndex == 0)
          ;
        else if (copyIndex == 1)
          entry->priority = (s16)agility * 5 / 6;
        else if (copyIndex == 2)
          entry->priority = (s16)agility * 4 / 6;
        else
          entry->priority = (s16)agility / 2;
      } else if (unit->actionsPerTurn == 3) {
        if (copyIndex == 0)
          ;
        else if (copyIndex == 1)
          entry->priority = (s16)agility * 3 / 4;
        else
          entry->priority = (s16)agility / 2;
      } else if (copyIndex != 0) {
        entry->priority = (s16)agility / 2;
      }

      if (unit->sleep != 0 || unit->stun != 0) {
        entry->command = 8;
        entry->parameter = 0;
        entry->target = 0x100;
      } else {
        BattleAI_ChooseAction(entry, 0);
      }

      entryCount++;

      if (battle->firstStrike == FIRST_STRIKE_ENEMIES)
        break;
    }
  }

  return entryCount;
}

/// Swaps two actions of the turn's order.
static inline void BattleAction_Swap(struct BattleActionRecord *a,
                                     struct BattleActionRecord *b) {
  struct BattleActionRecord temporary;

  Iwram_CopyWordsEntry(&temporary, a, sizeof(struct BattleActionRecord));
  Iwram_CopyWordsEntry(a, b, sizeof(struct BattleActionRecord));
  Iwram_CopyWordsEntry(b, &temporary, sizeof(struct BattleActionRecord));
}

/// Sorts the actions by descending priority, after raising the priority of
/// the Djinn with effects 46, 47, 53, 72, 74, 76, 79 and 88 by 10000.
void BattleActionOrder_Sort(struct BattleActionRecord *entries, s32 count) {
  s32 i;
  s32 j;
  s32 swapped;

  for (i = 0; i < count; i++) {
    struct BattleActionRecord *entry = &entries[i];

    if (entry->command == 5) {
      struct BattleAct *action;

      Unit_Get(entry->unitId);
      action = Ability_Get(Djinn_GetAbilityIdFar((entry->parameter) >> 8 & 15,
                                                 entry->parameter & 0xff));
      if (action->effect == 72 || action->effect == 76 ||
          action->effect == 79 || action->effect == 74 ||
          action->effect == 46 || action->effect == 47 ||
          action->effect == 88 || action->effect == 53) {
        u32 priority = entry->priority;

        entry->priority = priority + 10000;
      }
    }
  }

  do {
    swapped = 0;
    for (j = count - 1; j > 0; j--) {
      if ((s16)entries[j].priority > (s16)entries[j - 1].priority) {
        BattleAction_Swap(&entries[j], &entries[j - 1]);
        swapped++;
      }
    }
  } while (swapped != 0);
}

/// Link battle: exchanges the turn's actions with the other console and appends
/// the opponent's, their units and targets seen from this side. Returns how
/// many were received, or -1 when the exchange fails.
s32 Battle_AppendLinkedActions(struct BattleActionRecord *actions, s32 count) {
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 result = 0;
  s32 allocationSize = (u32)(count * 16 + 19) / 20 * 20;
  struct BattleLinkedActionState *state =
      (struct BattleLinkedActionState *)Heap_AllocEwram(40);
  s32 index;

  s32 LinkBattle_SendActions(void) {
    s32 timeout;
    s32 disconnected;
    s32 status = Serial_StartSend(state, 20);

    disconnected = 0;
    timeout = 300;
    if (status == -1)
      return status;
    while (Serial_GetActiveTransfers()) {
      wait(1);
      if (--timeout < 0)
        return -1;
      if ((gLinkStatus & 3) != 3) {
        if (++disconnected > 24)
          return -1;
      } else {
        disconnected = 0;
      }
    }
    if (allocationSize) {
      status = Serial_StartSend(actions, allocationSize);
      if (status == -1)
        return status;
      while (Serial_GetActiveTransfers()) {
        wait(1);
        if (--timeout < 0)
          return -1;
        if ((gLinkStatus & 3) != 3) {
          if (++disconnected > 24)
            return -1;
        } else {
          disconnected = 0;
        }
      }
    }
    return 0;
  }

  s32 LinkBattle_ReceiveActions(void) {
    s32 timeout;
    s32 disconnected;
    s32 status = Serial_StartReceive(state);

    disconnected = 0;
    timeout = 300;
    if (status == -1)
      return status;
    while (Serial_GetActiveTransfers()) {
      if (gSerialReceivedSize > 20)
        return -1;
      wait(1);
      if (--timeout < 0)
        return -1;
      if ((gLinkStatus & 3) != 3) {
        if (++disconnected > 24)
          return -1;
      } else {
        disconnected = 0;
      }
    }
    if (gSerialReceivedSize != 20)
      return -1;
    result = state->count;
    if (state->count) {
      status = Serial_StartReceive(actions + count);
      if (status == -1)
        return status;
      while (Serial_GetActiveTransfers()) {
        if (gSerialReceivedSize > (u32)(result * 16 + 19) / 20 * 20)
          return -1;
        wait(1);
        if (--timeout < 0)
          return -1;
        if ((gLinkStatus & 3) != 3) {
          if (++disconnected > 24)
            return -1;
        } else {
          disconnected = 0;
        }
      }
      if (gSerialReceivedSize != (u32)(result * 16 + 19) / 20 * 20)
        return -1;
    }
    return 0;
  }

  for (index = 0; index < count; index++) {
    struct BattleActionRecord *action = &actions[index];

    action->slot = battle->opponentSlots[action->unitId];

    if (battle->linkSide == 0) {
      if (action->priority & 1)
        action->priority++;
    } else {
      action->priority |= 1;
    }
  }

  if (battle->linkPaused == 0) {
    if (battle->linkSide == 0) {
      u32 saved;

      state->count = count;
      state->check = Battle_RandomFar();
      IRQ_LOCK(saved);
      gBattleRandomSeed = state->seed = gRandomSeed;
      IRQ_UNLOCK(saved);
      if (LinkBattle_SendActions() < 0 || LinkBattle_ReceiveActions() < 0)
        goto fail;
      result = state->count;
    } else {
      if (LinkBattle_ReceiveActions() < 0)
        goto fail;
      result = state->count;
      state->count = count;
      if (LinkBattle_SendActions() < 0)
        goto fail;
      if (Battle_RandomFar() != state->check)
        goto fail;
      gBattleRandomSeed = state->seed;
    }

    for (index = 0; index < result; index++) {
      struct BattleActionRecord *action = &actions[count + index];

      action->unitId = action->slot;
      action->target = action->target ^ 0x80;
    }

    Heap_Release(state);
    return result;
  }

fail:
  LinkBattle_ResetTransferState();
  Serial_RemoveIrqHandlers();
  Heap_Release(state);
  return -1;
}

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
      color = ((u16 *)BG_PLTT)[idx];
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
  struct Unit *unit;
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
  GameFlag_Set(FLAG_CHOOSING_COMMANDS);
  mode = &work->firstStrike;
  BattleCamera_SetCommandView(0);
  BattleMessage_CloseFar();

  // Ally commands (unavailable when enemies strike first).
  if (*mode != FIRST_STRIKE_ENEMIES) {
    cnt = Battle_BuildUnitActions(acts);
    if (cnt < 0)
      goto finish;
    // A failed escape gives enemies the same advantage as striking first.
    if (cnt != 0 && acts[0].command == COMMAND_FLEE &&
        BattleFlee_CheckSuccess() == 0)
      *mode = FIRST_STRIKE_ENEMIES;
  } else {
    cnt = 0;
  }

  // Link opponent or enemy actions.
  if (Ram_HeapSlots->battleWork->isLink != 0) {
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
    cnt += Battle_BuildEnemyActions(acts + cnt);
  }

  // Sort by priority and set guards for Defend (3 and 7) first.
  BattleActionOrder_Sort(acts, cnt);
  if (cnt > 0) {
    struct BattleActionRecord *act = acts;

    i = cnt;
    do {
      if (act->command == 3 || act->command == 7) {
        unit = Unit_Get(act->unitId);
        unit->defendLevel = act->command == 3 ? 1 : 3;
      }
      act++;
      i--;
    } while (i != 0);
  }

finish:
  GameFlag_Clear(FLAG_CHOOSING_COMMANDS);
  BattleCamera_SetDefaultView();
  Ram_HeapSlots->battleBackground->targetYaw = YAW_PARTY_ACTION;
  return cnt;
}

/// Returns the unit ID at a formation position.
/// slot: low 4 bits are the position; 0x80 selects the enemy formation.
s16 Battle_GetUnitAtPlace(s32 slot) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;

  if ((slot & 0x80) != 0)
    return work->enemyIds[slot & 0xf];
  return work->partyUnits[slot & 0xf];
}

/// Returns a unit's formation position, or -1 if absent.
/// Allies use 0x100 | position; enemies use 0x180 | position.
/// id: unit ID (0-7 allies, all others enemies).
s32 Battle_FindUnitPlace(u32 id) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  s32 i;
  s16 unit;

  if (id <= ALLY_MAX) {
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
