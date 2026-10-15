/* Draft, differ 10 of 320: The Broken Seal's BattleFx_RunRisingBurst with
 * The Lost Age's heap reads (the effect stays an argument), its blitter
 * fetch after each frame's columns and its actor timers. The empty asm at
 * the start stores the argument before 0x03000000 is started; the BG2PA
 * halfword pinned to r3 and set in a one-pass block loads from the pool
 * before the register address is started and keeps the first pool where
 * the native has it (an empty asm there moves the pool 4 bytes early).
 * Left, five reordered rows: the canvas spill (str r4, [sp, #16]) before
 * movs r0, #0 (here the spill is anti-dependent on the work's load, so the
 * argument wins its tie); movs r4, #0 after the REG_BG2X store; mov r9, r4
 * after the stores to sp+12 and sp+8; and the frame * 6 reload (ldr from
 * sp+8) before movs r3, #136 and before movs r2, #1. Tried: the three heap
 * reads in each order (20 to 102), const heap reads (22, 99), a one-pass
 * block or an empty asm around the REG_BG2X store (16, 14: either moves
 * other rows), alchemy permute 6 minutes on the earlier draft (18). */
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "mem.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_fx_work.h"

#define HI(v) (((s16 *)&(v))[1])


extern char ResourceId_Data134;
extern char ResourceId_LimeColumnSheet;
extern char ResourceId_BlastSheet;
extern const u16 gData_08198afc[];
extern const u16 gData_08198b0a[];

void BattleFx_BeginCanvasLayer(s32 layer);
void Func_08143000(void);
void Func_08143bb8(void);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
void Func_081180e8(s32 phase);
void Func_08118088(s32 actor, s32 variant);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
s32 Func_08196404(s32 slot, s32 width_shift, s32 height_shift, s32 flags, s32 mode);
void Func_08144aac(s32 side, DrawRectangle *draw);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping, s32 gravity);
void Func_08158ce0(s32 x, s32 y);
void Func_081434f8(void);

void Func_0816a730(struct BattleEffectArgument *effect)
{
    struct BattleFxWork *work;
    void *canvas;
    void *sprites;
    DrawRectangle draw[2];
    struct EffectPosition pos;
    struct EffectStep *spark;
    struct EffectStep *p;
    s32 frame;
    s32 i;
    s32 height;
    s32 mode;
    s32 x;
    u8 *src;
    s32 speed;
    s32 angle;
    s32 cell;
    s32 size;

    __asm__("");
    work = (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
    canvas = Ram_HeapSlots->battleFxWork[1];
    sprites = Ram_HeapSlots->unk64[0];
    BattleFx_BeginCanvasLayer(0);
    {
        register u16 value __asm__("r3");

        do {
            value = 0x100;
        } while (0);
        REG_BG2PA = value;
    }
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_LimeColumnSheet, work->sheet, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet + 0x460, 1, 0);
    work->transferMode = 2;
    work->transferValue = 75;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

    EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &pos);
    REG_BG2X = (64 - pos.x) << 8;

    for (i = 0, spark = work->particles; i != 16; i++) {
        speed = (Random16() & 0x1ff) + 128;
        angle = Random16() & 0xffff;
        spark->x = 0x400000;
        spark->y = 0x700000;
        spark->velocityX = (Trig_Sin(angle) * speed) >> 8;
        spark->velocityY = (Trig_Cos(angle) * speed) >> 9;
        spark->variant = Random16() & 7;
        spark++;
    }
    work->shakeFrames = 8;
    for (frame = 0; frame != 54; frame++) {
        if (frame == 10) {
            work->shakeFrames = 8;
            Func_081180e8(212);
            Func_08118088(effect->actors[0], 0);
            Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
        }

        if (frame > 7) {
            if (frame <= 31)
                height = frame * 12 - 96;
            else
                height = 272 - frame * 6;
            if (height > 0) {
                if (height > 80) {
                    height = 80;
                    mode = 1;
                } else {
                    mode = 0;
                }
                for (i = 0, x = 50; i != 2; i++, x += 14) {
                    if (i == 0)
                        Func_08196404(104, 7, 7, 3, mode);
                    else
                        Func_08196404(104, 7, 7, 7, mode);
                    (draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1])(canvas, work->sheet, x, 112 - height, 14, height);
                    Runtime_ReleaseHeapBlock(104);
                }
            }
        }

        Func_08144aac(effect->side, draw);
        for (i = 0, p = work->particles; i != 16; i++) {
            if (frame >= i / 2 + 8 && p->variant <= 28) {
                s32 px;
                s32 py;

                cell = p->variant / 3;
                px = HI(p->x);
                py = HI(p->y);
                if (cell > 6)
                    cell = 6;
                src = work->sheet + gData_08198afc[cell] + 0x460;
                size = gData_08198b0a[cell];
                draw[0](canvas, src, px - size / 2, py - size / 2, size, size);
                p->variant++;
                BattleFxKernels_IntegrateVector2(p, 62, -0x2000);
            }
            p++;
        }
        Runtime_ReleaseHeapBlock(188);
        Runtime_ReleaseHeapBlock(104);
        if (frame <= 7)
            Func_08158ce0(4, 4);
        else
            Func_08158ce0(16, 16);
        Func_081434f8();
        work->transferPending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback(Func_08143000);
    Func_08143bb8();
}
