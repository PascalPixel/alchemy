/* Draft, not exact (2026-09-24): candidate=3160 reference=3156 differing_halfwords=630,
   binary similarity 89%. The stack frame (332 bytes, every spill slot), the
   two frame loops, the skip test with its count reset, the noise loop end
   pointer, the 3x3 placement loop and most particle loops (indexed, so each
   gets its own induction pointer) match. Remaining: the rock and fall loops
   keep the Math_Mod result in r0 in the reference, the flag clear uses -13
   rather than 0xf3, and the phase-two spark loops pick r5/r6 the other way. */
#include "TYPES.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "FIXED_MATH.H"

void WaitFrames(s32);
s32 Math_ModU(s32, s32);
u32 Random16(void);
void Audio_PlayCue(s32);
void BattleFx_BeginCanvasLayer(s32);
void BattlePres_ConfigureEffectDisplay(void);
void Unnamed_080cd104(s32, s32);
void BattleFx_SelectLivingTargets(s32);
void BattleBackground_LoadFar(s32, s32, s32);
void BattleFx_SpawnObjects(s32, s32, s32);
void Render_ResetTransformState(void);
void AnimationObjects_SelectAnimationFar(void *, s32);
void Object_ApplyProjectedPlacementFar(void *, void *, void *, s32);
void ResourceObject_ReleaseFar(void *);
void BattleEffect_SetupBlendedDisplay(void);
void BattleEventRuntime_BeginPhaseFar(s32);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *, s32, s32);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
void Camera_ApplyShake(s32, s32);
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
extern u8 Value_000000b9;
extern u8 Value_000000ba;
extern u8 Value_00000121;

typedef struct Scale {
    s32 x;
    s32 y;
} Scale;

extern const Scale BattleFx10_UnitScale;
extern const u8 BattleFx10_Points[][2];
extern const s8 BattleFx10_ShakeOffsets[];
extern const u16 BattleFx10_RockCells[];
extern const u8 BattleFx10_RockWidths[];
extern const u8 BattleFx10_RockHeights[];
extern const u8 BattleFx10_Animations[];
extern const u8 BattleFx10_DebrisWidths[];
extern const u8 BattleFx10_DebrisHeights[];
extern const u16 BattleFx10_DebrisCells[];
extern const u8 BattleFx10_SprayWidths[];
extern const u8 BattleFx10_SprayHeights[];
extern const u16 BattleFx10_SprayCells[];
extern const u16 BattleFx10_BoulderCells[];
extern const u8 BattleFx10_BoulderWidths[];
extern const u8 BattleFx10_BoulderHeights[];
extern const u8 BattleFx10_FallWidths[];
extern const u8 BattleFx10_FallHeights[];
extern const u16 BattleFx10_FallCells[];
extern const u16 BattleFx_PuffCells[];
extern const u8 BattleFx_PuffSizes[];

/* The effect's work block (heap slot 39). */
struct Mode10Work {
    u8 sheet[0x7080];
    struct EffectStep sparks[64];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unknown_7788[0x20];
    s32 shake;
    u8 unknown_77ac[0x2c];
    void *objects[9];
    u8 unknown_77fc[0x28];
    s32 transfer_pending;
    struct BattleEffectArgument *effect;
};

#define PARTICLES ((struct EffectStep *)0x02010000)
#define TRAILS ((struct EffectStep *)0x02010e00)
#define HI(v) (((s16 *)&(v))[1])

