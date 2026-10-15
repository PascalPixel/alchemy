#include "types.h"
#include "callback_scheduler.h"

extern u16 Data_02004434;
extern u16 Data_0200443c;
extern u16 Data_02004440;
extern u16 *Data_02004444;
extern u16 Data_02004450;
extern u16 Data_02004454;

s32 Func_020001d4(u16 *list);
void Func_020001f8(void);

void Func_020000f4(void)
{
    Data_02004454 = 0;
    Data_02004450 = Func_020001d4(Data_02004444);
    Data_02004440 = 15;
    Data_0200443c = 16;
    Data_02004434 = 1;
    Scheduler_AddOrUpdateCallback(Func_020001f8, 0x480);
}
