#include "types.h"

/*
 * Resource 3bf long cutscene script at 0x02003054.
 *
 * Complete owner: `push {r5, lr} / sub sp, #8` at 0x02003054 through
 * `add sp, #8 / pop {r5} / pop {r0} / bx r0` at 0x02004630.  r0 holds the
 * popped return address, so the owner is VOID (HANDOVER section 0's
 * interworking-epilogue rule).  The row's 5,604 bytes end at 0x02004638, which
 * is exactly where the already-converted dispatcher at 0x02004638 begins.
 *
 * Five literal pools sit inside the span and are DATA, not code:
 *   0x02003372..0x02003387   0x020039c2..0x020039df   0x02003c82..0x02003c87
 *   0x020040fc..0x02004107   0x0200460c..0x0200461b
 * Each is branched over.  They matter more than usual here, because they
 * disassemble as perfectly plausible register writes — 0x02003384 decodes as
 * `lsls r5, r0, #4`, which would silently overwrite the line cursor if the
 * listing were read as code.  Every argument below was derived with those five
 * ranges excluded.
 *
 * CONTROL FLOW.  Six branches in 5,604 bytes; everything else is straight
 * line.  Three of them test the same query, Func_0808a070(0, 0):
 *   0x02003344  two-way, joining at 0x02003494
 *   0x02003c66  two-way, joining at 0x02003c9e
 *   0x0200434c  one-sided, joining at 0x02004360
 * and three are null checks on the pointer Func_0808a080(0) has just returned.
 *
 * CALL ACCOUNTING.  All 636 `bl` sites were resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3bf 3054`, never from the
 * disassembler's pc-relative annotations.  They collapse to 34 distinct
 * targets: 33 import veneers (635 sites) and one ordinary prologue of this
 * overlay, 0x02000c90, which already has a byte-exact source under
 * assets/code.  Veneers, each named by the main-image function in its trailing
 * word:
 *   0x020055d0 Func_080091b8 x1    0x020056a0 Func_0808a0f0 x12
 *   0x020055e8 Func_080091e0 x1    0x020056a8 Func_0808a100 x65
 *   0x02005618 Func_080770c8 x2    0x020056b0 Func_0808a110 x11
 *   0x02005628 Func_0808a010 x99   0x020056c0 Func_0808a130 x19
 *   0x02005630 Func_0808a018 x1    0x020056c8 Func_0808a138 x2
 *   0x02005638 Func_0808a020 x1    0x020056d0 Func_0808a148 x79
 *   0x02005648 Func_0808a070 x3    0x020056d8 Func_0808a150 x7
 *   0x02005650 Func_0808a080 x4    0x020056e8 Func_0808a170 x78
 *   0x02005658 Func_0808a090 x18   0x020056f0 Func_0808a178 x3
 *   0x02005678 Func_0808a0b8 x3    0x020056f8 Func_0808a180 x75
 *   0x02005680 Func_0808a0c8 x31   0x02005708 Func_0808a1b8 x24
 *   0x02005690 Func_0808a0e0 x6    0x02005710 Func_0808a1e0 x3
 *   0x02005698 Func_0808a0e8 x32   0x02005718 Func_0808a1e8 x41
 *   0x02005720 Func_0808a1f0 x2    0x02005728 Func_0808a200 x1
 *   0x02005738 Func_0808a210 x2    0x02005758 Func_0808a248 x1
 *   0x02005770 Func_0808a360 x1    0x02005778 Func_0808a368 x1
 *   0x020057a8 Func_080f9010 x6
 * That is 636 sites against the inventory's `calls=577`; as HANDOVER records,
 * the field is neither a site count nor reliable, and the multiset above is
 * the completeness proof.  Every call carries its site address in a trailing
 * comment so the transcription can be checked line by line.
 *
 * THE ONE SIX-ARGUMENT CALL.  Func_080091b8 at 0x02003074 is the only site in
 * this owner with stack-carried arguments: `movs r3,#26 / movs r2,#55 /
 * str r3,[sp,#0] / str r2,[sp,#4]` fills the 8-byte frame the prologue
 * reserved, and r0-r3 are then reloaded with 16, 75, 7 and 4.  Missing those
 * two is exactly the decompiler defect HANDOVER section 0 rule 3 warns about,
 * so they are spelled out.  The frame is used for nothing else.
 *
 * LINE CURSOR.  r5 is a text-line base reloaded three times — 0x2481 at
 * 0x02003174, 0x248e at 0x020037be and 0x24a6 at 0x02003d00 — with each line
 * reached as `r5 + n`.  The line ids are written below as absolute constants.
 * As in the sibling scripts 0x02001e94 and 0x020027b0, each Func_0808a170 line
 * is followed by a Func_0808a180(actor, 0) wait.
 *
 * COORDINATE VIEW.  The three null-checked blocks read `ldrsh [ptr, #0x0a]`
 * and `ldrsh [ptr, #0x12]`.  Those are the documented halfword views of the
 * 16.16 words at +0x08 and +0x10 — the integer parts of the X and Z
 * coordinates — so the pointer is typed s16 * and indexed [5] and [9] rather
 * than given a speculative struct.  Both reads are SIGNED.
 *
 * Link base.  None of this owner's pool words falls in the proven in-image
 * band (0x0200_8xxx and above), so all are ordinary values; 0x03001ebc is the
 * IWRAM workspace pointer the rest of this overlay loads.  The stored word at
 * workspace + 448 is 515 here (built as `224 << 1` for the offset then `+ 67`
 * on the same register for the value), against 512 in 0x02001e94/0x020027b0
 * and 516 in 0x02004da0 — the three scripts hand off different phases.
 *
 * Uncertainties:
 *  - Func_0808a018, Func_0808a020 and Func_0808a368 are each called with no
 *    argument register set; they are called with none here and the leftovers
 *    are noted rather than asserted as parameters.
 *  - Func_080091e0's first argument is the pointer Func_0808a080 returned in
 *    r0; the assembly does not reload r0 between the two branches.
 *  - Func_0808a080 is called with kind 12 once and kind 0 three times, and its
 *    result is used as an opaque handle in the first case and as the s16
 *    coordinate view in the other three.  One declaration covers both, which
 *    is why it is old-style.
 */

/* Old-style declarations are mandatory in overlay sources: one name can be
 * reached with different argument counts from different sites. */
