#include "types.h"

/*
 * resource_3c5 owner at 0x02001238, 2264 bytes: 0x02001238-0x02001b0f.  One of
 * the overlay's two large cutscene scripts -- a straight-line sequence of
 * actor/camera commands separated by frame waits, with three dialogue
 * branches and a three-actor cleanup tail.
 *
 * FRAME AND SIGNATURE.  `push {lr}` only: no locals, no stack arguments, no
 * saved high registers.  The single epilogue at 0x02001b00 is
 * `pop {r0} ; bx r0`, so r0 holds the popped return address and the owner is
 * **void**.  It takes no arguments (r0 is written before any read).
 *
 * POOL MAP, derived from the owner's own control flow -- what the code branches
 * over -- and NOT from any referenced-words listing.  Three data regions, none
 * of them decoded as instructions, and register state is carried straight
 * across each of them:
 *   0x020015b4-0x020015cf   7 words, jumped over by the `b.n` at 0x020015b2
 *                           (0x988, 0x98a, 0x2702, 0x107, 0x101, 0x105,
 *                           0x03001ebc);
 *   0x0200186c-0x0200187f   5 words, jumped over by the `b.n` at 0x02001868
 *                           (0x03001ebc, 0x101, 0x14ccc, 0xa666, 0x107);
 *   0x02001b04-0x02001b0f   3 trailing words past the return, filling the row
 *                           to its 0x02001b10 end (0x03001ebc, 0x13333,
 *                           0x9999).
 * Both interior pools sit in the middle of a dialogue branch: the `bne` that
 * opens each branch targets the instruction immediately AFTER the pool, while
 * the fall-through arm ends in the `b.n` that hops it.  Nothing is dropped by
 * skipping them and nothing live crosses them unmodelled.
 *
 * CALL ACCOUNTING.  `bun tools/lib/overlay_call_targets.ts resource_3c5 1238`
 * reports sites=251 / distinct_targets=29, all 29 import veneers, no prologue
 * and no `call_via`.  The inventory's `calls=230` is the usual lower bound and
 * is not used as a completeness proof.  The multiset comparison is: this file
 * contains 251 `Func_...(` occurrences with per-name multiplicities
 *   0808a010 x91  0808a180 x32  0808a1b8 x22  0808a110 x17  0808a1e8 x11
 *   0808a138 x11  0808a090 x11  0808a100 x6   0808a580 x5   0808a570 x4
 *   0808a0e8 x4   0808a0d0 x4   0808a178 x3   0808a130 x3   0808a0f0 x3
 *   0808a0b8 x3   0808a080 x3   0808a070 x3   0808a578 x2   0808a218 x2
 *   0808a210 x2   080770c8 x2   0808a460 x1   0808a208 x1   0808a170 x1
 *   0808a150 x1   0808a0c8 x1   0808a020 x1   0808a018 x1
 * which is exactly the tool's per-target table, entry for entry.
 *
 * THE SCRIPT'S IDIOM.  Func_0808a010(n) is the frame wait that separates
 * beats; it is 91 of the 251 sites and its argument is a frame count (10, 20,
 * 25, 30, 35, 40).  Func_0808a180(slot, 0) waits for the slot's queued action
 * to finish.  Func_0808a1b8(slot, heading, 0) faces a slot, and the headings
 * used here (0, 0x4000, 0x8000, 0xb000, 0xc000) are the same 16-bit angle
 * scale the rest of the overlay uses.  Func_0808a1e8(slot, id, frames) plays a
 * numbered animation, Func_0808a090/0808a0d0/0808a0f0 set positions, and
 * Func_0808a070(0, 0) is the dialogue call whose result selects the branch.
 *
 * THE DIALOGUE COUNTER.  Each of the three branches, on both arms, bumps the
 * halfword at byte 472 of the workspace: `ldr r3,=0x03001ebc / ldr r2,[r3] /
 * movs r3,#236 / lsls r3,#1 / adds r2,r2,r3 / ldrh / adds #1 / strh`.  The
 * workspace pointer variable is the same Data_03001ebc the byte-exact sibling
 * assets/code/resource_3c5_c_0200006c.c reads, and the overlay image is
 * writable EWRAM used as save state, so this is not const data.
 *
 * Uncertainties, recorded rather than guessed:
 *  - The three tail probes at 0x02001a70/0x02001aa0/0x02001ad0 read the record's
 *    s16 at +10 and +18.  Those are the integer parts of the 16.16 words at +8
 *    and +16, the halfword coordinate view the byte-exact resource_3c8:14f4
 *    already models twice for the same reason.
 *  - At each of those sites the register r3 still holds 18 when the `bl` is
 *    taken, because `movs r3,#18` supplied the load's offset.  That is the
 *    offset register, not a fourth argument; Func_0808a0b8 is spelled with
 *    three.  Recorded because an argument-window simulator reports it as
 *    `(slot, x, z, 18)` and the wrong reading is plausible.
 *  - The identity of the two 0x080770c8 progress ids 0x988 and 0x98a, and of
 *    the 0x2702 argument to Func_0808a170, is not established here; they are
 *    pooled constants passed straight through.
 *  - Every import is declared old-style: several are reached with different
 *    argument counts elsewhere in this overlay.
 */

