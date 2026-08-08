#include "types.h"

/*
 * resource_3c9 owner at 0x02001280, 72 bytes: a two-way branch on its
 * second argument, each arm calling the same three imports with
 * different constants -- the highest call count (6x) among this
 * overlay's unindexed candidates, per `bun tools-rs/overlay-unindexed
 * resource_3c9`.
 *
 * Complete owner: `push {r5, lr}` at 0x02001280 through `pop {r5} / pop
 * {r0} / bx r0` at 0x020012ba-0x020012be, followed by the two-word
 * literal pool 0x020012c0-0x020012c7; the next owner's prologue is
 * exactly at 0x020012c8, no gap. `arg1` is read before being
 * overwritten (the `cmp r1,#0` at entry), so it is a real argument;
 * `arg0` likewise survives into r5 unmodified and is used only as a
 * passed value, never dereferenced.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/lib/overlay_call_targets.ts resource_3c9
 * 1280 12c8`'s `+2` rule -- all four targets are main-image veneers.
 *
 * Uncertainty: `arg0` is passed straight into `Func_0808a080`, which
 * elsewhere in this project's tree always takes a small id and returns
 * a record pointer (e.g. resource_3b9_c_02001298.c); here the value
 * comes from this owner's own first parameter with no visible
 * conversion, so either this owner's caller already passes an id in
 * that slot, or `Func_0808a080` accepts more than one argument shape.
 * Recorded as a plain passed value rather than resolved.
 */


/*
 * Per-site call symbols: byte-matching this overlay's `bl` needs the RAW
 * pc-relative decode of the reference halfwords (standard Thumb BL
 * semantics), not `overlay_call_targets.ts`'s `+2`-corrected semantic
 * import address -- confirmed on the resource_3c9:3660 owner, where the
 * `+2`-corrected veneer address compiled to the wrong bytes and the raw
 * decode closed it byte-exact. Per site: 0x128a -> 0x02007070,
 * 0x1290 -> 0x02006fde, 0x1296 -> 0x02006f44, 0x12a0 -> 0x02006ffe,
 * 0x12aa -> 0x02007090, 0x12b0 -> 0x02006ffe, 0x12b6 -> 0x02006f64
 * (Func_0808a158, Func_0808a080, Func_080091e0, Func_0808a090 in call
 * order -- see the header comment for which import each is).
 */
void Func_02007070(s32 arg0, s32 arg1);        /* Func_0808a158, if-arm */
u8 *Func_02006fde(s32 arg0);                   /* Func_0808a080, if-arm */
void Func_02006f44(void *record, s32 arg1);    /* Func_080091e0, if-arm */
s32 Func_02006ffe();   /* raw-decoded target is shared by both the if-arm's
                         * Func_0808a090(arg0,0xcccc,0x6666) (3 args, return
                         * unused) and the else-arm's Func_0808a080(arg0) (1
                         * arg, u8* return) -- old-style declaration so both
                         * call shapes are legal against the one symbol. */
void Func_02007090(s32 arg0, s32 arg1);        /* Func_0808a158, else-arm */
void Func_02006f64(void *record, s32 arg1);    /* Func_080091e0, else-arm */

void Func_02001280(s32 arg0, s32 flag)
{
    if (flag != 0) {
        Func_02007070(arg0, 0);
        Func_02006f44(Func_02006fde(arg0), 1);
        Func_02006ffe(arg0, 0xcccc, 0x6666);
    } else {
        Func_02007090(arg0, 15);
        Func_02006f64((u8 *)Func_02006ffe(arg0), 0);
    }
}
