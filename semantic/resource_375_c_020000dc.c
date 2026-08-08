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
 */

/* Imports, old-style: arities are established per call site, not globally, and
 * the same name can take different argument counts elsewhere in this overlay.
 * Func_080770c0 and Func_0808a070 are tested, so they need return types. */
void Func_02001b64();
s32 Func_02001b4a();
void Func_02001c1c();
void Func_02001c34();
void Func_02001c2e();
void Func_02001c20();
void Func_02001b96();
void Func_02001c56();
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
        line = 0xf76;
    } else {
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
