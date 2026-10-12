#include "mem.h"
#include "types.h"
#include "item.h"
#include "game_flags.h"
#include "owner_state.h"

struct OwnerStats {
    u8 unknown_000[0x10];
    s16 base_hp;
    s16 base_ep;
    s16 hp_ratio;
    s16 pp_ratio;
    u16 base_attack;
    u16 base_defense;
    u16 base_agility;
    u8 base_luck;
    u8 base_turns;
    u8 base_20;
    u8 base_21;
    u8 unknown_022[2];
    s16 base_element[4][2];
    s16 max_hp;
    s16 max_ep;
    s16 hp;
    s16 ep;
    s16 atk;
    s16 def;
    s16 agi;
    u8 luck;
    u8 turns;
    u8 stat_44;
    u8 stat_45;
    u8 unknown_046[2];
    s16 element[4][2];
    u8 unknown_058[0x80];
    u16 equipment[15];
    u8 unknown_0f6[0x12];
    u32 djinn[4];
    u8 unknown_118[0x10];
    u8 character;
    u8 class_id;
    u8 unknown_12a[2];
    s8 element_level[4];
    s8 curse;
    u8 unknown_131[2];
    s8 attack_level;
    u8 unknown_134;
    s8 defense_level;
    u8 unknown_136;
    s8 resist_level;
    u8 unknown_138[3];
    u8 unknown_13b[7];
    u8 bonus_142;
    u8 bonus_143;
    u8 extra_turn;
    u8 unknown_145[2];
    s8 agility_level;
};

struct DjinnDefinition {
    u8 unknown_00[4];
    s8 hp;
    s8 ep;
    s8 atk;
    s8 def;
    s8 agi;
    s8 luck;
};

struct ClassRecord {
    u8 unknown_00[8];
    u8 hp;
    u8 ep;
    u8 atk;
    u8 def;
    u8 agi;
    u8 luck;
};

struct StatWork {
    s32 hp;
    s32 ep;
    s32 atk;
    s32 def;
    s32 agi;
    s32 unused_14;
    s32 luck;
    s32 turns;
    s32 stat_20;
    s32 stat_24;
    s32 element[4][2];
    s32 kind;
    s32 unused_4c[2];
    s32 amount;
    struct ItemDefinition *item;
    s32 unused_5c;
};

s32 __divsi3(s32 numerator, s32 denominator);

#define STAT_DIFF(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

