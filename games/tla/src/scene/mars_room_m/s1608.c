#include "mars_room_m.h"

void Func_02001608(void)
{
    Data_02003938 = 0;
    Data_02003934 = Func_020016e8(Data_02003928);
    Data_02003924 = 15;
    Data_02003920 = 16;
    Data_02003918 = 1;
    Value2((s32 (*)())Scheduler_AddOrUpdateCallback, (s32)Func_0200170c, 0x480);
}
