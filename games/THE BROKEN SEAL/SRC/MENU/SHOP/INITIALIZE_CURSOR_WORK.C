#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"

void *Func_080048b0(s32 kind, s32 size);
#define Runtime_AllocateHeapBlock Func_080048b0
void Func_0808a460(void);
u8 Func_08077158(void *);
s32 Func_08004080(void);
#define find_free_slot Func_08004080
s32 Func_08003fa4(u32 slot, u32 size, const void *src);
#define VramBlock_LoadCached Func_08003fa4

/* Shop work block (heap kind 55): clear it, set up the cursor state at
   +0x380 and cache the six cursor sprite frames before the cursor task
   Shop_StepCursor starts. */
void Shop_InitializeCursorWork(void)
{
    u8 *work;
    volatile s32 zero;
    s32 slot;

    work = Runtime_AllocateHeapBlock(55, 0xa70);
    Func_0808a460();
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x8500029c, (volatile u32 *)0x040000d4);
    work[0x3a8] = 12;
    work[0x3a7] = Func_08077158(work + 0x36e);
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
