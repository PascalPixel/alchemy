#include "types.h"

/*
 * Resource 37a gated cutscene at 0x02000488 (1388 bytes,
 * 0x02000488-0x020009f3).
 *
 * Role, settled BEFORE any disassembly (call-graph-first, HANDOVER §0):
 * `assets/code/resource_37a_overlay.s` holds the pool word 0x02008489, which
 * under the proven 0x02008000 link base is 0x02000488 + the Thumb bit — this
 * owner's own entry.  It sits in the overlay's script-command table as the
 * triple `0xffff000b / 0x02008489 / 0x00000002`, the same shape that names
 * 0x02000054 (0xffff000d), 0x02000108 (0xffff000f), 0x02000150 (0xffff000e),
 * 0x020001ec (0xffff000c), 0x0200022c (0xffff0013), 0x02000c00 (0xffff0009),
 * 0x02001a58 (0xffff0008), 0x02001ca8 (0xffff0014) and 0x02002924
 * (0xffff0010).  Nine of those are already tracked byte-exact or semantic
 * siblings, so this owner is a scripted-scene command handler taking no
 * arguments, exactly like them.
 *
 * Link base proof for this overlay (three independent witnesses, all odd pool
 * words resolving to already-tracked sibling entries under
 * in-image address = pool_word - 0x8000):
 *   0x02008d9d -> Func_02000d9c + 1   (assets/code/resource_37a_c_02000d9c.c)
 *   0x02008ef9 -> Func_02000ef8 + 1   (assets/code/resource_37a_c_02000ef8.c)
 *   0x0200901d -> Func_0200101c + 1   (assets/code/resource_37a_c_0200101c.c)
 *   0x02009141 -> Func_02001140 + 1   (assets/code/resource_37a_c_02001140.c)
 * plus 0x02008151/0x020081ed/0x0200822d/0x02008109/0x02008c01/0x02009a59/
 * 0x0200a925 naming seven more tracked entries.  The base is not in doubt here.
 *
 * Complete owner: `push {lr}` prologue at 0x02000488 and the matching
 * interworking return `pop {r0} / bx r0` at 0x020009da.  r0 holds the popped
 * return address, so nothing is returned: the owner is `void`.
 *
 * Pool map, derived by walking the owner's control flow from the prologue —
 * NOT from the disassembler's guess.  This owner has TWO literal pools and the
 * first one ends mid-row:
 *   0x0200081c-0x0200084f   reached only by `ldr rN,[pc,#k]`; the preceding
 *                           instruction at 0x0200081a is the bare forward
 *                           `b.n 0x02000860`, which hops it, and the two arms
 *                           of the 0x02000808 test rejoin at 0x02000860.
 *                           Code resumes at 0x02000850 (the `bne` arm) and
 *                           runs on to the epilogue.
 *   0x020009de-0x020009f3   the trailing pool after the return, preceded by the
 *                           usual 2-byte zero alignment word at 0x020009de.
 * Pool words, in order:
 *   0x00000814 0x00000809 0x00000fe3 0x00016666 0x0000b333 0x00009999
 *   0x00001333 0x00006666 0x00003333 0x00002005 0x00000101 0x00000105
 *   0x00000ff0 | 0x00000ff1 0x00000107 0x00000ff2 0x00002005 0x00000809
 * None of them decodes as a BL prefix, so no phantom call site is introduced
 * by either pool.
 *
 * All 140 call sites were resolved with tools/lib/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2); the disassembler's printed
 * `bl` targets are the usual overlay artefact and are NOT the callees.
 * sites=140, distinct_targets=28 (123 veneer, 17 overlay-local prologue),
 * against the inventory's calls=132 — that field counts distinct targets and
 * is a known low lower bound, so it is a floor, not an equality test.
 *
 * Per-target multiset, tool count == C call count, checked per target and not
 * as a total (a total match can hide a permutation):
 *   Func_02000054    1     Func_020025fc   16     Func_080770c0    2
 *   Func_080770c8    2     Func_0808a010   15     Func_0808a018    1
 *   Func_0808a020    1     Func_0808a070    1     Func_0808a080    6
 *   Func_0808a090    8     Func_0808a0b8    3     Func_0808a0c8    1
 *   Func_0808a0d0    4     Func_0808a0e8    3     Func_0808a0f0    6
 *   Func_0808a100    7     Func_0808a110    9     Func_0808a128    3
 *   Func_0808a138    6     Func_0808a170    4     Func_0808a178    1
 *   Func_0808a180    1     Func_0808a1b8   21     Func_0808a1e8   12
 *   Func_0808a1f0    2     Func_0808a208    1     Func_0808a210    1
 *   Func_080f9010    2
 *
 * Control flow, both branches verified against the resolved targets:
 *  - 0x02000492: `Func_080770c0(0x0814) != 0` runs the sibling command handler
 *    Func_02000054 (tracked byte-exact) before the scene.
 *  - 0x020004a0: `Func_080770c0(0x0809) != 0` branches straight to the
 *    epilogue at 0x020009da, so the whole scene is one-shot on flag 0x0809 —
 *    and the scene's own closing call is `Func_080770c8(0x0809)`, the setter
 *    for that same flag.  Gate and setter agree, which settles the one-shot
 *    reading with no dataflow work.
 *  - 0x02000810: `Func_0808a070(0, 0)` selects between cue 0x0ff0 (zero arm,
 *    fall-through) and cue 0x0ff1 plus one extra Func_0808a1e8(16, 0x107, 20)
 *    (non-zero arm).  The two arms rejoin at 0x02000860; the shared tail is
 *    written once after the `if/else`, not copied per arm, so the multiset is
 *    neither inflated nor deflated.
 * Func_080770c0/Func_080770c8 are the global story-flag test/set pair resolved
 * in semantic/overlays/resource_3cb_c_020010e8.c and re-used by this overlay's
 * own semantic/overlays/resource_37a_c_020025b0.c.
 *
 * Func_0808a080 is the scene-record accessor (the sense established in
 * HANDOVER: 0x0808a080 is the accessor, not the four-argument action).  It is
 * called six times, always with selector 0 and always null-checked.  Three of
 * those results feed Func_0808a0f0 from the 16.16 words at +0x08 and +0x10;
 * the other three feed Func_0808a0b8 from the SIGNED HALFWORDS at +0x0a and
 * +0x12, which are the integer parts of those same two 16.16 words (the
 * halfword-coordinate-view idiom).  The `movs r3,#10 / ldrsh r1,[r0,r3]` shape
 * means r3 is a load offset, not a trailing argument.
 *
 * Func_020025fc is this overlay's own two-argument helper (17 prologue-class
 * sites here; the same helper the siblings at 0x020009f4 and 0x02001ca8 call).
 * Two of its calls here pass 0x2005 rather than a small actor id; the sibling
 * at 0x020009f4 passes 0x1010 and 0x4010 the same way, so a packed id in that
 * register is an established shape in this overlay rather than a decoding
 * error.
 *
 * Shift-built constants are spelled with their assembled values and the source
 * shift noted, because the assembly builds them from 8-bit immediates.
 *
 * Uncertainties: 0x0814, 0x0809 and 0x0144 are story-flag ids, not addresses
 * under the link base; Func_0808a018 and Func_0808a020 are reached with no
 * argument register set by this owner (the scene begin/end bracket); the
 * record field widths are taken from `ldr`/`ldrsh` and their fixed-point
 * reading from the magnitudes involved.  Arities are taken from the registers
 * written before each site.
 */

