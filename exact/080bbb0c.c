#include "types.h"
#include "battle_actor.h"
#include "battle_calc.h"
#include "battle_command.h"
#include "battle_efx.h"
#include "battle_event.h"
#include "battle_msg.h"
#include "battle_runtime.h"
#include "battle_summon.h"
#include "battle_types.h"
#include "battle_work.h"
#include "runtime_1e74.h"
#include "runtime_mem.h"

/* 行動1件の対象解決。コピーを取り、命中とダメージ種別を決めて効果を出す。 */

#define BytePtr(p) ((u8 *)(p))

/* 属性テーブルはユニット+36 の s16 対。威力テーブルは +72。 */
struct AffinityPair {
    s16 low;
    s16 high;
};

#define ELEM_AT(unit, range) (*(s16 *)((u8 *)(unit) + 38 + (range) * 2 * 2))
#define S8OF(v) (*(s8 *)&(v))

#define CLAMP_MOD(v)                                                           \
    {                                                                          \
        if ((v) < -4)                                                          \
            (v) = -4;                                                          \
        if ((v) > 4)                                                           \
            (v) = 4;                                                           \
    }

#define TAKE_BONUS()                                                           \
    {                                                                          \
        if (range != 4) {                                                      \
            s32 off;                                                           \
                                                                               \
            off = range * 4 + 72;                                              \
            bonus = power - ((s16 *)((u8 *)target + off))[1];                  \
        }                                                                      \
    }

#define APPLY_GUARD()                                                          \
    {                                                                          \
        guard = S8OF(target->guard_level);                                     \
        if (guard != 0) {                                                      \
            if (guard == 1)                                                    \
                dmg /= 2;                                                      \
            else                                                               \
                dmg = Math_Div(dmg, 10);                                  \
        }                                                                      \
    }

/* 文面を変数に決めてから1回だけ積む形。呼び先を挟んで統合できない共有尾は
 * ソース側の変数だった(crossjump は bl を跨いで一致を探せない)。 */
#define TEXT_SIDE_V(player, enemy)                                             \
{                                                                              \
    s32 text;                                                                  \
                                                                               \
    if ((u32)target_id <= 7)                                                   \
        text = (player);                                                       \
    else                                                                       \
        text = (enemy);                                                        \
    BattleEvent_Push(BATTLE_EVENT_TEXT, text);                                 \
}

#define TEXT_SIDE(player, enemy)                                               \
{                                                                              \
    if ((u32)target_id <= 7)                                                   \
        BattleEvent_Push(BATTLE_EVENT_TEXT, (player));                         \
    else                                                                       \
        BattleEvent_Push(BATTLE_EVENT_TEXT, (enemy));                          \
}

#define TAKE_PWR()                                                             \
{                                                                              \
    pwr = action->power;                                                       \
    if (pwr == 0)                                                              \
        break;                                                                 \
}

#define APPLY_HP_HIT(ko_lo, ko_hi)                                             \
{                                                                              \
    if (target->hp <= dmg) {                                                   \
        dealt = target->hp;                                                    \
        target->hp = 0;                                                        \
        BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);                  \
        BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);                           \
        TEXT_SIDE(ko_lo, ko_hi);                                               \
    } else {                                                                   \
        dealt = dmg;                                                           \
        target->hp = (s16)(target->hp - dmg);                                  \
        BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);                   \
    }                                                                          \
}

#define APPLY_PP_HIT()                                                         \
{                                                                              \
    if (target->pp <= dmg) {                                                   \
        dealt = target->pp;                                                    \
        target->pp = 0;                                                        \
    } else {                                                                   \
        dealt = dmg;                                                           \
        target->pp = (s16)(target->pp - dmg);                                  \
    }                                                                          \
}

#define HEAL_CUR(cur, maxv, full_text, part_text)                              \
{                                                                              \
    if ((cur) + dmg > (maxv)) {                                                \
        dmg = (maxv) - (cur);                                                  \
        (cur) = (maxv);                                                        \
    } else {                                                                   \
        (cur) = (s16)((cur) + dmg);                                            \
    }                                                                          \
    BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);                               \
    if ((cur) == (maxv))                                                       \
        BattleEvent_Push(BATTLE_EVENT_TEXT, (full_text));                         \
    else {                                                                     \
        BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);                                \
        BattleEvent_Push(BATTLE_EVENT_TEXT, (part_text));                         \
    }                                                                          \
}

