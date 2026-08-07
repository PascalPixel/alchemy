#include "types.h"

/*
 * Resource 3ca cutscene body at 0x02000430 (1796 bytes,
 * 0x02000430-0x02000b33).
 *
 * Complete owner: `push {r5, r6, lr}` / `sub sp, #8` at 0x02000430, matching
 * `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0` at 0x02000af8.  The popped
 * register is r0, so it holds the return address and the owner is `void`.
 * The 8-byte frame exists only to carry the fifth and sixth arguments of the
 * single Func_080091c0 call at 0x0200045a.
 *
 * Two literal pools sit inside the span.  The map comes from walking the
 * owner's control flow, which is the only method immune to both pool traps:
 *   0x02000808-0x02000813, reached only by falling past the `b.n 0x02000840`
 *     at 0x02000806 (an arm-join branch, not a dedicated pool hop), and
 *   0x02000b00-0x02000b33, after the epilogue.
 * Pool contents: 0x000028fe, 0x03001ebc, 0x00000101 in the first; 0x03001ebc,
 * 0x00013333, 0x00009999, 0x02009820, 0x020098e0, 0x0200998c, 0x02009a4c,
 * 0x03001e70, 0xffffd000, 0xffffa000, 0x00023333, 0x0000028f, 0x00000000 in
 * the second.  The four 0x02009xxx words are even, so under the 0x02008000
 * link base they are in-image DATA addresses (file offsets 0x1820, 0x18e0,
 * 0x198c, 0x1a4c), not callbacks — the parity test.  Each is the second
 * argument of a Func_0808a098 call, which is exactly the shape that test was
 * derived from.
 *
 * Calls.  188 `bl` sites, all resolved with tools/lib/overlay_call_targets.ts;
 * the row's `calls=174` is the usual undercount.  The completeness proof is
 * per target, not a total — a matching total can hide a permutation:
 *
 *   Func_0808a010 x70   Func_0808a1b8 x28   Func_0808a180 x18
 *   Func_0808a110 x12   Func_0808a100  x7   Func_0808a138  x6
 *   Func_0808a1e8  x6   Func_0808a210  x4   Func_0808a090  x4
 *   Func_0808a098  x4   Func_0808a070  x3   Func_0808a178  x3
 *   Func_0808a570  x3   Func_0808a580  x3   Func_0808a0a0  x2
 *   Func_080f9010  x2   Func_080091c0  x1   Func_080770c8  x1
 *   Func_0808a018  x1   Func_0808a080  x1   Func_0808a0e8  x1
 *   Func_0808a170  x1   Func_0808a208  x1   Func_0808a218  x1
 *   Func_0808a238  x1   Func_0808a368  x1   Func_0808a370  x1
 *   Func_080f9020  x1   Func_020012ac  x1
 *   Total 188.
 *
 * Every call site here is straight-line except three, and all three are the
 * same shape: `Func_0808a070(0, 0)` returns a flag, and the two arms it
 * selects contain the *same* calls with the same arguments, differing only in
 * where the counter bump sits relative to the last call.  That is the
 * documented skip-beat counter, and it means the arms are behaviourally
 * identical.  They are nevertheless written out separately, because they are
 * genuinely distinct call sites in the image: collapsing them would drop six
 * calls from the multiset above, which is the mirror of the phantom-call trap.
 *
 * Two arithmetic idioms that are easy to misread and are spelled for their
 * effect here:
 *  - at 0x02000abe the displacement 224*2 = 448 is loaded, the address is
 *    formed, and only *then* is `subs r3, #192` applied to make the stored
 *    value 32.  The store is workspace + 448 = 32, not workspace + 256.
 *    Compare 0x02000da4, which writes 256 to the same field.
 *  - the counter bump is `ldr r2, [0x03001ebc] / adds r2, #472 / ldrh / adds
 *    #1 / strh`: a u16 at workspace + 472, where 472 is 236 << 1.
 *
 * UNCERTAINTIES.  (1) Func_0808a010 is read as "wait n frames" from the way
 * every site separates two visible state changes; the name is not otherwise
 * pinned down.  (2) Func_0808a018, Func_0808a218, Func_0808a368 and
 * Func_0808a370 are reached with no register deliberately set and are spelled
 * with no arguments.  (3) Func_0808a010's first argument is the only one this
 * owner ever varies for it, so its remaining registers are not asserted.
 */

/* The scene workspace pointer cell in IWRAM, and the record block. */
#define WORKSPACE   (*(u8 **)0x03001EBC)
#define RECORDS     (*(u8 **)0x03001E70)

/* The skip-beat counter: a u16 at workspace + 472 (236 << 1). */
#define BUMP_BEAT() (*(u16 *)(WORKSPACE + 472) += 1)

/* In-image data handed to Func_0808a098; even pool words under the 0x02008000
 * base, so data rather than callbacks. */