/* Overlay-local callees. */
s32 Func_02002e42();
void Func_020004ea(void);
s32 Func_02002e50();
void Func_02002e7a();
void Func_02002f00();
void Func_02002fa6();
void Func_02002eb4();
void Func_02002ed8();
void Func_02002ef8();
void Func_02002ea6();
void Func_02002fd4();
void Func_02002f60();
s32 *Func_02002ede();
void Func_02002f1c();
void Func_02002efe();
void Func_02002f22();
void Func_02002ee8();
void Func_02002fa4();
void Func_02002f66();
void Func_02002b36();
s32 *Func_02002f2c();
void Func_02002f6a();
s32 *Func_02002f40();
void Func_02002f7e();
void Func_02002f64();
void Func_02002f72();
void Func_02002f8e();
void Func_02002fa2();
void Func_02002fc2();
void Func_0200301e();
void Func_0200302a();
void Func_02003052();
void Func_0200306c();
void Func_02002fce();
void Func_02002ff2();
void Func_02002fb8();
void Func_02003030();
void Func_02002bf8();
void Func_0200307c();
void Func_02002c0c();
void Func_0200308e();
void Func_0200304e();
void Func_02002ff4();
void Func_020030a8();
void Func_02003068();
void Func_0200300e();
void Func_02003086();
void Func_020030ca();
void Func_02002c5a();
void Func_020030a2();
void Func_020030e6();
void Func_02002c76();
void Func_02003108();
void Func_02003056();
void Func_0200310a();
void Func_02002c9a();
void Func_020030e2();
void Func_02003078();
void Func_0200312c();
void Func_02003146();
void Func_02003094();
void Func_02003148();
void Func_02003154();
void Func_02003160();
void Func_0200316c();
void Func_0200317e();
void Func_0200316e();
void Func_020030dc();
void Func_02003144();
void Func_020030ea();
void Func_02002d22();
void Func_020031ac();
void Func_02002d34();
void Func_02002d44();
void Func_020031ce();
void Func_020031d8();
void Func_020031e2();
void Func_020031aa();
void Func_02003140();
void Func_02002d78();
void Func_020031b0();
void Func_02003156();
void Func_02003212();
void Func_0200321e();
void Func_0200322a();
void Func_02003234();
void Func_02002dbc();
void Func_02003240();
void Func_0200324c();
void Func_02003258();
void Func_02003264();
void Func_020031ca();
void Func_02003262();
s32 Func_020031f2();
void Func_0200326c();
void Func_020032a8();
void Func_020032da();
void Func_0200329c();
void Func_020032e8();
void Func_020032f4();
void Func_02003300();
void Func_02002e90();
void Func_020032ee();
void Func_020032d4();
void Func_02002eac();
void Func_020032e4();
void Func_02002ebc();
void Func_020032fe();
void Func_02003352();
void Func_02002eda();
void Func_0200330a();
void Func_02003312();
void Func_02003322();
void Func_020032c8();
void Func_02003330();
void Func_02002f08();
void Func_0200330e();
void Func_0200331c();
void Func_0200332a();
void Func_02003362();
s32 *Func_02003330_b();
void Func_02003370();
void Func_02003382();
void Func_02003392();
s32 *Func_02003360();
void Func_020033a0();
void Func_020033b2();
void Func_020033c2();
s32 *Func_02003390();
void Func_020033d0();
void Func_020033e2();
void Func_0200338a();
void Func_02003390_b();
void Func_020033b4();

