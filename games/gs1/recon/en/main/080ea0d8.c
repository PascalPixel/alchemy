#include "types.h"

/*
 * Battle-presentation scene at main:080ea0d8 (5756 bytes, the largest owner
 * in the 0x03001eec "battle work" family).  The single argument is the effect
 * state pointer, stored at work + 0x7828 exactly like every other member of
 * that family -- see games/gs1/recon/en/main/080e2538.c, 080e7404.c,
 * 080eb754.c and 080e823c.c for the shared prologue, the Value_XXXXXXXX
 * effect-id idiom and the Data_080ede48 sprite-cell table this owner reuses.
 *
 * The owner runs two self-contained animation passes, each one frame per
 * iteration, with Func_080030f8(1) as the frame barrier:
 *
 *   pass 1 (frames 0..159, cancellable from frame 5 through 0x03001b04)
 *     seeds two 128-entry particle tables at 0x02010000 / 0x02010e00, clamps
 *     eight progressively darker copies of a 770-byte ramp into work+0x2710,
 *     pushes a palette through the 0x03001388 IWRAM block copier, then per
 *     frame draws a 64-entry ring of billboards, steps sixteen scene objects
 *     down a receding grid, and from frame 116 onward blits a four-corner
 *     panel plus a growing 128-sample ring of sprites.
 *
 *   pass 2 (frames 0..319) rebuilds the 120x120 byte canvas at `work`, runs a
 *     cue/skip state machine (skipping forward to 150 / 214 / 280 when the
 *     player presses through), draws the staged sprite sheets, rasterises
 *     `rings` concentric Bresenham ellipses into that canvas, blits it whole
 *     through the second draw routine, and finally rains sixteen particles
 *     from work + 0x7080.
 *
 * `Func_080072f0`, `Func_080072f4`, `Func_080072f8`, `Func_08007308` and
 * `Func_0800730c` are NOT real callees: they are the r3/r4/r5/r9/sl entries of
 * the `_call_via_rN` trampoline bundle at games/gs1/asm/080072e4.s
 * (0x080072e4 + 4*N).  Every one of those `bl` sites is an indirect call
 * through whatever function pointer the compiler happened to place in that
 * register.  Here they are only ever the two rectangle blitters that
 * Func_080ed408(46, ...) and Func_080ed408(47, ...) publish into
 * heap_cache[7] (0x03001f08) and heap_cache[8] (0x03001f0c), and the fixed
 * IWRAM block routines at 0x03001388 and 0x03000168.  This draft therefore
 * spells the r4/r5/r9/sl sites as ordinary indirect calls through
 * `DrawRectangleFn` locals and keeps the established
 * `Func_080072f0(dest, a, b, routine)` spelling for the r3 site, the same
 * reading already adopted in games/gs1/recon/en/main/080e2538.c,
 * 080e08c0.c and 0801faa8.c.
 *
 * Known-deliberate oddities preserved from the reference rather than
 * "corrected":
 *   - the first particle seed writes field_10 twice and never writes the
 *     value it computed for the middle slot (0x080ea2ae/0x080ea2be);
 *   - the per-frame billboard block guarded by `frame < 0` in pass 1 and the
 *     party-member cue guarded by `frame > 319` in pass 2 can never run at
 *     runtime (pass 2 stops at 320 immediately after the increment), but the
 *     reference emits both in full, so both are kept;
 *   - the deferred-register queue disables interrupts by storing 0x208 (its
 *     own register address) into REG_IME rather than 0;
 *   - `(v * 8) / 8` in the ellipse rasteriser is a real signed round-toward-
 *     zero division the reference computes, not an identity to fold away.
 *
 * Uncertain: the roles of heap_cache[2] and heap_cache[5], the layout beyond
 * the fields touched here of the 28-byte particle record and of the two
 * Func_08009008 argument records, and whether the pool-loaded small constants
 * spelled `&Value_XXXXXXXX` below are link-time ids (the family precedent) or
 * plain literals the compiler chose to pool.
 */

typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 w, s32 h);

/* 28-byte animation record, shared by the two fixed tables at 0x02010000 and
   0x02010e00 and by the sixteen-entry table at work + 0x7080. */
typedef struct {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    s32 field_0c;
    s32 field_10;
    s32 field_14;
    s32 field_18;
} Particle;

/* Argument records handed to Func_08009008. */
typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 field_0c;
} PlaceWork;

typedef struct {
    s32 field_00;
    s32 field_04;
} ScalePair;

typedef struct {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    s32 field_0c;
} ScaleWork;

/* Three-word scene vector used by the effect-step helpers. */
typedef struct {
    s32 x;
    s32 y;
    s32 z;
} Vec3;

/* DMA3 as three consecutive words; the reference merges the writes into one
   stmia, which the Thumb backend has no pattern for (see the analysis in
   games/gs1/recon/en/main/08004838.c).  Three ordinary stores is the closest
   ordinary C reaches. */
