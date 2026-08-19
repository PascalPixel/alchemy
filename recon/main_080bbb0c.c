#include "types.h"
#include "battle_command.h"
#include "battle_event.h"
#include "battle_runtime.h"
#include "battle_types.h"
#include "runtime_1e74.h"

/* 行動1件の対象解決。コピーを取り、命中とダメージ種別を決めて効果を出す。 */

extern void *Data_03001e74;
extern u8 Data_03001388[];
extern u8 Data_080c2ab8[];
extern s32 Data_080c2ac0[];
extern s32 Data_080c2ad8[];
extern s32 Data_080c2af0[];
extern s32 Data_080c2b08[];
extern s32 Data_080c2b20[];
extern s32 Data_080c2b38[];
extern s32 Data_080c2b50[];
extern s32 Data_080c2b68[];

u32 Func_08004938(s32 size);
void Func_080072f0(void *dst, void *src, s32 size, void *fn);
s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_080022f4(s32 numerator, s32 denominator);
void Func_08002df0(void *buffer);
void Func_08015130(s32 mode);
s32 Func_080b7514(void);
s32 Func_080c1fa8(s32);
s32 Func_080b6cdc(s32);
s32 Func_080c1df4(s32, s32);
void Func_080c1f50(s32);
void Func_08077140(s32 slot, s32 unit, s32 mask);
void Func_080b7548(void);
void *Func_080b7dd0(s32);
void Func_080b6f44(void *obj, s32 unit, s32 x, s32 y);
void Func_080b6c90(void);
s32 Func_080b6ae0(s16 *entries);
void Func_080b8000(s32);
s32 Func_08077178(s32 actor, s32 target, s32 range, s32 effect, s32 table);
s32 Func_08077180(s32 power, s32 scale, s32 unused, s32 bonus);
s32 Func_08077188(s32 power, s32 bonus, s32 scale);
s32 Func_08077190(s32 power, s32 scale, s32 factor);
void Func_08077120(s32 unit, s32 amount);
s32 Func_080bbae8(s32 effect);

/* 効果番号。挙動から付けた慎重な名前で、原作の識別子の主張ではない。 */
enum {
    EFX_AIL131_CLEAR = 3,
    EFX_CURE_PART = 4,
    EFX_REVIVE_FULL = 5,
    EFX_ATK_UP2 = 6,
    EFX_ATK_UP1 = 7,
    EFX_ATK_DOWN2 = 8,
    EFX_ATK_DOWN1 = 9,
    EFX_DEF_UP2 = 10,
    EFX_DEF_UP1 = 11,
    EFX_DEF_DOWN2 = 12,
    EFX_DEF_DOWN1 = 13,
    EFX_MOD137_UP2 = 14,
    EFX_MOD137_UP1 = 15,
    EFX_MOD137_DOWN2 = 16,
    EFX_MOD137_DOWN1 = 17,
    EFX_AIL131_1 = 18,
    EFX_AIL131_2 = 19,
    EFX_AIL138 = 20,
    EFX_AIL139 = 21,
    EFX_AIL13A = 22,
    EFX_AIL13B = 23,
    EFX_AIL13C = 24,
    EFX_AIL13D_LOW = 25,
    EFX_AIL140 = 26,
    EFX_RETIRE = 27,
    EFX_AIL141 = 28,
    EFX_AIL13E = 29,
    EFX_AIL13F = 30,
    EFX_DRAIN_HP = 31,
    EFX_DRAIN_PP = 32,
    EFX_BUFF_CLEAR = 33,
    EFX_LETHAL = 34,
    EFX_HALF_DEF = 35,
    EFX_TEXT_NONE = 45,
    EFX_GUARD1 = 46,
    EFX_GUARD2 = 47,
    EFX_STANDBY = 50,
    EFX_STANDBY_WORK = 51,
    EFX_FORCE_ACTION = 53,
    EFX_BATTLE_END = 54,
    EFX_ACTOR_FLASH = 55,
    EFX_REVIVE_HALF = 56,
    EFX_REVIVE_80 = 57,
    EFX_AGI_SET_DOWN4 = 58,
    EFX_AGI_SET_UP8 = 59,
    EFX_DRAIN_HP_HALF = 60,
    EFX_HEAL_60 = 61,
    EFX_HEAL_30 = 62,
    EFX_PP_RESTORE_7 = 63,
    EFX_CURE_ALL = 64,
    EFX_SET_144 = 66,
    EFX_AIL13D_HIGH = 67,
    EFX_PP_LEECH = 69,
};

