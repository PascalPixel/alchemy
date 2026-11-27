// An ability applied to one target, btAbilityApplyToTarget: damage, healing and its added effect.
// Part of battle.c.

#include "battle_internal.h"

// Returns 1 for an effect that still applies when the damage has just knocked the target out: the
// drains.
Bool btAbilityEffectAppliesToDowned(Sint32 effect)
{
    switch (effect)
    {
        case ABILITY_EFFECT_DRAIN_HP:
        case ABILITY_EFFECT_DRAIN_PP:
        case ABILITY_EFFECT_DRAIN_HALF_HP:
        case ABILITY_EFFECT_PSY_LEECH    : return TRUE;
        default                          : return FALSE;
    }
}

// The target's checks, effect and messages

// Zephyr's Agility stage, beyond the usual highest (STAT_STAGE_MAX, battle_targeting.c): twice the
// Agility.
#define AGILITY_STAGE_DOUBLED 8

// The turns a stat change or a status lasts; a status or a lowered stat may end sooner.
#define STAT_CHANGE_TURNS     7
#define AGILITY_CHANGE_TURNS  5
#define STATUS_TURNS          7
// The Grim Reaper's countdown.
#define CURSE_TURNS           7

// The rate gmBattleCalculatePsynergyPowerStub and gmBattleCalculateRecoveryStub scale by: 256 is 1.
#define FULL_RATE             256

// The element power an item, or an ability with no element, acts with.
#define ITEM_ELEMENT_POWER    100

// An unleashed Djinni gives the Djinn bonus (more coins and experience, and a likelier item, from
// the enemies that fall) against a target weakest to its element, with a chance in percent of the
// user's power less the target's resistance, plus this many.
#define DJINN_BONUS_CHANCE    30

// A message operand that shows nothing: the event only waits for the player.
#define MESSAGE_NONE          -1

// Party unit ids run to ALLY_MAX; enemy units start at FIRST_ENEMY_UNIT.
#define IS_PARTY_UNIT(unitId) ((unitId) >= 0 && (unitId) <= ALLY_MAX)

// Holds a stat stage within STAT_STAGE_MIN to STAT_STAGE_MAX.
#define CLAMP_STAT_STAGE(stage)                                                                    \
    {                                                                                              \
        if ((stage) < STAT_STAGE_MIN)                                                              \
            (stage) = STAT_STAGE_MIN;                                                              \
        if ((stage) > STAT_STAGE_MAX)                                                              \
            (stage) = STAT_STAGE_MAX;                                                              \
    }

// Pushes one message for a unit of the party and another for an enemy.
#define PUSH_SIDE_MESSAGE(unitId, partyMessage, enemyMessage)                                      \
    {                                                                                              \
        if (IS_PARTY_UNIT(unitId))                                                                 \
            PUSH_MESSAGE(partyMessage);                                                            \
        else                                                                                       \
            PUSH_MESSAGE(enemyMessage);                                                            \
    }

// A guarding unit takes half the damage, or a tenth behind Total Defense.
#define REDUCE_BY_GUARD(damage, unit)                                                              \
    {                                                                                              \
        Sint32 guard = (unit)->defendLevel;                                                        \
                                                                                                   \
        if (guard != 0)                                                                            \
        {                                                                                          \
            if (guard == DEFEND_NORMAL)                                                            \
                (damage) /= 2;                                                                     \
            else                                                                                   \
                (damage) /= 10;                                                                    \
        }                                                                                          \
    }

