/* NONMATCHING: 2026-10-01 brief Wave2 CopyPalette plain-source attempt.
 * Removing this one source device changes BattleEffect_RunBurstShower.
 * First remaining difference: BattleEffect_RunBurstShower: ldr	r3, .L21+12 => lsl	r0, r0, #19 (735/735 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * This reduced draft preserves the affected function and its declarations.
 * Production retains the measured device with its FAKEMATCH reason.
 */
#include "DMA.H"
#include "BATTLE_PRESENTATION.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"

extern u8 gBattleFxWork[];

void WaitFrames(s32);

/* The resident word copy, handed to the palette copy and reached through
   the r3 bx bank. */
typedef s32 (*CopyWords)(void *, const void *, s32);

/* Heap-allocation cache: gWorkSlot[kind] holds kind's block address.
   This owner reads kinds 39 (its work block), 40, 41, 46 and 47. */
extern u8 gWorkSlot[];
void BattleFx_BeginCanvasLayer(s32);
void BattleFx_FetchRectangleBlitters(s32, u32 *);
void *Resource_GetTableEntry(s32);
void EffectPosition_ApplyAnimationAndYOffset(s32, s32 *);
void Audio_PlayCue(s32);
void BattleMotion_ApplyVariantMotionFar(s32, s32);
void BattleEventRuntime_BeginPhaseFar(s32);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
void Camera_ApplyShake(s32, s32);
void ObjectGroup_TickMemberTimers(void);
void Runtime_ReleaseHeapBlock(s32);
s32 BattleFx_EndCanvasLayer(void);
typedef struct BattleEffectArgument Efx;

/* One 28-byte record; the array starts at work + 0x7080.  x and y are
   16.16 fixed point and their integer halves are read directly. */
typedef struct Spark {
    s32 x;
    s32 y;
    s32 unk08;
    s32 dx;
    s32 dy;
    s32 unk14;
    s32 tick;
} Spark;

#define WORK_EFX ((Efx *)work->effect)
#define SHEET ((u8 *)work + 0xC56)
#define SPARKS ((Spark *)((u8 *)work + 0x7080))


extern u8 gCameraWork[];

/* Resource id the reference loads from its literal pool. */
extern const u16 ParticleStreams_CellOffsets[];
void BattleFx_BeginCanvasLayer(s32 mode);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_FetchRectangleBlitters(s32 alternate, u32 *output);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void SceneTransform_ApplyPosition(s32 *position);
void Graphics_SaveTransferWorkOnce(void);
void Graphics_RestoreTransferWork(void);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyRoll(s32 angle);

/* H-blank callback: feed the per-line BG2X/BG2Y words in the battle work
   area to the BG2 affine reference point. */
void BattleFx_ArmBg2AffineHBlankDma(void)
;

/* Six drawn arguments: destination, source cell, x, y, width, height.
   Called through the r4 bx bank, so it is an indirect call through a
   cached blitter entry rather than a fixed callee. */

/* Resource numbers are ResourceId_ rows, which the reference loads from its
   pool rather than materializing with a mov. */

/* Mode entries of the burst shower (effect series I). */
void BattleFx_RunSeriesIMode0(struct BattleEffectArgument *effect)
;

void BattleFx_RunSeriesIMode2(struct BattleEffectArgument *effect)
;

void BattleFx_RunSeriesIMode6(struct BattleEffectArgument *effect)
;

void BattleFx_RunSeriesIMode3(struct BattleEffectArgument *effect)
;

void BattleFx_RunSeriesIMode5(struct BattleEffectArgument *effect)
;

void BattleFx_RunSeriesIMode7(struct BattleEffectArgument *effect)
;

void BattleFx_RunSeriesIMode4(struct BattleEffectArgument *effect)
;

