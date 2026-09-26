/* Draft: complete 322-byte save-workspace initializer; 322 emitted bytes,
 * 120 differing halfwords / 77 aligned edits. The ROM's 08005788 pool word
 * is zero, and IdentifyFlash is tested at halfword width. A volatile DMA
 * fill local recovers the indirect stack store; sharing the retry/slot
 * counter recovers the total extent. Allocation and the first pool placement
 * remain different. A separate record-id snapshot enlarged the frame to 28
 * bytes; explicit retry-success goto and array-header trials changed the
 * block layout and were not retained. */
#include "save_state_family.h"

s32 Func_080056cc(void)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    volatile u32 zero;
    u32 index;
    u16 empty = 0;

    work = Func_080048f4(0x33, sizeof(*work));
    zero = 0;
    START_DMA(&zero, work, 0x85000440);
    SetFlashTimerIntr(2, (void (**)(void))0x030000f4);

    index = 0;
    for (;;) {
        if (index > 7)
            return 1;
        if ((u16)IdentifyFlash() == 0)
            break;
        Func_080030f8(1);
        index++;
    }

    for (index = 0; index <= 15; index++) {
        u32 status;

        work->occupied[index] = 0;
        work->record_id[index] = 0x10;
        work->sequence[index] = empty;
        status = Func_080058ac(index);
        START_DMA(&work->slot, &header, 0x84000004);
        WAIT_DMA();

        if (Func_08005c08(header.signature, (u8 *)SAVE_SIGNATURE, 7) != 0)
            continue;
        work->sequence[index] = header.sequence;
        if (header.record_id > 15 || status != 0)
            continue;

        work->occupied[index] = 1;
        work->record_id[index] = header.record_id;
        if (status < index) {
            do {
                if (work->record_id[status] == header.record_id) {
                    if (work->sequence[status] < header.sequence)
                        work->occupied[status] = 0;
                    else
                        work->occupied[index] = 0;
                }
                status++;
            } while (status < index);
        }
    }
    return 0;
}
