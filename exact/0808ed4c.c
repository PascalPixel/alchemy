#include "effect_runtime.h"

#define M2C_FIELD(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 Func_0808ec14();

s32 Func_0808ed4c(void) {
    s32 slot;

    slot = Func_0808ec14();
    if (slot == -1) {
        return 0;
    }
    { s32 base = *(s32 *)0x03001EBC; s32 offset = slot * 8; return *(s32 *)((s32)base + (s32)offset + (s32)0x11C); }
}
