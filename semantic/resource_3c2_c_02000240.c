#include "types.h"

/*
 * Resource 3c2 cutscene script at 0x02000240 -- the overlay's largest owner,
 * 2,068 bytes and 252 call sites.
 *
 * Owner is complete: `push {lr}` at 0x02000240 and the single interworking
 * epilogue `pop {r0} / bx r0` at 0x02000a40, so the owner is void and takes no
 * argument (r0 is written before its first use).  Two literal pools sit inside
 * the span: 0x02000644-0x02000657 and 0x02000a44-0x02000a53.  The FIRST ENDS
 * MID-ROW between an argument setup and its own call -- r0/r1 are loaded at
 * 0x0200063e/0x02000640, the `b.n 0x02000658` at 0x02000642 jumps the pool,
 * and the `bl Func_0808a180` that consumes them is the first instruction after
 * it.  That is why the argument pair for that one site is recovered by hand
 * below rather than from the straight-line register trace.
 *
 * Link base 0x02008000 (established from the export veneer table at file
 * offset 0; see resource_3c2_c_02000a78.c).  This owner references no in-image
 * pool word: every pool word here is either a plain constant (0x98a, 0x9b0,
 * 0x25eb, 0x13333, 0x9999, 0x107, 0x101) or the IWRAM workspace pointer cell
 * 0x03001ebc.
 *
 * Control flow: one early-out on story flag 0x98a, one two-armed test on
 * Func_0808a070(0, 0) near the end, one null check on the scene record, and
 * otherwise straight-line script.
 *
 * The two arms of the Func_0808a070 test are BEHAVIOURALLY IDENTICAL and are
 * deliberately kept separate.  Each waits, repaints actor 19 and bumps the
 * u16 skip-beat counter at workspace + 472 (`movs r3,#236 / lsls r3,#1` off
 * the 0x03001ebc pointer cell -- the documented cross-overlay idiom); they
 * differ only in the wait length (20 against 10) and in where the bump sits
 * relative to the Func_0808a180 call.  Merging them would deflate the
 * per-target multiset by two Func_0808a010 and two Func_0808a180 sites.
 *
 * Completeness proof, PER TARGET rather than by total.  Every site was taken
 * from `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3c2 0240 --json` (site ->
 * target), never from the summary histogram, and the C below was then counted
 * per callee.  Both sides agree on all 29 targets:
 *   Func_0808a010 104, Func_0808a180 38, Func_0808a138 22, Func_0808a1b8 17,
 *   Func_0808a1e8 15, Func_0808a110 12, Func_0808a148 8, Func_0808a580 5,
 *   Func_0808a090 4, Func_0808a150 3, Func_0808a0d0 2, Func_0808a0e8 2,
 *   Func_0808a100 2, Func_0808a130 2, Func_080f9010 2, and one each of
 *   Func_080770c0, Func_080770c8, Func_0808a018, Func_0808a020, Func_0808a070,
 *   Func_0808a080, Func_0808a0b8, Func_0808a0f0, Func_0808a170, Func_0808a178,
 *   Func_0808a210, Func_0808a218, Func_0808a4f0, Func_0808a570.
 *   Total 252 = 252 sites.
 *
 * Uncertainties.  The imports' interfaces are not established beyond the
 * argument registers each call site sets, so their arities are taken per
 * target from the register writes in the window before each `bl` and are
 * uniform across this owner: Func_0808a010/0170/0080/0e8/080f9010/080770c0/
 * 080770c8 take one, Func_0808a018/0020/0218/04f0 take none, Func_0808a210 and
 * Func_0808a570 take four, and the rest take two or three as written.
 * Func_0808a010's single argument is a frame count (10/14/20/28/30 recur), and
 * 19/20/21 recur as actor or slot ids, but neither reading is proven here.
 * The large literals 0x13333 and 0x9999 are 16.16 fixed-point (1.2 and 0.6).
 */

