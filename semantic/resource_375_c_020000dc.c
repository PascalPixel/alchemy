#include "types.h"

/*
 * resource_375 owner at 0x020000dc, 148 bytes: code 0x020000dc-0x02000163 and
 * the literal pool 0x02000164-0x0200016f (three small identifiers, not
 * addresses).
 *
 * Conventions.  An overlay `bl` stores the target's image offset minus two, not
 * a pc-relative displacement, so the disassembler's printed targets are wrong;
 * all sixteen calls below were resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`
 * and every one lands in the 8-byte-per-entry import veneer table that starts
 * at 0x02001a54, so each is named by that entry's trailing main-image word.
 * resource_375 is linked at 0x02008000 (witness: 0x02008171 in the table at
 * file offset 0x1efc is this overlay's Func_02000170 plus the Thumb bit).
 *
 * Return type: the epilogue `pop {r5} / pop {r0} / bx r0` pops the return
 * address into r0, so nothing is returned and the owner is void.
 *
 * Behaviour: open a scene with Func_0808a018, then branch on the event flag
 * 0x0815.  If it is set, a single short line (0x11c4) is shown and the scene
 * closes.  Otherwise a three-line sequence built from the consecutive ids
 * 0x0f76, 0x0f77 and 0x0f78 is played, with the choice between the last two
 * decided by Func_0808a070(0, 0), and the channel-12 state is stepped between
 * lines.  Func_0808a020 closes the scene on both paths.
 *
 * Uncertainties: 0x0815, 0x11c4 and 0x0f76 are identifiers whose tables are not
 * reconstructed, so they stay numeric; the recurring first argument 12 is a
 * channel or slot selector, likewise unnamed.  Func_0808a170 is reached with
 * one argument at all four of its sites and Func_0808a180 with two at both of
 * its, so the varying-arity caveat does not bite inside this owner.
 *
 * WHAT IS STILL WRONG, and why this row's score got worse rather than better.
 * An earlier draft assigned `line` in the flag-set arm, where it is never read,
 * and read it UNINITIALISED in the other -- so gcc had no value to fold and
 * emitted a shape that scored 15 differing halfwords at 144 bytes.  That number
 * was an artefact of undefined behaviour, not evidence of a near-miss, and
 * ranking by it would send the next reader at an owner that is nowhere near.
 *
 * Two readings here are proved by the bytes.  The reference tests
 * Func_02001b4a's result with a bare `cmp r0, #0` and never truncates it, so the
 * interface is wider than the `u8` it was declared: `lsls r0, r0, #24` was our
 * extension, not the ROM's.  And the reference loads 0xf76 into r5 inside the
 * else arm and derives the other two ids with `adds r0, r5, #1` and `#2`, so
 * `line` is initialised there and held across the calls -- which is what the
 * three consecutive ids in the behaviour note already said.
 *
 * With both corrections the row is 152 bytes against 148 and 60 halfwords out.
 * Every well-defined spelling tried lands on the same 152: initialised at the
 * declaration, assigned before the first call, assigned in the else arm, the
 * three ids written as explicit constants, compound increments before each call,
 * a separate local for the derived id, and `volatile` (160).  All of them let
 * gcc fold 0xf77 and 0xf78 into their own pool words, where the reference keeps
 * one word and adds to it.  Closing this needs whatever source keeps that base
 * un-folded, and it is not any of the above.
 */

/* Imports, old-style: arities are established per call site, not globally, and
 * the same name can take different argument counts elsewhere in this overlay.
 * Func_080770c0 and Func_0808a070 are tested, so they need return types.
 * Func_02001b4a is s32 and not u8: the reference never truncates its result. */
void Func_02001b64();
s32 Func_02001b4a();
void Func_02001c1c();
void Func_02001c34();
void Func_02001c2e();
void Func_02001c20();
void Func_02001b96();
s32 Func_02001c56();
s32 Func_02001bbe();
void Func_02001c60();
void Func_02001c68();
void Func_02001c48();
void Func_02001c88();
void Func_02001cac();
void Func_02001be8();

                     

                     

                    

                     

void Func_020000dc(void)
{
    s32 line;

    Func_02001b64();

    if (Func_02001b4a(0x815) != 0) {
        Func_02001c1c(0x11c4);
        Func_02001c34(12, 0);
    } else {
        line = 0xf76;
        Func_02001c2e(line);
        Func_02001c20(12, 0, 10);
        Func_02001c20(12, 2);
        Func_02001b96(6);
        Func_02001c56(12, 0);
        if (Func_02001bbe(0, 0) == 0) {
            Func_02001c60(line + 1);
        } else {
            Func_02001c68(line + 2);
        }
        Func_02001c48(12, 3);
        Func_02001c88(12, 0);
        Func_02001cac(12, 0xc000, 10);
    }

    Func_02001be8();
}
