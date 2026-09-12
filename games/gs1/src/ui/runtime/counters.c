#include "types.h"
#include "gs1_edition.h"
#include "global_cells.h"

struct UiCounterWork {
    u8 unknown_000[0xea8];
    u16 ten;
    u16 one;
    u16 zero;
    u16 fifteen;
    u8 unknown_eb0[0x400];
    u16 nine;
    u16 second_zero;
    u8 unknown_12b4[4];
    u16 result;
};

#if defined(GS1_EDITION_JA)
struct UiCounterWorkJa {
    u8 unknown_000[0xf38];
    u16 ten;
    u16 one;
    u16 zero;
    u16 fifteen;
    u8 unknown_f40[0x200];
    u16 nine;
    u16 second_zero;
    u8 unknown_1144[4];
    u16 result;
};
#endif

extern void *Data_03001e8c;
extern u8 Data_0801789d;

s32 Resource_CopyData(s32, s32, s32);
void ScheduleCallbackAfterFrames(void *, s32);
void UiWork_Finalize(struct Work *, s32);

s32 UiWork_IsIdle(void *arg0)
{
    if (*(u16 *)((u8 *)arg0 + 0x16) == 0) {
        if (*(s16 *)((u8 *)arg0 + 0x1a) == 0)
            return 1;
    }
    return 0;
}

void UiWork_ResetCounters(void)
{
    struct UiCounterWork *state = Data_03001e8c;

    state->fifteen = 15;
    state->ten = 10;
    state->nine = 9;
    state->zero = 0;
    state->one = 1;
}

void UiWork_InitCountersWithResourceAndScheduleRefresh(void)
{
    struct UiCounterWork *state = Data_03001e8c;
    s32 size;

    state->result = Resource_CopyData(95, 128 << 6, 0);
    state->nine = 9;
    state->ten = 10;
    state->zero = 0;
    state->fifteen = 15;
    state->second_zero = 0;
    size = 200;
    size <<= 4;
    ScheduleCallbackAfterFrames((void *)0x0801789d, size);
}

void UiWork_InitCountersAndScheduleRefresh(s32 initialize)
{
#if defined(GS1_EDITION_JA)
    struct UiCounterWorkJa *state = Data_03001e8c;
#else
    struct UiCounterWork *state = Data_03001e8c;
#endif
    s32 size;

    if (initialize != 0)
        state->result = Resource_CopyData(95, 128 << 6, 0);
    state->nine = 9;
    state->ten = 10;
    state->zero = 0;
    state->fifteen = 15;
    state->second_zero = 0;
    size = 200;
    size <<= 4;
    ScheduleCallbackAfterFrames(&Data_0801789d, size);
}

void UiWork_FinalizeSharedSlot(void)
{
    struct Work **slot;
    struct Work *work;

    slot = *(struct Work ***)ADDR_03001EE4;
    work = *slot;
    if (work != 0) {
        UiWork_Finalize(work, 1);
        *slot = 0;
    }
}
