#include "MEM.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"

struct OwnerElementStats {
    s16 power;
    s16 resist;
};

struct OwnerStats {
    s16 max_hp;
    s16 max_ep;
    s16 hp;
    s16 ep;
    u16 attack;
    u16 defense;
    u16 agility;
    u8 luck;
    u8 unknown_0f[5];
    struct OwnerElementStats elements[4];
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

    value = state->stats.max_ep;
    value += __divsi3(levels * 15, 10);
    floor = __divsi3(base->max_ep * 7, 10);
    if (value < floor)
        value = floor;
    if (value > 9999)
        value = 9999;
    state->stats.max_ep = value;

    floor = __divsi3(levels * 123, 10);
    value = state->stats.attack;
    value += floor;
    floor = __divsi3(base->attack * 7, 10);
    if (value < floor)
        value = floor;
    if (value > 999)
        value = 999;
    state->stats.attack = value;

    floor = __divsi3(levels * 33, 10);
    value = state->stats.defense;
    value += floor;
    floor = __divsi3(base->defense * 7, 10);
    if (value < floor)
        value = floor;
    if (value > 999)
        value = 999;
    state->stats.defense = value;

    floor = __divsi3(levels * 51, 10);
    value = state->stats.agility;
    value += floor;
    floor = __divsi3(base->agility * 7, 10);
    if (value < floor)
        value = floor;
    if (value > 999)
        value = 999;
    state->stats.agility = value;

    for (i = 0; i < 4; i++) {
        value = state->stats.elements[i].power + levels * 15;
        floor = __divsi3(base->elements[i].power * 7, 10);
        if (value < floor)
            value = floor;
        if (value > 200)
            value = 200;
        state->stats.elements[i].power = value;
    }

    state->level += levels;
    UnitCalcFar(owner);
    Runtime_BumpFree(base);
}
