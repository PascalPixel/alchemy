#include "PROTO.H"
#include "GLOBAL.H"
#include "CANVAS.H"
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_EFX.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "BATTLE_PRESENTATION.H"

#define Camera_ApplyShake ((void (*)(s32,s32))Camera_ApplyShake)
#define Camera_ApplyShake ((void (*)(s32,s32))Camera_ApplyShake)

extern u8 gMapCellBuffer[];

struct DualColumn {
    s32 x;
    s32 y;
    u8 unknown_08[8];
    s32 height;
    u8 unknown_14[4];
    s32 start;
};

struct DualPuff {
    s32 x;
    s32 y;
    u8 unknown_08[16];
    s32 age;
};

void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void WaitFrames(s32 frames);

typedef struct Column {
    s32 x;
    s32 unused[6];
} Column;

extern void *gWorkSlot[];
void BattleFx_PrepareCanvasEffect(void *, s32, s32, s32, s32 *, s32 *);
void Audio_PlayCue(s32);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
#define WORK_EFFECT ((struct BattleEffectArgument *)work->effect)

void WaitFrames(s32);

struct PillarWork {
    u8 sheet[0x7780];
    s32 transfer_mode;
    s32 transfer_value;
    u8 unknown_7788[0x20];
    s32 shake;
    u8 unknown_77ac[0x78];
    s32 transfer_pending;
    struct BattleEffectArgument *effect;
};

#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)


void BattleFx_RunDualTableMode0(struct BattleEffectArgument *effect)
{
    BattleFx_RunDualTable(effect, 0);
}

void BattleFx_RunDualTableMode1(struct BattleEffectArgument *effect)
{
    BattleFx_RunDualTable(effect, 1);
}

