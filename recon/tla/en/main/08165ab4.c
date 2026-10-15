/* Draft, differ 78 of 804: The Broken Seal's
 * BattleEffect_RunCirclingFallingScene, ported: Func_0813ba50 sets the
 * display, the six rock objects are resource 653 at objects 9 to 14, the
 * formation is resource 648, the blitters come from Func_081963ec(104, 19)
 * and (188, 3), the desert backdrop is resource 0x73, the effect stays an
 * argument for the hit loop, callback Func_08143000 at 0xc80. The 78
 * differing rows are only reordered (as a multiset they are the native's):
 * loads and stores ahead of independent constants, and the literal pools
 * that shift with them, the kind Func_08164b2c's header traces to a load
 * costing 3 and a store 2. */
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "input.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_fx_work.h"

#define HI(v) (((s16 *)&(v))[1])

struct FxAnimObject {
    u8 unknown_00[9];
    u8 unknown_09_0 : 2;
    u8 priority : 2;
    u8 unknown_09_4 : 4;
    u8 unknown_0a[16];
    u8 flags;
};

struct FxScale {
    s32 x;
    s32 y;
};

struct ScaledSprite;

extern char ResourceId_DesertBackdrop;
extern char ResourceId_Data134;
extern char ResourceId_BlastSheet;
extern struct FxScale Data_08196e6c;
extern u8 Data_08198a64[];
extern u8 Data_08198a6d[];
extern u16 Data_08198a76[];
extern u16 Data_08198a84[];
extern u16 Data_08197410[];

void BattleFx_BeginCanvasLayer(s32 layer);
void Func_0813ba50(void);
void Func_08143000(void);
void Func_08143bb8(void);
void BattleEffect_WipeCanvas(s32 mode, s32 value);
void Func_0814cc4c(struct BattleEffectArgument *effect);
void Func_08152404(s32 count, s32 resource, s32 priority);
void *ResourceObject_CreateFar(s32 resource);
void ResourceObject_ReleaseFar(void *object);
void Animation_ApplyChildArgumentFar(void *, s32);
void Func_081963ec(s32 slot, s32 kind);
void BattleBackground_LoadFar(s32, s32, s32);
void Audio_PlayCue(s32 cue);
void Render_ApplyProjectedPlacementFar(struct ScaledSprite *sprite, s32 *position, s32 *scale, s32 mode);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping, s32 gravity);
void Func_08118088(s32 actor, s32 variant);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_08158ce0(s32 x, s32 y);
void Func_081434f8(void);
void Func_081180e8(s32 phase);
void Func_0814cca8(void);

