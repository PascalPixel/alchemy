#include "save_state_family.h"

u32 Func_08005a78(s32 record_id, void *destination)
{
    struct SaveWorkspace *work;
    u32 index;

    work = SAVE_WORKSPACE;
    index = Func_08005b24(record_id);
    if (index > 15)
        return 1;
    Func_080058ac(index);
    /* The reference's standalone three-register Thumb STMIA is not emitted
     * by canonical GCC 2.96 from an ordinary-C aggregate store. */
    *DMA3 = (struct DmaRegisters){
        (u32)work->slot.record.payload,
        (u32)destination,
        0x840003fc
    };
    WAIT_DMA();
    return 0;
}
