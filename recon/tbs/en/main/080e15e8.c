/* Draft, not exact (2026-09-24): candidate=3920 reference=3920,
   differing_halfwords=1059, binary similarity 72.2%. Phase one (170
   frames, A or B skips it) draws three dotted beams between six projected
   points, a ring of dots and a falling column; phase two (192 frames)
   bursts sparks, flashes and smoke and pans the camera. Residual: the
   reference frame is 336 bytes, 4 more than this one, and its spill slots
   follow declaration order (ctrl 132, work 128, dst 124, aux 120, frame
   116, an unreferenced slot at 112, blit46 108, size 104, count 100, the
   scale pointer 96, the beam y offset 24k + 4 at 92, blit47 88, the ring
   offset lvl * 0x302 at 84, camera 80..68). Here count, the beam offset
   and the ring offset are expression temporaries spilled after the camera
   words, so every stack offset from 100 down shifts. What helped: count =
   frame * 2 with the beam length written from count * 2 (one giv chain),
   gWorkSlot read through a base pointer for blit46 (sym + 184), masks
   loaded into the result register (speed = 0x3ff; speed &= Random16()),
   and Trig_Sin(angle) * speed operand order. */
#include "TYPES.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "FIXED_MATH.H"
#include "IWRAM_CALL.H"

void WaitFrames(s32);
u32 Random16(void);
void Audio_PlayCue(s32);
void BattleFx_BeginCanvasLayer(s32);
void BattlePres_ConfigureEffectDisplay(void);
void Unnamed_080cd104(s32, s32);
void BattleBackground_LoadFar(s32, s32, s32);
void Graphics_UpdatePhasePalette(s32, s32, s32, s32);
void Camera_StoreSceneParameters(s32, s32, s32);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(s32 *);
void SceneTransform_ApplyPitch(s32);
void SceneTransform_ApplyRoll(s32);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyScale(s32 *);
void BattleEffect_SetupBlendedDisplay(void);
void BattleFx_SelectLivingTargets(void *);
void BattleFx_SpawnObjects(s32, s32, s32);
void *Resource_GetTableEntry(s32);
void BattleEventRuntime_BeginPhaseFar(s32);
void BattleMotion_ApplyVariantMotionFar(s32, s32);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *, s32, s32);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
void ObjectGroup_TickMemberTimers(void);
void Camera_ApplyShake(s32, s32);
void Unnamed_080e6d3c(s32, s32, s32);
void Unnamed_080e6eac(s32, s32, s32);
void ResourceObject_ReleaseFar(void *);
void Runtime_ReleaseHeapBlock(s32);
s32 BattleFx_EndCanvasLayer(void);

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);
typedef s32 (*RatioFn)(s32, s32);

extern u8 gWorkSlot[];
extern volatile u32 gKeysRepeat;

struct Cells03001ce0 {
    s32 unk00[4];
    s32 unk10;
};
extern struct Cells03001ce0 gProjection;

extern u8 Value_0000003d;
extern u8 Value_00000062;
extern u8 Value_00000066;
extern u8 Value_00000073;
extern u8 Value_00000074;
extern u8 Value_00000075;
extern u8 Value_000000bc;
extern u8 Value_000000ce;
extern u8 Value_000000cf;
extern u8 Value_000000d1;

extern s16 BattleFx12_Points[][2];
extern u16 BattleFx12_DotCells[];
extern u8 BattleFx12_Animations[];
extern u8 BattleFx12_SmokePalettes[];
extern u16 BattleFx12_SmokeCells[];
extern u8 BattleFx12_SmokeWidths[];
extern u8 BattleFx12_SmokeHeights[];

/* The effect's work block (heap slot 39). */
struct Mode12Work {
    u8 sheet[0x7080];
    struct EffectStep sparks[64];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unknown_7788[8];
    s32 unknown_7790;
    s32 unknown_7794;
    s32 unknown_7798;
    s32 unknown_779c;
    u8 unknown_77a0[8];
    s32 shake;
    u8 unknown_77ac[0x2c];
    void *objects[9];
    u8 unknown_77fc[0x28];
    s32 transfer_pending;
    struct BattleEffectArgument *effect;
};

struct Point {
    s32 x;
    s32 y;
    s32 z;
};

#define PARTICLES ((struct EffectStep *)0x02010c58)
#define SLOT(n) (((u32 *)gWorkSlot)[n])
#define HI(v) (((s16 *)&(v))[1])

