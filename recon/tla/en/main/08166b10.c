/* Draft, differ 149 of 863 (864 here): The Broken Seal's
 * BattleFx_RunFallingSword with The Lost Age's mode argument (Func_08166af8
 * and Func_08166b04 pass 0 and 1): mode 2 skips the sword sheet, mode 1
 * swaps each BG colour's red and green-blue fields first and at frame 61
 * copies resource 0x188's palette and calls Func_0815b634, mode 0 steps the
 * palette toward the blast sheet on frames 41 to 63; frame 60 starts phase
 * 144 or 145; frame 9 also flashes the first target. The blitters come from
 * Func_081963ec(104, 19) and (188, 35). Frame declared before the blitters
 * and the sheet held in a local for the column blits give the native's
 * stack slots (252 to 149). Left: 41 rows differ by register, the frame
 * reloads in the dust loop taking other scratch registers (the loop's
 * pointer and counter start in r1 here, r5 and r6 in the native), and one
 * extra row; the rest is reordered rows of the kind Func_08164b2c's header
 * traces to memory costs. */
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "system.h"
#include "fixed_math.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "effect_step.h"
#include "battle_efx.h"
#include "battle_effect_work.h"
#include "battle_fx_work.h"

#define HI(v) (((s16 *)&(v))[1])
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)
#define SPARK ((struct EffectStep *)(Ram_MapCellBuffer + 684 * sizeof(struct EffectStep)))

extern char ResourceId_SwordSheet;
extern char ResourceId_FirePillarSheetA;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_Data134;
extern char ResourceId_BlastSheet;
extern char ResourceId_Data188;
extern u8 Data_08198a9e[];
extern s32 Data_08198aa4[];
extern u16 Data_08197410[];

void BattleFx_BeginCanvasLayer(s32 layer);
void Func_08143000(void);
void Func_08143bb8(void);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
void Func_081963ec(s32 slot, s32 kind);
void Func_0815f0a0(s32 resource);
void Func_0815b634(void);
void Audio_PlayCue(s32 cue);
void Func_081180e8(s32 phase);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping, s32 gravity);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_08118088(s32 actor, s32 variant);
void Func_08158ce0(s32 x, s32 y);
void Func_081434f8(void);

