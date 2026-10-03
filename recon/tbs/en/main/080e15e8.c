#include "CANVAS.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "PROJECT.H"
/* Draft, 664 rows off, most of them from one cause: the frame is 4 bytes
   short. The ROM spills count itself (sp+100, between size and sc) as a copy
   of the frame * 2 induction; here count is replaced by the temporary of its
   own multiplication, whose slot falls below sc. The ROM also reduces
   frame * 4 for the three line lengths to an induction of its own (sp+24);
   here that value lives too briefly to be worth one. And the ROM keeps j in
   r8 and n in r10 where this has them the other way round. */
#include "TYPES.H"
#include "IO_REG.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "FIXED_MATH.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "SYSTEM.H"

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_AdvanceScrollOnInterval(void);
void AudioCommand_PlayFar(s32);
void BattlePres_ConfigureEffectDisplay(void);
void BattleEffect_WipeCanvas(s32, s32);
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
void BattleEventRuntime_BeginPhaseFar(s32);
void BattleMotion_ApplyVariantMotionFar(s32, s32);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *, s32, s32);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
void ObjectGroup_TickMemberTimers(void);
void Camera_ApplyShake(s32, s32);
void BattleFx_PlaceFormationObjects(s32, s32, s32);
void BattleEffect_RunImpactBurst(s32, s32, s32);

extern u8 gWorkSlot[];
extern volatile u32 gKeysRepeat;



extern s16 Mode12_Points[][2];
extern u16 ParticleStreams_CellOffsets[];
extern u8 Mode12_Animations[];
extern u8 Mode12_SmokeFlips[];
extern u16 BattleFx_GlintCellOffsets[];
extern u8 BattleFx_GlintCellWidths[];
extern u8 BattleFx_GlintCellHeights[];

struct Point {
    s32 x;
    s32 y;
    s32 z;
};

struct BlitterPair {
    DrawRectangle upper;
    DrawRectangle lower;
};

#define PARTICLES ((struct EffectStep *)(Ram_MapCellBuffer + 0xc58))
#define SLOT(n) (((DrawRectangle *)gWorkSlot)[n])
#define HI(v) (((s16 *)&(v))[1])

