#include "types.h"

#define Graphics_EnableObjLayerAndCallbacks Func_0800c5b4

s32 WaitFrames(s32);
s32 Func_080042c8(u32 value);
s32 Func_0808a330(s32, s32);
s32 Func_0808a348(s32);
void Func_0800c62c(void);
void Func_0800c880(void);

void Graphics_EnableObjLayerAndCallbacks(void) {
    Func_080042c8((u32)Func_0800c62c);
    Func_080042c8((u32)Func_0800c880);
    Func_0808a330(0x10000, 1);
    Func_0808a348(1);
    WaitFrames(1);
    *(u16 *)0x04000000 = (0xF1FF & *(u16 *)0x04000000) | 0x1000;
}
