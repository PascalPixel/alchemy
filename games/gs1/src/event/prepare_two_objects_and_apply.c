#include "types.h"

#define Event_PrepareTwoObjectsAndApply Func_080917f4

s32 Func_08077150(void *event);
s32 Func_08015428(void *first, void *second);

void Event_PrepareTwoObjectsAndApply(void *first, void *second)
{
    Func_08077150(first);
    Func_08077150(second);
    Func_08015428(first, second);
}
