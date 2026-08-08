#include "types.h"

/*
 * resource_3c9 owner at 0x02000480, 56 bytes: the same 3-D distance
 * helper already named in resource_3b3_c_02000314.c (byte-identical
 * body across at least five overlays there; this is a sixth copy, in a
 * different overlay's import band).
 *
 * Complete owner: `push {r5, lr}` at 0x02000480 through `pop {r5} / pop
 * {r1} / bx r1` at 0x020004b2-0x020004b6 (real return value survives in
 * r0, per HANDOVER's epilogue rule), followed by the one-word literal
 * pool 0x020004b8-0x020004bb; the next owner's prologue is exactly at
 * 0x020004bc. No branches.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3c9
 * 480 4bc`: one site, classified `call_via` (this overlay's own slot at
 * 0x02005ee0), with r3 loaded from the pool immediately before it --
 * 0x030001d8, the IWRAM square root already named in
 * resource_3b3_c_02000314.c and resource_3b7_c_02000e5c.c. Same
 * interleaved pointer-walk order as the precedent (a[0], b[0], a[1],
 * b[1], b[2], a[2] via `ldmia rN!` then two plain loads), same 16.16
 * fixed-point reduction (`asrs #16` before squaring, to keep the sum
 * inside 32 bits).
 */

s32 Func_030001d8();   /* IWRAM square root, reached through the call_via slot */

s32 Func_02000480(s32 *a, s32 *b)
{
    s32 dx = (a[0] - b[0]) >> 16;
    s32 dy = (a[1] - b[1]) >> 16;
    s32 dz = (a[2] - b[2]) >> 16;

    return Func_030001d8(dx * dx + dy * dy + dz * dz);
}
