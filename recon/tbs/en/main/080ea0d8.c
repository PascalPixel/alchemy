/* Battle effect mode 9 (BattleFx_InitializeMode dispatches kind 9 here):
   sixteen sprite objects recede over a spark field while a burst opens
   (160 frames, A or B skips after frame 4), then a second 320-frame pass
   draws concentric ellipses, falling objects and palette ramps.

   DRAFT, not yet C: 47.0% aligned similarity, 2552 differing halfwords
   (candidate 5752 bytes, reference 5756). Every call lines up with the
   reference; what remains is register allocation and spill-slot order.
   Known: the reference spills draw2 at sp+92 in the second pass (this
   draft keeps it in a register, shifting frame and later slots by 4);
   the dead frame<0 spark block keeps &scene in two high registers; the
   IO write queue inline stores 0x208 through a reloaded IME address; the
   petal blocks call draw through a callee-saved copy. Evidence used:
   (x * sx) / 8 with sx = 8 in a variable (the reference divides after a
   shift), the ring radius derived from the frame (a strength-reduced
   giv at sp+12), the gWorkSlot base spilled at sp+56 for slot 47, and
   the palette colour held as u16. Next: the palette loop masks with a
   pooled 0x1f and reads the red field from the unshifted colour. */
#include "TYPES.H"
#include "DMA.H"
#include "BATTLE_EFFECT_WORK.H"
typedef void (*DrawRectangle)(void *, const void *, s32, s32, s32, s32);
void Resource_LoadAndDecompress(s32, void *, s32, s32);
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "FIXED_MATH.H"

void WaitFrames(s32);
s32 Unnamed_080ed408(s32, s32, s32, s32, s32);
u32 Random16(void);
void Audio_PlayCue(s32);
void BattleFx_BeginCanvasLayer(s32);
void BattlePres_ConfigureEffectDisplay(void);
void Unnamed_080cd104(s32, s32);
void BattleFx_SelectLivingTargets(s32);
void BattleBackground_LoadFar(s32, s32, s32);
void BattlePresentation_ConfigurePaletteFadeFar(s32, s32, s32);
void BattleFx_SpawnObjects(s32, s32, s32);
void *Resource_GetTableEntry(s32);
void Render_ResetTransformState(void);
void SceneTransform_ApplyRoll(s32);
void SceneTransform_ApplyPitch(s32);
void SceneTransform_ApplyYaw(s32);
void Graphics_PrepareTransferInIwramWork(void *, void *);
void AnimationObjects_SelectAnimationFar(void *, s32);
void Object_ApplyProjectedPlacementFar(void *, void *, void *, s32);
void *ResourceObject_CreateFar(s32);
void ResourceObject_ReleaseFar(void *);
void BattleEffect_SetupBlendedDisplay(void);
void BattleEventRuntime_BeginPhaseFar(s32);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
void Runtime_ReleaseHeapBlock(s32);
s32 BattleFx_EndCanvasLayer(void);

extern u8 gWorkSlot[];
extern volatile u32 gKeysRepeat;

struct Cells03001ad0 {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
};
extern struct Cells03001ad0 gBgScroll;

struct Cells03001ce0 {
    s32 unk00[4];
    s32 unk10;
};
extern struct Cells03001ce0 gProjection;

extern u8 Value_0000003b;
extern u8 Value_0000003e;
extern u8 Value_00000036;
extern u8 Value_0000003a;
extern u8 Value_00000064;
extern u8 Value_00000065;
extern u8 Value_00000067;
extern u8 Value_00000070;
extern u8 Value_00000073;
extern u8 Value_000000bb;
extern u8 Value_000000ce;

typedef struct Scale {
    s32 x;
    s32 y;
} Scale;

#define BattleFx_DotCells Data_080ede48
#define BattleFx9_UnitScale Data_080edad0
#define BattleFx9_BurstHeights Data_080eef28
#define BattleFx9_BurstCells Data_080eef30
#define BattleFx9_ShardCells Data_080eef3e
#define BattleFx9_ShardWidths Data_080eef4a
#define BattleFx9_ShardHeights Data_080eef50
extern const u16 BattleFx_DotCells[];
extern const Scale BattleFx9_UnitScale;
extern const u8 BattleFx9_BurstHeights[];
extern const u16 BattleFx9_BurstCells[];
extern const u16 BattleFx9_ShardCells[];
extern const u8 BattleFx9_ShardWidths[];
extern const u8 BattleFx9_ShardHeights[];

