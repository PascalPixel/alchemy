/* Draft, not exact (2026-09-24): candidate=3164 reference=3156 differing_halfwords=930,
   binary similarity 63%. The stack frame (332 bytes, every spill slot), the
   two frame loops, the skip test with its count reset, the noise loop end
   pointer and the 3x3 placement loop all match; the residual is register
   allocation (ox/oy and loop temporaries take other high registers) and the
   scheduling that follows from it. */
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

    for (i = 0, p = PARTICLES; i != 64; i++, p++) {
        p->x = (Random16() & 63) + 32;
        p->y = (Random16() & 31) + 120;
        p->variant = -1;
    }

    for (i = 0; i != 32; i++) {
        pts[i][0] = BattleFx10_Points[i][0];
        pts[i][1] = BattleFx10_Points[i][1];
    }

    Audio_PlayCue(141);

    for (frame = 0; frame != 288 && (k = gKeysRepeat & 3, count = 16, !k || frame <= 16); frame++) {
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
            k = Math_Mod(i, 3);
            h = BattleFx10_RockHeights[k];
            blit46(dst, work->sheet + BattleFx10_RockCells[k],
                BattleFx10_Points[i][0] - ox, BattleFx10_Points[i][1] - h - oy,
                BattleFx10_RockWidths[k], h);
        }

        scale = BattleFx10_UnitScale;
        if (frame == 174) {
            for (i = 0; i != 9; i++) {
                ((u8 *)work->objects[i])[9] &= ~12;
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
            for (i = 0, p = PARTICLES; i != 32; i++, p++) {
                p->x = ((Random16() & 31) + 68) << 16;
                p->y = ((Random16() & 31) + 8) << 16;
                p->velocity_x = ((Random16() & 127) - 63) << 11;
                p->velocity_y = ((-Random16() & 127) - 64) << 11;
                p->variant = (Random16() & 15) + 32;
            }
        }
        if (frame == 64) {
            for (i = 0, p = PARTICLES + 32; i != 32; i++, p++) {
                p->x = (Math_ModU(Random16(), 48) + 60) << 16;
                p->y = ((Random16() & 31) + 52) << 16;
                p->velocity_x = ((Random16() & 127) - 63) << 12;
                p->velocity_y = ((-Random16() & 31) - 32) << 13;
                p->variant = (Random16() & 15) + 32;
            }
        }
        if (frame == 104) {
            for (i = 0, p = PARTICLES; i != 32; i++, p++) {
                p->x = ((Random16() & 63) + 52) << 16;
                p->y = ((Random16() & 31) + 72) << 16;
                p->velocity_x = ((Random16() & 127) - 63) << 11;
                p->velocity_y = ((-Random16() & 31) - 32) << 13;
                p->variant = (Random16() & 15) + 32;
            }
        }

        if (frame >= 32 && frame < 208) {
            for (i = 0, p = PARTICLES; i != 64; i++, p++) {
                if (p->variant >= 0) {
                    if (frame > 191) {
                        k = Math_Mod(i, 7) + 4;
                    } else {
                        k = i & 3;
                    }
                    blit47(dst, work->sheet + BattleFx10_DebrisCells[k], HI(p->x), HI(p->y),
                        BattleFx10_DebrisWidths[k], BattleFx10_DebrisHeights[k]);
                    p->x += p->velocity_x;
                    p->y += p->velocity_y;
                    p->velocity_y += 0x2000;
                }
            }
        }

        if (frame > 223) {
            if (frame == 224) {
                for (i = 0, p = PARTICLES; i != 128; i++, p++) {
                    p->x = 0x480000;
                    p->y = 0x380000;
                    p->velocity_x = (-(Random16() & 127) - 64) << 11;
                    p->velocity_y = ((Random16() & 127) + 16) << 11;
                    p->variant = Random16();
                }
            }
            for (i = 0, p = PARTICLES; i != 128; i++, p++) {
                if (frame >= i / 4 + 224) {
                    k = Math_Mod(i, 3);
                    if ((i & 1) == 0) {
                        blit46(dst, work->sheet + BattleFx10_SprayCells[k], HI(p->x), HI(p->y),
                            BattleFx10_SprayWidths[k], BattleFx10_SprayHeights[k]);
                    }
                    p->x += p->velocity_x;
                    p->y += p->velocity_y;
                    if ((p->x >> 16) < -16 || (p->y >> 16) > 120) {
                        p->x = 0x480000;
                        p->y = 0x380000;
                    }
                    p->variant++;
                }
            }
            if (frame == 228) {
                for (i = 0, p = PARTICLES, q = TRAILS; i != 128; i++, p++, q++) {
                    q->x = p->x;
                    q->y = p->y;
                    q->variant = 0;
                }
            }
            for (i = 0, p = PARTICLES, q = TRAILS; i != 128; i++, p++, q++) {
                if (frame >= i + 228) {
                    k = Math_Mod(q->variant / 2, 9);
                    h = BattleFx_PuffSizes[k];
                    blit46(dst, aux + BattleFx_PuffCells[k], HI(q->x) - h / 2, HI(q->y) - h / 2, h, h);
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

    for (i = 0, p = work->sparks; i != 64; i++, p++) {
        p->x = ((Random16() & 127) + 64) << 16;
        p->z = -0x100000;
        p->y = (-(Random16() & 127) - 64) << 16;
        p->velocity_x = (-(Random16() & 63) - 127) << 12;
        p->velocity_y = ((Random16() & 63) + 127) << 12;
        p->variant = 0;
    }
    for (i = 0, p = PARTICLES; i != 64; i++, p++) {
        p->x = Random16() & 127;
        p->y = (Random16() & 63) + i / 2;
        p->variant = -i / 2;
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
                k = Math_Mod(i, 3);
                h = BattleFx10_BoulderHeights[k];
                blit46(dst, work->sheet + BattleFx10_BoulderCells[k],
                    BattleFx10_Points[i][0] - 56, BattleFx10_Points[i][1] - h,
                    BattleFx10_BoulderWidths[k], h);
            }
        }
        if (frame == 72) {
            for (i = 0, p = work->sparks; i != 64; i++, p++) {
                p->x = (BattleFx10_Points[i & 15][0] - 56) << 16;
                p->y = BattleFx10_Points[i & 15][1] << 16;
                p->velocity_x = ((Random16() & 127) - 63) << 13;
                p->velocity_y = (-(Random16() & 31) - 16) << 14;
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
            for (i = 0, p = work->sparks; i != count; i++, p++) {
                y = HI(p->y);
                if (y <= 135) {
                    k = Math_Mod(i, 3);
                    h = BattleFx10_FallHeights[k];
                    blit46(dst, work->sheet + BattleFx10_FallCells[k], HI(p->x), y - h,
                        BattleFx10_FallWidths[k], h);
                    EffectStep_AdvanceWithGravity2D(p, 64, 0x10000);
                    if (HI(p->y) > 120 && p->velocity_y > 0x80000) {
                        p->velocity_y = -p->velocity_y / 4;
                        p->y = 0x780000;
                        work->shake = 1;
                    }
                }
            }
        }
        if (frame <= 71) {
            for (i = 0, p = work->sparks; i != 64; i++, p++) {
                blit46(dst, work->sheet, HI(p->x) - 12, HI(p->y) - 24, 24, 48);
                p->x += p->velocity_x;
                p->y += p->velocity_y;
                if ((p->y >> 16) > 120 && frame <= 47) {
                    p->x = ((Random16() & 127) + 64) << 16;
                    p->y = -0x100000;
                }
            }
        }

        for (i = 0; i != work->effect->count; i++) {
            if (frame == i * 8 + 32) {
                ObjectGroup_UpdateMembers(work->effect->actors[i], 9, 5, -1, 0);
            }
        }

        if (frame > 72) {
            for (i = 0, p = PARTICLES; i != 64; i++, p++) {
                if ((u32)p->variant < 18) {
                    k = Math_Mod(p->variant / 2, 9);
                    h = BattleFx_PuffSizes[k];
                    blit46(dst, aux + BattleFx_PuffCells[k], p->x - h / 2, p->y - h / 2, h, h);
                }
                if (++p->variant == 18 && frame <= 127) {
                    p->x = Random16() & 127;
                    p->y = (Random16() & 63) + (frame - 54) / 2;
                    p->variant = 0;
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
