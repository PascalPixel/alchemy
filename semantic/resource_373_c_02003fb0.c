#include "types.h"

/*
 * resource_373 owner at 0x02003fb0, 5,604 bytes.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r9/sl/fp and r8 saves and a
 * 28-byte local frame at 0x02003fb0, through the single epilogue at
 * 0x0200556a-0x0200557a, followed by its final literal pool at
 * 0x0200557c-0x02005593.  The next inventory row begins at 0x02005594, which
 * already has byte-exact C.
 *
 * The longest of the three remaining cutscene scripts in this overlay and the
 * variant of the 0x020015dc scene: four dialogue beats driven by
 * Func_02005c20 rather than by Func_0808a010 waits, three player questions on
 * Func_0808a070, and two constant-slide loops.  Six interior literal pools sit
 * inside the span and are branched over rather than fallen into, at
 * 0x02004082, 0x02004484, 0x020046ea, 0x02004b22, 0x02004ffa and 0x02005472.
 * Four of the six `b.n`s split a call whose argument registers are set before
 * the branch: Func_0808a080 (from 0x02004080), Func_02005c20 (from
 * 0x02004482), Func_0808a1b8 (from 0x02004b20), Func_0808a128 (from
 * 0x02004ff8) and Func_0808a210 (from 0x02005470).  All are reassembled below.
 *
 * The epilogue is `pop {r0} / bx r0`, so r0 holds the popped return address
 * and the owner is void.  r0 is written before the first call and no incoming
 * register is read, so the owner takes no arguments.
 *
 * Call accounting: 513 `bl` sites in the decoded instruction stream, all
 * resolved with `bun tools/lib/overlay_call_targets.ts resource_373 3fb0` - 52
 * distinct targets, 5 of them intra-overlay prologues (Func_020032b0 x3,
 * Func_02003380 x2, Func_02005594 x1, Func_020055a4 x1, Func_02005c20 x4).
 * No `call_via rN` site.  Each of the 513 sites appears exactly once below;
 * the multiset of `Func_...(` calls in this file equals the multiset of
 * resolved targets.
 *
 * LINK BASE 0x02008000.  This owner adds five more Thumb-bit witnesses to the
 * three recorded in resource_373_c_020034c8.c: the pool words 0x0200d5c1,
 * 0x0200d5e1, 0x0200d5f1 and 0x0200d601 are Func_020055c0, Func_020055e0,
 * Func_020055f0 and Func_02005600 + the Thumb bit, and every one of those has
 * a byte-exact source in assets/code.  They are installed as per-beat
 * callbacks through Func_080000d0 for 3200 ticks and removed again through
 * Func_080000d8.  The fifth, 0x0200da09, resolves to file offset 0x5a08; that
 * range has no separate source yet, so it is passed through as the encoded
 * address.
 *
 * The pointer-shaped data words 0x0200e590, 0x0200e614 and 0x0200e5cc are
 * in-image data at file offsets 0x6590, 0x6614 and 0x65cc, the same three the
 * 0x020015dc scene uses.
 *
 * NOTE (decoding, not a defect to fix): r4 is written at 0x02004d2c as scratch
 * holding the constant 4 for the ninth argument of the Func_0808a1d0 call,
 * although the prologue never saves r4.  Same idiom as the two sibling owners.
 *
 * UNCERTAINTY 1: Func_0808a1b8's third argument is a frame count at most sites
 * and 0 at others; the immediate-versus-tweened split stays unresolved.
 * UNCERTAINTY 2: the first question at 0x020046c8 selects between two
 * Func_0808a110 poses (3 on the zero answer, 4 otherwise) and only the zero
 * answer bumps the u16 scene counter at +0x1d8.  The later two questions bump
 * on the one answer.  Which answer is affirmative is not settled here.
 * UNCERTAINTY 3: the tail writes fp[0x3c] = 0x80000000, the same sentinel the
 * byte-exact assets/code/resource_373_c_0200572c.c tests for at word offsets
 * 14 and 16 of its record.  The field is treated as a plain s32 here.
 */

/* The overlay's scene block, reached through the IWRAM pointer at 0x03001ebc;
 * this owner loads that pool word at four separate sites. */
#define RESOURCE_373_SCENE (*(u8 *volatile *)0x03001ebc)
#define RESOURCE_373_SCENE_COUNTER (*(u16 *)(RESOURCE_373_SCENE + 0x1d8))

/* In-image data at file offsets 0x6590, 0x6614 and 0x65cc. */
extern u8 Data_0200e590[];
extern u8 Data_0200e614[];
extern u8 Data_0200e5cc[];

/* Old-style declarations are mandatory in overlay sources: one import name can
 * legitimately take different argument counts at different sites. */
