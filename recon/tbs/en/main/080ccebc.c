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
   Wraps marked FAKEMATCH only move scheduling. */
#include "TYPES.H"
extern u8 Value_00000059;
#include "GLOBAL_CELLS.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"

void Func_080cd594(s32);
s32 Func_080cdbc0(void);
s32 Func_080041d8(s32, s32);
void Func_08004278(s32);
void Func_080f9010(s32);
void Func_080030f8(s32);
void Func_08002dd8(s32);
void Func_080b50e8(s32);
void Func_080d6888(s32, s32, s32, s32, s32);
void Func_080cd52c(void);
void EffectPosition_ApplyStepAndYOffset(s32, struct EffectPosition *);

void Func_080ccebc(void *arg0)
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
    s32 count;

    base->effect = arg0;
    Func_080cd594(2);
    *(u16 *)0x04000020 = 0x100;
    *(u16 *)0x04000052 = 0x1000;

    EffectPosition_ApplyStepAndYOffset(
        ((struct BattleEffectArgument *)base->effect)->actors[0], &local1);
    EffectPosition_ApplyStepAndYOffset(
        ((struct BattleEffectArgument *)base->effect)->actors[
            ((struct BattleEffectArgument *)base->effect)->count - 1], &local2);
    mid = local1.x + (local2.x - local1.x) / 2;
    local1.x = mid;

    *(s32 *)0x04000028 = (64 - mid) << 8;
    Resource_LoadAndDecompress((s32)&Value_00000059, base, 1, 1);

    base->transfer_mode = 1;
    base->transfer_value = 0;
    Func_080041d8(0x080cd261, 0x480);

    Func_080f9010(0x8f);

    frame = 0;
    flash = 1;
    shade = 32;
next_frame:
    {
        if (frame <= 8)
            *(u16 *)0x04000052 = (frame << 1) | 0x1000;
        if (frame > 53)
            *(u16 *)0x04000052 = (0x7c - (frame << 1)) | 0x1000;

        BattleEffect_LoadWork(0x2e, 7, 7, 3, flash);
        (*(DrawRectangleFn *)0x03001F08)(
            second, (void *)base, 33, 41, shade, shade);
        do { Func_08002dd8(0x2e); } while (0); /* FAKEMATCH */

        BattleEffect_LoadWork(0x2e, 7, 7, 7, flash);
        (*(DrawRectangleFn *)0x03001F08)(
            second, (void *)base, 64, 41, shade, shade);
        do { Func_08002dd8(0x2e); } while (0); /* FAKEMATCH */

        BattleEffect_LoadWork(0x2e, 7, 7, 11, flash);
        (*(DrawRectangleFn *)0x03001F08)(
            second, (void *)base, 33, 72, shade, shade);
        Func_08002dd8(0x2e);

        BattleEffect_LoadWork(0x2e, 7, 7, 15, flash);
        (*(DrawRectangleFn *)0x03001F08)(
            second, (void *)base, 64, 72, shade, shade);
        Func_08002dd8(0x2e);

        if (frame == 32)
            Func_080b50e8(0x8f);

        count = ((struct BattleEffectArgument *)base->effect)->count;
        i = 0;
        if (count != 0) {
            s32 offset = 36;
next_actor:
            {
                if (frame == 10) {
                    void *p = base->effect;
                    Func_080d6888(
                        *(s16 *)((u8 *)p + offset), 7, -1, i, 8);
                }
                i++;
                offset += 2;
                if (i != ((struct BattleEffectArgument *)base->effect)->count)
                    goto next_actor;
            }
        }

        Func_080cd52c();
        base->transfer_pending = flash;
        Func_080030f8(1);

        frame++;
        if (frame != 63)
            goto next_frame;
    }

    Func_08004278(0x080cd261);
    Func_080cdbc0();
}
