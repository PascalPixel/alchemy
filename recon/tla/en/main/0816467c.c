/* Draft, differ 48 of 430: The Broken Seal's BattleEffect_RunImpactBurst
 * with The Lost Age's blitter loads (Func_081963ec(104, 19) and (188, 3),
 * read back from the heap slots at 0x68 and 0xbc), its resources (0x134,
 * 0x120, 0x121) and callback (Func_08143000 at 0xc80). Everything matches but reordered rows,
 * all of one kind: the native puts a store, a load or a pool load before an
 * independent constant (the arguments' spills before 0x03000000, the
 * spills of the heap reads, movs r1, #19 before movs r0, #104, the 0xffff
 * angle mask after the particle stores, the pooled sheet offsets), and the
 * shifted literal pools follow. The empty asm at the start and the BG2PA
 * halfword pinned in a one-pass block (as Func_0816a730) take it from 54
 * to 48. */
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "mem.h"
#include "system.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "battle_efx.h"
#include "battle_effect_work.h"
#include "battle_fx_work.h"

#define HI(v) (((s16 *)&(v))[1])
#define HAHEN_E 0x59d8

extern char ResourceId_Data134;
extern char ResourceId_FlareSheet;
extern char ResourceId_FlareImage;
extern const u16 Data_081989f2[];
extern const u8 Data_081989e2[];
extern const u8 Data_081989ea[];
extern const u16 Data_08197410[];

void Func_08143000(void);
void Func_081963ec(s32 slot, s32 kind);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping, s32 gravity);
void Audio_PlayCue(s32 cue);
void Func_0816442c(s32 formation, s32 x, s32 y);

void Func_0816467c(s32 formation, s32 x, s32 y)
{
    struct BattleFxWork *work;
    void *canvas;
    DrawRectangle kasan;
    DrawRectangle hikaku;
    u8 *sheet;
    s32 org_x;
    struct EffectStep *p;
    s32 frame;
    s32 i;

    __asm__("");
    work = (struct BattleFxWork *)Ram_HeapSlots->battle_fx_work[0];
    canvas = Ram_HeapSlots->battle_fx_work[1];
    sheet = Ram_HeapSlots->unknown_64[0];
    org_x = x;
    x += 40 << 16;
    x /= 2;
    {
        register u16 value __asm__("r3");

        do {
            value = 0x80;
        } while (0);
        REG_BG2PA = value;
    }
    REG_BG2X = 0;
    REG_BLDCNT = 0x3f46;
    Func_081963ec(104, 19);
    hikaku = (DrawRectangle)Ram_HeapSlots->unknown_64[1];
    Func_081963ec(188, 3);
    kasan = (DrawRectangle)Ram_HeapSlots->unknown_b8[1];
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_FlareSheet, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_FlareImage, work->sheet + HAHEN_E, 0, 0);
    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

    for (i = 0; i != 64; i++) {
        s32 speed = (Random16() & 255) + 256;
        s32 angle = Random16() & 0xffff;

        work->particles[i].x = x;
        work->particles[i].y = y;
        work->particles[i].velocity_x = (Trig_Sin(angle) * speed) >> 7;
        work->particles[i].velocity_y = -((Trig_Cos(angle) * speed) >> 6);
        work->particles[i].variant = (Random16() & 15) + 16;
    }

    for (i = 0; i != 3; i++) {
        work->particles[61 + i].x = x;
        work->particles[61 + i].y = y;
        work->particles[61 + i].velocity_x = (Trig_Sin(i * 0x5555) * 32) >> 6;
        work->particles[61 + i].velocity_y = -((Trig_Cos(i * 0x5555) * 32) >> 5);
    }

    for (i = 0; i != 64; i++) {
        s32 speed = (Random16() & 255) + 32;
        s32 angle = Random16() & 0xffff;
        struct EffectStep *cells = (struct EffectStep *)Ram_MapCellBuffer;

        cells[i].x = x;
        cells[i].y = y;
        cells[i].velocity_x = (Trig_Sin(angle) * speed) >> 6;
        cells[i].velocity_y = -((Trig_Cos(angle) * speed) >> 5);
        cells[i].variant = (Random16() & 15) + 20;
    }

    for (frame = 0; frame != 72; frame++) {
        if (frame == 4)
            Audio_PlayCue(154);
        if (frame == 32)
            Audio_PlayCue(212);

        if (frame <= 47) {
            s32 cell = (frame - 8) / 5;
            u32 width;
            u32 height;

            if (cell < 0)
                cell = 0;
            hikaku(canvas, work->sheet + Data_081989f2[cell],
                (x >> 16) - ((width = Data_081989e2[cell]) >> 1),
                (y >> 16) - ((height = Data_081989ea[cell]) >> 1), width, height);
        }

        for (i = 0, p = work->particles; i != 30; i++, p++) {
            if (frame > i / 2 && p->variant > 0) {
                s32 size;
                s32 px;
                s32 py;

                p->variant--;
                BattleFxKernels_IntegrateVector2(p, 60, 0);
                size = p->variant / 16 + 3;
                px = HI(p->x);
                py = HI(p->y);
                kasan(canvas, sheet + Data_08197410[size - 1], px - size / 2, py - size, size, size * 2);
            }
        }

        for (i = 0, p = (struct EffectStep *)Ram_MapCellBuffer; i != 60; i++, p++) {
            if (frame > 35 && p->variant > 0) {
                s32 size;
                s32 px;
                s32 py;

                p->variant--;
                BattleFxKernels_IntegrateVector2(p, 60, 0);
                size = p->variant / 16 + 1;
                px = HI(p->x);
                py = HI(p->y);
                kasan(canvas, sheet + Data_08197410[size - 1], px - size / 2, py - size, size, size * 2);
            }
        }

        for (i = 0; i != 3; i++) {
            struct EffectStep *hahen = &work->particles[61 + i];

            if (frame >= 36 && frame <= 63) {
                s32 cell;
                s32 px;
                s32 py;

                BattleFxKernels_IntegrateVector2(hahen, 64, 0);
                cell = (frame - 36) / 7;
                px = HI(hahen->x);
                py = HI(hahen->y);
                hikaku(canvas, work->sheet + cell * 0x120 + HAHEN_E, px - 6, py - 12, 12, 24);
            }
        }

        if (frame <= 35)
            Func_0816442c(formation, org_x, y);
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback(Func_08143000);
    Iwram_ClearWords((void *)0x06004000, 0x4000);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
}