/* 再構成用の別名。ABI 境界の宣言は Func_ のまま。 */
#define Sys_Alloc            Func_08004938
#define Sys_Free             Func_08002df0
#define Mem_Copy             Func_080072f0
#define Sys_SetMode          Func_08015130
#define Math_Div             Func_080022ec
#define Math_Mod             Func_080022f4
#define Battle_HitCheck      Func_08077178
#define Battle_CalcAttack    Func_08077180
#define Battle_CalcPower     Func_08077188
#define Battle_CalcRestore   Func_08077190
#define BattleUnit_Drain     Func_08077120
#define BattleUnit_Assign    Func_08077140
#define BattleEffect_OnDead  Func_080bbae8
#define Summon_FindSlot      Func_080b7514
#define Summon_ClassId       Func_080c1fa8
#define Summon_ClassValid    Func_080b6cdc
#define Summon_TakeCharge    Func_080c1df4
#define Summon_ResetCharge   Func_080c1f50
#define Summon_Refresh       Func_080b7548
#define Actor_GetObject      Func_080b7dd0
#define Actor_Place          Func_080b6f44
#define Actor_Commit         Func_080b6c90
#define Actor_ListSlots      Func_080b6ae0
#define Actor_RefreshSlot    Func_080b8000

/* 属性テーブルはユニット+36 の s16 対。威力テーブルは +72。 */
#define ELEM_AT(unit, range) (*(s16 *)((u8 *)(unit) + 38 + (range) * 4))
#define S8OF(v) (*(s8 *)&(v))

#define CLAMP_MOD(v)                                                           \
    do {                                                                       \
        if ((v) < -4)                                                          \
            (v) = -4;                                                          \
        if ((v) > 4)                                                           \
            (v) = 4;                                                           \
    } while (0)

#define TAKE_BONUS()                                                           \
    do {                                                                       \
        if (range != 4) {                                                      \
            s32 off;                                                           \
                                                                               \
            off = range * 4 + 72;                                              \
            bonus = power - ((s16 *)((u8 *)target + off))[1];                  \
        }                                                                      \
    } while (0)

#define APPLY_GUARD()                                                          \
    do {                                                                       \
        guard = S8OF(target->guard_level);                                     \
        if (guard != 0) {                                                      \
            if (guard == 1)                                                    \
                dmg /= 2;                                                      \
            else                                                               \
                dmg = Math_Div(dmg, 10);                                  \
        }                                                                      \
    } while (0)

#define TEXT_SIDE(player, enemy)                                               \
do {                                                                           \
    s32 text;                                                                  \
    if ((u32)target_id <= 7)                                                   \
        text = (player);                                                       \
    else                                                                       \
        text = (enemy);                                                        \
    BattleEvent_Push(BATTLE_EVENT_TEXT, text);                                    \
} while (0)

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

#define ADJUST_ATKDEF(field, delta, turns, value_expr, text)                   \
{                                                                              \
    (field) += (delta);                                                        \
    CLAMP_MOD(field);                                                          \
    BattleUnit_Recalculate(target_id);                                         \
    BattleEvent_Push(BATTLE_EVENT_VALUE, (value_expr));                           \
    BattleEvent_Push(BATTLE_EVENT_TEXT, (text));                                  \
    (turns) = 7;                                                               \
}

#define ADJUST_MOD137(delta, value_expr, text)                                 \
{                                                                              \
    target->status_137 += (delta);                                             \
    CLAMP_MOD(target->status_137);                                             \
    BattleEvent_Push(BATTLE_EVENT_VALUE, (value_expr));                        \
    BattleEvent_Push(BATTLE_EVENT_TEXT, (text));                               \
    target->status_136 = 7;                                                    \
}

#define SET_STATUS7(field, text)                                               \
{                                                                              \
    BattleEvent_Push(BATTLE_EVENT_TEXT, (text));                                  \
    (field) = 7;                                                               \
}

