#include "TYPES.H"
#include "SYSTEM.H"
#include "CALLBACK_SCHEDULER.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "RESOURCE_IDS.H"

/*
 * Battle effect that pops a marker over each affected unit in turn. Unit i
 * appears on frame i * 16 with sound cue 143 and stays for 72 frames: a
 * fixed 16x20 cel from the effect sheet and above it a 16x12 cel that cycles
 * through nine frames of six ticks from a random starting phase. The far
 * side's canvas is shifted 112 pixels left, and the effect runs for
 * (count + 1) * 32 frames.
 */

extern void *gWorkSlot[];

void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);

void BattleFx_RunTargetMarkers(struct BattleEffectArgument *efx)
{
    void **slot;
    struct BattleEffectWork *work;
    void *canvas;
    /* FAKEMATCH: the family's blitter pair; this effect loads only one. */
    DrawRectangle draw[2];
    struct EffectStep *marker;
    s32 i;
    s32 frame;
    s32 start;
    s32 cel;
    s32 y;
    struct EffectPosition pos;

    slot = &gWorkSlot[40];
    canvas = slot[0];
    work = slot[-1];
    work->effect = efx;
    BattleFx_BeginCanvasLayer(1);
    *(s16 *)0x04000020 = 0x100;
    *(s16 *)0x04000050 = 0;
    Resource_LoadAndDecompress((s32)&ResourceId_FlameColumnSheet, work, 1, 1);
    if (work->effect->side == 1) {
        *(s32 *)0x04000028 = -0x7000;
    }
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    draw[0] = (DrawRectangle)slot[6];
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    for (i = 0; i != work->effect->count; i++) {
        work->particles[i].variant = Random16() & 63;
    }
    frame = 0;
    while (frame != (work->effect->count << 5) + 32) {
        if (frame == 32) {
            BattleEventRuntime_BeginPhaseFar(0);
        }
        i = 0;
        if (work->effect->count != 0) {
            marker = work->particles;
            do {
                start = i << 4;
                if (frame == start) {
                    Audio_PlayCue(143);
                }
                if (frame < start) {
                    goto next;
                }
                if (frame >= start + 72) {
                    goto next;
                }
                EffectPosition_ApplyStepAndYOffset(work->effect->actors[i], &pos);
                if (work->effect->side == 1) {
                    pos.x -= 112;
                }
                y = pos.y;
                pos.y = y - 16;
                draw[0](canvas, (u8 *)work + 1728, pos.x - 8, y - 20, 16, 20);
                if (frame < start) {
                    goto next;
                }
                cel = ((frame - start) + marker->variant) / 6 % 9;
                draw[0](canvas, (u8 *)work + cel * 192, pos.x - 8, pos.y - 16, 16, 12);
            next:
                marker++;
                i++;
            } while (i != work->effect->count);
        }
        work->transfer_pending = 1;
        WaitFrames(1);
        frame += 1;
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
