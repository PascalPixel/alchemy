#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MEM.H"
#include "HEAP_STATE.H"
#include "CANVAS.H"
#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "RESOURCE_IDS.H"
#include "SYSTEM.H"
#include "IO_REG.H"
#include "FIXED_MATH.H"
#include "RESOURCE.H"
#include "PROJECT.H"
#include "DMA.H"
#include "BTLDISP.H"
#include "IO_WRITE_QUEUE.H"
#include "MAP_SCROLL.H"
#include "ANIMSPR.H"

#define Camera_ApplyShake ((void (*)(s32,s32))Camera_ApplyShake)

#define gBattleFxWork ((void *(*))&gBattleFxWork)


#define HI(v) (((s16 *)&(v))[1])

#define DUST ((struct EffectStep *)Ram_MapCellBuffer)
#define SPARK ((struct EffectStep *)(Ram_MapCellBuffer + 684 * sizeof(struct EffectStep)))


void AudioCommand_PlayFar(s32 cue);

void BattleFx_RunFallingSword(struct BattleEffectArgument *effect)
{
    void **heap;
    void **p;
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangle routine[2];
    u8 *sheet;
    struct EffectPosition pos;
    s32 frame;
    s32 half;
    s32 step;
    s32 origin;
    s32 i;

    heap = (void **)gBattleFxWork;
    p = heap;
    work = *p++;
    canvas = *p;
    sheet = heap[2];

    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    REG_BLDALPHA = 0x1010;
    EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &pos);
    half = pos.x / 2;

    BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
    routine[0] = heap[7];
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 1);
    routine[1] = heap[8];

    Resource_LoadAndDecompress((s32)&ResourceId_SwordSheet, work->sheet + 20000, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet + (221 << 4), 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);

    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    i = 0;
    do {
        struct EffectStep *flash = &work->particles[i];

        s32 scale = i * 2;
        s32 ang = Random16() & 0xFFFF;

        flash->x = Trig_Sin(ang) * scale;
        flash->y = -(Trig_Cos(ang) * scale);
        flash->variant = i / 2 + 25;
        i++;
    } while (i != 32);

    i = 0;
    do {
        DUST[i].variant = -1;
        i++;
    } while (i != 171 << 2);

    i = 0;
    origin = half << 16;
    do {
        struct EffectStep *spark = &SPARK[i];

        s32 mag;
        s32 ang;

        /* FAKEMATCH: needed to match: the mask is loaded into the magnitude before the random word is taken in, as the native code does */
        mag = 0x1FF;
        mag &= Random16();
        ang = Random16() & 0xFFFF;
        spark->x = origin;
        spark->y = 176 << 15;
        spark->velocity_x = (Trig_Sin(ang) * (mag + 32)) >> 5;
        spark->velocity_y = -(Trig_Cos(ang) * (mag + 32)) >> 6;
        spark->variant = (Random16() & 7) + 32;
        i++;
    } while (i != 170 << 1);

    frame = 0;
    do {
        if (frame >= 25 && frame <= 47) {
            BattleFx_StepPaletteToResource((s32)&ResourceId_BlastSheet);
        }
        if (frame > 56) {
            BattleFx_StepPaletteToResource((s32)&ResourceId_LightningBoltSheet);
        }

        if (frame == 8) {
            work->shake_frames = frame;
        }
        if (frame == 48) {
            work->shake_frames = 8;
        }
        if (frame == 60) {
            work->shake_frames = 16;
        }

        if (frame == 4) {
            AudioCommand_PlayFar(212);
        }
        if (frame == 32) {
            AudioCommand_PlayFar(164);
        }
        if (frame == 60) {
            AudioCommand_PlayFar(145);
            BattleEventRuntime_BeginPhaseFar(134);
        }

        if (frame > 55) {
            i = 0;
            do {
                struct EffectStep *flash = &work->particles[i];

                s32 x;
                s32 life;
                s32 y;

                y = HI(flash->y);
                x = HI(flash->x) + half;
                life = flash->variant;
                if (life >= 0 && life <= 17) {
                    routine[0](canvas,
                        (u8 *)work
                            + (FallingSword_FlashCells[life / 3] << 11)
                            + (221 << 4),
                        x - 16, y + 48, 32, 64);
                }
                life = flash->variant;
                flash->variant = life > 0 ? life - 1 : -1;
                i++;
            } while (i != 16);
        }

        if (frame == 28) {
            i = 0;
            do {
                struct EffectStep *dust = &DUST[i];

                if (dust->variant == -1) {
                    s32 mag;
                    s32 ang;

                    mag = Random16() & 63;
                    ang = Random16() & 0xFFFF;
                    dust->x = ((Trig_Sin(ang) * mag) >> 3) + origin;
                    dust->y = ((Trig_Cos(ang) * mag) >> 2) + (192 << 15);
                    dust->velocity_x = ((Random16() & 63) - 32) << 14;
                    dust->velocity_y = (-(Random16() & 63) - 8) << 13;
                    dust->variant = 0;
                }
                i++;
            } while (i != 128 << 1);
        }

        step = frame - 32;
        if (step >= 0 && step <= 31) {
            s32 cnt;

            cnt = 0;
            i = 0;
            do {
                struct EffectStep *dust = &DUST[i];

                if (dust->variant == -1) {
                    s32 mag;
                    s32 ang;

                    mag = Random16() & 63;
                    ang = Random16() & 0xFFFF;
                    dust->x = ((Trig_Sin(ang) * mag) >> 3) + origin;
                    dust->y = ((Trig_Cos(ang) * mag) >> 2) + (192 << 15);
                    dust->velocity_x = ((Random16() & 63) - 32) << 14;
                    dust->velocity_y = (-(Random16() & 63) - 8) << 13;
                    dust->variant = 0;
                    cnt++;
                    if (cnt == 16) {
                        break;
                    }
                }
                i++;
            } while (i != 171 << 2);
        }

        if (step >= 0 && step <= 31) {
            s32 phase = frame * 16;
            s32 x = half - 17;
            s32 width = 34;
            DrawRectangle blit = routine[0];
            s32 high = 104;
            s32 size = (phase - 256) % high;

            blit(canvas, work, x, 4 - size, width, high);
            blit(canvas, work, x, 108 - size, width, size);
        }

        if (frame <= 71) {
            i = 0;
            do {
                struct EffectStep *dust = &DUST[i];

                if (dust->variant >= 0) {
                    s32 size;

                    size = i % 3 + 2;
                    if (dust->velocity_y > 0) {
                        size += 2;
                    }
                    if (frame > 68 && size <= 5) {
                        size = 6;
                    }
                    if (frame > 70 && size <= 6) {
                        size = 7;
                    }
                    if (frame > 72 && size <= 7) {
                        size = 8;
                    }
                    if (frame > 74 && size <= 8) {
                        size = 9;
                    }
                    if (frame > 76) {
                        size = 10;
                    }
                    routine[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
                        HI(dust->x) - size / 2,
                        HI(dust->y) - size,
                        size, size * 2);

                    dust->x += dust->velocity_x;
                    dust->y += dust->velocity_y;
                    if (frame > 80) {
                        dust->velocity_y += -32768;
                    } else {
                        dust->velocity_y += FallingSword_DustGravity[i & 3];
                    }
                    dust->velocity_x = dust->velocity_x * 62 / 64;
                    dust->velocity_y = dust->velocity_y * 62 / 64;
                    dust->variant++;
                    if (dust->velocity_y > 0
                        && HI(dust->y) > 108) {
                        dust->variant = -1;
                    }
                }
                i++;
            } while (i != 171 << 1);
        }

        if (frame <= 95) {
            s32 x;
            s32 high;
            s32 top;

            x = half - 18;
            high = 120;
            if (frame > 60) {
                top = frame * 8 - 450;
            } else if (frame > 32) {
                top = step / 2 + 16;
            } else if (frame <= 9) {
                top = frame * 16 - 128;
            } else {
                top = 16;
            }
            if (top + 120 > 108) {
                high = high - top - 12;
            }
            if (high > 0) {
                routine[1](canvas, work->sheet + 20000, x, top, 36, high);
            }
        }

        if (frame > 59) {
            i = 0;
            do {
                struct EffectStep *spark = &SPARK[i];

                if (spark->variant > 0) {
                    s32 x;
                    s32 y;
                    s32 size;

                    EffectStep_AdvanceWithGravity2D(spark, 64, 128 << 6);
                    y = spark->y;
                    spark->variant--;
                    if (y > (216 << 15)) {
                        spark->velocity_y = -spark->velocity_y / 2;
                    } else {
                        x = spark->x;
                        if ((x >= 0 && x <= 0x7effff) && y >= 0) {
                            u8 *cell;

                            size = spark->variant / 5 + 1;
                            cell = sheet + ParticleStreams_CellOffsets[size - 1];
                            x >>= 16;
                            y >>= 16;
                            x -= size / 2;
                            y -= size;
                            routine[0](canvas, cell, x, y, size, size * 2);
                        }
                    }
                }
                i++;
            } while (i != 170 << 1);
        }

        if (frame == 68) {
            for (i = 0; i != work->effect->count; i++) {
                ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 16);
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 7);
            }
        }

        if (frame == 9) {
            Iwram_FillWords(canvas, 128 << 7, 0x3F3F3F3F);
        }
        if (frame == 60) {
            Iwram_FillWords(canvas, 128 << 7, 0x3F3F3F3F);
        }

        Camera_ApplyShake(16, 16);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
        frame++;
    } while (frame != 102);

    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}


