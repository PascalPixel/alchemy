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

extern u16 (*Flash_Handler0)(u16 sector, u8 *source);

u32 Random16(void);

s32 SaveState_CompareBytes(u8 *left, u8 *right, s32 count);

s32 SaveState_ChecksumWorkspace(void);

s32 SaveState_ReadSlotAndCheckChecksum(s32 index);

u32 SaveState_WriteWorkspaceSlot(s32 sector);

s32 SaveState_InvalidateSlot(s32 index);

u32 SaveState_WriteWorkspaceSlot(s32 sector)
{
    struct SaveWorkspace *work = Ram_HeapSlots->serial_work;
    void (*progress)(s32) = work->progress;

    if (progress != 0)
        progress(0);
    if (Flash_Handler0(sector, work->slot) != 0 || Flash_VerifySector(sector, (s32)work->slot) != 0)
        return 1;
    if (progress != 0)
        progress(1);
    if (Flash_Handler0(sector + 1, work->slot + 0x1000) != 0 || Flash_VerifySector(sector + 1, (s32)(work->slot + 0x1000)) != 0)
        return 1;
    if (progress != 0)
        progress(1);
    if (Flash_Handler0(sector + 2, work->slot + 0x2000) != 0 || Flash_VerifySector(sector + 2, (s32)(work->slot + 0x2000)) != 0)
        return 1;
    if (progress != 0)
        progress(1);
    return 0;
}

s32 SaveState_ReadSlotAndCheckChecksum(s32 index)
{
    struct SaveWorkspace *work = Ram_HeapSlots->serial_work;
    struct SaveSlotHeader header;

    ReadFlash(index, 0, work->slot, 0x1000);
    ReadFlash(index + 1, 0, work->slot + 0x1000, 0x1000);
    ReadFlash(index + 2, 0, work->slot + 0x2000, 0x1000);
    START_DMA(work->slot, &header, 0x84000004);
    WAIT_DMA();
    return (u16)SaveState_ChecksumWorkspace() - header.checksum;
}
