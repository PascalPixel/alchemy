#include "types.h"

/*
 * Resource 3ad, scene-entry hook at 0x02001a0c (200 bytes, 19 call sites).
 *
 * Complete owner: `push {r5, lr}` at 0x02001a0c, `movs r0,#0 / pop {r5} /
 * pop {r1} / bx r1` at 0x02001ab0.  The popped branch register is r1, so r0
 * survives and is the result; it is unconditionally 0 on both paths, so the
 * owner returns s32 0.  A seven-word literal pool occupies 0x02001ab8-0x02001ad3,
 * past the return and never reached as code:
 *   0x03001ebc (the shared workspace pointer cell), 0x02000240 (the shared
 *   scene-state table), 0x6a, 0xf333, 0x201, 0x202, 0x203.
 *
 * `Data_02000240[224]` (the signed halfword at byte offset 448) against the
 * small constant 0x6a is the cross-overlay scene-id idiom; the byte-exact
 * sibling `assets/code/resource_3ad_c_02000044.c` tests the same halfword
 * against the same value, spelled there as `(s32)&Value_0000006a`.
 *
 * r5 is loaded once with 0x204 (`movs r5,#129 / lsls r5,#2`) and serves two
 * purposes: it is the word stored into the workspace at +448, and it is still
 * live 140 bytes later as the story-flag id at 0x02001a9e.  Reading it as dead
 * scratch there would drop an argument.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`.  Per-target
 * multiset over the 19 sites: Func_0808a080 x5, Func_080770c0 x5,
 * Func_080091e0 x4, Func_0808a100 x2, and one each of Func_02000210,
 * Func_02000384, Func_020000d4 - 19 C call expressions below, matching the
 * row's calls=19.
 */

/* Old-style declarations: overlay import arities vary per call site. */
u8 *Func_02003566();
void Func_02003524();
u8 *Func_02003572();
void Func_02003530();
u8 *Func_0200357e();
void Func_0200353c();
u8 *Func_0200358a();
void Func_02003548();
u8 *Func_02003596();
s32 Func_02003568();
void Func_02001c7c();
s32 Func_02003576();
void Func_02001dfe();
s32 Func_02003586();
void Func_02001b5e();
s32 Func_02003594();
void Func_02003610();
s32 Func_020035a6();
void Func_02003622();
                        /* scene entity record by selector */

                        /* test a story flag (used in a condition) */
                        /* this overlay */
                        /* this overlay */
                        /* this overlay */

extern s16 Data_02000240[];

s32 Func_02001a0c(void)
{
    u8 *workspace;
    s32 flagId;

    flagId = 0x204;
    workspace = *(u8 **)0x03001ebc;
    *(s32 *)(workspace + 448) = flagId;

    if (Data_02000240[224] != 0x6a) {
        return 0;
    }

    Func_02003524(Func_02003566(8), 0);
    Func_02003530(Func_02003572(9), 0);
    Func_0200353c(Func_0200357e(10), 0);
    Func_02003548(Func_0200358a(11), 0);

    /* Word field at +28 of entity 11; 0xf333 is pooled, not computed. */
    *(s32 *)(Func_02003596(11) + 28) = 0xf333;

    if (Func_02003568(0x201) != 0) {
        Func_02001c7c();
    }
    if (Func_02003576(0x202) != 0) {
        Func_02001dfe();
    }
    /* 0x200 built as `movs r0,#128 / lsls r0,#2`. */
    if (Func_02003586(0x200) != 0) {
        Func_02001b5e();
    }
    if (Func_02003594(0x203) != 0) {
        Func_02003610(11, 5);
    }
    if (Func_020035a6(flagId) != 0) {
        Func_02003622(9, 5);
    }

    return 0;
}
