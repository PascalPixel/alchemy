#include "types.h"
  
extern u8 Data_00001bc0[];
extern s32 Func_02002456(s32);
extern void Func_02002510(s32);
extern void Func_02002518(s32);
extern void Func_02002530(s32,s32);
void Func_02000a90(void)
{
    if (Func_02002456(0x941) != 0)
        Func_02002510(0x24e8);
    else
        Func_02002518((s32)Data_00001bc0);
    Func_02002530(9, 0);
}
