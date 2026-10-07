/* BtlTgtExec, tla-en 0x08120454.
 * alchemy diff: listing 3450 instructions, draft 3454, common 3287,
 * differ 330.
 * This draft is The Broken Seal's source (0x080bbb0c, games/THE BROKEN
 * SEAL/SRC/BATTLE/ACTION_RESOLVE_TARGET_ACTION.C) with this game's
 * differences written in; they were worked out under GAME_TLA in that file
 * and moved here, where they stay until every edition of this game matches.
 * Then one source in games/COMMON carries only the real differences under
 * GAME_TLA. Not real differences, to settle against ☀️'s form then: the
 * locals block (this one orders offset, action_id, range, scale and pass
 * differently, which steers registers, and declares apwr for the whole
 * function), target_id u8 or s32 (no change), and MsgSplitFailed where ☀️
 * has MsgSplitFail (each game's PO name for the same message).
 * Scores: the old hand draft 806; ☀️'s source compiled for this game with
 * only its names resolved 3237. One power variable (apwr) for
 * every damage case moved it from differ 2590 to 779, the additive case's
 * own loop counter to 657, ⚓️'s unsigned guard_level to 552, case 401 as a
 * case of its own in the rate switch (rate 12 set before the test, as the
 * listing does) to 510, the HP drain halving dealt in apwr (declared for
 * the whole function, so it keeps r7 as in the listing) to
 * 428, the PP restore reading action->effect directly (the listing loads
 * it again) and adding max_pp / 10 before reading maxu in case 77 to 424
 * (draft 3454, common 3240). BattleEv_Push declared void for
 * this game (its body here sets no result, and void calls load r0 before r1,
 * as the listing does at 61 of 68 two-argument calls) to 330 (common 3287).
 * Then tried for the HP drain, where the listing keeps heal in r5 and the
 * amount in r6 (ours the reverse): the amount in tmp (384), in a
 * block-local amt (505), hp read after the halving (557); heal += apwr
 * before dmg = apwr changes nothing. The weapon case halves defense with a
 * signed shift sequence in the listing (lsrs #31, adds, asrs) where ours
 * shifts logically: def / 2 and scale /= 2 give the same logical shift,
 * defense read through the function-level value 382. With the void
 * BattleEv_Push, plain hp reads at the top of the heal still lose: signed
 * first 351, unsigned first 347, and plain max_hp reads in every branch too
 * 639. The listing reads maxu before
 * the division call and shares the add after it with 78 and 7%: reading
 * maxu first instead merges both loads (723); direct reads with maxv as
 * before give 435; with the void BattleEv_Push, maxu first gives 635, and
 * with (s16)maxu / 10 as well 635. There the listing copies the target
 * into r2 at the PP restore's head, so the effect's ldrb fits between the
 * pp loads and case 77 keeps r2; ours copies it into r3, which the effect
 * load takes. The summon's enemy reset as a for loop over saved[] keeps
 * 330 only with its (u16) read (377 without). Tried and worse: a while loop
 * for the immobilize scan (832), tmp or one shared amt for the drains (708,
 * 716), while loops for the affinity scans, plain hp/max_hp reads in the
 * heal (557: one ldrh shared by both; 445 with the signed read first),
 * the heal branches reading through stat_ptr (808), the HP drain's amount in a block-local amt or with the
 * new HP held in dmg (605 both), rate set to 0 in the switch's default
 * instead of before it (665). No change: target_id s32 instead of u8,
 * case 401 last, apwr's place among the declarations, BattleEv_Push
 * without a prototype, heal += dmg instead of heal += apwr (kept: one line
 * shared with ☀️), plain max_hp reads and value casts in place of the
 * heal and PP restore's pointer casts (kept: the same instructions).
 * The enemy-slot scan after a summon now matches. Still to bring over: the
 * rate switch's 409 test (the listing derives it from
 * 411 in r3; ours loads it fresh in r2), the summon message tail, the heal's
 * target copy (the listing copies r8 again in each branch; ours keeps one
 * copy in r4); then register order. The 409 test costs two bytes (a fresh
 * constant, not subs from 411), which moves every branch offset in the
 * rate switch; most other differences are one load swapped with a
 * neighbouring move, the load order K2 leaves open.
 * Tried after the split, against 330: case 409 a case of its own with rate
 * 12, before the 399 group or after 401 (348 both), 409 last among its
 * labels (no change), 409 sharing 401's case (336: draft 3450, but rate 12
 * is no longer set before the 401 test), default last or with 393 last,
 * rate as u8 or s16 (no change), rate as the function's kind (509) or tmp
 * (660); in the heal, case 76 reading max_hp directly or maxu before maxv
 * (337 both), maxu before maxv in case 71 (648), 2 * maxv / 5 (no change);
 * in the PP restore, old
 * read before heal (332), the block's declaration order and pp read
 * through s16 and u16 pointers (no change); in
 * the HP drain, a block-local amt (505: heal in r5 and amt in r6 as in the
 * listing, but apwr then shares r6 and the listing's copy from r7 is
 * lost; heal += apwr after amt = apwr keeps 505), the HP in hp0 (740) or
 * tmp (344); for the weapon case's signed
 * halving, (s16)def / 2 or (s16)scale / 2 (336: the sign is extended
 * first), (s16)target->defense read into def (382), defense read into tmp
 * (334), kind (no change) or apwr (560), value (382), power (383), guard
 * (332), count (no change), scale /= 2 and def /= 2 in place (no change):
 * the shift stays logical whichever variable holds the zero-extended
 * load, even one set elsewhere from signed values (power, value). */
#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "BATTLE_PARTY.H"
#include "IWRAM_CALL.H"
#include "BATTLE_ACTOR.H"
#include "BATTLE_CALC.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EVENT.H"
#include "BATTLE_MSG.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_SUMMON.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_WORK.H"
#include "MOTION_OBJECT.H"
#include "RUNTIME_MEM.H"
#include "BATTLE_PRESENTATION.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"