void BattleFx_InitializeMode12(struct BattleEffectArgument *efx)
{
    u8 ramp[32];
    s32 vec[3];
    struct Point pts[11];
    s32 pos[3];
    s32 scale[3];
    void **slots;
    s32 *ctrl;
    struct BattleEffectWork *work;
    void *dst;
    u8 *aux;
    s32 frame;
    DrawRectangle blit[2];
    s32 size;
    s32 count;
    s32 *sc;
    s32 i;
    s32 j;
    s32 n;
    struct EffectStep *p;
    DrawRectangle *cache;
    s32 rings = 0x2710;

    slots = (void **)(gWorkSlot + 44 * 4);
    ctrl = slots[0];
    work = slots[-5];
    dst = slots[-4];
    aux = slots[-3];
    work->effect = efx;
    BattleFx_BeginCanvasLayer(0x2000);
    *(u16 *)0x04000020 = 0x100;
    Resource_LoadAndDecompress((s32)&ResourceId_LightFanSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_LightFanSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesC, work->sheet + 0x1800, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, aux, 0, 0);

    {
    s32 offset;
    for (i = 0, offset = 0; i != 8; i++) {
        for (j = 0; j != 0x302; j++) {
            s32 v = aux[j];

            if (v > 64 - i * 7) {
                v = 64 - i * 7;
            }
            if (v < 0) {
                v = 0;
            }
            (work->sheet + offset)[rings + j] = v;
        }
        offset += j;
    }
    }

    BattlePres_ConfigureEffectDisplay();
    *(u16 *)0x05000000 = 0;
    *(u16 *)0x05000002 = 0;
    *(u16 *)0x0400000c = 0x2784;
    work->scroll_timer = 0;
    work->scroll_interval = 2;
    work->scroll_step_x = 1;
    work->scroll_step_y = 0;
    ctrl[4] = 1;
    Scheduler_AddOrUpdateCallback((s32)BattleFx_AdvanceScrollOnInterval, 0x480);
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    work->transfer_mode = 0;
    BattleEffect_WipeCanvas(0, 0);
    BattleBackground_LoadFar(1, (s32)&ResourceId_DuskCloudsBackdrop, 0);
    gProjection.center_y = 240;
    BattleEffect_WipeCanvas(0, 1);
    *(u16 *)0x04000020 = 0x80;
    BattleEffect_LoadWork(46, 7, 7, 3, 3);
    cache = (DrawRectangle *)gWorkSlot;
    blit[0] = cache[46];
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
            AudioCommand_PlayFar(140);
        }
        if (frame == 132) {
            AudioCommand_PlayFar(131);
        }
        if (frame == 151) {
            AudioCommand_PlayFar(145);
        }
        if (gKeysRepeat & (KEY_A | KEY_B)) {
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
        Camera_StoreSceneParameters(0x1fe0000, Iwram_RatioMulQ14(0x1fe0000, 0xc000), 0x7fff0000);
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
                size = 5;
                sc[0] = s;
                sc[1] = s;
                sc[2] = s;
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
            s32 reach = frame * 4;

            for (i = 0; i != 6; i++) {
                vec[0] = (Mode12_Points[i][0] - 96) << 16;
                vec[1] = 0;
                vec[2] = (Mode12_Points[i][1] - 96) << 16;
                EffectPosition_ApplyBaseAndYOffset(vec, (struct EffectPosition *)&pts[i]);
                pts[i].x = (pts[i].x >> 17) + 64;
                pts[i].y = HI(pts[i].y) + 60;
            }
            for (i = 0; i != 3; i++) {
                n = reach - i * 48 - 256;
                if (n > 48) {
                    n = 48;
                }
                if (n >= 0) {
                    for (j = 0; j != n; j++) {
                        s32 x = pts[i * 2].x + Iwram_MulQ16(j * (pts[i * 2 + 1].x - pts[i * 2].x), 0x555);
                        s32 y = pts[i * 2].y + Iwram_MulQ16(j * (pts[i * 2 + 1].y - pts[i * 2].y), 0x555);
                        blit[0](dst, aux + ParticleStreams_CellOffsets[size - 1], x - size / 2, y - size, size, size * 2);
                    }
                }
            }
        }

        if (frame <= 179) {
            DrawRectangle blit47;
            s32 lvl = 0;

            if (frame > 155) {
                lvl = frame - 156;
            }
            if (lvl > 7) {
                lvl = 7;
            }
            if (frame <= 139) {
                BattleEffect_LoadWork(47, 7, 7, 3, 3);
                blit47 = SLOT(47);
            } else {
                BattleEffect_LoadWork(47, 7, 7, 3, 2);
                blit47 = SLOT(47);
            }
            n = count;
            if (n > 128) {
                n = 128;
            }
            for (i = 0; i != n; i++) {
                s32 s;
                vec[0] = Trig_Sin(i << 9) * 96;
                vec[2] = -(Trig_Cos(i << 9) * 96);
                EffectPosition_ApplyBaseAndYOffset(vec, (struct EffectPosition *)&pts[0]);
                pts[0].x = (pts[0].x >> 17) + 64;
                pts[0].y = HI(pts[0].y) + 60;
                s = size + 1;
                blit47(dst, work->sheet + rings + lvl * 0x302 + ParticleStreams_CellOffsets[s - 1],
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
                    blit[0](dst, &ramp[w + i], w + 48 + i, 16 - i, 32 - w * 2 - i * 2, 1);
                }
            }
            for (i = 0; i != h; i++) {
                blit[0](dst, ramp + w, w + 48, i + 16, 32 - w * 2, 1);
            }
            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            SLOT(47)(dst, work, 32, h - 56, 32, 96);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            SLOT(47)(dst, work, 64, h - 56, 32, 96);
            Runtime_ReleaseHeapBlock(47);
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    ctrl[4] = 0;
    Scheduler_RemoveCallback((u32)BattleFx_AdvanceScrollOnInterval);
    BattleEffect_SetupBlendedDisplay();
    BattleFx_SelectLivingTargets(work->effect);
    BattleFx_SpawnObjects(9, 0x173, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, Ram_MapCellBuffer, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_VioletMoteSheet, work->sheet + 0x6000, 1, 0);
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_CrystalSheet), 128);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesB, aux, 0, 0);
    *(u16 *)0x04000052 = 0x1010;
    {
    s32 cam_x;
    s32 cam_y;
    s32 vel_x;
    s32 vel_y;
    s32 cx;
    s32 cy;

    cam_x = 0x500000;
    cam_y = 0x400000;
    vel_x = 0;
    vel_y = 0;
    for (i = 0; i != 910; i++) {
        PARTICLES[i].variant = 0;
    }
    for (i = 0; i != 16; i++) {
        work->particles[i].x = (Random16() & 31) - 16;
        work->particles[i].y = Random16() & 63;
        work->particles[i].variant = -(Random16() & 15);
    }
    work->transfer_mode = 2;
    work->transfer_value = 50;
    AudioCommand_PlayFar(145);
    cx = 60;
    cy = 112;

    for (frame = 0; frame != 192; frame++) {
        s32 d;
        if (frame == 20) {
            Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetB, work, 1, 0);
        }
        d = frame - 80;
        if ((u32)d <= 59 && !(frame & 7)) {
            AudioCommand_PlayFar(134);
        }
        if (frame == 140) {
            BattleEventRuntime_BeginPhaseFar(134);
        }
        for (j = 0; j != 1; j++) {
            s32 start = j * 128 + 8;
            if (frame >= start && frame < j * 128 + 17) {
                if (frame >= j * 128 + 9 && frame < j * 128 + 12) {
                    blit[0](dst, work->sheet, 36, 0, 48, 112);
                }
                if (frame >= j * 128 + 12 && frame < start + 8) {
                    blit[0](dst, work->sheet + 0x1500, 36, 0, 48, 112);
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
                    work->shake_frames = 8;
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
                    Iwram_FillWords(dst, 0x4000, 0x10101010);
                }
            }
            for (i = 0; i != 30; i++) {
                if (frame >= i * 2 + 80) {
                    if (frame < i * 2 + 82) {
                        s32 rx = Random16() & 3;
                        s32 ry = Random16() & 3;
                        BattleEffect_LoadWork(47, 7, 7, 3, 2);
                        SLOT(47)(dst, work->sheet + i % 3 * 0x1440, rx - 3, ry + 32, 72, 72);
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
                        work->shake_frames = 8;
                        for (j = 0; j != work->effect->count; j++) {
                            ObjectGroup_UpdateMembers(work->effect->actors[j],
                                Mode12_Animations[Random16() & 3], 5, j, 4);
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
            SLOT(47)(dst, smoke, ox + 60, oy - 24, 12, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            SLOT(47)(dst, smoke, sx, oy - 24, 12, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 11, 2);
            SLOT(47)(dst, smoke, ox + 60, oy, 12, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 15, 2);
            SLOT(47)(dst, smoke, sx, oy, 12, 24);
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
            DrawRectangle draw = SLOT(47);
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
                        draw(dst, aux + ParticleStreams_CellOffsets[s - 1], x - s / 2, y - s, s, s * 2);
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
            BattleFx_PlaceFormationObjects(0, cam_x, cam_y);
        }
        if ((u32)(frame - 16) <= 127) {
            s32 base = cam_x >> 17;
            for (i = 0; i != 3; i++) {
                s32 k = i & 3;
                s32 angle = Random16() & 0xffff;
                s32 x = ((Trig_Sin(angle) << 3) >> 16) + base - (BattleFx_GlintCellWidths[k] >> 1);
                s32 y = ((Trig_Cos(angle) * 40) >> 16) - (BattleFx_GlintCellHeights[k] >> 1);
                BattleEffect_LoadWork(47, 7, 7, 3 | Mode12_SmokeFlips[Random16() & 3], 2);
                SLOT(47)(dst, Ram_MapCellBuffer + BattleFx_GlintCellOffsets[k], x, y + 56,
                    BattleFx_GlintCellWidths[k], BattleFx_GlintCellHeights[k]);
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
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleEffect_RunImpactBurst(0, cam_x, cam_y);
    }
    for (i = 0; i != 9; i++) {
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
    }
    BattleFx_EndCanvasLayer();
}
