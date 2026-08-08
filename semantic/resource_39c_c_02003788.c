#include "types.h"

/*
 * resource_39c owner at 0x02003788, 216 bytes: one-time scene setup for
 * "self" -- if a linked object for id 265 doesn't already exist, run a
 * long linear sequence of state-clearing and positioning calls.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02003788 through `pop
 * {r5, r6, r7} / pop {r0} / bx r0` at 0x02003846-0x0200384a, followed by
 * the five-word literal pool 0x0200384c-0x0200385f; the next owner's
 * prologue is at 0x02003860. No incoming arguments are read before being
 * overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`'s `+2` rule.
 *
 * The owner-record[9] update (`(field9 & ~0xc) | 4`) is the same idiom
 * resource_39c_c_02005388.c/02005458.c use elsewhere in this overlay.
 * `self[0x55]` is written twice on the init path -- 0 early, then 3 near
 * the end -- and `self[0x6c]` likewise (a pool constant, then 0);
 * recorded exactly as compiled, not collapsed, since these are genuinely
 * two different writes in sequence rather than a repeated identical
 * store.
 *
 * Uncertainty: none of the fourteen resolved callees are identified beyond
 * call shape. The fixed-point conversion of `self`'s
 * fields 0x10/0x12 (signed halfwords, shifted to 16.16, one of them
 * additionally offset by -1.0 via the 0xfff00000 pool constant) is
 * recorded as read, not decoded further.
 */

extern void *Func_0808a080(s32 arg0);
extern s32 Func_080770c0(s32 arg0);
extern void Func_0808a018(void);
extern void Func_0808a210(s32, s32, s32, s32);
extern void Func_0808a0f0(s32, s32, s32);
extern void Func_0808a158(s32, s32);
extern void Func_080091e0(s32, s32);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_080f9010(s32);
extern void Func_0808a090(s32, s32, s32);
extern void Func_0808a580();
extern void Func_0808a5e8(void);
extern void Func_0808a020(void);

void Func_02003788(void)
{
    u8 *self = (u8 *)Func_0808a080(0);
    s32 existing = Func_080770c0(0x109);
    u8 *ownerRecord;

    if (existing != 0) {
        return;
    }

    Func_0808a018();

    self[0x55] = 0;
    Func_0808a210(-1, -1, -1, 0);

    Func_0808a0f0(0, *(short *)(self + 10) << 16, (*(short *)(self + 18) << 16) - 0x10000);
    Func_0808a158(0, 15);
    Func_0808a080(0);
    Func_080091e0(0, 0);
    Func_0808a360();
    Func_0808a370();
    Func_080f9010(228);

    *(s32 *)(self + 0x6c) = 0x02008cd1;
    Func_0808a090(0, 0x6666, 0x3333);
    Func_0808a580(0, 0, 8);
    Func_0808a158(0, 0);
    Func_0808a080(0);
    Func_080091e0(0, 1);

    ownerRecord = *(u8 **)(self + 0x50);
    ownerRecord[9] = (ownerRecord[9] & ~0x0c) | 4;

    Func_0808a580(0, 0, 10);

    *(s32 *)(self + 0x6c) = 0;
    self[0x55] = 3;

    Func_0808a5e8();
    Func_0808a020();
}
