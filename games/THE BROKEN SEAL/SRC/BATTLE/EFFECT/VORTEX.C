#include "BATTLE_PRESENTATION.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFFECT_WORK.H"

struct BattleObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

/* The camera sway the phased-delta callback reads: its strength at 0x77ac
   and its running flag at 0x77b0. */
#define WORK_FIELD(work, offset) (*(s32 *)((u8 *)(work) + (offset)))

extern u8 gWorkSlot[];
extern u8 gMapCellBuffer[];
/* By variant: how many motes fly, and for how many frames. */
extern u8 VortexMotes_Counts[];
/* By variant: how many columns the tornado has, how far it sways, and how
   hard the screen shakes. */
extern u8 Tornado_Shapes[];
extern u16 BattleFx_GlintCellOffsets[];
extern u8 BattleFx_GlintCellWidths[];
extern u8 BattleFx_GlintCellHeights[];

void BattleFx_BeginCanvasLayer(s32 mode);
void *Resource_GetTableEntry(s32 id);
u32 Resource_DecodeType01(const void *source, void *destination);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void Camera_ApplyPhasedDelta(void);
void BattleEventRuntime_BeginPhaseFar(s32 value);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 source, s32 destination);
void AudioCommand_PlayFar(s32 value);
void Camera_ApplyShake(s32 random_mask, s32 shake_range);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void BattleMotion_ApplyVariantMotionFar(s32 member_id, s32 variant);
s32 Battle_GetObjectTableValueFar(s32 id);
s32 BattleFx_EndCanvasLayer(void);
struct BattleObject **GetBattleObjectSlotFar(s32 id);

/*
 * Motes burst from the acting unit two frames apart, drift, then home on the
 * affected units in turn. A mote that reaches the ground becomes a column
 * drawn in two halves for sixteen frames and knocks its unit. The variant
 * picks how many motes there are and how long the effect runs.
 */
