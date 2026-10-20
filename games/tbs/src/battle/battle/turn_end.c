/// @file
/// The end of a turn and of a round: counters, Djinn recovery, poison and
/// the Grim Reaper.
/// Part of battle.c, the battle object, compiled only through it.

#include "../battle_internal.h"

/// Rolls whether a counter of five turns or fewer ends early, using luck, turns
/// and the supplied bias.
s32 BattleUnit_RollCounterEarlyEnd(s32 unitId, s32 turns, s32 bias) {
  struct BattleUnit *unit = UnitGetFar(unitId);

  if (turns <= 5) {
    s32 threshold = ((unit->luck * 3 - turns * 5) + bias) * 0x28f;
    if (threshold >= (BattleRandomFar() & 0xffff))
      return 1;
  }
  return 0;
}

/// Ticks Attack's duration; clears its stage on expiry or an early end of a
/// reduction.
s32 BattleUnit_TickAttackModifier(s32 unitId) {
  struct BattleUnit *unit = UnitGetFar(unitId);

  if (unit->attackTurns != 0) {
    s32 cleared;
    unit->attackTurns--;
    cleared = 0;
    if (unit->attackTurns == 0) {
      unit->attackStage = cleared;
      return 1;
    }
    if (unit->attackStage < 0) {
      if (BattleUnit_RollCounterEarlyEnd(unitId, unit->attackTurns, 30) != 0) {
        unit->attackStage = cleared;
        unit->attackTurns = cleared;
        return 1;
      }
    }
  }
  return 0;
}

/// Decrements a stat change's turn counter. Resets level and returns one when
/// the counter reaches zero, or when BattleUnit_RollCounterEarlyEnd ends a
/// negative-level stat reduction early.
#define DEFINE_TICK_MODIFIER(name, counter, level)                             \
  s32 name(s32 unitId) {                                                       \
    struct BattleUnit *state = UnitGetFar(unitId);                             \
                                                                               \
    if (state->counter != 0) {                                                 \
      state->counter--;                                                        \
      if (state->counter == 0) {                                               \
        state->level = 0;                                                      \
        return 1;                                                              \
      }                                                                        \
      if (state->level < 0 &&                                                  \
          BattleUnit_RollCounterEarlyEnd(unitId, state->counter, 20) != 0) {   \
        state->level = 0;                                                      \
        state->counter = 0;                                                    \
        return 1;                                                              \
      }                                                                        \
    }                                                                          \
    return 0;                                                                  \
  }

/// Ticks Defense duration and clears an expired or prematurely ended reduction.
DEFINE_TICK_MODIFIER(BattleUnit_TickDefenseModifier, defenseTurns, defenseStage)
/// Ticks Resistance duration and clears an expired or prematurely ended
/// reduction.
DEFINE_TICK_MODIFIER(BattleUnit_TickResistanceModifier, resistanceTurns,
                     resistanceStage)

/// Decrements a status condition's turn counter. Returns one when it reaches
/// zero, or when BattleUnit_RollCounterEarlyEnd(bias) ends it early.
#define DEFINE_TICK_STATUS(name, counter, bias)                                \
  s32 name(s32 unitId) {                                                       \
    struct BattleUnit *state = UnitGetFar(unitId);                             \
    if (state->counter != 0) {                                                 \
      state->counter--;                                                        \
      if (state->counter == 0)                                                 \
        return 1;                                                              \
      if (BattleUnit_RollCounterEarlyEnd(unitId, state->counter, bias) != 0) { \
        state->counter = 0;                                                    \
        return 1;                                                              \
      }                                                                        \
    }                                                                          \
    return 0;                                                                  \
  }

/// Ticks delusion and rolls its early end with bias 30.
DEFINE_TICK_STATUS(BattleUnit_TickDelusion, delusion, 30)

/// Ticks the unsigned confusion counter and rolls its early end with bias 60.
s32 BattleUnit_TickConfusion(s32 unitId) {
  struct BattleUnit *unit = UnitGetFar(unitId);
  if ((u8)unit->confusion != 0) {
    (*(u8 *)&unit->confusion)--;
    if (unit->confusion == 0)
      return 1;
    if (BattleUnit_RollCounterEarlyEnd(unitId, (u8)unit->confusion, 60) != 0) {
      unit->confusion = 0;
      return 1;
    }
  }
  return 0;
}