struct DmaChannel {
    const void *src;
    void *dst;
    u32 ctrl;
};

extern char Value_00000000;
extern char Value_00000036;
extern char Value_0000003a;
extern char Value_0000003b;
extern char Value_0000003e;
extern char Value_00000064;
extern char Value_00000065;
extern char Value_00000067;
extern char Value_00000070;
extern char Value_00000073;
extern char Value_00000080;
extern char Value_000000bb;
extern char Value_000000ce;
extern char Value_00000100;
extern char Value_03000168;
extern char Value_03001388;

extern const u16 Data_080ede48[];
extern const ScalePair Data_080edad0;
extern const u8 Data_080eef28[];
extern const u16 Data_080eef30[];
extern const u16 Data_080eef3e[];
extern const u8 Data_080eef4a[];
extern const u8 Data_080eef50[];

s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_080022fc(s32 a, s32 b);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
/* Runtime_ReleaseHeapBlock */
void Func_08002dd8(s32 kind);
/* GetResource */
void *Func_08002f40(s32 id);
void Func_080030f8(s32 frames);
/* Scheduler_AddOrUpdateCallback */
void Func_080041d8(void *callback, s32 interval);
/* Scheduler_RemoveCallback */
void Func_08004278(void *callback);
/* Random16 */
s32 Func_08004458(void);
/* Render_ResetTransformState */
void Func_080049ac(void);
/* SceneTransform_ApplyPitch */
void Func_08004bd4(s32 angle);
/* SceneTransform_ApplyYaw */
void Func_08004c1c(s32 angle);
void Func_08004c6c(s32 angle);
/* Graphics_PrepareTransferInIwramWork */
void Func_080051d8(void *work, void *out);
/* _call_via_r3 thunk, games/gs1/asm/080072e4.s */
void Func_080072f0(void *dest, s32 arg1, s32 arg2, void *routine);
void Func_08009008(void *object, PlaceWork *place, void *scale, s32 flag);
void Func_08009020(void *object, s32 order);
void *Func_08009030(s32 id);
void Func_08009038(void *object);
void Func_080b5038(s32 channel, s32 id, s32 arg);
void Func_080b5040(s32 channel, s32 id, s32 arg);
void Func_080b50e8(s32 id);
void Func_080c9048(void);
void Func_080cd104(s32 a, s32 b);
void Func_080cd594(s32 mode);
s32 Func_080cdbc0(void);
/* BattleEffect_SelectLivingTargets */
void Func_080d6750(void *object);
void Func_080d67dc(void);
/* object/group/update_members.c */
void Func_080d6888(s32 member, s32 b, s32 c, s32 d, s32 e);
/* BattleEffect_SpawnObjects */
void Func_080dbb24(s32 a, s32 b, s32 c);
/* Resource_LoadAndDecompress */
void Func_080e0524(s32 id, void *target, s32 flag_a, s32 flag_b);
/* EffectStep_AdvanceWithGravity3D */
void Func_080e38b8(Particle *particle, s32 step, s32 gravity);
/* EffectPosition_ApplyBaseAndYOffset */
void Func_080e3944(const void *source, Vec3 *out);
s32 Func_080ed408(s32 kind, s32 a, s32 b, s32 c, s32 d);
void Func_080f9010(s32 id);

#define WORK_S32(off) (*(s32 *)((u8 *)work + (off)))
#define WORK_OBJ(i) (((void **)((u8 *)work + 0x77D8))[i])

/*
 * Push one deferred register write onto the 32-entry queue at 0x02002090
 * with interrupts masked.  Written as a macro because the reference inlines
 * the whole sequence at each of its four sites.
 */
#define QUEUE_REGISTER_WRITE(value, reg, mode)                                \
    do {                                                                      \
        u16 saved;                                                            \
        s32 cnt;                                                              \
        u32 *entry;                                                           \
                                                                              \
        saved = *(u16 *)0x04000208;                                           \
        *(u16 *)0x04000208 = 0x208;                                           \
        cnt = *(u16 *)0x02002090;                                             \
        if (cnt <= 31) {                                                      \
            entry = (u32 *)((u8 *)0x02002090 + cnt * 12 + 4);                 \
            *(u16 *)0x02002090 = cnt + 1;                                     \
            *entry++ = (u32)(value);                                          \
            *entry++ = (u32)(reg);                                            \
            *entry = (u32)(mode);                                             \
        }                                                                     \
        *(u16 *)0x04000208 = saved;                                           \
    } while (0)

