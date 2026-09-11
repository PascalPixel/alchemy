#include "types.h"
#include "scene.h"
#include "abi/battle/event_runtime/wait_for_ready.h"
#include "global_cells.h"


void WaitFrames(u32);
void ScheduleCallback(void *);
s32 BattleEventRuntime_Reset(void);

s32 BattleEventRuntime_WaitForReady(void)
{
    s32 state;
    void *runtime;

    runtime = *(void **)ADDR_03001E74;
    state = FIELD_AT_OFFSET(runtime, s32 *, 0x800);
    if (state == 0) {
        FIELD_AT_OFFSET(runtime, s32 *, 0x800) = 1;
        state = 1;
    }
    if (state != 4) {
        do {
            WaitFrames(1U);
        } while (FIELD_AT_OFFSET(runtime, s32 *, 0x800) != 4);
    }
    ScheduleCallback((void *)Battle_Run);
    return BattleEventRuntime_Reset();
}