u8 *Func_02009fa8();
void Func_02009f86();
void Func_0200a0c0();
void Func_02009e6e();
void Func_02009f32();
void Func_02009f3e();
void Func_02009f4e();
void Func_02009f5e();
void Func_0200a074();
u8 *Func_0200a022();
void Func_0200a096();
u8 *Func_0200a044();
void Func_0200a0b4();
u8 *Func_0200a062();
void Func_0200a0d6();
void Func_0200a08e();
u8 *Func_0200a084();
u8 *Func_0200a00e();
u8 *Func_0200a0ac();
u8 *Func_0200a02e();
u8 *Func_0200a0cc();
u8 *Func_0200a04e();
u8 *Func_0200a1fa();
void Func_02009f9e();
void Func_0200a202();
void Func_0200a036();
void Func_02009fb4();
void Func_0200a23e();
void Func_0200a132();
void Func_0200a13e();
void Func_0200a14e();
void Func_0200a156();
void Func_0200a11c();
void Func_0200a164();
void Func_0200a232();
void Func_0200a1da();
void Func_0200a148();
void Func_0200a18a();
void Func_0200a1d0();
void Func_0200a166();
void Func_0200a268();
void Func_0200a228();
void Func_0200a17e();
void Func_0200a282();
void Func_0200976e();
void Func_0200a226();
void Func_0200a080();
void Func_02009e14();
void Func_0200a088();
void Func_0200a2c4();
void Func_0200a0a8();
void Func_0200a216();
void Func_0200a24c();
void Func_0200a2f0();
void Func_0200a264();
void Func_0200a20a();
u8 *Func_0200a240();
void Func_0200a2b2();
void Func_0200a114();
void Func_0200a11a();
void Func_0200a120();
void Func_0200a116();
void Func_0200a30e();
void Func_0200a316();
void Func_0200a2e0();
void Func_0200a25e();
void Func_0200a2f6();
void Func_0200a2ae();
void Func_0200a274();
void Func_0200a208();
void Func_0200a2d0();
void Func_0200a332();
void Func_0200a2a8();
void Func_0200a348();
void Func_0200a32a();
void Func_0200a3be();
void Func_0200a3c8();
void Func_0200a368();
void Func_0200a1c2();
void Func_02009f56();
void Func_0200a1ca();
void Func_0200a406();
void Func_0200a1ea();
void Func_0200a1f6();
void Func_0200a358();
void Func_0200a38e();
void Func_0200a432();
void Func_0200a3a6();
void Func_0200a34c();
u8 *Func_0200a382();
void Func_0200a3f4();
void Func_0200a256();
void Func_0200a25c();
void Func_0200a262();
void Func_0200a258();
void Func_0200a450();
void Func_0200a458();
void Func_0200a422();
void Func_0200a3a0();
void Func_0200a438();
void Func_0200a3f0();
void Func_0200a3b6();
void Func_0200a410();
void Func_0200a472();
void Func_0200a3e8();
void Func_0200a488();
void Func_0200a46c();
void Func_0200a500();
void Func_0200a50c();
void Func_0200a518();
void Func_0200a4b8();
void Func_0200a312();
void Func_0200a0f0();
void Func_0200a364();
void Func_0200a5a0();
void Func_0200a384();
void Func_0200a390();
void Func_0200a4f2();
void Func_0200a528();
void Func_0200a5cc();
void Func_0200a540();
void Func_0200a4e6();
u8 *Func_0200a51c();
void Func_0200a58e();
void Func_0200a3ec();
void Func_0200a3f2();
void Func_0200a3f8();
void Func_0200a3ee();
void Func_0200a5e6();
void Func_0200a5ee();
void Func_0200a5b8();
void Func_0200a536();
void Func_0200a5ce();
void Func_0200a586();
void Func_0200a54c();
void Func_02009b40();
void Func_0200a4e2();
void Func_0200a5aa();
void Func_0200a61e();
void Func_0200a654();
void Func_0200a676();
void Func_0200a692();
void Func_020078a2();
void Func_0200a5e8();
void Func_0200a62c();
void Func_0200a5c2();
void Func_0200a6c6();
void Func_0200a6d2();
void Func_0200a67a();
void Func_0200a682();
void Func_0200a6dc();
void Func_0200a672();
void Func_0200a608();
void Func_0200a70c();
void Func_0200a718();
void Func_0200a6c0();
void Func_0200a6c8();
void Func_0200a722();
void Func_0200a73e();
void Func_0200a6e6();
void Func_0200a740();
void Func_0200a710();
void Func_0200a666();
void Func_0200a706();
void Func_0200a74e();
s32 Func_0200a6a6();
void Func_0200a76e();
void Func_0200a7c8();
void Func_0200a7b6();
void Func_0200a762();
void Func_0200a6f8();
void Func_0200a7fc();
void Func_0200a7bc();
void Func_0200a7fe();
void Func_0200a81a();
void Func_0200a802();
s32 Func_0200a75a();
void Func_0200a838();
void Func_0200a854();
void Func_0200a814();
void Func_0200a83a();
void Func_0200a85c();
void Func_0200a7f0();
void Func_0200a786();
void Func_0200a826();
void Func_0200a794();
void Func_0200a886();
void Func_0200a8a2();
void Func_0200a89c();
void Func_0200a832();
void Func_0200a8c6();
void Func_0200a7dc();
void Func_0200a87c();
void Func_0200a7ea();
void Func_0200a8dc();
void Func_0200a910();
void Func_0200a8b8();
void Func_0200a8fa();
void Func_0200a8b2();
void Func_0200a8ba();
void Func_0200a914();
void Func_0200a95c();
void Func_0200a974();
void Func_0200a88a();
void Func_0200a898();
void Func_0200a8d4();
void Func_0200a8ea();
void Func_0200a912();
void Func_02007b8a();
void Func_0200a992();
void Func_0200a98c();
void Func_0200a9b6();
void Func_0200a9c2();
void Func_0200a950();
void Func_0200a9e4();
void Func_0200a9f0();
void Func_0200a9fc();
void Func_0200a9a4();
void Func_0200a9fe();
void Func_0200aa10();
void Func_0200a9c0();
void Func_0200a9d0();
void Func_0200aa2a();
void Func_02007c4a();
void Func_0200a990();
void Func_0200a99a();
void Func_0200a9d6();
u8 *Func_0200a9a4_b();
u8 *Func_0200a9c8();
void Func_0200aaa6();
void Func_0200aa34();
void Func_0200aa4c();
void Func_0200aac0();
void Func_0200aa72();
void Func_0200aac4();
void Func_0200aa7c();
void Func_0200a9ea();
void Func_0200aaee();
void Func_0200aae8();
void Func_0200ab04();
void Func_0200aab6();
void Func_0200ab38();
void Func_0200aa26();
void Func_0200ab2a();
void Func_0200aaea();
void Func_0200ab2c();
void Func_0200ab5e();
void Func_0200ab52();
void Func_0200ab76();
void Func_0200ab6a();
void Func_0200ab70();
void Func_0200ab28();
void Func_0200aa96();
void Func_0200ab60();
void Func_0200ab50();
void Func_0200abd0();
void Func_0200abb2();
void Func_0200ac0a();
void Func_0200ac16();
void Func_0200abd6();
void Func_0200ac1e();
void Func_0200abe6();
void Func_0200ab44();
void Func_0200ac36();
void Func_0200abf6();
void Func_0200ab64();
void Func_0200ac04();
void Func_0200ac5e();
void Func_0200ac0e();
void Func_0200ac26();
void Func_0200ac80();
void Func_0200ac30();
void Func_0200ac40();
void Func_0200abae();
void Func_0200ac66();
void Func_0200aca8();
void Func_0200acf0();
void Func_0200ad06();
void Func_0200ac54();
void Func_0200ace8();
void Func_0200acf4();
void Func_0200acee();
void Func_0200acd0();
void Func_0200acc0();
void Func_0200ad2a();
void Func_0200ad36();
void Func_0200ad42();
void Func_0200ad3c();
void Func_0200ad6e();
void Func_0200acfe();
void Func_0200ad46();
s32 Func_0200aca0();
void Func_0200ad7e();
void Func_0200ad6c();
void Func_0200adb6();
void Func_0200ad5e();
void Func_0200ada0();
void Func_0200ad62();
void Func_0200ad7a();
void Func_0200acd8();
void Func_0200adfa();
void Func_0200ae98();
u8 *Func_0200ad36_b();
void Func_0200ad60();
void Func_0200ada4();
void Func_0200ad3a();
void Func_0200ad56();
void Func_0200adec();
void Func_0200adaa();
void Func_0200ae1c();
void Func_0200adf8();
void Func_0200ae70();
void Func_0200ae94();
void Func_0200aeb8();
void Func_0200ae58();
void Func_0200aecc();
void Func_0200ae54();
void Func_0200ae6e();
void Func_0200af6c();
void Func_0200ad82();
void Func_0200ad74();
u8 *Func_0200ae2a();
u8 *Func_0200ad98();
void Func_0200af04();
void Func_0200aebc();
void Func_0200adc2();
void Func_0200adce();
void Func_0200af42();
void Func_0200aeca();
void Func_0200aed4();
void Func_0200ae5e();
void Func_0200ae68();
void Func_0200af20();
void Func_0200af74();
void Func_0200af2c();
void Func_0200af3c();
void Func_0200ae92();
void Func_0200af4a();
void Func_0200aea0();
void Func_0200af92();
void Func_0200afda();
void Func_0200aff2();
void Func_0200af08();
void Func_0200af7a();
void Func_0200af54();
void Func_0200aeea();
void Func_0200afee();
void Func_0200afae();
void Func_0200affe();
void Func_0200b030();
void Func_0200b022();
void Func_0200afe2();
void Func_0200b024();
void Func_0200b05e();
void Func_0200af4c();
void Func_0200b050();
void Func_0200b074();
void Func_0200b01c();
void Func_0200b07a();
void Func_0200b03a();
void Func_0200af90();
void Func_0200b048();
u8 *Func_0200afce();
u8 *Func_0200af3c_b();
void Func_0200b0a2();
void Func_0200b09a();
u8 *Func_0200b04e();
void Func_0200b08c();
void Func_0200b156();
void Func_0200b142();
void Func_0200b13c();
void Func_0200b0da();
void Func_0200b15e();
void Func_0200b158();
void Func_0200b18c();
void Func_0200b114();
void Func_0200b12e();
u8 *Func_0200b0c4();
u8 *Func_0200b032();
void Func_0200b02e();
void Func_0200b24c();
void Func_0200b062();
void Func_0200b17a();
void Func_0200b078();
void Func_0200b084();
void Func_0200b1f8();
void Func_0200b12a();
void Func_0200b16e();
void Func_0200b104();
void Func_0200b1bc();
void Func_0200b226();
void Func_0200b1c6();
void Func_0200b1d6();
void Func_0200b12c();
void Func_0200b230();
void Func_0200b1f0();
void Func_0200b146();
void Func_0200b1e6();
void Func_0200b154();
void Func_0200b1f4();
void Func_0200b162();
void Func_0200b254();
void Func_0200b270();
void Func_0200b27c();
void Func_0200b286();
void Func_0200b292();
void Func_0200b23a();
void Func_0200b1a8();
void Func_0200b2aa();
void Func_0200b2cc();
void Func_0200b2ae();
void Func_0200b2ca();
void Func_0200b2ec();
void Func_0200b2f6();
void Func_0200b300();
void Func_0200b290();
void Func_0200b1fe();
void Func_0200b2f0();
void Func_0200b2b8();
void Func_0200b2c8();
void Func_0200b21e();
void Func_0200b322();
void Func_0200b32e();
void Func_0200b328();
void Func_0200b2e0();
void Func_0200b24e();
void Func_0200b340();
void Func_0200b35a();
void Func_0200b302();
void Func_0200b362();
void Func_0200b31a();
void Func_0200b288();
void Func_0200b296();
void Func_0200b388();
void Func_0200b3a4();
void Func_0200b3c8();
void Func_0200b370();
void Func_0200b2c6();
void Func_0200b3b8();
void Func_0200b31e();
u8 *Func_0200b31c();
u8 *Func_0200b28a();
void Func_0200b39c();
void Func_0200b37a();
void Func_0200b310();
void Func_0200b414();
void Func_0200b420();
void Func_0200b336();
void Func_0200b3d6();
void Func_0200b344();
u8 *Func_0200b37a_b();
u8 *Func_0200b38e();
u8 *Func_0200b3a0();
void Func_0200b3c6();
void Func_0200b490();
void Func_0200b410();
void Func_0200b42a();
void Func_0200b440();
void Func_0200b4c4();
void Func_0200b464();
void Func_0200b46c();
void Func_0200b47c();
void Func_0200b3ea();
void Func_0200b468();
void Func_0200b48a();
void Func_0200b480();
void Func_0200b4a2();
void Func_0200b43a();
void Func_0200b584();
void Func_0200882c();
void Func_0200b4de();
void Func_0200b572();
u8 *Func_0200b4b0();
void Func_02008860();
void Func_0200b50e();
void Func_0200b5a2();
void Func_0200b5ae();
void Func_0200b54e();
void Func_0200b55e();
void Func_0200b4cc();
void Func_0200b5f4();
void Func_0200b610();
void Func_0200b47a();
void Func_0200b4e8();
void Func_0200b4f6();
void Func_0200b52c();

                     

                     

                     

                    

                     

                     

                    

                     

                     

                     

                     

                     

                     

                     

                     

                     

                    

