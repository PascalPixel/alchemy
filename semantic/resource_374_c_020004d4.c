#include "types.h"

/*
 * Resource 374 cutscene step at 0x020004d4 (276 bytes, 28 call sites).
 *
 * Complete owner.  Prologue `push {r5, r6, r7, lr} ; mov r7, r8 ; push {r7}`
 * at 0x020004d4, interworking return `pop {r3} ; mov r8, r3 ;
 * pop {r5, r6, r7} ; pop {r0} ; bx r0` at 0x020005d2.  r0 holds the popped
 * return address, so by the epilogue rule the owner is `void`.  Bytes
 * 0x020005dc-0x020005e7 are this owner's literal pool (0x00000eae,
 * 0x0200a581, 0x0200a5a1); they are reached only through `ldr rN, [pc, #imm]`
 * and are excluded from the reconstruction.  The next prologue
 * (`push {lr}` at 0x020005e8) is a different owner and already byte-exact.
 *
 * Link base.  This overlay is linked at 0x02008000 like the other confirmed
 * overlays: the two pool words 0x0200a581 and 0x0200a5a1 are file offsets
 * 0x2580 and 0x25a0 plus the Thumb bit, and the byte-exact sources
 * `assets/code/resource_374_c_02002580.c` and `..._020025a0.c` are exactly
 * the two per-frame callbacks this owner installs and later removes.  That is
 * both the link-base witness and the identity of the two constants.
 *
 * Call targets were resolved with `tools/lib/overlay_call_targets.ts`, never from
 * the disassembler's `bl` annotation (an overlay `bl` stores
 * target_offset - 2, not a pc-relative displacement).  All 28 sites resolve:
 * 23 through the import veneer table (named here by the main-image address in
 * the veneer's trailing word) and 5 to prologues inside this overlay, three of
 * which are already byte-exact (0x020022c8, 0x020022e0, 0x020022f8) and two of
 * which are one-line wrappers (0x02002564, 0x02002574).
 *
 * Old-style declarations are used for the imports: one main-image name is
 * reached here with different argument counts at different sites
 * (Func_0808a080 with one argument, Func_0808a150 with three), so a prototype
 * would assert an interface this evidence does not establish.
 */

/* Overlay-local callees (byte-exact siblings in assets/code). */
void Func_02002b78();
void Func_02002bec();
void Func_02002c3e();
void Func_02002c5c();
void Func_020027c8(s32, s32);
void Func_020027ec(s32, s32, s32);
void Func_02002cba();
void Func_02002bb0();
void Func_02002a84(void);
s32 Func_02002bec_b();
void Func_02002824(s32);
void Func_02002b20();
void Func_02002b36();
void Func_02002b40();
void Func_02002ce4();
s32 Func_02002c22();
void Func_02002b6c();
void Func_02002b8a();
void Func_02002b90();
void Func_02002b86();
void Func_02002dec();
void Func_02002cfc();
void Func_02002d04();
void Func_02002c4a();
void Func_02002b2e(void);
void Func_02002d10();
void Func_02002d40();
void Func_02002c74();

                        

/* Per-frame callbacks installed by this owner. */
void Func_02002580(void);
void Func_020025a0(void);

/* Main-image imports reached through this overlay's veneer table. */
                               /* frame wait, argument = frame count      */
                               /* install per-frame task (callback, rank) */
                               /* remove per-frame task (callback)        */

                     
                               /* returns the actor/effect record pointer */

                     

                     

void Func_020004d4(void)
{
    u32 i;
    u8 *record;
    u8 *flag;
    u8 saved;

    Func_02002b78();
    Func_02002bec(0, 82, 760);          /* 190 << 2 */
    Func_02002c3e(15, 0, 30);
    Func_02002c5c(0x0eae);              /* literal pool word */
    Func_020027c8(15, 20);
    Func_020027ec(15, 0xa000, 20);      /* 160 << 8 */
    Func_02002cba(15, 258);             /* 129 << 1 */
    Func_02002bb0(20);
    Func_02002a84();

    /*
     * 40 frames.  r0 is not reloaded between the two branches: the value
     * Func_0808a080(15) leaves in r0 is the argument of Func_020022f8.
     * Whether Func_020022f8 is intended to consume it is unverified; the
     * dataflow is preserved as written.
     */
    for (i = 0; i <= 39; i++) {
        Func_02002824(Func_02002bec_b(15));
        Func_02002b20(1);
    }

    Func_02002b36(Func_02002580, 3200); /* 200 << 4 */
    Func_02002b40(Func_020025a0, 3200);
    Func_02002ce4(0, 0xa000, 10);

    record = (u8 *)Func_02002c22(20);
    flag = record + 0x55;
    saved = *flag;
    *flag = 0;

    /*
     * 40 frames again, advancing the record's word at +12 by 0x1800 per
     * frame (192 << 5) while the byte at +0x55 is held at zero.
     */
    for (i = 0; i <= 39; i++) {
        *(s32 *)(record + 12) += 0x1800;
        Func_02002b6c(1);
    }

    *flag = saved;

    Func_02002b8a(Func_02002580);
    Func_02002b90(Func_020025a0);
    Func_02002b86(1);
    Func_02002dec(0xa1);
    Func_02002cfc(15, 0);
    Func_02002d04(20, 0);
    Func_02002c4a(40);
    Func_02002b2e();
    Func_02002d10(0, 15, 30);
    Func_02002d40(15, 0);
    Func_02002c74();
}
