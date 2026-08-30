#include "types.h"
#include "fixed_math.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

u32 Random16(void);

void Camera_ApplyShake(s32 arg0, u32 arg1) {
    s32 temp_r3;
    s32 temp_r6_2;
    s32 *temp_r0;
    s32 temp_r0_2;
    s32 temp_r1;
    s32 temp_r2;
    s32 temp_r5;
    s32 temp_r6;
    void *temp_r1_2;
    void *temp_r1_3;
    void *temp_r2_2;
    void *temp_r2_3;

    temp_r1 = *(s32 *)ADDR_03001EEC;
    temp_r0 = (s32 *)(temp_r1 + 0x77A8);
    if (*temp_r0 > 0) {
        temp_r6 = (arg0 - 1) & Random16();
        temp_r5 = (s32)(arg1 + (arg1 >> 0x1F)) >> 1;
        temp_r2 = ((arg1 - 1) & Random16()) - temp_r5;
        temp_r1_2 = (void *)ADDR_03001AD0;
        temp_r6_2 = temp_r6 - temp_r5;
        temp_r3 = temp_r2 + 0x20;
        FIELD_AT_OFFSET(temp_r1_2, s16 *, 4) = temp_r6_2;
        FIELD_AT_OFFSET(temp_r1_2, s16 *, 6) = temp_r3;
        temp_r1_3 = (void *)ADDR_03001CE0;
        FIELD_AT_OFFSET(temp_r1_3, s32 *, 0xC) = 0x78 - temp_r6_2;
        FIELD_AT_OFFSET(temp_r1_3, s32 *, 0x10) = 0x78 - temp_r2;
        *temp_r0 -= 1;
        return;
    }
    temp_r0_2 = FIELD_AT_OFFSET(temp_r1, s32 *, 0x77A0);
    temp_r2_2 = (void *)ADDR_03001AD0;
    FIELD_AT_OFFSET(temp_r2_2, s16 *, 4) = temp_r0_2;
    temp_r0_2 = FIELD_AT_OFFSET(temp_r1, s32 *, 0x77A4);
    FIELD_AT_OFFSET(temp_r2_2, s16 *, 6) = temp_r0_2;
    temp_r2_3 = (void *)ADDR_03001CE0;
    FIELD_AT_OFFSET(temp_r2_3, s32 *, 0xC) = 0x78;
    FIELD_AT_OFFSET(temp_r2_3, s32 *, 0x10) = 0x78;
}
