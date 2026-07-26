#include "types.h"

typedef int bool;
#define NULL ((void *)0)
extern u8 *Data_03001ebc;

extern s32 Func_0808ae74(u8 arg0);

s32 Func_0808b02c(u32 arg0) {
    u8 *table = Data_03001ebc + 0x1a0;
    return Func_0808ae74(table[arg0]);
}
