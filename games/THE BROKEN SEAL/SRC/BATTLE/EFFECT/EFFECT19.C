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

extern u8 gMapCellBuffer[];
extern u8 gBattleFxWork[];
void BattlePresentation_ProcessPendingGraphicsTransfer(void);

/* One column record at work + 0x7080. */
struct DualColumn {
    s32 x;
    s32 y;
    u8 unknown_08[8];
    s32 height;
    u8 unknown_14[4];
    s32 start;
};

/* One slot of the 512-entry spark pool in gMapCellBuffer; age -1 is free. */
struct DualSpark {
    s32 x;
    s32 y;
    u8 unknown_08[16];
    s32 age;
};

struct DualTableEffect {
    u8 unknown_00[4];
    s32 mirror;
    u8 unknown_08[12];
    s32 target_count;
    s32 table;
    u8 unknown_1c[8];
    s16 targets[8];
};

struct DualTableWork {
    u8 cells[0x7080];
    struct DualColumn columns[16];
    u8 unknown_7240[0x540];
    s32 unknown_7780;
    s32 unknown_7784;
    u8 unknown_7788[0x20];
    s32 cue;
    u8 unknown_77ac[0x78];
    s32 transfer_pending;
    struct DualTableEffect *effect;
};

extern void *Data_03001e50[];
extern u8 Data_080eeb4b[];
extern u16 Data_080eeb4e[];
extern u8 Data_080eeb48[];
extern s8 Data_080eeb71[];
extern u16 Data_080eeb58[];
extern u8 Data_080eeb54[];
extern u8 Data_080eeb5e[];
extern u8 Data_080eeb61[];
extern s8 Data_080eeb79[];
extern u8 Data_080eeb80[];
extern u16 Data_080eeb88[];
void BattleFx_BeginCanvasLayer(s32 mode);
void Func_080b50e8(s32 id);
void Func_080f9010(s32 id);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Camera_ApplyShake(s32 a, s32 b);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);
void Func_08002dd8(s32 id);
s32 BattleFx_EndCanvasLayer(void);

typedef struct Column {
    s32 x;
    s32 unused[6];
} Column;

extern u8 gWorkSlot[];
extern s8 RisingColumns_ColumnOffsets[];
void BattleFx_BeginCanvasLayer(s32);
void BattleFx_PrepareCanvasEffect(void *, s32, s32, s32, s32 *, s32 *);
void Audio_PlayCue(s32);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
void Camera_ApplyShake(s32, s32);
#define WORK_EFFECT ((struct BattleEffectArgument *)work->effect)

void WaitFrames(s32);
void BattleEventRuntime_BeginPhaseFar(s32);
void Runtime_ReleaseHeapBlock(s32);
extern u8 BattleFxPillar_Kinds[];
extern s8 BattleFxPillar_X[];
extern u8 BattleFxPillar_Counts[];
extern s8 BattleFxPillar_PuffWidths[];
extern u8 BattleFxPillar_PuffHeights[];
extern u16 BattleFxPillar_PuffCells[];

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

void BattleFx_RunDualTable(void *object, s32 arg1);

/* battle/effects/dual_table/run_mode0.c */
void BattleFx_RunDualTableMode0(s32 arg0)
{
    BattleFx_RunDualTable(arg0, 0);
}

/* battle/effects/dual_table/run_mode1.c */
void BattleFx_RunDualTableMode1(s32 arg0)
{
    BattleFx_RunDualTable(arg0, 1);
}

/* Battle effect with two variants: arg1 picks resource 0x83 or 0x84 and the
   matching cell tables. It seeds one column per table entry in the 28-byte
   records at work + 0x7080 (x, y, height limit, start frame), then for each
   frame grows every started column upward, drawing it with the two
   alternating rectangle blitters. Four frames after a column starts it cues
   the effect targets and drops a spark into gMapCellBuffer's 512-slot pool;
   the sparks are drawn and aged every frame. A sibling of MEMBER_ORBIT.C.

   Shape notes, measured against the ROM:
   - One i serves every loop, which gives it a live range long enough to put
     work in fp.
   - The column cursor is recomputed from i at the top of the column loop
     (loop.c reduces it after the start frame i * 4 + 8), and the cue loop
     derives the target id offset from its index the same way, so both
     inductions are seeded after the hoisted invariants as in the ROM.
   - The spawn scan is a while loop whose free-slot case breaks out.
   - The spark blits halve a signed table byte with a plain / 2. */
