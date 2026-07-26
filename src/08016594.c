#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Func_08015ec0(u32);
void Func_08003f3c(u8);

void Func_08016594(void *arg0) {
    Func_08015ec0((u32) arg0);
    if (M2C_FIELD(arg0, u8, 4) != 0) {
        Func_08003f3c(M2C_FIELD(arg0, u8, 0xE));
        if (M2C_FIELD(arg0, u8, 4) == 2) {
            u8 *dst = (u8 *) (*(s32 *)0x03001E8C);
            s32 idx = ((u32) M2C_FIELD(arg0, u8, 0x19) >> 4) * 2 + 0x12D0;
            *(u16 *)(dst + idx) = 0x3E7;
        }
    }
    M2C_FIELD(arg0, s8, 5) = 0;
}
