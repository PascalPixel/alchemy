/*
 * resource_379 @ 0x02000074, 2,628 bytes (0x02000074-0x02000ab7) — the whole
 * of this overlay's cutscene, and everything in the overlay outside the veneer
 * table at 0x02000ab8 and the animation data behind it.
 *
 * ENTRY NAME.  The only caller is the byte-exact
 * `assets/code/resource_379_c_02000054.c`, which runs this when
 * Data_02000240[225] == 15 and spells the callee `Func_020000dc`.  That name
 * is an artefact of the printed `bl` decoding: the halfwords at 0x02000066 are
 * `f000 f839`, storing 0x072, and an overlay `bl` stores target_offset - 2, so
 * the real target is 0x02000074 — this owner's prologue.  The inventory's
 * `resource_379:00dc` row is the same artefact: 0x020000dc is an
 * `ldr r1,[pc,#944]` in the middle of this body, not an entry.
 *
 * POOL MAP, from a control-flow walk starting at the prologue.  There is no
 * jump table and no computed branch; the walk reaches 2,524 bytes and leaves
 * three gaps:
 *
 *   0x02000482..0x02000484    2   alignment halfword
 *   0x02000484..0x020004a8   36   literal pool (9 words)
 *   0x020007d4..0x020007f0   28   literal pool (7 words)
 *   0x02000a92..0x02000a94    2   alignment halfword
 *   0x02000a94..0x02000ab8   36   literal pool (9 words)
 *
 * 104 pool bytes.  Every word in each gap is the target of an
 * `ldr rN,[pc,#k]` the walk reached, which is the second, independent witness.
 * Both of the first two gaps end exactly at a branch target — 0x020004a8 and
 * 0x020007f0 are the `bne.n` destinations — but "run to the next branch
 * target" would have been wrong for the third, which is followed by the veneer
 * table rather than by code.
 *
 * Three pool words decode as perfectly plausible instructions and are not:
 * 0x0000e666 at 0x020004a0 and 0x02000aac disassemble as `b.n 0x02000170` and
 * `b.n 0x0200077c`, and 0x0000cccc as `ldmia r4!,{r2,r3,r6,r7}`.  All three
 * are arguments — 0xe666 is the third argument of Func_080091f0 at two sites
 * and 0xcccc a coordinate pair fed to Func_0808a090.
 *
 * LINK BASE 0x02008000: the pool words 0x02008c00, 0x02008c64, 0x02008ea0 and
 * 0x02008cb4 are all even, so under the base they are in-image data at file
 * offsets 0xc00, 0xc64, 0xea0 and 0xcb4 — past the veneer table, which ends at
 * 0xc00, and inside the animation-script band that fills the rest of the
 * 0xeb6-byte image.  They are passed as the script argument of Func_0808a098
 * and Func_08009178, exactly as the parity rule predicts.
 *
 * CALLS: 287 sites, every one of them an import veneer, 41 distinct.  The two
 * `Func_0808a070(0, 0)` results are the only values ever tested; each one
 * chooses between two variants of the following beat.  The variants are NOT
 * merged even where they are behaviourally identical: the second pair differs
 * only in whether the skip-beat counter is bumped before or after the last
 * Func_0808a180 call, which is the documented variant shape, and folding them
 * would deflate the per-target multiset by nine.
 *
 * The skip-beat counter is the standard one: `movs r3,#236 / lsls #1` off the
 * 0x03001ebc state pointer, i.e. the u16 at workspace + 472.
 *
 * The epilogue is `pop {r5} ; pop {r0} ; bx r0` — r0 carries the popped return
 * address, so nothing is returned and the owner is void.  (The byte-exact
 * caller declares it `s32`; the result is discarded there, so the two agree on
 * the generated code.)
 */
#include "types.h"

/* IWRAM pointer cell naming the scene workspace. */
extern u8 *Data_03001ebc;

/* Old-style declarations: this overlay's imports vary their arity by site. */










































