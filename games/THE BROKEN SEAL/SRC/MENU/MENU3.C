#include "TYPES.H"
#include "SCENE.H"
#include "DMA.H"
#include "SYSTEM.H"

void ShopCursor_AdvanceFar(void *);
void Ui_ApplyTableScaleToObject(struct Object *object);
extern u8 *gSelectionWork;
s32 Scheduler_AddOrUpdateCallback(void (*)(void), s32);
void Menu_RunSelectedWorkspaceEntry(void);

s32 Scheduler_RemoveCallback(s32);

void Menu_RunSelectedWorkspaceEntry(void);

void Menu_RunSelectedWorkspaceEntry(void)
{
    u8 *base = gSelectionWork;
    u32 index;

    ShopCursor_AdvanceFar(base + 0x5a4);
    index = *(u16 *)(base + 0x574);
    index *= 4;
    index += 0x610;
    Ui_ApplyTableScaleToObject(*(void **)(base + index));
}

void Menu_InitializeSelectedWorkspace(void)
{
    void *work;
    volatile u32 zero;
    work = Runtime_AllocateBlock(20, 0x628);
    zero = 0;
    Dma_Set(&zero, work, 0x8500018a, (volatile u32 *)0x040000d4);
    Scheduler_AddOrUpdateCallback(Menu_RunSelectedWorkspaceEntry, 3200);
}

void Runtime_ScheduleCallbackAndReleaseBlock20A(void)
{
    Scheduler_RemoveCallback((s32)&Menu_RunSelectedWorkspaceEntry);
    Runtime_ReleaseHeapBlock(0x14);
}
