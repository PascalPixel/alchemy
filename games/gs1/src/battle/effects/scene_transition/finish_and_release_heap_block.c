#include "types.h"
#include "global_cells.h"

#define BattleEffect_FinishSceneAndReleaseHeapBlock Func_08097608

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_08002dd8(s32);
s32 ScheduleCallback(s32);
void Func_0808e0b0(struct Outer0808e0b0 *outer, s32 value);
struct Outer0808e0b0 *Object_GetById(u32);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_08097adc(void);
extern u8 Data_08097645;

void BattleEffect_FinishSceneAndReleaseHeapBlock(void) {
    void *work;

    work = *(void **)ADDR_03001EA8;
    Func_08097adc();
    ScheduleCallback((s32)&Data_08097645);
    Func_0808e0b0(Object_GetById(FIELD_AT_OFFSET(work, u16, 0x290)), 1);
    BattleEffect_PrepareBufferInterpolation();
    Func_08002dd8(0x16);
}
