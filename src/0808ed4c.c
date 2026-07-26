#include "types.h"

#define M2C_FIELD(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 Func_0808ec14();

s32 Func_0808ed4c(void) {
    s32 temp_r0;

    temp_r0 = Func_0808ec14();
    if (temp_r0 == -1) {
        return 0;
    }
    { s32 _mb = *(s32 *)0x03001EBC; s32 _mo = temp_r0 * 8; return *(s32 *)((s32)_mb + (s32)_mo + (s32)0x11C); }
}
