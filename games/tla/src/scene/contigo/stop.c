#include "contigo.h"

extern u16 Data_02006000;
extern u16 Data_02006008;
extern u16 Data_0200600c;
extern u16 *Data_02006010;
extern u16 Data_0200601c;
extern u16 Data_02006020;

s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
s32 Func_020001d4(s16 *list);
void Func_020001f8(void);

void Func_020000f4(void)
{
    Data_02006020 = 0;
    Data_0200601c = Func_020001d4(Data_02006010);
    Data_0200600c = 15;
    Data_02006008 = 16;
    Data_02006000 = 1;
    Scheduler_AddOrUpdateCallback(Func_020001f8, 0x480);
}

void Func_02000158(void)
{
    struct EventWork *work = Ram_HeapSlots->eventWork;

    work->screenEffect = 256;
    Scheduler_RemoveCallbackFar(Func_020001f8);
}

void Func_02000178(void)
{
    Data_02006020 = 0;
    Data_0200601c = Func_020001d4(Data_02006010);
    Data_0200600c = 2;
    Data_02006008 = 2;
    Data_02006000 = 1;
    Scheduler_AddOrUpdateCallback(Func_020001f8, 0x480);
}
