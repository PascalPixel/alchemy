#include "types.h"
#include "item.h"
#include "owner_state.h"
#include "runtime_mem.h"
#include "game_flags.h"
#include "battle_calc.h"

struct OwnerCombatState {
    u8 unknown_000[0x10];   /* 0x00 */
    s16 cur_hp;              /* 0x10 */
    s16 cur_pp;              /* 0x12 */
    s16 hp_growth_q14;       /* 0x14 */
    s16 pp_growth_q14;       /* 0x16 */
    u16 base_attack;         /* 0x18 */
    u16 base_defense;        /* 0x1a */
    u16 base_agility;        /* 0x1c */
    u8 base_luck;            /* 0x1e */
    u8 element_flags;        /* 0x1f */
    u8 class_form;           /* 0x20 */
    u8 unk_021;              /* 0x21 */
    u8 unknown_022[2];
    s16 growth[4][2];        /* 0x24 */
    s16 hp_pool;             /* 0x34 */
    s16 pp_pool;             /* 0x36 */
    s16 hp_pool_flag;        /* 0x38 */
    s16 pp_pool_flag;        /* 0x3a */
    s16 final_attack;        /* 0x3c */
    s16 final_defense;       /* 0x3e */
    s16 final_agility;       /* 0x40 */
    u8 final_luck;           /* 0x42 */
    u8 final_unk_1c;         /* 0x43 */
    u8 final_class_form;     /* 0x44 */
    u8 final_unk_36;         /* 0x45 */
    u8 unknown_046[2];
    s16 final_growth[4][2];  /* 0x48 */
    u8 unknown_058[0x80];
    u16 equip_mask[15];      /* 0xd8 */
    u8 unknown_0f6[18];
    u32 djinn_bits[4];       /* 0x108 */
    u8 unknown_118[16];
    u8 class_id;             /* 0x128 */
    u8 equip_active;         /* 0x129 */
    u8 unknown_12a[2];
    s8 luck_curve[4];        /* 0x12c */
    u8 packed_flags;         /* 0x130 */
    u8 unknown_131[2];
    s8 class_pct_attack;     /* 0x133 */
    u8 unknown_134;
    s8 class_pct_defense;    /* 0x135 */
    u8 unknown_136;
    s8 agility_curve[4];     /* 0x137 */
    u8 unknown_13b[7];
    u8 flag_322;             /* 0x142 */
    u8 flag_323;             /* 0x143 */
    u8 pending_flag;         /* 0x144 */
    u8 unknown_145[2];
    s8 class_pct_agility;    /* 0x147 */
};

struct RecalcWork {
    s32 hp_accum;    /* 0x00 */
    s32 pp_accum;    /* 0x04 */
    s32 attack;      /* 0x08 */
    s32 defense;     /* 0x0c */
    s32 agility;     /* 0x10 */
    u8 unknown_014[4];
    s32 luck;        /* 0x18 */
    s32 unk_1c;      /* 0x1c */
    s32 class_form;  /* 0x20 */
    s32 unk_24;      /* 0x24 */
    struct {
        s32 a;
        s32 b;
    } growth[4];     /* 0x28 */
    s32 effect_kind; /* 0x48 */
    u8 unknown_04c[8];
    s32 effect_amount; /* 0x54 */
    void *item;         /* 0x58 */
};

s32 Func_08004970(s32 size);
s8 *Func_0807a0cc(s32 owner, s32 bit_index);
s32 Func_08079ad8(s32 class_form);

