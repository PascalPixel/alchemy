#include "types.h"

/*
 * resource_39e owner at 0x02002ad0, 5000 bytes: the overlay's LARGEST
 * row and the last one it had left -- 525 calls in the familiar
 * Func_0808a0xx/a1xx staging vocabulary, the same ensemble-beat family
 * as the drafted 0x02001dbc (1704B) and 0x02001494 (2236B), just very
 * much longer. Opens Func_0808a018(), dialogue 0x1969, and the
 * four-slot (0/1/2/3) party staging block; runs long positioning,
 * facing and pose passes over slots 0-3, 8, 9, 10 and 17; calls the
 * one in-image callee Func_020041ec once at 0x02003a54; and closes
 * with Func_0808a020() and Func_080770c8(0x895) -- setting the story
 * flag 0x895 that the drafted 0x02002484 TESTS (its pool word 0x24fc
 * is 0x895), alongside 0x89a set by 0x02001494. The overlay's flag
 * chain therefore closes as: 1494 sets 0x89a + (0x898|0x899); 1d50
 * sets 0x898 on its non-skip arm only; 12e0 tests 0x898 to choose
 * 0x1160 vs 0x102c; 1dbc clears 0x898 and sets 0x89b; THIS row sets
 * 0x895; 2484 tests 0x89a/0x895; and 0x02000afc's three-way exit line
 * reports 0x895 vs 0x89b vs neither.
 *
 * Despite the size this row is TRANSCRIPTION, not analysis: the
 * prologue is a bare `push {lr}` -- no callee-saved registers and no
 * sp frame at all -- so nothing is live across a call and every
 * argument is rebuilt at its own site. All 28 distinct call targets
 * are vocabulary already declared elsewhere in this overlay; the row
 * introduces no new callee.
 *
 * Structure, complete: eight branches in 5000 bytes and no loops.
 *   - 0x02002b90 / 0x02002ba4 / 0x02002bb8: three consecutive null
 *     guards, `record = Func_0808a080(0); if (record != 0)
 *     Func_0808a0f0(N, *(s32*)(record+8), *(s32*)(record+16));` for
 *     N = 1, 2, 3. Func_0808a080(0) is re-fetched for each of the
 *     three -- it is NOT cached across them.
 *   - 0x02002ec4, 0x020034ca, 0x020038d4: forward `b.n` over inline
 *     literal pools (the inline-pool idiom of resource_39c/3bb); data,
 *     not control flow.
 *   - 0x02003cf6 `bne.n 0x02003d28` with its join branch at 0x02003d26
 *     `b.n 0x02003d56`: the row's ONE skip-beat gate, on
 *     Func_0808a070(0, 0). VERIFIED by reading both arms in this reconstruction
 *     (the carried-over description in the parked note was flagged
 *     UNVERIFIED and is now confirmed, with one point sharpened):
 *     BOTH arms bump the workspace u16 at Data_03001ebc + 472, and
 *     neither arm touches a story flag. The arms differ only in the
 *     slot they pose -- 1 when Func_0808a070 returns 0, 3 otherwise --
 *     and in where the bump sits (last in the ==0 arm, first in the
 *     other). This is the SYMMETRIC shape, matching the resource_3c9
 *     copies, and specifically NOT the shape of 0x02001d50 in this
 *     same overlay, where only one arm bumps and the other sets flag
 *     0x898. Branch sense stays untransferable between rows.
 *
 * Two bit operations on the record at 0x02003a1c and 0x02003a40 clear
 * and then set bit 0 of the byte at record+90, each around its own
 * fresh Func_0808a080(0). Role not identified.
 *
 * Complete owner: `push {lr}` at 0x02002ad0 through `pop {r0} / bx r0`
 * at 0x02003e3c-0x02003e3e, plus the three inline pools above and six
 * trailing pool words ending 0x02003e57; the next owner (`push {lr}`)
 * begins at 0x02003e58 (drafted, resource_39e_c_02003e58.c).
 * 0x02003e58 - 0x02002ad0 = 0x1388 = 5000 bytes exactly, computed and
 * not hand-translated.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_39e 2ad0 3e58,
 * the +2 rule). Drafting it takes resource_39e's residue to zero.
 *
 * Method note, because the size made it necessary: the argument text
 * below was produced by an independent mechanical read of the bounded
 * listing, then checked against a hand transcription of the first 104
 * calls -- 104 for 104, exact, including every shift form. The
 * extractor throws rather than skips on any register it cannot
 * resolve, and its output count is asserted against the tool's own
 * sites=525. All eight branches and all four real stores were
 * enumerated separately by grep, so the non-call structure is covered
 * by construction rather than by eye. Note the three inline pools must
 * be stepped OVER: disassembled as instructions their words write
 * r0-r3 (0x0101 at 0x2002ed4 reads as `lsls r1, r0, #4`), which is a
 * live trap for any register-tracking reader of a row like this.
 *
 * Exact-call relocation: the annotated logical call stream was paired, by
 * call-site address, with the raw linked Thumb target stream.  All 525 sites
 * agree in count and order.  The declarations below therefore name each
 * site's witnessed relocation address, with `_b`, `_c`, and later suffixes
 * only disambiguating repeated addresses.  This is the same representation
 * used by the exact resource_373 roof-repair scene; the Func_080... names in
 * this header describe logical callees, while the Func_020... names in the C
 * body preserve the linked call encodings.
 *
 * Uncertainty: callee and argument roles are by shape; slot ids and
 * constants are transcribed, not interpreted. The a1e8 second
 * arguments (0x100, 0x101, 0x102, 0x103, 0x105) and the a090 pairs
 * (0x6666/0x3333 and 0xcccc/0x6666) are taken from the literal pools
 * verbatim. Data_03001ebc is the standard workspace pointer cited
 * across this overlay. Called 1x within this overlay.
 */