void Func_02008674();
void Func_0200867a();
void Func_02008646();
void Func_02003d0c();
void Func_020086b0();
s32 Func_020086d6();
void Func_0200873a();
void Func_020087ae();
void Func_02008754();
void Func_020087c8();
void Func_0200876e();
void Func_020087e2();
void Func_02008788();
void Func_020087fc();
void Func_020087a2();
void Func_02008816();
void Func_020087bc();
void Func_02008830();
void Func_020087d6();
void Func_0200884e();
void Func_020087ee();
void Func_020087f4();
void Func_02008868();
void Func_02008810();
void Func_020088dc();
void Func_0200879a();
void Func_02008862();
void Func_0200887a();
void Func_020088a4();
void Func_02008892();
void Func_020088bc();
void Func_020088c6();
void Func_020088d0();
void Func_020087e6();
void Func_02008898();
void Func_020087f6();
void Func_020088d4();
void Func_020088b6();
void Func_020088c0();
void Func_020088ca();
void Func_02008832();
void Func_02008896();
void Func_0200884c();
void Func_02008912();
void Func_0200892a();
void Func_02008860();
void Func_02008900();
void Func_0200892e();
void Func_02008946();
void Func_02008928();
void Func_02008932();
void Func_0200893c();
void Func_0200893e();
void Func_0200896c();
void Func_02008984();
void Func_020089ae();
void Func_020088cc();
void Func_02008954();
void Func_0200899a();
void Func_020089b2();
void Func_020089de();
void Func_0200898e();
void Func_020088fc();
void Func_020089c2();
void Func_020089da();
void Func_020089bc();
void Func_0200891a();
void Func_020089e2();
void Func_020089fa();
void Func_020089ca();
void Func_02008938();
void Func_02008a00();
void Func_02008a18();
void Func_02008a1a();
void Func_02008a2a();
s32 Func_0200898a();
void Func_020089f6();
void Func_020089fe();
void Func_02008a06();
void Func_02008a38();
void Func_02008996();
void Func_02008a76();
void Func_02008a8e();
void Func_02008a70();
void Func_02008a7a();
void Func_02008a84();
void Func_020089ec();
void Func_02008a9e();
void Func_020089fc();
void Func_02008aae();
void Func_02008a0c();
void Func_02008a9c();
void Func_02008acc();
void Func_02008aec();
void Func_02008b04();
void Func_02008a3a();
void Func_02008af6();
void Func_02008b00();
void Func_02008b0a();
void Func_02008aea();
void Func_02008af2();
void Func_02008afa();
void Func_02008b02();
void Func_02008a88();
void Func_02008b3a();
void Func_02008b44();
void Func_02008b4e();
void Func_02008b58();
void Func_02008b62();
void Func_02008af8();
void Func_02008b06();
void Func_02008b14();
void Func_02008b22();
void Func_02008b56();
void Func_02008b74();
void Func_02008b68();
void Func_02008b86();
void Func_02008b7a();
void Func_02008b98();
void Func_02008c14();
void Func_02008bb6();
void Func_02008c30();
void Func_02008bb4();
void Func_02008bd2();
void Func_02008bc6();
void Func_02008be4();
void Func_02008c60();
void Func_02008c02();
void Func_02008c7e();
void Func_02008c58();
void Func_02008c62();
void Func_02008bb8();
void Func_02008c48();
void Func_02008c50();
void Func_02008bde();
void Func_02008d64();
void Func_02008c6c();
void Func_02008c26();
void Func_02008c6a();
void Func_02008c3c();
void Func_02008c80();
void Func_02008c52();
void Func_02008c96();
void Func_02008c68();
void Func_02008cac();
void Func_02008c88();
void Func_02008ccc();
void Func_02008cd8();
void Func_02008ce6();
void Func_02008dfe();
void Func_02008c84();
void Func_02008d70();
void Func_02008d7c();
void Func_02008d88();
void Func_02008d94();
void Func_02008cba();
void Func_02008d42();
void Func_02008d4a();
void Func_02008d52();
void Func_02008d5a();
void Func_02008ce0();
void Func_02008d1a();
void Func_02008d24();
void Func_02008d2e();
void Func_02008d38();
void Func_02008d6c();
void Func_02008d8a();
void Func_02008da2();
void Func_02008e0e();
void Func_02008db2();
void Func_02008da6();
void Func_02008dc4();
void Func_02008db8();
void Func_02008dd6();
void Func_02008dee();
void Func_02008e5a();
void Func_02008dde();
void Func_02008dfc();
void Func_02008df0();
void Func_02008e1a();
void Func_02008e32();
void Func_02008e9e();
void Func_02008e34();
void Func_02008e4c();
void Func_02008eb8();
void Func_02008de4();
void Func_02008eac();
void Func_02008ec4();
void Func_02008e7c();
void Func_02008e02();
void Func_02008ea2();
void Func_02008e10();
void Func_02008e98();
void Func_02008ede();
void Func_02008ef6();
void Func_02008e2c();
void Func_02008ee6();
void Func_02008e3c();
void Func_02008f36();
void Func_02008ef8();
void Func_02008f02();
void Func_02008f20();
void Func_02008f38();
void Func_02008f64();
void Func_02008f14();
void Func_02008e82();
void Func_02008f0a();
void Func_02008e90();
void Func_02008f56();
void Func_02008f6e();
void Func_02008f26();
void Func_02008fa2();
void Func_02008f78();
void Func_02008f90();
void Func_02008f50();
void Func_02008fc2();
void Func_02008f54();
void Func_02008f96();
void Func_02008fb4();
void Func_02008fcc();
void Func_02008ffc();
void Func_02009006();
void Func_02009010();
void Func_0200901a();
void Func_02008f30();
void Func_02008fe2();
void Func_02009036();
void Func_02008ff8();
void Func_02008fd8();
void Func_0200901e();
void Func_02009060();
void Func_02008f76();
void Func_0200903c();
void Func_02009054();
void Func_02008fe8();
void Func_02009050();
void Func_02009070();
void Func_02009088();
void Func_02009058();
void Func_02008fc6();
void Func_02009078();
void Func_02008fd6();
void Func_0200909e();
void Func_020090b6();
void Func_0200908e();
void Func_02009014();
void Func_020090dc();
void Func_020090f4();
void Func_020090c4();
void Func_02009032();
void Func_020090fa();
void Func_02009112();
void Func_02009052();
void Func_0200911a();
void Func_02009132();
void Func_02009068();
void Func_02009164();
void Func_02009126();
void Func_02009146();
void Func_0200915e();
void Func_02009140();
void Func_02009160();
void Func_02009178();
void Func_0200915a();
void Func_0200916e();
void Func_020090d6();
void Func_020091d2();
void Func_02009182();
void Func_020090f0();
void Func_02009190();
void Func_020090fe();
void Func_020091c6();
void Func_020091de();
void Func_02009208();
void Func_0200917c();
void Func_0200919a();
void Func_020091b2();
void Func_020091fa();
void Func_02009212();
void Func_020091f4();
void Func_02009152();
void Func_020091f2();
void Func_02009222();
void Func_0200923a();
void Func_0200921c();
void Func_0200917a();
void Func_0200922c();
void Func_0200918a();
void Func_0200923c();
void Func_0200924c();
void Func_02009256();
void Func_02009260();
void Func_0200926a();
void Func_020091c8();
void Func_02009258();
void Func_020091d6();
void Func_0200929e();
void Func_020092b6();
void Func_02009286();
void Func_020092bc();
void Func_020092d4();
void Func_02009300();
void Func_020092d8();
void Func_020092f0();
void Func_020092d2();
void Func_02009230();
void Func_020092f8();
void Func_02009308();
void Func_020092f2();
void Func_020092fc();
void Func_02009306();
void Func_02009310();
void Func_020092ee();
void Func_020092b4();
s32 Func_020092ac();
void Func_02009416();
void Func_0200935e();
void Func_02009376();
void Func_02009434();
void Func_0200937c();
void Func_02009394();
void Func_02009380();
void Func_0200938a();
void Func_0200939e();
void Func_020093a8();
void Func_02009422();
void Func_020093b2();
void Func_02009320();
void Func_020094a6();
void Func_020093ee();
void Func_02009406();
void Func_0200939c();
void Func_020093ba();
void Func_020093b0();
void Func_0200947a();
void Func_02009430();
void Func_02009448();
void Func_0200946a();
void Func_02009486();
void Func_0200941e();
void Func_020093a4();
void Func_02009482();
void Func_0200945a();
void Func_0200944a();
void Func_02009488();
void Func_020094a0();
void Func_020094cc();
void Func_02009464();
void Func_020093ea();
void Func_020094d6();
void Func_020093fc();
void Func_020094c2();
void Func_020094da();
void Func_020094aa();
void Func_020094d8();
void Func_020094f0();
void Func_02009426();
void Func_020094b6();
void Func_020094e0();
void Func_0200943e();
void Func_02009504();
void Func_0200951c();
void Func_020094ec();
void Func_02009550();
void Func_02009526();
void Func_0200953e();
void Func_02009568();
void Func_02009540();
void Func_02009558();
void Func_02009518();
void Func_02009570();
void Func_020095a6();
void Func_020094b4();
void Func_020095b4();
void Func_02009520();
void Func_02009580();
void Func_020094de();
void Func_02009590();
void Func_020095b0();
void Func_020095c8();
void Func_02009598();
void Func_020095e0();
void Func_02009516();
void Func_02009524();
void Func_020095ec();
void Func_02009604();
void Func_020095bc();
void Func_02009638();
void Func_02009610();
void Func_02009628();
void Func_02009654();
void Func_0200962c();
void Func_02009644();
void Func_020095fc();
void Func_02009582();
void Func_0200960a();
void Func_0200964a();
void Func_02009622();
void Func_0200962a();
void Func_02009632();
void Func_0200963a();
void Func_020095c0();
void Func_020096bc();
void Func_02009694();
void Func_020096ac();
void Func_020096d6();
void Func_020096ae();
void Func_020096c6();
void Func_020096b0();
void Func_020096ba();
void Func_020096c2();
void Func_02009620();
void Func_020096d2();
void Func_020096dc();
void Func_020096e6();
void Func_020096f0();
void Func_0200964e();
void Func_02009716();
void Func_0200972e();
void Func_02009758();
void Func_02009762();
void Func_0200976c();
void Func_02009776();
void Func_0200968c();
void Func_02009754();
void Func_0200973c();
void Func_020096aa();
void Func_02009772();
void Func_0200978a();
void Func_020096ca();
void Func_02009792();
void Func_020097aa();
void Func_020097d4();
void Func_020097de();
void Func_020097e8();
void Func_020097f2();
void Func_02009708();
void Func_02009790();
void Func_02009804();
void Func_02009830();
void Func_02009808();
void Func_02009820();
void Func_020097d8();
void Func_020097e0();
void Func_020097f0();
void Func_020097f8();
void Func_0200977e();
void Func_02009806();
void Func_0200978c();
void Func_02009854();
void Func_0200986c();
void Func_02009898();
void Func_02009870();
void Func_02009888();
void Func_0200986a();
void Func_020097c8();
void Func_02009868();
void Func_020097d6();
void Func_0200989e();
void Func_020098b6();
void Func_0200986e();
void Func_020097f4();
void Func_020098bc();
void Func_020098d4();
void Func_0200988c();
void Func_020098ec();
void Func_020098ce();
void Func_02009834();
void Func_020098fc();
void Func_02009914();
void Func_0200993e();
void Func_02009916();
void Func_0200992e();
void Func_0200994a();
void Func_02009938();
void Func_02009950();
void Func_02009920();
void Func_0200988e();
void Func_02009956();
void Func_0200996e();
void Func_020098ae();
void Func_02009984();
void Func_0200999c();
void Func_020099c8();
void Func_020099a0();
void Func_020099b8();
void Func_0200999a();
void Func_020098f8();
void Func_020099c0();
void Func_020099d8();
void Func_02009a04();
void Func_020099dc();
void Func_020099f4();
void Func_020099b4();
void Func_02009a0c();
void Func_020099ee();
void Func_0200994c();
void Func_020099d4();
void Func_0200995a();
void Func_02009a22();
void Func_02009a32();
s32 Func_02009992();
void Func_02009a3e();
void Func_02009a56();
void Func_0200998c();
void Func_02009a14();
void Func_02009a1c();
void Func_02009a24();
void Func_020099aa();
void Func_02009a64();
void Func_02009a3c();
void Func_02009a44();
void Func_020099ca();
void Func_02009a52();
s32 Func_02009a00();
void Func_02009a3a();
void Func_02009a60();
void Func_02009a72();
void Func_02009a82();
s32 Func_02009a30();
void Func_02009a6a();
void Func_02009a90();
void Func_02009aa2();
void Func_02009ab2();
s32 Func_02009a60_b();
void Func_02009a9a();
void Func_02009ac0();
void Func_02009ad2();
void Func_02009ace();
void Func_02009aec();
void Func_02009b04();
void Func_02009b70();
void Func_02009b42();
void Func_02009b22();
void Func_02009aa8();
void Func_02009b5a();
void Func_02009ab8();
void Func_02009b80();
void Func_02009b98();
void Func_02009bb0();
void Func_02009b68();
void Func_02009aee();
void Func_02009b2c();
void Func_02009b3a();
void Func_02009c12();
void Func_02009b76();
void Func_02009b24();
void Func_02009c14();
void Func_02009c1c();
void Func_02009bb6();
void Func_02009bf8();
void Func_02009bc6();
void Func_02009c08();
void Func_02009c4c();
void Func_02009bf4();
void Func_02009b7a();
void Func_02009c02();
void Func_02009b88();
void Func_02009c10();
void Func_02009b96();
void Func_02009bfa();
void Func_02009c06();
void Func_02009c24();
void Func_02009c18();
void Func_02009c36();
void Func_02009c2c();
void Func_02009c4a();
void Func_02009c40();
void Func_02009c5e();
void Func_02009c54();
void Func_02009c72();
void Func_02009c68();
void Func_02009c16();
void Func_02009d9c();
void Func_02009d7e();
void Func_02009c46();
void Func_02009d82();
void Func_02009c66();

                     

                     

                    

                     

                     

                     

                     

                     

                     

                     

                     

