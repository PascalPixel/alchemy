/* Draft, not exact (2026-09-25): 784 of 784 bytes, 182 halfwords differ, all register choice.
   Battle canvas effect: a 128x128 radial distance map (IWRAM square root), a 63-entry
   palette ramp, then 96 frames of a sine-wobbled scanline table while the palette rotates
   through DMA and the next-frame I/O queue. Shape, pools and literal placement match; the
   frame loop is a goto loop so the 0x04000052 stores keep their per-use literal, and pal
   is a variable so its store rematerialises 0x02010000 + 2. Residuals: the reference
   still passes the DMA destination as mov r1, r9 (here the constant reloads), prefers r5
   for its short-lived temporaries, and loads -128 fresh after the distance loop where
   this build derives it from the inner -1. */
#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "DMA.H"
#include "IO_WRITE_QUEUE.H"

extern volatile u16 Data_04000208;
#define REG_IME Data_04000208

void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_PrepareCanvasEffect(void *object, s32 a, s32 b, s32 c, s32 *out_a, s32 *out_b);
void BattleFx_FetchRectangleBlitters(s32 flag, DrawRectangleFn *out_callbacks);
void BattleFx_EndCanvasLayer(void);

void Func_080e6638(void *object)
{
    void **heap_cache;
    u8 *work;
    void *canvas;
    s32 screen_y;
    s32 screen_x;
    DrawRectangleFn routine[2];
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
    s32 cy;
    s32 d;
    s32 i;
    s32 t;
    s32 r;
    s32 g;
    s32 b;
    s32 color;
    u16 *shadow;
    s32 frame;
    s32 wave;
    s32 level;
    u16 *last;
    u16 *pal;
    volatile u16 *ime;
    s32 angle;
    s32 *line;

    heap_cache = (void **)0x03001eec;
    work = *heap_cache++;
    canvas = *heap_cache;
    *(void **)(work + 0x7828) = object;
    BattleFx_BeginCanvasLayer(0x2000);
    BattleFx_PrepareCanvasEffect(object, 6, *(s32 *)(*(u8 **)(work + 0x7828) + 4), 2, &screen_x, &screen_y);
    *(u16 *)0x0400000c = 0x2784;
    *(u16 *)0x04000052 = 0x1000;
    *(u16 *)0x04000020 = 0xaa;
    BattleFx_FetchRectangleBlitters(*(s32 *)(*(u8 **)(work + 0x7828) + 4), routine);
    *(s32 *)(work + 0x7780) = 2;
    *(s32 *)(work + 0x7784) = 75;
    Scheduler_AddOrUpdateCallback(0x080cd261, 0x480);

    for (y = 0; y != 64; y++) {
        for (x = 0; x != 64; x++) {
            cy = y / 8 + 64;
            dy = y - cy;
            dx = x - 64;
            d = ((s32 (*)(s32))0x030001d8)(dx * dx + dy * dy);
            d /= 2;
            if (d == 0)
                d = 1;
            if (d > 63)
                d = 63;
            work[y * 128 + x] = d;
            work[y * 128 + 127 - x] = d;
            work[(127 - y) * 128 + x] = d;
            work[(127 - y) * 128 + 127 - x] = d;
        }
    }

    shadow = (u16 *)0x02010002;
    for (i = 1; i != 64; i++) {
        if (i > 31)
            t = 64 - i;
        else
            t = i;
        r = t * 9;
        g = t * 7 - 42;
        b = t * 7 - 56;
        if (r < 0)
            r = 0;
        if (g < 0)
            g = 0;
        if (b < 0)
            b = 0;
        if (r > 255)
            r = 255;
        if (g > 255)
            g = 255;
        if (b > 250)
            b = 250;
        color = (b >> 3) << 10 | (g >> 3) << 5 | r >> 3;
        ((u16 *)0x05000000)[i] = color;
        *shadow++ = color;
    }

    routine[0](canvas, work, 0, 0, 128, 128);
    *(s32 *)(work + 0x7824) = 1;
    Scheduler_AddOrUpdateCallback(0x080dbb9d, 0x480);

    pal = (u16 *)0x02010000;
    last = (u16 *)0x0201007e;
    ime = &REG_IME;
    frame = 0;
    level = 0;
loop:
    {
        if (frame <= 8) {
            wave = level;
            *(u16 *)0x04000052 = level | 0x1000;
        } else {
            wave = frame * 2;
        }
        if (frame > 88)
            *(u16 *)0x04000052 = (0xc0 - level) | 0x1000;

        line = (s32 *)(work + 0x6980);
        angle = -(wave << 9);
        for (i = 0; i != 160; i++) {
            *line++ = ((i << 18) - (Trig_Sin(angle) << 7) + 0x40000) >> 10;
            angle += 0x200;
        }

        if (frame > 127) {
            *(s32 *)(work + 0x7824) = 1;
        } else {
            pal[1] = *last;
            Dma_Set((void *)0x0201007c, last, 0x80a0003e, (volatile u32 *)0x040000d4);
            {
                struct IoWriteQueue *q;
                u32 saved;
                s32 count;

                q = &gIoWriteQueue;
                saved = *ime;
                *ime = (u16)ime;
                count = q->count;
                if (count <= 31) {
                    u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);
                    q->count = count + 1;
                    *destination++ = 0x02010002;
                    *destination++ = 0x05000002;
                    *destination = 0x8000003f;
                }
                *ime = saved;
            }
        }
        WaitFrames(1);
        frame++;
        level += 2;
        if (frame != 96)
            goto loop;
    }

    Scheduler_RemoveCallback(0x080dbb9d);
    Scheduler_RemoveCallback(0x080cd261);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