/// Ticks charm and rolls its early end with bias 70.
DEFINE_TICK_STATUS(BattleUnit_TickCharm, charm, 70)
/// Ticks stun and rolls its early end with bias 40.
DEFINE_TICK_STATUS(BattleUnit_TickStun, stun, 40)
/// Ticks sleep and rolls its early end with bias 50.
DEFINE_TICK_STATUS(BattleUnit_TickSleep, sleep, 50)

/// Ticks both packed Psynergy-seal counters; only Bind's low-bit counter may
/// end early.
s32 Battle_AdvanceCounterAndCheckChance(s32 unitId) {
  s32 decremented;
  u8 remaining;
  s32 turns;
  s32 sealCounter;
  struct BattleUnit *unit;

  unit = UnitGetFar(unitId);
  sealCounter = unit->psySeal;
  turns = sealCounter & 0xFF;
  if (turns != 0) {
    if (turns > 7U) {
      turns += 0xF8;
      unit->psySeal = turns;
      sealCounter = turns;
    }
    if (sealCounter & 7) {
      decremented = sealCounter + 0xFF;
      unit->psySeal = decremented;
      sealCounter = decremented;
    }
    remaining = sealCounter;
    if (remaining == 0)
      return 1;
    if (remaining <= 7U &&
        BattleUnit_RollCounterEarlyEnd(unitId, unit->psySeal, 0x1E) != 0) {
      unit->psySeal = 0U;
      return 1;
    }
  }
  return 0;
}

/// Ticks HP-regeneration duration; returns one when it expires.
s32 BattleUnit_TickRefrain(s32 unitId) {
  u8 *turns = &UnitGetFar(unitId)->refrain;
  if (*turns != 0) {
    (*turns)--;
    if (*turns == 0) {
      return 1;
    }
  }
  return 0;
}

/// Ticks reflection duration; returns one when it expires.
s32 BattleUnit_TickReflect(s32 unitId) {
  u8 *turns = (u8 *)&UnitGetFar(unitId)->reflect;
  if (*turns != 0) {
    (*turns)--;
    if (*turns == 0) {
      return 1;
    }
  }
  return 0;
}

/// Ticks Agility's duration and clears its stage on expiry.
s32 BattleUnit_TickAgilityModifier(s32 unitId) {
  struct BattleUnit *unit = UnitGetFar(unitId);
  u8 *turns = &unit->agilityTurns;
  if (*turns != 0) {
    (*turns)--;
    if (*turns == 0) {
      unit->agilityStage = 0;
      return 1;
    }
  }
  return 0;
}

/// Ticks party Djinn recovery for living owners, then sets and removes every
/// expired entry. Returns one when any entry was removed.
s32 DjinnRecovery_Tick(void) {
  struct DjinnRecoveryList *list;
  struct DjinnRecoveryEntry *timedEntry;
  struct DjinnRecoveryEntry *expiredEntry;
  s32 index;
  s32 removed;
  s32 initialCount;

  list = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(0))->list;
  initialCount = list->count;
  index = 0;
  removed = 0;
  if (index < initialCount) {
    timedEntry = list->entries;
    do {
      if (timedEntry->turns > 0 && UnitGetFar(timedEntry->unitId)->hp != 0) {
        timedEntry->turns--;
      }
      index++;
      timedEntry++;
    } while (index < list->count);
  }
  index = 0;
  if (index < list->count) {
    expiredEntry = list->entries;
    do {
      if (expiredEntry->turns == 0) {
        u8 id = expiredEntry->unitId;

        Djinn_ActivateFar(id, expiredEntry->element, expiredEntry->index);
        DjinnRecovery_RemoveFar(id, expiredEntry->element, expiredEntry->index);
        UnitCalcFar(id);
        removed = 1;
      } else {
        expiredEntry++;
        index++;
      }
    } while (index < list->count);
  }
  return removed;
}

/// Advances party Djinn recovery twenty times, setting every expired Djinni.
s32 DjinnRecovery_TickTwenty(void) {
  s32 turns;

  turns = 19;
  do {
    turns--;
    DjinnRecovery_Tick();
  } while (turns >= 0);
  return 0;
}

void Battle_ReservedNoOpF674(void) {}

/// The order the two sides' units take their turn-end effects in, as
/// BATTLE_SIDE_ masks.
struct BattleGroupOrder {
  s32 group[2];
};