#define BattlePresentation_ConfigurePaletteFadeFar ((void (*)(s32, s32, s32))BattlePresentation_ConfigurePaletteFadeFar)

typedef struct Scale {
    s32 x;
    s32 y;
} Scale;

struct ShockwaveWork {
    u8 sheet[0x2710];
    u8 ramp[8][770];
};

extern const Scale ShockwaveScene_UnitScale;
extern const u8 ShockwaveScene_BurstHeights[];
extern const u16 ShockwaveScene_BurstCells[];
extern u16 ShockwaveScene_ShardCells[];
extern u8 ShockwaveScene_ShardWidths[];
extern u8 ShockwaveScene_ShardHeights[];

void Object_ApplyProjectedPlacementFar(void *, void *, void *, s32);

#define SPARKS (&DUST[128])

#define QUEUE_IO_WRITE(address, value, delay) { \
        struct IoWriteQueue *q; \
        u32 saved; \
        s32 count; \
 \
        q = &gIoWriteQueue; \
        do { \
            /* FAKEMATCH: a one-pass block from the IME read to its restore, \
               after q: its loop notes bound sched2 where the native's queue \
               writes are bounded (q loads ahead of the A/B branch's \
               frame = 150 store, the frame 64 block's fill stays after the \
               restore); without it 6 of 2455 rows differ. */ \
            do { \
                /* FAKEMATCH: the one-pass IME read of IO.C's and CANVAS.C's \
                   queue writes; without it 16 of 2455 rows differ. */ \
                saved = *ime; \
            } while (0); \
            *ime = (u16)ime; \
            count = q->count; \
            if (count <= 31) { \
                u32 *destination = q->entries[count]; \
                /* FAKEMATCH: the count stored through a u16 pointer, as \
                   IO_WRITE_QUEUE.H's and CANVAS.C's queue writes (the same \
                   halfword store, scheduled as the native's); stored as \
                   q->count, 8 of 2455 rows differ. */ \
                *(u16 *)&q->count = count + 1; \
                *destination++ = (value); \
                *destination++ = (address); \
                *destination = (delay); \
            } \
            *ime = saved; \
        } while (0); \
    }