void Func_08165ab4(struct BattleEffectArgument *effect)
{
    s32 position[4];
    DrawRectangle draw[2];
    struct FxScale scale;
    void *canvas;
    struct BattleFxWork *work;
    s32 frame;
    u8 *sheet;
    struct FxScale base;
    struct FxScale orb;
    s32 i;
    s32 k;

    canvas = Ram_HeapSlots->battle_fx_work[1];
    work = (struct BattleFxWork *)Ram_HeapSlots->battle_fx_work[0];
    sheet = Ram_HeapSlots->unknown_64[0];
    BattleFx_BeginCanvasLayer(0);
    Func_0813ba50();
    *(u16 *)0x05000000 = 0;
    *(u16 *)0x05000002 = 0;
    work->transfer_mode = 0;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
    BattleEffect_WipeCanvas(1, 0);
    Func_0814cc4c(work->effect);
    Func_08152404(9, 648, 2);
    for (i = 0; i != 6; i++) {
        struct FxAnimObject *object = ResourceObject_CreateFar(653);

        work->objects[9 + i] = object;
        if (object != 0) {
            object->flags = 0;
            Animation_ApplyChildArgumentFar(object, i % 3);
            ((struct FxAnimObject *)work->objects[9 + i])->priority = 1;
        }
    }
    Func_081963ec(104, 19);
    draw[0] = (DrawRectangle)Ram_HeapSlots->unknown_64[1];
    Func_081963ec(188, 3);
    draw[1] = (DrawRectangle)Ram_HeapSlots->unknown_b8[1];
    REG_WININ = 0x2737;
    REG_WIN0H = 0xf0;
    REG_WIN1V = 0x1088;
    WaitFrames(1);
    BattleBackground_LoadFar(1, (s32)&ResourceId_DesertBackdrop, 0);
    BattleEffect_WipeCanvas(1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet, 1, 1);
    REG_DISPCNT = 0x7741;
    REG_BG2PA = 0x80;
    REG_BLDALPHA = 0x1010;
    REG_BLDCNT = 0x3f44;
    work->transfer_mode = 2;
    work->transfer_value = 50;
    base.x = 0xbc0000;
    base.y = 0x5c0000;
    orb.x = 0xa00000;
    orb.y = 0x5c0000;
    for (i = 0; i != 6; i++) {
        struct EffectStep *rock = &work->particles[i];

        rock->x = (Random16() & 127) << 16;
        rock->y = i * -0x100000;
        rock->velocity_x = 0;
        rock->velocity_y = 0;
        rock->variant = 0;
    }
    for (i = 0; i != 58; i++)
        work->particles[6 + i].variant = 24;
    for (i = 0; i != 1024; i++)
        ((struct EffectStep *)Ram_MapCellBuffer)[i].variant = -1;
    work->fade_frames = 24;
    work->fade_step = 0;

    for (frame = 0; frame != 320 && !(gInput.repeat & 3); frame++) {
        if (frame == 94)
            Audio_PlayCue(156);
        if (frame == 136)
            Audio_PlayCue(156);
        if (frame == 178)
            Audio_PlayCue(156);
        if (frame == 260)
            Audio_PlayCue(145);
        scale = Data_08196e6c;
        if (frame >= 96 && frame <= 251)
            work->shake_frames = 1;
        else if (frame >= 260 && frame <= 263)
            work->shake_frames = 1;
        position[3] = 0;
        position[1] = 0;
        for (i = 0; i != 7; i++) {
            position[0] = (Data_08198a64[i] << 16) + base.x - 0x200000;
            position[2] = (Data_08198a6d[i] << 16) + base.y - 0x200000;
            Render_ApplyProjectedPlacementFar(work->objects[i], position, &scale.x, 0);
        }
        if (frame <= 90) {
            orb.x = Trig_Sin(frame << 9) * 16 + 0x9c0000;
            orb.y = Trig_Cos(frame << 9) * 16 + 0x5c0000;
        }
        if (frame <= 196) {
            s32 start;

            for (i = 0, start = 91; i != 3; start += 40, i++) {
                if (frame >= start && frame < start + 4)
                    orb.y += 0x80000;
                if (frame == start + 3) {
                    for (k = 0; k != 4; k++) {
                        struct EffectStep *puff = &work->particles[6 + i * 8 + k];

                        puff->x = 0x400000;
                        puff->y = 0x600000;
                        puff->velocity_x = ((Random16() & 255) - 127) << 10;
                        puff->velocity_y = ((Random16() & 255) - 127) << 10;
                        puff->variant = Random16() & 15;
                    }
                }
                if (frame >= start + 20 && frame < start + 36)
                    orb.y -= 0x20000;
            }
        }
        if (frame >= 244 && frame <= 251)
            orb.x -= 0x10000;
        if (frame >= 252 && frame <= 275)
            orb.x -= (frame - 250) << 16;
        if (frame <= 259) {
            position[0] = orb.x;
            position[1] = -0x1000000;
            position[2] = orb.y - 0x1000000;
            Render_ApplyProjectedPlacementFar(work->objects[7], position, &scale.x, 0);
            position[0] = orb.x + 0x200000;
            Render_ApplyProjectedPlacementFar(work->objects[8], position, &scale.x, 0);
        }
        position[1] = 0;
        for (i = 0; i != 6; i++) {
            struct EffectStep *rock = &work->particles[i];

            if (rock->variant != 2) {
                position[0] = rock->x;
                position[2] = rock->y;
                Render_ApplyProjectedPlacementFar(work->objects[9 + i], position, &scale.x, 0);
                rock->x += rock->velocity_x;
                rock->y += rock->velocity_y;
                if (frame > 96)
                    rock->velocity_y += 0x4000;
                if (rock->y > 0x780000) {
                    rock->variant++;
                    if (rock->variant == 1) {
                        rock->velocity_y = -rock->velocity_y / 2;
                        for (k = 0; k != 2; k++) {
                            struct EffectStep *puff = &work->particles[30 + i * 2 + k];

                            puff->x = rock->x / 2;
                            puff->y = rock->y - 0x200000;
                            puff->velocity_x = ((Random16() & 255) - 127) << 10;
                            puff->velocity_y = ((Random16() & 255) - 127) << 10;
                            puff->variant = Random16() & 15;
                        }
                    } else if (frame <= 199) {
                        rock->y = 0;
                        rock->velocity_y = 0;
                        rock->variant = 0;
                    }
                }
            }
        }
        for (i = 0; i != 56; i++) {
            struct EffectStep *puff = &work->particles[6 + i];

            if (puff->variant >= 0) {
                if (puff->variant >= 0 && puff->variant <= 23) {
                    s32 cell = puff->variant / 6 + 3;

                    draw[0](canvas, work->sheet + Data_08198a76[cell],
                        HI(puff->x) - Data_08198a84[cell] / 2,
                        HI(puff->y) - Data_08198a84[cell] / 2,
                        Data_08198a84[cell], Data_08198a84[cell]);
                }
                BattleFxKernels_IntegrateVector2(puff, 60, -0x4000);
                puff->variant++;
            }
        }
        if (frame == 260) {
            for (i = 0; i != effect->count; i++) {
                Func_08118088(effect->actors[i], 4);
                Func_0814cd48(effect->actors[i], 7, -1, i, 8);
            }
            work->shake_frames = 8;
        }
        if (frame == 260) {
            for (i = 0; i != 512; i++) {
                struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];
                s32 speed;
                s32 angle;

                speed = 0x3ff;
                speed &= Random16();
                angle = Random16() & 0xffff;
                spark->x = 0x200000;
                spark->y = 0x5c0000;
                spark->velocity_x = Trig_Sin(angle) * (speed + 32) >> 7;
                spark->velocity_y = -(Trig_Cos(angle) * (speed + 32) << 1) >> 7;
                spark->variant = (Random16() & 15) + 32;
            }
        }
        for (i = 0; i != 512; i++) {
            struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];

            if (spark->variant >= 0) {
                s32 size = (spark->variant >> 3) + 1;

                draw[i & 1](canvas, sheet + Data_08197410[size - 1],
                    HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
                BattleFxKernels_IntegrateVector2(spark, 62, 0x1000);
                spark->variant--;
            }
        }
        Func_08158ce0(8, 8);
        Func_081434f8();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Func_081180e8(134);
    Func_0814cca8();
    for (i = 0; i != 15; i++)
        ResourceObject_ReleaseFar(work->objects[i]);
    Scheduler_RemoveCallback(Func_08143000);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
