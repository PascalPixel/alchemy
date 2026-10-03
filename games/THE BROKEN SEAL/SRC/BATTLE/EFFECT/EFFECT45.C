#include "MAP_SCROLL.H"
#include "BATTLE_PRESENTATION.H"
#include "BATTLE_WORK.H"
#include "BATTLE_EFFECT_WORK.H"
#include "PROJECT.H"
/* Battle effect: open the canvas layer an effect draws into, the counterpart
   of BattleFx_EndCanvasLayer. Save the scroll pair in the work block, fade
   the battle palette out, start the palette fade transfer, switch BG2 to the
   affine canvas through the next-frame queue, set the identity projection,
   window and blend registers, lay out the 16 by 16 canvas tile map (two tile
   numbers per halfword) and clear the canvas buffers. */
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


typedef s32 (*ClearWordsFn)(void *destination, s32 size);

static __inline__ void ClearWords(ClearWordsFn clear, void *destination, s32 size)
{
    /* FAKEMATCH: direct calls share the size in r5 and move the routine to r6, changing the canvas tails. */
    clear(destination, size);
}

void BattleFx_BeginCanvasLayer(s32 bg_control);

void Audio_PlayCue(s32);
void WaitFrames(s32);
void Func_080b5048(u16, s32);




void QueueIoWriteDelay2(u32 first, u32 second);

#define QUEUE_DISPLAY_CONTROL(control) do { \
    /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        do { \
            /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
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
    /* FAKEMATCH: each queued display-control write is QueueIoWriteDelay2
     * (SYSTEM/IO_WRITE_QUEUE.C) written out inline with that function's odd
     * constructs, the one-pass loop around the IME read and the count stored
     * through an explicit u16 pointer; the queue and IME pointers are held for
     * the whole function as the ROM keeps them. */

    void **cache = (void **)(gWorkSlot + 39 * 4);
    struct BattleEffectWork *work = cache[0];
    struct BattleSession *battle = *(struct BattleSession **)(gWorkSlot + 9 * 4);
    void *canvas = cache[1];
    struct BattleBackgroundView *display = cache[5];
    volatile u16 *ime;
    struct IoWriteQueue *q;
    s32 row;
    s32 col;
    s32 offset;

    Runtime_ApplyValueToWork7818();
    display->second_mode = 1;
    WaitFrames(1);
    REG_BLDCNT = 0;
    q = &gIoWriteQueue;
    ime = &REG_IME;
    QUEUE_DISPLAY_CONTROL(0x7741);
    gBgScroll[1].y = 32;
    WaitFrames(1);
    BattlePresentation_ConfigurePaletteFadeFar(1, battle->background, 0);
    work->fade_frames = 0;
    work->fade_step = 0;
    Scheduler_AddOrUpdateCallback((s32)Palette_StepFadeTransfer, 0xc80);
    QUEUE_DISPLAY_CONTROL(0x7341);
    WaitFrames(1);
    REG_BG2CNT = bg_control | 0x784;
    QUEUE_DISPLAY_CONTROL(0x7341);
    Func_080b5028(0, 0, 0, 100);
    display->second_mode = 0;
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
    work->shake_frames = 0;
    work->saved_bg1_x = gBgScroll[1].x;
    work->saved_bg1_y = gBgScroll[1].y;
    WaitFrames(1);
}

