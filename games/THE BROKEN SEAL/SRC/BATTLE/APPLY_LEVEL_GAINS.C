#include "TYPES.H"

struct OwnerElementStats {
    s16 power;
    s16 resist;
};

struct OwnerStats {
    s16 max_hp;
    s16 max_pp;
    s16 hp;
    s16 pp;
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

typedef s32 (*WordCopyFn)(const void *source, void *destination, s32 size);

void *Runtime_BumpAllocateAlternatePool(s32 size);
struct OwnerState *Owner_GetStateFar(s32 owner);
s32 Math_Div(s32 numerator, s32 denominator);
void Owner_RecalculateStatsFar(s32 owner);
void Runtime_BumpFree(void *block);

/* Raise an owner by levels: each stat grows by a fixed tenth-step per level,
   never below 70% of its value before the change and never past its cap.
   Declared s32 without a return statement, as the ROM keeps r0 live. */
s32 Owner_ApplyLevelGains(s32 owner, s32 levels)
{
    struct OwnerStats *base;
    struct OwnerState *state;
    struct OwnerStats *stats;
    s32 value;
    s32 floor;
    s32 i;

    base = Runtime_BumpAllocateAlternatePool(sizeof(struct OwnerStats));
    state = Owner_GetStateFar(owner);
    stats = &state->stats;
    ((WordCopyFn)0x03001388)(base, stats, sizeof(struct OwnerStats));

    value = stats->max_hp;
    value += Math_Div(levels * 97, 10);
    floor = Math_Div(base->max_hp * 7, 10);
    if (value < floor)
        value = floor;
    if (value > 9999)
        value = 9999;
    stats->max_hp = value;

    value = state->stats.max_pp;
    value += Math_Div(levels * 15, 10);
    floor = Math_Div(base->max_pp * 7, 10);
    if (value < floor)
        value = floor;
    if (value > 9999)
        value = 9999;
    state->stats.max_pp = value;

    floor = Math_Div(levels * 123, 10);
    value = state->stats.attack;
    value += floor;
    floor = Math_Div(base->attack * 7, 10);
    if (value < floor)
        value = floor;
    if (value > 999)
        value = 999;
    state->stats.attack = value;

    floor = Math_Div(levels * 33, 10);
    value = state->stats.defense;
    value += floor;
    floor = Math_Div(base->defense * 7, 10);
    if (value < floor)
        value = floor;
    if (value > 999)
        value = 999;
    state->stats.defense = value;

    floor = Math_Div(levels * 51, 10);
    value = state->stats.agility;
    value += floor;
    floor = Math_Div(base->agility * 7, 10);
    if (value < floor)
        value = floor;
    if (value > 999)
        value = 999;
    state->stats.agility = value;

    for (i = 0; i < 4; i++) {
        value = state->stats.elements[i].power + levels * 15;
        floor = Math_Div(base->elements[i].power * 7, 10);
        if (value < floor)
            value = floor;
        if (value > 200)
            value = 200;
        state->stats.elements[i].power = value;
    }

    state->level += levels;
    Owner_RecalculateStatsFar(owner);
    Runtime_BumpFree(base);
}
