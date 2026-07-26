#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Func_080048f4(s32 arg0, s32 arg1);

void Func_080933d4(s32 arg0, s32 arg1) {
    void *temp_r3;

    temp_r3 = M2C_FIELD(Func_080048f4(0x1B, 0xCCC), void **, 0x1E0);
    M2C_FIELD(temp_r3, s32 *, 0x30) = arg0;
    M2C_FIELD(temp_r3, s32 *, 0x34) = arg1;
}
