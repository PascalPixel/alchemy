/// @file
/// The end of a turn: each unit's modifiers and conditions counted down, the
/// Djinn recovering, and the extras an action applies.
// Part of battle.c.

#include "battle_internal.h"
#include "sounds.h"
#include "djinn_ids.h"
#include "gamepak/far_call/game.h"
#include "module/battle/presentation.h"
#include "module/battle/presentation.h"

/// Rolls whether a stat change or status with five turns or fewer left ends
/// early, at `luck * 3 - turns * 5 + bias` percent, and returns 1 when it does.
s32 BattleUnit_RollCounterEarlyEnd(s32 unitId, s32 turns, s32 bias) {
  struct Unit *state = Unit_Get(unitId);

  if (turns <= 5) {
    s32 threshold = ((state->luck * 3 - turns * 5) + bias) * 0x28f;
    if (threshold >= (Battle_RandomFar() & 0xffff))
      return 1;
  }
  return 0;
}

/// Counts down an Attack change and returns 1 when it ends; a lowered Attack
/// may recover early.
s32 BattleUnit_CountDownAttackStage(s32 unitId) {
  struct Unit *state = Unit_Get(unitId);

  if (state->attackTurns != 0) {
    s32 zero;
    state->attackTurns--;
    zero = 0;
    if ((s8)state->attackTurns == 0) {
      state->attackStage = zero;
      return 1;
    }
    if (state->attackStage < 0) {
      if (BattleUnit_RollCounterEarlyEnd(unitId, state->attackTurns, 30) != 0) {
        state->attackStage = zero;
        state->attackTurns = zero;
        return 1;
      }
    }
  }
  return 0;
}

/// Counts down a Defense change and returns 1 when it ends; a lowered Defense
/// may recover early.
s32 BattleUnit_CountDownDefenseStage(s32 unitId) {
  struct Unit *state = Unit_Get(unitId);

  if (state->defenseTurns != 0) {
    state->defenseTurns--;
    if (state->defenseTurns == 0) {
      state->defenseStage = 0;
      return 1;
    }
    if (state->defenseStage < 0 &&
        BattleUnit_RollCounterEarlyEnd(unitId, state->defenseTurns, 20) != 0) {
      state->defenseStage = 0;
      state->defenseTurns = 0;
      return 1;
    }
  }
  return 0;
}

/// Counts down a Resistance change and returns 1 when it ends; a lowered
/// Resistance may recover early.
s32 BattleUnit_CountDownResistanceStage(s32 unitId) {
  struct Unit *state = Unit_Get(unitId);

  if (state->resistanceTurns != 0) {
    state->resistanceTurns--;
    if (state->resistanceTurns == 0) {
      state->resistanceStage = 0;
      return 1;
    }
    if (state->resistanceStage < 0 &&
        BattleUnit_RollCounterEarlyEnd(unitId, state->resistanceTurns, 20) !=
            0) {
      state->resistanceStage = 0;
      state->resistanceTurns = 0;
      return 1;
    }
  }
  return 0;
}

/// Counts down delusion and returns 1 when it ends, early or not.
s32 BattleUnit_CountDownDelusion(s32 unitId) {
  struct Unit *state = Unit_Get(unitId);
  if (state->delusion != 0) {
    state->delusion--;
    if (state->delusion == 0)
      return 1;
    if (BattleUnit_RollCounterEarlyEnd(unitId, state->delusion, 30) != 0) {
      state->delusion = 0;
      return 1;
    }
  }
  return 0;
}

/// Counts down confusion and returns 1 when it ends, early or not.
s32 BattleUnit_CountDownConfusion(s32 unitId) {
  struct Unit *state = Unit_Get(unitId);
  if ((u8)state->confusion != 0) {
    (*(u8 *)&state->confusion)--;
    if (state->confusion == 0)
      return 1;
    if (BattleUnit_RollCounterEarlyEnd(unitId, (u8)state->confusion, 60) != 0) {
      state->confusion = 0;
      return 1;
    }
  }
  return 0;
}

