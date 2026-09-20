#include "SAVE_STATE.H"

u32 Func_08005a78(s32 record_id, void *destination)
{
    struct SaveWorkspace *work;
    u32 index;

    work = SAVE_WORKSPACE;
    index = Func_08005b24(record_id);
    if (index > 15)
        return 1;
    Func_080058ac(index);
    START_DMA(work->slot.record.payload, destination, 0x840003fc);
    WAIT_DMA();
    return 0;
}