extern u8 *Data_03001ebc;       /* workspace pointer variable */

/* The dialogue-progress counter each branch bumps. */
#define DIALOGUE_COUNTER (*(u16 *)(Data_03001ebc + 472))

/* Halfword coordinate view of a record: the integer parts of the 16.16 words
 * at +8 and +16. */
#define POS_X(rec) (*(s16 *)((u8 *)(rec) + 10))
#define POS_Z(rec) (*(s16 *)((u8 *)(rec) + 18))

/* Old-style declarations are mandatory in overlay sources. */
void Func_0200405e();
void Func_02004064();
void Func_02004080();
void Func_020041b4();
void Func_0200412a();
void Func_020040b8();
void Func_020040de();
void Func_0200416a();
void Func_020040b0();
void Func_020041fe();
void Func_0200420e();
void Func_0200421c();
void Func_0200422a();
void Func_02004140();
void Func_020040f6();
void Func_020041b6();
void Func_02004104();
void Func_020041f8();
void Func_02004212();
void Func_0200421e();
void Func_0200412c();
void Func_0200419c();
void Func_0200413a();
void Func_020041fa();
void Func_02004148();
void Func_02004214();
void Func_02004220();
void Func_02004166();
void Func_0200416c();
void Func_020041f4();
void Func_0200417a();
void Func_0200423a();
void Func_02004188();
void Func_02004208();
void Func_02004218();
void Func_0200419e();
void Func_0200422e();
void Func_02004234();
void Func_02004244();
void Func_020041ca();
void Func_020042a6();
void Func_02004296();
void Func_020041e4();
void Func_020042ae();
void Func_0200427c();
void Func_02004202();
void Func_020042c2();
void Func_02004210();
void Func_020042dc();
void Func_02004222();
void Func_02004292();
void Func_02004230();
void Func_020042f0();
void Func_0200423e();
void Func_0200424c();
void Func_0200430c();
void Func_0200425a();
void Func_02004334();
void Func_0200426a();
void Func_02004336();
void Func_0200433c();
void Func_0200428a();
void Func_02004364();
void Func_0200436e();
void Func_0200436a();
void Func_020042b0();
void Func_0200437c();
void Func_0200438e();
void Func_020042d4();
void Func_02004394();
void Func_020043de();
void Func_020043ea();
void Func_020042f8();
void Func_02004368();
void Func_02004306();
void Func_020043c6();
void Func_02004314();
void Func_02004384();
void Func_02004322();
void Func_020043e2();
void Func_02004330();
void Func_0200440a();
void Func_020043fa();
void Func_02004348();
void Func_020043b8();
void Func_02004356();
void Func_02004416();
void Func_020043ec();
void Func_02004372();
void Func_02004432();
void Func_02004380();
void Func_02004408();
void Func_02004446();
s32 Func_020043b6();
void Func_020043a8();
void Func_02004474();
void Func_020043ba();
void Func_0200442a();
void Func_020043c8();
void Func_02004488();
void Func_02004404();
void Func_020044d0();
void Func_02004486();
void Func_02004424();
void Func_020044f4();
void Func_02004442();
void Func_0200451c();
void Func_02004482();
void Func_020045b4();
void Func_0200446a();
void Func_0200452a();
void Func_02004478();
void Func_02004544();
void Func_0200448a();
void Func_0200454a();
void Func_02004498();
void Func_02004520();
void Func_020044a6();
void Func_02004566();
void Func_020044b4();
void Func_02004590();
void Func_02004580();
void Func_020044ce();
void Func_02004556();
void Func_020044dc();
void Func_0200459c();
void Func_020044ea();
void Func_0200455a();
void Func_020044f8();
void Func_020045b8();
void Func_02004506();
void Func_020045e2();
void Func_020045ca();
s32 Func_0200453a();
void Func_0200452c();
void Func_0200453a_b();
void Func_020045fa();
void Func_02004568();
void Func_02004638();
void Func_02004586();
void Func_02004662();
void Func_02004652();
void Func_020045a0();
void Func_02004628();
void Func_020045ae();
void Func_020045e0();
void Func_02004712();
void Func_020045c8();
void Func_02004688();
void Func_020045d6();
void Func_02004608();
void Func_0200473a();
void Func_020045f0();
void Func_020046ca();
void Func_020046ba();
void Func_02004678();
void Func_02004616();
void Func_0200461c();
void Func_020046f8();
void Func_020046f4();
void Func_0200463a();
void Func_020046fa();
void Func_02004648();
void Func_020046d0();
void Func_02004656();
void Func_0200470e();
s32 Func_0200467e();
void Func_02004670();
void Func_020046e0();
void Func_0200467e_b();
void Func_0200473e();
void Func_020046b4();
void Func_02004724();
void Func_020046c2();
void Func_02004792();
void Func_020047ac();
void Func_020046f2();
void Func_02004762();
void Func_02004700();
void Func_020047cc();
void Func_020047d2();
void Func_02004720();
void Func_020047ba();
void Func_020047a2();
void Func_02004740();
void Func_0200480c();
void Func_02004818();
void Func_0200475e();
void Func_020047c6();
void Func_020047d6();
void Func_02004774();
void Func_020047aa();
void Func_020047b8();
void Func_020048e2();
void Func_020048f4();
void Func_020048f6();
void Func_02004908();
void Func_020047fe();
void Func_02004814();
void Func_0200483c();
void Func_020048a8();
void Func_020048b4();
void Func_020047fa();
void Func_020048c6();
void Func_020048cc();
void Func_0200481a();
void Func_02004850();
void Func_02004876();
void Func_02004902();
void Func_02004848();
void Func_0200487e();
void Func_020048a4();
void Func_02004930();
void Func_020048b2();
void Func_020048bc();
void Func_020048fc();
u8 *Func_020048c2();
void Func_020048e4();
void Func_0200490a();
void Func_0200491c();
void Func_0200492c();
u8 *Func_020048f2();
void Func_02004914();
void Func_0200493a();
void Func_0200494c();
void Func_0200495c();
u8 *Func_02004922();
void Func_02004944();
void Func_0200496a();
void Func_0200497c();
void Func_0200492a();
void Func_0200493e();
                     
                                /* wait n frames */

                                /* dialogue; result selects the branch */
                                /* record by slot index, or 0 */

                     

                     

                     

                     
                     
                                /* wait for the slot's action to finish */
                                /* face slot along heading */
                                /* play animation id for n frames */

                     

                     

