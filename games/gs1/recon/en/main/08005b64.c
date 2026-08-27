#include "save_state_family.h"

#undef START_DMA
#define START_DMA(source_, destination_, control_) do { \
    u32 src_ = (u32)(source_); \
    u32 dst_ = (u32)(destination_); \
    u32 ctl_ = (control_); \
    DMA3->control = ctl_; \
    DMA3->destination = dst_; \
    DMA3->source = src_; \
} while (0)

s32 Func_08005b64(s32 index)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    u32 zero;

    work = SAVE_WORKSPACE;
    zero = 0;
    START_DMA(&zero, &header, 0x85000004);
    WAIT_DMA();
    START_DMA(SAVE_HEADER_TEMPLATE, &header, 0x84000002);
    WAIT_DMA();
    header.record_id = 0x10;
    header.sequence = 0;
    START_DMA(&header, &work->slot.record.header, 0x84000004);
    WAIT_DMA();
    if (Func_08005868(index) != 0)
        return 1;
    work->occupied[index] = 0;
    work->record_id[index] = 0x10;
    work->sequence[index] = 0;
    return 0;
}
