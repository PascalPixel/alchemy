#include "TYPES.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#include "BATTLE_RUNTIME.H"

/*
 * Groups of battle keys, each group led by its cue with bit 15 set, ended by
 * zero. A key is (first << 4) + second; a key missing from the table takes
 * the last group's cue.
 */
extern const u16 gBattleCueTable[];

void BattleFx_SelectBattleCue(s32 first, s32 second)
{
    u16 key = (first << 4) + second;
    const u16 *tbl = gBattleCueTable;
    u16 cue;
    u16 value;

    if (BattleFlag_Test(0x16c)) {
        cue = 18;
    } else {
        for (;;) {
            value = *tbl++;
            if (value == 0)
                break;
            if (value == key)
                break;
            if (value & 0x8000)
                cue = value & 0xfff;
        }
    }
    Data_02000240.battle_cue = cue;
}
