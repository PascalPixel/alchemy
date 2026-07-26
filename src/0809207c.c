#include "types.h"

s32 Func_0808ba1c(u32 arg0);
void Func_08093a6c(s8 *object, s32 kind);

void Func_0809207c(s32 arg0, s32 arg1) {
    u8 *object = Func_0808ba1c(arg0);
    if (object != NULL) {
        s32 value = 1;
        value |= object[0x5A];
        object[0x5A] = value;
        Func_08093a6c(object, arg1);
    }
}
