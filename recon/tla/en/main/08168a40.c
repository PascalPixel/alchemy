/* Draft, differ 126 of 1065: The Broken Seal's BattleFx_RunObjectRow,
 * ported: Func_0813ba50 sets the display, the second BG colour is 0x18,
 * the formation is resource 644, the projection's centre is
 * gCameraSceneParameters[4], the control is the heap slot at 0xb0, the
 * forest backdrop resource 0x44, the fire sheet resource 0x158, one
 * blitter (Func_081963ec(104, 3)), the effect stays an argument for the
 * hit loops. The objects' flag byte is or-ed as a byte (as a struct field
 * the allocation of that block moved: 171). The 126 differing rows are
 * the native's own rows reordered: loads and stores ahead of independent
 * constants, the kind Func_08164b2c's header traces to a load costing 3
 * and a store 2. */
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "input.h"
#include "bg_scroll.h"
#include "effect_step.h"
#include "battle_efx.h"
#include "battle_effect_work.h"
#include "battle_fx_work.h"

#define HI(v) (((s16 *)&(v))[1])
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

struct FxPair {
    s32 a;
    s32 b;
};

struct FxPlacement {
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_0c;
};

struct FxControl {
    u8 unknown_00[16];
    s32 busy;
};

struct ScaledSprite;

extern char ResourceId_ForestBackdrop;
extern char ResourceId_Data134;
extern char ResourceId_FireBlobSheet;
extern struct FxPair Data_08196e7c;
extern struct FxPair Data_08196e84;
extern u8 Data_08198ada[];
extern u8 Data_08198ae3[];
extern u16 Data_08197410[];
extern u32 gCameraSceneParameters[];

void BattleFx_BeginCanvasLayer(s32 layer);
void Func_0813ba50(void);
void Func_08143000(void);
void Func_08143bb8(void);
void BattleEffect_WipeCanvas(s32 mode, s32 value);
void Func_0814cc4c(struct BattleEffectArgument *effect);
void Func_08152404(s32 count, s32 resource, s32 priority);
void ResourceObject_ReleaseFar(void *object);
void Func_081963ec(s32 slot, s32 kind);
void BattleBackground_LoadFar(s32, s32, s32);
void Audio_PlayCue(s32 cue);
void Render_ApplyProjectedPlacementFar(struct ScaledSprite *sprite, s32 *position, s32 *scale, s32 mode);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
void Func_08118088(s32 actor, s32 variant);
void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_08158ce0(s32 x, s32 y);
void Func_081434f8(void);
void Func_081180e8(s32 phase);
void Func_0814cca8(void);

