#include "types.h"
#define NULL ((void *)0)

/*
 * Resource 374 cutscene step at 0x02000248 (364 bytes, 33 call sites).
 *
 * Complete owner.  Prologue `push {r5, r6, r7, lr}` at 0x02000248 and the
 * interworking return `pop {r5, r6, r7} ; pop {r0} ; bx r0` at 0x0200038a.
 * r0 holds the popped return address, so the owner is `void`.  Bytes
 * 0x02000390-0x020003b3 are the owner's literal pool (0x00000815, 0x00001197,
 * 0x03001ebc, 0x03001e70, 0x00000f48, 0x0200a591, 0xffff0000, 0x00000121,
 * 0x00000f4b), reached only by `ldr rN, [pc, #imm]`.  The next prologue,
 * `push {lr}` at 0x020003b4, is the already byte-exact owner
 * `assets/code/resource_374_c_020003b4.c`.
 *
 * Link base 0x02008000: the pool word 0x0200a591 is file offset 0x2590 plus
 * the Thumb bit, i.e. the byte-exact per-frame callback
 * `assets/code/resource_374_c_02002590.c`, which this owner installs through
 * Func_080000d0 and removes through Func_080000d8.
 *
 * Call targets resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --` (an overlay `bl`
 * stores target_offset - 2, so the disassembler's annotations are unusable):
 * 30 import veneers and 3 prologues in this overlay, all three of the latter
 * already byte-exact (0x020022f8, 0x02002564, 0x02002574).
 *
 * Func_030003e0 is the ARM-mode remainder helper relocated into IWRAM, the
 * same family as the 0x030001d8 square root; the reading is taken from
 * `semantic/overlays/resource_3c4_c_020010c4.c` and matches its use here,
 * where `(frame, 10) == 0` gates one step of a 180-frame shake into 18 moves.
 *
 * The workspace pointer 0x03001ebc and the halfword counter at +472 are taken
 * from the byte-exact `assets/code/resource_374_c_020003b4.c`, which performs
 * exactly the same increment.
 *
 * Uncertainties:
 *  - Func_0808a010 is reached with 20, 15, 40 and 1; Func_0808a170 with five
 *    different scene identifiers.  Old-style declarations are therefore used
 *    throughout, since one name may carry more than one interface;
 *  - at 0x020002fc/0x02000300 r0 is not reloaded between the two branches, so
 *    Func_0808a080's result is Func_020022f8's argument; whether the callee
 *    consumes it is unverified and the dataflow is preserved as written.
 */

/* Workspace pointer globals. */
extern u8 *Data_03001ebc;
extern s32 **Data_03001e70;

/* Overlay-local callees (byte-exact siblings in assets/code). */
void Func_020028e8();
s32 Func_020028c6();
void Func_020029c2();
s32 Func_020028d8();
s32 Func_020028f2();
void Func_02002a06();
s32 Func_0200295e();
void Func_02002a10();
void Func_02002a18();
void Func_02002a30();
void Func_02002a2e();
void Func_02002a20();
void Func_02002a60();
void Func_02002976();
void Func_02002a1e();
void Func_02002984();
void Func_02002858(void);
s32 Func_020029c2_b();
void Func_020025fa(s32);
void Func_020028f6();
void Func_0200290c();
void Func_02002b6a();
s32 Func_0200290c_b();
void Func_020029e2();
void Func_02002ba4();
void Func_0200295a();
void Func_02002950();
void Func_020028dc(void);
void Func_02002ac4();
void Func_02002a0a();
void Func_02002ae0();
void Func_02002af8();
void Func_02002a2c();

                         
void Func_02002590(void);

/* Relocated IWRAM ARM helper: remainder. */

/* Main-image imports reached through this overlay's veneer table. */
                               /* frame wait                              */
                               /* install per-frame task (callback, rank) */
                               /* remove per-frame task (callback)        */
                               /* story-flag query, returns nonzero if set */

                     

                     

                     

                     

void Func_02000248(void)
{
    u8 *workspace;
    s32 *shake;
    s32 scene;
    u32 frame;
    s32 step;

    Func_020028e8();

    if (Func_020028c6(0x815) != 0) {
        scene = 0x1197;
        Func_020029c2(scene);

        workspace = Data_03001ebc;
        if (Func_020028d8(2) != 0) {
            *(u16 *)(workspace + 472) = (u16)(*(u16 *)(workspace + 472) + 1);
        }
        workspace = Data_03001ebc;
        if (Func_020028f2(3) != 0) {
            *(u16 *)(workspace + 472) = (u16)(*(u16 *)(workspace + 472) + 1);
        }

        Func_02002a06(17, 0);
        if (Func_0200295e(0, 0) == 0) {
            Func_02002a10(scene + 3);
        } else {
            Func_02002a18(scene + 4);
        }
        Func_02002a30(17, 0);
    } else {
        shake = *Data_03001e70;

        Func_02002a2e(0x0f48);
        Func_02002a20(17, 0, 0);
        Func_02002a60(17, 0);
        Func_02002976(20);
        Func_02002a1e(17, 2);
        Func_02002984(15);
        Func_02002858();

        /* 40 frames of the entry animation. */
        for (frame = 0; frame <= 39; frame++) {
            Func_020025fa(Func_020029c2_b(17));
            Func_020028f6(1);
        }

        Func_0200290c(Func_02002590, 3200);   /* 200 << 4 */
        Func_02002b6a(107);

        /*
         * 180 frames; every tenth frame the shared shake word alternates
         * between +0x10000 and -0x10000 (the pool word 0xffff0000).
         */
        step = 0;
        for (frame = 0; frame != 180; frame++) {
            if (Func_0200290c_b(frame, 10) == 0) {
                if ((step & 1) != 0) {
                } else {
                    *shake += 0x10000;
                    *shake += (s32)0xffff0000;
                }
                step++;
            }
            Func_020029e2(1);
        }

        Func_02002ba4(0x121);
        Func_0200295a(Func_02002590);
        Func_02002950(1);
        Func_020028dc();
        Func_02002ac4(17, 0);
        Func_02002a0a(40);
        Func_02002ae0(0x0f4b);
        Func_02002af8(17, 0);
    }

    Func_02002a2c();
}
