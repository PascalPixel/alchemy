/* Draft, not exact (2026-09-25): 1064 of 1040 bytes, 289 differing
   instruction lines; the block topology matches the reference.
   Battle effect: 64 shards seeded at the target's slot position fly out
   under gravity from frame 4 while the caster sprite (work cells, frames
   6-11) and a burst strip (0x02010000, frames 16-47) are blitted through
   the heap-46 rectangle routine; frame 8 clears the canvas, starts phase
   0x86 and cues sound 0xd4.
   Remaining, register allocation: the reference keeps work in fp, the
   frame in r9, the index in r8 and the blitter array in sl; here work loses
   fp to the frame counter by a hair of global-alloc priority and is spilled
   to the stack, which adds a slot (frame 92 bytes, not 88) and a reload at
   every use. The reference also recomputes work + 0x7828 once in the frame
   loop instead of using the hoisted copy. */
#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "DMA.H"
#include "BATTLE_EFX.H"

struct Shard {
    s32 x;
    s32 y;
    s32 z;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 life;
};

struct ShardEffect {
    u8 unknown_00[4];
    s32 mirror;
    s32 position;
    u8 unknown_0c[12];
    s32 cue;
    u8 unknown_1c[8];
    s16 target;
};

struct ShardWork {
    u8 cells[0x7080];
    struct Shard shards[64];
    u8 unknown_7780_pad[0x7780 - 0x7080 - 64 * 28];
    s32 unknown_7780;
    s32 unknown_7784;
    u8 unknown_7788[0x20];
    s32 phase;
    u8 unknown_77ac[0x78];
    s32 transfer_pending;
    struct ShardEffect *effect;
};

struct SlotObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

extern void *gWorkSlot[];
extern u16 ParticleStreams_CellOffsets[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_EndCanvasLayer(void);
void BattleMotion_ApproachTargetFar(s32 position, s32 target, s32 speed, s32 mode);
struct SlotObject **GetBattleObjectSlotFar(s32 target);
s32 Random16(void);
void EffectPosition_ApplyStepAndYOffset(s32 position, s32 *out);
s32 EffectPosition_ApplyBaseAndYOffset(void *shard, s32 *out);
void EffectStep_AdvanceWithGravity3D(void *shard, s32 a, s32 b);
s32 Scheduler_AddOrUpdateCallback(s32 callback, s32 order);
s32 Scheduler_RemoveCallback(u32 callback);
void BattleEventRuntime_BeginPhaseFar(s32 id);
void Audio_PlayCue(s32 cue);
void Runtime_ReleaseHeapBlock(s32 slot);
void BattleFx_RunNoEffectFrames(s32 frames);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(u8 *source, u8 *destination);
void BattleFx_FetchRectangleBlitters(s32 flag, DrawRectangleFn *out_callbacks);
void BattleMotion_ApplyVariantMotionFar(s32 target, s32 motion);
void ObjectGroup_UpdateMembers(s32 target, s32 b, s32 c, s32 d, s32 e);
void Camera_ApplyShake(s32 a, s32 b);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);