/* ターン数フィールドはオフセット定数で渡す。冒頭の代入が call-saved に住み、
 * プッシュ2回をまたいで生き延びる(306 は即値に畳めず const-prop が残す)。 */
#define ATK_TURNS 0x132
#define DEF_TURNS 0x134
#define RES_TURNS 0x136

#define ADJUST_ATKDEF(field, delta, toff_c, value_expr, text)                  \
{                                                                              \
    s32 toff;                                                                  \
                                                                               \
    toff = (toff_c);                                                           \
    (field) += (delta);                                                        \
    CLAMP_MOD(field);                                                          \
    BattleUnit_Recalculate(target_id);                                         \
    BattleEvent_Push(BATTLE_EVENT_VALUE, (value_expr));                           \
    BattleEvent_Push(BATTLE_EVENT_TEXT, (text));                                  \
    *BytePtr((u8 *)target + toff) = 7;                                          \
}

#define ADJUST_RES(delta, value_expr, text)                                 \
{                                                                              \
    target->res_modifier += (delta);                                             \
    CLAMP_MOD(target->res_modifier);                                             \
    BattleEvent_Push(BATTLE_EVENT_VALUE, (value_expr));                        \
    BattleEvent_Push(BATTLE_EVENT_TEXT, (text));                               \
    target->res_modifier_turns = 7;                                                    \
}

#define SET_STATUS7(field, text)                                               \
{                                                                              \
    BattleEvent_Push(BATTLE_EVENT_TEXT, (text));                                  \
    (field) = 7;                                                               \
}

s32 Func_080bbb0c(struct BattlePlan *plan, s32 slot)
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
    void *work;
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
    s32 pass;
    s32 scale;
    s8 *am;
    s32 guard;
    s16 saved[8];
    s32 count;
    s32 tmp;
    s32 size;

    bonus = 0;
    work = BattleWorkPtr;
    half = 0;
    dealt = 0;
    crush = 0;
    skip = 0;
    affinity = 0;
    size = sizeof(struct BattleUnit);
    copy = (struct BattleUnit *)Sys_Alloc(size);

    actor_id = plan->actor_id;
    target_id = plan->target_ids[slot];
    action_id = plan->action_id;
    range = plan->range_index;
    adjust = plan->target_adjustments[slot];
    modifier = plan->target_modifiers[slot];

    action = BattleAction_Get(action_id);
    actor = BattleUnit_Get(actor_id);
    target = BattleUnit_Get(target_id);
    Mem_Copy(copy, target, size, UnitCopyDesc);

    if (action->range != 255) {
        offset = plan->target_offsets[slot];
        if (offset < 0)
            offset = -offset;
    } else {
        offset = 0;
    }

    /* 属性相性。テーブルを二方向に走査して符号を決める。 */
    if (range != 4) {
        s16 *tbl;
        s32 i;

        value = ELEM_AT(target, range);
        tbl = (s16 *)((u8 *)target + 36);
        i = 0;
        if (value >= tbl[1]) {
            s16 *p;

            p = tbl;
            do {
                i++;
                p += 2;
                if (i > 3)
                    break;
            } while (value >= p[1]);
        }
        if (i == 4)
            affinity = -1;

        i = 0;
        {
            s32 off;

            off = 2;
            if (value <= *(s16 *)(BytePtr(tbl) + off)) {
                do {
                    i++;
                    if (i > 3)
                        break;
                } while (value <= ((struct AffinityPair *)((u8 *)target + 36))[i].high);
            }
        }
        if (i == 4)
            affinity = 1;
    }

    /* 攻撃力。元素武器でなければ 100。 */
    if ((u32)plan->range_index <= 3) {
        cmd = &plan->command;
        if (*cmd != 2) {
            s32 off;

            off = plan->range_index * 4 + 72;
            power = *(s16 *)((u8 *)actor + off);
            goto after_power;
        }
    } else
        cmd = &plan->command;
    power = 100;
