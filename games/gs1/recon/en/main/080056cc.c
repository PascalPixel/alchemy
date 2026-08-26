#include "save_state_family.h"

s32 Func_080056cc(void)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    u32 zero;
    u32 attempt;
    u32 index;

    work = Func_080048f4(0x33, sizeof(*work));
    zero = 0;
    START_DMA(&zero, work, 0x85000440);
    SetFlashTimerIntr(2, (void (**)(void))0x030000f4);

    attempt = 0;
    while (attempt <= 7 && IdentifyFlash() != 0) {
        Func_080030f8(1);
        attempt++;
    }
    if (attempt > 7)
        return 1;

    for (index = 0; index <= 15; index++) {
        u32 status;

        work->occupied[index] = 0;
        work->record_id[index] = 0x10;
        work->sequence[index] = 0xffff;
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
