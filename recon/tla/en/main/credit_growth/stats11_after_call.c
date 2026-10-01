/* Draft only, 2026-10-01. Bounded TLA source-lifetime attempt stats11-after-call.
 * Ordinary compiler emits 2052 text bytes; 1058 byte positions differ
 * from the full English native extent of 2036 bytes, including padding.
 * Listing: recon/tla/raw/080ad3f8.s. No adoption or new credit.
 * Complete six-edition owner/import proof remains mandatory.
 */
/*
 * Draft: Owner_RecalculateStats from the ☀️ source does not yet match ⚓️;
 * the ⚓️ body is about a quarter different (other stat offsets and a
 * shorter body). Links as recon/tla/raw/080ad3f8.s.
 */
#include "TYPES.H"
#include "ITEM.H"
#include "GAME_FLAGS.H"

/* One party member's stored state: base statistics, derived statistics
   and what the recalculation reads from equipment, Djinn and class. */
struct OwnerStats {
    u8 unknown_000[0x10];
    s16 base_hp;                /* 0x10 */
    s16 base_pp;                /* 0x12 */
    s16 hp_ratio;               /* 0x14 current/maximum HP, Q14 */
    s16 pp_ratio;               /* 0x16 current/maximum PP, Q14 */
    u16 base_attack;            /* 0x18 */
    u16 base_defense;           /* 0x1a */
    u16 base_agility;           /* 0x1c */
    u8 base_luck;               /* 0x1e */
    u8 base_turns;              /* 0x1f low nibble */
    u8 base_20;                 /* 0x20 */
    u8 base_21;                 /* 0x21 */
    u8 unknown_022[2];
    s16 base_element[4][2];     /* 0x24 power, resistance */
    s16 max_hp;                 /* 0x34 */
    s16 max_pp;                 /* 0x36 */
    s16 hp;                     /* 0x38 */
    s16 pp;                     /* 0x3a */
    s16 attack;                 /* 0x3c */
    s16 defense;                /* 0x3e */
    s16 agility;                /* 0x40 */
    u8 luck;                    /* 0x42 */
    u8 turns;                   /* 0x43 */
    u8 stat_44;                 /* 0x44 */
    u8 stat_45;                 /* 0x45 */
    u8 unknown_046[2];
    s16 element[4][2];          /* 0x48 */
    u8 unknown_058[0x80];
    u16 equipment[15];          /* 0xd8 */
    u8 unknown_0f6[0x12];
    u32 djinn[4];               /* 0x108 */
    u8 unknown_118[0x10];
    u8 character;               /* 0x128 */
    u8 class_id;                /* 0x129 */
    u8 unknown_12a[2];
    s8 element_level[4];        /* 0x12c */
    s8 curse;                   /* 0x130 */
    u8 unknown_131[2];
    s8 attack_level;            /* 0x133 */
    u8 unknown_134;
    s8 defense_level;           /* 0x135 */
    u8 unknown_136;
    s8 resist_level;            /* 0x137 */
    u8 unknown_138[3];
    u8 unknown_13b[7];
    u8 bonus_142;               /* 0x142 */
    u8 bonus_143;               /* 0x143 */
    u8 extra_turn;              /* 0x144 */
    u8 unknown_145[2];
    s8 agility_level;           /* 0x147 */
    u8 unknown_148[2];
    u16 bonus_flag_group;        /* 0x14a: eight owners share four elemental bonus flags */
};

/* Djinn definition: the statistic bonuses it adds while set. */
struct DjinnDefinition {
    u8 unknown_00[4];
    s8 hp;                      /* 0x04 */
    s8 pp;                      /* 0x05 */
    s8 attack;                  /* 0x06 */
    s8 defense;                 /* 0x07 */
    s8 agility;                 /* 0x08 */
    s8 luck;                    /* 0x09 */
};

/* Class record: statistic multipliers in tenths. */
struct ClassRecord {
    u8 unknown_00[8];
    u8 hp;                      /* 0x08 */
    u8 pp;                      /* 0x09 */
    u8 attack;                  /* 0x0a */
    u8 defense;                 /* 0x0b */
    u8 agility;                 /* 0x0c */
    u8 luck;                    /* 0x0d */
};

