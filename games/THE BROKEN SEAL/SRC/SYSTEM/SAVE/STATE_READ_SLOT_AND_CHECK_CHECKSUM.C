#include "TYPES.H"
#include "DMA.H"
#include "FLASH.H"
#include "GLOBAL_CELLS.H"

struct SaveSlotHeader {
    u8 signature[7];
    u8 record_id;
    u16 checksum;
    u16 sequence;
    u8 reserved[4];
};

struct SaveWorkspace {
    u8 occupied[16];
    u8 record_id[16];
    u16 sequence[16];
    union {
        struct {
            struct SaveSlotHeader header;
            u8 payload[0xff0];
        } record;
        u8 bytes[0x1000];
    } slot;
    u8 summary[3][64];
};

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