/* The effect's work block (heap slot 39). */
struct Mode9Work {
    u8 sheet[0x7080];
    struct EffectStep rain[16];
    u8 unknown_7240[0x540];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unknown_7788[0x2c];
    s32 unknown_77b4;
    s32 unknown_77b8;
    u8 unknown_77bc[0x1c];
    void *objects[16];
    u8 unknown_7818[0xc];
    s32 transfer_pending;
    struct BattleEffectArgument *effect;
};

#define DUST ((struct EffectStep *)0x02010000)
#define SPARKS ((struct EffectStep *)0x02010e00)

typedef void (*WordFill)(void *, s32, u32);
typedef void (*WordCopy)(void *, const void *, s32);

struct IoWriteQueue {
    u16 count;
    u16 pad;
    u32 entries[32][3];
};
#define gIoWriteQueue Data_02002090
extern struct IoWriteQueue gIoWriteQueue;

#define REG_IME (*(volatile u16 *)0x04000208)

#define QUEUE_IO_WRITE(address, value, delay)                               \
    {                                                                       \
        struct IoWriteQueue *q = &gIoWriteQueue;                            \
        u16 saved = REG_IME;                                                \
        s32 count;                                                          \
        REG_IME = 0x208;                                                    \
        count = q->count;                                                   \
        if (count <= 31) {                                                  \
            u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);           \
            q->count = count + 1;                                           \
            *destination++ = (value);                                       \
            *destination++ = (address);                                     \
            *destination = (delay);                                         \
        }                                                                   \
        REG_IME = saved;                                                    \
    }

