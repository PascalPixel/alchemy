#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SYSTEM.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE_IDS.H"

extern u8 gBattleFxWork[];
extern u8 gMapCellBuffer[];
extern BattleEffectDrawRectangle Data_03001e50[];

void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_EndCanvasLayer(void);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleMotion_ApproachTargetFar(s32 actor, s32 target, s32 frames, s32 speed);
void BattleMotion_ApplyVariantMotionFar(s32 id, s32 mode);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void Audio_PlayCue(s32 cue);

/* Battle effect: load one of three column pictures (by variant) into the
   work canvas and gMapCellBuffer, move the actor towards its
   target, then for 21 frames blit the four 120x120 quarters in turn, flood
   the canvas with colour 0x3f for frames 16..19, start event phase 134 at
   frame 18 and shake the target at frame 20, with the camera shaking
   throughout. */
void BattleFx_RunRevealColumn(struct BattleEffectArgument *effect, s32 variant)
{
    void **heap_cache;
    struct BattleEffectWork *work;
    void *canvas;
    BattleEffectDrawRectangle draw[2];
    s32 frame;

    heap_cache = (void **)gBattleFxWork;
    work = *heap_cache++;
    canvas = *heap_cache;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    *(volatile u16 *)0x04000050 = 0;
    if (variant == 0) {
        Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskE, work, 1, 0);
        Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskF, gMapCellBuffer, 1, 1);
    } else if (variant == 1) {
        Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskC, work, 1, 0);
        Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskD, gMapCellBuffer, 1, 1);
    } else {
        Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskA, work, 1, 0);
        Resource_LoadAndDecompress((s32)&ResourceId_RevealMaskB, gMapCellBuffer, 1, 1);
    }
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    if (variant == 1)
        BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0x80000);
    else
        BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
    WaitFrames(16);
    if (work->effect->side == 1)
        BattleEffect_LoadWork(46, 7, 7, 7, 0);
    else
        BattleEffect_LoadWork(46, 7, 7, 3, 0);
    draw[0] = Data_03001e50[46];
    Audio_PlayCue(212);
    for (frame = 0; frame != 21; frame++) {
        if (frame <= 3)
            draw[0](canvas, work, 0, 0, 120, 120);
        else if (frame <= 7)
            draw[0](canvas, (u8 *)work + 0x3840, 0, 0, 120, 120);
        else if (frame <= 11)
            draw[0](canvas, gMapCellBuffer, 0, 0, 120, 120);
        else if (frame <= 15)
            draw[0](canvas, gMapCellBuffer + 0x3840, 0, 0, 120, 120);
        if (frame >= 16 && frame <= 19)
            Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        if (frame == 18)
            BattleEventRuntime_BeginPhaseFar(134);
        if (frame == 20) {
            work->shake_frames = 8;
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
        }
        Camera_ApplyShake(16, 16);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}
