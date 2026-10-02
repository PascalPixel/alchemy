#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "RESOURCE_IDS.H"
#include "SYSTEM.H"

/* A battle object as its slot holds it: the world position follows two
   words this effect does not read. */
struct SlotObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

extern void *gBattleFxWork[];
extern void *gWorkSlot[];
extern u16 ParticleStreams_CellOffsets[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_EndCanvasLayer(void);
void BattleFx_FetchRectangleBlitters(s32 alternate, DrawRectangle *output);
void BattleFx_RunNoEffectFrames(s32 frames);
void BattleMotion_ApproachTargetFar(s32 actor, s32 target, s32 speed, s32 mode);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
struct SlotObject **GetBattleObjectSlotFar(s32 unit);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(u8 *source, u8 *destination);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode, s32 slot, s32 delay);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);

/* Battle effect: the actor closes on the first affected unit, a streak
   crosses it on frames 6-11 (mirrored for the other side) and a burst strip
   plays over it on frames 16-47, the second strip when the effect's variant
   is set. Sixty-four shards seeded at the unit fly out under gravity from
   frame 4, each living sixteen frames plus two for every four shards before
   it. Frame 8 clears the canvas, starts phase 0x86 and makes the unit
   react; its group is refreshed on frames 6 and 14. */
void BattleFx_RunFireBurstShards(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    u8 *cells;
    u8 *transfer;
    struct SlotObject *slot;
    struct EffectStep *shard;
    struct EffectPosition position;
    struct EffectPosition origin;
    struct EffectPosition out;
    volatile s32 fill;
    DrawRectangle routine[2];
    s32 i;
    s32 frame;
    s32 size;
    u8 *burst;

    heap_cache = gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    cells = heap_cache[2];
    transfer = heap_cache[-27];
    burst = Ram_MapCellBuffer;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_FireBurstSheet, burst, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, cells, 0, 0);
    BattleMotion_ApproachTargetFar(work->effect->actor, work->effect->actors[0], 4, 0);
    WaitFrames(1);
    slot = *GetBattleObjectSlotFar(work->effect->actors[0]);

    i = 0;
    do {
        struct EffectStep *seed = &work->particles[i];

        seed->x = slot->x;
        seed->y = slot->y;
        seed->z = slot->z;
        seed->velocity_x = (Random16() & 255) << 11;
        seed->velocity_y = ((Random16() & 255) - 127) << 12;
        seed->velocity_z = ((Random16() & 255) - 127) << 12;
        if (seed->x > 0)
            seed->velocity_x = -seed->velocity_x;
        seed->variant = i / 4 * 2 + 16;
        i++;
    } while (i != 64);

    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &origin);
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    work->transfer_mode = 2;
    work->transfer_value = 75;

    for (frame = 0; frame != 64; frame++) {
        if (frame == 8)
            BattleEventRuntime_BeginPhaseFar(0x86);
        if (work->effect->variant != 0 && frame == 8)
            Audio_PlayCue(0xd4);
        EffectPosition_ApplyStepAndYOffset(work->effect->actor, &position);
        if ((u32)(frame - 6) <= 5) {
            if (work->effect->side == 0)
                BattleEffect_LoadWork(46, 7, 7, 3, 3);
            else
                BattleEffect_LoadWork(46, 7, 7, 7, 3);
            routine[0] = gWorkSlot[46];
            if (work->effect->side == 0)
                routine[0](canvas, (u8 *)work + (frame - 6) * 0xd80, position.x / 2 - 24, position.y - 24, 48, 72);
            else
                routine[0](canvas, (u8 *)work + (frame - 6) * 0xd80, position.x / 2, position.y - 24, 48, 72);
            Runtime_ReleaseHeapBlock(46);
        }
        if ((u32)(frame - 16) <= 31) {
            s32 step = (frame - 16) / 2;
            s32 strip;

            BattleEffect_LoadWork(46, 7, 7, 3, 2);
            routine[0] = gWorkSlot[46];
            if (step > 2)
                step = 2;
            if (work->effect->variant == 0)
                strip = 0;
            else
                strip = 0x2580;
            routine[0](canvas, Ram_MapCellBuffer + strip + step * 0xc80, origin.x / 2 - 20, origin.y - 48, 40, 80);
            BattleFx_RunNoEffectFrames(10000);
            Runtime_ReleaseHeapBlock(46);
        }
        if (frame == 8) {
            fill = 0x3f3f3f3f;
            Dma_Set((void *)&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(transfer, transfer + 12);
        if (frame > 3) {
            BattleFx_FetchRectangleBlitters(work->effect->side, routine);
            for (i = 0; i != 64; i++) {
                s32 life;

                shard = &work->particles[i];
                life = shard->variant;
                if (life > 0) {
                    EffectPosition_ApplyBaseAndYOffset((s32 *)shard, &out);
                    out.x >>= 1;
                    out.y = out.y + origin.y - 112;
                    size = (life >> 3) + 2;
                    routine[(i / 2) & 1](canvas, cells + ParticleStreams_CellOffsets[size - 1], out.x - size / 2, out.y - size,
                        size, size * 2);
                    EffectStep_AdvanceWithGravity3D(shard, 60, -0x400);
                    shard->variant--;
                }
            }
            Runtime_ReleaseHeapBlock(47);
            Runtime_ReleaseHeapBlock(46);
        }
        if (frame == 8) {
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
            work->shake_frames = 4;
        }
        if (frame == 6)
            ObjectGroup_UpdateMembers(work->effect->actors[0], 10, -1, -1, 0);
        if (frame == 14)
            ObjectGroup_UpdateMembers(work->effect->actors[0], 10, -1, -1, 0);
        Camera_ApplyShake(16, 16);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}