void BattleEffect_RunShockwaveScene(struct BattleEffectArgument *effect)
{
    s32 anchor[3];
    s32 scene[3];
    s32 place[4];
    volatile u32 fill;
    s32 place2[4];
    Scale pair;
    Scale scale;
    void *canvas;
    struct BattleEffectWork *work;
    struct BattlePresentationWork *ctrl;
    DrawRectangle draw[2];
    s32 frame;
    u8 *ramp;
    struct BattleCamera *camera;
    s32 depth;
    s32 zoom;
    s32 near;
    s32 far;
    void **cache;
    DrawRectangle *slots;
    s32 cx;
    s32 cy;
    s32 squash;
    s32 sx;
    s32 ring;
    s32 rings;
    s32 shift;
    s32 i;
    s32 j;
    struct EffectStep *p;
    DrawRectangle blit;
    volatile u16 *ime;
    void *colors;

    cache = &gBattleFxWork[1];
    /* FAKEMATCH: a form the bytes prove: one IME pointer for the whole
       function, set at entry, as CANVAS.C's; it gets no register and reload
       rematerialises 0x04000208 at each queue write, whose IME store is the
       pointer's low half 0x208, as the native. A pointer per queue write
       differs in 397 of 2455 rows. */
    ime = (volatile u16 *)0x04000208;
    canvas = cache[0];
    work = cache[HEAP_SLOT_BATTLE_EFFECT - HEAP_SLOT_BATTLE_CANVAS];
    ctrl = cache[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE_CANVAS];
    ramp = cache[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_CANVAS];
    camera = cache[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_CANVAS];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0x2000);
    *(u16 *)0x04000020 = 0x100;
    BattlePres_ConfigureEffectDisplay();
    *(u16 *)0x05000000 = 0;
    *(u16 *)0x05000002 = 0;
    BattleEffect_LoadWork(46, 7, 7, 3, 3);
    draw[0] = (DrawRectangle)cache[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_CANVAS];
    work->transfer_mode = 0;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    BattleEffect_WipeCanvas(0, 0);
    BattleFx_SelectLivingTargets(work->effect);
    BattleFx_SpawnObjects(16, 0x17e, 1);
    gProjection.center_y = 240;
    WaitFrames(1);
    BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
    ctrl->initial_value = 1;
    gBgScroll[1].x = 0;
    BattleEffect_WipeCanvas(0, 1);
    *(u16 *)0x04000000 = 0x7741;
    *(u16 *)0x04000020 = 0x80;
    *(u16 *)0x04000052 = 0x1010;
    *(u16 *)0x04000050 = 0x3f44;
    depth = 0xffc00000;
    zoom = 0x80000;
    near = 0xffff;
    far = 0;
    /* FAKEMATCH: needed to match: the palette address set with the initial values spans the ramp loop, so reload rematerialises 0x05000000 at the copy */
    colors = (void *)0x05000000;
    Resource_LoadAndDecompress((s32)&ResourceId_PinkBurstSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_OverlayImageB, work->sheet + 0x600, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet + 0x95c, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, ramp, 0, 0);

    for (i = 0; i != 8; i++) {
        u8 *src = ramp;
        for (j = 0; j != 770; j++) {
            s32 v = *src++;
            if (v > 64 - i * 7) {
                v = 64 - i * 7;
            }
            if (v < 0) {
                v = 0;
            }
            ((struct ShockwaveWork *)work)->ramp[i][j] = v;
        }
    }

    Iwram_CopyWords(colors, Resource_GetTableEntry((s32)&ResourceId_BlueCrescentSheet), 128);

    for (i = 0; i != 128; i++) {
        DUST[i].x = (Random16() & 15) + 72;
        DUST[i].velocity_x = Random16() & 0xffff;
        DUST[i].velocity_y = Random16() & 0xffff;
        DUST[i].velocity_y = Random16() & 0xffff;
    }

    for (i = 0; i != 128; i++) {
        s32 angle = Random16() & 0xffff;
        /* FAKEMATCH: needed to match: a form the bytes prove: the amplitude is a value, not a literal: the native hoists 255 into sl and multiplies with muls */
        s32 amp = 255;
        /* FAKEMATCH: needed to match: register choice: the velocities' zero is a value set at the top of the body, which loop.c hoists into r8 as the native's */
        s32 speed = 0;
        SPARKS[i].x = (Trig_Sin(angle) * amp) >> 2;
        SPARKS[i].z = (Trig_Cos(angle) * amp) >> 2;
        SPARKS[i].y = ((Random16() & 63) + 32) << 16;
        SPARKS[i].velocity_x = speed;
        SPARKS[i].velocity_y = speed;
        SPARKS[i].variant = angle;
    }

    work->transfer_mode = 2;
    work->transfer_value = 50;
    *(u16 *)0x0400000c = 0x784;

    for (frame = 0; frame != 160 && (frame <= 4 || !(gKeysRepeat & (KEY_A | KEY_B))); frame++) {
        /* FAKEMATCH: needed to match: the frame > 143 block calls through a pointer to the slot table (base kept at sp+56) */
        slots = (DrawRectangle *)gWorkSlot;
        if (frame == 143) {
            Iwram_FillWords(canvas, 0x4000, 0x2a2a2a2a);
            AudioCommand_PlayFar(145);
        }
        if (frame == 80) {
            AudioCommand_PlayFar(142);
        }

        if (frame < 0) {
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
            for (i = 0; i != 128; i++) {
                s32 z;
                s32 size;

                EffectStep_AdvanceWithGravity3D(&SPARKS[i], 60, -0x400);
                EffectPosition_ApplyBaseAndYOffset((s32 *)&SPARKS[i], (struct EffectPosition *)scene);
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
                size = __divsi3(z, 100) + 1;
                if (near > z) {
                    near = z;
                }
                if (far < z) {
                    far = z;
                }
                draw[0](canvas, ramp + ParticleStreams_CellOffsets[size - 1], scene[0] - size / 2, scene[1] - size, size,
                    size * 2);
                if (frame == 64) {
                    SPARKS[i].velocity_x += (Trig_Sin(SPARKS[i].variant) * 255) >> 3;
                    SPARKS[i].velocity_y += (Trig_Cos(SPARKS[i].variant) * 255) >> 3;
                }
            }
        }

        if (frame == 72) {
            work->fade_frames = 24;
            work->fade_step = 0;
        }

        anchor[2] = 0;
        anchor[1] = 0;
        for (i = 0; i != 64; i++) {
            if (frame > i / 4 + 80 && DUST[i].x > 0) {
                s32 size;

                Render_ResetTransformState();
                SceneTransform_ApplyRoll(DUST[i].velocity_y);
                SceneTransform_ApplyPitch(DUST[i].velocity_x);
                SceneTransform_ApplyYaw(DUST[i].velocity_y + (frame << 9));
                anchor[0] = DUST[i].x;
                DUST[i].x -= 2;
                if (DUST[i].x < 0) {
                    DUST[i].x = 0;
                }
                EffectPosition_ApplyBaseAndYOffset(anchor, (struct EffectPosition *)scene);
                if (scene[2] < -60) {
                    scene[2] = -60;
                }
                if (scene[2] > 60) {
                    scene[2] = 60;
                }
                scene[2] += 60;
                size = __divsi3(scene[2], 20) + 2;
                scene[0] += 60;
                scene[1] += 80;
                draw[0](canvas, ramp + ParticleStreams_CellOffsets[size - 1], scene[0] - size / 2, scene[1] - size, size,
                    size * 2);
            }
        }

        pair = ShockwaveScene_UnitScale;
        place[3] = 0;
        place[1] = 0;
        for (i = 0; i != 16; i++) {
            place[0] = ((i % 4) << 21) + (152 << 15);
            place[2] = ((i / 4) << 21) + depth;
            Object_ApplyProjectedPlacementFar(work->objects[i], place, &pair, 0);
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
            for (i = 0; i != 16; i++) {
                AnimationObjects_SelectAnimationFar(work->objects[i], i + 16);
            }
        }
        if (frame == 76) {
            for (i = 0; i != 16; i++) {
                AnimationObjects_SelectAnimationFar(work->objects[i], i + 32);
            }
        }

        if (frame >= 116 && frame < 144) {
            s32 high;
            s32 cell;
            s32 n = (frame - 116) / 4;
            if (n > 6) {
                n = 6;
            }
            high = ShockwaveScene_BurstHeights[n];
            cell = ShockwaveScene_BurstCells[n] + 0x600;

            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work->sheet + cell, 60 - high, 80 - high * 2, high, high * 2);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work->sheet + cell, 60, 80 - high * 2, high, high * 2);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 11, 2);
            (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work->sheet + cell, 60 - high, 80, high, high * 2);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 15, 2);
            (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work->sheet + cell, 60, 80, high, high * 2);
            Runtime_ReleaseHeapBlock(47);

            for (i = 0; i != 6; i++) {
                s32 k = i & 3;
                s32 angle = Random16() & 0xffff;
                s32 x = ((Trig_Sin(angle) * 16) >> 16) - ShockwaveScene_ShardWidths[k] / 2;
                s32 y = ((Trig_Cos(angle) * 16) >> 16) - ShockwaveScene_ShardHeights[k] / 2;
                draw[0](canvas, work->sheet + ShockwaveScene_ShardCells[k], x + 60, y + 80, ShockwaveScene_ShardWidths[k],
                    ShockwaveScene_ShardHeights[k]);
            }
        }

        if (frame > 143) {
            s32 top = (frame << 4) - 2272;
            s32 n;
            s32 r;
            BattleEffect_LoadWork(47, 7, 7, 3, 3);
            (slots[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work, 36, top, 24, 64);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 3);
            (slots[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, work, 60, top, 24, 64);
            Runtime_ReleaseHeapBlock(47);

            n = (frame - 144) / 2;
            r = frame * 10 - 1424;
            work->transfer_value = 75;
            if (n <= 6) {
                s32 cell = n * 770 + ParticleStreams_CellOffsets[7] + 0x2710;
                /* FAKEMATCH: needed to match: the height is a value, so high / 2 is hoisted into fp */
                s32 high = 8;
                BattleEffect_LoadWork(47, 7, 7, 3, 2);
                blit = slots[HEAP_SLOT_BLITTER_ALTERNATE];
                for (i = 0; i != 128; i++) {
                    s32 angle = i << 9;
                    s32 x = ((r * Trig_Sin(angle)) >> 16) - high / 2;
                    s32 y = (r * Trig_Cos(angle)) >> 17;
                    blit(canvas, work->sheet + cell, x + 60, y + 72, high, high * 2);
                }
                Runtime_ReleaseHeapBlock(47);
            }
        }

        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Iwram_FillWords(canvas, 0x4000, 0);

    for (i = 0; i != 16; i++) {
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
    }

    for (i = 0; i != 16; i++) {
        struct AnimationObject *obj = (struct AnimationObject *)ResourceObject_CreateFar(0x186);
        work->objects[i] = obj;
        if (obj != 0) {
            obj->flags = 0;
            AnimationObjects_SelectAnimationFar(obj, __modsi3(i, 3));
            ((struct AnimationObject *)work->objects[i])->part[0].priority = 0;
        }
    }

    Runtime_ReleaseHeapBlock(46);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    Resource_LoadAndDecompress((s32)&ResourceId_BlueCrescentSheet, work->sheet + 0x4000, 1, 1);

    {
        u8 *c = ramp;
        for (i = 0; i != 770; i++, c++) {
            if (*c > 32) {
                *c += 224;
            } else if (*c != 0) {
                *c = 1;
            }
        }
    }

    work->fade_frames = 0;
    work->fade_step = 0;
    BattleBackground_LoadFar(1, (s32)&ResourceId_AerialViewBackdrop, 0);
    *(u16 *)0x04000020 = 0x100;
    *(u32 *)0x04000028 = -0x3c00;
    *(u16 *)0x0400000c = 0x784;

    fill = 0;
    Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);

    cx = 60;
    cy = 44;
    squash = 6;
    shift = 0;
    ring = 0;
    sx = 8;
    rings = 2;
    work->transfer_mode = 2;
    work->transfer_value = 75;

    fill = shift;
    Dma_Set(&fill, work, 0x85000e10, (volatile u32 *)0x040000d4);

    for (frame = 0; frame != 320; frame++) {
        if (frame == 66) {
            AudioCommand_PlayFar(145);
            AudioCommand_PlayFar(141);
        }
        if (frame == 155) {
            AudioCommand_PlayFar(162);
        }
        if (frame == 217) {
            AudioCommand_PlayFar(156);
        }
        if (frame == 280) {
            AudioCommand_PlayFar(157);
        }
        if (frame == 300) {
            BattleEventRuntime_BeginPhaseFar(145);
        }

        if (gKeysRepeat & (KEY_A | KEY_B)) {
            if (frame >= 5 && frame < 150) {
                frame = 150;
                QUEUE_IO_WRITE(0x04000020, 0x80, 0x20000);
                QUEUE_IO_WRITE(0x04000028, 0, 0x30000);
                Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet, work->sheet + 0x4000, 1, 0);
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
            Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet, work->sheet + 0x4000, 1, 0);
            Resource_LoadAndDecompress((s32)&ResourceId_OverlayImageA, work->sheet + 0x6000, 0, 0);
        }

        if (frame == 66) {
            u16 *pal = (u16 *)0x050000c0;
            for (i = 0; i != 128; i++, pal++) {
                *pal ^= 0x7fff;
            }
        }

        if (frame == 69) {
            fill = 0x3f3f3f3f;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
        }

        if (frame == 70) {
            BattlePresentation_ConfigurePaletteFadeFar(1, (s32)&ResourceId_AerialViewBackdrop, 7);
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
            BattleBackground_LoadFar(1, (s32)&ResourceId_BeachBackdrop, 0);
        }

        if (frame == 214) {
            fill = 0;
            Dma_Set(&fill, work, 0x85000e10, (volatile u32 *)0x040000d4);
            fill = 0x3f3f3f3f;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
            BattleBackground_LoadFar(1, (s32)&ResourceId_ForestBackdrop, 0);
            for (i = 0; i != 16; i++) {
                p = &work->particles[i];
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
            ctrl->initial_value = 0;
            *(u16 *)0x04000052 = 0x1010;
            for (i = 0; i != 16; i++) {
                p = &work->particles[i];
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
            /* FAKEMATCH: palette RAM through a volatile pointer: the native keeps red's
               mask 31 in the loop rather than hoisting it; a plain pointer differs
               in 52 of 2455 rows. */
            volatile u16 *pal = (volatile u16 *)0x05000002;
            for (i = 0; i != 63; i++, pal++) {
                s32 c = *pal;
                s32 b = ((u16)c >> 10) & 31;
                s32 g = ((u16)c >> 5) & 31;
                s32 r = c & 31;
                b++;
                g++;
                r++;
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
            u8 *sheet;
            s32 top = frame - 4;
            if ((frame & 7) < 4) {
                sheet = work->sheet + 0x4000;
            } else {
                sheet = work->sheet + 0x4240;
            }
            BattleEffect_LoadWork(47, 7, 7, 3, 2);
            (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, sheet, 36, top - 24, 24, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 7, 2);
            (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, sheet, 59, top - 24, 24, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 11, 2);
            (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, sheet, 36, top - 1, 24, 24);
            Runtime_ReleaseHeapBlock(47);
            BattleEffect_LoadWork(47, 7, 7, 15, 2);
            (((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE])(canvas, sheet, 59, top - 1, 24, 24);
            Runtime_ReleaseHeapBlock(47);
        }

        if (frame >= 64 && frame < 66) {
            draw[0](canvas, work->sheet + 0x6000, 52, 51, 16, 17);
        }
        if (frame >= 66 && frame < 68) {
            draw[0](canvas, work->sheet + 0x6110, 48, 40, 24, 41);
        }
        if (frame >= 68 && frame < 76) {
            s32 d = 76 - frame;
            draw[0](canvas, work->sheet + 0x64e8, 49 - d / 2, 38 - d, 22, 44);
            draw[0](canvas, work->sheet + 0x64e8, 49 - d / 2, d + 38, 22, 44);
            draw[0](canvas, work->sheet + 0x64e8, d / 2 + 49, 38 - d, 22, 44);
            draw[0](canvas, work->sheet + 0x64e8, d / 2 + 49, d + 38, 22, 44);
        }
        if (frame >= 78 && frame < 80) {
            draw[0](canvas, work->sheet + 0x6000, 52, 51, 16, 17);
        }
        if (frame >= 80 && frame < 82) {
            draw[0](canvas, work->sheet + 0x6110, 48, 40, 24, 41);
        }
        if (frame >= 82 && frame < 86) {
            s32 d = frame * 2 - 164;
            draw[0](canvas, work->sheet + 0x64e8, 49 - d / 4, 38 - d, 22, 44);
            draw[0](canvas, work->sheet + 0x64e8, 49 - d / 4, d + 38, 22, 44);
            draw[0](canvas, work->sheet + 0x64e8, d / 4 + 49, 38 - d, 22, 44);
            draw[0](canvas, work->sheet + 0x64e8, d / 4 + 49, d + 38, 22, 44);
        }

        if (frame >= 72 && frame < 76) {
            s32 r2 = (frame << 4) - 1136;
            /* FAKEMATCH: needed to match: the size is a value, so 5 and 20 are hoisted into fp and sl */
            s32 size = 10;
            for (i = 0; i != 256; i++) {
                s32 angle = i << 8;
                s32 x = ((Trig_Sin(angle) * r2) >> 16) - (size >> 1);
                s32 y = (Trig_Cos(angle) * r2) >> 17;
                draw[0](canvas, ramp + ParticleStreams_CellOffsets[size - 1], x + 60, y + 50, size, size * 2);
            }
        }

        if (frame > 85) {
            if (frame <= 213) {
                for (i = 0; i != rings; i++, ring++) {
                    s32 radius = ring;
                    s32 slope = 0;
                    s32 err = ring;
                    while (radius >= slope) {
                        s32 half;
                        s32 half2;
                        s32 left;
                        s32 right;
                        s32 top;
                        s32 bottom;
                        s32 dx;

                        half = radius / 2;
                        dx = (half * sx) / 8;
                        right = cx + dx;
                        /* FAKEMATCH: needed to match: a dead branch restores the label CSE and reload saw between the edges */
                        if (dx > cx) {
                            left = 0;
                        }
                        left = cx - dx;
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

                        half2 = slope / 2;
                        dx = (half2 * sx) / 8;
                        right = cx + dx;
                        /* FAKEMATCH: needed to match: a dead branch restores the label CSE and reload saw between the edges */
                        if (dx > cx) {
                            left = 0;
                        }
                        left = cx - dx;
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
                        right = cx + half2 + 1;
                        left = cx - half2 + 1;
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
        }

        if (frame >= 86 && frame < 150) {
            s32 angle = -0x4000;
            for (i = 0; i != 9; i++, angle += 0x1000) {
                s32 x = ((frame - 86) * Trig_Sin(angle)) >> 16;
                s32 y = ((frame - 70) * Trig_Cos(angle)) >> 16;
                draw[0](canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000, x + 44, y + 17, 32, 54);
            }
        }

        BattleEffect_LoadWork(47, 7, 7, 3, 3);
        draw[1] = ((DrawRectangle *)gWorkSlot)[HEAP_SLOT_BLITTER_ALTERNATE];
        if (frame > 85) {
            draw[1](canvas, work, 0, 0, 120, 120);
        }

        if (frame > 279) {
            shift -= 8;
        }

        if (frame > 319) {
            for (i = 0; i != work->effect->count; i++) {
                ObjectGroup_UpdateMembers(work->effect->actors[i], 14, 5, -1, 0);
            }
        }

        if (frame == 238) {
            fill = 0x3f3f3f3f;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
        }

        if (frame >= 214 && frame < 280) {
            gBgScroll[1].x += 8;
        }

        if (frame >= 246 && frame < 280) {
            shift += 8;
            work->transfer_mode = 0;
            work->transfer_value = 75;
        }

        if (frame > 213) {
            place2[3] = 0;
            place2[1] = 0;
            for (i = 0; i != 16; i++) {
                p = &work->particles[i];
                if (p->variant != 0) {
                    scale.x = p->z + (p->y >> 7);
                    if (scale.x < 0x800) {
                        scale.x = 0x800;
                    }
                    scale.y = scale.x;
                    place2[0] = p->x;
                    place2[1] = 0xff0000;
                    place2[2] = p->y + 0xff0000;
                    Object_ApplyProjectedPlacementFar(work->objects[i], place2, &scale, 0);
                    p->y -= p->velocity_y;
                    if (p->y < 0x100000) {
                        if (frame <= 279) {
                            p->x = ((Random16() & 63) + shift + 32) << 16;
                        } else {
                            p->x = ((Random16() & 63) + shift - 32) << 16;
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
                for (i = 0; i != 8; i++) {
                    s32 x = (i & 1) * 20 - (Random16() & 15) + (i / 2) * 4 + shift / 2 - 16;
                    s32 y = (i / 2) * 32 - (Random16() & 15);
                    draw[1](canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000, x, y, 32, 54);
                }
            } else {
                for (i = 0; i != 8; i++) {
                    s32 x = (i & 1) * 20 + (Random16() & 15) - (i / 2) * 4 + shift / 2 - 16;
                    s32 y = (i / 2) * 32 - (Random16() & 15);
                    draw[1](canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000, x, y, 32, 54);
                }
            }
        }

        Runtime_ReleaseHeapBlock(47);

        if (frame > 63) {
            /* FAKEMATCH: needed to match: forced temporary: the shake value through an s32 keeps the AND constant an immediate and the instruction count native's */
            s32 shake = (Random16() & 3) + 30;

            gBgScroll[1].y = shake;
        }

        work->transfer_pending = 1;
        WaitFrames(1);
    }

    for (i = 0; i != 16; i++) {
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
    }

    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
