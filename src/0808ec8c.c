#include "effect_runtime.h"

#define M2C_FIELD(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 Func_080030f8(s32);
s32 Func_08009080(void *, s32);
s32 Func_080091e0(void *, s32);
s32 Func_0808ec14();
s32 Func_080f9010(s32);

void Func_0808ec8c(void) {
    s32 slot;
    void *object;

    slot = Func_0808ec14();
    if (slot != -1) {
        { s32 base = *(s32 *)0x03001EBC; s32 offset = slot * 8; object = *(void **)((s32)base + (s32)offset + (s32)0x11C); }
        Func_080030f8(0x12);
        if (object != NULL) {
            Func_08009080(object, 7);
        }
        Func_080f9010(0x92);
        if (object != NULL) {
            M2C_FIELD(object, s32 *, 0x28) = 0x80000;
            Func_080091e0(object, 1);
        }
    }
}
