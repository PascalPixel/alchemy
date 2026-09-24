#include "TYPES.H"

struct OwnerLevelState {
    u8 unknown_000[0x0f];
    u8 level;                   /* 0x0f */
    u16 base_hp;                /* 0x10 */
    u16 base_pp;                /* 0x12 */
    u8 unknown_014[4];
    u16 base_attack;            /* 0x18 */
    u16 base_defense;           /* 0x1a */
    u16 base_agility;           /* 0x1c */
    u8 base_luck;               /* 0x1e */
    u8 base_turns;              /* 0x1f */
    u8 base_20;                 /* 0x20 */
    u8 base_21;                 /* 0x21 */
    u8 unknown_022[0x102];
    u32 experience;             /* 0x124 */
    u8 character;               /* 0x128 */
    u8 class_id;                /* 0x129 */
};

/* Growth record: each statistic at levels 0, 20, 40, 60, 80 and 100. */
struct OwnerGrowth {
    u8 unknown_00[0x50];
    s16 hp[6];                  /* 0x50 */
    s16 pp[6];                  /* 0x5c */
    u16 attack[6];              /* 0x68 */
    u16 defense[6];             /* 0x74 */
    u16 agility[6];             /* 0x80 */
    u8 luck[6];                 /* 0x8c */
};

/* What a level gained, for the level-up message. */
struct LevelUpResult {
    s16 level;
    u16 ability;
    u16 hp;
    u16 pp;
    u16 attack;
    u16 defense;
    u16 agility;
    u16 luck;
};

struct LevelUpWork {
    s32 class_id;
    s32 level;
    struct OwnerGrowth *growth;
    u8 unused_0c[0x20];
};

void *Runtime_BumpAllocateAlternatePool(s32 size);
void Runtime_BumpFree(void *buffer);
struct OwnerLevelState *Owner_GetState(s32 owner);
u32 Owner_GetLevelThreshold(s32 owner, s32 level);
struct OwnerGrowth *Owner_GetRecordStride180(s32 owner);
s32 Owner_RefreshClassActions(s32 owner);
void Owner_RecalculateStats(s32 owner);
u32 Random16(void);
s32 Math_Div(s32 numerator, s32 denominator);
u32 Math_DivU(u32 numerator, u32 denominator);

/* Raises a party member one level: the gain of each statistic is the
   growth between the two surrounding twenty-level marks, spread over twenty
   levels with a random remainder. Level 99 is the cap. */
struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *res)
{
    struct OwnerLevelState *st;
    struct LevelUpWork *work;
    u32 threshold;
    s16 band;
    s32 diff;
    s32 level;

    st = Owner_GetState(owner);
    work = Runtime_BumpAllocateAlternatePool(sizeof(struct LevelUpWork));
    work->class_id = st->class_id;
    level = st->level;
    work->level = level;
    res->level = level;
    res->ability = 0xffff;
    res->hp = 0;
    res->pp = 0;
    res->attack = 0;
    res->defense = 0;
    res->agility = 0;
    res->luck = 0;
    if (level < 99) {
        st->level++;
        res->level = level + 1;
        threshold = Owner_GetLevelThreshold(owner, st->level);
        if (threshold != (u32)-1 && st->experience < threshold)
            st->experience = threshold;
        work->growth = Owner_GetRecordStride180(owner);
        if (res->level == 1) {
            res->hp += work->growth->hp[0];
            res->pp += work->growth->pp[0];
            res->attack += work->growth->attack[0];
            res->defense += work->growth->defense[0];
            res->agility += work->growth->agility[0];
            res->luck += work->growth->luck[0];
        }
        band = Math_Div(res->level, 20);
        if (band < 0)
            band = 0;
        if (band > 4)
            band = 4;
        diff = work->growth->hp[band + 1] - work->growth->hp[band];
        res->hp += Math_DivU((Random16() * 20 >> 16) + diff, 20);
        diff = work->growth->pp[band + 1] - work->growth->pp[band];
        res->pp += Math_DivU((Random16() * 20 >> 16) + diff, 20);
        diff = work->growth->attack[band + 1] - work->growth->attack[band];
        res->attack += Math_DivU((Random16() * 20 >> 16) + diff, 20);
        diff = work->growth->defense[band + 1] - work->growth->defense[band];
        res->defense += Math_DivU((Random16() * 20 >> 16) + diff, 20);
        diff = work->growth->agility[band + 1] - work->growth->agility[band];
        res->agility += Math_DivU((Random16() * 20 >> 16) + diff, 20);
        diff = work->growth->luck[band + 1] - work->growth->luck[band];
        res->luck += Math_DivU((Random16() * 20 >> 16) + diff, 20);
        st->base_hp += res->hp;
        st->base_pp += res->pp;
        st->base_attack += res->attack;
        st->base_defense += res->defense;
        st->base_agility += res->agility;
        st->base_luck += res->luck;
        st->base_turns = 1;
        st->base_20 = 0;
        st->base_21 = 0;
        Owner_RefreshClassActions(owner);
        Owner_RecalculateStats(owner);
    }
    Runtime_BumpFree(work);
    return res;
}
