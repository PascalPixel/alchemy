#include "../../INCLUDE/TYPES.H"
#include "../../INCLUDE/IWRAM_CALL.H"
#include "../../INCLUDE/BATTLE_ACTOR.H"
#include "../../INCLUDE/BATTLE_CALC.H"
#include "../../INCLUDE/BATTLE_COMMAND.H"
#include "../../INCLUDE/BATTLE_EFX.H"
#include "../../INCLUDE/BATTLE_EVENT.H"
#include "../../INCLUDE/BATTLE_MSG.H"
#include "../../INCLUDE/BATTLE_RUNTIME.H"
#include "../../INCLUDE/BATTLE_SUMMON.H"
#include "../../INCLUDE/BATTLE_TYPES.H"
#include "../../INCLUDE/BATTLE_WORK.H"
#include "../../INCLUDE/MOTION_OBJECT.H"
#include "../../INCLUDE/RUNTIME_1E74.H"
#include "../../INCLUDE/RUNTIME_MEM.H"

void BattlePresentation_SpawnActorObject(void *object, s32 unit, s32 x, s32 y);
void BattleActor_CommitPlacement(void);
s32 BattleParty_ListPresentEnemies(s16 *entries);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);

/* 行動1件の対象解決。コピーを取り、命中とダメージ種別を決めて効果を出す。 */

#define CLAMP_MOD(v)                                                           \
    {                                                                          \
        if ((v) < -4)                                                          \
            (v) = -4;                                                          \
        if ((v) > 4)                                                           \
            (v) = 4;                                                           \
    }

/* Power against the target's resistance in the action's element. */
#define TAKE_BONUS()                                                           \
    {                                                                          \
        if (range != 4)                                                        \
            bonus = power - target->elements[range].resist;                    \
    }


#define TEXT_SIDE(player, enemy)                                               \
{                                                                              \
    if ((u32)target_id <= 7)                                                   \
        BattleEv_Push(BATTLE_EVENT_TEXT, (player));                         \
    else                                                                       \
        BattleEv_Push(BATTLE_EVENT_TEXT, (enemy));                          \
}

/* Guard levels 1 and 2 cut the damage to a half and a tenth. */
#define APPLY_GUARD()                                                         \
    {                                                                         \
        guard = target->guard_level;                                          \
        if (guard != 0) {                                                     \
            if (guard == 1)                                                   \
                dmg /= 2;                                                     \
            else                                                              \
                dmg = Math_Div(dmg, 10);                                      \
        }                                                                     \
    }

#define ADJUST_RES(delta, value_expr, text)                                 \
{                                                                              \
    target->res_modifier += (delta);                                             \
    CLAMP_MOD(target->res_modifier);                                             \
    BattleEv_Push(BATTLE_EVENT_VALUE, (value_expr));                        \
    BattleEv_Push(BATTLE_EVENT_TEXT, (text));                               \
    target->res_modifier_turns = 7;                                                    \
}

#define SET_STATUS7(field, text)                                               \
{                                                                              \
    BattleEv_Push(BATTLE_EVENT_TEXT, (text));                                  \
    (field) = 7;                                                               \
}

