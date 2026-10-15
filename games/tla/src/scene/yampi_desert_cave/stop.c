#include "yampi_desert_cave.h"

void Scheduler_RemoveCallbackFar(void (*callback)(void));

void Func_02000898(void)
{
    Data_02001fe8 = 0;
    Data_02001fe4 = Func_02000978(Data_02001fd8);
    Data_02001fd4 = 15;
    Data_02001fd0 = 16;
    Data_02001fc8 = 1;
    Scheduler_AddOrUpdateCallback(Func_0200099c, 0x480);
}

void Func_020008fc(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
    Scheduler_RemoveCallbackFar(Func_0200099c);
}

void Func_0200091c(void)
{
    Data_02001fe8 = 0;
    Data_02001fe4 = Func_02000978(Data_02001fd8);
    Data_02001fd4 = 2;
    Data_02001fd0 = 2;
    Data_02001fc8 = 1;
    Scheduler_AddOrUpdateCallback(Func_0200099c, 0x480);
}
