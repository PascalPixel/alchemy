#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void *Func_0808ba1c();

s16 Func_08091584(void) {
    s16 *temp_r0;
    void *temp_r0_2;
    void *temp_r0_3;

    temp_r0_2 = Func_0808ba1c();
    if ((M2C_FIELD(temp_r0_2, u8 *, 0x54) != 1) || (temp_r0_3 = M2C_FIELD(temp_r0_2, void **, 0x50), (temp_r0_3 == NULL)) || (temp_r0 = M2C_FIELD(temp_r0_3, s16 **, 0x28), (temp_r0 == NULL))) {
        return 0;
    }
    return *temp_r0;
}
