#include "mars_aerie.h"

void Func_02004fac(void)
{
    Data_02006cf0 = 0;
    Data_02006cec = Func_0200508c(Data_02006ce0);
    Data_02006cdc = 15;
    Data_02006cd8 = 16;
    Data_02006cd0 = 1;
    Scheduler_AddOrUpdateCallback(Func_020050b0, 0x480);
}

void Func_02005010(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 428) = 256;
    Scheduler_RemoveCallbackFar(Func_020050b0);
}

void Func_02005030(void)
{
    Data_02006cf0 = 0;
    Data_02006cec = Func_0200508c(Data_02006ce0);
    Data_02006cdc = 2;
    Data_02006cd8 = 2;
    Data_02006cd0 = 1;
    Scheduler_AddOrUpdateCallback(Func_020050b0, 0x480);
}
