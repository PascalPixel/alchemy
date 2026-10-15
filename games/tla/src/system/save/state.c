#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "runtime_interfaces.h"
#include "ram_buffer.h"
#include "save_state.h"
#include "dma.h"
#include "flash.h"

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

// Reads the summary of the latest save in each of the three files, clearing
// it first; returns how many files have one.
s32 SaveState_LoadSummaryRecords(void)
{
    struct SaveWorkspace *work = Ram_HeapSlots->serial_work;
    struct SaveSummary *summary;
    s32 count;
    u32 group;

    /* FAKEMATCH: ⚓️ loads the workspace before it starts the summary offset 0x303c in r2; the empty asm holds r2 until the workspace is loaded. Plainly the offset is started first (differ 2); a volatile barrier after the load holds the prologue's sub sp ahead of the offset instead (differ 2). */
    asm("" : "+l"(work) : : "r2");
    summary = work->summary;
    count = 0;
    for (group = 0; group <= 2; group++) {
        u32 slot;

        Dma_Fill32(0, summary, sizeof(*summary));
        slot = SaveState_FindLatestSlot(group);
        if (slot <= 14) {
            ReadFlash(slot, 0, (u8 *)summary, sizeof(*summary));
            ReadFlash(slot + 1, 16, summary->unknown_38, sizeof(summary->unknown_38));
            count++;
        }
        summary++;
    }
    return count;
}
