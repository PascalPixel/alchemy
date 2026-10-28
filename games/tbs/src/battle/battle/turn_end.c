/// @file
/// The end of an action and of a turn: counters, Djinn recovery, poison and
/// the Grim Reaper.
// Part of battle.c.

#include "../battle_internal.h"

/// Rolls whether a stat change or status with five turns or fewer left ends
/// early, and returns 1 when it does.
/// The chance is `luck * 3 - turns * 5 + bias` percent, so a lucky unit and a
/// counter near its end recover sooner; a chance above 100 always ends it.
/// Called by the turn-end CountDown functions below, each with its own bias.
s32 BattleUnit_RollCounterEarlyEnd(s32 unitId, s32 turns, s32 bias) {
  struct Unit *unit = Unit_GetFar(unitId);

  if (turns <= 5) {
    // 65536 / 100, rounded down, scales the percentage to the 16-bit random
    // range.
    s32 threshold = ((unit->luck * 3 - turns * 5) + bias) * (65536 / 100);
    if (threshold >= (Battle_RandomFar() & 0xffff))
      return 1;
  }
  return 0;
}

/// Counts down an Attack change and returns 1 when it ends.
/// Clears attackStage when the turns run out, and clears attackStage and
/// attackTurns when a lowered Attack recovers early (bias 30). Called by
/// Battle_ProcessTurnEnd, which then shows `MsgAtkNormal`.
s32 BattleUnit_CountDownAttackStage(s32 unitId) {
  struct Unit *unit = Unit_GetFar(unitId);

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

/// Defines a stat change's turn-end countdown, as
/// BattleUnit_CountDownAttackStage is with bias 20: clears `level` and returns
/// 1 when `counter` runs out, or clears both when a lowered stat recovers
/// early.
#define DEFINE_COUNT_DOWN_MODIFIER(name, counter, level)                       \
  s32 name(s32 unitId) {                                                       \
    struct Unit *state = Unit_GetFar(unitId);                                  \
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

/// Counts down a Defense change; Battle_ProcessTurnEnd shows `MsgDefNormal`
/// when it returns 1.
DEFINE_COUNT_DOWN_MODIFIER(BattleUnit_CountDownDefenseStage, defenseTurns,
                           defenseStage)
/// Counts down a Resistance change; Battle_ProcessTurnEnd shows
/// `MsgResNormal` when it returns 1.
DEFINE_COUNT_DOWN_MODIFIER(BattleUnit_CountDownResistanceStage, resistanceTurns,
                           resistanceStage)

/// Defines a status's turn-end countdown: returns 1 when `counter` runs out, or
/// clears it and returns 1 when the status ends early at `bias`.
#define DEFINE_COUNT_DOWN_STATUS(name, counter, bias)                          \
  s32 name(s32 unitId) {                                                       \
    struct Unit *state = Unit_GetFar(unitId);                                  \
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

/// Counts down delusion, which may end early at bias 30;
/// Battle_ProcessTurnEnd shows `MsgCureDelusion` when it returns 1.
DEFINE_COUNT_DOWN_STATUS(BattleUnit_CountDownDelusion, delusion, 30)

/// Counts down confusion, which may end early at bias 60;
/// Battle_ProcessTurnEnd shows `MsgConscious` when it returns 1.
s32 BattleUnit_CountDownConfusion(s32 unitId) {
  struct Unit *unit = Unit_GetFar(unitId);
  // Unit.confusion is counted down here as a u8, unlike the statuses
  // DEFINE_COUNT_DOWN_STATUS counts down, as are its reads in targeting.c and
  // battle_calc.c; the plain s8 reads below and in turn.c match too. It runs
  // from 0 to STATUS_TURNS, so its sign never shows.
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

/// Counts down charm, which may end early at bias 70; Battle_ProcessTurnEnd
/// shows `MsgConscious2` when it returns 1.
DEFINE_COUNT_DOWN_STATUS(BattleUnit_CountDownCharm, charm, 70)
/// Counts down stun, which may end early at bias 40; Battle_ProcessTurnEnd
/// shows `MsgCureStun` when it returns 1.
DEFINE_COUNT_DOWN_STATUS(BattleUnit_CountDownStun, stun, 40)
/// Counts down sleep, which may end early at bias 50; Battle_ProcessTurnEnd
/// shows `MsgWakes` when it returns 1.
DEFINE_COUNT_DOWN_STATUS(BattleUnit_CountDownSleep, sleep, 50)

/// Counts down both seals packed in Unit.psySeal and returns 1 when the
/// last ends.
/// Luff's seal counts down by 8 above the low three bits and Bind's by 1 in
/// them (PSY_SEAL_LUFF, PSY_SEAL_BIND); only Bind's, once Luff's is gone, may
/// end early (bias 30), which clears psySeal. Called by
/// Battle_ProcessTurnEnd, which then shows `MsgCureSeal`.
s32 Battle_AdvanceCounterAndCheckChance(s32 unitId) {
  s32 decremented;
  u8 remaining;
  s32 turns;
  s32 sealCounter;
  struct Unit *unit;

  unit = Unit_GetFar(unitId);
  sealCounter = unit->psySeal;
  turns = sealCounter & 0xFF;
  if (turns != 0) {
    // In the byte, adding 0xf8 is -8 and adding 0xff is -1.
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
        BattleUnit_RollCounterEarlyEnd(unitId, unit->psySeal, 30) != 0) {
      unit->psySeal = 0U;
      return 1;
    }
  }
  return 0;
}

/// Counts down Unit.regenerate and returns 1 when it runs out; it never
/// ends early. Battle_ProcessTurnEnd then shows `MsgRefrainEnd`.
s32 BattleUnit_CountDownRegenerate(s32 unitId) {
  u8 *turns = &Unit_GetFar(unitId)->regenerate;
  if (*turns != 0) {
    (*turns)--;
    if (*turns == 0) {
      return 1;
    }
  }
  return 0;
}

/// Counts down Reflect and returns 1 when it runs out; it never ends early.
/// Battle_ProcessTurnEnd then shows `MsgReflectEnd`.
s32 BattleUnit_CountDownReflect(s32 unitId) {
  u8 *turns = (u8 *)&Unit_GetFar(unitId)->reflect;
  if (*turns != 0) {
    (*turns)--;
    if (*turns == 0) {
      return 1;
    }
  }
  return 0;
}

/// Counts down an Agility change, clearing agilityStage and returning 1 when
/// it runs out; unlike the other stats it never ends early.
/// Battle_ProcessTurnEnd then shows `MsgAgiNormal`.
s32 BattleUnit_CountDownAgilityStage(s32 unitId) {
  struct Unit *unit = Unit_GetFar(unitId);
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

/// Brings the party's recovering Djinn one turn nearer, setting each whose
/// turns reach 0, and returns 1 when any was set.
/// A Djinni counts down only while its unit has HP; one that is set is taken
/// off the party's recovery list and its unit's stats worked out again
/// (Unit_Calculate). Called once at the end of every battle by
/// Battle_Run, and outside battle by Field_UpdateWalking as the party
/// walks, by the Djinn menu's tutorial and by DjinnRecovery_CountDownTwenty.
s32 DjinnRecovery_CountDown(void) {
  struct DjinnRecoveryList *list;
  struct DjinnRecoveryEntry *timedEntry;
  struct DjinnRecoveryEntry *expiredEntry;
  s32 index;
  s32 removed;
  s32 initialCount;

  list = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(
               DJINN_RECOVERY_PARTY))
              ->list;
  initialCount = list->count;
  index = 0;
  removed = 0;
  if (index < initialCount) {
    timedEntry = list->entries;
    do {
      if (timedEntry->turns > 0 && Unit_GetFar(timedEntry->unitId)->hp != 0) {
        timedEntry->turns--;
      }
      index++;
      timedEntry++;
    } while (index < list->count);
  }
  // Removing an entry moves the next one into its place, so the cursor
  // advances only past entries that stay.
  index = 0;
  if (index < list->count) {
    expiredEntry = list->entries;
    do {
      if (expiredEntry->turns == 0) {
        u8 id = expiredEntry->unitId;

        Djinn_SetFar(id, expiredEntry->element, expiredEntry->index);
        DjinnRecovery_RemoveFar(id, expiredEntry->element, expiredEntry->index);
        Unit_CalculateFar(id);
        removed = 1;
      } else {
        expiredEntry++;
        index++;
      }
    } while (index < list->count);
  }
  return removed;
}

/// Runs DjinnRecovery_CountDown twenty times, which sets every recovering
/// Djinni of a party member with HP; those on standby stay so. Returns 0.
/// Called by the Battle Arena when it restores the party, and by Vault's event
/// before Ivan hands back Isaac's Djinni.
s32 DjinnRecovery_CountDownTwenty(void) {
  s32 turns;

  turns = 19;
  do {
    turns--;
    DjinnRecovery_CountDown();
  } while (turns >= 0);
  return 0;
}

/// Does nothing; Battle_Run calls it at the end of each turn and of
/// the battle.
void Battle_ReservedNoOpF674(void) {}

/// The order the two sides' units take their turn-end effects in, as
/// gBattleParty_TurnEndSideOrder gives it. Battle_ProcessTurnEnd copies it
/// to its stack, and the link child swaps the copy's two sides.
struct BattleSideOrder {
  s32 group[2]; ///< enum BattleSideMask: the party, then the enemies
};

/// Ends a turn: sets recovered Djinn, then gives each unit its regeneration
/// and counts down its stat changes and statuses, showing each that ends.
/// Called by Battle_Run once every unit has acted. Changes HP, PP,
/// Unit.ready, the Djinn recovery lists and the counters the CountDown
/// functions clear, and puts the background back in BATTLE_BACKGROUND_ZOOMED.
#ifdef UBFIX
void Battle_ProcessTurnEnd(void) {
#else
s32 Battle_ProcessTurnEnd(void) {
#endif
  struct BattleWork *battle;
  s32 side;
  s32 sideCount;
  s32 count;
  s32 recoveryIndex;
  struct DjinnRecoveryList *list;
  struct DjinnRecoveryEntry *timedEntry;
  struct DjinnRecoveryEntry *expiredEntry;

  // Djinn recovery: the party's list, and in a link battle the opponent's
  // too. Only a Djinni whose unit is on the field with HP counts down.
  battle = gBattleWork;
  sideCount = (battle->isLink != 0) + 1;
  for (side = 0; side < sideCount; side++) {
    list =
        &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(side))->list;
    recoveryIndex = 0;
    if (recoveryIndex < list->count) {
      timedEntry = list->entries;
      do {
        if (timedEntry->turns > 0 &&
            BattleObject_Get(timedEntry->unitId) != 0 &&
            Unit_GetFar(timedEntry->unitId)->hp != 0) {
          timedEntry->turns--;
        }
        recoveryIndex++;
        timedEntry++;
      } while (recoveryIndex < list->count);
    }
    // Set each recovered Djinni, as DjinnRecovery_CountDown does, with its
    // message, the unit's POSE_CAST and its element's battle effect. As there,
    // the cursor stays on a removed entry; it would stay for ever on one whose
    // unit is off the field, but only a unit on the field counts down to 0
    // above.
    recoveryIndex = 0;
    if (recoveryIndex < list->count) {
      expiredEntry = list->entries;
      do {
        if (expiredEntry->turns == 0) {
          u8 unitId = expiredEntry->unitId;
          s32 element;

          if (BattleObject_Get(unitId) != 0) {
            BattlePlayback_Reset();
            BattlePlayback_SchedulePhase(30);
            BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
            // The Djinni's own ability, which names it (ABILITY_DJINNI_FIRST).
            BattlePlayback_Push(PLAYBACK_ABILITY_NAME,
                                expiredEntry->element * DJINN_PER_ELEMENT +
                                    expiredEntry->index + ABILITY_DJINNI_FIRST);
            BattlePlayback_Push(PLAYBACK_SOUND, SE_NOTE_HIGH);
            BattlePlayback_Push(PLAYBACK_DRAW_STATUS, 0);
            PUSH_MESSAGE(MsgDjinnSet);
            BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, unitId);
            Sound_PlayFar(SE_POWER_UP);
            Actor_SelectAnimationFar(BattleObject_Get(unitId)->actor,
                                     POSE_CAST);
            Actor_SetAnimationSpeedFar(BattleObject_Get(unitId)->actor, 32);
            element = expiredEntry->element;
            Djinn_SetFar(unitId, element, expiredEntry->index);
            DjinnRecovery_RemoveFar(unitId, expiredEntry->element,
                                    expiredEntry->index);
            Unit_CalculateFar(unitId);
            ElementGlow_Play(unitId, element, GLOW_BURST, 0);
            BattlePlayback_WaitForReady();
          }
        } else {
          expiredEntry++;
          recoveryIndex++;
        }
      } while (recoveryIndex < list->count);
    }
  }
  // Fade 0 leaves the palette as it is: this only puts the zoomed
  // background back and rebuilds its map.
  BattleBackground_SetMode(BATTLE_BACKGROUND_ZOOMED, gBattleWork->background,
                           0);
  {
    s32 *groupList;
    struct BattleSideOrder order = gBattleParty_TurnEndSideOrder;
    u16 unitIds[14];
    s32 group;
    s32 unitIndex;

    // Each unit's regeneration and counters, the party's first. The link
    // child takes the enemies first, so both consoles go through the
    // parent's units first and show the same messages in the same order.
    if (battle->isLink != 0 && battle->linkSide != 0) {
      order.group[0] = BATTLE_SIDE_MASK_ENEMIES;
      order.group[1] = BATTLE_SIDE_MASK_PARTY;
    }
    groupList = order.group;
    for (group = 0; group <= 1; group++) {
      count = BattleUnit_ListIds(groupList[group], unitIds);
      for (unitIndex = 0; unitIndex < count; unitIndex++) {
        s32 hpAmount;
        s32 ppAmount;
        s32 unitId = unitIds[unitIndex];
        struct Unit *unit = Unit_GetFar(unitId);

        if (unit->ready != 0) {
          unit->ready--;
        }
        // Regeneration, capped at the maximum, for a unit still standing.
        if (unit->hp != 0) {
          if (unit->hpRegen != 0 && unit->hp != unit->hpMax) {
            hpAmount = unit->hpRegen;
            if (unit->hp + hpAmount > unit->hpMax) {
              hpAmount = unit->hpMax - unit->hp;
            }
            Unit_AddHpFar(unitId, hpAmount);
            MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
            MessageArgument_PushFar(hpAmount, MESSAGE_ARGUMENT_NUMBER);
            if (unit->hp == unit->hpMax) {
              BattleMessage_ShowAndWaitFar(MsgHpFull);
            } else {
              BattleMessage_ShowAndWaitFar(MsgHpRecover);
            }
            Sound_PlayFar(SE_NOTE_HIGH);
            BattleMessage_WaitForAdvance();
          }
          if (unit->ppRegen != 0 && unit->pp != unit->ppMax) {
            ppAmount = unit->ppRegen;
            if (unit->pp + ppAmount > unit->ppMax) {
              ppAmount = unit->ppMax - unit->pp;
            }
            Unit_AddPpFar(unitId, ppAmount);
            MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
            MessageArgument_PushFar(ppAmount, MESSAGE_ARGUMENT_NUMBER);
            if (unit->pp == unit->ppMax) {
              BattleMessage_ShowAndWaitFar(MsgPpFull);
            } else {
              BattleMessage_ShowAndWaitFar(MsgPpRecover);
            }
            Sound_PlayFar(SE_NOTE_HIGH);
            BattleMessage_WaitForAdvance();
          }
        }
        // Each counter that ends rebuilds the unit's status icons and shows
        // its message; the end of stun or sleep also sets the unit standing.
        if (BattleUnit_CountDownAgilityStage(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));
          MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
          BattleMessage_ShowAndWaitFar(MsgAgiNormal);
          BattleMessage_WaitForAdvance();
        }
        if (BattleUnit_CountDownAttackStage(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));
          MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
          BattleMessage_ShowAndWaitFar(MsgAtkNormal);
          BattleMessage_WaitForAdvance();
        }
        if (BattleUnit_CountDownDefenseStage(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));
          MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
          BattleMessage_ShowAndWaitFar(MsgDefNormal);
          BattleMessage_WaitForAdvance();
        }
        if (BattleUnit_CountDownResistanceStage(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));
          MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
          BattleMessage_ShowAndWaitFar(MsgResNormal);
          BattleMessage_WaitForAdvance();
        }
        if (BattleUnit_CountDownDelusion(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));
          MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
          BattleMessage_ShowAndWaitFar(MsgCureDelusion);
          BattleMessage_WaitForAdvance();
        }
        if (BattleUnit_CountDownConfusion(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));
          MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
          BattleMessage_ShowAndWaitFar(MsgConscious);
          BattleMessage_WaitForAdvance();
        }
        if (BattleUnit_CountDownCharm(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));
          MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
          BattleMessage_ShowAndWaitFar(MsgConscious2);
          BattleMessage_WaitForAdvance();
        }
        if (BattleUnit_CountDownStun(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));
          MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
          BattleObject_UpdatePose(unitId);
          BattleMessage_ShowAndWaitFar(MsgCureStun);
          BattleMessage_WaitForAdvance();
        }
        if (BattleUnit_CountDownSleep(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));
          MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
          BattleObject_UpdatePose(unitId);
          BattleMessage_ShowAndWaitFar(MsgWakes);
          BattleMessage_WaitForAdvance();
        }
        if (Battle_AdvanceCounterAndCheckChance(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));
          MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
          BattleMessage_ShowAndWaitFar(MsgCureSeal);
          BattleMessage_WaitForAdvance();
        }
        if (BattleUnit_CountDownRegenerate(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));
          MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
          BattleMessage_ShowAndWaitFar(MsgRefrainEnd);
          BattleMessage_WaitForAdvance();
        }
        if (BattleUnit_CountDownReflect(unitId) != 0) {
          BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));
          MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
          BattleMessage_ShowAndWaitFar(MsgReflectEnd);
          BattleMessage_WaitForAdvance();
        }
      }
    }
  }
}

