#include "object_lookup.h"
#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
void RotateVectorByMagnitude(s32, s32, void *);
void BattleEffect_SetupObjectPair(s32 arg0, s32 arg1) {
    void *temp_r0; void *temp_r0_2; s32 temp_r1; void *temp_r6;
    temp_r6 = *(void **)ADDR_03001F30;
    FIELD_AT_OFFSET(temp_r6, s16, 0x18) = arg0;
    temp_r0 = ObjectTable_Get((s16) arg0);
    FIELD_AT_OFFSET(temp_r6, s16, 0x1A) = arg1;
    FIELD_AT_OFFSET(temp_r6, s32 *, 0x10) = (s32) temp_r0;
    temp_r0_2 = ObjectTable_Get((s16) arg1);
    temp_r1 = (FIELD_AT_OFFSET(temp_r0, u16, 6) + 0x2000) & 0xC000;
    FIELD_AT_OFFSET(temp_r6, s32 *, 0x14) = (s32) temp_r0_2;
    FIELD_AT_OFFSET(temp_r6, s32 *, 0) = temp_r1;
    if (temp_r0_2 != 0) {
        FIELD_AT_OFFSET(temp_r6, s32 *, 0x38) = (s32) FIELD_AT_OFFSET(temp_r0_2, s32 *, 0x6C);
        FIELD_AT_OFFSET(temp_r6, s32 *, 0x3C) = (s32) FIELD_AT_OFFSET(temp_r0_2, s32 *, 0);
        {
            u8 tmp = (u8) FIELD_AT_OFFSET(FIELD_AT_OFFSET(FIELD_AT_OFFSET(temp_r0_2, void **, 0x50), void **, 0x28), u8, 5);
            FIELD_AT_OFFSET(temp_r6, u8, 0x44) = tmp;
        }
        FIELD_AT_OFFSET(temp_r6, s32 *, 4) = (s32) FIELD_AT_OFFSET(temp_r0_2, s32 *, 8);
        FIELD_AT_OFFSET(temp_r6, s32 *, 0xC) = (s32) FIELD_AT_OFFSET(temp_r0_2, s32 *, 0x10);
        FIELD_AT_OFFSET(temp_r6, s32 *, 8) = (s32) FIELD_AT_OFFSET(temp_r0_2, s32 *, 0xC);
        return;
    }
    FIELD_AT_OFFSET(temp_r6, s32 *, 4) = (s32) FIELD_AT_OFFSET(temp_r0, s32 *, 8);
    FIELD_AT_OFFSET(temp_r6, s32 *, 0xC) = (s32) FIELD_AT_OFFSET(temp_r0, s32 *, 0x10);
    FIELD_AT_OFFSET(temp_r6, s32 *, 8) = (s32) FIELD_AT_OFFSET(temp_r0, s32 *, 0xC);
    RotateVectorByMagnitude(0x100000, temp_r1, (u8 *)temp_r6 + 4);
}
