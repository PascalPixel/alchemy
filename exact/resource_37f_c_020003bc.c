#include "types.h"

/*
 * Resource 37f, owner at 0x020003bc (76 bytes of code + a five-word literal
 * pool at 0x0200040c-0x0200041f).  `push {r5, lr}` at 0x020003bc, interworking
 * return `pop {r5} / pop {r0} / bx r0` at 0x02000404 — r0 is the popped return
 * address, so the owner is `void`.
 *
 * All seven call sites were resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`; all
 * seven are import veneers.  Func_080770c0(flag) is used only as a predicate.
 *
 * Near-identical sibling of 0x02000200: the same Func_0808a018 /
 * Func_080770c0 / Func_08015040 / Func_0808a020 skeleton with flag ids
 * 0x821 / 0xf02 instead of 0x81a / 0xf01, and with the fall-through arm
 * spelled out separately rather than shared.
 *
 * r5 is loaded with the workspace pointer *before* the Func_08015040 call and
 * used after it; that is why the owner saves r5 at all.
 */

extern u8 *Data_03001ebc;






extern void Func_02001f6c();
extern s32 Func_02001f52();
extern void Func_02001f56();
extern s32 Func_02001f66();
extern void Func_02001f6e();
extern void Func_02001f82();
extern void Func_02001fb6();
void Func_020003bc(void)
{
    u8 *workspace;

    Func_02001f6c();

    if (Func_02001f52(0x821) != 0) {
        Func_02001f56(0x1034, 1);
    } else if (Func_02001f66(0xf02) != 0) {
        workspace = Data_03001ebc;
        Func_02001f6e(0x1031, 1);
        {
            /*
             * The halfword store goes through a pointer local and then an s32 value local,
             * in that order.  Storing the literal straight into the halfword makes gcc
             * build the constant in HImode and fetch it from the literal pool
             * (`ldrh r3, .L7'), which costs a pool word the reference does not have;
             * splitting the address out first also fixes which register holds the address.
             */
            u16 *frame = (u16 *)(workspace + 370);
            s32 one = 1;
            *frame = (u16)one;
        }
    } else {
        Func_02001f82(0x1031, 1);
    }

    Func_02001fb6();
}
