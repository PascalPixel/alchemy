#include "RUNTIME_MEM.H"
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

extern void *gBattleFxWork[];

/* The whole-pixel half of a 16.16 coordinate. */
#define HI(v) (((s16 *)&(v))[1])

/* Two record tables share the map cell buffer: 684 dust records, then 340
   sparks. */
#define DUST ((struct EffectStep *)Ram_MapCellBuffer)
#define SPARK ((struct EffectStep *)(Ram_MapCellBuffer + 684 * sizeof(struct EffectStep)))

extern u16 ParticleStreams_CellOffsets[];
extern const u8 FallingSword_FlashCells[];
extern const s32 FallingSword_DustGravity[];

s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode, s32 slot, s32 delay);
void Camera_ApplyShake(s32 x, s32 y);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
void BattleFx_StepPaletteToResource(s32 resource_id);
void Audio_PlayCue(s32 cue);

/* Battle effect: a sword slides down the screen through a column of fire
   and strikes the ground, with sixteen radial flashes, a pool of 684 dust
   records and 340 sparks. */
void BattleFx_RunFallingSword(struct BattleEffectArgument *effect)
{
    void **heap;
    void **p;
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangle routine[2];
    void *sheet;
    struct EffectPosition pos;
    s32 frame;
    s32 half;
    s32 step;
    s32 origin;
    s32 i;

    /* The first two heap slots are read through a walking pointer; the
       base itself serves the sheet and the two blitters. */
    heap = (void **)gBattleFxWork;
    p = heap;
    work = *p++;
    canvas = *p;
    sheet = heap[2];

    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    *(volatile u16 *)0x04000052 = 0x1010;
    EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actors[0], &pos);
    half = pos.x / 2;

    BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
    routine[0] = heap[7];
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 1);
    routine[1] = heap[8];

    Resource_LoadAndDecompress((s32)&ResourceId_SwordSheet, (u8 *)work + 20000, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, (u8 *)work + (221 << 4), 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);

    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    /* Thirty-two flash records are seeded; sixteen are drawn. */
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

    /* Park the dust pool; the 340 records past it are seeded below. */
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

        /* FAKEMATCH: the mask is loaded into the magnitude before the
           random word is taken in, as the native code does; masking the
           call's result keeps the mask in a scratch register. */
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
            Audio_PlayCue(212);
        }
        if (frame == 32) {
            Audio_PlayCue(164);
        }
        if (frame == 60) {
            Audio_PlayCue(145);
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
                if ((u32)life <= 17) {
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

        /* Frame 28 seeds every still-parked record in the first 256. */
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

        /* Frames 32..63 scan all 684 and seed at most sixteen more each. */
        step = frame - 32;
        if ((u32)step <= 31) {
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

        /* The same guard again: the 34x104 fire column, wrapping through
           its 104 rows. */
        if ((u32)step <= 31) {
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
                    routine[0](canvas, (u8 *)sheet + ParticleStreams_CellOffsets[size - 1],
                        HI(dust->x) - size / 2,
                        HI(dust->y) - size,
                        size, size * 2);

                    /* EffectStep_AdvanceWithGravity2D open-coded with a
                       per-lane gravity and damping 62. */
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
                routine[1](canvas, (u8 *)work + 20000, x, top, 36, high);
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
                        if ((u32)x <= 0x007EFFFF && y >= 0) {
                            u8 *cell;

                            size = spark->variant / 5 + 1;
                            cell = (u8 *)sheet + ParticleStreams_CellOffsets[size - 1];
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

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}
