#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"

void *Runtime_AllocateHeapBlock(s32 kind, s32 size);
void Battle_ResetEffectCounterFar(void);
u8 Party_ListActiveOwnersFar(void *);
s32 find_free_slot(void);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *src);

/* Shop work block (heap kind 55): clear it, set up the cursor state at
   +0x380 and cache the six cursor sprite frames before the cursor task
   Shop_StepCursor starts. */
void Shop_InitializeCursorWork(void)
{
    u8 *work;
    volatile s32 zero;
    s32 slot;

    work = Runtime_AllocateHeapBlock(55, 0xa70);
    Battle_ResetEffectCounterFar();
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x8500029c, (volatile u32 *)0x040000d4);
    work[0x3a8] = 12;
    work[0x3a7] = Party_ListActiveOwnersFar(work + 0x36e);
    slot = find_free_slot();
    *(u16 *)(work + 0x390) = slot;
    VramBlock_LoadCached(slot, 128, (const void *)0x080b3940);
    slot = find_free_slot();
    *(u16 *)(work + 0x392) = slot;
    VramBlock_LoadCached(slot, 128, (const void *)0x080b3b40);
    slot = find_free_slot();
    *(u16 *)(work + 0x394) = slot;
    VramBlock_LoadCached(slot, 128, (const void *)0x080b3bc0);
    slot = find_free_slot();
    *(u16 *)(work + 0x396) = slot;
    VramBlock_LoadCached(slot, 128, (const void *)0x080b39c0);
    slot = find_free_slot();
    *(u16 *)(work + 0x39a) = slot;
    VramBlock_LoadCached(slot, 128, (const void *)0x080b3a40);
    slot = find_free_slot();
    *(u16 *)(work + 0x398) = slot;
    VramBlock_LoadCached(slot, 128, (const void *)0x080b3ac0);
    Scheduler_AddOrUpdateCallback(0x080b00f5, 0xc80);
}
