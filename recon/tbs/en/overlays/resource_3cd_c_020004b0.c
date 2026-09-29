/* NONMATCHING 2026-09-27: word-sized direction with signed-byte consumers
 * places direction in r8 and redraw in sl as the ROM does, but loses four
 * bytes and changes 163 halfwords (72 aligned edits). Moving the local
 * redraw clear after window refresh stays 580/584 and worsens to 225
 * halfwords (83 edits). The byte-sized direction's sign extension and its
 * zero shared with the call are required; keep the 584/19 baseline.
 * Cross-edition identity audit (2026-09-27): German, Spanish, French
 * and Italian retain the English core at 020004b0. The automatic Japanese
 * anchor at 020001c8 is NOT this ability selector: its mask is 0x1ff,
 * message base 0x182 and empty-entry test a halfword at +6, identifying
 * the item-selection family instead. Do not treat its register roles or
 * second window as evidence for this function. No source change admitted.
 * NONMATCHING 2026-09-27 redraw-flag probe: u8 redraw with the same
 * 0/1 publications emits the complete baseline binary identically (cmp):
 * 584/584 bytes, 19 differing halfwords / 19 aligned edits. Unlike the
 * earlier direction-storage trial, this tested the other flag's mode;
 * it changes neither lifetime nor reciprocal r8/sl roles. Reject after
 * this one bounded full-diff comparison; restore the s32 flag and do not
 * sweep remaining scalar widths. No new DONE or alignment credit.
 * 2026-09-27 signed-consumer probe: 584/584 bytes, 19 differing
 * halfwords / 19 aligned edits. u8 direction with explicit s8 consumers
 * emits identical bytes and allocator lifetimes to this canonical s8 model:
 * redraw 19 refs / 322 insns, direction 17 / 292. Complete diff and all
 * pool words agree except the reciprocal r8/sl role assignment. No new
 * family interface evidence changes that constraint. STOP byte-storage
 * axis; baseline retained, no DONE or alignment credit.
 * NONMATCHING: 584 bytes, candidate 584, 19 differing halfwords, 19 halfword
 * edits (2026-09-25). DebugMenu_SelectAbility, meant for
 * DEBUG/ITEM_LEVEL/SELECT_ABILITY.C as a single-overlay unit binding its
 * names at their runtime addresses (an import veneer's listing offset plus
 * 0x8000). Remaining: dir/redraw trade r8 and sl (same allocator wall as
 * 3cd:02000214 SelectItem twin); body layout, DMA, skip-empty
 * BattleAction_Get[4] loops, mask-after-label ands, and B-exit
 * Finalize(details) otherwise match
 * WALL: gcc 2.96 assigns redraw (more refs) to r8 and dir to sl; ROM wants
 * dir in r8 and redraw in sl — 19 halfwords of mov r8/sl only.
 * Bounded lifetime trials: separate next-redraw flag gives 584 bytes / 60
 * aligned edits, mutually exclusive direction scans 584 / 37, and resetting
 * direction before input 576 / 99. The first two keep the wrong entry sl;
 * the last removes the required entry zero. Keep the original 19-edit model.
 * 2026-09-27 family audit: complete 020004b0..020006f8, including pool;
 * fresh baseline 584 bytes / 19 differing halfwords / 19 aligned edits.
 * Text/window callees never receive direction or redraw state. BattleAction
 * lookup masks to 0x3fff; its byte +4 is the same empty-entry test already
 * emitted. No new alias/prototype witness justifies a state wrapper.
 * Reference never defines r9 before Finalize(details): do not initialize
 * the unused second window or invent shared caller state to repair it.
 * STOP family ownership axis; unchanged baseline, no spelling trials.
 * Astra 2026-09-27: a one-pass boundary around the final direction clear
 * gives 584/584, 20 halfwords / 20 edits; both allocator lifetimes stay
 * 19/322 and 17/292 and only the clear's scheduling changes. Conditional
 * clear (if dir != 0) gives 588/584, 69/41: the extra guard survives and
 * direction spans 372 instructions. Reject both; keep the 19-edit body. */
#include "FIELD_EVENT.H"
#include "DMA.H"
#include "TEXT_RENDER_RUNTIME.H"
extern u8 MsgAbilityDescription[];
extern u8 MsgAbilityName[];

