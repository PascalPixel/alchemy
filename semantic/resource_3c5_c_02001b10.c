#include "types.h"

/*
 * resource_3c5 owner at 0x02001b10, 2396 bytes: 0x02001b10-0x0200246b.  The
 * overlay's second large cutscene script, the sibling of the one at
 * 0x02001238: the same straight-line actor/camera command sequence, the same
 * two dialogue branches and the same three-actor cleanup tail.
 *
 * FRAME AND SIGNATURE.  `push {lr}` only -- no locals, no stack arguments, no
 * saved high registers.  The single epilogue at 0x0200245a is
 * `pop {r0} ; bx r0`, so r0 holds the popped return address and the owner is
 * **void**.  It takes no arguments (r0 is written before any read).
 *
 * POOL MAP, derived from the owner's own control flow -- what the code branches
 * over -- and NOT from any referenced-words listing.  Three data regions, none
 * decoded as instructions, with register state carried straight across them:
 *   0x02001f0c-0x02001f1b   4 words, hopped by the unconditional `b.n` at
 *                           0x02001f0a (0x989, 0x272f, 0x101, 0x301);
 *   0x020021f4-0x020021ff   3 words, hopped by the `b.n` at 0x020021f2, which
 *                           ends the fall-through arm of the second dialogue
 *                           branch; the `bne` that opens that branch targets
 *                           0x02002200, the halfword immediately after the
 *                           pool (0x103, 0x03001ebc, 0x101);
 *   0x02002460-0x0200246b   3 trailing words past the return, filling the row
 *                           to its 0x0200246c end (0x03001ebc, 0x13333,
 *                           0x9999).
 * The first of these is the plain case the guard exists for: an unconditional
 * branch over a pool in the middle of the script, with nothing else to mark it.
 *
 * CALL ACCOUNTING.  `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3c5 1b10`
 * reports sites=263 / distinct_targets=30 (29 import veneers plus one real
 * prologue, this overlay's own 0x02002548).  The inventory's `calls=249` is
 * the usual lower bound and is not used as a completeness proof.  The multiset
 * comparison is: this file contains 263 `Func_...(` occurrences with per-name
 * multiplicities
 *   0808a010 x88  0808a1b8 x49  0808a180 x27  0808a1e8 x18  0808a110 x13
 *   0808a138 x12  0808a100 x7   0808a090 x5   0808a0e8 x4   0808a570 x3
 *   0808a210 x3   0808a580 x3   0808a080 x3   0808a0b8 x3   0808a0f0 x3
 *   0808a018 x2   0808a0d0 x2   0808a218 x2   0808a128 x2   0808a178 x2
 *   0808a070 x2   0808a150 x2   080770c8 x1   0808a460 x1   0808a170 x1
 *   0808a208 x1   0808a200 x1   080770d0 x1   0808a020 x1   02002548 x1
 * which is exactly the tool's per-target table, entry for entry.
 *
 * THE SCRIPT'S IDIOM is identical to the 0x02001238 owner's: Func_0808a010(n)
 * is the frame wait between beats (88 of the 263 sites), Func_0808a180(slot, 0)
 * waits for a slot's queued action, Func_0808a1b8(slot, heading, 0) faces a
 * slot on the overlay's 16-bit angle scale, Func_0808a1e8(slot, id, frames)
 * plays a numbered animation, and Func_0808a070(0, 0) is the dialogue call
 * whose result selects the branch.  Func_02002548 is the overlay's own routine,
 * called once with no argument register set.
 *
 * THE DIALOGUE COUNTER.  Both branches, on both arms, bump the halfword at byte
 * 472 of the workspace, the same `ldr =0x03001ebc / ldr / movs #236 / lsls #1 /
 * ldrh / adds #1 / strh` sequence the sibling script uses.  The workspace
 * pointer variable is the Data_03001ebc that the byte-exact
 * assets/code/resource_3c5_c_0200006c.c reads, and the overlay image is
 * writable EWRAM used as save state, so this is not const data.
 *
 * Uncertainties, recorded rather than guessed:
 *  - The three tail probes at 0x0200237c/0x020023ac/0x020023dc read the
 *    record's s16 at +10 and +18: the integer parts of the 16.16 words at +8
 *    and +16, the halfword coordinate view the byte-exact resource_3c8:14f4
 *    already models twice.
 *  - At each of those sites r3 still holds 18 at the `bl`, because
 *    `movs r3,#18` supplied the load's offset.  That is the offset register,
 *    not a fourth argument; Func_0808a0b8 is spelled with three.  Recorded
 *    because an argument-window simulator reports `(slot, x, z, 18)`.
 *  - The progress ids 0x989 (Func_080770c8) and 0x301 (Func_080770d0), and the
 *    0x272f argument to Func_0808a170, are pooled constants passed straight
 *    through; their meaning is not established here.
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
void Func_02004936();
void Func_02004952();
void Func_02004a86();
void Func_020049fc();
void Func_0200498a();
void Func_020049b0();
void Func_02004a3a();
void Func_02004980();
void Func_02004acc();
void Func_02004adc();
void Func_02004aea();
void Func_02004a00();
void Func_020049b6();
void Func_02004aaa();
void Func_02004ac4();
void Func_02004ad0();
void Func_020049de();
void Func_020049e4();
void Func_02004ab0();
void Func_020049f6();
void Func_02004ad2();
void Func_02004ac2();
void Func_02004a84();
void Func_02004a8e();
void Func_02004a24();
void Func_02004b00();
void Func_02004b0c();
void Func_02004b18();
void Func_02004b24();
void Func_02004a5a();
void Func_02004b26();
void Func_02004b32();
void Func_02004b3e();
void Func_02004b4a();
void Func_02004a90();
void Func_02004a9e();
void Func_02004b5e();
void Func_02004aac();
void Func_02004b88();
void Func_02004b78();
void Func_02004ac6();
void Func_02004ba0();
void Func_02004b9c();
void Func_02004ae2();
void Func_02004ba2();
void Func_02004af0();
void Func_02004bbc();
void Func_02004bc8();
void Func_02004bd4();
void Func_02004b1a();
void Func_02004b20();
void Func_02004ba8();
void Func_02004b2e();
void Func_02004bee();
void Func_02004b3c();
void Func_02004bc4();
void Func_02004c0a();
void Func_02004b58();
void Func_02004c34();
void Func_02004c24();
void Func_02004b72();
void Func_02004be2();
void Func_02004b80();
void Func_02004c40();
void Func_02004b8e();
void Func_02004c16();
void Func_02004c5c();
void Func_02004baa();
void Func_02004c32();
void Func_02004bb8();
void Func_02004d22();
void Func_02004c9c();
void Func_02004ca2();
void Func_02004bf0();
void Func_02004c60();
void Func_02004bfe();
void Func_02004c04();
void Func_02004cd0();
void Func_02004cdc();
void Func_02004c22();
void Func_02004c8a();
void Func_02004c9a();
void Func_02004c38();
void Func_02004d04();
void Func_02004d10();
void Func_02004c56();
void Func_02004cc6();
void Func_02004c64();
void Func_02004d4c();
void Func_02004dc8();
void Func_02004394();
void Func_02004c7a();
void Func_02004d84();
void Func_02004ca0();
void Func_02004d74();
void Func_02004d64();
void Func_02004cb2();
void Func_02004d7e();
void Func_02004d8a();
void Func_02004d96();
void Func_02004da2();
void Func_02004dae();
void Func_02004dba();
void Func_02004dc6();
void Func_02004dd2();
void Func_02004d18();
void Func_02004df4();
void Func_02004e00();
void Func_02004e1e();
void Func_02004e2a();
void Func_02004e5c();
void Func_02004e68();
void Func_02004d76();
void Func_02004e52();
void Func_02004e42();
void Func_02004d90();
void Func_02004d9e();
void Func_02004e5e();
void Func_02004dac();
void Func_02004e34();
void Func_02004e86();
void Func_02004dcc();
void Func_02004e8c();
void Func_02004dda();
void Func_02004ea6();
void Func_02004dec();
void Func_02004eb8();
void Func_02004dfe();
void Func_02004eda();
void Func_02004ed6();
void Func_02004e1c();
void Func_02004edc();
void Func_02004e9a();
void Func_02004e38();
void Func_02004ef8();
void Func_02004e46();
void Func_02004f20();
void Func_02004f1c();
void Func_02004e62();
void Func_02004f1a();
void Func_02004f34();
void Func_02004f40();
void Func_02004f4c();
void Func_02004f58();
void Func_02004f64();
void Func_02004f70();
s32 Func_02004ed0();
void Func_02004ec2();
void Func_02004f4a();
void Func_02004ed0_b();
void Func_02004f90();
void Func_02004ef0();
void Func_02004f78();
void Func_02004efe();
void Func_02004fce();
void Func_02004fa4();
void Func_02004f2a();
void Func_02005084();
void Func_02005000();
void Func_02004f46();
void Func_02005006();
void Func_02004f54();
void Func_02005020();
void Func_02004f66();
void Func_02004fd6();
void Func_02004f74();
void Func_02005034();
void Func_02004f82();
void Func_02004ff2();
void Func_02005050();
void Func_02004f9e();
void Func_02005026();
void Func_02004fac();
void Func_02005064();
void Func_02004fba();
void Func_02005086();
void Func_02004fcc();
void Func_020050a6();
s32 Func_02004ff6();
void Func_02004fe8();
void Func_020050b4();
void Func_02004ffa();
void Func_0200506a();
void Func_02005008();
void Func_020050c8();
void Func_02005100();
void Func_02005046();
void Func_020050b6();
void Func_02005054();
void Func_02005124();
void Func_02005072();
void Func_0200514e();
void Func_0200513e();
void Func_0200508c();
void Func_020050fc();
void Func_0200509a();
void Func_020050a0();
void Func_02005128();
void Func_020050ae();
void Func_0200517a();
void Func_020050c0();
void Func_02005180();
void Func_020050ce();
void Func_020050dc();
void Func_0200519c();
void Func_020050ea();
void Func_020051b6();
void Func_02005184();
void Func_0200510a();
void Func_02005158();
void Func_020051e2();
void Func_020051ee();
void Func_02005134();
void Func_020051f4();
void Func_02005142();
void Func_020051dc();
void Func_020051e6();
void Func_0200515c();
void Func_020051c4();
void Func_020051cc();
void Func_020051d4();
void Func_020051e4();
void Func_02005182();
void Func_020051b4();
void Func_020051be();
void Func_020051c8();
void Func_02005208();
u8 *Func_020051ce();
void Func_020051f0();
void Func_02005216();
void Func_02005228();
void Func_02005238();
u8 *Func_020051fe();
void Func_02005220();
void Func_02005246();
void Func_02005258();
void Func_02005268();
u8 *Func_0200522e();
void Func_02005250();
void Func_02005276();
void Func_02005288();
void Func_02005236();
void Func_02005302();
void Func_0200530e();
void Func_0200531a();
void Func_02005326();
void Func_02005332();
void Func_0200533e();
void Func_02005284();
void Func_02005298();

                     
                                /* wait n frames */

                                /* dialogue; result selects the branch */
                                /* record by slot index, or 0 */

                     

                     

                     

                     
                                /* wait for the slot's action to finish */
                                /* face slot along heading */
                                /* play animation id for n frames */

                     

                     

