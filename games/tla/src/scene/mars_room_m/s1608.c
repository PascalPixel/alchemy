#include "mars_room_m.h"

void Func_02001608(void)
{
    gData_02003938 = 0;
    gData_02003934 = Func_020016e8(gData_02003928);
    gData_02003924 = 15;
    gData_02003920 = 16;
    gData_02003918 = 1;
    Value2((s32 (*)())Scheduler_AddOrUpdateCallback, (s32)Func_0200170c, 0x480);
}