s32 Func_080bbb0c(struct BattlePlan *plan, s32 slot)
{
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
    s32 sign;
    struct BattleUnit *copy;
    s32 power;
    s32 result;
    s16 *cmd;
    s32 size;
    s32 target_id;
    s32 range;
    struct BattleUnit *target;
    s32 n;
    s16 value;
    s32 dmg;
    s32 pass;
    s32 scale;
    s32 guard;
    s16 saved[8];
    s32 count;
    s32 *mult;
    s32 tmp;

    bonus = 0;
    work = Data_03001e74;
    half = 0;
    dealt = 0;
    crush = 0;
    skip = 0;
    sign = 0;
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
    Mem_Copy(copy, target, size, Data_03001388);

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
            sign = -1;

        i = 0;
        {
            s32 off;

            off = 2;
            if (value <= *(s16 *)((u8 *)tbl + off)) {
                s16 *p;

                p = (s16 *)((u8 *)target + 36);
                do {
                    i++;
                    p += 2;
                    if (i > 3)
                        break;
                } while (value <= p[1]);
            }
        }
        if (i == 4)
            sign = 1;
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

    if (plan->command == 5 && (u32)plan->range_index <= 3 && sign > 0) {
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
    result = plan->target_results[slot];
    if (result == -1)
        result = Battle_HitCheck(
            actor_id, target_id, range, action->effect, Data_080c2ab8[offset]);
    hit = result;

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

            ch = Summon_TakeCharge(st, 1);
            if (ch & 0x8000)
                Summon_ResetCharge(st);
            BattleUnit_Assign(rec, st, ch & 0x7fff);
            slots = (s16 *)((u8 *)work + 2);
            if (slots[50] == 254) {
                slots[50] = rec;
            } else {
                s32 i;
                s16 *p;

                p = slots + 50;
                for (i = 0; i <= 5; i++) {
                    if (p[0] == 255) {
                        p[0] = rec;
                        p[1] = 255;
                        break;
                    }
                    p++;
                    if (p[0] == 254) {
                        p[0] = rec;
                        break;
                    }
                }
            }
            Summon_Refresh();
            {
                void *obj;
                s32 x;
                s32 y;

                obj = Actor_GetObject(rec);
                x = *(s32 *)((u8 *)obj + 12);
                if (x < 0)
                    x += 0xffff;
                y = *(s32 *)((u8 *)obj + 16);
                x >>= 16;
                if (y < 0)
                    y += 0xffff;
                y >>= 16;
                Actor_Place(obj, rec, x, y);
            }
            Actor_Commit();
            count = Actor_ListSlots(saved);
            if (count > 0) {
                s16 *q;

                q = saved;
                while (count != 0) {
                    count--;
                    Actor_RefreshSlot(*q);
                    q++;
                }
            }
            BattleEvent_Push(BATTLE_EVENT_UNIT, rec);
            if (action_id == 0x1f7)
                BattleEvent_Push(BATTLE_EVENT_TEXT, 0x8f5);
            else
                BattleEvent_Push(BATTLE_EVENT_TEXT, 0x8f3);
        } else if (action_id == 0x1f7) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x8f4);
        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x8f6);
        }
    }

    if (hit != 0) {
        s32 efx;

        efx = action->effect;
        if (efx == EFX_FORCE_ACTION) {
            s16 *tbl;

            hit = 0;
            tbl = (s16 *)((u8 *)work + 748);
            if (tbl[0] == (s16)target_id) {
                hit = 1;
            } else {
                for (n = 1; n <= 19; n++) {
                    if (tbl[n * 8] == (s16)target_id) {
                        hit = 1;
                        break;
                    }
                }
            }
        } else if (efx == EFX_HALF_DEF) {
            half = 1;
        } else if (efx == EFX_LETHAL) {
            crush = 1;
        } else if (efx == EFX_RETIRE) {
            skip = 1;
        } else if (efx == EFX_ACTOR_FLASH) {
            if (actor->hp != 0)
                BattleEvent_Push(BATTLE_EVENT_ACTOR_EFFECT, actor_id);
        } else if (efx == EFX_DRAIN_PP) {
            if (target->pp != 0)
                nibble = 10;
            else
                hit = 0;
        }
    }

    /* ダメージ種別。HP が残っているか、分類が非ゼロなら種別スイッチへ。 */
    if (skip == 0
        && (target->hp != 0 || BattleEffect_Classify(action->effect) != 0)
        && (u32)(nibble + 1) <= 12) {
        s32 pp;
        s32 cur;

        switch (nibble + 1) {
        case 4:
        case 5:
        {
            s32 pwr;

            scale = target->defense;
            cur = target->hp;
            if (half != 0)
                scale = (u32)scale >> 1;
            pass = 1;
            do {
                TAKE_BONUS();
                if (pass == 0)
                    bonus = 0;
                pwr = action->power;
                if (nibble == 4)
                    dmg = Math_Div(
                        Battle_CalcAttack(actor->attack, scale, 0, bonus) * pwr,
                        10);
                else
                    dmg = Battle_CalcAttack(actor->attack, scale, pwr, bonus);
                dmg *= adjust;
                if (modifier == 1)
                    dmg = dmg * 5 / 4;
                else if (modifier != 0)
                    dmg = dmg * 3 / 2;
                dmg += (u8)Math_Mod(((u8 *)target)[15], 5) + 6;
                if (pass == 0) {
                    BattleEvent_Push(BATTLE_EVENT_MARK, 0);
                    {
                        s32 text;

                        if ((u32)target_id <= 7)
                            text = 0x823;
                        else
                            text = 0x822;
                        BattleEvent_Push(BATTLE_EVENT_TEXT_CONTINUE, text);
                    }
                }
                dmg += BattleRandom_Next() & 3;
                APPLY_GUARD();
                if (dmg <= 0)
                    dmg = 1;
                if (crush != 0) {
                    if (dmg < cur - 1)
                        dmg = cur - 1;
                    if (dmg <= 0)
                        dmg = 1;
                }
                if (BattleFlag_Test(366) != 0 && *cmd == 5 && cur <= dmg) {
                    dmg = cur - 1;
                }
                pass++;
            } while (pass <= 1);
            BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            TEXT_SIDE(0x834 + sign, 0x831 + sign);
            if (cur <= dmg) {
                dealt = cur;
                target->hp = 0;
                BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
                TEXT_SIDE(0x825, 0x825);
            } else {
                dealt = dmg;
                target->hp = (s16)(cur - dmg);
                BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            }
            BattleUnit_UpdateRatios(target_id);
            break;
        }

        case 11:
        {
            s32 pwr;

            TAKE_PWR();
            pp = target->pp;
            TAKE_BONUS();
            dmg = Battle_CalcPower(action->power, bonus, 256);
            dmg = Math_Div(Data_080c2ac0[offset] * dmg, 100);
            dmg *= adjust;
            APPLY_GUARD();
            if (action->effect == EFX_DRAIN_PP && dmg > pp)
                dmg = pp;
            BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            TEXT_SIDE(0x82a, 0x829);
            pp -= dmg;
            if (pp <= 0)
                pp = 0;
            BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->pp - pp;
            target->pp = pp;
            BattleUnit_UpdateRatios(target_id);
            break;
        }

        case 2:
        {
            s32 pwr;

            TAKE_PWR();
            cur = target->hp;
            dmg = Battle_CalcRestore(pwr, range == 4 ? 100 : power, 256);
            dmg = Math_Div(Data_080c2ad8[offset] * dmg, 100);
            dmg *= adjust;
            dmg += BattleRandom_Next() & 3;
            cur += dmg;
            if (cur > target->max_hp) {
                cur = target->max_hp;
                dmg = cur - target->hp;
            }
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            if (cur == target->max_hp)
                BattleEvent_Push(BATTLE_EVENT_TEXT, 0x820);
            else {
                BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
                BattleEvent_Push(BATTLE_EVENT_TEXT, 0x81d);
            }
            goto hp_tail;
        }

        case 0:
        {
            s32 pwr;

            TAKE_PWR();
            pp = target->pp;
            TAKE_BONUS();
            dmg = Battle_CalcPower(pwr, bonus, 256);
            dmg = Math_Div(Data_080c2af0[offset] * dmg, 100);
            dmg *= adjust;
            APPLY_GUARD();
            BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            TEXT_SIDE(0x827, 0x826);
            pp -= dmg;
            if (pp <= 0)
                pp = 0;
            BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            goto pp_store;
        }

        case 6:
        case 7:
        case 9:
        {
            s32 pwr;
            s32 kind;

            TAKE_PWR();
            pass = 1;
            cur = target->hp;
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
                        dmg += Math_Div(kind * target->max_hp, 100);
                    }
                }
                dmg = Battle_CalcPower(dmg, bonus, 256);
                dmg *= adjust;
                if (nibble == 6)
                    dmg = Math_Div(Data_080c2b38[offset] * dmg, 100);
                else if (nibble == 5)
                    dmg = Math_Div(Data_080c2b08[offset] * dmg, 100);
                else if (nibble == 8)
                    dmg = Math_Div(Data_080c2b20[offset] * dmg, 100);
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
            TEXT_SIDE(0x834 + sign, 0x831 + sign);
            cur -= dmg;
            if (cur > 0)
                BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            else {
                BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
                cur = 0;
                TEXT_SIDE(0x825, 0x824);
            }
            goto hp_tail;
        }

        case 12:
        {
            s32 pwr;

            TAKE_PWR();
            pp = target->pp;
            dmg = Battle_CalcRestore(pwr, range == 4 ? 100 : power, 256);
            dmg = Math_Div(Data_080c2b50[offset] * dmg, 100);
            dmg *= adjust;
            pp += dmg;
            if (pp > target->max_pp) {
                pp = target->max_pp;
                dmg = pp - target->pp;
            }
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            if (pp == target->max_pp)
                BattleEvent_Push(BATTLE_EVENT_TEXT, 0x821);
            else {
                BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
                BattleEvent_Push(BATTLE_EVENT_TEXT, 0x81e);
            }
pp_store:
            target->pp = (s16)pp;
            BattleUnit_UpdateRatios(target_id);
            break;
        }

        case 1:
        case 8:
        case 10:
            break;

        case 3:
            if (hit != 0) {
            s32 pwr;

            TAKE_PWR();
            cur = target->hp;
            TAKE_BONUS();
            dmg = Battle_CalcPower(pwr, bonus, 256);
            dmg *= adjust;
            dmg = Math_Div(Data_080c2b68[offset] * dmg, 100);
            APPLY_GUARD();
            BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            TEXT_SIDE(0x827, 0x826);
            cur -= dmg;
            if (cur > 0)
                BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            else {
                BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
                cur = 0;
                TEXT_SIDE(0x825, 0x824);
            }
hp_tail:
            dealt = target->hp - cur;
            target->hp = (s16)cur;
            BattleUnit_UpdateRatios(target_id);
            break;
            }
            BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x854);
            break;

        }
    }

    /* 付加効果 */
    BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
    n = action->effect;
    if (BattleEffect_Classify(n) == 0 && target->hp == 0
        && BattleEffect_OnDead(n) == 0)
        goto done;
    if (hit == 0)
        goto done;
    if ((u32)(n - 3) > 66)
        goto done;

    switch (n) {
    case EFX_CURE_ALL:
        if (target->status_138 != 0) {
            target->status_138 = 0;
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88b);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->status_13b != 0) {
            target->status_13b = 0;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88d);
        }
        target->status_13c = 0;
        if (target->status_13d != 0) {
            target->status_13d = 0;
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88c);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->status_141 != 0) {
            target->status_141 = 0;
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x894);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->status_140 != 0) {
            target->status_140 = 0;
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88f);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->status_131 != 0) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x884);
            target->status_131 = 0;
        }
        BattleEvent_Push(BATTLE_EVENT_RESET, 0);
        break;

    case EFX_CURE_PART:
        if (target->status_138 != 0) {
            target->status_138 = 0;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88b);
        }
        if (target->status_13b != 0) {
            target->status_13b = 0;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88d);
        }
        target->status_13c = 0;
        if (target->status_13d != 0) {
            target->status_13d = 0;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x88c);
        }
        if (target->status_141 == 0)
            break;
        target->status_141 = 0;
        BattleEvent_Push(BATTLE_EVENT_RESET, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x894);
        break;

    case EFX_HEAL_60:
    case EFX_HEAL_30:
    {
        u16 old;
        u16 maxu;
        s32 heal;

        old = target->hp;
        heal = target->hp;
        maxu = target->max_hp;
        if (action->effect == EFX_HEAL_60)
            tmp = target->max_hp * 60;
        else
            tmp = target->max_hp * 30;
        tmp = Math_Div(tmp, 100);
        heal += tmp;
        if (heal > (s16)maxu)
            heal = (s16)maxu;
        tmp = heal - (s16)old;
        if (tmp == 0 && nibble != 1)
            break;
        if (heal == (s16)maxu)
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x820);
        else {
            BattleEvent_Push(BATTLE_EVENT_VALUE, tmp);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x81d);
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

        maxv = target->max_pp;
        heal = target->pp;
        old = heal;
        tmp = Math_Div(maxv * 7, 100);
        heal += tmp;
        if (heal > maxv)
            heal = maxv;
        tmp = heal - old;
        if (tmp == 0 && nibble != 11)
            break;
        if (heal == maxv)
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x821);
        else {
            BattleEvent_Push(BATTLE_EVENT_VALUE, tmp);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x81e);
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
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x877);
        break;

    case EFX_AGI_SET_DOWN4:
        S8OF(target->agility_modifier) = 252;
        target->agility_modifier_turns = 5;
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, copy->agility - target->agility);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x878);
        break;

    case EFX_ATK_DOWN1:
        ADJUST_ATKDEF(target->attack_modifier, -1, target->attack_modifier_turns,
                      copy->attack - target->attack, 0x860);
        break;

    case EFX_ATK_DOWN2:
        ADJUST_ATKDEF(target->attack_modifier, -2, target->attack_modifier_turns,
                      copy->attack - target->attack, 0x860);
        break;

    case EFX_ATK_UP1:
        ADJUST_ATKDEF(target->attack_modifier, 1, target->attack_modifier_turns,
                      target->attack - copy->attack, 0x861);
        break;

    case EFX_ATK_UP2:
        ADJUST_ATKDEF(target->attack_modifier, 2, target->attack_modifier_turns,
                      target->attack - copy->attack, 0x861);
        break;

    case EFX_DEF_DOWN1:
        ADJUST_ATKDEF(target->defense_modifier, -1, target->defense_modifier_turns,
                      copy->defense - target->defense, 0x862);
        break;

    case EFX_DEF_DOWN2:
        ADJUST_ATKDEF(target->defense_modifier, -2, target->defense_modifier_turns,
                      copy->defense - target->defense, 0x862);
        break;

    case EFX_DEF_UP1:
        ADJUST_ATKDEF(target->defense_modifier, 1, target->defense_modifier_turns,
                      target->defense - copy->defense, 0x863);
        break;

    case EFX_DEF_UP2:
        ADJUST_ATKDEF(target->defense_modifier, 2, target->defense_modifier_turns,
                      target->defense - copy->defense, 0x863);
        break;

    case EFX_REVIVE_FULL:
        if (target->hp != 0)
            break;
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x864);
        target->hp = target->max_hp;
        BattleUnit_UpdateRatios(target_id);
        break;

    case EFX_REVIVE_HALF:
        if (target->hp != 0)
            break;
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x864);
        target->hp = (s16)(target->max_hp / 2);
        BattleUnit_UpdateRatios(target_id);
        break;

    case EFX_REVIVE_80:
        if (target->hp != 0)
            break;
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x864);
        target->hp = (s16)Math_Div(target->max_hp * 8, 10);
        BattleUnit_UpdateRatios(target_id);
        break;

    case EFX_AIL131_CLEAR:
        if (target->status_131 != 0)
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x884);
        target->status_131 = 0;
        break;

    case EFX_MOD137_DOWN1:
        ADJUST_MOD137(-1, (copy->status_137 - target->status_137) * 20, 0x865);
        break;

    case EFX_MOD137_DOWN2:
        ADJUST_MOD137(-2, (copy->status_137 - target->status_137) * 20, 0x865);
        break;

    case EFX_MOD137_UP1:
        ADJUST_MOD137(1, (target->status_137 - copy->status_137) * 20, 0x866);
        break;

    case EFX_MOD137_UP2:
        ADJUST_MOD137(2, (target->status_137 - copy->status_137) * 20, 0x866);
        break;

    case EFX_AIL131_1:
        if (target->status_131 != 0)
            break;
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x867);
        target->status_131 = 1;
        break;

    case EFX_AIL131_2:
        if (target->status_131 > 1)
            break;
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x874);
        target->status_131 = 2;
        break;

    case EFX_AIL138:
        SET_STATUS7(target->status_138, 0x868);
        break;

    case EFX_AIL139:
        SET_STATUS7(target->status_139, 0x869);
        break;

    case EFX_AIL13A:
        SET_STATUS7(target->status_13a, 0x86a);
        break;

    case EFX_AIL13B:
        SET_STATUS7(target->status_13b, 0x86b);
        break;

    case EFX_AIL13C:
        SET_STATUS7(target->status_13c, 0x86c);
        break;

    case EFX_AIL13D_LOW:
        TEXT_SIDE(0x86d, 0x876);
        target->status_13d |= 7;
        break;

    case EFX_AIL13D_HIGH:
        TEXT_SIDE(0x86d, 0x876);
        target->status_13d |= 16;
        break;

    case EFX_RETIRE:
        BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
        if (target->status_12a == 2)
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x84f);
        else if (action_id == 219)
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x850);
        else
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x84c);
        target->hp = 0;
        BattleUnit_UpdateRatios(target_id);
        break;

    case EFX_AIL13E:
        SET_STATUS7(target->status_13e, 0x86f);
        break;

    case EFX_AIL13F:
        SET_STATUS7(target->status_13f, 0x870);
        break;

    case EFX_DRAIN_HP:
    case EFX_DRAIN_HP_HALF:
        dmg = dealt;
        if (n == EFX_DRAIN_HP_HALF)
            dmg /= 2;
        if (actor->hp + dmg > actor->max_hp) {
            dmg = actor->max_hp - actor->hp;
            actor->hp = actor->max_hp;
        } else {
            actor->hp = (s16)(actor->hp + dmg);
        }
        BattleEvent_Push(BATTLE_EVENT_RESET, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT, actor_id);
        if (actor->hp == actor->max_hp)
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x820);
        else {
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x81d);
        }
        BattleUnit_UpdateRatios(actor_id);
        break;

    case EFX_DRAIN_PP:
        dmg = dealt;
        if (actor->pp + dmg > actor->max_pp) {
            dmg = actor->max_pp - actor->pp;
            actor->pp = actor->max_pp;
        } else {
            actor->pp = (s16)(actor->pp + dmg);
        }
        BattleEvent_Push(BATTLE_EVENT_RESET, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT, actor_id);
        if (actor->pp == actor->max_pp)
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x821);
        else {
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x81e);
        }
        BattleUnit_UpdateRatios(actor_id);
        break;

    case EFX_PP_LEECH:
        dmg = Math_Div(dealt, 10);
        if (target->pp < dmg)
            dmg = target->pp;
        if (actor->pp + dmg > actor->max_pp)
            dmg = actor->max_pp - actor->pp;
        if (dmg == 0)
            break;
        BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
        TEXT_SIDE(0x85f, 0x85e);
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
        if (target->status_137 > 0) {
            target->status_137 = 0;
            target->status_136 = 0;
        }
        if (target->agility_modifier > 0)
            target->agility_modifier = 0;
        target->status_12c = 0;
        target->status_12e = 0;
        target->status_12f = 0;
        target->status_12d = 0;
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x896);
        break;

    case EFX_AIL140:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x872);
        target->status_140 = 1;
        break;

    case EFX_AIL141:
        if (target->status_141 == 0) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x873);
            target->status_141 = 7;
            break;
        }
        if (target->status_141 <= 1)
            break;
        target->status_141 -= 1;
        BattleEvent_Push(BATTLE_EVENT_VALUE, target->status_141);
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x875);
        break;

    case EFX_SET_144:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x87d);
        ((u8 *)target)[0x144] = 2;
        break;

    case EFX_BATTLE_END:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x87e);
        target->battle_end_state = 1;
        if ((u32)target_id <= 7)
            ((u8 *)work)[67] |= 2;
        break;

    case EFX_FORCE_ACTION:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x87f);
        target->forced_action = 1;
        break;

    case EFX_GUARD1:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x881);
        if (S8OF(target->guard_level) > 0)
            break;
        S8OF(target->guard_level) = 1;
        break;

    case EFX_GUARD2:
        BattleEvent_Push(BATTLE_EVENT_TEXT, 0x882);
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
        if (target->status_13c != 0 && target->status_13c <= 6
            && dealt > 0 && (BattleRandom_Next() & 3) == 0) {
            target->status_13c = 0;
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, 0x85f);
        }
    }
    Sys_Free(copy);
    BattleUnit_Recalculate(target_id);
    Sys_SetMode(((u8 *)Data_03001e74)[65]);
    if (target->hp != 0)
        BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
    if (actor->status_140 != 0 && (BattleRandom_Next() & 3) == 0 && dealt > 0) {
        s32 share;

        share = dealt >> 2;
        if (share == 0)
            share = 1;
        plan->pending_amount_60 += share;
    }
}
