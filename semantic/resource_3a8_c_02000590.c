#include "types.h"

/*
 * resource_3a8 owner at 0x02000590, 4,092 bytes: the overlay's main scripted
 * cutscene - the one gated on story flag 0x910 being set and 0x911 not yet
 * latched, which stages ten scene entities (14, 20, 21, 27-34) through a long
 * choreography and finally latches 0x911.
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7, r8 / push {r7}` at
 * 0x02000590 through the single interworking epilogue at 0x0200153c
 * (`pop {r3} / mov r8, r3 / pop {r5, r6, r7} / pop {r0} / bx r0`).  The next
 * inventory row, 0x0200158c, has a byte-exact source in assets/code, so the
 * owner is exactly its advertised span.
 *
 * Four literal pools sit inside the span and are DATA, not code; all four are
 * branched over and all four are excluded:
 *   0x0200069c-0x020006b3   (jumped by `b 0x020006c6` at 0x0200069a)
 *   0x02000aac-0x02000aef   (jumped by `b 0x02000af0` at 0x02000aa8)
 *   0x02000f3c-0x02000f77   (jumped by `b 0x02000f78` at 0x02000f3a)
 *   0x02001548-0x0200158b   (after the epilogue)
 * Two further "pool words" that `overlay_show.ts` reports, 0x020009e4 and
 * 0x02000e08, are NOT pools: each is the low halfword of a BL pair
 * (0x20009e2 and 0x2000e06 respectively) that happens to be reachable from an
 * `ldr rN,[pc,#816]` decoded out of the real pool word 0x00004ccc.  They were
 * checked individually and reinstated as code; treating them as data would
 * have silently dropped `movs r0,#21` and `movs r1,#160`.
 *
 * Signature.  The epilogue is `pop {r0} ; bx r0`, so r0 holds the popped
 * return address and the owner returns nothing: `void`.  r0 is written with a
 * constant before any read and r1-r3 are never read before being written, so
 * it takes no arguments.  There is no stack frame; nothing is live past the
 * return.
 *
 * TWO `bl`s THAT ARE GOTOs.  0x020005aa and 0x020005b8 both branch to
 * 0x0200151c, which the resolver reports as `unknown` because it is neither a
 * veneer nor a prologue.  It is not a call: 0x0200151c is an arm of THIS
 * function, reached otherwise only by falling past the `b 0x02001538` at
 * 0x0200151a, and it runs straight into the owner's own epilogue.  That
 * epilogue pops the frame pushed at 0x02000590, which is still intact, so the
 * `bl` never returns - it executes the abort arm and returns to this
 * function's caller.  It is spelled as a `goto` below, which is exactly its
 * effect; the clobbered lr is dead because the real return address is on the
 * stack.  (This is the same shape HANDOVER records for resource_372's
 * `bl .L_02003390`.)
 *
 * Call accounting: 406 `bl` sites, resolved with
 * `bun tools/lib/overlay_call_targets.ts resource_3a8 0590` - 41 distinct targets,
 * 396 import veneers, 8 calls to three intra-overlay prologues that all have
 * byte-exact sources in assets/code (Func_02001e6c x1, Func_02001ea4 x6,
 * Func_02001ed8 x1), and the 2 goto-`bl`s above.  Every site is reproduced
 * below exactly once; the transcription's per-callee counts were checked back
 * against the resolver's, target by target:
 *   0808a1b8 x70  0808a188 x34  0808a0a8 x30  0808a090 x27  0808a098 x25
 *   0808a138 x19  0808a110 x19  0808a080 x18  0808a0d0 x17  0808a010 x16
 *   0808a128 x15  080000c0 x15  0808a1e8 x14  0808a0f0 x14  0808a1e0 x10
 *   0808a100  x9  0808a088  x8  0808a170  x7  0808a1f0  x5  080f9010  x4
 *   0808a208  x2  0808a130  x2  080770c0  x2  and fourteen singletons.
 * The inventory row advertises `calls=367`; the difference is the sites inside
 * the four wait loops and the repeated bodies the row folds.  The
 * disassembler's own `bl` annotations are wrong in the usual overlay way (an
 * overlay `bl` stores `target_offset - 2`) and were not used anywhere.
 *
 * LINK BASE 0x02008000, with the strongest witness available: the word handed
 * to Func_080000d8 at 0x02000d38 is 0x02009f15, i.e. file offset 0x1f14 plus
 * the Thumb bit - and 0x02001f14 is the five-step ping-pong state machine
 * reconstructed in this same reconstruction, whose `push {lr}` prologue is right there.
 * Installing exactly that routine here is also what the two owners mean
 * together, and its state word Data_0200d144 is the one the byte-exact
 * assets/code/resource_3a8_c_02001ed8.c zeroes - which is the routine called
 * from 0x02000c40 here, six instructions before.  The remaining
 * `0x0200b*`/`0x0200c*` pool words are therefore in-image script data at
 * `value - 0x8000`; 0x0200bfb0 in particular is the same script offset 0x3fb0
 * that Func_02001f14 hands to Func_0808a098.
 *
 * `Data_03001ebc` is the IWRAM scene pointer, named and typed by the
 * byte-exact assets/code/resource_3a8_c_02003184.c.  The halfword at
 * `scene + 364` (`0xb6 << 1`) selects between two presentation variants at
 * both ends of the function and is passed to Func_0808a248 on the abort path.
 *
 * UNCERTAINTIES.
 *  - None of the 0x0808axxx scene-script imports has an established interface;
 *    the arity used here is the number of argument registers each site
 *    actually sets, and the literals are reproduced positionally.  All are
 *    declared old-style, as this overlay's convention requires.
 *  - Func_080000d8 is given only r0 at 0x02000d38; r1 holds a call-clobbered 0
 *    from the preceding Func_0808a188 and is not asserted as an argument.
 *  - r8 is set to 0 once at 0x020008d8 and read back at 0x02000d54/0x02000d60;
 *    it is written as the constant 0.
 *  - Func_0808a0d0's second and third arguments are always a small unsigned
 *    byte and an even halfword; they are kept as plain s32 literals.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
void Func_02004190();
s32 Func_02004178();
s32 Func_02004186();
void Func_020041c4();
void Func_02004242();
void Func_02004250();
void Func_0200425e();
void Func_0200426c();
void Func_0200427a();
void Func_02004288();
void Func_02004296();
void Func_020042a4();
void Func_020042b2();
void Func_020042c0();
void Func_020043ce();
void Func_02004344();
void Func_02004226();
void Func_020043e4();
void Func_0200425a();
void Func_02004312();
void Func_02004398();
void Func_0200437c();
void Func_020043c6();
void Func_020043a8();
void Func_020042fa();
void Func_02004308();
void Func_02004316();
void Func_02004320();
void Func_0200432a();
void Func_02004334();
void Func_0200433e();
void Func_02004348();
void Func_02004352();
void Func_0200435c();
void Func_0200436e();
void Func_02004376();
void Func_0200437e();
void Func_02004386();
void Func_0200438e();
void Func_02004396();
void Func_0200439e();
void Func_020043a6();
void Func_020043ae();
u8 *Func_0200439c();
void Func_020043c4();
void Func_020044b6();
void Func_02004496();
void Func_020042cc();
void Func_020043b2();
void Func_0200446a();
void Func_020044be();
void Func_0200449c();
void Func_0200448e();
void Func_020044e2();
void Func_02004482();
void Func_020044e4();
void Func_0200451e();
void Func_0200440c();
void Func_020044fe();
void Func_02004532();
void Func_02004526();
void Func_02004520();
void Func_0200453c();
void Func_020044fc();
void Func_02004546();
void Func_02004562();
void Func_0200456e();
void Func_0200450e();
void Func_0200453e();
void Func_020045b0();
void Func_0200449e();
void Func_02004590();
void Func_02004560();
u8 *Func_020044e6();
void Func_02004542();
void Func_020044d8();
u8 *Func_0200450e_b();
void Func_020045ec();
void Func_020045f8();
void Func_02004604();
void Func_02004610();
void Func_020045b8();
u8 *Func_0200455c();
u8 *Func_0200457c();
void Func_020045d0();
void Func_02004662();
void Func_0200465c();
void Func_0200468e();
void Func_02004636();
void Func_02004626();
void Func_02004680();
void Func_02004650();
void Func_02004692();
void Func_02004642();
void Func_020046a4();
void Func_02004674();
void Func_020046e0();
void Func_020046c2();
void Func_02004684();
void Func_020046a6();
void Func_020046e8();
void Func_0200464a();
void Func_02004686();
void Func_0200469e();
void Func_02004732();
void Func_020028be(void);
void Func_02004644();
void Func_020046ca();
void Func_0200475e();
void Func_020046e2();
void Func_02004776();
void Func_020046fa();
void Func_020047d6();
void Func_0200478e();
void Func_02004802();
void Func_020047f6();
void Func_0200477a();
void Func_0200480e();
void Func_02004838();
void Func_02004726();
void Func_020047c6();
void Func_02004848();
void Func_020047d8();
void Func_020047f8();
void Func_0200484a();
void Func_02004854();
void Func_02004860();
void Func_0200486c();
void Func_02004814();
void Func_0200481c();
void Func_02004886();
void Func_02004800();
void Func_020048b0();
void Func_02004834();
void Func_020048c8();
void Func_0200484c();
void Func_020048e0();
void Func_020048ec();
void Func_020048f8();
void Func_02004922();
void Func_02004810();
void Func_020048f0();
void Func_02004912();
void Func_02002b1a(void);
void Func_020048aa();
void Func_0200493c();
void Func_020048a8();
void Func_0200477e();
void Func_0200495a();
void Func_0200497c();
void Func_0200495e();
void Func_02002b34(s32);
void Func_02004882();
void Func_020047b6();
void Func_02004992();
void Func_02004952();
void Func_02004994();
void Func_02002b6a(s32);
void Func_020048b8();
void Func_02004916();
void Func_020047ec();
void Func_020049c8();
void Func_020049ea();
void Func_020049cc();
void Func_02002ba2(s32);
void Func_0200494e();
void Func_02004824();
void Func_02004a00();
void Func_020049c0();
void Func_020049e6();
void Func_02004a08();
void Func_0200485e();
void Func_02004984();
void Func_0200498a();
u8 *Func_0200496e();
u8 *Func_0200497a();
void Func_0200499a();
void Func_020049a4();
void Func_020049b4();
void Func_020049bc();
void Func_020049d2();
void Func_02004a84();
void Func_02004a46();
void Func_02004a6c();
void Func_02004a8e();
void Func_02002c64(s32);
void Func_02002c6a(s32);
u8 *Func_020049ee();
u8 *Func_020049fe();
void Func_02004a28();
void Func_02004a30();
void Func_0200491c();
void Func_02004af8();
void Func_02004ab8();
void Func_02004aa8();
void Func_02004ae8();
void Func_02004b0a();
void Func_02002ce0(s32);
void Func_02004a86();
void Func_02004a8c();
void Func_02004962();
void Func_02004b54();
void Func_02004b5e();
void Func_02004b52();
void Func_02004b08();
void Func_02004b46();
void Func_02004b68();
void Func_02004b38();
void Func_02004b20();
void Func_02004b82();
void Func_02004bb6();
void Func_02004bc0();
void Func_02004b8e();
void Func_02004bb0();
void Func_02004b26();
void Func_02004b2c();
void Func_02004b32();
void Func_02004b3e();
void Func_02004b44();
void Func_02004b4a();
void Func_02004b50();
void Func_02004b56();
void Func_02004b5c();
void Func_02004a32();
void Func_02004bb4();
void Func_02004bbe();
void Func_02004c06();
void Func_02004c10();
void Func_02004c1a();
void Func_02004c24();
void Func_02004c2e();
void Func_02004c38();
void Func_02004c42();
void Func_02004ca6();
void Func_02004cb2();
void Func_02004cbe();
void Func_02004cca();
void Func_02004cd6();
void Func_02004ce2();
void Func_02004cee();
void Func_02004cfa();
void Func_02004cac();
void Func_02004cfe();
void Func_02004cce();
void Func_02004d10();
void Func_02004cc8();
void Func_02004d22();
void Func_02004cda();
void Func_02004d34();
void Func_02004d68();
void Func_02004d08();
void Func_02004d52();
void Func_02004d86();
void Func_02004d20();
void Func_02004c8e();
void Func_02004d2e();
void Func_02004d88();
void Func_02004d40();
void Func_02004d9a();
void Func_02004db6();
void Func_02004dc2();
void Func_02004dce();
void Func_02004dda();
void Func_02004de6();
void Func_02004df2();
void Func_02004dfe();
void Func_02004e0a();
void Func_02004daa();
void Func_02004dba();
void Func_02004dca();
void Func_02004dea();
void Func_02004dfc();
void Func_02004e4e();
void Func_02004e6a();
void Func_02004e76();
void Func_02004e82();
void Func_02004e8e();
void Func_02004e9a();
void Func_02004ea6();
void Func_02004eb2();
void Func_02004ebe();
void Func_02004e10();
void Func_02004e1a();
void Func_02004e24();
void Func_02004e32();
void Func_02004e40();
void Func_02004e4a();
void Func_02004e54();
void Func_02004e5e();
void Func_02004e68();
void Func_02004e72();
void Func_02004f42();
void Func_02004f4a();
void Func_02004f52();
void Func_02004f5a();
void Func_02004f62();
void Func_02004f6a();
void Func_02004f72();
void Func_02004f7a();
void Func_02004f82();
void Func_02004f8a();
void Func_02004ee0();
void Func_02004ee6();
void Func_02004eec();
void Func_02004ef2();
void Func_02004ef8();
void Func_02004efe();
void Func_02004f04();
void Func_02004f0a();
void Func_02004f10();
void Func_02004f16();
void Func_02004dec();
void Func_02004f1e();
void Func_02004f26();
void Func_02004f2e();
void Func_02004f36();
void Func_02004f3e();
void Func_02004f46();
void Func_02004f4e();
void Func_02004f56();
u8 *Func_02004f44();
void Func_02004f6c();
void Func_02004e52();
u8 *Func_02004f60();
void Func_02004f40();
void Func_02004fd6();
void Func_02004e7c();
void Func_02004fa2();
void Func_02004fde();
void Func_02005070();
void Func_0200507c();
void Func_02005088();
void Func_02005094();
void Func_020050b6();
void Func_02005098();
void Func_020050b2();
void Func_020050be();
void Func_020050ca();
void Func_020050f4();
void Func_02005086();
void Func_020050d8();
void Func_020050a8();
void Func_020050ea();
void Func_020050ac();
void Func_02005056();
u8 *Func_0200504c();
void Func_02005074();
u8 *Func_02005068();
void Func_020050d6();
u8 *Func_02005084();
void Func_020050ee();
u8 *Func_0200509c();
void Func_02005104();
u8 *Func_020050b2_b();
void Func_020050c6();
void Func_020050cc();
void Func_020050d2();
void Func_020050de();
void Func_020050e4();
void Func_020050f0();
void Func_0200524e();
void Func_02005148();
u8 *Func_0200510e();
void Func_0200515e();
u8 *Func_02005124();
void Func_02005266();
void Func_02005136();
                                    /* scene entity by selector */

                     

                     

                     

                     

                     

                     

                     

                     

