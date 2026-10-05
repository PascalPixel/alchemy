#include "TYPES.H"
#include "DMA.H"
#include "FLASH.H"
#include "RAM_BUFFER.H"
#include "SAVE_STATE.H"
#include "RUNTIME_INTERFACES.H"
#include "IRQ.H"
#include "IO_REG.H"

/* Reads the newest copy of record 3's summary into the workspace, with the
   four bytes its second sector keeps at 0x110; 1 when there is one. */
s32 SaveState_LoadExtraSummary(void)
{
    struct SaveWorkspace *work;
    struct SaveSummary *summary;
    volatile u32 zero;
    u32 index;

    work = Ram_HeapSlots->serial_work;
    summary = &work->extra;
    zero = 0;
    Dma_Set((const void *)&zero, summary, 0x85000010, REG_DMA3);
    index = SaveState_FindLatestSlot(3);
    if (index <= 14) {
        ReadFlash((u16)index, 0, (u8 *)summary, sizeof(*summary));
        ReadFlash((u16)(index + 1), 0x110, work->extra.unknown_38, sizeof(work->extra.unknown_38));
        return 1;
    }
    return 0;
}

/* Removes the save's serial interrupt handler and frees its workspace. */
void SaveState_ReleaseWorkspace(void)
{
    Runtime_SetIrqHandler(5, 0, NULL);
    Runtime_ReleaseHeapBlock(204);
}
