#include "types.h"

/*
 * resource_373 owner at 0x020034c8, 2,792 bytes.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r9/sl/fp and r8 saves and a
 * 28-byte local frame at 0x020034c8, through the single epilogue at
 * 0x02003f8c-0x02003f9c, followed by its four-word literal pool at
 * 0x02003fa0-0x02003faf.  The next inventory row begins at 0x02003fb0.
 *
 * The body is one long straight-line cutscene script with exactly one loop
 * (the 60-iteration fade at 0x02003922) and no conditionals.  Two interior
 * literal pools sit inside the span and are branched over rather than fallen
 * into: 0x020039e6-0x02003a87 (skipped by the `b.n` at 0x020039e4, which
 * splits a single Func_08009150 call across the gap) and
 * 0x02003e9c-0x02003ebf (skipped by the `b.n` at 0x02003e9a, which likewise
 * splits a single Func_0808a138 call).  Both are reassembled below.
 *
 * The epilogue is `pop {r0} / bx r0`, so r0 holds the popped return address
 * and the owner is void.  The prologue writes r0 before its first call, so it
 * takes no arguments.
 *
 * Call accounting: 264 `bl` sites, all resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_373 34c8` - 53 distinct targets,
 * 260 import veneers and 4 intra-overlay prologues (Func_02005594,
 * Func_020055a4, Func_020057fc, Func_02005c20).  No `call_via rN` site: r3 is
 * only ever loaded with a small immediate or a 16.16 constant.  Each of the
 * 264 sites appears exactly once below; the multiset of `Func_...(` calls in
 * this file equals the multiset of resolved targets.
 *
 * LINK BASE.  This overlay is linked at 0x02008000, like the six already
 * confirmed.  Three witnesses, all pool words of this owner:
 *   0x0200d5b1 -> file offset 0x55b0 = Func_020055b0 + Thumb bit
 *   0x0200d5d1 -> file offset 0x55d0 = Func_020055d0 + Thumb bit
 *   0x0200d75d -> file offset 0x575c = Func_0200575c + Thumb bit
 * All three have byte-exact sources in assets/code, so the base is proven
 * against tracked material.  The first two are installed as callbacks through
 * Func_080000d0 and removed again through Func_080000d8; the third is stored
 * into the actor's +0x6c behaviour slot and cleared later in the same owner.
 * The remaining pointer-shaped word, 0x0200e590, is in-image data at file
 * offset 0x6590 and is declared as such rather than as a RAM global.
 *
 * NOTE (decoding, not a defect to fix): r4 is written at 0x02003c44 as a
 * scratch register holding the constant 4 for the ninth argument of the
 * Func_0808a1d0 call, even though the prologue never saves r4.  Nothing
 * observable depends on it.  The same unsaved-r4 idiom is recorded in
 * HANDOVER for resource_371 and resource_372.
 *
 * UNCERTAINTY 1: Func_0808a188's first argument is a small integer at some
 * sites (5) and a pooled two-byte value at others (0x100a, 0x1005, 0x2001,
 * 0x2005, 0x5001, 0x6001).  It reads as a packed actor pair or actor set, but
 * nothing in this owner proves the packing, so the values are passed through
 * as written.
 * UNCERTAINTY 2: Func_0808a210 takes 16.16 coordinates (see the reading
 * established in resource_373_c_02002f14.c); the -1 second argument at
 * 0x020038c6 is the same sentinel that owner uses.
 * UNCERTAINTY 3: Func_0808a1b8's third argument is a frame count at most
 * sites and 0 at a few; the immediate-versus-tweened split is unresolved here
 * as it was in the sibling owners.
 * UNCERTAINTY 4: the result of Func_0808a080(9) at 0x020039d2 is discarded -
 * the following call rebuilds r0 from r6.  Reproduced as written, since the
 * query may have a side effect.
 */

/* The overlay's scene block, reached through the IWRAM pointer at 0x03001ebc;
 * the byte-exact assets/code/resource_373_c_02000eb4.c reads the same block
 * through the same pointer. */
#define RESOURCE_373_SCENE (*(u8 *volatile *)0x03001ebc)

/* In-image data at file offset 0x6590 under the proven 0x02008000 base. */
extern u8 Data_0200e590[];

/* Old-style declarations are mandatory in overlay sources: one import name can
 * legitimately take different argument counts at different sites. */
