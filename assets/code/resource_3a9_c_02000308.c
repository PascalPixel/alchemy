#include "resource_3a9.h"

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];
extern u8 Value_00000064;

extern void Func_0200065e(void);

s32 Func_02000308(void)
{
    *(s32 *)(Data_03001ebc + 448) = 0x209;
    if (Data_02000240[224] == (s32) (u32) &Value_00000064) {
        Func_0200065e();
    }
    return 0;
}
