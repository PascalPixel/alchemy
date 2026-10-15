/* Draft of Func_081509b4, ported from The Broken Seal's BattleFx_RunPuffArc
 * (listing 274 instructions, draft 262, differ 316). The Lost Age blends
 * at 0x1010, runs 60 frames and loads its blitter through Func_081963ec;
 * the logic agrees. Register allocation differs throughout: the native
 * spills the effect argument, canvas, blitter and frame to the stack and
 * keeps work in r10 and the table bases in r9 and r11 across the three
 * draws; here work is spilled and the effect stays in fp. */
#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "io_reg.h"
#include "ram_buffer.h"

extern char ResourceId_EmberStreakSheet;

extern const u16 Data_0819747a[];
extern const u8 Data_08197467[];
extern const u8 Data_08197473[];
extern const u8 Data_0819746d[];

void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void BattleFx_BeginCanvasLayer(s32 bg_control);
void Func_08143bb8(void);
void Func_08143000(void);
void Func_081434f8(void);
void Func_081963ec(s32 slot, s32 mode);
void Func_081180e8(s32 phase);
void Func_0814cd48(s32 actor, s32 a, s32 b, s32 c, s32 d);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);

#define PUFF_COUNT 9

// Nine flames along an arc around the targets (The Broken Seal's
// BattleFx_RunPuffArc).
void Func_081509b4(struct BattleEffectArgument *effect)
{
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    DrawRectangle draw;
    s32 i;
    s32 cell;
    u32 w;
    s32 sign;
    s32 org;
    s32 angle;
    s32 tick;
    struct EffectStep *puff;
    struct EffectStep *p;

    work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
    canvas = Ram_HeapSlots->battleFxWork[1];
    BattleFx_BeginCanvasLayer(0);
    REG_BLDCNT = 0x3f46;
    REG_BLDALPHA = 0x1010;
    Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1, 1);
    Func_081963ec(104, 3);
    draw = (DrawRectangle)Ram_HeapSlots->unk64[1];

    if (effect->actors[0] > 127) {
        org = 0;
        sign = 1;
    } else {
        org = 64;
        sign = -1;
    }
    angle = -0x4000;
    i = 0;
    tick = 0;
    puff = work->particles;
    do {
        puff->x = ((sign * ((Trig_Sin(angle) << 5) >> 16)) + org) + 20;
        puff->y = ((Trig_Cos(angle) << 4) >> 16) + 40;
        puff->variant = tick;
        angle += 0x1000;
        tick -= 4;
        i += 1;
        puff++;
    } while (i != PUFF_COUNT);

    work->transferMode = 2;
    if (effect->variant == 2)
        work->transferValue = 75;
    else
        work->transferValue = 50;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
    Audio_PlayCue(136);
    frame = 0;
    do {
        if (frame == 24)
            Func_081180e8(133);

        i = 0;
        p = work->particles;
        do {
            if (p->variant >= 0 && p->variant <= 23) {
                cell = p->variant / 4;
                draw(canvas, work->sheet + Data_0819747a[cell],
                    p->x - ((w = Data_08197467[cell]) >> 1), p->y + Data_08197473[cell], w,
                    Data_0819746d[cell]);
                if (effect->variant != 0)
                    draw(canvas, work->sheet + Data_0819747a[cell],
                        p->x - ((w = Data_08197467[cell]) >> 1),
                        (p->y + Data_08197473[cell]) - 16, w, Data_0819746d[cell]);
                if (effect->variant == 2)
                    draw(canvas, work->sheet + Data_0819747a[cell],
                        p->x - ((w = Data_08197467[cell]) >> 1),
                        (p->y + Data_08197473[cell]) - 32, w, Data_0819746d[cell]);
            }
            i += 1;
            p->variant += 1;
            p++;
        } while (i != PUFF_COUNT);

        i = 0;
        while (i != effect->count) {
            if (frame == (i * 8) + 16)
                Func_0814cd48(effect->actors[i], 10, 5, i, 12);
            i += 1;
        }
        Func_081434f8();
        work->transferPending = 1;
        WaitFrames(1);
        frame += 1;
    } while (frame != 60);
    Runtime_ReleaseHeapBlock(104);
    Scheduler_RemoveCallback(Func_08143000);
    Func_08143bb8();
}
