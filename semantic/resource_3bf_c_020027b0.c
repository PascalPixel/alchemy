#include "types.h"

/*
 * Resource 3bf two-variant cutscene script at 0x020027b0.
 *
 * Complete owner: `push {r5, lr}` at 0x020027b0 through
 * `pop {r5} / pop {r0} / bx r0` at 0x0200302e.  r0 holds the popped return
 * address, so the owner is VOID (HANDOVER section 0's interworking-epilogue
 * rule).  Three literal pools sit inside the advertised 2,212 bytes and are
 * data, not code: 0x0200298a..0x020029af, 0x02002dc0..0x02002ddb (branched
 * over by the `b.n` at 0x02002dbe) and 0x02003034..0x02003053, which closes
 * the row exactly where the next owner, 0x02003054, begins.  The middle pool
 * is the one that makes the listing look like code — 0xb333 disassembles as
 * `cbz`, 0xcccc as `ldmia` — which is the pool-decodes-as-instructions trap
 * HANDOVER records.
 *
 * CONTROL FLOW.  Despite its size this owner has only four branches.  Two
 * guards at the top choose between two long straight-line scripts, and both
 * scripts end in the same three-statement tail plus one shared final call:
 *
 *     if (query 0x301)                 -> return immediately
 *     notify 0x270; unnamed reset
 *     if (query 0x942) script A (0x020027d8..0x02002988)
 *     else            script B (0x020029b0..0x02003026)
 *     shared final call at 0x0200302a
 *
 * The early return is worth spelling out: it is written as `bl 0x0200302e`,
 * which `overlay_call_targets.ts` reports as `unknown` because 0x0200302e is
 * `pop {r5}`, not a prologue.  It is the owner's own epilogue — the "a `bl`
 * can be a long unconditional branch to the owner's own exit" idiom, used here
 * because the epilogue is out of `b.n` range.  It is not a call, and lr is
 * clobbered harmlessly because the epilogue pops the return address off the
 * stack rather than using lr.
 *
 * CALL ACCOUNTING.  All 247 `bl` sites were resolved with
 * `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3bf 27b0`, never from the
 * disassembler's pc-relative annotations.  They collapse to 36 distinct
 * targets: 31 import veneers (238 sites), four ordinary prologues of this same
 * overlay (0x0200252c, 0x020025f8, 0x0200269c, 0x02002718 — eight sites, each
 * called once per script) and the one long branch above.  Every veneer is
 * named by the main-image function in its trailing word:
 *   0x020055e8 Func_080091e0 x2    0x020056a8 Func_0808a100 x6
 *   0x02005600 Func_08015040 x2    0x020056b0 Func_0808a110 x5
 *   0x02005610 Func_080770c0 x2    0x020056b8 Func_0808a128 x2
 *   0x02005618 Func_080770c8 x2    0x020056c0 Func_0808a130 x8
 *   0x02005628 Func_0808a010 x47   0x020056d0 Func_0808a148 x22
 *   0x02005630 Func_0808a018 x1    0x020056d8 Func_0808a150 x4
 *   0x02005638 Func_0808a020 x1    0x020056e8 Func_0808a170 x25
 *   0x02005650 Func_0808a080 x2    0x020056f8 Func_0808a180 x25
 *   0x02005658 Func_0808a090 x6    0x02005708 Func_0808a1b8 x13
 *   0x02005680 Func_0808a0c8 x10   0x02005718 Func_0808a1e8 x16
 *   0x02005688 Func_0808a0d0 x2    0x02005728 Func_0808a200 x2
 *   0x02005690 Func_0808a0e0 x4    0x02005730 Func_0808a208 x2
 *   0x02005698 Func_0808a0e8 x12   0x02005740 Func_0808a218 x1
 *   0x020056a0 Func_0808a0f0 x2    0x02005748 Func_0808a220 x1
 *   0x02005760 Func_0808a250 x2    0x02005768 Func_0808a260 x2
 *   0x020057a8 Func_080f9010 x7
 * That is 247 sites against the inventory's `calls=225`; as HANDOVER records,
 * the field counts neither sites nor reliably, and the multiset above is the
 * completeness proof.  Each call's address is carried in a trailing comment so
 * the transcription can be checked site by site.
 *
 * The same import family, the same 0x03001ebc workspace and the same
 * 0x02000240 + 0x22b byte appear in the sibling script at 0x02001e94, and the
 * Func_0808a170 / Func_0808a180 pairing is the same "show line / wait for it"
 * idiom: script A plays lines 0x247d..0x247f and script B plays 0x2464..0x247b
 * (0x247a is skipped, which is in the assembly).  Line ids are written here as
 * absolute constants; the original keeps the base in r5 and reaches each line
 * as `r5 + n`.
 *
 * Link base.  None of this owner's pool words falls in the proven in-image
 * band (0x0200_8xxx and above), so all of them are ordinary values: 0x03001ebc
 * is the IWRAM workspace pointer the other owners in this overlay load, and
 * 0x02000240 is the table they index.  The rest are plain constants that
 * needed pooling because they exceed an 8-bit immediate.
 *
 * Uncertainties:
 *  - Func_0808a018 and Func_0808a020 are each called with no argument register
 *    set at all; they are called with none here and the leftovers in r0-r2 are
 *    noted rather than asserted as parameters.
 *  - Func_080091e0's first argument is the pointer Func_0808a080 has just
 *    returned in r0; the assembly does not reload r0 between the two branches.
 *  - the byte written at 0x02000240 + 0x22b is at an ODD offset, so it is a
 *    byte field of that table and not one of the s16 entries the sibling gates
 *    index.
 *  - the store to workspace + 448 is built as `224 << 1` for the offset and
 *    `+ 64` on the same register for the value, so the offset is 448 and the
 *    stored word is 512.  Both scripts end with the identical five-statement
 *    tail, which is why it appears twice below rather than being hoisted.
 */

