#include "room.h"

extern u16 Data_0200687c;
extern u16 Data_02006884;
extern u16 Data_02006898;
extern u16 Data_0200689c;
extern u16 Data_0200e888;
extern u16 *Data_0200e88c;

s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
s32 Func_02004794(u16 *list);
void Func_020047b8(void);

void Func_02004738(void)
{
    Data_0200689c = 0;
    Data_02006898 = Func_02004794(Data_0200e88c);
    Data_0200e888 = 2;
    Data_02006884 = 2;
    Data_0200687c = 1;
    Scheduler_AddOrUpdateCallback(Func_020047b8, 0x480);
}
