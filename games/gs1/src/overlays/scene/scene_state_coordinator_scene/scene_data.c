#include "types.h"

#define SceneData_GetTableC194 Func_02000030
#define SceneData_GetTablec1dc Func_0200003c
#define SceneData_GetTablec1f4 Func_02000044
#define SceneData_SelectBlockAndResetCounters Func_020026a8
extern u8 Data_0200c194[];
extern u8 Data_0200c1dc[];
extern u8 Data_0200c1f4[];
extern u8 Data_0200a1b9[];
extern u8 Data_0200c57e[];
extern u8 Data_0200be4e[];
extern u8 Data_0200c5aa[];
extern u8 Data_0200be76[];
extern u8 Data_0200c628[];
extern u16 Data_0200c790;
extern u16 Data_0200c764;
extern u16 Data_0200c79c;
extern u8 *Data_0200c7a0;
extern u16 Data_0200c7f8;
extern u16 Data_0200c76c;
extern u32 Data_0200c770;

void Func_020061d0(u8 *, s32);

/* Contiguous unnamed leaf-owner run for resource_3ba. */

u8 *SceneData_GetTableC194(void)
{
    return Data_0200c194;
}

u8 *SceneData_GetTablec1dc(void)
{
    return Data_0200c1dc;
}

u8 *SceneData_GetTablec1f4(void)
{
    return Data_0200c1f4;
}

void SceneData_SelectBlockAndResetCounters(s32 a, s32 b)
{
    u8 *p;

    Data_0200c790 = a;
    Data_0200c764 = b << 4;
    Func_020061d0(Data_0200a1b9, 3200);
    p = Data_0200c57e;
    if (a == 2) {
        p = Data_0200be4e;
    }
    if (a == 4) {
        p = Data_0200c5aa;
    }
    if (a == 3) {
        if (b != 0) {
            p = Data_0200be76;
        } else {
            p = Data_0200c628;
        }
    }
    Data_0200c79c = 0;
    Data_0200c7a0 = p;
    Data_0200c7f8 = 0;
    Data_0200c76c = 0;
    Data_0200c770 = 0;
}
