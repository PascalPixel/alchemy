#include "TYPES.H"
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
#include "RESOURCE.H"
#include "TRANSFORM.H"
#include "DMA.H"
#include "BTLDISP.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"
#include "MOTOBJ.H"
#include "IO_REG.H"

#define EffectPosition_ApplyAnimationAndYOffset ((void (*)(s32,s32*))EffectPosition_ApplyAnimationAndYOffset)
#define Camera_ApplyShake ((void (*)(s32,s32))Camera_ApplyShake)
#define SceneTransform_ApplyPosition ((void (*)(s32*))SceneTransform_ApplyPosition)




typedef s32 (*CopyWords)(void *, const void *, s32);

void Audio_PlayCue(s32);
typedef struct BattleEffectArgument Efx;

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
#define SHEET (work->sheet + 0xC56)
#define SPARKS ((Spark *)work->particles)

static __inline__ void CopyPalette(CopyWords copy, void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: needed to match: a direct call shifts the palette destination before loading the routine */
    copy(destination, source, size);
}


void BattleFx_ArmBg2AffineHBlankDma(void)
{
    struct BattleEffectWork *work = *(void **)gBattleFxWork;
    volatile u16 *channel = REG_DMA0;
    channel[5] &= 0xc5ff;
    channel[5] &= 0x7fff;
    (void)channel[5];
    Dma_Set(work->bg2_x, (void *)0x04000028, 0xa6600001, (volatile u32 *)channel);
}

void BattleFx_RunSeriesIMode0(struct BattleEffectArgument *effect)
{
    BattleEffect_RunBurstShower(effect, 0);
}

void BattleFx_RunSeriesIMode2(struct BattleEffectArgument *effect)
{
    BattleEffect_RunBurstShower(effect, 2);
}

void BattleFx_RunSeriesIMode6(struct BattleEffectArgument *effect)
{
    BattleEffect_RunBurstShower(effect, 6);
}

void BattleFx_RunSeriesIMode3(struct BattleEffectArgument *effect)
{
    BattleEffect_RunBurstShower(effect, 3);
}

void BattleFx_RunSeriesIMode5(struct BattleEffectArgument *effect)
{
    BattleEffect_RunBurstShower(effect, 5);
}

void BattleFx_RunSeriesIMode7(struct BattleEffectArgument *effect)
{
    BattleEffect_RunBurstShower(effect, 7);
}

void BattleFx_RunSeriesIMode4(struct BattleEffectArgument *effect)
{
    BattleEffect_RunBurstShower(effect, 4);
}

