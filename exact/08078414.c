#include "item.h"

extern struct ItemDefinition Data_0807b6a8[];

struct ItemDefinition *Func_08078414(s32 item)
{
    return Data_0807b6a8 + (item & 0x1ff);
}