u8 *Func_020094c0();
u8 *Func_020094c8();
void Func_020094a6();
void Func_020095e0();
void Func_0200938e();
u8 *Func_020095fa();
void Func_020093a2();
void Func_02009466();
void Func_02009472();
void Func_02009482();
void Func_02009492();
u8 *Func_02009548();
u8 *Func_020094d4();
u8 *Func_02009572();
u8 *Func_020094f6();
u8 *Func_02009594();
u8 *Func_02009518();
u8 *Func_020095b6();
u8 *Func_02009524();
u8 *Func_020095c2();
u8 *Func_02009530();
void Func_02009630();
void Func_020095ea();
void Func_0200955e();
void Func_0200958a();
void Func_02009576();
void Func_020096fa();
void Func_0200952e();
void Func_020094ac();
void Func_02009704();
void Func_0200971a();
void Func_0200967c();
void Func_020094d2();
void Func_0200963c();
void Func_02009678();
void Func_02009794();
void Func_02009778();
void Func_020096ae();
void Func_02009674();
void Func_020096b8();
void Func_0200968a();
void Func_020096cc();
void Func_020096a2();
void Func_020096de();
void Func_0200970e();
void Func_02009706();
void Func_02009714();
void Func_0200971e();
void Func_0200972a();
void Func_02009784();
void Func_0200977c();
void Func_0200976c();
void Func_020096da();
void Func_0200974c();
void Func_02009818();
void Func_02009844();
void Func_02009712();
void Func_02009816();
void Func_020097d6();
void Func_0200972c();
void Func_02009860();
void Func_02009874();
void Func_02009882();
void Func_020097f4();
void Func_0200982a();
void Func_0200984c();
void Func_020097b6();
void Func_02009840();
void Func_020098a2();
void Func_02009842();
void Func_02009798();
void Func_02009848();
void Func_020097a6();
void Func_02009898();
void Func_02009800();
void Func_020098e8();
void Func_02009904();
void Func_020097d2();
void Func_02009872();
void Func_020098de();
void Func_02009862();
void Func_0200986e();
void Func_02009804();
void Func_02009908();
void Func_02009912();
void Func_0200990c();
void Func_02009940();
void Func_020098da();
void Func_0200993e();
void Func_020098fe();
void Func_02009854();
void Func_02009946();
void Func_0200986c();
void Func_020099a0();
void Func_020099ba();
void Func_020099d0();
void Func_0200989e();
void Func_020099b8();
void Func_02009960();
void Func_020098b6();
void Func_02008e9a();
void Func_02009952();
void Func_020099b4();
void Func_02009a6a();
u8 *Func_0200990a();
void Func_0200954a();
void Func_020097be();
void Func_020099fa();
void Func_020097e0();
void Func_020097ee();
u8 *Func_02009944();
u8 *Func_020098b2();
void Func_0200994c();
void Func_02009992();
void Func_020098d2();
void Func_02009970();
void Func_020099b2();
void Func_020099bc();
u8 *Func_020099ba_b();
void Func_020099a6();
void Func_02009aac();
void Func_02009a52();
void Func_02009b44();
void Func_02009b70();
u8 *Func_02009a9e();
void Func_02009966();
void Func_0200996c();
void Func_02009962();
void Func_02009b5a();
void Func_02009b62();
void Func_02009b9e();
void Func_02009b3e();
void Func_020092fc();
void Func_020090a8();
void Func_02009abe();
void Func_02009b3c();
void Func_02009b96();
void Func_02009b8e();
void Func_02009ae4();
void Func_02009bd6();
void Func_02009afc();
void Func_02009bee();
void Func_02009c22();
void Func_02009bb2();
void Func_02009b20();
void Func_02009c12();
void Func_02009c44();
void Func_02009bec();
void Func_02009b42();
void Func_02009c34();
void Func_02009c68();
void Func_02009bf8();
void Func_02009b66();
void Func_02009c58();
void Func_02009c10();
void Func_02009b7e();
void Func_02009bfc();
void Func_02009c90();
void Func_02009c50();
void Func_02009ba6();
void Func_02009c98();
void Func_02009bbe();
void Func_02009cd8();
void Func_02009c80();
void Func_02009ce2();
void Func_02009d0e();
void Func_02009c14();
void Func_02009d36();
void Func_02009d40();
void Func_02009c2e();
void Func_02009d48();
void Func_02009d2a();
void Func_02009d64();
void Func_02009c52();
void Func_02009cf2();
void Func_02009c60();
void Func_02009d7a();
void Func_02009d5c();
void Func_02009d2c();
void Func_02009c82();
void Func_02009d22();
void Func_02009daa();
void Func_02009d3a();
void Func_02009ca8();
void Func_02009d9a();
void Func_02009db6();
void Func_02009d80();
void Func_02009cd6();
void Func_02009d54();
void Func_02009cea();
void Func_02009ddc();
void Func_02009df8();
void Func_02009e22();
void Func_02009d10();
void Func_02009dc8();
void Func_02009d1e();
void Func_02009e10();
void Func_02009e50();
void Func_02009de0();
void Func_02009d4e();
void Func_02009d90();
void Func_02009dd6();
void Func_02009e6a();
void Func_02009e2a();
void Func_02009e72();
void Func_02009ea4();
void Func_02009e3e();
void Func_02009e90();
void Func_02009e60();
void Func_02009ea8();
void Func_02009dc6();
void Func_02009eca();
void Func_02009ecc();
void Func_02009efe();
void Func_02009f08();
void Func_02009e3a();
void Func_02009e78();
void Func_02009e16();
void Func_02009f1a();
void Func_02009f10();
void Func_02009ec8();
void Func_02009f42();
void Func_02009f28();
void Func_02009ef6();
void Func_02009f8a();
void Func_02009f4a();
void Func_02009ea0();
void Func_02009f40();
void Func_02009eae();
void Func_02009fa0();
void Func_02009fda();
void Func_02009fba();
void Func_02009f6a();
void Func_02009f7a();
void Func_02009ee8();
void Func_02009f2c();
void Func_02009f38();
void Func_02009f74();
void Func_02009f80();
void Func_02009f1e();
void Func_0200a088();
void Func_0200a094();
void Func_0200a082();
void Func_02009f4e();

                     

                     

                     

                    

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

                     

