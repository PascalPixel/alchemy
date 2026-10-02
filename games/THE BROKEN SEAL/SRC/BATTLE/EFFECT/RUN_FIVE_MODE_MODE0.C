#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "B5_CONTEXT.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"

extern u8 gBattleFxWork[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_EndCanvasLayer(void);
struct B5Context *GetBattleObjectSlotFar(s32 id);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void AudioCommand_PlayFar(s32 value);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 first, s32 last);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);

void BattleFx_RunFiveMode(struct BattleEffectArgument *effect, s32 mode);

/* battle/effects/five_mode/run_mode0.c */
void BattleFx_RunFiveModeMode0(struct BattleEffectArgument *effect)
{
    BattleFx_RunFiveMode(effect, 0);
}

/* battle/effects/five_mode/run_mode1.c */
void BattleFx_RunFiveModeMode1(struct BattleEffectArgument *effect)
{
    BattleFx_RunFiveMode(effect, 1);
}

/* battle/effects/five_mode/run_mode2.c */
void BattleFx_RunFiveModeMode2(struct BattleEffectArgument *effect)
{
    BattleFx_RunFiveMode(effect, 2);
}

/* battle/effects/five_mode/run_mode4.c */
void BattleFx_RunFiveModeMode4(struct BattleEffectArgument *effect)
{
    BattleFx_RunFiveMode(effect, 4);
}

/* battle/effects/five_mode/run_mode3.c */
void BattleFx_RunFiveModeMode3(struct BattleEffectArgument *effect)
{
    BattleFx_RunFiveMode(effect, 3);
}

/* battle/effects/five_mode/run_mode1_b.c */
void BattleFx_RunFiveModeMode1B(struct BattleEffectArgument *effect)
{
    BattleFx_RunFiveMode(effect, 1);
}

/* Four bytes for each mode: whether the streak flickers through three
   sheets, whether the target is struck, which motion the target takes (-1
   for none) and whether the target trembles. */
extern s8 FiveMode_Records[];
/* How far a trembling target moves on each of eight frames. */
extern s8 FiveMode_Tremble[];

/* Battle effect: three streaks fly from the actor to each target in turn,
   one target every 32 frames, while the background ripples. The streak count
   and the height the streaks fly at are variables, as Camelot had them: the
   compiler multiplies by the one and adds the other from a register. */
void BattleFx_RunFiveMode(struct BattleEffectArgument *effect, s32 mode)
{
    struct EffectPosition position;
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 k;
    DrawRectangle draw_lower;
    DrawRectangle draw_upper;
    struct BattleCamera *camera;
    struct MotionObject *source;
    struct MotionObject *target;
    s32 resource;
    s32 frame;
    s32 j;
    s32 streaks = 3;
    s32 lift = 0x140000;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    camera = *(struct BattleCamera **)((u8 *)heap_cache - 108);
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    if (work->effect->side == 0) {
        BattleEffect_LoadWork(46, 7, 7, 11, 2);
        draw_upper = heap_cache[7];
        BattleEffect_LoadWork(47, 7, 7, 3, 2);
        draw_lower = heap_cache[8];
    } else {
        BattleEffect_LoadWork(46, 7, 7, 15, 2);
        draw_upper = heap_cache[7];
        BattleEffect_LoadWork(47, 7, 7, 7, 2);
        draw_lower = heap_cache[8];
    }
    Resource_LoadAndDecompress((s32)&ResourceId_FiveModeImage, work, 0, 0);
    switch (mode) {
    case 0:
        resource = (s32)&ResourceId_EmberStreakSheet;
        break;
    case 1:
        resource = (s32)&ResourceId_LimePalette;
        break;
    case 2:
        resource = (s32)&ResourceId_VioletPaletteD;
        break;
    case 3:
        resource = (s32)&ResourceId_RedPaletteB;
        break;
    case 4:
    default:
        resource = (s32)&ResourceId_CyanPalette;
        break;
    }
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry(resource), 128);
    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    source = GetBattleObjectSlotFar(work->effect->actor)->object;
    {
        s32 total = streaks * work->effect->count * 6 + 48;

        for (k = 0; k != work->effect->count; k++) {
            target = GetBattleObjectSlotFar(work->effect->actors[k])->object;
            for (j = 0; j != streaks; j++) {
                struct EffectStep *streak = &work->particles[k * streaks + j];

                streak->x = source->x;
                streak->y = source->y + lift;
                streak->z = source->z;
                streak->velocity_x = (target->x - streak->x) / 24;
                streak->velocity_y = (target->y + lift - streak->y) / 24;
                streak->velocity_z = (target->z - streak->z) / 24;
                streak->variant = 0;
            }
        }

        for (frame = 0; frame != total; frame++) {
            s32 *row = work->bg2_x;

            for (k = 0; k != 160; k++)
                *row++ = (0x80000 - Trig_Sin((frame + k) << 12) * 2) >> 10;
            if (frame > total - 16)
                *(volatile u16 *)0x04000052 = (total - frame) | 0x1000;
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
            for (k = 0; k != work->effect->count; k++) {
                if (frame >= k * 32) {
                    for (j = 0; j != streaks; j++) {
                        if (frame >= k * 32 + j * 6) {
                            struct EffectStep *streak = &work->particles[k * streaks + j];
                            s32 step;

                            EffectPosition_ApplyBaseAndYOffset((s32 *)streak, &position);
                            position.x >>= 1;
                            step = streak->variant / 8;
                            if (step > 5)
                                step = 5;
                            if (FiveMode_Records[mode * 4]) {
                                u8 *cell = &work->sheet[step * 800 + frame / 2 % 3 * 4800];

                                draw_upper(canvas, cell, position.x - 10, position.y - 40, 20, 40);
                                draw_lower(canvas, cell, position.x - 10, position.y, 20, 40);
                            } else {
                                u8 *cell = &work->sheet[step * 800 + 0x2580];

                                draw_upper(canvas, cell, position.x - 10, position.y - 40, 20, 40);
                                draw_lower(canvas, cell, position.x - 10, position.y, 20, 40);
                            }
                            EffectStep_AdvanceWithGravity3D(streak, 64, 0);
                            streak->variant++;
                        }
                    }
                    if (FiveMode_Records[mode * 4 + 3] && frame >= k * 32 + 30
                        && frame < k * 32 + 62) {
                        struct MotionObject *struck =
                            GetBattleObjectSlotFar(work->effect->actors[k])->object;

                        struck->x += FiveMode_Tremble[(frame - k * 32 - 30) % 8] << 16;
                        if (struck->x > 0)
                            struck->x += 0x8000;
                        else
                            struck->x -= 0x8000;
                        ObjectGroup_UpdateMembers(work->effect->actors[k], -1, 5, -1, 0);
                    }
                    if (FiveMode_Records[mode * 4 + 1]) {
                        if (frame == k * 32 + 24) {
                            AudioCommand_PlayFar(133);
                            if (k == 0)
                                BattleEventRuntime_BeginPhaseFar(-1);
                            ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 8);
                        }
                        if (frame == k * 32 + 40)
                            ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 8);
                    }
                    if (FiveMode_Records[mode * 4 + 2] != -1 && frame == k * 32 + 24) {
                        work->shake_frames = 4;
                        BattleMotion_ApplyVariantMotionFar(work->effect->actors[k],
                            FiveMode_Records[mode * 4 + 2]);
                    }
                }
            }
            Camera_ApplyShake(8, 8);
            ObjectGroup_TickMemberTimers();
            work->transfer_pending = 1;
            WaitFrames(1);
        }
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
