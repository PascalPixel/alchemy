/* 2026-09-30 (Mercury's helper): EXACT, 644 of 644 bytes with two
   FAKEMATCHes (an unused draw[1] slot for the spare frame word, and a
   tagged copier temporary so the IWRAM copy address loads before
   0x05000000). Adopting it after PREPARE_SCENE.C needs that host's older
   declarations reconciled first: its BattleFx_RunTwoResource(s32, s32)
   prototype and its local Scheduler_RemoveCallback(void (*)(void)) clash
   with this definition and with CALLBACK_SCHEDULER.H. Compile it under #if
   defined(TBS_EDITION_EN) until the other editions adopt theirs. */
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

extern u8 TwoResource_CellWidths[];
extern u8 TwoResource_CellHeights[];
extern u16 TwoResource_CellSourceOffsets[];
extern u8 TwoResource_CellX[];
extern s8 TwoResource_CellBiasY[];

/* Plays a six-cell sheet whose first four cells load into the work block
   and last two into the map cell buffer, four frames a cell, while the
   screen shakes; mode 1 runs 74 frames and launches the target, other
   modes 48. */
void BattleFx_RunTwoResource(struct BattleEffectArgument *efx, s32 mode)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangle draw[2]; /* FAKEMATCH: only draw[0] is used; the pair is the reference's spare frame word */
    s32 *object;
    struct EffectPosition position;
    s32 frames;
    s32 frame;
    s32 cell;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = efx;
    BattleFx_BeginCanvasLayer(0);
    *(u16 *)0x04000020 = 0x100;
    if (work->effect->side == 1) {
        BattleEffect_LoadWork(46, 7, 7, 3, 1);
        draw[0] = (DrawRectangle)heap_cache[46 - 39];
    } else {
        BattleEffect_LoadWork(46, 7, 7, 7, 1);
        draw[0] = (DrawRectangle)heap_cache[46 - 39];
    }
    Resource_LoadAndDecompress((s32)&ResourceId_MagentaSwirlSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_MagentaTailSheet, (void *)0x02010000, 1, 0);
    if (mode == 0) {
        void *palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
        /* FAKEMATCH: the copier address is set before the costly 0x05000000 operand, as the reference loads it */
        s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

        copy((void *)0x05000000, palette, 128);
    }
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    object = *GetBattleObjectSlotFar(work->effect->actors[0]);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &position);
    if (work->effect->side == 0)
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
                draw[0](canvas, (u8 *)work + TwoResource_CellSourceOffsets[cell],
                    TwoResource_CellX[cell + work->effect->side * 6], TwoResource_CellBiasY[cell] + 32,
                    TwoResource_CellWidths[cell], TwoResource_CellHeights[cell]);
            else
                draw[0](canvas, (u8 *)0x02010000 + TwoResource_CellSourceOffsets[cell],
                    TwoResource_CellX[cell + work->effect->side * 6], TwoResource_CellBiasY[cell] + 32,
                    TwoResource_CellWidths[cell], TwoResource_CellHeights[cell]);
        }
        if (frame == 8) {
            if (mode == 0) {
                BattleEventRuntime_BeginPhaseFar(0x85);
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
            } else {
                Audio_PlayCue(0x86);
                ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
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
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
