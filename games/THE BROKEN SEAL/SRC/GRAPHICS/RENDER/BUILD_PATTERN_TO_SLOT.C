#include "TYPES.H"

s32 Runtime_ReleaseHeapBlock(s32);
s32 VramBlock_LoadCached(s32, s32, const void *);
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Ui_PrepareTransferFromTableEntry(u32 index);

s32 Ui_BuildPatternToSlot(s32 icon, s32 unused, s32 slot)
{
    u8 *work;

    work = Runtime_AllocateHeapBlock(0x11, 0x608);
    Ui_PrepareTransferFromTableEntry(icon);
    VramBlock_LoadCached(slot, 0x80, work + 0x400);
    Runtime_ReleaseHeapBlock(0x11);
    return 1;
}
