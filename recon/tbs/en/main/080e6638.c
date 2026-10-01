/* Draft, not exact: 235 (3 register-only, 2 operand, 3 reordered), all in the
 * last loop. The reference loads the last ramp entry before the interrupt
 * mask register ahead of the loop (here the mask pointer is assigned first
 * and the ramp address hoisted after it), and loads the write queue before
 * reading the mask. What settled the rest: the palette entry is stored
 * through a chained assignment, so its address is live across the colour and
 * the 0x05000000 reload takes r5, which puts r5 in the reload rotation; the
 * ramp is one pointer variable; the two loops are plain for loops.
 * Tried for the order: an explicit pointer to the last entry before the
 * mask pointer (it loses r9 to the hoisted blend register address), and
 * the mask pointer assigned inside the loop with or without a direct
 * restore (the loop pass then leaves it in r0). The reference hoists both,
 * the ramp address first, and leaves the blend address without a register. */
#include "TYPES.H"
#include "DMA.H"
#include "IO_REG.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "BATTLE_PRESENTATION.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "IO_WRITE_QUEUE.H"

extern u8 gBattleFxWork[];

void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_PrepareCanvasEffect(void *object, s32 a, s32 b, s32 c, s32 *out_a, s32 *out_b);
void BattleFx_FetchRectangleBlitters(s32 alternate, DrawRectangleFn *output);
void BattleFx_ArmBg2AffineHBlankDma(void);
void BattleFx_EndCanvasLayer(void);

/*
 * Battle effect: a rippling disc. A 128 x 128 map of distances from a point
 * just below the centre is drawn once with a 63-colour ramp; for 96 frames
 * a sine wave bends the scanlines while the ramp rotates one colour a frame.
 */
void Func_080e6638(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    struct BattleEffectWork *work;
    void *canvas;
    s32 screen_y;
    s32 screen_x;
    DrawRectangleFn draw[2];
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
    s32 frame;
    s32 wave;
    s32 level;
    u16 *pal;
    volatile u16 *ime;
    s32 angle;
    s32 *line;

    heap_cache = (void **)gBattleFxWork;
    work = *heap_cache++;
    canvas = *heap_cache;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0x2000);
    BattleFx_PrepareCanvasEffect(effect, 6, work->effect->side, 2, &screen_x, &screen_y);
    REG_BG2CNT = 0x2784;
    REG_BLDALPHA = 0x1000;
    REG_BG2PA = 0xaa;
    BattleFx_FetchRectangleBlitters(work->effect->side, draw);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (y = 0; y != 64; y++) {
        for (x = 0; x != 64; x++) {
            cy = y / 8 + 64;
            dy = y - cy;
            dx = x - 64;
            d = Iwram_Sqrt(dx * dx + dy * dy);
            d /= 2;
            if (d == 0)
                d = 1;
            if (d > 63)
                d = 63;
            ((u8 *)work)[y * 128 + x] = d;
            ((u8 *)work)[y * 128 + 127 - x] = d;
            ((u8 *)work)[(127 - y) * 128 + x] = d;
            ((u8 *)work)[(127 - y) * 128 + 127 - x] = d;
        }
    }

    pal = (u16 *)Ram_MapCellBuffer;
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
        r >>= 3;
        g >>= 3;
        b >>= 3;
        pal[i] = ((u16 *)BG_PLTT)[i] = b << 10 | g << 5 | r;
    }

    draw[0](canvas, work, 0, 0, 128, 128);
    work->transfer_pending = 1;
    Scheduler_AddOrUpdateCallback((s32)BattleFx_ArmBg2AffineHBlankDma, 0x480);

    ime = &REG_IME;
    for (frame = 0, level = 0; frame != 96; frame++, level += 2) {
        if (frame <= 8) {
            wave = level;
            REG_BLDALPHA = level | 0x1000;
        } else {
            wave = frame * 2;
        }
        if (frame > 88)
            REG_BLDALPHA = (0xc0 - level) | 0x1000;

        line = work->bg2_x;
        for (i = 0, angle = -(wave << 9); i != 160; i++) {
            *line++ = ((i << 18) - (Trig_Sin(angle) << 7) + 0x40000) >> 10;
            angle += 0x200;
        }

        if (frame > 127) {
            work->transfer_pending = 1;
        } else {
            pal[1] = pal[63];
            Dma_Set(pal + 62, pal + 63, 0x80a0003e, REG_DMA3);
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
                    *destination++ = (u32)(pal + 1);
                    *destination++ = 0x05000002;
                    *destination = 0x8000003f;
                }
                *ime = saved;
            }
        }
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((s32)BattleFx_ArmBg2AffineHBlankDma);
    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