extern u8 Data_02009820[];
extern u8 Data_020098e0[];
extern u8 Data_0200998c[];
extern u8 Data_02009a4c[];

/* Main-image imports reached through this overlay's veneer table.  Old-style
 * declarations are mandatory: import arity varies between call sites, and two
 * names can be one callee. */
void Func_020017f2();
void Func_02001894();
void Func_020017e8();
void Func_0200187e();
void Func_020018f6();
void Func_02001904();
void Func_02001912();
void Func_02001890();
void Func_0200184e();
void Func_020018b6();
void Func_0200185c();
void Func_020018dc();
void Func_0200186a();
void Func_020018ca();
void Func_02001878();
void Func_020018f8();
void Func_02001886();
void Func_02001898();
void Func_02001918();
void Func_020018a6();
void Func_02001932();
void Func_0200193e();
void Func_0200194a();
void Func_020018d0();
void Func_02001928();
void Func_02001930();
void Func_02001940();
void Func_020018ee();
void Func_02001956();
void Func_020018fc();
void Func_02001988();
void Func_0200190e();
void Func_02001986();
s32  Func_02001936();
void Func_020019bc();
void Func_020019b4();
void Func_02001954();
void Func_020019e8();
void Func_020019f0();
void Func_0200197e();
void Func_02001a0a();
void Func_02001990();
void Func_02001a22();
void Func_02001a1a();
void Func_020019a8();
void Func_02001a32();
void Func_020019b8();
void Func_02001a4a();
void Func_02001a2a();
void Func_020019d0();
void Func_02001a5c();
void Func_020019e2();
void Func_02001a62();
void Func_02001a84();
void Func_02001a7c();
void Func_02001a72();
void Func_02001a18();
void Func_02001a98();
void Func_02001a26();
void Func_02001ab2();
void Func_02001a38();
void Func_02001a46();
void Func_02001abe();
s32  Func_02001a6e();
void Func_02001a60();
void Func_02001ac8();
void Func_02001a6e_b();
void Func_02001afa();
void Func_02001a80();
void Func_02001b00();
void Func_02001aa0();
void Func_02001b08();
void Func_02001aae();
void Func_02001b3a();
void Func_02001ac0();
void Func_02001b50();
void Func_02001ade();
void Func_02001b72();
void Func_02001af0();
void Func_02001b7c();
void Func_02001b02();
void Func_02001b62();
void Func_02001b10();
void Func_02001b90();
void Func_02001b1e();
void Func_02001b9e();
void Func_02001b2c();
void Func_02001b8c();
void Func_02001b40();
void Func_02001ba0();
void Func_02001b4e();
void Func_02001bc6();
void Func_02001be2();
void Func_02001bee();
void Func_02001bfa();
void Func_02001b80();
s32  Func_02001ba0_b();
void Func_02001b92();
void Func_02001bf2();
void Func_02001c20();
void Func_02001bcc();
void Func_02001c2c();
void Func_02001bda();
void Func_02001c6a();
void Func_02001bf8();
void Func_02001c84();
void Func_02001c0a();
void Func_02001c8a();
void Func_02001c18();
void Func_02001ca4();
void Func_02001c2a();
void Func_02001c30();
void Func_02001cbc();
void Func_02001c42();
void Func_02001ca2();
void Func_02001c50();
void Func_02001cd0();
void Func_02001c5e();
void Func_02001cbe();
void Func_02001c6c();
void Func_02001cec();
void Func_02001d62();
void Func_02001c80();
void Func_02001cd8();
void Func_02001ce0();
void Func_02001cf0();
void Func_02001c9e();
void Func_02001cda();
void Func_02001ce4();
void Func_02001cee();
void Func_02001cfe();
void Func_02001cd4();
void Func_02001d7e();
void Func_02001d1e();
void Func_02001cf4();
void Func_02001d2c();
void Func_02001d3a();
void Func_02001d8e();
void Func_02001d9a();
void Func_02001da6();
void Func_02001e0a();
void Func_02001d38();
void Func_02001dc4();
void Func_02001d4a();
void Func_02001dd6();
void Func_02001d5c();
void Func_02001d94();
void Func_02001da2();
void Func_02001df6();
void Func_02001d7c();
void Func_02001e26();
void Func_02001e32();
void Func_02001d98();
void Func_02001e24();
void Func_02001e30();
void Func_02001e3c();
void Func_02001e48();
void Func_02001dce();
void Func_02001e2e();
void Func_02001e36();
void Func_02001e46();
void Func_02001df4();
void Func_02001ee2();
void Func_02001ef0();
void Func_02001cfc(void);
u8  *Func_02001e34();
void Func_02001f0a();
void Func_02001f18();
void Func_02001e4c();
void Func_02001ee4();
void Func_02001efe();
void Func_02001e6e();
void Func_02001f48();
void Func_02001f54();
void Func_02001e9a();
void Func_02001f52();

                        /* wait n frames */
                     
                        /* returns the branch flag */
                        /* scene-record accessor: slot id -> record */

                     

                     

                     

                     

                     

                     

                     

