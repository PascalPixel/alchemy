#include "TYPES.H"
#include "GLOBAL_CELLS.H"
extern u8 Data_03001b00[];

void Runtime_SetMainState19(void)
{
    *(s32 *)((u32)&Data_03001b00) = 0x13;
}
