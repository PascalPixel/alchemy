#include "types.h"
#include "mem.h"

s32 Func_080381c0(s32 unit, void *buffer, s32 mode);

s32 BattlePres_BuildUnitEntries(s32 unit)
{
    void *buffer = Runtime_BumpAllocateAlternatePool(17);
    s32   count = Func_080381c0(unit, buffer, 0);
    s32   result = -1;

    if (count >= 0)
        result = count;
    MemFree(buffer);
    return result;
}