void Func_08168a40(struct BattleEffectArgument *effect)
{
    struct FxPlacement place;
    struct EffectPosition unit;
    u8 jitter_row[16];
    u8 hit_row[14];
    struct FxPlacement place2;
    void *canvas;
    struct BattleFxWork *work;
    DrawRectangle blit;
    u8 *sheet;
    s32 base_x;
    s32 grow;
    s32 saved_x;
    struct FxControl *control;
    s32 shift;
    s32 wave;
    s32 frame;
    s32 i;

    canvas = Ram_HeapSlots->battle_fx_work[1];
    work = (struct BattleFxWork *)Ram_HeapSlots->battle_fx_work[0];
    sheet = Ram_HeapSlots->unknown_64[0];
    BattleFx_BeginCanvasLayer(0);
    Func_0813ba50();
    REG_BG2CNT = 0x784;
    *(u16 *)0x05000000 = 0;
    *(u16 *)0x05000002 = 0x18;
    work->transfer_mode = 0;
    Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
    BattleEffect_WipeCanvas(1, 0);
    Func_08152404(9, 644, 1);
    gCameraSceneParameters[4] = 240;
    Func_0814cc4c(work->effect);
    REG_WININ = 0x2737;
    REG_WIN0H = 0xca;
    WaitFrames(1);
    BattleBackground_LoadFar(1, (s32)&ResourceId_ForestBackdrop, 0);
    BattleEffect_WipeCanvas(1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_FireBlobSheet, work->sheet, 1, 1);
    REG_DISPCNT = 0x7741;
    REG_BG2PA = 0x80;
    REG_BLDALPHA = 0x100e;
    REG_BLDCNT = 0x3f44;

    base_x = 0;
    grow = 0;
    saved_x = gBgScroll[1].x;
    control = (struct FxControl *)Ram_HeapSlots->transition_work;
    shift = 0;
    work->transfer_mode = 1;
    work->transfer_value = base_x;
    control->busy = 1;

    i = 0;
    do {
        struct EffectStep *fire = &work->particles[i];

        fire->x = (Random16() & 31) + 16;
        fire->y = ((Random16() & 31) + 48) << 16;
        fire->velocity_y = ((Random16() & 31) - 16) << 16;
        fire->variant = Random16() % 48 + 2;
        i++;
    } while (i != 64);

    Func_081963ec(104, 3);
    blit = (DrawRectangle)Ram_HeapSlots->unknown_64[1];
    REG_BG2CNT = 0x786;

    for (frame = 0; frame != 120; frame++) {
        wave = 0;
        if (frame == 0)
            Audio_PlayCue(136);
        if (frame == 26)
            Audio_PlayCue(141);
        if (frame == 40)
            Audio_PlayCue(154);
        if (frame == 72)
            Audio_PlayCue(154);
        if (frame == 104)
            Audio_PlayCue(154);
        if ((gInput.repeat & 3) != 0 && frame > 16)
            break;

        if (frame >= 24 && frame <= 55)
            shift++;
        if (shift > 24)
            shift = 24;
        if (frame <= 135) {
            gBgScroll[1].x -= shift;
            grow += shift;
        }

        if (frame <= 149) {
            struct FxPair pair = Data_08196e7c;
            s32 lift;

            lift = 0;
            if (frame > 103)
                lift = frame * 16 - 1664;
            if (frame >= 8 && frame <= 31)
                base_x = base_x + shift - 8;
            if (frame > 7) {
                s32 amp;
                s32 ang;

                amp = 96;
                if (frame <= 104)
                    amp = 32;
                ang = (frame * 1024 - 0x2000) & 0xffff;
                if (ang > 0x8000)
                    ang -= 0x8000;
                wave = (Trig_Sin(ang) * amp) >> 16;
                if ((frame & 31) == 8)
                    work->shake_frames = 4;
            }
            place.unknown_0c = 0;
            place.y = 255 << 16;
            i = 0;
            do {
                place.x = ((base_x + Data_08198ada[i] - lift) << 16) + (224 << 16);
                place.z = ((Data_08198ae3[i] - wave) << 16) + (144 << 15);
                Render_ApplyProjectedPlacementFar(work->objects[i], &place.x, &pair.a, 0);
                i++;
            } while (i != 9);
        }

        if (frame <= 26) {
            s32 size = grow + 4;
            s32 count = frame * 8;

            if (size > 10)
                size = 10;
            if (count > 64)
                count = 64;
            for (i = 0; i != count; i++) {
                s32 ang = i << 10;
                s32 x = ((Trig_Sin(ang) * (grow * 2 + 8)) >> 16) + grow + 96;
                s32 y = ((Trig_Cos(ang) * (grow * 12 + 48)) >> 16) + 64;

                blit(canvas, sheet + Data_08197410[size - 1], x - size / 2, y - size, size, size * 2);
            }
        }

        if (frame == 24) {
            work->transfer_mode = 2;
            work->transfer_value = 50;
        }
        if (frame == 28)
            REG_BG2CNT = 0x784;

        if (frame > 17) {
            i = 0;
            do {
                struct EffectStep *fire = &work->particles[i];

                if (fire->variant == 0) {
                    s32 size = i % 3 + 1;

                    blit(canvas, sheet + Data_08197410[size - 1], fire->x, HI(fire->y) - size, size, size * 2);
                    fire->x += 2;
                    fire->y += fire->velocity_y;
                    fire->velocity_y = fire->velocity_y * 48 / 64;
                } else {
                    fire->variant--;
                }
                if (fire->x > 128 || fire->variant == 1) {
                    fire->x = (Random16() & 31) + base_x + 172;
                    fire->y = ((Random16() & 31) - wave + 56) << 16;
                    fire->velocity_y = ((Random16() & 31) - 16) << 15;
                }
                i++;
            } while (i != 48);
        }

        if (frame > 31) {
            s32 rise = (frame - 32) / 2;
            s32 y;

            if (rise > 40)
                rise = 40;
            i = 0;
            y = 0;
            do {
                blit(canvas, work->sheet + (Random16() & 3) * 0x600, 120 - rise, y, 48, 32);
                i++;
                y += 18;
            } while (i != 6);
        }

        if (work->shake_frames > 0) {
            work->shake_frames--;
            gBgScroll[1].y = (Random16() & 7) + 28;
        } else {
            gBgScroll[1].y = 32;
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    gBgScroll[1].x = saved_x;
    control->busy = 0;
    Func_0814cca8();
    REG_WIN0H = 0xf0;

    i = 0;
    do {
        ((u8 *)work->objects[i])[9] |= 12;
        i++;
    } while (i != 9);

    {
        s32 row_x;
        u8 *hit;
        u8 *jitter;

        row_x = 224;
        hit = hit_row;
        i = 0;
        do {
            hit[i] = 0;
            i++;
        } while (i != 14);
        jitter = jitter_row;
        i = 0;
        do {
            jitter[i] = Random16() & 31;
            i++;
        } while (i != 16);
        i = 0;
        do {
            PARTICLES[i].variant = 0;
            i++;
        } while (i != 160 << 1);

        work->transfer_mode = 2;
        work->transfer_value = 75;
        REG_BG2CNT = 0x784;
        REG_BLDALPHA = 0x1010;

        frame = 0;
        do {
            if (frame <= 23) {
                struct FxPair pair2 = Data_08196e84;
                s32 ang;
                s32 rise;

                row_x -= 16;
                if (frame <= 8) {
                    ang = frame * 2048 + 0x4000;
                    if (ang > 0x8000)
                        ang -= 0x8000;
                    rise = (Trig_Sin(ang) << 6) >> 16;
                } else {
                    ang = frame * 2048 + 0x4000;
                    if (ang > 0x8000)
                        ang -= 0x8000;
                    rise = (Trig_Sin(ang) << 5) >> 16;
                }
                place2.unknown_0c = 0;
                place2.y = 255 << 16;
                i = 0;
                do {
                    place2.x = (row_x + Data_08198ada[i]) << 16;
                    place2.z = ((Data_08198ae3[i] - rise) << 16) + (144 << 15);
                    Render_ApplyProjectedPlacementFar(work->objects[i], &place2.x, &pair2.a, 0);
                    i++;
                } while (i != 9);
            }

            if (frame == 8) {
                work->shake_frames = frame;
                Audio_PlayCue(145);
            }
            if (frame == 11)
                Audio_PlayCue(145);
            if (frame == 46)
                Audio_PlayCue(137);

            for (i = 0; i != effect->count; i++) {
                if (hit[i] == 0) {
                    EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[i], &unit);
                    if (unit.x > row_x) {
                        s32 k;

                        hit[i] = 1;
                        k = 0;
                        do {
                            struct EffectStep *burst = &PARTICLES[i * 32 + k];
                            s32 up;

                            burst->x = unit.x << 15;
                            burst->y = (unit.y - 16) << 16;
                            burst->velocity_x = ((Random16() & 255) - 128) << 10;
                            up = (Random16() & 255) - 192;
                            burst->velocity_y = up << 11;
                            burst->x += burst->velocity_x * 4;
                            burst->y += up << 13;
                            burst->variant = (Random16() & 15) + 8;
                            k++;
                        } while (k != 32);
                        Func_08118088(effect->actors[i], 1);
                        Audio_PlayCue(134);
                    }
                }
            }

            i = 0;
            do {
                struct EffectStep *burst = &PARTICLES[i];

                if (burst->variant > 0) {
                    s32 size = 3;

                    blit(canvas, sheet + Data_08197410[size - 1], HI(burst->x) - size / 2, HI(burst->y) - size, size, size * 2);
                    burst->x += burst->velocity_x;
                    burst->y += burst->velocity_y;
                    burst->variant--;
                }
                i++;
            } while (i != 192);

            if (frame == 48)
                Audio_PlayCue(136);
            if (frame > 40) {
                s32 slide = (frame - 40) * 12;
                s32 y;

                work->transfer_mode = 0;
                work->transfer_value = 75;
                i = 0;
                y = -8;
                do {
                    blit(canvas, work->sheet + (Random16() & 3) * 0x600, jitter[i] - slide + 120, y, 48, 32);
                    i++;
                    y += 8;
                } while (i != 16);
            }
            if (frame > 64)
                work->transfer_mode = 2;
            if (frame == 58) {
                for (i = 0; i != effect->count; i++)
                    Func_0814cd48(effect->actors[i], 14, 5, -1, 0);
            }

            Func_08158ce0(8, 8);
            Func_081434f8();
            work->transfer_pending = 1;
            WaitFrames(1);
            frame++;
        } while (frame != 96);
    }

    Func_081180e8(134);
    i = 0;
    do {
        ResourceObject_ReleaseFar(work->objects[i]);
        i++;
    } while (i != 9);
    Scheduler_RemoveCallback(Func_08143000);
    Runtime_ReleaseHeapBlock(104);
    Func_08143bb8();
}
