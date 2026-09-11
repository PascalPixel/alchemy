#include "types.h"
#include "scene.h"

u32 SaveState_DeleteRecord(s32 record_id)
{
    s32 index;
    s32 deletion_result;

    index = State_Do(record_id);
    if (index > 0xFU) {
        return 1U;
    }
    deletion_result = State_Check(index);
    return (u32)((0 - deletion_result) | deletion_result) >> 0x1F;
}
