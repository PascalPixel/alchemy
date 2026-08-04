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
 * `bun tools/overlay_call_targets.ts resource_373 34c8` - 53 distinct targets,
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
void Func_02005594();
void Func_020055a4();
void Func_020057fc();
void Func_02005c20();
void Func_080000c0();
void Func_080000d0();
void Func_080000d8();
void Func_08009128();
void Func_08009150();
void Func_08009180();
void Func_080091c0();
u8 *Func_080091e0();
void Func_08009208();
void Func_08009210();
void Func_08015210();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0c0();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a130();
void Func_0808a138();
void Func_0808a148();
void Func_0808a150();
void Func_0808a158();
void Func_0808a170();
void Func_0808a180();
void Func_0808a188();
void Func_0808a1b8();
void Func_0808a1d0();
void Func_0808a1e0();
void Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a200();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
u8 *Func_0808a228();
void Func_0808a248();
void Func_0808a360();
void Func_0808a368();
void Func_0808a370();
void Func_0808a4f0();
void Func_080f9010();

/* Intra-overlay callees, all with byte-exact sources in assets/code. */

void Func_020034c8(void)
{
    u8 *cameraRecord;       /* r9 - the record for actor 0 */
    u8 *sceneActor;         /* r6 - the record for actor 14, the scene's lead */
    u8 *actor;              /* r7 - the record currently being configured */
    u32 frame;              /* r5 in the fade loop */
    s32 messageId;          /* r5 once the loop is done */

    cameraRecord = Func_0808a080(0);
    sceneActor = Func_0808a080(14);

    Func_0808a018();

    /* -1.0, -1.0, -1.0 in 16.16 with mode 0 - the "no placement" sentinel. */
    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);

    /* The freshly acquired record's +0x55 flag is cleared. */
    actor = Func_0808a228();
    actor[0x55] = 0;

    Func_080000c0(1);
    Func_080091c0(0x31, 0x35, 8, 4, 20, 50);
    Func_08009180(2, 0x66, 0x54, 0x29, 2, 1);
    Func_08009180(1, 0x66, 0x53, 0x29, 1, 1);
    Func_08009180(0, 0x67, 0x52, 0x2a, 1, 1);

    /* Three actors placed at the same X/Y with stepped Z (0x348, 0x34c and
     * 0x350 in whole units, i.e. 210, 211 and 212 << 18). */
    actor = Func_0808a080(11);
    actor[0x55] = 0;
    *(s32 *)(actor + 12) = 0x00a00000;
    *(s32 *)(actor + 16) = 0x03480000;
    *(s32 *)(actor + 8) = 0x01840000;
    Func_080091e0(actor, 0);

    actor = Func_0808a080(12);
    actor[0x55] = 0;
    *(s32 *)(actor + 12) = 0x00a00000;
    *(s32 *)(actor + 16) = 0x034c0000;
    *(s32 *)(actor + 8) = 0x01840000;
    Func_080091e0(actor, 0);

    actor = Func_0808a080(13);
    actor[0x55] = 0;
    *(s32 *)(actor + 16) = 0x03500000;
    *(s32 *)(actor + 12) = 0x00a00000;
    *(s32 *)(actor + 8) = 0x01840000;
    Func_080091e0(actor, 0);

    Func_080091e0(Func_0808a080(11), 0);
    Func_080091e0(Func_0808a080(12), 0);

    Func_0808a100(0, 11);
    Func_0808a098(0, Data_0200e590);
    Func_08009208();
    Func_08015210(0xee8, 0, 0);
    Func_08009210();

    Func_0808a210(0x01530000, 0x00a00000, 0x04950000, 0);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a208(0x547a, 0xa8f);

    Func_0808a210(0x01280000, 0x00a00000, 0x03990000, 1);
    Func_0808a0f0(5, 0x01990000, 0x046e0000);
    Func_080000c0(1);
    Func_0808a090(5, 0xb333, 0x5999);
    Func_0808a0c8(5, 0x1a4, 0x42c);
    Func_0808a4f0();

    /* The scene block's u32 at +0x1c8 is a countdown/timer, set to 60 here and
     * again just before the owner returns. */
    *(s32 *)(RESOURCE_373_SCENE + 0x1c8) = 60;

    Func_0808a360();
    Func_0808a0e8(5);
    Func_0808a090(5, 0x10000, 0x8000);
    Func_0808a0d0(5, 0x155, 0x428);
    Func_0808a090(5, 0xcccc, 0x6666);
    Func_0808a0d0(5, 0x167, 0x409);
    Func_0808a090(8, 0x8000, 0x4000);
    Func_0808a0c8(8, 0x13e, 0x3b3);
    Func_0808a100(8, 2);
    Func_0808a0d0(5, 0x19c, 0x409);
    Func_0808a0d0(5, 0x19c, 0x3fb);
    Func_0808a0d0(5, 0x176, 0x3f0);
    Func_0808a0d0(5, 0x15b, 0x3bb);
    Func_0808a0d0(8, 0x13e, 0x3b3);
    Func_0808a150(5, 8, 40);
    Func_0808a138(8, 2);
    Func_0808a110(5, 3);
    Func_0808a010(10);
    Func_0808a0c8(8, 0x17b, 0x3f9);
    Func_0808a208(0x8000, 0x1000);
    Func_0808a0d0(5, 0x14d, 0x398);
    Func_0808a0d0(5, 0x12b, 0x39c);
    Func_0808a218();
    Func_0808a010(10);
    Func_0808a1b8(5, 0xf000, 30);
    Func_0808a138(5, 2);
    Func_0808a010(20);
    Func_0808a208(0x20000, 0x4000);
    Func_0808a210(0x01830000, 0x00a00000, 0x03620000, 1);

    /* r5 has held the pooled 0xee8 since the Func_08015210 call above; the
     * bump makes the message id passed to Func_0808a170 one past it. */
    messageId = 0xee8 + 1;

    Func_0808a218();
    Func_0808a128(10, 2, 20);
    Func_0808a170(messageId);
    Func_0808a188(0x100a, 0, 10);

    /* 128 << 9 into both of the camera record's +0x18 / +0x1c words. */
    *(s32 *)(cameraRecord + 24) = 0x10000;
    *(s32 *)(cameraRecord + 28) = 0x10000;

    Func_0808a098(0, 1);
    Func_0808a150(10, 0, 40);
    Func_0808a1f0(0, 0x102);
    Func_0808a138(0, 2);
    Func_0808a010(40);
    Func_0808a130(10, 2);
    Func_0808a010(40);
    Func_0808a188(0x100a, 0, 40);
    Func_0808a100(0, 11);
    Func_0808a098(0, Data_0200e590);
    Func_0808a200(5, 1);
    Func_0808a218();
    Func_0808a010(40);
    Func_0808a110(5, 3);
    Func_0808a1b8(5, 0xd000, 10);
    Func_0808a0d0(5, 0x138, 0x2f7);
    Func_0808a0d0(5, 0x169, 0x2f8);
    Func_0808a010(20);
    Func_0808a1b8(5, 0x8000, 40);
    Func_0808a1b8(5, 0, 40);
    Func_0808a188(0x6001, 0, 10);
    Func_0808a1e8(5, 0x100, 0);
    Func_0808a128(5, 4, 40);
    Func_0808a1b8(5, 0xc000, 30);
    Func_0808a138(5, 2);
    Func_0808a010(40);
    Func_0808a188(5, 0, 30);
    Func_0808a110(5, 3);
    Func_0808a010(10);
    Func_0808a208(0x40000, 0x8000);
    Func_0808a210(0x018c0000, -1, 0x024c0000, 1);
    Func_0808a0d0(5, 0x1c8, 0x2e3);
    Func_0808a218();
    Func_0808a010(10);
    Func_0808a1e8(1, 0x105, 40);
    Func_0808a138(1, 2);
    Func_0808a010(20);

    Func_02005594();

    Func_0808a100(1, 17);
    Func_0808a188(0x2001, 0, 20);
    Func_080f9010(0x83);

    /* 60 frames of the per-frame step, one call per frame. */
    for (frame = 0; frame <= 59; frame++) {
        Func_02005c20(Func_0808a080(1));
        Func_080000c0(1);
    }

    Func_0808a1e0(1, 1);

    /* Two overlay callbacks installed for 3200 ticks each; both are removed
     * again below.  0x0200d5b1 and 0x0200d5d1 are Func_020055b0 and
     * Func_020055d0 with the Thumb bit set. */
    Func_080000d0((void *)0x0200d5b1, 0xc80);
    Func_080000d0((void *)0x0200d5d1, 0xc80);

    Func_080091e0(Func_0808a080(14), 0);

    sceneActor[0x55] = 0;
    *(s32 *)(sceneActor + 8) = 0x01ac0000;
    /* 0x0200d75d is Func_0200575c + Thumb bit - the actor's behaviour hook. */
    *(s32 *)(sceneActor + 0x6c) = 0x0200d75d;
    *(s32 *)(sceneActor + 12) = 0x00d00000;
    *(u16 *)(sceneActor + 6) = 0x8000;
    *(s32 *)(sceneActor + 16) = 0x02480000;

    Func_0808a010(4);
    Func_0808a090(14, 0x20000, 0x20000);
    Func_08009150(sceneActor, 0x01980000, 0x00d00000, 0x02480000);
    Func_0808a010(40);
    Func_0808a090(9, 0x2666, 0x1333);
    Func_0808a090(14, 0x2666, 0x1333);

    /* Result discarded - see UNCERTAINTY 4. */
    Func_0808a080(9);

    /* The `b.n` at 0x020039e4 jumps the interior pool; the call's arguments
     * are set before the branch and the `bl` itself is at 0x02003a88. */
    Func_08009150(sceneActor, 0x01880000, 0x00d00000, 0x02480000);

    Func_0808a0c0(9, 0x17a, 0x248);
    Func_0808a010(20);
    Func_0808a188(0x2005, 0, 10);

    *(s32 *)(sceneActor + 0x6c) = 0;

    Func_0808a1e0(1, 2);

    /* Set bit 0 of the +0x23 flag byte of actor 1's record. */
    actor = Func_0808a080(1);
    actor[0x23] = (u8)(actor[0x23] | 1);

    Func_080000d8((void *)0x0200d5b1);
    Func_080000d8((void *)0x0200d5d1);
    Func_080000c0(1);
    Func_0808a158(1, 0);
    Func_0808a158(9, 0);
    Func_0808a1b8(1, 0x3000, 0);
    Func_0808a100(1, 1);

    Func_020057fc(sceneActor);
    Func_020055a4();

    Func_0808a010(10);
    Func_0808a0d0(5, 0x1a8, 0x270);
    Func_0808a150(1, 5, 60);
    Func_0808a138(1, 2);
    Func_0808a010(10);
    Func_0808a188(0x6001, 0, 20);
    Func_0808a110(5, 3);
    Func_0808a010(10);
    Func_0808a188(5, 0, 20);
    Func_0808a1e8(1, 0x102, 40);
    Func_0808a110(5, 4);
    Func_0808a010(10);
    Func_0808a188(5, 0, 20);
    Func_0808a1e8(1, 0x101, 80);
    Func_0808a138(5, 1);
    Func_0808a010(10);
    Func_0808a188(5, 0, 20);
    Func_0808a1e8(1, 0x102, 80);
    Func_0808a110(5, 4);
    Func_0808a010(10);
    Func_0808a188(5, 0, 20);
    Func_0808a110(1, 3);
    Func_0808a010(10);
    Func_0808a0d0(5, 0x184, 0x25c);
    Func_0808a1b8(5, 0xa000, 20);
    Func_0808a138(5, 2);
    Func_0808a010(10);
    Func_0808a188(0x1005, 0, 20);
    Func_0808a110(5, 4);
    Func_0808a010(10);
    Func_0808a1e8(1, 0x105, 40);
    Func_0808a138(1, 2);
    Func_0808a010(10);
    Func_0808a1b8(5, 0, 0);
    Func_0808a1b8(1, 0x8000, 40);

    /* Eleven arguments: four in registers and seven on the stack. */
    Func_0808a1d0(1, 1, 2, 25, 2, 5, 10, 14, 4, 14, 0);

    Func_0808a010(40);
    Func_0808a1f0(5, 0x102);
    Func_0808a1f0(1, 0x102);
    Func_0808a010(80);
    Func_0808a1e8(5, 0x101, 40);
    Func_0808a188(0x1005, 0, 20);
    Func_0808a1f0(1, 0x102);
    Func_0808a010(80);
    Func_0808a110(1, 4);
    Func_0808a010(10);
    Func_0808a1e8(5, 0x101, 40);
    Func_0808a188(0x1005, 0, 20);
    Func_0808a138(1, 2);
    Func_0808a010(40);
    Func_0808a110(1, 4);
    Func_0808a010(20);
    Func_0808a1e8(5, 0x101, 80);
    Func_0808a110(5, 4);
    Func_0808a010(10);
    Func_0808a188(0x1005, 0, 10);
    Func_0808a1b8(5, 0x1000, 40);
    Func_0808a148(5, 1, 40);
    Func_0808a138(1, 2);
    Func_0808a010(10);
    Func_0808a0d0(5, 0x17c, 0x26c);
    Func_0808a010(10);
    Func_0808a188(0x1005, 0, 10);
    Func_0808a1b8(1, 0x5000, 30);
    Func_0808a1f0(1, 0x102);
    Func_0808a010(80);
    Func_0808a138(1, 2);
    Func_0808a010(10);
    Func_0808a188(0x6001, 0, 20);
    Func_0808a110(5, 4);
    Func_0808a010(10);
    Func_0808a1e8(1, 0x101, 80);
    Func_0808a110(5, 4);
    Func_0808a010(10);
    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a0d0(1, 0x19c, 0x25c);
    Func_0808a1b8(1, 0x5000, 20);
    Func_0808a138(5, 1);
    Func_0808a010(10);
    Func_0808a188(0x1005, 0, 20);
    Func_0808a1e8(1, 0x101, 80);
    Func_0808a128(5, 4, 30);
    Func_0808a188(0x1005, 0, 20);
    Func_0808a138(1, 3);
    Func_0808a010(10);
    Func_0808a188(0x6001, 0, 20);
    Func_0808a010(30);
    Func_0808a1b8(5, 0xe000, 40);
    Func_0808a110(5, 3);
    Func_0808a188(0x1005, 0, 20);
    Func_0808a1e8(1, 0x101, 80);
    Func_0808a1e8(5, 0x103, 40);
    Func_0808a090(5, 0xcccc, 0x6666);
    Func_0808a0c8(5, 0x1ac, 0x274);
    Func_0808a010(20);
    Func_0808a1b8(1, 0x3000, 0);
    Func_0808a0e8(5);
    Func_0808a180(0x5001, 0);
    Func_0808a100(5, 1);
    Func_0808a010(60);
    Func_0808a1b8(5, 0xb000, 30);

    /* The `b.n` at 0x02003e9a jumps the second interior pool; r0 is set
     * before it and the `bl` is at 0x02003ec2. */
    Func_0808a138(1, 2);

    Func_0808a0d0(5, 0x1ac, 0x274);
    Func_0808a1b8(5, 0xb000, 20);
    Func_0808a138(1, 3);
    Func_0808a010(10);
    Func_0808a110(5, 4);
    Func_0808a010(10);
    Func_0808a188(0x2005, 0, 20);
    Func_0808a1f0(1, 0x102);
    Func_0808a010(40);
    Func_0808a188(0x5001, 0, 20);
    Func_0808a100(1, 3);
    Func_0808a110(5, 3);
    Func_0808a010(10);
    Func_0808a090(5, 0x8000, 0x4000);
    Func_0808a090(1, 0x8000, 0x4000);
    Func_0808a0c8(5, 0x1c2, 0x2ee);
    Func_0808a0c8(1, 0x1c2, 0x2ee);
    Func_0808a010(60);

    *(s32 *)(RESOURCE_373_SCENE + 0x1c8) = 60;

    Func_0808a368();
    Func_0808a370();
    Func_0808a248(12);

    /* Common exit; no argument register is set. */
    Func_0808a020();
}