void Func_02001238(void)
{
    u8 *record;

    Func_0200405e(0x988);
    Func_02004064(0x98a);
    Func_02004080();
    Func_020041b4();
    Func_0200412a(0x2702);
    Func_020040b8(0, 0x10000, 0x8000);
    Func_020040de(0, 0x128, 0x160);
    Func_0200416a(0, 0xc000, 0);
    Func_020040b0(0xa);
    Func_020041fe(0xa, 0x10, 0, 0xc000);
    Func_0200420e(1, -8, 0x10, 0xc000);
    Func_0200421c(2, 8, 0x10, 0xc000);
    Func_0200422a(3, 0x18, 0x10, 0xc000);
    Func_02004140(3);
    Func_020040f6(0x14);
    Func_020041b6(0xe, 0);
    Func_02004104(0xa);
    Func_020041f8(0x30000, 0x6000);
    Func_02004212(0x1180000, -1, 0x1200000, 1);
    Func_0200421e();
    Func_0200412c(0x14);
    Func_0200419c(0xb, 3);
    Func_0200413a(0x1e);
    Func_020041fa(0xb, 0);
    Func_02004148(0xa);
    Func_02004214(0xe, 0xc000, 0);
    Func_02004220(0xb, 0xc000, 0);
    Func_02004166(0x1e);
    Func_0200416c(0xa);
    Func_020041f4(0xe, 2);
    Func_0200417a(0x14);
    Func_0200423a(0xe, 0);
    Func_02004188(0xa);
    Func_02004208(0xd, 2);
    Func_02004218(0xc, 2);
    Func_0200419e(0x28);
    Func_0200421e(0xd, 2);
    Func_0200422e(0xc, 2);
    Func_020041b4(0x28);
    Func_02004234(0xd, 2);
    Func_02004244(0xc, 2);
    Func_020041ca(0x28);
    Func_020042a6(0xc, 0x102, 0x32);
    Func_02004296(0xc, 0);
    Func_020041e4(0xa);
    Func_020042ae(0xd, 0, 0);
    Func_020041f4(0x19);
    Func_0200427c(0xd, 2);
    Func_02004202(0x14);
    Func_020042c2(0xd, 0);
    Func_02004210(0xa);
    Func_020042dc(0xc, 0x8000, 0);
    Func_02004222(0x14);
    Func_02004292(0xd, 4);
    Func_02004230(0x14);
    Func_020042f0(0xd, 0);
    Func_0200423e(0xa);
    Func_020042ae(0xb, 4);
    Func_0200424c(0x14);
    Func_0200430c(0xb, 0);
    Func_0200425a(0x14);
    Func_02004334(0xd, 0x107, 0x28);
    Func_0200426a(0xa);
    Func_02004336(0xd, 0x4000, 0);
    Func_0200427c(0x14);
    Func_0200433c(0xd, 0);
    Func_0200428a(0x14);
    Func_02004364(0xd, 0x101, 0x4b);
    Func_0200436e(0xe, 0x101, 0x3c);
    Func_0200436a(0xc, 0x4000, 0);
    Func_020042b0(0x14);
    Func_0200437c(0xb, 0x4000, 0);
    Func_020042c2(0x14);
    Func_0200438e(0xe, 0x4000, 0);
    Func_020042d4(0x1e);
    Func_02004394(0xe, 0);
    Func_020043de(0x1180000, -1, 0x1400000, 1);
    Func_020043ea();
    Func_020042f8(0x14);
    Func_02004368(0xa, 3);
    Func_02004306(0x1e);
    Func_020043c6(0xa, 0);
    Func_02004314(0xa);
    Func_02004384(0xa, 4);
    Func_02004322(0x14);
    Func_020043e2(0xa, 0);
    Func_02004330(0x14);
    Func_0200440a(0xe, 0x105, 0x3c);
    Func_020043fa(0xe, 0);
    Func_02004348(0xa);
    Func_020043b8(3, 4);
    Func_02004356(0x14);
    Func_02004416(3, 0);
    Func_02004364(0xa);
    Func_020043ec(2, 2);
    Func_02004372(0x14);
    Func_02004432(2, 0);
    Func_02004380(0xa);
    Func_02004408(1, 2);
    Func_0200438e(0x14);
    Func_02004446(1, 0);

    if (Func_020043b6(0, 0) != 0) {
        Func_020043a8(0x1e);
        DIALOGUE_COUNTER++;
        Func_02004474(0xa, 0x8000, 0);
        Func_020043ba(0x1e);
        Func_0200442a(0xa, 4);
        Func_020043c8(0x14);
        Func_02004488(0xa, 0);
    } else {
        Func_02004404(0x1e);
        DIALOGUE_COUNTER++;
        Func_020044d0(0xa, 0x8000, 0);
        Func_02004416(0x1e);
        Func_02004486(0xa, 3);
        Func_02004424(0x1e);
        Func_020044f4(0xa, 0);
    }

    Func_02004442(0xa);
    Func_0200451c(0xe, 0x101, 0x3c);
    Func_02004482(0xe, 0x10000, 0x8000);
    Func_020045b4(0xe, 0, 0x10);
    Func_0200446a(0x14);
    Func_0200452a(0xe, 0);
    Func_02004478(0xa);
    Func_02004544(0xa, 0xc000, 0);
    Func_0200448a(0x23);
    Func_0200454a(0xa, 0);
    Func_02004498(0xa);
    Func_02004520(0xe, 2);
    Func_020044a6(0x14);
    Func_02004566(0xe, 0);
    Func_020044b4(0x14);
    Func_02004590(3, 0x102, 0x28);
    Func_02004580(3, 0);
    Func_020044ce(0xa);
    Func_02004556(2, 2);
    Func_020044dc(0x14);
    Func_0200459c(2, 0);
    Func_020044ea(0xa);
    Func_0200455a(1, 3);
    Func_020044f8(0x1e);
    Func_020045b8(1, 0);
    Func_02004506(0x14);
    Func_020045e2(0xe, 0x100, 0x28);
    Func_020045ca(0xe, 0);

    if (Func_0200453a(0, 0) != 0) {
        Func_0200452c(0x1e);
        Func_0200459c(0xe, 4);
        Func_0200453a_b(0x14);
        Func_020045fa(0xe, 0);
    } else {
        DIALOGUE_COUNTER++;
        Func_0200455a(0x1e);
        Func_020045ca(0xe, 4);
        Func_02004568(0x14);
        Func_02004638(0xe, 0);
        DIALOGUE_COUNTER++;
    }

    Func_02004586(0xa);
    Func_02004662(0xa, 0x102, 0x32);
    Func_02004652(0xa, 0);
    Func_020045a0(0xa);
    Func_02004628(0xd, 2);
    Func_020045ae(0x14);
    Func_020045e0(0xd, 0x14ccc, 0xa666);
    Func_02004712(0xd, 0, 0x10);
    Func_020045c8(0x14);
    Func_02004688(0xd, 0);
    Func_020045d6(0xa);
    Func_02004608(0xc, 0x14ccc, 0xa666);
    Func_0200473a(0xc, 0, 0x10);
    Func_020045f0(0x14);
    Func_020046ca(0xc, 0x107, 0x32);
    Func_020046ba(0xc, 0);
    Func_02004608(0xa);
    Func_02004678(0xe, 3);
    Func_02004616(0x1e);
    Func_0200461c(0xa);
    Func_020046f8(0xa, 0x102, 0x3c);
    Func_020046f4(0xa, 0x8000, 0);
    Func_0200463a(0x19);
    Func_020046fa(0xa, 0);
    Func_02004648(0xa);
    Func_020046d0(0xa, 2);
    Func_02004656(0x14);
    Func_0200470e(0xa, 0);

    if (Func_0200467e(0, 0) != 0) {
        DIALOGUE_COUNTER++;
        Func_02004670(0x1e);
        Func_020046e0(0xa, 4);
        Func_0200467e_b(0x14);
        Func_0200473e(0xa, 0);
    } else {
        Func_020046b4(0x1e);
        Func_02004724(0xa, 3);
        Func_020046c2(0x1e);
        Func_02004792(0xa, 0);
        DIALOGUE_COUNTER++;
    }

    Func_020046e0(0xa);
    Func_020047ac(0xa, 0xc000, 0);
    Func_020046f2(0x23);
    Func_02004762(0xe, 3);
    Func_02004700(0x1e);
    Func_020047cc(0xe, 0xb000, 0);
    Func_02004712(0x28);
    Func_020047d2(0xe, 0);
    Func_02004720(0xa);
    Func_020047ba(0xc, 0xd, 0x32);
    Func_02004792(0xc, 3);
    Func_020047a2(0xd, 3);
    Func_02004740(0x1e);
    Func_0200480c(0xc, 0x4000, 0);
    Func_02004818(0xd, 0x4000, 0);
    Func_0200475e(0x14);
    Func_020047c6(0xc, 3);
    Func_020047d6(0xd, 3);
    Func_02004774(0x1e);
    Func_020047aa(0xc, 0x10000, 0x8000);
    Func_020047b8(0xd, 0x10000, 0x8000);
    Func_020048e2(0xc, 0x20, 0);
    Func_020048f4(0xd, 0x20, 0);
    Func_020048f6(0xc, 0, 0x10);
    Func_02004908(0xd, 0x10, 0);
    Func_020047fe(0xd, 0x158, 0x138);
    Func_02004814(0xc, 0x158, 0x150);
    Func_0200483c(0xd, 1);
    Func_020048a8(0xc, 0x8000, 0);
    Func_020048b4(0xd, 0x8000, 0);
    Func_020047fa(0xa);
    Func_020048c6(0xe, 0x4000, 0);
    Func_0200480c(0x14);
    Func_020048cc(0xe, 0);
    Func_0200481a(0xa);
    Func_02004850(0xe, 0x10000, 0x8000);
    Func_02004876(0xe, 0x148, 0x138);
    Func_02004902(0xe, 0x8000, 0);
    Func_02004848(0x1e);
    Func_0200487e(0xb, 0x10000, 0x8000);
    Func_020048a4(0xb, 0x148, 0x148);
    Func_02004930(0xb, 0x8000, 0);
    Func_02004876(0x14);
    Func_020048a8(1, 0x13333, 0x9999);
    Func_020048b2(2, 0x13333, 0x9999);
    Func_020048bc(3, 0x13333, 0x9999);
    Func_020048fc(1, 2);

    record = Func_020048c2(0);
    if (record != 0) {
        Func_020048e4(1, POS_X(record), POS_Z(record));
    }
    Func_0200490a(1);
    Func_0200491c(1, 0, 0);
    Func_0200492c(2, 2);

    record = Func_020048f2(0);
    if (record != 0) {
        Func_02004914(2, POS_X(record), POS_Z(record));
    }
    Func_0200493a(2);
    Func_0200494c(2, 0, 0);
    Func_0200495c(3, 2);

    record = Func_02004922(0);
    if (record != 0) {
        Func_02004944(3, POS_X(record), POS_Z(record));
    }
    Func_0200496a(3);
    Func_0200497c(3, 0, 0);
    Func_0200492a(0xa);
    Func_0200493e();
}