extern u8 Value_00000333;
extern u8 Data_0200890c[];
extern u8 Data_02008914[];
extern volatile u32 gDebugKeysPressed;
extern volatile u32 gDebugKeysRepeated;

struct TextRenderWork *Engine_DebugCreateWindow(s32 x, s32 y, s32 width, s32 height, s32 style);
void Engine_DebugFinalizeWindow(struct TextRenderWork *window, s32 mode);
void Engine_DebugRedrawWindow(struct TextRenderWork *window);
void Engine_DebugClearWindow(struct TextRenderWork *window);
void UiText_DrawStringInWindowFar(u8 *text, struct TextRenderWork *window, s32 x, s32 y);
void UiText_DrawNumberAtOffsetFar(s32 value, s32 format, struct TextRenderWork *window, s32 x, s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 text, struct TextRenderWork *window, s32 x, s32 y);
void Main_08015078(s32 text, struct TextRenderWork *window, s32 x, s32 y);
s32 Func_02000cee(s32 value, s32 divisor);
u8 *BattleAction_Get(s32 action);

void DebugMenu_SelectAbility(void)
{
    struct TextRenderWork *window;
    struct TextRenderWork *details;
    s32 ability;
    s32 redraw;
    s32 index;
    s8 dir;
    s32 count;

    dir = 0;
    Engine_AudioPlayCue(112);
    window = Engine_DebugCreateWindow(0, 0, 30, 12, 2);
    ability = 1;
    redraw = 1;
    Dma_Set((const void *)0x05000200, (void *)0x050001c0, 0x80000010,
            (volatile u32 *)0x040000d4);
    Dma_Set((const void *)0x050001e8, (void *)0x050001dc, 0x80000001,
            (volatile u32 *)0x040000d4);
    Engine_TaskWait(1);
    count = 270;
    for (;;) {
        if (redraw) {
            redraw = 0;
            ability = Func_02000cee(ability + 270, count);
            Engine_DebugRedrawWindow(window);
            Engine_DebugClearWindow(window);
            UiText_DrawStringInWindowFar(Data_0200890c, window, 0, 0);
            UiText_DrawNumberAtOffsetFar(ability, 0, window, 80, 0);
            UiText_DrawStringInWindowFar(Data_02008914, window, 0, 72);
            index = ability & 0x3fff;
            UiText_DrawCharacterAtOffsetFar(index + (s32)MsgAbilityName, window, 120, 0);
            UiText_DrawCharacterAtOffsetFar(index + (s32)MsgAbilityDescription, window, 0, 24);
            Main_08015078(index + (s32)MsgAbilityDescription, window, 0, 48);
        }
        if (gDebugKeysPressed & 2) {
            Engine_AudioPlayCue(113);
            goto done;
        }
        if (gDebugKeysRepeated & 64) {
            dir = -1;
            ability--;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gDebugKeysRepeated & 128) {
            dir = 1;
            ability++;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gDebugKeysRepeated & 16) {
            dir = 1;
            ability += 10;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gDebugKeysRepeated & 32) {
            dir = -1;
            ability -= 10;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gDebugKeysRepeated & 256) {
            dir = 1;
            ability += 30;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (gDebugKeysRepeated & 512) {
            dir = -1;
            ability -= 30;
            redraw = 1;
            Engine_AudioPlayCue(111);
        }
        if (dir == -1) {
            ability = Func_02000cee(ability + 270, count);
            goto check_back;
        step_back:
            ability = Func_02000cee(ability + 269, count);
        check_back:
            if (BattleAction_Get(ability & 0x3fff)[4] == 0)
                goto step_back;
        }
        if (dir == 1) {
            ability = Func_02000cee(ability + 270, count);
            goto check_forward;
        step_forward:
            ability = Func_02000cee(ability + 271, count);
        check_forward:
            if (BattleAction_Get(ability & 0x3fff)[4] == 0)
                goto step_forward;
        }
        dir = 0;
        Engine_TaskWait(1);
    }
done:
    Engine_DebugRedrawWindow(window);
    Engine_TaskWait(1);
    Engine_DebugFinalizeWindow(window, 1);
    Engine_DebugFinalizeWindow(details, 1);
}
