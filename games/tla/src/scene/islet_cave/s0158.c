#include "types.h"
#include "ram_buffer.h"

extern u16 gData_02001a14;
extern u16 gData_02001a1c;
extern u16 gData_02001a20;
extern u16 *gData_02001a24;
extern u16 gData_02001a30;
extern u16 gData_02001a34;

s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
s32 Func_020001d4(s16 *list);
void Func_020001f8(void);

void Func_020000f4(void)
{
    gData_02001a34 = 0;
    gData_02001a30 = Func_020001d4(gData_02001a24);
    gData_02001a20 = 15;
    gData_02001a1c = 16;
    gData_02001a14 = 1;
    Scheduler_AddOrUpdateCallback(Func_020001f8, 0x480);
}

void Func_02000158(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
    Scheduler_RemoveCallbackFar(Func_020001f8);
}

void Func_02000178(void)
{
    gData_02001a34 = 0;
    gData_02001a30 = Func_020001d4(gData_02001a24);
    gData_02001a20 = 2;
    gData_02001a1c = 2;
    gData_02001a14 = 1;
    Scheduler_AddOrUpdateCallback(Func_020001f8, 0x480);
}