/// Advances Djinn recovery on each battle side, applies HP/PP regeneration and
/// announces expired stat changes and conditions in the round's side order.
s32 Battle_ProcessRoundEnd(void) {
  struct BattleWork *battle;
  s32 side;
  s32 sideCount;
  s32 count;
  s32 recoveryIndex;
  struct DjinnRecoveryList *list;
  struct DjinnRecoveryEntry *timedEntry;
  struct DjinnRecoveryEntry *expiredEntry;

  battle = gBattleWork;
  sideCount = (battle->twoSided != 0) + 1;
  for (side = 0; side < sideCount; side++) {
    list =
        &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(side))->list;
    recoveryIndex = 0;
    if (recoveryIndex < list->count) {
      timedEntry = list->entries;
      do {
        if (timedEntry->turns > 0 && BattleObjGet(timedEntry->unitId) != 0 &&
            UnitGetFar(timedEntry->unitId)->hp != 0) {
          timedEntry->turns--;
        }
        recoveryIndex++;
        timedEntry++;
      } while (recoveryIndex < list->count);
    }
    recoveryIndex = 0;
    if (recoveryIndex < list->count) {
      expiredEntry = list->entries;
      do {
        if (expiredEntry->turns == 0) {
          u8 unitId = expiredEntry->unitId;
          s32 element;

          if (BattleObjGet(unitId) != 0) {
            BattleEventRuntime_Reset();
            BattleEventRuntime_SchedulePhase(30);
            BattleEvPush(BATTLE_EVENT_UNIT_NAME, unitId);
            BattleEvPush(BATTLE_EVENT_ABILITY_NAME, expiredEntry->element * 20 +
                                                        expiredEntry->index +
                                                        300);
            BattleEvPush(BATTLE_EVENT_SOUND, SE_175);
            BattleEvPush(BATTLE_EVENT_DRAW_STATUS, 0);
            PUSH_MESSAGE(MsgDjinnSet);
            BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, unitId);
            AudioCommand_PlayFar(SE_212);
            ObjectDispatch_SelectAnimationFar(BattleObjGet(unitId)->object, 3);
            ObjectDispatch_SetAnimationSpeedFar(
                (struct DispatchObject *)BattleObjGet(unitId)->object, 32);
            element = expiredEntry->element;
            Djinn_ActivateFar(unitId, element, expiredEntry->index);
            DjinnRecovery_RemoveFar(unitId, expiredEntry->element,
                                    expiredEntry->index);
            UnitCalcFar(unitId);
            BattleFx_PlayUnitElementEffect(unitId, element, 3, 0);
            BattleEventRuntime_WaitForReady();
          }
        } else {
          expiredEntry++;
          recoveryIndex++;
        }
      } while (recoveryIndex < list->count);
    }
  }
  BattlePresentation_ConfigurePaletteFade(2, gBattleWork->background, 0);
  {
    s32 *groupList;
    struct BattleGroupOrder order = gBattleParty_RoundEndGroupOrder;
    u16 unitIds[14];
    s32 group;
    s32 unitIndex;

    if (battle->twoSided != 0 && battle->linkSide != 0) {
      order.group[0] = 2;
      order.group[1] = 1;
    }
    groupList = order.group;
    for (group = 0; group <= 1; group++) {
      count = BattleParty_ListActorIds(groupList[group], unitIds);
      for (unitIndex = 0; unitIndex < count; unitIndex++) {
        s32 hpAmount;
        s32 ppAmount;
        s32 unitId = unitIds[unitIndex];
        struct BattleUnit *unit = UnitGetFar(unitId);

        if (unit->ready != 0) {
          unit->ready--;
        }
        if (unit->hp != 0) {
          if (unit->hpRegen != 0 && unit->hp != unit->maxHp) {
            hpAmount = unit->hpRegen;
            if (unit->hp + hpAmount > unit->maxHp) {
              hpAmount = unit->maxHp - unit->hp;
            }
            Unit_AddHpFar(unitId, hpAmount);
            UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
            UiWork_PushValueSlotFar(hpAmount, VALUE_NUMBER);
            if (unit->hp == unit->maxHp) {
              UiText_ShowMessageAndWaitCoreFar(MsgHpFull);
            } else {
              UiText_ShowMessageAndWaitCoreFar(MsgHpRecover);
            }
            AudioCommand_PlayFar(SE_175);
            BattlePresentation_WaitForAdvance();
          }
          if (unit->ppRegen != 0 && unit->pp != unit->maxPp) {
            ppAmount = unit->ppRegen;
            if (unit->pp + ppAmount > unit->maxPp) {
              ppAmount = unit->maxPp - unit->pp;
            }
            Unit_AddPpFar(unitId, ppAmount);
            UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
            UiWork_PushValueSlotFar(ppAmount, VALUE_NUMBER);
            if (unit->pp == unit->maxPp) {
              UiText_ShowMessageAndWaitCoreFar(MsgPpFull);
            } else {
              UiText_ShowMessageAndWaitCoreFar(MsgPpRecover);
            }
            AudioCommand_PlayFar(SE_175);
            BattlePresentation_WaitForAdvance();
          }
        }
        if (BattleUnit_TickAgilityModifier(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObjGet(unitId));
          UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgAgiNormal);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickAttackModifier(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObjGet(unitId));
          UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgAtkNormal);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickDefenseModifier(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObjGet(unitId));
          UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgDefNormal);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickResistanceModifier(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObjGet(unitId));
          UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgResNormal);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickDelusion(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObjGet(unitId));
          UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgCureDelusion);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickConfusion(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObjGet(unitId));
          UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgConscious);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickCharm(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObjGet(unitId));
          UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgConscious2);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickStun(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObjGet(unitId));
          UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
          BattlePres_SetActorModeAndAction(unitId);
          UiText_ShowMessageAndWaitCoreFar(MsgCureStun);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickSleep(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObjGet(unitId));
          UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
          BattlePres_SetActorModeAndAction(unitId);
          UiText_ShowMessageAndWaitCoreFar(MsgWakes);
          BattlePresentation_WaitForAdvance();
        }
        if (Battle_AdvanceCounterAndCheckChance(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObjGet(unitId));
          UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgCureSeal);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickRefrain(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObjGet(unitId));
          UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgRefrainEnd);
          BattlePresentation_WaitForAdvance();
        }
        if (BattleUnit_TickReflect(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObjGet(unitId));
          UiWork_PushValueSlotFar(unitId, VALUE_UNIT_NAME);
          UiText_ShowMessageAndWaitCoreFar(MsgReflectEnd);
          BattlePresentation_WaitForAdvance();
        }
      }
    }
  }
}

