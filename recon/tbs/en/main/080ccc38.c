#include "CANVAS.H"
#include "MOTION_OBJECT.H"
#include "RUNTIME_MEM.H"
#include "HEAP_STATE.H"
#include "B5_CONTEXT.H"
/*
 * BattleFx_RunTwoResource — unmatched canonical draft.
 *
 * Closed 2026-10-02 trial: replacing the unread DrawRectangle draw[2]
 * reservation with a used scalar and four matching scalar selectors emitted
 * 636B in all six editions against native 644B. The whole PRESENT3 owner
 * changed 2860 to 2852B; its 1632B prefix and 584B SpiderWeb were unchanged.
 * The native 32B local frame became 24B, callback sp+12 became r9, canvas r9
 * became fp, and position sp+20 became sp+12. The pre-loop cell-offset-table
 * ldr/mov pair and two callback reloads disappeared. Complete comparison had
 * 385 positional byte differences in JA/EN/IT and 386 in DE/ES/FR, including
 * 8 absent bytes. All 21 pool values matched; 20/22 physical call targets
 * matched, with two r9 indirect stubs instead of native r4. That unread
 * storage/selector/frame/scheduling axis remains closed.
 *
 * T0, 2026-10-03: one natural owner/contract baseline uses HEAP_STATE slots
 * 39/40/46, MotionObject fields, and maintained Camera/Audio/B5 contracts.
 * Global TBS flags and the pinned era assembler emit 640B / native 644B.
 * The complete relocated raw 644B reproduces the EN ROM. T0 has 498 differing
 * bytes in the shared 640B and 4 missing bytes (positional, not an instruction
 * score). All 32 candidate / 34 native relocations resolve. All 22 physical
 * calls retain order; 20 targets match, but both blitter calls use
 * _call_via_sl (r10) rather than _call_via_r4. There are 21 pool words each:
 * 20 match in order; gWorkSlot replaces the interior gBattleFxWork cell.
 * Natural padding is 4B each. Instructions/stores are 254/20 versus 256/21;
 * Branch instructions are 43 each, including 22 BL calls and the final BX;
 * non-call/non-return branches are 20. Pools and alignment are excluded.
 * Local frame is 24/32B, saved registers 32B each, total 56/64B. Position is
 * sp+12/native sp+20, callback r10/native sp+12, canvas r11/native r9.
 * Whole-owner slot calculations and differing instruction presence remain;
 * this does not justify another scheduling/storage/device attempt.
 *
 * Native pointer lifetimes and live effect reads are preserved. The existing
 * copier-address-order FAKEMATCH remains unchanged. No new device, layout,
 * header, resource ID, compiler option or routing was introduced. T0 is
 * frozen after this single compile; no adoption or other-edition claim.
 */
#include "TYPES.H"
#include "EFFECT_STEP.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "BATTLE_PRESENTATION.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "RESOURCE.H"

void BattleEventRuntime_BeginPhaseFar(s32 cue);
void BattleMotion_ApplyVariantMotionFar(s32 member_id, s32 variant);
void AudioCommand_PlayFar(s32 cue);
void Camera_ApplyShake(s32 random_mask, u32 shake_range);
void ObjectGroup_TickMemberTimers(void);

extern u8 TwoResource_CellWidths[];
extern u8 TwoResource_CellHeights[];
extern u16 TwoResource_CellSourceOffsets[];
extern u8 TwoResource_CellX[];
extern s8 TwoResource_CellBiasY[];

void BattleFx_RunTwoResource(struct BattleEffectArgument *efx, s32 mode)
{
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangle draw;
    struct MotionObject *object;
    struct EffectPosition position;
    s32 frames;
    s32 frame;
    s32 cell;

    work = gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
    canvas = gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
    work->effect = efx;
    BattleFx_BeginCanvasLayer(0);
    *(u16 *)0x04000020 = 0x100;
    if (work->effect->side == 1) {
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 1);
        draw = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    } else {
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 7, 1);
        draw = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
    }
    Resource_LoadAndDecompress((s32)&ResourceId_MagentaSwirlSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_MagentaTailSheet, Ram_MapCellBuffer, 1, 0);
    if (mode == 0) {
        void *palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
        /* FAKEMATCH: the copier address is set before the costly 0x05000000 operand, as the reference loads it */
        s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

        copy((void *)0x05000000, palette, 128);
    }
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    object = GetBattleObjectSlotFar(work->effect->actors[0])->object;
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
                draw(canvas, (u8 *)work + TwoResource_CellSourceOffsets[cell],
                    TwoResource_CellX[cell + work->effect->side * 6], TwoResource_CellBiasY[cell] + 32,
                    TwoResource_CellWidths[cell], TwoResource_CellHeights[cell]);
            else
                draw(canvas, Ram_MapCellBuffer + TwoResource_CellSourceOffsets[cell],
                    TwoResource_CellX[cell + work->effect->side * 6], TwoResource_CellBiasY[cell] + 32,
                    TwoResource_CellWidths[cell], TwoResource_CellHeights[cell]);
        }
        if (frame == 8) {
            if (mode == 0) {
                BattleEventRuntime_BeginPhaseFar(0x85);
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 1);
            } else {
                AudioCommand_PlayFar(0x86);
                ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 4);
            }
            work->shake_frames = 8;
        }
        if (mode == 1) {
            if (frame == 13) {
                object->velocity_y = 0xc0000;
                object->vertical_motion_strength = 0x7851;
                object->vertical_motion_phase = 0x4000;
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
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}