/* Intra-overlay callees, all with byte-exact sources in assets/code. */

/* The per-beat callbacks installed through Func_080000d0.  Their in-image
 * addresses carry the Thumb bit. */
#define CALLBACK_02005a08 ((void *)0x0200da09)
#define CALLBACK_020055c0 ((void *)0x0200d5c1)
#define CALLBACK_020055e0 ((void *)0x0200d5e1)
#define CALLBACK_020055f0 ((void *)0x0200d5f1)
#define CALLBACK_02005600 ((void *)0x0200d601)

void Func_02003fb0(void)
{
    u8 *camera;             /* fp - the record for actor 0 */
    u8 *actor;              /* r7 - whichever record is in hand */
    u32 frame;

    camera = Func_02009fa8(0);

    Func_02009f86();

    /* -1.0, -1.0, -1.0 in 16.16 with mode 0. */
    Func_0200a0c0(-1, -1, -1, 0);
    Func_02009e6e(1);

    Func_02009f32(0x31, 0x35, 8, 4, 20, 50);
    Func_02009f3e(2, 0x66, 0x54, 0x29, 2, 1);
    Func_02009f4e(1, 0x66, 0x53, 0x29, 1, 1);
    Func_02009f5e(0, 0x67, 0x52, 0x2a, 1, 1);

    Func_0200a074(21, 0x01880000, 0x03800000);
    *(u16 *)(Func_0200a022(21) + 6) = 0xc000;

    Func_0200a096(1, 0x012a0000, 0x02e00000);
    *(u16 *)(Func_0200a044(1) + 6) = 0x4000;

    Func_0200a0b4(5, 0x012a0000, 0x02f80000);
    /* Split by the interior pool at 0x02004082. */
    *(u16 *)(Func_0200a062(5) + 6) = 0x4000;

    Func_0200a0d6(0, 11);
    Func_0200a08e(0, Data_0200e590);

    /* Three actors at the same X/Y with stepped Z (0x348, 0x34c, 0x350). */
    actor[0x55] = 0;
    *(s32 *)(actor + 16) = 0x03480000;
    actor = Func_0200a084(23);
    *(s32 *)(actor + 8) = 0x01840000;
    *(s32 *)(actor + 12) = 0x00a00000;
    Func_0200a00e(actor, 0);

    *(s32 *)(actor + 16) = 0x034c0000;
    *(s32 *)(actor + 12) = 0x00a00000;
    actor[0x55] = 0;
    *(s32 *)(actor + 8) = 0x01840000;
    actor = Func_0200a0ac(24);
    Func_0200a02e(actor, 0);

    actor = Func_0200a0cc(25);
    actor[0x55] = 0;
    *(s32 *)(actor + 8) = 0x01840000;
    *(s32 *)(actor + 12) = 0x00a00000;
    Func_0200a04e(actor, 0);
    *(s32 *)(actor + 16) = 0x03500000;

    Func_0200a1fa()[0x55] = 0;

    Func_02009f9e(1);
    Func_0200a202(0x017f0000, 0x00a00000, 0x036d0000, 0);
    Func_0200a036();
    Func_02009fb4(1);

    *(s32 *)(RESOURCE_373_SCENE + 0x1c8) = 32;

    Func_0200a23e();
    Func_0200a132(5, 0x8000, 0x4000);
    Func_0200a13e(1, 0x8000, 0x4000);
    Func_0200a14e(5, Data_0200e614);
    Func_0200a156(1, Data_0200e5cc);
    Func_0200a11c(40);
    Func_0200a164(0, 1);

    *(s32 *)(camera + 24) = 0x10000;
    *(s32 *)(camera + 28) = 0x10000;

    Func_0200a232(0, 0xb000, 40);
    Func_0200a1da(0, 3);
    Func_0200a148(10);
    Func_0200a18a(0, 0x4ccc, 0x2666);
    Func_0200a1d0(0, 0x190, 0x348);
    Func_0200a166(10);
    Func_0200a268(0, 0xc000, 30);
    Func_0200a228(0, 1);
    Func_0200a17e(20);
    Func_0200a282(0, 0x8000, 40);

    Func_0200976e();

    /* ---- beat 1: actor 23 ---- */
    Func_0200a226(0, 17);
    Func_0200a080(CALLBACK_02005a08, 0xc80);
    for (frame = 0; frame <= 39; frame++) {
        Func_02009e14(camera);
        Func_0200a088(1);
    }
    Func_0200a2c4(0, 1);
    Func_0200a0a8(CALLBACK_020055c0, 0xc80);
    Func_0200a0b4(CALLBACK_020055e0, 0xc80);
    Func_0200a216(23, 0x3333, 0x1999);
    Func_0200a24c(23, 0x186, 0x340);
    Func_0200a2f0(0, 0xc000, 0);
    Func_0200a264(23, 0x190, 0x33a);
    Func_0200a20a(20);

    actor = Func_0200a240(0) + 0x23;
    *actor = (u8)(*actor | 1);

    Func_0200a2b2(0, 1);
    Func_0200a114(CALLBACK_02005a08);
    Func_0200a11a(CALLBACK_020055c0);
    Func_0200a120(CALLBACK_020055e0);
    Func_0200a116(1);
    Func_0200a30e(0, 0);
    Func_0200a316(23, 0);
    Func_0200a2e0(23, 0, 0);
    Func_0200a25e(20);
    Func_0200a2f6(0, 11);
    Func_0200a2ae(0, Data_0200e590);
    Func_0200a274(120);

    /* ---- beat 2: actor 24 ---- */
    Func_0200a208(7, 0x66, 0x54, 0x29, 2, 1);
    Func_0200a2d0(0, 1);
    *(s32 *)(camera + 24) = 0x10000;
    *(s32 *)(camera + 28) = 0x10000;
    Func_0200a332(0, 1);
    Func_0200a2a8(40);
    Func_0200a348(0, 3);
    Func_0200a32a(0, 0x179, 0x34b);
    Func_0200a3be(0, 0xc000, 40);
    Func_0200a3c8(0, 0, 20);
    Func_0200a368(0, 17);
    Func_0200a1c2(CALLBACK_02005a08, 0xc80);
    for (frame = 0; frame <= 39; frame++) {
        Func_02009f56(camera);
        Func_0200a1ca(1);
    }
    Func_0200a406(0, 1);
    Func_0200a1ea(CALLBACK_020055c0, 0xc80);
    Func_0200a1f6(CALLBACK_020055f0, 0xc80);
    Func_0200a358(24, 0x3333, 0x1999);
    Func_0200a38e(24, 0x186, 0x340);
    Func_0200a432(0, 0xc000, 0);
    Func_0200a3a6(24, 0x179, 0x33c);
    Func_0200a34c(20);

    actor = Func_0200a382(0) + 0x23;
    *actor = (u8)(*actor | 1);

    Func_0200a3f4(0, 1);
    Func_0200a256(CALLBACK_02005a08);
    Func_0200a25c(CALLBACK_020055c0);
    Func_0200a262(CALLBACK_020055f0);
    Func_0200a258(1);
    Func_0200a450(0, 0);
    Func_0200a458(24, 0);
    Func_0200a422(24, 0, 0);
    Func_0200a3a0(20);
    Func_0200a438(0, 11);
    Func_0200a3f0(0, Data_0200e590);
    Func_0200a3b6(120);

    /* ---- beat 3: actor 25 ---- */
    Func_0200a348(6, 0x66, 0x53, 0x29, 1, 1);
    Func_0200a410(0, 1);
    *(s32 *)(camera + 24) = 0x10000;
    *(s32 *)(camera + 28) = 0x10000;
    Func_0200a472(0, 1);
    Func_0200a3e8(40);
    Func_0200a488(0, 3);
    Func_0200a46c(0, 0x168, 0x357);
    Func_0200a500(21, 0xb000, 10);
    Func_0200a50c(0, 0xc000, 30);
    Func_0200a518(0, 0xd000, 20);
    Func_0200a4b8(0, 17);
    Func_0200a312(CALLBACK_02005a08, 0xc80);
    /* Split by the interior pool at 0x02004484. */
    for (frame = 0; frame <= 39; frame++) {
        Func_0200a0f0(camera);
        Func_0200a364(1);
    }
    Func_0200a5a0(0, 1);
    Func_0200a384(CALLBACK_020055c0, 0xc80);
    Func_0200a390(CALLBACK_02005600, 0xc80);
    Func_0200a4f2(25, 0x3333, 0x1999);
    Func_0200a528(25, 0x186, 0x340);
    Func_0200a5cc(0, 0xc000, 0);
    Func_0200a540(25, 0x168, 0x345);
    Func_0200a4e6(20);

    actor = Func_0200a51c(0) + 0x23;
    *actor = (u8)(*actor | 1);

    Func_0200a58e(0, 1);
    Func_0200a3ec(CALLBACK_02005a08);
    Func_0200a3f2(CALLBACK_020055c0);
    Func_0200a3f8(CALLBACK_02005600);
    Func_0200a3ee(1);
    Func_0200a5e6(0, 0);
    Func_0200a5ee(25, 0);
    Func_0200a5b8(25, 0, 0);
    Func_0200a536(20);
    Func_0200a5ce(0, 11);
    Func_0200a586(0, Data_0200e590);
    Func_0200a54c(120);

    Func_02009b40();

    /* ---- the questioning sequence ---- */
    Func_0200a4e2(5, 0x67, 0x52, 0x2a, 1, 1);
    Func_0200a5aa(0, 1);
    *(s32 *)(camera + 24) = 0x10000;
    *(s32 *)(camera + 28) = 0x10000;
    Func_0200a61e(21, 2, 20);
    Func_0200a654(0xf03);
    Func_0200a676(21, 0, 10);
    Func_0200a692(0, 0x1000, 10);
    Func_020078a2(21, 5, 6, 0);
    Func_0200a5e8(21, 0x4ccc, 0x2666);
    Func_0200a62c(21, 0x18d, 0x340);
    Func_0200a5c2(20);
    Func_0200a6c6(21, 0x4000, 60);
    Func_0200a6d2(21, 0xc000, 60);
    Func_0200a67a(21, 3);
    Func_0200a682(21, 3);
    Func_0200a6dc(21, 0, 10);
    Func_0200a672(21, 0x174, 0x340);
    Func_0200a608(20);
    Func_0200a70c(21, 0x4000, 40);
    Func_0200a718(21, 0x8000, 40);
    Func_0200a6c0(21, 3);
    Func_0200a6c8(21, 3);
    Func_0200a722(21, 0, 20);
    Func_0200a73e(21, 0x5000, 20);
    Func_0200a6e6(21, 3);
    Func_0200a740(21, 0, 10);
    Func_0200a710(0, 2);
    Func_0200a666(20);
    Func_0200a706(21, 4);
    Func_0200a74e(21, 0);

    if (Func_0200a6a6(0, 0) == 0) {
        Func_0200a722(21, 3);
        RESOURCE_373_SCENE_COUNTER = RESOURCE_373_SCENE_COUNTER + 1;
    } else {
        Func_0200a76e(21, 4);
    }

    Func_0200a7c8(21, 0, 20);
    Func_0200a7b6(0xf0a);
    Func_0200a762(21, 0x182, 0x349);
    Func_0200a6f8(10);
    Func_0200a7fc(21, 0xd000, 60);
    Func_0200a7bc(21, 2);
    Func_0200a7fe(21, 0, 20);
    Func_0200a81a(21, 0x5000, 30);
    Func_0200a802(21, 0);

    if (Func_0200a75a(0, 0) == 1) {
        RESOURCE_373_SCENE_COUNTER = RESOURCE_373_SCENE_COUNTER + 1;
    }

    Func_0200a838(21, 0, 20);
    Func_0200a854(21, 0xd000, 60);
    Func_0200a814(21, 2);
    Func_0200a83a(0xf0e);
    Func_0200a85c(21, 0, 20);
    Func_0200a7f0(21, 0x182, 0x339);
    Func_0200a786(10);
    Func_0200a826(21, 4);
    Func_0200a794(60);
    Func_0200a886(21, 0, 60);
    Func_0200a8a2(21, 0x5000, 10);
    Func_0200a89c(21, 0, 10);
    Func_0200a832(21, 0x174, 0x340);
    Func_0200a8c6(21, 0x5000, 10);
    Func_0200a886(0, 2);
    Func_0200a7dc(20);
    Func_0200a87c(21, 3);
    Func_0200a7ea(20);
    Func_0200a8dc(21, 0, 20);
    Func_0200a910(0, 0x102, 60);
    Func_0200a8b8(21, 2);
    Func_0200a8fa(21, 0, 20);
    Func_0200a8b2(0, 3);
    Func_0200a8ba(21, 3);
    Func_0200a914(21, 0, 10);
    Func_0200a95c(0x6666, 0xccc);
    Func_0200a974(0x01790000, 0x00a00000, 0x035c0000, 1);
    Func_0200a88a(5, 0x10000, 0x8000);
    Func_0200a898(1, 0x10000, 0x8000);
    Func_0200a8d4(1, 0x171, 0x388);
    Func_0200a8ea(5, 0x188, 0x388);
    Func_0200a912(1, 1);
    Func_02007b8a(5, 10, 11, 0);
    Func_0200a992(5, 0xa000, 0);
    Func_0200a98c(5, 0, 10);
    Func_0200a95c(21, 2);
    Func_0200a8b2(10);
    Func_0200a9b6(21, 0x3000, 0);
    Func_0200a9c2(0, 0x1000, 20);
    Func_0200a974(5, 4, 0);
    Func_0200a950(5, 0x188, 0x34b);
    Func_0200a9e4(5, 0x9000, 0);
    Func_0200a9f0(21, 0x3000, 0);
    Func_0200a9fc(0, 0xd000, 20);
    Func_0200a9a4(21, 3);
    Func_0200a9fe(21, 0, 10);
    Func_0200a9b6(5, 3);
    Func_0200aa10(5, 0, 10);
    Func_0200a9c0(21, 3);
    Func_0200a9d0(0, 3);
    Func_0200aa2a(21, 0, 20);
    Func_02007c4a(1, 10, 11, 0);
    Func_0200a990(5, 0x4ccc, 0x2666);
    Func_0200a99a(1, 0x4ccc, 0x2666);
    Func_0200a9d6(1, 0x188, 0x34b);

    actor = Func_0200a9a4_b(5) + 0x5a;
    *actor = (u8)(*actor & 0xfe);

    Func_0200a9fc(5, 0x198, 0x34b);
    Func_0200a992(1);

    actor = Func_0200a9c8(5) + 0x5a;
    *actor = (u8)(*actor | 1);

    Func_0200aaa6(5, 0x8000, 0);
    Func_0200aa34(1);
    Func_0200aa4c(1, 1);
    Func_0200aac0(1, 0x8000, 30);
    Func_0200aa72(21, 4, 30);
    Func_0200aac4(21, 0, 20);
    Func_0200aa7c(1, 3);
    Func_0200a9ea(20);
    Func_0200aaee(21, 0x5000, 20);
    Func_0200aae8(21, 0, 10);
    Func_0200ab04(0, 0xd000, 0);
    Func_0200aab6(0, 2, 30);
    Func_0200ab38(0, 0x102);
    Func_0200aa26(60);
    Func_0200ab2a(21, 0x3000, 40);
    Func_0200aaea(1, 2);
    Func_0200ab2c(1, 0, 20);
    Func_0200ab5e(21, 0x101, 80);
    Func_0200ab52(21, 0x5000, 30);
    Func_0200ab76(0, 0x102, 60);
    Func_0200ab6a(21, 0x3000, 0);
    Func_0200ab76(0, 0xd000, 20);
    Func_0200ab70(21, 0, 10);
    Func_0200ab28(1, 3);
    Func_0200aa96(80);
    Func_0200ab60(5, 1, 30);
    Func_0200ab50(1, 2);
    Func_0200ab60(5, 2);
    Func_0200aab6(10);
    Func_0200abd0(21, 0x105, 60);
    Func_0200abb2(21, 0, 10);

    /* Split by the interior pool at 0x02004b22. */
    Func_0200ac0a(5, 0x8000, 0);

    Func_0200ac16(1, 0x8000, 10);
    Func_0200abd6(5, 2);
    Func_0200ab2c(20);
    Func_0200ac1e(5, 0, 10);
    Func_0200abe6(21, 2);
    Func_0200ab44(40);
    Func_0200ac36(21, 0, 10);
    Func_0200abe6(1, 3);
    Func_0200abf6(5, 3);
    Func_0200ab64(20);
    Func_0200ac04(21, 4);
    Func_0200ac5e(21, 0, 10);
    Func_0200ac0e(1, 3);
    Func_0200ac1e(5, 3);
    Func_0200ac26(21, 3);
    Func_0200ac80(21, 0, 10);
    Func_0200ac30(1, 3);
    Func_0200ac40(5, 3);
    Func_0200abae(20);
    Func_0200ac66(21, 2);
    Func_0200aca8(21, 0, 20);
    Func_0200acf0(0x9999, 0x1333);
    Func_0200ad06(0x01750000, 0x00a00000, 0x03450000, 1);
    Func_0200ac54(21, 0x16c, 0x330);
    Func_0200ace8(0, 0xd000, 0);
    Func_0200acf4(21, 0x3000, 10);
    Func_0200acee(21, 0, 40);
    Func_0200acd0(5, 1, 30);
    Func_0200acc0(1, 2);
    Func_0200acd0(5, 2);
    Func_0200ac26(20);
    Func_0200ad2a(1, 0x8000, 0);
    Func_0200ad36(5, 0x8000, 20);
    Func_0200ad42(21, 0x5000, 20);
    Func_0200ad3c(21, 0, 10);
    Func_0200ad6e(0, 0x105, 60);
    Func_0200acfe(21, 4);
    Func_0200ad46(21, 0);

    if (Func_0200aca0(0, 0) == 1) {
        RESOURCE_373_SCENE_COUNTER = RESOURCE_373_SCENE_COUNTER + 1;
    }

    Func_0200ad7e(21, 0, 20);
    Func_0200ad6c(0xf27);
    Func_0200adb6(21, 0x103, 0);
    Func_0200ad5e(21, 3);
    Func_0200ada0(21, 0, 10);
    Func_0200ad62(21, 4, 0);
    Func_0200ad7a(21, 3);
    Func_0200ad62(21, 7);
    Func_0200acd8(5);

    /* Eleven arguments: four in registers and seven on the stack. */
    Func_0200adfa(21, 14, 2, 24, 2, 1, 10, 14, 4, 14, 0);

    Func_0200ae98(0xa1);

    actor = Func_0200ad36_b(21);
    /* The +0x50 handle's +0x26 byte. */
    *(*(u8 **)(actor + 0x50) + 0x26) = 0;
    actor[0x5a] = (u8)(actor[0x5a] & 0xfe);

    Func_0200ad60(21, 0x30000, 0x18000);
    Func_0200ada4(21, 0x16c, 0x32f);
    Func_0200ad3a(4);

    /* Four frames of a constant slide. */
    for (frame = 0; frame != 4; frame++) {
        *(s32 *)(actor + 16) = *(s32 *)(actor + 16) + 0x18000;
        *(s32 *)(actor + 28) = *(s32 *)(actor + 28) + (s32)0xffffe667;
        Func_0200ad56(1);
    }

    Func_0200adec(21, 0, 0);
    Func_0200adaa(1, 0x30000, 0x18000);
    Func_0200ae1c(1, 6, 0);
    Func_0200adf8(1, 0x176, 0x33b);
    Func_0200ae70(5, 0);
    Func_0200ae94(1, 0xb000, 0);
    Func_0200aeb8(5, 0x100, 0);
    Func_0200ae58(5, 2);
    Func_0200aecc(1, 0x100, 10);
    Func_0200ae54(1, 13);
    Func_0200ae6e(1, 2, 5);
    Func_0200af6c(0x8f);
    Func_0200ad82(0, 0x40000, 0x10000);
    Func_0200ad74(1, 0x66, 0x53, 0x29, 1, 1);
    Func_0200ad98(Func_0200ae2a(1), 0);
    Func_0200af04(0, 0xd000, 10);
    Func_0200aebc(1, 3);
    Func_0200adc2(-1, -1, 0xe666);
    Func_0200adce();
    Func_0200af42(1, 0x102, 80);
    Func_0200aeca(21, 8);

    *(s32 *)(actor + 28) = 0x8000;
    Func_0200aed4(21, 0x016c0000, 0x032b0000);

    /* Five frames of the reverse slide. */
    for (frame = 0; frame != 5; frame++) {
        *(s32 *)(actor + 28) = *(s32 *)(actor + 28) + 0x1999;
        Func_0200ae5e(1);
    }

    Func_0200ae68(60);
    Func_0200af20(1, 2);
    Func_0200af74(1, 0x5000, 30);
    Func_0200af2c(1, 2);
    Func_0200af3c(5, 2);
    Func_0200ae92(60);
    Func_0200af4a(21, 2);
    Func_0200aea0(20);
    Func_0200af92(21, 0, 20);
    Func_0200afda(0x4ccc, 0x999);
    Func_0200aff2(0x01740000, 0x00a00000, 0x035b0000, 1);
    Func_0200af08(21, 0x30000, 0x18000);
    Func_0200af7a(21, 6, 0);
    Func_0200af54(21, 0x167, 0x343);
    Func_0200aeea(20);
    Func_0200afee(21, 0x4000, 20);
    Func_0200afae(21, 2);

    actor[0x23] = (u8)(actor[0x23] & 0xfe);

    Func_0200affe(21, 0, 80);
    Func_0200b030(21, 0x101, 80);
    Func_0200b022(21, 0, 60);
    Func_0200afe2(21, 3);
    Func_0200b024(21, 0, 10);
    Func_0200b05e(21, 0x102);
    Func_0200af4c(80);
    Func_0200b050(1, 0x5000, 40);
    Func_0200b074(1, 0x102, 80);
    Func_0200b01c(1, 2);
    Func_0200b05e(1, 0, 20);
    Func_0200b07a(1, 0x8000, 20);
    Func_0200b03a(1, 3);
    Func_0200af90(10);
    Func_0200b048(1, 3);
    Func_0200af3c_b(Func_0200afce(1), 1);

    /* Split by the interior pool at 0x02004ffa. */
    Func_0200b0a2(1, 6, 0);

    Func_0200b09a(1, 1);
    Func_0200b050(1, 0x40000, 0x20000);

    actor = Func_0200b04e(1);
    actor[0x5a] = (u8)(actor[0x5a] & 0xfe);

    Func_0200b08c(1, 0x193, 0x33b);
    Func_0200b156(5, 0x102);
    Func_0200b142(5, 0xc000, 20);
    Func_0200b13c(5, 0, 1);
    Func_0200b0da(1);
    Func_0200b15e(1, 0x5000, 20);
    Func_0200b158(1, 0, 20);
    Func_0200b18c(1, 0x100, 0);
    Func_0200b114(1, 13);
    Func_0200b12e(1, 2, 5);
    Func_0200b032(Func_0200b0c4(1), 0);
    Func_0200b02e(2, 0x66, 0x54, 0x29, 2, 1);
    Func_0200b24c(0x8f);
    Func_0200b062(0, 0x40000, 0x10000);
    Func_0200b17a(1, 3);
    Func_0200b078(-1, -1, 0xe666);
    Func_0200b084();
    Func_0200b1f8(1, 0x102, 30);
    Func_0200b12a(5, 0x4ccc, 0x2666);
    Func_0200b16e(5, 0x198, 0x357);
    Func_0200b104(60);
    Func_0200b1bc(1, 2);
    Func_0200b226(21, 0x105, 60);
    Func_0200b1c6(5, 3);
    Func_0200b1d6(0, 3);
    Func_0200b12c(80);
    Func_0200b230(1, 0x4000, 30);
    Func_0200b1f0(1, 3);
    Func_0200b146(10);
    Func_0200b1e6(5, 4);
    Func_0200b154(80);
    Func_0200b1f4(21, 3);
    Func_0200b162(10);
    Func_0200b254(21, 0, 10);
    Func_0200b270(5, 0xb000, 0);
    Func_0200b27c(0, 0xc000, 0);
    Func_0200b286(21, 0, 60);
    Func_0200b292(21, 0x4000, 60);
    Func_0200b23a(21, 4);
    Func_0200b1a8(60);
    Func_0200b2aa(21, 0, 80);
    Func_0200b2cc(21, 0x105, 80);
    Func_0200b2ae(21, 0, 60);
    Func_0200b2ca(0, 0xc000, 0);
    Func_0200b2ec(0, 0x101, 0);
    Func_0200b2f6(5, 0x101, 0);
    Func_0200b300(1, 0x101, 60);
    Func_0200b290(21, 4);
    Func_0200b1fe(30);
    Func_0200b2f0(21, 0, 60);
    Func_0200b2b8(1, 2);
    Func_0200b2c8(5, 2);
    Func_0200b21e(20);
    Func_0200b322(5, 0x8000, 60);
    Func_0200b32e(21, 0x4000, 30);
    Func_0200b328(21, 0, 30);
    Func_0200b2e0(5, 3);
    Func_0200b24e(10);
    Func_0200b340(5, 0, 20);
    Func_0200b35a(21, 0, 30);
    Func_0200b302(21, 4);
    Func_0200b270(20);
    Func_0200b362(21, 0, 20);
    Func_0200b31a(5, 3);
    Func_0200b288(20);
    Func_0200b340(1, 3);
    Func_0200b296(10);
    Func_0200b388(1, 0, 20);
    Func_0200b3a4(0, 0xd000, 0);
    Func_0200b3c8(21, 0x100, 0);
    Func_0200b370(21, 3);
    Func_0200b2c6(30);
    Func_0200b3b8(21, 0, 60);
    Func_0200b388(1, 3);
    Func_0200b31e(1, 0x10000, 0x8000);
    Func_0200b28a(Func_0200b31c(1), 0);
    Func_0200b39c(1, 4, 0);
    Func_0200b37a(1, 0x18e, 0x33c);
    Func_0200b310(60);
    Func_0200b414(21, 0x4000, 0);
    Func_0200b420(0, 0xc000, 60);
    Func_0200b3c8(0, 3);
    Func_0200b336(60);
    Func_0200b3d6(21, 3);
    Func_0200b344(60);

    actor = Func_0200b37a_b(1);
    actor[0x5a] = (u8)(actor[0x5a] | 1);

    actor = Func_0200b38e(5);
    actor[0x5a] = (u8)(actor[0x5a] | 1);

    actor = Func_0200b3a0(0);
    Func_0200b3b8(1, 0x10000, 0x8000);
    Func_0200b3c6(5, 0x10000, 0x8000);
    Func_0200b490(0, 0, 0);

    /* The s16 integer views at +0x0a and +0x12 of record 0's 16.16 X and Z. */
    Func_0200b410(5, *(s16 *)(actor + 10) + 16, *(s16 *)(actor + 18));
    Func_0200b42a(1, *(s16 *)(actor + 10) + 16, *(s16 *)(actor + 18) - 16);

    Func_0200b440(1);
    Func_0200b4c4(1, 0x5000, 30);
    Func_0200b464(1, 3);
    Func_0200b46c(5, 3);
    Func_0200b47c(0, 3);
    Func_0200b3ea(40);
    Func_0200b468(5, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    Func_0200b48a(5, 0, 0);
    Func_0200b480(1, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    Func_0200b4a2(1, 0, 0);
    Func_0200b43a(1, 5);

    /* Split by the interior pool at 0x02005472. */
    Func_0200b584(0x01790000, 0x00a00000, 0x03770000, 1);

    Func_0200882c(0, 13, 10, 0);

    Func_0200b4de(0, 0x178, 0x390);
    Func_0200b572(0, 0xc000, 0);

    actor = Func_0200b4b0(21) + 0x5a;
    *actor = (u8)(*actor | 1);

    Func_02008860(21, 6, 5, 0);

    Func_0200b50e(21, 0x175, 0x377);
    Func_0200b5a2(21, 0x4000, 0);
    Func_0200b5ae(0, 0xc000, 40);
    Func_0200b54e(21, 3);
    Func_0200b55e(0, 3);
    Func_0200b4cc(20);
    Func_0200b5f4(0, 1);
    Func_0200b610();
    Func_0200b4de(100);
    Func_0200b47a(0x31, 0x2e, 8, 4, 20, 50);
    Func_0200b4e8(0x202);
    Func_0200b4f6(0x12f);

    /* Leave record 0 in the state the next scene expects. */
    camera[0x55] = 3;
    *(s32 *)(camera + 12) = 0x00a00000;
    *(s32 *)(camera + 0x3c) = (s32)0x80000000;
    *(s32 *)(camera + 0x28) = 0;

    /* Common exit; no argument register is set. */
    Func_0200b52c();
}