void BattleFx_RunDualTable(struct BattleEffectArgument *effect, s32 mode)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    void *rectangle[2];
    void *second_slot;

    s32 frame;
    s32 duration;
    void **draw;
    s32 cue_frame;
    s32 fade_start;
    s32 fade_end;
    s32 start;

    u8 *age;

    struct DualColumn *seed;
    s32 x;
    s32 height;
    s32 limit;

    s32 puff_cells;
    struct DualColumn *column;
    s32 length;
    s32 reach;
    s8 pillar_height;

    s32 n;
    s32 react_frame;

    s32 i;
    struct DualPuff *puff;
    s32 puff_age;
    s32 image;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    REG_BG2PA = 0x100;
    REG_BLDCNT = 0;
    if (mode == 1) {
        Resource_LoadAndDecompress((s32)&ResourceId_GreenPillarSheet, work->sheet, 1, 1);
    } else {
        Resource_LoadAndDecompress((s32)&ResourceId_VineSheet, work->sheet, 1, 1);
    }
    if (work->effect->side == 1) {
        REG_BG2X = -0x7000;
    }
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    rectangle[0] = gWorkSlot[46];
    BattleEffect_LoadWork(47, 7, 7, 7, 1);
    second_slot = gWorkSlot[47];
    /* FAKEMATCH: the columns draw through a second pointer to the pair, whose address the frame keeps in a stack slot. */
    draw = rectangle;
    draw[1] = second_slot;

    duration = DualTable_ColumnCounts[work->effect->variant] * 4 + 56;

    age = gMapCellBuffer + 0x18;
    i = 0;
    do {
        i += 1;
        *(s32 *)age = -1;
        age += sizeof(struct DualPuff);
    } while (i != 0x400);

    i = 0;
    seed = (struct DualColumn *)work->particles;
    do {
        x = (DualTable_ColumnX[i] + (7 & Random16())) - 4;
        seed->y = i / 2 + 108;
        seed->x = x;
        height = (63 & Random16()) + 55;
        seed->height = height;
        limit = DualTable_HeightLimits[i % 3];
        if (limit < height) {
            seed->height = limit;
        }
        limit = i * 4 + 8;
        seed->start = limit;
        i += 1;
        seed++;
    } while (i != 16);

    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    frame = 0;
    if (duration != 0) {
        cue_frame = duration - 64;
        fade_start = duration - 20;
        fade_end = duration - 4;
        do {
            if (frame == cue_frame) {
                BattleEventRuntime_BeginPhaseFar(0x84);
            }
            if (frame >= fade_start && frame < fade_end) {
                REG_BLDCNT = 0x3F44;
                REG_BLDALPHA = ((duration - frame) - 5) | 0x1000;
            }
            if (frame < fade_end) {
                i = 0;
                if (DualTable_ColumnCounts[work->effect->variant] != 0) {
                    do {
                        column = (struct DualColumn *)((u8 *)work->particles + i * sizeof(struct DualColumn));
                        start = i * 4 + 8;
                        if (frame == i * 4 + 9) {
                            work->shake_frames = 2;
                        }
                        if (frame > start) {
                            u32 shape = i % 3;

                            length = (frame - start) * 8;
                            reach = column->height;
                            if (length > reach) {
                                length = reach;
                            }
                            if (mode == 0) {
                                {
                                    s32 side = 1 & i;
                                    s32 cell = DualTable_VineCells[shape];
                                    shape = DualTable_VineWidths[shape];
                                    ((DrawRectangleFn)draw[side])(canvas, work->sheet + cell,
                                        column->x - (shape >> 1),
                                        column->y - length, shape, length);
                                }
                            } else {
                                pillar_height = DualTable_PillarHeights[7 & i];
                                if (length > pillar_height) {
                                    length = pillar_height;
                                }
                                {
                                    s32 side = 1 & i;
                                    s32 cell = DualTable_PillarCells[shape];
                                    shape = DualTable_PillarWidths[shape];
                                    ((DrawRectangleFn)draw[side])(canvas, work->sheet + cell,
                                        column->x - (shape >> 1),
                                        column->y - length, shape, length);
                                }
                            }
                        }
                        n = 0;
                        if (work->effect->count != 0) {
                            react_frame = start + 4;
                            do {
                                if (frame == react_frame) {
                                    if (!(i & 1)) {
                                        Audio_PlayCue(0x85);
                                    }
                                    ObjectGroup_UpdateMembers(work->effect->actors[n], 7, 5, n, 3);
                                }
                                n += 1;
                            } while (n != work->effect->count);
                        } else {
                            react_frame = start + 4;
                        }
                        if ((frame == react_frame) || (frame == (start + 8))) {
                            /* FAKEMATCH: the scan reuses length as its puff pointer; a pointer variable of its own takes another register. */
                            length = (u32)gMapCellBuffer;
                            n = 0;
                            while (n != 0x200) {
                                if (((struct DualPuff *)length)->age == -1) {
                                    ((struct DualPuff *)length)->x = ((Random16() & 0xF) + column->x) - 8;
                                    ((struct DualPuff *)length)->y = (Random16() & 0xF) + 80;
                                    ((struct DualPuff *)length)->age = 0;
                                    break;
                                }
                                length += sizeof(struct DualPuff);
                                n += 1;
                            }
                        }
                        i += 1;
                    } while (i != DualTable_ColumnCounts[work->effect->variant]);
                }
            }

            i = 0;
            puff = (struct DualPuff *)gMapCellBuffer;
            do {
                puff_age = puff->age;
                if (puff_age >= 0) {
                    image = puff_age / 2;
                    puff_cells = 0x1e59;
                    if (mode != 0) {
                        puff_cells = 0xaff;
                    }
                    ((DrawRectangleFn)rectangle[0])(
                        canvas,
                        work->sheet + (DualTable_PuffCells[image] + puff_cells),
                        puff->x - DualTable_PuffWidths[image],
                        puff->y - (s8)DualTable_PuffHeights[image] / 2,
                        DualTable_PuffWidths[image], (s8)DualTable_PuffHeights[image]);
                    ((DrawRectangleFn)rectangle[1])(
                        canvas,
                        work->sheet + (DualTable_PuffCells[image] + puff_cells),
                        puff->x,
                        puff->y - (s8)DualTable_PuffHeights[image] / 2,
                        DualTable_PuffWidths[image], (s8)DualTable_PuffHeights[image]);
                    puff->age++;
                    if (puff->age == 14) {
                        puff->age = -1;
                    }
                }
                puff++;
                i += 1;
            } while (i != 0x200);

            Camera_ApplyShake(4, 4);
            ObjectGroup_TickMemberTimers();
            work->transfer_pending = 1;
            WaitFrames(1);
            frame += 1;
        } while (frame != duration);
    }
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