/* Per-call relocation witnesses, as in the exact roof-repair scene. */
void Func_02006eb0();
void Func_02006ef2();
void Func_02006f18();
void Func_02007014();
void Func_02007028();
void Func_02006f3e();
void Func_02006fc8();
void Func_02006f80();
void Func_02006ee6();
void Func_02006fb4();
void Func_02006fd6();
void Func_02006ff2();
void Func_02006f92();
void Func_02006f10();
void Func_0200700c();
void Func_02006fac();
void Func_02006f2a();
void Func_02007014_b();
void Func_02007030();
void Func_02006fe8();
void Func_02006f4e();
void Func_0200704a();
u8 *Func_02006f90();
void Func_02006fe6();
u8 *Func_02006fa4();
void Func_02006ffa();
u8 *Func_02006fb8();
void Func_0200700e();
void Func_02006fe4();
void Func_02006ff2_b();
void Func_02007000();
void Func_0200700e_b();
void Func_02007034();
void Func_02007042();
void Func_02007050();
void Func_0200705e();
void Func_0200707c();
void Func_02007082();
void Func_02007088();
void Func_02007114();
void Func_0200709a();
void Func_02007126();
void Func_0200703c();
void Func_0200713e();
void Func_02007148();
void Func_02007152();
void Func_0200715c();
void Func_02007160();
void Func_02007160_b();
void Func_0200707e();
void Func_0200710e();
void Func_0200708c();
void Func_02007176();
void Func_02007192();
void Func_020071a6();
void Func_02007198();
void Func_02007148_b();
void Func_020070c6();
void Func_020071b0();
void Func_020071d4();
void Func_020071c6();
void Func_02007176_b();
void Func_020071ea();
void Func_020071e4();
void Func_020071fe();
void Func_0200720a();
void Func_02007216();
void Func_020071c6_b();
void Func_02007238();
void Func_0200713e_b();
void Func_0200723a();
void Func_02007246();
void Func_02007252();
void Func_02007168();
void Func_02007252_b();
void Func_02007202();
void Func_02007180();
void Func_0200726a();
void Func_02007232();
void Func_02007198_b();
void Func_02007282();
void Func_0200728c();
void Func_02007266();
void Func_020072b2();
void Func_020072be();
void Func_0200725e();
void Func_020071dc();
void Func_020072c6();
void Func_020072e8();
void Func_020072da();
void Func_020072f6();
void Func_020072f0();
void Func_020072b8();
void Func_0200721e();
void Func_020072ae();
void Func_0200722c();
void Func_02007316();
void Func_02007332();
void Func_0200733e();
void Func_02007254();
void Func_02007350();
void Func_0200735c();
void Func_02007356();
void Func_02007306();
void Func_02007284();
void Func_02007336();
void Func_02007294();
void Func_02007392();
void Func_02007364();
void Func_0200736e();
void Func_02007378();
void Func_020073d4();
void Func_020073c6();
void Func_02007376();
void Func_020072f4();
void Func_020073de();
void Func_0200738e();
void Func_0200730c();
void Func_020073ac();
void Func_0200731a();
void Func_02007404();
void Func_02007428();
void Func_020073e2();
void Func_02007340();
void Func_0200742a();
void Func_0200744e();
void Func_02007440();
void Func_02007412();
void Func_02007466();
void Func_02007472();
void Func_0200747e();
void Func_02007394();
void Func_02007424();
void Func_020073a2();
void Func_0200748c();
void Func_0200744c();
void Func_020073ba();
void Func_020074a4();
void Func_02007454();
void Func_020073d2();
void Func_020074bc();
void Func_02007484();
void Func_020073ea();
void Func_020074d4();
void Func_020074f0();
void Func_02007490();
void Func_0200740e();
void Func_020074f8();
void Func_020074b8();
void Func_020074c0();
void Func_020074c8();
void Func_020074d8();
void Func_0200743e();
void Func_020074ce();
void Func_0200744c_b();
void Func_02007536();
void Func_020074fe();
void Func_02007464();
void Func_0200754e();
void Func_020074f6();
void Func_020074fe_b();
void Func_02007506();
void Func_02007516();
void Func_02007494();
void Func_0200757e();
void Func_020075a0();
void Func_02007548();
void Func_020074b6();
void Func_020075a0_b();
void Func_02007550();
void Func_020074ce_b();
void Func_020075b8();
void Func_020075da();
void Func_020075e4();
void Func_020075d6();
void Func_020075fa();
void Func_020075a2();
void Func_02007510();
void Func_020075a0_c();
void Func_0200751e();
void Func_02007608();
void Func_02007612();
void Func_020075d2();
void Func_020075da_b();
void Func_020075e2();
void Func_020075f2();
void Func_02007558();
void Func_020075e8();
void Func_02007566();
void Func_02007650();
void Func_02007672();
void Func_02007676();
void Func_02007676_b();
void Func_02007594();
void Func_0200763c();
void Func_020075a2_b();
void Func_0200768c();
void Func_020076a8();
void Func_020076b4();
void Func_020076ae();
void Func_02007656();
void Func_0200765e();
void Func_02007666();
void Func_02007676_c();
void Func_020075f4();
void Func_020076f0();
void Func_020076ea();
void Func_020076aa();
void Func_020076b2();
void Func_020076ba();
void Func_020076ca();
void Func_02007630();
void Func_020076c0();
void Func_0200763e();
void Func_02007728();
void Func_0200774a();
void Func_0200773c();
void Func_020076ec();
void Func_0200766a();
void Func_02007754();
void Func_0200775e();
void Func_02007788();
void Func_02007792();
void Func_0200779c();
void Func_020077a6();
void Func_020076ac();
void Func_02007796();
void Func_020077b2();
void Func_020076c8();
void Func_020077b2_b();
void Func_020077d4();
void Func_020077de();
void Func_020077e8();
void Func_020077f2();
void Func_020077a2();
void Func_02007708();
void Func_020077f2_b();
void Func_020077c4();
void Func_020077ce();
void Func_020077d8();
void Func_02007736();
void Func_020077be();
void Func_020077c6();
void Func_020077d6();
void Func_02007754_b();
void Func_02007850();
void Func_0200785c();
void Func_02007868();
void Func_0200777e();
s32 Func_0200786e();
void Func_0200788a();
void Func_02007896();
void Func_020078a2();
void Func_020078ae();
void Func_020077bc();
void Func_020078b8();
void Func_020078b8_b();
void Func_020077d6_b();
void Func_020078d2();
void Func_020078cc();
void Func_020078e8();
void Func_020078f4();
void Func_020078fe();
void Func_02007910();
void Func_0200791a();
void Func_0200790c();
void Func_02007928();
void Func_0200793a();
void Func_0200792c();
void Func_0200794e();
void Func_02007958();
void Func_02007962();
void Func_0200796c();
void Func_0200795e();
void Func_02007980();
void Func_0200798a();
void Func_02007994();
void Func_020079ac();
void Func_0200799e();
void Func_02007946();
void Func_0200794e_b();
void Func_02007956();
void Func_02007966();
void Func_020078e4();
void Func_0200798c();
void Func_020078f2();
void Func_020079dc();
void Func_020079f6();
void Func_02007a02();
void Func_02007a0e();
void Func_02007a1a();
void Func_02007930();
void Func_02007a2c();
void Func_02007a38();
void Func_02007a44();
void Func_02007a50();
void Func_02007966_b();
void Func_02007a62();
void Func_02007a02_b();
void Func_02007980_b();
void Func_02007a6a();
void Func_02007a10();
void Func_02007a82();
void Func_02007aa6();
void Func_02007ab2();
void Func_02007abe();
void Func_02007aca();
void Func_02007ad6();
void Func_02007ae2();
void Func_020079f0();
void Func_02007aec();
void Func_02007af8();
void Func_02007b04();
void Func_02007b10();
void Func_02007b1a();
void Func_02007a30();
void Func_02007a7a();
void Func_02007aa8();
void Func_02007b4a();
void Func_02007b3c();
void Func_02007b58();
void Func_02007ad6_b();
void Func_02007a7c();
void Func_02007b78();
void Func_02007b82();
void Func_02007b00();
void Func_02007aa6_b();
void Func_02007ba2();
void Func_02007b5a();
void Func_02007ac0();
void Func_02007baa();
void Func_02007bcc();
void Func_02007bbe();
void Func_02007b7e();
void Func_02007bf0();
void Func_02007af6();
void Func_02007be0();
void Func_02007c02();
void Func_02007bf4();
void Func_02007c18();
void Func_02007c0a();
void Func_02007bca();
void Func_02007c36();
void Func_02007c28();
void Func_02007c44();
void Func_02007c50();
void Func_02007c5c();
void Func_02007c68();
void Func_02007b7e_b();
void Func_02007c0e();
void Func_02007b8c();
void Func_02007c76();
void Func_02007c26();
void Func_02007ba4();
void Func_02007c8e();
void Func_02007cb0();
void Func_02007ca2();
void Func_02007c62();
void Func_02007bd0();
void Func_02007cba();
void Func_02007cdc();
void Func_02007cce();
void Func_02007cf2();
void Func_02007ce4();
void Func_02007c8c();
void Func_02007c12();
void Func_02007c9a();
void Func_02007c20();
void Func_02007ca8();
void Func_02007c2e();
void Func_02007cbe();
void Func_02007ce8();
void Func_02007c46();
void Func_02007d30();
void Func_02007c9a_b();
void Func_02007cc8();
void Func_02007c6e();
void Func_02007d0e();
void Func_02007c7c();
void Func_02007d66();
void Func_02007d38();
void Func_02007cda();
void Func_02007d96();
void Func_02007db6();
void Func_02007dc2();
void Func_02007dce();
void Func_02007d54();
void Func_02007cea();
void Func_02007d7a();
void Func_02007cf8();
void Func_02007de2();
void Func_02007d68();
void Func_02007d7e();
void Func_02007dc6();
void Func_02007d9c();
void Func_02007e16();
void Func_02007dd6();
void Func_02007e42();
void Func_02007e46();
void Func_02007e52();
void Func_02007e4c();
void Func_02007e0c();
void Func_02007e14();
void Func_02007e1c();
void Func_02007e2c();
void Func_02007dfa();
void Func_02007e94();
void Func_02007e9e();
void Func_02007eaa();
void Func_02007eb6();
void Func_02007e56();
void Func_02007dd4();
void Func_02007ebe();
void Func_02007e6e();
void Func_02007dec();
u8 *Func_02007e22();
void Func_02007e6a();
void Func_02007e10();
u8 *Func_02007e46_b();
void Func_02007e26();
void Func_02007c42();
void Func_02007e30();
void Func_02007e60();
void Func_02007ec8();
void Func_02007e46_c();
void Func_02007f30();
void Func_02007ed8();
void Func_02007ee0();
void Func_02007ee8();
void Func_02007ef8();
void Func_02007e76();
void Func_02007f72();
void Func_02007f7e();
void Func_02007f8a();
void Func_02007f96();
void Func_02007fa2();
void Func_02007eb8();
void Func_02007f26();
void Func_02007f6e();
void Func_02007ed4();
void Func_02007fd0();
void Func_02007fca();
void Func_02007f7a();
void Func_02007ef8_b();
void Func_02007fe2();
void Func_02007f50();
void Func_02007f7e_b();
void Func_02008008();
void Func_02007f72_b();
void Func_02007f7c();
void Func_02007fa2_b();
void Func_02007fb0();
void Func_02007fc6();
void Func_02007fdc();
void Func_02007fd2();
void Func_02007ffc();
void Func_02007ffa();
void Func_0200800c();
void Func_0200800a();
void Func_0200801c();
void Func_02008012();
void Func_020080b4();
void Func_02008064();
void Func_02007fca_b();
void Func_020080c6();
void Func_020080c0();
void Func_020080da();
void Func_020080e6();
void Func_02008086();
void Func_02008004();
void Func_020080ee();
void Func_02008110();
void Func_020080c0_b();
void Func_0200811c();
s32 Func_0200811c_b();
void Func_020080c4();
void Func_02008042();
void Func_020080ea();
void Func_02008050();
void Func_0200814c();
void Func_02008158();
void Func_02008152();
void Func_02008174();
void Func_0200817e();
void Func_02008116();
void Func_02008094();
void Func_0200817e_b();
void Func_020081a2();
void Func_020081ae();
void Func_0200818e();
s32 Func_020080e6_b();
void Func_020080d0();
void Func_02008160();
void Func_020080de();
s64 Func_020081c8(); /* ignored two-word return is the witnessed call schedule */
void Func_02008100();
void Func_020081a0();
void Func_0200811e();
void Func_02008208();
void Func_020081d0();
void Func_0200821a();
void Func_020081c2();
void Func_020081d2();
void Func_02008150();
void Func_020081b6();
void Func_020081cc();
void Func_020081ee();
void Func_020081ec();
void Func_020081fe();
void Func_0200818c();
void Func_0200823e();
void Func_0200819c();
void Func_0200824e();
void Func_02008290();
void Func_020082b4();
void Func_0200825c();
void Func_020082ce();
void Func_020081d4();
void Func_02008264();
void Func_020081e2();
void Func_020082cc();
void Func_0200825a();
void Func_0200827c();
void Func_02008218();
void Func_020081fe_b();

