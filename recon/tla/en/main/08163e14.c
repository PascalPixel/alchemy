/* Draft, differ 292 of 699 (701 here): The Broken Seal's
 * BattleEffect_RunEmberColumns, ported: the palette goes by DMA 3, the
 * blitters come from Func_08144aac(0, draw), the effect stays an argument
 * (its count and actors are read from it; only Func_0814cc4c reads the
 * work's copy), formation resource 649, callback Func_08143000 at 0xc80,
 * byte column table. Left is register allocation: the native keeps frame
 * in r9 and every loop counter in r8 and spills frame - 32 to sp+8; here
 * frame is spilled, frame - 32 takes fp and the counters move between r8
 * and sl, so most rows differ by register. Tried: the block's locals at
 * the top (332), k before frame (292), focus and velocity as scalars
 * (435), frame and k pinned to r9 and r8 (339 to 629). */
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "dma.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "input.h"
#include "motobj.h"
#include "battle_session.h"
#include "battle_efx.h"
#include "battle_effect_work.h"
#include "battle_fx_work.h"

#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_WaterSpraySheet;
extern char ResourceId_FirePillarSheetA;
extern char ResourceId_Data134;
extern const u8 Data_0819896a[];
extern const u16 Data_08197410[];
extern const s32 Data_08198974[];

void Func_08143000(void);
void Func_08143bb8(void);
void BattleFx_BeginCanvasLayer(s32 layer);
void Func_08144aac(s32 side, DrawRectangle *draw);
void Func_0814cc4c(struct BattleEffectArgument *effect);
void Func_08152404(s32 count, s32 resource, s32 priority);
void Func_081180e8(s32 phase);
void Audio_PlayCue(s32 cue);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_0816442c(s32 formation, s32 x, s32 y);
void Func_0816467c(s32 formation, s32 x, s32 y);

struct Point2D {
    s32 x;
    s32 y;
};

