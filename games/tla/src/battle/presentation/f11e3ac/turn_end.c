/// @file
/// The end of a turn: each unit's modifiers and conditions counted down, the
/// Djinn recovering, and the extras an action applies.
// Part of f11e3ac.c.

#include "../f11e3ac_internal.h"

s32 BattleUnit_RollCounterEarlyEnd(s32 unitId, s32 turns, s32 bias) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);

  if (turns <= 5) {
    s32 threshold = ((state->luck * 3 - turns * 5) + bias) * 0x28f;
    if (threshold >= (Battle_RandomFar() & 0xffff))
      return 1;
  }
  return 0;
}

s32 BattleUnit_CountDownAttackModifier(s32 unitId) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);

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

s32 BattleUnit_CountDownDefenseModifier(s32 unitId) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);

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

s32 BattleUnit_CountDownResistanceModifier(s32 unitId) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);

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

s32 BattleUnit_CountDownDelusion(s32 unitId) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);
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

s32 BattleUnit_CountDownConfusion(s32 unitId) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);
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

s32 BattleUnit_CountDownCharm(s32 unitId) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);
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

s32 BattleUnit_CountDownStun(s32 unitId) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);
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

s32 BattleUnit_CountDownSleep(s32 unitId) {
  struct BattleUnit *state = Unit_Get__f11f66c(unitId);
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

s32 Battle_AdvanceCounterAndCheckChance(s32 id) {
  s32 next;
  s32 expired;
  u8 remaining;
  s32 turns;
  s32 packed;
  struct BattleUnit *unit;

  unit = Unit_Get__f11f66c(id);
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

s32 sub_08124af8(s32 id) {
  struct BattleUnit *unit = Unit_Get__f11f66c(id);
  u8 *count = &unit->refrain;

  if (*count != 0) {
    s32 max;
    s32 before;
    s32 hp;
    s32 gain;

    BattlePlayback_Reset();
    hp = unit->hp;
    max = unit->maxHp;
    before = hp;
    hp += max * (*count + 1) / 10;
    if (hp > max)
      hp = max;
    gain = hp - before;
    if (gain != 0) {
      BattleEvent_Push(0, id);
      if (hp == unit->maxHp) {
        BattleEvent_Push(4, (u32)&MsgHpFull);
      } else {
        {
          u32 opcode = 1;

          /* FAKEMATCH(barrier): an empty asm on opcode, so its 1 is set
           * before gain is copied to r1; as BattleEvent_Push(1, gain), 2 of 74
           * instructions differ in tla-en. */
          asm volatile("" : "+l"(opcode));
          BattleEvent_Push(opcode, gain);
        }
        BattleEvent_Push(4, (u32)&MsgHpRecover);
      }
      unit->hp = hp;
      Unit_UpdateRatiosFar(id);
    }
    BattleEvent_DispatchQueued();
    if (--unit->refrain == 0)
      return 1;
  }
  return 0;
}

s32 BattleUnit_CountDownReflect(s32 id) {
  u8 *value = (u8 *)&((struct BattleUnit *)Unit_Get__f11f66c(id))->reflect;
  if (*value != 0) {
    (*value)--;
    if (*value == 0) {
      return 1;
    }
  }
  return 0;
}

s32 BattleUnit_CountDownAgilityModifier(s32 id) {
  struct BattleUnit *unit = Unit_Get__f11f66c(id);
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

s32 DjinnRecovery_CountDown(void) {
  struct PlacementList *list;
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
          ((struct BattleUnit *)Unit_Get__f11f66c(timedEntry->unitId))->hp !=
              0) {
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

s32 DjinnRecovery_CountDownTwenty(void) {
  s32 turns;

  turns = 19;
  do {
    turns--;
    DjinnRecovery_CountDown();
  } while (turns >= 0);
  return 0;
}

void Func_08124cc0(void) {}

/// Counts down one side's Djinn recovery at the turn's end: an owner must be in
/// the battle; each Djinni that comes back plays its event (its icon, sound
/// 175, the flash and its message) before it is Set. The message is
/// "... is set to ...!" Returns 1 when a count fell, 2 when one was Set, else
/// 0.
// ☀️'s DjinnRecovery_CountDown with ⚓️'s presentation.
s32 Func_08124cc4(s32 side) {
  struct PlacementList *list;
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
      if (timedEntry->turns > 0 &&
          BattleParty_IsUnitListed(timedEntry->unitId) &&
          ((struct BattleUnit *)Unit_Get__f11f66c(timedEntry->unitId))->hp !=
              0) {
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

        if (BattleParty_IsUnitListed(id)) {
          u8 element;

          result = 2;
          BattlePlayback_Reset();
          BattlePlayback_SchedulePhase(30);
          BattleEvent_Push(0, id);
          BattleEvent_Push(3, entry->element * 20 + entry->index + 300);
          BattleEvent_Push(14, 175);
          BattleEvent_Push(10, 0);
          BattleEvent_Push(4, (u32)&MsgDjinnIsSet);
          BattleEvent_Push(11, id);
          if (BattleParty_IsUnitListed(id)) {
            Actor_SetMode((struct Actor *)BattleObject_Get(id)->object, 3);
            Actor_SetAnimationSpeedFar(
                (struct Actor *)BattleObject_Get(id)->object, 32);
          }
          element = entry->element;
          Djinn_ActivateFar(id, element, entry->index);
          DjinnRecovery_RemoveFar(id, entry->element, entry->index);
          Unit_CalculateFar(id);
          if (BattleParty_IsUnitListed(id))
            Func_08127308(id, element, 3, 0);
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

struct TurnEndList {
  struct DjinnRecoveryEntry entries[72];
  s32 count;
};

struct TurnEndTable {
  u8 padding[8];
  struct TurnEndList list;
};

struct BattleGroupOrder {
  s32 group[2];
};

extern const struct BattleGroupOrder Data_081297c4;

void Func_0812628c(s32 mode, s32 background, s32 value);

#define ANNOUNCE_END(test, message)                                            \
  if (test(unitId) != 0) {                                                     \
    BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));             \
    MessageArgument_PushFar(unitId, 1);                                        \
    Message_ShowAndWaitCoreFar(message);                                       \
    BattleMessage_WaitForAdvance();                                            \
  }

#define ANNOUNCE_END_POSE(test, message)                                       \
  if (test(unitId) != 0) {                                                     \
    BattleUnit_BuildStatusFlags(unitId, BattleObject_Get(unitId));             \
    MessageArgument_PushFar(unitId, 1);                                        \
    BattleObject_UpdatePose(unitId);                                           \
    Message_ShowAndWaitCoreFar(message);                                       \
    BattleMessage_WaitForAdvance();                                            \
  }

void Func_08124e20(void) {
  struct BattleWork *battle;
  s32 side;
  s32 sideCount;
  s32 count;
  s32 recoveryIndex;
  struct TurnEndList *list;
  struct DjinnRecoveryEntry *timedEntry;
  struct DjinnRecoveryEntry *expiredEntry;

  battle = Ram_HeapSlots->battleWork;
  sideCount = (battle->twoSided != 0) + 1;
  for (side = 0; side < sideCount; side++) {
    list = &((struct TurnEndTable *)Djinn_GetRecoveryTableFar(side))->list;
    recoveryIndex = 0;
    timedEntry = list->entries;
    if (recoveryIndex < list->count) {
      do {
        if (timedEntry->turns > 0 &&
            BattleObject_Get(timedEntry->unitId) != 0 &&
            ((struct BattleUnit *)Unit_Get__f11f66c(timedEntry->unitId))->hp !=
                0) {
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
            BattleEvent_Push(0, unitId);
            BattleEvent_Push(3, expiredEntry->element * 20 +
                                    expiredEntry->index + 300);
            BattleEvent_Push(14, 175);
            BattleEvent_Push(10, 0);
            BattleEvent_Push(4, (u32)&MsgDjinnIsSet);
            BattleEvent_Push(11, unitId);
            Sound_PlayFar(212);
            if (BattleParty_IsUnitListed(unitId)) {
              Actor_SetMode(BattleObject_Get(unitId)->object, 3);
              Actor_SetAnimationSpeedFar(
                  (struct Actor *)BattleObject_Get(unitId)->object, 32);
            }
            element = expiredEntry->element;
            Djinn_ActivateFar(unitId, element, expiredEntry->index);
            DjinnRecovery_RemoveFar(unitId, expiredEntry->element,
                                    expiredEntry->index);
            Unit_CalculateFar(unitId);
            if (BattleParty_IsUnitListed(unitId))
              Func_08127308(unitId, element, 3, 0);
            BattlePlayback_WaitForReady();
          }
        } else {
          expiredEntry++;
          recoveryIndex++;
        }
      } while (recoveryIndex < list->count);
    }
  }
  Func_0812628c(2, *(u16 *)&Ram_HeapSlots->battleWork->unk670[0], 0);
  {
    s32 *groupList;
    struct BattleGroupOrder order = Data_081297c4;
    u16 unitIds[16];
    s32 group;
    s32 unitIndex;

    if (battle->twoSided != 0 && battle->linkSide != 0) {
      order.group[0] = 2;
      order.group[1] = 1;
    }
    groupList = order.group;
    for (group = 0; group <= 1; group++) {
      count = BattleParty_ListActorIds(groupList[group], unitIds);
      if (groupList[group] == 1)
        count += BattleParty_PrepareReserveOwners(unitIds + count);
      for (unitIndex = 0; unitIndex < count; unitIndex++) {
        s32 hpAmount;
        s32 ppAmount;
        s32 unitId = unitIds[unitIndex];
        struct BattleUnit *unit = Unit_Get__f11f66c(unitId);

        unit->equipBonus[1] = 0;
        if (unit->ready != 0) {
          unit->ready--;
        }
        if (unit->hp != 0) {
          if (unit->hpRegen != 0 && unit->hp != unit->maxHp) {
            hpAmount = unit->hpRegen;
            switch (unit->enemyId) {
            case 91:
            case 92:
            case 93:
            case 94:
            case 95:
            case 96:
            case 97:
            case 218:
              hpAmount *= 10;
              break;
            }
            if (unit->hp + hpAmount > unit->maxHp) {
              hpAmount = unit->maxHp - unit->hp;
            }
            Unit_AddHpFar(unitId, hpAmount);
            MessageArgument_PushFar(unitId, 1);
            MessageArgument_PushFar(hpAmount, 5);
            if (unit->hp == unit->maxHp) {
              Message_ShowAndWaitCoreFar((s32)&MsgHpFull);
            } else {
              Message_ShowAndWaitCoreFar((s32)&MsgHpRecover);
            }
            Sound_PlayFar(175);
            BattleMessage_WaitForAdvance();
          }
          if (unit->ppRegen != 0 && unit->pp != unit->maxPp) {
            ppAmount = unit->ppRegen;
            if (unit->pp + ppAmount > unit->maxPp) {
              ppAmount = unit->maxPp - unit->pp;
            }
            Unit_AddPpFar(unitId, ppAmount);
            MessageArgument_PushFar(unitId, 1);
            MessageArgument_PushFar(ppAmount, 5);
            if (unit->pp == unit->maxPp) {
              Message_ShowAndWaitCoreFar((s32)&MsgPpFull);
            } else {
              Message_ShowAndWaitCoreFar((s32)&MsgPpRecover);
            }
            Sound_PlayFar(175);
            BattleMessage_WaitForAdvance();
          }
        }
        ANNOUNCE_END(BattleUnit_CountDownAgilityModifier, MSG_AGI_NORMAL);
        ANNOUNCE_END(BattleUnit_CountDownAttackModifier, (s32)&MsgAtkNormal);
        ANNOUNCE_END(BattleUnit_CountDownDefenseModifier, (s32)&MsgDefNormal);
        ANNOUNCE_END(BattleUnit_CountDownResistanceModifier,
                     (s32)&MsgResNormal);
        ANNOUNCE_END(BattleUnit_CountDownDelusion, MSG_CURE_DELUSION);
        ANNOUNCE_END(BattleUnit_CountDownConfusion, MSG_CONSCIOUS);
        ANNOUNCE_END(BattleUnit_CountDownCharm, (s32)&MsgConscious2);
        ANNOUNCE_END_POSE(BattleUnit_CountDownStun, MSG_CURE_STUN);
        ANNOUNCE_END_POSE(BattleUnit_CountDownSleep, MSG_WAKES);
        ANNOUNCE_END(Battle_AdvanceCounterAndCheckChance, MSG_CURE_SEAL);
        ANNOUNCE_END(sub_08124af8, (s32)&MsgRefrainEnd);
        ANNOUNCE_END(BattleUnit_CountDownReflect, MSG_REFLECT_END);
      }
    }
  }
}

void Djinn_DeactivateFar(s32 unitId, s32 element, s32 djinni);

void Battle_ApplyActionExtras(s32 unitId, s32 all, s32 announce) {
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
  struct PlacementList *list;
  struct PlacementList *pending;

  shown = 0;
again:
  {
    count = 0;
    active = 0;
    for (element = 0; element <= 3; element++) {
      for (djinni = 0; djinni <= 19; djinni++) {
        if (Djinn_IsActiveFar(unitId, element, djinni)) {
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
          if (Djinn_IsReadyFar(unitId, element, djinni)) {
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
      Djinn_DeactivateFar(unitId, element, djinni);
      if (announce) {
        BattleEvent_Push(0, unitId);
        BattleEvent_Push(3, element * 20 + djinni + 300);
        BattleEvent_Push(4, (u32)&MsgDjinnOnStandby);
        return;
      }
    } else if (announce) {
      return;
    }

    list = &Djinn_GetRecoveryTableFar(unitId < 0 || unitId > 7)->list;
    for (i = 0; i < list->count; i++) {
      if (list->entries[i].element == element &&
          list->entries[i].index == djinni && list->entries[i].turns == -1)
        list->entries[i].turns = -2;
    }

    pending = &Djinn_GetRecoveryTableFar(unitId < 0 || unitId > 7)->list;
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
      BattleEvent_Push(0, unitId);
      if (!all) {
        s32 message;

        BattleEvent_Push(3, element * 20 + djinni + 300);
        message = (u32)&MsgDjinnDrained;
        if (unitId < 0 || unitId > 7)
          message++;
        BattleEvent_Push(4, message);
      } else {
        BattleEvent_Push(4, (u32)&MsgAllDjinnDrained);
      }
    }
    if (all)
      goto again;
  }
}

void BattleUnit_ProcessActionEnd(struct BattlePlan *plan) {
  u32 unitId;
  struct BattleUnit *unit;
  s32 battleContinues;
  s32 counts[4];
  struct PlacementList *list;
  s32 strongestElement;
  s32 recoveryTurn;

  unitId = plan->actorId;
  battleContinues = 0;
  unit = Unit_Get__f11f66c(unitId);
  if (unitId <= 7 ||
      ((struct BattleWork *)Ram_HeapSlots->battleWork)->twoSided != 0) {
    list = &Djinn_GetRecoveryTableFar(unitId > 7)->list;
    for (strongestElement = 0; strongestElement < list->count;
         strongestElement++) {
      if (list->entries[strongestElement].unitId == unitId &&
          list->entries[strongestElement].turns == -1) {
        Djinn_DeactivateFar(unitId, list->entries[strongestElement].element,
                            list->entries[strongestElement].index);
      }
    }
  }
  if (BattleParty_ListLivingUnits(1, 0) != 0 &&
      BattleParty_ListLivingUnits(2, 0) != 0) {
    battleContinues = 1;
  }
  {
    s32 i;
    s32 owner;
    struct PlacementList *pending;

    pending =
        &Djinn_GetRecoveryTableFar(
             unitId > 7 &&
             ((struct BattleWork *)Ram_HeapSlots->battleWork)->twoSided != 0)
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
    s32 i;

    before = Heap_AllocEwram(sizeof(struct BattleUnit));
    Iwram_CopyWords(before, unit, sizeof(struct BattleUnit));
    for (i = 0; i <= 3; i++) {
      if (counts[i] > 4)
        counts[i] = 4;
      if (unit->elementLevels[i] < counts[i])
        unit->elementLevels[i] = counts[i];
    }
    Unit_CalculateFar(unitId);
    for (i = 0; i <= 3; i++) {
      s32 gain = unit->element[i].power - before->element[i].power;

      if (gain > 0) {
        BattlePlayback_Reset();
        BattlePlayback_SchedulePhase(25);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
        BattleEvent_Push(BATTLE_EVENT_NUMBER, gain);
        BattleEvent_Push(BATTLE_EVENT_SOUND, 175);
        // Messages 3285 to 3288, 3232, 3240, 3185, 3191 and 3188 in English;
        // all but MsgGoesDown still need their names in the PO catalogs.
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgEarthPowerUp + i);
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, unitId);
        Sound_PlayFar(212);
        Actor_SetMode(BattleObject_Get(unitId)->object, 3);
        Actor_SetAnimationSpeedFar(
            (struct Actor *)BattleObject_Get(unitId)->object, 32);
        Func_08127308(unitId, i, 2, -1);
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
      BattleEvent_Push(BATTLE_EVENT_HIT, unitId);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, plan->hauntDamage);
      BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgCurseDamage);
      if (Unit_AddHpFar(unitId, -plan->hauntDamage) == 0) {
        if (BattleUnit_KeepsOneHp(unitId)) {
          Unit_AddHpFar(unitId, 1);
          BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, unitId);
        } else {
          BattleEvent_Push(BATTLE_EVENT_DEFEAT, unitId);
          BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
          BattleEvent_Push(BATTLE_EVENT_MESSAGE,
                           unitId <= 7 ? (u32)MsgGoesDown
                                       : (u32)MsgStrengthExhausted);
        }
      } else {
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, unitId);
      }
    }
    BattleEvent_DispatchQueued();
    BattlePlayback_Reset();
    poison = &unit->poison;
    if (*poison != 0) {
      s32 damage = unit->maxHp * *poison / 10;
      struct BattleWork *state = Ram_HeapSlots->battleWork;

      BattleEvent_Push(BATTLE_EVENT_HIT, unitId);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, damage);
      BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgPoisonDamage);
      if (*poison != 0) {
        state->events.pendingCue = 134;
      } else {
        state->events.pendingCue = 133;
      }
      if (Unit_AddHpFar(unitId, -damage) == 0) {
        if (BattleUnit_KeepsOneHp(unitId)) {
          Unit_AddHpFar(unitId, 1);
          BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, unitId);
        } else {
          BattleEvent_Push(BATTLE_EVENT_DEFEAT, unitId);
          BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
          BattleEvent_Push(BATTLE_EVENT_MESSAGE,
                           unitId <= 7 ? (u32)MsgGoesDown
                                       : (u32)MsgStrengthExhausted);
        }
      } else {
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, unitId);
      }
    }
    BattleEvent_DispatchQueued();
    BattlePlayback_Reset();
    reaper = &unit->grimReaper;
    if (*reaper != 0 && !BattleUnit_KeepsOneHp(unitId) && --*reaper == 0 &&
        Unit_AddHpFar(unitId, 0xc0000000) == 0) {
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
      BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgReaperCalls);
      BattleEvent_Push(BATTLE_EVENT_HIT, unitId);
      BattleEvent_Push(BATTLE_EVENT_DEFEAT, unitId);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, unitId);
      if (unitId <= 7) {
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgReaperCalls - 3);
      } else {
        BattleEvent_Push(BATTLE_EVENT_MESSAGE, (u32)MsgReaperCalls + 3);
      }
    }
    BattleEvent_DispatchQueued();
  }
  if (BattleObject_Get(unitId)->object != 0)
    Unit_CalculateFar(unitId);
}