extern void Func_02000b70();
extern void Func_02000bd0();
extern void Func_02000bd8();
extern void Func_02000be0();
extern void Func_02000be8();
extern void Func_02000bf0();
extern void Func_02000bf8();
extern void Func_02000bf2();
extern void Func_02000c56();
extern void Func_02000c0c();
extern void Func_02000c70();
extern void Func_02000cc0();
extern void Func_02000cbc();
extern void Func_02000ba2();
extern void Func_02000cb4();
extern void Func_02000bc4();
extern void Func_02000d08();
extern void Func_02000cd8();
extern void Func_02000cf2();
extern void Func_02000c30();
extern void Func_02000c20();
extern void Func_02000d2c();
extern void Func_02000d52();
extern void Func_02000c50();
extern void Func_02000d3c();
extern void Func_02000d62();
extern void Func_02000d2e();
extern void Func_02000c4e();
extern void Func_02000d5a();
extern void Func_02000d80();
extern void Func_02000c66();
extern void Func_02000c72();
extern void Func_02000c90();
extern void Func_02000d26();
extern void Func_02000ca8();
extern void Func_02000d50();
extern void Func_02000d64();
extern void Func_02000cc2();
extern void Func_02000d6a();
extern void Func_02000cd0();
extern void Func_02000d40();
extern void Func_02000d94();
extern void Func_02000d76();
extern void Func_02000cfc();
extern void Func_02000d6c();
extern void Func_02000dac();
extern void Func_02000d8c();
extern void Func_02000dbc();
extern void Func_02000da6();
extern void Func_02000ddc();
extern void Func_02000dc6();
extern void Func_02000d4c();
extern void Func_02000dcc();
extern void Func_02000dfc();
extern void Func_02000de6();
extern void Func_02000e1c();
extern void Func_02000d82();
extern void Func_02000d72();
extern void Func_02000e7e();
extern void Func_02000ea4();
extern void Func_02000da2();
extern void Func_02000e34();
extern void Func_02000e3e();
extern void Func_02000e48();
extern void Func_02000e38();
extern void Func_02000e40();
extern void Func_02000e50();
extern void Func_02000e58();
extern void Func_02000e60();
extern void Func_02000dde();
extern void Func_02000dea();
extern void Func_02000e30();
extern void Func_02000e84();
extern void Func_02000ef8();
extern void Func_02000f12();
extern void Func_02000eb8();
extern void Func_02000ec0();
extern void Func_02000ee0();
extern void Func_02000ee8();
extern void Func_02000f5c();
extern void Func_02000e92();
extern void Func_02000f46();
extern void Func_02000f5a();
extern void Func_02000eb0();
extern void Func_02000ede();
extern void Func_02000f26();
extern void Func_02000f3a();
extern void Func_02000f8a();
extern void Func_02000ef0();
extern void Func_02000f68();
extern void Func_02000f08();
extern void Func_02000fa8();
extern s32 Func_02000f30();
extern void Func_02000f22();
extern void Func_02000f92();
extern void Func_02000f38();
extern void Func_02000fec();
extern void Func_02000fac();
extern void Func_02000f7a();
extern void Func_02000fa6();
extern void Func_02000fce();
extern void Func_02000f9c();
extern void Func_02001004();
extern void Func_0200100c();
extern void Func_02000fb2();
extern void Func_02001066();
extern void Func_02001026();
extern void Func_02001012();
extern void Func_0200103a();
extern void Func_02001082();
extern void Func_020010a4();
extern void Func_0200106c();
extern void Func_02000ffa();
extern void Func_020010ae();
extern void Func_020010ba();
extern void Func_020010c6();
extern void Func_020010d2();
extern void Func_020010de();
extern void Func_02001102();
extern void Func_02001122();
extern void Func_0200111e();
extern void Func_02001054();
extern void Func_020010d4();
extern void Func_02001104();
extern void Func_0200106a();
extern void Func_020010ea();
extern void Func_0200111a();
extern void Func_02001080();
extern void Func_02001100();
extern void Func_02001130();
extern void Func_02001096();
extern void Func_0200114a();
extern void Func_02001156();
extern void Func_02001188();
extern void Func_02001194();
extern void Func_0200113c();
extern void Func_02001144();
extern void Func_0200110a();
extern void Func_020010e0();
extern void Func_02001148();
extern void Func_02001150();
extern void Func_02001126();
extern void Func_020010fc();
extern void Func_020011c8();
extern void Func_020011e2();
extern void Func_02001166();
extern void Func_02001174();
extern void Func_02001182();
extern void Func_02001204();
extern void Func_0200120e();
extern void Func_0200123a();
extern void Func_0200121e();
extern void Func_020011a0();
extern void Func_020011e8();
extern void Func_020011d2();
extern void Func_020011dc();
extern void Func_020011aa();
extern void Func_0200122a();
extern void Func_0200125a();
extern void Func_02001276();
extern void Func_020011cc();
extern void Func_02001288();
extern void Func_020011de();
extern void Func_0200125e();
extern void Func_0200128e();
extern void Func_020011f4();
extern void Func_02001286();
extern void Func_020012b6();
extern void Func_020012c0();
extern void Func_02001296();
extern void Func_02001234();
extern void Func_020012f0();
extern void Func_02001246();
extern void Func_020012fa();
extern void Func_02001258();
extern void Func_020012f8();
extern s32 Func_02001280();
extern void Func_02001272();
extern void Func_02001304();
extern void Func_02001282();
extern void Func_02001336();
extern void Func_02001342();
extern void Func_020012a0();
extern void Func_02001320();
extern void Func_02001350();
extern void Func_020012e4();
extern void Func_02001376();
extern void Func_020012f4();
extern void Func_020013a8();
extern void Func_020013b4();
extern void Func_02001312();
extern void Func_02001392();
extern void Func_020013d2();
extern void Func_02001338();
extern void Func_020013ea();
extern void Func_02001348();
extern void Func_020013c0();
extern void Func_020013d0();
extern void Func_0200135e();
extern void Func_020013c6();
extern void Func_020013d6();
extern void Func_02001374();
extern void Func_020013f4();
extern void Func_02001424();
extern void Func_0200140e();
extern void Func_02001394();
extern void Func_02001414();
extern void Func_020013a2();
extern void Func_02001412();
extern void Func_020013b0();
extern void Func_02001458();
extern void Func_020013be();
extern void Func_0200143e();
extern void Func_0200147a();
extern void Func_020013d8();
extern void Func_02001448();
extern void Func_020013e6();
extern void Func_0200149a();
extern void Func_020013f8();
extern void Func_02001478();
extern void Func_020014a8();
extern void Func_020014a0();
extern void Func_0200141e();
extern void Func_02001486();
extern void Func_02001496();
extern void Func_02001434();
extern void Func_0200148e();
extern void Func_02001444();
extern void Func_02001426();
extern void Func_0200155c();
extern void Func_0200145a();
extern void Func_020014ae();
extern void Func_0200146c();
extern void Func_020014d4();
extern void Func_020014e8();
extern void Func_0200154c();
extern void Func_020014aa();
extern void Func_0200151a();
extern void Func_020014b8();
extern void Func_02001530();
extern void Func_02001540();
extern void Func_020014ce();
extern void Func_02001522();
extern void Func_0200150c();
extern void Func_020014ea();
extern void Func_02001528();
extern void Func_020015b4();
extern void Func_020015c0();
extern void Func_02001516();
extern void Func_02001506();
extern void Func_02001612();
extern void Func_02001638();
extern void Func_02001536();
extern void Func_02001652();
extern void Func_02001562();
extern void Func_02001560();
extern void Func_02001656();
extern void Func_0200158a();
void Func_02000074(void)
{
    u8 *state;

    Func_02000b70();   /* 76 */
    Func_02000bd0(14, 0);   /* 7e */
    Func_02000bd8(15, 0);   /* 86 */
    Func_02000be0(16, 0);   /* 8e */
    Func_02000be8(17, 0);   /* 96 */
    Func_02000bf0(18, 0);   /* 9e */
    Func_02000bf8(19, 0);   /* a6 */
    Func_02000bf2(11, 0x109, 0x1e7);   /* b0 */
    Func_02000c56(11, 0xa000, 0);   /* bc */
    Func_02000c0c(12, 0x100, 0x1f4);   /* ca */
    Func_02000c70(12, 0xa000, 0);   /* d6 */
    Func_02000cc0(0x10003, 0x10006);   /* de */
    Func_02000cbc();   /* e2 */
    Func_02000ba2(60);   /* e8 */
    Func_02000cb4(0x1000000, -1, 0x2640000, 0);   /* fa */
    Func_02000cc0();   /* fe */
    Func_02000bc4();   /* 102 */
    state = Data_03001ebc;
    *(s32 *)(state + 448) = 0;
    *(s32 *)(state + 456) = 32;
    Func_02000d08();   /* 11e */
    Func_02000cd8(0xcccc, 0x1999);   /* 126 */
    Func_02000cf2(0x1000000, -1, 0x1f40000, 1);   /* 138 */
    Func_02000c30(20);   /* 13e */
    Func_02000c20(0x10000, 0x20000, 0x10000);   /* 14e */
    Func_02000d2c();   /* 152 */
    Func_02000d52(145);   /* 158 */
    Func_02000c50(30);   /* 15e */
    Func_02000d3c();   /* 162 */
    Func_02000d62(145);   /* 168 */
    Func_02000d2e();   /* 16c */
    Func_02000c4e(0x20000, 0x30000, 0x10000);   /* 17c */
    Func_02000d5a();   /* 180 */
    Func_02000d80(145);   /* 186 */
    Func_02000c66(-1, -1, 0xe666);   /* 194 */
    Func_02000c72();   /* 198 */
    Func_02000c90(60);   /* 19e */
    Func_02000d26(0x1122);   /* 1a4 */
    Func_02000d52(8, 0x102, 0);   /* 1b0 */
    Func_02000ca8(60);   /* 1b6 */
    Func_02000d50(8, 0);   /* 1be */
    Func_02000d64(9, 0x5000, 0);   /* 1ca */
    Func_02000cc2(30);   /* 1d0 */
    Func_02000d6a(9, 0);   /* 1d8 */
    Func_02000cd0(30);   /* 1de */
    Func_02000d40(11, 4);   /* 1e6 */
    Func_02000d80(11, 0);   /* 1ee */
    Func_02000d94(9, 0x3000, 0);   /* 1fa */
    Func_02000d76(12, 11, 0);   /* 204 */
    Func_02000cfc(30);   /* 20a */
    Func_02000d6c(12, 4);   /* 212 */
    Func_02000dac(12, 0);   /* 21a */
    Func_02000d8c(13, 1);   /* 222 */
    Func_02000dbc(13, 0);   /* 22a */
    Func_02000da6(10, 13, 0);   /* 234 */
    Func_02000d2c(30);   /* 23a */
    Func_02000dac(10, 1);   /* 242 */
    Func_02000ddc(10, 0);   /* 24a */
    Func_02000dc6(9, 10, 0);   /* 254 */
    Func_02000d4c(30);   /* 25a */
    Func_02000dcc(9, 1);   /* 262 */
    Func_02000dfc(9, 0);   /* 26a */
    Func_02000de6(10, 9, 0);   /* 274 */
    Func_02000d6c(30);   /* 27a */
    Func_02000ddc(10, 4);   /* 282 */
    Func_02000e1c(10, 0);   /* 28a */
    Func_02000d82(60);   /* 290 */
    Func_02000d72(0x20000, 0x30000, 0x10000);   /* 2a0 */
    Func_02000e7e();   /* 2a4 */
    Func_02000ea4(145);   /* 2aa */
    Func_02000da2(60);   /* 2b0 */
    Func_02000e34(8, 9, 0);   /* 2ba */
    Func_02000e3e(10, 11, 0);   /* 2c4 */
    Func_02000e48(12, 13, 0);   /* 2ce */
    Func_02000e38(8, 2);   /* 2d6 */
    Func_02000e40(9, 2);   /* 2de */
    Func_02000e48(10, 2);   /* 2e6 */
    Func_02000e50(11, 2);   /* 2ee */
    Func_02000e58(12, 2);   /* 2f6 */
    Func_02000e60(13, 2);   /* 2fe */
    Func_02000dde(-1, -1, 0xe666);   /* 30c */
    Func_02000dea();   /* 310 */
    Func_02000e30(0, 0x8000, 0x4000);   /* 31e */
    Func_02000e3e(1, 0x8000, 0x4000);   /* 32c */
    Func_02000e84(0, 0x780000, 0x1020000);   /* 33a */
    Func_02000ef8(0x18000, 0x3000);   /* 346 */
    Func_02000f12(0x700000, -1, 0x1400000, 1);   /* 358 */
    Func_02000e50(40);   /* 35e */
    Func_02000eb8(0, 2);   /* 366 */
    Func_02000ec0(1, 2);   /* 36e */
    Func_02000ea4(0, 120, 0x140);   /* 37a */
    Func_02000eb8(1, 104, 0x140);   /* 386 */
    Func_02000ee0(0, 1);   /* 38e */
    Func_02000ee8(1, 1);   /* 396 */
    Func_02000f5c();   /* 39a */
    Func_02000e92(30);   /* 3a0 */
    Func_02000f46(1, 0x3000, 0);   /* 3ac */
    Func_02000f5a(1, 0x100, 0);   /* 3b8 */
    Func_02000eb0(50);   /* 3be */
    Func_02000ede(1, 0x18000, 0xc000);   /* 3cc */
    Func_02000f26(1, 2);   /* 3d4 */
    Func_02000f12(1, 105, 0x156);   /* 3e0 */
    Func_02000f3a(1, 1);   /* 3e8 */
    Func_02000f5a(1, 2);   /* 3f0 */
    Func_02000f8a(1, 0);   /* 3f8 */
    Func_02000ef0(10);   /* 3fe */
    Func_02000f68(0, 1);   /* 406 */
    Func_02000f8a(0, 1, 0);   /* 410 */
    Func_02000f08(20);   /* 416 */
    Func_02000fa8(1, 0);   /* 41e */
    if (Func_02000f30(0, 0) != 0) goto pose_variant_b;
    Func_02000f22(60);   /* 430 */
    Func_02000f8a(0, 3);   /* 438 */
    Func_02000f92(1, 3);   /* 440 */
    Func_02000f38(50);   /* 446 */
    Func_02000fec(0, 0xc000, 0);   /* 452 */
    Func_02000fac(1, 2);   /* 45a */
    Func_02000f7a(1, 0x10000, 0x8000);   /* 468 */
    Func_02000fa6(1, 103, 0x140);   /* 474 */
    Func_02000fce(1, 1);   /* 47c */
    goto pose_join;
pose_variant_b:                                 /* 0x020004a8 */
    Func_02000f9c(60);   /* 4aa */
    Func_02001004(0, 3);   /* 4b2 */
    Func_0200100c(1, 3);   /* 4ba */
    Func_02000fb2(50);   /* 4c0 */
    Func_02001066(1, 0x3000, 0);   /* 4cc */
    Func_02001026(0, 2);   /* 4d4 */
    Func_02001012(0, 120, 0x154);   /* 4e0 */
    Func_0200103a(0, 1);   /* 4e8 */
pose_join:                                      /* 0x020004ec */
    Func_02001082(12, 0);   /* 4f0 */
    Func_020010a4(1, 0x102);   /* 4fa */
    Func_0200106c(1, 2);   /* 502 */
    Func_02000ffa(40);   /* 508 */
    Func_020010ae(9, 0xa000, 0);   /* 514 */
    Func_020010ba(11, 0xa000, 0);   /* 520 */
    Func_020010c6(10, 0xa000, 0);   /* 52c */
    Func_020010d2(12, 0xa000, 0);   /* 538 */
    Func_020010de(13, 0xa000, 0);   /* 544 */
    Func_02001102(0x30000, 0x6000);   /* 550 */
    Func_02001122(10, 1);   /* 558 */
    Func_0200111e();   /* 55c */
    Func_02001054(50);   /* 562 */
    Func_020010d4(10, 2);   /* 56a */
    Func_02001104(10, 0);   /* 572 */
    Func_0200106a(30);   /* 578 */
    Func_020010ea(8, 1);   /* 580 */
    Func_0200111a(8, 0);   /* 588 */
    Func_02001080(40);   /* 58e */
    Func_02001100(9, 1);   /* 596 */
    Func_02001130(9, 0);   /* 59e */
    Func_02001096(40);   /* 5a4 */
    Func_0200114a(0, 0x3000, 0);   /* 5b0 */
    Func_02001156(1, 0x3000, 0);   /* 5bc */
    Func_02001188(0x700000, -1, 0x1400000, 1);   /* 5ce */
    Func_02001194();   /* 5d2 */
    Func_0200113c(0, 2);   /* 5da */
    Func_02001144(1, 2);   /* 5e2 */
    Func_0200110a(1);   /* 5e8 */
    Func_020010e0(50);   /* 5ee */
    Func_02001148(0, 3);   /* 5f6 */
    Func_02001150(1, 3);   /* 5fe */
    Func_02001126(1);   /* 604 */
    Func_020010fc(60);   /* 60a */
    Func_020011c8(0x10000, 0x2000);   /* 616 */
    Func_020011e2(0xd60000, -1, 0x1d80000, 1);   /* 628 */
    Func_02001148(0, 0x10000, 0x8000);   /* 636 */
    Func_02001156(1, 0x10000, 0x8000);   /* 644 */
    Func_02001166(0, 0x2008c00);   /* 64c */
    Func_02001144(30);   /* 652 */
    Func_02001174(1, 0x2008c64);   /* 65a */
    Func_02001182(1);   /* 660 */
    Func_02001204(0, 0, 0);   /* 66a */
    Func_0200120e(1, 0, 0);   /* 674 */
    Func_0200123a();   /* 678 */
    Func_0200121e(9, 0x8000, 0);   /* 684 */
    Func_020011a0(8, 0xcccc, 0x6666);   /* 68e */
    Func_020011e8(8, 2);   /* 696 */
    Func_020011d2(8, 0x109, 0x1c7);   /* 6a0 */
    Func_020011dc(8, 246, 0x1c7);   /* 6aa */
    Func_02001204(8, 1);   /* 6b2 */
    Func_020011aa(30);   /* 6b8 */
    Func_0200122a(9, 1);   /* 6c0 */
    Func_0200125a(9, 0);   /* 6c8 */
    Func_02001276(0, 0x102, 0);   /* 6d4 */
    Func_020011cc(50);   /* 6da */
    Func_02001288(1, 0x102, 0);   /* 6e6 */
    Func_020011de(50);   /* 6ec */
    Func_0200125e(8, 1);   /* 6f4 */
    Func_0200128e(8, 0);   /* 6fc */
    Func_020011f4(40);   /* 702 */
    Func_02001286(0, 1, 0);   /* 70c */
    Func_02001204(50);   /* 712 */
    Func_020012b6(0, 0, 0);   /* 71c */
    Func_020012c0(1, 0, 0);   /* 726 */
    Func_0200121e(20);   /* 72c */
    Func_02001286(0, 4);   /* 734 */
    Func_02001296(1, 4);   /* 73c */
    Func_02001234(40);   /* 742 */
    Func_020012f0(10, 0x102, 0);   /* 74e */
    Func_02001246(50);   /* 754 */
    Func_020012fa(1, 0x3000, 0);   /* 760 */
    Func_02001258(10);   /* 766 */
    Func_020012f8(10, 0);   /* 76e */
    if (Func_02001280(0, 0) != 0) goto beat_variant_b;
    Func_02001272(40);   /* 780 */
    Func_02001304(8, 9, 0);   /* 78a */
    Func_02001282(50);   /* 790 */
    Func_02001336(8, 0x8000, 0);   /* 79c */
    Func_02001342(9, 0x8000, 0);   /* 7a8 */
    Func_020012a0(40);   /* 7ae */
    Func_02001320(9, 1);   /* 7b6 */
    Func_02001350(9, 0);   /* 7be */
    state = Data_03001ebc;
    *(u16 *)(state + 472) += 1;
    goto beat_join;
beat_variant_b:                                 /* 0x020007f0 */
    Func_020012e4(40);   /* 7f2 */
    Func_02001376(8, 9, 0);   /* 7fc */
    Func_020012f4(50);   /* 802 */
    Func_020013a8(8, 0x8000, 0);   /* 80e */
    Func_020013b4(9, 0x8000, 0);   /* 81a */
    Func_02001312(40);   /* 820 */
    Func_02001392(9, 1);   /* 828 */
    state = Data_03001ebc;
    *(u16 *)(state + 472) += 1;
    Func_020013d2(9, 0);   /* 840 */
beat_join:                                      /* 0x02000844 */
    Func_02001338(30);   /* 846 */
    Func_020013ea(1, 0, 0);   /* 850 */
    Func_02001348(30);   /* 856 */
    Func_020013c0(0, 2);   /* 85e */
    Func_020013d0(1, 2);   /* 866 */
    Func_0200135e(40);   /* 86c */
    Func_020013c6(0, 4);   /* 874 */
    Func_020013d6(1, 4);   /* 87c */
    Func_02001374(60);   /* 882 */
    Func_020013f4(8, 1);   /* 88a */
    Func_02001424(8, 0);   /* 892 */
    Func_0200140e(8, 9, 0);   /* 89c */
    Func_02001394(30);   /* 8a2 */
    Func_02001414(8, 1);   /* 8aa */
    Func_020013a2(30);   /* 8b0 */
    Func_02001412(8, 3);   /* 8b8 */
    Func_020013b0(30);   /* 8be */
    Func_02001458(8, 0);   /* 8c6 */
    Func_020013be(20);   /* 8cc */
    Func_0200143e(9, 1);   /* 8d4 */
    Func_0200147a(9, 0xb000, 0);   /* 8e0 */
    Func_020013d8(30);   /* 8e6 */
    Func_02001448(9, 3);   /* 8ee */
    Func_020013e6(50);   /* 8f4 */
    Func_0200149a(8, 0x8000, 0);   /* 900 */
    Func_020013f8(20);   /* 906 */
    Func_02001478(8, 1);   /* 90e */
    Func_020014a8(8, 0);   /* 916 */
    Func_0200140e(40);   /* 91c */
    Func_020014a0(8, 9, 0);   /* 926 */
    Func_0200141e(40);   /* 92c */
    Func_02001486(8, 3);   /* 934 */
    Func_02001496(9, 3);   /* 93c */
    Func_02001434(30);   /* 942 */
    Func_0200148e(8, 255, 0x1bd);   /* 94c */
    Func_02001444(40);   /* 952 */
    Func_02001426(0x2008ea0, 45, 11);   /* 95c */
    Func_0200155c(188);   /* 962 */
    Func_0200145a(30);   /* 968 */
    Func_020014ae(8, 255, 0x186);   /* 974 */
    Func_0200146c(20);   /* 97a */
    Func_02001496(9, 0xcccc, 0x6666);   /* 984 */
    Func_020014a0(10, 0xcccc, 0x6666);   /* 98e */
    Func_020014d4(9, 255, 0x186);   /* 99a */
    Func_020014e8(10, 255, 0x1cc);   /* 9a6 */
    Func_0200154c(10, 0x8000, 0);   /* 9b2 */
    Func_020014aa(40);   /* 9b8 */
    Func_0200151a(10, 3);   /* 9c0 */
    Func_020014b8(30);   /* 9c6 */
    Func_02001530(0, 1);   /* 9ce */
    Func_02001540(1, 1);   /* 9d6 */
    Func_020014ce(40);   /* 9dc */
    Func_02001522(10, 255, 0x186);   /* 9e8 */
    Func_0200150c(0, 0x2008cb4);   /* 9f2 */
    Func_020014ea(40);   /* 9f8 */
    Func_0200151a(1, 0x2008cb4);   /* a00 */
    Func_02001528(1);   /* a06 */
    Func_020015b4(11, 0x102, 0);   /* a12 */
    Func_020015c0(12, 0x102, 0);   /* a1e */
    Func_02001516(40);   /* a24 */
    Func_02001506(0x20000, 0x30000, 0x10000);   /* a34 */
    Func_02001612();   /* a38 */
    Func_02001638(145);   /* a3e */
    Func_02001536(30);   /* a44 */
    state = Data_03001ebc;
    *(s32 *)(state + 448) = 0;
    *(s32 *)(state + 456) = 64;
    Func_02001652();   /* a60 */
    Func_02001540(-1, -1, 0xe666);   /* a6e */
    Func_0200154c();   /* a72 */
    Func_02001562(0x12f);   /* a78 */
    Func_02001560(0x879);   /* a7e */
    Func_02001656(1);   /* a84 */
    Func_0200158a();   /* a88 */
}