void Unnamed_080ea0d8(struct BattleEffectArgument *efx)
{
    s32 anchor[3];
    s32 scene[3];
    s32 place[4];
    u32 fill;
    s32 place2[4];
    Scale pair;
    Scale scale;
    void *canvas;
    struct Mode9Work *work;
    s32 *ctrl;
    DrawRectangle draw2;
    DrawRectangle draw;
    s32 frame;
    u8 *ramp;
    void *iwram;
    s32 depth;
    s32 zoom;
    s32 near;
    s32 far;
    u32 *cache;
    DrawRectangle *slots;
    s32 cx;
    s32 cy;
    s32 squash;
    s32 sx;
    s32 ring;
    s32 rings;
    s32 shift;
    s32 i;
    s32 i1;
    s32 i2;
    s32 i3;
    s32 i4;
    s32 i5;
    s32 i6;
    s32 i7;
    s32 i8;
    s32 i9;
    s32 i10;
    s32 i11;
    s32 i12;
    s32 i13;
    s32 i14;
    s32 i15;
    s32 i16;
    s32 i17;
    s32 i18;
    s32 i19;
    s32 i20;
    s32 i21;
    s32 i22;
    s32 i23;
    s32 i24;
    s32 i25;
    s32 j;
    s32 lim;
    s32 pos;
    s32 x;
    s32 y;
    s32 size;
    s32 high;
    s32 wide;
    struct EffectStep *p;
    DrawRectangle blit;
    s32 z;
    s32 amp;

    cache = (u32 *)0x03001ef0;
    canvas = (void *)cache[40 - 40];
    work = (struct Mode9Work *)cache[39 - 40];
    ctrl = (s32 *)cache[44 - 40];
    ramp = (u8 *)cache[41 - 40];
    iwram = (void *)cache[12 - 40];
    work->effect = efx;
    BattleFx_BeginCanvasLayer(0x2000);
    *(u16 *)0x04000020 = 0x100;
    BattlePres_ConfigureEffectDisplay();
    *(u16 *)0x05000000 = 0;
    *(u16 *)0x05000002 = 0;
    Unnamed_080ed408(46, 7, 7, 3, 3);
    draw = (DrawRectangle)cache[46 - 40];
    work->transfer_mode = 0;
    Scheduler_AddOrUpdateCallback(0x080CD261, 0x480);
    Unnamed_080cd104(0, 0);
    BattleFx_SelectLivingTargets((s32)work->effect);
    BattleFx_SpawnObjects(16, 0x17e, 1);
    gProjection.unk10 = 240;
    WaitFrames(1);
    BattleBackground_LoadFar(1, (s32)&Value_0000003b, 0);
    ctrl[4] = 1;
    gBgScroll.unk04 = 0;
    Unnamed_080cd104(0, 1);
    *(u16 *)0x04000000 = 0x7741;
    *(u16 *)0x04000020 = 0x80;
    *(u16 *)0x04000052 = 0x1010;
    *(u16 *)0x04000050 = 0x3f44;
    depth = 0xffc00000;
    zoom = 0x80000;
    near = 0xffff;
    far = 0;
    Resource_LoadAndDecompress((s32)&Value_000000bb, work, 1, 1);
    Resource_LoadAndDecompress((s32)&Value_00000067, work->sheet + 0x600, 0, 0);
    Resource_LoadAndDecompress((s32)&Value_000000ce, work->sheet + 0x95c, 1, 0);
    Resource_LoadAndDecompress((s32)&Value_00000073, ramp, 0, 0);

    /* Eight progressively darker clamped copies of the 770-byte ramp. */
    for (i1 = 0, pos = 0, lim = 64; i1 != 8; i1++, pos += 770, lim -= 7) {
        u8 *src = ramp;
        u8 *dst = work->sheet + pos + 0x2710;
        for (j = 0; j != 770; j++) {
            s32 v = *src++;
            if (v > lim) {
                v = lim;
            }
            if (v < 0) {
                v = 0;
            }
            *dst++ = v;
        }
    }

    ((WordCopy)0x03001388)((void *)0x05000000, Resource_GetTableEntry((s32)&Value_00000064), 128);

    for (i2 = 0; i2 != 128; i2++) {
        DUST[i2].x = (Random16() & 15) + 72;
        DUST[i2].velocity_x = Random16() & 0xffff;
        DUST[i2].velocity_y = Random16() & 0xffff;
        DUST[i2].velocity_y = Random16() & 0xffff;
    }

    amp = 255;
    lim = 0;
    for (i3 = 0; i3 != 128; i3++) {
        s32 angle = Random16() & 0xffff;
        SPARKS[i3].x = (Trig_Sin(angle) * amp) >> 2;
        SPARKS[i3].z = (Trig_Cos(angle) * amp) >> 2;
        SPARKS[i3].y = ((Random16() & 63) + 32) << 16;
        SPARKS[i3].velocity_x = lim;
        SPARKS[i3].velocity_y = lim;
        SPARKS[i3].variant = angle;
    }

    work->transfer_mode = 2;
    work->transfer_value = 50;
    *(u16 *)0x0400000c = 0x784;

    slots = (DrawRectangle *)gWorkSlot;
    for (frame = 0; frame != 160 && (frame <= 4 || !(gKeysRepeat & 3)); frame++) {
        if (frame == 143) {
            ((WordFill)0x03000168)(canvas, 0x4000, 0x2a2a2a2a);
            Audio_PlayCue(145);
        }
        if (frame == 80) {
            Audio_PlayCue(142);
        }

        if (frame < 0) {
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(iwram, (u8 *)iwram + 12);
            for (i4 = 0; i4 != 128; i4++) {
                EffectStep_AdvanceWithGravity3D(&SPARKS[i4], 60, -0x400);
                EffectPosition_ApplyBaseAndYOffset((s32)&SPARKS[i4], (struct EffectPosition *)scene);
                scene[0] >>= 1;
                scene[1] -= 120;
                if (scene[2] < 100) {
                    scene[2] = 100;
                }
                if (scene[2] > 900) {
                    scene[2] = 900;
                }
                z = scene[2] - 100;
                scene[2] = z;
                size = Math_Div(z, 100) + 1;
                if (near > z) {
                    near = z;
                }
                if (far < z) {
                    far = z;
                }
                draw(canvas, ramp + BattleFx_DotCells[size - 1], scene[0] - size / 2, scene[1] - size, size,
                    size * 2);
                if (frame == 64) {
                    SPARKS[i4].velocity_x += (Trig_Sin(SPARKS[i4].variant) * 255) >> 3;
                    SPARKS[i4].velocity_y += (Trig_Cos(SPARKS[i4].variant) * 255) >> 3;
                }
            }
        }

        if (frame == 72) {
            work->unknown_77b4 = 24;
            work->unknown_77b8 = 0;
        }

        anchor[2] = 0;
        anchor[1] = 0;
        for (i5 = 0; i5 != 64; i5++) {
            if (frame > i5 / 4 + 80 && DUST[i5].x > 0) {
                Render_ResetTransformState();
                SceneTransform_ApplyRoll(DUST[i5].velocity_y);
                SceneTransform_ApplyPitch(DUST[i5].velocity_x);
                SceneTransform_ApplyYaw(DUST[i5].velocity_y + (frame << 9));
                anchor[0] = DUST[i5].x;
                DUST[i5].x -= 2;
                if (DUST[i5].x < 0) {
                    DUST[i5].x = 0;
                }
                EffectPosition_ApplyBaseAndYOffset((s32)anchor, (struct EffectPosition *)scene);
                if (scene[2] < -60) {
                    scene[2] = -60;
                }
                if (scene[2] > 60) {
                    scene[2] = 60;
                }
                scene[2] += 60;
                size = Math_Div(scene[2], 20) + 2;
                scene[0] += 60;
                scene[1] += 80;
                draw(canvas, ramp + BattleFx_DotCells[size - 1], scene[0] - size / 2, scene[1] - size, size,
                    size * 2);
            }
        }

        pair = BattleFx9_UnitScale;
        place[3] = 0;
        place[1] = 0;
        for (i6 = 0; i6 != 16; i6++) {
            place[0] = ((i6 % 4) << 21) + (152 << 15);
            place[2] = ((i6 / 4) << 21) + depth;
            Object_ApplyProjectedPlacementFar(work->objects[i6], place, &pair, 0);
        }

        depth += zoom;
        if (frame <= 47) {
            zoom -= 0x4000;
        }
        if (frame > 32) {
            zoom = zoom * 60 / 64;
        }
        if (frame == 144) {
            zoom = -0x80000;
        }
        if (frame == 146) {
            zoom = 0x10000;
        }

        if (frame == 72) {
            for (i7 = 0; i7 != 16; i7++) {
                AnimationObjects_SelectAnimationFar(work->objects[i7], i7 + 16);
            }
        }
        if (frame == 76) {
            for (i8 = 0; i8 != 16; i8++) {
                AnimationObjects_SelectAnimationFar(work->objects[i8], i8 + 32);
            }
        }

        if (frame >= 116 && frame < 144) {
            u8 *cell;
            s32 n = (frame - 116) / 4;
            if (n > 6) {
                n = 6;
            }
            high = BattleFx9_BurstHeights[n];
            cell = work->sheet + BattleFx9_BurstCells[n] + 0x600;

            Unnamed_080ed408(47, 7, 7, 3, 2);
            (*(DrawRectangle *)0x03001f0c)(canvas, cell, 60 - high, 80 - high * 2, high, high * 2);
            Runtime_ReleaseHeapBlock(47);
            Unnamed_080ed408(47, 7, 7, 7, 2);
            (*(DrawRectangle *)0x03001f0c)(canvas, cell, 60, 80 - high * 2, high, high * 2);
            Runtime_ReleaseHeapBlock(47);
            Unnamed_080ed408(47, 7, 7, 11, 2);
            (*(DrawRectangle *)0x03001f0c)(canvas, cell, 60 - high, 80, high, high * 2);
            Runtime_ReleaseHeapBlock(47);
            Unnamed_080ed408(47, 7, 7, 15, 2);
            (*(DrawRectangle *)0x03001f0c)(canvas, cell, 60, 80, high, high * 2);
            Runtime_ReleaseHeapBlock(47);

            for (i9 = 0; i9 != 6; i9++) {
                s32 k = i9 & 3;
                s32 angle = Random16() & 0xffff;
                x = ((Trig_Sin(angle) * 16) >> 16) - BattleFx9_ShardWidths[k] / 2;
                y = ((Trig_Cos(angle) * 16) >> 16) - BattleFx9_ShardHeights[k] / 2;
                draw(canvas, work->sheet + BattleFx9_ShardCells[k], x + 60, y + 80, BattleFx9_ShardWidths[k],
                    BattleFx9_ShardHeights[k]);
            }
        }

        if (frame > 143) {
            s32 top = (frame << 4) - 2272;
            s32 n;
            Unnamed_080ed408(47, 7, 7, 3, 3);
            (slots[47])(canvas, work, 36, top, 24, 64);
            Runtime_ReleaseHeapBlock(47);
            Unnamed_080ed408(47, 7, 7, 7, 3);
            (slots[47])(canvas, work, 60, top, 24, 64);
            Runtime_ReleaseHeapBlock(47);

            n = (frame - 144) / 2;
            work->transfer_value = 75;
            if (n <= 6) {
                u8 *cell = work->sheet + n * 770 + BattleFx_DotCells[7] + 0x2710;
                high = 8;
                Unnamed_080ed408(47, 7, 7, 3, 2);
                blit = slots[47];
                for (i10 = 0; i10 != 128; i10++) {
                    s32 angle = i10 << 9;
                    x = (((frame * 10 - 1424) * Trig_Sin(angle)) >> 16) - high / 2;
                    y = ((frame * 10 - 1424) * Trig_Cos(angle)) >> 17;
                    blit(canvas, cell, x + 60, y + 72, 8, 16);
                }
                Runtime_ReleaseHeapBlock(47);
            }
        }

        work->transfer_pending = 1;
        WaitFrames(1);
    }

    ((WordFill)0x03000168)(canvas, 0x4000, 0);

    for (i11 = 0; i11 != 16; i11++) {
        ResourceObject_ReleaseFar(work->objects[i11]);
    }

    for (i12 = 0; i12 != 16; i12++) {
        u8 *obj = ResourceObject_CreateFar(0x186);
        work->objects[i12] = obj;
        if (obj != 0) {
            obj[38] = 0;
            AnimationObjects_SelectAnimationFar(obj, Math_Mod(i12, 3));
            ((u8 *)work->objects[i12])[9] &= ~12;
        }
    }

    Runtime_ReleaseHeapBlock(46);
    Unnamed_080ed408(46, 7, 7, 3, 2);
    draw = ((DrawRectangle *)gWorkSlot)[46];
    Resource_LoadAndDecompress((s32)&Value_00000064, work->sheet + 0x4000, 1, 1);

    {
        u8 *c = ramp;
        for (i13 = 0; i13 != 770; i13++, c++) {
            if (*c > 32) {
                *c += 224;
            } else if (*c != 0) {
                *c = 1;
            }
        }
    }

    work->unknown_77b4 = 0;
    work->unknown_77b8 = 0;
    BattleBackground_LoadFar(1, (s32)&Value_0000003e, 0);
    *(u16 *)0x04000020 = 0x100;
    *(u32 *)0x04000028 = -0x3c00;
    *(u16 *)0x0400000c = 0x784;

    fill = 0;
    Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);

    cx = 60;
    cy = 44;
    shift = 0;
    ring = 0;
    squash = 6;
    sx = 8;
    rings = 2;
    work->transfer_mode = 2;
    work->transfer_value = 75;

    fill = shift;
    Dma_Set(&fill, work, 0x85000e10, (volatile u32 *)0x040000d4);

    for (frame = 0; frame != 320; frame++) {
        if (frame == 66) {
            Audio_PlayCue(145);
            Audio_PlayCue(141);
        }
        if (frame == 155) {
            Audio_PlayCue(162);
        }
        if (frame == 217) {
            Audio_PlayCue(156);
        }
        if (frame == 280) {
            Audio_PlayCue(157);
        }
        if (frame == 300) {
            BattleEventRuntime_BeginPhaseFar(145);
        }

        if (gKeysRepeat & 3) {
            if (frame >= 5 && frame < 150) {
                frame = 150;
                QUEUE_IO_WRITE(0x04000020, 0x80, 0x20000);
                QUEUE_IO_WRITE(0x04000028, 0, 0x30000);
                Resource_LoadAndDecompress((s32)&Value_00000070, work->sheet + 0x4000, 1, 0);
            } else if (frame >= 155 && frame < 214) {
                frame = 214;
            } else if (frame >= 219 && frame < 280) {
                frame = 280;
            }
        }

        if (frame == 64) {
            QUEUE_IO_WRITE(0x04000020, 0x80, 0x20000);
            QUEUE_IO_WRITE(0x04000028, 0, 0x30000);
            fill = 0;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
            Resource_LoadAndDecompress((s32)&Value_00000070, work->sheet + 0x4000, 1, 0);
            Resource_LoadAndDecompress((s32)&Value_00000065, work->sheet + 0x6000, 0, 0);
        }

        if (frame == 66) {
            u16 *pal = (u16 *)0x050000c0;
            for (i14 = 0; i14 != 128; i14++, pal++) {
                *pal ^= 0x7fff;
            }
        }

        if (frame == 69) {
            fill = 0x3f3f3f3f;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
        }

        if (frame == 70) {
            BattlePresentation_ConfigurePaletteFadeFar(1, (s32)&Value_0000003e, 7);
        }

        if (frame == 150) {
            cx = 112;
            cy = 32;
            ring = 0;
            fill = 0;
            rings = 4;
            squash = 8;
            Dma_Set(&fill, work, 0x85000e10, (volatile u32 *)0x040000d4);
            fill = 0x3f3f3f3f;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
            BattleBackground_LoadFar(1, (s32)&Value_00000036, 0);
        }

        if (frame == 214) {
            fill = 0;
            Dma_Set(&fill, work, 0x85000e10, (volatile u32 *)0x040000d4);
            fill = 0x3f3f3f3f;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
            BattleBackground_LoadFar(1, (s32)&Value_0000003a, 0);
            for (i15 = 0; i15 != 16; i15++) {
                p = &work->rain[i15];
                p->x = ((Random16() & 63) + 32) << 16;
                p->y = ((Random16() & 31) + 96) << 16;
                p->velocity_y = ((Random16() & 15) + 16) << 15;
                p->z = 0x8000 - (p->x >> 7);
                p->variant = 1;
            }
        }

        if (frame == 280) {
            BattleEffect_SetupBlendedDisplay();
            fill = 0;
            Dma_Set(&fill, work, 0x85000e10, (volatile u32 *)0x040000d4);
            fill = 0x01010101;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
            *(u16 *)0x04000052 = 0x1010;
            ctrl[4] = 0;
            for (i16 = 0; i16 != 16; i16++) {
                p = &work->rain[i16];
                p->x = ((Random16() & 63) + 256) << 16;
                p->y = ((Random16() & 31) + 96) << 16;
                p->velocity_y = ((Random16() & 15) + 16) << 15;
                p->z = 0;
            }
            shift = 272;
            work->transfer_mode = 0;
            work->transfer_value = 0;
        }

        if (frame > 279) {
            u16 *pal = (u16 *)0x05000002;
            for (i17 = 0; i17 != 63; i17++, pal++) {
                u16 c = *pal;
                s32 b = ((c >> 10) & 31) + 1;
                s32 g = ((c >> 5) & 31) + 1;
                s32 r = (c & 31) + 1;
                if (b > 31) {
                    b = 31;
                }
                if (g > 31) {
                    g = 31;
                }
                if (r > 31) {
                    r = 31;
                }
                *pal = (b << 10) | (g << 5) | r;
            }
        }

        if (frame == 182) {
            fill = 0x3f3f3f3f;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
        }

        if (frame <= 63) {
            u8 *sheet = work->sheet + 0x4000;
            s32 top = frame - 28;
            if ((frame & 7) > 3) {
                sheet = work->sheet + 0x4240;
            }
            Unnamed_080ed408(47, 7, 7, 3, 2);
            (*(DrawRectangle *)0x03001f0c)(canvas, sheet, 36, top - 24 + 24, 24, 24);
            Runtime_ReleaseHeapBlock(47);
            Unnamed_080ed408(47, 7, 7, 7, 2);
            (*(DrawRectangle *)0x03001f0c)(canvas, sheet, 59, top, 24, 24);
            Runtime_ReleaseHeapBlock(47);
            Unnamed_080ed408(47, 7, 7, 11, 2);
            (*(DrawRectangle *)0x03001f0c)(canvas, sheet, 36, top + 23, 24, 24);
            Runtime_ReleaseHeapBlock(47);
            Unnamed_080ed408(47, 7, 7, 15, 2);
            (*(DrawRectangle *)0x03001f0c)(canvas, sheet, 59, top + 23, 24, 24);
            Runtime_ReleaseHeapBlock(47);
        }

        if (frame >= 64 && frame < 66) {
            draw(canvas, work->sheet + 0x6000, 52, 51, 16, 17);
        }
        if (frame >= 66 && frame < 68) {
            draw(canvas, work->sheet + 0x6110, 48, 40, 24, 41);
        }
        if (frame >= 68 && frame < 76) {
            u8 *petal = work->sheet + 0x64e8;
            s32 d = 76 - frame;
            draw(canvas, petal, 49 - d / 2, 38 - d, 22, 44);
            draw(canvas, petal, 49 - d / 2, d + 38, 22, 44);
            draw(canvas, petal, d / 2 + 49, 38 - d, 22, 44);
            draw(canvas, petal, d / 2 + 49, d + 38, 22, 44);
        }
        if (frame >= 78 && frame < 80) {
            draw(canvas, work->sheet + 0x6000, 52, 51, 16, 17);
        }
        if (frame >= 80 && frame < 82) {
            draw(canvas, work->sheet + 0x6110, 48, 40, 24, 41);
        }
        if (frame >= 82 && frame < 86) {
            u8 *petal = work->sheet + 0x64e8;
            s32 d = frame * 2 - 164;
            draw(canvas, petal, 49 - d / 4, 38 - d, 22, 44);
            draw(canvas, petal, 49 - d / 4, frame * 2 - 126, 22, 44);
            draw(canvas, petal, d / 4 + 49, 38 - d, 22, 44);
            draw(canvas, petal, d / 4 + 49, frame * 2 - 126, 22, 44);
        }

        if (frame >= 72 && frame < 76) {
            s32 r2 = (frame << 4) - 1136;
            for (i18 = 0; i18 != 256; i18++) {
                s32 angle = i18 << 8;
                x = ((r2 * Trig_Sin(angle)) >> 16) - 5;
                y = (r2 * Trig_Cos(angle)) >> 17;
                draw(canvas, ramp + BattleFx_DotCells[9], x + 60, y + 50, 10, 20);
            }
        }

        if (frame > 85 && frame <= 213 && rings != 0) {
            for (i19 = 0; i19 != rings; i19++, ring++) {
                s32 radius = ring;
                s32 slope = 0;
                s32 err = ring;
                while (radius >= slope) {
                    s32 half;
                    s32 left;
                    s32 right;
                    s32 top;
                    s32 bottom;

                    half = radius / 2;
                    right = cx + (half * sx) / 8;
                    left = cx - (half * sx) / 8;
                    bottom = cy + (slope * squash) / 8;
                    top = cy - (slope * sx) / 8;
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
                    work->sheet[bottom * 120 + right] = 20;
                    work->sheet[top * 120 + right] = 20;
                    work->sheet[bottom * 120 + left] = 20;
                    work->sheet[top * 120 + left] = 20;
                    right = cx + half + 1;
                    left = cx - half + 1;
                    if (left < 0) {
                        left = 0;
                    }
                    if (right > 119) {
                        right = 119;
                    }
                    work->sheet[bottom * 120 + right] = 20;
                    work->sheet[top * 120 + right] = 20;
                    work->sheet[bottom * 120 + left] = 20;
                    work->sheet[top * 120 + left] = 20;

                    half = slope / 2;
                    right = cx + (half * sx) / 8;
                    left = cx - (half * sx) / 8;
                    bottom = cy + (radius * squash) / 8;
                    top = cy - (radius * sx) / 8;
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
                    work->sheet[bottom * 120 + right] = 20;
                    work->sheet[top * 120 + right] = 20;
                    work->sheet[bottom * 120 + left] = 20;
                    work->sheet[top * 120 + left] = 20;
                    right = cx + half + 1;
                    left = cx - half + 1;
                    if (left < 0) {
                        left = 0;
                    }
                    if (right > 119) {
                        right = 119;
                    }
                    work->sheet[bottom * 120 + right] = 20;
                    work->sheet[top * 120 + right] = 20;
                    work->sheet[bottom * 120 + left] = 20;
                    work->sheet[top * 120 + left] = 20;

                    err = err - slope * 2 - 1;
                    if (err < 0) {
                        err = err + radius * 2 - 2;
                        radius--;
                    }
                    slope++;
                }
            }
        }

        if (frame >= 86 && frame < 150) {
            s32 angle = -0x4000;
            for (i20 = 0; i20 != 9; i20++, angle += 0x1000) {
                x = ((frame - 86) * Trig_Sin(angle)) >> 16;
                y = ((frame - 70) * Trig_Cos(angle)) >> 16;
                draw(canvas, work->sheet + ((frame / 2 + i20) % 4) * 1728 + 0x4000, x + 44, y + 17, 32, 54);
            }
        }

        Unnamed_080ed408(47, 7, 7, 3, 3);
        draw2 = ((DrawRectangle *)gWorkSlot)[47];
        if (frame > 85) {
            draw2(canvas, work, 0, 0, 120, 120);
        }

        if (frame > 279) {
            shift -= 8;
        }

        if (frame > 319) {
            if (work->effect->count != 0) {
                for (i21 = 0; i21 != work->effect->count; i21++) {
                    ObjectGroup_UpdateMembers(work->effect->actors[i21], 14, 5, -1, 0);
                }
            }
        }

        if (frame == 238) {
            fill = 0x3f3f3f3f;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
        }

        if (frame >= 214 && frame < 280) {
            gBgScroll.unk04 += 8;
        }

        if (frame >= 246 && frame < 280) {
            shift += 8;
            work->transfer_mode = 0;
            work->transfer_value = 75;
        }

        if (frame > 213) {
            place2[3] = 0;
            place2[1] = 0;
            for (i22 = 0; i22 != 16; i22++) {
                p = &work->rain[i22];
                if (p->variant != 0) {
                    scale.x = p->z + (p->y >> 7);
                    if (scale.x < 0x800) {
                        scale.x = 0x800;
                    }
                    scale.y = scale.x;
                    place2[1] = 0xff0000;
                    place2[0] = p->x;
                    place2[2] = p->y + 0xff0000;
                    Object_ApplyProjectedPlacementFar(work->objects[i22], place2, &scale, 0);
                    p->y -= p->velocity_y;
                    if (p->y < 0x100000) {
                        if (frame > 279) {
                            p->x = ((Random16() & 63) + shift - 32) << 16;
                        } else {
                            p->x = ((Random16() & 63) + shift + 32) << 16;
                        }
                        if (frame > 301) {
                            p->variant = 0;
                        } else if (frame > 245) {
                            p->y = 0x600000;
                        } else {
                            p->y = 0xc00000 - p->x;
                        }
                    }
                }
            }

            if (frame <= 279) {
                for (i23 = 0; i23 != 8; i23++) {
                    x = (i23 & 1) * 20 - (Random16() & 15) + (i23 / 2) * 4 + shift / 2 - 16;
                    y = (i23 / 2) * 32 - (Random16() & 15);
                    draw2(canvas, work->sheet + ((frame / 2 + i23) % 4) * 1728 + 0x4000, x, y, 32, 54);
                }
            } else {
                for (i24 = 0; i24 != 8; i24++) {
                    x = (i24 & 1) * 20 + (Random16() & 15) - (i24 / 2) * 4 + shift / 2 - 16;
                    y = (i24 / 2) * 32 - (Random16() & 15);
                    draw2(canvas, work->sheet + ((frame / 2 + i24) % 4) * 1728 + 0x4000, x, y, 32, 54);
                }
            }
        }

        Runtime_ReleaseHeapBlock(47);

        if (frame > 63) {
            gBgScroll.unk06 = (Random16() & 3) + 30;
        }

        work->transfer_pending = 1;
        WaitFrames(1);
    }

    for (i25 = 0; i25 != 16; i25++) {
        ResourceObject_ReleaseFar(work->objects[i25]);
    }

    Scheduler_RemoveCallback(0x080CD261);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
