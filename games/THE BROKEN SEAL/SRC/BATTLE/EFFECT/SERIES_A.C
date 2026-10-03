#include "RESOURCE.H"
#include "BATTLE_PRESENTATION.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "MOTION_OBJECT.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

extern u8 gBattleFxWork[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void BattleFx_PrepareCanvasEffect(struct BattleEffectArgument *effect, s32 kind,
    s32 side, s32 narrow, s32 *x, s32 *y);
void BattleFx_FetchRectangleBlitters(s32 alternate, DrawRectangle *output);
u32 Resource_DecodeType01(const void *source, void *destination);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyRoll(s32 angle);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 member_id);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);
void ObjectGroup_TickMemberTimers(void);

extern u16 ParticleStreams_CellOffsets[];
extern u16 BattleFx_PuffCells[];
extern u8 BattleFx_PuffSizes[];

void BattleFx_RunSeriesA(struct BattleEffectArgument *effect, u32 mode);

/* Mode entries of battle effect series A. */
void BattleFx_RunSeriesAMode0(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 0);
}

void BattleFx_RunSeriesAMode1(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 1);
}

void BattleFx_RunSeriesAMode2(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 2);
}

void BattleFx_RunSeriesAMode3Or4(struct BattleEffectArgument *effect)
{
    if (effect->variant == 0) {
        BattleFx_RunSeriesA(effect, 3);
        return;
    }
    BattleFx_RunSeriesA(effect, 4);
}

void BattleFx_RunSeriesAMode4(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 4);
}

void BattleFx_RunSeriesAMode5(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 5);
}

void BattleFx_RunSeriesAMode6(struct BattleEffectArgument *effect)
{
    BattleFx_RunSeriesA(effect, 6);
}

/* Battle effect: a djinni hovers over the acting side while a cloud of
   sparks gathers on, or drifts past, every affected unit, eight frames
   apart, in one of seven modes. */