void Owner_RecalculateStats(s32 owner)
{
    struct RecalcWork *work;
    struct OwnerCombatState *st;
    s32 i, j;
    s32 v, q0, q1;

    work = (struct RecalcWork *)Func_08004970(96);
    st = (struct OwnerCombatState *)Owner_GetState(owner);

    work->hp_accum = st->cur_hp;
    work->pp_accum = st->cur_pp;
    work->attack = st->base_attack;
    work->defense = st->base_defense;
    work->agility = st->base_agility;
    work->luck = st->base_luck;
    work->unk_1c = st->element_flags & 0xf;
    work->class_form = st->class_form;
    work->unk_24 = st->unk_021;

    {
        s16 *src;
        s32 *dst;
        s32 cnt;

        src = &st->growth[0][0];
        dst = &work->growth[0].a;
        cnt = 3;
        do {
            dst[0] = src[0];
            dst[1] = src[1];
            src += 2;
            dst += 2;
            cnt--;
        } while (cnt >= 0);
    }

    {
        s32 d;

        q0 = st->hp_pool;
        q0 *= st->hp_growth_q14;
        if (q0 < 0) {
            q0 += 0x3fff;
        }
        q0 >>= 14;
        d = q0 - st->hp_pool_flag;
        if (d < 0) {
            d = st->hp_pool_flag - q0;
            if (d > 1) {
                goto do_reset;
            }
        } else {
            if (d > 1) {
                goto do_reset;
            }
        }

        q1 = st->pp_pool;
        q1 *= st->pp_growth_q14;
        if (q1 < 0) {
            q1 += 0x3fff;
        }
        q1 >>= 14;
        d = q1 - st->pp_pool_flag;
        if (d < 0) {
            d = st->pp_pool_flag - q1;
            if (d > 1) {
                goto do_reset;
            }
        } else {
            if (d <= 1) {
                goto reset_done;
            }
        }

    do_reset:
        st->hp_growth_q14 = 0x4000;
        st->pp_growth_q14 = 0x4000;
        st->hp_pool_flag = st->hp_pool;
        st->pp_pool_flag = st->pp_pool;
    reset_done:
        ;
    }

    st->packed_flags = st->packed_flags & ~3;
    if (st->packed_flags & 4) {
        st->packed_flags |= 1;
    }

    if (st->pending_flag != 0) {
        work->unk_1c += 1;
    }

    st->flag_322 = 0;
    st->flag_323 = 0;

    if (st->equip_active != 0) {
        for (i = 0; i < 15; i++) {
            if (st->equip_mask[i] & 0x200) {
                struct ItemDefinition *item;

                item = Item_GetDirect(st->equip_mask[i]);
                work->item = item;
                if (item->flags & 1) {
                    st->packed_flags |= 3;
                }
                work->attack += item->primary_bonus;
                work->defense += item->secondary_bonus;

                for (j = 0; j < 4; j++) {
                    s32 kind;

                    kind = item->effects[j].kind;
                    work->effect_kind = kind;
                    work->effect_amount = item->effects[j].amount;

                    switch (kind) {
                    case 0:
                        break;
                    case 1:
                        work->hp_accum += work->effect_amount;
                        break;
                    case 2:
                        work->class_form += work->effect_amount;
                        break;
                    case 3:
                        work->pp_accum += work->effect_amount;
                        break;
                    case 4:
                        work->unk_24 += work->effect_amount;
                        break;
                    case 5:
                        work->agility += work->effect_amount;
                        break;
                    case 6:
                        work->luck += work->effect_amount;
                        break;
                    case 15:
                        work->growth[0].a += work->effect_amount;
                        break;
                    case 16:
                        work->growth[1].a += work->effect_amount;
                        break;
                    case 17:
                        work->growth[2].a += work->effect_amount;
                        break;
                    case 18:
                        work->growth[3].a += work->effect_amount;
                        break;
                    case 19:
                        work->growth[0].b += work->effect_amount;
                        break;
                    case 20:
                        work->growth[1].b += work->effect_amount;
                        break;
                    case 21:
                        work->growth[2].b += work->effect_amount;
                        break;
                    case 22:
                        work->growth[3].b += work->effect_amount;
                        break;
                    case 23:
                        st->flag_322 = (u8)(st->flag_322 + work->effect_amount);
                        break;
                    case 24:
                        st->flag_323 = (u8)(st->flag_323 + work->effect_amount);
                        break;
                    case 25:
                        st->packed_flags |= 8;
                        break;
                    case 26:
                        work->unk_1c += work->effect_amount;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }

    if (st->packed_flags & 8) {
        st->packed_flags = st->packed_flags & ~0xa;
    }

    for (i = 0; i < 4; i++) {
        u32 bits;

        bits = st->djinn_bits[i];
        for (j = 0; j < 20; j++) {
            if (bits & (1u << j)) {
                s8 *djinn;

                djinn = Func_0807a0cc(i, j);
                work->hp_accum += djinn[4];
                work->pp_accum += djinn[5];
                work->attack += djinn[6];
                work->defense += djinn[7];
                work->agility += djinn[8];
                work->luck += djinn[9];
            }
        }
    }

    {
        u8 *classdef;

        classdef = (u8 *)Func_08079ad8(st->equip_active);
        work->hp_accum = Math_Div(classdef[8] * work->hp_accum, 10);
        work->pp_accum = Math_Div(classdef[9] * work->pp_accum, 10);
        work->attack = Math_Div(classdef[10] * work->attack, 10);
        work->defense = Math_Div(classdef[11] * work->defense, 10);
        work->agility = Math_Div(classdef[12] * work->agility, 10);
        work->luck = Math_Div(classdef[13] * work->luck, 10);
    }

    for (i = 0; i < 15; i++) {
        if (st->equip_mask[i] & 0x200) {
            struct ItemDefinition *item;

            item = Item_GetDirect(st->equip_mask[i]);
            work->item = item;

            for (j = 0; j < 4; j++) {
                s32 kind;
                s32 idx;

                kind = item->effects[j].kind;
                work->effect_kind = kind;
                idx = kind - 7;
                work->effect_amount = item->effects[j].amount;

                switch (idx) {
                case 0:
                    work->hp_accum = Math_Div(work->effect_amount * work->hp_accum, 10);
                    break;
                case 1:
                    work->class_form = Math_Div(work->effect_amount * work->class_form, 10);
                    break;
                case 2:
                    work->pp_accum = Math_Div(work->effect_amount * work->pp_accum, 10);
                    break;
                case 3:
                    work->unk_24 = Math_Div(work->effect_amount * work->unk_24, 10);
                    break;
                case 4:
                    work->attack = Math_Div(work->effect_amount * work->attack, 10);
                    break;
                case 5:
                    work->defense = Math_Div(work->effect_amount * work->defense, 10);
                    break;
                case 6:
                    work->agility = Math_Div(work->effect_amount * work->agility, 10);
                    break;
                case 7:
                    work->luck = Math_Div(work->effect_amount * work->luck, 10);
                    break;
                default:
                    break;
                }
            }
        }
    }

    v = st->class_pct_attack;
    v = v * work->attack + 8;
    if (v < 0) {
        v += 7;
    }
    work->attack = v >> 3;

    v = st->class_pct_defense;
    v = v * work->defense + 8;
    if (v < 0) {
        v += 7;
    }
    work->defense = v >> 3;

    v = st->class_pct_agility;
    v = v * work->agility + 8;
    if (v < 0) {
        v += 7;
    }
    work->agility = v >> 3;

    for (i = 0; i < 4; i++) {
        s32 *slot;
        s32 x;

        slot = &((s32 *)&work->growth[0].a)[i * 2];
        x = st->luck_curve[i];
        *slot += 5 * (x * x + x);
    }

    for (i = 0; i < 4; i++) {
        s32 *slot;
        s32 x;

        slot = &((s32 *)&work->growth[0].b)[i * 2];
        x = st->agility_curve[i];
        *slot += 20 * x;
    }

    if (st->equip_active != 0) {
        if (st->class_id > 5) {
            v = 0;
        } else {
            switch (st->class_id) {
            case 0:
                v = GameFlag_Test(272);
                break;
            case 1:
                v = GameFlag_Test(274);
                break;
            case 2:
                v = GameFlag_Test(275);
                break;
            case 3:
                v = GameFlag_Test(273);
                break;
            case 4:
                v = 0;
                break;
            case 5:
                v = GameFlag_Test(274);
                break;
            default:
                v = 0;
                break;
            }
        }
        if (v != 0) {
            work->unk_24 += 4;
        }
    }

    if (work->attack < 0) {
        work->attack = 0;
    }
    if (work->attack > 999) {
        work->attack = 999;
    }
    if (work->defense < 0) {
        work->defense = 0;
    }
    if (work->defense > 999) {
        work->defense = 999;
    }
    if (work->agility < 0) {
        work->agility = 0;
    }
    if (work->agility > 999) {
        work->agility = 999;
    }
    if (work->luck < 0) {
        work->luck = 0;
    }
    if (work->luck > 99) {
        work->luck = 99;
    }
    if (work->unk_1c < 0) {
        work->unk_1c = 0;
    }
    if (work->unk_1c > 2) {
        work->unk_1c = 2;
    }
    if (work->class_form < 0) {
        work->class_form = 0;
    }
    if (work->class_form > 10000) {
        work->class_form = 10000;
    }
    if (work->unk_24 < 0) {
        work->unk_24 = 0;
    }
    if (work->unk_24 > 200) {
        work->unk_24 = 200;
    }

    for (i = 0; i < 4; i++) {
        if (work->growth[i].a < 0) {
            work->growth[i].a = 0;
        }
        if (work->growth[i].a > 200) {
            work->growth[i].a = 200;
        }
        if (work->growth[i].b < 0) {
            work->growth[i].b = 0;
        }
        if (work->growth[i].b > 200) {
            work->growth[i].b = 200;
        }
    }

    st->final_attack = (s16)work->attack;
    st->final_defense = (s16)work->defense;
    st->final_agility = (s16)work->agility;
    st->final_luck = (u8)work->luck;
    st->final_unk_1c = (u8)work->unk_1c;
    st->final_class_form = (u8)work->class_form;
    st->final_unk_36 = (u8)work->unk_24;

    for (i = 0; i < 4; i++) {
        st->final_growth[i][0] = (s16)work->growth[i].a;
        st->final_growth[i][1] = (s16)work->growth[i].b;
    }

    {
        s32 cap;

        cap = (st->equip_active != 0) ? 1999 : 9999;

        v = st->hp_pool;
        if (v < 0) {
            v = 0;
        }
        if (v > cap) {
            v = cap;
        }
        st->hp_pool = (s16)v;
        if (st->cur_hp != v) {
            q0 = st->hp_growth_q14 * v;
            if (q0 < 0) {
                q0 += 0x3fff;
            }
            q0 >>= 14;
            if (q0 < 0) {
                q0 = 0;
            }
            if (q0 > cap) {
                q0 = cap;
            }
            if (st->hp_pool_flag != 0 && q0 == 0) {
                q0 = 1;
            }
            st->hp_pool_flag = (s16)q0;
        }

        v = st->pp_pool;
        if (v < 0) {
            v = 0;
        }
        if (v > cap) {
            v = cap;
        }
        st->pp_pool = (s16)v;
        if (st->cur_pp != v) {
            q1 = st->pp_growth_q14 * v;
            if (q1 < 0) {
                q1 += 0x3fff;
            }
            q1 >>= 14;
            if (q1 < 0) {
                q1 = 0;
            }
            if (q1 > cap) {
                q1 = cap;
            }
            if (st->pp_pool_flag != 0 && q1 == 0) {
                q1 = 1;
            }
            st->pp_pool_flag = (s16)q1;
        }
    }

    Sys_Free(work);
}
