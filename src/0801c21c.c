#include "types.h"

s32 Func_08003f3c(u32 index);
void Func_0801c21c(void)
{
    u32 offset = 0x30c;
    u8 *work = *(u8 **)0x03001e98 + offset;

    if (*(u16 *)(work + 0x0a) != 0) {
        Func_08003f3c(*(u16 *)(work + 0x0c));
        *(u16 *)(work + 0x0a) = offset = 0;
    }
}
