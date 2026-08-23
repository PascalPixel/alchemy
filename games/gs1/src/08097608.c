#include "types.h"
#include "global_cells.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_08002dd8(s32);
s32 ScheduleCallback(s32);
void Func_0808e0b0(struct Outer0808e0b0 *outer, s32 value);
struct Outer0808e0b0 *Object_GetById(u32);
void Func_0809748c(void);
void Func_08097adc(void);
extern u8 Data_08097645;

void Func_08097608(void) {
    void *temp_r5;

    temp_r5 = *(void **)ADDR_03001EA8;
    Func_08097adc();
    ScheduleCallback((s32)&Data_08097645);
    Func_0808e0b0(Object_GetById(M2C_FIELD(temp_r5, u16, 0x290)), 1);
    Func_0809748c();
    Func_08002dd8(0x16);
}
