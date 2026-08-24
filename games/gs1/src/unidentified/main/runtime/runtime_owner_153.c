#include "types.h"

extern u8 Data_08011bf5;
void Func_08002dd8(s32);
void ScheduleCallback(void *);

void Func_08011bc8(void)
{
    ScheduleCallback(&Data_08011bf5);
    Func_08002dd8(0x1C);
}
