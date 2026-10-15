#include "types.h"
#include "ram_buffer.h"
#include "save_state.h"
#include "savedata.h"
#include "dma.h"
#include "flash.h"

#define DMA3 ((volatile struct DmaRegisters *)0x040000d4)
#define DMA3_BUSY 0x80000000

#define START_DMA(source_, destination_, control_) \
    Dma_Set((const void *)(source_), (void *)(destination_), (control_), (volatile u32 *)0x040000d4)

static __inline__ void WaitDma(struct DmaRegisters volatile *channel)
{
    while ((channel->control & DMA3_BUSY) != 0) {}
}

#define WAIT_DMA() WaitDma(DMA3)

extern u16 (*gFlash_Handler0)(u16 sector, u8 *source);

u32 Random16(void);

s32 SaveState_CompareBytes(u8 *left, u8 *right, s32 count);

s32 SaveState_ChecksumWorkspace(void);

s32 SaveState_ReadSlotAndCheckChecksum(s32 index);

u32 SaveState_WriteWorkspaceSlot(s32 sector);

s32 SaveState_InvalidateSlot(s32 index);

u32 SaveState_ReadRecordPayload(s32 recordId, void *destination)
{
    struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
    u32 index;

    index = SaveState_FindLatestSlot(recordId);
    if (index > 14)
        return 1;
    SaveState_ReadSlotAndCheckChecksum(index);
    START_DMA(work->slot + 16, destination, 0x84000bfc);
    WAIT_DMA();
    return 0;
}

u32 SaveState_DeleteRecord(s32 recordId)
{
    struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
    s32 index;
    s32 result;

    index = SaveState_FindLatestSlot(recordId);
    if ((u32)index > 14)
        return 1;
    work->sectorCount = 0;
    work->sectorTotal = 3;
    result = SaveState_InvalidateSlot(index);
    return (u32)((0 - result) | result) >> 31;
}

s32 SaveState_ChecksumWorkspace(void)
{
    struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
    u32 limit;
    u32 offset;
    s32 sum;

    sum = 0;
    offset = 0;
    limit = 0x2fef;
    do {
        sum += work->slot[16 + offset + 0];
        sum += work->slot[16 + offset + 1];
        sum += work->slot[16 + offset + 2];
        sum += work->slot[16 + offset + 3];
        sum += work->slot[16 + offset + 4];
        sum += work->slot[16 + offset + 5];
        sum += work->slot[16 + offset + 6];
        sum += work->slot[16 + offset + 7];
        offset += 8;
    } while (offset <= limit);
    return sum;
}

u32 SaveState_FindLatestSlot(s32 recordId)
{
    struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
    u32 latest_sequence;
    u32 latest_slot;
    u32 i;

    latest_sequence = 0;
    latest_slot = 15;
    for (i = 0; i <= 14; i += 3) {
        if (work->occupied[i] != 0 && recordId == work->recordId[i]) {
            if (latest_sequence < work->sequence[i]) {
                latest_sequence = work->sequence[i];
                latest_slot = i;
            }
        }
    }
    return latest_slot;
}

s32 SaveState_InvalidateSlot(s32 index)
{
    struct SaveWorkspace *work = Ram_HeapSlots->serialWork;
    struct SaveSlotHeader header;
    volatile u32 zero;

    zero = 0;
    START_DMA(&zero, &header, 0x85000004);
    WAIT_DMA();
    START_DMA(gSave_HeaderTemplate, &header, 0x84000002);
    WAIT_DMA();
    header.recordId = 16;
    header.sequence = 0;
    START_DMA(&header, work->slot, 0x84000004);
    WAIT_DMA();
    if (SaveState_WriteWorkspaceSlot(index) != 0)
        return 1;
    work->occupied[index] = 0;
    work->recordId[index] = 16;
    work->sequence[index] = 0;
    return 0;
}