/// Counts down charm and returns 1 when it ends, early or not.
s32 BattleUnit_CountDownCharm(s32 unitId) {
  struct Unit *state = Unit_Get(unitId);
  if (state->charm != 0) {
    state->charm--;
    if (state->charm == 0)
      return 1;
    if (BattleUnit_RollCounterEarlyEnd(unitId, state->charm, 70) != 0) {
      state->charm = 0;
      return 1;
    }
  }
  return 0;
}

/// Counts down a stun and returns 1 when it ends, early or not.
s32 BattleUnit_CountDownStun(s32 unitId) {
  struct Unit *state = Unit_Get(unitId);
  if (state->stun != 0) {
    state->stun--;
    if (state->stun == 0)
      return 1;
    if (BattleUnit_RollCounterEarlyEnd(unitId, state->stun, 40) != 0) {
      state->stun = 0;
      return 1;
    }
  }
  return 0;
}

/// Counts down sleep and returns 1 when it ends, early or not.
s32 BattleUnit_CountDownSleep(s32 unitId) {
  struct Unit *state = Unit_Get(unitId);
  if (state->sleep != 0) {
    state->sleep--;
    if (state->sleep == 0)
      return 1;
    if (BattleUnit_RollCounterEarlyEnd(unitId, state->sleep, 50) != 0) {
      state->sleep = 0;
      return 1;
    }
  }
  return 0;
}

/// Counts down both seals in Unit.psySeal and returns 1 when the last
/// ends; only Bind's, once Luff's is gone, may end early.
s32 BattleUnit_CountDownPsynergySeal(s32 id) {
  s32 next;
  s32 expired;
  u8 remaining;
  s32 turns;
  s32 packed;
  struct Unit *unit;

  unit = Unit_Get(id);
  packed = unit->psySeal;
  turns = packed & 0xFF;
  if (turns != 0) {
    if ((u32)turns > 7U) {
      turns += 0xF8;
      unit->psySeal = turns;
      packed = turns;
    }
    if (packed & 7) {
      next = packed + 0xFF;
      unit->psySeal = next;
      packed = next;
    }
    expired = 1;
    remaining = packed;
    if (remaining == 0)
      return expired;
    if ((u32)remaining <= 7U &&
        BattleUnit_RollCounterEarlyEnd(id, unit->psySeal, 0x1E) != 0) {
      unit->psySeal = 0U;
      return 1;
    }
  }
  expired = 0;
  return expired;
}

/// While Regenerate lasts, heals the unit by a tenth of its max HP for each
/// turn it has left and one more, shown in playback; then counts it down and
/// returns 1 when it runs out.
s32 BattleUnit_CountDownRegenerate(s32 id) {
  struct Unit *unit = Unit_Get(id);
  u8 *count = &unit->regenerate;

  if (*count != 0) {
    s32 max;
    s32 before;
    s32 hp;
    s32 gain;

    BattlePlayback_Reset();
    hp = unit->hp;
    max = unit->hpMax;
    before = hp;
    hp += max * (*count + 1) / 10;
    if (hp > max)
      hp = max;
    gain = hp - before;
    if (gain != 0) {
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, id);
      if (hp == unit->hpMax) {
        BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)&MsgHpFull);
      } else {
        {
          u32 opcode = PLAYBACK_NUMBER;

          BattlePlayback_Push(opcode, gain);
        }
        BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)&MsgHpRecover);
      }
      unit->hp = hp;
      Unit_RecalculateGaugesFar(id);
    }
    BattlePlayback_RunAll();
    if (--unit->regenerate == 0)
      return 1;
  }
  return 0;
}

/// Counts down Reflect and returns 1 when it runs out; it never ends early.
s32 BattleUnit_CountDownReflect(s32 id) {
  u8 *value = (u8 *)&((struct Unit *)Unit_Get(id))->reflect;
  if (*value != 0) {
    (*value)--;
    if (*value == 0) {
      return 1;
    }
  }
  return 0;
}

