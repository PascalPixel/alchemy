#include "types.h"
#include "scene.h"
#include "abi/battle/presentation/act/run_entries.h"


s32 WaitFrames(s32);

s32 BattlePres_RunActorEntries(void *tbl)
{
    s32 i;
    s8 n;

    Battle_Apply(0, 0);
    n = FIELD_AT_OFFSET(tbl, s8 *, 1);
    if (n == 0) {
        Battle_Run();
    } else {
        i = 0;
        if (i < (s32)n) {
            do {
                Battle_Apply2(tbl, i);
                Battle_Check();
                i += 1;
            } while (i < (s32)FIELD_AT_OFFSET(tbl, s8 *, 1));
        }
    }
    return WaitFrames(1);
}
