/* Draft, differ 68 of 699: The Broken Seal's BattleEffect_RunEmberColumns,
 * ported: the palette goes by DMA 3, the blitters come from
 * Func_08144aac(0, draw), the effect stays an argument (its count and
 * actors are read from it; only Func_0814cc4c reads the work's copy),
 * formation resource 649, callback Func_08143000 at 0xc80. The column
 * table is not const: the native reads it again after each draw call, and
 * as const data the reuse moved every register (differ 292). Everything
 * left is reordered rows where the native puts a load or a store ahead of
 * an independent constant (the argument's store before 0x03000000, the
 * work's reloads, the pooled values, the 58 multiply), the kind
 * Func_08164b2c's header traces to a load costing 3 and a store 2. */
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
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_fx_work.h"

#define HI(v) (((s16 *)&(v))[1])

extern char ResourceId_WaterSpraySheet;
extern char ResourceId_FirePillarSheetA;
extern char ResourceId_Data134;
extern u8 gData_0819896a[];
extern const u16 gData_08197410[];
extern const s32 gData_08198974[];

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

    canvas = Ram_HeapSlots->battleFxWork[1];
    work = (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
    graphics = Ram_HeapSlots->unk64[0];
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
    work->transferMode = 2;
    work->transferValue = 75;
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
                        spark->velocityX = ((Random16() & 63) - 32) << 14;
                        spark->velocityY = (-(Random16() & 63) - 8) << 13;
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
                        spark->velocityX = ((Random16() & 63) - 32) << 14;
                        spark->velocityY = (-(Random16() & 63) - 8) << 13;
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

                    draw[0](canvas, work->sheet + 0x6e4, gData_0819896a[k * 2] - 17,
                        gData_0819896a[k * 2 + 1] - span - 104, 34, 104);
                    draw[0](canvas, work->sheet + 0x6e4, gData_0819896a[k * 2] - 17,
                        gData_0819896a[k * 2 + 1] - span, 34, span);
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

                    if (spark->velocityY > 0)
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
                    draw[spark->velocityY > 0](canvas,
                        graphics + gData_08197410[size - 1], HI(spark->x) - size / 2,
                        HI(spark->y) - size, size, size * 2);
                    spark->x += spark->velocityX;
                    spark->y += spark->velocityY;
                    if (frame > 80)
                        spark->velocityY += -0x8000;
                    else
                        spark->velocityY += gData_08198974[k & 3];
                    spark->velocityX = spark->velocityX * 62 / 64;
                    spark->velocityY = spark->velocityY * 62 / 64;
                    spark->variant++;
                    if (spark->velocityY > 0 && HI(spark->y) > 104)
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
                            obj->velocityY = 0x48000;
                            obj->verticalMotionStrength = 0xab85;
                        }
                        if (phase == 6)
                            Func_0814cd48(effect->actors[k], 0, 5, -1, 0);
                    }
                }
            }
            work->transferPending = 1;
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