s32 Battle_ResolveTargetAction(struct BattlePlan *plan, s32 slot)
{
    /*
     * 宣言順はスピルスロット順(先頭ほど高位)。参照の割付:
     * offset=fp action=76 actor=72 actor_id=68 action_id=64 bonus=60
     * work=56 half=52 adjust=48 dealt=44 crush=40 hit=36 modifier=32
     * skip=28 nibble=24 affinity=20 copy=16 power=12 (temp)=8 cmd=4
     * target=r7 target_id=sl range=r9 saved=84..
     */
    s32 offset;
    struct BattleAction *action;
    struct BattleUnit *actor;
    s32 actor_id;
    s32 action_id;
    s32 bonus;
    struct BattleSession *work;
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
    s32 g1;
    s32 power;
    s16 hp0;
    s32 kind;
    s16 *cmd;
    s32 target_id;
    s8 *rm;
    s32 range;
    struct BattleUnit *target;
    s32 n;
    s32 value;
    s32 dmg;
    s32 scale;
    s32 pass;
    s8 *am;
    s32 guard;
    s16 saved[8];
    s32 count;
    s32 tmp;
    s32 size;
    bonus = 0;
    work = gBattleWork;
    half = 0;
    dealt = 0;
    crush = 0;
    skip = 0;
    affinity = 0;
    size = sizeof(struct BattleUnit);
    copy = (struct BattleUnit *)Runtime_BumpAllocate(size);

    actor_id = plan->actor_id;
    target_id = plan->target_ids[slot];
    action_id = plan->action_id;
    range = plan->range_index;
    adjust = plan->target_adjustments[slot];
    modifier = plan->target_modifiers[slot];

    action = BattleAction_Get(action_id);
    actor = Owner_GetStateFar(actor_id);
    target = Owner_GetStateFar(target_id);
    Iwram_CopyWords(copy, target, size);

    if (action->range != 255) {
        offset = plan->target_offsets[slot];
        if (offset < 0)
            offset = -offset;
    } else {
        offset = 0;
    }

    /* 属性相性。テーブルを二方向に走査して符号を決める。 */
    if (range != 4) {
        struct BattleElementLevel *levels;
        s32 i;

        value = target->element_levels[range].level;
        levels = target->element_levels;
        i = 0;
        if (value >= levels[0].level) {
            struct BattleElementLevel *p;

            p = levels;
            do {
                i++;
                p++;
                if (i > 3)
                    break;
            } while (value >= p->level);
        }
        if (i == 4)
            affinity = -1;

        i = 0;
        {
            s32 off;

            /* FAKEMATCH: a byte offset held in a temporary makes the second
               scan reload the first level, as the ROM does. */
            off = 2;
            if (value <= *(s16 *)((u8 *)levels + off)) {
                do {
                    i++;
                    if (i > 3)
                        break;
                } while (value <= target->element_levels[i].level);
            }
        }
        if (i == 4)
            affinity = 1;
    }

    /* 攻撃力。元素武器でなければ 100。 */
    if ((u32)plan->range_index <= 3) {
        cmd = &plan->command;
        if (*cmd != 2) {
            power = actor->elements[plan->range_index].power;
            goto after_power;
        }
    } else
        cmd = &plan->command;
    power = 100;
after_power:

    if (plan->command == 5 && (u32)plan->range_index <= 3 && affinity > 0) {
        s32 chance;

        chance = power - target->elements[plan->range_index].resist;
        chance += 30;
        chance *= 0x28f;
        if (chance > (BattleRandom16Far() & 0xffff))
            BattleEv_Push(BATTLE_EVENT_SCRIPT_UPDATE, 5);
    }


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
    if ((u8)(action->effect + 206) <= 1) {
        s32 st;
        s32 rec;

        st = actor->class_id;
        rec = Summon_FindSlot();
        if (action->effect == EFX_STANDBY_WORK)
            st = BattleFormation_SelectRandomAvailableMember(work->enemy_group);
        if (hit != 0 && Summon_ClassValid(st) != 0 && rec >= 0) {
            s32 ch;
            s16 *slots;
            s32 cursor;

            ch = Summon_TakeCharge(st, 1);
            if (ch & 0x8000)
                Summon_ResetCharge(st);
            BattleUnit_AssignFar(rec, st, ch & 0x7fff);
            /* FAKEMATCH: the enemy list is reached by byte offsets from a
               base 50 entries before it, as the ROM addresses it. */
            slots = work->enemy_units - 50;
            {
                s32 off;
                s32 i;
                s32 j;
                s32 jsave;

                off = 100;
                i = 0;
                jsave = 0;
                if (*(s16 *)((u8 *)slots + off) == 254) {
                    *(s16 *)((u8 *)slots + off) = rec;
                } else {
                    s32 woff;
                    s32 next;
                    u8 *base;

                    j = 0;
                    woff = 100;
                    for (;;) {
                        base = (u8 *)slots;
                        cursor = j + 100;
                        n = *(s16 *)(cursor + (s32)base);
                        if (n == 255) {
                            *(s16 *)((s32)base + cursor) = rec;
                            next = jsave + 102;
                            *(s16 *)(base + next) = n;
                            break;
                        }
                        i++;
                        next = j + 2;
                        j = next;
                        if (i > 5)
                            break;
                        jsave = next;
                        woff = next + 100;
                        if (*(s16 *)(woff + (s32)base) == 254) {
                            *(s16 *)(woff + (s32)base) = rec;
                            break;
                        }
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
                if (listed > 0) {
                    u16 *q;

                    q = (u16 *)saved;
                    count = listed;
                    do {
                        Actor_ResetMotionAtAnchor(*q++);
                        count--;
                    } while (count != 0);
                }
            }
            BattleEv_Push(BATTLE_EVENT_UNIT, rec);
            if (action_id != 0x1f7)
                BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgAppears);
            else
                BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgSplitOff);
        } else if (action_id == 0x1f7) {
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgSplitFail);
        } else {
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgNoOneCame);
        }
    }


    if (hit != 0) {
        s32 efx;

        efx = action->effect;
        if (action->effect == EFX_IMMOBILIZE) {
            s32 hidx;

            hit = 0;
            /* FAKEMATCH: the first record's offset is held in a temporary
               and added to the work pointer, as the ROM addresses it. */
            hidx = (u8 *)&work->actions[0].unit_id - (u8 *)work;
            if (*(s16 *)((u8 *)work + hidx) == target_id) {
                hit = 1;
                goto hit_effect_done;
            } else {
                do {
                    n = 0;
                scan_next:
                    n++;
                    if ((u32)n <= 19) {
                        if (work->actions[n].unit_id == target_id)
                            hit = 1;
                        else
                            goto scan_next;
                    }
                    goto hit_effect_done;
                } while (0);
            }
        }
        if (action->effect == EFX_HALF_DEF) {
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
                nibble = 10;
            else
                hit = 0;
        }
    hit_effect_done:
        ;
    }

    /* ダメージ種別。HP が残っているか、分類が非ゼロなら種別スイッチへ。 */
    if (skip == 0
        && (target->hp != 0 || BattleFx_IsReviveFar(action->effect) != 0)) {
        s32 pp;
        s32 cur;


        /*
         * -1 PPダメージ(別系)  1 HP回復  2 HPダメージ  3/4 武器攻撃
         *  5/6/8 加算攻撃(アイテム・distance テーブル別)  10 PP吸収系  11 PP回復
         *  0/7/9 は空。
         */
        switch (nibble) {
        case BATTLE_DAMAGE_WEAPON:
        case BATTLE_DAMAGE_WEAPON_SCALED:
        {
            s32 def;
            s32 apwr;


            hp0 = target->hp;
            def = target->defense;
            scale = def;
            cur = hp0;
            if (half != 0)
                scale = (u32)def >> 1;
            pass = 1;
            do {
                TAKE_BONUS();
                if (pass == 0)
                    bonus = 0;
                apwr = action->power;
                if (nibble == 4)
                    dmg = Math_Div(
                        Battle_CalcAttack(actor->attack, scale, 0,
                                          bonus)
                            * apwr,
                        10);
                else {

                    dmg = Battle_CalcAttack(actor->attack, scale, apwr,
                                            bonus);
                }
                dmg *= adjust;
                if (modifier != 0) {
                    if (modifier == 1)
                        dmg = dmg * 5 / 4;
                    else
                        dmg = dmg * 3 / 2;
                    dmg += (u8)Math_DivU(target->level, 5) + 6;
                    if (pass == 0) {
                        BattleEv_Push(BATTLE_EVENT_MARK, 0);
                        {
                            s32 text;

                            text = (s32)&MsgCritical;
                            if ((u32)target_id <= 7)
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
                    if (dmg < cur - 1) {
                        dmg = cur - 1;
                        if (dmg <= 0)
                            dmg = 1;
                    }
                }
                if (GameFlag_TestFar(366) != 0 && *cmd == 5
                    && cur <= dmg) {
                    dmg = cur - 1;
                }
                pass++;
            } while (pass <= 1);
            BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            cur -= dmg;
            BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
            {
                s32 text;

                if ((u32)target_id <= 7)
                    text = (s32)&MsgDmgEmphP + affinity;
                else
                    text = (s32)&MsgDmgEmphE + affinity;
                BattleEv_Push(BATTLE_EVENT_TEXT, text);
            }

            if (cur <= 0) {
                BattleEv_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
                cur = 0;
                TEXT_SIDE((s32)&MsgGoesDown, (s32)&MsgFelled);
            } else
                BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->hp - cur;
            target->hp = (s16)cur;
            Owner_RecalculateRatiosFar(target_id);
            break;
        }

        case BATTLE_DAMAGE_PP_DRAIN:
        {

            if (action->power == 0)
                break;
            pp = target->pp;
            TAKE_BONUS();
            dmg = action->power;
            dmg = Battle_CalcPower(dmg, bonus, 256);
            dmg = Math_Div(dmg * PpLossFalloff[offset], 100);
            dmg *= adjust;
            APPLY_GUARD();
            if (action->effect == EFX_DRAIN_PP && dmg > pp)
                dmg = pp;

            BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;

                if ((u32)target_id <= 7)
                    text = (s32)&MsgPpLossP;
                else
                    text = (s32)&MsgPpLossE;
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

            dmg = action->power;
            dmg = Battle_CalcRestore(dmg, range == 4 ? 100 : power, 256);
            dmg = Math_Div(dmg * HpHealFalloff[offset], 100);
            dmg *= adjust;
            dmg += BattleRandom16Far() & 3;
            cur += dmg;
            if (cur > target->max_hp) {
                cur = target->max_hp;
                dmg = cur - target->hp;
            }
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            if (cur == target->max_hp)
                BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgHpFull);
            else {
                BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
                BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgHpRecover);
            }
            dealt = target->hp - cur;
            target->hp = (s16)cur;
            Owner_RecalculateRatiosFar(target_id);
            break;
        }

        case BATTLE_DAMAGE_PP_DIRECT:
        {

            if (action->power == 0)
                break;
            pp = target->pp;
            TAKE_BONUS();
            dmg = action->power;
            dmg = Battle_CalcPower(dmg, bonus, 256);
            dmg = Math_Div(dmg * PpDmgFalloff[offset], 100);
            dmg *= adjust;
            APPLY_GUARD();
            BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;

                if ((u32)target_id <= 7)
                    text = (s32)&MsgDmgP;
                else
                    text = (s32)&MsgDmgE;
                BattleEv_Push(BATTLE_EVENT_TEXT, text);
                pp -= dmg;
            }
            if (pp <= 0)
                pp = 0;
            BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            goto pp_store;
        }

        case BATTLE_DAMAGE_HP_ADDITIVE_6:
        case BATTLE_DAMAGE_HP_ADDITIVE_7:
        case BATTLE_DAMAGE_HP_ADDITIVE_9:
        {


            if (action->power == 0)
                break;
            cur = target->hp;
            pass = 1;
            do {
                TAKE_BONUS();
                if (pass == 0)
                    bonus = 0;
                dmg = action->power;
                if (*cmd == 6) {
                    s32 item;

                    item = action_id - 380;
                    if ((u32)item <= 21) {
                        switch (item) {
                        case 0:
                        case 6:
                        case 12:
                        case 18:
                            kind = 3;
                            break;
                        case 1:
                        case 7:
                        case 13:
                        case 19:
                            kind = 6;
                            break;
                        case 2:
                        case 8:
                        case 14:
                        case 20:
                            kind = 9;
                            break;
                        case 3:
                        case 9:
                        case 15:
                        case 21:
                            kind = 12;
                            break;
                        }
                    }
                    dmg += Math_Div(target->max_hp * kind, 100);
                }
                dmg = Battle_CalcPower(dmg, bonus, 256);
                dmg *= adjust;
                switch (nibble & 15) {
                case 5:
                    dmg = Math_Div(HpDmgFalloff5[offset] * dmg, 100);
                    break;
                case 8:
                    dmg = Math_Div(dmg * HpDmgFalloff8[offset], 100);
                    break;
                case 6:
                    dmg = Math_Div(dmg * HpDmgFalloff6[offset], 100);
                    break;
                }
                dmg += BattleRandom16Far() & 3;
                APPLY_GUARD();
                if (GameFlag_TestFar(366) != 0 && *cmd == 6 && cur > dmg) {
                    dmg = cur;
                }
                pass++;
            } while (pass <= 1);
            BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;

                if ((u32)target_id <= 7)
                    text = (s32)&MsgDmgEmphP + affinity;
                else
                    text = (s32)&MsgDmgEmphE + affinity;
                BattleEv_Push(BATTLE_EVENT_TEXT, text);
                cur -= dmg;
            }

            if (cur <= 0) {
                BattleEv_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
                {
                    s32 text;

                    cur = 0;
                    if ((u32)target_id <= 7)
                        text = (s32)&MsgGoesDown;
                    else
                        text = (s32)&MsgFelled;
                    BattleEv_Push(BATTLE_EVENT_TEXT, text);
                }
            } else
                BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->hp - cur;
            target->hp = (s16)cur;
            Owner_RecalculateRatiosFar(target_id);
            break;
        }

        case BATTLE_DAMAGE_PP_HEAL:
        {
            if (action->power == 0)
                break;
            pp = target->pp;
            dmg = action->power;
            dmg = Battle_CalcRestore(dmg, range == 4 ? 100 : power, 256);
            dmg = Math_Div(dmg * PpHealFalloff[offset], 100);
            dmg *= adjust;
            pp += dmg;
            if (pp > target->max_pp) {
                pp = target->max_pp;
                dmg = pp - target->pp;
            }
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            if (pp == target->max_pp)
                BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgPpFull);
            else {
                BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
                BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgPpRecover);
            }
