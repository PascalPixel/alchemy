#include "types.h"

u32 Func_08005b24(s32);
s32 Func_08005b64(s32);

u32 SaveState_DeleteRecord(s32 record_id)
{
    s32 index;
    s32 deletion_result;

    index = Func_08005b24(record_id);
    if (index > 0xFU) {
        return 1U;
    }
    deletion_result = Func_08005b64(index);
    return (u32) ((0 - deletion_result) | deletion_result) >> 0x1F;
}