void Func_02003054(void)
{
    s16 *actor;

    Func_02008674(0x301);   /* 200305a */
    Func_0200867a(0x941);   /* 2003060 */

    /* Six-argument call: r0-r3 plus 26 and 55 spilled to sp+0 and sp+4 by the
     * `str r3,[sp,#0] / str r2,[sp,#4]` pair at 0x02003068. */
    Func_02008646(0x10, 0x4b, 7, 4, 26, 55);   /* 2003074 */
    Func_02003d0c(4);   /* 200307a */
    Func_020086b0();   /* 200307e */
    actor = Func_020086d6(0xc);   /* 2003084 */
    Func_02008674(actor, 1);   /* 200308a */
    Func_0200873a(0, 0x1c80000, 0xb80000);   /* 2003098 */
    Func_020087ae(0, 0x4000, 0);   /* 20030a4 */
    Func_02008754(1, 0x1b80000, 0xc00000);   /* 20030b2 */
    Func_020087c8(1, 0x4000, 0);   /* 20030be */
    Func_0200876e(3, 0x1e80000, 0xb80000);   /* 20030cc */
    Func_020087e2(3, 0x4000, 0);   /* 20030d8 */
    Func_02008788(2, 0x1d80000, 0xb80000);   /* 20030e6 */
    Func_020087fc(2, 0x4000, 0);   /* 20030f2 */
    Func_020087a2(0xc, 0x2080000, 0xe00000);   /* 2003100 */
    Func_02008816(0xc, 0xb000, 0);   /* 200310c */
    Func_020087bc(0xb, 0x1c00000, 0xed0000);   /* 200311a */
    Func_02008830(0xb, 0x8000, 0);   /* 2003126 */
    Func_020087d6(0xf, 0x1c00000, 0xee0000);   /* 2003134 */
    Func_0200884e(0xf, 3);   /* 200313c */
    Func_020087ee(0xb, 3);   /* 2003144 */
    Func_020087f4(0xd, 0x1ca0000, 0xf30000);   /* 2003152 */
    Func_02008868(0xd, 0x4000, 0);   /* 200315e */
    Func_02008810(0xd, 5);   /* 2003166 */
    Func_020088dc();   /* 200316a */
    Func_0200879a(0x78);   /* 2003170 */
    Func_02008862(0x2481);   /* 2003178 */
    Func_0200887a(0xd, 0);   /* 2003180 */
    Func_020088a4(1, 0x101, 0x3c);   /* 200318a */
    Func_0200887a(0x2482);   /* 2003190 */
    Func_02008892(1, 0);   /* 2003198 */
    Func_020088bc(0, 0x101, 1);   /* 20031a2 */
    Func_020088c6(2, 0x101, 1);   /* 20031ac */
    Func_020088d0(3, 0x101, 1);   /* 20031b6 */
    Func_020087e6(0x3c);   /* 20031bc */
    Func_02008898(0xc, 0xd, 0);   /* 20031c6 */
    Func_020087f6(0x3c);   /* 20031cc */
    Func_020088bc(0x2483);   /* 20031d2 */
    Func_020088d4(0xc, 0);   /* 20031da */
    Func_020088b6(0, 0xc, 0);   /* 20031e4 */
    Func_020088c0(1, 0xc, 0);   /* 20031ee */
    Func_020088ca(2, 0xc, 0);   /* 20031f8 */
    Func_020088d4(3, 0xc, 0);   /* 2003202 */
    Func_02008832(0x3c);   /* 2003208 */
    Func_02008896(0xc, 0x200, 0xe8);   /* 2003214 */
    Func_020088c6(0xc, 4);   /* 200321c */
    Func_0200884c(0x3c);   /* 2003222 */
    Func_02008912(0x2484);   /* 2003228 */
    Func_0200892a(0xc, 0);   /* 2003230 */
    Func_02008860(0xf);   /* 2003236 */
    Func_02008900(0xd, 2);   /* 200323e */
    Func_0200892e(0x2485);   /* 2003244 */
    Func_02008946(0xd, 0);   /* 200324c */
    Func_02008928(0, 0xd, 0);   /* 2003256 */
    Func_02008932(2, 0xd, 0);   /* 2003260 */
    Func_0200893c(3, 0xd, 0);   /* 200326a */
    Func_02008946(1, 0xd, 0);   /* 2003274 */
    Func_0200893e(2, 1);   /* 200327c */
    Func_0200896c(0x2486);   /* 2003282 */
    Func_02008984(2, 0);   /* 200328a */
    Func_020089ae(3, 0x105, 0);   /* 2003294 */
    Func_02008946(3, 4);   /* 200329c */
    Func_020088cc(0x50);   /* 20032a2 */
    Func_02008954(3, 1);   /* 20032aa */
    Func_0200899a(0x2487);   /* 20032b0 */
    Func_020089b2(3, 0);   /* 20032b8 */
    Func_020089de(0xc, 0x102, 0x41);   /* 20032c4 */
    Func_0200898e(0xc, 2);   /* 20032cc */
    Func_020088fc(0x64);   /* 20032d2 */
    Func_020089c2(0x2488);   /* 20032d8 */
    Func_020089da(0xd, 0);   /* 20032e0 */
    Func_020089bc(1, 0, 0);   /* 20032ea */
    Func_0200891a(0xa);   /* 20032f0 */
    Func_020089e2(0x2489);   /* 20032f8 */
    Func_020089fa(1, 0);   /* 2003300 */
    Func_020089ca(2, 1);   /* 2003308 */
    Func_02008938(0x1e);   /* 200330e */
    Func_02008a00(0x248a);   /* 2003316 */
    Func_02008a18(2, 0);   /* 200331e */
    Func_020089fa(3, 0, 0);   /* 2003328 */
    Func_02008a1a(0x248b);   /* 2003330 */
    Func_02008a2a(3, 0);   /* 2003338 */

    if (Func_0200898a(0, 0) != 0) {   /* 2003340 */
        Func_020089f6(0x248c);   /* 200338c */
        Func_020089fe(3, 0);   /* 2003394 */
        Func_02008a06(0, 0xc, 0);   /* 200339e */
        Func_02008a38(1, 0xc, 0);   /* 20033a8 */
        Func_02008996(2, 0xc, 0);   /* 20033b2 */
        Func_02008a76(3, 0xc, 0);   /* 20033bc */
        Func_02008a8e(0x14);   /* 20033c2 */
        Func_02008a70(0xc, 0, 0);   /* 20033cc */
        Func_02008a7a(0x3c);   /* 20033d2 */
        Func_02008a84(0xc, 0xd, 0);   /* 20033dc */
        Func_02008a8e(0x50);   /* 20033e2 */
        Func_020089ec(0xc, 3);   /* 20033ea */
        Func_02008a9e(0x1e);   /* 20033f0 */
        Func_020089fc(0xc, 0, 0);   /* 20033fa */
        Func_02008aae(0x248d);   /* 2003402 */
        Func_02008a0c(0xc, 0);   /* 200340a */
        Func_02008a9c(0x3c);   /* 2003410 */
        Func_02008a1a(2, 0xc, 0);   /* 200341a */
        Func_02008acc(1, 0xc, 0);   /* 2003424 */
        Func_02008aec(3, 0xc, 0);   /* 200342e */
        Func_02008b04(0, 0xc, 0);   /* 2003438 */
        Func_02008a3a(2, 3);   /* 2003440 */
        Func_02008aec(1, 3);   /* 2003448 */
        Func_02008af6(3, 3);   /* 2003450 */
        Func_02008b00(0, 3);   /* 2003458 */
        Func_02008b0a(0x50);   /* 200345e */
        Func_02008aea(0xc, 0xd, 0);   /* 2003468 */
        Func_02008af2(2, 0xd, 0);   /* 2003472 */
        Func_02008afa(1, 0xd, 0);   /* 200347c */
        Func_02008b02(3, 0xd, 0);   /* 2003486 */
        Func_02008a88(0, 0xd, 0);   /* 2003490 */
    } else {
    }
        Func_02008b3a(2, 3);   /* 200334c */
        Func_02008b44(1, 3);   /* 2003354 */
        Func_02008b4e(3, 3);   /* 200335c */
        Func_02008b58(0xc, 0xd, 0);   /* 2003366 */
        Func_02008b62(0x3c);   /* 200336c */

    Func_02008af8(0, 0x8000, 0x4000);   /* 200349e */
    Func_02008b06(1, 0x8000, 0x4000);   /* 20034ac */
    Func_02008b14(3, 0x8000, 0x4000);   /* 20034ba */
    Func_02008b22(2, 0x8000, 0x4000);   /* 20034c8 */
    Func_02008b56(1, 0x1a0, 0xd8);   /* 20034d4 */
    Func_02008b74(1);   /* 20034da */
    Func_02008b68(1, 0x1a0, 0xf8);   /* 20034e6 */
    Func_02008b86(1);   /* 20034ec */
    Func_02008b7a(1, 0x1b8, 0xf8);   /* 20034f8 */
    Func_02008b98(1);   /* 20034fe */
    Func_02008c14(1, 0xc000, 0);   /* 200350a */
    Func_02008b98(0, 0x1b8, 0xd8);   /* 2003516 */
    Func_02008bb6(0);   /* 200351c */
    Func_02008c30(0, 0, 0);   /* 2003526 */
    Func_02008bb4(3, 0x1e8, 0xf8);   /* 2003532 */
    Func_02008bd2(3);   /* 2003538 */
    Func_02008bc6(3, 0x1c8, 0xf8);   /* 2003544 */
    Func_02008be4(3);   /* 200354a */
    Func_02008c60(3, 0xc000, 0);   /* 2003556 */
    Func_02008be4(2, 0x1c8, 0xd8);   /* 2003562 */
    Func_02008c02(2);   /* 2003568 */
    Func_02008c7e(2, 0x8000, 0);   /* 2003574 */
    Func_02008c58(1, 0, 0);   /* 200357e */
    Func_02008c62(3, 2, 0);   /* 2003588 */
    Func_02008bb8(0x3c);   /* 200358e */
    Func_02008c48(0, 3);   /* 2003596 */
    Func_02008c48(2, 3);   /* 200359e */
    Func_02008c50(1, 3);   /* 20035a6 */
    Func_02008c58(3, 3);   /* 20035ae */
    Func_02008bde(0x64);   /* 20035b4 */
    Func_02008d64(0xe2);   /* 20035ba */
    Func_02008c6c(0xd, 7);   /* 20035c2 */
    Func_02008c26(0, 0x1999, 0xccc);   /* 20035cc */
    Func_02008c6a(0, -24, 0);   /* 20035d8 */
    Func_02008c3c(1, 0x1999, 0xccc);   /* 20035e2 */
    Func_02008c80(1, -24, 0);   /* 20035ee */
    Func_02008c52(3, 0x1999, 0xccc);   /* 20035f8 */
    Func_02008c96(3, -24, 0);   /* 2003604 */
    Func_02008c68(2, 0x1999, 0xccc);   /* 200360e */
    Func_02008cac(2, -24, 0);   /* 200361a */
    Func_02008c7e(0xb, 0x1999, 0xccc);   /* 2003624 */
    Func_02008c88(0xf, 0x1999, 0xccc);   /* 200362e */
    Func_02008ccc(0xb, -24, 0);   /* 200363a */
    Func_02008cd8(0xf, -24, 0);   /* 2003646 */
    Func_02008ce6(0);   /* 200364c */
    Func_02008dfe(0x120);   /* 2003654 */
    Func_02008c84(0x3c);   /* 200365a */
    Func_02008d70(0, 0x4000, 0);   /* 2003666 */
    Func_02008d7c(1, 0xc000, 0);   /* 2003672 */
    Func_02008d88(2, 0x4000, 0);   /* 200367e */
    Func_02008d94(3, 0xc000, 0);   /* 200368a */
    Func_02008cba(0x3c);   /* 2003690 */
    Func_02008d42(0, 3);   /* 2003698 */
    Func_02008d4a(1, 3);   /* 20036a0 */
    Func_02008d52(2, 3);   /* 20036a8 */
    Func_02008d5a(3, 3);   /* 20036b0 */
    Func_02008ce0(0x64);   /* 20036b6 */
    Func_02008d1a(0, 0xb333, 0x5999);   /* 20036c0 */
    Func_02008d24(1, 0xb333, 0x5999);   /* 20036ca */
    Func_02008d2e(2, 0xb333, 0x5999);   /* 20036d4 */
    Func_02008d38(3, 0xb333, 0x5999);   /* 20036de */
    Func_02008d6c(0, 0x1c8, 0xb8);   /* 20036ea */
    Func_02008d8a(0);   /* 20036f0 */
    Func_02008da2(0, 1);   /* 20036f8 */
    Func_02008e0e(0, 0x4000, 0);   /* 2003704 */
    Func_02008d94(1, 0x1d0, 0x100);   /* 2003712 */
    Func_02008db2(1);   /* 2003718 */
    Func_02008da6(1, 0x1e0, 0xf8);   /* 2003724 */
    Func_02008dc4(1);   /* 200372a */
    Func_02008db8(1, 0x1b8, 0xc0);   /* 2003736 */
    Func_02008dd6(1);   /* 200373c */
    Func_02008dee(1, 1);   /* 2003744 */
    Func_02008e5a(1, 0x4000, 0);   /* 2003750 */
    Func_02008dde(3, 0x1e8, 0xf8);   /* 200375c */
    Func_02008dfc(3);   /* 2003762 */
    Func_02008df0(3, 0x1e8, 0xb8);   /* 200376e */
    Func_02008dfc(2, 0x1d8, 0xb8);   /* 200377a */
    Func_02008e1a(2);   /* 2003780 */
    Func_02008e32(2, 1);   /* 2003788 */
    Func_02008e9e(2, 0x4000, 0);   /* 2003794 */
    Func_02008e34(3);   /* 200379a */
    Func_02008e4c(3, 1);   /* 20037a2 */
    Func_02008eb8(3, 0x4000, 0);   /* 20037ae */
    Func_02008dde(0x1e);   /* 20037b4 */
    Func_02008de4(0x3c);   /* 20037ba */
    Func_02008eac(0x248e);   /* 20037c2 */
    Func_02008ec4(1, 0);   /* 20037ca */
    Func_02008e7c(0xd, 6);   /* 20037d2 */
    Func_02008e02(0x78);   /* 20037d8 */
    Func_02008ea2(0xd, 2);   /* 20037e0 */
    Func_02008e10(0x3c);   /* 20037e6 */
    Func_02008e98(0xd, 7);   /* 20037ee */
    Func_02008ede(0x248f);   /* 20037f4 */
    Func_02008ef6(0xd, 0);   /* 20037fc */
    Func_02008e2c(0x14);   /* 2003802 */
    Func_02008ee6(3, 2, 0);   /* 200380c */
    Func_02008e3c(0xa);   /* 2003812 */
    Func_02008f36(3, 0x101, 0x50);   /* 200381c */
    Func_02008ef8(3, 0xd, 0);   /* 2003826 */
    Func_02008f02(2, 0xd, 0);   /* 2003830 */
    Func_02008f20(0x2490);   /* 2003836 */
    Func_02008f38(3, 0);   /* 200383e */
    Func_02008f64(0xd, 0x102, 0x46);   /* 200384a */
    Func_02008f14(0xd, 2);   /* 2003852 */
    Func_02008e82(0x3c);   /* 2003858 */
    Func_02008f0a(0xd, 5);   /* 2003860 */
    Func_02008e90(0x46);   /* 2003866 */
    Func_02008f56(0x2491);   /* 200386c */
    Func_02008f6e(0xd, 0);   /* 2003874 */
    Func_02008f26(0xd, 7);   /* 200387c */
    Func_02008fa2(2, 0x108, 0x28);   /* 2003888 */
    Func_02008f78(0x2492);   /* 200388e */
    Func_02008f90(2, 0);   /* 2003896 */
    Func_02008f50(0xc, 3);   /* 200389e */
    Func_02008fc2(1, 0x103, 0x3c);   /* 20038a8 */
    Func_02008f36(1, 0x1b8, 0xd0);   /* 20038b4 */
    Func_02008f54(1);   /* 20038ba */
    Func_02008f96(1, 0, 0);   /* 20038c4 */
    Func_02008fb4(0x2493);   /* 20038ca */
    Func_02008fcc(1, 0);   /* 20038d2 */
    Func_02008f02(0x14);   /* 20038d8 */
    Func_02008ffc(0, 0x101, 0);   /* 20038e2 */
    Func_02009006(2, 0x101, 0);   /* 20038ec */
    Func_02009010(3, 0x101, 0);   /* 20038f6 */
    Func_0200901a(0xc, 0x101, 0);   /* 2003900 */
    Func_02008f30(0x46);   /* 2003906 */
    Func_02008fe2(1, 2, 0);   /* 2003910 */
    Func_02009036(1, 0x102, 0x4b);   /* 200391c */
    Func_02008ff8(1, 0, 0);   /* 2003926 */
    Func_02008fd8(1, 4);   /* 200392e */
    Func_0200901e(0x2494);   /* 2003934 */
    Func_02009036(1, 0);   /* 200393c */
    Func_02009060(0xc, 0x101, 0);   /* 2003946 */
    Func_02008f76(0x3c);   /* 200394c */
    Func_0200903c(0x2495);   /* 2003952 */
    Func_02009054(0xc, 0);   /* 200395a */
    Func_02008fe8(1, 0x1c0, 0xd0);   /* 2003966 */
    Func_02009006(1);   /* 200396c */
    Func_0200901e(1, 1);   /* 2003974 */
    Func_02009050(1, 0xc, 0);   /* 200397e */
    Func_02009070(0x2496);   /* 2003986 */
    Func_02009088(1, 0);   /* 200398e */
    Func_02009058(0xc, 1);   /* 2003996 */
    Func_02008fc6(0x3c);   /* 200399c */
    Func_02009078(1, 0xc, 0);   /* 20039a6 */
    Func_02008fd6(0x3c);   /* 20039ac */
    Func_0200909e(0x2497);   /* 20039b4 */
    Func_020090b6(0xc, 0);   /* 20039bc */
    Func_0200908e(1, 4);   /* 20039e4 */
    Func_02009014(0x3c);   /* 20039ea */
    Func_020090dc(0x2498);   /* 20039f2 */
    Func_020090f4(1, 0);   /* 20039fa */
    Func_020090c4(2, 1);   /* 2003a02 */
    Func_02009032(0x1e);   /* 2003a08 */
    Func_020090fa(0x2499);   /* 2003a10 */
    Func_02009112(2, 0);   /* 2003a18 */
    Func_020090f4(1, 3, 0);   /* 2003a22 */
    Func_02009052(0x14);   /* 2003a28 */
    Func_0200911a(0x249a);   /* 2003a30 */
    Func_02009132(1, 0);   /* 2003a38 */
    Func_02009068(0x1e);   /* 2003a3e */
    Func_02009164(3, 0x100, 0x50);   /* 2003a4a */
    Func_02009126(3, 1, 0);   /* 2003a54 */
    Func_02009146(0x249b);   /* 2003a5c */
    Func_0200915e(3, 0);   /* 2003a64 */
    Func_02009140(3, 0xd, 0);   /* 2003a6e */
    Func_02009160(0x249c);   /* 2003a76 */
    Func_02009178(3, 0);   /* 2003a7e */
    Func_0200915a(0, 0xd, 0);   /* 2003a88 */
    Func_02009164(2, 0xd, 0);   /* 2003a92 */
    Func_0200916e(1, 0xd, 0);   /* 2003a9c */
    Func_02009178(3, 0xd, 0);   /* 2003aa6 */
    Func_020090d6(0x78);   /* 2003aac */
    Func_020091d2(0xd, 0x102, 0x1e);   /* 2003ab8 */
    Func_02009182(0xd, 1);   /* 2003ac0 */
    Func_020090f0(0x78);   /* 2003ac6 */
    Func_02009190(0xc, 1);   /* 2003ace */
    Func_020090fe(0x3c);   /* 2003ad4 */
    Func_020091c6(0x249d);   /* 2003adc */
    Func_020091de(0xc, 0);   /* 2003ae4 */
    Func_02009208(1, 0x107, 0x6e);   /* 2003aee */
    Func_0200917c(1, 0x1c8, 0xd4);   /* 2003afa */
    Func_0200919a(1);   /* 2003b00 */
    Func_020091b2(1, 1);   /* 2003b08 */
    Func_020091fa(0x249e);   /* 2003b10 */
    Func_02009212(1, 0);   /* 2003b18 */
    Func_020091f4(2, 1, 0);   /* 2003b22 */
    Func_02009152(0x3c);   /* 2003b28 */
    Func_020091f2(2, 1);   /* 2003b30 */
    Func_02009222(0x249f);   /* 2003b38 */
    Func_0200923a(2, 0);   /* 2003b40 */
    Func_0200921c(1, 0xd, 0);   /* 2003b4a */
    Func_0200917a(0x50);   /* 2003b50 */
    Func_0200922c(1, 2, 0);   /* 2003b5a */
    Func_0200918a(0x3c);   /* 2003b60 */
    Func_0200923c(1, 0xd, 0);   /* 2003b6a */
    Func_0200919a(0x1e);   /* 2003b70 */
    Func_0200924c(0, 0xd, 0);   /* 2003b7a */
    Func_02009256(3, 0xd, 0);   /* 2003b84 */
    Func_02009260(0xd, 0xd, 0);   /* 2003b8e */
    Func_0200926a(0xc, 0xd, 0);   /* 2003b98 */
    Func_020091c8(0x50);   /* 2003b9e */
    Func_02009258(1, 3);   /* 2003ba6 */
    Func_020091d6(0x1e);   /* 2003bac */
    Func_0200929e(0x24a0);   /* 2003bb4 */
    Func_020092b6(1, 0);   /* 2003bbc */
    Func_02009286(0xd, 2);   /* 2003bc4 */
    Func_020091f4(0x46);   /* 2003bca */
    Func_020092bc(0x24a1);   /* 2003bd2 */
    Func_020092d4(0xd, 0);   /* 2003bda */
    Func_02009300(3, 0x100, 0x3c);   /* 2003be6 */
    Func_020092d8(0x24a2);   /* 2003bee */
    Func_020092f0(3, 0);   /* 2003bf6 */
    Func_020092d2(2, 0, 0);   /* 2003c00 */
    Func_02009230(0x50);   /* 2003c06 */
    Func_020092f8(0x24a3);   /* 2003c0e */
    Func_02009308(2, 0);   /* 2003c16 */
    Func_020092f2(1, 0, 0);   /* 2003c20 */
    Func_020092fc(3, 0, 0);   /* 2003c2a */
    Func_02009306(2, 0, 0);   /* 2003c34 */
    Func_02009310(0xc, 0, 0);   /* 2003c3e */
    Func_020092ee(0xe, 0x1c80000, 0x1300000);   /* 2003c4c */
    Func_020092b4(0xe, 0x8000, 0x4000);   /* 2003c5a */

    if (Func_020092ac(0, 0) != 0) {   /* 2003c62 */
        Func_02009416(0x13);   /* 2003c8a */
        Func_0200935e(0x24a5);   /* 2003c92 */
        Func_02009376(0xe, 0);   /* 2003c9a */
    } else {
    }
        Func_02009434(0x13);   /* 2003c6c */
        Func_0200937c(0x24a4);   /* 2003c74 */
        Func_02009394(0xe, 0);   /* 2003c7c */

    Func_02009376(0, 0xe, 0);   /* 2003ca4 */
    Func_02009380(1, 0xe, 0);   /* 2003cae */
    Func_0200938a(3, 0xe, 0);   /* 2003cb8 */
    Func_02009394(2, 0xe, 0);   /* 2003cc2 */
    Func_0200939e(0xc, 0xe, 0);   /* 2003ccc */
    Func_020093a8(0xd, 0xe, 0);   /* 2003cd6 */
    Func_02009422(0x1c80000, -1, 0xf00000, 1);   /* 2003ce8 */
    Func_020093b2(0xd, 1);   /* 2003cf0 */
    Func_02009320(0x3c);   /* 2003cf6 */
    Func_020094a6(8);   /* 2003cfc */
    Func_020093ee(0x24a6);   /* 2003d04 */
    Func_02009406(0xd, 0);   /* 2003d0c */
    Func_0200939c(0xe, 0x1c8, 0x118);   /* 2003d1a */
    Func_020093ba(0xe);   /* 2003d20 */
    Func_020093b0(0xe, 0x1b8, 0x100);   /* 2003d2e */
    Func_0200947a(0x1c80000, -1, 0xe00000, 1);   /* 2003d40 */
    Func_02009430(0x24a7);   /* 2003d46 */
    Func_02009448(0xe, 0);   /* 2003d4e */
    Func_020093ee(0xe);   /* 2003d54 */
    Func_0200946a(0xe, 0xd000, 0);   /* 2003d60 */
    Func_02009486(0xd, 0x102, 0x50);   /* 2003d6c */
    Func_0200941e(0xe, 4);   /* 2003d74 */
    Func_020093a4(0x59);   /* 2003d7a */
    Func_0200946a(0x24a8);   /* 2003d80 */
    Func_02009482(0xe, 0);   /* 2003d88 */
    Func_0200945a(0xd, 2);   /* 2003d90 */
    Func_0200944a(0xe, 3);   /* 2003d98 */
    Func_02009488(0x24a9);   /* 2003d9e */
    Func_020094a0(0xe, 0);   /* 2003da6 */
    Func_020094cc(0xd, 0x100, 0x50);   /* 2003db2 */
    Func_02009464(0xe, 4);   /* 2003dba */
    Func_020093ea(0x50);   /* 2003dc0 */
    Func_020094d6(0xe, 0x3000, 0);   /* 2003dcc */
    Func_020093fc(0x14);   /* 2003dd2 */
    Func_020094c2(0x24aa);   /* 2003dd8 */
    Func_020094da(0xe, 0);   /* 2003de0 */
    Func_020094aa(0xd, 1);   /* 2003de8 */
    Func_020094d8(0x24ab);   /* 2003dee */
    Func_020094f0(0xd, 0);   /* 2003df6 */
    Func_02009426(0x1e);   /* 2003dfc */
    Func_020094b6(0xe, 3);   /* 2003e04 */
    Func_020094e0(0xe, 0xd, 0);   /* 2003e0e */
    Func_0200943e(0x14);   /* 2003e14 */
    Func_02009504(0x24ac);   /* 2003e1a */
    Func_0200951c(0xe, 0);   /* 2003e22 */
    Func_020094ec(0xd, 1);   /* 2003e2a */
    Func_02009550(0xd, 0x102, 0x50);   /* 2003e36 */
    Func_02009526(0x24ad);   /* 2003e3c */
    Func_0200953e(0xd, 0);   /* 2003e44 */
    Func_02009568(0xe, 0x103, 0x3c);   /* 2003e4e */
    Func_02009540(0x24ae);   /* 2003e56 */
    Func_02009558(0xe, 0);   /* 2003e5e */
    Func_02009518(0xe, 3);   /* 2003e66 */
    Func_02009558(0x24af);   /* 2003e6e */
    Func_02009570(0xe, 0);   /* 2003e76 */
    Func_020094a6(0x14);   /* 2003e7c */
    Func_020095a6(0xd, 0x101);   /* 2003e84 */
    Func_020094b4(0x50);   /* 2003e8a */
    Func_020095b4(0xd, 0);   /* 2003e92 */
    Func_02009520(0xe, 0x1f0, 0xf0);   /* 2003e9e */
    Func_0200953e(0xe);   /* 2003ea4 */
    Func_02009580(0xe, 0xc, 0);   /* 2003eae */
    Func_020094de(0x14);   /* 2003eb4 */
    Func_02009590(0xc, 0xe, 0);   /* 2003ebe */
    Func_020095b0(0x24b0);   /* 2003ec6 */
    Func_020095c8(0xe, 0);   /* 2003ece */
    Func_02009598(0xc, 1);   /* 2003ed6 */
    Func_020095c8(0x24b1);   /* 2003ede */
    Func_020095e0(0xc, 0);   /* 2003ee6 */
    Func_02009516(0x28);   /* 2003eec */
    Func_020095a6(0xe, 3);   /* 2003ef4 */
    Func_02009524(0x14);   /* 2003efa */
    Func_020095ec(0x24b2);   /* 2003f02 */
    Func_02009604(0xe, 0);   /* 2003f0a */
    Func_020095bc(0xe, 3);   /* 2003f12 */
    Func_02009638(0xc, 0x102, 0x3c);   /* 2003f1e */
    Func_02009610(0x24b3);   /* 2003f26 */
    Func_02009628(0xc, 0);   /* 2003f2e */
    Func_02009654(0xe, 0x100, 0x46);   /* 2003f3a */
    Func_0200962c(0x24b4);   /* 2003f42 */
    Func_02009644(0xe, 0);   /* 2003f4a */
    Func_020095fc(0xc, 3);   /* 2003f52 */
    Func_02009582(0x8c);   /* 2003f58 */
    Func_0200960a(0xe, 3);   /* 2003f60 */
    Func_02009590(0x78);   /* 2003f66 */
    Func_0200964a(0, 0xe, 0);   /* 2003f70 */
    Func_02009622(0, 3);   /* 2003f78 */
    Func_0200962a(2, 3);   /* 2003f80 */
    Func_02009632(1, 3);   /* 2003f88 */
    Func_0200963a(3, 3);   /* 2003f90 */
    Func_020095c0(0x78);   /* 2003f96 */
    Func_020096bc(0xe, 0x108, 0xb4);   /* 2003fa2 */
    Func_02009694(0x24b5);   /* 2003faa */
    Func_020096ac(0xe, 0);   /* 2003fb2 */
    Func_020096d6(0xc, 0x101, 0x50);   /* 2003fbc */
    Func_020096ae(0x24b6);   /* 2003fc4 */
    Func_020096c6(0xc, 0);   /* 2003fcc */
    Func_020096b0(0, 1, 0);   /* 2003fd6 */
    Func_020096ba(3, 2, 0);   /* 2003fe0 */
    Func_02009610(0x50);   /* 2003fe6 */
    Func_020096c2(0xe, 0, 0);   /* 2003ff0 */
    Func_02009620(0x3c);   /* 2003ff6 */
    Func_020096d2(0, 0xe, 0);   /* 2004000 */
    Func_020096dc(1, 0xe, 0);   /* 200400a */
    Func_020096e6(2, 0xe, 0);   /* 2004014 */
    Func_020096f0(3, 0xe, 0);   /* 200401e */
    Func_0200964e(0x3c);   /* 2004024 */
    Func_02009716(0x24b7);   /* 200402c */
    Func_0200972e(0xe, 0);   /* 2004034 */
    Func_02009758(0, 0x101, 0);   /* 200403e */
    Func_02009762(1, 0x101, 0);   /* 2004048 */
    Func_0200976c(2, 0x101, 0);   /* 2004052 */
    Func_02009776(3, 0x101, 0);   /* 200405c */
    Func_0200968c(0x64);   /* 2004062 */
    Func_02009754(0x24b8);   /* 200406a */
    Func_0200976c(1, 0);   /* 2004072 */
    Func_0200973c(2, 1);   /* 200407a */
    Func_020096aa(0x3c);   /* 2004080 */
    Func_02009772(0x24b9);   /* 2004088 */
    Func_0200978a(2, 0);   /* 2004090 */
    Func_0200976c(0xe, 2, 0);   /* 200409a */
    Func_020096ca(0x14);   /* 20040a0 */
    Func_02009792(0x24ba);   /* 20040a8 */
    Func_020097aa(0xe, 0);   /* 20040b0 */
    Func_020097d4(0, 0x101, 0);   /* 20040ba */
    Func_020097de(1, 0x101, 0);   /* 20040c4 */
    Func_020097e8(2, 0x101, 0);   /* 20040ce */
    Func_020097f2(3, 0x101, 0);   /* 20040d8 */
    Func_02009708(0x78);   /* 20040de */
    Func_02009790(0xe, 4);   /* 20040e6 */
    Func_02009716(0x78);   /* 20040ec */
    Func_020097de(0x24bb);   /* 20040f4 */
    Func_02009804(0xe, 0);   /* 200410a */
    Func_02009830(0xe, 0x102, 0x5a);   /* 2004116 */
    Func_02009808(0x24bc);   /* 200411e */
    Func_02009820(0xe, 0);   /* 2004126 */
    Func_020097d8(0, 3);   /* 200412e */
    Func_020097e0(2, 3);   /* 2004136 */
    Func_020097e8(3, 3);   /* 200413e */
    Func_020097f0(1, 3);   /* 2004146 */
    Func_020097f8(0xc, 3);   /* 200414e */
    Func_0200977e(0x50);   /* 2004154 */
    Func_02009806(0xe, 4);   /* 200415c */
    Func_0200978c(0x78);   /* 2004162 */
    Func_02009854(0x24bd);   /* 200416a */
    Func_0200986c(0xe, 0);   /* 2004172 */
    Func_02009898(0xc, 0x100, 0x3c);   /* 200417e */
    Func_02009870(0x24be);   /* 2004186 */
    Func_02009888(0xc, 0);   /* 200418e */
    Func_0200986a(0xe, 0xc, 0);   /* 2004198 */
    Func_020097c8(0x14);   /* 200419e */
    Func_02009868(0xe, 1);   /* 20041a6 */
    Func_020097d6(0x32);   /* 20041ac */
    Func_0200989e(0x24bf);   /* 20041b4 */
    Func_020098b6(0xe, 0);   /* 20041bc */
    Func_0200986e(1, 3);   /* 20041c4 */
    Func_020097f4(0x46);   /* 20041ca */
    Func_020098bc(0x24c0);   /* 20041d2 */
    Func_020098d4(1, 0);   /* 20041da */
    Func_0200988c(3, 3);   /* 20041e2 */
    Func_020098d4(0x24c1);   /* 20041ea */
    Func_020098ec(3, 0);   /* 20041f2 */
    Func_020098ce(0xe, 0, 0);   /* 20041fc */
    Func_020098b6(0xe, 3);   /* 2004204 */
    Func_02009834(0x14);   /* 200420a */
    Func_020098fc(0x24c2);   /* 2004212 */
    Func_02009914(0xe, 0);   /* 200421a */
    Func_0200993e(2, 0x101, 0x5a);   /* 2004224 */
    Func_02009916(0x24c3);   /* 200422c */
    Func_0200992e(2, 0);   /* 2004234 */
    Func_0200994a(0xe, 0x5000, 0);   /* 2004240 */
    Func_02009870(0x14);   /* 2004246 */
    Func_02009938(0x24c4);   /* 200424e */
    Func_02009950(0xe, 0);   /* 2004256 */
    Func_02009920(0xc, 1);   /* 200425e */
    Func_0200988e(0x3c);   /* 2004264 */
    Func_02009956(0x24c5);   /* 200426c */
    Func_0200996e(0xc, 0);   /* 2004274 */
    Func_02009950(0xe, 0xc, 0);   /* 200427e */
    Func_020098ae(0x14);   /* 2004284 */
    Func_0200993e(0xe, 3);   /* 200428c */
    Func_020098bc(0x14);   /* 2004292 */
    Func_02009984(0x24c6);   /* 200429a */
    Func_0200999c(0xe, 0);   /* 20042a2 */
    Func_020099c8(1, 0x100, 0x46);   /* 20042ae */
    Func_020099a0(0x24c7);   /* 20042b6 */
    Func_020099b8(1, 0);   /* 20042be */
    Func_0200999a(0xe, 0, 0);   /* 20042c8 */
    Func_020098f8(0x1e);   /* 20042ce */
    Func_020099c0(0x24c8);   /* 20042d6 */
    Func_020099d8(0xe, 0);   /* 20042de */
    Func_02009a04(3, 0x100, 0x50);   /* 20042ea */
    Func_020099dc(0x24c9);   /* 20042f2 */
    Func_020099f4(3, 0);   /* 20042fa */
    Func_020099b4(0xe, 3);   /* 2004302 */
    Func_020099f4(0x24ca);   /* 200430a */
    Func_02009a0c(0xe, 0);   /* 2004312 */
    Func_020099ee(2, 0, 0);   /* 200431c */
    Func_0200994c(0x1e);   /* 2004322 */
    Func_020099d4(2, 3);   /* 200432a */
    Func_0200995a(0x64);   /* 2004330 */
    Func_02009a22(0x24cb);   /* 2004338 */
    Func_02009a32(2, 0);   /* 2004340 */

    if (Func_02009992(0, 0) != 0) {   /* 2004348 */
    }
        Func_02009a3e(0x24cc);   /* 2004354 */
        Func_02009a56(0xe, 0);   /* 200435c */

    Func_0200998c(0x1e);   /* 2004362 */
    Func_02009a14(1, 3);   /* 200436a */
    Func_02009a1c(2, 3);   /* 2004372 */
    Func_02009a24(3, 3);   /* 200437a */
    Func_020099aa(0x64);   /* 2004380 */
    Func_02009a64(0, 0xc, 0);   /* 200438a */
    Func_02009a3c(0, 3);   /* 2004392 */
    Func_02009a44(0xc, 3);   /* 200439a */
    Func_020099ca(0x64);   /* 20043a0 */
    Func_02009a52(1, 2);   /* 20043a8 */
    actor = Func_02009a00(0);   /* 20043ae */
    if (actor != 0) {
        Func_02009a3a(1, actor[5], actor[9]);   /* 20043c0 */
    }
    Func_02009a60(1);   /* 20043c6 */
    Func_02009a72(1, 0, 0);   /* 20043d0 */
    Func_02009a82(2, 2);   /* 20043d8 */
    actor = Func_02009a30(0);   /* 20043de */
    if (actor != 0) {
        Func_02009a6a(2, actor[5], actor[9]);   /* 20043f0 */
    }
    Func_02009a90(2);   /* 20043f6 */
    Func_02009aa2(2, 0, 0);   /* 2004400 */
    Func_02009ab2(3, 2);   /* 2004408 */
    actor = Func_02009a60_b(0);   /* 200440e */
    if (actor != 0) {
        Func_02009a9a(3, actor[5], actor[9]);   /* 2004420 */
    }
    Func_02009ac0(3);   /* 2004426 */
    Func_02009ad2(3, 0, 0);   /* 2004430 */
    Func_02009a60(0x1e);   /* 2004436 */
    Func_02009a9a(0xc, 0x6666, 0x3333);   /* 2004440 */
    Func_02009ace(0xc, 0x1d8, 0xb8);   /* 200444c */
    Func_02009aec(0xc);   /* 2004452 */
    Func_02009b04(0xc, 1);   /* 200445a */
    Func_02009b70(0xc, 0x3000, 0);   /* 2004466 */
    Func_02009b42(0xe, 0, 0);   /* 2004470 */
    Func_02009b22(0xe, 3);   /* 2004478 */
    Func_02009aa8(0x14);   /* 200447e */
    Func_02009b5a(0xe, 0xd, 0);   /* 2004488 */
    Func_02009ab8(0x14);   /* 200448e */
    Func_02009b80(0x24cd);   /* 2004496 */
    Func_02009b98(0xe, 0);   /* 200449e */
    Func_02009b70(0xd, 2);   /* 20044a6 */
    Func_02009b98(0x24ce);   /* 20044ae */
    Func_02009bb0(0xd, 0);   /* 20044b6 */
    Func_02009b68(0, 3);   /* 20044be */
    Func_02009aee(0x1e);   /* 20044c4 */
    Func_02009b2c(0, 0x10000, 0x8000);   /* 20044d2 */
    Func_02009b3a(0xc, 0x10000, 0x8000);   /* 20044e0 */
    Func_02009c12(0, 1);   /* 20044e8 */
    Func_02009b76(0, 0x1e0, 0xf8);   /* 20044f4 */
    Func_02009b24(0x28);   /* 20044fa */
    Func_02009c14(0xc, 0);   /* 2004502 */
    Func_02009c1c(0, 0);   /* 200450a */
    Func_02009b98(0xc, 0x1e0, 0xd8);   /* 2004516 */
    Func_02009bb6(0);   /* 200451c */
    Func_02009bf8(0, 0xe, 0);   /* 2004526 */
    Func_02009bc6(0xc);   /* 200452c */
    Func_02009c08(0xc, 0xe, 0);   /* 2004536 */
    Func_02009c4c(0xc, 0x3000, 0);   /* 2004542 */
    Func_02009bf4(0, 3);   /* 200454a */
    Func_02009b7a(5);   /* 2004550 */
    Func_02009c02(0xc, 3);   /* 2004558 */
    Func_02009b88(0x64);   /* 200455e */
    Func_02009c10(0xe, 3);   /* 2004566 */
    Func_02009b96(0x64);   /* 200456c */
    Func_02009bfa(0xc, 0x1e0, 0xf8);   /* 2004578 */
    Func_02009c06(0, 0x1c8, 0xf8);   /* 2004584 */
    Func_02009c24(0xc);   /* 200458a */
    Func_02009c18(0xc, 0x1c8, 0xf8);   /* 2004596 */
    Func_02009c36(0);   /* 200459c */
    Func_02009c2c(0, 0x1c8, 0x168);   /* 20045aa */
    Func_02009c4a(0xc);   /* 20045b0 */
    Func_02009c40(0xc, 0x1c8, 0x168);   /* 20045be */
    Func_02009c5e(0);   /* 20045c4 */
    Func_02009c54(0, 0x160, 0x168);   /* 20045d2 */
    Func_02009c72(0xc);   /* 20045d8 */
    Func_02009c68(0xc, 0x160, 0x168);   /* 20045e6 */
    Func_02009c16(0x14);   /* 20045ec */
    Func_02009d9c(0x11);   /* 20045f2 */

    {
        u8 *workspace = *(u8 **)0x03001ebc;

        *(s32 *)(workspace + 448) = 515;
    }
    /* No argument register is set for this call. */
    Func_02009d7e();   /* 2004604 */
    Func_02009c46(1);   /* 200461c */
    Func_02009c4c(0xd2);   /* 2004622 */
    Func_02009d82(4);   /* 2004628 */
    Func_02009c66();   /* 200462c */
}