void BattleEffect_RunRisingColumns(struct BattleEffectArgument *effect)
{
    u32 *cache, *entry;
    struct BattleEffectWork *work;
    void *dst;
    DrawRectangle draw[2];
    s32 origin_x, origin_y;
    struct EffectPosition first, last;
    s32 i, frame, cell, height, offset, middle;
    Column *column;

    cache = (u32 *)(gWorkSlot + 39);
    entry = cache;
    work = (struct BattleEffectWork *)*entry++;
    dst = (void *)*entry;
    WORK_EFFECT = effect;
    BattleFx_PrepareCanvasEffect(effect, 4, effect->side, 4, &origin_x, &origin_y);
    BattleFx_BeginCanvasLayer(1);
    REG_BG2PA = 0x100;
    REG_BLDCNT = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_GreenVineSheet, work->sheet, 1, 1);
    EffectPosition_ApplyStepAndYOffset(WORK_EFFECT->actors[0], &first);
    EffectPosition_ApplyStepAndYOffset(WORK_EFFECT->actors[WORK_EFFECT->count - 1], &last);
    middle = first.x;
    middle += (last.x - middle) / 2;
    first.x = middle;
    REG_BG2X = (64 - first.x) << 8;
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    draw[0] = (DrawRectangle)cache[7];
    BattleEffect_LoadWork(47, 7, 7, 7, 1);
    draw[1] = (DrawRectangle)cache[8];
    column = (Column *)(work->sheet + 0x7080);
    i = 0;
    do {
        column[i].x = RisingColumns_ColumnOffsets[i] + 64;
        i++;
    } while (i != 16);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    frame = 0;
    do {
        if (frame == 32) {
            Audio_PlayCue(143);
            for (i = 0; i != WORK_EFFECT->count; i++)
                ObjectGroup_UpdateMembers(WORK_EFFECT->actors[i], 7, 5, i, 16);
        }
        i = 0;
        column = (Column *)(work->sheet + 0x7080);
        do {
            if (frame == i * 4 + 5)
                *(s32 *)(work->sheet + 0x77a8) = 2;
            offset = i * 2;
            if (frame > offset + 4) {
                cell = (frame / 4 + i) % 5;
                if (frame < offset + 32) {
                    height = (frame - offset) * 4 - 16;
                    if (height > 32) height = 32;
                } else {
                    height = 160 - (frame - offset) * 4;
                }
                if (height > 0)
                    draw[i & 1](dst, work->sheet + (cell << 10), column->x - 16,
                        (i & 7) - height + 104, 32, height);
            }
            i++;
            column++;
        } while (i != 16);
        Camera_ApplyShake(4, 4);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
        frame++;
    } while (frame != 70);
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

