#include "TYPES.H"
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

extern void *gWindowWork;
void UiWork_ProcessAll(void);

s32 VramBlock_LoadCached(s32, s32, s32);
void Scheduler_AddOrUpdateCallback(void *, s32);
void UiWork_Finalize(struct Work *, s32);

s32 UiWork_IsIdle(void *arg0)
{
#if defined(TBS_EDITION_JA)
    /* The Japanese check counts a missing work as idle. */
    if (arg0 == NULL)
        return 1;
#endif
    if (*(u16 *)((u8 *)arg0 + 0x16) == 0) {
        if (*(s16 *)((u8 *)arg0 + 0x1a) == 0)
            return 1;
    }
    return 0;
}

void UiWork_ResetCounters(void)
{
    struct UiCounterWork *state = gWindowWork;

    state->fifteen = 15;
    state->ten = 10;
    state->nine = 9;
    state->zero = 0;
    state->one = 1;
}

void UiWork_InitCountersWithResourceAndScheduleRefresh(void)
{
    struct UiCounterWork *state = gWindowWork;
    s32 size;

    state->result = VramBlock_LoadCached(95, 128 << 6, 0);
    state->nine = 9;
    state->ten = 10;
    state->zero = 0;
    state->fifteen = 15;
    state->second_zero = 0;
    size = 200;
    size <<= 4;
    Scheduler_AddOrUpdateCallback((void *)UiWork_ProcessAll, size);
}

void UiWork_InitCountersAndScheduleRefresh(s32 initialize)
{
    struct UiCounterWork *state = gWindowWork;
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
    Scheduler_AddOrUpdateCallback(UiWork_ProcessAll, size);
}

void UiWork_FinalizeSharedSlot(void)
{
    struct Work **slot;
    struct Work *work;

    slot = *(struct Work ***)((u32)&Data_03001ee4);
    work = *slot;
    if (work != 0) {
        UiWork_Finalize(work, 1);
        *slot = 0;
    }
}
