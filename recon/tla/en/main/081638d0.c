/* Draft, differ 6 of 354: The Broken Seal's BattleFx_RunRipplingDisc,
 * ported: the canvas setup through Func_0815585c(effect, side << 4 | 70),
 * the blitters from Func_08144aac, 80 frames (not 96) with the fade from
 * frame 72, a shake and actor motion at frame 48, the BG2 line offsets at
 * 0x7080 in the work, callbacks Func_08143000 and Func_08152474 at 0xc80.
 * The empty asm at the start stores the argument before 0x03000000 is
 * started; the BG2CNT halfword pinned in a one-pass block loads from the
 * pool first (as Func_0816a730); screen_x declared first takes sp+44.
 * Left, two reordered pairs: the native starts the third argument
 * (add r2, sp, #44) between the side's shift and its orrs, and in the I/O
 * queue push it increments the count before scaling the entry offset and
 * stores the count before adding the queue's base. Tried: the flags
 * written 70 | side << 4 or with 0x46 (6), the push written out with the
 * count stored first (6, the increment then takes r3), ++count (6), the
 * count store in a one-pass block (8). */
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "dma.h"
#include "system.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "io_write_queue.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_fx_work.h"

void BattleFx_BeginCanvasLayer(s32 layer);
void Func_0815585c(struct BattleEffectArgument *effect, s32 flags, s32 *b, s32 *a);
void Func_08144aac(s32 side, DrawRectangle *draw);
void Func_08143000(void);
void Func_08143bb8(void);
void Func_08152474(void);
void Func_081180e8(s32 phase);
void Func_08118088(s32 actor, s32 variant);
void Func_08158ce0(s32 x, s32 y);
void Func_081434f8(void);

void Func_081638d0(struct BattleEffectArgument *effect)
{
    struct BattleFxWork *work;
    void *canvas;
    s32 screen_x[3];
    s32 screen_y[3];
    DrawRectangle draw[2];
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
    s32 angle;
    s32 *line;

    __asm__("");
    work = (struct BattleFxWork *)Ram_HeapSlots->battle_fx_work[0];
    canvas = Ram_HeapSlots->battle_fx_work[1];
    BattleFx_BeginCanvasLayer(0x2000);
    Func_0815585c(effect, (effect->side << 4) | 70, screen_x, screen_y);
    {
        register u16 value __asm__("r3");

        do {
            value = 0x2784;
        } while (0);
        REG_BG2CNT = value;
    }
    REG_BLDALPHA = 0x1000;
    REG_BG2PA = 0xaa;
    Func_08144aac(effect->side, draw);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

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
            work->sheet[y * 128 + x] = d;
            work->sheet[y * 128 + 127 - x] = d;
            work->sheet[(127 - y) * 128 + x] = d;
            work->sheet[(127 - y) * 128 + 127 - x] = d;
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
        pal[i] = ((u16 *)0x05000000)[i] = b << 10 | g << 5 | r;
    }

    draw[0](canvas, work->sheet, 0, 0, 128, 128);
    work->transfer_pending = 1;
    Scheduler_AddOrUpdateCallback(Func_08152474, 0xc80);

    for (frame = 0; frame != 80; frame++) {
        if (frame <= 8) {
            level = frame * 2;
            wave = level;
            REG_BLDALPHA = level | 0x1000;
        } else {
            wave = frame * 2;
        }
        if (frame > 72)
            REG_BLDALPHA = (0xa0 - frame * 2) | 0x1000;
        if (frame == 48) {
            Func_081180e8(134);
            work->shake_frames = 8;
            Func_08118088(effect->actors[0], 0);
        }

        line = (s32 *)work->unknown_7080;
        for (i = 0, angle = -(wave << 9); i != 160; i++) {
            *line++ = ((i << 18) - (Trig_Sin(angle) << 7) + 0x40000) >> 10;
            angle += 0x200;
        }

        if (frame > 127) {
            work->transfer_pending = 1;
        } else {
            pal[1] = pal[63];
            Dma_Set(pal + 62, pal + 63, 0x80a0003e, (volatile u32 *)0x040000d4);
            IO_WRITE_QUEUE_PUSH((u32)((u16 *)0x05000000 + 1), (u32)(pal + 1), 0x8000003f);
        }
        Func_08158ce0(8, 8);
        Func_081434f8();
        WaitFrames(1);
    }

    Scheduler_RemoveCallback(Func_08152474);
    Scheduler_RemoveCallback(Func_08143000);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