void BattleFx_InitializeMode12(struct BattleEffectArgument *efx)
{
    u8 ramp[32];
    s32 vec[3];
    struct Point pts[11];
    s32 pos[3];
    s32 scale[3];
    s32 *ctrl;
    struct Mode12Work *work;
    void *dst;
    u8 *aux;
    s32 frame;
    DrawRectangle blit46;
    s32 size;
    s32 count;
    s32 *sc;
    DrawRectangle blit47;
    s32 lvl;
    s32 cam_x;
    s32 cam_y;
    s32 vel_x;
    s32 vel_y;
    s32 i;
    s32 j;
    s32 n;
    struct EffectStep *p;
    u32 *cache;
    s32 cx;
    s32 cy;

    ctrl = (s32 *)SLOT(44);
    work = (struct Mode12Work *)SLOT(39);
    dst = (void *)SLOT(40);
    aux = (u8 *)SLOT(41);
    cache = (u32 *)gWorkSlot;
    work->effect = efx;
    BattleFx_BeginCanvasLayer(0x2000);
    *(u16 *)0x04000020 = 0x100;
    Resource_LoadAndDecompress((s32)&Value_000000bc, work, 1, 1);
    Resource_LoadAndDecompress((s32)&Value_000000bc, work, 1, 1);
    Resource_LoadAndDecompress((s32)&Value_00000075, work->sheet + 0x1800, 0, 0);
    Resource_LoadAndDecompress((s32)&Value_00000073, aux, 0, 0);

    for (i = 0, n = 0, count = 64; i != 8; i++) {
        s32 lim = count;
        u8 *src = aux;
        u8 *out = work->sheet + n + 0x2710;
        for (j = 0; j != 0x302; j++) {
            s32 v = *src++;
            if (v > lim) {
                v = lim;
            }
            if (v < 0) {
                v = 0;
            }
            *out++ = v;
        }
        n += j;
        count -= 7;
    }

    BattlePres_ConfigureEffectDisplay();
    *(u16 *)0x05000000 = 0;
    *(u16 *)0x05000002 = 0;
    *(u16 *)0x0400000c = 0x2784;
    work->unknown_7790 = 0;
    work->unknown_7794 = 2;
    work->unknown_7798 = 1;
    work->unknown_779c = 0;
    ctrl[4] = 1;
    Scheduler_AddOrUpdateCallback(0x080C90E5, 0x480);
    Scheduler_AddOrUpdateCallback(0x080CD261, 0x480);
    work->transfer_mode = 0;
    Unnamed_080cd104(0, 0);
    BattleBackground_LoadFar(1, (s32)&Value_0000003d, 0);
    gProjection.unk10 = 240;
    Unnamed_080cd104(0, 1);
    *(u16 *)0x04000020 = 0x80;
    BattleEffect_LoadWork(46, 7, 7, 3, 3);
    blit46 = (DrawRectangle)cache[46];
    *(u16 *)0x04000000 = 0x7741;
    *(u16 *)0x04000020 = 0x80;
    *(u16 *)0x04000052 = 0x100f;
    *(u16 *)0x04000050 = 0x3f44;
    work->transfer_mode = 2;
    work->transfer_value = 75;

    for (i = 0; i != 16; i++) {
        ramp[i] = i * 8 + 3;
        ramp[31 - i] = i * 8 + 3;
        if (ramp[i] > 63) {
            ramp[i] = 63;
        }
        if (ramp[31 - i] > 63) {
            ramp[31 - i] = 63;
        }
    }
    *(u16 *)0x0400000c = 0x784;
    sc = scale;

    for (frame = 0; frame != 170; frame++) {
        size = 2;
        count = frame * 2;
        if (frame == 16) {
            Audio_PlayCue(140);
        }
        if (frame == 132) {
            Audio_PlayCue(131);
        }
        if (frame == 151) {
            Audio_PlayCue(145);
        }
        if (gKeysRepeat & 3) {
            break;
        }
        Graphics_UpdatePhasePalette(frame * 2, 0xaaab, 0x5555, 0);
        if (frame == 150) {
            work->transfer_mode = 1;
            work->transfer_value = 0x1a1a1a1a;
        } else {
            work->transfer_mode = size;
            work->transfer_value = 75;
        }
        Camera_StoreSceneParameters(0x1fe0000, ((RatioFn)0x0300013c)(0x1fe0000, 0xc000), 0x7fff0000);
        Render_ResetTransformState();
        if (frame > 128) {
            s32 a = frame - 128;
            if (a > 22) {
                a = 20;
            }
            pos[0] = 0;
            pos[1] = -a << 17;
            pos[2] = 0x2000000;
            size = (a >> 2) + 2;
            if (size > 8) {
                size = 8;
            }
            SceneTransform_ApplyPosition(pos);
            SceneTransform_ApplyPitch(-0x1000);
            SceneTransform_ApplyRoll(0x8000);
            SceneTransform_ApplyPitch(0x8000);
            SceneTransform_ApplyYaw(a << 12);
            if (frame > 150) {
                s32 s = frame * 0x1800 - 0xd9400;
                sc[0] = s;
                sc[1] = s;
                sc[2] = s;
                size = 5;
            } else {
                s32 s = 0x10000 - a * 3 * 0x400;
                sc[0] = s;
                sc[1] = s;
                sc[2] = s;
            }
            SceneTransform_ApplyScale(sc);
        } else {
            pos[0] = 0;
            pos[1] = 0;
            pos[2] = 0x1000000;
            SceneTransform_ApplyPosition(pos);
            SceneTransform_ApplyPitch(-0x1000);
            SceneTransform_ApplyRoll(frame << 8);
            SceneTransform_ApplyPitch(frame << 8);
        }

        if (frame <= 149) {
            for (i = 0; i != 6; i++) {
                vec[0] = (BattleFx12_Points[i][0] - 96) << 16;
                vec[1] = 0;
                vec[2] = (BattleFx12_Points[i][1] - 96) << 16;
                EffectPosition_ApplyBaseAndYOffset((s32)vec, (struct EffectPosition *)&pts[i]);
                pts[i].x = (pts[i].x >> 17) + 64;
                pts[i].y = HI(pts[i].y) + 60;
            }
            for (i = 0; i != 3; i++) {
                s32 len = count * 2 - i * 48 - 256;
                if (len > 48) {
                    len = 48;
                }
                if (len >= 0) {
                    for (j = 0; j != len; j++) {
                        s32 x = pts[i * 2].x + Iwram_MulQ16(j * (pts[i * 2 + 1].x - pts[i * 2].x), 0x555);
                        s32 y = pts[i * 2].y + Iwram_MulQ16(j * (pts[i * 2 + 1].y - pts[i * 2].y), 0x555);
                        blit46(dst, aux + BattleFx12_DotCells[size - 1], x - size / 2, y - size, size, size * 2);
                    }
                }
            }
        }

        if (frame <= 179) {
            lvl = 0;
            if (frame > 155) {
                lvl = frame - 156;
            }
            if (lvl > 7) {
                lvl = 7;
            }
            if (frame <= 139) {
                BattleEffect_LoadWork(47, 7, 7, 3, 3);
                blit47 = (DrawRectangle)SLOT(47);
            } else {
                BattleEffect_LoadWork(47, 7, 7, 3, 2);
                blit47 = (DrawRectangle)SLOT(47);
            }
            n = count;
            if (n > 128) {
                n = 128;
            }
            for (i = 0; i != n; i++) {
                s32 s;
                vec[0] = Trig_Sin(i << 9) * 96;
                vec[2] = -(Trig_Cos(i << 9) * 96);
                EffectPosition_ApplyBaseAndYOffset((s32)vec, (struct EffectPosition *)&pts[0]);
                pts[0].x = (pts[0].x >> 17) + 64;
                pts[0].y = HI(pts[0].y) + 60;
                s = size + 1;
                blit47(dst, work->sheet + 0x2710 + lvl * 0x302 + BattleFx12_DotCells[s - 1],
                    pts[0].x - s / 2, pts[0].y - s, s, s * 2);
            }
            Runtime_ReleaseHeapBlock(47);
        }

        if ((u32)(frame - 151) <= 16) {
            s32 w = 0;
            s32 h = (frame - 151) * 20 + 44;
            if (frame > 151) {
                w = frame - 152;
            }
            if (w > 15) {
                w = 15;
            }
            for (i = 1; i != 10; i++) {
                if (w + i <= 15) {
                    blit46(dst, &ramp[w + i], w + 48 + i, 16 - i, 32 - w * 2 - i * 2, 1);
                }
            }
            for (i = 0; i != h; i++) {
                blit46(dst, ramp + w, w + 48, i + 16, 32 - w * 2, 1);
            }
            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            ((DrawRectangle)SLOT(47))(dst, work, 32, h - 56, 32, 96);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            ((DrawRectangle)SLOT(47))(dst, work, 64, h - 56, 32, 96);
            Runtime_ReleaseHeapBlock(47);
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    ctrl[4] = 0;
    Scheduler_RemoveCallback(0x080C90E5);
    BattleEffect_SetupBlendedDisplay();
    BattleFx_SelectLivingTargets(work->effect);
    BattleFx_SpawnObjects(9, 0x173, 1);
    Resource_LoadAndDecompress((s32)&Value_000000ce, (void *)0x02010000, 1, 0);
    Resource_LoadAndDecompress((s32)&Value_000000d1, work, 1, 1);
    Resource_LoadAndDecompress((s32)&Value_00000066, work->sheet + 0x6000, 1, 0);
    ((WordCopyFn)0x03001388)((void *)0x05000000, Resource_GetTableEntry((s32)&Value_000000cf), 128);
    Resource_LoadAndDecompress((s32)&Value_00000074, aux, 0, 0);
    *(u16 *)0x04000052 = 0x1010;
    cam_x = 0x500000;
    cam_y = 0x400000;
    vel_x = 0;
    vel_y = 0;
    for (i = 0; i != 910; i++) {
        PARTICLES[i].variant = 0;
    }
    for (i = 0; i != 16; i++) {
        work->sparks[i].x = (Random16() & 31) - 16;
        work->sparks[i].y = Random16() & 63;
        work->sparks[i].variant = -(Random16() & 15);
    }
    work->transfer_mode = 2;
    work->transfer_value = 50;
    Audio_PlayCue(145);
    cx = 60;
    cy = 112;

    for (frame = 0; frame != 192; frame++) {
        s32 d;
        if (frame == 20) {
            Resource_LoadAndDecompress((s32)&Value_00000062, work, 1, 0);
        }
        d = frame - 80;
        if ((u32)d <= 59 && !(frame & 7)) {
            Audio_PlayCue(134);
        }
        if (frame == 140) {
            BattleEventRuntime_BeginPhaseFar(134);
        }
        for (j = 0; j != 1; j++) {
            s32 start = j * 128 + 8;
            if (frame >= start && frame < j * 128 + 17) {
                if (frame >= j * 128 + 9 && frame < j * 128 + 12) {
                    blit46(dst, work->sheet, 36, 0, 48, 112);
                }
                if (frame >= j * 128 + 12 && frame < start + 8) {
                    blit46(dst, work->sheet + 0x1500, 36, 0, 48, 112);
                }
                if (frame == start + 2) {
                    n = 0;
                    for (i = 0, p = PARTICLES; i != 910; i++, p++) {
                        if (p->variant == 0) {
                            s32 speed = 0x3ff;
                            s32 angle;
                            speed &= Random16();
                            angle = (Random16() & 0x7fff) - 0x4000;
                            p->x = cx << 16;
                            p->y = cy << 16;
                            speed += 32;
                            p->velocity_x = (Trig_Sin(angle) * speed) >> 7;
                            p->velocity_y = -((Trig_Cos(angle) * speed) << 1) >> 7;
                            p->variant = (Random16() & 7) + 32;
                            if (++n == 512) {
                                break;
                            }
                        }
                    }
                    work->shake = 8;
                }
            }
        }
        if (frame == 10) {
            for (i = 0; i != work->effect->count; i++) {
                ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 8);
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 4);
            }
        }
        if ((u32)d <= 63) {
            s32 ox;
            s32 oy;
            s32 sx;
            u8 *smoke;
            for (i = 0; i != 6; i++) {
                if (frame == i * 8 + 80) {
                    ((WordCopyFn)0x03000168)(dst, (void *)0x4000, 0x10101010);
                }
            }
            for (i = 0; i != 30; i++) {
                if (frame >= i * 2 + 80) {
                    if (frame < i * 2 + 82) {
                        s32 rx = Random16() & 3;
                        s32 ry = Random16() & 3;
                        BattleEffect_LoadWork(47, 7, 7, 3, 2);
                        ((DrawRectangle)SLOT(47))(dst, work->sheet + Math_Mod(i, 3) * 0x1440, rx - 3, ry + 32, 72, 72);
                        Runtime_ReleaseHeapBlock(47);
                    }
                    if (frame == i * 2 + 80) {
                        n = 0;
                        for (j = 0, p = PARTICLES; j != 910; j++, p++) {
                            if (p->variant == 0) {
                                s32 speed = 0x3ff;
                                s32 angle;
                                speed &= Random16();
                                angle = Random16() & 0xffff;
                                p->x = 0x100000;
                                p->y = 0x500000;
                                speed += 32;
                                p->velocity_x = (Trig_Sin(angle) * speed) >> 7;
                                p->velocity_y = -((Trig_Cos(angle) * speed) << 1) >> 7;
                                p->variant = (Random16() & 7) + 32;
                                if (++n == 16) {
                                    break;
                                }
                            }
                        }
                        work->shake = 8;
                        for (j = 0; j != work->effect->count; j++) {
                            ObjectGroup_UpdateMembers(work->effect->actors[j],
                                BattleFx12_Animations[Random16() & 3], 5, j, 4);
                            BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 4);
                        }
                    }
                }
            }
            {
                s32 r = 7;
                s32 angle;
                r &= Random16();
                angle = Random16() & 0xffff;
                ox = (Trig_Sin(angle) * (r + 8)) >> 16;
                sx = ox + 72;
                oy = 32 - ((Trig_Cos(angle) * (r + 8)) >> 16);
            }
            smoke = work->sheet + 0x6000;
            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            ((DrawRectangle)SLOT(47))(dst, smoke, ox + 60, oy - 24, 12, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            ((DrawRectangle)SLOT(47))(dst, smoke, sx, oy - 24, 12, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 11, 2);
            ((DrawRectangle)SLOT(47))(dst, smoke, ox + 60, oy, 12, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 15, 2);
            ((DrawRectangle)SLOT(47))(dst, smoke, sx, oy, 12, 24);
            Runtime_ReleaseHeapBlock(47);
            n = 0;
            for (i = 0, p = PARTICLES; i != 910; i++, p++) {
                if (p->variant == 0) {
                    s32 speed = 63;
                    s32 angle;
                    speed &= Random16();
                    angle = Random16() & 0xffff;
                    p->x = sx << 16;
                    p->y = oy << 16;
                    speed += 64;
                    p->velocity_x = (Trig_Sin(angle) * speed) >> 7;
                    p->velocity_y = -((Trig_Cos(angle) * speed) << 1) >> 7;
                    p->variant = (Random16() & 7) + 16;
                    if (++n == 4) {
                        break;
                    }
                }
            }
        }

        BattleEffect_LoadWork(47, 7, 7, 15, 2);
        {
            DrawRectangle draw = (DrawRectangle)SLOT(47);
            for (i = 0, p = PARTICLES; i != 910; i++, p++) {
                if (p->variant > 0) {
                    p->variant--;
                    EffectStep_AdvanceWithGravity2D(p, 60, 0);
                    if (p->y > 0x780000) {
                        p->velocity_y = -p->velocity_y / 2;
                    } else if ((u32)p->x <= 0x7effff && p->y >= 0) {
                        s32 x = p->x >> 16;
                        s32 y = p->y >> 16;
                        s32 s = p->variant / 16 + 1;
                        draw(dst, aux + BattleFx12_DotCells[s - 1], x - s / 2, y - s, s, s * 2);
                    }
                }
            }
        }
        Runtime_ReleaseHeapBlock(47);

        if (frame > 15) {
            if (frame == 32) {
                vel_x = 0x40000;
                vel_y = -0x4000;
            }
            if (frame > 31) {
                cam_x += vel_x;
                cam_y += vel_y;
                vel_x = vel_x * 60 / 64;
                vel_y = vel_y * 60 / 64;
            }
            Unnamed_080e6d3c(0, cam_x, cam_y);
        }
        if ((u32)(frame - 16) <= 127) {
            s32 base = cam_x >> 17;
            for (i = 0; i != 3; i++) {
                s32 k = i & 3;
                s32 angle = Random16() & 0xffff;
                s32 x = ((Trig_Sin(angle) << 3) >> 16) + base - (BattleFx12_SmokeWidths[k] >> 1);
                s32 y = ((Trig_Cos(angle) * 40) >> 16) - (BattleFx12_SmokeHeights[k] >> 1);
                BattleEffect_LoadWork(47, 7, 7, 3 | BattleFx12_SmokePalettes[Random16() & 3], 2);
                ((DrawRectangle)SLOT(47))(dst, (u8 *)0x02010000 + BattleFx12_SmokeCells[k], x, y + 56,
                    BattleFx12_SmokeWidths[k], BattleFx12_SmokeHeights[k]);
                Runtime_ReleaseHeapBlock(47);
            }
        }
        if (frame <= 31) {
            Camera_ApplyShake(8, 16);
        } else {
            Camera_ApplyShake(4, 4);
        }
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback(0x080CD261);
    Unnamed_080e6eac(0, cam_x, cam_y);
    for (i = 0; i != 9; i++) {
        ResourceObject_ReleaseFar(work->objects[i]);
    }
    BattleFx_EndCanvasLayer();
}
