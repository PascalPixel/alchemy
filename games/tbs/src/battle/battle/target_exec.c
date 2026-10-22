/// @file
/// An ability applied to one target, BattleTarget_Execute: damage, healing and
/// its added effect.
// Part of battle.c.

#include "../battle_internal.h"

/// Returns 1 for an effect that still applies when the ability's damage has
/// just knocked its target out: the drains, which give the damage they dealt
/// back to the actor.
/// Called by BattleTarget_Execute before the added effect.
/// @param effect the ability's effect (enum AbilityEffect)
s32 BattleEffect_CheckDead(s32 effect) {
  switch (effect) {
  case ABILITY_EFFECT_DRAIN_HP:
  case ABILITY_EFFECT_DRAIN_PP:
  case ABILITY_EFFECT_DRAIN_HALF_HP:
  case ABILITY_EFFECT_PSY_LEECH:
    return 1;
  default:
    return 0;
  }
}

// The target's checks, effect and messages
/// A stat stage changes the stat by an eighth of it (Unit_Calculate), from half
/// at the lowest to one and a half at the highest.
#define STAT_STAGE_MIN -4
#define STAT_STAGE_MAX 4
/// Zephyr's Agility stage, beyond the usual highest: twice the Agility.
#define AGILITY_STAGE_DOUBLED 8
/// A Resistance stage adds 20 to the resistance to every element
/// (Unit_Calculate).
#define RESISTANCE_PER_STAGE 20

// The turns a stat change or a status lasts. A status, or a lowered stat but
// never a raised one or Agility, may end sooner at the rolls the end of every
// turn makes (BattleUnit_RollCounterEarlyEnd).
#define STAT_CHANGE_TURNS 7
#define AGILITY_CHANGE_TURNS 5
#define STATUS_TURNS 7
/// The Grim Reaper's countdown.
#define CURSE_TURNS 7

/// BattleUnit.psySeal: Bind's seal counts its seven turns in the low three
/// bits, and may end sooner; Luff's counts down 8 a turn, two turns that never
/// end early (Battle_AdvanceCounterAndCheckChance).
#define PSY_SEAL_BIND 7
#define PSY_SEAL_LUFF 16

/// The rate Battle_CalculatePsynergyPowerFar and Battle_CalculateRecoveryFar
/// scale by: 256 is 1.
#define FULL_RATE 256

/// A message operand that shows nothing: the event only waits for the player.
#define MESSAGE_NONE -1

/// Party unit ids run to ALLY_MAX; enemy units start at FIRST_ENEMY_UNIT.
#define IS_PARTY_UNIT(unitId) ((unitId) >= 0 && (unitId) <= ALLY_MAX)

// The helpers below are macros because the bytes need them so: written as
// inline functions they compile to different code.

/// Holds a stat stage within STAT_STAGE_MIN to STAT_STAGE_MAX.
#define CLAMP_STAT_STAGE(stage)                                                \
  {                                                                            \
    if ((stage) < STAT_STAGE_MIN)                                              \
      (stage) = STAT_STAGE_MIN;                                                \
    if ((stage) > STAT_STAGE_MAX)                                              \
      (stage) = STAT_STAGE_MAX;                                                \
  }

/// Pushes one message for a unit of the party and another for an enemy.
#define PUSH_SIDE_MESSAGE(unitId, partyMessage, enemyMessage)                  \
  {                                                                            \
    if (IS_PARTY_UNIT(unitId))                                                 \
      PUSH_MESSAGE(partyMessage);                                              \
    else                                                                       \
      PUSH_MESSAGE(enemyMessage);                                              \
  }

/// A guarding unit takes half the damage, or a tenth behind Total Defense.
#define REDUCE_BY_GUARD(damage, unit)                                          \
  {                                                                            \
    s32 guard = (unit)->guardLevel;                                            \
                                                                               \
    if (guard != 0) {                                                          \
      if (guard == GUARD_DEFEND)                                               \
        (damage) /= 2;                                                         \
      else                                                                     \
        (damage) /= 10;                                                        \
    }                                                                          \
  }

