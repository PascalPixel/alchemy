#include "types.h"
#include "scene.h"
#include "abi/battle/placement/update_timed_entries_twenty_times.h"

s32 BattlePlacement_UpdateTimedEntriesTwentyTimes(void)
{
    s32 cnt;

    cnt = 0x13;
    do {
        cnt -= 1;
        Battle_Check();
    } while (cnt >= 0);
    return 0;
}
