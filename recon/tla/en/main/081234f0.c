#include "TYPES.H"
#include "SYSTEM.H"
extern u8 Data_03001e74[];

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Scheduler_RemoveCallback(void *);
s32 BattleEventRuntime_Reset(void);
void BattleEvent_Playback(void);

s32 BattleEventRuntime_WaitForReady(void)
{
    s32 state;
    void *runtime;

    runtime = *(void **)((u32)&Data_03001e74);
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
    Scheduler_RemoveCallback((void *)BattleEvent_Playback);
    return BattleEventRuntime_Reset();
}