/* Old-style declarations are mandatory in overlay sources: one name can be
 * reached with different argument counts from different sites. */
s32 Func_02007dc6();
void Func_02007dde();
void Func_02007dfa();
s32 Func_02007de0();
void Func_02007e3c();
void Func_02007e78();
void Func_02007eca();
void Func_02004d2a();
void Func_02007f22();
void Func_02007f1e();
void Func_02007ed8();
s32 Func_02007e76();
void Func_02007e14();
void Func_02007e5a();
void Func_02007e94();
void Func_02007ec8();
void Func_02007ee6();
void Func_02007e7c();
void Func_02007f68();
void Func_02007f74();
void Func_02007f80();
void Func_02007f8c();
void Func_02007f32();
void Func_02007fce();
void Func_02007f8e();
void Func_02007fa6();
void Func_02007f3c();
void Func_02007f5a();
void Func_02007fd6();
void Func_02007efc();
void Func_02007f70();
void Func_02007f7e();
void Func_02007f14();
void Func_0200809a();
void Func_02007efa();
void Func_02007f96();
void Func_02004f02();
void Func_02007f38();
void Func_02007fd8();
void Func_02007fe0();
void Func_02007fe8();
void Func_02007ff0();
void Func_02007f5e();
void Func_02005054();
void Func_02008016();
void Func_02008034();
void Func_0200804c();
void Func_02004ff4();
void Func_02007f94();
void Func_020080e6();
void Func_0200805e();
void Func_02008164();
void Func_02007fea();
void Func_020080b2();
void Func_020080ca();
void Func_020080b4();
void Func_0200800a();
void Func_020080f6();
void Func_020080b0();
s32 Func_0200804e();
void Func_02007fec();
void Func_020080f2();
void Func_0200810a();
void Func_02008078();
void Func_02008106();
void Func_02004f66();
void Func_0200809c();
void Func_020080d0();
void Func_020080ee();
void Func_02008084();
void Func_0200814a();
void Func_02008162();
void Func_0200818e();
void Func_020080a4();
void Func_0200822a();
void Func_02008170();
void Func_02008188();
void Func_020080be();
void Func_0200814e();
void Func_020080cc();
void Func_02008154();
void Func_02008174();
void Func_020080e2();
void Func_0200811c();
void Func_02008150();
void Func_0200816e();
void Func_02008186();
void Func_0200810c();
void Func_020081f8();
void Func_0200811e();
void Func_0200820a();
void Func_02008130();
void Func_020081e2();
void Func_02008140();
void Func_02008206();
void Func_0200821e();
void Func_020081e4();
void Func_0200825e();
void Func_0200823a();
void Func_02008252();
void Func_02008218();
void Func_02008196();
void Func_02008226();
void Func_020081a4();
void Func_02008208();
void Func_02008270();
void Func_020081c6();
void Func_0200828c();
void Func_020082a4();
void Func_020081da();
void Func_02008262();
void Func_020081e8();
void Func_020082ae();
void Func_020082c6();
void Func_0200827c();
void Func_0200838a();
void Func_020082d2();
void Func_020082ea();
void Func_020082cc();
void Func_020082d6();
void Func_020082e0();
void Func_0200823e();
void Func_0200832a();
void Func_020082fc();
void Func_0200825a();
void Func_020083e0();
void Func_02008374();
void Func_02008394();
void Func_02008390();
void Func_020082b2();
void Func_020082e8();
void Func_02008398();
void Func_0200830e();
void Func_020083a6();
void Func_020083a2();
void Func_020083ae();
void Func_020083ba();
void Func_020083c6();
void Func_020083d2();
void Func_0200839a();
void Func_02008392();
void Func_02008300();
void Func_020083c8();
void Func_020083c2();
void Func_020083e2();
void Func_020083fa();
void Func_02008330();
void Func_02008340();
void Func_02008408();
void Func_02008420();
void Func_0200840a();
void Func_02008414();
void Func_0200836a();
void Func_0200841c();
void Func_02008426();
void Func_02008430();
void Func_0200843a();
void Func_02008432();
void Func_020083a0();
void Func_02008468();
void Func_02008480();
void Func_020084aa();
void Func_020083c0();
void Func_02008448();
void Func_02008490();
void Func_020084a8();
void Func_0200843c();
void Func_02008478();
void Func_020084f4();
void Func_020084dc();
void Func_02008488();
void Func_020084a6();
void Func_02008522();
void Func_02008510();
void Func_02008528();
void Func_02008552();
void Func_02008530();
void Func_02008548();
void Func_020084fa();
void Func_0200853c();
void Func_02008524();
void Func_020084a2();
void Func_0200856a();
void Func_02008582();
void Func_0200853a();
void Func_0200859a();
void Func_020085c6();
void Func_020085d2();
void Func_020085de();
void Func_020085ea();
void Func_020085f6();
void Func_0200850c();
void Func_020085d4();
void Func_020085ec();
void Func_0200861c();
void Func_02008532();
void Func_020085fa();
void Func_02008612();
void Func_020085e2();
void Func_02008550();
void Func_02008618();
void Func_02008630();
void Func_020085e4();
void Func_02008660();
void Func_02008586();
void Func_02008608();
void Func_0200859e();
void Func_02008724();
void Func_02008584();
void Func_02008620();
void Func_0200866a();
void Func_02005596();
void Func_0200874c();
void Func_02008694();
void Func_020086ac();
void Func_0200869e();
void Func_020086a8();
void Func_020086b2();
void Func_020086bc();
void Func_02005708();
void Func_02005690();
void Func_02008636();
void Func_02008788();
void Func_02008664();

                     

                     

                     

                    

                     

                     

                     

                     

                     

                     

                     