void BattleFx_RaisePillars(struct BattleEffectArgument *efx)
{
    void *dst;
    s32 frame;
    DrawRectangle blit47;
    DrawRectangle blit46;
    s32 total;
    u32 *cache;
    u32 *cursor;
    struct PillarWork *work;
    s32 i;
    s32 j;
    s32 age;
    s32 start;

    cache = (u32 *)(gWorkSlot + 39);
    cursor = cache;
    work = (struct PillarWork *)*cursor++;
    dst = (void *)*cursor;
    work->effect = efx;
    BattleFx_BeginCanvasLayer(1);
    REG_BG2PA = 0x100;
    REG_BLDCNT = 0;
    REG_BLDALPHA = 0x1010;
    Resource_LoadAndDecompress((s32)&ResourceId_ThornSheet, work->sheet, 1, 1);
    if (work->effect->side == 1) {
        REG_BG2X = 0xffff9000;
    }
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    blit46 = ((DrawRectangle *)cache)[46 - 39];
    BattleEffect_LoadWork(47, 7, 7, 7, 1);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    blit47 = ((DrawRectangle *)cache)[47 - 39];
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    total = BattleFxPillar_Counts[work->effect->variant] * 8 + 56;

    for (i = 0; i != 1024; i++) {
        PARTICLES[i].variant = -1;
    }

    for (frame = 0; frame != total; frame++) {
        if (frame == total - 64) {
            BattleEventRuntime_BeginPhaseFar(132);
        }
        if (frame >= total - 16) {
            REG_BLDCNT = 0x3f44;
            REG_BLDALPHA = (total - frame - 1) | 0x1000;
        }

        for (i = 0; i != BattleFxPillar_Counts[work->effect->variant]; i++) {
            start = i * 8 + 8;
            age = frame - (i * 8 + 8);
            if (frame > start) {
                s32 h;
                s32 w;

                if (BattleFxPillar_Kinds[i] < 2) {
                    h = age * 16;
                    w = age * 6;
                    if (h > 80) {
                        h = 80;
                    }
                    if (w > 30) {
                        w = 30;
                    }
                    if (BattleFxPillar_Kinds[i] & 1) {
                        blit47(dst, work->sheet, BattleFxPillar_X[i] - w, 108 - h, 48, h);
                    } else {
                        blit46(dst, work->sheet, BattleFxPillar_X[i] + w, 108 - h, 48, h);
                    }
                } else {
                    h = age * 8;
                    w = age;
                    if (h > 64) {
                        h = 64;
                    }
                    if (age > 8) {
                        w = 8;
                    }
                    if (BattleFxPillar_Kinds[i] & 1) {
                        blit47(dst, work->sheet + 0xf00, BattleFxPillar_X[i] - w, 108 - h, 32, h);
                    } else {
                        blit46(dst, work->sheet + 0xf00, BattleFxPillar_X[i] + w, 108 - h, 32, h);
                    }
                }
                if (frame == start + 1) {
                    work->shake = 3;
                }
                if (frame < start + 3) {
                    s32 y = (Random16() & 31) + 72;
                    struct EffectStep *p;
                    for (j = 0; j != 64; j++) {
                        p = &PARTICLES[j];
                        if (p->variant == -1) {
                            p->x = BattleFxPillar_X[i] + (Random16() & 31) + 32;
                            if (p->x > 96) {
                                p->x = 96;
                            }
                            p->y = y;
                            p->variant = 0;
                            break;
                        }
                    }
                }
            }
            for (j = 0; j != work->effect->count; j++) {
                if (frame == start + 4) {
                    Audio_PlayCue(132);
                    ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 3);
                }
            }
        }

        for (i = 0; i != 64; i++) {
            if (PARTICLES[i].variant >= 0) {
                s32 n = PARTICLES[i].variant / 2;
                blit46(dst, work->sheet + BattleFxPillar_PuffCells[n], PARTICLES[i].x - BattleFxPillar_PuffWidths[n],
                    PARTICLES[i].y - (s8)BattleFxPillar_PuffHeights[n] / 2, BattleFxPillar_PuffWidths[n], (s8)BattleFxPillar_PuffHeights[n]);
                blit47(dst, work->sheet + BattleFxPillar_PuffCells[n], PARTICLES[i].x,
                    PARTICLES[i].y - (s8)BattleFxPillar_PuffHeights[n] / 2, BattleFxPillar_PuffWidths[n], (s8)BattleFxPillar_PuffHeights[n]);
                if (++PARTICLES[i].variant == 14) {
                    PARTICLES[i].variant = -1;
                }
            }
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