/// Counts down an Agility change and returns 1 when it runs out; it never ends
/// early.
s32 BattleUnit_CountDownAgilityStage(s32 id) {
  struct Unit *unit = Unit_Get(id);
  u8 *value = &unit->agilityTurns;
  if (*value != 0) {
    (*value)--;
    if (*value == 0) {
      unit->agilityStage = 0;
      return 1;
    }
  }
  return 0;
}

/// Brings the party's recovering Djinn one turn nearer, setting each that is
/// ready, and returns 1 when any was set.
s32 DjinnRecovery_CountDown(void) {
  struct DjinnRecoveryList *list;
  struct DjinnRecoveryEntry *timedEntry;
  struct DjinnRecoveryEntry *expiredEntry;
  s32 index;
  s32 removed;
  s32 initialCount;

  list = &Djinn_GetRecoveryTableFar(0)->list;
  initialCount = list->count;
  index = 0;
  removed = 0;
  if (index < initialCount) {
    timedEntry = list->entries;
    do {
      if (timedEntry->turns > 0 &&
          ((struct Unit *)Unit_Get(timedEntry->unitId))->hp != 0) {
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
s32 DjinnRecovery_CountDownTwenty(void) {
  s32 turns;

  turns = 19;
  do {
    turns--;
    DjinnRecovery_CountDown();
  } while (turns >= 0);
  return 0;
}

/// Does nothing; Battle_Run calls it at the end of each turn and of the battle.
void Battle_TurnEndNoOp(void) {}

/// Counts down one side's Djinn recovery at the turn's end: an owner must be in
/// the battle; each Djinni that comes back plays its event (its icon, sound
/// 175, the flash and its message) before it is Set. The message is
/// "... is set to ...!" Returns 1 when a count fell, 2 when one was Set, else
/// 0.
s32 DjinnRecovery_CountDownSide(s32 side) {
  struct DjinnRecoveryList *list;
  struct DjinnRecoveryEntry *timedEntry;
  struct DjinnRecoveryEntry *entry;
  s32 result = 0;
  s32 i;
  s32 count;

  list = &Djinn_GetRecoveryTableFar(side)->list;
  i = 0;
  if (i < list->count) {
    timedEntry = list->entries;
    do {
      if (timedEntry->turns > 0 && BattleUnit_IsListed(timedEntry->unitId) &&
          ((struct Unit *)Unit_Get(timedEntry->unitId))->hp != 0) {
        timedEntry->turns--;
        result = 1;
      }
      count = list->count;

      i++;
      timedEntry++;
    } while (i < count);
  }
  i = 0;
  if (i < list->count) {
    entry = list->entries;
    do {
      if (entry->turns == 0) {
        u8 id = entry->unitId;

        if (BattleUnit_IsListed(id)) {
          u8 element;

          result = 2;
          BattlePlayback_Reset();
          BattlePlayback_SchedulePhase(30);
          BattlePlayback_Push(PLAYBACK_UNIT_NAME, id);
          BattlePlayback_Push(PLAYBACK_ABILITY_NAME,
                              entry->element * DJINN_PER_ELEMENT +
                                  entry->index + ABILITY_DJINNI_FIRST);
          BattlePlayback_Push(PLAYBACK_SOUND, SE_NOTE_HIGH);
          BattlePlayback_Push(PLAYBACK_DRAW_STATUS, 0);
          BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)&MsgDjinnIsSet);
          BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, id);
          if (BattleUnit_IsListed(id)) {
            Actor_SelectAnimationFar(
                (struct Actor *)BattleObject_Get(id)->object, 3);
            Actor_SetAnimationSpeedFar(
                (struct Actor *)BattleObject_Get(id)->object, 32);
          }
          element = entry->element;
          Djinn_SetFar(id, element, entry->index);
          DjinnRecovery_RemoveFar(id, entry->element, entry->index);
          Unit_CalculateFar(id);
          if (BattleUnit_IsListed(id))
            ElementGlow_Play(id, element, 3, 0);
          BattlePlayback_WaitForReady();
        }
      } else {
        entry++;
        i++;
      }
    } while (i < list->count);
  }
  return result;
}

// The Japanese text holds these messages in the same order two places later,
// but its labels for several of them belong to their neighbours, so the
// Japanese release reaches each of those by its place beside a message whose
// label fits its text.
#if LANGUAGE == LANGUAGE_JAPANESE
// ねむりからさめた! ("wakes from slumber"), three before MsgResNormal.
#define MSG_WAKES ((s32)&MsgResNormal - 3)
// すばやさが もどった! ("Agility returns to normal"), after MsgDefNormal.
#define MSG_AGI_NORMAL ((s32)&MsgDefNormal + 1)
// われにかえった! ("regains consciousness"), two after MsgDefNormal.
#define MSG_CONSCIOUS ((s32)&MsgDefNormal + 2)
// まぼろしを うちやぶった! ("sees clearly once again"), three before
// MsgConscious2.
#define MSG_CURE_DELUSION ((s32)&MsgConscious2 - 3)
// エナジーふうじがとけた! ("Psynergy seal is gone"), two before MsgConscious2.
#define MSG_CURE_SEAL ((s32)&MsgConscious2 - 2)
// しびれがきえた! ("no longer stunned"), the message before MsgConscious2.
#define MSG_CURE_STUN ((s32)&MsgConscious2 - 1)
// リフレクトのこうかがきえた! ("Reflect no longer affects"), after
// MsgRefrainEnd.
#define MSG_REFLECT_END ((s32)&MsgRefrainEnd + 1)
#else
#define MSG_WAKES ((s32)&MsgWakes)
#define MSG_AGI_NORMAL ((s32)&MsgAgiNormal)
#define MSG_CONSCIOUS ((s32)&MsgConscious)
#define MSG_CURE_DELUSION ((s32)&MsgCureDelusion)
#define MSG_CURE_SEAL ((s32)&MsgCureSeal)
#define MSG_CURE_STUN ((s32)&MsgCureStun)
#define MSG_REFLECT_END ((s32)&MsgReflectEnd)
#endif

#define ANNOUNCE_END(test, message)                                            \
  if (test(unitId) != 0) {                                                     \
    BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));             \
    MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);                   \
    BattleMessage_ShowAndWaitFar(message);                                     \
    BattleMessage_WaitForAdvance();                                            \
  }

