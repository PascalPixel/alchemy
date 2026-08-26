#include "save_state_family.h"

s32 Func_080058ac(s32 index)
{
    struct SaveWorkspace *work;
    struct SaveSlotHeader header;
    u32 checksum;

    work = SAVE_WORKSPACE;
    ReadFlash((u16)index, 0, work->slot.bytes, sizeof(work->slot));
    START_DMA(&work->slot, &header, 0x84000004);
    WAIT_DMA();
    checksum = Func_08005ae0();
    return (u16)checksum - header.checksum;
}
