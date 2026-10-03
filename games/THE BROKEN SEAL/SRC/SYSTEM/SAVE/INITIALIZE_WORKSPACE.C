#include "RUNTIME_MEM.H"
#include "SAVE_STATE.H"
#include "SYSTEM.H"
#include "IRQ.H"

/* Allocates the save workspace, waits for the flash chip to identify itself
   (eight tries, a frame apart; returns 1 if it never does), then reads all
   sixteen slots: a slot with the signature, a record id below sixteen and a
   good checksum is marked occupied, and of two slots holding the same record
   only the one with the later sequence number stays. */
s32 SaveState_InitializeWorkspace(void)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    struct SaveSlotHeader *slot;
    volatile u32 zero;
    u32 index;

    work = Runtime_AllocateBlock(0x33, sizeof(*work));
    zero = 0;
    START_DMA(&zero, work, 0x85000440);
    SetFlashTimerIntr(2, &gIrqHandlers[5]);

    for (index = 0; index < 8; index++) {
        if ((u16)IdentifyFlash() == 0)
            break;
        WaitFrames(1);
    }
    if (index > 7)
        return 1;

    for (index = 0; index <= 15; index++) {
        u32 status;
        u32 record;
        u8 id;

        work->occupied[index] = 0;
        work->record_id[index] = 0x10;
        work->sequence[index] = 0;
        status = SaveState_ReadSlotAndCheckChecksum(index);
        START_DMA(&work->slot, &header, 0x84000004);
        WAIT_DMA();
        slot = &header;

        if (SaveState_CompareBytes(slot->signature, (u8 *)Save_Signature, 7) != 0)
            continue;
        work->sequence[index] = slot->sequence;
        record = slot->record_id;
        id = record;
        if (id > 15)
            continue;
        if (status != 0)
            continue;

        /* The checksum was good, so status is zero: it is the counter of
           the search through the slots read so far. */
        work->occupied[index] = 1;
        work->record_id[index] = record;
        if (status < index) {
            do {
                if (work->record_id[status] == (u8)record) {
                    if (work->sequence[status] < slot->sequence)
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
