#include "types.h"
#include "scene.h"

/* item/call_handler48.c */
s32 Item_CallHandler48(s32 arg0, s32 arg1)
{
    Item_Check(arg1);
    return 0;
}

/* party/adjust_byte205_by_direction.c */
extern u8 gCell[];

void Party_AdjustByte205ByDirection(s32 arg0)
{
    u8 value = gCell[0x205];
    if (arg0 & 0x20)
        value += 0xff;
    else
        value += 1;
    gCell[0x205] = value;
}
