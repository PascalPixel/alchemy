/* Draft, not exact (2026-09-29): permuter score 1193 (was the unparsed
   m2c listing). The cell tables now carry labels in unidentified.s and the
   three resources are named rows. What lines up: loading the kind-46
   blitter in each branch after BattleEffect_LoadWork lets jump2 cross-jump
   the call and the load into one tail, as the reference has; sharing one
   temporary between the canvas word and the removed callback gives the
   canvas r9 and spills the blitter (a fake, to drop if another route to
   that allocation appears). Remaining: the reference hoists the cell
   offsets table into fp before the frame loop and rematerialises
   work + 0x77a8 at both shake stores (the spilled pseudo is the frame's
   extra word, 32 bytes against 28); here loop.c hoists the shake address
   instead and the offsets table is reloaded in each branch. The m2c form
   (a tableA local) does the same. Eight minutes of permutation from here
   found nothing lower. */
/* alchemy permute: BattleFx_RunTwoResource against recon/tbs/raw/080ccc38.s: score 1373 (11 register-only, 3 stack-only, 15 operand, 5 reordered, 4 inserted, 3 deleted).
   Job 0, iteration 1200; rewrites: 1x share one temporary between two statements. */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "RESOURCE_IDS.H"

extern u8 gBattleFxWork[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
void *Resource_GetTableEntry(s32 id);
void **GetBattleObjectSlotFar(s32 member_id);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void BattleMotion_ApplyVariantMotionFar(s32 member_id, s32 variant);
void Audio_PlayCue(s32 cue);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
s32 BattleFx_EndCanvasLayer(void);

/* Six animation cells: width, height, the source offset (cells 0 to 3 in
   the work block, 4 and 5 in the map cell buffer), the x for either side
   and the vertical bias. */
extern u8 TwoResource_CellWidths[];
extern u8 TwoResource_CellHeights[];
extern u16 TwoResource_CellSourceOffsets[];
extern u8 TwoResource_CellX[];
extern s8 TwoResource_CellBiasY[];

#define WORK_EFX (work->effect)

/* Plays a six-cell sheet whose first four cells load into the work block
   and last two into the map cell buffer, four frames a cell, while the
   screen shakes; mode 1 runs 74 frames and launches the target, other
   modes 48. */
void BattleFx_RunTwoResource(struct BattleEffectArgument *efx, s32 mode)
{
    u32 *cache;
    u32 *entry;
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangle draw;
    s32 *object;
    struct EffectPosition position;
    s32 frames;
    s32 frame;
    s32 cell;
    u16 *offsets = TwoResource_CellSourceOffsets;
    u32 tmp;

    cache = (u32 *)gBattleFxWork;
    entry = cache;
    work = (struct BattleEffectWork *)*entry++;
    tmp = *entry;
    canvas = (void *)tmp;
    work->effect = efx;
    BattleFx_BeginCanvasLayer(0);
    *(s16 *)0x04000020 = 0x100;
    if (WORK_EFX->side == 1) {
        BattleEffect_LoadWork(46, 7, 7, 3, 1);
        draw = (DrawRectangle)cache[46 - 39];
    } else {
        BattleEffect_LoadWork(46, 7, 7, 7, 1);
        draw = (DrawRectangle)cache[46 - 39];
    }
    Resource_LoadAndDecompress((s32)&ResourceId_MagentaSwirlSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_MagentaTailSheet, (void *)0x02010000, 1, 0);
    if (mode == 0)
        Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_LimePalette), 128);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    object = *GetBattleObjectSlotFar(WORK_EFX->actors[0]);
    EffectPosition_ApplyStepAndYOffset(WORK_EFX->actors[0], &position);
    if (WORK_EFX->side == 0)
        *(s32 *)0x04000028 = (16 - position.x) << 8;
    else
        *(s32 *)0x04000028 = (112 - position.x) << 8;
    frames = 74;
    if (mode != 1)
        frames = 48;
    for (frame = 0; frame != frames; frame++) {
        cell = frame / 4;
        if (cell <= 5) {
            if (cell <= 3)
                draw(canvas, (u8 *)work + offsets[cell], TwoResource_CellX[cell + WORK_EFX->side * 6], TwoResource_CellBiasY[cell] + 32, TwoResource_CellWidths[cell], TwoResource_CellHeights[cell]);
            else
                draw(canvas, (u8 *)0x02010000 + offsets[cell], TwoResource_CellX[cell + WORK_EFX->side * 6], TwoResource_CellBiasY[cell] + 32, TwoResource_CellWidths[cell], TwoResource_CellHeights[cell]);
        }
        if (frame == 8) {
            if (mode == 0) {
                BattleEventRuntime_BeginPhaseFar(0x85);
                BattleMotion_ApplyVariantMotionFar(WORK_EFX->actors[0], 1);
            } else {
                Audio_PlayCue(0x86);
                ObjectGroup_UpdateMembers(WORK_EFX->actors[0], 7, 5, 0, 4);
            }
            work->shake_frames = 8;
        }
        if (mode == 1) {
            if (frame == 13) {
                object[10] = 0xc0000;
                object[18] = 0x7851;
                object[17] = 0x4000;
            }
            if (frame == 65) {
                work->shake_frames = 4;
                BattleEventRuntime_BeginPhaseFar(0x86);
            }
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    tmp = (u32)BattlePresentation_ProcessPendingGraphicsTransfer;
    Scheduler_RemoveCallback(tmp);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
