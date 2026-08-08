#include "types.h"

/*
 * Resource 3b5, overlay initialiser at 0x02000728 (364 bytes; 330 bytes of
 * code, two bytes of alignment, and the 32-byte literal pool at
 * 0x02000874-0x02000893).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000728 and
 * `pop {r5, r6} / pop {r1} / bx r1` at 0x0200086c-0x02000870.  It pops into
 * r1, so r0 survives and IS the result; the only value ever left there is the
 * `movs r0, #0` at 0x0200086a, so this returns a constant 0.
 *
 * Role: exported entry point.  The overlay image begins with a table of
 * eight-byte `ldr r4, [pc, #0] / bx r4 / .word target` veneers, and its FIRST
 * entry holds 0x02008729 = Func_02000728 + the Thumb bit under this overlay's
 * proven 0x02008000 link base.  This owner is therefore the root of the
 * overlay's call graph, and its pool words name three other rows in advance:
 *
 *   0x02008171 -> Func_02000170 + 1, installed as the per-frame callback
 *                 (actor field +0x6c) of actors 16 and 17;
 *   0x020086e9 -> Func_020006e8 + 1, the per-frame callback of actor 14;
 *   0x02008ec0 / 0x02008f90 -> in-image data at file offsets 0xec0 and 0xf90
 *                 (EVEN, so data, not code), the two actors' scripts.
 *
 * It also calls two overlay-local rows directly, Func_02000ca8 and
 * Func_02000224, both resolved as `prologue` by
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`.
 *
 * The store at 0x02000736 is the documented displacement/value trap: r2 is
 * built as 224 << 1 = 448, used as the workspace displacement, then reduced by
 * `subs r2, #192` to 32, which is the value stored.  It is workspace[448] = 32,
 * not workspace[256].
 *
 * The seven Func_0808a0f0 calls in the 0x950 arm are seven distinct call
 * sites, spelled out in the assembly for ids 20, 21, 22, 24, 25, 26 and 27 —
 * note that 23 is skipped, so this is not a loop and is not folded into one.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`; twenty-six
 * sites, against the inventory's `calls=24`, which is a documented lower
 * bound.
 */

/* Old-style declarations: overlay import arities are not fixed per name. */
void Func_020014f8();
void Func_020014d8();
u8 *Func_020014ce();
void Func_02001520();
void Func_02001500();
u8 *Func_020014f6();
u8 *Func_02001506();
s32 Func_020014d8_b();
void Func_0200155a();
s32 Func_020014f0();
void Func_0200145c();
s32 Func_02001500_b();
void Func_020009e8();
void Func_02001588();
s32 Func_02001518();
void Func_0200159a();
void Func_020015a8();
void Func_020015b6();
void Func_020015c4();
void Func_020015d2();
void Func_020015e0();
void Func_020015ee();
s32 Func_02001586();
void Func_02001608();
void Func_0200167c();
void Func_02001624();

                    

                     

s32 Func_02000728(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    u8 *actor;
    void *frame_callback = (void *)0x02008171;   /* Func_02000170 + Thumb bit */

    *(s32 *)(workspace + 448) = 32;

    Func_020014f8(16, 0x1600000, 0x1600000);
    *(u16 *)(actor + 100) = 1;
    Func_020014d8(16, (void *)0x02008ec0);
    actor = Func_020014ce(16);
    *(void **)(actor + 0x6c) = frame_callback;

    Func_02001520(17, 0x1700000, 0x1400000);
    Func_02001500(17, (void *)0x02008f90);
    *(u16 *)(actor + 100) = 0;
    *(void **)(actor + 0x6c) = frame_callback;
    actor = Func_020014f6(17);

    actor = Func_02001506(14);
    *(void **)(actor + 0x6c) = (void *)0x020086e9;  /* Func_020006e8 + Thumb */

    if (Func_020014d8_b(0x8c1) != 0) {
        Func_0200155a(28, 0x13c0000, 0x1480000);
    }

    if (Func_020014f0(0x201) != 0) {
        Func_0200145c();
    }

    if (Func_02001500_b(0x200) != 0) {
        Func_020009e8();
        Func_02001588(8, 4);
    }

    if (Func_02001518(0x950) != 0) {
        Func_0200159a(20, 0x2080000, 0x2300000);
        Func_020015a8(21, 0x2080000, 0x2300000);
        Func_020015b6(22, 0x2080000, 0x2300000);
        Func_020015c4(24, 0x2080000, 0x2300000);
        Func_020015d2(25, 0x2080000, 0x2300000);
        Func_020015e0(26, 0x2080000, 0x2300000);
        Func_020015ee(27, 0x2080000, 0x2300000);
    } else if (Func_02001586(0x962) != 0) {
        Func_02001608(27, 0x1180000, 0x500000);
        Func_0200167c(27, 0x2000, 0);
        Func_02001624(27, 1);
    }

    return 0;
}
