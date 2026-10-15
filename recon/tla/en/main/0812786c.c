#include "mem.h"
#include "types.h"
#include "iwram_call.h"

struct OwnerElementStats {
    s16 pow;
    s16 res;
};

struct OwnerStats {
    s16 max_hp;
    s16 max_pp;
    s16 hp;
    s16 pp;
    u16 atk;
    u16 def;
    u16 agi;
    u8 luck;
    u8 unknown_0f[5];
    struct OwnerElementStats elm[4];
};

struct OwnerState {
    u8 unknown_00[15];
    u8 level;
    struct OwnerStats stats;
};

struct OwnerState *UnitGetFar(s32 owner);
s32 __divsi3(s32 numerator, s32 denominator);
void UnitCalcFar(s32 owner);

s32 Owner_ApplyLevelGains(s32 owner, s32 levels)
{
    struct OwnerStats *base;
    struct OwnerState *state;
    struct OwnerStats *stats;
    s32 value;
    s32 floor;
    s32 i;

    base = Runtime_BumpAllocateAlternatePool(sizeof(struct OwnerStats));
    state = UnitGetFar(owner);
    stats = &state->stats;
    Iwram_CopyWords(base, stats, sizeof(struct OwnerStats));

    value = stats->max_hp;
    value += __divsi3(levels * 97, 10);
    floor = __divsi3(base->max_hp * 7, 10);
    if (value < floor)
        value = floor;
    if (value > 9999)
        value = 9999;
    stats->max_hp = value;

    value = state->stats.max_pp;
    value += __divsi3(levels * 15, 10);
    floor = __divsi3(base->max_pp * 7, 10);
    if (value < floor)
        value = floor;
    if (value > 9999)
        value = 9999;
    state->stats.max_pp = value;

    floor = __divsi3(levels * 123, 10);
    value = state->stats.atk;
    value += floor;
    floor = __divsi3(base->atk * 7, 10);
    if (value < floor)
        value = floor;
    if (value > 999)
        value = 999;
    state->stats.atk = value;

    floor = __divsi3(levels * 33, 10);
    value = state->stats.def;
    value += floor;
    floor = __divsi3(base->def * 7, 10);
    if (value < floor)
        value = floor;
    if (value > 999)
        value = 999;
    state->stats.def = value;

    floor = __divsi3(levels * 51, 10);
    value = state->stats.agi;
    value += floor;
    floor = __divsi3(base->agi * 7, 10);
    if (value < floor)
        value = floor;
    if (value > 999)
        value = 999;
    state->stats.agi = value;

    for (i = 0; i < 4; i++) {
        value = state->stats.elm[i].pow + levels * 15;
        floor = __divsi3(base->elm[i].pow * 7, 10);
        if (value < floor)
            value = floor;
        if (value > 200)
            value = 200;
        state->stats.elm[i].pow = value;
    }

    state->level += levels;
    UnitCalcFar(owner);
    MemFree(base);
}
