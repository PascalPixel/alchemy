#include "types.h"
#include "callback_scheduler.h"

extern u16  gData_02004434;
extern u16  gData_0200443c;
extern u16  gData_02004440;
extern u16 *gData_02004444;
extern u16  gData_02004450;
extern u16  gData_02004454;

void Func_020001f8(void);

s32 Func_020001d4(u16 *list);

void Func_02000178(void)
{
    gData_02004454 = 0;
    gData_02004450 = Func_020001d4(gData_02004444);
    gData_02004440 = 2;
    gData_0200443c = 2;
    gData_02004434 = 1;
    Scheduler_AddOrUpdateCallback(Func_020001f8, 0x480);
}
