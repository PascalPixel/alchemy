#include "fixed_math.h"
#include "types.h"
#include "object_efx.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
s32 Object_SetCallback(void *, s32);

void BattleEffect_UpdateDriftingFallObject(void *obj) {
    s32 r;

    FIELD_AT_OFFSET(obj, s32 *, 0xC) = (s32) (FIELD_AT_OFFSET(obj, s32 *, 0xC) + 0xFFFFB334);
    r = Rand();
    FIELD_AT_OFFSET(obj, s32 *, 8) = (s32) (FIELD_AT_OFFSET(obj, s32 *, 8) + (r - Rand()));
    if ((s32) FIELD_AT_OFFSET(obj, s32 *, 0xC) <= (s32) FIELD_AT_OFFSET(obj, s32 *, 0x14)) {
        Object_SetCallback(obj, Data_0809f0b0);
    }
}