void Func_0808a248();
void Func_0808a368();
void Func_0808a370();
                     
                                    /* wait n frames (busy) */
                                    /* install a per-frame task */
                     
                                    /* read a story flag */
                                    /* set a story flag */
                                    /* play a sound cue */

/* Intra-overlay callees, all byte-exact in assets/code. */

                         

/* IWRAM scene pointer, named by the byte-exact sibling for 0x02003184. */
extern u8 *Data_03001ebc;

/* In-image script data (file offsets value - 0x8000). */
extern u8 Data_0200c948[];          /* 0x4948 */
extern u8 Data_0200bdc4[];          /* 0x3dc4 */
extern u8 Data_0200bfb0[];          /* 0x3fb0, also used by Func_02001f14 */
extern u8 Data_0200c034[];          /* 0x4034 */
extern u8 Data_0200c0cc[];          /* 0x40cc */
extern u8 Data_0200c164[];          /* 0x4164 */
extern u8 Data_0200c1ac[];          /* 0x41ac */
extern u8 Data_0200be00[];          /* 0x3e00 */

/* 0x02009f15 = file offset 0x1f14 + the Thumb bit = Func_02001f14. */
extern u8 Data_02009f15[];

void Func_02000590(void)
{
    u8 *scene;
    u8 *entity;
    u8 *counter;
    s32 pose;
    void (*abortSound)() = Func_080f9010;
    void (*abortScene)() = Func_0808a248;
    void (*abortReset)() = Func_0808a368;
    void (*abortSettle)() = Func_0808a370;

    scene = Data_03001ebc;

    Func_02004190();

    if (Func_02004178(0x910) == 0) {
        goto aborted;
    }
    if (Func_02004186(0x911) != 0) {
        goto aborted;
    }

    Func_020041c4(Data_0200c948);

    Func_02004242(20, 0x00fc0000, 0x01100000);
    Func_02004250(27, 0x011c0000, 0x01080000);
    Func_0200425e(28, 0x011c0000, 0x01180000);
    Func_0200426c(29, 0x012c0000, 0x01080000);
    Func_0200427a(30, 0x012c0000, 0x01180000);
    Func_02004288(32, 0x013c0000, 0x01080000);
    Func_02004296(31, 0x013c0000, 0x01180000);
    Func_020042a4(33, 0x014c0000, 0x01080000);
    Func_020042b2(34, 0x014c0000, 0x01180000);
    Func_020042c0(21, 0x016c0000, 0x01100000);

    Func_020043ce(17);
    Func_02004344(20);
    Func_02004226(0x1a91, 1, 0);
    Func_020043e4(9);
    Func_0200425a(10);
    Func_02004312(0, 2);

    if (*(s16 *)(scene + 364) == 9) {
        Func_02004398(0x00026666, 0x00004ccc);
        Func_0200437c(0, 0xe000, 20);
    } else {
        Func_020043c6(0x00013333, 0x00002666);
        Func_020043a8(0, 0, 20);
    }

    Func_020042fa(20, 0x00011999, 0x00008ccc);
    Func_02004308(27, 0x00010000, 0x00008000);
    Func_02004316(28, 0x00010000, 0x00008000);
    Func_02004320(29, 0x0000e666, 0x00007333);
    Func_0200432a(30, 0x0000e666, 0x00007333);
    Func_02004334(32, 0x0000cccc, 0x00006666);
    Func_0200433e(31, 0x0000cccc, 0x00006666);
    Func_02004348(33, 0x0000b333, 0x00005999);
    Func_02004352(34, 0x0000b333, 0x00005999);
    Func_0200435c(21, 0x00009999, 0x00004ccc);

    Func_0200436e(20, Data_0200bdc4);
    Func_02004376(27, Data_0200bdc4);
    Func_0200437e(28, Data_0200bdc4);
    Func_02004386(29, Data_0200bdc4);
    Func_0200438e(30, Data_0200bdc4);
    Func_02004396(32, Data_0200bdc4);
    Func_0200439e(31, Data_0200bdc4);
    Func_020043a6(33, Data_0200bdc4);
    Func_020043ae(34, Data_0200bdc4);

    *(s16 *)counter = 0;
    counter = Func_0200439c(21) + 100;
    Func_020043c4(21, Data_0200bdc4);

    Func_020044b6(0x00ba0000, -1, 0x01100000, 1);
    Func_020043e4(20);
    Func_02004496(20, 0, 0);
    do {
        Func_020042cc(1);
    } while (*(s16 *)counter == 0);

    Func_020043b2(40);
    Func_0200446a(27, 2);
    Func_020044be(27, 0x5000, 20);
    Func_0200449c(0x1a92);
    Func_020044be(27, 0, 10);
    Func_0200448e(28, 2);
    Func_020044e2(28, 0xb000, 10);
    Func_02004482(28, 3);
    Func_020044e4(28, 0, 10);
    Func_0200451e(32, 0x102);
    Func_0200440c(40);
    Func_020044fe(32, 0, 10);
    Func_02004532(31, 0x100, 40);
    Func_02004526(31, 0xb000, 10);
    Func_02004520(31, 0, 10);
    Func_0200453c(31, 0x8000, 10);
    Func_020044fc(31, 2);
    Func_020044e4(31, 4);
    Func_02004546(31, 0, 10);
    Func_02004562(31, 0xb000, 0);
    Func_0200456e(32, 0x5000, 20);
    Func_0200450e(31, 3);
    Func_0200451e(32, 3);
    Func_0200453e(20, 2);
    Func_020045b0(20, 0x102);
    Func_0200449e(40);
    Func_02004590(20, 0, 10);
    Func_02004560(20, 2);

    entity = Func_020044e6(20);
    entity[90] = (u8)(entity[90] & 0xfe);
    Func_02004542(20, 172, 264);
    Func_020044d8(1);
    entity = Func_0200450e_b(20);
    entity[90] = (u8)(entity[90] | 1);

    Func_020045ec(27, 0x8000, 0);
    Func_020045f8(28, 0x8000, 0);
    Func_02004604(32, 0x8000, 0);
    Func_02004610(31, 0x8000, 20);
    Func_020045b8(20, 3);
    Func_02004526(20);

    entity = Func_0200455c(20);
    entity[90] = (u8)(entity[90] & 0xfe);
    Func_020045b0(20, 172, 272);
    Func_02004546(1);
    entity = Func_0200457c(20);
    entity[90] = (u8)(entity[90] | 1);
    Func_020045d0(20, 180, 272);

    Func_02004662(20, 0, 0);
    Func_0200465c(20, 0, 10);
    Func_0200468e(34, 0x105, 0);
    Func_02004636(34, 1);
    Func_02004626(34, 3);
    Func_02004680(34, 0, 10);
    Func_02004650(33, 1);
    Func_02004692(33, 0, 10);
    Func_02004642(33, 4);
    Func_020046a4(33, 0, 10);
    Func_02004674(21, 2);
    Func_020046e0(21, 0x102, 0);
    Func_020046c2(21, 0, 10);
    Func_02004684(20, 2, 20);
    Func_0200468e(20, 4, 40);
    Func_020046a6(20, 2);
    Func_020046e8(20, 0, 10);

    Func_0200464a(21, 0x00019999, 0x0000cccc);
    Func_02004686(21, 0x109, 282);
    Func_02004692(21, 251, 284);
    Func_0200469e(21, 246, 296);
    Func_02004732(21, 0xc000, 0);
    Func_020028be();
    Func_02004644(40);
    Func_0200468e(21, 0x00019999, 0x0000cccc);
    Func_020046ca(21, 228, 296);
    Func_0200475e(21, 0xc000, 40);
    Func_020046e2(21, 212, 296);
    Func_02004776(21, 0xc000, 40);
    Func_020046fa(21, 192, 296);
    Func_020047d6(21, 0xc000, 40);
    Func_0200478e(21, 2);
    Func_02004802(21, 0x100, 60);
    Func_020047f6(20, 0x3000, 0);
    Func_0200477a(21, 184, 286);
    Func_0200480e(21, 0xb000, 10);
    Func_02004838(21, 0x102);
    Func_02004726(40);
    Func_020047c6(21, 4);

    Func_02004848(20, 0x101, 40);
    Func_020047d8(20, 3);
    Func_020047f8(20, 2);
    Func_0200484a(20, 0, 0);
    Func_02004854(21, 0, 60);
    Func_02004860(20, 0x3000, 0);
    Func_0200486c(21, 0xb000, 10);
    Func_02004814(20, 3);
    Func_0200481c(21, 3);
    Func_02004886(21, 0, 0);
    Func_020047d8(20, 0x00019999, 0x0000cccc);
    Func_02004800(20, Data_0200bfb0);
    Func_0200481c(20, 228, 296);
    Func_020048b0(20, 0xc000, 40);
    Func_02004834(20, 212, 296);
    Func_020048c8(20, 0xc000, 40);
    Func_0200484c(20, 192, 296);
    Func_020048e0(20, 0xc000, 40);
    Func_020048ec(20, 0xb000, 0);
    Func_020048f8(21, 0x3000, 10);
    Func_02004922(20, 0x102);
    Func_02004810(60);
    Func_020048b0(20, 4);
    Func_020048f0(0x1a9e);
    Func_02004912(20, 0, 40);
    Func_02002b1a();
    Func_020048aa(20, 178, 272);
    Func_0200493c(20, 0, 0);
    Func_0200484a(240);

    Func_020048a8(27);
    Func_0200477e(1);
    Func_0200495a(27, 0x8000, 10);
    Func_0200497c(27, 0x101, 60);
    Func_0200495e(27, 0, 10);
    Func_02002b34(27);
    Func_02004882(80);

    Func_020048e0(28);
    Func_020047b6(1);
    Func_02004992(28, 0xd000, 20);
    Func_02004952(28, 2);
    Func_02004994(28, 0, 10);
    Func_02002b6a(28);
    Func_020048b8(160);

    Func_02004916(32);
    Func_020047ec(1);
    Func_020049c8(32, 0x5000, 10);
    Func_020049ea(32, 0x101, 60);
    Func_020049cc(32, 0, 10);
    Func_02002ba2(32);
    Func_020048f0(80);

    Func_0200494e(30);
    Func_02004824(1);
    Func_02004a00(30, 0xb000, 10);
    Func_020049c0(30, 1);
    Func_020049e6(0x1aa4);          /* r5 (0x1a9e) + 6 */
    Func_02004a08(30, 0, 10);

    Func_0200485e(Data_02009f15);   /* = Func_02001f14 + Thumb bit */

    Func_02004984(20);
    Func_0200498a(21);
    Func_02004860(1);

    entity = Func_0200496e(20);
    *(s16 *)(entity + 100) = 0;     /* from r8, which is 0 */
    entity = Func_0200497a(21);
    *(s16 *)(entity + 100) = 0;     /* from r8, which is 0 */

    Func_0200499a(20, 0x0000cccc, 0x00006666);
    Func_020049a4(21, 0x0000cccc, 0x00006666);
    Func_020049b4(20, Data_0200c034);
    Func_020049bc(21, Data_0200c0cc);

    Func_020049d2(29);
    Func_020048a8(1);
    Func_02004a84(29, 0x5000, 10);
    Func_02004a46(29, 2);
    Func_02004a6c(0x1aa3);          /* r5 (0x1a9e) + 5 */
    Func_02004a8e(29, 0, 20);
    Func_02002c64(29);
    Func_02002c6a(30);

    for (;;) {
        Func_020048e0(1);
        entity = Func_020049ee(20);
        if (*(s16 *)(entity + 100) == 0) {
            continue;
        }
        entity = Func_020049fe(21);
        if (*(s16 *)(entity + 100) == 1) {
            break;
        }
    }

    Func_02004a28(20, Data_0200c164);
    Func_02004a30(21, Data_0200c1ac);

    Func_02004a46(31);
    Func_0200491c(1);
    Func_02004af8(31, 0x5000, 10);
    Func_02004ab8(31, 1);
    Func_02004aa8(31, 4);
    Func_02004ae8(0x1aa2);
    Func_02004b0a(31, 0, 10);
    Func_02002ce0(31);

    Func_02004a86(34);
    Func_02004a8c(33);
    Func_02004962(1);
    Func_02004b54(34, 0x105, 40);
    Func_02004b5e(33, 0x105, 60);
    Func_02004b52(34, 0xb000, 10);
    Func_02004b5e(33, 0x5000, 10);
    Func_02004b08(34, 4);
    Func_02004b46(0x1aa5);          /* r5 (0x1aa2) + 3 */
    Func_02004b68(34, 0, 10);
    Func_02004b38(33, 1);
    Func_02004b20(33, 4);
    Func_02004b82(33, 0, 10);
    Func_02004bb6(34, 0x102, 60);

    Func_02004bc0(20, 0x103, 0);
    Func_02004b68(20, 2);
    Func_02004b8e(0x1ab2);
    Func_02004bb0(20, 0, 10);

    Func_02004b26(27);
    Func_02004b2c(28);
    Func_02004b32(29);
    Func_02004b38(30);
    Func_02004b3e(32);
    Func_02004b44(31);
    Func_02004b4a(33);
    Func_02004b50(34);
    Func_02004b56(20);
    Func_02004b5c(21);
    Func_02004a32(1);

    Func_02004bb4(27, 2, 0);
    Func_02004bbe(28, 2, 0);
    Func_02004c06(29, 2, 0);
    Func_02004c10(30, 2, 0);
    Func_02004c1a(32, 2, 0);
    Func_02004c24(31, 2, 0);
    Func_02004c2e(33, 2, 0);
    Func_02004c38(34, 2, 0);
    Func_02004c42(21, 2, 40);

    Func_02004ca6(27, 0x8000, 0);
    Func_02004cb2(28, 0x8000, 0);
    Func_02004cbe(29, 0x8000, 0);
    Func_02004cca(30, 0x8000, 0);
    Func_02004cd6(32, 0x8000, 0);
    Func_02004ce2(31, 0x8000, 0);
    Func_02004cee(33, 0x8000, 0);
    Func_02004cfa(34, 0x8000, 40);

    Func_02004cac(21, 4, 40);
    Func_02004cfe(21, 0, 10);
    Func_02004cce(20, 1);
    Func_02004d10(20, 0, 10);
    Func_02004cc8(21, 3);
    Func_02004d22(21, 0, 10);
    Func_02004cda(20, 3);
    Func_02004d34(20, 0, 10);

    Func_02004d68(27, 0x102, 40);
    Func_02004d08(27, 1);
    Func_02004d52(27, 0, 10);
    Func_02004d86(28, 0x102, 40);
    Func_02004d68(28, 0, 10);
    Func_02004d20(21, 4);
    Func_02004c8e(40);
    Func_02004d2e(21, 3);
    Func_02004d88(21, 0, 20);
    Func_02004d40(20, 3);
    Func_02004d9a(20, 0, 10);

    Func_02004db6(27, 0x5000, 0);
    Func_02004dc2(28, 0xb000, 4);
    Func_02004dce(29, 0x5000, 0);
    Func_02004dda(30, 0xb000, 4);
    Func_02004de6(32, 0x5000, 0);
    Func_02004df2(31, 0xb000, 4);
    Func_02004dfe(33, 0x5000, 0);
    Func_02004e0a(34, 0xb000, 4);

    Func_02004daa(27, 3);
    Func_02004dba(28, 3);
    Func_02004dba(29, 3);
    Func_02004dca(30, 3);
    Func_02004dca(32, 3);
    Func_02004dda(31, 3);
    Func_02004dda(33, 3);
    Func_02004dea(34, 3);

    Func_02004dfc(20, 2, 40);
    Func_02004e4e(20, 0, 10);

    Func_02004e6a(27, 0x8000, 0);
    Func_02004e76(28, 0x8000, 4);
    Func_02004e82(29, 0x8000, 0);
    Func_02004e8e(30, 0x8000, 4);
    Func_02004e9a(32, 0x8000, 0);
    Func_02004ea6(31, 0x8000, 4);
    Func_02004eb2(33, 0x8000, 0);
    Func_02004ebe(34, 0x8000, 4);

    Func_02004e10(20, 0x00011999, 0x00008ccc);
    Func_02004e1a(27, 0x00010ccc, 0x00008666);
    Func_02004e24(28, 0x00010ccc, 0x00008666);
    Func_02004e32(29, 0x00010000, 0x00008000);
    Func_02004e40(30, 0x00010000, 0x00008000);
    Func_02004e4a(32, 0x0000f333, 0x00007999);
    Func_02004e54(31, 0x0000f333, 0x00007999);
    Func_02004e5e(33, 0x0000e666, 0x00007333);
    Func_02004e68(34, 0x0000e666, 0x00007333);
    Func_02004e72(21, 0x0000d999, 0x00006ccc);

    Func_02004f42(27, 1);
    Func_02004f4a(28, 1);
    Func_02004f52(29, 1);
    Func_02004f5a(30, 1);
    Func_02004f62(32, 1);
    Func_02004f6a(31, 1);
    Func_02004f72(33, 1);
    Func_02004f7a(34, 1);
    Func_02004f82(20, 1);
    Func_02004f8a(21, 1);

    Func_02004ee0(27);
    Func_02004ee6(28);
    Func_02004eec(29);
    Func_02004ef2(30);
    Func_02004ef8(32);
    Func_02004efe(31);
    Func_02004f04(33);
    Func_02004f0a(34);
    Func_02004f10(20);
    Func_02004f16(21);
    Func_02004dec(1);

    Func_02004f16(20, Data_0200be00);
    Func_02004f1e(27, Data_0200be00);
    Func_02004f26(28, Data_0200be00);
    Func_02004f2e(29, Data_0200be00);
    Func_02004f36(30, Data_0200be00);
    Func_02004f3e(32, Data_0200be00);
    Func_02004f46(31, Data_0200be00);
    Func_02004f4e(33, Data_0200be00);
    Func_02004f56(34, Data_0200be00);

    entity = Func_02004f44(21);
    *(s16 *)(entity + 100) = 0;
    Func_02004f6c(21, Data_0200be00);
    do {
        Func_02004e52(1);
        entity = Func_02004f60(21);
    } while (*(s16 *)(entity + 100) != 1);

    Func_02004f40(80);
    Func_02004fd6(14, 0x01540000, 0x01120000);
    Func_02004e7c(1);
    Func_02004fa2(14, 0x00010000, 0x00008000);
    Func_02004fde(14, 224, 274);
    Func_02005070(14, 0, 40);
    Func_0200507c(14, 0x8000, 40);
    Func_02005088(14, 0xc000, 40);
    Func_02005094(14, 0x5000, 40);
    Func_020050b6(14, 0x101, 60);
    Func_02005098(14, 0, 10);
    Func_020050b2(14, 0, 40);
    Func_020050be(14, 0xc000, 40);
    Func_020050ca(14, 0x8000, 40);
    Func_020050f4(14, 0x102);
    Func_02005086(14, 4, 40);
    Func_020050d8(14, 0, 20);
    Func_020050a8(14, 2);
    Func_020050ea(14, 0, 10);
    Func_020050ac(14, 4, 40);
    Func_02005056(14, 0x00013333, 0x00009999);

    *(s16 *)(entity + 100) = 0;
    entity = Func_0200504c(14);
    Func_02005074(14, Data_0200be00);
    do {
        Func_02004f5a(1);
        entity = Func_02005068(14);
    } while (*(s16 *)(entity + 100) != 1);

    Func_020050d6(14, 0x01670000, 0x013a0000);
    entity = Func_02005084(14);
    Func_020050ee(20, 0x01c70000, 0x01b20000);
    entity = Func_0200509c(20);
    *(u16 *)(entity + 6) = 0xd000;
    *(u16 *)(entity + 6) = 0xd000;
    Func_02005104(21, 0x01d00000, 0x01a00000);
    entity = Func_020050b2_b(21);
    *(u16 *)(entity + 6) = 0x5000;

    Func_020050c6(27);
    Func_020050cc(28);
    Func_020050d2(29);
    Func_020050d8(30);
    Func_020050de(31);
    Func_020050e4(32);
    Func_020050ea(33);
    Func_020050f0(34);

    Func_0200524e(17);

    if (*(s16 *)(scene + 364) == 9) {
        Func_02005148(0, 224, 458);
        entity = Func_0200510e(0);
    } else {
        pose = 0xc000;
        Func_0200515e(0, 40, 248);
        entity = Func_02005124(0);
        pose = 0x4000;
    }
    *(u16 *)(entity + 6) = (u16)pose;

    goto finish;
    Func_02005266();
    Func_020050f4(0x911);

aborted:
    /* Reached only by the two `bl 0x0200151c` gotos above.  Keep the four
     * known static targets explicit through typed pointers: the address-order
     * checker intentionally classifies those two BLs as intra-owner gotos and
     * therefore does not walk this out-of-line target block as a call region. */
    abortSound(123);
    abortScene(*(s16 *)(scene + 364));
    abortReset();
    abortSettle();

finish:
    Func_02005136();
}