#define ANNOUNCE_END_POSE(test, message)                                       \
  if (test(unitId) != 0) {                                                     \
    BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));             \
    MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);                   \
    BattleObject_UpdatePose(unitId);                                           \
    BattleMessage_ShowAndWaitFar(message);                                     \
    BattleMessage_WaitForAdvance();                                            \
  }

/// Ends a turn: counts down the recovering Djinn and sets those recovered, then
/// gives each unit its regeneration and counts down its stat changes and
/// statuses, showing each that ends.
void Battle_ProcessTurnEnd(void) {
  struct BattleWork *battle;
  s32 side;
  s32 sideCount;
  s32 count;
  s32 recoveryIndex;
  struct DjinnRecoveryList *list;
  struct DjinnRecoveryEntry *timedEntry;
  struct DjinnRecoveryEntry *expiredEntry;

  battle = Ram_HeapSlots->battleWork;
  sideCount = (battle->isLink != 0) + 1;
  for (side = 0; side < sideCount; side++) {
    list = &Djinn_GetRecoveryTableFar(side)->list;
    recoveryIndex = 0;
    timedEntry = list->entries;
    if (recoveryIndex < list->count) {
      do {
        if (timedEntry->turns > 0 &&
            BattleObject_Get(timedEntry->unitId) != 0 &&
            ((struct Unit *)Unit_Get(timedEntry->unitId))->hp != 0) {
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
          u8 element;

          if (BattleObject_Get(unitId) != 0) {
            BattlePlayback_Reset();
            BattlePlayback_SchedulePhase(30);
            BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
            BattlePlayback_Push(PLAYBACK_ABILITY_NAME,
                                expiredEntry->element * DJINN_PER_ELEMENT +
                                    expiredEntry->index + ABILITY_DJINNI_FIRST);
            BattlePlayback_Push(PLAYBACK_SOUND, SE_NOTE_HIGH);
            BattlePlayback_Push(PLAYBACK_DRAW_STATUS, 0);
            BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)&MsgDjinnIsSet);
            BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, unitId);
            Sound_PlayFar(SE_POWER_UP);
            if (BattleUnit_IsListed(unitId)) {
              Actor_SelectAnimationFar(BattleObject_Get(unitId)->object, 3);
              Actor_SetAnimationSpeedFar(
                  (struct Actor *)BattleObject_Get(unitId)->object, 32);
            }
            element = expiredEntry->element;
            Djinn_SetFar(unitId, element, expiredEntry->index);
            DjinnRecovery_RemoveFar(unitId, expiredEntry->element,
                                    expiredEntry->index);
            Unit_CalculateFar(unitId);
            if (BattleUnit_IsListed(unitId))
              ElementGlow_Play(unitId, element, 3, 0);
            BattlePlayback_WaitForReady();
          }
        } else {
          expiredEntry++;
          recoveryIndex++;
        }
      } while (recoveryIndex < list->count);
    }
  }
  BattleBackground_SetMode(2, Ram_HeapSlots->battleWork->background, 0);
  {
    s32 *groupList;
    struct BattleGroupOrder order = gBattleParty_TurnEndSideOrder;
    u16 unitIds[16];
    s32 group;
    s32 unitIndex;

    if (battle->isLink != 0 && battle->linkSide != 0) {
      order.group[0] = 2;
      order.group[1] = 1;
    }
    groupList = order.group;
    for (group = 0; group <= 1; group++) {
      count = BattleUnit_ListIds(groupList[group], unitIds);
      if (groupList[group] == 1)
        count += BattleParty_PrepareReserveOwners(unitIds + count);
      for (unitIndex = 0; unitIndex < count; unitIndex++) {
        s32 hpAmount;
        s32 ppAmount;
        s32 unitId = unitIds[unitIndex];
        struct Unit *unit = Unit_Get(unitId);

        unit->equipBonus[1] = 0;
        if (unit->ready != 0) {
          unit->ready--;
        }
        if (unit->hp != 0) {
          if (unit->hpRegen != 0 && unit->hp != unit->hpMax) {
            hpAmount = unit->hpRegen;
            switch (unit->enemyId) {
            case ENEMY_SERPENT:
            case ENEMY_SERPENT_2:
            case ENEMY_SERPENT_3:
            case ENEMY_SERPENT_4:
            case ENEMY_SERPENT_5:
            case ENEMY_SERPENT_6:
            case ENEMY_SERPENT_7:
            case ENEMY_DULLAHAN:
              hpAmount *= 10;
              break;
            }
            if (unit->hp + hpAmount > unit->hpMax) {
              hpAmount = unit->hpMax - unit->hp;
            }
            Unit_AddHpFar(unitId, hpAmount);
            MessageArgument_PushFar(unitId, MESSAGE_ARGUMENT_ACTOR);
            MessageArgument_PushFar(hpAmount, MESSAGE_ARGUMENT_NUMBER);
            if (unit->hp == unit->hpMax) {
              BattleMessage_ShowAndWaitFar((s32)&MsgHpFull);
            } else {
              BattleMessage_ShowAndWaitFar((s32)&MsgHpRecover);
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
              BattleMessage_ShowAndWaitFar((s32)&MsgPpFull);
            } else {
              BattleMessage_ShowAndWaitFar((s32)&MsgPpRecover);
            }
            Sound_PlayFar(SE_NOTE_HIGH);
            BattleMessage_WaitForAdvance();
          }
        }
        ANNOUNCE_END(BattleUnit_CountDownAgilityStage, MSG_AGI_NORMAL);
        ANNOUNCE_END(BattleUnit_CountDownAttackStage, (s32)&MsgAtkNormal);
        ANNOUNCE_END(BattleUnit_CountDownDefenseStage, (s32)&MsgDefNormal);
        ANNOUNCE_END(BattleUnit_CountDownResistanceStage, (s32)&MsgResNormal);
        ANNOUNCE_END(BattleUnit_CountDownDelusion, MSG_CURE_DELUSION);
        ANNOUNCE_END(BattleUnit_CountDownConfusion, MSG_CONSCIOUS);
        ANNOUNCE_END(BattleUnit_CountDownCharm, (s32)&MsgConscious2);
        ANNOUNCE_END_POSE(BattleUnit_CountDownStun, MSG_CURE_STUN);
        ANNOUNCE_END_POSE(BattleUnit_CountDownSleep, MSG_WAKES);
        ANNOUNCE_END(BattleUnit_CountDownPsynergySeal, MSG_CURE_SEAL);
        ANNOUNCE_END(BattleUnit_CountDownRegenerate, (s32)&MsgRefrainEnd);
        ANNOUNCE_END(BattleUnit_CountDownReflect, MSG_REFLECT_END);
      }
    }
  }
}

