#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "irqlock.h"
#include "iwram_call.h"

static __inline__ void CopyWords_(void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as the scene layer does: called directly, CSE shares one 0x1000 in r6 across the first call (differ 16). */
    Iwram_CopyWords(destination, source, size);
}

static __inline__ void FillWords_(void *destination, s32 size, u32 value)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as the scene layer does. */
    Iwram_FillWords(destination, size, value);
}

void Func_08143114(void)
{
    u8 *work = Ram_HeapSlots->battle_fx_work[0];
    u32 ime;
    s32 *pending;

    IRQ_LOCK(ime);
    pending = (s32 *)(work + 0x78ec);
    if (*pending == 1) {
        *pending = 0;
        IRQ_UNLOCK(ime);
        CopyWords_(*(void **)(work + 0x7b74), work + 0x6780, 0x1000);
        FillWords_(work + 0x6780, 0x1000, 0);
    } else {
        IRQ_UNLOCK(ime);
    }
}