void BattleFx_InitializeMode10(struct BattleEffectArgument *efx)
{
    s32 pts[32][2];
    s32 pos[4];
    void *dst;
    struct Mode10Work *work;
    s32 frame;
    DrawRectangle blit46;
    DrawRectangle blit47;
    u8 *aux;
    s32 step;
    s32 row;
    u32 *cache;
    s32 count;
    s32 i;
    s32 j;
    s32 k;
    s32 skip;
    s32 ox;
    s32 oy;
    s32 h;
    s32 x;
    s32 y;
    s32 obj;
    s32 t;
    struct EffectStep *p;
    struct EffectStep *q;
    Scale scale;

    cache = (u32 *)(gWorkSlot + 40 * 4);
    dst = (void *)cache[40 - 40];
    work = (struct Mode10Work *)cache[39 - 40];
    aux = (u8 *)cache[41 - 40];
    step = 1;
    work->effect = efx;
    BattleFx_BeginCanvasLayer(0);
    BattlePres_ConfigureEffectDisplay();
    *(u16 *)0x05000000 = 0;
    *(u16 *)0x05000002 = 0;
    work->transfer_mode = 0;
    Scheduler_AddOrUpdateCallback(0x080CD261, 0x480);
    *(u16 *)0x04000048 = 0x2137;
    Unnamed_080cd104(1, 0);
    *(u16 *)0x04000040 = 0xf0f0;
    Resource_LoadAndDecompress((s32)&Value_000000b9, work, 1, 1);
    Resource_LoadAndDecompress((s32)&Value_000000ba, aux, 0, 0);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    blit46 = (DrawRectangle)cache[46 - 40];
    BattleEffect_LoadWork(47, 7, 7, 3, 1);
    blit47 = (DrawRectangle)cache[47 - 40];
    gProjection.unk10 = 240;
    BattleFx_SelectLivingTargets((s32)work->effect);
    WaitFrames(1);
    BattleBackground_LoadFar(1, (s32)&Value_0000003b, 0);
    BattleFx_SpawnObjects(9, 0x174, 1);
    *(u16 *)0x04000000 = 0x7741;
    *(u16 *)0x04000020 = 0x80;
    *(u16 *)0x04000052 = 0x1010;
    *(u16 *)0x04000050 = 0x3f44;
    work->transfer_mode = 2;
    work->transfer_value = 50;

    for (i = 0; i != 64; i++) {
        PARTICLES[i].x = (Random16() & 63) + 32;
        PARTICLES[i].y = (Random16() & 31) + 120;
        PARTICLES[i].variant = -1;
    }

    for (i = 0; i != 32; i++) {
        pts[i][0] = BattleFx10_Points[i][0];
        pts[i][1] = BattleFx10_Points[i][1];
    }

    Audio_PlayCue(141);

    for (frame = 0; frame != 288 && (count = 16, !(gKeysRepeat & 3) || frame <= 16); frame++) {
        if (frame >= 0 && frame < 16) {
            u16 *phase = (u16 *)0x02010000;
            if (frame == 1) {
                u8 *noise = (u8 *)0x02010002;
                s32 n;
                for (n = 0; n != 128; n++) {
                    noise[n] = Random16() & 63;
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

        if (frame > 103) {
            count = 0;
        } else if (frame > 63) {
            count = 6;
        } else if (frame > 31) {
            count = 10;
        }

        if (frame <= 167) {
            ox = (Random16() & 3) - 1;
            oy = (Random16() & 3) - 1;
            gBgScroll.unk04 = ox;
            gBgScroll.unk06 = oy + 32;
        } else {
            ox = 0;
            oy = 0;
            gBgScroll.unk04 = 0;
            gBgScroll.unk06 = 32;
        }
        if (frame >= 176 && frame < 180) {
            ox = -BattleFx10_ShakeOffsets[frame - 176];
            oy = BattleFx10_ShakeOffsets[frame - 176];
            gBgScroll.unk04 = ox;
            gBgScroll.unk06 = oy + 32;
        }

        for (i = 0; i != count; i++) {
            s32 n = Math_Mod(i, 3);
            blit46(dst, work->sheet + BattleFx10_RockCells[n],
                BattleFx10_Points[i][0] - ox, BattleFx10_Points[i][1] - BattleFx10_RockHeights[n] - oy,
                BattleFx10_RockWidths[n], BattleFx10_RockHeights[n]);
        }

        scale = BattleFx10_UnitScale;
        if (frame == 174) {
            for (i = 0; i != 9; i++) {
                ((struct { u8 pad[9]; u8 tile_hi : 2; u8 priority : 2; u8 palette : 4; } *)work->objects[i])->priority = 0;
            }
        }
        if (frame > 208) {
            AnimationObjects_SelectAnimationFar(work->objects[2], BattleFx10_Animations[(frame / 4) & 3]);
        }

        scale.x = 0x10000;
        scale.y = 0x10000;
        pos[3] = 0;
        pos[1] = 0xff0000;
        for (row = 0; row != 3; row++) {
            for (j = 0; j != 3; j++) {
                pos[0] = (j * 32 + 144 - ox) << 16;
                pos[2] = (row * 32 + 76 - oy) << 16;
                Object_ApplyProjectedPlacementFar(work->objects[row * 3 + j], pos, &scale, 0);
            }
        }

        t = frame - 160;
        if (t >= 0 && t < 158) {
            s32 u = frame - 208;
            s32 sy;
            x = 80;
            sy = 8;
            if (frame <= 175) {
                x = 96 - t;
                sy = t * 4 - 56;
            } else if (frame > 208) {
                sy = u / 4 + 8;
            }
            blit47(dst, work->sheet + 0xc46, x, sy, 24, 48);
        }

        if (frame == 32) {
            Audio_PlayCue(134);
        }
        if (frame == 64) {
            Audio_PlayCue(134);
        }
        if (frame == 104) {
            Audio_PlayCue(134);
        }
        if (frame == 176) {
            Audio_PlayCue(134);
        }
        if (frame == 226) {
            Audio_PlayCue(145);
        }
        Render_ResetTransformState();

        if (frame == 32) {
            for (i = 0; i != 32; i++) {
                PARTICLES[i].x = ((Random16() & 31) + 68) << 16;
                PARTICLES[i].y = ((Random16() & 31) + 8) << 16;
                PARTICLES[i].velocity_x = ((Random16() & 127) - 63) << 11;
                PARTICLES[i].velocity_y = ((-Random16() & 127) - 64) << 11;
                PARTICLES[i].variant = (Random16() & 15) + 32;
            }
        }
        if (frame == 64) {
            for (i = 0; i != 32; i++) {
                PARTICLES[i + 32].x = (Math_ModU(Random16(), 48) + 60) << 16;
                PARTICLES[i + 32].y = ((Random16() & 31) + 52) << 16;
                PARTICLES[i + 32].velocity_x = ((Random16() & 127) - 63) << 12;
                PARTICLES[i + 32].velocity_y = ((-Random16() & 31) - 32) << 13;
                PARTICLES[i + 32].variant = (Random16() & 15) + 32;
            }
        }
        if (frame == 104) {
            for (i = 0; i != 32; i++) {
                PARTICLES[i].x = ((Random16() & 63) + 52) << 16;
                PARTICLES[i].y = ((Random16() & 31) + 72) << 16;
                PARTICLES[i].velocity_x = ((Random16() & 127) - 63) << 11;
                PARTICLES[i].velocity_y = ((-Random16() & 31) - 32) << 13;
                PARTICLES[i].variant = (Random16() & 15) + 32;
            }
        }

        if (frame >= 32 && frame < 208) {
            for (i = 0; i != 64; i++) {
                if (PARTICLES[i].variant >= 0) {
                    s32 n;
                    if (frame > 191) {
                        n = Math_Mod(i, 7) + 4;
                    } else {
                        n = i & 3;
                    }
                    blit47(dst, work->sheet + BattleFx10_DebrisCells[n], HI(PARTICLES[i].x), HI(PARTICLES[i].y),
                        BattleFx10_DebrisWidths[n], BattleFx10_DebrisHeights[n]);
                    PARTICLES[i].x += PARTICLES[i].velocity_x;
                    PARTICLES[i].y += PARTICLES[i].velocity_y;
                    PARTICLES[i].velocity_y += 0x2000;
                }
            }
        }

        if (frame > 223) {
            if (frame == 224) {
                for (i = 0; i != 128; i++) {
                    PARTICLES[i].x = 0x480000;
                    PARTICLES[i].y = 0x380000;
                    PARTICLES[i].velocity_x = (-(Random16() & 127) - 64) << 11;
                    PARTICLES[i].velocity_y = ((Random16() & 127) + 16) << 11;
                    PARTICLES[i].variant = Random16();
                }
            }
            for (i = 0; i != 128; i++) {
                if (frame >= i / 4 + 224) {
                    s32 n = Math_Mod(i, 3);
                    if ((i & 1) == 0) {
                        blit46(dst, work->sheet + BattleFx10_SprayCells[n], HI(PARTICLES[i].x), HI(PARTICLES[i].y),
                            BattleFx10_SprayWidths[n], BattleFx10_SprayHeights[n]);
                    }
                    PARTICLES[i].x += PARTICLES[i].velocity_x;
                    PARTICLES[i].y += PARTICLES[i].velocity_y;
                    if ((PARTICLES[i].x >> 16) < -16 || (PARTICLES[i].y >> 16) > 120) {
                        PARTICLES[i].x = 0x480000;
                        PARTICLES[i].y = 0x380000;
                    }
                    PARTICLES[i].variant++;
                }
            }
            if (frame == 228) {
                for (i = 0; i != 128; i++) {
                    p = &PARTICLES[i];
                    q = &TRAILS[i];
                    q->x = p->x;
                    q->y = p->y;
                    q->variant = 0;
                }
            }
            for (i = 0, q = TRAILS, p = PARTICLES; i != 128; i++, q++, p++) {
                if (frame >= i + 228) {
                    s32 n = Math_Mod(q->variant / 2, 9);
                    s32 sz = BattleFx_PuffSizes[n];
                    blit46(dst, aux + BattleFx_PuffCells[n], HI(q->x) - sz / 2, HI(q->y) - sz / 2, sz, sz);
                    if (++q->variant == 18) {
                        q->x = p->x;
                        q->y = p->y;
                        q->variant = 0;
                    }
                }
            }
        }

        work->transfer_pending = 1;
        WaitFrames(1);
    }

    for (i = 0; i != 9; i++) {
        ResourceObject_ReleaseFar(work->objects[i]);
    }
    BattleEffect_SetupBlendedDisplay();
    *(u16 *)0x04000052 = 0x1010;
    Audio_PlayCue((s32)&Value_00000121);

    for (i = 0; i != 64; i++) {
        work->sparks[i].x = ((Random16() & 127) + 64) << 16;
        work->sparks[i].z = -0x100000;
        work->sparks[i].y = (-(Random16() & 127) - 64) << 16;
        work->sparks[i].velocity_x = (-(Random16() & 63) - 127) << 12;
        work->sparks[i].velocity_y = ((Random16() & 63) + 127) << 12;
        work->sparks[i].variant = 0;
    }
    for (i = 0; i != 64; i++) {
        PARTICLES[i].x = Random16() & 127;
        PARTICLES[i].y = (Random16() & 63) + i / 2;
        PARTICLES[i].variant = -i / 2;
    }

    for (frame = 0; frame != 146; frame++) {
        count = 0;
        if (frame == 96) {
            BattleEventRuntime_BeginPhaseFar(134);
        }
        if (frame == 16) {
            work->shake = 32;
        }
        if (frame > 16) {
            count = (frame - 16) / 2;
            if (count > 16) {
                count = 16;
            }
        }
        if (frame >= 9 && frame < 72 && (frame & 3) == 0) {
            Audio_PlayCue(132);
        }
        if (frame == 72) {
            Audio_PlayCue(145);
        }
        if (frame > 64) {
            blit46(dst, work->sheet + 0x14f9, (64 - frame) * 7 + 88, frame * 14 - 992, 40, 80);
        }
        if (frame <= 71) {
            for (i = 0; i != count; i++) {
                s32 n = Math_Mod(i, 3);
                blit46(dst, work->sheet + BattleFx10_BoulderCells[n],
                    BattleFx10_Points[i][0] - 56, BattleFx10_Points[i][1] - BattleFx10_BoulderHeights[n],
                    BattleFx10_BoulderWidths[n], BattleFx10_BoulderHeights[n]);
            }
        }
        if (frame == 72) {
            for (i = 0; i != 64; i++) {
                work->sparks[i].x = (BattleFx10_Points[i & 15][0] - 56) << 16;
                work->sparks[i].y = BattleFx10_Points[i & 15][1] << 16;
                work->sparks[i].velocity_x = ((Random16() & 127) - 63) << 13;
                work->sparks[i].velocity_y = (-(Random16() & 31) - 16) << 14;
            }
            if (frame == 72) {
                work->shake = 4;
            }
        }
        if (frame > 71) {
            count = 32;
            if (frame != 72) {
                count = 64;
            }
            for (i = 0; i != count; i++) {
                q = &work->sparks[i];
                y = HI(q->y);
                if (y <= 135) {
                    s32 n = Math_Mod(i, 3);
                    blit46(dst, work->sheet + BattleFx10_FallCells[n], HI(q->x), y - BattleFx10_FallHeights[n],
                        BattleFx10_FallWidths[n], BattleFx10_FallHeights[n]);
                    EffectStep_AdvanceWithGravity2D(q, 64, 0x10000);
                    if (HI(q->y) > 120 && q->velocity_y > 0x80000) {
                        q->velocity_y = -q->velocity_y / 4;
                        q->y = 0x780000;
                        work->shake = 1;
                    }
                }
            }
        }
        if (frame <= 71) {
            for (i = 0; i != 64; i++) {
                blit46(dst, work->sheet, HI(work->sparks[i].x) - 12, HI(work->sparks[i].y) - 24, 24, 48);
                work->sparks[i].x += work->sparks[i].velocity_x;
                work->sparks[i].y += work->sparks[i].velocity_y;
                if ((work->sparks[i].y >> 16) > 120 && frame <= 47) {
                    work->sparks[i].x = ((Random16() & 127) + 64) << 16;
                    work->sparks[i].y = -0x100000;
                }
            }
        }

        for (i = 0; i != work->effect->count; i++) {
            if (frame == i * 8 + 32) {
                ObjectGroup_UpdateMembers(work->effect->actors[i], 9, 5, -1, 0);
            }
        }

        if (frame > 72) {
            for (i = 0; i != 64; i++) {
                if ((u32)PARTICLES[i].variant < 18) {
                    s32 n = Math_Mod(PARTICLES[i].variant / 2, 9);
                    s32 sz = BattleFx_PuffSizes[n];
                    blit46(dst, aux + BattleFx_PuffCells[n], PARTICLES[i].x - sz / 2, PARTICLES[i].y - sz / 2, sz, sz);
                }
                if (++PARTICLES[i].variant == 18 && frame <= 127) {
                    PARTICLES[i].x = Random16() & 127;
                    PARTICLES[i].y = (Random16() & 63) + (frame - 54) / 2;
                    PARTICLES[i].variant = 0;
                }
            }
        }

        if (frame >= 72 && frame < 80) {
            Camera_ApplyShake(8, 8);
        } else {
            Camera_ApplyShake(2, 2);
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback(0x080CD261);
    BattleFx_EndCanvasLayer();
}
