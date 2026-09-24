/* Draft, not exact (2026-09-24): candidate=3452 reference=3452
   differing_halfwords=241, binary similarity 96%. Everything through the
   phase-one draws matches except two scheduling swaps (the 192 block loads
   0x4e20 before the size stores; the strip loop loads dst after x). Phase
   two differs in reload temporaries (r0/r1 against r6/r7), the burst loop
   builds 112 << 16 from a register holding 112 and loads its masks into the
   result register (0x3ff & r, 0x7fff & a), which suggests they were
   variables with constant equivalents; declaring them moves the frame. What
   closed the rest: loop-top locals that loop.c hoists (the cells pointer and
   radius in the >221 loop, the constant 3 in the particle loop), centre
   temporaries for the flare draw, and one pointer variable per spark loop. */
#include "TYPES.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "FIXED_MATH.H"

void WaitFrames(s32);
u32 Random16(void);
s32 Trig_Sin(s32);
s32 Trig_Cos(s32);
void Audio_PlayCue(s32);
void BattleFx_BeginCanvasLayer(s32);
void BattlePres_ConfigureEffectDisplay(void);
void Runtime_SetIrqHandler(s32, s32, s32);
void Unnamed_080cd104(s32, s32);
void BattleFx_SelectLivingTargets(s32);
void BattleBackground_LoadFar(s32, s32, s32);
void BattleFx_SpawnObjects(s32, s32, s32);
void Object_ApplyProjectedPlacementFar(void *, void *, void *, s32);
void ResourceObject_ReleaseFar(void *);
void BattleEffect_SetupBlendedDisplay(void);
void BattleEventRuntime_BeginPhaseFar(s32);
void BattleMotion_ApplyVariantMotionFar(s32, s32);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *, s32, s32);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
void ObjectGroup_TickMemberTimers(void);
void Camera_ApplyShake(s32, s32);
void Func_080cde90(s32, s32, s32, s32, s32);
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

extern u8 Value_0000003d;
extern u8 Value_0000006e;
extern u8 Value_0000006f;
extern u8 Value_00000073;
extern u8 Value_00000076;
extern u8 Value_00000121;

typedef struct Scale {
    s32 x;
    s32 y;
} Scale;

extern const Scale BattleFx6_UnitScale;
extern const u8 BattleFx6_ObjectX[];
extern const u8 BattleFx6_ObjectY[];
extern const s32 BattleFx6_Gravity[];
extern const u16 BattleFx6_FlareCells[];
extern const u16 ParticleStreams_CellOffsets[];

/* The effect's work block (heap slot 39). */
struct Mode6Work {
    u8 sheet[0x7080];
    struct EffectStep sparks[64];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unknown_7788[0x20];
    s32 shake;
    u8 unknown_77ac[0x8];
    s32 unknown_77b4;
    s32 unknown_77b8;
    u8 unknown_77bc[0x1c];
    void *objects[8];
    u8 unknown_77f8[0x2c];
    s32 transfer_pending;
    struct BattleEffectArgument *effect;
};

#define PARTICLES ((struct EffectStep *)0x02010000)
#define HI(v) (((s16 *)&(v))[1])

