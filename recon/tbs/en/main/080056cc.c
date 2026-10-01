/* Draft, not exact: SaveState_InitializeWorkspace, 324 bytes. 2026-10-01
 * (wave 1, slice 1): 2134 (23 register-only, 4 stack-only, 5 operand, 8
 * reordered, 8 inserted, 6 deleted), was 2485. The flash retry is a counted
 * loop that breaks when the chip answers, with the failure tested after it
 * (index > 7): that gives the reference's layout exactly, the return-1
 * block after the loop behind a jump. The slot loop reuses the counter and
 * keeps the record id in a u8. Remaining: allocation in the slot loop. The
 * reference keeps the work pointer in fp, the counter in r7 and the header
 * address in r8, loads the record id into r2 with a copy in r1 (ip inside
 * the search) and puts the search's zero in r4; here the work pointer is
 * spilled, the counter takes r8, the header fp and the id r4. A 120-second
 * permute reaches 1440 only with pointer temporaries. */
#include "SAVE_STATE.H"

s32 SaveState_InitializeWorkspace(void)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    volatile u32 zero;
    u32 index;

    work = Runtime_AllocateBlock(0x33, sizeof(*work));
    zero = 0;
    START_DMA(&zero, work, 0x85000440);
    SetFlashTimerIntr(2, (void (**)(void))0x030000f4);

    for (index = 0; index < 8; index++) {
        if ((u16)IdentifyFlash() == 0)
            break;
        WaitFrames(1);
    }
    if (index > 7)
        return 1;

    for (index = 0; index <= 15; index++) {
        u32 status;
        u8 id;

        work->occupied[index] = 0;
        work->record_id[index] = 0x10;
        work->sequence[index] = 0;
        status = SaveState_ReadSlotAndCheckChecksum(index);
        START_DMA(&work->slot, &header, 0x84000004);
        WAIT_DMA();

        if (SaveState_CompareBytes(header.signature, (u8 *)Save_Signature, 7) != 0)
            continue;
        id = header.record_id;
        work->sequence[index] = header.sequence;
        if (id > 15)
            continue;
        if (status != 0)
            continue;

        work->occupied[index] = 1;
        work->record_id[index] = id;
        if (status < index) {
            do {
                if (work->record_id[status] == id) {
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
