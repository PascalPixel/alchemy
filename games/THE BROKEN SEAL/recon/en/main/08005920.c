#include "save_state_family.h"

s32 Func_08005920(s32 record_id, void *source)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    u32 zero;
    u32 current;
    u32 slot;

    work = SAVE_WORKSPACE;
    zero = 0;
    START_DMA(&zero, &work->slot, 0x85000400);
    WAIT_DMA();
    current = Func_08005b24(record_id);
    slot = Func_08005810(record_id);
    if (slot > 15)
        return 1;

    START_DMA(source, work->slot.record.payload, 0x840003fc);
    WAIT_DMA();
    START_DMA(SAVE_HEADER_TEMPLATE, &header, 0x84000002);
    WAIT_DMA();
    header.record_id = record_id;
    header.checksum = Func_08005ae0();
    header.sequence = Func_08005c2c(record_id) + 1;
    START_DMA(&header, &work->slot.record.header, 0x84000004);
    WAIT_DMA();

    if (Func_08005868(slot) != 0)
        return 1;
    if (current <= 15 && Func_08005b64(current) != 0)
        return 1;

    if (header.sequence > 0xfde8) {
        header.sequence = 1;
        START_DMA(&header, &work->slot.record.header, 0x84000004);
        WAIT_DMA();
        if (Func_08005868(current) != 0)
            return 1;
        if (Func_08005b64(slot) != 0)
            return 1;
        slot = current;
    }

    work->occupied[slot] = 1;
    work->record_id[slot] = record_id;
    work->sequence[slot] = header.sequence;
    return 0;
}
