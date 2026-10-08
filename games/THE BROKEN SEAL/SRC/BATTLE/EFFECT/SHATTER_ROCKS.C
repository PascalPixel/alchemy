#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "CANVAS.H"
#include "MEM.H"
#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "SYSTEM.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "B5_CONTEXT.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "BTLDISP.H"
#include "RESOURCE.H"
#include "PROJECT.H"
#include "FIXED_MATH.H"
#include "IWRAM_CALL.H"

#define Camera_ApplyShake ((void (*)(s32,s32))Camera_ApplyShake)


void AudioCommand_PlayFar(s32 cue);

#define HI(v) (((s16 *)&(v))[1])

extern u8 gWorkSlot[];

extern s16 Mode12_Points[][2];
extern u8 Mode12_Animations[];
extern u8 Mode12_SmokeFlips[];

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

static __inline__ s32 DivQ16(s32 divisor, s32 value)
{
    /* FAKEMATCH: needed to match: a direct call keeps 0x1fe0000 in r5 for the camera call */
    return Iwram_RatioMulQ14(divisor, value);
}

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
    s32 i;
    s32 j;
    /* FAKEMATCH: global allocation ranks n (60 refs over 195 insns) above j (95 over 534) and gives n r8; the native has j in r8 and n in sl. */
    register s32 n asm("sl");
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
        u8 *src;
        for (j = 0, src = aux; j != 0x302; j++) {
            s32 v = *src++;

            if (v > 64 - i * 7) {
                v = 64 - i * 7;
            }
            if (v < 0) {
                v = 0;
            }
            (work->sheet + 10000)[offset + j] = v;
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
        Camera_StoreSceneParameters(0x1fe0000, DivQ16(0x1fe0000, 0xc000), 0x7fff0000);
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
                scale[0] = frame * 0x1800 - 0xd9400;
                scale[1] = scale[0];
                scale[2] = scale[0];
                size = 5;
            } else {
                scale[0] = 0x10000 - a * 3 * 0x400;
                scale[1] = scale[0];
                scale[2] = scale[0];
            }
            SceneTransform_ApplyScale(scale);
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
                vec[0] = (Mode12_Points[i][0] - 96) << 16;
                vec[1] = 0;
                vec[2] = (Mode12_Points[i][1] - 96) << 16;
                EffectPosition_ApplyBaseAndYOffset(vec, (struct EffectPosition *)&pts[i]);
                pts[i].x = (pts[i].x >> 17) + 64;
                pts[i].y = (pts[i].y >> 16) + 60;
            }
            for (i = 0; i != 3; i++) {
                n = count * 2 - i * 48;
                n -= 256;
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
                s32 s = size + 1;
                vec[0] = Trig_Sin(i << 9) * 96;
                vec[2] = -(Trig_Cos(i << 9) * 96);
                EffectPosition_ApplyBaseAndYOffset(vec, (struct EffectPosition *)&pts[0]);
                pts[0].x = (pts[0].x >> 17) + 64;
                pts[0].y = (pts[0].y >> 16) + 60;
                blit47(dst, work->sheet + (lvl * 0x302 + ParticleStreams_CellOffsets[s - 1]) + rings,
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
                s32 k = 32;
                /* FAKEMATCH: loop.c matches this 32 with the smoke calls' 32 (life 19, savings 2: 12 * 2 * 19 = 456 >= 450 insns) and hoists it out of the frame loop, so it loses its register and later reloads rotate; the native builds it in this loop's preheader. */
                asm("" : "+r"(k));
                blit[0](dst, ramp + w, w + 48, i + 16, k - w * 2, 1);
            }
            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            SLOT(47)(dst, work, 32, (frame - 151) * 20 - 12, 32, 96);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            SLOT(47)(dst, work, 64, (frame - 151) * 20 - 12, 32, 96);
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
                    blit[0](dst, work->sheet, 36, 0, 48, cy);
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
            for (i = 0; i != 6; i++) {
                if (frame == i * 8 + 80) {
                    Iwram_FillWords(dst, 0x4000, 0x10101010);
                }
            }
            for (i = 0; i != 30; i++) {
                if (frame >= i * 2 + 80) {
                    if (frame < i * 2 + 82) {
                        s32 rx = (Random16() & 3) - 3;
                        s32 ry = (Random16() & 3) + 32;
                        BattleEffect_LoadWork(47, 7, 7, 3, 2);
                        SLOT(47)(dst, work->sheet + i % 3 * 0x1440, rx, ry, 72, 72);
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
            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            SLOT(47)(dst, work->sheet + 0x6000, ox + 60, oy - 24, 12, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            SLOT(47)(dst, work->sheet + 0x6000, sx, oy - 24, 12, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 11, 2);
            SLOT(47)(dst, work->sheet + 0x6000, ox + 60, oy, 12, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 15, 2);
            SLOT(47)(dst, work->sheet + 0x6000, sx, oy, 12, 24);
            Runtime_ReleaseHeapBlock(47);
            j = 0;
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
                    if (++j == 4) {
                        break;
                    }
                }
            }
        }

        BattleEffect_LoadWork(47, 7, 7, 15, 2);
        {
            DrawRectangle draw = SLOT(47);
            struct EffectStep *q;
            for (i = 0, q = PARTICLES; i != 910; i++, q++) {
                if (q->variant > 0) {
                    q->variant--;
                    EffectStep_AdvanceWithGravity2D(q, 60, 0);
                    if (q->y > 0x780000) {
                        q->velocity_y = -q->velocity_y / 2;
                    } else if ((u32)q->x <= 0x7effff && q->y >= 0) {
                        s32 x = q->x >> 16;
                        s32 y = q->y >> 16;
                        s32 s = q->variant / 16 + 1;
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
            s32 base;
            for (i = 0, base = cam_x >> 17; i != 3; i++) {
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

void BattleFx_RunShatteringRocks(struct BattleEffectArgument *object)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangleFn blit;
    s32 rows;
    struct EffectPosition first;
    struct EffectPosition final;
    struct EffectStep *rock;
    struct EffectStep *shard;
    s32 frame;
    s32 i;
    s32 j;
    s32 cell;
    u32 width;
    u32 height;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = object;
    BattleFx_BeginCanvasLayer(1);
    REG_BG2PA = 0x100;
    REG_BLDCNT = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 1);
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    blit = heap_cache[7];
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &first);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[work->effect->count - 1], &final);
    first.x += (final.x - first.x) / 2;
    REG_BG2X = (64 - first.x) << 8;
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    rows = ShatterRocks_RockCounts[work->effect->variant];
    for (i = 0; i != rows; i++) {
        rock = &work->particles[i];
        rock->y = -0x400000;
        rock->velocity_y = 0;
    }
    for (i = 0; i != rows; i++) {
        for (j = 0; j != 21; j++) {
            struct EffectStep *seed = &((struct EffectStep *)Ram_MapCellBuffer)[i * 21 + j];

            seed->x = (ShatterRocks_ShardOffsets[j * 2] + ShatterRocks_RockX[i]) << 16;
            seed->y = ShatterRocks_ShardOffsets[j * 2 + 1] << 16;
            seed->velocity_x = ((Random16() % 96) - 48) << 10;
            seed->velocity_y = -((Random16() & 127) + 32) << 11;
            seed->z = 32;
            seed->variant = 0;
        }
    }
    for (frame = 0; frame != ShatterRocks_DropFrames[rows - 1] + 80; frame++) {
        if (frame == ShatterRocks_DropFrames[rows - 1] + 48)
            BattleEventRuntime_BeginPhaseFar(132);
        for (i = 0; i != rows; i++) {
            rock = &work->particles[i];
            if (frame == ShatterRocks_DropFrames[i] + 18) {
                AudioCommand_PlayFar(134);
                work->shake_frames = 4;
            }
            if (frame >= ShatterRocks_DropFrames[i] + 18) {
                for (j = 0; j != 21; j++) {
                    shard = &((struct EffectStep *)Ram_MapCellBuffer)[i * 21 + j];
                    cell = (j % 5) * 3 + shard->variant / 96 % 3;
                    blit(canvas, work->sheet + ShatterRocks_ShardCells[cell] + 0x83c,
                        HI(shard->x) - ((width = ShatterRocks_ShardWidths[cell]) >> 1),
                        HI(shard->y) - ((height = ShatterRocks_ShardHeights[cell]) >> 1),
                        width, height);
                    EffectStep_AdvanceWithGravity2D(shard, 64, 0x4000);
                    shard->variant += shard->z;
                    if (shard->z > 1 && (frame & 1))
                        shard->z--;
                }
            }
            if (frame < ShatterRocks_DropFrames[i] + 18) {
                if (frame >= ShatterRocks_DropFrames[i])
                    blit(canvas, work, ShatterRocks_RockX[i] + 47, HI(rock->y), 34, 62);
                rock->y += rock->velocity_y;
                if (frame > ShatterRocks_DropFrames[i])
                    rock->velocity_y += 0x10000;
                if (rock->y > 0x320000)
                    rock->y = 0x320000;
            }
            if (frame == ShatterRocks_DropFrames[i] + 18) {
                for (j = 0; j != work->effect->count; j++)
                    ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 8);
            }
        }
        Camera_ApplyShake(2, 4);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
