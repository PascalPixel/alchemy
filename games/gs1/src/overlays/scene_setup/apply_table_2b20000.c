#include "types.h"

extern void Func_020052c0(s32, s32, s32);
extern u8 Data_02b20000[];

void FieldScene_ApplyTable2b20000(void)
{
    Func_020052c0((s32)Data_02b20000, 0, 0x2480000);
}
