#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "irqlock.h"
#include "iwram_call.h"

static __inline__ void CopyWords_(void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as the scene layer does: called directly, CSE shares one 0x1000 in r6 across the first call (differ 16); Func_08143000 called directly differs 58. */
    Iwram_CopyWords(destination, source, size);
}

static __inline__ void FillWords_(void *destination, s32 size, u32 value)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as the scene layer does. */
    Iwram_FillWords(destination, size, value);
}

// The battle effect work, as far as the canvas transfer reads it.
struct CanvasWork
{
    u8 unk0000[0x7780];
    s32 transferMode;
    s32 transferValue;
    u8 unk7788[0x15c];
    s32 framesSinceTransfer;
    s32 transferPending;
};

void ColorBuffer_Halve(void *source, void *destination, s32 size);
void ColorBuffer_ScaleThreeQuarters(void *source, void *destination, s32 size);
void ColorBuffer_Darken(void *source, s32 amount, void *destination, s32 size);
void ColorBuffer_Brighten(void *source, s32 amount, void *destination, s32 size);

// The frame callback that copies the effect canvas to BG character block 1
// when a transfer is pending, in the way the transfer mode asks for.
void Func_08143000(void)
{
    struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
    void *source = Ram_HeapSlots->battleFxWork[1];
    u32 ime;

    IRQ_LOCK(ime);
    if (work->transferPending == 1)
    {
        work->transferPending = 0;
        IRQ_UNLOCK(ime);
        switch (work->transferMode)
        {
        case 0:
            CopyWords_((void *)0x06004000, source, 0x4000);
            break;
        case 1:
            CopyWords_((void *)0x06004000, source, 0x4000);
            FillWords_(source, 0x4000, work->transferValue);
            break;
        case 2:
            if (work->transferValue == 50)
                ColorBuffer_Halve(source, (void *)0x06004000, 0x4000);
            else
                ColorBuffer_ScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
            break;
        case 3:
            ColorBuffer_Darken(source, work->transferValue, (void *)0x06004000, 0x4000);
            break;
        case 4:
            ColorBuffer_Brighten(source, work->transferValue, (void *)0x06004000, 0x4000);
            break;
        }
        /* FAKEMATCH: the native reads the counter before it stores 1, a load nothing uses (ldr r2, [r3]; movs r2, #1); a plain store leaves it out and the two counter updates share one store, which moves the literal pool (differ 38). */
        (void)*(volatile s32 *)&work->framesSinceTransfer;
        work->framesSinceTransfer = 1;
    }
    else
    {
        IRQ_UNLOCK(ime);
        work->framesSinceTransfer++;
    }
}

void Func_08143114(void)
{
    u8 *work = Ram_HeapSlots->battleFxWork[0];
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

// The same callback for the 0x7800-byte canvas in the map cell buffer, copied to
// BG character block 2.
void Func_08143174(void)
{
    struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
    void *source = Ram_MapCellBuffer;
    u32 ime;

    IRQ_LOCK(ime);
    if (work->transferPending == 1)
    {
        work->transferPending = 0;
        IRQ_UNLOCK(ime);
        switch (work->transferMode)
        {
        case 0:
            CopyWords_((void *)0x06008000, source, 0x7800);
            break;
        case 1:
            CopyWords_((void *)0x06008000, source, 0x7800);
            FillWords_(source, 0x7800, work->transferValue);
            break;
        case 2:
            if (work->transferValue == 50)
                ColorBuffer_Halve(source, (void *)0x06008000, 0x7800);
            else
                ColorBuffer_ScaleThreeQuarters(source, (void *)0x06008000, 0x7800);
            break;
        case 3:
            ColorBuffer_Darken(source, work->transferValue, (void *)0x06008000, 0x7800);
            break;
        }
        /* FAKEMATCH: the dead read before the counter is set to 1, as in Func_08143000. */
        (void)*(volatile s32 *)&work->framesSinceTransfer;
        work->framesSinceTransfer = 1;
    }
    else
    {
        IRQ_UNLOCK(ime);
        work->framesSinceTransfer++;
    }
}

// The same for the canvas in the second effect work block.
void Func_08143264(void)
{
    struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
    void *source;
    u32 ime;

    IRQ_LOCK(ime);
    if (work->transferPending == 1)
    {
        source = Ram_HeapSlots->battleFxWork[1];
        work->transferPending = 0;
        IRQ_UNLOCK(ime);
        switch (work->transferMode)
        {
        case 0:
            CopyWords_((void *)0x06008000, source, 0x7800);
            break;
        case 1:
            CopyWords_((void *)0x06008000, source, 0x7800);
            FillWords_(source, 0x3c00, work->transferValue);
            break;
        case 2:
            if (work->transferValue == 50)
                ColorBuffer_Halve(source, (void *)0x06008000, 0x3c00);
            else
                ColorBuffer_ScaleThreeQuarters(source, (void *)0x06008000, 0x3c00);
            break;
        case 3:
            ColorBuffer_Darken(source, work->transferValue, (void *)0x06008000, 0x3c00);
            break;
        }
        /* FAKEMATCH: the dead read before the counter is set to 1, as in Func_08143000. */
        (void)*(volatile s32 *)&work->framesSinceTransfer;
        work->framesSinceTransfer = 1;
    }
    else
    {
        IRQ_UNLOCK(ime);
        work->framesSinceTransfer++;
    }
}