void BattleEffect_RunBurstShower(Efx *efx, s32 mode)
{
    u32 *cache;
    struct BattleEffectWork *work;
    void *dst;
    u8 *aux;
    void *blit[2];
    s32 pos[3];
    s32 seat[8][3];
    s32 aim[3];
    u16 *pal;
    s32 frame;
    s32 i;
    s32 pick;
    s32 lum;
    s32 id;
    s32 width;
    s32 height;

    cache = (u32 *)(gWorkSlot + 40 * 4);
    dst = (void *)cache[40 - 40];
    work = (struct BattleEffectWork *)cache[39 - 40];
    aux = (u8 *)cache[41 - 40];
    work->effect = efx;
    BattleFx_BeginCanvasLayer(0);
    *(s16 *)0x04000052 = 0x1010;

    if (mode == 7) {
        BattleEffect_LoadWork(46, 7, 7, 3, 2);
        blit[0] = (void *)cache[46 - 40];
        BattleEffect_LoadWork(47, 7, 7, 7, 2);
        blit[1] = (void *)cache[47 - 40];
    } else {
        BattleFx_FetchRectangleBlitters(WORK_EFX->side, (u32 *)blit);
    }

    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work, 1, 0);
    if (mode == 5) {
        Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetA, SHEET, 1, 1);
    } else if (mode == 7) {
        Resource_LoadAndDecompress((s32)&ResourceId_DemonFaceSheet, SHEET, 1, 1);
    } else {
        Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, SHEET, 1, 1);
        Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, aux, 0, 0);
        if (mode == 6) {
            for (i = 0, pal = (u16 *)0x05000000; i != 64; i++) {
                lum = i / 4;
                *pal = (u16)(((lum << 10) | (lum << 5)) | lum);
                pal++;
            }
            *(s16 *)0x04000050 = 0;
        } else {
            switch (mode) {
            case 0:
                id = (s32)&ResourceId_FlashBurstSheet;
                break;
            case 1:
                id = (s32)&ResourceId_IceBlockSheet;
                break;
            case 2:
                id = (s32)&ResourceId_WaterSpraySheet;
                break;
            case 3:
                id = (s32)&ResourceId_PinkPalette;
                break;
            case 4:
            default:
                id = (s32)&ResourceId_MarsDjinnSheet;
                break;
            }
            Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry(id), 128);
        }
    }

    if (mode == 7) {
        work->transfer_mode = 2;
        work->transfer_value = 50;
    } else {
        work->transfer_mode = 2;
        work->transfer_value = 75;
    }
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (i = 0; i != 64; i++) {
        SPARKS[i].tick = -1;
    }

    EffectPosition_ApplyAnimationAndYOffset(WORK_EFX->actor, pos);
    if (mode == 3) {
        pos[1] -= 16;
    }
    if (mode == 4) {
        if (WORK_EFX->side == 1) {
            pos[0] += 28;
        } else {
            pos[0] -= 28;
        }
    }
    if (mode == 7) {
        if (WORK_EFX->side == 1) {
            pos[0] += 16;
        } else {
            pos[0] -= 16;
        }
    }
    if (mode == 5) {
        pos[0] = pos[0] / 3;
        *(s16 *)0x04000020 = 85;
    }

    i = 0;
    while (i != WORK_EFX->count) {
        EffectPosition_ApplyStepAndYOffset(WORK_EFX->actors[i], (struct EffectPosition *)seat[i]);
        i += 1;
    }

    frame = 0;
    do {
        pick = frame % WORK_EFX->count;
        if (frame == 4) {
            Audio_PlayCue(0x88);
        }
        if (mode != 6) {
            if (frame == 24) {
                BattleEventRuntime_BeginPhaseFar(0x86);
            }
        } else {
            if (frame == 60) {
                BattleEventRuntime_BeginPhaseFar(0x86);
            }
        }

        if (mode == 5) {
            /* One large cell of a nine-cell sheet; the sequence advances
               every third frame and wraps every ninth. */
            if (WORK_EFX->side == 1) {
                ((DrawRectangle)blit[0])(dst,
                    SHEET
                        + ((frame / 3 % 3 * 9)
                            << 9),
                    pos[0] - 2, pos[1] - 32, 72, 62);
            } else {
                ((DrawRectangle)blit[0])(dst,
                    SHEET
                        + ((frame / 3 % 3 * 9)
                            << 9),
                    pos[0] - 70, pos[1] - 32, 72, 62);
            }
        } else {
            aim[0] = (seat[pick][0] + (Random16() & 31)) - 16;
            aim[1] = (seat[pick][1] + (Random16() & 63)) - 16;
            if (frame <= 47) {
                /* The original uses half the vertical fixed-point scale for x. */
                SPARKS[frame].x = pos[0] << 15;
                SPARKS[frame].y = pos[1] << 16;
                SPARKS[frame].dx = (aim[0] - pos[0]) << 11;
                SPARKS[frame].dy = (aim[1] - pos[1]) << 11;
                SPARKS[frame].tick = 0;
            }
        }

        i = 0;
        width = 32;
        height = 64;
        do {
            Spark *spark = &SPARKS[i];
            if (spark->tick >= 0) {
                if (mode == 7) {
                    if (spark->tick > 5) {
                        ((DrawRectangle)blit[WORK_EFX->side])(dst, SHEET,
                            ((s16 *)&spark->x)[1] - 16,
                            ((s16 *)&spark->y)[1] - 32, width, height);
                    }
                } else if (mode == 4) {
                    if (spark->tick > 5) {
                        ((DrawRectangle)blit[0])(dst,
                            SHEET + ((spark->tick / 4) << 11),
                            ((s16 *)&spark->x)[1] - 16,
                            ((s16 *)&spark->y)[1] - 32, width, height);
                    }
                } else if (mode != 5) {
                    if (spark->tick > 1) {
                        ((DrawRectangle)blit[0])(dst,
                            SHEET + ((spark->tick / 4) << 11),
                            ((s16 *)&spark->x)[1] - 16,
                            ((s16 *)&spark->y)[1] - 32, width, height);
                    }
                }
                spark->x += spark->dx;
                spark->y += spark->dy;
                spark->tick += 1;
                if (spark->tick == 24) {
                    spark->tick = -1;
                }
            }
            i += 1;
        } while (i != 64);

        if (mode == 5) {
            i = 0;
            while (i != WORK_EFX->count) {
                if ((frame >= (i * 4) + 2) && ((frame & 7) == i)) {
                    *(s32 *)((u8 *)work + 0x77A8) = 8;
                    ObjectGroup_UpdateMembers(WORK_EFX->actors[i], 7, 5, i, 4);
                }
                i += 1;
            }
        } else {
            i = 0;
            while (i != WORK_EFX->count) {
                if ((frame >= (i * 4) + 16) && ((frame & 7) == i)) {
                    *(s32 *)((u8 *)work + 0x77A8) = 8;
                    if (mode == 6) {
                        ObjectGroup_UpdateMembers(WORK_EFX->actors[i], 14, 5, i, 4);
                    } else {
                        ObjectGroup_UpdateMembers(WORK_EFX->actors[i], 7, 5, i, 4);
                    }
                    BattleMotion_ApplyVariantMotionFar(WORK_EFX->actors[i], 4);
                }
                i += 1;
            }
        }

        Camera_ApplyShake(4, 4);
        if (mode != 6) {
            ObjectGroup_TickMemberTimers();
        }
        work->transfer_pending = 1;
        WaitFrames(1);
        frame += 1;
    } while (frame != 64);

    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

/*
 * Battle effect: a cloud of 256 stars scattered through a cube around the
 * scene centre (only the first 64 are shown). For 160 frames star i appears
 * from frame i / 4, turned by a yaw, pitch or roll (or both of the last two,
 * by i & 3) that grows with the frame, and is drawn from one of four sheets
 * at a size of 1..9 pixels by depth. Thirty frames after it appears each
 * star is pulled back towards the centre.
 */
void BattleFx_RunSwirlingStars(void *object)
;