/// Ends one unit's action: its Djinn on standby are taken off its set, the
/// Djinn a summon used begin to recover, and the unit takes its summon's power
/// boost, the haunt's damage, poison and the Grim Reaper.
/// Called by BattleStaging_DispatchAction after every action, with the
/// action's run. Changes the side's Djinn recovery list, the unit's
/// summonBoost, HP and grimReaper, and works out its stats again
/// (Unit_Calculate); the messages are queued and played at once
/// (BattlePlayback_RunAll).
#ifdef UBFIX
void BattleUnit_ProcessActionEnd(struct BattleActionRun *plan) {
#else
s32 BattleUnit_ProcessActionEnd(struct BattleActionRun *plan) {
#endif
  s32 unitId;
  struct Unit *unit;
  s32 battleContinues;
  s32 counts[ELEMENT_COUNT];
  struct DjinnRecoveryList *list;
  s32 strongestElement;
  s32 recoveryTurn;

  // Take the unit's standby Djinn (DJINN_RECOVERY_AFTER_BATTLE, as an
  // unleashed Djinni is left by BattleActionRun_Build) off its set. The table
  // is the opponent's for an enemy unit id.
  unitId = plan->unitId;
  battleContinues = 0;
  unit = Unit_GetFar(unitId);
  list = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(
               (unitId < 0 || unitId > ALLY_MAX)))
              ->list;
  for (strongestElement = 0; strongestElement < list->count;
       strongestElement++) {
    if (list->entries[strongestElement].unitId == unitId &&
        list->entries[strongestElement].turns == DJINN_RECOVERY_AFTER_BATTLE) {
      Djinn_PutOnStandbyFar(unitId, list->entries[strongestElement].element,
                            list->entries[strongestElement].index);
    }
  }
  if (BattleUnit_ListLiving(BATTLE_SIDE_MASK_PARTY, 0) != 0 &&
      BattleUnit_ListLiving(BATTLE_SIDE_MASK_ENEMIES, 0) != 0) {
    battleContinues = 1;
  }
  {
    s32 i;
    s32 holderId;
    struct DjinnRecoveryList *pending;
    pending = &((struct DjinnRecoveryTable *)Djinn_GetRecoveryTableFar(
                    (unitId < 0 || unitId > ALLY_MAX)))
                   ->list;
    // The Djinn a summon used (DJINN_RECOVERY_SUMMONED, from
    // BattleActionRun_Build), unit by unit: they recover one a turn, after
    // the last the unit already has recovering and no sooner than in 2 turns.
    // counts gathers how many of each element the summon used.
    for (i = 0; i < ELEMENT_COUNT; i++) {
      counts[i] = 0;
    }
    for (;;) {
      holderId = -1;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].turns == DJINN_RECOVERY_SUMMONED) {
          holderId = pending->entries[i].unitId;
          break;
        }
      }
      if (holderId == -1) {
        break;
      }
      recoveryTurn = -1;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].unitId == holderId &&
            pending->entries[i].turns > recoveryTurn) {
          recoveryTurn = pending->entries[i].turns;
        }
      }
      recoveryTurn++;
      if (recoveryTurn <= 1) {
        recoveryTurn = 2;
      }
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].unitId == holderId &&
            pending->entries[i].turns == DJINN_RECOVERY_SUMMONED) {
          pending->entries[i].turns = recoveryTurn;
          counts[pending->entries[i].element]++;
          recoveryTurn++;
        }
      }
    }
  }
  if (battleContinues != 0) {
    struct Unit *before;
    s32 mostOfOneElement;
    s32 i;
    // The summon's boost: the element it used most Djinn of rises to that
    // many levels (summonBoost), and each element's power that Unit_Calculate
    // then raises is announced against a copy taken before.
    mostOfOneElement = 0;
    before = Heap_AllocEwram(sizeof(struct Unit));
    Iwram_CopyWords(before, unit, sizeof(struct Unit));
    strongestElement = -1;
    for (i = 0; i < ELEMENT_COUNT; i++) {
      if (counts[i] > mostOfOneElement) {
        mostOfOneElement = counts[i];
        strongestElement = i;
      }
    }
    if (strongestElement >= 0 &&
        unit->summonBoost[strongestElement] < mostOfOneElement) {
      unit->summonBoost[strongestElement] = mostOfOneElement;
    }
    Unit_CalculateFar(unitId);
    for (i = 0; i < ELEMENT_COUNT; i++) {
      s32 gain = unit->element[i].power - before->element[i].power;

      if (gain > 0) {
        BattlePlayback_Reset();
        BattlePlayback_SchedulePhase(25);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
        BattlePlayback_Push(PLAYBACK_NUMBER, gain);
        BattlePlayback_Push(PLAYBACK_SOUND, SE_NOTE_HIGH);
        // MsgEarthPowerUp and the next three, in enum Element order.
        PUSH_MESSAGE(MsgEarthPowerUp + i);
        BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, unitId);
        Sound_PlayFar(SE_POWER_UP);
        Actor_SelectAnimationFar(BattleObject_Get(unitId)->actor, POSE_CAST);
        Actor_SetAnimationSpeedFar(BattleObject_Get(unitId)->actor, 32);
        // The pillar of each element whose power rose, its strength the most
        // Djinn of one element the summon used, less one.
        ElementGlow_Play(unitId, i, GLOW_PILLAR, mostOfOneElement - 1);
        BattlePlayback_WaitForReady();
      }
    }
    Heap_Release(before);
  }
  // Damage at the action's end, each queue played at once: the haunting
  // spirit's (the shares of its damage BattleAbility_ApplyToTarget gathered),
  // poison's, then the Grim Reaper's countdown.
  if (battleContinues != 0) {
    s8 *poison;
    BattlePlayback_Reset();
    if (plan->hauntDamage != 0) {
      BattlePlayback_Push(PLAYBACK_HIT, unitId);
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
      BattlePlayback_Push(PLAYBACK_NUMBER, plan->hauntDamage);
      PUSH_MESSAGE(MsgCurseDamage);
      if (Unit_AddHpFar(unitId, -plan->hauntDamage) == 0) {
        s32 text;
        BattlePlayback_Push(PLAYBACK_DEFEAT, unitId);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
        if (unitId >= 0 && unitId <= ALLY_MAX) {
          text = MsgGoesDown;
        } else {
          text = MsgStrengthExhausted;
        }
        PUSH_MESSAGE(text);
      } else {
        BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, unitId);
      }
    }
    BattlePlayback_RunAll();
    BattlePlayback_Reset();
    poison = &unit->poison;
    // A tenth of the maximum HP, a fifth for deadly poison (POISON_VENOM).
    if (*poison != 0) {
      s32 damage = unit->hpMax * *poison / 10;
      struct BattleWork *state = gBattleWork;
      BattlePlayback_Push(PLAYBACK_HIT, unitId);
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
      BattlePlayback_Push(PLAYBACK_NUMBER, damage);
      PUSH_MESSAGE(MsgPoisonDamage);
      // DEAD: *poison is nonzero here, so the hit's sound is always
      // SE_HEAVY_IMPACT; the compare and both stores are in the ROM.
      if (*poison != 0) {
        state->playback.pendingSound = SE_HEAVY_IMPACT;
      } else {
        state->playback.pendingSound = SE_IMPACT;
      }
      if (Unit_AddHpFar(unitId, -damage) == 0) {
        s32 text;
        BattlePlayback_Push(PLAYBACK_DEFEAT, unitId);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
        if (unitId >= 0 && unitId <= ALLY_MAX) {
          text = MsgGoesDown;
        } else {
          text = MsgStrengthExhausted;
        }
        PUSH_MESSAGE(text);
      } else {
        BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, unitId);
      }
    }
    BattlePlayback_RunAll();
    BattlePlayback_Reset();
    // At 0 the Reaper takes far more HP than any unit has. The downed
    // message is MsgGoesDown or MsgStrengthExhausted, three either side of
    // MsgReaperCalls (see Matching on message offsets).
    if (unit->grimReaper != 0) {
      if (--unit->grimReaper == 0 && Unit_AddHpFar(unitId, 0xc0000000) == 0) {
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
        PUSH_MESSAGE(MsgReaperCalls);
        BattlePlayback_Push(PLAYBACK_HIT, unitId);
        BattlePlayback_Push(PLAYBACK_DEFEAT, unitId);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
        if (unitId >= 0 && unitId <= ALLY_MAX) {
          PUSH_MESSAGE(MsgReaperCalls - 3);
        } else {
          PUSH_MESSAGE(MsgReaperCalls + 3);
        }
      }
    }
    BattlePlayback_RunAll();
  }
  Unit_CalculateFar(unitId);
}
