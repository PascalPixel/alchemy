#include "TYPES.H"
#include "SCENE.H"
#include "IWRAM_CALL.H"
s32 Func_08009268(void *, s32 *);

/* battle/effects/data/lookup_result.c */
typedef struct Entry {
    s16 first;
    s16 second;
    s16 third;
    s16 result;
} Entry;

extern Entry BattleFx_ResultRules[];

s32 GameFlag_TestFar(s32 flag);
void BattleFx_SelectResultPointer(s32 arg0);

s32 BattleFx_LookupResult(void *arg0)
{
    s32 value;
    Entry *entry = BattleFx_ResultRules;
    s32 key = Func_08009268(arg0, &value);
    s32 result = 0;

    while (entry->first != -1) {
        if (entry->first == value &&
            (entry->second == -1 || entry->second == key) &&
            (entry->third == -1 || GameFlag_TestFar(entry->third) == 0)) {
            result = entry->result;
            break;
        }
        entry++;
    }
    BattleFx_SelectResultPointer(key);
    return result;
}

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

struct EncounterZone {
    u16 rate;
    u16 level;
    u16 enemies[8];
    u8 weights[8];
};

struct FieldPartyState {
    u8 unknown_000[0x238];
    s32 encounter_steps;
    u8 unknown_23c[8];
    s32 encounters_off;
    u8 unknown_248[4];
    s16 no_encounters;
};

extern u8 *gEventWork;
extern struct FieldPartyState gGameState;
extern struct EncounterZone Encounter_EnemyGroupTable[];

s32 GameFlag_IsSet(s32 flag);
u8 *Owner_GetStateFar(s32 unit);
s32 Party_GetAverageLevelFar(void);
u32 Random16(void);
void BattleFx_SelectBattleCue(s32 zone);

s32 Encounter_SelectEnemyGroup(s32 zone, s32 steps)
{
    u8 *work;
    struct EncounterZone *entry;
    s32 rate;
    s32 level;
    s32 bias;
    s32 step;
    s32 total;
    s32 sum;
    s32 pick;
    s32 i;

    work = gEventWork;
    if (GameFlag_IsSet(0x15f) != 0)
        goto encounter;
    if (GameFlag_IsSet(0x160) != 0 || GameFlag_IsSet(0x161) != 0)
        return 0;
    if (zone == 0)
        return 0;
    if (gGameState.no_encounters != 0)
        return 0;
    entry = &Encounter_EnemyGroupTable[zone];
    rate = entry->rate;
    if (rate == 0)
        return 0;
    if (GameFlag_IsSet(5) != 0) {
        level = *(s32 *)(Owner_GetStateFar(5) + 292);
        if (level > 130)
            return 0;
    }
    level = Party_GetAverageLevelFar() - entry->level;
    if (level < 0)
        level = 0;
    if (level > 5)
        level = 5;
    if (level > 0 && gGameState.encounters_off != 0)
        return 0;
    rate += level * 5;
    bias = *(s32 *)(work + 0x1a8);
    if (bias == 0) {
        s32 a = Random16();
        s32 b = Random16();
        s32 c = Random16();

        bias = (a - b + c - (s32)Random16()) / 2;
        *(s32 *)(work + 0x1a8) = bias;
    }
    step = Iwram_RatioMulQ14((rate << 20) + ((rate << 4) - 16) * bias, 0x100000);
    total = gGameState.encounter_steps += Iwram_MulQ16(step, steps);
    if (total < *(s32 *)(work + 0x1ac))
        return 0;
encounter:
    *(s32 *)(work + 0x1a8) = 0;
    sum = 0;
    for (i = 0; i < 8; i++)
        sum += entry->weights[i];
    if (sum == 0)
        return 0;
    pick = (u32)(Random16() * sum) >> 16;
    pick -= entry->weights[0];
    i = 0;
    if (pick >= 0) {
        do {
            i++;
            if (i > 7)
                break;
            pick -= entry->weights[i];
        } while (pick >= 0);
    }
    sum = entry->enemies[i];
    BattleFx_SelectBattleCue(zone);
    return sum;
}
#endif
