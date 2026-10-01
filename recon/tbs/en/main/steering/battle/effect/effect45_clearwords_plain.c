/* NONMATCHING: 2026-10-01 brief Wave2 ClearWords plain-source attempt.
 * Removing this one source device changes BattleFx_BeginCanvasLayer, BattleFx_OpenCanvasLayer.
 * First remaining difference: BattleFx_BeginCanvasLayer: mov	r1, #128 => mov	r5, #128 (342/344 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * This reduced draft preserves the affected function and its declarations.
 * Production retains the measured device with its FAKEMATCH reason.
 */
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
#include "RAM_BUFFER.H"

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


void BattleFx_BeginCanvasLayer(s32 bg_control);

void Audio_PlayCue(s32);
void WaitFrames(s32);
void Func_080b5048(u16, s32);

struct Cells03001ce0 {
    s32 unk00[4];
    s32 unk10;
};

extern struct Cells03001ce0 gProjection;

void QueueIoWriteDelay2(u32 first, u32 second);

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

    Iwram_ClearWords(canvas, 0x4000);
    Iwram_ClearWords((void *)0x06004000, 0x4000);
    *(s32 *)(work + 0x77a8) = 0;
    *(s32 *)(work + 0x77a0) = gBgScroll.unk04;
    *(s32 *)(work + 0x77a4) = gBgScroll.unk06;
    WaitFrames(1);
}

#undef QUEUE_DISPLAY_CONTROL
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

/* Battle effect: open a canvas layer over the battle scene. The variant of
   BattleFx_BeginCanvasLayer that fades the battle palette only to level 128,
   starts the palette fade transfer at 24 frames and queues the display
   without the windows until the canvas is set up; it lays out the same 16 by
   16 canvas tile map (two tile numbers per halfword) and clears the canvas
   buffers.

   FAKEMATCH: each queued display-control write is QueueIoWriteDelay2
   (SYSTEM/IO_WRITE_QUEUE.C) written out inline with that function's odd
   constructs, the one-pass loop around the IME read and the count stored
   through an explicit u16 pointer; the queue and IME pointers are held for
   the whole function as the ROM keeps them. */
void BattleFx_OpenCanvasLayer(s32 bg_control)
;

#undef QUEUE_DISPLAY_CONTROL

/* Battle effect: open the canvas layer with a 16 by 16 tile canvas on a
   32-wide map. The left half of each map row holds the canvas tile pairs,
   the right half is cleared. */
void BattleFx_BeginTiledCanvas(s32 bg_control)
;

#define QUEUE_DISPLAY_CONTROL(control) {                                    \
        volatile u16 *ime;                                                  \
        struct IoWriteQueue *q;                                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        q = &gIoWriteQueue;                                                 \
        do {                                                                \
            do {                                                            \
                ime = &REG_IME;                                             \
                saved = *ime;                                               \
            } while (0);                                                    \
            *ime = (u16)ime;                                                \
            count = q->count;                                               \
            if (count <= 31) {                                              \
                u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);       \
                *(u16 *)&q->count = count + 1;                              \
                *destination++ = (control);                                 \
                *destination++ = 0x04000000;                                \
                *destination = 0x20000;                                     \
            }                                                               \
            *ime = saved;                                                   \
        } while (0);                                                        \
    }

/* Battle effect: close the canvas layer an effect opened. Restore the scroll
   pair saved in the work block, recentre the projection, clear the canvas
   tiles, stop the canvas transfer callback, switch the display back through
   the next-frame queue, and fade the battle palette back in over eight
   frames.

   FAKEMATCH: each queued display-control write is QueueIoWriteDelay2
   (SYSTEM/IO_WRITE_QUEUE.C) written out inline with that function's odd
   constructs, the one-pass loop around the IME read and the count stored
   through an explicit u16 pointer, plus a one-pass loop around everything
   after the queue pointer so the queue literal loads ahead of the store
   before it. */
void BattleFx_EndCanvasLayer(void)
;

#undef QUEUE_DISPLAY_CONTROL

void BattleFx_SetTransitionFlagAndDisplay(void)
;