/// Djinnfest, Fugue, Djinn Blast, Djinn Storm and Djinn Stun: puts one of the
/// unit's set Djinn at random on standby (every one, when all is set), or, when
/// none is set, drains one that could be set so it must recover. Djinn Stun's
/// announce shows MsgDjinnOnStandby; the others show MsgDjinnDrained or
/// MsgAllDjinnDrained.
void BattleUnit_DrainDjinn(s32 unitId, s32 all, s32 announce) {
  u8 elements[10];
  u8 indices[10];
  s32 shown;
  s32 count;
  s32 active;
  s32 element;
  s32 djinni;
  s32 i;
  s32 owner;
  s32 recoveryTurn;
  struct DjinnRecoveryList *list;
  struct DjinnRecoveryList *pending;

  shown = 0;
again:
  {
    count = 0;
    active = 0;
    for (element = 0; element <= 3; element++) {
      for (djinni = 0; djinni <= 19; djinni++) {
        if (Djinn_IsSetFar(unitId, element, djinni)) {
          elements[count] = element;
          indices[count] = djinni;
          count++;
          active = 1;
          if (count == 10)
            goto picked;
        }
      }
    }
    if (count == 0) {
      for (element = 0; element <= 3; element++) {
        for (djinni = 0; djinni <= 19; djinni++) {
          if (Djinn_CanSetFar(unitId, element, djinni)) {
            elements[count] = element;
            indices[count] = djinni;
            count++;
            if (count == 10)
              goto picked;
          }
        }
      }
    }
  picked:
    if (count == 0)
      return;

    count = ((u32)Battle_RandomFar() * count) >> 16;
    element = elements[count];
    djinni = indices[count];
    if (active) {
      DjinnRecovery_AddStandbyFar(unitId, element, djinni);
      Djinn_PutOnStandbyFar(unitId, element, djinni);
      if (announce) {
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
        BattlePlayback_Push(PLAYBACK_ABILITY_NAME, element * DJINN_PER_ELEMENT +
                                                       djinni +
                                                       ABILITY_DJINNI_FIRST);
        BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)&MsgDjinnOnStandby);
        return;
      }
    } else if (announce) {
      return;
    }

    list = &Djinn_GetRecoveryTableFar(unitId < 0 || unitId > ALLY_MAX)->list;
    for (i = 0; i < list->count; i++) {
      if (list->entries[i].element == element &&
          list->entries[i].index == djinni && list->entries[i].turns == -1)
        list->entries[i].turns = -2;
    }

    pending = &Djinn_GetRecoveryTableFar(unitId < 0 || unitId > ALLY_MAX)->list;
    for (;;) {
      owner = -1;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].turns == -2) {
          owner = pending->entries[i].unitId;
          break;
        }
      }
      if (owner == -1)
        break;
      recoveryTurn = -1;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].unitId == owner &&
            pending->entries[i].turns > recoveryTurn)
          recoveryTurn = pending->entries[i].turns;
      }
      recoveryTurn++;
      if (recoveryTurn <= 1)
        recoveryTurn = 2;
      for (i = 0; i < pending->count; i++) {
        if (pending->entries[i].unitId == owner &&
            pending->entries[i].turns == -2) {
          pending->entries[i].turns = recoveryTurn;
          recoveryTurn++;
        }
      }
    }

    if (!shown) {
      shown = 1;
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
      if (!all) {
        s32 message;

        BattlePlayback_Push(PLAYBACK_ABILITY_NAME, element * DJINN_PER_ELEMENT +
                                                       djinni +
                                                       ABILITY_DJINNI_FIRST);
        message = (u32)&MsgDjinnDrained;
        if (unitId < 0 || unitId > ALLY_MAX)
          message++;
        BattlePlayback_Push(PLAYBACK_MESSAGE, message);
      } else {
        BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)&MsgAllDjinnDrained);
      }
    }
    if (all)
      goto again;
  }
}

