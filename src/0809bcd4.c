#include "types.h"

extern u8 Data_02010000[];

extern void Func_08003f3c(u16 arg0);
extern void Func_08015018(u32 arg0, u32 arg1);

void Func_0809bcd4(void) {
    Func_08003f3c(*(u16 *)(Data_02010000 + 0));
    Func_08003f3c(*(u16 *)(Data_02010000 + 2));
    Func_08015018(*(u32 *)(Data_02010000 + 0x1c), 2);
}