void Func_080ea0d8(void *object)
{
    void **heap;
    void *canvas;
    void *work;
    void *ctrl;
    void *ramp;
    void *iwram;
    struct DmaChannel *dma;
    DrawRectangleFn draw;
    DrawRectangleFn draw2;
    Particle *dust;
    Particle *spark;
    Particle *rain;
    u32 fill;
    s32 frame;
    s32 depth;
    s32 zoom;
    s32 near;
    s32 far;
    s32 radius;
    s32 cnt;
    s32 i;
    s32 j;
    s32 lim;
    s32 pos;
    s32 span;
    s32 wide;
    s32 high;
    s32 x;
    s32 y;
    s32 rings;
    s32 ring;
    s32 shift;
    s32 slope;
    s32 err;
    s32 cx;
    s32 cy;
    s32 squash;

    heap = (void **)0x03001EEC;
    canvas = heap[1];
    work = heap[0];
    ctrl = heap[5];
    ramp = heap[2];
    iwram = *(void **)0x03001E80;
    dma = (struct DmaChannel *)0x040000D4;
    dust = (Particle *)0x02010000;
    spark = (Particle *)0x02010E00;
    rain = (Particle *)((u8 *)work + 0x7080);

    *(void **)((u8 *)work + 0x7828) = object;
    Func_080cd594(128 << 6);
    *(u16 *)0x04000020 = (s32)&Value_00000100;
    Func_080c9048();
    *(u16 *)0x05000000 = (s32)&Value_00000000;
    *(u16 *)0x05000002 = (s32)&Value_00000000;
    Func_080ed408(46, 7, 7, 3, 3);
    draw = (DrawRectangleFn)heap[7];
    WORK_S32(0x7780) = 0;
    Func_080041d8((void *)0x080CD261, 0x480);
    Func_080cd104(0, 0);
    Func_080d6750(*(void **)((u8 *)work + 0x7828));
    Func_080dbb24(16, 0x17E, 1);
    *(s32 *)0x03001CF0 = 240;
    Func_080030f8(1);
    Func_080b5040(1, (s32)&Value_0000003b, 0);
    *(s32 *)((u8 *)ctrl + 16) = 1;
    *(u16 *)0x03001AD4 = 0;
    Func_080cd104(0, 1);
    *(u16 *)0x04000000 = 0x7741;
    *(u16 *)0x04000020 = (s32)&Value_00000080;
    *(u16 *)0x04000052 = 0x1010;
    *(u16 *)0x04000050 = 0x3F44;

    depth = (s32)0xFFC00000;
    zoom = 128 << 12;
    near = 0xFFFF;
    far = 0;

    Func_080e0524((s32)&Value_000000bb, work, 1, 1);
    Func_080e0524((s32)&Value_00000067, (u8 *)work + 0x600, 0, 0);
    Func_080e0524((s32)&Value_000000ce, (u8 *)work + 0x95C, 1, 0);
    Func_080e0524((s32)&Value_00000073, ramp, 0, 0);

    /* Eight progressively darker clamped copies of the 770-byte ramp. */
    lim = 64;
    pos = 0;
    i = 0;
    do {
        const u8 *src;
        u8 *dst;

        src = (const u8 *)ramp;
        dst = (u8 *)work + pos + 0x2710;
        j = 0;
        do {
            s32 v;

            v = *src;
            src++;
            if (v > lim) {
                v = lim;
            }
            if (v < 0) {
                v = 0;
            }
            j++;
            *dst = (u8)v;
            dst++;
        } while (j != 770);
        i++;
        pos += 770;
        lim -= 7;
    } while (i != 8);

    Func_080072f0((void *)0x05000000,
        (s32)Func_08002f40((s32)&Value_00000064), 128, &Value_03001388);

    i = 0;
    do {
        dust[i].field_00 = (Func_08004458() & 15) + 72;
        dust[i].field_0c = Func_08004458() & 0xFFFF;
        dust[i].field_10 = Func_08004458() & 0xFFFF;
        dust[i].field_10 = Func_08004458() & 0xFFFF;
        i++;
    } while (i != 128);

    i = 0;
    do {
        s32 angle;

        angle = Func_08004458() & 0xFFFF;
        spark[i].field_00 = (255 * Func_08002322(angle)) >> 2;
        spark[i].field_08 = (255 * Func_0800231c(angle)) >> 2;
        spark[i].field_04 = ((Func_08004458() & 63) + 32) << 16;
        spark[i].field_0c = 0;
        spark[i].field_10 = 0;
        spark[i].field_18 = angle;
        i++;
    } while (i != 128);

    WORK_S32(0x7780) = 2;
    WORK_S32(0x7784) = 50;
    *(u16 *)0x0400000C = 0x784;

    span = -1424;
    frame = 0;
    do {
        Vec3 scene;
        Vec3 anchor;
        PlaceWork place;
        ScalePair pair;

        if (frame == 143) {
            Func_080072f0(canvas, 128 << 7, 0x2A2A2A2A, &Value_03000168);
            Func_080f9010(145);
        }
        if (frame == 80) {
            Func_080f9010(142);
        }

        /*
         * Unreachable at runtime -- `frame` is never negative -- but the
         * reference emits the whole block, so it is reproduced here.
         */
        if (frame < 0) {
            Func_080049ac();
            Func_080051d8(iwram, (u8 *)iwram + 12);
            i = 0;
            do {
                s32 size;

                Func_080e38b8(&spark[i], 60, -1024);
                Func_080e3944(&spark[i], &scene);
                scene.x >>= 1;
                scene.y -= 120;
                if (scene.z <= 99) {
                    scene.z = 100;
                }
                if (scene.z > 900) {
                    scene.z = 900;
                }
                scene.z -= 100;
                size = Func_080022ec(scene.z, 100) + 1;
                if (near > scene.z) {
                    near = scene.z;
                }
                if (far < scene.z) {
                    far = scene.z;
                }
                draw(canvas, (u8 *)ramp + Data_080ede48[size - 1],
                    scene.x - size / 2, scene.y - size, size, size * 2);
                if (frame == 64) {
                    spark[i].field_0c +=
                        (255 * Func_08002322(spark[i].field_18)) >> 3;
                    spark[i].field_10 +=
                        (255 * Func_0800231c(spark[i].field_18)) >> 3;
                }
                i++;
            } while (i != 128);
        }

        if (frame == 72) {
            WORK_S32(0x77B4) = 24;
            WORK_S32(0x77B8) = 0;
        }

        anchor.z = 0;
        anchor.y = 0;
        i = 0;
        do {
            if (frame > i / 4 + 80 && dust[i].field_00 > 0) {
                s32 size;

                Func_080049ac();
                Func_08004c6c(dust[i].field_10);
                Func_08004bd4(dust[i].field_0c);
                Func_08004c1c(dust[i].field_10 + (frame << 9));
                anchor.x = dust[i].field_00;
                dust[i].field_00 -= 2;
                if (dust[i].field_00 < 0) {
                    dust[i].field_00 = 0;
                }
                Func_080e3944(&anchor, &scene);
                if (scene.z < -60) {
                    scene.z = -60;
                }
                if (scene.z > 60) {
                    scene.z = 60;
                }
                scene.z += 60;
                size = Func_080022ec(scene.z, 20) + 2;
                scene.x += 60;
                scene.y += 80;
                draw(canvas, (u8 *)ramp + Data_080ede48[size - 1],
                    scene.x - size / 2, scene.y - size, size, size * 2);
            }
            i++;
        } while (i != 64);

        pair = Data_080edad0;
        place.field_0c = 0;
        place.y = 0;
        i = 0;
        do {
            place.x = ((i % 4) << 21) + (152 << 15);
            place.z = ((i / 4) << 21) + depth;
            Func_08009008(WORK_OBJ(i), &place, &pair, 0);
            i++;
        } while (i != 16);

        depth += zoom;
        if (frame <= 47) {
            zoom += -16384;
        }
        if (frame > 32) {
            zoom = zoom * 60 / 64;
        }
        if (frame == 144) {
            zoom = (s32)0xFFF80000;
        }
        if (frame == 146) {
            zoom = 128 << 9;
        }

        if (frame == 72) {
            i = 0;
            do {
                Func_08009020(WORK_OBJ(i), i + 16);
                i++;
            } while (i != 16);
        }
        if (frame == 76) {
            i = 0;
            do {
                Func_08009020(WORK_OBJ(i), i + 32);
                i++;
            } while (i != 16);
        }

        if (frame >= 116 && frame <= 143) {
            const u8 *cell;
            s32 idx;
            s32 off;

            idx = (frame - 116) / 4;
            if (idx > 6) {
                idx = 6;
            }
            high = Data_080eef28[idx];
            off = Data_080eef30[idx] + (192 << 3);

            Func_080ed408(47, 7, 7, 3, 2);
            cell = (const u8 *)work + off;
            draw2 = (DrawRectangleFn)heap[8];
            draw2(canvas, cell, 60 - high, 80 - high * 2, high, high * 2);
            Func_08002dd8(47);

            Func_080ed408(47, 7, 7, 7, 2);
            draw2 = (DrawRectangleFn)heap[8];
            draw2(canvas, cell, 60, 80 - high * 2, high, high * 2);
            Func_08002dd8(47);

            Func_080ed408(47, 7, 7, 11, 2);
            draw2 = (DrawRectangleFn)heap[8];
            draw2(canvas, cell, 60 - high, 80, high, high * 2);
            Func_08002dd8(47);

            Func_080ed408(47, 7, 7, 15, 2);
            draw2 = (DrawRectangleFn)heap[8];
            draw2(canvas, cell, 60, 80, high, high * 2);
            Func_08002dd8(47);

            i = 0;
            do {
                s32 slot;
                s32 angle;

                slot = i & 3;
                angle = Func_08004458() & 0xFFFF;
                wide = Data_080eef4a[slot];
                x = ((Func_08002322(angle) * 16) >> 16) - (wide >> 1);
                high = Data_080eef50[slot];
                y = ((Func_0800231c(angle) * 16) >> 16) - (high >> 1);
                draw(canvas, (u8 *)work + Data_080eef3e[slot], x + 60,
                    y + 80, wide, high);
                i++;
            } while (i != 6);
        }

        if (frame > 143) {
            s32 idx;
            s32 off;

            off = (frame << 4) - 2272;
            Func_080ed408(47, 7, 7, 3, 3);
            draw2 = (DrawRectangleFn)heap[8];
            draw2(canvas, work, 36, off, 24, 64);
            Func_08002dd8(47);

            Func_080ed408(47, 7, 7, 7, 3);
            draw2 = (DrawRectangleFn)heap[8];
            draw2(canvas, work, 60, off, 24, 64);
            Func_08002dd8(47);

            idx = (frame - 144) / 2;
            WORK_S32(0x7784) = 75;
            if (idx <= 6) {
                const u8 *cell;

                cell = (const u8 *)work + idx * 770 + Data_080ede48[7]
                    + 10000;
                Func_080ed408(47, 7, 7, 3, 2);
                draw2 = (DrawRectangleFn)heap[8];
                i = 0;
                do {
                    s32 angle;

                    angle = i << 9;
                    x = ((span * Func_08002322(angle)) >> 16) - 4;
                    y = (span * Func_0800231c(angle)) >> 17;
                    draw2(canvas, cell, x + 60, y + 72, 8, 16);
                    i++;
                } while (i != 128);
                Func_08002dd8(47);
            }
        }

        WORK_S32(0x7824) = 1;
        Func_080030f8(1);
        span += 10;
        frame++;
    } while (frame != 160
        && (frame <= 4 || (*(s32 *)0x03001B04 & 3) == 0));

    Func_080072f0(canvas, 128 << 7, 0, &Value_03000168);

    i = 0;
    do {
        Func_08009038(WORK_OBJ(i));
        i++;
    } while (i != 16);

    i = 0;
    do {
        void *obj;

        obj = Func_08009030(195 << 1);
        WORK_OBJ(i) = obj;
        if (obj != NULL) {
            *((u8 *)obj + 38) = 0;
            Func_08009020(obj, Func_080022fc(i, 3));
            *((u8 *)WORK_OBJ(i) + 9) &= ~12;
        }
        i++;
    } while (i != 16);

    Func_08002dd8(46);
    Func_080ed408(46, 7, 7, 3, 2);
    draw = (DrawRectangleFn)heap[7];
    Func_080e0524((s32)&Value_00000064, (u8 *)work + (128 << 7), 1, 1);

    {
        u8 *p;

        p = (u8 *)ramp;
        i = 0;
        do {
            s32 v;

            v = *p;
            if ((u32)v > 32) {
                *p = (u8)(v + 224);
            } else if (v != 0) {
                *p = 1;
            }
            i++;
            p++;
        } while (i != 770);
    }

    WORK_S32(0x77B4) = 0;
    WORK_S32(0x77B8) = 0;
    Func_080b5040(1, (s32)&Value_0000003e, 0);
    *(u16 *)0x04000020 = (s32)&Value_00000100;
    *(u32 *)0x04000028 = -15360;
    *(u16 *)0x0400000C = 0x784;

    fill = 0;
    dma->src = &fill;
    dma->dst = canvas;
    dma->ctrl = 0x85001000;

    cx = 60;
    cy = 44;
    shift = 0;
    ring = 0;
    squash = 6;
    rings = 2;
    WORK_S32(0x7780) = 2;
    WORK_S32(0x7784) = 75;

    fill = 0;
    dma->src = &fill;
    dma->dst = work;
    dma->ctrl = 0x85000E10;

    frame = 0;
    do {
        if (frame == 66) {
            Func_080f9010(145);
            Func_080f9010(141);
        }
        if (frame == 155) {
            Func_080f9010(162);
        }
        if (frame == 217) {
            Func_080f9010(156);
        }
        if (frame == 140 * 2) {
            Func_080f9010(157);
        }
        if (frame == 150 * 2) {
            Func_080b50e8(145);
        }

        if ((*(s32 *)0x03001B04 & 3) != 0) {
            if (frame >= 5 && frame <= 149) {
                frame = 150;
                QUEUE_REGISTER_WRITE(0x80, 0x04000020, 128 << 10);
                QUEUE_REGISTER_WRITE(0, 0x04000028, 192 << 10);
                Func_080e0524((s32)&Value_00000070,
                    (u8 *)work + (128 << 7), 1, 0);
            } else if (frame >= 155 && frame <= 213) {
                frame = 214;
            } else if (frame >= 219 && frame <= 279) {
                frame = 140 * 2;
            }
        }

        if (frame == 64) {
            QUEUE_REGISTER_WRITE(0x80, 0x04000020, 128 << 10);
            QUEUE_REGISTER_WRITE(0, 0x04000028, 192 << 10);
            fill = 0;
            dma->src = &fill;
            dma->dst = canvas;
            dma->ctrl = 0x85001000;
            Func_080e0524((s32)&Value_00000070,
                (u8 *)work + (128 << 7), 1, 0);
            Func_080e0524((s32)&Value_00000065,
                (u8 *)work + (192 << 7), 0, 0);
        }

        if (frame == 66) {
            u16 *pal;

            pal = (u16 *)0x050000C0;
            i = 0;
            do {
                *pal ^= 0x7FFF;
                i++;
                pal++;
            } while (i != 128);
        }

        if (frame == 69) {
            fill = 0x3F3F3F3F;
            dma->src = &fill;
            dma->dst = canvas;
            dma->ctrl = 0x85001000;
        }

        if (frame == 70) {
            Func_080b5038(1, (s32)&Value_0000003e, 7);
        }

        if (frame == 150) {
            cx = 112;
            cy = 32;
            ring = 0;
            fill = 0;
            rings = 4;
            squash = 8;
            dma->src = &fill;
            dma->dst = work;
            dma->ctrl = 0x85000E10;
            fill = 0x3F3F3F3F;
            dma->src = &fill;
            dma->dst = canvas;
            dma->ctrl = 0x85001000;
            Func_080b5040(1, (s32)&Value_00000036, 0);
        }

        if (frame == 214) {
            fill = 0;
            dma->src = &fill;
            dma->dst = work;
            dma->ctrl = 0x85000E10;
            fill = 0x3F3F3F3F;
            dma->src = &fill;
            dma->dst = canvas;
            dma->ctrl = 0x85001000;
            Func_080b5040(1, (s32)&Value_0000003a, 0);

            i = 0;
            do {
                rain[i].field_00 = ((Func_08004458() & 63) + 32) << 16;
                rain[i].field_04 = ((Func_08004458() & 31) + 96) << 16;
                rain[i].field_10 = ((Func_08004458() & 15) + 16) << 15;
                rain[i].field_08 = (128 << 8) - (rain[i].field_00 >> 7);
                rain[i].field_18 = 1;
                i++;
            } while (i != 16);
        }

        if (frame == 140 * 2) {
            Func_080d67dc();
            fill = 0;
            dma->src = &fill;
            dma->dst = work;
            dma->ctrl = 0x85000E10;
            fill = 0x01010101;
            dma->src = &fill;
            dma->dst = canvas;
            dma->ctrl = 0x85001000;
            *(u16 *)0x04000052 = 0x1010;
            *(s32 *)((u8 *)ctrl + 16) = 0;

            i = 0;
            do {
                rain[i].field_00 = ((Func_08004458() & 63) + (128 << 1))
                    << 16;
                rain[i].field_04 = ((Func_08004458() & 31) + 96) << 16;
                rain[i].field_10 = ((Func_08004458() & 15) + 16) << 15;
                rain[i].field_08 = 0;
                i++;
            } while (i != 16);

            shift = 136 * 2;
            WORK_S32(0x7780) = 0;
            WORK_S32(0x7784) = 0;
        }

        if (frame > 279) {
            u16 *pal;

            pal = (u16 *)0x05000002;
            i = 0;
            do {
                s32 c;
                s32 r;
                s32 g;
                s32 b;

                c = *pal;
                b = ((c >> 10) & 31) + 1;
                g = ((c >> 5) & 31) + 1;
                r = (c & 31) + 1;
                if (b > 31) {
                    b = 31;
                }
                if (g > 31) {
                    g = 31;
                }
                if (r > 31) {
                    r = 31;
                }
                i++;
                *pal = (u16)((b << 10) | (g << 5) | r);
                pal++;
            } while (i != 63);
        }

        if (frame == 182) {
            fill = 0x3F3F3F3F;
            dma->src = &fill;
            dma->dst = canvas;
            dma->ctrl = 0x85001000;
        }

        if (frame <= 63) {
            const u8 *sheet;
            s32 top;

            sheet = (const u8 *)work + (128 << 7);
            if ((frame & 7) > 3) {
                sheet = (const u8 *)work + 0x4240;
            }
            top = frame - 28;

            Func_080ed408(47, 7, 7, 3, 2);
            draw2 = (DrawRectangleFn)heap[8];
            draw2(canvas, sheet, 36, top, 24, 24);
            Func_08002dd8(47);

            Func_080ed408(47, 7, 7, 7, 2);
            draw2 = (DrawRectangleFn)heap[8];
            draw2(canvas, sheet, 59, top, 24, 24);
            Func_08002dd8(47);

            Func_080ed408(47, 7, 7, 11, 2);
            draw2 = (DrawRectangleFn)heap[8];
            draw2(canvas, sheet, 36, top + 23, 24, 24);
            Func_08002dd8(47);

            Func_080ed408(47, 7, 7, 15, 2);
            draw2 = (DrawRectangleFn)heap[8];
            draw2(canvas, sheet, 59, top + 23, 24, 24);
            Func_08002dd8(47);
        }

        if (frame >= 64 && frame <= 65) {
            draw(canvas, (u8 *)work + (192 << 7), 52, 51, 16, 17);
        }
        if (frame >= 66 && frame <= 67) {
            draw(canvas, (u8 *)work + 0x6110, 48, 40, 24, 41);
        }

        if (frame >= 68 && frame <= 75) {
            const u8 *petal;
            s32 d;

            petal = (const u8 *)work + 0x64E8;
            d = 76 - frame;
            draw(canvas, petal, 49 - d / 2, 38 - d, 22, 44);
            draw(canvas, petal, 49 - d / 2, d + 38, 22, 44);
            draw(canvas, petal, 49 + d / 2, 38 - d, 22, 44);
            draw(canvas, petal, 49 + d / 2, d + 38, 22, 44);
        }

        if (frame >= 78 && frame <= 79) {
            draw(canvas, (u8 *)work + (192 << 7), 52, 51, 16, 17);
        }
        if (frame >= 80 && frame <= 81) {
            draw(canvas, (u8 *)work + 0x6110, 48, 40, 24, 41);
        }

        if (frame >= 82 && frame <= 85) {
            const u8 *petal;
            s32 d;

            petal = (const u8 *)work + 0x64E8;
            d = frame * 2 - 164;
            draw(canvas, petal, 49 - d / 4, 38 - d, 22, 44);
            draw(canvas, petal, 49 - d / 4, frame * 2 - 126, 22, 44);
            draw(canvas, petal, 49 + d / 4, 38 - d, 22, 44);
            draw(canvas, petal, 49 + d / 4, frame * 2 - 126, 22, 44);
        }

        if (frame >= 72 && frame <= 75) {
            s32 radius2;

            radius2 = (frame << 4) - 1136;
            i = 0;
            do {
                s32 angle;

                angle = i << 8;
                x = ((radius2 * Func_08002322(angle)) >> 16) - 5;
                y = (radius2 * Func_0800231c(angle)) >> 17;
                draw(canvas, (u8 *)ramp + Data_080ede48[10 - 1], x + 60,
                    y + 50, 10, 20);
                i++;
            } while (i != 128 * 2);
        }

        if (frame > 85 && frame <= 213 && rings != 0) {
            i = 0;
            do {
                radius = ring;
                slope = 0;
                err = ring;
                while (radius >= slope) {
                    s32 half;
                    s32 left;
                    s32 right;
                    s32 top;
                    s32 bottom;

                    half = radius / 2;
                    right = cx + (half * 8) / 8;
                    left = cx - (half * 8) / 8;
                    bottom = cy + (slope * squash) / 8;
                    top = cy - (slope * 8) / 8;
                    if (top < 0) {
                        top = 0;
                    }
                    if (bottom > 119) {
                        bottom = 119;
                    }
                    if (left < 0) {
                        left = 0;
                    }
                    if (right > 119) {
                        right = 119;
                    }
                    ((u8 *)work)[bottom * 120 + right] = 20;
                    ((u8 *)work)[top * 120 + right] = 20;
                    ((u8 *)work)[bottom * 120 + left] = 20;
                    ((u8 *)work)[top * 120 + left] = 20;
                    right = cx + half + 1;
                    left = cx - half + 1;
                    if (left < 0) {
                        left = 0;
                    }
                    if (right > 119) {
                        right = 119;
                    }
                    ((u8 *)work)[bottom * 120 + right] = 20;
                    ((u8 *)work)[top * 120 + right] = 20;
                    ((u8 *)work)[bottom * 120 + left] = 20;
                    ((u8 *)work)[top * 120 + left] = 20;

                    half = slope / 2;
                    right = cx + (half * 8) / 8;
                    left = cx - (half * 8) / 8;
                    bottom = cy + (radius * squash) / 8;
                    top = cy - (radius * 8) / 8;
                    if (left < 0) {
                        left = 0;
                    }
                    if (right > 119) {
                        right = 119;
                    }
                    if (top < 0) {
                        top = 0;
                    }
                    if (bottom > 119) {
                        bottom = 119;
                    }
                    ((u8 *)work)[bottom * 120 + right] = 20;
                    ((u8 *)work)[top * 120 + right] = 20;
                    ((u8 *)work)[bottom * 120 + left] = 20;
                    ((u8 *)work)[top * 120 + left] = 20;
                    right = cx + half + 1;
                    left = cx - half + 1;
                    if (left < 0) {
                        left = 0;
                    }
                    if (right > 119) {
                        right = 119;
                    }
                    ((u8 *)work)[bottom * 120 + right] = 20;
                    ((u8 *)work)[top * 120 + right] = 20;
                    ((u8 *)work)[bottom * 120 + left] = 20;
                    ((u8 *)work)[top * 120 + left] = 20;

                    err = err - slope * 2 - 1;
                    if (err < 0) {
                        err = err + radius * 2 - 2;
                        radius--;
                    }
                    slope++;
                }
                ring++;
                i++;
            } while (i != rings);
        }

        if (frame >= 86 && frame <= 149) {
            s32 angle;

            angle = -16384;
            i = 0;
            do {
                s32 cell;

                x = ((frame - 86) * Func_08002322(angle)) >> 16;
                y = ((frame - 70) * Func_0800231c(angle)) >> 16;
                cell = (frame / 2 + i) % 4;
                draw(canvas, (u8 *)work + cell * 1728 + (128 << 7), x + 44,
                    y + 17, 32, 54);
                i++;
                angle += 128 << 5;
            } while (i != 9);
        }

        Func_080ed408(47, 7, 7, 3, 3);
        draw2 = (DrawRectangleFn)heap[8];
        if (frame > 85) {
            draw2(canvas, work, 0, 0, 120, 120);
        }

        if (frame > 279) {
            shift -= 8;
        }

        if (frame > 319) {
            void *state;

            state = *(void **)((u8 *)work + 0x7828);
            if (*(s32 *)((u8 *)state + 20) != 0) {
                i = 0;
                pos = 36;
                do {
                    Func_080d6888(
                        *(s16 *)((u8 *)(*(void **)((u8 *)work + 0x7828))
                            + pos),
                        14, 5, -1, 0);
                    i++;
                    pos += 2;
                } while (i
                    != *(s32 *)((u8 *)(*(void **)((u8 *)work + 0x7828))
                        + 20));
            }
        }

        if (frame == 238) {
            fill = 0x3F3F3F3F;
            dma->src = &fill;
            dma->dst = canvas;
            dma->ctrl = 0x85001000;
        }

        if (frame >= 214 && frame <= 279) {
            *(u16 *)0x03001AD4 += 8;
        }

        if (frame >= 246 && frame <= 279) {
            shift += 8;
            WORK_S32(0x7780) = 0;
            WORK_S32(0x7784) = 75;
        }

        if (frame > 213) {
            PlaceWork place2;
            ScaleWork scale;

            place2.field_0c = 0;
            place2.y = 0;
            i = 0;
            do {
                if (rain[i].field_18 != 0) {
                    s32 z;

                    z = rain[i].field_08 + (rain[i].field_04 >> 7);
                    if (z <= 2047) {
                        z = 128 << 4;
                    }
                    scale.field_04 = z;
                    place2.y = 255 << 16;
                    place2.x = rain[i].field_00;
                    place2.z = rain[i].field_04 + (255 << 16);
                    Func_08009008(WORK_OBJ(i), &place2, &scale, 0);
                    rain[i].field_04 -= rain[i].field_10;
                    if (rain[i].field_04 <= 0xFFFFF) {
                        if (frame > 279) {
                            rain[i].field_00 =
                                ((Func_08004458() & 63) + shift - 32) << 16;
                        } else {
                            rain[i].field_00 =
                                ((Func_08004458() & 63) + shift + 32) << 16;
                        }
                        if (frame > 301) {
                            rain[i].field_18 = 0;
                        } else if (frame > 245) {
                            rain[i].field_04 = 192 << 15;
                        } else {
                            rain[i].field_04 = (192 << 16) - rain[i].field_00;
                        }
                    }
                }
                i++;
            } while (i != 16);

            if (frame <= 279) {
                i = 0;
                do {
                    s32 cell;

                    x = ((i & 1) * 5) * 4 - (Func_08004458() & 15)
                        + (i / 2) * 4 + shift / 2 - 16;
                    y = (i / 2) * 32 - (Func_08004458() & 15);
                    cell = (frame / 2 + i) % 4;
                    draw2(canvas, (u8 *)work + cell * 1728 + (128 << 7), x,
                        y, 32, 54);
                    i++;
                } while (i != 8);
            } else {
                i = 0;
                do {
                    s32 cell;

                    x = ((i & 1) * 5) * 4 + (Func_08004458() & 15)
                        - (i / 2) * 4 + shift / 2 - 16;
                    y = (i / 2) * 32 - (Func_08004458() & 15);
                    cell = (frame / 2 + i) % 4;
                    draw2(canvas, (u8 *)work + cell * 1728 + (128 << 7), x,
                        y, 32, 54);
                    i++;
                } while (i != 8);
            }
        }

        Func_08002dd8(47);

        if (frame > 63) {
            *(u16 *)0x03001AD6 = (u16)((Func_08004458() & 3) + 30);
        }

        WORK_S32(0x7824) = 1;
        Func_080030f8(1);
        frame++;
    } while (frame != 160 * 2);

    i = 0;
    do {
        Func_08009038(WORK_OBJ(i));
        i++;
    } while (i != 16);

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(46);
    Func_080cdbc0();
}