struct StatWork {
    s32 hp;                     /* 0x00 */
    s32 pp;                     /* 0x04 */
    s32 attack;                 /* 0x08 */
    s32 defense;                /* 0x0c */
    s32 agility;                /* 0x10 */
    s32 unused_14;
    s32 luck;                   /* 0x18 */
    s32 turns;                  /* 0x1c */
    s32 stat_20;                /* 0x20 */
    s32 stat_24;                /* 0x24 */
    s32 element[4][2];          /* 0x28 */
    s32 kind;                   /* 0x48 */
    s32 unused_4c[2];
    s32 amount;                 /* 0x54 */
    struct ItemDefinition *item; /* 0x58 */
    s32 unused_5c;
};

void *Runtime_BumpAllocateAlternatePool(s32 size);
void Sys_Free(void *buffer);
struct OwnerStats *Owner_GetState(s32 owner);
struct ClassRecord *Owner_GetRecordStride84(s32 class_id);
struct DjinnDefinition *Djinn_GetDefinition(s32 element, s32 djinn);
s32 __divsi3(s32 numerator, s32 denominator);

/* Distance between a stored value and one recomputed from its ratio. */
#define STAT_DIFF(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

/* Rebuilds a party member's derived statistics from the base values,
   equipped items, set Djinn, class multipliers and levels, clamps them and
   rescales current HP and PP to the new maxima. */
