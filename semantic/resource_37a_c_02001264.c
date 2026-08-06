#include "types.h"

/*
 * Resource 37a scene setup at 0x02001264 (284 bytes, 0x02001264-0x0200137f).
 *
 * Complete owner: `push {r5, r6, lr}` / `sub sp, #8` prologue at 0x02001264
 * and the matching interworking return `add sp, #8 / pop {r5, r6} / pop {r0} /
 * bx r0` at 0x02001360.  r0 holds the popped return address, so nothing is
 * returned: the owner is `void`.  The body is completely straight-line; there
 * is no branch in it at all.
 *
 * Pool map, derived by walking the owner from its prologue: every halfword from
 * 0x02001264 to 0x02001366 is reached as an instruction, and the 24 bytes at
 * 0x02001368-0x0200137f are the literal pool
 * (0x023e0000, 0x002051cc, 0x00000201, 0x00000202, 0x03001ebc, 0x00010005).
 * Nothing branches over or into them; the `lsls`/`subs` the disassembler prints
 * there are constants, not code.
 *
 * All 24 call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2); the disassembler's printed
 * `bl` targets are the usual overlay artefact and are not the callees.
 * sites=24, distinct_targets=13, all veneer, against the inventory's calls=22
 * (that field counts distinct targets and is a known low lower bound).
 *
 *   0x02001276 -> veneer 0x02002a9c -> Func_0808a210
 *   0x0200128a -> veneer 0x02002994 -> Func_08009180
 *   0x0200129c -> veneer 0x02002994 -> Func_08009180
 *   0x020012ac -> veneer 0x02002994 -> Func_08009180
 *   0x020012be -> veneer 0x02002994 -> Func_08009180
 *   0x020012ce -> veneer 0x02002a9c -> Func_0808a210
 *   0x020012d2 -> veneer 0x0200298c -> Func_08009128
 *   0x020012de -> veneer 0x02002a1c -> Func_0808a0f0
 *   0x020012e8 -> veneer 0x02002a1c -> Func_0808a0f0
 *   0x020012ee -> veneer 0x0200296c -> Func_080000c0
 *   0x020012f6 -> veneer 0x02002abc -> Func_0808a330
 *   0x020012fc -> veneer 0x02002acc -> Func_0808a348
 *   0x02001302 -> veneer 0x020029bc -> Func_080770c8
 *   0x0200130a -> veneer 0x020029c4 -> Func_080770d0
 *   0x02001310 -> veneer 0x020029c4 -> Func_080770d0
 *   0x0200132a -> veneer 0x02002ad4 -> Func_0808a360
 *   0x0200132e -> veneer 0x02002ae4 -> Func_0808a370
 *   0x02001334 -> veneer 0x020029cc -> Func_0808a010
 *   0x0200133a -> veneer 0x02002af4 -> Func_080f9010
 *   0x02001342 -> veneer 0x02002abc -> Func_0808a330
 *   0x02001348 -> veneer 0x02002acc -> Func_0808a348
 *   0x0200134e -> veneer 0x020029cc -> Func_0808a010
 *   0x02001356 -> veneer 0x02002abc -> Func_0808a330
 *   0x0200135c -> veneer 0x02002acc -> Func_0808a348
 *
 * Func_08009180 is the established six-argument renderer entry: r0-r3 plus two
 * stack words at [sp,#0] and [sp,#4].  The two stack slots are written out of
 * order relative to the register arguments at three of the four sites (the
 * second site sets [sp,#4] before r0-r3 and reuses r6 for [sp,#0]), and r5=3 /
 * r6=8 are held live across all four sites — recovered from the predecessor
 * blocks, not from any per-site window.
 *
 * Func_080770c8/Func_080770d0 are the global story-flag set/clear pair resolved
 * in semantic/overlays/resource_3cb_c_020010e8.c: this owner sets flag 0x0201
 * and clears 0x0200 and 0x0202, i.e. it selects one of three scene variants.
 *
 * The store block at 0x02001314 is the shared runtime-defaults write also seen
 * in the byte-exact assets/code/resource_37a_c_02001380.c: the pointer table at
 * 0x03001ebc yields the runtime record, whose word at +0x1c0 becomes 0x100 and
 * whose word at +0x1c8 becomes 32.  In assembly r3 is a value (448 - 192 = 256)
 * and then a displacement (256 + 200 = 456 = 0x1c8); the two uses are unrelated
 * despite sharing the register.
 *
 * Uncertainties: 0x002051cc and 0x00010005 are opaque packed constants (neither
 * lies in the overlay's in-image band, so neither is an address under the
 * 0x02008000 link base); Func_08009128, Func_0808a360 and Func_0808a370 are
 * reached with no argument register set by this owner.  Arities are taken from
 * the registers written before each site.
 */

/* Old-style declarations: arities vary per call site across this overlay. */














extern void Func_02003d14();
extern void Func_02003c20();
extern void Func_02003c32();
extern void Func_02003c42();
extern void Func_02003c54();
extern void Func_02003d6c();
extern void Func_02003c60();
extern void Func_02003cfc();
extern void Func_02003d06();
extern void Func_02003c5c();
extern void Func_02003db4();
extern void Func_02003dca();
extern void Func_02003cc0();
extern void Func_02003cd0();
extern void Func_02003cd6();
extern void Func_02003e00();
extern void Func_02003e14();
extern void Func_02003d02();
extern void Func_02003e30();
extern void Func_02003e16();
extern void Func_02003d1c();
extern void Func_02003e2a();
void Func_02001264(void)
{
    u8 **runtime_globals = (u8 **)0x03001ebc;
    u8 *runtime;
    s32 a;
    s32 b;

    Func_02003d14(-1, -1, -1, 0);
    b = 3;
    Func_02003c20(30, 43, 32, 40, a, b);
    Func_02003c32(30, 43, 33, 39, a, 1);
    Func_02003c42(30, 43, 36, 38, b, b);
    Func_02003c54(14, 41, 32, 41, a, 4);
    Func_02003d6c(0x023e0000, -1, 0x9e0000, 0);     /* 158 << 16 */
    Func_02003c60();
    Func_02003cfc(16, 0x023e0000, 0x780000);        /* 240 << 15 */
    Func_02003d06(0, 0, 0);
    Func_02003c5c(1);
    Func_02003db4(0x2051cc, 1);
    Func_02003dca(20);
    Func_02003cc0(0x201);
    Func_02003cd0(0x200);
    Func_02003cd6(0x202);

    *(s32 *)(runtime + 0x1c0) = 0x100;
    *(s32 *)(runtime + 0x1c8) = 32;
    runtime = runtime_globals[0];

    Func_02003e00();
    Func_02003e14();
    Func_02003d02(40);
    Func_02003e30(171);
    Func_02003e00(0x10005, 1);
    Func_02003e16(8);
    Func_02003d1c(32);
    Func_02003e14(0x2051cc, 1);
    Func_02003e2a(24);
}