void Unnamed_080e90a8(struct ShardEffect *object)
{
    void **heap_cache;
    void **cursor;
    struct ShardWork *work;
    void *canvas;
    u8 *cells;
    u8 *transfer;
    struct SlotObject *slot;
    struct Shard *shard;
    s32 position[3];
    s32 origin[3];
    s32 out[3];
    volatile s32 fill;
    DrawRectangleFn routine[2];
    s32 i;
    s32 frame;
    s32 size;
    u8 *burst;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    cells = heap_cache[2];
    transfer = heap_cache[-27];
    burst = (u8 *)0x02010000;
    work->effect = object;
    BattleFx_BeginCanvasLayer(0);
    Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_FireBurstSheet, burst, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, cells, 0, 0);
    BattleMotion_ApproachTargetFar(work->effect->position, work->effect->target, 4, 0);
    WaitFrames(1);
    slot = *GetBattleObjectSlotFar(work->effect->target);

    i = 0;
    do {
        struct Shard *seed = &work->shards[i];

        seed->x = slot->x;
        seed->y = slot->y;
        seed->z = slot->z;
        seed->vx = (Random16() & 255) << 11;
        seed->vy = ((Random16() & 255) - 127) << 12;
        seed->vz = ((Random16() & 255) - 127) << 12;
        if (seed->x > 0) {
            seed->vx = -seed->vx;
        }
        seed->life = i / 4 * 2 + 16;
        i++;
    } while (i != 64);

    EffectPosition_ApplyStepAndYOffset(work->effect->target, origin);
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    work->unknown_7780 = 2;
    work->unknown_7784 = 75;

    for (frame = 0; frame != 64; frame++) {
        if (frame == 8) {
            BattleEventRuntime_BeginPhaseFar(0x86);
        }
        if (work->effect->cue != 0 && frame == 8) {
            Audio_PlayCue(0xd4);
        }
        EffectPosition_ApplyStepAndYOffset(work->effect->position, position);
        if ((u32)(frame - 6) <= 5) {
            if (work->effect->mirror == 0) {
                BattleEffect_LoadWork(46, 7, 7, 3, 3);
            } else {
                BattleEffect_LoadWork(46, 7, 7, 7, 3);
            }
            routine[0] = gWorkSlot[46];
            if (work->effect->mirror == 0) {
                routine[0](canvas, work->cells + (frame - 6) * 0xd80, position[0] / 2 - 24, position[1] - 24, 48, 72);
            } else {
                routine[0](canvas, work->cells + (frame - 6) * 0xd80, position[0] / 2, position[1] - 24, 48, 72);
            }
            Runtime_ReleaseHeapBlock(46);
        }
        if ((u32)(frame - 16) <= 31) {
            s32 step = (frame - 16) / 2;
            s32 base;

            BattleEffect_LoadWork(46, 7, 7, 3, 2);
            routine[0] = gWorkSlot[46];
            if (step > 2) {
                step = 2;
            }
            if (work->effect->cue == 0) {
                base = 0;
            } else {
                base = 0x2580;
            }
            routine[0](canvas, (u8 *)0x02010000 + base + step * 0xc80, origin[0] / 2 - 20, origin[1] - 48, 40, 80);
            BattleFx_RunNoEffectFrames(10000);
            Runtime_ReleaseHeapBlock(46);
        }
        if (frame == 8) {
            fill = 0x3f3f3f3f;
            Dma_Set(&fill, canvas, 0x85001000, (volatile u32 *)0x040000d4);
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(transfer, transfer + 12);
        if (frame > 3) {
            BattleFx_FetchRectangleBlitters(work->effect->mirror, routine);
            for (i = 0; i != 64; i++) {
                s32 life;

                shard = &work->shards[i];
                life = shard->life;

                if (life > 0) {
                    EffectPosition_ApplyBaseAndYOffset(shard, out);
                    out[0] >>= 1;
                    out[1] = out[1] + origin[1] - 112;
                    size = (life >> 3) + 2;
                    routine[(i / 2) & 1](canvas, cells + ParticleStreams_CellOffsets[size - 1], out[0] - size / 2, out[1] - size,
                        size, size * 2);
                    EffectStep_AdvanceWithGravity3D(shard, 60, -0x400);
                    shard->life--;
                }
            }
            Runtime_ReleaseHeapBlock(47);
            Runtime_ReleaseHeapBlock(46);
        }
        if (frame == 8) {
            BattleMotion_ApplyVariantMotionFar(work->effect->target, 4);
            work->phase = 4;
        }
        if (frame == 6) {
            ObjectGroup_UpdateMembers(work->effect->target, 10, -1, -1, 0);
        }
        if (frame == 14) {
            ObjectGroup_UpdateMembers(work->effect->target, 10, -1, -1, 0);
        }
        Camera_ApplyShake(16, 16);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}
