/* Draft, not exact (2026-09-24): 155 differing halfwords, 436 of 440 bytes;
   control flow and calls match. Residual: the reference loads the party
   state base (Data_02000240) from the pool at each use where this candidate
   keeps it in r5, and loads the unit field into r0. The goto into the
   encounter pick follows the reference, which reaches it with the zone
   entry unset when flag 0x15f forces an encounter. */

#include "IWRAM_CALL.H"

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

extern u8 *Data_03001ebc;
extern struct FieldPartyState Data_02000240;

s32 Func_080770c0(s32 flag);
u8 *Func_08077008(s32 unit);
s32 Func_080772c8(void);
u32 Random16(void);
void BattleFx_SelectBattleCue(s32 zone);

s32 Func_0808ae74(s32 zone, s32 steps)
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
    u8 *weight;

    work = Data_03001ebc;
    if (Func_080770c0(0x15f) != 0)
        goto encounter;
    if (Func_080770c0(0x160) != 0 || Func_080770c0(0x161) != 0)
        goto none;
    if (zone == 0)
        return 0;
    if (Data_02000240.no_encounters != 0)
        return 0;
    entry = &((struct EncounterZone *)0x0809c610)[zone];
    rate = entry->rate;
    if (rate == 0)
        return 0;
    if (Func_080770c0(5) != 0) {
        if (*(s32 *)(Func_08077008(5) + 292) > 130)
            goto none;
    }
    level = Func_080772c8() - entry->level;
    if (level < 0)
        level = 0;
    if (level > 5)
        level = 5;
    if (level > 0 && Data_02000240.encounters_off != 0) {
none:
        return 0;
    }
    rate += level * 5;
    bias = *(s32 *)(work + 0x1a8);
    if (bias == 0) {
        s32 a = Random16();
        s32 b = Random16();
        s32 c = Random16();

        bias = (a - b + c - (s32)Random16()) / 2;
        *(s32 *)(work + 0x1a8) = bias;
    }
    step = ((s32 (*)(s32, s32))0x0300013c)((rate << 20) + ((rate << 4) - 16) * bias, 0x100000);
    total = Data_02000240.encounter_steps += Iwram_MulQ16(step, steps);
    if (total < *(s32 *)(work + 0x1ac))
        return 0;
encounter:
    *(s32 *)(work + 0x1a8) = 0;
    sum = 0;
    {
        u8 *each = entry->weights;

        for (i = 7; i >= 0; i--)
            sum += *each++;
    }
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
