#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_08002dd8(s32);
s32 Func_08004278(s32);
void Func_0808e0b0(struct Outer0808e0b0 *outer, s32 value);
s32 Func_08092054(u32);
void Func_0809748c(void);
void Func_08097adc(void);

void Func_08097608(void) {
    void *temp_r5;

    temp_r5 = *(void **)0x03001EA8;
    Func_08097adc();
    Func_08004278(0x08097645);
    Func_0808e0b0(Func_08092054(M2C_FIELD(temp_r5, u16, 0x290)), 1);
    Func_0809748c();
    Func_08002dd8(0x16);
}
