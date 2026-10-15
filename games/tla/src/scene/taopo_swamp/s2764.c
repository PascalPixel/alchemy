#include "local.h"

/* The accessors step the work pointer by whole entries and then take entry 0: the bytes add the base to index * 20 before the field offset. */
#define VENT_AT(work, index) \
    ((struct VentWork *)((u8 *)(work) + (index) * sizeof(struct VentEntry)))

void Func_02002764(s32 index, s32 value)
{
    struct VentWork *work;

    do
    {
        work = VENT_WORK;
    } while (0);
    VENT_AT(work, index)->entries[0].value = value;
}
