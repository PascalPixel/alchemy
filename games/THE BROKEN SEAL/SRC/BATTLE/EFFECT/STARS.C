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

extern u8 gWorkSlot[];
extern u16 ParticleStreams_CellOffsets[];
/* Outer and inner radius of a star outline, by vertex parity. */
extern u8 SpinningStars_Radii[];
extern u8 gMapCellBuffer[];

void BattleFx_BeginCanvasLayer(s32 mode);
void *Resource_GetTableEntry(s32 id);
u32 Resource_DecodeType01(const void *source, void *destination);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void Graphics_UpdatePhasePalette(s32 frame, s32 red_phase, s32 green_phase, s32 blue_phase);
void BattleEventRuntime_BeginPhaseFar(s32 value);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 source, s32 destination);
void AudioCommand_PlayFar(s32 value);
void Camera_ApplyShake(s32 random_mask, s32 shake_range);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
s32 BattleFx_EndCanvasLayer(void);
struct BattleObject **GetBattleObjectSlotFar(s32 id);

/*
 * Eight star outlines leave the acting unit eight frames apart, fly to the
 * first target in twelve steps and bounce off the ground there, shaking the
 * screen and knocking every affected unit. Each outline is ten vertices
 * spun about its point and joined by twelve dots a side.
 */
void BattleFx_RunSpinningStars(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    DrawRectangle draw[2];
    s32 i;
    u8 *graphics;
    s32 facing;
    struct BattleObject *source;
    struct BattleObject *target;
    struct EffectStep *point;
    struct EffectStep *step;
    struct EffectPosition pos;
    s32 size;
    s32 j;
    s32 k;
    struct EffectStep *trails;

    heap_cache = (void **)(gWorkSlot + 39 * 4);
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    graphics = heap_cache[2];
    facing = *(s32 *)(gWorkSlot + 12 * 4);
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    work->transfer_mode = 2;
    work->transfer_value = 50;
    draw[0] = (DrawRectangle)heap_cache[7];
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    source = *GetBattleObjectSlotFar(work->effect->actor);
    target = *GetBattleObjectSlotFar(work->effect->actors[0]);
    for (i = 0, point = work->particles; i != 8; i++) {
        s32 delay = i * 8;

        point->x = source->x / 2;
        point->y = source->y + 0x780000;
        point->z = source->z;
        point->velocity_x = (target->x + ((s32)((Random16() & 0x7f) - 64) << 16) - point->x) / 12;
        point->velocity_y = (target->y - point->y + 0x140000) / 12;
        point->velocity_z = (target->z - point->z) / 12;
        point->variant = (s32)(Random16() & 15) + delay;
        point++;
    }

    size = 2;
    trails = (struct EffectStep *)gMapCellBuffer;
    for (frame = 0; frame != 128; frame++) {
        Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
        if (frame == 96)
            BattleEventRuntime_BeginPhaseFar(134);
        for (i = 0; i != 8; i++) {
            step = &work->particles[i];
            if (frame >= step->variant) {
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork(facing, facing + 12);
                EffectPosition_ApplyBaseAndYOffset((s32 *)step, &pos);
                pos.x >>= 1;
                if (pos.x >= -8 && pos.x <= 127) {
                    if (pos.y <= 127) {
                        if (pos.y >= -8) {
                            for (j = 0; j != 10; j++) {
                                trails[i * 10 + j].velocity_x = pos.x
                                    + ((Trig_Sin(j * 0x199a - ((frame - step->variant) << 11))
                                        * SpinningStars_Radii[j & 1]) / 2 >> 16);
                                trails[i * 10 + j].velocity_y = pos.y
                                    - (Trig_Cos(j * 0x199a - ((frame - step->variant) << 11))
                                        * SpinningStars_Radii[j & 1] >> 16);
                            }
                            for (j = 0; j != 10; j++) {
                                struct EffectStep *from = &trails[j + i * 10];
                                struct EffectStep *to = &trails[i * 10 + (j + 1) % 10];

                                for (k = 0; k != 12; k++) {
                                    s32 x = from->velocity_x + (to->velocity_x - from->velocity_x) * k / 12;
                                    s32 y = from->velocity_y + (to->velocity_y - from->velocity_y) * k / 12;

                                    draw[0](canvas,
                                        graphics + ParticleStreams_CellOffsets[size - 1],
                                        x - size / 2, y - size, size, size * 2);
                                }
                            }
                        }
                    }
                }
                if (step->y <= 0x1dffff) {
                    step->velocity_y = -step->velocity_y;
                    step->velocity_x /= 2;
                    step->velocity_z /= 2;
                    work->shake_frames = 4;
                    AudioCommand_PlayFar(134);
                    for (j = 0; j != work->effect->count; j++)
                        ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 8);
                }
                step->x += step->velocity_x;
                step->y += step->velocity_y;
                step->z += step->velocity_z;
            }
        }
        Camera_ApplyShake(4, 4);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