#define CLAMP_MOD(v)                                                           \
    {                                                                          \
        if ((v) < -4)                                                          \
            (v) = -4;                                                          \
        if ((v) > 4)                                                           \
            (v) = 4;                                                           \
    }

#define IS_PARTY(id) ((id) >= 0 && (id) < 8)

#define TAKE_BONUS()                                                           \
    {                                                                          \
        if (range != 4)                                                        \
            bonus = power - target->elements[range].resist;                    \
    }

#define MES_PUSH(m) BattleEv_Push(BATTLE_EVENT_TEXT, (u32)(m))

#define TEXT_SIDE(player, enemy)                                               \
{                                                                              \
    if (IS_PARTY(target_id))                                                   \
        MES_PUSH(player);                                                      \
    else                                                                       \
        MES_PUSH(enemy);                                                       \
}

#define APPLY_GUARD()                                                         \
    {                                                                         \
        guard = target->guard_level;                                          \
        if (guard != 0) {                                                     \
            if (guard == 1)                                                   \
                dmg /= 2;                                                     \
            else if (guard == 2)                                              \
                dmg = dmg * 2 / 5;                                            \
            else                                                              \
                dmg /= 10;                                                    \
        }                                                                     \
    }

#define ADJUST_RES(delta, value_expr, text)                                 \
{                                                                              \
    target->res_modifier += (delta);                                           \
    CLAMP_MOD(target->res_modifier);                                           \
    BattleEv_Push(BATTLE_EVENT_VALUE, (value_expr));                           \
    MES_PUSH(text);                                                            \
    target->res_modifier_turns = 7;                                            \
}

#define SET_STATUS7(field, text)                                               \
{                                                                              \
    MES_PUSH(text);                                                            \
    (field) = 7;                                                               \
}