void BattleFx_RunVortexMotes(struct BattleEffectArgument *effect)
{
    struct EffectPosition pos;
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangle draw[2];
    u8 *palette;
    struct BattleObject *source;
    struct BattleObject *object;
    struct EffectStep *point;
    struct EffectStep *seat;
    struct EffectStep *target;
    struct BattleCamera *camera;
    s32 frame;
    s32 y;
    s32 i;
    s8 *flags;

    flags = (s8 *)gMapCellBuffer;
    heap_cache = (void **)(gWorkSlot + 39 * 4);
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    palette = Resource_GetTableEntry((s32)&ResourceId_VortexSheet);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw[0] = (DrawRectangle)heap_cache[7];
    BattleEffect_LoadWork(47, 7, 7, 15, 2);
    *(u16 *)0x04000052 = 0x0f0f;
    draw[1] = (DrawRectangle)heap_cache[8];
    source = *GetBattleObjectSlotFar(work->effect->actor);
    y = source->y + Battle_GetObjectTableValueFar(work->effect->actor);
    for (i = 0; i != 30; i++) {
        point = &work->particles[i];
        point->x = source->x;
        point->y = y;
        point->z = source->z;
        point->velocity_x = ((s32)((Random16() & 255) - 127) << 16) >> 5;
        point->velocity_y = ((s32)((Random16() & 127) - 16) << 16) >> 6;
        point->velocity_z = ((s32)((Random16() & 255) - 127) << 16) >> 5;
        point->variant = -1;
        flags[i] = 0;
    }
    for (i = 0; i != work->effect->count; i++) {
        seat = &work->particles[32 + i];
        object = *GetBattleObjectSlotFar(work->effect->actors[i]);
        seat->x = object->x;
        seat->y = 0;
        seat->z = object->z;
    }
    WORK_FIELD(work, 0x77ac) = 0;
    WORK_FIELD(work, 0x77b0) = 0;
    Scheduler_AddOrUpdateCallback((s32)Camera_ApplyPhasedDelta, 0x480);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    AudioCommand_PlayFar(164);

    for (frame = 0; frame != VortexMotes_Counts[work->effect->variant * 2 + 1]; frame++) {
        camera = *(struct BattleCamera **)(gWorkSlot + 12 * 4);
        if (frame >= 17 && frame < 64)
            WORK_FIELD(work, 0x77ac) = 0x180;
        else
            WORK_FIELD(work, 0x77ac) = 0;
        if (frame == VortexMotes_Counts[work->effect->variant * 2 + 1] - 16)
            BattleEventRuntime_BeginPhaseFar(132);
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        for (i = 0; i != VortexMotes_Counts[work->effect->variant * 2]; i++) {
            point = &work->particles[i];
            if (frame > i * 2 && flags[i] == 0) {
                EffectPosition_ApplyBaseAndYOffset((s32 *)point, &pos);
                pos.x >>= 1;
                if (pos.depth < 160)
                    pos.depth = 160;
                if (pos.depth > 0x31f)
                    pos.depth = 0x31f;
                draw[0](canvas, (u8 *)work + 0xc00, pos.x - 6, pos.y - 12, 12, 24);
                point->x += point->velocity_x;
                point->y += point->velocity_y;
                point->z += point->velocity_z;
            }
            if (frame > i * 2 + 48 && flags[i] == 0) {
                target = &work->particles[32 + i % work->effect->count];
                point->velocity_x += (target->x - point->x) >> 9;
                point->velocity_y += (target->y - point->y) >> 9;
                point->velocity_z += (target->z - point->z) >> 9;
                if (frame < i * 2 + 85) {
                    point->velocity_x = point->velocity_x * 60 / 64;
                    point->velocity_y = point->velocity_y * 60 / 64;
                    point->velocity_z = point->velocity_z * 60 / 64;
                }
                if (point->y < 0) {
                    flags[i] = 1;
                    point->variant = 0;
                    point->x = pos.x;
                    point->y = pos.y + (Random16() & 31) - 16;
                    ObjectGroup_UpdateMembers(work->effect->actors[i % work->effect->count], 7, 5,
                        i % work->effect->count, 4);
                    BattleMotion_ApplyVariantMotionFar(work->effect->actors[i % work->effect->count], 0);
                    work->shake_frames = 4;
                    AudioCommand_PlayFar(132);
                }
            }
            if ((u32)point->variant < 16) {
                draw[0](canvas, (u8 *)work + ((point->variant / 2) % 3 << 10),
                    point->x - 16, point->y - 56, 16, 64);
                draw[1](canvas, (u8 *)work + ((point->variant / 2) % 3 << 10),
                    point->x, point->y - 56, 16, 64);
                point->variant++;
            }
        }
        Camera_ApplyShake(work->effect->variant * 2 + 2, work->effect->variant * 2 + 2);
        if (WORK_FIELD(work, 0x77b0) == 0)
            WORK_FIELD(work, 0x77b0) = 1;
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((s32)Camera_ApplyPhasedDelta);
    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

/*
 * A swaying tornado of stacked segments, one column a variant step, with
 * four glints circling each column. The canvas clears on seven beats, and
 * each affected unit is struck on those beats three frames after the last.
 */
void BattleFx_RunTornado(struct BattleEffectArgument *effect)
{
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 wave;
    u8 *palette;
    struct EffectStep *point;
    s32 frame;
    s32 x;
    s32 y;
    s32 segment;
    s32 cell;
    s32 i;
    s32 j;

    heap_cache = (void **)(gWorkSlot + 39 * 4);
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    *(u16 *)0x04000052 = 0x1010;
    palette = Resource_GetTableEntry((s32)&ResourceId_TornadoSheet);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw[0] = (DrawRectangle)heap_cache[7];
    BattleEffect_LoadWork(47, 7, 7, 7, 2);
    draw[1] = (DrawRectangle)heap_cache[8];
    for (j = 0; j != 16; j++) {
        point = &work->particles[j];
        point->x = Random16() & 31;
        point->y = (Random16() & 63) + 16;
        point->variant = -(s32)(Random16() & 15);
    }
    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    *(u16 *)0x04000052 = 0x1000;
    WaitFrames(1);
    AudioCommand_PlayFar(141);

    for (frame = 0; frame != 80; frame++) {
        wave = Trig_Sin(frame << 10) << 4;
        if (frame == 32)
            BattleEventRuntime_BeginPhaseFar(133);
        for (i = 0; i != 7; i++) {
            if (frame == i * 8 + 16)
                Iwram_FillWords(canvas, 0x4000, 0x08080808);
        }
        if (work->effect->side == 1)
            wave += 0x200000;
        else
            wave -= 0x200000;
        if (frame <= 16)
            *(u16 *)0x04000052 = frame | 0x1000;
        if (frame > 63)
            *(u16 *)0x04000052 = (0x4f - frame) | 0x1000;
        for (i = 0; i != Tornado_Shapes[work->effect->variant * 3]; i++) {
            x = ((Tornado_Shapes[work->effect->variant * 3 + 1]
                    * Trig_Sin((frame << 11) + i * 0x4000) + wave) >> 16) + 40;
            y = (Trig_Cos((frame << 11) + i * 0x4000) * 2) >> 16;
            segment = (frame / 2) % 3;
            draw[0](canvas, (u8 *)work + segment * 0xa00 + 0xc56, x, y + 16, 40, 32);
            draw[0](canvas, (u8 *)work + segment * 0x500 + 0x2a56, x, y + 48, 40, 32);
            draw[0](canvas, (u8 *)work + segment * 0xa00 + 0x1156, x, y + 80, 40, 32);
            for (j = 0; j != 4; j++) {
                point = &work->particles[i * 4 + j];
                if (point->variant >= 0) {
                    cell = point->variant / 2 + (j / 2) * 3;
                    draw[j & 1](canvas, (u8 *)work + BattleFx_GlintCellOffsets[cell],
                        point->x + x, point->y + y,
                        BattleFx_GlintCellWidths[cell], BattleFx_GlintCellHeights[cell]);
                }
                point->variant++;
                if (point->variant == 6) {
                    point->x = Random16() & 31;
                    point->y = (Random16() & 63) + 16;
                    point->variant = 0;
                }
            }
        }
        for (j = 0; j != work->effect->count; j++) {
            for (i = 0; i != 7; i++) {
                if (frame == i * 8 + j * 3 + 16) {
                    ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 4);
                    BattleMotion_ApplyVariantMotionFar(work->effect->actors[j], 6);
                }
            }
        }
        work->shake_frames = 1;
        Camera_ApplyShake(Tornado_Shapes[work->effect->variant * 3 + 2],
            Tornado_Shapes[work->effect->variant * 3 + 2] * 2);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}