void BattleFx_RunDualTable(void *object, s32 arg1)
{
    void **heap_cache;
    void **cursor;
    struct DualTableWork *work;
    s32 mode;
    void *draw_destination;
    s32 status;
    void *rectangle[2];
    void *second_slot;

    s32 sp24;
    s32 sp20;
    void **slot_pair;
    s32 sp18;
    s32 sp14;
    s32 sp10;
    s32 sp0C;

    u8 *slot_cursor;

    struct DualColumn *seed;
    s32 temp_r2_134;
    s32 temp_r5_140;
    s32 temp_r3_145;

    u8 *cell_base;
    struct DualColumn *column;
    s32 var_r5_297;
    s32 temp_r3_298;
    u8 temp_r4_315;
    s8 temp_r3_335;
    u8 temp_r4_348;

    s32 n;
    s32 id_offset;
    s32 cue_frame;


    s32 i;
    struct DualSpark *spark;
    s32 temp_r2_474;
    s32 temp_r7_481;
    s8 temp_r5_490;
    s8 temp_r4_498;
    s8 temp_r4_518;
    s32 temp_r3_538;

    mode = arg1;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    work->effect = object;
    BattleFx_BeginCanvasLayer(1);
    *(s16 *)0x04000020 = 0x100;
    *(s16 *)0x04000050 = 0;
    if (mode == 1) {
        Resource_LoadAndDecompress((s32)&ResourceId_GreenPillarSheet, work, 1, 1);
    } else {
        Resource_LoadAndDecompress((s32)&ResourceId_VineSheet, work, 1, 1);
    }
    if (work->effect->mirror == 1) {
        *(s32 *)0x04000028 = 0xFFFF9000;
    }
    status = BattleEffect_LoadWork(46, 7, 7, 3, 1);
    rectangle[0] = Data_03001e50[46];
    status = BattleEffect_LoadWork(47, 7, 7, 7, 1);
    second_slot = Data_03001e50[47];
    slot_pair = rectangle;
    slot_pair[1] = second_slot;

    sp20 = Data_080eeb5e[work->effect->table] * 4 + 0x38;

    slot_cursor = gMapCellBuffer + 0x18;
    i = 0;
    do {
        i += 1;
        *(s32 *)slot_cursor = -1;
        slot_cursor += 28;
    } while (i != 0x400);

    i = 0;
    seed = work->columns;
    do {
        temp_r2_134 = (Data_080eeb61[i] + (7 & Random16())) - 4;
        seed->y = i / 2 + 0x6C;
        seed->x = temp_r2_134;
        temp_r5_140 = (63 & Random16()) + 0x37;
        seed->height = temp_r5_140;
        temp_r3_145 = Data_080eeb4b[i % 3];
        if (temp_r3_145 < temp_r5_140) {
            seed->height = temp_r3_145;
        }
        temp_r3_145 = i * 4 + 8;
        seed->start = temp_r3_145;
        i += 1;
        seed++;
    } while (i != 16);

    work->unknown_7780 = 1;
    work->unknown_7784 = 0;
    Scheduler_AddOrUpdateCallback((void *)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    sp24 = 0;
    if (sp20 != 0) {
        sp18 = sp20 - 64;
        sp14 = sp20 - 20;
        sp10 = sp20 - 4;
        do {
            if (sp24 == sp18) {
                Func_080b50e8(0x84);
            }
            if (sp24 >= sp14 && sp24 < sp10) {
                *(s16 *)0x04000050 = 0x3F44;
                *(s16 *)0x04000052 = ((sp20 - sp24) - 5) | 0x1000;
            }
            if (sp24 < sp10) {
                i = 0;
                if (Data_080eeb5e[work->effect->table] != 0) {
                    do {
                        column = (struct DualColumn *)((u8 *)work + 0x7080 + i * 0x1C);
                        sp0C = i * 4 + 8;
                        if (sp24 == ((i * 4) + 9)) {
                            work->cue = 2;
                        }
                        if (sp24 > sp0C) {
                            u32 texture_index = i % 3;

                            var_r5_297 = (sp24 - sp0C) * 8;
                            temp_r3_298 = column->height;
                            if (var_r5_297 > temp_r3_298) {
                                var_r5_297 = temp_r3_298;
                            }
                            if (mode == 0) {
                                {
                                    s32 sel = 1 & i;
                                    s32 cell = Data_080eeb4e[texture_index];
                                    texture_index = Data_080eeb48[texture_index];
                                    ((DrawRectangleFn)slot_pair[sel])(draw_destination, (u8 *)work + cell,
                                        column->x - (texture_index >> 1),
                                        column->y - var_r5_297, texture_index, var_r5_297);
                                }
                            } else {
                                temp_r3_335 = Data_080eeb71[7 & i];
                                if (var_r5_297 > temp_r3_335) {
                                    var_r5_297 = temp_r3_335;
                                }
                                {
                                    s32 sel = 1 & i;
                                    s32 cell = Data_080eeb58[texture_index];
                                    texture_index = Data_080eeb54[texture_index];
                                    ((DrawRectangleFn)slot_pair[sel])(draw_destination, (u8 *)work + cell,
                                        column->x - (texture_index >> 1),
                                        column->y - var_r5_297, texture_index, var_r5_297);
                                }
                            }
                        }
                        n = 0;
                        if (work->effect->target_count != 0) {
                            cue_frame = sp0C + 4;
                            do {
                                id_offset = n * 2 + 0x24;
                                if (sp24 == cue_frame) {
                                    if (!(i & 1)) {
                                        Func_080f9010(0x85);
                                    }
                                    ObjectGroup_UpdateMembers(*(s16 *)((u8 *)work->effect + id_offset), 7, 5, n, 3);
                                }
                                n += 1;
                            } while (n != work->effect->target_count);
                        } else {
                            cue_frame = sp0C + 4;
                        }
                        if ((sp24 == cue_frame) || (sp24 == (sp0C + 8))) {
                            var_r5_297 = (u32)gMapCellBuffer;
                            n = 0;
                            while (n != 0x200) {
                                if (((struct DualSpark *)var_r5_297)->age == -1) {
                                    ((struct DualSpark *)var_r5_297)->x = ((Random16() & 0xF) + column->x) - 8;
                                    ((struct DualSpark *)var_r5_297)->y = (Random16() & 0xF) + 0x50;
                                    ((struct DualSpark *)var_r5_297)->age = 0;
                                    break;
                                }
                                var_r5_297 += 0x1C;
                                n += 1;
                            }
                        }
                        i += 1;
                    } while (i != Data_080eeb5e[work->effect->table]);
                }
            }

            i = 0;
            spark = (struct DualSpark *)gMapCellBuffer;
            do {
                temp_r2_474 = spark->age;
                if (temp_r2_474 >= 0) {
                    temp_r7_481 = temp_r2_474 / 2;
                    cell_base = (u8 *)0x1E59;
                    if (mode != 0) {
                        cell_base = (u8 *)0xAFF;
                    }
                    ((DrawRectangleFn)rectangle[0])(
                        draw_destination,
                        (u8 *)work + (Data_080eeb88[temp_r7_481] + (s32)cell_base),
                        spark->x - Data_080eeb79[temp_r7_481],
                        spark->y - (s8)Data_080eeb80[temp_r7_481] / 2,
                        Data_080eeb79[temp_r7_481], (s8)Data_080eeb80[temp_r7_481]);
                    ((DrawRectangleFn)rectangle[1])(
                        draw_destination,
                        (u8 *)work + (Data_080eeb88[temp_r7_481] + (s32)cell_base),
                        spark->x,
                        spark->y - (s8)Data_080eeb80[temp_r7_481] / 2,
                        Data_080eeb79[temp_r7_481], (s8)Data_080eeb80[temp_r7_481]);
                    temp_r3_538 = spark->age + 1;
                    spark->age = temp_r3_538;
                    if (temp_r3_538 == 0xE) {
                        spark->age = -1;
                    }
                }
                spark++;
                i += 1;
            } while (i != 0x200);

            Camera_ApplyShake(4, 4);
            ObjectGroup_TickMemberTimers();
            work->transfer_pending = 1;
            WaitFrames(1);
            sp24 += 1;
        } while (sp24 != sp20);
    }
    Scheduler_RemoveCallback((void *)BattlePresentation_ProcessPendingGraphicsTransfer);
    Func_08002dd8(47);
    Func_08002dd8(46);
    BattleFx_EndCanvasLayer();
}

/* Sixteen columns grow and retract in staggered windows. Five image cells
 * cycle with frame and column, using the alternating cached blitters.
 * Position-query outputs are separate from the two three-word coordinates.
 */
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

    cache = (u32 *)(gWorkSlot + 39 * 4);
    entry = cache;
    work = (struct BattleEffectWork *)*entry++;
    dst = (void *)*entry;
    WORK_EFFECT = effect;
    BattleFx_PrepareCanvasEffect(effect, 4, effect->side, 4, &origin_x, &origin_y);
    BattleFx_BeginCanvasLayer(1);
    *(s16 *)0x04000020 = 0x100;
    *(s16 *)0x04000050 = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_GreenVineSheet, work, 1, 1);
    EffectPosition_ApplyStepAndYOffset(WORK_EFFECT->actors[0], &first);
    EffectPosition_ApplyStepAndYOffset(WORK_EFFECT->actors[WORK_EFFECT->count - 1], &last);
    middle = first.x;
    middle += (last.x - middle) / 2;
    first.x = middle;
    *(s32 *)0x04000028 = (64 - first.x) << 8;
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    draw[0] = (DrawRectangle)cache[7];
    BattleEffect_LoadWork(47, 7, 7, 7, 1);
    draw[1] = (DrawRectangle)cache[8];
    column = (Column *)((u8 *)work + 0x7080);
    i = 0;
    do {
        column[i].x = RisingColumns_ColumnOffsets[i] + 64;
        i++;
    } while (i != 16);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    frame = 0;
    do {
        if (frame == 32) {
            Audio_PlayCue(143);
            for (i = 0; i != WORK_EFFECT->count; i++)
                ObjectGroup_UpdateMembers(WORK_EFFECT->actors[i], 7, 5, i, 16);
        }
        i = 0;
        column = (Column *)((u8 *)work + 0x7080);
        do {
            if (frame == i * 4 + 5)
                *(s32 *)((u8 *)work + 0x77a8) = 2;
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
                    draw[i & 1](dst, (u8 *)work + (cell << 10), column->x - 16,
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
    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

/* Battle effect: a row of pillars rises out of the ground one after another,
   eight frames apart, each kicking up a puff of dust and shaking the camera
   while the targets flinch; the screen fades back over the last 16 frames.
   Kinds 0 and 1 are tall columns that slide outwards as they grow, kinds 2
   and 3 short stumps; odd kinds slide left.

   The pillar loop keeps the reference's inductions: the start frame is its
   own variable (the frame tests are start, start + 1, start + 3 and
   start + 4), the pillar's age is computed before the start test, and the
   kind is read from its table at each test. */
void FunctionHead_080dd9c0(struct BattleEffectArgument *efx)
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

    cache = (u32 *)(gWorkSlot + 39 * 4);
    cursor = cache;
    work = (struct PillarWork *)*cursor++;
    dst = (void *)*cursor;
    work->effect = efx;
    BattleFx_BeginCanvasLayer(1);
    *(u16 *)0x04000020 = 0x100;
    *(u16 *)0x04000050 = 0;
    *(u16 *)0x04000052 = 0x1010;
    Resource_LoadAndDecompress((s32)&ResourceId_ThornSheet, work, 1, 1);
    if (work->effect->side == 1) {
        *(u32 *)0x04000028 = 0xffff9000;
    }
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    blit46 = ((DrawRectangle *)cache)[46 - 39];
    BattleEffect_LoadWork(47, 7, 7, 7, 1);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    blit47 = ((DrawRectangle *)cache)[47 - 39];
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    total = BattleFxPillar_Counts[work->effect->variant] * 8 + 56;

    for (i = 0; i != 1024; i++) {
        PARTICLES[i].variant = -1;
    }

    for (frame = 0; frame != total; frame++) {
        if (frame == total - 64) {
            BattleEventRuntime_BeginPhaseFar(132);
        }
        if (frame >= total - 16) {
            *(u16 *)0x04000050 = 0x3f44;
            *(u16 *)0x04000052 = (total - frame - 1) | 0x1000;
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

    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
