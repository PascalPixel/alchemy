#include "effect_runtime.h"

#define M2C_FIELD(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 Func_0808ec14();

void Func_0808ed78(void) {
    s32 slot;
    void *object;

    slot = Func_0808ec14();
    if (slot != -1) {
        { s32 base = *(s32 *)0x03001EBC; s32 offset = slot * 8; object = *(void **)((s32)base + (s32)offset + (s32)0x11C); }
        if (object != NULL) {
            M2C_FIELD(object, s8 *, 0x54) = 0;
        }
    }
}
