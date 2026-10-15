#include "ride.h"

s32 Func_02002418(s32 index)
{
    return ((struct RideRow *)(gData_020023c4 + 0x188 + index * 16))->a;
}

void Func_02002428(s32 index, s16 value)
{
    ((struct RideRow *)(gData_020023c4 + 0x188 + index * 16))->a = value;
}

s32 Func_02002438(void)
{
    return RIDE_WORK->counter;
}

s32 Func_02002444(s32 limit)
{
    struct RideWork *work = RIDE_WORK;

    if (!GameFlag_Test(260))
    {
        if (++work->counter >= limit)
            work->counter = 0;
    }
    return work->counter;
}

void Func_0200246c(s32 index, s32 value)
{
    struct RideWork  *work = RIDE_WORK;
    struct RideEntry *entry = &work->entries[index];

    entry->d = (value & 15) << 16;
    entry->b = value & 0xf0;
}

void Func_0200248c(s32 index, s16 value)
{
    RIDE_WORK->entries[index].c = value;
}

s32 Func_0200249c(s32 index)
{
    return ((struct RideRow *)(gData_020023c4 + 0x188 + index * 16))->b;
}

void Func_020024ac(u32 index, s32 value)
{
    struct RideWork *work = RIDE_WORK;

    if (index <= 3)
        work->slots[index].value = value;
}
