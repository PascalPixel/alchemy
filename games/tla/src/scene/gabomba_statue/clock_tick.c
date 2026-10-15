#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"

struct ClockWork {
    u8 unk00[132];
    s32 timer;
    u8 ringing;
    u8 mode;
};

extern u8 gData_020023c4[];
void Func_02005854(s32 sound);
void Func_02005090(void);
s32 Math_Cosine(s32 angle);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Func_02005210(void);
void Func_0200525c(void);
s32 __divsi3(s32 numerator, s32 denominator);
s32 __modsi3(s32 numerator, s32 denominator);

void Func_02005210(void)
{
    u8 *event = Ram_HeapSlots->eventWork;
    struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);

    if (*(s16 *)(event + 358) == 0 && *(s16 *)(event + 346) == 0 && *(s16 *)(event + 350) == 0
        && clock->timer > 0)
        clock->timer--;
    Func_02005090();
}

void Func_0200525c(void)
{
    u8 *event = Ram_HeapSlots->eventWork;
    struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);

    if (*(s16 *)(event + 358) == 0 && *(s16 *)(event + 346) == 0 && *(s16 *)(event + 350) == 0
        && clock->timer > 0)
        clock->timer--;
}