void Func_02001b10(void)
{
    u8 *record;

    Func_02004936(0x989);
    Func_02004952();
    Func_02004a86();
    Func_020049fc(0x272f);
    Func_0200498a(0, 0x10000, 0x8000);
    Func_020049b0(0, 0x128, 0x138);
    Func_02004a3a(0, 0, 0);
    Func_02004980(0xa);
    Func_02004acc(1, 0, 0x10, 0);
    Func_02004adc(2, 0xfffffff0, 0xfffffff8, 0);
    Func_02004aea(3, 0xfffffff0, 0x18, 0);
    Func_02004a00(3);
    Func_020049b6(0x14);
    Func_02004aaa(0x30000, 0x6000);
    Func_02004ac4(0x1180000, -1, 0x1480000, 1);
    Func_02004ad0();
    Func_020049de(0xa);
    Func_020049e4(0xa);
    Func_02004ab0(0xa, 0xb000, 0);
    Func_020049f6(0xa);
    Func_02004ad2(0xa, 0x100, 0x28);
    Func_02004ac2(0xa, 0);
    Func_02004a84(0xa, 4, 0xd);
    Func_02004a8e(0xa, 4, 0x1e);
    Func_02004a24(0xa);
    Func_02004b00(0xb, 0x100, 0);
    Func_02004b0c(0xc, 0x100, 0);
    Func_02004b18(0xd, 0x100, 0);
    Func_02004b24(0xe, 0x100, 0x28);
    Func_02004a5a(0xa);
    Func_02004b26(0xe, 0xb000, 0);
    Func_02004b32(0xb, 0xb000, 0);
    Func_02004b3e(0xc, 0xb000, 0);
    Func_02004b4a(0xd, 0xb000, 0);
    Func_02004a90(0x1e);
    Func_02004b18(0xe, 2);
    Func_02004a9e(0x14);
    Func_02004b5e(0xe, 0);
    Func_02004aac(0x14);
    Func_02004b88(0xd, 0x102, 0x28);
    Func_02004b78(0xd, 0);
    Func_02004ac6(0x14);
    Func_02004ba0(0xc, 0x101, 0x32);
    Func_02004b9c(0xc, 0x8000, 0);
    Func_02004ae2(0x19);
    Func_02004ba2(0xc, 0);
    Func_02004af0(0xa);
    Func_02004bbc(0xe, 0x8000, 0);
    Func_02004bc8(0xb, 0x8000, 0);
    Func_02004bd4(0xd, 0x8000, 0);
    Func_02004b1a(0x1e);
    Func_02004b20(0xa);
    Func_02004ba8(3, 2);
    Func_02004b2e(0x14);
    Func_02004bee(3, 0);
    Func_02004b3c(0xa);
    Func_02004bc4(2, 2);
    Func_02004b4a(0x14);
    Func_02004c0a(2, 0);
    Func_02004b58(0xa);
    Func_02004c34(1, 0x102, 0x28);
    Func_02004c24(1, 0);
    Func_02004b72(0x14);
    Func_02004be2(0xa, 4);
    Func_02004b80(0x14);
    Func_02004c40(0xa, 0);
    Func_02004b8e(0xa);
    Func_02004c16(0xe, 2);
    Func_02004b9c(0x19);
    Func_02004c5c(0xe, 0);
    Func_02004baa(0xa);
    Func_02004c32(0xa, 2);
    Func_02004bb8(0x14);
    Func_02004bee(0xa, 0x10000, 0x8000);
    Func_02004d22(0xa, 0, 0xffffffd8);
    Func_02004c9c(0xa, 0, 0);
    Func_02004be2(0x14);
    Func_02004ca2(0xa, 0);
    Func_02004bf0(0xa);
    Func_02004c60(0xe, 3);
    Func_02004bfe(0x1e);
    Func_02004c04(0xa);
    Func_02004cd0(0xd, 0x4000, 0);
    Func_02004cdc(0xc, 0xc000, 0);
    Func_02004c22(0x1e);
    Func_02004c8a(0xc, 3);
    Func_02004c9a(0xd, 3);
    Func_02004c38(0x1e);
    Func_02004d04(0xd, 0x8000, 0);
    Func_02004d10(0xc, 0x8000, 0);
    Func_02004c56(0x14);
    Func_02004cc6(0xa, 3);
    Func_02004c64(0x1e);
    Func_02004d4c(0xa, 1);
    Func_02004dc8(0xa, 0, 0xffffffe0);
    Func_02004394();
    Func_02004c7a(0x301);
    Func_02004d84(-1, -1, -1, 0);
    Func_02004ca0();
    Func_02004d74(0xa, 0x102, 0x28);
    Func_02004d64(0xa, 0);
    Func_02004cb2(0xa);
    Func_02004d7e(0xe, 0xb000, 0);
    Func_02004d8a(0xb, 0xb000, 0);
    Func_02004d96(0xc, 0xb000, 0);
    Func_02004da2(0xd, 0xb000, 0);
    Func_02004dae(0, 0xc000, 0);
    Func_02004dba(1, 0xc000, 0);
    Func_02004dc6(2, 0xc000, 0);
    Func_02004dd2(3, 0xc000, 0);
    Func_02004d18(0x1e);
    Func_02004df4(0xb, 0x100, 0);
    Func_02004e00(0xc, 0x100, 0);
    Func_02004e1e(0xd, 0x100, 0);
    Func_02004e2a(0xe, 0x100, 0x46);
    Func_02004e5c(0x1180000, -1, 0x1380000, 1);
    Func_02004e68();
    Func_02004d76(0xa);
    Func_02004e52(0xc, 0x102, 0x28);
    Func_02004e42(0xc, 0);
    Func_02004d90(0xa);
    Func_02004e00(0xd, 4);
    Func_02004d9e(0x14);
    Func_02004e5e(0xd, 0);
    Func_02004dac(0xa);
    Func_02004e34(2, 2);
    Func_02004dba(0x1e);
    Func_02004e86(2, 0x2000, 0);
    Func_02004dcc(0x14);
    Func_02004e8c(2, 0);
    Func_02004dda(0xa);
    Func_02004ea6(0xa, 0x4000, 0);
    Func_02004dec(0x1e);
    Func_02004eb8(3, 0xe000, 0);
    Func_02004dfe(0x1e);
    Func_02004eda(3, 0x102, 0x28);
    Func_02004ed6(0, 0x3000, 0);
    Func_02004e1c(0x14);
    Func_02004edc(3, 0);
    Func_02004e2a(0xa);
    Func_02004e9a(1, 4);
    Func_02004e38(0x14);
    Func_02004ef8(1, 0);
    Func_02004e46(0x14);
    Func_02004f20(0xe, 0x103, 0x32);
    Func_02004f1c(0xe, 0x8000, 0);
    Func_02004e62(0x14);
    Func_02004f1a(0xe, 0);
    Func_02004f34(0, 0, 0);
    Func_02004f40(0xb, 0x8000, 0);
    Func_02004f4c(0xc, 0x8000, 0);
    Func_02004f58(0xd, 0x8000, 0);
    Func_02004f64(2, 0x1000, 0);
    Func_02004f70(3, 0xe000, 0);

    if (Func_02004ed0(0, 0) != 0) {
        Func_02004ec2(0x1e);
        Func_02004f4a(0xe, 2);
        Func_02004ed0_b(0x14);
        Func_02004f90(0xe, 0);
    } else {
        Func_02004ef0(0x1e);
        Func_02004f78(0xe, 2);
        Func_02004efe(0x14);
        DIALOGUE_COUNTER++;
        Func_02004fce(0xe, 0);
        DIALOGUE_COUNTER++;
    }

    Func_02004f1c(0xa);
    Func_02004fa4(0xa, 2);
    Func_02004f2a(0x14);
    Func_02005084(0xa, 0, 0x10);
    Func_02005000(0xa, 0x2000, 0);
    Func_02004f46(0x14);
    Func_02005006(0xa, 0);
    Func_02004f54(0xa);
    Func_02005020(0xe, 0xa000, 0);
    Func_02004f66(0x14);
    Func_02004fd6(0xe, 4);
    Func_02004f74(0x14);
    Func_02005034(0xe, 0);
    Func_02004f82(0xa);
    Func_02004ff2(0xa, 3);
    Func_02004f90(0x1e);
    Func_02005050(0xa, 0);
    Func_02004f9e(0xa);
    Func_02005026(0xe, 2);
    Func_02004fac(0x14);
    Func_02005064(0xe, 0);
    Func_02004fba(0x28);
    Func_02005086(0xa, 0x5000, 0);
    Func_02004fcc(0x14);
    Func_020050a6(0xa, 0x101, 0x3c);

    if (Func_02004ff6(0, 0) != 0) {
        Func_02004fe8(0x1e);
        Func_020050b4(0xa, 0x2000, 0);
        Func_02004ffa(0x14);
        Func_0200506a(0xa, 4);
        Func_02005008(0x14);
        Func_020050c8(0xa, 0);
        DIALOGUE_COUNTER++;
    } else {
        Func_02005034(0x1e);
        Func_02005100(0xa, 0x2000, 0);
        Func_02005046(0x14);
        DIALOGUE_COUNTER++;
        Func_020050b6(0xa, 3);
        Func_02005054(0x1e);
        Func_02005124(0xa, 0);
    }

    Func_02005072(0xa);
    Func_0200514e(0xe, 0x102, 0x32);
    Func_0200513e(0xe, 0);
    Func_0200508c(0xf);
    Func_020050fc(0xa, 3);
    Func_0200509a(0x1e);
    Func_020050a0(0x14);
    Func_02005128(0xe, 2);
    Func_020050ae(0x28);
    Func_0200517a(0xe, 0x8000, 0);
    Func_020050c0(0x14);
    Func_02005180(0xe, 0);
    Func_020050ce(0xa);
    Func_0200513e(0xe, 3);
    Func_020050dc(0x1e);
    Func_0200519c(0xe, 0);
    Func_020050ea(0xa);
    Func_020051b6(0xa, 0x5000, 0);
    Func_020050fc(0x19);
    Func_02005184(0xa, 2);
    Func_0200510a(0x14);
    Func_02005158(0xa, 0x138, 0x138);
    Func_020051e2(0, 0, 0);
    Func_020051ee(0xa, 0x8000, 0);
    Func_02005134(0x19);
    Func_020051f4(0xa, 0);
    Func_02005142(0xa);
    Func_020051dc(0, 1, 0);
    Func_020051e6(3, 2, 0);
    Func_0200515c(0x1e);
    Func_020051c4(0, 3);
    Func_020051cc(1, 3);
    Func_020051d4(3, 3);
    Func_020051e4(2, 3);
    Func_02005182(0x1e);
    Func_020051b4(1, 0x13333, 0x9999);
    Func_020051be(2, 0x13333, 0x9999);
    Func_020051c8(3, 0x13333, 0x9999);
    Func_02005208(1, 2);

    record = Func_020051ce(0);
    if (record != 0) {
        Func_020051f0(1, POS_X(record), POS_Z(record));
    }
    Func_02005216(1);
    Func_02005228(1, 0, 0);
    Func_02005238(2, 2);

    record = Func_020051fe(0);
    if (record != 0) {
        Func_02005220(2, POS_X(record), POS_Z(record));
    }
    Func_02005246(2);
    Func_02005258(2, 0, 0);
    Func_02005268(3, 2);

    record = Func_0200522e(0);
    if (record != 0) {
        Func_02005250(3, POS_X(record), POS_Z(record));
    }
    Func_02005276(3);
    Func_02005288(3, 0, 0);
    Func_02005236(0xa);
    Func_02005302(0, 0xc000, 0);
    Func_0200530e(0xa, 0xb000, 0);
    Func_0200531a(0xe, 0xb000, 0);
    Func_02005326(0xb, 0xb000, 0);
    Func_02005332(0xc, 0xb000, 0);
    Func_0200533e(0xd, 0xb000, 0);
    Func_02005284(0x1e);
    Func_02005298();
}
