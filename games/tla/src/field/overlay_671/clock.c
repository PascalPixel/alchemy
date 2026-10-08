#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"

struct ClockWork {
    u8 unknown_00[132];
    s32 timer;
    u8 ringing;
    u8 mode;
};

extern u8 Data_020023c4[];
void Func_02005854(s32 sound);
void Func_02005090(void);
s32 Math_Cosine(s32 angle);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Func_02005210(void);
void Func_0200525c(void);
s32 __divsi3(s32 numerator, s32 denominator);
s32 __modsi3(s32 numerator, s32 denominator);

void Func_0200533c(void)
{
    struct ClockWork *clock = (struct ClockWork *)(Data_020023c4 + 0x288);

    if (clock->mode)
        Scheduler_RemoveCallbackFar(Func_02005210);
    else
        Scheduler_RemoveCallbackFar(Func_0200525c);
}

void Func_02005364(void)
{
    struct ClockWork *clock = (struct ClockWork *)(Data_020023c4 + 0x288);

    if (clock->mode)
        Scheduler_AddOrUpdateCallback(Func_02005210, 1152);
    else
        Scheduler_AddOrUpdateCallback(Func_0200525c, 1152);
    clock->timer = 3600;
}

s32 Func_020053a0(void)
{
    return ((struct ClockWork *)(Data_020023c4 + 0x288))->timer;
}

void Func_020053ac(void)
{
    ((struct ClockWork *)(Data_020023c4 + 0x288))->timer = 3600;
}