void Func_08166b10(struct BattleEffectArgument *effect, s32 mode)
{
    struct BattleFxWork *work;
    void *canvas;
    s32 frame;
    DrawRectangle routine[2];
    u8 *sheet;
    struct EffectPosition pos;
    s32 half;
    s32 step;
    s32 origin;
    s32 i;

    work = (struct BattleFxWork *)Ram_HeapSlots->battle_fx_work[0];
    canvas = Ram_HeapSlots->battle_fx_work[1];
    sheet = Ram_HeapSlots->unknown_64[0];

    BattleFx_BeginCanvasLayer(1);
    REG_BLDALPHA = 0x1010;
    EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &pos);
    half = pos.x / 2;

    Func_081963ec(104, 19);
    routine[0] = (DrawRectangle)Ram_HeapSlots->unknown_64[1];
    Func_081963ec(188, 35);
    routine[1] = (DrawRectangle)Ram_HeapSlots->unknown_b8[1];

    if (mode != 2)
        Resource_LoadAndDecompress((s32)&ResourceId_SwordSheet, work->sheet + 20000, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet + (221 << 4), 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);

    if (mode == 1) {
        u16 *color = (u16 *)0x05000000;

        for (i = 0; i != 64; i++) {
            s32 red = *color & 0x1f;
            s32 green = (*color >> 5) & 0x1f;
            s32 blue = (*color >> 10) & 0x1f;

            *color++ = (red << 10) | (blue << 5) | green;
        }
    }

    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

    i = 0;
    do {
        struct EffectStep *flash = &work->particles[i];
        s32 scale = i * 2;
        s32 ang = Random16() & 0xFFFF;

        flash->x = Trig_Sin(ang) * scale;
        flash->y = -(Trig_Cos(ang) * scale);
        flash->variant = i / 2 + 25;
        i++;
    } while (i != 32);

    i = 0;
    do {
        PARTICLES[i].variant = -1;
        i++;
    } while (i != 171 << 2);

    i = 0;
    origin = half << 16;
    do {
        struct EffectStep *spark = &SPARK[i];
        s32 mag;
        s32 ang;

        mag = 0x1FF;
        mag &= Random16();
        ang = Random16() & 0xFFFF;
        spark->x = origin;
        spark->y = 176 << 15;
        spark->velocity_x = (Trig_Sin(ang) * (mag + 32)) >> 5;
        spark->velocity_y = -(Trig_Cos(ang) * (mag + 32)) >> 6;
        spark->variant = (Random16() & 7) + 32;
        i++;
    } while (i != 170 << 1);

    frame = 0;
    do {
        if (mode == 0) {
            if (frame >= 41 && frame <= 63)
                Func_0815f0a0((s32)&ResourceId_BlastSheet);
        } else if (mode == 1) {
            if (frame == 61) {
                Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_Data188), 128);
                Func_0815b634();
            }
        }

        if (frame == 8)
            work->shake_frames = frame;
        if (frame == 48)
            work->shake_frames = 8;
        if (frame == 60)
            work->shake_frames = 16;

        if (frame == 4)
            Audio_PlayCue(212);
        if (frame == 32)
            Audio_PlayCue(164);
        if (frame == 60) {
            if (mode == 1)
                Func_081180e8(144);
            else
                Func_081180e8(145);
        }

        if (frame > 55) {
            i = 0;
            do {
                struct EffectStep *flash = &work->particles[i];
                s32 x;
                s32 life;
                s32 y;

                y = HI(flash->y);
                x = HI(flash->x) + half;
                life = flash->variant;
                if (life >= 0 && life <= 17)
                    routine[0](canvas, work->sheet + (Data_08198a9e[life / 3] << 11) + (221 << 4), x - 16, y + 48, 32, 64);
                life = flash->variant;
                flash->variant = life > 0 ? life - 1 : -1;
                i++;
            } while (i != 16);
        }

        if (frame == 28) {
            i = 0;
            do {
                struct EffectStep *dust = &PARTICLES[i];

                if (dust->variant == -1) {
                    s32 mag;
                    s32 ang;

                    mag = Random16() & 63;
                    ang = Random16() & 0xFFFF;
                    dust->x = ((Trig_Sin(ang) * mag) >> 3) + origin;
                    dust->y = ((Trig_Cos(ang) * mag) >> 2) + (192 << 15);
                    dust->velocity_x = ((Random16() & 63) - 32) << 14;
                    dust->velocity_y = (-(Random16() & 63) - 8) << 13;
                    dust->variant = 0;
                }
                i++;
            } while (i != 128 << 1);
        }

        step = frame - 32;
        if (step >= 0 && step <= 31) {
            s32 cnt;

            cnt = 0;
            i = 0;
            do {
                struct EffectStep *dust = &PARTICLES[i];

                if (dust->variant == -1) {
                    s32 mag;
                    s32 ang;

                    mag = Random16() & 63;
                    ang = Random16() & 0xFFFF;
                    dust->x = ((Trig_Sin(ang) * mag) >> 3) + origin;
                    dust->y = ((Trig_Cos(ang) * mag) >> 2) + (192 << 15);
                    dust->velocity_x = ((Random16() & 63) - 32) << 14;
                    dust->velocity_y = (-(Random16() & 63) - 8) << 13;
                    dust->variant = 0;
                    cnt++;
                    if (cnt == 16)
                        break;
                }
                i++;
            } while (i != 171 << 2);
        }

        if (step >= 0 && step <= 31) {
            s32 phase = frame * 16;
            s32 x = half - 17;
            s32 width = 34;
            DrawRectangle blit = routine[0];
            u8 *src = work->sheet;
            s32 high = 104;
            s32 size = (phase - 256) % high;

            blit(canvas, src, x, 4 - size, width, high);
            blit(canvas, src, x, 108 - size, width, size);
        }

        if (frame <= 71) {
            i = 0;
            do {
                struct EffectStep *dust = &PARTICLES[i];

                if (dust->variant >= 0) {
                    s32 size;

                    size = i % 3 + 2;
                    if (dust->velocity_y > 0)
                        size += 2;
                    if (frame > 68 && size <= 5)
                        size = 6;
                    if (frame > 70 && size <= 6)
                        size = 7;
                    if (frame > 72 && size <= 7)
                        size = 8;
                    if (frame > 74 && size <= 8)
                        size = 9;
                    if (frame > 76)
                        size = 10;
                    routine[0](canvas, sheet + Data_08197410[size - 1],
                        HI(dust->x) - size / 2, HI(dust->y) - size, size, size * 2);

                    dust->x += dust->velocity_x;
                    dust->y += dust->velocity_y;
                    if (frame > 80)
                        dust->velocity_y += -32768;
                    else
                        dust->velocity_y += Data_08198aa4[i & 3];
                    dust->velocity_x = dust->velocity_x * 62 / 64;
                    dust->velocity_y = dust->velocity_y * 62 / 64;
                    dust->variant++;
                    if (dust->velocity_y > 0 && HI(dust->y) > 108)
                        dust->variant = -1;
                }
                i++;
            } while (i != 171 << 1);
        }

        if (frame <= 95) {
            s32 x;
            s32 high;
            s32 top;

            x = half - 18;
            high = 120;
            if (frame > 60)
                top = frame * 8 - 450;
            else if (frame > 32)
                top = step / 2 + 16;
            else if (frame <= 9)
                top = frame * 16 - 128;
            else
                top = 16;
            if (top + 120 > 108)
                high = high - top - 12;
            if (high > 0)
                routine[1](canvas, work->sheet + 20000, x, top, 36, high);
        }

        if (frame > 59) {
            i = 0;
            do {
                struct EffectStep *spark = &SPARK[i];

                if (spark->variant > 0) {
                    s32 x;
                    s32 y;
                    s32 size;

                    BattleFxKernels_IntegrateVector2(spark, 64, 128 << 6);
                    y = spark->y;
                    spark->variant--;
                    if (y > (216 << 15)) {
                        spark->velocity_y = -spark->velocity_y / 2;
                    } else {
                        x = spark->x;
                        if ((x >= 0 && x <= 0x7effff) && y >= 0) {
                            u8 *cell;

                            size = spark->variant / 5 + 1;
                            cell = sheet + Data_08197410[size - 1];
                            x >>= 16;
                            y >>= 16;
                            x -= size / 2;
                            y -= size;
                            routine[0](canvas, cell, x, y, size, size * 2);
                        }
                    }
                }
                i++;
            } while (i != 170 << 1);
        }

        if (frame == 68) {
            for (i = 0; i != effect->count; i++) {
                Func_0814cd48(effect->actors[i], 7, 5, i, 16);
                Func_08118088(effect->actors[i], 7);
            }
        }

        if (frame == 9) {
            Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
            Iwram_FillWords(canvas, 128 << 7, 0x3F3F3F3F);
        }
        if (frame == 60)
            Iwram_FillWords(canvas, 128 << 7, 0x3F3F3F3F);

        Func_08158ce0(16, 16);
        Func_081434f8();
        work->transfer_pending = 1;
        WaitFrames(1);
        frame++;
    } while (frame != 102);

    Scheduler_RemoveCallback(Func_08143000);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
