#include "types.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

#define SaveState_GetLatestSequence Func_08005c2c

u32 SaveState_GetLatestSequence(s32 record_id)
{
    u16 *var_r1;
    u16 temp_r3_2;
    u32 var_r0;
    u32 var_r4;
    void *temp_r3;
    void *var_r2;

    temp_r3 = *(void **)ADDR_03001F1C;
    var_r4 = 0;
    var_r0 = 0;
    var_r1 = temp_r3 + 0x20;
    var_r2 = temp_r3;
    do {
        if ((FIELD_AT_OFFSET(var_r2, u8 *, 0) != 0) && (record_id == FIELD_AT_OFFSET(var_r2, u8 *, 0x10))) {
            temp_r3_2 = *var_r1;
            if (var_r0 < (u32) temp_r3_2) {
                var_r0 = (u32) temp_r3_2;
            }
        }
        var_r4 += 1;
        var_r1 += 1;
        var_r2 += 1;
    } while (var_r4 <= 0xFU);
    return var_r0;
}
