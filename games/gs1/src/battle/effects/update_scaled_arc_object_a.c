#include "types.h"
#include "object_efx.h"

#define BattleEffect_UpdateScaledArcObjectA Func_08096d2c

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void BattleEffect_UpdateScaledArcObjectA(void *obj) {
    s32 v;
    s32 step;
    void *link;

    step = FIELD_AT_OFFSET(obj, u16 *, 0x64) + 1;
    link = *(void * volatile *)((u8 *)obj + 0x68);
    *(volatile s16 *)((u8 *)obj + 0x64) = step;
    v = (s16)step;
    if (v > 0x1F) {
        Object_SetCallback((s32)obj, Data_0809f0b0);
        return;
    }
    v = Func_08002322(v << 10);
    FIELD_AT_OFFSET(obj, s32 *, 0x18) = v;
    FIELD_AT_OFFSET(obj, s32 *, 0x1C) = v;
    FIELD_AT_OFFSET(obj, s32 *, 8) = FIELD_AT_OFFSET(link, s32 *, 8);
    FIELD_AT_OFFSET(obj, s32 *, 0xC) += 0x10000;
    FIELD_AT_OFFSET(obj, s32 *, 0x10) =
        FIELD_AT_OFFSET(link, s32 *, 0x10) + ((0x10000 - v) * 5) + 0x90000;
}
