#include "types.h"

u8 *Item_Get(s32 item);
s32 Func_08100d58(s32 kind);

s32 Func_08100d40(s32 item)
{
    return Func_08100d58(*(u16 *)(Item_Get(item) + 40) & 0x3fff);
}
