#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "B5_CONTEXT.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"

extern u8 gBattleFxWork[];
extern DrawRectangle gWorkSlot[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_EndCanvasLayer(void);
void BattleFx_PrepareCanvasEffect(struct BattleEffectArgument *effect, s32 kind, s32 side,
    s32 anchor, s32 *out_x, s32 *out_y);
struct B5Context *GetBattleObjectSlotFar(s32 id);
void BattleMotion_ApproachTargetFar(s32 actor, s32 target, s32 frames, s32 speed);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 first, s32 last);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);

/* Seven bytes for each burst scene: which burst sheet and palette it uses,
   how many bursts strike, how many motes each wakes, how far apart they are,
   how long the scene runs and whether a burst flashes the canvas. */
extern u8 BurstScene_Records[];
extern u16 ParticleStreams_CellOffsets[];

/* Battle effect: bursts strike the target one after another. Each shows six
   frames of the burst sheet (or, on its odd pairs, of the wide streak in the
   map cell buffer), makes the target react on its third frame and wakes a
   handful of the motes waiting above it, then leaves a fading column. The
   motes fall, bounce and shrink until their life in variant runs out. */
void BattlePres_RunBurstScene(struct BattleEffectArgument *effect, s32 variant)
{
    struct EffectPosition actor_position;
    struct EffectPosition target_position;
    struct EffectPosition position;
    s32 screen_x;
    s32 screen_y;
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    void *sheet;
    struct BattleCamera *camera;
    struct MotionObject *target;
    u8 *palette;
    s32 resource;
    s32 row;
    s32 column;
    s32 i;
    s32 j;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    camera = *(struct BattleCamera **)((u8 *)heap_cache - 108);
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    *(volatile u16 *)0x04000052 = 0x1010;
    if (work->effect->unknown_001c == 1)
        BattleFx_PrepareCanvasEffect(effect, 7, work->effect->side, 2, &screen_x, &screen_y);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work, 1, 0);
    for (row = 0; row != 288; row++) {
        for (column = 0; column != 40; column++) {
            s32 source = row * 40 + column;
            s32 offset = row * 20 + column / 2 + 0x5100;

            ((u8 *)work)[offset] = ((u8 *)work)[source];
        }
    }
    if (BurstScene_Records[variant * 7] == 0)
        Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA, work, 1, 1);
    else
        Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, Ram_MapCellBuffer + 0x5e00, 1, 0);
    switch (BurstScene_Records[variant * 7 + 1]) {
    case 0:
        resource = (s32)&ResourceId_MarsDjinnSheet;
        break;
    case 1:
        resource = (s32)&ResourceId_LimePalette;
        break;
    case 2:
        resource = (s32)&ResourceId_BlueArcSheetB;
        break;
    default:
        resource = (s32)&ResourceId_EmberStreakSheet;
        break;
    }
    palette = Resource_GetTableEntry(resource);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    WaitFrames(1);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &target_position);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
    for (i = 0; i != 768; i++) {
        struct EffectStep *mote = &((struct EffectStep *)Ram_MapCellBuffer)[i];

        mote->x = target->x;
        mote->y = target->y + 0x190000;
        mote->z = target->z;
        mote->velocity_x = (Random16() & 255) << 12;
        mote->velocity_y = ((Random16() & 255) - 127) << 12;
        mote->velocity_z = ((Random16() & 255) - 127) << 12;
        if (mote->x > 0)
            mote->velocity_x = -mote->velocity_x;
        mote->variant = -1;
    }
    BattleMotion_ApproachTargetFar(work->effect->actor, work->effect->actors[0], 4, 0);
    for (frame = 0; frame != BurstScene_Records[variant * 7 + 5]; frame++) {
        s32 bursts = BurstScene_Records[variant * 7 + 2];

        EffectPosition_ApplyStepAndYOffset(work->effect->actor, &actor_position);
        actor_position.x /= 2;
        if (work->effect->side == 0) {
            BattleEffect_LoadWork(46, 7, 7, 3, 2);
            BattleEffect_LoadWork(47, 7, 7, 11, 2);
        } else {
            BattleEffect_LoadWork(46, 7, 7, 7, 2);
            BattleEffect_LoadWork(47, 7, 7, 15, 2);
        }
        draw[0] = gWorkSlot[46];
        draw[1] = gWorkSlot[47];
        for (i = 0; i != bursts; i++) {
            s32 start = i * BurstScene_Records[variant * 7 + 4];

            if (frame >= start && frame < start + 6) {
                s32 step = frame - start;

                if ((i & 3) <= 1 || BurstScene_Records[variant * 7] == 1) {
                    if (work->effect->side == 0)
                        draw[i & 1](canvas, (u8 *)work + step * 3456,
                            target_position.x / 2 - 16, target_position.y - 40, 48, 72);
                    else
                        draw[i & 1](canvas, (u8 *)work + step * 3456,
                            target_position.x / 2 - 32, target_position.y - 40, 48, 72);
                } else {
                    if (work->effect->side == 0)
                        draw[i & 1](canvas, Ram_MapCellBuffer + 0x5e00 + step * 768,
                            target_position.x / 2 - 16, actor_position.y - 8, 48, 16);
                    else
                        draw[i & 1](canvas, Ram_MapCellBuffer + 0x5e00 + step * 768,
                            target_position.x / 2 - 32, actor_position.y - 8, 48, 16);
                }
            }
            if (frame == start + 2) {
                if (BurstScene_Records[variant * 7 + 6] == 1)
                    Iwram_FillWords(canvas, 0x4000, 0x2f2f2f2f);
                ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
                if (i == bursts - 1) {
                    BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
                    work->shake_frames = 8;
                    BattleEventRuntime_BeginPhaseFar(134);
                } else {
                    if (i & 1)
                        BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 7);
                    work->shake_frames = 4;
                    Audio_PlayCue(134);
                }
                for (j = 0; j != BurstScene_Records[variant * 7 + 3]; j++)
                    ((struct EffectStep *)Ram_MapCellBuffer)[i * 32 + j].variant = (Random16() & 7) + 15;
            }
            if (frame >= start + 2 && frame < start + 14)
                draw[0](canvas, (u8 *)work + ((frame - start - 2) / 2) * 960 + 0x5100,
                    target_position.x / 2 - 10, target_position.y - 24, 20, 48);
        }
        Runtime_ReleaseHeapBlock(47);
        Runtime_ReleaseHeapBlock(46);
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        BattleEffect_LoadWork(46, 7, 7, 3, 3);
        BattleEffect_LoadWork(47, 7, 7, 3, 2);
        draw[0] = gWorkSlot[46];
        draw[1] = gWorkSlot[47];
        for (i = 0; i != 512; i++) {
            struct EffectStep *mote = &((struct EffectStep *)Ram_MapCellBuffer)[i];

            s32 size = mote->variant;

            if (size > 0) {
                EffectPosition_ApplyBaseAndYOffset(&mote->x, &position);
                position.x /= 2;
                size >>= 3;
                size++;
                draw[(i / 2) & 1](canvas, (u8 *)sheet + ParticleStreams_CellOffsets[size - 1],
                    position.x - size / 2, position.y - size, size, size * 2);
                EffectStep_AdvanceWithGravity3D(mote, 60, -0x400);
                if (mote->y < 0x80000)
                    mote->velocity_y = -mote->velocity_y / 2;
                mote->variant--;
            }
        }
        Runtime_ReleaseHeapBlock(47);
        Runtime_ReleaseHeapBlock(46);
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}
