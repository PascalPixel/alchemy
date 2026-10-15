/* Near miss, differ 10 of 270: The Broken Seal's BattleFx_BeginCanvasLayer with
   ⚓️'s Func_0813ba50 call, DISPCNT 0x1741, Func_08143488 at 0x480, Func_0814355c
   for the display registers and one more work word cleared. The BLDCNT and WININ
   rows take BattleFx_OpenCanvasLayer's devices (canvas.c), which match there.
   Left: the last block, where the native loads gBgScroll's address and both
   scroll halves one instruction earlier, each between the two halves of a work
   offset (movs r3, #240; ldr r1, =gBgScroll; lsls r3, r3, #7). Tried: the scroll
   pointer as a local first, in a one-pass block, the x half read ahead behind a
   loop note (differ 12 to 20); alchemy permute for 5 minutes (no better). */
#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "callback_scheduler.h"
#include "battle_work.h"
#include "battle_display.h"
#include "bg_scroll.h"

struct CanvasWork
{
    u8 unknown_0000[0x77a0];
    s32 savedBg1X;
    s32 savedBg1Y;
    s32 shakeFrames;
    u8 unknown_77ac[8];
    s32 fadeFrames;
    s32 fadeStep;
    u8 unknown_77bc[0x12c];
    s32 unknown_78e8;
};

static __inline__ s32 ClearWords_(void *destination, s32 size)
{
    return Iwram_ClearWords(destination, size);
}

void Func_0813ba50(void);
void Func_081434d8(void);
void Func_08143488(void);
void Func_0814355c(void);
void BattlePresentation_ConfigurePaletteFadeFar(s32 mode, s32 background, s32 value);
void BattlePres_SetupTransitionSceneFar(s32 a, s32 b, s32 c, s32 d);

void BattleFx_BeginCanvasLayer(s32 bgControl)
{
    struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battle_fx_work[0];
    struct BtlWork *btl = Ram_HeapSlots->battle_work;
    void *canvas = Ram_HeapSlots->battle_fx_work[1];
    struct BattleBackgroundView *display = (struct BattleBackgroundView *)Ram_HeapSlots->transition_work;
    s32 row;
    s32 col;
    s32 ofs;

    Func_0813ba50();
    Func_081434d8();
    display->second_mode = 1;
    WaitFrames(1);
    {
        register u16 zero asm("r3") = 0;
        struct IoWriteQueue *q;
        u32 saved;
        s32 count;
        u16 *count_;

        __asm__("" : "+l"(zero) : : "r2");
        q = &gIoWriteQueue;
        count_ = &q->count;
        REG_BLDCNT = zero;
        IRQ_LOCK(saved);
        count = *count_;
        if (count <= 31)
        {
            u32 *entry = q->entries[count];

            *count_ = count + 1;
            *entry++ = 0x1741;
            *entry++ = (u32)&REG_DISPCNT;
            *entry = 0x20000;
        }
        IRQ_UNLOCK(saved);
    }
    gBgScroll[1].y = 32;
    WaitFrames(1);

    BattlePresentation_ConfigurePaletteFadeFar(1, *(u16 *)btl->unknown_670, 0);
    work->fadeFrames = 0;
    work->fadeStep = 0;
    Scheduler_AddOrUpdateCallback(Func_08143488, 0x480);

    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
    WaitFrames(1);
    REG_BG2CNT = bgControl | 0x784;
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
    BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
    display->second_mode = 0;
    WaitFrames(1);

    Func_0814355c();
    {
        register u16 inside asm("r2") = 0x3537;

        __asm__("" : "+l"(inside) : : "r3");
        REG_WININ = inside;
    }
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7741, 0x20000);

    ofs = 0;
    for (row = 0; row != 16; row++)
    {
        for (col = 0; col != 8; col++, ofs += 2)
        {
            s32 chr;
            s16 pair;

            chr = row * 16 + col * 2;
            pair = ((chr + 1) << 8) | chr;
            *(volatile s16 *)((u8 *)BG_CANVAS_MAP + ofs) = pair;
        }
    }

    ClearWords_(canvas, 0x4000);
    ClearWords_(BG_CHAR_BLOCK(1), 0x4000);
    work->shakeFrames = 0;
    work->unknown_78e8 = 0;
    work->savedBg1X = gBgScroll[1].x;
    work->savedBg1Y = gBgScroll[1].y;
    WaitFrames(1);
}
