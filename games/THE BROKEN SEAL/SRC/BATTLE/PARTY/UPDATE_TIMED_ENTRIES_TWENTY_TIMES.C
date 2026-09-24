#include "TYPES.H"

s32 BattlePlacement_UpdateTimedEntries();

s32 BattlePlacement_UpdateTimedEntriesTwentyTimes(void)
{
    s32 cnt;

    cnt = 0x13;
    do {
        cnt -= 1;
        BattlePlacement_UpdateTimedEntries();
    } while (cnt >= 0);
    return 0;
}