/* Overlay imports (through the veneer table at file offset 0x0b34).  Old-style
 * declarations: overlay imports vary their argument count between call sites,
 * and two names can be one import. */
void Func_02000d84();
s32 Func_02000d82();
void Func_02000e8e();
void Func_02000daa();
void Func_02000e5c();
void Func_02000df8();
void Func_02000e64();
void Func_02000ea4();
void Func_02000e22();
void Func_02000e96();
void Func_02000e6c();
void Func_02000df2();
void Func_02000e62();
void Func_02000e00();
void Func_02000e98();
void Func_02000e0e();
void Func_02000e7e();
void Func_02000e1c();
void Func_02000e4e();
void Func_02000f02();
void Func_02000ed4();
void Func_02000e42();
void Func_02000ee6();
void Func_02000e54();
void Func_02000ef6();
void Func_02000f10();
void Func_02000f08();
void Func_02000eee();
void Func_02000e8c();
void Func_02000f24();
void Func_02000e9a();
void Func_02000ecc();
void Func_02000f80();
void Func_02000f8a();
void Func_02000f5e();
void Func_02000f64();
void Func_02000eda();
void Func_02000f86();
void Func_02000f7e();
void Func_02000ef4();
void Func_02000fa0();
void Func_02000f98();
void Func_02000f0e();
void Func_02000fb2();
void Func_02000f20();
void Func_02000fb8();
void Func_02000f2e();
void Func_02000f9e();
void Func_02000f3c();
void Func_02000f9c();
void Func_02000f4a();
void Func_02000fe2();
void Func_02000f58();
void Func_02000ffc();
void Func_02000f6a();
void Func_02001014();
void Func_0200100c();
void Func_02000f82();
void Func_02000fea();
void Func_02000ffa();
void Func_02000fca();
void Func_02000fe6();
void Func_02001088();
void Func_0200105a();
void Func_02000fc8();
void Func_02000fce();
void Func_0200102e();
void Func_02000fdc();
void Func_02001074();
void Func_02001096();
void Func_0200108e();
void Func_02001004();
void Func_02001012();
void Func_020010aa();
void Func_02001020();
void Func_0200109a();
void Func_020010ce();
void Func_0200103c();
void Func_020010d4();
void Func_0200104a();
void Func_020010ba();
void Func_02001058();
void Func_020010f0();
void Func_02001066();
void Func_020010e0();
void Func_02001114();
void Func_02001082();
void Func_020010e2();
void Func_02001090();
void Func_02001128();
void Func_0200109e();
void Func_0200110e();
void Func_020010ac();
void Func_0200110c();
void Func_02001152();
void Func_020010c8();
void Func_0200114a();
void Func_0200116a();
void Func_02001150();
void Func_020010ee();
void Func_02001186();
void Func_020010fc();
void Func_02001176();
void Func_020011aa();
void Func_02001118();
void Func_0200111e();
void Func_0200118e();
void Func_0200112c();
void Func_020011c4();
void Func_0200113a();
void Func_020011b4();
void Func_020011e8();
void Func_02001156();
void Func_020011ae();
void Func_020011be();
void Func_0200116c();
void Func_02001172();
void Func_020011d2();
void Func_02001180();
void Func_0200122e();
void Func_020011a4();
void Func_02001250();
void Func_0200125c();
void Func_020011c2();
void Func_0200123c();
void Func_02001270();
void Func_020011de();
void Func_020011e4();
void Func_02001254();
void Func_020011f2();
void Func_0200128a();
void Func_02001200();
void Func_020012ac();
void Func_02001286();
void Func_0200121c();
void Func_0200128c();
void Func_0200122a();
void Func_02001230();
void Func_020012d4();
void Func_02001242();
void Func_020012da();
void Func_020012c0();
void Func_0200125e();
void Func_020012be();
void Func_0200126c();
void Func_02001272();
void Func_0200131e();
void Func_02001316();
void Func_020012ec();
void Func_0200129a();
void Func_02001332();
void Func_020012a8();
void Func_02001352();
void Func_020012b8();
void Func_02001366();
void Func_0200134a();
void Func_020012e8();
void Func_02001380();
void Func_020012f6();
void Func_020013a2();
void Func_0200139a();
void Func_02001310();
void Func_020013b6();
void Func_02001396();
void Func_02001334();
void Func_0200133a();
void Func_020013bc();
void Func_020013e2();
void Func_02001358();
void Func_020013b8();
void Func_020013fe();
void Func_02001374();
void Func_02001420();
void Func_0200142c();
void Func_02001438();
void Func_02001430();
void Func_020013a6();
void Func_02001416();
void Func_020013b4();
void Func_0200144c();
void Func_020013ca();
void Func_020013d0();
void Func_02001474();
void Func_02001452();
void Func_020013f0();
void Func_02001488();
void Func_0200146e();
void Func_0200140c();
void Func_020014a4();
void Func_0200141a();
void Func_02001494();
void Func_020014c8();
void Func_02001436();
void Func_02001496();
void Func_02001444();
void Func_020014dc();
void Func_020014ba();
void Func_020014ca();
void Func_02001468();
void Func_020014f0();
void Func_02001554();
void Func_0200148a();
void Func_0200151a();
s32 Func_020014b2();
void Func_0200153c();
void Func_020014c4();
void Func_0200156c();
void Func_020014e2();
void Func_02001552();
void Func_02001588();
void Func_020014fe();
void Func_020015aa();
void Func_020015a2();
void Func_02001518();
void Func_02001578();
void Func_02001526();
void Func_02001586();
void Func_02001534();
void Func_0200162a();
void Func_0200159c();
u8 *Func_02001572();
void Func_02001594();
void Func_020015bc();
void Func_0200157a();
void Func_0200163e();
void Func_02001592();

                     

                     

                     

                     

                     

                     

                     

                     

                     