/// Applies the planned ability to one of its targets: first its damage or
/// healing, then its added effect (a status, a stat change, a revival...).
/// Nothing is shown here: each step queues battle events (the names and
/// numbers a message needs, the message, the target's reactions) that
/// BattleEvent_Playback then plays in order. The units themselves change at
/// once: Unit_CalculateFar recalculates a unit's stats from its stages, and
/// Unit_UpdateRatiosFar its HP and PP gauges after either changes.
///
/// Called for each target in turn by BattleEvent_Playback (an action with an
/// effect) and BattlePresentation_RunActorEntries (one without). Changes the
/// target's HP, PP, statuses, stat stages and turns, and its guard; the actor's
/// HP or PP for a drain, and plan->hauntDamage; adds an enemy to
/// gBattleWork->enemyIds when one is called; sets gBattleWork->challengeFlag
/// bit 1 when the party is challenged; queues the events that show it all.
/// It takes Battle_RandomFar rolls, and borrows a heap block for the target's
/// copy.
///
/// It runs in phases, each marked in the body:
/// 1. Setup: a copy of the target as it was; the target's distance from the
///    aimed place; the element's affinity (resisted, neutral or weak); the
///    actor's power in the element.
/// 2. The Djinn bonus roll, and whether the effect lands (hit).
/// 3. Calling an ally (Ally Search, Search for Help, Speed Surge).
/// 4. The effects that change the damage step: Ground, Torch, Severe Blow,
///    instant death, a self-destruct, Psy Drain.
/// 5. Damage and healing, by the ability kind (attack, PP damage, HP and PP
///    healing, Psynergy damage, an effect's damage).
/// 6. The added effect, by the ability's effect: cures, heals, stat changes,
///    revivals, statuses, drains, Break, the curse, guards.
/// 7. Waking from damage, the recalculated stats, the redraw, and the evil
///    spirit's share for a haunted actor.
///
/// @param plan the action under way: the actor, the ability, its element and
/// its targets, each with its distance from the aimed position, damage
/// multiplier, critical hit and any hit roll made in advance
/// @param slot which of the plan's targets (0 to plan->targetCount - 1)
#ifdef UBFIX
void BattleTarget_Execute(struct BattlePlan *plan, s32 slot) {
#else
s32 BattleTarget_Execute(struct BattlePlan *plan, s32 slot) {
#endif
  s32 distance; // steps from the aimed position, 0 to 5
  struct BattleAct *ability;
  struct BattleUnit *actor;
  s32 actorId;
  s32 abilityId;
  s32 elementBonus; // elemental power less resistance
  struct BattleWork *work;
  s32 halveDefense; // the attack counts half the target's Defense
  s32 multiplier;   // 0 on a miss, 2 or 3 for a double or triple blow
  s32 damageDealt;  // HP or PP the target lost (negative if healed)
  s32 leaveOneHp;   // the blow leaves the target 1 HP at most
  s32 hit;          // the ability's effect lands
  s32 critical;     // enum BattleCritical
  s32 skipDamage;   // no damage step: instant death replaces it
  s32 kind;         // the ability kind: enum AbilityKind
  s32 affinity;     // -1 a resisted element, 1 a weakness, else 0
  // A copy of the target as it was, to measure each stat's change; a heap
  // block freed at the end.
  struct BattleUnit *savedTarget;
  s32 elementPower; // the actor's power in the ability's element
  s32 targetId;
  s32 element;
  struct BattleUnit *target;
  s32 amount; // the damage, or the HP or PP restored
  s32 unitSize;

  //------------------------------------------------------------------
  // 1. Setup
  //------------------------------------------------------------------
  elementBonus = 0;
  work = gBattleWork;
  halveDefense = 0;
  damageDealt = 0;
  leaveOneHp = 0;
  skipDamage = 0;
  affinity = 0;
  unitSize = sizeof(struct BattleUnit);
  savedTarget = MemAlloc(unitSize);

  actorId = plan->actorId;
  targetId = plan->targetIds[slot];
  abilityId = plan->abilityId;
  element = plan->element;
  multiplier = plan->targetMultiplier[slot];
  critical = plan->targetCritical[slot];

  ability = BattleAction_Get(abilityId);
  actor = Unit_GetFar(actorId);
  target = Unit_GetFar(targetId);
  Iwram_CopyWords(savedTarget, target, unitSize);

  // The target's distance from the aimed position: every damage and
  // healing table falls off with it. An ability that reaches the whole side
  // treats every target as aimed at.
  if (ability->range != ABILITY_RANGE_ALL) {
    distance = plan->targetOffset[slot];
    if (distance < 0)
      distance = -distance;
  } else {
    distance = 0;
  }

  // The element's affinity: -1 when the target resists no element better
  // than this one, 1 when it resists none worse. It picks the damage
  // message ("damage.", "damage!" or "damage!!!").
  if (element != ELEMENT_NONE) {
    s32 resistance;
    s32 i;

    resistance = target->baseElement[element].resistance;
    for (i = 0; i < ELEMENT_COUNT; i++) {
      if (resistance < target->baseElement[i].resistance)
        break;
    }
    if (i == ELEMENT_COUNT)
      affinity = -1;

    for (i = 0; i < ELEMENT_COUNT; i++) {
      if (resistance > target->baseElement[i].resistance)
        break;
    }
    if (i == ELEMENT_COUNT)
      affinity = 1;
  }

  // The actor's power in the ability's element; items always use 100. (Here
  // and below the element is read from the plan again, as the bytes need.)
  if (plan->element >= ELEMENT_VENUS && plan->element <= ELEMENT_JUPITER &&
      plan->command != COMMAND_ITEM)
    elementPower = actor->element[plan->element].power;
  else
    elementPower = 100;

  //------------------------------------------------------------------
  // 2. The Djinn bonus, and whether the effect lands
  //------------------------------------------------------------------
  // A Djinni striking the target's weakness may earn the Djinn bonus: an
  // enemy it fells then gives more coins and experience and a better chance
  // of its item (BattleEnemy_RecordDefeat). The chance is the power less
  // the resistance, plus 30, in percent; the event's operand is unused.
  if (plan->command == COMMAND_DJINN &&
      (plan->element >= ELEMENT_VENUS && plan->element <= ELEMENT_JUPITER) &&
      affinity > 0) {
    s32 chance;

    chance = elementPower - target->element[plan->element].resistance;
    chance += 30;
    // Percent to a fraction of the 16-bit random range.
    chance *= 65536 / 100;
    if (chance > (Battle_RandomFar() & 0xffff))
      BattleEvent_Push(BATTLE_EVENT_DJINN_BONUS, 5);
  }

  // Whether the effect lands: a roll made when the action was planned, or
  // one made now, less likely the farther the target is.
  kind = ability->flags & ABILITY_KIND_MASK;
  {
    s32 plannedHit;

    plannedHit = plan->targetHit[slot];
    if (plannedHit == HIT_NOT_ROLLED)
      hit = Battle_RollEffectSuccessFar(actorId, targetId, element,
                                        ability->effect, gHitTable[distance]);
    else
      hit = plannedHit;
  }

  //------------------------------------------------------------------
  // 3. Calling an ally
  //------------------------------------------------------------------
  // Calling for help: an enemy calls another of its kind, or a random member
  // of its formation, into an empty enemy slot. Speed Surge is an enemy
  // splitting itself in two. It needs the roll, room among the enemy
  // sprite slots (Enemy_CheckRoom) and a free unit id.
  if (ability->effect == ABILITY_EFFECT_CALL_ALLY ||
      ability->effect == ABILITY_EFFECT_CALL_HELP) {
    s32 classId;
    s32 newUnitId;

    classId = actor->classId;
    newUnitId = Enemy_GetFreeSlot();
    if (ability->effect == ABILITY_EFFECT_CALL_HELP)
      classId = BattleFormation_GetRandomMember(work->enemyGroup);
    if (hit != 0 && Enemy_CheckRoom(classId) != 0 && newUnitId >= 0) {
      s32 suffix;

      // The newcomer's name letter; on the first duplicate the enemy
      // already there takes a letter too.
      suffix = BattleEnemy_ClaimNameLetter(classId, 1);
      if (suffix & 0x8000)
        BattleEnemy_LetterFirstOfClass(classId);
      BattleUnit_SetFar(newUnitId, classId, suffix & 0x7fff);
      // Fill a removed enemy's place in the list, or append the newcomer.
      {
        s32 i;

        for (i = 0; i < BATTLE_ENEMY_CAPACITY; i++) {
          if (work->enemyIds[i] == BATTLE_UNIT_REMOVED) {
            work->enemyIds[i] = newUnitId;
            break;
          }
          if (work->enemyIds[i] == BATTLE_UNIT_LIST_END) {
            work->enemyIds[i] = newUnitId;
            work->enemyIds[i + 1] = BATTLE_UNIT_LIST_END;
            break;
          }
        }
      }
      // Lay the enemies out again and put the newcomer's sprite in place
      // (the anchors are 16.16 fixed point).
      Enemy_Update();
      {
        struct BattleObj *obj;

        obj = BattleObject_Get(newUnitId);
        BattleObject_Make(obj, newUnitId, obj->anchorX / 0x10000,
                          obj->anchorZ / 0x10000);
      }
      BattleObject_Put();
      {
        s16 enemyIds[8];
        s32 enemyCount;
        s32 i;

        enemyCount = BattleEnemy_List(enemyIds);
        for (i = 0; i < enemyCount; i++)
          ObjMotRst((u16)enemyIds[i]);
      }
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, newUnitId);
      if (abilityId != ABILITY_SPEED_SURGE)
        PUSH_MESSAGE(MsgAppears);
      else
        PUSH_MESSAGE(MsgSplitOff);
    } else if (abilityId == ABILITY_SPEED_SURGE) {
      PUSH_MESSAGE(MsgSplitFail);
    } else {
      PUSH_MESSAGE(MsgNoOneCame);
    }
  }

  //------------------------------------------------------------------
  // 4. Effects that change how the damage step works
  //------------------------------------------------------------------
  if (hit != 0) {
    if (ability->effect == ABILITY_EFFECT_GROUND) {
      u32 i;

      // Ground holds only a target with an entry in this round's action
      // list. BattlePresentation_DispatchAction clears each entry once it has
      // played, so a target that has already acted is left alone; the auto
      // battle's BattlePresentation_RunAction clears none.
      hit = 0;
      for (i = 0; i < BATTLE_ACTION_CAPACITY; i++) {
        if (work->actions[i].unitId == targetId) {
          hit = 1;
          break;
        }
      }
    } else if (ability->effect == ABILITY_EFFECT_HALVE_DEFENSE) {
      halveDefense = 1;
    } else if (ability->effect == ABILITY_EFFECT_LEAVE_ONE_HP) {
      leaveOneHp = 1;
    } else if (ability->effect == ABILITY_EFFECT_INSTANT_DEATH) {
      skipDamage = 1;
    } else if (ability->effect == ABILITY_EFFECT_SELF_DESTRUCT) {
      // The actor leaves the battle (BattleObject_DestroyTemporaryObject).
      if (actor->hp != 0)
        BattleEvent_Push(BATTLE_EVENT_REMOVE_UNIT, actorId);
    } else if (ability->effect == ABILITY_EFFECT_DRAIN_PP) {
      // Psy Drain does PP damage, and misses a target with no PP.
      if (target->pp != 0)
        kind = ABILITY_KIND_PP_DAMAGE;
      else
        hit = 0;
    }
  }

  //------------------------------------------------------------------
  // 5. Damage and healing
  //------------------------------------------------------------------
  // A downed target is skipped, unless the ability revives.
  if (skipDamage == 0 &&
      (target->hp != 0 || Battle_IsReviveEffectFar(ability->effect) != 0)) {
    s32 pp;
    s32 newHp;
    s32 power;

    switch (kind) {
    // An attack: the actor's Attack against the target's Defense.
    case ABILITY_KIND_ATTACK:
    case ABILITY_KIND_ATTACK_MULTIPLIED: {
      s32 hp;
      s32 defense;
      s32 baseDefense;
      s32 pass;

      hp = target->hp;
      baseDefense = target->def;
      defense = baseDefense;
      if (halveDefense != 0)
        defense = (u32)baseDefense / 2;
      // This loop runs once, with pass 1, so its pass-0 code never runs:
      // the element bonus always applies and the critical hit message is
      // never shown.
      for (pass = 1; pass <= 1; pass++) {
        if (element != ELEMENT_NONE)
          elementBonus = elementPower - target->element[element].resistance;
        if (pass == 0)
          elementBonus = 0;
        power = ability->power;
        if (kind == ABILITY_KIND_ATTACK_MULTIPLIED)
          amount = Battle_CalculateAttackDamageFar(actor->atk, defense, 0,
                                                   elementBonus) *
                   power / 10;
        else
          amount = Battle_CalculateAttackDamageFar(actor->atk, defense, power,
                                                   elementBonus);
        amount *= multiplier;
        // A critical hit adds a quarter (a half for an ability's), then
        // the target's level / 5 + 6.
        if (critical != CRITICAL_NONE) {
          if (critical == CRITICAL_HIT)
            amount = amount * 5 / 4;
          else
            amount = amount * 3 / 2;
          amount += (target->level / 5) + 6;
          if (pass == 0) {
            BattleEvent_Push(BATTLE_EVENT_MARK, 0);
            {
              s32 message;

              // "A critical hit!", or "A bitter blow!" (the next message)
              // when it lands on the party.
              message = MsgCritical;
              if (IS_PARTY_UNIT(targetId))
                message += 1;
              BattleEvent_Push(BATTLE_EVENT_MESSAGE_NO_WAIT, message);
            }
          }
        }
        amount += Battle_RandomFar() & 3;
        REDUCE_BY_GUARD(amount, target);
        if (amount <= 0)
          amount = 1;
        if (leaveOneHp != 0) {
          if (amount < hp - 1) {
            amount = hp - 1;
            if (amount <= 0)
              amount = 1;
          }
        }
        // In the Djinn tutorial a Djinni's blow always leaves 1 HP.
        if (GameFlag_TestFar(FLAG_DJINN_LESSON) != 0 &&
            plan->command == COMMAND_DJINN && hp <= amount) {
          amount = hp - 1;
        }
      }
      // The flinch, then "<target> takes <amount> damage"; a fall queues the
      // defeat before its message, and a target still standing is redrawn.
      hp -= amount;
      BattleEvent_Push(BATTLE_EVENT_HIT, targetId);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, amount);
      // The affinity picks "damage.", "damage!" or "damage!!!".
      PUSH_MESSAGE((IS_PARTY_UNIT(targetId) ? MsgDmgEmphP : MsgDmgEmphE) +
                   affinity);

      if (hp <= 0) {
        BattleEvent_Push(BATTLE_EVENT_DEFEAT, targetId);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
        hp = 0;
        PUSH_SIDE_MESSAGE(targetId, MsgGoesDown, MsgFelled);
      } else
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, targetId);
      damageDealt = target->hp - hp;
      target->hp = hp;
      Unit_UpdateRatiosFar(targetId);
      break;
    }

    case ABILITY_KIND_PP_DAMAGE: {
      if (ability->power == 0)
        break;
      pp = target->pp;
      if (element != ELEMENT_NONE)
        elementBonus = elementPower - target->element[element].resistance;
      power = ability->power;
      amount = Battle_CalculatePsynergyPowerFar(power, elementBonus, FULL_RATE);
      amount = amount * gPpLossTable[distance] / 100;
      amount *= multiplier;
      REDUCE_BY_GUARD(amount, target);
      // Psy Drain takes no more PP than the target has.
      if (ability->effect == ABILITY_EFFECT_DRAIN_PP && amount > pp)
        amount = pp;

      BattleEvent_Push(BATTLE_EVENT_HIT, targetId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, amount);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
      PUSH_MESSAGE(IS_PARTY_UNIT(targetId) ? MsgPpLossP : MsgPpLossE);
      pp -= amount;
      if (pp <= 0)
        pp = 0;
      BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, targetId);
      damageDealt = target->pp - pp;
      target->pp = pp;
      Unit_UpdateRatiosFar(targetId);
      break;
    }

    // Healing, no further than the maximum: "HP fully restored" there, else
    // the amount restored.
    case ABILITY_KIND_HEAL_HP: {
      if (ability->power == 0)
        break;
      newHp = target->hp;

      // An ability with no element (an item) heals as if at power 100.
      power = ability->power;
      amount = Battle_CalculateRecoveryFar(
          power, element == ELEMENT_NONE ? 100 : elementPower, FULL_RATE);
      amount = amount * gHpRecoveryTable[distance] / 100;
      amount *= multiplier;
      amount += Battle_RandomFar() & 3;
      newHp += amount;
      if (newHp > target->maxHp) {
        newHp = target->maxHp;
        amount = newHp - target->hp;
      }
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
      if (newHp == target->maxHp)
        PUSH_MESSAGE(MsgHpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, amount);
        PUSH_MESSAGE(MsgHpRecover);
      }
      damageDealt = target->hp - newHp;
      target->hp = newHp;
      Unit_UpdateRatiosFar(targetId);
      break;
    }

    // Never reached, as the ability kind is 0 to 15: PP damage that, unlike
    // ABILITY_KIND_PP_DAMAGE, does not count as damage dealt.
    case -1: {
      if (ability->power == 0)
        break;
      pp = target->pp;
      if (element != ELEMENT_NONE)
        elementBonus = elementPower - target->element[element].resistance;
      power = ability->power;
      amount = Battle_CalculatePsynergyPowerFar(power, elementBonus, FULL_RATE);
      amount = amount * gPpDmgTable[distance] / 100;
      amount *= multiplier;
      REDUCE_BY_GUARD(amount, target);
      BattleEvent_Push(BATTLE_EVENT_HIT, targetId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, amount);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
      PUSH_MESSAGE(IS_PARTY_UNIT(targetId) ? MsgDmgP : MsgDmgE);
      pp -= amount;
      if (pp <= 0)
        pp = 0;
      BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, targetId);
      target->pp = pp;
      Unit_UpdateRatiosFar(targetId);
      break;
    }

    // HP damage from the ability's power, each ability kind with its own
    // fall-off by distance.
    case ABILITY_KIND_DAMAGE:
    case ABILITY_KIND_UNLEASHED_DAMAGE:
    case ABILITY_KIND_SUMMON: {
      s32 pass;

      if (ability->power == 0)
        break;
      newHp = target->hp;
      // Runs once, like the attack's loop.
      for (pass = 1; pass <= 1; pass++) {
        if (element != ELEMENT_NONE)
          elementBonus = elementPower - target->element[element].resistance;
        if (pass == 0)
          elementBonus = 0;
        power = ability->power;
        // A summon adds 3% of the target's max HP for each Djinni it costs.
        if (plan->command == COMMAND_SUMMON) {
          s32 maxHpPercent;

          switch (abilityId) {
          case ABILITY_VENUS:
          case ABILITY_MERCURY:
          case ABILITY_MARS:
          case ABILITY_JUPITER:
            maxHpPercent = 3;
            break;
          case ABILITY_RAMSES:
          case ABILITY_NEREID:
          case ABILITY_KIRIN:
          case ABILITY_ATALANTA:
            maxHpPercent = 6;
            break;
          case ABILITY_CYBELE:
          case ABILITY_NEPTUNE:
          case ABILITY_TIAMAT:
          case ABILITY_PROCNE:
            maxHpPercent = 9;
            break;
          case ABILITY_JUDGMENT:
          case ABILITY_BOREAS:
          case ABILITY_METEOR:
          case ABILITY_THOR:
            maxHpPercent = 12;
            break;
            // BUG: another id with summon-kind data leaves this uninitialized.
            // It has no known summon bonus, so use its base power alone.
#ifdef UBFIX
          default:
            maxHpPercent = 0;
            break;
#endif
          }
          power += target->maxHp * maxHpPercent / 100;
        }
        amount =
            Battle_CalculatePsynergyPowerFar(power, elementBonus, FULL_RATE);
        amount *= multiplier;
        // The fall-off by distance. (The ability kind is masked again, as the
        // bytes need.)
        switch (kind & ABILITY_KIND_MASK) {
        case ABILITY_KIND_DAMAGE:
          amount = amount * gHpDmgTable5[distance] / 100;
          break;
        case ABILITY_KIND_SUMMON:
          amount = amount * gHpDmgTable8[distance] / 100;
          break;
        case ABILITY_KIND_UNLEASHED_DAMAGE:
          amount = amount * gHpDmgTable6[distance] / 100;
          break;
        }
        amount += Battle_RandomFar() & 3;
        REDUCE_BY_GUARD(amount, target);
        // In the Djinn tutorial a summon always fells its target.
        if (GameFlag_TestFar(FLAG_DJINN_LESSON) != 0 &&
            plan->command == COMMAND_SUMMON && newHp > amount) {
          amount = newHp;
        }
      }
      BattleEvent_Push(BATTLE_EVENT_HIT, targetId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, amount);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
      PUSH_MESSAGE((IS_PARTY_UNIT(targetId) ? MsgDmgEmphP : MsgDmgEmphE) +
                   affinity);
      newHp -= amount;

      if (newHp <= 0) {
        BattleEvent_Push(BATTLE_EVENT_DEFEAT, targetId);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
        newHp = 0;
        PUSH_MESSAGE(IS_PARTY_UNIT(targetId) ? MsgGoesDown : MsgFelled);
      } else
        BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, targetId);
      damageDealt = target->hp - newHp;
      target->hp = newHp;
      Unit_UpdateRatiosFar(targetId);
      break;
    }

    case ABILITY_KIND_HEAL_PP: {
      if (ability->power == 0)
        break;
      pp = target->pp;
      power = ability->power;
      amount = Battle_CalculateRecoveryFar(
          power, element == ELEMENT_NONE ? 100 : elementPower, FULL_RATE);
      amount = amount * gPpRecoveryTable[distance] / 100;
      amount *= multiplier;
      pp += amount;
      if (pp > target->maxPp) {
        pp = target->maxPp;
        amount = pp - target->pp;
      }
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
      if (pp == target->maxPp)
        PUSH_MESSAGE(MsgPpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, amount);
        PUSH_MESSAGE(MsgPpRecover);
      }
      target->pp = pp;
      Unit_UpdateRatiosFar(targetId);
      break;
    }

    // The ability kinds with no damage or healing.
    case ABILITY_KIND_BLANK:
    case ABILITY_KIND_EFFECT_ONLY:
    case ABILITY_KIND_NO_POWER:
      break;

    // An effect ability: its power deals damage only if the effect lands.
    case ABILITY_KIND_EFFECT:
      if (hit != 0) {
        if (ability->power == 0)
          break;
        newHp = target->hp;
        if (element != ELEMENT_NONE)
          elementBonus = elementPower - target->element[element].resistance;
        power = ability->power;
        amount =
            Battle_CalculatePsynergyPowerFar(power, elementBonus, FULL_RATE);
        amount *= multiplier;
        amount = amount * gHpDmgTable[distance] / 100;
        REDUCE_BY_GUARD(amount, target);
        BattleEvent_Push(BATTLE_EVENT_HIT, targetId);
        BattleEvent_Push(BATTLE_EVENT_NUMBER, amount);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
        PUSH_MESSAGE(IS_PARTY_UNIT(targetId) ? MsgDmgP : MsgDmgE);
        newHp -= amount;

        if (newHp <= 0) {
          BattleEvent_Push(BATTLE_EVENT_DEFEAT, targetId);
          BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
          newHp = 0;
          PUSH_SIDE_MESSAGE(targetId, MsgGoesDown, MsgFelled);
        } else
          BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, targetId);
        damageDealt = target->hp - newHp;
        target->hp = newHp;
        Unit_UpdateRatiosFar(targetId);
        break;
      }
      BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, targetId);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
      PUSH_MESSAGE(MsgNoEffect);
      break;
    }
  }

  //------------------------------------------------------------------
  // 6. The added effect: a status, a stat change, a cure or a revival
  //------------------------------------------------------------------
  // It needs the effect to have landed on a target still standing, unless
  // it revives or drains. The target's name goes first, for the effect's
  // first message; a message uses up the arguments pushed before it, so each
  // later message pushes the name again.
  BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
  if ((Battle_IsReviveEffectFar(ability->effect) != 0 || target->hp != 0 ||
       BattleEffect_CheckDead(ability->effect) != 0) &&
      hit != 0) {
    switch (ability->effect) {
    // Tonic leaves confusion and charm, and ends sleep without a message.
    // Most of its messages push the name for the next after them; Restore
    // below clears the arguments and pushes the name before each instead.
    case ABILITY_EFFECT_CURE_ALL:
      if (target->delusion != 0) {
        target->delusion = 0;
        PUSH_MESSAGE(MsgCureDelusion);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
      }
      if (target->stun != 0) {
        target->stun = 0;
        BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
        PUSH_MESSAGE(MsgCureStun);
      }
      target->sleep = 0;
      if (target->psySeal != 0) {
        target->psySeal = 0;
        PUSH_MESSAGE(MsgCureSeal);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
      }
      if (target->grimReaper != 0) {
        target->grimReaper = 0;
        PUSH_MESSAGE(MsgCureCurse);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
      }
      if (target->haunt != 0) {
        target->haunt = 0;
        PUSH_MESSAGE(MsgCureSpirit);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
      }
      if (target->poison != 0) {
        PUSH_MESSAGE(MsgCurePoison);
        target->poison = 0;
      }
      BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      break;

    // Restore cures what Tonic does, but not the evil spirit or poison.
    case ABILITY_EFFECT_RESTORE:
      if (target->delusion != 0) {
        target->delusion = 0;
        BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
        PUSH_MESSAGE(MsgCureDelusion);
      }
      if (target->stun != 0) {
        target->stun = 0;
        BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
        PUSH_MESSAGE(MsgCureStun);
      }
      target->sleep = 0;
      if (target->psySeal != 0) {
        target->psySeal = 0;
        BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
        PUSH_MESSAGE(MsgCureSeal);
      }
      if (target->grimReaper == 0)
        break;
      target->grimReaper = 0;
      BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
      PUSH_MESSAGE(MsgCureCurse);
      break;

    case ABILITY_EFFECT_HEAL_HP_60:
    case ABILITY_EFFECT_HEAL_HP_30: {
      s32 oldHp;
      s32 maxHpBits;
      s32 maxHp;
      s32 newHp;
      s32 restored;
#ifndef UBFIX
      u16 *stat;
#endif

      // UB: a pointer walks from hp down past it to maxHp, reading each stat
      // twice, unsigned and signed, as the ROM does; the member reads behind
      // UBFIX share one load each.
#ifdef UBFIX
      oldHp = (u16)target->hp;
      newHp = target->hp;
      maxHpBits = (u16)target->maxHp;
      maxHp = target->maxHp;
#else
      stat = (u16 *)&target->hp;
      oldHp = *stat--;
      newHp = *(s16 *)(stat + 1);
      stat--;
      maxHpBits = *stat--;
      maxHp = *(s16 *)(stat + 1);
#endif
      if (ability->effect == ABILITY_EFFECT_HEAL_HP_60)
        newHp += maxHp * 60 / 100;
      else
        newHp += maxHp * 30 / 100;
      if (newHp > (s16)maxHpBits)
        newHp = (s16)maxHpBits;
      restored = newHp - (s16)oldHp;
      // Nothing to show if no HP came back, unless the ability heals.
      if (restored == 0 && kind != ABILITY_KIND_HEAL_HP)
        break;
      if (newHp == (s16)maxHpBits)
        PUSH_MESSAGE(MsgHpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, restored);
        PUSH_MESSAGE(MsgHpRecover);
      }
      target->hp = newHp;
      Unit_UpdateRatiosFar(targetId);
      break;
    }

    case ABILITY_EFFECT_HEAL_PP_7: {
      s32 oldPp;
      s32 maxPp;
      s32 newPp;
      s32 restored;

      newPp = target->pp;
      maxPp = target->maxPp;
      oldPp = newPp;
      newPp += maxPp * 7 / 100;
      if (newPp > maxPp)
        newPp = maxPp;
      restored = newPp - oldPp;
      if (restored == 0 && kind != ABILITY_KIND_HEAL_PP)
        break;
      if (newPp == maxPp)
        PUSH_MESSAGE(MsgPpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, restored);
        PUSH_MESSAGE(MsgPpRecover);
      }
      target->pp = newPp;
      Unit_UpdateRatiosFar(targetId);
      break;
    }

    // The stat changes. The amount shown is how much the stat itself moved,
    // measured against the saved copy once Unit_Calculate has recalculated it,
    // so a stage already at its limit shows 0; the turns start again either
    // way. Zephyr and Vine set Agility's stage outright.
    case ABILITY_EFFECT_AGILITY_UP:
      target->agilityStage = AGILITY_STAGE_DOUBLED;
      target->agilityTurns = AGILITY_CHANGE_TURNS;
      Unit_CalculateFar(targetId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->agi - savedTarget->agi);
      PUSH_MESSAGE(MsgAgiUp);
      break;

    case ABILITY_EFFECT_AGILITY_DOWN:
      target->agilityStage = STAT_STAGE_MIN;
      target->agilityTurns = AGILITY_CHANGE_TURNS;
      Unit_CalculateFar(targetId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, savedTarget->agi - target->agi);
      PUSH_MESSAGE(MsgAgiDown);
      break;

    case ABILITY_EFFECT_ATTACK_DOWN:
      target->attackStage -= 1;
      CLAMP_STAT_STAGE(target->attackStage);
      Unit_CalculateFar(targetId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, savedTarget->atk - target->atk);
      PUSH_MESSAGE(MsgAtkDown);
      target->attackTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_ATTACK_DOWN_2:
      target->attackStage -= 2;
      CLAMP_STAT_STAGE(target->attackStage);
      Unit_CalculateFar(targetId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, savedTarget->atk - target->atk);
      PUSH_MESSAGE(MsgAtkDown);
      target->attackTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_ATTACK_UP:
      target->attackStage += 1;
      CLAMP_STAT_STAGE(target->attackStage);
      Unit_CalculateFar(targetId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->atk - savedTarget->atk);
      PUSH_MESSAGE(MsgAtkUp);
      target->attackTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_ATTACK_UP_2:
      target->attackStage += 2;
      CLAMP_STAT_STAGE(target->attackStage);
      Unit_CalculateFar(targetId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->atk - savedTarget->atk);
      PUSH_MESSAGE(MsgAtkUp);
      target->attackTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_DEFENSE_DOWN:
      target->defenseStage -= 1;
      CLAMP_STAT_STAGE(target->defenseStage);
      Unit_CalculateFar(targetId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, savedTarget->def - target->def);
      PUSH_MESSAGE(MsgDefDown);
      target->defenseTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_DEFENSE_DOWN_2:
      target->defenseStage -= 2;
      CLAMP_STAT_STAGE(target->defenseStage);
      Unit_CalculateFar(targetId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, savedTarget->def - target->def);
      PUSH_MESSAGE(MsgDefDown);
      target->defenseTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_DEFENSE_UP:
      target->defenseStage += 1;
      CLAMP_STAT_STAGE(target->defenseStage);
      Unit_CalculateFar(targetId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->def - savedTarget->def);
      PUSH_MESSAGE(MsgDefUp);
      target->defenseTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_DEFENSE_UP_2:
      target->defenseStage += 2;
      CLAMP_STAT_STAGE(target->defenseStage);
      Unit_CalculateFar(targetId);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->def - savedTarget->def);
      PUSH_MESSAGE(MsgDefUp);
      target->defenseTurns = STAT_CHANGE_TURNS;
      break;

    // The revivals bring back a downed target with all, half or 80% of its
    // HP.
    case ABILITY_EFFECT_REVIVE:
      if (target->hp != 0)
        break;
      PUSH_MESSAGE(MsgRevived);
      target->hp = target->maxHp;
      Unit_UpdateRatiosFar(targetId);
      break;

    case ABILITY_EFFECT_REVIVE_HALF:
      if (target->hp != 0)
        break;
      PUSH_MESSAGE(MsgRevived);
      target->hp = target->maxHp / 2;
      Unit_UpdateRatiosFar(targetId);
      break;

    case ABILITY_EFFECT_REVIVE_80:
      if (target->hp != 0)
        break;
      PUSH_MESSAGE(MsgRevived);
      target->hp = target->maxHp * 8 / 10;
      Unit_UpdateRatiosFar(targetId);
      break;

    case ABILITY_EFFECT_CURE_POISON:
      if (target->poison != 0)
        PUSH_MESSAGE(MsgCurePoison);
      target->poison = 0;
      break;

    // A Resistance change needs no recalculation to show: it is the change
    // in stages times RESISTANCE_PER_STAGE.
    case ABILITY_EFFECT_RESISTANCE_DOWN:
      target->resistanceStage -= 1;
      CLAMP_STAT_STAGE(target->resistanceStage);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, (savedTarget->resistanceStage -
                                             target->resistanceStage) *
                                                RESISTANCE_PER_STAGE);
      PUSH_MESSAGE(MsgResDown);
      target->resistanceTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_RESISTANCE_DOWN_2:
      target->resistanceStage -= 2;
      CLAMP_STAT_STAGE(target->resistanceStage);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, (savedTarget->resistanceStage -
                                             target->resistanceStage) *
                                                RESISTANCE_PER_STAGE);
      PUSH_MESSAGE(MsgResDown);
      target->resistanceTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_RESISTANCE_UP:
      target->resistanceStage += 1;
      CLAMP_STAT_STAGE(target->resistanceStage);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, (target->resistanceStage -
                                             savedTarget->resistanceStage) *
                                                RESISTANCE_PER_STAGE);
      PUSH_MESSAGE(MsgResUp);
      target->resistanceTurns = STAT_CHANGE_TURNS;
      break;

    case ABILITY_EFFECT_RESISTANCE_UP_2:
      target->resistanceStage += 2;
      CLAMP_STAT_STAGE(target->resistanceStage);
      BattleEvent_Push(BATTLE_EVENT_NUMBER, (target->resistanceStage -
                                             savedTarget->resistanceStage) *
                                                RESISTANCE_PER_STAGE);
      PUSH_MESSAGE(MsgResUp);
      target->resistanceTurns = STAT_CHANGE_TURNS;
      break;

    // Poison does nothing to a poisoned target, deadly poison nothing to one
    // that already has it.
    case ABILITY_EFFECT_POISON:
      if (target->poison != 0)
        break;
      PUSH_MESSAGE(MsgPoisoned);
      target->poison = POISON_NORMAL;
      break;

    case ABILITY_EFFECT_DEADLY_POISON:
      if (target->poison > POISON_NORMAL)
        break;
      PUSH_MESSAGE(MsgVenom);
      target->poison = POISON_DEADLY;
      break;

    case ABILITY_EFFECT_DELUSION:
      PUSH_MESSAGE(MsgDelusion);
      target->delusion = STATUS_TURNS;
      break;

    case ABILITY_EFFECT_CONFUSION:
      PUSH_MESSAGE(MsgConfused);
      target->confusion = STATUS_TURNS;
      break;

    case ABILITY_EFFECT_CHARM: // "X's heart has been stolen!"
      PUSH_MESSAGE(MsgCharmed);
      target->charm = STATUS_TURNS;
      break;

    case ABILITY_EFFECT_STUN:
      PUSH_MESSAGE(MsgStunned);
      target->stun = STATUS_TURNS;
      break;

    case ABILITY_EFFECT_SLEEP:
      PUSH_MESSAGE(MsgAsleep);
      target->sleep = STATUS_TURNS;
      break;

    case ABILITY_EFFECT_SEAL:
      PUSH_SIDE_MESSAGE(targetId, MsgPsyBlock, MsgPsySeal);
      target->psySeal |= PSY_SEAL_BIND;
      break;

    case ABILITY_EFFECT_SEAL_2_TURNS:
      PUSH_SIDE_MESSAGE(targetId, MsgPsyBlock, MsgPsySeal);
      target->psySeal |= PSY_SEAL_LUFF;
      break;

    // Instant death: "X goes down!" on the party, Drown's "X suffocates!"
    // or "X's spirit has been drained!" on an enemy.
    case ABILITY_EFFECT_INSTANT_DEATH:
      BattleEvent_Push(BATTLE_EVENT_DEFEAT, targetId);
      if (target->side == BATTLE_UNIT_PARTY)
        PUSH_MESSAGE(MsgKoDown);
      else if (abilityId == ABILITY_DROWN)
        PUSH_MESSAGE(MsgSuffocate);
      else
        PUSH_MESSAGE(MsgSpiritDrain);
      target->hp = 0;
      Unit_UpdateRatiosFar(targetId);
      break;

    case ABILITY_EFFECT_REGENERATE:
      PUSH_MESSAGE(MsgRefrain);
      target->refrain = STATUS_TURNS;
      break;

    case ABILITY_EFFECT_REFLECT:
      PUSH_MESSAGE(MsgReflect);
      target->reflect = STATUS_TURNS;
      break;

    // The drains give the damage dealt back to the actor: Drain all of it
    // as HP, Sap half.
    case ABILITY_EFFECT_DRAIN_HP:
    case ABILITY_EFFECT_DRAIN_HALF_HP: {
      s32 newHp;

      newHp = actor->hp;
      amount = damageDealt;
      if (ability->effect == ABILITY_EFFECT_DRAIN_HALF_HP)
        amount /= 2;
      newHp += amount;
      if (newHp > actor->maxHp) {
        newHp = actor->maxHp;
        amount = newHp - actor->hp;
      }
      BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, actorId);
      if (newHp == actor->maxHp)
        PUSH_MESSAGE(MsgHpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, amount);
        PUSH_MESSAGE(MsgHpRecover);
      }
      actor->hp = newHp;
      Unit_UpdateRatiosFar(actorId);
      break;
    }

    // Psy Drain gives the PP taken.
    case ABILITY_EFFECT_DRAIN_PP: {
      s32 newPp;
      s32 drained;

      newPp = actor->pp;
      drained = damageDealt;
      newPp += drained;
      if (newPp > actor->maxPp) {
        newPp = actor->maxPp;
        drained = newPp - actor->pp;
      }
      BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);
      BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, actorId);
      if (newPp == actor->maxPp)
        PUSH_MESSAGE(MsgPpFull);
      else {
        BattleEvent_Push(BATTLE_EVENT_NUMBER, drained);
        PUSH_MESSAGE(MsgPpRecover);
      }
      actor->pp = newPp;
      Unit_UpdateRatiosFar(actorId);
      break;
    }

    // Psy Leech: a tenth of the damage as PP, as far as the target has it
    // and the actor has room.
    case ABILITY_EFFECT_PSY_LEECH:
      amount = damageDealt / 10;
      if (target->pp < amount)
        amount = target->pp;
      if (actor->pp + amount > actor->maxPp)
        amount = actor->maxPp - actor->pp;
      if (amount == 0)
        break;
      BattleEvent_Push(BATTLE_EVENT_NUMBER, amount);
      PUSH_SIDE_MESSAGE(targetId, MsgLeechTake, MsgLeechGain);
      Unit_AddPpFar(actorId, amount);
      break;

    // Break returns raised Attack, Defense, Resistance and Agility stages to
    // 0, leaving lowered ones, and takes away a summon's boost.
    case ABILITY_EFFECT_BREAK:
      if (target->attackStage > 0) {
        target->attackStage = 0;
        target->attackTurns = 0;
      }
      if (target->defenseStage > 0) {
        target->defenseStage = 0;
        target->defenseTurns = 0;
      }
      if (target->resistanceStage > 0) {
        target->resistanceStage = 0;
        target->resistanceTurns = 0;
      }
      if (target->agilityStage > 0)
        target->agilityStage = 0;
      target->elementLevels[ELEMENT_VENUS] = 0;
      target->elementLevels[ELEMENT_MERCURY] = 0;
      target->elementLevels[ELEMENT_MARS] = 0;
      target->elementLevels[ELEMENT_JUPITER] = 0;
      PUSH_MESSAGE(MsgBuffsReset);
      break;

    // The target now pays for the HP and PP it takes: see the evil spirit's
    // share at the end of this function.
    case ABILITY_EFFECT_HAUNT:
      PUSH_MESSAGE(MsgEvilSpirit);
      target->haunt = 1;
      break;

    // The Grim Reaper counts down to the target's death; each later curse
    // brings it a turn closer.
    case ABILITY_EFFECT_CURSE:
      if (target->grimReaper == 0) {
        PUSH_MESSAGE(MsgDeathCurse);
        target->grimReaper = CURSE_TURNS;
        break;
      }
      if (target->grimReaper <= 1)
        break;
      target->grimReaper -= 1;
      BattleEvent_Push(BATTLE_EVENT_NUMBER, target->grimReaper);
      PUSH_MESSAGE(MsgDeathCount);
      break;

    // Kite: the target acts twice in the next turn (the count drops at each
    // turn's end).
    case ABILITY_EFFECT_ACT_TWICE:
      PUSH_MESSAGE(MsgReadies);
      target->ready = 2;
      break;

    // The target rises to the challenge and won't flee; on the party, no one
    // may run ("No time to run!").
    case ABILITY_EFFECT_CHALLENGE:
      PUSH_MESSAGE(MsgChallenge);
      target->challenge = 1;
      if (IS_PARTY_UNIT(targetId))
        work->challengeFlag |= 2;
      break;

    // The target loses its next action (BattleCommand_BuildPlan).
    case ABILITY_EFFECT_GROUND:
      PUSH_MESSAGE(MsgImmobile);
      target->immobile = 1;
      break;

    // The guards: the message shows either way, but a guard is never lowered.
    case ABILITY_EFFECT_DEFEND:
      PUSH_MESSAGE(MsgAura);
      if (target->guardLevel > 0)
        break;
      target->guardLevel = GUARD_DEFEND;
      break;

    case ABILITY_EFFECT_TOTAL_DEFENSE:
      PUSH_MESSAGE(MsgAura2);
      if (target->guardLevel > GUARD_DEFEND)
        break;
      target->guardLevel = GUARD_TOTAL_DEFENSE;
      break;

    // Contain, Threaten and Tremble do nothing; the player only reads their
    // message.
    case ABILITY_EFFECT_THREATEN:
      PUSH_MESSAGE(MESSAGE_NONE);
      break;

    default:
      break;
    }
  }

  //------------------------------------------------------------------
  // 7. Waking from damage, and the recalculated stats
  //------------------------------------------------------------------
  BattleEvent_Push(BATTLE_EVENT_CLEAR_ARGUMENTS, 0);

  // Losing HP or PP wakes a sleeping target still standing one time in four,
  // unless it fell asleep this round: Battle_ProcessRoundEnd has not yet
  // counted it down from STATUS_TURNS.
  if (target->hp != 0 && target->sleep != 0 && target->sleep < STATUS_TURNS &&
      damageDealt > 0 && (Battle_RandomFar() & 3) == 0) {
    target->sleep = 0;
    BattleEvent_Push(BATTLE_EVENT_UNIT_NAME, targetId);
    PUSH_MESSAGE(MsgWakes);
  }
  MemFree(savedTarget);
  Unit_CalculateFar(targetId);
  // The party's status is drawn now, not through the queue.
  PartyStatus_DrawFar(gBattleWork->partyStatus);
  if (target->hp != 0)
    BattleEvent_Push(BATTLE_EVENT_REFRESH_UNIT, targetId);
  // A haunted actor pays, one time in four, for each target it took HP or PP
  // from: a quarter of that (at least 1), gathered in hauntDamage and taken
  // from its own HP at the end of its turn (BattleUnit_ProcessTurnEnd).
  if (actor->haunt != 0 && (Battle_RandomFar() & 3) == 0 && damageDealt > 0) {
    s32 share;

    share = damageDealt >> 2;
    if (share == 0)
      share = 1;
    plan->hauntDamage += share;
  }
}

/// Returns 1 for an ability that costs PP, and for Psy Drain, which costs
/// none.
/// Called by BattleCommand_BuildPlan (a cast's staging) and
/// BattleCommand_SelectAutomatic (an enemy's PP and seal checks).
u32 Ability_IsPsynergy(s32 abilityId) {
  u8 pp;

  if (abilityId == ABILITY_PSY_DRAIN)
    return 1;
  pp = BattleAction_Get(abilityId)->ppCost;
  // The sign bit of -pp | pp: 1 when pp is not 0, with no branch.
  return (u32)((0 - pp) | pp) >> 31;
}