#undef QUEUE_DISPLAY_CONTROL
#define QUEUE_DISPLAY_CONTROL(control) do { \
    /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        do { \
            /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
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
   buffers. */
void BattleFx_OpenCanvasLayer(s32 bg_control)
{
    /* FAKEMATCH: each queued display-control write is QueueIoWriteDelay2
     * (SYSTEM/IO_WRITE_QUEUE.C) written out inline with that function's odd
     * constructs, the one-pass loop around the IME read and the count stored
     * through an explicit u16 pointer; the queue and IME pointers are held for
     * the whole function as the ROM keeps them. */

    void **cache = (void **)(gWorkSlot + 39 * 4);
    struct BattleEffectWork *work = cache[0];
    struct BattleSession *battle = *(struct BattleSession **)(gWorkSlot + 9 * 4);
    void *canvas = cache[1];
    struct BattleBackgroundView *display = cache[5];
    volatile u16 *ime;
    struct IoWriteQueue *q;
    s32 row;
    s32 col;
    s32 offset;

    Runtime_ApplyValueToWork7818();
    display->second_mode = 1;
    WaitFrames(1);
    *(volatile u16 *)0x04000050 = 0;
    q = &gIoWriteQueue;
    ime = &REG_IME;
    QUEUE_DISPLAY_CONTROL(0x1741);
    gBgScroll[1].y = 32;
    WaitFrames(1);
    BattlePresentation_ConfigurePaletteFadeFar(1, battle->background, 128);
    work->fade_frames = 24;
    work->fade_step = 0;
    Scheduler_AddOrUpdateCallback((s32)Palette_StepFadeTransfer, 0xc80);
    QUEUE_DISPLAY_CONTROL(0x1341);
    WaitFrames(1);
    *(volatile u16 *)0x0400000c = bg_control | 0x784;
    QUEUE_DISPLAY_CONTROL(0x1341);
    Func_080b5028(0, 0, 0, 100);
    display->second_mode = 0;
    WaitFrames(1);

    *(volatile u16 *)0x04000050 = 0x3f44;
    *(volatile u16 *)0x04000052 = 0x100e;
    *(volatile u32 *)0x04000028 = 0;
    *(volatile s32 *)0x0400002c = -0x1000;
    *(volatile u16 *)0x04000020 = 0x80;
    *(volatile u16 *)0x04000022 = 0;
    *(volatile u16 *)0x04000024 = 0;
    *(volatile u16 *)0x04000026 = 0x100;
    *(volatile u16 *)0x04000040 = 0xf0;
    *(volatile u16 *)0x04000044 = 0x1088;
    *(volatile u16 *)0x04000042 = 0xf0;
    *(volatile u16 *)0x04000046 = 0x1088;
    *(volatile u16 *)0x04000048 = 0x3537;
    *(volatile u16 *)0x0400004a = 0x3f21;
    QUEUE_DISPLAY_CONTROL(0x7741);
    WaitFrames(1);

    offset = 0;
    for (row = 0; row != 16; row++) {
        for (col = 0; col != 8; col++, offset += 2) {
            s32 tile;
            s16 pair;

            tile = row * 16 + col * 2;
            pair = ((tile + 1) << 8) | tile;
            *(volatile s16 *)(0x06003800 + offset) = pair;
        }
    }

    ClearWords(Iwram_ClearWords, canvas, 0x4000);
    ClearWords(Iwram_ClearWords, (void *)0x06004000, 0x4000);
    WaitFrames(1);
}

#undef QUEUE_DISPLAY_CONTROL

/* Battle effect: open the canvas layer with a 16 by 16 tile canvas on a
   32-wide map. The left half of each map row holds the canvas tile pairs,
   the right half is cleared. */
void BattleFx_BeginTiledCanvas(s32 bg_control)
{
    s32 row;
    s32 col;
    s32 offset;

    BattleFx_BeginCanvasLayer(bg_control);
    *(volatile u16 *)0x0400000c = bg_control | 0x6784;
    offset = 0;
    for (row = 0; row != 16; row++) {
        for (col = 0; col != 8; col++, offset += 2) {
            s32 tile;
            s16 pair;

            tile = row * 16 + col * 2;
            pair = ((tile + 1) << 8) | tile;
            *(volatile s16 *)(0x06003800 + offset) = pair;
        }
        for (col = 0; col != 8; col++, offset += 2) {
            *(volatile u16 *)(0x06003800 + offset) = 0;
        }
    }
}

#define QUEUE_DISPLAY_CONTROL(control) {                                    \
        volatile u16 *ime;                                                  \
        struct IoWriteQueue *q;                                             \
        u32 saved;                                                          \
        s32 count;                                                          \
                                                                            \
        q = &gIoWriteQueue;                                                 \
        do { \
            /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
            do { \
                /* FAKEMATCH: removing this one-pass boundary changes measured instruction scheduling. */ \
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
   frames. */
void BattleFx_EndCanvasLayer(void)
{
    /* FAKEMATCH: each queued display-control write is QueueIoWriteDelay2
     * (SYSTEM/IO_WRITE_QUEUE.C) written out inline with that function's odd
     * constructs, the one-pass loop around the IME read and the count stored
     * through an explicit u16 pointer, plus a one-pass loop around everything
     * after the queue pointer so the queue literal loads ahead of the store
     * before it. */

    struct BgScroll *scroll;
    struct BattleEffectWork *work = *(struct BattleEffectWork **)(gWorkSlot + 39 * 4);
    struct BattleSession *battle = *(struct BattleSession **)(gWorkSlot + 9 * 4);
    s32 i;

    Audio_PlayCue(0x121);
    scroll = &gBgScroll[1];
    scroll->x = work->saved_bg1_x;
    scroll->y = work->saved_bg1_y;
    gProjection.center_x = 120;
    gProjection.center_y = 120;
    *(volatile u16 *)0x0400000c = 0x787;
    Iwram_ClearWords((void *)0x06004000, 0x4000);
    Scheduler_RemoveCallback((s32)Palette_StepFadeTransfer);
    scroll->y = 32;
    QUEUE_DISPLAY_CONTROL(0x7341);
    *(volatile u16 *)0x04000050 = 0;
    WaitFrames(1);

    BattlePresentation_ConfigurePaletteFadeFar(2, battle->background, 7);
    WaitFrames(1);
    for (i = 0; i != 8; i++) {
        Func_080b5048(battle->background, 21 - i * 3);
        WaitFrames(1);
    }

    QUEUE_DISPLAY_CONTROL(0x7541);
    WaitFrames(1);
}

#undef QUEUE_DISPLAY_CONTROL

void BattleFx_SetTransitionFlagAndDisplay(void)
{
  struct BattleSession *state;
  s32 one;
  s32 transfer;
  s32 *flag;

  flag = &((struct BattleBackgroundView *)Ram_WorkSlot[44])->second_mode;
  state = Ram_WorkSlot[9];
  *flag = 1;
  transfer = 0x1541;
  QueueIoWriteDelay2(0x04000000, transfer);
  one = 1;
  WaitFrames(one);
  BattlePresentation_ConfigurePaletteFadeFar(2, state->background, 0);
  transfer = one;
  WaitFrames(transfer);
}
