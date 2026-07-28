#include "object_lookup.h"
#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080048f4(s32, s32);
s32 Func_080933f8(s32, s32, s32, s32);

void Func_08093500(s32 arg0, s32 arg1) {
    void *temp_r5;

    temp_r5 = Func_0808ba1c();
    Func_080048f4(0x1B, 0xCCC);
    if (temp_r5 != NULL) {
        Func_080933f8(M2C_FIELD(temp_r5, s32 *, 8), -1, M2C_FIELD(temp_r5, s32 *, 0x10), arg1);
    }
}