/// Ends one unit's action: its Djinn on standby are taken off its set, the
/// Djinn a summon used begin to recover, and the unit takes its summon's power
/// boost, the haunt's damage, poison and the Grim Reaper.
void BattleUnit_ProcessActionEnd(struct BattlePlan *plan) {
  u32 unitId;
  struct Unit *unit;
  s32 battleContinues;
  s32 counts[4];
  struct DjinnRecoveryList *list;
  s32 strongestElement;
  s32 recoveryTurn;

  unitId = plan->unitId;
  battleContinues = 0;
  unit = Unit_Get(unitId);
  if (unitId <= ALLY_MAX ||
      ((struct BattleWork *)Ram_HeapSlots->battleWork)->isLink != 0) {
    list = &Djinn_GetRecoveryTableFar(unitId > ALLY_MAX)->list;
    for (strongestElement = 0; strongestElement < list->count;
         strongestElement++) {
      if (list->entries[strongestElement].unitId == unitId &&
          list->entries[strongestElement].turns == -1) {
        Djinn_PutOnStandbyFar(unitId, list->entries[strongestElement].element,
                              list->entries[strongestElement].index);
      }
    }
  }
  if (BattleUnit_ListLiving(1, 0) != 0 && BattleUnit_ListLiving(2, 0) != 0) {
    battleContinues = 1;
  }
  {
    s32 i;
    s32 owner;
    struct DjinnRecoveryList *pending;

    pending =
        &Djinn_GetRecoveryTableFar(
             unitId > ALLY_MAX &&
             ((struct BattleWork *)Ram_HeapSlots->battleWork)->isLink != 0)
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
    struct Unit *before;
    s32 i;

    before = Heap_AllocEwram(sizeof(struct Unit));
    Iwram_CopyWordsEntry(before, unit, sizeof(struct Unit));
    for (i = 0; i <= 3; i++) {
      if (counts[i] > 4)
        counts[i] = 4;
      if (unit->summonBoost[i] < counts[i])
        unit->summonBoost[i] = counts[i];
    }
    Unit_CalculateFar(unitId);
    for (i = 0; i <= 3; i++) {
      s32 gain = unit->element[i].power - before->element[i].power;

      if (gain > 0) {
        BattlePlayback_Reset();
        BattlePlayback_SchedulePhase(25);
        BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
        BattlePlayback_Push(PLAYBACK_NUMBER, gain);
        BattlePlayback_Push(PLAYBACK_SOUND, 175);
        // Messages 3285 to 3288, 3232, 3240, 3185, 3191 and 3188 in English;
        // all but MsgGoesDown still need their names in the PO catalogs.
        BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)MsgEarthPowerUp + i);
        BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, unitId);
        Sound_PlayFar(SE_POWER_UP);
        Actor_SelectAnimationFar(BattleObject_Get(unitId)->object, 3);
        Actor_SetAnimationSpeedFar(
            (struct Actor *)BattleObject_Get(unitId)->object, 32);
        ElementGlow_Play(unitId, i, 2, -1);
        BattlePlayback_WaitForReady();
      }
    }
    Heap_Release(before);
  }
  if (battleContinues != 0 && BattleObject_Get(unitId)->object != 0) {
    s8 *poison;
    u8 *reaper;

    BattlePlayback_Reset();
    if (plan->hauntDamage != 0) {
      BattlePlayback_Push(PLAYBACK_HIT, unitId);
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
      BattlePlayback_Push(PLAYBACK_NUMBER, plan->hauntDamage);
      BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)MsgCurseDamage);
      if (Unit_AddHpFar(unitId, -plan->hauntDamage) == 0) {
        if (BattleUnit_KeepsOneHp(unitId)) {
          Unit_AddHpFar(unitId, 1);
          BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, unitId);
        } else {
          BattlePlayback_Push(PLAYBACK_DEFEAT, unitId);
          BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
          BattlePlayback_Push(PLAYBACK_MESSAGE,
                              unitId <= ALLY_MAX ? (u32)MsgGoesDown
                                                 : (u32)MsgStrengthExhausted);
        }
      } else {
        BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, unitId);
      }
    }
    BattlePlayback_RunAll();
    BattlePlayback_Reset();
    poison = &unit->poison;
    if (*poison != 0) {
      s32 damage = unit->hpMax * *poison / 10;
      struct BattleWork *state = Ram_HeapSlots->battleWork;

      BattlePlayback_Push(PLAYBACK_HIT, unitId);
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
      BattlePlayback_Push(PLAYBACK_NUMBER, damage);
      BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)MsgPoisonDamage);
      if (*poison != 0) {
        state->events.pendingCue = 134;
      } else {
        state->events.pendingCue = 133;
      }
      if (Unit_AddHpFar(unitId, -damage) == 0) {
        if (BattleUnit_KeepsOneHp(unitId)) {
          Unit_AddHpFar(unitId, 1);
          BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, unitId);
        } else {
          BattlePlayback_Push(PLAYBACK_DEFEAT, unitId);
          BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
          BattlePlayback_Push(PLAYBACK_MESSAGE,
                              unitId <= ALLY_MAX ? (u32)MsgGoesDown
                                                 : (u32)MsgStrengthExhausted);
        }
      } else {
        BattlePlayback_Push(PLAYBACK_REFRESH_UNIT, unitId);
      }
    }
    BattlePlayback_RunAll();
    BattlePlayback_Reset();
    reaper = &unit->grimReaper;
    if (*reaper != 0 && !BattleUnit_KeepsOneHp(unitId) && --*reaper == 0 &&
        Unit_AddHpFar(unitId, 0xc0000000) == 0) {
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
      BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)MsgReaperCalls);
      BattlePlayback_Push(PLAYBACK_HIT, unitId);
      BattlePlayback_Push(PLAYBACK_DEFEAT, unitId);
      BattlePlayback_Push(PLAYBACK_UNIT_NAME, unitId);
      if (unitId <= ALLY_MAX) {
        BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)MsgReaperCalls - 3);
      } else {
        BattlePlayback_Push(PLAYBACK_MESSAGE, (u32)MsgReaperCalls + 3);
      }
    }
    BattlePlayback_RunAll();
  }
  if (BattleObject_Get(unitId)->object != 0)
    Unit_CalculateFar(unitId);
}