void Func_08163e14(struct BattleEffectArgument *effect)
{
    DrawRectangle draw[2];
    void *canvas;
    struct BattleFxWork *work;
    u8 *graphics;
    u8 *palette;

    canvas = Ram_HeapSlots->battle_fx_work[1];
    work = (struct BattleFxWork *)Ram_HeapSlots->battle_fx_work[0];
    graphics = Ram_HeapSlots->unknown_64[0];
    BattleFx_BeginCanvasLayer(0);
    REG_BLDALPHA = 0x1010;
    Func_08144aac(0, draw);

    palette = Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet);
    Dma_Set(palette, (void *)0x05000000, 0x84000020, (volatile u32 *)0x040000d4);
    palette += 128;
    Resource_DecodeType01(palette, work->sheet);
    palette = Resource_GetTableEntry((s32)&ResourceId_FirePillarSheetA);
    palette += 128;
    Resource_DecodeType01(palette, work->sheet + 0x6e4);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Data134), graphics);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
    {
        struct Point2D focus;
        struct Point2D velocity;
        s32 rise;
        s32 frame;
        s32 k;
        struct EffectStep *point;
        struct EffectStep *spark;
        struct EffectStep *sparks;
        s32 *life;

        rise = 0;
        focus.x = 256 << 16;
        focus.y = 88 << 16;
        velocity.x = -(16 << 16);
        velocity.y = -(4 << 16);
        for (k = 0; k != 64; k++)
            work->particles[k].variant = -1;
        for (k = 0; k != 16; k++) {
            point = &work->particles[24 + k];
            point->x = Random16() & 127;
            point->y = (Random16() & 7) + 56;
            point->variant = -(Random16() & 15);
        }
        life = &((struct EffectStep *)Ram_MapCellBuffer)->variant;
        for (k = 0; k != 1024; k++) {
            *life = -1;
            life += 7;
        }
        sparks = (struct EffectStep *)Ram_MapCellBuffer;
        Func_0814cc4c(work->effect);
        WaitFrames(1);
        Func_08152404(12, 649, 2);

        for (frame = 0; frame != 124; frame++) {
            if ((gInput.repeat & 3) && frame > 32 && frame <= 97)
                frame = 98;
            if (frame == 120)
                Func_081180e8(134);
            if (frame <= 15)
                rise += 2;

            if (frame <= 99) {
                focus.x += velocity.x;
                focus.y += velocity.y;
                velocity.x = velocity.x * 58 / 64;
                velocity.y = velocity.y * 56 / 64;
                if (focus.x < 120 << 16)
                    velocity.x += 0x8000;
            }
            Func_0816442c(1, focus.x, focus.y);

            if (frame == 28) {
                for (k = 0; k != 256; k++) {
                    spark = &sparks[k];
                    if (spark->variant == -1) {
                        s32 radius = Random16() & 63;
                        s32 angle = Random16() & 0xffff;

                        spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
                        spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
                        spark->velocity_x = ((Random16() & 63) - 32) << 14;
                        spark->velocity_y = (-(Random16() & 63) - 8) << 13;
                        spark->variant = 0;
                    }
                }
            }
            if (frame >= 32 && frame < 80) {
                s32 spawned = 0;

                for (k = 0; k != 1024; k++) {
                    spark = &sparks[k];
                    if (spark->variant == -1) {
                        s32 radius = Random16() & 63;
                        s32 angle = Random16() & 0xffff;

                        spark->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
                        spark->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
                        spark->velocity_x = ((Random16() & 63) - 32) << 14;
                        spark->velocity_y = (-(Random16() & 63) - 8) << 13;
                        spark->variant = 0;
                        spawned++;
                        if (spawned == 16)
                            break;
                    }
                }
            }
            if (frame == 0)
                Audio_PlayCue(164);
            if (frame == 32)
                Audio_PlayCue(145);
            if (frame == 80)
                Audio_PlayCue(144);

            if (frame >= 32 && frame < 80) {
                for (k = 0; k != 3; k++) {
                    s32 span = (frame * 16 - 256 + k * 25) % 104;

                    draw[0](canvas, work->sheet + 0x6e4, Data_0819896a[k * 2] - 17,
                        Data_0819896a[k * 2 + 1] - span - 104, 34, 104);
                    draw[0](canvas, work->sheet + 0x6e4, Data_0819896a[k * 2] - 17,
                        Data_0819896a[k * 2 + 1] - span, 34, span);
                }
            }

            if (frame <= 95) {
                for (k = 0; k != 5; k++)
                    draw[0](canvas, work->sheet, k * 32 + ((frame / 4) & 31) - 32, 120 - rise, 32, 32);
            }

            for (k = 0; k != 1024; k++) {
                spark = &sparks[k];
                if (spark->variant >= 0) {
                    s32 size = k % 3 + 2;

                    if (spark->velocity_y > 0)
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
                    draw[spark->velocity_y > 0](canvas,
                        graphics + Data_08197410[size - 1], HI(spark->x) - size / 2,
                        HI(spark->y) - size, size, size * 2);
                    spark->x += spark->velocity_x;
                    spark->y += spark->velocity_y;
                    if (frame > 80)
                        spark->velocity_y += -0x8000;
                    else
                        spark->velocity_y += Data_08198974[k & 3];
                    spark->velocity_x = spark->velocity_x * 62 / 64;
                    spark->velocity_y = spark->velocity_y * 62 / 64;
                    spark->variant++;
                    if (spark->velocity_y > 0 && HI(spark->y) > 104)
                        spark->variant = -1;
                }
            }

            if (frame <= 79) {
                for (k = 0; k != effect->count; k++) {
                    if (frame > 29) {
                        s32 phase = frame % 12;

                        if (phase == 0) {
                            struct MotionObject *obj = GetBattleObjectSlotFar(effect->actors[k])->object;

                            Func_0814cd48(effect->actors[k], 7, 5, -1, 0);
                            obj->velocity_y = 0x48000;
                            obj->vertical_motion_strength = 0xab85;
                        }
                        if (phase == 6)
                            Func_0814cd48(effect->actors[k], 0, 5, -1, 0);
                    }
                }
            }
            work->transfer_pending = 1;
            WaitFrames(1);
        }

        Scheduler_RemoveCallback(Func_08143000);
        Runtime_ReleaseHeapBlock(188);
        Runtime_ReleaseHeapBlock(104);
        Func_0816467c(1, focus.x, focus.y);
        for (k = 0; k != 12; k++)
            ResourceObject_ReleaseFar(work->objects[k]);
    }
    Func_08143bb8();
}