void BattleEffect_RunBurstShower(Efx *efx, s32 mode)
{
    u32 *cache;
    struct BattleEffectWork *work;
    void *dst;
    u8 *aux;
    DrawRectangle blit[2];
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

    cache = (u32 *)&gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
    dst = (void *)cache[0];
    work = (struct BattleEffectWork *)cache[-1];
    aux = (u8 *)cache[1];
    work->effect = efx;
    BattleFx_BeginCanvasLayer(0);
    REG_BLDALPHA = 0x1010;

    if (mode == 7) {
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
        blit[0] = (DrawRectangle)cache[46 - 40];
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
        blit[1] = (DrawRectangle)cache[47 - 40];
    } else {
        BattleFx_FetchRectangleBlitters(WORK_EFX->side, blit);
    }

    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
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
                *pal = ((lum << 10) | (lum << 5)) | lum;
                pal++;
            }
            REG_BLDCNT = 0;
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
            CopyPalette(Iwram_CopyWords, (void *)0x05000000, Resource_GetTableEntry(id), 128);
        }
    }

    if (mode == 7) {
        work->transfer_mode = 2;
        work->transfer_value = 50;
    } else {
        work->transfer_mode = 2;
        work->transfer_value = 75;
    }
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

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
        REG_BG2PA = 85;
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
                    work->shake_frames = 8;
                    ObjectGroup_UpdateMembers(WORK_EFX->actors[i], 7, 5, i, 4);
                }
                i += 1;
            }
        } else {
            i = 0;
            while (i != WORK_EFX->count) {
                if ((frame >= (i * 4) + 16) && ((frame & 7) == i)) {
                    work->shake_frames = 8;
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

    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}

void BattleFx_RunSwirlingStars(void *object)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 record[3];
    struct EffectPosition pos;
    DrawRectangleFn draw[2];
    struct EffectStep *star;
    s32 i;
    s32 frame;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = object;
    BattleFx_BeginCanvasLayer(0);
    Resource_LoadAndDecompress((s32)&ResourceId_StarDotSheet, work->sheet, 1, 1);
    BattleFx_FetchRectangleBlitters(work->effect->side ^ 1, draw);
    for (i = 0; i != 256; i++) {
        star = &((struct EffectStep *)Ram_MapCellBuffer)[i];
        star->x = ((Random16() & 0xff) - 127) << 16;
        star->y = ((Random16() & 0xff) - 127) << 16;
        star->z = ((Random16() & 0xff) - 127) << 16;
        star->velocity_x = 0;
        star->velocity_y = 0;
        star->velocity_z = 0;
        star->variant = 0;
    }
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    record[0] = 0;
    record[1] = 160 << 15;
    record[2] = 0;
    for (frame = 0; frame != 160; frame++) {
        struct BattleCamera *camera;

        camera = gCameraWork;
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
        SceneTransform_ApplyPosition(record);
        star = (struct EffectStep *)Ram_MapCellBuffer;
        for (i = 0; i != 64; i++, star++) {
            if (frame > i / 4 && star->variant == 0) {
                s32 size;

                Graphics_SaveTransferWorkOnce();
                switch (i & 3) {
                case 0:
                    SceneTransform_ApplyYaw(frame * (i + 32) * 8);
                    break;
                case 1:
                    SceneTransform_ApplyPitch(-frame * (i + 32) * 8);
                    break;
                case 2:
                    SceneTransform_ApplyRoll(-frame * (i + 32) * 8);
                    break;
                case 3:
                    SceneTransform_ApplyPitch(-frame * (i + 32) * 8);
                    SceneTransform_ApplyRoll(-frame * (i + 32) * 8);
                    break;
                }
                EffectPosition_ApplyBaseAndYOffset((s32 *)star, &pos);
                pos.x >>= 1;
                Graphics_RestoreTransferWork();
                if (pos.depth < 250)
                    pos.depth = 250;
                if (pos.depth > 0x27a)
                    pos.depth = 0x27a;
                size = 9 - (pos.depth - 250) / 64;
                draw[0](canvas,
                    work->sheet + ((i & 3) * 770 + ParticleStreams_CellOffsets[size - 1]),
                    pos.x - size / 2, pos.y - size, size, size * 2);
                EffectStep_AdvanceWithGravity3D(star, 60, 0);
                if (frame > i / 4 + 30) {
                    s32 dx = -star->x >> 8;
                    s32 dy = -star->y >> 8;
                    s32 dz = -star->z >> 8;

                    star->velocity_x += dx;
                    star->velocity_y += dy;
                    star->velocity_z += dz;
                }
            }
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}

void BattleFx_RunOrbitingSparks(void)
{
    /* FAKEMATCH: needed to match: relative heap-cell loads preserve load and literal order */
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 record[3];
    struct EffectPosition center;
    struct EffectPosition pos;
    s32 frame;
    /* FAKEMATCH: needed to match: only draw[0] is used: the second word fills four bytes of the frame */
    DrawRectangleFn draw[2];
    s32 member;
    struct BattleCamera *camera;
    u8 *palette;
    s32 x_offset;
    s32 i;

    heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    camera = ((struct BattleCamera **)gWorkSlot)[HEAP_SLOT_CAMERA];
    BattleFx_BeginCanvasLayer(1);
    REG_BG2PA = 0x100;
    REG_BLDALPHA = 0x1010;
    palette = Resource_GetTableEntry((s32)&ResourceId_SkullSheet);
    Iwram_CopyWords(BG_PLTT, palette, 128);
    Resource_DecodeType01(palette + 128, work);
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    work->transfer_mode = 2;
    work->transfer_value = 75;
    draw[0] = (DrawRectangleFn)heap_cache[7];
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    if (work->effect->side == 0)
        x_offset = 0;
    else
        x_offset = -112;
    /* FAKEMATCH: needed to match: a one-pass loop is a sched2 barrier, so the seed counter is set before its pointer */
    do {
        REG_BG2X = x_offset << 8;
    } while (0);
    for (i = 0; i != 64; i++) {
        struct EffectStep *step = &work->particles[i];

        step->x = 0;
        step->y = 0;
        step->velocity_x = 0;
        step->z = 4;
    }
    for (frame = 0; frame != work->effect->count * 16 + 64; frame++) {
        for (member = 0; member != work->effect->count; member++) {
            struct MotionObject *member_object;
            u32 window;

            member_object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
            EffectPosition_ApplyStepAndYOffset(work->effect->actors[member], &pos);
            window = frame - member * 16;
            pos.x += x_offset;
            if (window < 64) {
                /* FAKEMATCH: record is filled through r0 so loop.c cannot hoist its address into fp */
                register s32 *rec asm("r0");

                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
                rec = record;
                rec[0] = member_object->x;
                rec[1] = member_object->y;
                rec[2] = member_object->z;
                EffectPosition_ApplyBaseAndYOffset(rec, &center);
                /* FAKEMATCH: needed to match: sched2 barrier, so this store precedes the spark counter as in the reference */
                do {
                    center.x += x_offset;
                } while (0);
                for (i = 0; i != 3; i++) {
                    struct EffectStep *spark = &work->particles[member * 3 + i];
                    s32 x;
                    s32 y;

                    x = pos.x + ((Trig_Sin(spark->velocity_x + i * 0x5555) << 3) >> 16);
                    y = pos.y + ((Trig_Cos(spark->velocity_x + i * 0x5555) << 3) >> 16);
                    spark->velocity_x += 0x200;
                    draw[0](canvas, work->sheet + i * 0x240, x - 12, y - 28, 24, 24);
                }
            }
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}
