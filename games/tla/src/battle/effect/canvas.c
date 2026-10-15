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

// The battle effect work, as far as the canvas layer reads it.
struct CanvasWork
{
    u8  unk0000[0x77a0];
    s32 savedBg1X;
    s32 savedBg1Y;
    s32 shakeFrames;
    u8  unk77ac[8];
    s32 fadeFrames;
    s32 fadeStep;
    u8  unk77bc[0x12c];
    s32 unk78e8;
};

extern u32 gCameraSceneParameters[];

void Audio_PlayCue(s32 cue);
void Func_081434d8(void);
void Func_08143488(void);
void Func_0814355c(void);
void BattleFx_BeginCanvasLayer(s32 bg_control);
void BattlePresentation_ConfigurePaletteFadeFar(s32 mode, s32 background, s32 value);
void BattlePresentation_SetPaletteLevelFar(s32 background, s32 level);
void BattlePres_SetupTransitionSceneFar(s32 a, s32 b, s32 c, s32 d);

static __inline__ s32 ClearWords_(void *destination, s32 size)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants and loads the routine once into a callee-saved register, as the native does: called directly, the two clears share one 0x4000 and the effect work's zeros reuse an earlier register (differ 24 in BattleFx_OpenCanvasLayer). */
    return Iwram_ClearWords(destination, size);
}

// Opens the effect canvas layer: BG2 becomes a 2x-wide 128x128 canvas of
// 16x16 characters, blended over the battle background, whose palette
// starts fading.
void BattleFx_OpenCanvasLayer(s32 bgControl)
{
    struct CanvasWork           *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
    struct BattleWork           *battle = Ram_HeapSlots->battleWork;
    void                        *canvas = Ram_HeapSlots->battleFxWork[1];
    struct BattleBackgroundView *display =
        (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
    s32 row;
    s32 col;
    s32 offset;

    Func_081434d8();
    display->secondMode = 1;
    WaitFrames(1);
    {
        /* FAKEMATCH: the native loads the zero from the pool before it starts the register address, as in Func_08143bb8: pinned to the r3 it takes behind a barrier that holds r2. The plain store starts the address first (differ 2), as does the pin without its barrier. */
        register u16         zero asm("r3") = 0;
        struct IoWriteQueue *q;
        u32                  saved;
        s32                  count;
        u16                 *count_;

        /* FAKEMATCH: the barrier of the pin above. */
        __asm__("" : "+l"(zero) : : "r2");
        /* FAKEMATCH: IO_WRITE_QUEUE_PUSH written out, so that the queue's address is taken before the BLDCNT store: the native loads it between the halves of the register address; through the macro, or with the store after the queue's address as the macro orders them, it loads after them (differ 2). */
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
    BattlePresentation_ConfigurePaletteFadeFar(1, *(u16 *)battle->unk670, 128);
    work->fadeFrames = 24;
    work->fadeStep = 0;
    Scheduler_AddOrUpdateCallback(Func_08143488, 0x480);
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x1341, 0x20000);
    WaitFrames(1);
    REG_BG2CNT = bgControl | 0x784;
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x1341, 0x20000);
    BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
    display->secondMode = 0;
    WaitFrames(1);

    Func_0814355c();
    {
        /* FAKEMATCH: the native loads WININ's value from the pool before it starts the register address; plainly, or pinned without the barrier, the address starts first (differ 2). Pinned to the r2 it takes, behind a barrier that holds r3. */
        register u16 inside asm("r2") = 0x3537;

        /* FAKEMATCH: the barrier of the pin above. */
        __asm__("" : "+l"(inside) : : "r3");
        REG_WININ = inside;
    }
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7741, 0x20000);
    WaitFrames(1);

    // The canvas map: 16 rows of 8 character pairs.
    offset = 0;
    for (row = 0; row != 16; row++)
    {
        for (col = 0; col != 8; col++, offset += 2)
        {
            s32 chr;
            s16 pair;

            chr = row * 16 + col * 2;
            pair = ((chr + 1) << 8) | chr;
            *(volatile s16 *)((u8 *)BG_CANVAS_MAP + offset) = pair;
        }
    }

    ClearWords_(canvas, 0x4000);
    ClearWords_(BG_CHAR_BLOCK(1), 0x4000);
    work->shakeFrames = 0;
    work->unk78e8 = 0;
    WaitFrames(1);
}

