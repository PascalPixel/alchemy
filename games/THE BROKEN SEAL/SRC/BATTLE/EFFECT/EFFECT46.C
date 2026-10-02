#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_PRESENTATION.H"
#include "FIXED_MATH.H"
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
void BattlePres_SetupTransitionSceneFar(s32 x, s32 depth, s32 y, s32 mode);
void AudioCommand_PlayFar(s32 cue);

extern struct BattleCamera *gCameraWork;
extern u8 PuffArc_CellWidths[];
extern u8 PuffArc_CellHeights[];
extern u8 PuffArc_CellBiasY[];
extern u16 PuffArc_CellSourceOffsets[];

/* Where one copy of the flame is drawn. */
struct TrailPoint {
    s32 x;
    s32 y;
};

/* Battle effect: a flame flies along a stored path to the target and
   bursts, then drops on it a second time. Nineteen fading copies trail
   behind the flame; puffs mark where it starts, and embers and flashes fly
   from each strike. The camera swings for the first sixty-four frames. */
void BattleEffect_RunDualParticleStream(struct BattleEffectArgument *effect)
{
    struct TrailPoint trail[20];
    struct EffectPosition position;
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 drift;
    s8 *path;
    s32 x;
    s32 y;
    u8 *sheet;
    struct BattleCamera *camera;
    s32 i;
    s32 j;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    drift = 0;
    x = 0;
    y = 0;
    sheet = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    *(volatile u16 *)0x04000052 = 0x1010;
    Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetB, work, 1, 1);
    for (i = 1; i != 20; i++) {
        for (j = 0; j != 936; j++) {
            s32 shade = work->sheet[0x1680 + j];

            if (i > 10) {
                shade = shade - i * 4 + 40;
                if (shade < 0)
                    shade = 0;
                work->sheet[0x1680 + (i - 10) * 936 + j] = shade;
            }
        }
    }
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, &work->sheet[0x3c00], 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, Ram_MapCellBuffer, 1, 0);
    BattleFx_FetchRectangleBlitters(work->effect->side, draw);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (j = 0; j != 40; j++) {
        struct EffectStep *ember = &work->particles[8 + j];

        if (work->effect->side == 0)
            ember->x = -0x380000;
        else
            ember->x = 0x380000;
        ember->y = 0;
        ember->z = 0;
        ember->velocity_x = ((Random16() & 63) - 32) << 14;
        ember->velocity_y = (Random16() & 63) << 13;
        ember->velocity_z = ((Random16() & 63) - 32) << 14;
        ember->variant = 1;
    }
    for (j = 0; j != 16; j++) {
        struct EffectStep *flash = &work->particles[48 + j];

        if (work->effect->side == 0)
            flash->x = -0x380000;
        else
            flash->x = 0x380000;
        flash->y = 0x140000;
        flash->z = 0;
        flash->velocity_x = ((Random16() & 63) - 32) << 14;
        flash->velocity_y = (Random16() & 63) << 12;
        flash->velocity_z = ((Random16() & 63) - 32) << 14;
        flash->variant = 0;
    }
    for (i = 0, j = -0x4000; i != 8; i++, j += 0x1000) {
        struct EffectStep *puff = &work->particles[i];

        if (work->effect->side == 1)
            puff->x = (Trig_Sin(j) * 24 >> 16) + 88;
        else
            puff->x = (-(Trig_Sin(j) * 24) >> 16) + 16;
        puff->y = (Trig_Cos(j) * 16 >> 16) + 40;
        puff->variant = -(i * 2);
    }
    path = Resource_GetTableEntry((s32)&ResourceId_DualStreamPath);

    for (frame = 0; frame != 150; frame++) {
        camera = gCameraWork;
        if (frame == 83)
            BattleEventRuntime_BeginPhaseFar(134);
        if (frame == 0)
            AudioCommand_PlayFar(136);
        if (frame == 50)
            AudioCommand_PlayFar(136);
        if (work->effect->side == 0) {
            if (frame <= 63)
                camera->yaw -= 0x100;
        } else {
            if (frame <= 63)
                camera->yaw += 0x100;
        }
        BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
        if (frame <= 17) {
            s32 cell = frame / 3;

            draw[0](canvas, &work->sheet[0x3c00] + PuffArc_CellSourceOffsets[cell],
                48, PuffArc_CellBiasY[cell] + 60,
                PuffArc_CellWidths[cell], PuffArc_CellHeights[cell]);
            draw[1](canvas, &work->sheet[0x3c00] + PuffArc_CellSourceOffsets[cell],
                56, PuffArc_CellBiasY[cell] + 60,
                PuffArc_CellWidths[cell], PuffArc_CellHeights[cell]);
        }
        if (frame >= 18 && frame <= 58) {
            if (frame == 18) {
                x = (path[0] << 8) + (u8)path[1];
                y = (path[2] << 8) + (u8)path[3] + 16;
                path += 4;
            } else {
                x += path[0];
                y += path[1];
                path += 2;
            }
        }
        if (frame >= 78 && frame <= 118) {
            if (frame == 78) {
                x = -56;
                y = 48;
            } else {
                y -= 16;
            }
        }
        for (j = 19; j != 0; j--) {
            if (frame > j + 18 && frame <= j + 83) {
                trail[j].x = trail[j - 1].x;
                trail[j].y = trail[j - 1].y;
                if (j > 10)
                    draw[0](canvas, &work->sheet[0x1680 + (j - 10) * 936],
                        trail[j].x, trail[j].y, 24, 39);
                else
                    draw[0](canvas, &work->sheet[0x1680], trail[j].x, trail[j].y, 24, 39);
            }
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((u8 *)camera, (u8 *)camera->pos);
        if (frame >= 18 && frame <= 83) {
            s32 lift;

            if (work->effect->side == 1)
                position.x = 64 - x / 2;
            else
                position.x = x / 2 + 64;
            position.y = 60 - y;
            lift = (position.y - trail[0].y - 24) / 2;
            if (lift > 2)
                lift = 2;
            if (lift < -2)
                lift = -2;
            drift += lift;
            if (drift > 8)
                drift = 8;
            if (drift < -8)
                drift = -8;
            lift = drift / 4 + 2;
            trail[0].x = position.x - 12;
            trail[0].y = position.y - 20;
            draw[0](canvas, &work->sheet[lift * 0x480],
                trail[0].x - 6, trail[0].y - 2, 24, 48);
        }
        if (frame == 83) {
            work->shake_frames = 8;
            ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
        }
        if (frame > 83) {
            for (i = 0; i != 56; i++) {
                struct EffectStep *ember = &work->particles[8 + i];

                if (ember->y >= 0) {
                    s32 size;

                    EffectPosition_ApplyBaseAndYOffset(&ember->x, &position);
                    position.x >>= 1;
                    if (position.depth <= 159)
                        position.depth = 160;
                    if (position.depth > 799)
                        position.depth = 799;
                    size = 9 - (position.depth - 160) / 64;
                    if (i > 47) {
                        if (ember->variant <= 11) {
                            draw[0](canvas, Ram_MapCellBuffer + ember->variant / 2 * 0x800,
                                position.x - 16, position.y - 32, 32, 64);
                            ember->variant++;
                        }
                    } else {
                        draw[0](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
                            position.x - size / 2, position.y - size, size, size * 2);
                    }
                    ember->x += ember->velocity_x;
                    ember->y += ember->velocity_y;
                    ember->z += ember->velocity_z;
                    ember->velocity_y -= 0x2000;
                }
            }
        }
        if (frame == 50) {
            work->shake_frames = 12;
            ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
        }
        if (frame > 49) {
            for (i = 0; i != 8; i++) {
                struct EffectStep *puff = &work->particles[i];

                if ((u32)puff->variant <= 11) {
                    s32 cell = puff->variant / 2;

                    draw[1](canvas, &work->sheet[0x3c00] + PuffArc_CellSourceOffsets[cell],
                        puff->x - PuffArc_CellWidths[cell] / 2,
                        puff->y + PuffArc_CellBiasY[cell],
                        PuffArc_CellWidths[cell], PuffArc_CellHeights[cell]);
                }
                puff->variant++;
            }
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

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
