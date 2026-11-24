/// @file
/// A turn's actions: the party's commands, the enemies' or the link
/// opponent's, and their action order.
// Part of battle.c.

#include "battle_internal.h"
#include "fixed_math.h"
#include "battle/battle.h"
#include "gamepak/far_call/window.h"
#include "battle/presentation.h"
#include "macros.h"

/// The odds of fleeing in hundredths of a percent, against a roll below
/// FLEE_ROLL_RANGE: FLEE_BASE_CHANCE, FLEE_CHANCE_PER_FAILURE more for each
/// failed attempt, and FLEE_CHANCE_PER_LEVEL for each level of the party's
/// average above the enemies'.
enum FleeChance {
  FLEE_ROLL_RANGE = 10000,
  FLEE_BASE_CHANCE = 5000,
  FLEE_CHANCE_PER_FAILURE = 2000,
  FLEE_CHANCE_PER_LEVEL = 500
};

/// The random swaps that shuffle the enemies' order each turn.
#define TURN_ORDER_SWAPS 32

/// An empty action's priority, below every other as the sort reads it (s16).
#define PRIORITY_LOWEST 32768

/// Sets the camera up for the fixed view the battle shows while commands are
/// chosen, raised by verticalOffset pixels.
void BattleCamera_SetCommandView(s32 verticalOffset) {
  struct BattleCamera *camera = Ram_HeapSlots->cameraState;
  struct BattleBackgroundState *view = Ram_HeapSlots->battleBackground;
  struct Point eyeOffset;
  u32 result;

  // The focus 5 pixels up, a quarter turn of yaw, a slight downward pitch, 750
  // pixels away.
  camera->focus[1] = toFIXED(5);
  camera->focus[0] = 0;
  camera->focus[2] = 0;
  view->targetYaw = DEGtoANG(90);
  camera->yaw = DEGtoANG(90);
  camera->pitch = TRIG_TURN - TRIG_TURN * 3 / 64;
  camera->followPosition = 0;
  camera->distance = toFIXED(750);
  camera->followEye = 0;

  Matrix_Initialize();
  Matrix_TranslateVector(&camera->focus[0]);
  Matrix_RotateY((s16)camera->yaw);
  Matrix_RotateX((s16)camera->pitch);

  SET_POSITION(&eyeOffset, 0, 0, camera->distance);
  Iwram_TransformVector(&eyeOffset, (s32 *)camera);

  result = Iwram_RatioMulQ14(toFIXED(969), FIXED_ONE * 3 / 4);
  Projection_Set(0, result, toFIXED(1938));

  // While the view is busy, BattleCamera_SetRange leaves BG1's scroll alone.
  gProjection.centerY = verticalOffset + BATTLE_PROJECTION_CENTER_Y;
  view->busy = 1;
  BattleCamera_SetRange(toFIXED(120), toFIXED(118 - verticalOffset), 0, 8 << 8,
                        FIXED_ONE * 2);
  view->keepRange = 1;
  view->busy = 0;
}

#define Unit_CalculateFar ((void (*)(u16))Unit_CalculateFar)

