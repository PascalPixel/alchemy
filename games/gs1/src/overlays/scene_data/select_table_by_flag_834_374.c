#include "types.h"

#define SceneData_SelectTableByFlag834 Func_02000094
#define NULL ((void *)0)

extern u8 Data_0200b144[];
extern u8 Data_0200b108[];
s32 Func_0200270e(s32);

void *SceneData_SelectTableByFlag834(void) {
    if (Func_0200270e(0x834) != 0) {
        return Data_0200b144;
    }
    return Data_0200b108;
}
