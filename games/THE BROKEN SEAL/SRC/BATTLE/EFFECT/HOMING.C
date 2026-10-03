#include "RUNTIME_MEM.H"
#include "HEAP_STATE.H"
#include "MOTION_OBJECT.H"
#include "CANVAS.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

/* Heap-allocation cache: gWorkSlot[kind] holds kind's block address. */

void Camera_ApplyPhasedDelta(void);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
s32 Battle_GetObjectTableValueFar(s32 member_id);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void BattlePres_SetupTransitionSceneFar(s32 a, s32 b, s32 c, s32 d);
void Audio_PlayCue(s32 cue);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);

extern u16 ParticleStreams_CellOffsets[];
extern u8 HomingEmbers_Counts[];
extern u8 HomingEmbers_FlareWidths[];
extern u8 HomingEmbers_FlareHeights[];
extern u8 HomingEmbers_FlareBiasY[];
extern u16 HomingEmbers_FlareCells[];

/* Battle effect: embers leave the acting unit in a random spray, one every
   two frames, then turn towards the affected units in turn and burst into a
   six-cel flare where they land. The variant picks how many embers fly. */
void BattleFx_RunHomingEmbers(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 facing;
    void *sheet;
    s32 top;
    struct EffectPosition screen;
    DrawRectangle draw[2];
    struct MotionObject *source;
    s32 i;

    heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    facing = *(s32 *)((u8 *)gWorkSlot + 12 * 4);
    sheet = heap_cache[2];
    work->effect = effect;
    if (effect->side == 1)
        BattleFx_BeginCanvasLayer(1);
    else
        BattleFx_BeginCanvasLayer(0);
    Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 3);
    draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 2);
    draw[1] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER_ALTERNATE];
    REG_BLDALPHA = 0x1010;

    source = GetBattleObjectSlotFar(work->effect->actor)->object;
    top = source->y + Battle_GetObjectTableValueFar(work->effect->actor);
    for (i = 0; i != 64; i++) {
        struct EffectStep *ember = &((struct EffectStep *)Ram_MapCellBuffer)[i];
        s32 angle;
        s32 speed;

        angle = Random16();
        speed = (Random16() & 127) + 127;
        ember->velocity_x = (Trig_Sin(angle) * speed) >> 6;
        ember->velocity_y = (s32)(((Random16() & 127) - 16) << 16) >> 6;
        ember->velocity_z = (Trig_Cos(angle) * speed) >> 6;
        ember->x = source->x;
        ember->y = top;
        ember->z = source->z;
        ember->variant = -1;
    }

    work->camera_delta = 0;
    work->camera_phase = 0;
    Scheduler_AddOrUpdateCallback((s32)Camera_ApplyPhasedDelta, 0x480);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (frame = 0; frame != (HomingEmbers_Counts[work->effect->variant] >> 1) + 132; frame++) {
        if (frame > 16 && frame < 80)
            work->camera_delta = 0x100;
        else
            work->camera_delta = 0;
        if (frame == (HomingEmbers_Counts[work->effect->variant] >> 1) + 108)
            BattleEventRuntime_BeginPhaseFar(133);
        BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(facing, facing + 12);

        for (i = 0; i != HomingEmbers_Counts[work->effect->variant]; i++) {
            struct EffectStep *ember = &((struct EffectStep *)Ram_MapCellBuffer)[i];
            s32 start;

            start = i / 2;
            if (frame > start && ember->variant == -1) {
                s32 size;

                EffectPosition_ApplyBaseAndYOffset((s32 *)ember, &screen);
                screen.x >>= 1;
                if (screen.depth < 160)
                    screen.depth = 160;
                if (screen.depth > 799)
                    screen.depth = 799;
                size = 10 - (screen.depth - 160) / 64;
                draw[frame < start + 48 ? 1 : 0](canvas, (u8 *)sheet + ParticleStreams_CellOffsets[size - 1],
                    screen.x - size / 2, screen.y - size, size, size * 2);
                ember->x += ember->velocity_x;
                ember->y += ember->velocity_y;
                ember->z += ember->velocity_z;
            }
            if (frame > start + 48 && ember->variant == -1) {
                struct MotionObject *target;
                s32 vx;
                s32 vy;
                s32 vz;

                target = GetBattleObjectSlotFar(
                    work->effect->actors[i % work->effect->count])->object;
                vx = ember->velocity_x += (target->x - ember->x) >> 9;
                vy = ember->velocity_y += (target->y - ember->y) >> 9;
                vz = ember->velocity_z += (target->z - ember->z) >> 9;
                if (frame < start + 85) {
                    ember->velocity_x = vx * 60 / 64;
                    ember->velocity_y = vy * 60 / 64;
                    ember->velocity_z = vz * 60 / 64;
                }
                if (ember->y < 0) {
                    ember->variant = 0;
                    ember->x = screen.x;
                    ember->y = screen.y;
                    Audio_PlayCue(136);
                    ObjectGroup_UpdateMembers(
                        work->effect->actors[i % work->effect->count], 10, 5,
                        i % work->effect->count, 4);
                    work->shake_frames = 2;
                }
            }
        }

        for (i = 0; i != HomingEmbers_Counts[work->effect->variant]; i++) {
            struct EffectStep *ember = &((struct EffectStep *)Ram_MapCellBuffer)[i];

            if ((u32)ember->variant < 12) {
                s32 cel;
                u32 width;

                cel = ember->variant / 2;
                draw[1](canvas, (u8 *)work + HomingEmbers_FlareCells[cel],
                    ember->x - ((width = HomingEmbers_FlareWidths[cel]) >> 1),
                    ember->y + HomingEmbers_FlareBiasY[cel] - 56,
                    width, HomingEmbers_FlareHeights[cel]);
                ember->variant++;
            }
        }

        if (work->camera_phase == 0)
            work->camera_phase = 1;
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Scheduler_RemoveCallback((u32)Camera_ApplyPhasedDelta);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}