void UnitCalc(s32 owner)
{
    struct StatWork *work;
    struct OwnerStats *st;
    s32 i, j, el;
    s32 value;
    s32 flag;
    s32 kind;
    s32 amount;
    s32 cap;
    s16 old;

    work = Runtime_BumpAllocateAlternatePool(sizeof(struct StatWork));
    st = UnitGet(owner);
    work->hp = st->base_hp;
    work->ep = st->base_ep;
    work->atk = st->base_attack;
    work->def = st->base_defense;
    work->agi = st->base_agility;
    work->luck = st->base_luck;
    work->turns = st->base_turns & 15;
    work->stat_20 = st->base_20;
    work->stat_24 = st->base_21;
    {
        s16 *src = st->base_element[0];
        s32 *dst = work->element[0];
        for (i = 0; i < 4; i++) {
            dst[0] = src[0];
            dst[1] = src[1];
            src += 2;
            dst += 2;
        }
    }

    if (STAT_DIFF(st->max_hp * st->hp_ratio / 0x4000, st->hp) > 1
        || STAT_DIFF(st->max_ep * st->pp_ratio / 0x4000, st->ep) > 1) {
        st->hp_ratio = 0x4000;
        st->pp_ratio = 0x4000;
        st->hp = st->max_hp;
        st->ep = st->max_ep;
    }

    st->curse &= ~3;
    if (st->curse & 4)
        st->curse |= 1;
    if (st->extra_turn)
        work->turns++;
    st->bonus_142 = 0;
    st->bonus_143 = 0;

    if (st->class_id) {
        for (i = 0; i < 15; i++) {
            if (!(st->equipment[i] & 0x200))
                continue;
            work->item = Item_GetDirect(st->equipment[i]);
            if (work->item->flags & 1)
                st->curse |= 3;
            /* FAKEMATCH: a do-while barrier keeps the defense load after the
               item bonus load, as the ROM schedules it */
            do {
                work->atk += work->item->primary_bonus;
            } while (0);
            work->def += work->item->secondary_bonus;
            for (j = 0; j < 4; j++) {
                kind = work->item->effects[j].kind;
                amount = work->item->effects[j].amount;
                work->kind = kind;
                work->amount = amount;
                switch (work->kind) {
                case 0:
                    break;
                case 1:
                    work->hp += work->amount;
                    break;
                case 2:
                    work->stat_20 += work->amount;
                    break;
                case 3:
                    work->ep += work->amount;
                    break;
                case 4:
                    work->stat_24 += work->amount;
                    break;
                case 5:
                    work->agi += work->amount;
                    break;
                case 6:
                    work->luck += work->amount;
                    break;
                case 15:
                    work->element[0][0] += work->amount;
                    break;
                case 16:
                    work->element[1][0] += work->amount;
                    break;
                case 17:
                    work->element[2][0] += work->amount;
                    break;
                case 18:
                    work->element[3][0] += work->amount;
                    break;
                case 19:
                    work->element[0][1] += work->amount;
                    break;
                case 20:
                    work->element[1][1] += work->amount;
                    break;
                case 21:
                    work->element[2][1] += work->amount;
                    break;
                case 22:
                    work->element[3][1] += work->amount;
                    break;
                case 23:
                    st->bonus_142 += work->amount;
                    break;
                case 24:
                    st->bonus_143 += work->amount;
                    break;
                case 25:
                    st->curse |= 8;
                    break;
                case 26:
                    work->turns += work->amount;
                    break;
                }
            }
        }
        if (st->curse & 8)
            st->curse &= ~9;

        for (el = 0; el < 4; el++) {
            u32 bits = st->djinn[el];

            for (i = 0; i < 20; i++) {
                if (bits & (1 << i)) {
                    const struct DjinnDefinition *djinn =
                        (const struct DjinnDefinition *)Djinn_GetDefinition(el, i);

                    work->hp += djinn->hp;
                    work->ep += djinn->ep;
                    work->atk += djinn->atk;
                    work->def += djinn->def;
                    work->agi += djinn->agi;
                    work->luck += djinn->luck;
                }
            }
        }

        {
            struct ClassRecord *class = (struct ClassRecord *)Owner_GetRecordStride84(st->class_id);

            work->hp = __divsi3(work->hp * class->hp, 10);
            work->ep = __divsi3(work->ep * class->ep, 10);
            work->atk = __divsi3(work->atk * class->atk, 10);
            work->def = __divsi3(work->def * class->def, 10);
            work->agi = __divsi3(work->agi * class->agi, 10);
            work->luck = __divsi3(work->luck * class->luck, 10);
        }

        for (i = 0; i < 15; i++) {
            if (!(st->equipment[i] & 0x200))
                continue;
            work->item = Item_GetDirect(st->equipment[i]);
            for (j = 0; j < 4; j++) {
                kind = work->item->effects[j].kind;
                amount = work->item->effects[j].amount;
                work->kind = kind;
                kind -= 7;
                work->amount = amount;
                switch (kind) {
                case 0:
                    work->hp = __divsi3(work->hp * work->amount, 10);
                    break;
                case 1:
                    work->stat_20 = __divsi3(work->stat_20 * work->amount, 10);
                    break;
                case 2:
                    work->ep = __divsi3(work->ep * work->amount, 10);
                    break;
                case 3:
                    work->stat_24 = __divsi3(work->stat_24 * work->amount, 10);
                    break;
                case 4:
                    work->atk = __divsi3(work->atk * work->amount, 10);
                    break;
                case 5:
                    work->def = __divsi3(work->def * work->amount, 10);
                    break;
                case 6:
                    work->agi = __divsi3(work->agi * work->amount, 10);
                    break;
                case 7:
                    work->luck = __divsi3(work->luck * work->amount, 10);
                    break;
                }
            }
        }
    }

    work->atk = work->atk * (st->attack_level + 8) / 8;
    work->def = work->def * (st->defense_level + 8) / 8;
    work->agi = work->agi * (st->agility_level + 8) / 8;
    for (i = 0; i < 4; i++)
        work->element[i][0] += (st->element_level[i] * st->element_level[i] + st->element_level[i]) * 5;
    for (i = 0; i < 4; i++)
        work->element[i][1] += st->resist_level * 20;

    if (st->class_id) {
        flag = 0;
        switch (st->character) {
        case 0:
            flag = GameFlag_Test(0x110);
            break;
        case 1:
            flag = GameFlag_Test(0x112);
            break;
        case 2:
            flag = GameFlag_Test(0x113);
            break;
        case 3:
            flag = GameFlag_Test(0x111);
            break;
        case 5:
            flag = GameFlag_Test(0x112);
            break;
        }
        if (flag)
            work->stat_24 += 4;
    }

    if (work->atk < 0)
        work->atk = 0;
    if (work->atk > 999)
        work->atk = 999;
    if (work->def < 0)
        work->def = 0;
    if (work->def > 999)
        work->def = 999;
    if (work->agi < 0)
        work->agi = 0;
    if (work->agi > 999)
        work->agi = 999;
    if (work->luck < 0)
        work->luck = 0;
    if (work->luck > 99)
        work->luck = 99;
    if (work->turns < 0)
        work->turns = 0;
    if (work->turns > 2)
        work->turns = 2;
    if (work->stat_20 < 0)
        work->stat_20 = 0;
    if (work->stat_20 > 10000)
        work->stat_20 = 10000;
    if (work->stat_24 < 0)
        work->stat_24 = 0;
    if (work->stat_24 > 200)
        work->stat_24 = 200;
    for (i = 0; i < 4; i++) {
        if (work->element[i][0] < 0)
            work->element[i][0] = 0;
        if (work->element[i][0] > 200)
            work->element[i][0] = 200;
        if (work->element[i][1] < 0)
            work->element[i][1] = 0;
        if (work->element[i][1] > 200)
            work->element[i][1] = 200;
    }

    st->atk = work->atk;
    st->def = work->def;
    st->agi = work->agi;
    st->luck = work->luck;
    st->turns = work->turns;
    st->stat_44 = work->stat_20;
    st->stat_45 = work->stat_24;
    for (i = 0; i < 4; i++) {
        st->element[i][0] = work->element[i][0];
        st->element[i][1] = work->element[i][1];
    }

    cap = 9999;
    if (st->class_id)
        cap = 1999;

    old = st->max_hp;
    if (work->hp < 0)
        work->hp = 0;
    if (work->hp > cap)
        work->hp = cap;
    st->max_hp = work->hp;
    if (old != st->max_hp) {
        value = work->hp * st->hp_ratio / 0x4000;
        if (value < 0)
            value = 0;
        if (value > cap)
            value = cap;
        if (st->hp != 0 && value == 0)
            value = 1;
        st->hp = value;
    }

    old = st->max_ep;
    if (work->ep < 0)
        work->ep = 0;
    if (work->ep > cap)
        work->ep = cap;
    st->max_ep = work->ep;
    if (old != st->max_ep) {
        value = work->ep * st->pp_ratio / 0x4000;
        if (value < 0)
            value = 0;
        if (value > cap)
            value = cap;
        if (st->ep != 0 && value == 0)
            value = 1;
        st->ep = value;
    }

    MemFree(work);
}