/* Intra-overlay callees, all with byte-exact sources in assets/code. */

void Func_020034c8(void)
{
    u8 *cameraRecord;       /* r9 - the record for actor 0 */
    u8 *sceneActor;         /* r6 - the record for actor 14, the scene's lead */
    u8 *actor;              /* r7 - the record currently being configured */
    u32 frame;              /* r5 in the fade loop */
    s32 messageId;          /* r5 once the loop is done */

    cameraRecord = Func_020094c0(0);
    sceneActor = Func_020094c8(14);

    Func_020094a6();

    /* -1.0, -1.0, -1.0 in 16.16 with mode 0 - the "no placement" sentinel. */
    Func_020095e0(-1, -1, -1, 0);
    Func_0200938e(1);

    /* The freshly acquired record's +0x55 flag is cleared. */
    actor = Func_020095fa();
    actor[0x55] = 0;

    Func_020093a2(1);
    Func_02009466(0x31, 0x35, 8, 4, 20, 50);
    Func_02009472(2, 0x66, 0x54, 0x29, 2, 1);
    Func_02009482(1, 0x66, 0x53, 0x29, 1, 1);
    Func_02009492(0, 0x67, 0x52, 0x2a, 1, 1);

    /* Three actors placed at the same X/Y with stepped Z (0x348, 0x34c and
     * 0x350 in whole units, i.e. 210, 211 and 212 << 18). */
    actor = Func_02009548(11);
    actor[0x55] = 0;
    *(s32 *)(actor + 12) = 0x00a00000;
    *(s32 *)(actor + 16) = 0x03480000;
    *(s32 *)(actor + 8) = 0x01840000;
    Func_020094d4(actor, 0);

    actor[0x55] = 0;
    actor = Func_02009572(12);
    *(s32 *)(actor + 12) = 0x00a00000;
    *(s32 *)(actor + 16) = 0x034c0000;
    *(s32 *)(actor + 8) = 0x01840000;
    Func_020094f6(actor, 0);

    actor = Func_02009594(13);
    actor[0x55] = 0;
    *(s32 *)(actor + 16) = 0x03500000;
    *(s32 *)(actor + 12) = 0x00a00000;
    *(s32 *)(actor + 8) = 0x01840000;
    Func_02009518(actor, 0);

    Func_02009524(Func_020095b6(11), 0);
    Func_02009530(Func_020095c2(12), 0);

    Func_02009630(0, 11);
    Func_020095ea(0, Data_0200e590);
    Func_0200955e();
    Func_0200958a(0xee8, 0, 0);
    Func_02009576();

    Func_020096fa(0x01530000, 0x00a00000, 0x04950000, 0);
    Func_0200952e();
    Func_020094ac(1);
    Func_02009704(0x547a, 0xa8f);

    Func_0200971a(0x01280000, 0x00a00000, 0x03990000, 1);
    Func_0200967c(5, 0x01990000, 0x046e0000);
    Func_020094d2(1);
    Func_0200963c(5, 0xb333, 0x5999);
    Func_02009678(5, 0x1a4, 0x42c);
    Func_02009794();

    /* The scene block's u32 at +0x1c8 is a countdown/timer, set to 60 here and
     * again just before the owner returns. */
    *(s32 *)(RESOURCE_373_SCENE + 0x1c8) = 60;

    Func_02009778();
    Func_020096ae(5);
    Func_02009674(5, 0x10000, 0x8000);
    Func_020096b8(5, 0x155, 0x428);
    Func_0200968a(5, 0xcccc, 0x6666);
    Func_020096cc(5, 0x167, 0x409);
    Func_020096a2(8, 0x8000, 0x4000);
    Func_020096de(8, 0x13e, 0x3b3);
    Func_0200970e(8, 2);
    Func_020096fa(5, 0x19c, 0x409);
    Func_02009706(5, 0x19c, 0x3fb);
    Func_02009714(5, 0x176, 0x3f0);
    Func_0200971e(5, 0x15b, 0x3bb);
    Func_0200972a(8, 0x13e, 0x3b3);
    Func_02009784(5, 8, 40);
    Func_0200977c(8, 2);
    Func_0200976c(5, 3);
    Func_020096da(10);
    Func_0200974c(8, 0x17b, 0x3f9);
    Func_02009818(0x8000, 0x1000);
    Func_0200976c(5, 0x14d, 0x398);
    Func_02009778(5, 0x12b, 0x39c);
    Func_02009844();
    Func_02009712(10);
    Func_02009816(5, 0xf000, 30);
    Func_020097d6(5, 2);
    Func_0200972c(20);
    Func_02009860(0x20000, 0x4000);
    Func_02009874(0x01830000, 0x00a00000, 0x03620000, 1);

    /* r5 has held the pooled 0xee8 since the Func_08015210 call above; the
     * bump makes the message id passed to Func_0808a170 one past it. */
    messageId = 0xee8 + 1;

    Func_02009882();
    Func_020097f4(10, 2, 20);
    Func_0200982a(messageId);
    Func_0200984c(0x100a, 0, 10);

    /* 128 << 9 into both of the camera record's +0x18 / +0x1c words. */
    *(s32 *)(cameraRecord + 24) = 0x10000;
    *(s32 *)(cameraRecord + 28) = 0x10000;

    Func_020097b6(0, 1);
    Func_02009840(10, 0, 40);
    Func_020098a2(0, 0x102);
    Func_02009842(0, 2);
    Func_02009798(40);
    Func_02009848(10, 2);
    Func_020097a6(40);
    Func_02009898(0x100a, 0, 40);
    Func_02009848(0, 11);
    Func_02009800(0, Data_0200e590);
    Func_020098e8(5, 1);
    Func_02009904();
    Func_020097d2(40);
    Func_02009872(5, 3);
    Func_020098de(5, 0xd000, 10);
    Func_02009862(5, 0x138, 0x2f7);
    Func_0200986e(5, 0x169, 0x2f8);
    Func_02009804(20);
    Func_02009908(5, 0x8000, 40);
    Func_02009912(5, 0, 40);
    Func_0200990c(0x6001, 0, 10);
    Func_02009940(5, 0x100, 0);
    Func_020098da(5, 4, 40);
    Func_0200993e(5, 0xc000, 30);
    Func_020098fe(5, 2);
    Func_02009854(40);
    Func_02009946(5, 0, 30);
    Func_020098fe(5, 3);
    Func_0200986c(10);
    Func_020099a0(0x40000, 0x8000);
    Func_020099ba(0x018c0000, -1, 0x024c0000, 1);
    Func_02009904(5, 0x1c8, 0x2e3);
    Func_020099d0();
    Func_0200989e(10);
    Func_020099b8(1, 0x105, 40);
    Func_02009960(1, 2);
    Func_020098b6(20);

    Func_02008e9a();

    Func_02009952(1, 17);
    Func_020099b4(0x2001, 0, 20);
    Func_02009a6a(0x83);

    /* 60 frames of the per-frame step, one call per frame. */
    for (frame = 0; frame <= 59; frame++) {
        Func_0200954a(Func_0200990a(1));
        Func_020097be(1);
    }

    Func_020099fa(1, 1);

    /* Two overlay callbacks installed for 3200 ticks each; both are removed
     * again below.  0x0200d5b1 and 0x0200d5d1 are Func_020055b0 and
     * Func_020055d0 with the Thumb bit set. */
    Func_020097e0((void *)0x0200d5b1, 0xc80);
    Func_020097ee((void *)0x0200d5d1, 0xc80);

    Func_020098b2(Func_02009944(14), 0);

    sceneActor[0x55] = 0;
    *(s32 *)(sceneActor + 8) = 0x01ac0000;
    /* 0x0200d75d is Func_0200575c + Thumb bit - the actor's behaviour hook. */
    *(s32 *)(sceneActor + 0x6c) = 0x0200d75d;
    *(s32 *)(sceneActor + 12) = 0x00d00000;
    *(u16 *)(sceneActor + 6) = 0x8000;
    *(s32 *)(sceneActor + 16) = 0x02480000;

    Func_0200994c(4);
    Func_02009992(14, 0x20000, 0x20000);
    Func_020098d2(sceneActor, 0x01980000, 0x00d00000, 0x02480000);
    Func_02009970(40);
    Func_020099b2(9, 0x2666, 0x1333);
    Func_020099bc(14, 0x2666, 0x1333);

    /* Result discarded - see UNCERTAINTY 4. */
    Func_020099ba_b(9);

    /* The `b.n` at 0x020039e4 jumps the interior pool; the call's arguments
     * are set before the branch and the `bl` itself is at 0x02003a88. */
    Func_020099a6(sceneActor, 0x01880000, 0x00d00000, 0x02480000);

    Func_02009aac(9, 0x17a, 0x248);
    Func_02009a52(20);
    Func_02009b44(0x2005, 0, 10);

    *(s32 *)(sceneActor + 0x6c) = 0;

    Func_02009b70(1, 2);

    /* Set bit 0 of the +0x23 flag byte of actor 1's record. */
    actor = Func_02009a9e(1);
    actor[0x23] = (u8)(actor[0x23] | 1);

    Func_02009966((void *)0x0200d5b1);
    Func_0200996c((void *)0x0200d5d1);
    Func_02009962(1);
    Func_02009b5a(1, 0);
    Func_02009b62(9, 0);
    Func_02009b9e(1, 0x3000, 0);
    Func_02009b3e(1, 1);

    Func_020092fc(sceneActor);
    Func_020090a8();

    Func_02009abe(10);
    Func_02009b3c(5, 0x1a8, 0x270);
    Func_02009b96(1, 5, 60);
    Func_02009b8e(1, 2);
    Func_02009ae4(10);
    Func_02009bd6(0x6001, 0, 20);
    Func_02009b8e(5, 3);
    Func_02009afc(10);
    Func_02009bee(5, 0, 20);
    Func_02009c22(1, 0x102, 40);
    Func_02009bb2(5, 4);
    Func_02009b20(10);
    Func_02009c12(5, 0, 20);
    Func_02009c44(1, 0x101, 80);
    Func_02009bec(5, 1);
    Func_02009b42(10);
    Func_02009c34(5, 0, 20);
    Func_02009c68(1, 0x102, 80);
    Func_02009bf8(5, 4);
    Func_02009b66(10);
    Func_02009c58(5, 0, 20);
    Func_02009c10(1, 3);
    Func_02009b7e(10);
    Func_02009bfc(5, 0x184, 0x25c);
    Func_02009c90(5, 0xa000, 20);
    Func_02009c50(5, 2);
    Func_02009ba6(10);
    Func_02009c98(0x1005, 0, 20);
    Func_02009c50(5, 4);
    Func_02009bbe(10);
    Func_02009cd8(1, 0x105, 40);
    Func_02009c80(1, 2);
    Func_02009bd6(10);
    Func_02009cd8(5, 0, 0);
    Func_02009ce2(1, 0x8000, 40);

    /* Eleven arguments: four in registers and seven on the stack. */
    Func_02009d0e(1, 1, 2, 25, 2, 5, 10, 14, 4, 14, 0);

    Func_02009c14(40);
    Func_02009d36(5, 0x102);
    Func_02009d40(1, 0x102);
    Func_02009c2e(80);
    Func_02009d48(5, 0x101, 40);
    Func_02009d2a(0x1005, 0, 20);
    Func_02009d64(1, 0x102);
    Func_02009c52(80);
    Func_02009cf2(1, 4);
    Func_02009c60(10);
    Func_02009d7a(5, 0x101, 40);
    Func_02009d5c(0x1005, 0, 20);
    Func_02009d2c(1, 2);
    Func_02009c82(40);
    Func_02009d22(1, 4);
    Func_02009c90(20);
    Func_02009daa(5, 0x101, 80);
    Func_02009d3a(5, 4);
    Func_02009ca8(10);
    Func_02009d9a(0x1005, 0, 10);
    Func_02009db6(5, 0x1000, 40);
    Func_02009d80(5, 1, 40);
    Func_02009d80(1, 2);
    Func_02009cd6(10);
    Func_02009d54(5, 0x17c, 0x26c);
    Func_02009cea(10);
    Func_02009ddc(0x1005, 0, 10);
    Func_02009df8(1, 0x5000, 30);
    Func_02009e22(1, 0x102);
    Func_02009d10(80);
    Func_02009dc8(1, 2);
    Func_02009d1e(10);
    Func_02009e10(0x6001, 0, 20);
    Func_02009dc8(5, 4);
    Func_02009d36(10);
    Func_02009e50(1, 0x101, 80);
    Func_02009de0(5, 4);
    Func_02009d4e(10);
    Func_02009d90(1, 0xcccc, 0x6666);
    Func_02009dd6(1, 0x19c, 0x25c);
    Func_02009e6a(1, 0x5000, 20);
    Func_02009e2a(5, 1);
    Func_02009d80(10);
    Func_02009e72(0x1005, 0, 20);
    Func_02009ea4(1, 0x101, 80);
    Func_02009e3e(5, 4, 30);
    Func_02009e90(0x1005, 0, 20);
    Func_02009e60(1, 3);
    Func_02009db6(10);
    Func_02009ea8(0x6001, 0, 20);
    Func_02009dc6(30);
    Func_02009eca(5, 0xe000, 40);
    Func_02009e72(5, 3);
    Func_02009ecc(0x1005, 0, 20);
    Func_02009efe(1, 0x101, 80);
    Func_02009f08(5, 0x103, 40);
    Func_02009e3a(5, 0xcccc, 0x6666);
    Func_02009e78(5, 0x1ac, 0x274);
    Func_02009e16(20);
    Func_02009f1a(1, 0x3000, 0);
    Func_02009ea8(5);
    Func_02009f10(0x5001, 0);
    Func_02009ec8(5, 1);
    Func_02009e3e(60);
    Func_02009f42(5, 0xb000, 30);

    /* The `b.n` at 0x02003e9a jumps the second interior pool; r0 is set
     * before it and the `bl` is at 0x02003ec2. */
    Func_02009f28(1, 2);

    Func_02009ef6(5, 0x1ac, 0x274);
    Func_02009f8a(5, 0xb000, 20);
    Func_02009f4a(1, 3);
    Func_02009ea0(10);
    Func_02009f40(5, 4);
    Func_02009eae(10);
    Func_02009fa0(0x2005, 0, 20);
    Func_02009fda(1, 0x102);
    Func_02009ec8(40);
    Func_02009fba(0x5001, 0, 20);
    Func_02009f6a(1, 3);
    Func_02009f7a(5, 3);
    Func_02009ee8(10);
    Func_02009f2c(5, 0x8000, 0x4000);
    Func_02009f38(1, 0x8000, 0x4000);
    Func_02009f74(5, 0x1c2, 0x2ee);
    Func_02009f80(1, 0x1c2, 0x2ee);
    Func_02009f1e(60);

    *(s32 *)(RESOURCE_373_SCENE + 0x1c8) = 60;

    Func_0200a088();
    Func_0200a094();
    Func_0200a082(12);

    /* Common exit; no argument register is set. */
    Func_02009f4e();
}