/// Ends every unit's guard (Defend and the like) and recalculates its stats.
void BattleUnit_ResetGuardLevels(void) {
  u16 unitIds[UNIT_MAX];
  s32 count;
  s32 unitIndex;

  count = BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
  for (unitIndex = 0; unitIndex < count; unitIndex++) {
    struct Unit *unit;

    unit = Unit_Get(unitIds[unitIndex]);
    unit->defendLevel = 0;
    Unit_CalculateFar(unitIds[unitIndex]);
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
  struct BattleState *battle;

  escaped = 0;
  battle = Ram_HeapSlots->battleState;
  if (battle->firstStrike == FIRST_STRIKE_PARTY) {
    escaped = 1;
  } else {
    levelTotal = 0;
    chance =
        FLEE_BASE_CHANCE + (battle->fleeFailures * FLEE_CHANCE_PER_FAILURE);
    livingCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, livingUnits);
    for (unitIndex = 0; unitIndex < livingCount; unitIndex++) {
      levelTotal += Unit_Get((s32)livingUnits[unitIndex])->level;
    }
    chance += levelTotal * FLEE_CHANCE_PER_LEVEL / livingCount;
    livingCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, livingUnits);
    levelTotal = 0;
    for (unitIndex = 0; unitIndex < livingCount; unitIndex++) {
      levelTotal += Unit_Get((s32)livingUnits[unitIndex])->level;
    }
    chance -= levelTotal * FLEE_CHANCE_PER_LEVEL / livingCount;
    if ((chance > 0) &&
        ((u32)((u32)(FLEE_ROLL_RANGE * Random16()) >> 16) < (u32)chance)) {
      escaped = 1;
    }
    battle->fleeFailures++;
  }
  if (gMainRoutineState.battleType == BATTLE_TYPE_NO_ESCAPE) {
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
  u16 unitIds[UNIT_MAX];
  s32 entryCount = 0;
  struct BattleState *battle = Ram_HeapSlots->battleState;
  s32 unitCount;
  s32 i;

  if (battle->firstStrike == FIRST_STRIKE_PARTY)
    return 0;

  unitCount = BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, unitIds);
  if (unitCount == 0)
    return 0;

  for (i = 0; i < TURN_ORDER_SWAPS; i++) {
    u32 first = (unitCount * Random16()) >> 16;
    u32 second = (unitCount * Random16()) >> 16;
    s32 swap = unitIds[first];
    unitIds[first] = unitIds[second];
    unitIds[second] = swap;
  }

  // Striking first, only two to five of the enemies act, each once.
  if (battle->firstStrike == FIRST_STRIKE_ENEMIES) {
    s32 limit = (Random_Below(5)) + 1;

    if (limit <= 1)
      limit = 2;
    if (limit < unitCount)
      unitCount = limit;
  }

  for (i = 0; i < unitCount; i++) {
    s32 unitId = unitIds[i];
    struct Unit *unit = Unit_Get(unitId);
    s32 copyIndex;

    for (copyIndex = 0; copyIndex < unit->actionsPerTurn; copyIndex++) {
      struct BattleActionRecord *entry = &entries[entryCount];
      s32 agility;

      entry->unitId = unitId;
      agility = unit->agility;
      entry->priority = agility;
      entry->turnAction = copyIndex;
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
        entry->command = COMMAND_NONE;
        entry->parameter = 0;
        entry->target = TARGET_NONE_ENEMY;
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

/// Sorts the actions by descending priority, after putting the Djinn that
/// shield, guard or defend ahead of the rest.
void BattleActionOrder_Sort(struct BattleActionRecord *entries, s32 count) {
  s32 i;
  s32 j;
  s32 swapped;

  for (i = 0; i < count; i++) {
    struct BattleActionRecord *entry = &entries[i];

    if (entry->command == COMMAND_DJINN) {
      struct AbilityDefinition *action;

      // DEAD: Unit_Get's result is not used; the ROM keeps the call.
      Unit_Get(entry->unitId);
      action = Ability_Get(
          Djinn_GetAbilityIdFar(DJINN_COMMAND_ELEMENT(entry->parameter),
                                DJINN_COMMAND_INDEX(entry->parameter)));
      // These Djinn act ahead of the turn order.
      if (action->effect == ABILITY_EFFECT_SHIELD ||
          action->effect == ABILITY_EFFECT_BREATH ||
          action->effect == ABILITY_EFFECT_VANISH ||
          action->effect == ABILITY_EFFECT_COUNTER ||
          action->effect == ABILITY_EFFECT_DEFEND ||
          action->effect == ABILITY_EFFECT_TOTAL_DEFENSE ||
          action->effect == ABILITY_EFFECT_GUARD_AURA ||
          action->effect == ABILITY_EFFECT_GROUND) {
        u32 priority = entry->priority;

        entry->priority = priority + PRIORITY_FIRST_IN_TURN;
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

/// The link sends in packets of 20 bytes (the header is one), and a transfer
/// may take 300 frames.
#define LINK_PACKET_BYTES 20
#define LINK_TRANSFER_TIMEOUT 300
/// The bytes of count actions (16 each, a struct BattleActionRecord), in
/// whole packets.
#define LINK_ACTIONS_BYTES(count)                                              \
  ((u32)((count) * 16 + LINK_PACKET_BYTES - 1) / LINK_PACKET_BYTES *           \
   LINK_PACKET_BYTES)

/// Link battle: exchanges the turn's actions with the other console and appends
/// the opponent's, their units and targets seen from this side. Returns how
/// many were received, or -1 when the exchange fails.
s32 Battle_AppendLinkedActions(struct BattleActionRecord *actions, s32 count) {
  struct BattleState *battle = Ram_HeapSlots->battleState;
  s32 result = 0;
  s32 allocationSize = LINK_ACTIONS_BYTES(count);
  struct BattleLinkedActionState *state =
      Heap_AllocEwram(sizeof(struct BattleLinkedActionState));
  s32 index;

  s32 LinkBattle_SendActions(void) {
    s32 timeout;
    s32 disconnected;
    s32 status = Serial_StartSend(state, LINK_PACKET_BYTES);

    disconnected = 0;
    timeout = LINK_TRANSFER_TIMEOUT;
    if (status == -1)
      return status;
    while (Serial_GetActiveTransfers()) {
      wait(1);
      if (--timeout < 0)
        return -1;
      if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
        if (++disconnected > LINK_MAX_MISSED_FRAMES)
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
        if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
          if (++disconnected > LINK_MAX_MISSED_FRAMES)
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
    timeout = LINK_TRANSFER_TIMEOUT;
    if (status == -1)
      return status;
    while (Serial_GetActiveTransfers()) {
      if (gSerialReceivedSize > LINK_PACKET_BYTES)
        return -1;
      wait(1);
      if (--timeout < 0)
        return -1;
      if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
        if (++disconnected > LINK_MAX_MISSED_FRAMES)
          return -1;
      } else {
        disconnected = 0;
      }
    }
    if (gSerialReceivedSize != LINK_PACKET_BYTES)
      return -1;
    result = state->count;
    if (state->count) {
      status = Serial_StartReceive(actions + count);
      if (status == -1)
        return status;
      while (Serial_GetActiveTransfers()) {
        if (gSerialReceivedSize > LINK_ACTIONS_BYTES(result))
          return -1;
        wait(1);
        if (--timeout < 0)
          return -1;
        if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
          if (++disconnected > LINK_MAX_MISSED_FRAMES)
            return -1;
        } else {
          disconnected = 0;
        }
      }
      if (gSerialReceivedSize != LINK_ACTIONS_BYTES(result))
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
      Battle_SetRandomSeed(state->seed = gRandomSeed);
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
      Battle_SetRandomSeed(state->seed);
    }

    for (index = 0; index < result; index++) {
      struct BattleActionRecord *action = &actions[count + index];

      action->unitId = action->slot;
      action->target = action->target ^ PLACE_ENEMY_SIDE;
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

/// Fills BG palettes 14 and 4 with palettes 15 and 5 (the windows' and the
/// basic colours) with offset added to each colour channel, held within 0 to
/// RGB_LEVEL_MAX.
void Palette_CopyWithBrightnessOffset(s32 offset) {
  s32 bank;
  s32 source = WINDOW_PALETTE;
  s32 i;

  // Palettes 15 and 5, each into the palette before it, 14 and 4.
  for (bank = 0; bank < 2; bank++) {
    for (i = 0; i < PALETTE_COLORS; i++) {
      s32 index = source * PALETTE_COLORS + i;
      u32 color = ((u16 *)BG_PLTT)[index];
      s32 blue = RGB_BLUE(color);
      s32 green = RGB_GREEN(color);
      s32 red = RGB_RED(color);
      s32 adjusted;

      blue += offset;
      green += offset;
      red += offset;
      if (blue > RGB_LEVEL_MAX)
        blue = RGB_LEVEL_MAX;
      if (green > RGB_LEVEL_MAX)
        green = RGB_LEVEL_MAX;
      if (red > RGB_LEVEL_MAX)
        red = RGB_LEVEL_MAX;
      if (blue < 0)
        blue = 0;
      if (green < 0)
        green = 0;
      if (red < 0)
        red = 0;
      adjusted = RGB(red, green, blue);
      ((u16 *)BG_PLTT)[index - PALETTE_COLORS] = adjusted;
    }
    source = BASIC_COLORS_PALETTE;
  }
}

/// Builds this turn's actions and sorts them by descending priority: the
/// allies' commands and either the link opponent's or the enemies' actions.
/// Returns the action count, or a negative value on cancellation or link
/// failure.
s32 Battle_GatherActions(struct BattleActionRecord *actions) {
  struct Unit *unit;
  struct BattleState *battle;
  u8 *firstStrike;
  s32 count;
  s32 received;
  s32 i;

  battle = Ram_HeapSlots->battleState;
  {
    struct BattleActionRecord *records = battle->actions;
    u32 n;

    for (n = 0; n < BATTLE_ACTION_CAPACITY; n++) {
      records[n].unitId = BATTLE_UNIT_LIST_END;
      records[n].priority = PRIORITY_LOWEST;
    }
  }

  BattleUnit_ResetGuardLevels();
  Palette_CopyWithBrightnessOffset(8);
  GameFlag_Set(FLAG_CHOOSING_COMMANDS);
  firstStrike = &battle->firstStrike;
  BattleCamera_SetCommandView(0);
  BattleMessage_CloseFar();

  // Ally commands (unavailable when enemies strike first).
  if (*firstStrike != FIRST_STRIKE_ENEMIES) {
    count = Battle_BuildUnitActions(actions);
    if (count < 0)
      goto finish;
    // A failed escape gives enemies the same advantage as striking first.
    if (count != 0 && actions[0].command == COMMAND_FLEE &&
        BattleFlee_CheckSuccess() == 0)
      *firstStrike = FIRST_STRIKE_ENEMIES;
  } else {
    count = 0;
  }

  // Link opponent or enemy actions.
  if (Ram_HeapSlots->battleState->isLink != 0) {
    received = Battle_AppendLinkedActions(actions, count);
    if (LinkBattle_WaitSync() < 0) {
      count = -1;
      goto finish;
    }
    count += received;
    if (received < 0) {
      count = -1;
      goto finish;
    }
  } else {
    count += Battle_BuildEnemyActions(actions + count);
  }

  // Sorted, and every unit that defends this turn guards from its start.
  BattleActionOrder_Sort(actions, count);
  if (count > 0) {
    struct BattleActionRecord *action = actions;

    i = count;
    do {
      if (action->command == COMMAND_DEFEND ||
          action->command == COMMAND_TOTAL_DEFENSE) {
        unit = Unit_Get(action->unitId);
        unit->defendLevel =
            action->command == COMMAND_DEFEND ? DEFEND_NORMAL : DEFEND_TOTAL;
      }
      action++;
      i--;
    } while (i != 0);
  }

finish:
  GameFlag_Clear(FLAG_CHOOSING_COMMANDS);
  BattleCamera_SetDefaultView();
  Ram_HeapSlots->battleBackground->targetYaw = YAW_PARTY_ACTION;
  return count;
}

/// Returns the unit id at a list place, PLACE_ENEMY_SIDE set for the enemies'
/// list.
s32 Battle_GetUnitAtPlace(s32 taggedPlace) {
  struct BattleState *battle = Ram_HeapSlots->battleState;

  if ((taggedPlace & PLACE_ENEMY_SIDE) != 0)
    return battle->enemyIds[taggedPlace & PLACE_INDEX_MASK];
  return battle->partyUnits[taggedPlace & PLACE_INDEX_MASK];
}

/// Returns the list place of a unit, PLACE_PARTY or PLACE_ENEMY with its place,
/// or -1 when it is in neither list.
s32 Battle_FindUnitPlace(u32 unitId) {
  struct BattleState *battle = Ram_HeapSlots->battleState;
  s32 placeIndex;
  s16 listedUnit;

  if (unitId <= ALLY_MAX) {
    for (placeIndex = 0;; placeIndex++) {
      listedUnit = battle->partyUnits[placeIndex];
      if (listedUnit == BATTLE_UNIT_LIST_END)
        return -1;
      if (listedUnit != BATTLE_UNIT_REMOVED && listedUnit == unitId)
        return placeIndex | PLACE_PARTY;
    }
  }
  for (placeIndex = 0;; placeIndex++) {
    listedUnit = battle->enemyIds[placeIndex];
    if (listedUnit == BATTLE_UNIT_LIST_END)
      return -1;
    if (listedUnit != BATTLE_UNIT_REMOVED && listedUnit == unitId)
      return placeIndex | PLACE_ENEMY;
  }
}