/* In-image callee: the scene entry point that installs the curtain task. */

void Func_02000430(void)
{
    u8 *record;
    u8 *workspace;
    s32 *swing;

    Func_020017f2();
    Func_02001894(-1, -1, -1, 0);
    /* Six arguments; the last two, 18 and 12, travel in the frame. */
    Func_020017e8(18, 0, 3, 1, 18, 12);
    Func_0200187e(0x28fe);

    Func_020018f6(1, -10, 16, 0xc000);
    Func_02001904(3, 0, 24, 0xc000);
    Func_02001912(2, 10, 16, 0xc000);
    Func_02001890(1);

    Func_0200184e(50);
    Func_020018b6(1, 2);
    Func_0200185c(20);
    Func_020018dc(1, 0);
    Func_0200186a(10);
    Func_020018ca(2, 3);
    Func_02001878(20);
    Func_020018f8(2, 0);
    Func_02001886(10);
    Func_02001912(3, 0x4000, 0);
    Func_02001898(40);
    Func_02001918(3, 0);
    Func_020018a6(10);
    Func_02001932(0, 0x4000, 0);
    Func_0200193e(1, 0x4000, 0);
    Func_0200194a(2, 0x4000, 0);
    Func_020018d0(60);
    Func_02001928(0, 3);
    Func_02001930(1, 3);
    Func_02001940(2, 3);
    Func_020018ee(60);
    Func_02001956(1, 2);
    Func_020018fc(30);
    Func_02001988(1, 0xe000, 0);
    Func_0200190e(20);
    Func_02001986(1, 0);

    /* First skip-beat pair: the same three calls either way. */
    if (Func_02001936(0, 0) == 0) {
        Func_02001928(30);
        Func_020019bc(1, 258, 50);      /* 129 << 1 */
        Func_020019b4(1, 0);
        BUMP_BEAT();
    } else {
        Func_02001954(30);
        Func_020019e8(1, 258, 50);
        BUMP_BEAT();
        Func_020019f0(1, 0);
    }

    Func_0200197e(10);
    Func_02001a0a(2, 0x8000, 0);
    Func_02001990(30);
    Func_02001a22(2, 0x101, 50);
    Func_02001a1a(2, 0);
    Func_020019a8(10);
    Func_02001a32(1, 0, 0);
    Func_020019b8(30);
    Func_02001a4a(1, 0x101, 60);
    Func_02001a2a(3, 2);
    Func_020019d0(30);
    Func_02001a5c(3, 0xa000, 0);
    Func_020019e2(30);
    Func_02001a62(3, 0);
    Func_020019f0(10);
    Func_02001a84(1, 256, 50);          /* 128 << 1 */
    Func_02001a7c(1, 0);
    Func_02001a0a(10);
    Func_02001a72(2, 2);
    Func_02001a18(20);
    Func_02001a98(2, 0);
    Func_02001a26(10);
    Func_02001ab2(3, 0xc000, 0);
    Func_02001a38(20);
    Func_02001a98(3, 4);
    Func_02001a46(20);
    Func_02001abe(3, 0);

    /* Second skip-beat pair. */
    if (Func_02001a6e(0, 0) == 0) {
        Func_02001a60(20);
        Func_02001ac8(1, 2);
        Func_02001a6e_b(20);
        Func_02001afa(1, 0xe000, 0);
        Func_02001a80(20);
        Func_02001b00(1, 0);
        BUMP_BEAT();
    } else {
        Func_02001aa0(20);
        Func_02001b08(1, 2);
        Func_02001aae(20);
        Func_02001b3a(1, 0xe000, 0);
        Func_02001ac0(20);
        BUMP_BEAT();
        Func_02001b50(1, 0);
    }

    Func_02001ade(10);
    Func_02001b72(0, 258, 50);
    Func_02001af0(10);
    Func_02001b7c(2, 0xa000, 0);
    Func_02001b02(20);
    Func_02001b62(2, 3);
    Func_02001b10(20);
    Func_02001b90(2, 0);
    Func_02001b1e(20);
    Func_02001b9e(1, 0);
    Func_02001b2c(20);
    Func_02001b8c(0, 3);
    Func_02001b3a(30);
    Func_02001b40(10);
    Func_02001ba0(3, 3);
    Func_02001b4e(20);
    Func_02001bc6(3, 0);
    Func_02001be2(1, 0xe000, 0);
    Func_02001bee(3, 0xc000, 0);
    Func_02001bfa(2, 0xa000, 0);
    Func_02001b80(20);

    /* Third skip-beat pair. */
    if (Func_02001ba0_b(0, 0) == 0) {
        Func_02001b92(30);
        Func_02001bf2(3, 3);
        Func_02001ba0(20);
        Func_02001c20(3, 0);
        BUMP_BEAT();
    } else {
        Func_02001bcc(30);
        Func_02001c2c(3, 3);
        Func_02001bda(20);
        BUMP_BEAT();
        Func_02001c6a(3, 0);
    }

    Func_02001bf8(10);
    Func_02001c84(1, 0xc000, 0);
    Func_02001c0a(30);
    Func_02001c8a(1, 0);
    Func_02001c18(10);
    Func_02001ca4(0, 0xc000, 0);
    Func_02001c2a(30);
    Func_02001c30(10);
    Func_02001cbc(2, 0xc000, 0);
    Func_02001c42(30);
    Func_02001ca2(2, 3);
    Func_02001c50(20);
    Func_02001cd0(2, 0);
    Func_02001c5e(10);
    Func_02001cbe(3, 3);
    Func_02001c6c(20);
    Func_02001cec(3, 0);

    Func_02001d62(17);
    Func_02001c80(10);
    Func_02001cd8(0, 3);
    Func_02001ce0(1, 3);
    Func_02001cf0(2, 3);
    Func_02001c9e(40);

    Func_02001cd0(0, 0x13333, 0x9999);
    Func_02001cda(1, 0x13333, 0x9999);
    Func_02001ce4(2, 0x13333, 0x9999);
    Func_02001cee(3, 0x13333, 0x9999);

    Func_02001cfe(0, Data_02009820);
    Func_02001cd4(50);
    Func_02001d7e(0x1080000, -1, 0xc80000, 1);   /* 132 << 17, 200 << 16 */
    Func_02001d1e(1, Data_020098e0);
    Func_02001cf4(50);
    Func_02001d2c(2, Data_0200998c);
    Func_02001d3a(2);

    Func_02001d8e(0, 0x4000, 0);
    Func_02001d9a(1, 0x4000, 0);
    Func_02001da6(2, 0x4000, 0);
    Func_02001e0a(3, 0, -32);
    Func_02001d38(30);
    Func_02001dc4(3, 0x4000, 0);
    Func_02001d4a(60);
    Func_02001dd6(3, 0x8000, 0);
    Func_02001d5c(20);
    Func_02001d94(3, Data_02009a4c);
    Func_02001da2(3);
    Func_02001df6(3, 0xc000, 0);
    Func_02001d7c(20);
    Func_02001e26(0xd80000, -1, 0xa80000, 1);    /* 216 << 16, 168 << 16 */
    Func_02001e32();
    Func_02001d98(20);

    Func_02001e24(0, 0x8000, 0);
    Func_02001e30(1, 0x8000, 0);
    Func_02001e3c(2, 0x8000, 0);
    Func_02001e48(3, 0x8000, 0);
    Func_02001dce(40);
    Func_02001e26(0, 3);
    Func_02001e2e(1, 3);
    Func_02001e36(3, 3);
    Func_02001e46(2, 3);
    Func_02001df4(30);

    Func_02001ee2(67);
    Func_02001ef0(240);
    Func_02001cfc();
    Func_02001e0a(80);

    /*
     * Slot 8's record is retuned, and a 16.16 field 284 bytes into the record
     * block (142 << 1) is stepped from -12288.0/65536 to -8192.0/65536 between
     * two 60-frame moves.  The record-block pointer is read once, before the
     * accessor call, and both stores go through it.
     */
    swing = (s32 *)(RECORDS + 284);   /* read before the accessor call */
    record = Func_02001e34(8);
    *(s32 *)(record + 52) = 131;
    *(s32 *)(record + 48) = 0x20000;        /* 128 << 10 */
    *swing = (s32)0xffffd000;
    Func_02001f0a(8, 60, 0);
    *swing = (s32)0xffffa000;
    Func_02001f18(8, 60, 0);

    Func_02001e46(80);
    Func_02001e4c(100);
    Func_02001ee4(0x23333, 0x28f);
    Func_02001efe(0x3280000, -1, 0xa80000, 1);   /* 202 << 18, 168 << 16 */
    Func_02001e6e(300);                          /* 150 << 1 */

    /*
     * Tear-down.  The 448 displacement is formed first and only then reduced
     * to the stored value 32 — see the header note.
     */
    workspace = WORKSPACE;
    *(s32 *)(workspace + 448) = 32;
    *(volatile u16 *)0x05000000 = 0;
    *(s32 *)(workspace + 456) = 96;

    Func_02001f48();
    Func_02001f54();
    Func_02001e9a(30);
    Func_02001e9a(282);                          /* 141 << 1 */
    Func_02001f52(0, 9);
}
