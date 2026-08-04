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
 * Call targets resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
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
void Func_020022f8(s32);
void Func_02002564(void);
void Func_02002574(void);
void Func_02002590(void);

/* Relocated IWRAM ARM helper: remainder. */
s32 Func_030003e0();

/* Main-image imports reached through this overlay's veneer table. */
void Func_080000c0();          /* frame wait                              */
void Func_080000d0();          /* install per-frame task (callback, rank) */
void Func_080000d8();          /* remove per-frame task (callback)        */
s32 Func_080770c0();           /* story-flag query, returns nonzero if set */
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
s32 Func_0808a080();
void Func_0808a130();
void Func_0808a150();
void Func_0808a158();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a190();
void Func_080f9010();

void Func_02000248(void)
{
    u8 *workspace;
    s32 *shake;
    s32 scene;
    s32 frame;
    s32 step;

    Func_0808a018();

    if (Func_080770c0(0x815) != 0) {
        scene = 0x1197;
        Func_0808a170(scene);

        workspace = Data_03001ebc;
        if (Func_080770c0(2) != 0) {
            *(u16 *)(workspace + 472) = (u16)(*(u16 *)(workspace + 472) + 1);
        }
        workspace = Data_03001ebc;
        if (Func_080770c0(3) != 0) {
            *(u16 *)(workspace + 472) = (u16)(*(u16 *)(workspace + 472) + 1);
        }

        Func_0808a178(17, 0);
        if (Func_0808a070(0, 0) == 0) {
            Func_0808a170(scene + 3);
        } else {
            Func_0808a170(scene + 4);
        }
        Func_0808a180(17, 0);
    } else {
        shake = *Data_03001e70;

        Func_0808a170(0x0f48);
        Func_0808a150(17, 0, 0);
        Func_0808a190(17, 0);
        Func_0808a010(20);
        Func_0808a130(17, 2);
        Func_0808a010(15);
        Func_02002564();

        /* 40 frames of the entry animation. */
        for (frame = 0; frame <= 39; frame++) {
            Func_020022f8(Func_0808a080(17));
            Func_080000c0(1);
        }

        Func_080000d0(Func_02002590, 3200);   /* 200 << 4 */
        Func_080f9010(107);

        /*
         * 180 frames; every tenth frame the shared shake word alternates
         * between +0x10000 and -0x10000 (the pool word 0xffff0000).
         */
        step = 0;
        for (frame = 0; frame != 180; frame++) {
            if (Func_030003e0(frame, 10) == 0) {
                if ((step & 1) != 0) {
                    *shake += (s32)0xffff0000;
                } else {
                    *shake += 0x10000;
                }
                step++;
            }
            Func_0808a010(1);
        }

        Func_080f9010(0x121);
        Func_080000d8(Func_02002590);
        Func_080000c0(1);
        Func_02002574();
        Func_0808a158(17, 0);
        Func_0808a010(40);
        Func_0808a170(0x0f4b);
        Func_0808a180(17, 0);
    }

    Func_0808a020();
}
