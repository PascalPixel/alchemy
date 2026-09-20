#include "TYPES.H"
#include "DMA.H"
#include "FLASH.H"
#include "GLOBAL_CELLS.H"

#include "SAVE_STATE.H"

struct DmaChannel {
    u32 source;
    u32 destination;
    u32 control;
};

static __inline__ void Dma_WaitForCompletion(volatile struct DmaChannel *channel)
{
    while ((channel->control & 0x80000000) != 0) {}
}

s32 SaveState_ReadSlotAndCheckChecksum(s32 index)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    u32 checksum;

    work = *(struct SaveWorkspace **)ADDR_03001F1C;
    ReadFlash((u16)index, 0, work->slot.bytes, sizeof(work->slot));
    Dma_Set(&work->slot, &header, 0x84000004, (volatile u32 *)0x040000d4);
    Dma_WaitForCompletion((volatile struct DmaChannel *)0x040000d4);
    checksum = SaveState_ChecksumWorkspace();
    return (u16)checksum - header.checksum;
}