void Owner_RecalculateStats(s32 owner)
{
    struct StatWork *work;
    struct OwnerStats *st;
    s32 i, j, el;
    s32 value;
    s32 flag;
    s32 kind;
    s32 amount;
    s32 hp_cap;
    s32 pp_cap;
    s16 old;

    work = Runtime_BumpAllocateAlternatePool(sizeof(struct StatWork));
    st = Owner_GetState(owner);
    {
        /* FAKEMATCH: preserve native count/pointer lifetimes and copy-load/store scheduling; the empty asm boundaries keep the original values. */
        /* FAKEMATCH: retain the native operand register in this bounded lifetime experiment; the full output still differs. */
        register s32 cnt asm("r5");
        /* FAKEMATCH: retain the native operand register in this bounded lifetime experiment; the full output still differs. */
        register s16 *src asm("r1");
        /* FAKEMATCH: retain the native operand register in this bounded lifetime experiment; the full output still differs. */
        register s32 *dst asm("r2");
        s32 tmp;
        tmp = st->base_hp;
        cnt = 3;
        /* FAKEMATCH: this source boundary or scalar construction tests native statistic-copy and bonus scheduling; the full output still differs. */
        asm("" : "+r"(tmp), "+r"(cnt));
        work->hp = tmp;
        tmp = st->base_pp;
        src = (s16 *)st;
        /* FAKEMATCH: this source boundary or scalar construction tests native statistic-copy and bonus scheduling; the full output still differs. */
        asm("" : "+r"(tmp), "+r"(src));
        work->pp = tmp;
        src += 18;
        /* FAKEMATCH: this source boundary or scalar construction tests native statistic-copy and bonus scheduling; the full output still differs. */
        asm("" : : "r"(src));
        work->attack = st->base_attack;
        work->defense = st->base_defense;
        work->agility = st->base_agility;
        work->luck = st->base_luck;
        work->turns = st->base_turns & 15;
        tmp = st->base_20;
        dst = (s32 *)work;
        /* FAKEMATCH: this source boundary or scalar construction tests native statistic-copy and bonus scheduling; the full output still differs. */
        asm("" : "+r"(tmp), "+r"(dst));
        work->stat_20 = tmp;
        tmp = st->base_21;
        dst += 10;
        /* FAKEMATCH: this source boundary or scalar construction tests native statistic-copy and bonus scheduling; the full output still differs. */
        asm("" : "+r"(tmp), "+r"(dst));
        work->stat_24 = tmp;
        do {
            tmp = src[0];
            cnt--;
            /* FAKEMATCH: this source boundary or scalar construction tests native statistic-copy and bonus scheduling; the full output still differs. */
            asm("" : "+r"(tmp), "+r"(cnt));
            dst[0] = tmp;
            tmp = src[1];
            src += 2;
            /* FAKEMATCH: this source boundary or scalar construction tests native statistic-copy and bonus scheduling; the full output still differs. */
            asm("" : "+r"(tmp), "+r"(src));
            dst[1] = tmp;
            dst += 2;
        } while (cnt >= 0);
    }
    if (STAT_DIFF(st->max_hp * st->hp_ratio / 0x4000, st->hp) > 1
        || STAT_DIFF(st->max_pp * st->pp_ratio / 0x4000, st->pp) > 1) {
        st->hp_ratio = 0x4000;
        st->pp_ratio = 0x4000;
        st->hp = st->max_hp;
        st->pp = st->max_pp;
    }

    st->curse &= ~3;
    if (st->curse & 4)
        st->curse |= 1;
    if (st->extra_turn)
        work->turns++;
    st->bonus_142 = 0;

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
                work->attack += work->item->primary_bonus;
            } while (0);
            work->defense += work->item->secondary_bonus;
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
                    work->pp += work->amount;
                    break;
                case 4:
                    work->stat_24 += work->amount;
                    break;
                case 5:
                    work->agility += work->amount;
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
                    struct DjinnDefinition *djinn = Djinn_GetDefinition(el, i);
                    /* FAKEMATCH: bound the live element reload around the first Djinn accumulation. */
                    /* FAKEMATCH: this source boundary or scalar construction tests native statistic-copy and bonus scheduling; the full output still differs. */
                    asm("" : : "r"(el));

                    work->hp += djinn->hp;
                    work->pp += djinn->pp;
                    work->attack += djinn->attack;
                    work->defense += djinn->defense;
                    work->agility += djinn->agility;
                    work->luck += djinn->luck;
                }
            }
        }

        {
            struct ClassRecord *class = Owner_GetRecordStride84(st->class_id);

            work->hp = __divsi3(work->hp * class->hp, 10);
            work->pp = __divsi3(work->pp * class->pp, 10);
            work->attack = __divsi3(work->attack * class->attack, 10);
            work->defense = __divsi3(work->defense * class->defense, 10);
            work->agility = __divsi3(work->agility * class->agility, 10);
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
                kind -= 7; /* rate effects 7..14 scale a statistic in tenths */
                work->amount = amount;
                switch (kind) {
                case 0:
                    work->hp = __divsi3(work->hp * work->amount, 10);
                    break;
                case 1:
                    work->stat_20 = __divsi3(work->stat_20 * work->amount, 10);
                    break;
                case 2:
                    work->pp = __divsi3(work->pp * work->amount, 10);
                    break;
                case 3:
                    work->stat_24 = __divsi3(work->stat_24 * work->amount, 10);
                    break;
                case 4:
                    work->attack = __divsi3(work->attack * work->amount, 10);
                    break;
                case 5:
                    work->defense = __divsi3(work->defense * work->amount, 10);
                    break;
                case 6:
                    work->agility = __divsi3(work->agility * work->amount, 10);
                    break;
                case 7:
                    work->luck = __divsi3(work->luck * work->amount, 10);
                    break;
                }
            }
        }
    }

    /* FAKEMATCH: preserve the native scaled-statistic store before loading the next level field. */
    work->attack = work->attack * (st->attack_level + 8) / 8;
    /* FAKEMATCH: this source boundary or scalar construction tests native statistic-copy and bonus scheduling; the full output still differs. */
    asm("" : : : "memory");
    work->defense = work->defense * (st->defense_level + 8) / 8;
    /* FAKEMATCH: this source boundary or scalar construction tests native statistic-copy and bonus scheduling; the full output still differs. */
    asm("" : : : "memory");
    work->agility = work->agility * (st->agility_level + 8) / 8;
    for (i = 0; i < 4; i++)
        work->element[i][0] += (st->element_level[i] * st->element_level[i] + st->element_level[i]) * 5;
    for (i = 0; i < 4; i++)
        work->element[i][1] += st->resist_level * 20;

    if (st->class_id) {
        flag = 0;
        /* FAKEMATCH: case order retains the native tail-merging of repeated elemental flag tests. */
        switch (st->bonus_flag_group) {
        case 1:
            flag = GameFlag_Test(0x112);
            break;
        case 0:
            flag = GameFlag_Test(0x110);
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
        case 4:
            flag = GameFlag_Test(0x110);
            break;
        case 6:
            flag = GameFlag_Test(0x113);
            break;
        case 7:
            flag = GameFlag_Test(0x111);
            break;
        }
        if (flag)
            work->stat_24 += 4;
    }

    if (work->attack < 0)
        work->attack = 0;
    if (work->attack > 999)
        work->attack = 999;
    if (work->defense < 0)
        work->defense = 0;
    if (work->defense > 999)
        work->defense = 999;
    if (work->agility < 0)
        work->agility = 0;
    if (work->agility > 999)
        work->agility = 999;
    if (work->luck < 0)
        work->luck = 0;
    if (work->luck > 99)
        work->luck = 99;
    if (st->class_id == 0) {
        if (work->turns < 0)
            work->turns = 0;
        if (work->turns > 4)
            work->turns = 4;
    } else {
        if (work->turns < 0)
            work->turns = 0;
        if (work->turns > 2)
            work->turns = 2;
    }
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

    {
        /* FAKEMATCH: retain native elemental-copy register lifetimes and scalar-store scheduling with empty source boundaries. */
        /* FAKEMATCH: retain the native operand register in this bounded lifetime experiment; the full output still differs. */
        register s32 cnt asm("r5");
        /* FAKEMATCH: retain the native operand register in this bounded lifetime experiment; the full output still differs. */
        register s16 *dst asm("r1");
        /* FAKEMATCH: retain the native operand register in this bounded lifetime experiment; the full output still differs. */
        register s32 *src asm("r2");
        s32 tmp;
        tmp = work->attack;
        dst = (s16 *)st;
        /* FAKEMATCH: this source boundary or scalar construction tests native statistic-copy and bonus scheduling; the full output still differs. */
        asm("" : "+r"(tmp), "+r"(dst));
        st->attack = tmp;
        dst += 36;
        /* FAKEMATCH: this source boundary or scalar construction tests native statistic-copy and bonus scheduling; the full output still differs. */
        asm("" : : "r"(dst));
        tmp = work->defense;
        /* FAKEMATCH: this source boundary or scalar construction tests native statistic-copy and bonus scheduling; the full output still differs. */
        asm("mov %1, #3" : "+r"(tmp), "=r"(cnt));
        st->defense = tmp;
        st->agility = work->agility;
        {
            u8 *bp = &st->luck;
            *bp = work->luck;
            bp++;
            /* FAKEMATCH: this source boundary or scalar construction tests native statistic-copy and bonus scheduling; the full output still differs. */
            asm("" : : "r"(bp));
            *bp = work->turns;
        }
        st->stat_44 = work->stat_20;
        tmp = work->stat_24;
        st->stat_45 = tmp;
        /* FAKEMATCH: this source boundary or scalar construction tests native statistic-copy and bonus scheduling; the full output still differs. */
        asm("" : : "m"(st->stat_45));
        src = (s32 *)work;
        src += 10;
        do {
            tmp = src[0];
            cnt--;
            /* FAKEMATCH: this source boundary or scalar construction tests native statistic-copy and bonus scheduling; the full output still differs. */
            asm("" : "+r"(tmp), "+r"(cnt));
            dst[0] = tmp;
            tmp = src[1];
            src += 2;
            /* FAKEMATCH: this source boundary or scalar construction tests native statistic-copy and bonus scheduling; the full output still differs. */
            asm("" : "+r"(tmp), "+r"(src));
            dst[1] = tmp;
            dst += 2;
        } while (cnt >= 0);
    }
    if (st->class_id == 0) {
        hp_cap = 16383;
        pp_cap = 9999;
    } else {
        hp_cap = 1999;
        pp_cap = hp_cap;
    }

    old = st->max_hp;
    if (work->hp < 0)
        work->hp = 0;
    if (work->hp > hp_cap)
        work->hp = hp_cap;
    st->max_hp = work->hp;
    if (old != st->max_hp) {
        value = work->hp * st->hp_ratio / 0x4000;
        if (value < 0)
            value = 0;
        if (value > hp_cap)
            value = hp_cap;
        if (st->hp != 0 && value == 0)
            value = 1;
        st->hp = value;
    }

    old = st->max_pp;
    if (work->pp < 0)
        work->pp = 0;
    if (work->pp > pp_cap)
        work->pp = pp_cap;
    st->max_pp = work->pp;
    if (old != st->max_pp) {
        value = work->pp * st->pp_ratio / 0x4000;
        if (value < 0)
            value = 0;
        if (value > pp_cap)
            value = pp_cap;
        if (st->pp != 0 && value == 0)
            value = 1;
        st->pp = value;
    }

    Sys_Free(work);
}
