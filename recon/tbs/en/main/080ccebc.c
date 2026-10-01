/* Battle effect: a spider web drawn as four rectangles on the canvas layer,
   centred between the first and last affected units, fading in over the
   first frames and out over the last of 63; frame 10 hits every unit.

   2026-10-01 slice-6: rewritten from the listing, 15 register-only
   instructions off (was 90 of every kind). Every instruction and the pool
   agree; only two high registers are swapped: the reference keeps the frame
   in r9 and the step in r10, this draft the other way round. The allocator
   hands out r8, r10, r9 in priority order, so the reference ranks size,
   step, frame; here the frame (20 weighted references) outranks the step
   (13). Found: the slot table is the constant Ram_WorkSlot (a label makes
   the slot 46 address hang off the slot 40 one); the ROM holds slot 40's
   address in the register that later holds the size, and BLDALPHA's address
   in the one that later holds the step, which only a variable assigned
   twice reproduces. Pinning the frame to r9 changes three instructions
   instead; the permuter (100 s) found only the declaration order kept
   here. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "B5_CONTEXT.H"

extern u8 ResourceId_SpiderWebSheet;

void BattleFx_BeginCanvasLayer(s32);
s32 BattleFx_EndCanvasLayer(void);
void Audio_PlayCue(s32);
void BattleEventRuntime_BeginPhaseFar(s32);
void ObjectGroup_TickMemberTimers(void);

void Unnamed_080ccebc(struct BattleEffectArgument *efx)
{
    struct EffectPosition first;
    struct EffectPosition last;
    void *dst;
    struct BattleEffectWork *work;
    s32 size;
    s32 step;
    s32 frame;
    s32 i;

    size = (s32)&Ram_WorkSlot[40];
    dst = *(void **)size;
    work = *(struct BattleEffectWork **)(size - 4);
    work->effect = efx;
    BattleFx_BeginCanvasLayer(2);
    step = 0x04000052;
    *(u16 *)0x04000020 = 0x100;
    *(u16 *)step = 0x1000;
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &first);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[work->effect->count - 1], &last);
    first.x += (last.x - first.x) / 2;
    *(s32 *)0x04000028 = (64 - first.x) << 8;
    Resource_LoadAndDecompress((s32)&ResourceId_SpiderWebSheet, work, 1, 1);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    Audio_PlayCue(143);
    for (frame = 0, step = 1, size = 32; frame != 63; frame++) {
        if (frame <= 8)
            *(u16 *)0x04000052 = (frame << 1) | 0x1000;
        if (frame > 53)
            *(u16 *)0x04000052 = (0x7c - (frame << 1)) | 0x1000;
        BattleEffect_LoadWork(46, 7, 7, 3, step);
        ((DrawRectangle)Ram_WorkSlot[46])(dst, work, 33, 41, size, size);
        Runtime_ReleaseHeapBlock(46);
        BattleEffect_LoadWork(46, 7, 7, 7, step);
        ((DrawRectangle)Ram_WorkSlot[46])(dst, work, 64, 41, size, size);
        Runtime_ReleaseHeapBlock(46);
        BattleEffect_LoadWork(46, 7, 7, 11, step);
        ((DrawRectangle)Ram_WorkSlot[46])(dst, work, 33, 72, size, size);
        Runtime_ReleaseHeapBlock(46);
        BattleEffect_LoadWork(46, 7, 7, 15, step);
        ((DrawRectangle)Ram_WorkSlot[46])(dst, work, 64, 72, size, size);
        Runtime_ReleaseHeapBlock(46);
        if (frame == 32)
            BattleEventRuntime_BeginPhaseFar(143);
        for (i = 0; i != work->effect->count; i++) {
            if (frame == 10)
                ObjectGroup_UpdateMembers(work->effect->actors[i], 7, -1, i, 8);
        }
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = step;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}
