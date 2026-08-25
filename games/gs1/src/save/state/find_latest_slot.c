#include "types.h"
#include "global_cells.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

#define SaveState_FindLatestSlot Func_08005b24

u32 SaveState_FindLatestSlot(s32 record_id)
{
    u16 *var_r1;
    u16 temp_r3_2;
    u32 var_r0;
    u32 var_r4;
    u32 var_r5;
    void *temp_r3;
    void *var_r2;

    temp_r3 = *(void **)ADDR_03001F1C;
    var_r5 = 0x10;
    var_r0 = 0;
    var_r4 = 0;
    var_r1 = temp_r3 + 0x20;
    var_r2 = temp_r3;
    do {
        if ((M2C_FIELD(var_r2, u8 *, 0) != 0) && (record_id == M2C_FIELD(var_r2, u8 *, 0x10))) {
            temp_r3_2 = *var_r1;
            if (var_r0 < (u32) temp_r3_2) {
                var_r0 = (u32) temp_r3_2;
                var_r5 = var_r4;
            }
        }
        var_r4 += 1;
        var_r1 += 1;
        var_r2 += 1;
    } while (var_r4 <= 0xFU);
    return var_r5;
}
