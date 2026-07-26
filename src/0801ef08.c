#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Func_0801eea0(s32 flags);
void Func_0801f200(s32);
void *Func_080048f4(s32 arg0, s32 arg1);
s32 Func_080162d4(u16, u16, u16, u16, s32);

void Func_0801ef08(s32 arg0) {
    s32 zero;
    void *temp_r5;
    s8 *temp_r6;

    temp_r5 = Func_080048f4(0x10, 0x10);
    temp_r6 = (s8 *)((u8 *)*(void **)0x03001E8C + 0xEA6);
    zero = 0;
    *temp_r6 = 1;
    Func_0801eea0(arg0);
    M2C_FIELD(temp_r5, s32 *, 0) = Func_080162d4(M2C_FIELD(temp_r5, u16 *, 4), M2C_FIELD(temp_r5, u16 *, 6), M2C_FIELD(temp_r5, u16 *, 8), M2C_FIELD(temp_r5, u16 *, 0xA), 6);
    Func_0801f200(arg0);
    *temp_r6 = zero;
}
