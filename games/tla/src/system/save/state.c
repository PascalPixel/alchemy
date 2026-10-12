#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "runtime_interfaces.h"
#include "ram_buffer.h"
#include "save_state.h"

s32 SaveState_CompareBytes(u8 *left, u8 *right, s32 count)
{
    s32 difference = 0;

    while (count != 0) {
        difference = *left - *right;
        if (difference != 0)
            break;
        count--;
        left++;
        right++;
    }
    return difference;
}

u32 SaveState_FindLatestSequence(s32 record_id)
{
    struct SaveWorkspace *work = Ram_HeapSlots->serial_work;
    u32 latest_sequence;
    u32 i;

    latest_sequence = 0;
    for (i = 0; i <= 14; i += 3) {
        if (work->occupied[i] != 0 && record_id == work->record_id[i]) {
            if (latest_sequence < work->sequence[i])
                latest_sequence = work->sequence[i];
        }
    }
    return latest_sequence;
}