void BattleFx_BeginTiledCanvas(s32 bg_control)
{
    s32 row;
    s32 col;
    s32 offset;

    BattleFx_BeginCanvasLayer(bg_control);
    REG_BG2CNT = bg_control | 0x4784;
    offset = 0;
    for (row = 0; row != 16; row++)
    {
        for (col = 0; col != 8; col++, offset += 2)
        {
            s32 tile;
            s16 pair;

            tile = row * 16 + col * 2;
            pair = ((tile + 1) << 8) | tile;
            *(volatile s16 *)(0x06003800 + offset) = pair;
        }
        for (col = 0; col != 8; col++, offset += 2)
        {
            *(volatile u16 *)(0x06003800 + offset) = 0;
        }
    }
}

void BattleFx_BeginTiledCanvasFilled(s32 bg_control)
{
    s32 row;
    s32 col;
    s32 offset;

    BattleFx_BeginCanvasLayer(bg_control);
    REG_BG2CNT = bg_control | 0x4784;
    offset = 0;
    for (row = 0; row != 16; row++)
    {
        for (col = 0; col != 8; col++, offset += 2)
        {
            s32 tile;
            s16 pair;

            tile = row * 16 + col * 2;
            pair = ((tile + 1) << 8) | tile;
            *(volatile s16 *)(0x06003800 + offset) = pair;
        }
        for (col = 0; col != 8; col++, offset += 2)
        {
            *(volatile u16 *)(0x06003800 + offset) = 0xf0f;
        }
    }
}

// Closes the effect canvas layer: BG1 back where it was, BG2 and its
// characters cleared, then the background palette faded back in.
void Func_08143bb8(void)
{
    struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
    struct BattleWork *battle = Ram_HeapSlots->battleWork;
    s32                i;

    Audio_PlayCue(390);
    gBgScroll[1].x = work->savedBg1X;
    gBgScroll[1].y = work->savedBg1Y;
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
    {
        /* FAKEMATCH: the native loads the control word from the pool before it starts the register address; the plain store starts the address first (differ 2), as does a pin without the barrier. Pinned to the r3 it takes, as in f18284c.c. */
        register u16 control asm("r3") = 0x787;

        /* FAKEMATCH: the barrier of the pin above: it holds r2 until the value is loaded. */
        __asm__("" : "+l"(control) : : "r2");
        REG_BG2CNT = control;
    }
    Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
    Scheduler_RemoveCallback(Func_08143488);
    gBgScroll[1].y = 32;
    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7341, 0x20000);
    /* FAKEMATCH: the native loads the zero from the pool between the first two instructions of the register address; after the push (whose IME restore is a one-pass block) the plain store loads it one instruction later (differ 2), as does a pinned zero with or without its own barrier. With the push written out and a plain restore, the load falls right but the restore sinks below the address. */
    __asm__ volatile("");
    REG_BLDCNT = 0;
    WaitFrames(1);

    BattlePresentation_ConfigurePaletteFadeFar(2, *(u16 *)battle->unk670, 7);
    WaitFrames(1);
    for (i = 0; i != 8; i++)
    {
        BattlePresentation_SetPaletteLevelFar(*(u16 *)battle->unk670, 21 - i * 3);
        WaitFrames(1);
    }

    IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT, 0x7541, 0x20000);
    WaitFrames(1);
}

// Hides the canvas layer: marks the background view, queues DISPCNT 0x1541,
// fades the background palette back and queues BG1's control word.
void Func_08143d04(void)
{
    struct BattleWork *battle;
    s32               *flag;

    flag = &((struct BattleBackgroundView *)Ram_HeapSlots->transitionWork)->secondMode;
    battle = Ram_HeapSlots->battleWork;
    *flag = 1;
    QueueIoWriteDelay2((u32)&REG_DISPCNT, 0x1541);
    WaitFrames(1);
    BattlePresentation_ConfigurePaletteFadeFar(2, *(u16 *)battle->unk670, 0);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT, 0x1f83, 0x20000);
    WaitFrames(1);
}
