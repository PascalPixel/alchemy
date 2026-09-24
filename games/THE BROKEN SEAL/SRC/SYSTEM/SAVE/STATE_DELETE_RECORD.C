#include "TYPES.H"

u32 SaveState_FindLatestSlot(s32);
s32 SaveState_InvalidateSlot(s32);

u32 SaveState_DeleteRecord(s32 record_id)
{
    s32 index;
    s32 deletion_result;

    index = SaveState_FindLatestSlot(record_id);
    if (index > 0xFU) {
        return 1U;
    }
    deletion_result = SaveState_InvalidateSlot(index);
    return (u32)((0 - deletion_result) | deletion_result) >> 0x1F;
}
