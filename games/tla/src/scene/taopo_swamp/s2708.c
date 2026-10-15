#include "local.h"

/* The accessors step the work pointer by whole entries and then take entry 0: the bytes add the base to index * 20 before the field offset. */
#define VENT_AT(work, index) \
    ((struct VentWork *)((u8 *)(work) + (index) * sizeof(struct VentEntry)))

s32 Func_02002708(s32 index)
{
    struct VentWork *work;

    do
    {
        work = VENT_WORK;
    } while (0);
    return VENT_AT(work, index)->entries[0].active;
}

void Func_0200271c(s32 index, s32 active)
{
    struct VentWork *work;

    do
    {
        work = VENT_WORK;
    } while (0);
    VENT_AT(work, index)->entries[0].active = active;
}