/* Old-style declarations: arities vary per call site across this overlay. */

                     

                     

                     

                     

                     

                     

                     

/* Used for their return values, so they need a real return type. */

                     

void Func_02000488(void)
{
    s32 *record;

    if (Func_02002e42(0x814) != 0)
        Func_020004ea();

    /* One-shot gate: the scene's own tail sets this same flag. */
    if (Func_02002e50(0x809) != 0)
        return;

    Func_02002e7a();
    Func_02002f00(0xfe3);
    Func_02002fa6(17);

    Func_02002eb4(0, 0x10000, 0x8000);          /* 128 << 9, 128 << 8 */
    Func_02002ed8(0, 288, 232);                 /* 144 << 1 */
    Func_02002ef8(0, 0);
    Func_02002ea6(20);
    Func_02002fd4(21);
    Func_02002f60(0, 0xc000, 0);                /* 192 << 8 */

    record = Func_02002ede(0);
    if (record != 0)
        Func_02002f1c(16, record[2], record[4]);    /* +0x08, +0x10 */

    Func_02002efe(16, 0x16666, 0xb333);
    Func_02002f22(16, 288, 206);                /* 144 << 1 */
    Func_02002ee8(40);
    Func_02002fa4(16, 256, 0);                  /* 128 << 1 */
    Func_02002f66(16, 4, 60);
    Func_02002b36(16, 20);

    record = Func_02002f2c(0);
    if (record != 0)
        Func_02002f6a(1, record[2], record[4]);

    record = Func_02002f40(0);
    if (record != 0)
        Func_02002f7e(5, record[2], record[4]);

    Func_02002f64(1, 0x8000, 0x4000);           /* 128 << 8, 128 << 7 */
    Func_02002f72(5, 0x8000, 0x4000);
    Func_02002f8e(1, 280, 248);                 /* 140 << 1 */
    Func_02002fa2(5, 296, 248);                 /* 148 << 1 */
    Func_02002fc2(1, 1);
    Func_0200301e(1, 0xd000, 0);                /* 208 << 8 */
    Func_0200302a(5, 0xb000, 30);               /* 176 << 8 */
    Func_02003052(0x9999, 0x1333);
    Func_0200306c(0x1200000, -1, 0xd50000, 1);  /* 144 << 17, 213 << 16 */

    Func_02002fce(16, 0x6666, 0x3333);
    Func_02002ff2(16, 288, 176);                /* 144 << 1 */
    Func_02002fb8(40);
    Func_02003030(16, 2);
    Func_02002bf8(16, 6);
    Func_0200307c(16, 0x4000, 60);              /* 128 << 7 */
    Func_02002c0c(16, 20);
    Func_0200308e(16, 0, 40);
    Func_0200304e(16, 3);
    Func_02002ff4(10);
    Func_020030a8(16, 0x8000, 40);              /* 128 << 8 */
    Func_02003068(16, 3);
    Func_0200300e(20);

    Func_02003086(5, 2);
    Func_020030ca(5, 0x9000, 10);               /* 144 << 8 */
    Func_02002c5a(5, 10);
    Func_020030a2(1, 2);
    Func_020030e6(1, 0xf000, 10);               /* 240 << 8 */
    Func_02002c76(1, 6);
    Func_02003108(5, 258);                      /* 129 << 1 */
    Func_02003056(40);
    Func_0200310a(5, 0xa000, 10);               /* 160 << 8 */
    Func_02002c9a(0x2005, 10);

    Func_020030e2(16, 2);
    Func_02003078(10);
    Func_0200312c(16, 0xa000, 20);              /* 160 << 8 */
    Func_02003146(16, 258);                     /* 129 << 1 */
    Func_02003094(20);

    Func_02003148(0, 0x5000, 40);               /* 160 << 7 */
    Func_02003154(0, 0xc000, 0);                /* 192 << 8 */
    Func_02003160(1, 0xe000, 0);                /* 224 << 8 */
    Func_0200316c(5, 0xa000, 40);               /* 160 << 8 */
    Func_0200317e(1, 0x101, 20);
    Func_0200316e(1, 0);
    Func_020030dc(60);
    Func_02003144(16, 4);
    Func_020030ea(40);
    Func_02002d22(16, 20);
    Func_020031ac(5, 0x101, 40);
    Func_02002d34(5, 60);
    Func_0200316c(16, 3);
    Func_02002d44(16, 10);
    Func_020031ce(0, 0x105, 0);
    Func_020031d8(1, 0x105, 0);
    Func_020031e2(5, 0x105, 60);
    Func_020031aa(1, 2);
    Func_02003140(20);
    Func_02002d78(1, 10);
    Func_020031b0(16, 3);
    Func_02003156(20);
    Func_02003212(0, 258, 0);                   /* 129 << 1 */
    Func_0200321e(1, 258, 0);
    Func_0200322a(5, 258, 80);
    Func_02003234(16, 0x105, 80);
    Func_02002dbc(16, 6);
    Func_02003240(0, 0x4000, 0);                /* 128 << 7 */
    Func_0200324c(1, 0xf000, 0);                /* 240 << 8 */
    Func_02003258(5, 0x9000, 60);               /* 144 << 8 */
    Func_02003264(16, 0x4000, 10);              /* 128 << 7 */
    Func_02003234(16, 3);
    Func_020031ca(6);
    Func_02003262(16, 0);

    if (Func_020031f2(0, 0) == 0) {
        Func_0200326c(0xff0);
    } else {
        Func_020032a8(0xff1);
        Func_020032da(16, 0x107, 20);
    }

    /* Shared tail of both arms (the rejoin at 0x02000860). */
    Func_0200329c(16, 4, 20);
    Func_020032e8(0, 0xc000, 0);                /* 192 << 8 */
    Func_020032f4(1, 0xe000, 0);                /* 224 << 8 */
    Func_02003300(5, 0xa000, 0);                /* 160 << 8 */
    Func_02002e90(16, 6);
    Func_020032ee(0xff2);
    Func_0200326c(30);
    Func_020032d4(5, 4);
    Func_02002eac(0x2005, 6);
    Func_020032e4(1, 3);
    Func_02002ebc(1, 20);
    Func_020032fe(16, 6, 20);
    Func_02003352(16, 260, 20);                 /* 130 << 1 */
    Func_02002eda(16, 30);
    Func_0200330a(0, 3);
    Func_02003312(1, 3);
    Func_02003322(5, 3);
    Func_020032c8(20);
    Func_02003330(16, 3);
    Func_02002f08(16, 6);

    Func_0200330e(1, 0x10000, 0x8000);          /* 128 << 9, 128 << 8 */
    Func_0200331c(5, 0x10000, 0x8000);
    Func_0200332a(16, 0x20000, 0x10000);        /* 128 << 10, 128 << 9 */

    Func_02003362(16, 2);
    record = Func_02003330_b(0);
    if (record != 0)
        Func_02003352(16, ((s16 *)record)[5], ((s16 *)record)[9]);  /* +0x0a, +0x12 */
    Func_02003370(16);
    Func_02003382(16, 0, 0);

    Func_02003392(1, 2);
    record = Func_02003360(0);
    if (record != 0)
        Func_02003382(1, ((s16 *)record)[5], ((s16 *)record)[9]);
    Func_020033a0(1);
    Func_020033b2(1, 0, 0);

    Func_020033c2(5, 2);
    record = Func_02003390(0);
    if (record != 0)
        Func_020033b2(5, ((s16 *)record)[5], ((s16 *)record)[9]);
    Func_020033d0(5);
    Func_020033e2(5, 0, 0);

    Func_0200338a(0x144);                       /* 162 << 1 */
    Func_02003390_b(0x809);
    Func_020033b4();
}
