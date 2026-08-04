#include "effect_runtime.h"

#define M2C_FIELD(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 Func_08009080(s32, s32);
s32 Func_0808ec14();

void Func_0808ed1c(void) {
    s32 slot;
    s32 object;

    slot = Func_0808ec14();
    if (slot != -1) {
        { s32 base = *(s32 *)0x03001EBC; s32 offset = slot * 8; object = *(s32 *)((s32)base + (s32)offset + (s32)0x11C); }
        if (object != 0) {
            Func_08009080(object, 2);
        }
    }
}