/* Pointer CELL, not the workspace itself: the original does
 * `ldr r3,[pc] / ldr r2,[r3]`. */
extern u8 *Data_03001ebc;

void Func_02000240(void)
{
    u16 *counter;
    u8 *record;

    Func_02000d84(0x9b0);

    if (Func_02000d82(0x98a) != 0) {
        return;
    }

    Func_02000e8e(30);
    Func_02000daa();
    Func_02000e5c(0x1700000, -1, 0x680000, 1);
    Func_02000df8(0, 368, 160);
    Func_02000e64(0, 0xc000, 0);
    Func_02000ea4(19, 0, -16, 0xc000);
    Func_02000e22(19);
    Func_02000e96();
    Func_02000e6c(0x25eb);
    Func_02000df2(10);
    Func_02000e62(20, 2);
    Func_02000e00(20);
    Func_02000e98(20, 0);
    Func_02000e0e(10);
    Func_02000e7e(19, 2);
    Func_02000e1c(20);
    Func_02000e4e(19, 0x13333, 0x9999);
    Func_02000f02(19, 0, -16);
    Func_02000ed4(19, 0, 0);
    Func_02000e42(30);
    Func_02000ee6(19, 0xe000, 0);
    Func_02000e54(30);
    Func_02000ef6(19, 0, 0);
    Func_02000e64(30);
    Func_02000f10(19, 256, 40);
    Func_02000f08(19, 0);
    Func_02000e7e(10);
    Func_02000eee(21, 2);
    Func_02000e8c(20);
    Func_02000f24(21, 0);
    Func_02000e9a(10);
    Func_02000ecc(19, 0x13333, 0x9999);
    Func_02000f80(19, 0, -24);
    Func_02000f8a(19, 48, 0);
    Func_02000f5e(19, 0xc000, 0);
    Func_02000ecc(30);
    Func_02000f64(19, 0);
    Func_02000eda(20);
    Func_02000f86(19, 256, 40);
    Func_02000f7e(19, 0);
    Func_02000ef4(20);
    Func_02000fa0(20, 258, 40);
    Func_02000f98(20, 0);
    Func_02000f0e(10);
    Func_02000fb2(19, 0x2000, 0);
    Func_02000f20(20);
    Func_02000fb8(19, 0);
    Func_02000f2e(10);
    Func_02000f9e(21, 2);
    Func_02000f3c(20);
    Func_02000f9c(21, 4);
    Func_02000f4a(20);
    Func_02000fe2(21, 0);
    Func_02000f58(10);
    Func_02000ffc(19, 0xc000, 0);
    Func_02000f6a(30);
    Func_02001014(19, 263, 40);
    Func_0200100c(19, 0);
    Func_02000f82(10);
    Func_02000fea(20, 2);
    Func_02000ffa(21, 2);
    Func_02000f98(20);
    Func_02000fca(0, 0x13333, 0x9999);
    Func_02000fe6(0, 368, 104);
    Func_02001088(0, 16, 0);
    Func_0200105a(0, 0, 0);
    Func_02000fc8(20);
    Func_02000fce(10);
    Func_0200102e(19, 4);
    Func_02000fdc(20);
    Func_02001074(19, 0);
    Func_02000fea(20);
    Func_02001096(19, 258, 50);
    Func_0200108e(19, 0);
    Func_02001004(10);
    Func_02001074(20, 2);
    Func_02001012(20);
    Func_020010aa(20, 0);
    Func_02001020(10);
    Func_0200109a(0, 20, 0);
    Func_020010ce(19, 0x2000, 0);
    Func_0200103c(20);
    Func_020010d4(19, 0);
    Func_0200104a(10);
    Func_020010ba(21, 2);
    Func_02001058(20);
    Func_020010f0(21, 0);
    Func_02001066(10);
    Func_020010e0(0, 21, 0);
    Func_02001114(19, 0xc000, 0);
    Func_02001082(40);
    Func_020010e2(19, 3);
    Func_02001090(30);
    Func_02001128(19, 0);
    Func_0200109e(10);
    Func_0200110e(21, 2);
    Func_020010ac(20);
    Func_0200110c(21, 4);
    Func_020010ba(20);
    Func_02001152(21, 0);
    Func_020010c8(10);
    Func_0200114a(19, 0, 30);
    Func_0200116a(19, 0);
    Func_020010e0(10);
    Func_02001150(20, 2);
    Func_020010ee(20);
    Func_02001186(20, 0);
    Func_020010fc(10);
    Func_02001176(0, 20, 0);
    Func_020011aa(19, 0x2000, 0);
    Func_02001118(40);
    Func_0200111e(10);
    Func_0200118e(21, 2);
    Func_0200112c(20);
    Func_020011c4(21, 0);
    Func_0200113a(10);
    Func_020011b4(0, 21, 0);
    Func_020011e8(19, 0xc000, 0);
    Func_02001156(40);
    Func_020011ae(0, 3);
    Func_020011be(19, 3);
    Func_0200116c(30);
    Func_02001172(10);
    Func_020011d2(20, 4);
    Func_02001180(20);
    Func_0200122e(20, 0);
    Func_020011a4(10);
    Func_02001250(0, 256, 0);
    Func_0200125c(19, 256, 40);
    Func_020011c2(10);
    Func_0200123c(0, 20, 0);
    Func_02001270(19, 0x2000, 0);
    Func_020011de(40);
    Func_020011e4(10);
    Func_02001254(21, 2);
    Func_020011f2(20);
    Func_0200128a(21, 0);
    Func_02001200(10);
    Func_020012ac(0, 256, 50);
    Func_02001286(0, 21, 0);
    Func_0200121c(30);
    Func_0200128c(21, 2);
    Func_0200122a(30);
    Func_02001230(10);
    Func_020012d4(19, 0xc000, 0);
    Func_02001242(30);
    Func_020012da(19, 0);
    Func_02001250(10);
    Func_020012c0(21, 2);
    Func_0200125e(20);
    Func_020012be(21, 3);
    Func_0200126c(30);
    Func_02001272(10);
    Func_0200131e(19, 258, 40);
    Func_02001316(19, 0);
    Func_0200128c(10);
    Func_020012ec(20, 4);
    Func_0200129a(20);
    Func_02001332(20, 0);
    Func_020012a8(10);
    Func_02001352(19, 257, 50);
    Func_020012b8(10);
    Func_02001332(0, 20, 0);
    Func_02001366(19, 0x2000, 0);
    Func_020012d4(20);
    Func_020012da(10);
    Func_0200134a(20, 2);
    Func_020012e8(20);
    Func_02001380(20, 0);
    Func_020012f6(10);
    Func_020013a2(19, 256, 40);
    Func_0200139a(19, 0);
    Func_02001310(10);
    Func_02001380(21, 2);
    Func_0200131e(20);
    Func_020013b6(21, 0);
    Func_02001396(21, 2);
    Func_02001334(20);
    Func_0200133a(10);
    Func_020013bc(19, 0, 0);
    Func_0200134a(30);
    Func_020013e2(19, 0);
    Func_02001358(10);
    Func_020013b8(20, 4);
    Func_02001366(20);
    Func_020013fe(20, 0);
    Func_02001374(10);
    Func_02001420(0, 258, 0);
    Func_0200142c(19, 258, 80);
    Func_02001438(21, 258, 50);
    Func_02001430(21, 0);
    Func_020013a6(10);
    Func_02001416(21, 2);
    Func_020013b4(20);
    Func_0200144c(21, 0);
    Func_0200142c(21, 3);
    Func_020013ca(20);
    Func_020013d0(10);
    Func_02001474(19, 0xc000, 0);
    Func_020013e2(30);
    Func_02001452(19, 2);
    Func_020013f0(10);
    Func_02001488(19, 0);
    Func_020013fe(10);
    Func_0200146e(20, 2);
    Func_0200140c(20);
    Func_020014a4(20, 0);
    Func_0200141a(10);
    Func_02001494(0, 20, 0);
    Func_020014c8(19, 0x2000, 0);
    Func_02001436(30);
    Func_02001496(19, 3);
    Func_02001444(30);
    Func_020014dc(19, 0);
    Func_02001452(10);
    Func_020014ba(20, 2);
    Func_020014ca(21, 2);
    Func_02001468(30);
    Func_0200146e(10);
    Func_020014f0(19, 0, 20);
    Func_02001554(19, -12, 0);
    Func_0200148a(20);
    Func_0200151a(19, 0);

    if (Func_020014b2(0, 0) == 0) {
        Func_020014a4(20);
        Func_0200153c(19, 0);
        counter = (u16 *)(Data_03001ebc + 472);
        *counter = (u16)(*counter + 1);
    } else {
        Func_020014c4(10);
        counter = (u16 *)(Data_03001ebc + 472);
        *counter = (u16)(*counter + 1);
        Func_0200156c(19, 0);
    }

    Func_020014e2(10);
    Func_02001552(19, 2);
    Func_020014f0(20);
    Func_02001588(19, 0);
    Func_020014fe(10);
    Func_020015aa(19, 258, 50);
    Func_020015a2(19, 0);
    Func_02001518(10);
    Func_02001578(0, 3);
    Func_02001526(20);
    Func_02001586(19, 3);
    Func_02001534(30);
    Func_0200162a(30);
    Func_0200156c(19, 0x13333, 0x9999);
    Func_0200159c(19, 2);

    record = Func_02001572(0);
    if (record != 0) {
        Func_02001594(19, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }

    Func_020015aa(19);
    Func_020015bc(19, 0, 0);
    Func_0200157a(10);
    Func_0200163e();
    Func_02001592();
}