s32 BtlTgtExec(struct BattlePlan *plan, s32 slot)
{
    struct BattleAction *action;
    s32 action_id;
    struct BattleUnit *actor;
    s32 actor_id;
    s32 range;
    s32 bonus;
    struct BattleSession *work;
    s32 offset;
    s32 half;
    s32 adjust;
    s32 dealt;
    s32 crush;
    s32 hit;
    s32 modifier;
    s32 skip;
    s32 nibble;
    s32 affinity;
    struct BattleUnit *copy;
    s32 power;
    s32 hp0;
    s32 kind;
    s32 scale;
    s32 pass;
    s16 *cmd;
    u8 target_id;
    s8 *rm;
    struct BattleUnit *target;
    u32 n;
    s32 value;
    s32 dmg;
    s32 apwr;
    s8 *am;
    s32 guard;
    s16 saved[8];
    s32 count;
    s32 tmp;
    s32 size;
    bonus = 0;
    work = Ram_HeapSlots->battle_work;
    half = 0;
    dealt = 0;
    crush = 0;
    skip = 0;
    affinity = 0;
    size = sizeof(struct BattleUnit);
    copy = Runtime_BumpAllocate(size);

    if (action_id == 312 || action_id == 316)
        actor_id = plan->actor_id2;
    else
        actor_id = plan->actor_id;
    target_id = plan->target_ids[slot];
    action_id = plan->action_id;
    range = plan->range_index;
    adjust = plan->target_adjustments[slot];
    modifier = plan->target_modifiers[slot];

    action = BattleAction_Get(action_id);
    actor = Owner_GetState(actor_id);
    target = Owner_GetState(target_id);
    Iwram_CopyWords(copy, target, size);
    switch (target->enemy_id) {
    case 221:
        if (plan->command == 1 && (u32)(actor_id ^ target_id) >> 7 != 0) {
            BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            MES_PUSH(MsgDissipated);
            goto done;
        }
        break;
    case 101:
    case 102:
    case 103:
        if (action_id != 573 && work->unknown_868 != 0
            && (u32)(actor_id ^ target_id) >> 7 != 0) {
            BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            MES_PUSH(MsgFieldProtects);
            goto done;
        }
        break;
    }
    if (target->guard_level == 4 && (u32)(actor_id ^ target_id) >> 7 != 0) {
        BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
        BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
        MES_PUSH(MsgNoEffect);
        goto done;
    }

    if (action->range != 255) {
        offset = plan->target_offsets[slot];
        if (offset < 0)
            offset = -offset;
    } else {
        offset = 0;
    }

    if (range != 4) {
        s32 i;

        value = target->base_elements[range].resist;
        for (i = 0; i < 4 && value >= target->base_elements[i].resist; i++)
            ;
        if (i == 4)
            affinity = -1;

        for (i = 0; i < 4 && value <= target->base_elements[i].resist; i++)
            ;
        if (i == 4)
            affinity = 1;
    }

    if (plan->range_index >= 0 && plan->range_index <= 3) {
        cmd = &plan->command;
        if (*cmd != 2) {
            power = actor->elements[plan->range_index].power;
            goto after_power;
        }
    } else
        cmd = &plan->command;
    power = 100;
after_power:

    if (plan->command == 5 && (plan->range_index >= 0 && plan->range_index <= 3) && affinity > 0) {
        s32 chance;

        chance = power - target->elements[plan->range_index].resist;
        chance += 30;
        chance *= 65536 / 100;
        if (chance > (BattleRandom16Far() & 0xffff))
            BattleEv_Push(BATTLE_EVENT_SCRIPT_UPDATE, 5);
    }

    if (action_id == 686 || action_id == 357)
        Battle_ApplyActionExtras(target_id, 0, 0);
    if (action_id == 720 || action_id == 721)
        Battle_ApplyActionExtras(target_id, 1, 0);
    if (action_id == 728)
        Battle_ApplyActionExtras(target_id, 1, 1);
    nibble = action->target_flags & 15;
    {
        s32 first;

        first = plan->target_results[slot];
        if (first == -1)
            hit = Battle_HitCheck(
                actor_id, target_id, range, action->effect,
                HitFalloff[offset]);
        else
            hit = first;
    }
    if (action->effect == EFX_STANDBY || action->effect == EFX_STANDBY_WORK
        || action->effect == 86 || action->effect == 87) {
        s32 st;
        s32 rec;
        s32 ch;

        st = actor->enemy_id;
        ch = -1;
        rec = Summon_FindSlot();
        if (action->effect == EFX_STANDBY_WORK) {
            st = BattleFormation_SelectRandomAvailableMember(work->enemy_group);
        } else if (action->effect == 86) {
            if (actor->enemy_id == 164)
                st = (BattleRandom16Far() & 3) + 378;
            else
                st = 81;
        } else if (action->effect == 87) {
            if (work->queue_count != 0) {
                st = work->queued_summons[work->queue_head];
                ch = work->queued_charges[work->queue_head];
            } else
                hit = 0;
        }
        if (hit != 0 && Summon_ClassValid(st) != 0 && rec >= 0) {
            if (ch == -1) {
                ch = Summon_TakeCharge(st, 1);
                if (ch & 0x8000)
                    Summon_ResetCharge(st);
            }
            BattleUnit_AssignFar(rec, st, ch & 0x7fff);
            if (action->effect == 87) {
                work->queue_count--;
                work->queue_head = (work->queue_head + 1) % 2;
            }
            if (actor->enemy_id == 164)
                BattleParty_InsertUnitCentered(work->enemy_units, rec);
            else
            {
                s32 i;

                for (i = 0; i < 6; i++) {
                    if (work->enemy_units[i] == 254) {
                        work->enemy_units[i] = rec;
                        break;
                    }
                    if (work->enemy_units[i] == 255) {
                        work->enemy_units[i] = rec;
                        work->enemy_units[i + 1] = 255;
                        break;
                    }
                }
            }
            Summon_Refresh();
            {
                struct BattleObjectSlot *object;

                object = GetBattleObjectSlot(rec);
                BattlePresentation_SpawnActorObject(object, rec,
                    object->anchor_x / 0x10000, object->anchor_z / 0x10000);
            }
            BattleActor_CommitPlacement();
            {
                s32 listed;

                listed = BattleParty_ListPresentEnemies(saved);
                for (count = 0; count < listed; count++)
                    Actor_ResetMotionAtAnchor((u16)saved[count]);
            }
            BattleEv_Push(BATTLE_EVENT_UNIT, rec);
            if (action->effect == 87)
                MES_PUSH(MsgUpToFight);
            else
            if (action_id != 503)
                MES_PUSH(MsgAppears);
            else
                MES_PUSH(MsgSplitOff);
        } else if (action_id == 503) {
            MES_PUSH(MsgSplitFailed);
        } else {
            MES_PUSH(MsgNoOneCame);
        }
    }

    if (hit != 0) {
        s32 efx;

        efx = action->effect;
        if (action->effect == EFX_IMMOBILIZE || action->effect == 83) {
            hit = 0;
            for (n = 0; n < 20; n++) {
                if (work->actions[n].unit_id == target_id) {
                    hit = 1;
                    break;
                }
            }
        } else if (action->effect == EFX_HALF_DEF) {
            half = 1;
        } else if (action->effect == EFX_LETHAL) {
            crush = 1;
        } else if (action->effect == EFX_INSTANT_DOWN) {
            skip = 1;
        } else if (action->effect == EFX_ACTOR_FLASH) {
            if (actor->hp != 0)
                BattleEv_Push(BATTLE_EVENT_ACTOR_EFFECT, actor_id);
        } else if (action->effect == EFX_DRAIN_PP) {
            if (target->pp != 0)
                nibble = BATTLE_DAMAGE_PP_DRAIN;
            else
                hit = 0;
        } else if (action->effect == 90) {
            half = 2;
        } else if (action->effect == 91) {
            half = 2;
        }
    }

    if ((action->effect != 75 || slot == 0) && skip == 0
        && (target->hp != 0 || BattleFx_IsReviveFar(action->effect) != 0)) {
        s32 pp;
        s32 cur;

        switch (nibble) {
        case BATTLE_DAMAGE_WEAPON:
        case BATTLE_DAMAGE_WEAPON_SCALED:
        {
            s32 def;

            if (target->guard_level == 4)
                break;
            hp0 = target->hp;
            def = target->defense;
            scale = def;
            if (half != 0) {
                scale = (u32)def >> 1;
                if (half == 2)
                    scale = 0;
            }
            pass = 1;
            do {
                TAKE_BONUS();
                if (pass == 0)
                    bonus = 0;
                apwr = action->power;
                if (nibble == BATTLE_DAMAGE_WEAPON_SCALED)
                    dmg = Battle_CalcAttack(actor->attack, scale, 0,
                                          bonus)
                            * apwr / 10;
                else {
                    kind = actor->attack;
                    if (action_id == 312 || action_id == 316) {
                        kind = ((struct BattleUnit *)Owner_GetState(plan->actor_id))->attack;
                        if (action_id == 312)
                            kind += ((struct BattleUnit *)Owner_GetState(plan->actor_id2))->attack / 4;
                    }
                    dmg = Battle_CalcAttack(kind, scale, apwr, bonus);
                }
                dmg *= adjust;
                if (modifier != 0) {
                    if (modifier == 1)
                        dmg = dmg * 5 / 4;
                    else
                        dmg = dmg * 3 / 2;
                    dmg += (target->level / 5) + 6;
                    if (pass == 0) {
                        BattleEv_Push(BATTLE_EVENT_MARK, 0);
                        {
                            s32 text;

                            text = (s32)MsgCritical;
                            if (IS_PARTY(target_id))
                                text += 1;
                            BattleEv_Push(BATTLE_EVENT_TEXT_CONTINUE, text);
                        }
                    }
                }
                dmg += BattleRandom16Far() & 3;
                APPLY_GUARD();
                if (dmg <= 0)
                    dmg = 1;
                if (crush != 0) {
                    if (dmg < hp0 - 1) {
                        dmg = hp0 - 1;
                        if (dmg <= 0)
                            dmg = 1;
                    }
                }
                if (GameFlag_Test(366) != 0 && plan->command == 5
                    && hp0 <= dmg) {
                    dmg = hp0 - 1;
                }
                pass++;
            } while (pass <= 1);
            hp0 -= dmg;
            BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
            {
                s32 text;

                if (IS_PARTY(target_id))
                    text = (s32)MsgDmgEmphP + affinity;
                else
                    text = (s32)MsgDmgEmphE + affinity;
                BattleEv_Push(BATTLE_EVENT_TEXT, text);
            }

            if (hp0 <= 0 && BattleUnit_KeepsOneHp(target_id) != 0)
                hp0 = 1;
            if (hp0 <= 0) {
                hp0 = 0;
                BattleEv_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                if (target->enemy_id != 369 && target->enemy_id != 373) {
                    BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
                    TEXT_SIDE(MsgGoesDown, MsgFelled);
                }
            } else
                BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->hp - hp0;
            target->hp = hp0;
            Owner_RecalculateRatiosFar(target_id);
            break;
        }

        case BATTLE_DAMAGE_PP_DRAIN:
        {
            if (target->guard_level == 4)
                break;
            if (action->power == 0)
                break;
            pp = target->pp;
            TAKE_BONUS();
            apwr = action->power;
            dmg = Battle_CalcPower(apwr, bonus, 256);
            dmg = dmg * PpLossFalloff[offset] / 100;
            dmg *= adjust;
            APPLY_GUARD();
            if (action->effect == EFX_DRAIN_PP && dmg > pp)
                dmg = pp;
            if (dmg > pp)
                dmg = pp;

            BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;

                if (IS_PARTY(target_id))
                    text = (s32)MsgPpLossP;
                else
                    text = (s32)MsgPpLossE;
                BattleEv_Push(BATTLE_EVENT_TEXT, text);
                pp -= dmg;
            }
            if (pp <= 0)
                pp = 0;
            BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->pp - pp;
            target->pp = pp;
            Owner_RecalculateRatiosFar(target_id);
            break;
        }

        case BATTLE_DAMAGE_HP_HEAL:
        {
            if (action->power == 0)
                break;
            cur = target->hp;
            if (cur == 0)
                break;

            apwr = action->power;
            dmg = Battle_CalcRestore(apwr, range == 4 ? 100 : power, 256);
            dmg = dmg * HpHealFalloff[offset] / 100;
            dmg *= adjust;
            dmg += BattleRandom16Far() & 3;
            cur += dmg;
            if (cur > target->max_hp) {
                cur = target->max_hp;
                dmg = cur - target->hp;
            }
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            if (cur == target->max_hp)
                MES_PUSH(MsgHpFull);
            else {
                BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
                MES_PUSH(MsgHpRecover);
            }
            dealt = target->hp - cur;
            target->hp = cur;
            Owner_RecalculateRatiosFar(target_id);
            break;
        }

        case BATTLE_DAMAGE_PP_DIRECT:
        {
            if (target->guard_level == 4)
                break;
            if (action->power == 0)
                break;
            pp = target->pp;
            TAKE_BONUS();
            apwr = action->power;
            dmg = Battle_CalcPower(apwr, bonus, 256);
            dmg = dmg * PpDmgFalloff[offset] / 100;
            dmg *= adjust;
            APPLY_GUARD();
            BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;

                if (IS_PARTY(target_id))
                    text = (s32)MsgDmgP;
                else
                    text = (s32)MsgDmgE;
                BattleEv_Push(BATTLE_EVENT_TEXT, text);
                pp -= dmg;
            }
            if (pp <= 0)
                pp = 0;
            BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            target->pp = pp;
            Owner_RecalculateRatiosFar(target_id);
            break;
        }

        case BATTLE_DAMAGE_HP_ADDITIVE_6:
        case BATTLE_DAMAGE_HP_ADDITIVE_7:
        case BATTLE_DAMAGE_HP_ADDITIVE_9:
        {
            s32 round;

            if (target->guard_level == 4)
                break;
            if (action->power == 0)
                break;
            cur = target->hp;
            round = 1;
            do {
                TAKE_BONUS();
                if (round == 0)
                    bonus = 0;
                apwr = action->power;
                if (action_id == 683 || action_id == 673 || action_id == 724
                    || plan->command == 6 || plan->command == 10) {
                    s32 rate;

                    rate = 0;
                    switch (action_id) {
                    default:
                    case 393:
                        break;
                    case 406: case 388: case 396: case 380:
                        rate = 3;
                        break;
                    case 409: case 399: case 391: case 383:
                        rate = 12;
                        break;
                    case 401:
                        rate = 12;
                        break;
                    case 683:
                        rate = 35;
                        break;
                    case 407: case 400: case 397: case 389: case 384: case 381:
                        rate = 6;
                        break;
                    case 398: case 392: case 390: case 410: case 408: case 382:
                        rate = 9;
                        break;
                    case 402:
                        rate = 7;
                        break;
                    case 411: case 403: case 385:
                        rate = 15;
                        break;
                    case 394:
                        rate = 21;
                        break;
                    case 412:
                        rate = 24;
                        break;
                    case 673: case 386:
                        rate = 30;
                        break;
                    case 724: case 404:
                        rate = 40;
                        break;
                    }
                    value = target->max_hp;
                    if (value > 10000)
                        value = 10000;
                    apwr += value * rate / 100;
                }
                dmg = Battle_CalcPower(apwr, bonus, 256);
                dmg *= adjust;
                switch (nibble & 15) {
                case BATTLE_DAMAGE_HP_ADDITIVE_6:
                    dmg = HpDmgFalloff5[offset] * dmg / 100;
                    break;
                case BATTLE_DAMAGE_HP_ADDITIVE_9:
                    dmg = dmg * HpDmgFalloff8[offset] / 100;
                    break;
                case BATTLE_DAMAGE_HP_ADDITIVE_7:
                    dmg = dmg * HpDmgFalloff6[offset] / 100;
                    break;
                }
                dmg += BattleRandom16Far() & 3;
                APPLY_GUARD();
                if ((gPartyState.battle_rule_24b == 6
                        || (GameFlag_Test(366) != 0 && plan->command == 6))
                    && cur > dmg) {
                    dmg = cur;
                }
                round++;
            } while (round <= 1);
            BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;

                if (IS_PARTY(target_id))
                    text = (s32)MsgDmgEmphP + affinity;
                else
                    text = (s32)MsgDmgEmphE + affinity;
                BattleEv_Push(BATTLE_EVENT_TEXT, text);
                cur -= dmg;
            }

            if (cur <= 0 && BattleUnit_KeepsOneHp(target_id) != 0)
                cur = 1;
            if (gPartyState.battle_rule_24b == 6)
                cur = 0;
            if (cur <= 0) {
                BattleEv_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
                {
                    s32 text;

                    cur = 0;
                    if (IS_PARTY(target_id))
                        text = (s32)MsgGoesDown;
                    else
                        text = (s32)MsgFelled;
                    BattleEv_Push(BATTLE_EVENT_TEXT, text);
                }
            } else
                BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->hp - cur;
            target->hp = cur;
            Owner_RecalculateRatiosFar(target_id);
            break;
        }

        case BATTLE_DAMAGE_PP_HEAL:
        {
            if (action->power == 0)
                break;
            pp = target->pp;
            apwr = action->power;
            dmg = Battle_CalcRestore(apwr, range == 4 ? 100 : power, 256);
            dmg = dmg * PpHealFalloff[offset] / 100;
            dmg *= adjust;
            pp += dmg;
            if (pp > target->max_pp) {
                pp = target->max_pp;
                dmg = pp - target->pp;
            }
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            if (pp == target->max_pp)
                MES_PUSH(MsgPpFull);
            else {
                BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
                MES_PUSH(MsgPpRecover);
            }
            target->pp = pp;
            Owner_RecalculateRatiosFar(target_id);
            break;
        }

        case 0:
        case 7:
        case 9:
            break;

        case BATTLE_DAMAGE_HP_POWER:
            if (target->guard_level == 4)
                break;
            if (hit != 0) {
            if (action->power == 0)
                break;
            cur = target->hp;
            TAKE_BONUS();
            apwr = action->power;
            dmg = Battle_CalcPower(apwr, bonus, 256);
            dmg *= adjust;
            dmg = dmg * HpDmgFalloff[offset] / 100;
            APPLY_GUARD();
            BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;

                if (IS_PARTY(target_id))
                    text = (s32)MsgDmgP;
                else
                    text = (s32)MsgDmgE;
                BattleEv_Push(BATTLE_EVENT_TEXT, text);
                cur -= dmg;
            }

            if (cur <= 0 && BattleUnit_KeepsOneHp(target_id) != 0)
                cur = 1;
            if (cur <= 0) {
                cur = 0;
                BattleEv_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                if (target->enemy_id != 369 && target->enemy_id != 373) {
                    BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
                    TEXT_SIDE(MsgGoesDown, MsgFelled);
                }
            } else
                BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->hp - cur;
            target->hp = cur;
            Owner_RecalculateRatiosFar(target_id);
            break;
            }
            BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            MES_PUSH(MsgNoEffect);
            break;

        }
    }

    if (work->unknown_868 != 0 && action_id == 573) {
        BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
        MES_PUSH(MsgFieldBroken);
        work->unknown_868 = 0;
    }
    BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
    if ((action->effect != 75 || slot != 0)
        && (BattleFx_IsReviveFar(action->effect) != 0 || target->hp != 0
            || BattleFx_CanAffectDefeatedUnit(action->effect) != 0)
        && hit != 0) {
        switch (action->effect) {

        case EFX_CURE_ALL:
            if (target->delusion != 0) {
                target->delusion = 0;
                MES_PUSH(MsgCureDelusion);
                BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            }
            if (target->stun != 0) {
                target->stun = 0;
                BattleEv_Push(BATTLE_EVENT_RESET, 0);
                BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
                MES_PUSH(MsgCureStun);
            }
            if (target->sleep != 0) {
                target->sleep = 0;
                MES_PUSH(MsgWakes);
                BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            }
            if (target->psy_seal != 0) {
                target->psy_seal = 0;
                MES_PUSH(MsgCureSeal);
                BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            }
            if (target->death_count != 0) {
                target->death_count = 0;
                MES_PUSH(MsgCureCurse);
                BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            }
            if (target->evil_spirit != 0) {
                target->evil_spirit = 0;
                MES_PUSH(MsgCureSpirit);
                BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            }
            if (target->poison != 0) {
                MES_PUSH(MsgCurePoison);
                target->poison = 0;
            }
            BattleEv_Push(BATTLE_EVENT_RESET, 0);
            break;

        case EFX_CURE_PART:
            if (target->delusion != 0) {
                target->delusion = 0;
                BattleEv_Push(BATTLE_EVENT_RESET, 0);
                BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
                MES_PUSH(MsgCureDelusion);
            }
            if (target->stun != 0) {
                target->stun = 0;
                BattleEv_Push(BATTLE_EVENT_RESET, 0);
                BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
                MES_PUSH(MsgCureStun);
            }
            if (target->sleep != 0) {
                target->sleep = 0;
                BattleEv_Push(BATTLE_EVENT_RESET, 0);
                BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
                MES_PUSH(MsgWakes);
            }
            if (target->psy_seal != 0) {
                target->psy_seal = 0;
                BattleEv_Push(BATTLE_EVENT_RESET, 0);
                BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
                MES_PUSH(MsgCureSeal);
            }
            if (target->death_count == 0)
                break;
            target->death_count = 0;
            BattleEv_Push(BATTLE_EVENT_RESET, 0);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            MES_PUSH(MsgCureCurse);
            break;

        case EFX_HEAL_60:
        case EFX_HEAL_30:
        case 70:
        case 71:
        case 76:
        {
            s32 old;
            s32 maxu;
            s32 maxv;
            s32 heal;
            u16 *stat_ptr;

            /* FAKEMATCH: walking a pointer down from hp to max_hp reads each stat twice, unsigned and signed, as the ROM does; plain field reads share one load. */
            stat_ptr = (u16 *)&target->hp;
            old = *stat_ptr--;
            heal = *(s16 *)(stat_ptr + 1);
            if (action->effect == 76) {
                maxv = target->max_hp;
                maxu = (u16)target->max_hp;
                heal += maxv * 2 / 5;
            } else if (action->effect == 71) {
                maxv = target->max_hp;
                maxu = (u16)target->max_hp;
                heal += maxv * 7 / 10;
            } else if (action->effect == 70) {
                maxu = (u16)target->max_hp;
                heal += (s16)maxu / 2;
            } else
            {
                stat_ptr--;
                maxu = *stat_ptr--;
                maxv = *(s16 *)(stat_ptr + 1);
                if (action->effect == EFX_HEAL_60)
                    heal += maxv * 60 / 100;
                else
                    heal += maxv * 30 / 100;
            }
            if (heal > (s16)maxu)
                heal = (s16)maxu;
            tmp = heal - (s16)old;
            if (tmp == 0 && nibble != BATTLE_DAMAGE_HP_HEAL)
                break;
            if (heal == (s16)maxu)
                MES_PUSH(MsgHpFull);
            else {
                BattleEv_Push(BATTLE_EVENT_VALUE, tmp);
                MES_PUSH(MsgHpRecover);
            }
            target->hp = heal;
            Owner_RecalculateRatiosFar(target_id);
            break;
        }

        case EFX_PP_RESTORE_7:
        case 77:
        case 78:
        {
            s32 old;
            s32 maxv;
            s32 heal;
            s32 maxu;

            heal = target->pp;
            old = (u16)target->pp;
            if (action->effect == 77) {
                heal += (s16)(target->max_pp / 10);
                maxu = (u16)target->max_pp;
            } else if (action->effect == 78) {
                maxv = target->max_pp;
                maxu = (u16)target->max_pp;
                heal += maxv * 3 / 10;
            } else {
                maxv = target->max_pp;
                maxu = (u16)target->max_pp;
                heal += maxv * 7 / 100;
            }
            if (heal > (s16)maxu)
                heal = (s16)maxu;
            tmp = heal - (s16)old;
            if (tmp == 0 && nibble != BATTLE_DAMAGE_PP_HEAL)
                break;
            if (heal == (s16)maxu)
                MES_PUSH(MsgPpFull);
            else {
                BattleEv_Push(BATTLE_EVENT_VALUE, tmp);
                MES_PUSH(MsgPpRecover);
            }
            target->pp = heal;
            Owner_RecalculateRatiosFar(target_id);
            break;
        }

        case EFX_AGI_SET_UP8:
            target->agility_modifier = 8;
            target->agility_modifier_turns = 5;
            Owner_RecalculateStatsFar(target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, target->agility - copy->agility);
            MES_PUSH(MsgAgiUp);
            break;

        case EFX_AGI_SET_DOWN4:
            am = &target->agility_modifier;
        {
            u8 v;

            v = -4;
            *am = v;
        }
            target->agility_modifier_turns = 5;
            Owner_RecalculateStatsFar(target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, copy->agility - target->agility);
            MES_PUSH(MsgAgiDown);
            break;

        case EFX_ATK_DOWN1:
            target->attack_modifier -= 1;
            CLAMP_MOD(target->attack_modifier);
            Owner_RecalculateStatsFar(target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, copy->attack - target->attack);
            MES_PUSH(MsgAtkDown);
            target->attack_modifier_turns = 7;
            break;

        case EFX_ATK_DOWN2:
            target->attack_modifier -= 2;
            CLAMP_MOD(target->attack_modifier);
            Owner_RecalculateStatsFar(target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, copy->attack - target->attack);
            MES_PUSH(MsgAtkDown);
            target->attack_modifier_turns = 7;
            break;

        case EFX_ATK_UP1:
            target->attack_modifier += 1;
            CLAMP_MOD(target->attack_modifier);
            Owner_RecalculateStatsFar(target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, target->attack - copy->attack);
            MES_PUSH(MsgAtkUp);
            target->attack_modifier_turns = 7;
            break;

        case EFX_ATK_UP2:
            target->attack_modifier += 2;
            CLAMP_MOD(target->attack_modifier);
            Owner_RecalculateStatsFar(target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, target->attack - copy->attack);
            MES_PUSH(MsgAtkUp);
            target->attack_modifier_turns = 7;
            break;

        case EFX_DEF_DOWN1:
            target->defense_modifier -= 1;
            CLAMP_MOD(target->defense_modifier);
            Owner_RecalculateStatsFar(target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, copy->defense - target->defense);
            MES_PUSH(MsgDefDown);
            target->defense_modifier_turns = 7;
            break;

        case EFX_DEF_DOWN2:
            target->defense_modifier -= 2;
            CLAMP_MOD(target->defense_modifier);
            Owner_RecalculateStatsFar(target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, copy->defense - target->defense);
            MES_PUSH(MsgDefDown);
            target->defense_modifier_turns = 7;
            break;

        case EFX_DEF_UP1:
            target->defense_modifier += 1;
            CLAMP_MOD(target->defense_modifier);
            Owner_RecalculateStatsFar(target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, target->defense - copy->defense);
            MES_PUSH(MsgDefUp);
            target->defense_modifier_turns = 7;
            break;

        case EFX_DEF_UP2:
            target->defense_modifier += 2;
            CLAMP_MOD(target->defense_modifier);
            Owner_RecalculateStatsFar(target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, target->defense - copy->defense);
            MES_PUSH(MsgDefUp);
            target->defense_modifier_turns = 7;
            break;

        case EFX_REVIVE_FULL:
            if (target->hp != 0)
                break;
            MES_PUSH(MsgRevived);
            target->hp = target->max_hp;
            Owner_RecalculateRatiosFar(target_id);
            break;

        case EFX_REVIVE_HALF:
            if (target->hp != 0)
                break;
            MES_PUSH(MsgRevived);
            target->hp = target->max_hp / 2;
            Owner_RecalculateRatiosFar(target_id);
            break;

        case EFX_REVIVE_80:
            if (target->hp != 0)
                break;
            MES_PUSH(MsgRevived);
            target->hp = target->max_hp * 8 / 10;
            Owner_RecalculateRatiosFar(target_id);
            break;

        case 73:
            if (target->hp != 0)
                break;
            MES_PUSH(MsgRevived);
            target->hp = (s16)(target->max_hp * 6 / 10);
            Owner_RecalculateRatiosFar(target_id);
            break;

        case EFX_CURE_POISON:
            if (target->poison != 0)
                MES_PUSH(MsgCurePoison);
            target->poison = 0;
            break;

        case EFX_RES_DOWN1:
            ADJUST_RES(-1, (copy->res_modifier - target->res_modifier) * 20, MsgResDown);
            break;

        case EFX_RES_DOWN2:
        {
            target->res_modifier -= 2;
            if (-4 > target->res_modifier)
                target->res_modifier = -4;
            if (target->res_modifier > 4)
                target->res_modifier = 4;
            BattleEv_Push(BATTLE_EVENT_VALUE, (copy->res_modifier - target->res_modifier) * 20);
            MES_PUSH(MsgResDown);
            target->res_modifier_turns = 7;
        }
            break;

        case EFX_RES_UP1:
            ADJUST_RES(1, (target->res_modifier - copy->res_modifier) * 20, MsgResUp);
            break;

        case EFX_RES_UP2:
        {
            target->res_modifier += 2;
            if (*(rm = &target->res_modifier) < -4)
                target->res_modifier = -4;
            if (target->res_modifier > 4)
                target->res_modifier = 4;
            BattleEv_Push(BATTLE_EVENT_VALUE, (target->res_modifier - copy->res_modifier) * 20);
            MES_PUSH(MsgResUp);
            target->res_modifier_turns = 7;
        }
            break;

        case EFX_POISON:
            if (target->poison != 0)
                break;
            MES_PUSH(MsgPoisoned);
            target->poison = 1;
            break;

        case EFX_VENOM:
            if (target->poison > 1)
                break;
            MES_PUSH(MsgVenom);
            target->poison = 2;
            break;

        case EFX_DELUSION:
        case 75:
            SET_STATUS7(target->delusion, MsgDelusion);
            break;

        case EFX_CONFUSE:
            SET_STATUS7(target->confusion, MsgConfused);
            break;

        case EFX_CHARM:
            SET_STATUS7(target->charm, MsgCharmed);
            break;

        case EFX_STUN:
        case 85:
            SET_STATUS7(target->stun, MsgStunned);
            break;

        case EFX_SLEEP:
            SET_STATUS7(target->sleep, MsgAsleep);
            break;

        case EFX_PSY_BLOCK:
            TEXT_SIDE(MsgPsyBlock, MsgPsySeal);
            target->psy_seal |= 7;
            break;

        case EFX_PSY_SEAL:
            TEXT_SIDE(MsgPsyBlock, MsgPsySeal);
            target->psy_seal |= 16;
            break;

        case EFX_INSTANT_DOWN:
            if (BattleUnit_KeepsOneHp(target_id) != 0)
                break;
            BattleEv_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
            if (target->status_12a == BATTLE_UNIT_PARTY)
                MES_PUSH(MsgKoDown);
            else if (action_id == 219)
                MES_PUSH(MsgSuffocate);
            else
                MES_PUSH(MsgSpiritDrain);
            target->hp = 0;
            Owner_RecalculateRatiosFar(target_id);
            break;

        case EFX_REFRAIN:
            MES_PUSH(MsgHpRegen);
            target->refrain = 5;
            break;

        case EFX_REFLECT:
            SET_STATUS7(target->reflect, MsgReflect);
            break;

        case EFX_DRAIN_HP:
        case EFX_DRAIN_HP_HALF:
        {
            s32 heal;

            heal = actor->hp;
            apwr = dealt;
            if (action->effect == EFX_DRAIN_HP_HALF)
                apwr /= 2;
            dmg = apwr;
            heal += dmg;
            if (heal > actor->max_hp) {
                heal = actor->max_hp;
                dmg = heal - actor->hp;
            }
            BattleEv_Push(BATTLE_EVENT_RESET, 0);
            BattleEv_Push(BATTLE_EVENT_UNIT, actor_id);
            if (heal == actor->max_hp)
                MES_PUSH(MsgHpFull);
            else {
                BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
                MES_PUSH(MsgHpRecover);
            }
            actor->hp = heal;
            Owner_RecalculateRatiosFar(actor_id);
            break;
        }

        case EFX_DRAIN_PP:
        {
            s32 heal;
            s32 amt;

            heal = actor->pp;
            amt = dealt;
            heal += amt;
            if (heal > actor->max_pp) {
                heal = actor->max_pp;
                amt = heal - actor->pp;
            }
            BattleEv_Push(BATTLE_EVENT_RESET, 0);
            BattleEv_Push(BATTLE_EVENT_UNIT, actor_id);
            if (heal == actor->max_pp)
                MES_PUSH(MsgPpFull);
            else {
                BattleEv_Push(BATTLE_EVENT_VALUE, amt);
                MES_PUSH(MsgPpRecover);
            }
            actor->pp = heal;
            Owner_RecalculateRatiosFar(actor_id);
            break;
        }

        case EFX_PP_LEECH:
            dmg = dealt / 10;
            if (target->pp < dmg)
                dmg = target->pp;
            if (actor->pp + dmg > actor->max_pp)
                dmg = actor->max_pp - actor->pp;
            if (dmg == 0)
                break;
            BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
            TEXT_SIDE(MsgLeechTake, MsgLeechGain);
            Owner_AdjustSecondValueFar(actor_id, dmg);
            break;

        case 84:
            dmg = (s16)(target->max_pp / 10);
            if (target->pp < dmg)
                dmg = target->pp;
            if (dmg == 0)
                break;
            BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
            TEXT_SIDE(MsgMorePpLossP, MsgMorePpLossE);
            Owner_AdjustSecondValueFar(target_id, -dmg);
            break;

        case EFX_BUFF_CLEAR:
            if (target->attack_modifier > 0) {
                target->attack_modifier = 0;
                target->attack_modifier_turns = 0;
            }
            if (target->defense_modifier > 0) {
                target->defense_modifier = 0;
                target->defense_modifier_turns = 0;
            }
            if (target->res_modifier > 0) {
                target->res_modifier = 0;
                target->res_modifier_turns = 0;
            }
            if (target->agility_modifier > 0)
                target->agility_modifier = 0;
            target->element_modifier[0] = 0;
            target->element_modifier[1] = 0;
            target->element_modifier[2] = 0;
            target->element_modifier[3] = 0;
            MES_PUSH(MsgBuffsReset);
            break;

        case EFX_EVIL_SPIRIT:
            MES_PUSH(MsgEvilSpirit);
            target->evil_spirit = 1;
            break;

        case EFX_DEATH_CURSE:
        case 80:
            if (target->death_count == 0) {
                MES_PUSH(MsgDeathCurse);
                target->death_count = 7;
                break;
            }
            if (target->death_count <= 1)
                break;
            target->death_count -= 1;
            BattleEv_Push(BATTLE_EVENT_VALUE, target->death_count);
            MES_PUSH(MsgDeathCount);
            break;

        case EFX_READY:
            MES_PUSH(MsgReadies);
            target->ready_pose = 2;
            break;

        case EFX_CHALLENGE:
            MES_PUSH(MsgChallenge);
            target->battle_end_state = 1;
            if (IS_PARTY(target_id))
                work->flags_043 |= 2;
            break;

        case EFX_IMMOBILIZE:
        case 83:

            MES_PUSH(MsgImmobile);
            target->cannot_move = 1;
            break;

        case EFX_GUARD1:

            MES_PUSH(MsgAura);
            if (target->guard_level > 0)
                break;
            target->guard_level = 1;
            break;

        case 72:
            MES_PUSH(MsgAura);
            if ((u32)target->guard_level > 1)
                break;
            target->guard_level = 2;
            break;

        case EFX_GUARD2:
        case 88:
            MES_PUSH(MsgAura2);
            if (target->guard_level > 2)
                break;
            target->guard_level = 3;
            break;

        case 79:
            MES_PUSH(MsgVanished);
            if ((u32)target->guard_level > 3)
                break;
            target->guard_level = 4;
            break;

        case EFX_TEXT_NONE:
            BattleEv_Push(BATTLE_EVENT_TEXT, -1);
            break;

        case 81:
            MES_PUSH(MsgAllResting);
            work->unknown_046[1] = 1;
            break;

        case 82:
            MES_PUSH(MsgBlownAway);
            BattleEv_Push(15, target_id);
            break;

        case 74:
            MES_PUSH(MsgCounterReady);
            target->unknown_142[1] = 1;
            break;

        default:
            break;
        }
    }

done:
    BattleEv_Push(BATTLE_EVENT_RESET, 0);
    if (plan->command != 9 && (s8)target->unknown_142[1] != 0
        && action->target_mode == 1 && target->hp != 0) {
        work->unknown_858 = target_id;
        work->unknown_85c = actor_id;
    }

    if (target->hp != 0) {
        if (target->sleep != 0)
        if (target->sleep <= 6
            && dealt > 0 && (3 & BattleRandom16Far()) == 0) {
            target->sleep = 0;
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            MES_PUSH(MsgWakes);
        }
    }
    Sys_Free(copy);
    Owner_RecalculateStatsFar(target_id);
    UiWindow_DrawPartyStatusContentsFar(Ram_HeapSlots->battle_work->party_status_mode);
    if (target->hp != 0)
        BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
    if (action_id != 403 && actor->evil_spirit != 0
        && (BattleRandom16Far() & 3) == 0 && dealt > 0) {
        s32 share;

        share = dealt >> 2;
        if (share == 0)
            share = 1;
        plan->pending_amount_60 += share;
    }
}
