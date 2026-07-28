#include "types.h"

extern u8 Data_02000040[];

u8 Func_080793b8(s32 no)
{
    u32 index;

    index = (u32)no << 20;
    no = (u32)index >> 23;
    return Data_02000040[no];
}
