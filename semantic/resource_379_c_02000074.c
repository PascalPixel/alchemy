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
typedef signed int s32;
typedef unsigned char u8;
typedef unsigned short u16;

/* IWRAM pointer cell naming the scene workspace. */
extern u8 *Data_03001ebc;

/* Old-style declarations: this overlay's imports vary their arity by site. */
void Func_080000c0();
void Func_08009128();
void Func_08009178();
void Func_080091f0();
void Func_080091f8();
void Func_080770c8();
void Func_080770d0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0a0();
void Func_0808a0b8();
void Func_0808a0c0();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a130();
void Func_0808a138();
void Func_0808a148();
void Func_0808a150();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a220();
void Func_0808a248();
void Func_0808a2d8();
void Func_0808a2e8();
void Func_0808a360();
void Func_0808a368();
void Func_080f9010();

void Func_02000074(void)
{
    u8 *state;

    Func_0808a018();   /* 76 */
    Func_0808a100(14, 0);   /* 7e */
    Func_0808a100(15, 0);   /* 86 */
    Func_0808a100(16, 0);   /* 8e */
    Func_0808a100(17, 0);   /* 96 */
    Func_0808a100(18, 0);   /* 9e */
    Func_0808a100(19, 0);   /* a6 */
    Func_0808a0d0(11, 0x109, 0x1e7);   /* b0 */
    Func_0808a1b8(11, 0xa000, 0);   /* bc */
    Func_0808a0d0(12, 0x100, 0x1f4);   /* ca */
    Func_0808a1b8(12, 0xa000, 0);   /* d6 */
    Func_0808a2e8(0x10003, 0x10006);   /* de */
    Func_0808a2d8();   /* e2 */
    Func_080000c0(60);   /* e8 */
    Func_0808a210(0x1000000, -1, 0x2640000, 0);   /* fa */
    Func_0808a218();   /* fe */
    Func_08009128();   /* 102 */
    state = Data_03001ebc;
    *(s32 *)(state + 448) = 0;
    *(s32 *)(state + 456) = 32;
    Func_0808a360();   /* 11e */
    Func_0808a208(0xcccc, 0x1999);   /* 126 */
    Func_0808a210(0x1000000, -1, 0x1f40000, 1);   /* 138 */
    Func_0808a010(20);   /* 13e */
    Func_080091f0(0x10000, 0x20000, 0x10000);   /* 14e */
    Func_0808a2d8();   /* 152 */
    Func_080f9010(145);   /* 158 */
    Func_0808a010(30);   /* 15e */
    Func_0808a2d8();   /* 162 */
    Func_080f9010(145);   /* 168 */
    Func_0808a218();   /* 16c */
    Func_080091f0(0x20000, 0x30000, 0x10000);   /* 17c */
    Func_0808a2d8();   /* 180 */
    Func_080f9010(145);   /* 186 */
    Func_080091f0(-1, -1, 0xe666);   /* 194 */
    Func_080091f8();   /* 198 */
    Func_0808a010(60);   /* 19e */
    Func_0808a170(0x1122);   /* 1a4 */
    Func_0808a1e8(8, 0x102, 0);   /* 1b0 */
    Func_0808a010(60);   /* 1b6 */
    Func_0808a180(8, 0);   /* 1be */
    Func_0808a1b8(9, 0x5000, 0);   /* 1ca */
    Func_0808a010(30);   /* 1d0 */
    Func_0808a180(9, 0);   /* 1d8 */
    Func_0808a010(30);   /* 1de */
    Func_0808a110(11, 4);   /* 1e6 */
    Func_0808a180(11, 0);   /* 1ee */
    Func_0808a1b8(9, 0x3000, 0);   /* 1fa */
    Func_0808a148(12, 11, 0);   /* 204 */
    Func_0808a010(30);   /* 20a */
    Func_0808a110(12, 4);   /* 212 */
    Func_0808a180(12, 0);   /* 21a */
    Func_0808a138(13, 1);   /* 222 */
    Func_0808a180(13, 0);   /* 22a */
    Func_0808a148(10, 13, 0);   /* 234 */
    Func_0808a010(30);   /* 23a */
    Func_0808a138(10, 1);   /* 242 */
    Func_0808a180(10, 0);   /* 24a */
    Func_0808a148(9, 10, 0);   /* 254 */
    Func_0808a010(30);   /* 25a */
    Func_0808a138(9, 1);   /* 262 */
    Func_0808a180(9, 0);   /* 26a */
    Func_0808a148(10, 9, 0);   /* 274 */
    Func_0808a010(30);   /* 27a */
    Func_0808a110(10, 4);   /* 282 */
    Func_0808a180(10, 0);   /* 28a */
    Func_0808a010(60);   /* 290 */
    Func_080091f0(0x20000, 0x30000, 0x10000);   /* 2a0 */
    Func_0808a2d8();   /* 2a4 */
    Func_080f9010(145);   /* 2aa */
    Func_0808a010(60);   /* 2b0 */
    Func_0808a150(8, 9, 0);   /* 2ba */
    Func_0808a150(10, 11, 0);   /* 2c4 */
    Func_0808a150(12, 13, 0);   /* 2ce */
    Func_0808a130(8, 2);   /* 2d6 */
    Func_0808a130(9, 2);   /* 2de */
    Func_0808a130(10, 2);   /* 2e6 */
    Func_0808a130(11, 2);   /* 2ee */
    Func_0808a130(12, 2);   /* 2f6 */
    Func_0808a130(13, 2);   /* 2fe */
    Func_080091f0(-1, -1, 0xe666);   /* 30c */
    Func_080091f8();   /* 310 */
    Func_0808a090(0, 0x8000, 0x4000);   /* 31e */
    Func_0808a090(1, 0x8000, 0x4000);   /* 32c */
    Func_0808a0f0(0, 0x780000, 0x1020000);   /* 33a */
    Func_0808a208(0x18000, 0x3000);   /* 346 */
    Func_0808a210(0x700000, -1, 0x1400000, 1);   /* 358 */
    Func_0808a010(40);   /* 35e */
    Func_0808a100(0, 2);   /* 366 */
    Func_0808a100(1, 2);   /* 36e */
    Func_0808a0b8(0, 120, 0x140);   /* 37a */
    Func_0808a0c0(1, 104, 0x140);   /* 386 */
    Func_0808a100(0, 1);   /* 38e */
    Func_0808a100(1, 1);   /* 396 */
    Func_0808a218();   /* 39a */
    Func_0808a010(30);   /* 3a0 */
    Func_0808a1b8(1, 0x3000, 0);   /* 3ac */
    Func_0808a1e8(1, 0x100, 0);   /* 3b8 */
    Func_0808a010(50);   /* 3be */
    Func_0808a090(1, 0x18000, 0xc000);   /* 3cc */
    Func_0808a100(1, 2);   /* 3d4 */
    Func_0808a0c0(1, 105, 0x156);   /* 3e0 */
    Func_0808a100(1, 1);   /* 3e8 */
    Func_0808a138(1, 2);   /* 3f0 */
    Func_0808a180(1, 0);   /* 3f8 */
    Func_0808a010(10);   /* 3fe */
    Func_0808a130(0, 1);   /* 406 */
    Func_0808a150(0, 1, 0);   /* 410 */
    Func_0808a010(20);   /* 416 */
    Func_0808a178(1, 0);   /* 41e */
    if (Func_0808a070(0, 0) != 0) goto pose_variant_b;
    Func_0808a010(60);   /* 430 */
    Func_0808a100(0, 3);   /* 438 */
    Func_0808a100(1, 3);   /* 440 */
    Func_0808a010(50);   /* 446 */
    Func_0808a1b8(0, 0xc000, 0);   /* 452 */
    Func_0808a100(1, 2);   /* 45a */
    Func_0808a090(1, 0x10000, 0x8000);   /* 468 */
    Func_0808a0c0(1, 103, 0x140);   /* 474 */
    Func_0808a100(1, 1);   /* 47c */
    goto pose_join;
pose_variant_b:                                 /* 0x020004a8 */
    Func_0808a010(60);   /* 4aa */
    Func_0808a100(0, 3);   /* 4b2 */
    Func_0808a100(1, 3);   /* 4ba */
    Func_0808a010(50);   /* 4c0 */
    Func_0808a1b8(1, 0x3000, 0);   /* 4cc */
    Func_0808a100(0, 2);   /* 4d4 */
    Func_0808a0c0(0, 120, 0x154);   /* 4e0 */
    Func_0808a100(0, 1);   /* 4e8 */
pose_join:                                      /* 0x020004ec */
    Func_0808a180(12, 0);   /* 4f0 */
    Func_0808a1f0(1, 0x102);   /* 4fa */
    Func_0808a138(1, 2);   /* 502 */
    Func_0808a010(40);   /* 508 */
    Func_0808a1b8(9, 0xa000, 0);   /* 514 */
    Func_0808a1b8(11, 0xa000, 0);   /* 520 */
    Func_0808a1b8(10, 0xa000, 0);   /* 52c */
    Func_0808a1b8(12, 0xa000, 0);   /* 538 */
    Func_0808a1b8(13, 0xa000, 0);   /* 544 */
    Func_0808a208(0x30000, 0x6000);   /* 550 */
    Func_0808a220(10, 1);   /* 558 */
    Func_0808a218();   /* 55c */
    Func_0808a010(50);   /* 562 */
    Func_0808a138(10, 2);   /* 56a */
    Func_0808a180(10, 0);   /* 572 */
    Func_0808a010(30);   /* 578 */
    Func_0808a138(8, 1);   /* 580 */
    Func_0808a180(8, 0);   /* 588 */
    Func_0808a010(40);   /* 58e */
    Func_0808a138(9, 1);   /* 596 */
    Func_0808a180(9, 0);   /* 59e */
    Func_0808a010(40);   /* 5a4 */
    Func_0808a1b8(0, 0x3000, 0);   /* 5b0 */
    Func_0808a1b8(1, 0x3000, 0);   /* 5bc */
    Func_0808a210(0x700000, -1, 0x1400000, 1);   /* 5ce */
    Func_0808a218();   /* 5d2 */
    Func_0808a130(0, 2);   /* 5da */
    Func_0808a130(1, 2);   /* 5e2 */
    Func_0808a0a0(1);   /* 5e8 */
    Func_0808a010(50);   /* 5ee */
    Func_0808a100(0, 3);   /* 5f6 */
    Func_0808a100(1, 3);   /* 5fe */
    Func_0808a0a0(1);   /* 604 */
    Func_0808a010(60);   /* 60a */
    Func_0808a208(0x10000, 0x2000);   /* 616 */
    Func_0808a210(0xd60000, -1, 0x1d80000, 1);   /* 628 */
    Func_0808a090(0, 0x10000, 0x8000);   /* 636 */
    Func_0808a090(1, 0x10000, 0x8000);   /* 644 */
    Func_0808a098(0, 0x2008c00);   /* 64c */
    Func_0808a010(30);   /* 652 */
    Func_0808a098(1, 0x2008c64);   /* 65a */
    Func_0808a0a0(1);   /* 660 */
    Func_0808a1b8(0, 0, 0);   /* 66a */
    Func_0808a1b8(1, 0, 0);   /* 674 */
    Func_0808a218();   /* 678 */
    Func_0808a1b8(9, 0x8000, 0);   /* 684 */
    Func_0808a090(8, 0xcccc, 0x6666);   /* 68e */
    Func_0808a100(8, 2);   /* 696 */
    Func_0808a0c0(8, 0x109, 0x1c7);   /* 6a0 */
    Func_0808a0c0(8, 246, 0x1c7);   /* 6aa */
    Func_0808a100(8, 1);   /* 6b2 */
    Func_0808a010(30);   /* 6b8 */
    Func_0808a138(9, 1);   /* 6c0 */
    Func_0808a180(9, 0);   /* 6c8 */
    Func_0808a1e8(0, 0x102, 0);   /* 6d4 */
    Func_0808a010(50);   /* 6da */
    Func_0808a1e8(1, 0x102, 0);   /* 6e6 */
    Func_0808a010(50);   /* 6ec */
    Func_0808a138(8, 1);   /* 6f4 */
    Func_0808a180(8, 0);   /* 6fc */
    Func_0808a010(40);   /* 702 */
    Func_0808a150(0, 1, 0);   /* 70c */
    Func_0808a010(50);   /* 712 */
    Func_0808a1b8(0, 0, 0);   /* 71c */
    Func_0808a1b8(1, 0, 0);   /* 726 */
    Func_0808a010(20);   /* 72c */
    Func_0808a100(0, 4);   /* 734 */
    Func_0808a110(1, 4);   /* 73c */
    Func_0808a010(40);   /* 742 */
    Func_0808a1e8(10, 0x102, 0);   /* 74e */
    Func_0808a010(50);   /* 754 */
    Func_0808a1b8(1, 0x3000, 0);   /* 760 */
    Func_0808a010(10);   /* 766 */
    Func_0808a178(10, 0);   /* 76e */
    if (Func_0808a070(0, 0) != 0) goto beat_variant_b;
    Func_0808a010(40);   /* 780 */
    Func_0808a150(8, 9, 0);   /* 78a */
    Func_0808a010(50);   /* 790 */
    Func_0808a1b8(8, 0x8000, 0);   /* 79c */
    Func_0808a1b8(9, 0x8000, 0);   /* 7a8 */
    Func_0808a010(40);   /* 7ae */
    Func_0808a138(9, 1);   /* 7b6 */
    Func_0808a180(9, 0);   /* 7be */
    state = Data_03001ebc;
    *(u16 *)(state + 472) += 1;
    goto beat_join;
beat_variant_b:                                 /* 0x020007f0 */
    Func_0808a010(40);   /* 7f2 */
    Func_0808a150(8, 9, 0);   /* 7fc */
    Func_0808a010(50);   /* 802 */
    Func_0808a1b8(8, 0x8000, 0);   /* 80e */
    Func_0808a1b8(9, 0x8000, 0);   /* 81a */
    Func_0808a010(40);   /* 820 */
    Func_0808a138(9, 1);   /* 828 */
    state = Data_03001ebc;
    *(u16 *)(state + 472) += 1;
    Func_0808a180(9, 0);   /* 840 */
beat_join:                                      /* 0x02000844 */
    Func_0808a010(30);   /* 846 */
    Func_0808a1b8(1, 0, 0);   /* 850 */
    Func_0808a010(30);   /* 856 */
    Func_0808a130(0, 2);   /* 85e */
    Func_0808a138(1, 2);   /* 866 */
    Func_0808a010(40);   /* 86c */
    Func_0808a100(0, 4);   /* 874 */
    Func_0808a110(1, 4);   /* 87c */
    Func_0808a010(60);   /* 882 */
    Func_0808a138(8, 1);   /* 88a */
    Func_0808a180(8, 0);   /* 892 */
    Func_0808a148(8, 9, 0);   /* 89c */
    Func_0808a010(30);   /* 8a2 */
    Func_0808a138(8, 1);   /* 8aa */
    Func_0808a010(30);   /* 8b0 */
    Func_0808a110(8, 3);   /* 8b8 */
    Func_0808a010(30);   /* 8be */
    Func_0808a180(8, 0);   /* 8c6 */
    Func_0808a010(20);   /* 8cc */
    Func_0808a138(9, 1);   /* 8d4 */
    Func_0808a1b8(9, 0xb000, 0);   /* 8e0 */
    Func_0808a010(30);   /* 8e6 */
    Func_0808a110(9, 3);   /* 8ee */
    Func_0808a010(50);   /* 8f4 */
    Func_0808a1b8(8, 0x8000, 0);   /* 900 */
    Func_0808a010(20);   /* 906 */
    Func_0808a138(8, 1);   /* 90e */
    Func_0808a180(8, 0);   /* 916 */
    Func_0808a010(40);   /* 91c */
    Func_0808a150(8, 9, 0);   /* 926 */
    Func_0808a010(40);   /* 92c */
    Func_0808a100(8, 3);   /* 934 */
    Func_0808a110(9, 3);   /* 93c */
    Func_0808a010(30);   /* 942 */
    Func_0808a0d0(8, 255, 0x1bd);   /* 94c */
    Func_0808a010(40);   /* 952 */
    Func_08009178(0x2008ea0, 45, 11);   /* 95c */
    Func_080f9010(188);   /* 962 */
    Func_0808a010(30);   /* 968 */
    Func_0808a0c8(8, 255, 0x186);   /* 974 */
    Func_0808a010(20);   /* 97a */
    Func_0808a090(9, 0xcccc, 0x6666);   /* 984 */
    Func_0808a090(10, 0xcccc, 0x6666);   /* 98e */
    Func_0808a0c8(9, 255, 0x186);   /* 99a */
    Func_0808a0d0(10, 255, 0x1cc);   /* 9a6 */
    Func_0808a1b8(10, 0x8000, 0);   /* 9b2 */
    Func_0808a010(40);   /* 9b8 */
    Func_0808a110(10, 3);   /* 9c0 */
    Func_0808a010(30);   /* 9c6 */
    Func_0808a130(0, 1);   /* 9ce */
    Func_0808a138(1, 1);   /* 9d6 */
    Func_0808a010(40);   /* 9dc */
    Func_0808a0c8(10, 255, 0x186);   /* 9e8 */
    Func_0808a098(0, 0x2008cb4);   /* 9f2 */
    Func_0808a010(40);   /* 9f8 */
    Func_0808a098(1, 0x2008cb4);   /* a00 */
    Func_0808a0a0(1);   /* a06 */
    Func_0808a1e8(11, 0x102, 0);   /* a12 */
    Func_0808a1e8(12, 0x102, 0);   /* a1e */
    Func_0808a010(40);   /* a24 */
    Func_080091f0(0x20000, 0x30000, 0x10000);   /* a34 */
    Func_0808a2d8();   /* a38 */
    Func_080f9010(145);   /* a3e */
    Func_0808a010(30);   /* a44 */
    state = Data_03001ebc;
    *(s32 *)(state + 448) = 0;
    *(s32 *)(state + 456) = 64;
    Func_0808a368();   /* a60 */
    Func_080091f0(-1, -1, 0xe666);   /* a6e */
    Func_080091f8();   /* a72 */
    Func_080770d0(0x12f);   /* a78 */
    Func_080770c8(0x879);   /* a7e */
    Func_0808a248(1);   /* a84 */
    Func_0808a020();   /* a88 */
}