/// Processes the actor's standby and summoned Djinn, raises summon element
/// power, then applies haunting, poison and the Grim Reaper before
/// recalculating stats.
s32 BattleUnit_ProcessTurnEnd(struct BattlePlan *plan) {
  s32 unitId;
  struct BattleUnit *unit;
  s32 battleContinues;
  s32 counts[4];
  struct DjinnRecoveryList *list;
  s32 strongestElement;
  s32 recoveryTurn;

  unitId = plan->actorId;
  battleContinues = 0;
  unit = UnitGetFar(unitId);
  list = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(
               (unitId < 0 || unitId > 7)))
              ->list;
  for (strongestElement = 0; strongestElement < list->count;
       strongestElement++) {
    if (list->entries[strongestElement].unitId == unitId &&
        list->entries[strongestElement].turns == -1) {
      Djinn_DeactivateFar(unitId, list->entries[strongestElement].element,
                          list->entries[strongestElement].index);
    }
  }
  if (BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, 0) != 0 &&
      BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, 0) != 0) {
    battleContinues = 1;
  }
  {
    s32 i;
    s32 owner;
    struct DjinnRecoveryList *pending;
    pending = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(
                    (unitId < 0 || unitId > 7)))
                   ->list;
    for (i = 0; i < 4; i++) {
      counts[i] = 0;
    }
    for (;;) {
      owner = -1;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].turns == -2) {
          owner = pending->entries[i].unitId;
          break;
        }
      }
      if (owner == -1) {
        break;
      }
      recoveryTurn = -1;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].unitId == owner &&
            pending->entries[i].turns > recoveryTurn) {
          recoveryTurn = pending->entries[i].turns;
        }
      }
      recoveryTurn++;
      if (recoveryTurn <= 1) {
        recoveryTurn = 2;
      }
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].unitId == owner &&
            pending->entries[i].turns == -2) {
          pending->entries[i].turns = recoveryTurn;
          counts[pending->entries[i].element]++;
          recoveryTurn++;
        }
      }
    }
  }
  if (battleContinues != 0) {
    struct BattleUnit *before;
    s32 largestSummonCost;
    s32 i;
    largestSummonCost = 0;
    before = Runtime_BumpAllocateAlternatePool(sizeof(struct BattleUnit));
    Iwram_CopyWords(before, unit, sizeof(struct BattleUnit));
    strongestElement = -1;
    for (i = 0; i <= 3; i++) {
      if (counts[i] > largestSummonCost) {
        largestSummonCost = counts[i];
        strongestElement = i;
      }
    }
    if (strongestElement >= 0 &&
        unit->elementLevels[strongestElement] < largestSummonCost) {
      unit->elementLevels[strongestElement] = largestSummonCost;
    }
    UnitCalcFar(unitId);
    for (i = 0; i <= 3; i++) {
      s32 gain = unit->element[i].power - before->element[i].power;

      if (gain > 0) {
        BattleEventRuntime_Reset();
        BattleEventRuntime_SchedulePhase(25);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, unitId);
        BattleEvPush(BATTLE_EVENT_NUMBER, gain);
        BattleEvPush(BATTLE_EVENT_SOUND, SE_175);
        PUSH_MESSAGE(MsgEarthPowerUp + i);
        BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, unitId);
        AudioCommand_PlayFar(SE_212);
        ObjectDispatch_SelectAnimationFar(BattleObjGet(unitId)->object, 3);
        ObjectDispatch_SetAnimationSpeedFar(
            (struct DispatchObject *)BattleObjGet(unitId)->object, 32);
        BattleFx_PlayUnitElementEffect(unitId, i, 2, largestSummonCost - 1);
        BattleEventRuntime_WaitForReady();
      }
    }
    MemFree(before);
  }
  if (battleContinues != 0) {
    s8 *poison;
    BattleEventRuntime_Reset();
    if (plan->hauntDamage != 0) {
      BattleEvPush(BATTLE_EVENT_HIT, unitId);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, unitId);
      BattleEvPush(BATTLE_EVENT_NUMBER, plan->hauntDamage);
      PUSH_MESSAGE(MsgCurseDamage);
      if (Unit_AddHpFar(unitId, -plan->hauntDamage) == 0) {
        s32 text;
        BattleEvPush(BATTLE_EVENT_DEFEAT, unitId);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, unitId);
        if (unitId >= 0 && unitId <= 7) {
          text = MsgGoesDown;
        } else {
          text = MsgStrengthExhausted;
        }
        PUSH_MESSAGE(text);
      } else {
        BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, unitId);
      }
    }
    BattleEv_DispatchQueued();
    BattleEventRuntime_Reset();
    poison = &unit->poison;
    if (*poison != 0) {
      s32 damage = unit->maxHp * *poison / 10;
      struct BattleWork *state = gBattleWork;
      BattleEvPush(BATTLE_EVENT_HIT, unitId);
      BattleEvPush(BATTLE_EVENT_UNIT_NAME, unitId);
      BattleEvPush(BATTLE_EVENT_NUMBER, damage);
      PUSH_MESSAGE(MsgPoisonDamage);
      if (*poison != 0) {
        state->events.pendingCue = SE_HEAVY_IMPACT;
      } else {
        state->events.pendingCue = SE_133;
      }
      if (Unit_AddHpFar(unitId, -damage) == 0) {
        s32 text;
        BattleEvPush(BATTLE_EVENT_DEFEAT, unitId);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, unitId);
        if (unitId >= 0 && unitId <= 7) {
          text = MsgGoesDown;
        } else {
          text = MsgStrengthExhausted;
        }
        PUSH_MESSAGE(text);
      } else {
        BattleEvPush(BATTLE_EVENT_REFRESH_UNIT, unitId);
      }
    }
    BattleEv_DispatchQueued();
    BattleEventRuntime_Reset();
    if (unit->grimReaper != 0) {
      if (--unit->grimReaper == 0 && Unit_AddHpFar(unitId, 0xc0000000) == 0) {
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, unitId);
        PUSH_MESSAGE(MsgReaperCalls);
        BattleEvPush(BATTLE_EVENT_HIT, unitId);
        BattleEvPush(BATTLE_EVENT_DEFEAT, unitId);
        BattleEvPush(BATTLE_EVENT_UNIT_NAME, unitId);
        if (unitId >= 0 && unitId <= 7) {
          PUSH_MESSAGE(MsgReaperCalls - 3);
        } else {
          PUSH_MESSAGE(MsgReaperCalls + 3);
        }
      }
    }
    BattleEv_DispatchQueued();
  }
  UnitCalcFar(unitId);
}
