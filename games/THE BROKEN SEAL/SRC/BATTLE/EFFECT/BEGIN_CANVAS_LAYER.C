/* Battle effect: open the canvas layer an effect draws into, the counterpart
   of BattleFx_EndCanvasLayer. Save the scroll pair in the work block, fade
   the battle palette out, start the palette fade transfer, switch BG2 to the
   affine canvas through the next-frame queue, set the identity projection,
   window and blend registers, lay out the 16 by 16 canvas tile map (two tile
   numbers per halfword) and clear the canvas buffers.

   FAKEMATCH: each queued display-control write is QueueIoWriteDelay2
   (SYSTEM/IO_WRITE_QUEUE.C) written out inline with that function's odd
   constructs, the one-pass loop around the IME read and the count stored
   through an explicit u16 pointer; the queue and IME pointers are held for
   the whole function as the ROM keeps them. */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SYSTEM.H"
#include "CALLBACK_SCHEDULER.H"
#include "IO_WRITE_QUEUE.H"
#include "IO_REG.H"

void Runtime_ApplyValueToWork7818(void);
void BattlePresentation_ConfigurePaletteFadeFar(s32, u16, s32);
void Func_080b5028(s32, s32, s32, s32);
void Palette_StepFadeTransfer(void);

extern u8 gWorkSlot[];

struct Cells03001ad0 {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
};
extern struct Cells03001ad0 gBgScroll;

typedef s32 (*ClearWordsFn)(void *destination, s32 size);

static __inline__ void ClearWords(ClearWordsFn clear, void *destination, s32 size)
{
    clear(destination, size);
}

#define QUEUE_DISPLAY_CONTROL(control) do {                                 \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        do {                                                                \
            saved = *ime;                                                   \
        } while (0);                                                        \
        *ime = (u16)ime;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            *(u16 *)&q->count = count + 1;                                  \
            *destination++ = (control);                                     \
            *destination++ = 0x04000000;                                    \
            *destination = 0x20000;                                         \
        }                                                                   \
        *ime = saved;                                                       \
    } while (0)

void BattleFx_BeginCanvasLayer(s32 bg_control)
{
    void **cache = (void **)(gWorkSlot + 39 * 4);
    u8 *work = cache[0];
    u8 *battle = *(u8 **)(gWorkSlot + 9 * 4);
    void *canvas = cache[1];
    u8 *display = cache[5];
    volatile u16 *ime;
    struct IoWriteQueue *q;
    s32 row;
    s32 col;
    s32 offset;

    Runtime_ApplyValueToWork7818();
    *(s32 *)(display + 12) = 1;
    WaitFrames(1);
    REG_BLDCNT = 0;
    q = &gIoWriteQueue;
    ime = &REG_IME;
    QUEUE_DISPLAY_CONTROL(0x7741);
    gBgScroll.unk06 = 32;
    WaitFrames(1);
    BattlePresentation_ConfigurePaletteFadeFar(1, *(u16 *)(battle + 0x648), 0);
    *(s32 *)(work + 0x77b4) = 0;
    *(s32 *)(work + 0x77b8) = 0;
    Scheduler_AddOrUpdateCallback((s32)Palette_StepFadeTransfer, 0xc80);
    QUEUE_DISPLAY_CONTROL(0x7341);
    WaitFrames(1);
    REG_BG2CNT = bg_control | 0x784;
    QUEUE_DISPLAY_CONTROL(0x7341);
    Func_080b5028(0, 0, 0, 100);
    *(s32 *)(display + 12) = 0;
    WaitFrames(1);

    REG_BLDCNT = 0x3f44;
    REG_BLDALPHA = 0x100e;
    REG_BG2X = 0;
    REG_BG2Y = -0x1000;
    REG_BG2PA = 0x80;
    REG_BG2PB = 0;
    REG_BG2PC = 0;
    REG_BG2PD = 0x100;
    REG_WIN0H = 0xf0;
    REG_WIN0V = 0x1088;
    REG_WIN1H = 0xf0;
    REG_WIN1V = 0x1088;
    REG_WININ = 0x3537;
    REG_WINOUT = 0x3f21;
    QUEUE_DISPLAY_CONTROL(0x7741);

    offset = 0;
    for (row = 0; row != 16; row++) {
        for (col = 0; col != 8; col++, offset += 2) {
            s32 tile;
            s16 pair;

            tile = row * 16 + col * 2;
            pair = ((tile + 1) << 8) | tile;
            *(volatile s16 *)((u8 *)BG_CANVAS_MAP + offset) = pair;
        }
    }

    ClearWords(Iwram_ClearWords, canvas, 0x4000);
    ClearWords(Iwram_ClearWords, (void *)0x06004000, 0x4000);
    *(s32 *)(work + 0x77a8) = 0;
    *(s32 *)(work + 0x77a0) = gBgScroll.unk04;
    *(s32 *)(work + 0x77a4) = gBgScroll.unk06;
    WaitFrames(1);
}