// Applies the action's ability to one of its targets, slot, in the seven steps of README.md's "How
// an ability resolves", queueing what playback will show.
UNREAD_RESULT btAbilityApplyToTarget(BATTLE_ACTION_RUN *plan, Sint32 slot)
{
    Sint32              distance; // steps from the aimed position, 0 to 5
    ABILITY_DEFINITION *ability;
    UNIT               *actor;
    Sint32              unitId;
    Sint32              abilityId;
    Sint32              elementBonus; // elemental power less resistance
    BATTLE_STATE       *battle;
    Sint32              halveDefense; // the attack counts half the target's Defense
    Sint32              multiplier;   // 0 on a miss, 2 or 3 for a double or triple blow
    Sint32              damageDealt;  // HP or PP the target lost (negative if healed)
    Sint32              leaveOneHp;   // the blow leaves the target 1 HP at most
    Sint32              hit;          // the ability's effect lands
    Sint32              critical;     // BATTLE_CRITICAL
    Sint32              skipDamage;   // no damage step: instant death replaces it
    Sint32              kind;         // the ability kind: ABILITY_FORMULA
    Sint32              affinity;     // -1 a resisted element, 1 a weakness, else 0
    // A copy of the target as it was, to measure each stat's change; a heap block freed at the end.
    UNIT               *savedTarget;
    Sint32              elementPower; // the user's power in the ability's element
    Sint32              targetId;
    Sint32              element;
    UNIT               *target;
    Sint32              amount; // the damage, or the HP or PP restored
    Sint32              unitSize;

    // 1. The target as it was, its distance and the element.
    elementBonus = 0;
    battle       = BattleStateWork;
    halveDefense = 0;
    damageDealt  = 0;
    leaveOneHp   = 0;
    skipDamage   = 0;
    affinity     = 0;
    unitSize     = sizeof(UNIT);
    savedTarget  = syHeapAllocate(unitSize);

    unitId       = plan->unitId;
    targetId     = plan->targetIds[slot];
    abilityId    = plan->abilityId;
    element      = plan->element;
    multiplier   = plan->targetMultiplier[slot];
    critical     = plan->targetCritical[slot];

    ability      = gmAbilityGetDirectStub(abilityId);
    actor        = gmUnitGetStub(unitId);
    target       = gmUnitGetStub(targetId);
    syWordsCopy(savedTarget, target, unitSize);

    // An ability that reaches the whole side treats every target as aimed at.
    if (ability->range != ABILITY_RANGE_ALL)
    {
        distance = plan->targetOffset[slot];
        if (distance < 0)
            distance = -distance;
    }
    else
    {
        distance = 0;
    }

    // The affinity picks the message: "damage.", "damage!" or "damage!!!".
    if (element != ELEMENT_NONE)
    {
        Sint32 resistance;
        Sint32 i;

        resistance = target->baseElement[element].resistance;
        for (i = 0; i < ELEMENT_COUNT; i++)
        {
            if (resistance < target->baseElement[i].resistance)
                break;
        }
        if (i == ELEMENT_COUNT)
            affinity = -1;

        for (i = 0; i < ELEMENT_COUNT; i++)
        {
            if (resistance > target->baseElement[i].resistance)
                break;
        }
        if (i == ELEMENT_COUNT)
            affinity = 1;
    }

    // The user's power in the ability's element; items always use ITEM_ELEMENT_POWER.
    if (plan->element >= ELEMENT_VENUS && plan->element <= ELEMENT_JUPITER
        && plan->command != COMMAND_ITEM)
        elementPower = actor->element[plan->element].power;
    else
        elementPower = ITEM_ELEMENT_POWER;

    // 2. The Djinn bonus, and whether the effect lands.
    if (plan->command == COMMAND_DJINN
        && (plan->element >= ELEMENT_VENUS && plan->element <= ELEMENT_JUPITER) && affinity > 0)
    {
        Sint32 chance;

        chance  = elementPower - target->element[plan->element].resistance;
        chance += DJINN_BONUS_CHANCE;
        // Percent to a fraction of the 16-bit random range.
        chance *= toFIXED(1) / 100;
        // The bonus is all or nothing: nothing reads the event's operand.
        if (chance > (gmBattleRandomStub() & 0xffff))
            btPlaybackPush(PLAYBACK_DJINN_BONUS, 5);
    }

    // Whether the effect lands: a roll made when the action was built, or one made now, less likely
    // the farther the target is.
    kind = ability->flags & ABILITY_FORMULA_MASK;
    {
        Sint32 plannedHit;

        plannedHit = plan->targetHit[slot];
        if (plannedHit == HIT_NOT_ROLLED)
            hit = gmBattleRollEffectSuccessStub(
                unitId, targetId, element, ability->effect, RangeFalloffEffectChance[distance]);
        else
            hit = plannedHit;
    }

    // 3. Calling an ally; Speed Surge is an enemy splitting in two.
    if (ability->effect == ABILITY_EFFECT_CALL_ALLY || ability->effect == ABILITY_EFFECT_CALL_HELP)
    {
        Sint32 enemyId;
        Sint32 newUnitId;

        enemyId   = actor->id;
        newUnitId = btEnemyGetFreeUnitId();
        if (ability->effect == ABILITY_EFFECT_CALL_HELP)
            enemyId = btFormationGetRandomMember(battle->formation);
        if (hit != 0 && btEnemyHasSpriteRoom(enemyId) != 0 && newUnitId >= 0)
        {
            Sint32 suffix;

            // The newcomer's name letter; on the first duplicate the enemy already there takes a
            // letter too.
            suffix = btEnemyClaimNameLetter(enemyId, 1);
            if (suffix & ENEMY_LETTER_FIRST_TOO)
                btEnemyLetterFirstOfKind(enemyId);
            gmBattleUnitSetStub(newUnitId, enemyId, suffix & ENEMY_LETTER_MASK);
            // Fill a removed enemy's place in the list, or append the newcomer.
            {
                Sint32 i;

                for (i = 0; i < ENEMY_BATTLE_SIZE; i++)
                {
                    if (battle->enemyIds[i] == BATTLE_UNIT_REMOVED)
                    {
                        battle->enemyIds[i] = newUnitId;
                        break;
                    }
                    if (battle->enemyIds[i] == BATTLE_UNIT_LIST_END)
                    {
                        battle->enemyIds[i]     = newUnitId;
                        battle->enemyIds[i + 1] = BATTLE_UNIT_LIST_END;
                        break;
                    }
                }
            }
            // Lay the enemies out again and put the newcomer's sprite in place (the anchors
            // are 16.16 fixed point).
            btPlacementRelayoutEnemies();
            {
                BATTLE_OBJECT *battleObject;

                battleObject = btObjectGet(newUnitId);
                btObjectMake(battleObject,
                             newUnitId,
                             battleObject->anchorX / toFIXED(1),
                             battleObject->anchorZ / toFIXED(1));
            }
            btObjectPut();
            {
                Sint16 enemyIds[8];
                Sint32 enemyCount;
                Sint32 i;

                enemyCount = btEnemyList(enemyIds);
                for (i = 0; i < enemyCount; i++)
                    btMotionReturnToSpot((Uint16)enemyIds[i]);
            }
            btPlaybackPush(PLAYBACK_UNIT_NAME, newUnitId);
            if (abilityId != ABILITY_SPEED_SURGE)
                PUSH_MESSAGE(MsgAppears);
            else
                PUSH_MESSAGE(MsgSplitOff);
        }
        else if (abilityId == ABILITY_SPEED_SURGE)
        {
            PUSH_MESSAGE(MsgSplitFail);
        }
        else
        {
            PUSH_MESSAGE(MsgNoOneCame);
        }
    }

    // 4. Effects that change how the damage step works.
    if (hit != 0)
    {
        if (ability->effect == ABILITY_EFFECT_GROUND)
        {
            Uint32 i;

            // Ground holds only a target that has not yet acted this turn.
            hit = 0;
            for (i = 0; i < BATTLE_ACTION_CAPACITY; i++)
            {
                if (battle->actions[i].unitId == targetId)
                {
                    hit = 1;
                    break;
                }
            }
        }
        else if (ability->effect == ABILITY_EFFECT_HALVE_DEFENSE)
        {
            halveDefense = 1;
        }
        else if (ability->effect == ABILITY_EFFECT_LEAVE_ONE_HP)
        {
            leaveOneHp = 1;
        }
        else if (ability->effect == ABILITY_EFFECT_INSTANT_DEATH)
        {
            skipDamage = 1;
        }
        else if (ability->effect == ABILITY_EFFECT_SELF_DESTRUCT)
        {
            // The user leaves the battle (btUnitRemoveEnemy).
            if (actor->hp != 0)
                btPlaybackPush(PLAYBACK_REMOVE_UNIT, unitId);
        }
        else if (ability->effect == ABILITY_EFFECT_DRAIN_PP)
        {
            // Psy Drain does PP damage, and misses a target with no PP.
            if (target->pp != 0)
                kind = ABILITY_FORMULA_PP_DAMAGE;
            else
                hit = 0;
        }
    }

    // 5. Damage and healing; a downed target is skipped unless the ability
    // revives.
    if (skipDamage == 0 && (target->hp != 0 || gmBattleIsReviveEffectStub(ability->effect) != 0))
    {
        Sint32 pp;
        Sint32 newHp;
        Sint32 power;

        switch (kind)
        {
            // An attack: the user's Attack against the target's Defense.
            case ABILITY_FORMULA_ATTACK:
            case ABILITY_FORMULA_ATTACK_MULTIPLIED:
            {
                Sint32 hp;
                Sint32 defense;
                Sint32 baseDefense;
                Sint32 pass;

                hp          = target->hp;
                baseDefense = target->defense;
                defense     = baseDefense;
                if (halveDefense != 0)
                    defense = (Uint32)baseDefense / 2;
                // The loop runs only its pass 1: the element bonus always applies and the critical
                // hit message is never shown.
                for (pass = 1; pass <= 1; pass++)
                {
                    if (element != ELEMENT_NONE)
                        elementBonus = elementPower - target->element[element].resistance;
                    if (pass == 0)
                        elementBonus = 0;
                    power = ability->power;
                    if (kind == ABILITY_FORMULA_ATTACK_MULTIPLIED)
                        amount = gmBattleCalculateAttackDamageStub(
                                     actor->attack, defense, 0, elementBonus)
                               * power / 10;
                    else
                        amount = gmBattleCalculateAttackDamageStub(
                            actor->attack, defense, power, elementBonus);
                    amount *= multiplier;
                    // A critical hit adds a quarter (a half for an ability's), then the target's
                    // level / 5 + 6.
                    if (critical != CRITICAL_NONE)
                    {
                        if (critical == CRITICAL_HIT)
                            amount = amount * 5 / 4;
                        else
                            amount = amount * 3 / 2;
                        amount += (target->level / 5) + 6;
                        if (pass == 0)
                        {
                            btPlaybackPush(PLAYBACK_MARK, 0);
                            {
                                Sint32 msg;

                                // "A critical hit!", or "A bitter blow!" (the next message) when it
                                // lands on the party.
                                msg = MsgCritical;
                                if (IS_PARTY_UNIT(targetId))
                                    msg++;
                                btPlaybackPush(PLAYBACK_MESSAGE_NO_WAIT, msg);
                            }
                        }
                    }
                    amount += gmBattleRandomStub() & 3;
                    REDUCE_BY_GUARD(amount, target);
                    if (amount <= 0)
                        amount = 1;
                    if (leaveOneHp != 0)
                    {
                        if (amount < hp - 1)
                        {
                            amount = hp - 1;
                            if (amount <= 0)
                                amount = 1;
                        }
                    }
                    // In the Djinn tutorial a Djinni's blow always leaves 1 HP.
                    if (gmFlagGetStub(FLAG_DJINN_LESSON) != 0 && plan->command == COMMAND_DJINN
                        && hp <= amount)
                    {
                        amount = hp - 1;
                    }
                }
                // The flinch, then "<target> takes <amount> damage"; a fall queues the defeat
                // before its message, and a target still standing is redrawn.
                hp -= amount;
                btPlaybackPush(PLAYBACK_HIT, targetId);
                btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                btPlaybackPush(PLAYBACK_NUMBER, amount);
                // The affinity picks "damage.", "damage!" or "damage!!!".
                PUSH_MESSAGE((IS_PARTY_UNIT(targetId) ? MsgDmgEmphP : MsgDmgEmphE) + affinity);

                if (hp <= 0)
                {
                    btPlaybackPush(PLAYBACK_DEFEAT, targetId);
                    btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                    hp = 0;
                    PUSH_SIDE_MESSAGE(targetId, MsgGoesDown, MsgFelled);
                }
                else
                    btPlaybackPush(PLAYBACK_REFRESH_UNIT, targetId);
                damageDealt = target->hp - hp;
                target->hp  = hp;
                gmUnitRecalculateGaugesStub(targetId);
                break;
            }

            case ABILITY_FORMULA_PP_DAMAGE:
            {
                if (ability->power == 0)
                    break;
                pp = target->pp;
                if (element != ELEMENT_NONE)
                    elementBonus = elementPower - target->element[element].resistance;
                power   = ability->power;
                amount  = gmBattleCalculatePsynergyPowerStub(power, elementBonus, FULL_RATE);
                amount  = amount * RangeFalloffPpLoss[distance] / 100;
                amount *= multiplier;
                REDUCE_BY_GUARD(amount, target);
                // Psy Drain takes no more PP than the target has.
                if (ability->effect == ABILITY_EFFECT_DRAIN_PP && amount > pp)
                    amount = pp;

                btPlaybackPush(PLAYBACK_HIT, targetId);
                btPlaybackPush(PLAYBACK_NUMBER, amount);
                btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                PUSH_MESSAGE(IS_PARTY_UNIT(targetId) ? MsgPpLossP : MsgPpLossE);
                pp -= amount;
                if (pp <= 0)
                    pp = 0;
                btPlaybackPush(PLAYBACK_REFRESH_UNIT, targetId);
                damageDealt = target->pp - pp;
                target->pp  = pp;
                gmUnitRecalculateGaugesStub(targetId);
                break;
            }

            // Healing, no further than the maximum: "HP fully restored" there, else the amount
            // restored.
            case ABILITY_FORMULA_HEAL_HP:
            {
                if (ability->power == 0)
                    break;
                newHp  = target->hp;

                // An ability with no element (an item) heals at ITEM_ELEMENT_POWER.
                power  = ability->power;
                amount = gmBattleCalculateRecoveryStub(
                    power, element == ELEMENT_NONE ? ITEM_ELEMENT_POWER : elementPower, FULL_RATE);
                amount  = amount * RangeFalloffHealHp[distance] / 100;
                amount *= multiplier;
                amount += gmBattleRandomStub() & 3;
                newHp  += amount;
                if (newHp > target->hpMax)
                {
                    newHp  = target->hpMax;
                    amount = newHp - target->hp;
                }
                btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                if (newHp == target->hpMax)
                    PUSH_MESSAGE(MsgHpFull);
                else
                {
                    btPlaybackPush(PLAYBACK_NUMBER, amount);
                    PUSH_MESSAGE(MsgHpRecover);
                }
                damageDealt = target->hp - newHp;
                target->hp  = newHp;
                gmUnitRecalculateGaugesStub(targetId);
                break;
            }

            // Never reached, as the ability kind is 0 to 15: PP damage that, unlike
            // ABILITY_FORMULA_PP_DAMAGE, does not count as damage dealt.
            case -1:
            {
                if (ability->power == 0)
                    break;
                pp = target->pp;
                if (element != ELEMENT_NONE)
                    elementBonus = elementPower - target->element[element].resistance;
                power   = ability->power;
                amount  = gmBattleCalculatePsynergyPowerStub(power, elementBonus, FULL_RATE);
                amount  = amount * RangeFalloffPpDamage[distance] / 100;
                amount *= multiplier;
                REDUCE_BY_GUARD(amount, target);
                btPlaybackPush(PLAYBACK_HIT, targetId);
                btPlaybackPush(PLAYBACK_NUMBER, amount);
                btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                PUSH_MESSAGE(IS_PARTY_UNIT(targetId) ? MsgDmgP : MsgDmgE);
                pp -= amount;
                if (pp <= 0)
                    pp = 0;
                btPlaybackPush(PLAYBACK_REFRESH_UNIT, targetId);
                target->pp = pp;
                gmUnitRecalculateGaugesStub(targetId);
                break;
            }

            // HP damage from the ability's power, each ability kind with its own fall-off by
            // distance.
            case ABILITY_FORMULA_DAMAGE:
            case ABILITY_FORMULA_ENEMY_DAMAGE:
            case ABILITY_FORMULA_SUMMON:
            {
                Sint32 pass;

                if (ability->power == 0)
                    break;
                newHp = target->hp;
                // Runs once, like the attack's loop.
                for (pass = 1; pass <= 1; pass++)
                {
                    if (element != ELEMENT_NONE)
                        elementBonus = elementPower - target->element[element].resistance;
                    if (pass == 0)
                        elementBonus = 0;
                    power = ability->power;
                    // A summon adds 3% of the target's max HP for each Djinni it costs.
                    if (plan->command == COMMAND_SUMMON)
                    {
                        Sint32 maxHpPercent;

                        switch (abilityId)
                        {
                            case ABILITY_VENUS:
                            case ABILITY_MERCURY:
                            case ABILITY_MARS:
                            case ABILITY_JUPITER : maxHpPercent = 3; break;
                            case ABILITY_RAMSES  :
                            case ABILITY_NEREID  :
                            case ABILITY_KIRIN   :
                            case ABILITY_ATALANTA: maxHpPercent = 6; break;
                            case ABILITY_CYBELE  :
                            case ABILITY_NEPTUNE :
                            case ABILITY_TIAMAT  :
                            case ABILITY_PROCNE  : maxHpPercent = 9; break;
                            case ABILITY_JUDGMENT:
                            case ABILITY_BOREAS:
                            case ABILITY_METEOR:
                            case ABILITY_THOR:
                                maxHpPercent = 12;
                                break;
                                // BUG: another id with summon-kind data leaves this uninitialized.
                                // It has no known summon bonus, so use its base power alone.
#if FIX_UNINITIALIZED_READ
                            default: maxHpPercent = 0; break;
#endif
                        }
                        power += target->hpMax * maxHpPercent / 100;
                    }
                    amount  = gmBattleCalculatePsynergyPowerStub(power, elementBonus, FULL_RATE);
                    amount *= multiplier;
                    // The fall-off by distance. FAKEMATCH(dead): the ability kind masked again.
                    switch (kind & ABILITY_FORMULA_MASK)
                    {
                        case ABILITY_FORMULA_DAMAGE:
                            amount = amount * RangeFalloffPsynergyDamage[distance] / 100;
                            break;
                        case ABILITY_FORMULA_SUMMON:
                            amount = amount * RangeFalloffSummonDamage[distance] / 100;
                            break;
                        case ABILITY_FORMULA_ENEMY_DAMAGE:
                            amount = amount * RangeFalloffEnemyDamage[distance] / 100;
                            break;
                    }
                    amount += gmBattleRandomStub() & 3;
                    REDUCE_BY_GUARD(amount, target);
                    // In the Djinn tutorial a summon always fells its target.
                    if (gmFlagGetStub(FLAG_DJINN_LESSON) != 0 && plan->command == COMMAND_SUMMON
                        && newHp > amount)
                    {
                        amount = newHp;
                    }
                }
                btPlaybackPush(PLAYBACK_HIT, targetId);
                btPlaybackPush(PLAYBACK_NUMBER, amount);
                btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                PUSH_MESSAGE((IS_PARTY_UNIT(targetId) ? MsgDmgEmphP : MsgDmgEmphE) + affinity);
                newHp -= amount;

                if (newHp <= 0)
                {
                    btPlaybackPush(PLAYBACK_DEFEAT, targetId);
                    btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                    newHp = 0;
                    PUSH_MESSAGE(IS_PARTY_UNIT(targetId) ? MsgGoesDown : MsgFelled);
                }
                else
                    btPlaybackPush(PLAYBACK_REFRESH_UNIT, targetId);
                damageDealt = target->hp - newHp;
                target->hp  = newHp;
                gmUnitRecalculateGaugesStub(targetId);
                break;
            }

            case ABILITY_FORMULA_HEAL_PP:
            {
                if (ability->power == 0)
                    break;
                pp     = target->pp;
                power  = ability->power;
                amount = gmBattleCalculateRecoveryStub(
                    power, element == ELEMENT_NONE ? ITEM_ELEMENT_POWER : elementPower, FULL_RATE);
                amount  = amount * RangeFalloffHealPp[distance] / 100;
                amount *= multiplier;
                pp     += amount;
                if (pp > target->ppMax)
                {
                    pp     = target->ppMax;
                    amount = pp - target->pp;
                }
                btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                if (pp == target->ppMax)
                    PUSH_MESSAGE(MsgPpFull);
                else
                {
                    btPlaybackPush(PLAYBACK_NUMBER, amount);
                    PUSH_MESSAGE(MsgPpRecover);
                }
                target->pp = pp;
                gmUnitRecalculateGaugesStub(targetId);
                break;
            }

            // The ability kinds with no damage or healing.
            case ABILITY_FORMULA_BLANK:
            case ABILITY_FORMULA_EFFECT_ONLY:
            case ABILITY_FORMULA_NO_POWER   : break;

            // An effect ability: its power deals damage only if the effect lands.
            case ABILITY_FORMULA_EFFECT:
                if (hit != 0)
                {
                    if (ability->power == 0)
                        break;
                    newHp = target->hp;
                    if (element != ELEMENT_NONE)
                        elementBonus = elementPower - target->element[element].resistance;
                    power   = ability->power;
                    amount  = gmBattleCalculatePsynergyPowerStub(power, elementBonus, FULL_RATE);
                    amount *= multiplier;
                    amount  = amount * RangeFalloffEffectDamage[distance] / 100;
                    REDUCE_BY_GUARD(amount, target);
                    btPlaybackPush(PLAYBACK_HIT, targetId);
                    btPlaybackPush(PLAYBACK_NUMBER, amount);
                    btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                    PUSH_MESSAGE(IS_PARTY_UNIT(targetId) ? MsgDmgP : MsgDmgE);
                    newHp -= amount;

                    if (newHp <= 0)
                    {
                        btPlaybackPush(PLAYBACK_DEFEAT, targetId);
                        btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                        newHp = 0;
                        PUSH_SIDE_MESSAGE(targetId, MsgGoesDown, MsgFelled);
                    }
                    else
                        btPlaybackPush(PLAYBACK_REFRESH_UNIT, targetId);
                    damageDealt = target->hp - newHp;
                    target->hp  = newHp;
                    gmUnitRecalculateGaugesStub(targetId);
                    break;
                }
                btPlaybackPush(PLAYBACK_REFRESH_UNIT, targetId);
                btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                PUSH_MESSAGE(MsgNoEffect);
                break;
        }
    }

    // 6. The added effect; each message after the first pushes the name
    // again.
    btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
    if ((gmBattleIsReviveEffectStub(ability->effect) != 0 || target->hp != 0
         || btAbilityEffectAppliesToDowned(ability->effect) != 0)
        && hit != 0)
    {
        switch (ability->effect)
        {
            // Tonic leaves confusion and charm, and ends sleep without a message.
            case ABILITY_EFFECT_CURE_ALL:
                if (target->delusion != 0)
                {
                    target->delusion = 0;
                    PUSH_MESSAGE(MsgCureDelusion);
                    btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                }
                if (target->stun != 0)
                {
                    target->stun = 0;
                    btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
                    btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                    PUSH_MESSAGE(MsgCureStun);
                }
                target->sleep = 0;
                if (target->psySeal != 0)
                {
                    target->psySeal = 0;
                    PUSH_MESSAGE(MsgCureSeal);
                    btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                }
                if (target->grimReaper != 0)
                {
                    target->grimReaper = 0;
                    PUSH_MESSAGE(MsgCureCurse);
                    btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                }
                if (target->haunt != 0)
                {
                    target->haunt = 0;
                    PUSH_MESSAGE(MsgCureSpirit);
                    btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                }
                if (target->poison != 0)
                {
                    PUSH_MESSAGE(MsgCurePoison);
                    target->poison = 0;
                }
                btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
                break;

            // Restore cures what Tonic does, but not the evil spirit or poison.
            case ABILITY_EFFECT_RESTORE:
                if (target->delusion != 0)
                {
                    target->delusion = 0;
                    btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
                    btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                    PUSH_MESSAGE(MsgCureDelusion);
                }
                if (target->stun != 0)
                {
                    target->stun = 0;
                    btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
                    btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                    PUSH_MESSAGE(MsgCureStun);
                }
                target->sleep = 0;
                if (target->psySeal != 0)
                {
                    target->psySeal = 0;
                    btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
                    btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                    PUSH_MESSAGE(MsgCureSeal);
                }
                if (target->grimReaper == 0)
                    break;
                target->grimReaper = 0;
                btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
                btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
                PUSH_MESSAGE(MsgCureCurse);
                break;

            case ABILITY_EFFECT_FIZZ:
            case ABILITY_EFFECT_SPRITZ:
            {
                Sint32 oldHp;
                Sint32 maxHpBits;
                Sint32 hpMax;
                Sint32 newHp;
                Sint32 restored;
#if !FIX_POINTER_OUTSIDE_OBJECT
                Uint16 *stat;
#endif

                // UB: a pointer walks from hp down past it to hpMax, reading each stat twice,
                // unsigned and signed; the member reads behind UBFIX share one load each.
#if FIX_POINTER_OUTSIDE_OBJECT
                oldHp     = (Uint16)target->hp;
                newHp     = target->hp;
                maxHpBits = (Uint16)target->hpMax;
                hpMax     = target->hpMax;
#else
                stat  = (Uint16 *)&target->hp;
                oldHp = *stat--;
                newHp = *(Sint16 *)(stat + 1);
                stat--;
                maxHpBits = *stat--;
                hpMax     = *(Sint16 *)(stat + 1);
#endif
                if (ability->effect == ABILITY_EFFECT_FIZZ)
                    newHp += hpMax * 60 / 100;
                else
                    newHp += hpMax * 30 / 100;
                if (newHp > (Sint16)maxHpBits)
                    newHp = (Sint16)maxHpBits;
                restored = newHp - (Sint16)oldHp;
                // Nothing to show if no HP came back, unless the ability heals.
                if (restored == 0 && kind != ABILITY_FORMULA_HEAL_HP)
                    break;
                if (newHp == (Sint16)maxHpBits)
                    PUSH_MESSAGE(MsgHpFull);
                else
                {
                    btPlaybackPush(PLAYBACK_NUMBER, restored);
                    PUSH_MESSAGE(MsgHpRecover);
                }
                target->hp = newHp;
                gmUnitRecalculateGaugesStub(targetId);
                break;
            }

            case ABILITY_EFFECT_EMBER:
            {
                Sint32 oldPp;
                Sint32 ppMax;
                Sint32 newPp;
                Sint32 restored;

                newPp  = target->pp;
                ppMax  = target->ppMax;
                oldPp  = newPp;
                newPp += ppMax * 7 / 100;
                if (newPp > ppMax)
                    newPp = ppMax;
                restored = newPp - oldPp;
                if (restored == 0 && kind != ABILITY_FORMULA_HEAL_PP)
                    break;
                if (newPp == ppMax)
                    PUSH_MESSAGE(MsgPpFull);
                else
                {
                    btPlaybackPush(PLAYBACK_NUMBER, restored);
                    PUSH_MESSAGE(MsgPpRecover);
                }
                target->pp = newPp;
                gmUnitRecalculateGaugesStub(targetId);
                break;
            }

            // The stat changes show how far the stat itself moved, 0 at its limit; Zephyr and Vine
            // set Agility's stage outright.
            case ABILITY_EFFECT_AGILITY_UP:
                target->agilityStage = AGILITY_STAGE_DOUBLED;
                target->agilityTurns = AGILITY_CHANGE_TURNS;
                gmUnitCalculateStub(targetId);
                btPlaybackPush(PLAYBACK_NUMBER, target->agility - savedTarget->agility);
                PUSH_MESSAGE(MsgAgiUp);
                break;

            case ABILITY_EFFECT_AGILITY_DOWN:
                target->agilityStage = STAT_STAGE_MIN;
                target->agilityTurns = AGILITY_CHANGE_TURNS;
                gmUnitCalculateStub(targetId);
                btPlaybackPush(PLAYBACK_NUMBER, savedTarget->agility - target->agility);
                PUSH_MESSAGE(MsgAgiDown);
                break;

            case ABILITY_EFFECT_ATTACK_DOWN:
                target->attackStage--;
                CLAMP_STAT_STAGE(target->attackStage);
                gmUnitCalculateStub(targetId);
                btPlaybackPush(PLAYBACK_NUMBER, savedTarget->attack - target->attack);
                PUSH_MESSAGE(MsgAtkDown);
                target->attackTurns = STAT_CHANGE_TURNS;
                break;

            case ABILITY_EFFECT_ATTACK_DOWN_SHARPLY:
                target->attackStage -= 2;
                CLAMP_STAT_STAGE(target->attackStage);
                gmUnitCalculateStub(targetId);
                btPlaybackPush(PLAYBACK_NUMBER, savedTarget->attack - target->attack);
                PUSH_MESSAGE(MsgAtkDown);
                target->attackTurns = STAT_CHANGE_TURNS;
                break;

            case ABILITY_EFFECT_ATTACK_UP:
                target->attackStage++;
                CLAMP_STAT_STAGE(target->attackStage);
                gmUnitCalculateStub(targetId);
                btPlaybackPush(PLAYBACK_NUMBER, target->attack - savedTarget->attack);
                PUSH_MESSAGE(MsgAtkUp);
                target->attackTurns = STAT_CHANGE_TURNS;
                break;

            case ABILITY_EFFECT_ATTACK_UP_SHARPLY:
                target->attackStage += 2;
                CLAMP_STAT_STAGE(target->attackStage);
                gmUnitCalculateStub(targetId);
                btPlaybackPush(PLAYBACK_NUMBER, target->attack - savedTarget->attack);
                PUSH_MESSAGE(MsgAtkUp);
                target->attackTurns = STAT_CHANGE_TURNS;
                break;

            case ABILITY_EFFECT_DEFENSE_DOWN:
                target->defenseStage--;
                CLAMP_STAT_STAGE(target->defenseStage);
                gmUnitCalculateStub(targetId);
                btPlaybackPush(PLAYBACK_NUMBER, savedTarget->defense - target->defense);
                PUSH_MESSAGE(MsgDefDown);
                target->defenseTurns = STAT_CHANGE_TURNS;
                break;

            case ABILITY_EFFECT_DEFENSE_DOWN_SHARPLY:
                target->defenseStage -= 2;
                CLAMP_STAT_STAGE(target->defenseStage);
                gmUnitCalculateStub(targetId);
                btPlaybackPush(PLAYBACK_NUMBER, savedTarget->defense - target->defense);
                PUSH_MESSAGE(MsgDefDown);
                target->defenseTurns = STAT_CHANGE_TURNS;
                break;

            case ABILITY_EFFECT_DEFENSE_UP:
                target->defenseStage++;
                CLAMP_STAT_STAGE(target->defenseStage);
                gmUnitCalculateStub(targetId);
                btPlaybackPush(PLAYBACK_NUMBER, target->defense - savedTarget->defense);
                PUSH_MESSAGE(MsgDefUp);
                target->defenseTurns = STAT_CHANGE_TURNS;
                break;

            case ABILITY_EFFECT_DEFENSE_UP_SHARPLY:
                target->defenseStage += 2;
                CLAMP_STAT_STAGE(target->defenseStage);
                gmUnitCalculateStub(targetId);
                btPlaybackPush(PLAYBACK_NUMBER, target->defense - savedTarget->defense);
                PUSH_MESSAGE(MsgDefUp);
                target->defenseTurns = STAT_CHANGE_TURNS;
                break;

            // The revivals bring back a downed target with all, half or 80% of its HP.
            case ABILITY_EFFECT_REVIVE:
                if (target->hp != 0)
                    break;
                PUSH_MESSAGE(MsgRevived);
                target->hp = target->hpMax;
                gmUnitRecalculateGaugesStub(targetId);
                break;

            case ABILITY_EFFECT_REVIVE_HALF:
                if (target->hp != 0)
                    break;
                PUSH_MESSAGE(MsgRevived);
                target->hp = target->hpMax / 2;
                gmUnitRecalculateGaugesStub(targetId);
                break;

            case ABILITY_EFFECT_DEW:
                if (target->hp != 0)
                    break;
                PUSH_MESSAGE(MsgRevived);
                target->hp = target->hpMax * 8 / 10;
                gmUnitRecalculateGaugesStub(targetId);
                break;

            case ABILITY_EFFECT_CURE_POISON:
                if (target->poison != 0)
                    PUSH_MESSAGE(MsgCurePoison);
                target->poison = 0;
                break;

            // A Resistance change needs no recalculation to show: it is the change in stages times
            // RESISTANCE_PER_STAGE.
            case ABILITY_EFFECT_RESISTANCE_DOWN:
                target->resistanceStage--;
                CLAMP_STAT_STAGE(target->resistanceStage);
                btPlaybackPush(PLAYBACK_NUMBER,
                               (savedTarget->resistanceStage - target->resistanceStage)
                                   * RESISTANCE_PER_STAGE);
                PUSH_MESSAGE(MsgResDown);
                target->resistanceTurns = STAT_CHANGE_TURNS;
                break;

            case ABILITY_EFFECT_RESISTANCE_DOWN_SHARPLY:
                target->resistanceStage -= 2;
                CLAMP_STAT_STAGE(target->resistanceStage);
                btPlaybackPush(PLAYBACK_NUMBER,
                               (savedTarget->resistanceStage - target->resistanceStage)
                                   * RESISTANCE_PER_STAGE);
                PUSH_MESSAGE(MsgResDown);
                target->resistanceTurns = STAT_CHANGE_TURNS;
                break;

            case ABILITY_EFFECT_RESISTANCE_UP:
                target->resistanceStage++;
                CLAMP_STAT_STAGE(target->resistanceStage);
                btPlaybackPush(PLAYBACK_NUMBER,
                               (target->resistanceStage - savedTarget->resistanceStage)
                                   * RESISTANCE_PER_STAGE);
                PUSH_MESSAGE(MsgResUp);
                target->resistanceTurns = STAT_CHANGE_TURNS;
                break;

            case ABILITY_EFFECT_RESISTANCE_UP_SHARPLY:
                target->resistanceStage += 2;
                CLAMP_STAT_STAGE(target->resistanceStage);
                btPlaybackPush(PLAYBACK_NUMBER,
                               (target->resistanceStage - savedTarget->resistanceStage)
                                   * RESISTANCE_PER_STAGE);
                PUSH_MESSAGE(MsgResUp);
                target->resistanceTurns = STAT_CHANGE_TURNS;
                break;

            // Poison does nothing to a poisoned target, deadly poison nothing to one that already
            // has it.
            case ABILITY_EFFECT_POISON:
                if (target->poison != 0)
                    break;
                PUSH_MESSAGE(MsgPoisoned);
                target->poison = POISON_NORMAL;
                break;

            case ABILITY_EFFECT_VENOM:
                if (target->poison > POISON_NORMAL)
                    break;
                PUSH_MESSAGE(MsgVenom);
                target->poison = POISON_VENOM;
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

            // Instant death: "X goes down!" on the party, Drown's "X suffocates!" or "X's spirit
            // has been drained!" on an enemy.
            case ABILITY_EFFECT_INSTANT_DEATH:
                btPlaybackPush(PLAYBACK_DEFEAT, targetId);
                if (target->side == BATTLE_SIDE_PARTY)
                    PUSH_MESSAGE(MsgKoDown);
                else if (abilityId == ABILITY_DROWN)
                    PUSH_MESSAGE(MsgSuffocate);
                else
                    PUSH_MESSAGE(MsgSpiritDrain);
                target->hp = 0;
                gmUnitRecalculateGaugesStub(targetId);
                break;

            case ABILITY_EFFECT_REGENERATE:
                PUSH_MESSAGE(MsgRefrain);
                target->regenerate = STATUS_TURNS;
                break;

            case ABILITY_EFFECT_REFLECT:
                PUSH_MESSAGE(MsgReflect);
                target->reflect = STATUS_TURNS;
                break;

            // The drains give the damage dealt back to the user: Drain all of it as HP, Sap half.
            case ABILITY_EFFECT_DRAIN_HP:
            case ABILITY_EFFECT_DRAIN_HALF_HP:
            {
                Sint32 newHp;

                newHp  = actor->hp;
                amount = damageDealt;
                if (ability->effect == ABILITY_EFFECT_DRAIN_HALF_HP)
                    amount /= 2;
                newHp += amount;
                if (newHp > actor->hpMax)
                {
                    newHp  = actor->hpMax;
                    amount = newHp - actor->hp;
                }
                btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
                btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
                if (newHp == actor->hpMax)
                    PUSH_MESSAGE(MsgHpFull);
                else
                {
                    btPlaybackPush(PLAYBACK_NUMBER, amount);
                    PUSH_MESSAGE(MsgHpRecover);
                }
                actor->hp = newHp;
                gmUnitRecalculateGaugesStub(unitId);
                break;
            }

            // Psy Drain gives the PP taken.
            case ABILITY_EFFECT_DRAIN_PP:
            {
                Sint32 newPp;
                Sint32 drained;

                newPp    = actor->pp;
                drained  = damageDealt;
                newPp   += drained;
                if (newPp > actor->ppMax)
                {
                    newPp   = actor->ppMax;
                    drained = newPp - actor->pp;
                }
                btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);
                btPlaybackPush(PLAYBACK_UNIT_NAME, unitId);
                if (newPp == actor->ppMax)
                    PUSH_MESSAGE(MsgPpFull);
                else
                {
                    btPlaybackPush(PLAYBACK_NUMBER, drained);
                    PUSH_MESSAGE(MsgPpRecover);
                }
                actor->pp = newPp;
                gmUnitRecalculateGaugesStub(unitId);
                break;
            }

            // Psy Leech: a tenth of the damage as PP, as far as the target has it and the user has
            // room.
            case ABILITY_EFFECT_PSY_LEECH:
                amount = damageDealt / 10;
                if (target->pp < amount)
                    amount = target->pp;
                if (actor->pp + amount > actor->ppMax)
                    amount = actor->ppMax - actor->pp;
                if (amount == 0)
                    break;
                btPlaybackPush(PLAYBACK_NUMBER, amount);
                PUSH_SIDE_MESSAGE(targetId, MsgLeechTake, MsgLeechGain);
                gmUnitAddPpStub(unitId, amount);
                break;

            // Break returns raised Attack, Defense, Resistance and Agility stages to 0, leaving
            // lowered ones, and takes away a summon's boost.
            case ABILITY_EFFECT_BREAK:
                if (target->attackStage > 0)
                {
                    target->attackStage = 0;
                    target->attackTurns = 0;
                }
                if (target->defenseStage > 0)
                {
                    target->defenseStage = 0;
                    target->defenseTurns = 0;
                }
                if (target->resistanceStage > 0)
                {
                    target->resistanceStage = 0;
                    target->resistanceTurns = 0;
                }
                if (target->agilityStage > 0)
                    target->agilityStage = 0;
                target->summonBoost[ELEMENT_VENUS]   = 0;
                target->summonBoost[ELEMENT_MERCURY] = 0;
                target->summonBoost[ELEMENT_MARS]    = 0;
                target->summonBoost[ELEMENT_JUPITER] = 0;
                PUSH_MESSAGE(MsgBuffsReset);
                break;

            // "An evil spirit grips" the target: from now on its own blows may hurt it (the share
            // at the end of this function).
            case ABILITY_EFFECT_HAUNT:
                PUSH_MESSAGE(MsgEvilSpirit);
                target->haunt = 1;
                break;

            // The Grim Reaper counts down to the target's death; each later curse brings it a turn
            // closer.
            case ABILITY_EFFECT_CURSE:
                if (target->grimReaper == 0)
                {
                    PUSH_MESSAGE(MsgDeathCurse);
                    target->grimReaper = CURSE_TURNS;
                    break;
                }
                if (target->grimReaper <= 1)
                    break;
                target->grimReaper--;
                btPlaybackPush(PLAYBACK_NUMBER, target->grimReaper);
                PUSH_MESSAGE(MsgDeathCount);
                break;

            // Kite: the target acts twice in the next turn (the count drops at each turn's end).
            case ABILITY_EFFECT_ACT_TWICE:
                PUSH_MESSAGE(MsgReadies);
                target->ready = 2;
                break;

            // The target rises to the challenge and won't flee; on the party, no one may run ("This
            // is no time to be running away!").
            case ABILITY_EFFECT_CHALLENGE:
                PUSH_MESSAGE(MsgChallenge);
                target->challenge = 1;
                if (IS_PARTY_UNIT(targetId))
                    battle->challengeFlag |= BATTLE_CHALLENGED_PARTY;
                break;

            // The target loses its next action (btActionRunBuild).
            case ABILITY_EFFECT_GROUND:
                PUSH_MESSAGE(MsgImmobile);
                target->immobile = 1;
                break;

            // The guards: the message shows either way, but a guard is never lowered.
            case ABILITY_EFFECT_DEFEND:
                PUSH_MESSAGE(MsgDefendAura);
                if (target->defendLevel > 0)
                    break;
                target->defendLevel = DEFEND_NORMAL;
                break;

            case ABILITY_EFFECT_TOTAL_DEFENSE:
                PUSH_MESSAGE(MsgTotalDefenseAura);
                if (target->defendLevel > DEFEND_NORMAL)
                    break;
                target->defendLevel = DEFEND_TOTAL;
                break;

            // Contain, Threaten and Tremble do nothing; the player only reads their message.
            case ABILITY_EFFECT_THREATEN: PUSH_MESSAGE(MESSAGE_NONE); break;

            default                     : break;
        }
    }

    // 7. Waking from damage, and the stats worked out again.
    btPlaybackPush(PLAYBACK_CLEAR_ARGUMENTS, 0);

    // Damage wakes a sleeping target one time in four, unless it fell asleep this turn.
    if (target->hp != 0 && target->sleep != 0 && target->sleep < STATUS_TURNS && damageDealt > 0
        && (gmBattleRandomStub() & 3) == 0)
    {
        target->sleep = 0;
        btPlaybackPush(PLAYBACK_UNIT_NAME, targetId);
        PUSH_MESSAGE(MsgWakes);
    }
    syHeapRelease(savedTarget);
    gmUnitCalculateStub(targetId);
    // The party's status is drawn now, not through the queue.
    mnPartyStatusDrawStub(BattleStateWork->partyStatus);
    if (target->hp != 0)
        btPlaybackPush(PLAYBACK_REFRESH_UNIT, targetId);
    // A haunted user takes, one time in four, a quarter of the HP or PP it took from the target (at
    // least 1), at the end of its action.
    if (actor->haunt != 0 && (gmBattleRandomStub() & 3) == 0 && damageDealt > 0)
    {
        Sint32 share;

        share = damageDealt >> 2;
        if (share == 0)
            share = 1;
        plan->hauntDamage += share;
    }
}

// Returns 1 for an ability that costs PP, and for Psy Drain, which costs none.
Uint32 btAbilityIsPsynergy(Sint32 abilityId)
{
    Uint8 pp;

    if (abilityId == ABILITY_PSY_DRAIN)
        return 1;
    pp = gmAbilityGetDirectStub(abilityId)->ppCost;
    return pp != 0;
}