after_power:

    if (plan->command == 5 && (u32)plan->range_index <= 3 && affinity > 0) {
        s32 chance;

        {
            s32 off;

            off = plan->range_index * 4 + 72;
            chance = power - ((s16 *)((u8 *)target + off))[1];
        }
        chance += 30;
        chance *= 0x28f;
        if (chance > (BattleRandom_Next() & 0xffff))
            BattleEvent_Push(BATTLE_EVENT_SCRIPT_UPDATE, 5);
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
            st = Summon_ClassId(*(s32 *)work);
        if (hit != 0 && Summon_ClassValid(st) != 0 && rec >= 0) {
            s32 ch;
            s16 *slots;
            s32 cursor;

            ch = Summon_TakeCharge(st, 1);
            if (ch & 0x8000)
                Summon_ResetCharge(st);
            BattleUnit_Assign(rec, st, ch & 0x7fff);
            slots = (s16 *)(BytePtr(work) + 2);
            {
                s32 off;
                s32 i;
                s32 j;
                s32 jsave;

                off = 100;
                i = 0;
                jsave = 0;
                if (*(s16 *)(BytePtr(slots) + off) == 254) {
                    *(s16 *)(BytePtr(slots) + off) = rec;
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
                s32 x;
                s32 y;

                /* The slot cursor is dead here; its word now carries the object. */
                cursor = (s32)Actor_GetObject(rec);
                x = *(s32 *)(cursor + 12);
                if (x < 0)
                    x += 0xffff;
                y = *(s32 *)(cursor + 16);
                x >>= 16;
                if (y < 0)
                    y += 0xffff;
                y >>= 16;
                Actor_Place((void *)cursor, rec, x, y);
            }
            Actor_Commit();
            {
                s32 listed;

                listed = Actor_ListSlots(saved);
                if (listed > 0) {
                    u16 *q;

                    q = (u16 *)saved;
                    count = listed;
                    do {
                        Actor_RefreshSlot(*q++);
                        count--;
                    } while (count != 0);
                }
            }
            BattleEvent_Push(BATTLE_EVENT_UNIT, rec);
            if (action_id != 0x1f7)
                BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_APPEARS);
            else
                BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_SPLIT_OFF);
        } else if (action_id == 0x1f7) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_SPLIT_FAIL);
        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_NO_ONE_CAME);
        }
    }

    if (hit != 0) {
        s32 efx;

        efx = action->effect;
        if (action->effect == EFX_IMMOBILIZE) {
            s32 hidx;

            hit = 0;
            hidx = 748;
            if (*(s16 *)(BytePtr(work) + hidx) == target_id) {
                hit = 1;
            } else {
                n = 0;
            scan_next:
                n++;
                if ((u32)n <= 19) {
                    s32 idx;

                    idx = ((n << 1) << 3) + 748;
                    if (*(s16 *)(BytePtr(work) + idx) == target_id)
                        hit = 1;
                    else
                        goto scan_next;
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
                BattleEvent_Push(BATTLE_EVENT_ACTOR_EFFECT, actor_id);
        } else if (action->effect == EFX_DRAIN_PP) {
            if (target->pp != 0)
                nibble = 10;
            else
                hit = 0;
        }
    }

    /* ダメージ種別。HP が残っているか、分類が非ゼロなら種別スイッチへ。 */
    if (skip == 0
        && (target->hp != 0 || BattleEffect_Classify(action->effect) != 0)) {
        s32 pp;
        s32 cur;

        /*
         * -1 PPダメージ(別系)  1 HP回復  2 HPダメージ  3/4 武器攻撃
         *  5/6/8 加算攻撃(アイテム・distance テーブル別)  10 PP吸収系  11 PP回復
         *  0/7/9 は空。
         */
        switch (nibble) {
        case DK_ATTACK:
        case DK_ATTACK_X:
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
                        Battle_CalcAttack(actor->attack, scale, 0, bonus)
                            * apwr,
                        10);
                else
                    dmg = Battle_CalcAttack(actor->attack, scale, apwr,
                                            bonus);
                dmg *= adjust;
                if (modifier != 0) {
                    if (modifier == 1)
                        dmg = dmg * 5 / 4;
                    else
                        dmg = dmg * 3 / 2;
                    dmg += (u8)Math_Mod(((u8 *)target)[15], 5) + 6;
                    if (pass == 0) {
                        BattleEvent_Push(BATTLE_EVENT_MARK, 0);
                        {
                            s32 text;

                            text = MSG_CRITICAL;
                            if ((u32)target_id <= 7)
                                text += 1;
                            BattleEvent_Push(BATTLE_EVENT_TEXT_CONTINUE, text);
                        }
                    }
                }
                dmg += BattleRandom_Next() & 3;
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
                if (BattleFlag_Test(366) != 0 && *cmd == 5 && cur <= dmg) {
                    dmg = cur - 1;
                }
                pass++;
            } while (pass <= 1);
            BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            cur -= dmg;
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            {
                s32 text;

                if ((u32)target_id <= 7)
                    text = MSG_DMG_EMPH_P + affinity;
                else
                    text = MSG_DMG_EMPH_E + affinity;
                BattleEvent_Push(BATTLE_EVENT_TEXT, text);
            }
            if (cur <= 0) {
                BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
                cur = 0;
                TEXT_SIDE(MSG_GOES_DOWN, MSG_FELLED);
            } else
                BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->hp - cur;
            target->hp = (s16)cur;
            BattleUnit_UpdateRatios(target_id);
            break;
        }

        case DK_PP_DRAIN:
        {
            if (action->power == 0)
                break;
            pp = target->pp;
            if (range != 4) {
                s32 off;

                off = range * 4;
                off = off + 72;
                bonus = power - ((s16 *)((u8 *)target + off))[1];
            }
            dmg = action->power;
            dmg = Battle_CalcPower(dmg, bonus, 256);
            dmg = Math_Div(dmg * PpLossFalloff[offset], 100);
            dmg *= adjust;
            APPLY_GUARD();
            if (action->effect == EFX_DRAIN_PP && dmg > pp)
                dmg = pp;
            BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;

                if ((u32)target_id <= 7)
                    text = MSG_PP_LOSS_P;
                else
                    text = MSG_PP_LOSS_E;
                BattleEvent_Push(BATTLE_EVENT_TEXT, text);
                pp -= dmg;
            }
            if (pp <= 0)
                pp = 0;
            BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->pp - pp;
            target->pp = pp;
            BattleUnit_UpdateRatios(target_id);
            break;
        }

        case DK_HP_HEAL:
        {
            if (action->power == 0)
                break;
            cur = target->hp;
            dmg = action->power;
            dmg = Battle_CalcRestore(dmg, range == 4 ? 100 : power, 256);
            dmg = Math_Div(dmg * HpHealFalloff[offset], 100);
            dmg *= adjust;
            dmg += BattleRandom_Next() & 3;
            cur += dmg;
            if (cur > target->max_hp) {
                cur = target->max_hp;
                dmg = cur - target->hp;
            }
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            if (cur == target->max_hp)
                BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_HP_FULL);
            else {
                BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
                BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_HP_RECOVER);
            }
            dealt = target->hp - cur;
            target->hp = (s16)cur;
            BattleUnit_UpdateRatios(target_id);
            break;
        }

        case DK_PP_DMG:
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
            BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;

                if ((u32)target_id <= 7)
                    text = MSG_DMG_P;
                else
                    text = MSG_DMG_E;
                BattleEvent_Push(BATTLE_EVENT_TEXT, text);
                pp -= dmg;
            }
            if (pp <= 0)
                pp = 0;
            BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            goto pp_store;
        }

        case DK_HP_DMG_6:
        case DK_HP_DMG_7:
        case DK_HP_DMG_9:
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
                dmg += BattleRandom_Next() & 3;
                APPLY_GUARD();
                if (BattleFlag_Test(366) != 0 && *cmd == 6 && cur > dmg) {
                    dmg = cur;
                }
                pass++;
            } while (pass <= 1);
            BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;

                if ((u32)target_id <= 7)
                    text = MSG_DMG_EMPH_P + affinity;
                else
                    text = MSG_DMG_EMPH_E + affinity;
                BattleEvent_Push(BATTLE_EVENT_TEXT, text);
                cur -= dmg;
            }
            if (cur <= 0) {
                BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
                {
                    s32 text;

                    cur = 0;
                    if ((u32)target_id <= 7)
                        text = MSG_GOES_DOWN;
                    else
                        text = MSG_FELLED;
                    BattleEvent_Push(BATTLE_EVENT_TEXT, text);
                }
            } else
                BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->hp - cur;
            target->hp = (s16)cur;
            BattleUnit_UpdateRatios(target_id);
            break;
        }

        case DK_PP_HEAL:
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
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            if (pp == target->max_pp)
                BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_PP_FULL);
            else {
                BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
                BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_PP_RECOVER);
            }
