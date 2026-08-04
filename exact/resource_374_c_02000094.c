#include "types.h"
#define NULL ((void *)0)

extern u8 Data_0200b144[];
extern u8 Data_0200b108[];
s32 Func_0200270e(s32);

void *Func_02000094(void) {
    if (Func_0200270e(0x834) != 0) {
        return Data_0200b144;
    }
    return Data_0200b108;
}
