#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "SYSTEM.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "B5_CONTEXT.H"
#include "RAM_BUFFER.H"

extern u8 gBattleFxWork[];
extern u8 ShatterRocks_ShardOffsets[];
extern s8 ShatterRocks_RockX[];
extern u8 ShatterRocks_DropFrames[];
extern u8 ShatterRocks_RockCounts[];
extern u8 ShatterRocks_ShardWidths[];
extern u8 ShatterRocks_ShardHeights[];
extern u16 ShatterRocks_ShardCells[];

void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_EndCanvasLayer(void);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void Audio_PlayCue(s32 cue);

#define HI(v) (((s16 *)&(v))[1])

/* Battle effect: up to five rocks (by variant) fall in turn between the
   first and last affected units, each from its own frame and speeding up
   until it lands. Eighteen frames after it starts falling a rock shatters
   into 21 shards with a cue and a shake, and every affected unit reacts;
   the shards fly up under gravity, cycling through three cells each. */
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
    *(volatile u16 *)0x04000020 = 0x100;
    *(volatile u16 *)0x04000050 = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work, 1, 1);
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    blit = heap_cache[7];
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &first);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[work->effect->count - 1], &final);
    first.x += (final.x - first.x) / 2;
    *(volatile u32 *)0x04000028 = (64 - first.x) << 8;
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
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
                Audio_PlayCue(134);
                work->shake_frames = 4;
            }
            if (frame >= ShatterRocks_DropFrames[i] + 18) {
                for (j = 0; j != 21; j++) {
                    shard = &((struct EffectStep *)Ram_MapCellBuffer)[i * 21 + j];
                    cell = (j % 5) * 3 + shard->variant / 96 % 3;
                    blit(canvas, (u8 *)work + ShatterRocks_ShardCells[cell] + 0x83c,
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
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