void Func_020027b0(void)
{
    s32 actor;

    if (Func_02007dc6(0x301) != 0) {
        /* `bl 0x0200302e` is a long branch to this owner's own epilogue,
         * not a call; lr is clobbered harmlessly because the epilogue pops
         * the return address off the stack. */
        return;
    }

    Func_02007dde(0x270);
    /* No argument register is set for this call. */
    Func_02007dfa();

    if (Func_02007de0(0x942) != 0) {
        Func_02007e3c(0, 0x8000, 0x4000);   /* 20027e2 */
        Func_02007e78(0, 0x1c8, 0xd8);   /* 20027ee */
        Func_02007eca(0, 0xc, 0);   /* 20027f8 */
        Func_02004d2a();   /* 20027fc */
        Func_02007f22(0xc, 0x100, 0x3c);   /* 2002808 */
        Func_02007f1e(0xc, 0x8000, 0);   /* 2002814 */
        Func_02007ed8(0xc, 4, 0);   /* 200281e */
        actor = Func_02007e76(0xc);   /* 2002824 */
        Func_02007e14(actor, 1);   /* 200282a */
        Func_02007e5a(0x1e);   /* 2002830 */
        Func_02007e94(2, 0xb333, 0x5999);   /* 200283a */
        Func_02007ec8(2, 0x1d0, 0xc0);   /* 2002846 */
        Func_02007ee6(2);   /* 200284c */
        Func_02007e7c(0x1e);   /* 2002852 */
        Func_02007f68(0, 0x4000, 0);   /* 200285e */
        Func_02007f74(2, 0x4000, 0);   /* 200286a */
        Func_02007f80(1, 0x4000, 0);   /* 2002876 */
        Func_02007f8c(3, 0x4000, 0);   /* 2002882 */
        Func_02007f32(0xd, 0x1c80000, 0x1400000);   /* 2002890 */
        Func_02007fce(0x20000, 0x4000);   /* 200289c */
        Func_02007f8e(0x247d);   /* 20028a4 */
        Func_02007fa6(0xd, 0);   /* 20028ac */
        Func_02007f3c(0xd, 0x1ca, 0x110);   /* 20028ba */
        Func_02007f5a(0xd);   /* 20028c0 */
        Func_02007fd6(0xd, 0x5000, 0);   /* 20028cc */
        Func_02007efc(0x28);   /* 20028d2 */
        Func_02007f70(0xd, -8, 8);   /* 20028de */
        Func_02007f7e(0xd);   /* 20028e4 */
        Func_02007f14(0x3c);   /* 20028ea */
        Func_0200809a(0x9b);   /* 20028f0 */
        Func_02007efa(0x247e, 1);   /* 20028f8 */
        Func_02007f96(0xd, 8, -8);   /* 2002904 */
        Func_02004f02();   /* 2002908 */
        Func_02007f38(0x78);   /* 200290e */
        Func_02007fd8(0, 2);   /* 2002916 */
        Func_02007fe0(2, 2);   /* 200291e */
        Func_02007fe8(1, 2);   /* 2002926 */
        Func_02007ff0(3, 2);   /* 200292e */
        Func_02007f5e(0x14);   /* 2002934 */
        Func_02005054();   /* 200293a */
        Func_02008016(0xd, 0, 0);   /* 2002944 */
        Func_02008034(0x247f);   /* 200294a */
        Func_0200804c(0xd, 0);   /* 2002952 */
        Func_02004ff4();   /* 2002956 */

        {
            u8 *workspace = *(u8 **)0x03001ebc;

            *(s32 *)(workspace + 448) = 512;
        }
        Func_02007f94(1);
        Func_020080e6(0xa3, 4);
        Func_020080e6(0x62, 4);
        *((u8 *)0x02000240 + 0x22b) = 3;
    } else {
        Func_0200805e(0, 1);   /* 20029b4 */
        Func_02008164(0x11);   /* 20029ba */
        Func_02007fea(0x1e);   /* 20029c0 */
        Func_020080b2(0x2464);   /* 20029c8 */
        Func_020080ca(0xc, 0);   /* 20029d0 */
        Func_020080b4(0, 0xc, 0);   /* 20029da */
        Func_0200800a(0x8c);   /* 20029e0 */
        Func_020080f6(0xc, 0x8000, 0);   /* 20029ec */
        Func_020080b0(0xc, 4, 0);   /* 20029f6 */
        actor = Func_0200804e(0xc);   /* 20029fc */
        Func_02007fec(actor, 1);   /* 2002a02 */
        Func_020080f2(0x2465);   /* 2002a08 */
        Func_0200810a(0xc, 0);   /* 2002a10 */
        Func_02008078(0, 0x8000, 0x4000);   /* 2002a1e */
        Func_020080b4(0, 0x1c8, 0xd8);   /* 2002a2a */
        Func_02008106(0, 0xc, 0);   /* 2002a34 */
        Func_02004f66();   /* 2002a38 */
        Func_0200809c(2, 0xb333, 0x5999);   /* 2002a42 */
        Func_020080d0(2, 0x1d0, 0xc0);   /* 2002a4e */
        Func_020080ee(2);   /* 2002a54 */
        Func_02008084(0x1e);   /* 2002a5a */
        Func_0200814a(0x2466);   /* 2002a60 */
        Func_02008162(2, 0);   /* 2002a68 */
        Func_0200818e(0xc, 0x100, 0);   /* 2002a74 */
        Func_020080a4(0x6e);   /* 2002a7a */
        Func_0200822a(0x3c);   /* 2002a80 */
        Func_02008170(0x2467);   /* 2002a86 */
        Func_02008188(0xc, 0);   /* 2002a8e */
        Func_020080be(0x1e);   /* 2002a94 */
        Func_0200814e(2, 3);   /* 2002a9c */
        Func_020080cc(0x14);   /* 2002aa2 */
        Func_02008154(2, 1);   /* 2002aaa */
        Func_02008174(0xc, 1);   /* 2002ab2 */
        Func_020080e2(0x14);   /* 2002ab8 */
        Func_0200811c(0xc, 0x6666, 0x3333);   /* 2002ac2 */
        Func_02008150(0xc, 0x208, 0xd0);   /* 2002ace */
        Func_0200816e(0xc);   /* 2002ad4 */
        Func_02008186(0xc, 1);   /* 2002adc */
        Func_0200810c(0x14);   /* 2002ae2 */
        Func_020081f8(0xc, 0xb000, 0);   /* 2002aee */
        Func_0200811e(0x1e);   /* 2002af4 */
        Func_0200820a(0xc, 0x5000, 0);   /* 2002b00 */
        Func_02008130(0x1e);   /* 2002b06 */
        Func_020081e2(0xc, 2, 0);   /* 2002b10 */
        Func_02008140(0x14);   /* 2002b16 */
        Func_02008206(0x2468);   /* 2002b1c */
        Func_0200821e(0xc, 0);   /* 2002b24 */
        Func_02008154(0x28);   /* 2002b2a */
        Func_020081e4(2, 3);   /* 2002b32 */
        Func_02008162(0x14);   /* 2002b38 */
        Func_0200825e(0xc, 0x108, 0);   /* 2002b44 */
        Func_02008174(0x78);   /* 2002b4a */
        Func_0200823a(0x2469);   /* 2002b50 */
        Func_02008252(0xc, 0);   /* 2002b58 */
        Func_02008188(0x19);   /* 2002b5e */
        Func_02008218(2, 3);   /* 2002b66 */
        Func_02008196(0x1e);   /* 2002b6c */
        Func_02008226(0xc, 3);   /* 2002b74 */
        Func_020081a4(0x28);   /* 2002b7a */
        Func_02008208(2, 0x1e0, 0xc8);   /* 2002b86 */
        Func_02008226(2);   /* 2002b8c */
        Func_02008270(2, 0xc, 0);   /* 2002b96 */
        Func_020081c6(0x3c);   /* 2002b9c */
        Func_0200828c(0x246a);   /* 2002ba2 */
        Func_020082a4(2, 0);   /* 2002baa */
        Func_020081da(0x14);   /* 2002bb0 */
        Func_02008262(0xc, 4);   /* 2002bb8 */
        Func_020081e8(0x50);   /* 2002bbe */
        Func_020082ae(0x246b);   /* 2002bc4 */
        Func_020082c6(0xc, 0);   /* 2002bcc */
        Func_0200827c(0xd, 0x1c80000, 0x1400000);   /* 2002bda */
        Func_0200838a(0x13);   /* 2002be0 */
        Func_020082d2(0x246c);   /* 2002be8 */
        Func_020082ea(0xd, 0);   /* 2002bf0 */
        Func_020082cc(0, 0xd, 0);   /* 2002bfa */
        Func_020082d6(2, 0xd, 0);   /* 2002c04 */
        Func_020082e0(1, 0xd, 0);   /* 2002c0e */
        Func_0200823e(5);   /* 2002c14 */
        Func_0200832a(3, 0x4000, 0);   /* 2002c20 */
        Func_020082fc(0xc, 0xd, 0);   /* 2002c2a */
        Func_0200825a(0x1e);   /* 2002c30 */
        Func_020083e0(0x3d);   /* 2002c36 */
        Func_02008374(0x20000, 0x4000);   /* 2002c42 */
        Func_02008394(0xd, 1);   /* 2002c4a */
        Func_02008390();   /* 2002c4e */
        Func_020082b2(0xd, 0xcccc, 0x6666);   /* 2002c58 */
        Func_020082e8(0xd, 0x1c8, 0x130);   /* 2002c66 */
        Func_02008398(0xd, 1);   /* 2002c6e */
        Func_0200830e(0xd);   /* 2002c74 */
        Func_020083a6(1, 1);   /* 2002c7c */
        Func_020083a2(0, 0x102, 0);   /* 2002c88 */
        Func_020083ae(2, 0x102, 0);   /* 2002c94 */
        Func_020083ba(1, 0x102, 0);   /* 2002ca0 */
        Func_020083c6(3, 0x102, 0);   /* 2002cac */
        Func_020083d2(0xc, 0x102, 0);   /* 2002cb8 */
        Func_020082e8(0x3c);   /* 2002cbe */
        Func_0200839a(0xc, 0xd, 0);   /* 2002cc8 */
        Func_02008392(0xc, 2);   /* 2002cd0 */
        Func_02008300(0x3c);   /* 2002cd6 */
        Func_020083c8(0x246d);   /* 2002cde */
        Func_020083e0(0xc, 0);   /* 2002ce6 */
        Func_020083c2(0xd, 0xc, 0);   /* 2002cf0 */
        Func_020083e2(0x246e);   /* 2002cf8 */
        Func_020083fa(0xd, 0);   /* 2002d00 */
        Func_02008330(0x3c);   /* 2002d06 */
        Func_020083e2(0xd, 2, 0);   /* 2002d10 */
        Func_02008340(0x1e);   /* 2002d16 */
        Func_02008408(0x246f);   /* 2002d1e */
        Func_02008420(0xd, 0);   /* 2002d26 */
        Func_0200840a(3, 2, 0);   /* 2002d30 */
        Func_02008414(0, 1, 0);   /* 2002d3a */
        Func_0200836a(0x3c);   /* 2002d40 */
        Func_0200841c(0, 0xd, 0);   /* 2002d4a */
        Func_02008426(2, 0xd, 0);   /* 2002d54 */
        Func_02008430(1, 0xd, 0);   /* 2002d5e */
        Func_0200843a(3, 0xd, 0);   /* 2002d68 */
        Func_02008432(0xd, 1);   /* 2002d70 */
        Func_020083a0(0x3c);   /* 2002d76 */
        Func_02008468(0x2470);   /* 2002d7e */
        Func_02008480(0xd, 0);   /* 2002d86 */
        Func_020084aa(1, 0x103, 0);   /* 2002d90 */
        Func_020083c0(0x3c);   /* 2002d96 */
        Func_02008448(0xd, 4);   /* 2002d9e */
        Func_02008490(0x2471);   /* 2002da6 */
        Func_020084a8(0xd, 0);   /* 2002dae */
        Func_0200843c(1, 0x1c8, 0xf8);   /* 2002dba */
        Func_02008478(1);   /* 2002dde */
        Func_020084f4(1, 0x4000, 0);   /* 2002dea */
        Func_020084dc(0x2472);   /* 2002df2 */
        Func_020084f4(1, 0);   /* 2002dfa */
        Func_02008488(2, 0x1d8, 0xd8);   /* 2002e06 */
        Func_020084a6(2);   /* 2002e0c */
        Func_02008522(2, 0x4000, 0);   /* 2002e18 */
        Func_02008448(0xa);   /* 2002e1e */
        Func_02008510(0x2473);   /* 2002e26 */
        Func_02008528(2, 0);   /* 2002e2e */
        Func_02008552(0xc, 0x105, 0);   /* 2002e38 */
        Func_02008468(0x3c);   /* 2002e3e */
        Func_02008530(0x2474);   /* 2002e46 */
        Func_02008548(0xc, 0);   /* 2002e4e */
        Func_020084dc(3, 0x1b8, 0xd8);   /* 2002e5a */
        Func_020084fa(3);   /* 2002e60 */
        Func_0200853c(3, 0xd, 0);   /* 2002e6a */
        Func_02008524(3, 3);   /* 2002e72 */
        Func_020084a2(0xa);   /* 2002e78 */
        Func_0200856a(0x2475);   /* 2002e80 */
        Func_02008582(3, 0);   /* 2002e88 */
        Func_0200853a(0xd, 4);   /* 2002e90 */
        Func_02008582(0x2476);   /* 2002e98 */
        Func_0200859a(0xd, 0);   /* 2002ea0 */
        Func_020085c6(0, 0x102, 0);   /* 2002eac */
        Func_020085d2(1, 0x102, 0);   /* 2002eb8 */
        Func_020085de(3, 0x102, 0);   /* 2002ec4 */
        Func_020085ea(2, 0x102, 0);   /* 2002ed0 */
        Func_020085f6(0xd, 0x108, 0);   /* 2002edc */
        Func_0200850c(0x3c);   /* 2002ee2 */
        Func_020085d4(0x2477);   /* 2002eea */
        Func_020085ec(0xd, 0);   /* 2002ef2 */
        Func_02008522(0x14);   /* 2002ef8 */
        Func_0200861c(1, 0x103, 0);   /* 2002f02 */
        Func_02008532(0x3c);   /* 2002f08 */
        Func_020085fa(0x2478);   /* 2002f10 */
        Func_02008612(1, 0);   /* 2002f18 */
        Func_020085e2(0xd, 1);   /* 2002f20 */
        Func_02008550(0x3c);   /* 2002f26 */
        Func_02008618(0x2479);   /* 2002f2e */
        Func_02008630(0xd, 0);   /* 2002f36 */
        Func_020085c6(0xd, 0x1c8, 0x118);   /* 2002f44 */
        Func_020085e4(0xd);   /* 2002f4a */
        Func_02008660(0xd, 0x5000, 0);   /* 2002f56 */
        Func_02008586(0x50);   /* 2002f5c */
        Func_020085fa(0xd, -8, 8);   /* 2002f68 */
        Func_02008608(0xd);   /* 2002f6e */
        Func_0200859e(0x3c);   /* 2002f74 */
        Func_02008724(0x9b);   /* 2002f7a */
        Func_02008584(0x247e, 1);   /* 2002f82 */
        Func_02008620(0xd, 8, -8);   /* 2002f8e */
        Func_0200866a(0xd, 0xb, 0);   /* 2002f98 */
        Func_02005596();   /* 2002f9c */
        Func_0200874c(0x34);   /* 2002fa2 */
        Func_02008694(0x247b);   /* 2002faa */
        Func_020086ac(0xd, 0);   /* 2002fb2 */
        Func_020085e2(0x3c);   /* 2002fb8 */
        Func_02008694(0, 0xb, 0);   /* 2002fc2 */
        Func_0200869e(1, 0xb, 0);   /* 2002fcc */
        Func_020086a8(2, 0xb, 0);   /* 2002fd6 */
        Func_020086b2(3, 0xb, 0);   /* 2002fe0 */
        Func_020086bc(0xc, 0xb, 0);   /* 2002fea */
        Func_02005708();   /* 2002fee */
        Func_02005690();   /* 2002ff2 */
        Func_02008612(0x942);   /* 2002ff8 */

        {
            u8 *workspace = *(u8 **)0x03001ebc;

            *(s32 *)(workspace + 448) = 512;
        }
        *((u8 *)0x02000240 + 0x22b) = 3;
        Func_02008636(1);
        Func_02008788(0xa3, 4);
        Func_02008788(0x62, 4);
    }

    /* No argument register is set for this call. */
    Func_02008664();
}
