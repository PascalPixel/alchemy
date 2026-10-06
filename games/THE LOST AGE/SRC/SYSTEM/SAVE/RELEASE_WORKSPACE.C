#include "TYPES.H"
#include "DMA.H"
#include "FLASH.H"
#include "RAM_BUFFER.H"
#include "SAVE_STATE.H"
#include "RUNTIME_INTERFACES.H"
#include "IRQ.H"
#include "IO_REG.H"

s32 SaveState_LoadExtraSummary(void)
{
    struct SaveWorkspace *work;
    struct SaveSummary *summary;
    u32 index;

    work = Ram_HeapSlots->serial_work;
    summary = &work->extra;
    Dma_Fill32(0, summary, sizeof(*summary));
    index = SaveState_FindLatestSlot(3);
    if (index <= 14) {
        ReadFlash(index, 0, (u8 *)summary, sizeof(*summary));
        ReadFlash(index + 1, 0x110, work->extra.unknown_38, sizeof(work->extra.unknown_38));
        return 1;
    }
    return 0;
}

void SaveState_ReleaseWorkspace(void)
{
    Runtime_SetIrqHandler(5, 0, NULL);
    Runtime_ReleaseHeapBlock(204);
}
