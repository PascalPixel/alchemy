#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "IWRAM_CALL.H"
#include "EVENTWRK.H"
#include "BATTLE_UNIT.H"

#define BattleFx_SetSpecialFromTable ((void (*)(void))BattleFx_SetSpecialFromTable)

struct BattleLocationRule {
    s16 map;
    s16 room;
    s16 flag : 15;
    s16 shift : 1;
    s16 result;
};

s32 GameFlag_TestFar(s32 flag);


typedef struct Entry {
    s16 first;
    s16 second;
    s16 third;
    s16 result;
} Entry;

extern Entry BattleFx_ResultRules[];

struct EncounterZone {
    u16 rate;
    u16 level;
    u16 enemies[8];
    u8 weights[8];
};

struct FieldPartyState {
    u8 unknown_000[448];
    s16 scene;
    s16 entrance;
    u8 unknown_1c4[116];
    s32 encounter_steps;
    u8 unknown_23c[8];
    s32 encounters_off;
    u8 unknown_248[4];
    s16 no_encounters;
};

extern struct EventWork *gEventWork;
extern struct FieldPartyState gGameState;
extern struct EncounterZone Encounter_EnemyGroupTable[];
struct BattleUnit *Owner_GetStateFar(s32 unit);
u32 Random16(void);
void BattleFx_SelectBattleCue(s32 zone);

void BattleFx_SelectLocationRule(s32 check)
{
    /* FAKEMATCH: the shift temporaries order the zero between the two stores. */
    struct EventWork *work;
    s32 map;
    s32 room;
    u8 *out;
    const struct BattleLocationRule *entry;
    s32 track;
    s16 fade;
    u32 i;

    work = gEventWork;
    map = gGameState.scene;
    room = gGameState.entrance;
    out = work->encounter_groups;
    entry = Battle_LocationRules;
    track = 0;
    fade = 0;
    if (check != 0) {
        while (entry->map != -1) {
            if (entry->map == map && (entry->room == -1 || entry->room == room)) {
                if (entry->flag == -1 || GameFlag_TestFar(entry->flag) == 0) {
                    track = entry->result;
                    fade = entry->shift;
                    break;
                }
            }
            entry++;
        }
    }
    *out++ = 0;
    for (i = 0; i <= 6; i++) {
        *out++ = track;
        if (track != 0)
            track++;
    }
    if (fade != 0) {
        u8 v = work->encounter_groups[1];
        u8 *d = &work->encounter_groups[0];
        u8 zero = 0;

        *d = v;
        work->encounter_groups[1] = zero;
    }
    work->encounter_bias = 0;
    work->encounter_threshold = 0x100000;
    BattleFx_SetSpecialFromTable();
}

s32 BattleFx_LookupResult(void *arg0)
{
    s32 value;
    Entry *entry = BattleFx_ResultRules;
    s32 key = GetWorldMapTerrainBehaviorFar(arg0, &value);
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

s32 Encounter_SelectEnemyGroup(s32 zone, s32 steps)
{
    struct EventWork *work;
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
        level = (s32)Owner_GetStateFar(5)->experience;
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
    bias = work->encounter_bias;
    if (bias == 0) {
        s32 a = Random16();
        s32 b = Random16();
        s32 c = Random16();

        bias = (a - b + c - (s32)Random16()) / 2;
        work->encounter_bias = bias;
    }
    step = Iwram_RatioMulQ14((rate << 20) + ((rate << 4) - 16) * bias, 0x100000);
    total = gGameState.encounter_steps += Iwram_MulQ16(step, steps);
    if (total < work->encounter_threshold)
        return 0;
encounter:
    work->encounter_bias = 0;
    sum = 0;
    for (i = 0; i < 8; i++)
        sum += entry->weights[i];
    if (sum == 0)
        return 0;
    pick = (Random16() * sum) >> 16;
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