pp_store:
            target->pp = (s16)pp;
            BattleUnit_UpdateRatios(target_id);
            break;
        }

        case 0:
        case 7:
        case 9:
            /* 空スロット */
            break;

        case DK_HP_DMG:
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
            BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;

                if ((u32)target_id <= 7)
                    text = MSG_DMG_P;
                else
                    text = MSG_DMG_E;
                BattleEvent_Push(BATTLE_EVENT_TEXT, text);
                cur -= dmg;
            }
            if (cur <= 0) {
                BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
                cur = 0;
                TEXT_SIDE(MSG_GOES_DOWN, MSG_FELLED);
            } else
                BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
dealt = target->hp - cur;
            target->hp = (s16)cur;
            BattleUnit_UpdateRatios(target_id);
            break;
            }
            BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_NO_EFFECT);
            break;

        }
    }

    /* 付加効果 */
    BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
    if (BattleEffect_Classify(action->effect) == 0 && target->hp == 0
        && BattleEffect_OnDead(action->effect) == 0)
        goto done;
    if (hit == 0)
        goto done;
    if ((u32)(action->effect - 3) > 66)
        goto done;

    switch (action->effect) {
    case EFX_CURE_ALL:
        if (target->delusion != 0) {
            target->delusion = 0;
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_DELUSION);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->stun != 0) {
            target->stun = 0;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_STUN);
        }
        target->sleep = 0;
        if (target->psy_seal != 0) {
            target->psy_seal = 0;
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_SEAL);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->death_count != 0) {
            target->death_count = 0;
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_CURSE);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->evil_spirit != 0) {
            target->evil_spirit = 0;
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_SPIRIT);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->poison != 0) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_POISON);
            target->poison = 0;
        }
        BattleEvent_Push(BATTLE_EVENT_RESET, 0);
        break;

    case EFX_CURE_PART:
        if (target->delusion != 0) {
            target->delusion = 0;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_DELUSION);
        }
        if (target->stun != 0) {
            target->stun = 0;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_STUN);
        }
        target->sleep = 0;
        if (target->psy_seal != 0) {
            target->psy_seal = 0;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_SEAL);
        }
        if (target->death_count == 0)
            break;
        target->death_count = 0;
        BattleEvent_Push(BATTLE_EVENT_RESET, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_CURSE);
        break;

    case EFX_HEAL_60:
    case EFX_HEAL_30:
    {
        s32 old;
        s32 maxu;
        s32 maxv;
        s32 heal;
        u16 *stat_ptr;

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
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_HP_FULL);
        else {
            BattleEvent_Push(BATTLE_EVENT_VALUE, tmp);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_HP_RECOVER);
        }
        target->hp = (s16)heal;
        BattleUnit_UpdateRatios(target_id);
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
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_PP_FULL);
        else {
            BattleEvent_Push(BATTLE_EVENT_VALUE, tmp);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_PP_RECOVER);
        }
        target->pp = (s16)heal;
        BattleUnit_UpdateRatios(target_id);
        break;
    }

    case EFX_AGI_SET_UP8:
        S8OF(target->agility_modifier) = 8;
        target->agility_modifier_turns = 5;
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, target->agility - copy->agility);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_AGI_UP);
        break;

    case EFX_AGI_SET_DOWN4:
        am = &S8OF(target->agility_modifier);
    {
        u8 v;

        v = -4;
        *am = v;
    }
        target->agility_modifier_turns = 5;
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, copy->agility - target->agility);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_AGI_DOWN);
        break;

    case EFX_ATK_DOWN1:
        target->attack_modifier += -1;
        CLAMP_MOD(target->attack_modifier);
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, copy->attack - target->attack);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_ATK_DOWN);
        target->attack_modifier_turns = 7;
        break;

    case EFX_ATK_DOWN2:
        target->attack_modifier += -2;
        CLAMP_MOD(target->attack_modifier);
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, copy->attack - target->attack);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_ATK_DOWN);
        target->attack_modifier_turns = 7;
        break;

    case EFX_ATK_UP1:
        target->attack_modifier += 1;
        CLAMP_MOD(target->attack_modifier);
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, target->attack - copy->attack);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_ATK_UP);
        target->attack_modifier_turns = 7;
        break;

    case EFX_ATK_UP2:
        target->attack_modifier += 2;
        CLAMP_MOD(target->attack_modifier);
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, target->attack - copy->attack);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_ATK_UP);
        target->attack_modifier_turns = 7;
        break;

    case EFX_DEF_DOWN1:
        target->defense_modifier += -1;
        CLAMP_MOD(target->defense_modifier);
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, copy->defense - target->defense);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_DEF_DOWN);
        target->defense_modifier_turns = 7;
        break;

    case EFX_DEF_DOWN2:
        target->defense_modifier += -2;
        CLAMP_MOD(target->defense_modifier);
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, copy->defense - target->defense);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_DEF_DOWN);
        target->defense_modifier_turns = 7;
        break;

    case EFX_DEF_UP1:
        target->defense_modifier += 1;
        CLAMP_MOD(target->defense_modifier);
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, target->defense - copy->defense);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_DEF_UP);
        target->defense_modifier_turns = 7;
        break;

    case EFX_DEF_UP2:
        target->defense_modifier += 2;
        CLAMP_MOD(target->defense_modifier);
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, target->defense - copy->defense);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_DEF_UP);
        target->defense_modifier_turns = 7;
        break;


    case EFX_REVIVE_FULL:
        if (target->hp != 0)
            break;
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_REVIVED);
        target->hp = target->max_hp;
        BattleUnit_UpdateRatios(target_id);
        break;

    case EFX_REVIVE_HALF:
        if (target->hp != 0)
            break;
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_REVIVED);
        target->hp = (s16)(target->max_hp / 2);
        BattleUnit_UpdateRatios(target_id);
        break;

    case EFX_REVIVE_80:
        if (target->hp != 0)
            break;
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_REVIVED);
        target->hp = (s16)Math_Div(target->max_hp * 8, 10);
        BattleUnit_UpdateRatios(target_id);
        break;

    case EFX_CURE_POISON:
        if (target->poison != 0)
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_POISON);
        target->poison = 0;
        break;

    case EFX_RES_DOWN1:
        ADJUST_RES(-1, (copy->res_modifier - target->res_modifier) * 20, MSG_RES_DOWN);
        break;

    case EFX_RES_DOWN2:
    {
        target->res_modifier += -2;
        if (-4 > target->res_modifier)
            target->res_modifier = -4;
        if (target->res_modifier > 4)
            target->res_modifier = 4;
        BattleEvent_Push(BATTLE_EVENT_VALUE, (copy->res_modifier - target->res_modifier) * 20);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_RES_DOWN);
        target->res_modifier_turns = 7;
    }
        break;

    case EFX_RES_UP1:
        ADJUST_RES(1, (target->res_modifier - copy->res_modifier) * 20, MSG_RES_UP);
        break;

    case EFX_RES_UP2:
    {
        target->res_modifier += 2;
        if (*(rm = &S8OF(target->res_modifier)) < -4)
            target->res_modifier = -4;
        if (target->res_modifier > 4)
            target->res_modifier = 4;
        BattleEvent_Push(BATTLE_EVENT_VALUE, (target->res_modifier - copy->res_modifier) * 20);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_RES_UP);
        target->res_modifier_turns = 7;
    }
        break;

    case EFX_POISON:
        if (target->poison != 0)
            break;
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_POISONED);
        target->poison = 1;
        break;

    case EFX_VENOM:
        if (target->poison > 1)
            break;
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_VENOM);
        target->poison = 2;
        break;

    case EFX_DELUSION:
        SET_STATUS7(target->delusion, MSG_DELUSION);
        break;

    case EFX_CONFUSE:
        SET_STATUS7(target->confusion, MSG_CONFUSED);
        break;

    case EFX_CHARM:
        SET_STATUS7(target->charm, MSG_CHARMED);
        break;

    case EFX_STUN:
        SET_STATUS7(target->stun, MSG_STUNNED);
        break;

    case EFX_SLEEP:
        SET_STATUS7(target->sleep, MSG_ASLEEP);
        break;

    case EFX_PSY_BLOCK:
        TEXT_SIDE(MSG_PSY_BLOCK, MSG_PSY_SEAL);
        target->psy_seal |= 7;
        break;

    case EFX_PSY_SEAL:
        TEXT_SIDE(MSG_PSY_BLOCK, MSG_PSY_SEAL);
        target->psy_seal |= 16;
        break;

    case EFX_INSTANT_DOWN:
        BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
        if (target->status_12a == 2)
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_KO_DOWN);
        else if (action_id == 219)
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_SUFFOCATE);
        else
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_SPIRIT_DRAIN);
        target->hp = 0;
        BattleUnit_UpdateRatios(target_id);
        break;

    case EFX_REFRAIN:
        SET_STATUS7(target->refrain, MSG_REFRAIN);
        break;

    case EFX_REFLECT:
        SET_STATUS7(target->reflect, MSG_REFLECT);
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
        BattleEvent_Push(BATTLE_EVENT_RESET, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT, actor_id);
        if (heal == actor->max_hp)
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_HP_FULL);
        else {
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_HP_RECOVER);
        }
        actor->hp = (s16)heal;
        BattleUnit_UpdateRatios(actor_id);
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
        BattleEvent_Push(BATTLE_EVENT_RESET, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT, actor_id);
        if (heal == actor->max_pp)
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_PP_FULL);
        else {
            BattleEvent_Push(BATTLE_EVENT_VALUE, amt);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_PP_RECOVER);
        }
        actor->pp = (s16)heal;
        BattleUnit_UpdateRatios(actor_id);
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
        BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
        TEXT_SIDE(MSG_LEECH_TAKE, MSG_LEECH_GAIN);
        BattleUnit_Drain(actor_id, dmg);
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
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_BUFFS_RESET);
        break;

    case EFX_EVIL_SPIRIT:
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_EVIL_SPIRIT);
        target->evil_spirit = 1;
        break;

    case EFX_DEATH_CURSE:
        if (target->death_count == 0) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_DEATH_CURSE);
            target->death_count = 7;
            break;
        }
        if (target->death_count <= 1)
            break;
        target->death_count -= 1;
        BattleEvent_Push(BATTLE_EVENT_VALUE, target->death_count);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_DEATH_COUNT);
        break;

    case EFX_READY:
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_READIES);
        ((u8 *)target)[0x144] = 2;  /* ready_pose。構造体表記だと共有尾が壊れる */
        break;

    case EFX_CHALLENGE:
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CHALLENGE);
        target->battle_end_state = 1;
        if ((u32)target_id <= 7)
            ((u8 *)work)[67] |= 2;
        break;

    case EFX_IMMOBILIZE:
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_IMMOBILE);
        target->cannot_move = 1;
        break;

    case EFX_GUARD1:
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_AURA);
        if (S8OF(target->guard_level) > 0)
            break;
        g1 = 1;
        S8OF(target->guard_level) = g1;
        break;

    case EFX_GUARD2:
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_AURA_2);
        if (S8OF(target->guard_level) > 1)
            break;
        S8OF(target->guard_level) = 2;
        break;

    case EFX_TEXT_NONE:
        BattleEvent_Push(BATTLE_EVENT_TEXT, (u32)-1);
        break;

    default:
        break;
    }

done:
    /* 終了処理 */
    BattleEvent_Push(BATTLE_EVENT_RESET, 0);
    if (target->hp != 0) {
        if (target->sleep != 0)
        if (target->sleep <= 6
            && dealt > 0 && (3 & BattleRandom_Next()) == 0) {
            target->sleep = 0;
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_WAKES);
        }
    }
    Sys_Free(copy);
    BattleUnit_Recalculate(target_id);
    Sys_SetMode(((u8 *)BattleWorkPtr)[65]);
    if (target->hp != 0)
        BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
    if (actor->evil_spirit != 0 && (BattleRandom_Next() & 3) == 0 && dealt > 0) {
        s32 share;

        share = dealt >> 2;
        if (share == 0)
            share = 1;
        plan->pending_amount_60 += share;
    }
}
