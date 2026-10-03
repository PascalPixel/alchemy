#include "EDITION.H"
#include "TYPES.H"
#include "WINDOW.H"
#include "CALLBACK_SCHEDULER.H"
#include "TBS_EDITION.H"
#include "GLOBAL_CELLS.H"
extern u8 Data_03001ee4[];

struct UiCounterWork {
    u8 unknown_000[RENDER_WORD2_OFS];
    u16 ten;
    u16 one;
    u16 zero;
    u16 fifteen;
    /* nine sits just before the render entry count, wherever the edition
       puts it */
    u8 unknown_eb0[RENDER_ENTRY_COUNT_OFS - 2 - (RENDER_WORD2_OFS + 8)];
    u16 nine;
    u16 second_zero;
    u8 unknown_12b4[4];
    u16 result;
};

void UiWork_ProcessAll(void);

s32 VramBlock_LoadCached(s32, s32, s32);

s32 UiWork_IsIdle(void *arg0)
{
#if !EDITION_INTERNATIONAL
    /* The Japanese check counts a missing work as idle. */
    if (arg0 == NULL)
        return 1;
#endif
    if (((struct UiWindow *)arg0)->flags == 0) {
        if (((struct UiWindow *)arg0)->duration == 0)
            return 1;
    }
    return 0;
}

void UiWork_ResetCounters(void)
{
    struct UiCounterWork *state = (struct UiCounterWork *)gWindowWork[0];

    state->fifteen = 15;
    state->ten = 10;
    state->nine = 9;
    state->zero = 0;
    state->one = 1;
}

void UiWork_InitCountersWithResourceAndScheduleRefresh(void)
{
    struct UiCounterWork *state = (struct UiCounterWork *)gWindowWork[0];
    s32 size;

    state->result = VramBlock_LoadCached(95, 128 << 6, 0);
    state->nine = 9;
    state->ten = 10;
    state->zero = 0;
    state->fifteen = 15;
    state->second_zero = 0;
    size = 200;
    size <<= 4;
    Scheduler_AddOrUpdateCallback((s32)((void *)UiWork_ProcessAll), size);
}

void UiWork_InitCountersAndScheduleRefresh(s32 initialize)
{
    struct UiCounterWork *state = (struct UiCounterWork *)gWindowWork[0];
    s32 size;

    if (initialize != 0)
        state->result = VramBlock_LoadCached(95, 128 << 6, 0);
    state->nine = 9;
    state->ten = 10;
    state->zero = 0;
    state->fifteen = 15;
    state->second_zero = 0;
    size = 200;
    size <<= 4;
    Scheduler_AddOrUpdateCallback((s32)(UiWork_ProcessAll), size);
}

void UiWork_FinalizeSharedSlot(void)
{
    struct UiWindow **slot;
    struct UiWindow *work;

    slot = *(struct UiWindow ***)((u32)&Data_03001ee4);
    work = *slot;
    if (work != 0) {
        UiWork_Finalize(work, 1);
        *slot = 0;
    }
}
