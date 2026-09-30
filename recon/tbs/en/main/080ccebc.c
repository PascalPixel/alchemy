/* Draft, not exact (2026-09-26): 576 of 584 bytes, 281 differing halfwords,
   125 aligned edits. Whole-owner baseline was 584/235/181 with divergent
   topology. ROM-evidenced corrections: live effect argument/count reloads,
   word BG2X store, direct BLDALPHA stores (not BG2X read-modify-writes),
   transfer_pending at +0x7824, pooled resource 0x59 and immediate interval
   0x480. Typed state gives 576/280/148 and equal topology; explicit frame
   and member back edges give the retained 576/281/125. The 32-byte frame
   and position-helper accesses match. Remaining: initial cell/IO address
   lifetimes, high-register assignment, rectangle-slot address hoisting,
   indexed effect-slot loads, and eight missing bytes. Two fresh hypotheses
   preserved at the lane checkpoint; no exact-C credit.
   Wraps marked FAKEMATCH only move scheduling.
   2026-09-29 (alchemy permute scorer): the draft scored 2725 (56
   register-only, 16 operand, 10 reordered, 6 inserted, 9 deleted). This body
   is the permuter's best after a 300-second search (about 40,000 candidates):
   2330 (60 register-only, 15 operand, 10 reordered, 5 inserted, 6
   deleted). Its rewrites are search output, not a
   reading of the ROM; the literal addresses still keep it a draft.
 */
#include "TYPES.H"
extern u8 ResourceId_SpiderWebSheet;
#include "GLOBAL_CELLS.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"

void BattleFx_BeginCanvasLayer(s32);
s32 BattleFx_EndCanvasLayer(void);
s32 Scheduler_AddOrUpdateCallback(s32, s32);
void Scheduler_RemoveCallback(s32);
void Audio_PlayCue(s32);
void WaitFrames(s32);
void Runtime_ReleaseHeapBlock(s32);
void BattleEventRuntime_BeginPhaseFar(s32);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
void ObjectGroup_TickMemberTimers(void);
void EffectPosition_ApplyStepAndYOffset(s32, struct EffectPosition *);

void Unnamed_080ccebc(void *arg0)
{
    u32 *cell = (u32 *)0x03001ef0;
    struct BattleEffectWork *base = (struct BattleEffectWork *)*(cell - 1);
    void *second = (void *)*cell;
    struct EffectPosition local1;
    struct EffectPosition local2;
    s32 mid;
    s32 frame;
    s32 flash;
    s32 shade;
    s32 i;
    u16 *tmp2;
    u8 *tmp3;

    base->effect = arg0;
    tmp2 = (u16 *)0x04000020;
    BattleFx_BeginCanvasLayer(2);
    tmp3 = &ResourceId_SpiderWebSheet;
    *tmp2 = 0x100;
    *(u16 *)0x04000052 = 0x1000;
    EffectPosition_ApplyStepAndYOffset(((struct BattleEffectArgument *)base->effect)->actors[0], &local1);
    EffectPosition_ApplyStepAndYOffset(((struct BattleEffectArgument *)base->effect)->actors[((struct BattleEffectArgument *)base->effect)->count - 1], &local2);
    mid = local1.x + (local2.x - local1.x) / 2;
    local1.x = mid;
    *(s32 *)0x04000028 = (64 - mid) << 8;
    Resource_LoadAndDecompress((s32)tmp3, base, 1, 1);
    base->transfer_mode = 1;
    base->transfer_value = 0;
    Scheduler_AddOrUpdateCallback(0x080cd261, 0x480);
    Audio_PlayCue(0x8f);
    frame = 0;
    flash = 1;
    shade = 32;
next_frame:
    {
        if (frame <= 8)
            *(u16 *)0x04000052 = (frame << 1) | (u32)0x1000;
        if (53 < frame)
            *(u16 *)0x04000052 = (0x7c - (frame << 1)) | 0x1000;
        BattleEffect_LoadWork(0x2e, 7, 7, 3, flash);
        (*(DrawRectangleFn *)0x03001F08)(second, (void *)base, 33, 41, shade, shade);
        do {
            Runtime_ReleaseHeapBlock(0x2e);
        } while (0);
        /* FAKEMATCH */
        BattleEffect_LoadWork(0x2e, 7, 7, 7, flash);
        (*(DrawRectangleFn *)0x03001F08)(second, (void *)base, 64, 41, shade, shade);
        do {
            Runtime_ReleaseHeapBlock(0x2e);
        } while (0);
        /* FAKEMATCH */
        BattleEffect_LoadWork(0x2e, 7, 7, 11, (s32)flash);
        (*(DrawRectangleFn *)0x03001F08)(second, (void *)base, 33, 72, shade, shade);
        Runtime_ReleaseHeapBlock(0x2e);
        BattleEffect_LoadWork(0x2e, 7, 7, 15, flash);
        ((DrawRectangleFn *)0x03001F08)[0](second, (void *)base, 64, 72, shade, shade);
        Runtime_ReleaseHeapBlock(0x2e);
        if (frame == 32)
            BattleEventRuntime_BeginPhaseFar(0x8f);
        i = 0;
        if (((struct BattleEffectArgument *)base->effect)->count) {
            s32 offset = 36;
        next_actor:
            {
                if (frame == 10) {
                    void *p = base->effect;
                    s32 tmp;
                    tmp = -1;
                    ObjectGroup_UpdateMembers(*(s16 *)((u8 *)p + offset), 7, tmp, i, 8);
                }
                i += 1;
                offset += 2;
                if (i != base->effect->count)
                    goto next_actor;
            }
        }
        ObjectGroup_TickMemberTimers();
        base->transfer_pending = flash;
        WaitFrames(1);
        frame += 1;
        if (frame != 63)
            goto next_frame;
    }
    Scheduler_RemoveCallback(0x080cd261);
    BattleFx_EndCanvasLayer();
}
