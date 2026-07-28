#include "item.h"

struct ItemDefinition *Func_08078414(s32 item)
{
    return (struct ItemDefinition *)0x0807B6A8 + (item & 0x1ff);
}
