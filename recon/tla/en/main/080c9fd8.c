/* Draft, differ 100 of 180 (ported from The Broken Seal's
   Encounter_SelectEnemyGroup; ⚓️ drops the flag 5 level check, reads the
   cloak byte at 0x26d and the encounter fields at 0x194 and 0x198 of the
   event work). The native keeps the party state's address in r5 from the
   cloak test to the avoid test, across Party_GetAverageLevelFar, the zone
   in r0, the rate in r7 and the entry in r9; this draft reloads the party
   address at each use and allocates the rest differently. The flag and zone
   tests are one condition here, as the native shares their return. */
#include "types.h"
#include "iwram_call.h"
#include "party_state.h"
#include "ram_buffer.h"

struct EncounterZone {
    u16 rate;
    u16 level;
    u16 enemies[8];
    u8 weights[8];
};

struct EncounterWork {
    u8 unk000[0x194];
    s32 bias;
    s32 unk198;
    s32 threshold;
};

s32 GameFlag_Test(s32 flag);
s32 Party_GetAverageLevelFar(void);
s32 Random16(void);
void BattleFx_SelectBattleCue(s32 zone, s32 group);
extern const struct EncounterZone Encounter_EnemyGroupTable[];

#define ENCOUNTER_STEPS (*(s32 *)&party->unknown_258[0])

s32 Encounter_SelectEnemyGroup(s32 zone, s32 steps)
{
    struct EncounterWork *work;
    const struct EncounterZone *entry;
    s32 rate;
    s32 level;
    s32 bias;
    s32 step;
    s32 total;
    s32 sum;
    s32 pick;
    s32 i;
    struct PartyState *party = &gPartyState;

    work = Ram_HeapSlots->eventWork;
    if (GameFlag_Test(351) == 0) {
        if (GameFlag_Test(352) != 0 || GameFlag_Test(353) != 0 || zone == 0)
            return 0;
        if ((s8)party->unk26d[0] != 0)
            return 0;
        entry = Encounter_EnemyGroupTable + zone;
        rate = entry->rate;
        if (rate == 0)
            return 0;

        level = Party_GetAverageLevelFar() - entry->level;
        if (level < 0)
            level = 0;
        if (level > 5)
            level = 5;
        if (level > 0 && party->avoidDistance != 0)
            return 0;
        rate += level * 5;

        bias = work->bias;
        if (bias == 0) {
            s32 a = Random16();
            s32 b = Random16();
            s32 c = Random16();

            bias = (a - b + c - (s32)Random16()) / 2;
            work->bias = bias;
        }
        step = Iwram_RatioMulQ14((rate << 20) + ((rate << 4) - 16) * bias, 0x100000);
        total = ENCOUNTER_STEPS += Iwram_MulQ16(step, steps);
        if (total < work->threshold)
            return 0;
    }

    work->bias = 0;
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
    BattleFx_SelectBattleCue(zone, i);
    return sum;
}