void BattleFx_InitializeMode6(struct BattleEffectArgument *efx)
{
    s32 pos[4];
    s32 seat[3];
    void *dst;
    u8 *aux;
    s32 step;
    s32 px;
    s32 py;
    s32 vx;
    s32 vy;
    s32 scroll;
    s32 *spot;
    s32 j;
    s32 burst;
    s32 bx;
    u32 *cache;
    struct Mode6Work *work;
    s32 frame;
    s32 t;
    s32 i;
    s32 n;
    s32 s;
    struct EffectStep *p;
    struct EffectStep *q1;
    struct EffectStep *q2;
    struct EffectStep *q3;
    struct EffectStep *q4;
    struct EffectStep *q;
    DrawRectangle blit[2];
    Scale scale;

    cache = (u32 *)(gWorkSlot + 40 * 4);
    dst = (void *)cache[40 - 40];
    work = (struct Mode6Work *)cache[39 - 40];
    aux = (u8 *)cache[41 - 40];
    work->effect = efx;
    BattleFx_BeginCanvasLayer(0);
    BattlePres_ConfigureEffectDisplay();
    *(u16 *)0x05000000 = 0;
    *(u16 *)0x05000002 = 0;
    Scheduler_AddOrUpdateCallback(0x080EC0E1, 0x480);
    Runtime_SetIrqHandler(2, 0, 0x080EC0F1);
    work->transfer_mode = 0;
    Scheduler_AddOrUpdateCallback(0x080CD261, 0x480);
    *(u16 *)0x04000048 = 0x2137;
    Unnamed_080cd104(1, 0);
    *(u16 *)0x04000040 = 0xf0f0;
    BattleFx_SelectLivingTargets((s32)work->effect);
    BattleFx_SpawnObjects(8, 0x17a, 1);
    gProjection.unk10 = 240;
    WaitFrames(1);
    BattleBackground_LoadFar(1, (s32)&Value_0000003d, 0);
    Resource_LoadAndDecompress((s32)&Value_00000073, aux, 0, 0);
    Resource_LoadAndDecompress((s32)&Value_0000006e, work, 1, 1);
    Resource_LoadAndDecompress((s32)&Value_00000076, work->sheet + 0x4e20, 0, 0);
    *(u16 *)0x04000000 = 0x7741;
    *(u16 *)0x04000020 = 0x80;
    *(u16 *)0x04000052 = 0x1010;
    *(u16 *)0x04000050 = 0x3f44;
    step = 1;
    px = 140 << 17;
    py = 200 << 16;
    vx = -0x40000;
    vy = -0x80000;
    scroll = gBgScroll.unk04;
    spot = (s32 *)cache[44 - 40];
    work->transfer_mode = 2;
    work->transfer_value = 75;
    spot[4] = 1;
    BattleEffect_LoadWork(46, 7, 7, 3, 3);
    blit[0] = (DrawRectangle)cache[46 - 40];
    BattleEffect_LoadWork(47, 7, 7, 3, 2);
    blit[1] = (DrawRectangle)cache[47 - 40];
    *(u16 *)0x0400000c = 0x784;

    for (i = 0; i != 1024; i++) {
        PARTICLES[i].variant = -1;
    }
    for (i = 0, q1 = work->sparks; i != 32; i++, q1++) {
        s32 a = (Random16() & 0x3fff) + 0x8000;
        s32 r = (Random16() & 127) + 255;
        q1->x = (Trig_Sin(a) * r) >> 2;
        q1->y = (Trig_Cos(a) * r) >> 2;
        q1->velocity_x = -q1->x / 32;
        q1->velocity_y = -q1->y / 32;
        q1->variant = 0;
    }
    for (i = 0; i != 712; i++) {
        s32 r = Random16() & 255;
        s32 a = Random16() & 0xffff;
        PARTICLES[i].x = ((Trig_Sin(a) * r) >> 3) + 0x800000;
        PARTICLES[i].y = ((Trig_Cos(a) * r) >> 4) + (192 << 15);
        PARTICLES[i].velocity_x = (-(Random16() & 0x1fff) - 0x800) << 5;
        PARTICLES[i].velocity_y = (-(Random16() & 0xfff) - 0x1800) << 5;
        PARTICLES[i].variant = 0;
    }

    frame = 0;
    do {
        if (frame == 24) {
            Audio_PlayCue(162);
        }
        if (frame == 76) {
            Audio_PlayCue(164);
        }
        if (frame == 154) {
            Audio_PlayCue(142);
        }
        if (frame == 222) {
            Audio_PlayCue(145);
        }
        if (frame >= 0 && frame < 16) {
            u16 *phase = (u16 *)0x02010000;
            if (frame == 1) {
                u8 *noise = (u8 *)0x02010002;
                s32 k;
                for (k = 0; k != 128; k++) {
                    noise[k] = Random16() & 63;
                }
                *phase = 0;
                Scheduler_AddOrUpdateCallback(0x080D66CD, 0x480);
            }
            *phase += step;
            step += 3;
            if (frame == 15) {
                Scheduler_RemoveCallback(0x080D66CD);
            }
        }

        if (frame > 21) {
            scale = BattleFx6_UnitScale;
            if (frame <= 83) {
                if (frame > 48) {
                    vx = vx * 60 / 64;
                }
                vy = vy * 60 / 64;
                if (py > 0x840000) {
                    vy -= 0x2000;
                } else {
                    vy += 0x2000;
                }
                px += vx;
                py += vy;
            } else if (frame <= 170) {
                if (frame <= 107) {
                    vx += 0x2000;
                }
                vx = vx * 60 / 64;
                px += vx;
                if (frame <= 117) {
                    if (py > 0x8a0000) {
                        vy -= 0x2000;
                    } else {
                        vy += 0x2000;
                    }
                    vy = vy * 56 / 64;
                    py += vy;
                }
            }
            pos[3] = 0;
            pos[1] = 0;
            for (i = 0; i != 8; i++) {
                pos[0] = (((BattleFx6_ObjectX[i] * 3) >> 1) << 16) + px - 0x200000;
                pos[2] = (((BattleFx6_ObjectY[i] * 3) >> 1) << 16) + py - 0x180000;
                Object_ApplyProjectedPlacementFar(work->objects[i], pos, &scale, 0);
            }
        }

        if (frame == 76) {
            for (i = 0; i != 1024; i++) {
                if (PARTICLES[i].variant == -1) {
                    s32 r = Random16() & 255;
                    s32 a = Random16() & 0xffff;
                    PARTICLES[i].x = ((Trig_Sin(a) * r) >> 3) + 0x400000;
                    PARTICLES[i].y = ((Trig_Cos(a) * r) >> 4) + (192 << 15);
                    PARTICLES[i].velocity_x = (32 - (Random16() & 63)) << 12;
                    PARTICLES[i].velocity_y = (-(Random16() & 31) - 16) << 13;
                    PARTICLES[i].variant = 0;
                }
            }
        }
        if (frame == 152) {
            Runtime_SetIrqHandler(2, 96, 0x080EC0F1);
            work->unknown_77b4 = 24;
            work->unknown_77b8 = 0;
        }

        t = frame - 152;
        if (t >= 0 && t < 88) {
            for (i = 0, q2 = work->sparks; i != 32; i++, q2++) {
                if (frame >= i / 4 + 152 && frame < i / 4 + 152 + 32) {
                    s32 fx = HI(q2->x) + 112;
                    s32 fy = HI(q2->y) + 62;
                    s32 r = (i & 3) + 5;
                    s = r * 2;
                    blit[0](dst, work->sheet + BattleFx6_FlareCells[r - 1] + 0x4e20, fx - r, fy - r, s, s);
                    q2->x += q2->velocity_x;
                    q2->y += q2->velocity_y;
                }
            }
        }

        if (frame == 222) {
            for (i = 0; i != 64; i++) {
                q3 = &work->sparks[i];
                q3->x = (Random16() & 15) - 8;
                q3->y = (Random16() & 15) - 8;
                if (q3->x < 0) {
                    q3->x -= 4;
                } else {
                    q3->x += 4;
                }
                if (q3->y < 0) {
                    q3->y -= 4;
                } else {
                    q3->y += 4;
                }
                q3->x += 100;
                q3->y += 52;
                q3->velocity_x = -6;
                q3->z = q3->x;
                q3->velocity_y = q3->y;
                q3->variant = 0;
            }
        }
        if (frame > 221) {
            for (i = 0; i != 64; i++) {
                const u16 *cells = BattleFx6_FlareCells;
                s32 r = 1;
                if (frame >= i / 2 + 222) {
                    blit[0](dst, work->sheet + cells[r - 1] + 0x4e20,
                        work->sparks[i].x - r, work->sparks[i].y - r, r * 2, r * 2);
                    work->sparks[i].x += work->sparks[i].velocity_x;
                    work->sparks[i].y += work->sparks[i].velocity_y;
                    if (work->sparks[i].y < 0) {
                        work->sparks[i].y = work->sparks[i].velocity_y;
                        work->sparks[i].x = work->sparks[i].z;
                    }
                }
            }
        }

        if (frame > 175) {
            n = 5;
            if (frame > 221) {
                n = 60;
                if (frame != 223 && frame != 224) {
                    n = 40;
                }
            }
            if (n > 60) {
                n = 60;
            }
            for (i = 0; i != n; i++) {
                s32 w;
                s32 d = (i + 1) / 2;
                w = n * 2 - d * 4;
                if (w < 12) {
                    w = 12;
                }
                if (w > 63) {
                    w = 63;
                }
                if (i & 1) {
                    d = -d;
                }
                Func_080cde90(d + 48, 0, d / 2 + 111, 63 - (d + 1) / 2, w);
            }
        }

        if (t >= 0 && t < 40) {
            s32 z;
            z = t / 2 + 1;
            if (z > 4) {
                z = 4;
            }
            blit[1](dst, work->sheet + BattleFx6_FlareCells[z - 1] + 0x4e20, 108 - z, 60 - z, z * 2, z * 2);
        }
        if (frame >= 192 && frame < 200) {
            s32 z;
            z = (frame - 192) / 2 + 3;
            if (z > 6) {
                z = 6;
            }
            blit[1](dst, work->sheet + BattleFx6_FlareCells[z - 1] + 0x4e20, 108 - z, 60 - z, z * 2, z * 2);
        }
        if (frame > 199) {
            s32 z;
            s32 u = frame - 200;
            z = u / 4 + 5;
            if (z > 8) {
                z = 8;
            }
            blit[1](dst, work->sheet + BattleFx6_FlareCells[z - 1] + 0x4e20, 108 - z, 60 - z, z * 2, z * 2);
            if (frame <= 213) {
                z = u / 4 + 1;
                if (z > 4) {
                    z = 4;
                }
                blit[1](dst, work->sheet + BattleFx6_FlareCells[z - 1] + 0x4e20, 100 - z, 52 - z, z * 2, z * 2);
            }
        }
        if (frame >= 214 && frame < 222) {
            s32 z;
            z = (frame - 214) / 4;
            if (z > 2) {
                z = 2;
            }
            blit[1](dst, work->sheet + z * 9 * 256 + 0x400, 63, 18, 48, 48);
        }
        if (frame > 221) {
            s32 z;
            z = (frame - 222) / 2 % 4 + 3;
            blit[0](dst, work->sheet + z * 9 * 256 + 0x400, 72, 30, 48, 48);
            if (z == 5) {
                blit[0](dst, work->sheet + 0x1600, 66, 22, 48, 48);
            }
        }

        if (frame > 27) {
            DrawRectangle draw = blit[1];
            for (i = 0; i != 1024; i++) {
                s32 m = 3;
                if (PARTICLES[i].variant >= 0) {
                    s32 k = Math_Mod(i, 3) + 2;
                    draw(dst, aux + ParticleStreams_CellOffsets[k - 1],
                        HI(PARTICLES[i].x) - k / 2, HI(PARTICLES[i].y) - k, k, k * 2);
                    EffectStep_AdvanceWithGravity2D(&PARTICLES[i], 62, BattleFx6_Gravity[i & m]);
                    PARTICLES[i].variant++;
                    if (PARTICLES[i].velocity_y > 0 && HI(PARTICLES[i].y) > 104) {
                        PARTICLES[i].variant = -1;
                    }
                }
            }
        }
        for (i = 0; i != 5; i++) {
            blit[1](dst, work->sheet, i * 32 + ((frame / 4) & 31) - 32, 88, 32, 32);
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    } while (++frame != 244 && (frame <= 16 || !(gKeysRepeat & 3)));

    gBgScroll.unk04 = scroll;
    spot[4] = 0;
    Runtime_SetIrqHandler(2, 0, 0);
    BattleEffect_SetupBlendedDisplay();
    Scheduler_RemoveCallback(0x080EC0E1);
    *(u16 *)0x04000040 = 0xf0;
    work->transfer_mode = 2;
    work->transfer_value = 75;
    for (i = 0; i != 1024; i++) {
        PARTICLES[i].variant = 0;
    }
    for (i = 0; i != 16; i++) {
        work->sparks[i].x = (Random16() & 31) + 32;
        work->sparks[i].y = 0;
        work->sparks[i].variant = 0;
    }
    for (i = 0; i != work->effect->count; i++) {
        EffectPosition_ApplyStepAndYOffset(work->effect->actors[i], (struct EffectPosition *)seat);
        work->sparks[i].x = seat[0] / 2;
    }
    Resource_LoadAndDecompress((s32)&Value_0000006f, work, 1, 1);
    Audio_PlayCue((s32)&Value_00000121);
    *(u16 *)0x04000020 = 0x80;
    *(u16 *)0x04000052 = 0x1010;

    frame = 0;
    do {
        if (frame == 96) {
            BattleEventRuntime_BeginPhaseFar(134);
        }
        for (i = 0, q4 = work->sparks; i != 5; i++, q4++) {
            if (frame == i * 16 + 7) {
                Audio_PlayCue(154);
            }
            if (frame >= i * 16 && q4->variant <= 31) {
                s32 wobble = Math_Mod(frame * 16, 104);
                blit[1](dst, work->sheet, q4->x - 8, q4->y + wobble - 216, 17, 104);
                blit[1](dst, work->sheet, q4->x - 8, q4->y + wobble - 112, 17, 104 - wobble);
                blit[1](dst, work->sheet + 0x6e8, q4->x - 17, q4->y - 65, 34, 65);
                if (q4->y <= 111) {
                    q4->y += 16;
                } else {
                    q4->variant++;
                }
                if (q4->y > 111) {
                    burst = 0;
                    bx = q4->x;
                    if (q4->variant <= 7) {
                        work->shake = 4;
                    }
                    for (j = 0; j != 1024; j++) {
                        if (PARTICLES[j].variant == 0) {
                            s32 r = Random16() & 0x3ff;
                            s32 a = (Random16() & 0x7fff) - 0x4000;
                            PARTICLES[j].x = bx << 16;
                            PARTICLES[j].y = 112 << 16;
                            r += 32;
                            PARTICLES[j].velocity_x = (Trig_Sin(a) * r) >> 7;
                            PARTICLES[j].velocity_y = -(Trig_Cos(a) * r * 2) >> 7;
                            PARTICLES[j].variant = (Random16() & 7) + 32;
                            if (++burst == 4) {
                                break;
                            }
                        }
                    }
                }
            }
        }
        for (i = 0; i != work->effect->count; i++) {
            if (frame == i * 16 + 8) {
                ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 8);
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 2);
            }
        }
        for (i = 0; i != 1024; i++) {
            if (PARTICLES[i].variant > 0) {
                s32 x;
                s32 y;
                PARTICLES[i].variant--;
                EffectStep_AdvanceWithGravity2D(&PARTICLES[i], 60, 0x4000);
                y = PARTICLES[i].y >> 16;
                if (y > 120) {
                    PARTICLES[i].velocity_y = -PARTICLES[i].velocity_y / 2;
                } else if (PARTICLES[i].x >= 0 && (x = PARTICLES[i].x >> 16) <= 126 && PARTICLES[i].y >= 0) {
                    s32 k = PARTICLES[i].variant / 8 + 3;
                    blit[i & 1](dst, aux + ParticleStreams_CellOffsets[k - 1], x - k / 2, y - k, k, k * 2);
                }
            }
        }
        Camera_ApplyShake(4, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    } while (++frame != 144);

    for (i = 0; i != 8; i++) {
        ResourceObject_ReleaseFar(work->objects[i]);
    }
    Scheduler_RemoveCallback(0x080CD261);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