void BattleFx_RunSeriesA(struct BattleEffectArgument *effect, u32 mode)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 total;
    s32 member;
    void *sheet;
    s32 limit;
    s32 shift;
    s32 narrow;
    DrawRectangle *draw;
    s32 frame;
    s32 i;
    void *palette;
    struct EffectPosition base;
    struct EffectPosition screen;
    s32 point[3];
    s32 origin_x;
    s32 origin_y;
    DrawRectangle callbacks[2];

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    shift = 0;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    if (work->effect->unknown_001c == 1) {
        s32 kind = mode == 6 ? 2 : 1;

        if (mode == 6 || mode == 0)
            BattleFx_PrepareCanvasEffect(effect, kind, work->effect->side, 0,
                &origin_x, &origin_y);
        else
            BattleFx_PrepareCanvasEffect(effect, kind, work->effect->side, 1,
                &origin_x, &origin_y);
        work->effect->variant = 0;
    }
    if (mode == 0) {
        EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &base);
        shift = 64 - base.x;
        REG_BG2X = shift << 8;
        REG_BG2PA = 0x100;
        narrow = 0;
    } else {
        narrow = 1;
    }
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, work, 0, 0);
    if (mode <= 1 || mode == 3 || mode == 4 || mode == 5) {
        if (work->effect->variant == 0)
            palette = Resource_GetTableEntry((s32)&ResourceId_IceShardSheet);
        else
            palette = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
    } else if (mode == 6) {
        palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
    } else {
        palette = Resource_GetTableEntry((s32)&ResourceId_BlastSheet);
    }
    /* FAKEMATCH: forwarded through Call3, the palette copy loads its routine
       before it shifts the destination, as the reference does. */
    Call3((void (*)())Iwram_CopyWords, (s32)BG_PLTT, (s32)palette, 128);
    if (narrow == 0) {
        if (mode == 6)
            palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet);
        else
            palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet);
    } else {
        if (mode == 6)
            palette = Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet);
        else
            palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSmallSheet);
    }
    palette = (u8 *)palette + 128;
    Resource_DecodeType01(palette, (u8 *)work + 0x1000);
    BattleFx_FetchRectangleBlitters(work->effect->side, draw = callbacks);

    if (mode == 0 || mode == 6) {
        for (i = 0; i != 512; i++) {
            struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];


            spark->x = ((Random16() & 0xff) - 127) << 15;
            spark->y = ((Random16() & 0x7f) + 64) << 15;
            spark->z = ((Random16() & 0xff) - 127) << 15;
            spark->variant = 0;
        }
        total = work->effect->count * 8 + 88;
    } else if (mode == 1) {
        for (i = 0; i != 512; i++) {
            struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];

            spark->x = ((Random16() & 0xff) - 127) << 15;
            spark->y = ((Random16() & 0xff) - 127) << 15;
            spark->z = ((Random16() & 0xff) - 127) << 15;
            spark->variant = 0;
        }
        total = work->effect->count * 8 + 88;
    } else if (mode == 2) {
        for (i = 0; i != 512; i++) {
            struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];
            s32 heading = Random16() & 0xffff;
            s32 radius = (Random16() & 63) + 32;

            spark->x = Trig_Sin(heading) * radius;
            spark->y = -50 << 16;
            spark->z = Trig_Cos(heading) * radius;
            spark->velocity_y = ((Random16() & 31) + 32) << 13;
            spark->variant = 0;
        }
        total = work->effect->count * 8 + 88;
    } else if (mode == 3) {
        for (i = 0; i != 512; i++) {
            struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];

            spark->x = ((Random16() & 0xff) - 127) << 15;
            spark->y = ((Random16() & 0xff) - 127) << 14;
            spark->z = ((Random16() & 0xff) - 127) << 15;
            spark->variant = 0;
        }
        total = work->effect->count * 8 + 72;
    } else {
        for (i = 0; i != 512; i++) {
            struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];

            spark->x = ((Random16() & 0xff) - 127) << 15;
            spark->y = ((Random16() & 0xff) - 127) << 15;
            spark->z = ((Random16() & 0xff) - 127) << 15;
            spark->variant = 0;
        }
        total = work->effect->count * 8 + 72;
    }

    limit = 64;
    if (work->effect->variant == 0)
        limit = 32;
    else if (work->effect->variant == 2)
        limit = 128;
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (frame = 0; frame != total; frame++) {
        struct BattleCamera *camera = gCameraWork;

        if (frame == 40)
            BattleEventRuntime_BeginPhaseFar(0);
        if (work->effect->unknown_001c == 1) {
            if (narrow == 0) {
                s32 x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin_x + shift - 20;
                s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + origin_y - 24;

                if (frame > 32)
                    y = y - frame * 2 + 64;
                callbacks[0](canvas, (u8 *)work + 0x1000, x, y, 40, 40);
                if (frame <= 3)
                    draw[1](canvas, (u8 *)work + 0x1000, x, y, 40, 40);
            } else {
                s32 x = ((Trig_Sin(frame << 11) * 10) >> 16) + origin_x / 2 - 10;
                s32 y = ((Trig_Cos(frame << 11) * 4) >> 16) + origin_y - 24;

                if (frame > 32)
                    y = y - frame * 2 + 64;
                callbacks[0](canvas, (u8 *)work + 0x1000, x, y, 20, 40);
                if (frame <= 3)
                    draw[1](canvas, (u8 *)work + 0x1000, x, y, 20, 40);
            }
        }

        for (member = 0; member != work->effect->count; member++) {
            struct MotionObject *object =
                GetBattleObjectSlotFar(work->effect->actors[member])->object;
            s32 start = member * 8;

            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
            point[0] = object->x;
            point[1] = 160 << 14;
            point[2] = object->z;
            SceneTransform_ApplyPosition(point);
            if (frame == start + 20)
                Audio_PlayCue(126);
            if (frame == start + 36)
                ObjectGroup_UpdateMembers(work->effect->actors[member], 7, -1, member, 28);
            if (frame > start) {
                if (mode == 0 || mode == 6) {
                    SceneTransform_ApplyYaw((frame - member * 8) << 9);
                } else if (mode == 1) {
                    SceneTransform_ApplyPitch(frame << 9);
                    SceneTransform_ApplyRoll(frame << 9);
                } else if (mode == 2) {
                    SceneTransform_ApplyYaw((frame - member * 40) << 9);
                } else if (mode == 3) {
                    SceneTransform_ApplyYaw((frame - member * 8) << 9);
                } else {
                    SceneTransform_ApplyYaw((frame - member * 8) << 9);
                    SceneTransform_ApplyPitch((frame - member * 8) << 9);
                }
                for (i = 0; i != limit; i++) {
                    struct EffectStep *spark =
                        &((struct EffectStep *)Ram_MapCellBuffer)[member * 64 + i];
                    s32 end;

                    if (mode == 3 || mode == 4 || mode == 5)
                        end = start + i / 2 + 32;
                    else
                        end = 0x10000;
                    if (frame > start + i / 4 && frame < end) {
                        s32 xx = (spark->x >> 8) * (spark->x >> 8);
                        s32 yy = (spark->y >> 8) * (spark->y >> 8);
                        s32 zz = (spark->z >> 8) * (spark->z >> 8);
                        s32 distance = Iwram_Sqrt(xx + yy + zz) >> 9;

                        if (distance != 0) {
                            s32 size;

                            EffectPosition_ApplyBaseAndYOffset((s32 *)spark, &screen);
                            if (mode == 0)
                                screen.x += shift;
                            else
                                screen.x >>= 1;
                            screen.y += 16;
                            if (screen.depth < 314)
                                screen.depth = 314;
                            if (screen.depth > 634)
                                screen.depth = 634;
                            size = 3 - (screen.depth - 314) / 128;
                            if (mode == 0)
                                size = (i * 4 + frame) % 9;
                            if (mode == 0 || mode == 3 || mode == 4 || mode == 5) {
                                u8 *cell = (u8 *)work + BattleFx_PuffCells[size];
                                u32 edge = BattleFx_PuffSizes[size];
                                u32 half = edge >> 1;

                                draw[1](canvas, cell, screen.x - half, screen.y - half,
                                    edge, edge);
                            } else {
                                draw[1](canvas,
                                    (u8 *)sheet + ParticleStreams_CellOffsets[size - 1],
                                    screen.x - size / 2, screen.y - size, size, size * 2);
                            }
                            if (mode <= 2 || mode == 6) {
                                spark->x -= spark->x / distance;
                                spark->y -= spark->y / distance;
                                spark->z -= spark->z / distance;
                            }
                        }
                    }
                }
            }
        }
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