extern u8 *Data_03001ebc;

#define BUMP_SCENE_COUNTER() do { \
    u8 *counter_workspace = Data_03001ebc; \
    u16 *scene_counter = (u16 *)(counter_workspace + 472); \
    *scene_counter += 1; \
} while (0)

void Func_02002ad0(void)
{
    u8 *record;

    Func_02006eb0();
    Func_02006ef2(0, 0x6666, 0x3333);
    Func_02006f18(0, 236 << 1, 134 << 2);
    Func_02007014();
    Func_02007028();
    Func_02006f3e(0);
    Func_02006fc8(9, 0, 20);
    Func_02006f80(9, 2);
    Func_02006ee6(20);
    Func_02006fb4(0x1969);
    Func_02006fd6(9, 0, 20);
    Func_02006ff2(0, 128 << 8, 20);
    Func_02006f92(0, 3);
    Func_02006f10(30);
    Func_0200700c(8, 128 << 7, 30);
    Func_02006fac(8, 3);
    Func_02006f2a(20);
    Func_02007014_b(8, 0, 20);
    Func_02007030(0, 192 << 8, 20);
    Func_02006fe8(0, 1);
    Func_02006f4e(20);
    Func_0200704a(9, 208 << 8, 0);
    record = Func_02006f90(0);
    if (record != 0)
        Func_02006fe6(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Func_02006fa4(0);
    if (record != 0)
        Func_02006ffa(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Func_02006fb8(0);
    if (record != 0)
        Func_0200700e(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    Func_02006fe4(0, 128 << 8, 128 << 7);
    Func_02006ff2_b(1, 128 << 8, 128 << 7);
    Func_02007000(2, 128 << 8, 128 << 7);
    Func_0200700e_b(3, 128 << 8, 128 << 7);
    Func_02007034(0, 232 << 1, 252 << 1);
    Func_02007042(2, 240 << 1, 252 << 1);
    Func_02007050(1, 248 << 1, 248 << 1);
    Func_0200705e(3, 224 << 1, 248 << 1);
    Func_0200707c(0);
    Func_02007082(2);
    Func_02007088(3);
    Func_02007114(3, 224 << 8, 0);
    Func_0200709a(1);
    Func_02007126(1, 160 << 8, 0);
    Func_0200703c(20);
    Func_0200713e(0, 0x101, 0);
    Func_02007148(1, 0x101, 0);
    Func_02007152(2, 0x101, 0);
    Func_0200715c(3, 0x101, 60);
    Func_02007160(1, 160 << 7, 20);
    Func_02007160_b(1, 0);
    Func_0200707e(20);
    Func_0200710e(8, 3);
    Func_0200708c(20);
    Func_02007176(8, 0, 20);
    Func_02007192(1, 160 << 8, 20);
    Func_020071a6(3, 128 << 1, 60);
    Func_02007198(3, 0, 20);
    Func_02007148_b(8, 3);
    Func_020070c6(20);
    Func_020071b0(8, 0, 20);
    Func_020071d4(2, 128 << 1, 60);
    Func_020071c6(2, 0, 20);
    Func_02007176_b(8, 3);
    Func_020071ea(8, 192 << 6, 20);
    Func_020071e4(8, 0, 20);
    Func_020071fe(0, 0, 0);
    Func_0200720a(1, 192 << 7, 0);
    Func_02007216(3, 128 << 6, 0);
    Func_020071c6_b(2, 2);
    Func_02007238(2, 129 << 1);
    Func_0200713e_b(60);
    Func_0200723a(0, 192 << 8, 0);
    Func_02007246(1, 160 << 8, 0);
    Func_02007252(3, 224 << 8, 0);
    Func_02007168(20);
    Func_02007252_b(2, 0, 20);
    Func_02007202(8, 3);
    Func_02007180(20);
    Func_0200726a(8, 0, 20);
    Func_02007232(2, 2);
    Func_02007198_b(20);
    Func_02007282(2, 0, 20);
    Func_0200728c(8, 0, 20);
    Func_02007266(0, 2, 50);
    Func_020072b2(0, 192 << 8, 0);
    Func_020072be(2, 192 << 8, 30);
    Func_0200725e(2, 3);
    Func_020071dc(20);
    Func_020072c6(8, 0, 20);
    Func_020072e8(3, 0x101, 60);
    Func_020072da(3, 0, 20);
    Func_020072f6(8, 160 << 7, 20);
    Func_020072f0(8, 0, 20);
    Func_020072b8(3, 1);
    Func_0200721e(20);
    Func_020072ae(8, 3);
    Func_0200722c(20);
    Func_02007316(8, 0, 20);
    Func_02007332(0, 224 << 8, 0);
    Func_0200733e(1, 192 << 7, 0);
    Func_02007254(30);
    Func_02007350(0, 192 << 8, 0);
    Func_0200735c(1, 160 << 8, 20);
    Func_02007356(1, 0, 20);
    Func_02007306(8, 4);
    Func_02007284(20);
    Func_02007336(8, 2, 0);
    Func_02007294(10);
    Func_02007392(8, 0, 20);
    Func_02007364(0, 2, 0);
    Func_0200736e(1, 2, 0);
    Func_02007378(3, 2, 0);
    Func_020073d4(2, 129 << 1, 60);
    Func_020073c6(2, 0, 20);
    Func_02007376(8, 3);
    Func_020072f4(20);
    Func_020073de(8, 0, 20);
    Func_0200738e(2, 3);
    Func_0200730c(20);
    Func_020073ac(1, 2);
    Func_0200731a(20);
    Func_02007404(1, 0, 20);
    Func_02007428(8, 128 << 1, 60);
    Func_020073e2(8, 1, 0);
    Func_02007340(20);
    Func_0200742a(8, 0, 20);
    Func_0200744e(2, 128 << 1, 60);
    Func_02007440(2, 0, 20);
    Func_02007412(8, 1, 0);
    Func_02007466(0, 192 << 8, 0);
    Func_02007472(1, 160 << 8, 0);
    Func_0200747e(3, 224 << 8, 0);
    Func_02007394(20);
    Func_02007424(8, 3);
    Func_020073a2(20);
    Func_0200748c(8, 0, 20);
    Func_0200744c(2, 2);
    Func_020073ba(20);
    Func_020074a4(2, 0, 20);
    Func_02007454(8, 3);
    Func_020073d2(20);
    Func_020074bc(8, 0, 20);
    Func_02007484(3, 2);
    Func_020073ea(20);
    Func_020074d4(3, 0, 20);
    Func_020074f0(8, 160 << 7, 20);
    Func_02007490(8, 3);
    Func_0200740e(20);
    Func_020074f8(8, 0, 20);
    Func_020074b8(0, 2);
    Func_020074c0(1, 2);
    Func_020074c8(2, 2);
    Func_020074d8(3, 2);
    Func_0200743e(20);
    Func_020074ce(8, 4);
    Func_0200744c_b(20);
    Func_02007536(8, 0, 20);
    Func_020074fe(8, 2);
    Func_02007464(20);
    Func_0200754e(8, 0, 20);
    Func_020074f6(0, 3);
    Func_020074fe_b(1, 3);
    Func_02007506(2, 3);
    Func_02007516(3, 3);
    Func_02007494(20);
    Func_0200757e(8, 0, 20);
    Func_020075a0(1, 0x103, 0);
    Func_02007548(1, 2);
    Func_020074b6(60);
    Func_020075a0_b(1, 0, 20);
    Func_02007550(8, 4);
    Func_020074ce_b(20);
    Func_020075b8(8, 0, 20);
    Func_020075da(0, 0x101, 0);
    Func_020075e4(1, 0x101, 60);
    Func_020075d6(1, 0, 20);
    Func_020075fa(8, 129 << 1, 0);
    Func_020075a2(8, 1);
    Func_02007510(60);
    Func_020075a0_c(8, 4);
    Func_0200751e(20);
    Func_02007608(8, 0, 20);
    Func_02007612(8, 0, 20);
    Func_020075d2(0, 1);
    Func_020075da_b(1, 1);
    Func_020075e2(2, 1);
    Func_020075f2(3, 1);
    Func_02007558(20);
    Func_020075e8(8, 4);
    Func_02007566(20);
    Func_02007650(8, 0, 20);
    Func_02007672(1, 0x101, 60);
    Func_02007676(1, 160 << 7, 20);
    Func_02007676_b(1, 0);
    Func_02007594(20);
    Func_0200763c(2, 1);
    Func_020075a2_b(20);
    Func_0200768c(2, 0, 20);
    Func_020076a8(1, 160 << 8, 20);
    Func_020076b4(8, 192 << 8, 20);
    Func_020076ae(8, 0, 20);
    Func_02007656(0, 3);
    Func_0200765e(1, 3);
    Func_02007666(2, 3);
    Func_02007676_c(3, 3);
    Func_020075f4(20);
    Func_020076f0(8, 128 << 7, 20);
    Func_020076ea(8, 0, 20);
    Func_020076aa(0, 2);
    Func_020076b2(1, 2);
    Func_020076ba(2, 2);
    Func_020076ca(3, 2);
    Func_02007630(20);
    Func_020076c0(8, 4);
    Func_0200763e(20);
    Func_02007728(8, 0, 20);
    Func_0200774a(2, 0x101, 60);
    Func_0200773c(2, 0, 20);
    Func_020076ec(8, 3);
    Func_0200766a(20);
    Func_02007754(8, 0, 20);
    Func_0200775e(8, 0, 20);
    Func_02007788(0, 129 << 1);
    Func_02007792(1, 129 << 1);
    Func_0200779c(2, 129 << 1);
    Func_020077a6(3, 129 << 1);
    Func_020076ac(60);
    Func_02007796(3, 0, 20);
    Func_020077b2(8, 160 << 7, 20);
    Func_020076c8(20);
    Func_020077b2_b(8, 0, 20);
    Func_020077d4(0, 0x101, 0);
    Func_020077de(1, 0x101, 0);
    Func_020077e8(2, 0x101, 0);
    Func_020077f2(3, 0x101, 60);
    Func_020077a2(1, 1);
    Func_02007708(20);
    Func_020077f2_b(1, 0, 20);
    Func_020077c4(0, 1, 0);
    Func_020077ce(2, 1, 0);
    Func_020077d8(3, 1, 0);
    Func_02007736(20);
    Func_020077be(0, 3);
    Func_020077c6(2, 3);
    Func_020077d6(3, 3);
    Func_02007754_b(60);
    Func_02007850(0, 192 << 8, 0);
    Func_0200785c(2, 192 << 8, 0);
    Func_02007868(3, 208 << 8, 0);
    Func_0200777e(20);
    Func_0200786e(8, 0);
    Func_0200788a(0, 129 << 1, 0);
    Func_02007896(1, 129 << 1, 0);
    Func_020078a2(2, 129 << 1, 0);
    Func_020078ae(3, 129 << 1, 0);
    Func_020077bc(60);
    Func_020078b8(1, 192 << 7, 20);
    Func_020078b8_b(1, 0);
    Func_020077d6_b(20);
    Func_020078d2(3, 128 << 6, 20);
    Func_020078cc(3, 0, 20);
    Func_020078e8(1, 160 << 8, 0);
    Func_020078f4(3, 224 << 8, 30);
    Func_020078fe(8, 0, 20);
    Func_02007910(8, 0x105, 60);
    Func_0200791a(2, 0x101, 60);
    Func_0200790c(2, 0, 20);
    Func_02007928(8, 192 << 8, 20);
    Func_0200793a(8, 0x105, 60);
    Func_0200792c(8, 0, 20);
    Func_0200794e(0, 0x101, 0);
    Func_02007958(1, 0x101, 0);
    Func_02007962(2, 0x101, 0);
    Func_0200796c(3, 0x101, 60);
    Func_0200795e(8, 0, 20);
    Func_02007980(0, 0x101, 0);
    Func_0200798a(1, 0x101, 0);
    Func_02007994(2, 0x101, 0);
    Func_020079ac(3, 0x101, 60);
    Func_0200799e(8, 0, 20);
    Func_02007946(0, 3);
    Func_0200794e_b(1, 3);
    Func_02007956(2, 3);
    Func_02007966(3, 3);
    Func_020078e4(20);
    Func_0200798c(8, 1);
    Func_020078f2(20);
    Func_020079dc(8, 0, 20);
    Func_020079f6(0, 0, 0);
    Func_02007a02(1, 192 << 7, 0);
    Func_02007a0e(2, 128 << 8, 0);
    Func_02007a1a(3, 128 << 6, 0);
    Func_02007930(60);
    Func_02007a2c(0, 192 << 8, 0);
    Func_02007a38(1, 160 << 8, 0);
    Func_02007a44(2, 192 << 8, 0);
    Func_02007a50(3, 224 << 8, 0);
    Func_02007966_b(30);
    Func_02007a62(8, 128 << 7, 20);
    Func_02007a02_b(8, 4);
    Func_02007980_b(20);
    Func_02007a6a(8, 0, 20);
    Func_02007a10(10, 236 << 17, 152 << 18);
    Func_02007a82(10, 0, 20);
    Func_02007aa6(0, 128 << 1, 0);
    Func_02007ab2(1, 128 << 1, 0);
    Func_02007abe(2, 128 << 1, 0);
    Func_02007aca(3, 128 << 1, 0);
    Func_02007ad6(8, 128 << 1, 0);
    Func_02007ae2(9, 128 << 1, 0);
    Func_020079f0(60);
    Func_02007aec(0, 128 << 7, 0);
    Func_02007af8(1, 128 << 7, 0);
    Func_02007b04(2, 128 << 7, 0);
    Func_02007b10(3, 128 << 7, 0);
    Func_02007b1a(9, 0, 0);
    Func_02007a30(30);
    Func_02007a7a(10, 0xcccc, 0x6666);
    Func_02007aa8(10, 236 << 1, 134 << 2);
    Func_02007b4a(8, 0x101, 60);
    Func_02007b3c(8, 0, 20);
    Func_02007b58(2, 192 << 8, 40);
    Func_02007ad6_b(2, 244 << 1, 128 << 2);
    Func_02007a7c(10);
    Func_02007b78(2, 192 << 7, 20);
    Func_02007b82(0, 0, 30);
    Func_02007b00(0, 228 << 1, 128 << 2);
    Func_02007aa6_b(10);
    Func_02007ba2(0, 128 << 6, 20);
    Func_02007b5a(10, 2);
    Func_02007ac0(20);
    Func_02007baa(10, 0, 20);
    Func_02007bcc(8, 0x101, 60);
    Func_02007bbe(8, 0, 20);
    Func_02007b7e(10, 2);
    Func_02007bf0(10, 129 << 1);
    Func_02007af6(60);
    Func_02007be0(10, 0, 20);
    Func_02007c02(8, 0x101, 60);
    Func_02007bf4(8, 0, 20);
    Func_02007c18(10, 128 << 1, 60);
    Func_02007c0a(10, 0, 20);
    Func_02007bca(8, 1);
    Func_02007c36(8, 128 << 1, 60);
    Func_02007c28(8, 0, 20);
    Func_02007c44(0, 224 << 8, 0);
    Func_02007c50(1, 160 << 8, 0);
    Func_02007c5c(2, 160 << 8, 0);
    Func_02007c68(3, 224 << 8, 0);
    Func_02007b7e_b(40);
    Func_02007c0e(10, 3);
    Func_02007b8c(20);
    Func_02007c76(10, 0, 20);
    Func_02007c26(8, 4);
    Func_02007ba4(20);
    Func_02007c8e(8, 0, 20);
    Func_02007cb0(8, 0x101, 60);
    Func_02007ca2(8, 0, 20);
    Func_02007c62(17, 1);
    Func_02007bd0(20);
    Func_02007cba(10, 0, 20);
    Func_02007cdc(8, 0x105, 60);
    Func_02007cce(8, 0, 20);
    Func_02007cf2(8, 128 << 1, 30);
    Func_02007ce4(8, 0, 20);
    Func_02007c8c(0, 3);
    Func_02007c12(2);
    Func_02007c9a(2, 3);
    Func_02007c20(1);
    Func_02007ca8(3, 3);
    Func_02007c2e(5);
    Func_02007cbe(1, 3);
    Func_02007ce8(8, 0, 0);
    Func_02007c46(20);
    Func_02007d30(8, 0, 20);
    Func_02007c9a_b(10, 0xcccc, 0x6666);
    Func_02007cc8(10, 236 << 1, 252 << 1);
    Func_02007c6e(20);
    Func_02007d0e(10, 1);
    Func_02007c7c(20);
    Func_02007d66(10, 0, 20);
    Func_02007d38(8, 10, 0);
    Func_02007cda(8, 0xcccc, 0x6666);
    Func_02007d96(0, 128 << 6, 0);
    Func_02007db6(1, 192 << 7, 0);
    Func_02007dc2(2, 192 << 7, 0);
    Func_02007dce(3, 128 << 6, 0);
    Func_02007d54(8);
    Func_02007cea(30);
    Func_02007d7a(8, 3);
    Func_02007cf8(20);
    Func_02007de2(8, 0, 20);
    Func_02007d68(10, 236 << 1, 142 << 2);
    Func_02007d7e(8, 236 << 1, 134 << 2);
    Func_02007dc6(1, 2);
    Func_02007d9c(10);
    Func_02007e16(1, 0, 20);
    Func_02007dd6(8, 2);
    Func_02007e42(8, 129 << 1, 60);
    Func_02007e46(10, 208 << 8, 0);
    Func_02007e52(8, 208 << 8, 20);
    Func_02007e4c(8, 0, 20);
    Func_02007e0c(0, 2);
    Func_02007e14(1, 2);
    Func_02007e1c(2, 2);
    Func_02007e2c(3, 2);
    Func_02007dfa(8, 236 << 1, 128 << 2);
    Func_02007e94(8, 0, 0);
    Func_02007e9e(0, 0, 0);
    Func_02007eaa(2, 128 << 8, 0);
    Func_02007eb6(9, 208 << 8, 20);
    Func_02007e56(8, 3);
    Func_02007dd4(20);
    Func_02007ebe(8, 0, 20);
    Func_02007e6e(2, 3);
    Func_02007dec(20);
    *(u8 *)(Func_02007e22(0) + 90) &= ~1;
    Func_02007e6a(0, 224 << 1, 128 << 2);
    Func_02007e10(1);
    {
        /* Distinct byte temporaries select the reference's OR destination. */
        u8 *bit = Func_02007e46_b(0) + 90;
        u8 old = *bit;
        u8 one = 1;
        *bit = one | old;
    }
    Func_02007e26(20);
    Func_02007c42();
    Func_02007e30(60);
    Func_02007e60(2, 144);
    Func_02007ec8(8, 3);
    Func_02007e46_c(20);
    Func_02007f30(8, 0, 20);
    Func_02007ed8(0, 3);
    Func_02007ee0(1, 3);
    Func_02007ee8(2, 3);
    Func_02007ef8(3, 3);
    Func_02007e76(20);
    Func_02007f72(0, 128 << 6, 0);
    Func_02007f7e(1, 192 << 7, 0);
    Func_02007f8a(2, 192 << 7, 0);
    Func_02007f96(3, 128 << 6, 0);
    Func_02007fa2(9, 192 << 6, 0);
    Func_02007eb8(20);
    Func_02007f26(8, 236 << 1, 138 << 2);
    Func_02007f6e(8, 2);
    Func_02007ed4(20);
    Func_02007fd0(8, 208 << 8, 20);
    Func_02007fca(8, 0, 20);
    Func_02007f7a(8, 3);
    Func_02007ef8_b(20);
    Func_02007fe2(8, 0, 20);
    Func_02007f50(8, 128 << 8, 128 << 7);
    Func_02007f7e_b(8, 240 << 1, 135 << 2);
    Func_02008008(8, 0, 20);
    Func_02007f72_b(8, 0xcccc, 0x6666);
    Func_02007f7c(9, 0xcccc, 0x6666);
    Func_02007fa2_b(8, 236 << 1, 152 << 2);
    Func_02007fb0(9, 236 << 1, 136 << 2);
    Func_02007fc6(10, 236 << 1, 152 << 2);
    Func_02007fdc(9);
    Func_02007fd2(9, 236 << 1, 152 << 2);
    Func_02007ffc(10, 0, 0);
    Func_02007ffa(8);
    Func_0200800c(8, 0, 0);
    Func_0200800a(9);
    Func_0200801c(9, 0, 0);
    Func_02008012(2, 244 << 1, 130 << 2);
    Func_020080b4(2, 0x101, 60);
    Func_02008064(3, 1);
    Func_02007fca_b(20);
    Func_020080c6(3, 128 << 6, 20);
    Func_020080c0(3, 0, 20);
    Func_020080da(0, 0, 0);
    Func_020080e6(2, 160 << 8, 20);
    Func_02008086(2, 4);
    Func_02008004(20);
    Func_020080ee(2, 0, 20);
    Func_02008110(3, 0x105, 60);
    Func_020080c0_b(1, 1);
    Func_0200811c(1, 192 << 7, 20);
    Func_0200811c_b(1, 0);
    Func_020080c4(0, 3);
    Func_02008042(20);
    Func_020080ea(3, 2);
    Func_02008050(20);
    Func_0200814c(0, 192 << 8, 0);
    Func_02008158(1, 128 << 8, 0);
    Func_02008152(3, 0, 20);
    Func_02008174(0, 0x101, 0);
    Func_0200817e(1, 0x101, 60);
    Func_02008116(3, 4);
    Func_02008094(20);
    Func_0200817e_b(3, 0, 20);
    Func_020081a2(0, 129 << 1, 0);
    Func_020081ae(1, 129 << 1, 60);
    Func_0200818e(3, 0);

    if (Func_020080e6_b(0, 0) == 0) {
        Func_020080d0(20);
        Func_02008160(1, 3);
        Func_020080de(20);
        Func_020081c8(1, 0, 20);
        BUMP_SCENE_COUNTER();
    } else {
        Func_02008100(20);
        BUMP_SCENE_COUNTER();
        {
            /* Reverse declaration order preserves the two independent moves. */
            s32 pose1 = 3;
            s32 pose0 = 3;
            Func_020081a0(pose0, pose1);
        }
        Func_0200811e(20);
        Func_02008208(3, 0, 20);
    }

    Func_020081d0(1, 1);
    Func_0200821a(1, 0, 20);
    Func_020081c2(0, 3);
    Func_020081d2(3, 3);
    Func_02008150(20);
    Func_020081b6(1, 224 << 1, 128 << 2);
    Func_020081cc(3, 224 << 1, 128 << 2);
    Func_020081ee(3, 0, 0);
    Func_020081ec(1);
    Func_020081fe(1, 0, 0);
    Func_0200818c(20);
    Func_0200823e(0, 2, 20);
    Func_0200819c(30);
    Func_0200824e(2, 0, 20);
    Func_02008290(2, 0, 20);
    Func_020082b4(0, 129 << 1, 60);
    Func_0200825c(2, 2);
    Func_020082ce(2, 129 << 1);
    Func_020081d4(60);
    Func_02008264(2, 4);
    Func_020081e2(20);
    Func_020082cc(2, 0, 20);
    Func_0200825a(2, 224 << 1, 128 << 2);
    Func_0200827c(2, 0, 0);
    Func_02008218();
    Func_020081fe_b(0x895);
}
