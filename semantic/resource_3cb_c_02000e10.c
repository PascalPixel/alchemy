#include "types.h"

/*
 * Resource 3cb selection prompt at 0x02000e10 (264 bytes,
 * 0x02000e10-0x02000f17).
 *
 * Complete owner: `push {r5, r6, r7, lr} / sub sp, #4` prologue at 0x02000e10
 * and the matching `add sp, #4 / pop {r5, r6, r7} / pop {r1} / bx r1`
 * interworking return at 0x02000ef4.  The popped return address goes to r1, so
 * r0 survives and IS the result — the value returned by the final
 * Func_0808a020 call.
 *
 * The 28 bytes at 0x02000efc-0x02000f17 are this owner's literal pool
 * (0x02000240, 0x00000989, 0x03001b04, 0x03001c94, 0x0000098a, 0x0000098b,
 * 0x0000098c).  Execution reaches `bx r1` at 0x02000efa and never falls into
 * them; the `lsrs`/`subs`/`adds` halfwords the disassembler prints there are
 * data.  Func_02000f18 (byte-exact in assets/code) begins immediately after.
 *
 * All sixteen call sites were resolved with cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --
 * (true_target_offset = stored_displacement + 2), matching the inventory's
 * calls=16.  Every one is an import veneer:
 *   0x02000e16 -> 0x02001844 -> Func_0808a018
 *   0x02000e28 -> 0x0200187c -> Func_0808a148
 *   0x02000e2e -> 0x02001884 -> Func_0808a170
 *   0x02000e36 -> 0x02001894 -> Func_0808a180
 *   0x02000e46 -> 0x020017ac -> Func_08015010
 *   0x02000e60 -> 0x02001714 -> Func_080000c0
 *   0x02000e6a -> 0x020017d4 -> Func_08015060
 *   0x02000e78 -> 0x020017dc -> Func_080150a0
 *   0x02000eae -> 0x020017b4 -> Func_08015018
 *   0x02000eb8 -> 0x020018dc -> Func_080b5110
 *   0x02000ece -> 0x02001884 -> Func_0808a170
 *   0x02000ed6 -> 0x02001894 -> Func_0808a180
 *   0x02000ede -> 0x02001884 -> Func_0808a170
 *   0x02000ee6 -> 0x02001894 -> Func_0808a180
 *   0x02000eec -> 0x02001714 -> Func_080000c0
 *   0x02000ef0 -> 0x0200184c -> Func_0808a020
 *
 * Link base: resource_3cb is linked at 0x02008000 (handler table at the tail
 * of assets/code/resource_3cb_overlay.s: 0x02009051 = Func_02001050 + 1, which
 * is byte-exact in assets/code).  All the pool addresses here are therefore
 * below the image: 0x02000240 is the ordinary RAM record the byte-exact
 * assets/code/resource_3cb_c_02000060.c also reads (this owner uses the word
 * at offset 500, built as 250 << 1), and 0x03001b04 / 0x03001c94 are the
 * engine's IWRAM key state.
 *
 * The two key words are read with the standard GBA KEYINPUT bit assignment:
 * 0x03001b04 is tested for 0x20 (Left, step back) and 0x10 (Right, step
 * forward), and 0x03001c94 for 0x01 (A, confirm) and 0x02 (B, cancel).  Two
 * separate words are used because one is the repeating cursor input and the
 * other the edge-triggered buttons; which is which is not established beyond
 * that usage.
 *
 * Behaviour: pose cue 0x989 for the speaker, open a 6-by-4 chooser through
 * Func_08015010, then run a cursor loop.  The highlight is repainted through
 * Func_08015060/Func_080150a0 only when the selection actually moves.  B
 * cancels with selection -1; A confirms.  The chooser is closed with
 * Func_08015018(handle, 1) and the outcome cue follows: 0x98a when cancelled,
 * otherwise Func_080b5110(selection) chooses 0x98b (nonzero) or 0x98c (zero).
 * The reply is addressed to speaker 9 in every outcome, then the owner waits
 * ten units and closes with Func_0808a020.
 *
 * Uncertainties: the selection is clamped below at 0 but never above — either
 * Func_080150a0 clamps it or the caller's chooser is wide enough; nothing here
 * establishes which.  The literal 3 passed to Func_080150a0 and the trailing
 * stack argument (2 when opening, 0 when repainting) are not interpreted.
 * Func_0808a018 and Func_0808a020 are reached with no argument register set by
 * this owner.
 */

extern u8 Data_02000240[];

/* Old-style declarations: arities vary per site across this overlay. */
void Func_0200265c();
void Func_020026a6();
void Func_020026b4();
void Func_020026cc();
s32 Func_020025f4();
void Func_02002576();
void Func_02002640();
void Func_02002656();
void Func_02002664();
s32 Func_02002796();
void Func_02002754();
void Func_0200276c();
void Func_02002764();
void Func_0200277c();
void Func_02002602();
s32 Func_0200273e();

                     

                     

/* Used for their return values. */

                    

s32 Func_02000e10(s32 speaker)
{
    volatile u32 *cursorKeys = (volatile u32 *)0x03001b04;
    volatile u32 *buttonKeys = (volatile u32 *)0x03001c94;
    s32 chooser;
    s32 selection = 0;
    s32 painted = -1;
    u32 outcomeCue;

    Func_0200265c();
    Func_020026a6(speaker, *(u32 *)&Data_02000240[500], 0);
    Func_020026b4(0x989);
    Func_020026cc(speaker, 0);

    chooser = Func_020025f4(0, 0, 6, 4, 2);

    /* The frame wait is the body of a bottom-tested input loop. */
    goto paintSelection;

waitForInput:
    if ((*buttonKeys & 2) != 0) {
        selection = -1;
        goto selectionMade;
    }
    Func_02002576(1);

paintSelection:
    if (selection != painted) {
        Func_02002640(chooser);
        Func_02002656(selection, 3, chooser, 0, 0);
        painted = selection;
    }

    if ((*cursorKeys & 0x20) != 0) {
        selection--;
    }
    if ((*cursorKeys & 0x10) != 0) {
        selection++;
    }
    if (selection < 0) {
        selection = 0;
    }

    if ((*buttonKeys & 1) != 0) {
        goto selectionMade;
    }
    goto waitForInput;

selectionMade:
    Func_02002664(chooser, 1);

    if (selection < 0) {
        outcomeCue = 0x98a;
        goto emitChosenOutcome;
    } else if (Func_02002796(selection) != 0) {
        outcomeCue = 0x98b;
        goto emitChosenOutcome;
    }
    goto emitZeroOutcome;

emitChosenOutcome:
    /* Cancel and nonzero choices share this physical cue/portrait pair. */
    Func_02002754(outcomeCue);
    Func_0200276c(9, 0);
    goto outcomeComplete;

emitZeroOutcome:
    Func_02002764(0x98c);
    Func_0200277c(9, 0);

outcomeComplete:
    Func_02002602(10);
    return Func_0200273e();
}