pp_store:
            target->pp = (s16)pp;
            Owner_RecalculateRatiosFar(target_id);
            break;
        }

        case 0:
        case 7:
        case 9:
            /* 空スロット */
            break;

        case BATTLE_DAMAGE_HP_POWER:
            if (hit != 0) {
            if (action->power == 0)
                break;
            cur = target->hp;
            TAKE_BONUS();
            dmg = action->power;
            dmg = Battle_CalcPower(dmg, bonus, 256);
            dmg *= adjust;
            dmg = Math_Div(dmg * HpDmgFalloff[offset], 100);
            APPLY_GUARD();
            BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;

                if ((u32)target_id <= 7)
                    text = (s32)&MsgDmgP;
                else
                    text = (s32)&MsgDmgE;
                BattleEv_Push(BATTLE_EVENT_TEXT, text);
                cur -= dmg;
            }

            if (cur <= 0) {
                BattleEv_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
                cur = 0;
                TEXT_SIDE((s32)&MsgGoesDown, (s32)&MsgFelled);
            } else
                BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->hp - cur;
            target->hp = (s16)cur;
            Owner_RecalculateRatiosFar(target_id);
            break;
            }
            BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgNoEffect);
            break;

        }
    }

    /* 付加効果 */

    BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
    if (BattleFx_IsReviveFar(action->effect) == 0 && target->hp == 0
        && BattleFx_CanAffectDefeatedUnit(action->effect) == 0)
        goto done;
    if (hit == 0)
        goto done;
    if ((u32)(action->effect - 3) > 69 - 3)
        goto done;

    switch (action->effect) {

    case EFX_CURE_ALL:
        if (target->delusion != 0) {
            target->delusion = 0;
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureDelusion);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->stun != 0) {
            target->stun = 0;
            BattleEv_Push(BATTLE_EVENT_RESET, 0);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureStun);
        }
        target->sleep = 0;
        if (target->psy_seal != 0) {
            target->psy_seal = 0;
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureSeal);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->death_count != 0) {
            target->death_count = 0;
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureCurse);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->evil_spirit != 0) {
            target->evil_spirit = 0;
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureSpirit);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->poison != 0) {
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCurePoison);
            target->poison = 0;
        }
        BattleEv_Push(BATTLE_EVENT_RESET, 0);
        break;

    case EFX_CURE_PART:
        if (target->delusion != 0) {
            target->delusion = 0;
            BattleEv_Push(BATTLE_EVENT_RESET, 0);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureDelusion);
        }
        if (target->stun != 0) {
            target->stun = 0;
            BattleEv_Push(BATTLE_EVENT_RESET, 0);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureStun);
        }
        target->sleep = 0;
        if (target->psy_seal != 0) {
            target->psy_seal = 0;
            BattleEv_Push(BATTLE_EVENT_RESET, 0);
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureSeal);
        }
        if (target->death_count == 0)
            break;
        target->death_count = 0;
        BattleEv_Push(BATTLE_EVENT_RESET, 0);
        BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCureCurse);
        break;

    case EFX_HEAL_60:
    case EFX_HEAL_30:
    {
        s32 old;
        s32 maxu;
        s32 maxv;
        s32 heal;
        u16 *stat_ptr;

        /* FAKEMATCH: walking a pointer down from hp to max_hp reads each
           stat twice, unsigned and signed, as the ROM does; plain field
           reads share one load. */
        stat_ptr = (u16 *)&target->hp;
        old = *stat_ptr--;
        heal = *(s16 *)(stat_ptr + 1);
        stat_ptr--;
        maxu = *stat_ptr--;
        maxv = *(s16 *)(stat_ptr + 1);
        if (action->effect == EFX_HEAL_60)
            heal += Math_Div(maxv * 60, 100);
        else
            heal += Math_Div(maxv * 30, 100);
        if (heal > (s16)maxu)
            heal = (s16)maxu;
        tmp = heal - (s16)old;
        if (tmp == 0 && nibble != 1)
            break;
        if (heal == (s16)maxu)
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgHpFull);
        else {
            BattleEv_Push(BATTLE_EVENT_VALUE, tmp);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgHpRecover);
        }
        target->hp = (s16)heal;
        Owner_RecalculateRatiosFar(target_id);
        break;
    }

    case EFX_PP_RESTORE_7:

    {

        s32 old;
        s32 maxv;
        s32 heal;

        heal = target->pp;
        maxv = target->max_pp;
        old = heal;
        heal += Math_Div(maxv * 7, 100);
        if (heal > maxv)
            heal = maxv;
        tmp = heal - old;
        if (tmp == 0 && nibble != 11)
            break;
        if (heal == maxv)
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgPpFull);
        else {
            BattleEv_Push(BATTLE_EVENT_VALUE, tmp);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgPpRecover);
        }
        target->pp = (s16)heal;
        Owner_RecalculateRatiosFar(target_id);
        break;
    }

    case EFX_AGI_SET_UP8:
        target->agility_modifier = 8;
        target->agility_modifier_turns = 5;
        Owner_RecalculateStatsFar(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, target->agility - copy->agility);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgAgiUp);
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
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgAgiDown);
        break;

    case EFX_ATK_DOWN1:
        target->attack_modifier += -1;
        CLAMP_MOD(target->attack_modifier);
        Owner_RecalculateStatsFar(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, copy->attack - target->attack);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgAtkDown);
        target->attack_modifier_turns = 7;
        break;

    case EFX_ATK_DOWN2:
        target->attack_modifier += -2;
        CLAMP_MOD(target->attack_modifier);
        Owner_RecalculateStatsFar(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, copy->attack - target->attack);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgAtkDown);
        target->attack_modifier_turns = 7;
        break;

    case EFX_ATK_UP1:
        target->attack_modifier += 1;
        CLAMP_MOD(target->attack_modifier);
        Owner_RecalculateStatsFar(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, target->attack - copy->attack);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgAtkUp);
        target->attack_modifier_turns = 7;
        break;

    case EFX_ATK_UP2:
        target->attack_modifier += 2;
        CLAMP_MOD(target->attack_modifier);
        Owner_RecalculateStatsFar(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, target->attack - copy->attack);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgAtkUp);
        target->attack_modifier_turns = 7;
        break;

    case EFX_DEF_DOWN1:
        target->defense_modifier += -1;
        CLAMP_MOD(target->defense_modifier);
        Owner_RecalculateStatsFar(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, copy->defense - target->defense);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgDefDown);
        target->defense_modifier_turns = 7;
        break;

    case EFX_DEF_DOWN2:
        target->defense_modifier += -2;
        CLAMP_MOD(target->defense_modifier);
        Owner_RecalculateStatsFar(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, copy->defense - target->defense);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgDefDown);
        target->defense_modifier_turns = 7;
        break;

    case EFX_DEF_UP1:
        target->defense_modifier += 1;
        CLAMP_MOD(target->defense_modifier);
        Owner_RecalculateStatsFar(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, target->defense - copy->defense);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgDefUp);
        target->defense_modifier_turns = 7;
        break;

    case EFX_DEF_UP2:
        target->defense_modifier += 2;
        CLAMP_MOD(target->defense_modifier);
        Owner_RecalculateStatsFar(target_id);
        BattleEv_Push(BATTLE_EVENT_VALUE, target->defense - copy->defense);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgDefUp);
        target->defense_modifier_turns = 7;
        break;


    case EFX_REVIVE_FULL:
        if (target->hp != 0)
            break;
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgRevived);
        target->hp = target->max_hp;
        Owner_RecalculateRatiosFar(target_id);
        break;

    case EFX_REVIVE_HALF:
        if (target->hp != 0)
            break;
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgRevived);
        target->hp = (s16)(target->max_hp / 2);
        Owner_RecalculateRatiosFar(target_id);
        break;

    case EFX_REVIVE_80:
        if (target->hp != 0)
            break;
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgRevived);
        target->hp = (s16)Math_Div(target->max_hp * 8, 10);
        Owner_RecalculateRatiosFar(target_id);
        break;



    case EFX_CURE_POISON:
        if (target->poison != 0)
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCurePoison);
        target->poison = 0;
        break;

    case EFX_RES_DOWN1:
        ADJUST_RES(-1, (copy->res_modifier - target->res_modifier) * 20, (s32)&MsgResDown);
        break;

    case EFX_RES_DOWN2:
    {
        target->res_modifier += -2;
        if (-4 > target->res_modifier)
            target->res_modifier = -4;
        if (target->res_modifier > 4)
            target->res_modifier = 4;
        BattleEv_Push(BATTLE_EVENT_VALUE, (copy->res_modifier - target->res_modifier) * 20);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgResDown);
        target->res_modifier_turns = 7;
    }
        break;

    case EFX_RES_UP1:
        ADJUST_RES(1, (target->res_modifier - copy->res_modifier) * 20, (s32)&MsgResUp);
        break;

    case EFX_RES_UP2:
    {
        target->res_modifier += 2;
        if (*(rm = &target->res_modifier) < -4)
            target->res_modifier = -4;
        if (target->res_modifier > 4)
            target->res_modifier = 4;
        BattleEv_Push(BATTLE_EVENT_VALUE, (target->res_modifier - copy->res_modifier) * 20);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgResUp);
        target->res_modifier_turns = 7;
    }
        break;

    case EFX_POISON:
        if (target->poison != 0)
            break;
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgPoisoned);
        target->poison = 1;
        break;

    case EFX_VENOM:
        if (target->poison > 1)
            break;
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgVenom);
        target->poison = 2;
        break;

    case EFX_DELUSION:

        SET_STATUS7(target->delusion, (s32)&MsgDelusion);
        break;

    case EFX_CONFUSE:
        SET_STATUS7(target->confusion, (s32)&MsgConfused);
        break;

    case EFX_CHARM:
        SET_STATUS7(target->charm, (s32)&MsgCharmed);
        break;

    case EFX_STUN:

        SET_STATUS7(target->stun, (s32)&MsgStunned);
        break;

    case EFX_SLEEP:
        SET_STATUS7(target->sleep, (s32)&MsgAsleep);
        break;

    case EFX_PSY_BLOCK:
        TEXT_SIDE((s32)&MsgPsyBlock, (s32)&MsgPsySeal);
        target->psy_seal |= 7;
        break;

    case EFX_PSY_SEAL:
        TEXT_SIDE((s32)&MsgPsyBlock, (s32)&MsgPsySeal);
        target->psy_seal |= 16;
        break;

    case EFX_INSTANT_DOWN:

        BattleEv_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
        if (target->status_12a == 2)
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgKoDown);
        else if (action_id == 219)
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgSuffocate);
        else
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgSpiritDrain);
        target->hp = 0;
        Owner_RecalculateRatiosFar(target_id);
        break;

    case EFX_REFRAIN:
        SET_STATUS7(target->refrain, (s32)&MsgRefrain);
        break;

    case EFX_REFLECT:
        SET_STATUS7(target->reflect, (s32)&MsgReflect);
        break;

    case EFX_DRAIN_HP:
    case EFX_DRAIN_HP_HALF:
    {

        s32 heal;

        heal = actor->hp;
        dmg = dealt;
        if (action->effect == EFX_DRAIN_HP_HALF)
            dmg /= 2;
        heal += dmg;
        if (heal > actor->max_hp) {
            heal = actor->max_hp;
            dmg = heal - actor->hp;
        }
        BattleEv_Push(BATTLE_EVENT_RESET, 0);
        BattleEv_Push(BATTLE_EVENT_UNIT, actor_id);
        if (heal == actor->max_hp)
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgHpFull);
        else {
            BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgHpRecover);
        }
        actor->hp = (s16)heal;
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
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgPpFull);
        else {
            BattleEv_Push(BATTLE_EVENT_VALUE, amt);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgPpRecover);
        }
        actor->pp = (s16)heal;
        Owner_RecalculateRatiosFar(actor_id);
        break;
    }

    case EFX_PP_LEECH:
        dmg = Math_Div(dealt, 10);
        if (target->pp < dmg)
            dmg = target->pp;
        if (actor->pp + dmg > actor->max_pp)
            dmg = actor->max_pp - actor->pp;
        if (dmg == 0)
            break;
        BattleEv_Push(BATTLE_EVENT_VALUE, dmg);
        TEXT_SIDE((s32)&MsgLeechTake, (s32)&MsgLeechGain);
        Owner_AdjustSecondValueFar(actor_id, dmg);
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
        target->status_12c = 0;
        target->status_12d = 0;
        target->status_12e = 0;
        target->status_12f = 0;
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgBuffsReset);
        break;

    case EFX_EVIL_SPIRIT:
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgEvilSpirit);
        target->evil_spirit = 1;
        break;

    case EFX_DEATH_CURSE:

        if (target->death_count == 0) {
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgDeathCurse);
            target->death_count = 7;
            break;
        }
        if (target->death_count <= 1)
            break;
        target->death_count -= 1;
        BattleEv_Push(BATTLE_EVENT_VALUE, target->death_count);
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgDeathCount);
        break;

    case EFX_READY:
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgReadies);
        target->ready_pose = 2;
        break;

    case EFX_CHALLENGE:
        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgChallenge);
        target->battle_end_state = 1;
        if ((u32)target_id <= 7)
            work->flags_043 |= 2;
        break;

    case EFX_IMMOBILIZE:

        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgImmobile);
        target->cannot_move = 1;
        break;

    case EFX_GUARD1:

        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgAura);
        if (target->guard_level > 0)
            break;
        g1 = 1;
        target->guard_level = g1;
        break;



    case EFX_GUARD2:

        BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgAura2);
        if (target->guard_level > 1)
            break;
        target->guard_level = 2;
        break;



    case EFX_TEXT_NONE:
        BattleEv_Push(BATTLE_EVENT_TEXT, (u32)-1);
        break;



    default:
        break;
    }

done:
    /* 終了処理 */
    BattleEv_Push(BATTLE_EVENT_RESET, 0);

    if (target->hp != 0) {
        if (target->sleep != 0)
        if (target->sleep <= 6
            && dealt > 0 && (3 & BattleRandom16Far()) == 0) {
            target->sleep = 0;
            BattleEv_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgWakes);
        }
    }
    Sys_Free(copy);
    Owner_RecalculateStatsFar(target_id);
    UiWindow_DrawPartyStatusContentsFar(((struct BattleSession *)gBattleWork)->party_status_mode);
    if (target->hp != 0)
        BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
    if (actor->evil_spirit != 0
        && (BattleRandom16Far() & 3) == 0 && dealt > 0) {
        s32 share;

        share = dealt >> 2;
        if (share == 0)
            share = 1;
        plan->pending_amount_60 += share;
    }
}
